#pragma once

#define TRACE_TAG ADB

#include "sysdeps.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <thread>

#include "errors.h"
#include "file.h"
#include "logging.h"
#include "stringprintf.h"

#include "adb.h"
#include "adb_auth.h"
#include "adb_client.h"
#include "adb_listeners.h"
#include "adb_mdns.h"
#include "adb_utils.h"
#include "adb_wifi.h"
#include "usb.h"
#include "commandline.h"
#include "chrono.h"
#include "transport.h"

const char** __adb_argv;
const char** __adb_envp;

static void setup_daemon_logging() {
    const std::string log_file_path(GetLogFilePath());
    int fd = unix_open(log_file_path, O_WRONLY | O_CREAT | O_APPEND, 0640);
    if (fd == -1) {
        PLOG(FATAL) << "cannot open " << log_file_path;
    }
    if (_dup2(fd, STDOUT_FILENO) == -1) {
        PLOG(FATAL) << "cannot redirect stdout";
    }
    if (_dup2(fd, STDERR_FILENO) == -1) {
        PLOG(FATAL) << "cannot redirect stderr";
    }
    unix_close(fd);

    fprintf(stderr, "--- adb starting (pid %d) ---\n", _getpid());
    LOG(INFO) << adb_version();
}

void adb_server_cleanup() {
    // Upon exit, we want to clean up in the following order:
    //   1. close_smartsockets, so that we don't get any new clients
    //   2. kick_all_transports, to avoid writing only part of a packet to a transport.
    //   3. usb_cleanup, to tear down the USB stack.
    //   4. mdns_cleanup, to tear down mdns stack.
    close_smartsockets();
    kick_all_transports();
    usb_cleanup();
    mdns_cleanup();

#ifdef _WIN32
    // ::WSACleanup();
#endif
}

static void intentionally_leak() {
    void* p = ::operator new(1);
    // The analyzer is upset about this leaking. NOLINTNEXTLINE
    LOG(INFO) << "leaking pointer " << p;
}

