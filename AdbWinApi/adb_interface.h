#pragma once
#include "adb_object_handle.h"

/** \brief Encapsulates an interface on our USB device.

  This is an abstract class that implements functionality common for both,
  legacy, and WinUsb based interfaces.
*/
class ADBWIN_API_CLASS AdbInterfaceObject : public AdbObjectHandle {
public:
    /** \brief Constructs the object.

      @param[in] interf_name Name of the interface
    */
    explicit AdbInterfaceObject(const wchar_t* interf_name);

protected:
    /** \brief Destructs the object.

     We hide destructor in order to prevent ourseves from accidentaly allocating
     instances on the stack. If such attemp occur, compiler will error.
    */
    virtual ~AdbInterfaceObject();

    //
    // Abstract
    //

public:
    /** \brief Gets serial number for interface's device.

      @param[out] buffer Buffer for the serail number string. Can be NULL in
             which case buffer_char_size will contain number of characters
             required for the string.
      @param[in,out] buffer_char_size On the way in supplies size (in characters)
             of the buffer. On the way out, if method failed and GetLastError
             reports ERROR_INSUFFICIENT_BUFFER, will contain number of characters
             required for the name.
      @param[in] ansi If true the name will be returned as single character
             string. Otherwise name will be returned as wide character string.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool GetSerialNumber(void* buffer,
        unsigned long* buffer_char_size,
        bool ansi) = 0;


    /** \brief Gets information about an endpoint on this interface.

      @param[in] endpoint_index Zero-based endpoint index. There are two
             shortcuts for this parameter: ADB_QUERY_BULK_WRITE_ENDPOINT_INDEX
             and ADB_QUERY_BULK_READ_ENDPOINT_INDEX that provide infor about
             (default?) bulk write and read endpoints respectively.
      @param[out] info Upon successful completion will have endpoint information.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool GetEndpointInformation(UCHAR endpoint_index,
        AdbEndpointInformation* info) = 0;

    /** \brief Opens an endpoint on this interface.

      @param[in] endpoint_index Zero-based endpoint index. There are two
             shortcuts for this parameter: ADB_QUERY_BULK_WRITE_ENDPOINT_INDEX
             and ADB_QUERY_BULK_READ_ENDPOINT_INDEX that provide infor about
             (default?) bulk write and read endpoints respectively.
      @param[in] access_type Desired access type. In the current implementation
             this parameter has no effect on the way endpoint is opened. It's
             always read / write access.
      @param[in] sharing_mode Desired share mode. In the current implementation
             this parameter has no effect on the way endpoint is opened. It's
             always shared for read / write.
      @return Handle to the opened endpoint object or NULL on failure.
              If NULL is returned GetLastError() provides extended information
              about the error that occurred.
    */
    virtual ADBAPIHANDLE OpenEndpoint(UCHAR endpoint_index,
        AdbOpenAccessType access_type,
        AdbOpenSharingMode sharing_mode) = 0;

    //
    // Operations
    //

public:
    /** \brief Gets interface device name.

      @param[out] buffer Buffer for the name. Can be NULL in which case
             buffer_char_size will contain number of characters required to fit
             the name.
      @param[in,out] buffer_char_size On the way in supplies size (in characters)
             of the buffer. On the way out if method failed and GetLastError
             reports ERROR_INSUFFICIENT_BUFFER will contain number of characters
             required to fit the name.
      @param[in] ansi If true the name will be returned as single character
             string. Otherwise name will be returned as wide character string.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool GetInterfaceName(void* buffer,
        unsigned long* buffer_char_size,
        bool ansi);

    /** \brief Gets device descriptor for the USB device associated with
      this interface.

      @param[out] desc Upon successful completion will have usb device
             descriptor.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool GetUsbDeviceDescriptor(USB_DEVICE_DESCRIPTOR* desc);

    /** \brief Gets descriptor for the selected USB device configuration.

      @param[out] desc Upon successful completion will have usb device
             configuration descriptor.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool GetUsbConfigurationDescriptor(
        USB_CONFIGURATION_DESCRIPTOR* desc);

    /** \brief Gets descriptor for this interface.

      @param[out] desc Upon successful completion will have interface
             descriptor.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool GetUsbInterfaceDescriptor(USB_INTERFACE_DESCRIPTOR* desc);

public:
    /// Gets name of the USB interface (device name) for this object
    const std::wstring& interface_name() const {
        return interface_name_;
    }

    /// This is a helper for extracting object from the AdbObjectHandleMap
    static AdbObjectType Type() {
        return AdbObjectTypeInterface;
    }

    /// Gets cached usb device descriptor
    const USB_DEVICE_DESCRIPTOR* usb_device_descriptor() const {
        return &usb_device_descriptor_;
    }

    /// Gets cached usb configuration descriptor
    const USB_CONFIGURATION_DESCRIPTOR* usb_config_descriptor() const {
        return &usb_config_descriptor_;
    }

    /// Gets cached usb interface descriptor
    const USB_INTERFACE_DESCRIPTOR* usb_interface_descriptor() const {
        return &usb_interface_descriptor_;
    }

protected:
    /// Cached usb device descriptor
    USB_DEVICE_DESCRIPTOR         usb_device_descriptor_;

    /// Cached usb configuration descriptor
    USB_CONFIGURATION_DESCRIPTOR  usb_config_descriptor_;

    /// Cached usb interface descriptor
    USB_INTERFACE_DESCRIPTOR      usb_interface_descriptor_;

private:
    /// Name of the USB interface (device name) for this object
    std::wstring                  interface_name_;
};