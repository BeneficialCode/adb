#include "pch.h"
#include "adb_api.h"
#include "adb_interface_enum.h"
#include "adb_helper_routines.h"

AdbInterfaceEnumObject::AdbInterfaceEnumObject()
    : AdbObjectHandle(AdbObjectTypeInterfaceEnumerator) {
    current_interface_ = interfaces_.begin();
}

AdbInterfaceEnumObject::~AdbInterfaceEnumObject() {
}

bool AdbInterfaceEnumObject::InitializeEnum(GUID class_id,
    bool exclude_not_present,
    bool exclude_removed,
    bool active_only) {
    // Calc flags for SetupDiGetClassDevs
    DWORD flags = DIGCF_DEVICEINTERFACE;
    if (exclude_not_present)
        flags |= DIGCF_PRESENT;

    // Do the enum
    bool ret = EnumerateDeviceInterfaces(class_id,
        flags,
        exclude_removed,
        active_only,
        &interfaces_);

    // If enum was successfull set current enum pointer
    // to the beginning of the array
    if (ret)
        current_interface_ = interfaces_.begin();

    return ret;
}

bool AdbInterfaceEnumObject::Next(AdbInterfaceInfo** pInfo, ULONG* size) {
    // Make sure that it's opened
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    // Lets see if enum is over
    if (interfaces_.end() == current_interface_) {
        SetLastError(ERROR_NO_MORE_ITEMS);
        return false;
    }

    AdbInstanceEnumEntry& entry = *current_interface_;

    *size = entry.GetFlatSize();
   
    // All checks passed
    entry.Save(pInfo, size);
    current_interface_++;
    return true;
}

bool AdbInterfaceEnumObject::Reset() {
    // Make sure that it's opened
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    current_interface_ = interfaces_.begin();

    return true;
}