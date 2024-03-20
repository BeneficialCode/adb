#include "pch.h"
#include "adb_legacy_io_completion.h"

AdbLegacyIOCompletion::AdbLegacyIOCompletion(
    AdbLegacyEndpointObject* parent_io_obj,
    ULONG expected_trans_size,
    HANDLE event_hndl,
    bool is_write_ctl)
    : AdbIOCompletion(parent_io_obj, expected_trans_size, event_hndl),
    transferred_bytes_(0),
    is_write_ioctl_(is_write_ctl) {
}

AdbLegacyIOCompletion::~AdbLegacyIOCompletion() {
}

bool AdbLegacyIOCompletion::GetOvelappedIoResult(LPOVERLAPPED ovl_data,
    ULONG* bytes_transferred,
    bool wait) {
    if (NULL != bytes_transferred) {
        *bytes_transferred = 0;
    }

    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    ULONG transfer;
    bool ret = GetOverlappedResult(parent_legacy_io_object()->usb_handle(),
        overlapped(),
        &transfer,
        wait) ? true :
        false;

    // TODO: This is bizzare but I've seen it happening
    // that GetOverlappedResult with wait set to true returns "prematurely",
    // with wrong transferred bytes value and GetLastError reporting
    // ERROR_IO_PENDING. So, lets give it an up to a 20 ms loop!
    ULONG error = GetLastError();

    if (wait && ret && (0 == transfer) && (0 != expected_transfer_size_) &&
        ((ERROR_IO_INCOMPLETE == error) || (ERROR_IO_PENDING == error))) {
        for (int trying = 0; trying < 10; trying++) {
            Sleep(2);
            ret = GetOverlappedResult(parent_legacy_io_object()->usb_handle(),
                overlapped(),
                &transfer,
                wait) ? true :
                false;
            error = GetLastError();
            if (!ret || (0 != transfer) ||
                ((ERROR_IO_INCOMPLETE != error) && (ERROR_IO_PENDING != error))) {
                break;
            }
        }
    }

    if (nullptr != ovl_data) {
        CopyMemory(ovl_data, overlapped(), sizeof(OVERLAPPED));
    }

    if (nullptr != bytes_transferred) {
        *bytes_transferred = is_write_ioctl() ? transferred_bytes_ : transfer;
    }

    return ret;
}
