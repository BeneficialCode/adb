#include "pch.h"
#include "adb_io_completion.h"

AdbIOCompletion::AdbIOCompletion(AdbEndpointObject* parent_io_obj,
    ULONG expected_trans_size,
    HANDLE event_hndl)
    : AdbObjectHandle(AdbObjectTypeIoCompletion),
    expected_transfer_size_(expected_trans_size),
    parent_io_object_(parent_io_obj) {
    parent_io_obj->AddRef();
    ZeroMemory(&overlapped_, sizeof(overlapped_));
    overlapped_.hEvent = event_hndl;
}

AdbIOCompletion::~AdbIOCompletion() {
    parent_io_object_->Release();
}

bool AdbIOCompletion::IsCompleted() {
    SetLastError(NO_ERROR);
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return true;
    }

    return HasOverlappedIoCompleted(overlapped()) ? true : false;
}