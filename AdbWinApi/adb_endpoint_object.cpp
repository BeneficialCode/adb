#include "pch.h"
#include "adb_endpoint_object.h"

AdbEndpointObject::AdbEndpointObject(AdbInterfaceObject* parent_interf,
    UCHAR endpoint_id,
    UCHAR endpoint_index)
    : AdbObjectHandle(AdbObjectTypeEndpoint),
    parent_interface_(parent_interf),
    endpoint_id_(endpoint_id),
    endpoint_index_(endpoint_index) {
    if (nullptr != parent_interface_)
        parent_interface_->AddRef();
}

AdbEndpointObject::~AdbEndpointObject() {
    if (nullptr != parent_interface_)
        parent_interface_->Release();
}

bool AdbEndpointObject::GetEndpointInformation(AdbEndpointInformation* info) {
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    return parent_interface()->GetEndpointInformation(endpoint_index(), info);
}

ADBAPIHANDLE AdbEndpointObject::AsyncRead(void* buffer,
    ULONG bytes_to_read,
    ULONG* bytes_read,
    HANDLE event_handle,
    ULONG time_out) {
    return CommonAsyncReadWrite(true,
        buffer,
        bytes_to_read,
        bytes_read,
        event_handle,
        time_out);
}

ADBAPIHANDLE AdbEndpointObject::AsyncWrite(void* buffer,
    ULONG bytes_to_write,
    ULONG* bytes_written,
    HANDLE event_handle,
    ULONG time_out) {
    return CommonAsyncReadWrite(false,
        buffer,
        bytes_to_write,
        bytes_written,
        event_handle,
        time_out);
}

bool AdbEndpointObject::SyncRead(void* buffer,
    ULONG bytes_to_read,
    ULONG* bytes_read,
    ULONG time_out) {
    return CommonSyncReadWrite(true,
        buffer,
        bytes_to_read,
        bytes_read,
        time_out);
}

bool AdbEndpointObject::SyncWrite(void* buffer,
    ULONG bytes_to_write,
    ULONG* bytes_written,
    ULONG time_out) {
    return CommonSyncReadWrite(false,
        buffer,
        bytes_to_write,
        bytes_written,
        time_out);
}