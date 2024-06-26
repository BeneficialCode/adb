#pragma once

#include <sys/types.h>

#include "adb.h"
#include "transport.h"

// USB host/client interface.

#define ADB_USB_INTERFACE(handle_ref_type)                       \
    void usb_init();                                             \
    void usb_cleanup();                                          \
    int usb_write(handle_ref_type h, const void* data, int len); \
    int usb_read(handle_ref_type h, void* data, int len);        \
    int usb_close(handle_ref_type h);                            \
    void usb_reset(handle_ref_type h);                           \
    void usb_kick(handle_ref_type h);                            \
    size_t usb_get_max_packet_size(handle_ref_type)

struct usb_handle;
ADB_USB_INTERFACE(usb_handle*);

// USB device detection.
int is_adb_interface(int usb_class, int usb_subclass, int usb_protocol);

bool should_use_libusb();

namespace libusb {
    void usb_init();
}

struct UsbConnection : public BlockingConnection {
    explicit UsbConnection(usb_handle* handle) : handle_(handle) {}
    ~UsbConnection();

    bool Read(apacket* packet) override final;
    bool Write(apacket* packet) override final;
    bool DoTlsHandshake(RSA* key, std::string* auth_key) override final;

    void Close() override final;
    virtual void Reset() override final;

    usb_handle* handle_;
};