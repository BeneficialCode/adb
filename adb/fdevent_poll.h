#pragma once

#include "sysdeps.h"

#include <deque>
#include <list>
#include <mutex>
#include <unordered_map>

#include "thread_annotations.h"

#include "adb_unique_fd.h"
#include "fdevent.h"

struct PollNode {
    fdevent* fde;
    adb_pollfd pollfd;

    explicit PollNode(fdevent* fde) : fde(fde) {
        memset(&pollfd, 0, sizeof(pollfd));
        pollfd.fd = fde->fd.get();

#if defined(__linux__)

#endif
    }
};

struct fdevent_context_poll final : public fdevent_context {
    fdevent_context_poll();
    virtual ~fdevent_context_poll();

    virtual void Register(fdevent* fde) final;
    virtual void Unregister(fdevent* fde) final;

    virtual void Set(fdevent* fde, unsigned events) final;

    virtual void Loop() final;

    virtual size_t InstalledCount() final;

protected:
    virtual void Interrupt() final;

public:
    unique_fd interrupt_fd_;
    fdevent* interrupt_fde_ = nullptr;
};