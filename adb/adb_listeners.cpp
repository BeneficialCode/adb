#include "adb_listeners.h"

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <list>
#include <memory>

#include "stringprintf.h"
#include "strings.h"
#include "thread_annotations.h"
#include "sockets.h"

#include "socket_spec.h"
#include "sysdeps.h"
#include "transport.h"

// A listener is an entity which binds to a local port and, upon receiving a connection on that
// port, creates an asocket to connect the new local connection to a specific remote service.
//
// TODO: some listeners read from the new connection to determine what exact service to connect to
// on the far side.
class alistener {
public:
    alistener(const std::string& _local_name, const std::string& _connect_to);
    ~alistener();

    fdevent* fde = nullptr;
    int fd = -1;

    std::string local_name;
    std::string connect_to;
    atransport* transport = nullptr;
    adisconnect disconnect;

private:
    DISALLOW_COPY_AND_ASSIGN(alistener);
};

alistener::alistener(const std::string& _local_name, const std::string& _connect_to)
    : local_name(_local_name), connect_to(_connect_to) {
}

alistener::~alistener() {
    // Closes the corresponding fd.
    fdevent_destroy(fde);

    if (transport) {
        transport->RemoveDisconnect(&disconnect);
    }
}

// listener_list retains ownership of all created alistener objects. Removing an alistener from
// this list will cause it to be deleted.
static auto& listener_list_mutex = *new std::mutex();
typedef std::list<std::unique_ptr<alistener>> ListenerList;
static _Guarded_by_(listener_list_mutex) ListenerList& listener_list = *new ListenerList();

#if ADB_HOST
static void ss_listener_event_func(int _fd, unsigned ev, void* _l) {
    if (ev & FDE_READ) {
        unique_fd fd(adb_socket_accept(_fd, nullptr, nullptr));
        if (fd < 0) return;

        int rcv_buf_size = CHUNK_SIZE;
        adb_setsockopt(fd.get(), SOL_SOCKET, SO_RCVBUF, &rcv_buf_size, sizeof(rcv_buf_size));

        asocket* s = create_local_socket(std::move(fd));
        if (s) {
            connect_to_smartsocket(s);
            return;
        }
    }
}
#endif

static void listener_event_func(int _fd, unsigned ev, void* _l)
{
    alistener* listener = reinterpret_cast<alistener*>(_l);

    if (ev & FDE_READ) {
        unique_fd fd(adb_socket_accept(_fd, nullptr, nullptr));
        if (fd < 0) {
            return;
        }

        asocket* s = create_local_socket(std::move(fd));
        if (s) {
            s->transport = listener->transport;
            connect_to_remote(s, listener->connect_to);
            return;
        }
    }
}

// Called as a transport disconnect function. |arg| is the raw alistener*.
static void listener_disconnect(void* arg, atransport*) {
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    for (auto iter = listener_list.begin(); iter != listener_list.end(); ++iter) {
        if (iter->get() == arg) {
            (*iter)->transport = nullptr;
            listener_list.erase(iter);
            return;
        }
    }
}

// Write the list of current listeners (network redirections) into a string.
std::string format_listeners(){
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    std::string result;
    for (auto& l : listener_list) {
        // Ignore special listeners like those for *smartsocket*
        if (l->connect_to[0] == '*') {
            continue;
        }
        //  <device-serial> " " <local-name> " " <remote-name> "\n"
        // Entries from "adb reverse" have no serial.
        android::base::StringAppendF(
            &result, "%s %s %s\n",
            !l->transport->serial.empty() ? l->transport->serial.c_str() : "(reverse)",
            l->local_name.c_str(), l->connect_to.c_str());
    }
    return result;
}

InstallStatus remove_listener(const char* local_name, atransport* transport) {
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    for (auto iter = listener_list.begin(); iter != listener_list.end(); ++iter) {
        if (local_name == (*iter)->local_name) {
            listener_list.erase(iter);
            return INSTALL_STATUS_OK;
        }
    }
    return INSTALL_STATUS_LISTENER_NOT_FOUND;
}

void remove_all_listeners() {
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    auto iter = listener_list.begin();
    while (iter != listener_list.end()) {
        // Never remove smart sockets.
        if ((*iter)->connect_to[0] == '*') {
            ++iter;
        }
        else {
            iter = listener_list.erase(iter);
        }
    }
}

void enable_server_sockets() {
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    for (auto& l : listener_list) {
        if (l->connect_to == "*smartsocket*") {
            fdevent_set(l->fde, FDE_READ);
        }
    }
}

#if ADB_HOST
void close_smartsockets() {
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    auto pred = [](const std::unique_ptr<alistener>& listener) {
        return listener->local_name == "*smartsocket*";
        };
    listener_list.remove_if(pred);
}
#endif

InstallStatus install_listener(const std::string& local_name, const char* connect_to,
    atransport* transport, int flags, int* resolved_tcp_port,
    std::string* error){
    std::lock_guard<std::mutex> lock(listener_list_mutex);
    for (auto& l : listener_list) {
        if (local_name == l->local_name) {
            // Can't repurpose a smartsocket.
            if (l->connect_to[0] == '*') {
                *error = "cannot repurpose smartsocket";
                return INSTALL_STATUS_INTERNAL_ERROR;
            }

            // Can't repurpose a listener if INSTALL_LISTENER_NO_REBIND is set
            if (flags & INSTALL_LISTENER_NO_REBIND) {
                *error = "cannot rebind";
                return INSTALL_STATUS_CANNOT_REBIND;
            }

            l->connect_to = connect_to;
            if (l->transport != transport) {
                l->transport->RemoveDisconnect(&l->disconnect);
                l->transport = transport;
                l->transport->AddDisconnect(&l->disconnect);
            }
            return INSTALL_STATUS_OK;
        }
    }

    auto listener = std::make_unique<alistener>(local_name, connect_to);

    int resolved = 0;
    listener->fd = socket_spec_listen(listener->local_name, error, &resolved);
    if (listener->fd < 0) {
        return INSTALL_STATUS_CANNOT_BIND;
    }

    // If the caller requested port 0, update the listener name with the resolved port.
    if (resolved != 0) {
        listener->local_name = android::base::StringPrintf("tcp:%d", resolved);
        if (resolved_tcp_port) {
            *resolved_tcp_port = resolved;
        }
    }

    close_on_exec(listener->fd);
    if (listener->connect_to == "*smartsocket*") {
#if ADB_HOST
        listener->fde = fdevent_create(listener->fd, ss_listener_event_func, listener.get());
#else
        LOG(FATAL) << "attempted to connect to *smartsocket* in daemon";
#endif
    }
    else {
        listener->fde = fdevent_create(listener->fd, listener_event_func, listener.get());
    }
    if ((flags & INSTALL_LISTENER_DISABLED) == 0) {
        fdevent_set(listener->fde, FDE_READ);
    }

    listener->transport = transport;

    if (transport) {
        listener->disconnect.opaque = listener.get();
        listener->disconnect.func = listener_disconnect;
        transport->AddDisconnect(&listener->disconnect);
    }

    listener_list.push_back(std::move(listener));
    return INSTALL_STATUS_OK;
}