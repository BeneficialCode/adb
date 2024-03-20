#include "pch.h"
#include "adb_winusb_io_completion.h"

AdbWinUsbIOCompletion::AdbWinUsbIOCompletion(
    AdbWinUsbEndpointObject* parent_io_obj,
    ULONG expected_trans_size,
    HANDLE event_hndl)
    : AdbIOCompletion(parent_io_obj, expected_trans_size, event_hndl) {
}

AdbWinUsbIOCompletion::~AdbWinUsbIOCompletion() {
}

LONG AdbWinUsbIOCompletion::Release() {
    ATLASSERT(ref_count_ > 0);
    LONG ret = InterlockedDecrement(&ref_count_);
    ATLASSERT(ret >= 0);
    if (0 == ret) {
        LastReferenceReleased();
        delete this;
    }
    return ret;
}

bool AdbWinUsbIOCompletion::GetOvelappedIoResult(LPOVERLAPPED ovl_data,
    ULONG* bytes_transferred,
    bool wait) {
    if (nullptr != bytes_transferred) {
        *bytes_transferred = 0;
    }

    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    ULONG transfer;
    bool ret = WinUsb_GetOverlappedResult(
        parent_winusb_io_object()->winusb_handle(),
        overlapped(),
        &transfer,
        wait ? TRUE : FALSE) ? true : false;

    // TODO: This is bizzare but I've seen it happening
    // that GetOverlappedResult with wait set to true returns "prematurely",
    // with wrong transferred bytes value and GetLastError reporting
    // ERROR_IO_PENDING. So, lets give it an up to a 20 ms loop!
    ULONG error = GetLastError();

    if (wait && ret && (0 == transfer) && (0 != expected_transfer_size_) &&
        ((ERROR_IO_INCOMPLETE == error) || (ERROR_IO_PENDING == error))) {
        for (int trying = 0; trying < 10; trying++) {
            Sleep(2);
            ret = WinUsb_GetOverlappedResult(
                parent_winusb_io_object()->winusb_handle(),
                overlapped(),
                &transfer,
                wait ? TRUE : FALSE) ? true : false;
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
        *bytes_transferred = transfer;
    }

    return ret;
}