// one_device: if null, server owns all devices, else server owns only
//     device where atransport::MatchesTarget(one_device) is true.
int adb_server_main(int is_daemon, const std::string& socket_spec, const char* one_device,
    int ack_reply_fd) {
#if defined(_WIN32)
    // adb start-server starts us up with stdout and stderr hooked up to
    // anonymous pipes. When the C Runtime sees this, it makes stderr and
    // stdout buffered, but to improve the chance that error output is seen,
    // unbuffer stdout and stderr just like if we were run at the console.
    // This also keeps stderr unbuffered when it is redirected to adb.log.
    if (is_daemon) {
        if (setvbuf(stdout, nullptr, _IONBF, 0) == -1) {
            PLOG(FATAL) << "cannot make stdout unbuffered";
        }
        if (setvbuf(stderr, nullptr, _IONBF, 0) == -1) {
            PLOG(FATAL) << "cannot make stderr unbuffered";
        }
    }

    // TODO: On Ctrl-C, consider trying to kill a starting up adb server (if we're in
    // launch_server) by calling GenerateConsoleCtrlEvent().

    // On Windows, SIGBREAK is when Ctrl-Break is pressed or the console window is closed. It should
    // act like Ctrl-C.
    signal(SIGBREAK, [](int) { raise(SIGINT); });
#endif
    signal(SIGINT, [](int) { fdevent_run_on_looper([]() { exit(0); }); });

    if (one_device) {
        transport_set_one_device(one_device);
    }

    const char* reject_kill_server = getenv("ADB_REJECT_KILL_SERVER");
    if (reject_kill_server && strcmp(reject_kill_server, "1") == 0) {
        adb_set_reject_kill_server(true);
    }

    const char* leak = getenv("ADB_LEAK");
    if (leak && strcmp(leak, "1") == 0) {
        intentionally_leak();
    }

    if (is_daemon) {
        close_stdin();
        setup_daemon_logging();
    }

    atexit(adb_server_cleanup);

    init_transport_registration();
    init_reconnect_handler();

    adb_wifi_init();
    if (!getenv("ADB_MDNS") || strcmp(getenv("ADB_MDNS"), "0") != 0) {
        init_mdns_transport_discovery();
    }

    if (!getenv("ADB_USB") || strcmp(getenv("ADB_USB"), "0") != 0) {
        if (should_use_libusb()) {
            libusb::usb_init();
        }
        else {
            usb_init();
        }
    }
    else {
        adb_notify_device_scan_complete();
    }

    if (!getenv("ADB_EMU") || strcmp(getenv("ADB_EMU"), "0") != 0) {
        local_init(android::base::StringPrintf("tcp:%d", DEFAULT_ADB_LOCAL_TRANSPORT_PORT));
    }

    std::string error;

    auto start = std::chrono::steady_clock::now();

    // If we told a previous adb server to quit because of version mismatch, we can get to this
    // point before it's finished exiting. Retry for a while to give it some time. Don't actually
    // accept any connections until adb_wait_for_device_initialization finishes below.
    while (install_listener(socket_spec, "*smartsocket*", nullptr, INSTALL_LISTENER_DISABLED,
        nullptr, &error) != INSTALL_STATUS_OK) {
        if (std::chrono::steady_clock::now() - start > 0.5s) {
            LOG(FATAL) << "could not install *smartsocket* listener: " << error;
        }

        std::this_thread::sleep_for(100ms);
    }

    adb_auth_init();

    if (is_daemon) {
#if !defined(_WIN32)
        // Start a new session for the daemon. Do this here instead of after the fork so
        // that a ctrl-c between the "starting server" and "done starting server" messages
        // gets a chance to terminate the server.
        // setsid will fail with EPERM if it's already been a lead process of new session.
        // Ignore such error.
        if (setsid() == -1 && errno != EPERM) {
            PLOG(FATAL) << "setsid() failed";
        }
#endif
    }

    // Wait for the USB scan to complete before notifying the parent that we're up.
    // We need to perform this in a thread, because we would otherwise block the event loop.
    std::thread notify_thread([ack_reply_fd]() {
        adb_wait_for_device_initialization();

        if (ack_reply_fd >= 0) {
            // Any error output written to stderr now goes to adb.log. We could
            // keep around a copy of the stderr fd and use that to write any errors
            // encountered by the following code, but that is probably overkill.
#if defined(_WIN32)
            const HANDLE ack_reply_handle = cast_int_to_handle(ack_reply_fd);
            const CHAR ack[] = "OK\n";
            const DWORD bytes_to_write = arraysize(ack) - 1;
            DWORD written = 0;
            if (!WriteFile(ack_reply_handle, ack, bytes_to_write, &written, NULL)) {
                LOG(FATAL) << "cannot write ACK to handle " << ack_reply_handle
                    << android::base::SystemErrorCodeToString(GetLastError());
            }
            if (written != bytes_to_write) {
                LOG(FATAL) << "cannot write " << bytes_to_write << " bytes of ACK: only wrote "
                    << written << " bytes";
            }
            CloseHandle(ack_reply_handle);
#else
            // TODO(danalbert): Can't use SendOkay because we're sending "OK\n", not
            // "OKAY".
            if (!android::base::WriteStringToFd("OK\n", ack_reply_fd)) {
                PLOG(FATAL) << "error writing ACK to fd " << ack_reply_fd;
            }
            unix_close(ack_reply_fd);
#endif
        }
        // We don't accept() client connections until this point: this way, clients
        // can't see wonky state early in startup even if they're connecting directly
        // to the server instead of going through the adb program.
        fdevent_run_on_looper([] { enable_server_sockets(); });
        });
    notify_thread.detach();

#if defined(__linux__)

#endif

    D("Event loop starting");
    fdevent_loop();
    return 0;
}

void init_winsock() {
    WSADATA wsaData;
    int rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc != 0) {
        LOG(FATAL) << "could not initialize Winsock: "
            << android::base::SystemErrorCodeToString(rc);
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable WinSock DLL. */
        LOG(FATAL) << "could not find a usable WinSock DLL";
    }
}

