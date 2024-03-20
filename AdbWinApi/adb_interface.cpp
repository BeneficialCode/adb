#include "pch.h"
#include "adb_interface.h"

AdbInterfaceObject::AdbInterfaceObject(const wchar_t* interf_name)
    : AdbObjectHandle(AdbObjectTypeInterface),
    interface_name_(interf_name) {
}

AdbInterfaceObject::~AdbInterfaceObject() {
}

bool AdbInterfaceObject::GetInterfaceName(void* buffer,
    unsigned long* buffer_char_size,
    bool ansi) {
    if (nullptr == buffer_char_size) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    // Lets see if buffer is big enough
    ULONG name_len = static_cast<ULONG>(interface_name_.length() + 1);
    if ((nullptr == buffer) || (*buffer_char_size < name_len)) {
        *buffer_char_size = name_len;
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return false;
    }

    if (!ansi) {
        // If user asked for wide char name just return it
        wcscpy_s(reinterpret_cast<wchar_t*>(buffer), *buffer_char_size, interface_name_.c_str());
        return true;
    }

    // We need to convert name from wide char to ansi string
    int res = WideCharToMultiByte(CP_ACP,
        0,
        interface_name().c_str(),
        static_cast<int>(name_len),
        reinterpret_cast<PSTR>(buffer),
        static_cast<int>(*buffer_char_size),
        nullptr,
        nullptr);
    return (res != 0);
}

bool AdbInterfaceObject::GetUsbDeviceDescriptor(USB_DEVICE_DESCRIPTOR* desc) {
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    if (nullptr == desc) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    CopyMemory(desc, usb_device_descriptor(), sizeof(USB_DEVICE_DESCRIPTOR));

    return true;
}

bool AdbInterfaceObject::GetUsbConfigurationDescriptor(
    USB_CONFIGURATION_DESCRIPTOR* desc) {
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    if (NULL == desc) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    CopyMemory(desc, usb_config_descriptor(),
        sizeof(USB_CONFIGURATION_DESCRIPTOR));

    return true;
}

bool AdbInterfaceObject::GetUsbInterfaceDescriptor(
    USB_INTERFACE_DESCRIPTOR* desc) {
    if (!IsOpened()) {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    if (NULL == desc) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    CopyMemory(desc, usb_interface_descriptor(), sizeof(USB_INTERFACE_DESCRIPTOR));

    return true;
}