static void _init_console() {
    DWORD old_out_console_mode;

    HANDLE out = _get_console_handle(STDOUT_FILENO, &old_out_console_mode);
    if (out == nullptr) {
        return;
    }

    // Try to use ENABLE_VIRTUAL_TERMINAL_PROCESSING on the output console to process virtual
    // terminal sequences on newer versions of Windows 10 and later.
    // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    // On older OSes that don't support the flag, SetConsoleMode() will return an error.
    // ENABLE_VIRTUAL_TERMINAL_PROCESSING also solves a problem where the last column of the
    // console cannot be overwritten.
    //
    // Note that we don't use DISABLE_NEWLINE_AUTO_RETURN because it doesn't seem to be necessary.
    // If we use DISABLE_NEWLINE_AUTO_RETURN, _console_write_utf8() would need to be modified to
    // translate \n to \r\n.
    if (!SetConsoleMode(out, old_out_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        return;
    }

    // If SetConsoleMode() succeeded, the console supports virtual terminal processing, so we
    // should set the TERM env var to match so that it will be propagated to adbd on devices.
    //
    // Below's direct manipulation of env vars and not g_environ_utf8 assumes that _init_env() has
    // not yet been called. If this fails, _init_env() should be called after _init_console().
    if (g_environ_utf8.size() > 0) {
        LOG(FATAL) << "environment variables have already been converted to UTF-8";
    }

#pragma push_macro("getenv")
#undef getenv
#pragma push_macro("putenv")
#undef putenv
    char* tags;
    size_t len;
    errno_t err = _dupenv_s(&tags, &len, "TERM");
    if (tags == nullptr) {
        // This is the same TERM value used by Gnome Terminal and the version of ssh included with
        // Windows.
        _putenv("TERM=xterm-256color");
    }
#pragma pop_macro("putenv")
#pragma pop_macro("getenv")
}

// Setup shadow UTF-8 environment variables.
static void _init_env() {
    // If some name/value pairs exist, then we've already done the setup below.
    if (g_environ_utf8.size() != 0) {
        return;
    }

    std::vector<std::pair<std::wstring, std::wstring>> envs;

    PEB peb;
    if (!GetProcessPeb(GetCurrentProcess(), &peb))
        return;

    RTL_USER_PROCESS_PARAMETERS* params = peb.ProcessParameters;

    envs.reserve(64);

    BYTE* buffer = nullptr;
    int size = 1 << 16;
    buffer = (BYTE*)malloc(size);
    if (!buffer) {
        return;
    }

    memcpy(buffer, params->Environment, size);

    // Read the environment block from the process.
    for (auto p = (PWSTR)buffer; *p;) {
        std::pair<std::wstring, std::wstring> var;
        auto equal = wcschr(p, L'=');
        if (!equal)
            break;
        *equal = L'\0';
        var.first = p;
        p += ::wcslen(p) + 1;
        var.second = p;
        p += ::wcslen(p) + 1;
        envs.push_back(std::move(var));
    }

    free(buffer);

    // Read name/value pairs from UTF-16 _wenviron and write new name/value
    // pairs to UTF-8 g_environ_utf8. Note that it probably does not make sense
    // to use the D() macro here because that tracing only works if the
    // ADB_TRACE environment variable is setup, but that env var can't be read
    // until this code completes.
    for (auto env : envs) {
        // If we encounter an error converting UTF-16, don't error-out on account of a single env
        // var because the program might never even read this particular variable.
        std::string name_utf8;
        if (!android::base::WideToUTF8(env.first, &name_utf8)) {
            continue;
        }

        // Store lowercase name so that we can do case-insensitive searches.
        std::transform(name_utf8.begin(), name_utf8.end(), name_utf8.begin(), 
            [](unsigned char c) { return std::tolower(c); });

        std::string value_utf8;
        if (!android::base::WideToUTF8(env.second, &value_utf8)) {
            continue;
        }

        char* const value_dup = _strdup(value_utf8.c_str());

        // Don't overwrite a previus env var with the same name. In reality,
        // the system probably won't let two env vars with the same name exist
        // in _wenviron.
        g_environ_utf8.insert({ name_utf8, value_dup });
    }
}

static void _init_winsock() {
    static std::once_flag once;
    std::call_once(once, []() {
        WSADATA wsaData;
        int rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (rc != 0) {
            LOG(FATAL) << "could not initialize Winsock: "
                << android::base::SystemErrorCodeToString(rc);
        }

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            /* Tell the user that we could not find a usable WinSock DLL. */
            LOG(FATAL) << "could not find a usable WinSock DLL";
        }
        // Note that we do not call atexit() to register WSACleanup to be called
        // at normal process termination because:
        // 1) When exit() is called, there are still threads actively using
        //    Winsock because we don't cleanly shutdown all threads, so it
        //    doesn't make sense to call WSACleanup() and may cause problems
        //    with those threads.
        // 2) A deadlock can occur when exit() holds a C Runtime lock, then it
        //    calls WSACleanup() which tries to unload a DLL, which tries to
        //    grab the LoaderLock. This conflicts with the device_poll_thread
        //    which holds the LoaderLock because AdbWinApi.dll calls
        //    setupapi.dll which tries to load wintrust.dll which tries to load
        //    crypt32.dll which calls atexit() which tries to acquire the C
        //    Runtime lock that the other thread holds.
        });
}

static bool _init_sysdeps() {
    _init_console();
    _init_env();
    _init_winsock();
    return true;
}

int main(int argc, char* argv[], char* envp[]) {
    bool _sysdeps_init = _init_sysdeps();
    __adb_argv = const_cast<const char**>(argv);
    __adb_envp = const_cast<const char**>(envp);
    adb_trace_init(argv);
    return adb_commandline(argc - 1, const_cast<const char**>(argv + 1));
}

