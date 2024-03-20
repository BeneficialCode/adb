#pragma once
#include "adb_endpoint_object.h"
#include "adb_legacy_interface.h"

/** Encapsulates a handle opened to an endpoint on our device controlled by
  a custom (legacy) USB driver.
*/
class AdbLegacyEndpointObject : public AdbEndpointObject {
public:
    /** \brief Constructs the object

      @param[in] interface Parent legacy USB interface for this object.
      @param[in] endpoint_id Endpoint ID (endpoint address) on the device.
      @param[in] endpoint_index Zero-based endpoint index in the interface's
            array of endpoints.
    */
    AdbLegacyEndpointObject(AdbLegacyInterfaceObject* parent_interf,
        UCHAR endpoint_id,
        UCHAR endpoint_index);

protected:
    /** \brief Destructs the object.

      We hide destructor in order to prevent ourseves from accidentaly allocating
      instances on the stack. If such attemp occur, compiler will error.
    */
    virtual ~AdbLegacyEndpointObject();

    //
    // Abstract overrides
    //

protected:
    /** \brief Common code for async read / write

      @param[in] is_read Read or write selector.
      @param[in,out] buffer Pointer to the buffer for read / write.
      @param[in] bytes_to_transfer Number of bytes to be read / written.
      @param[out] bytes_transferred Number of bytes read / written. Can be NULL.
      @param[in] event_handle Event handle that should be signaled when async I/O
             completes. Can be NULL. If it's not NULL this handle will be used to
             initialize OVERLAPPED structure for this I/O.
      @param[in] time_out A timeout (in milliseconds) required for this I/O to
             complete. Zero value in this parameter means that there is no
             timeout set for this I/O.
      @return A handle to IO completion object or NULL on failure. If NULL is
              returned GetLastError() provides extended error information.
    */
    virtual ADBAPIHANDLE CommonAsyncReadWrite(bool is_read,
        void* buffer,
        ULONG bytes_to_transfer,
        ULONG* bytes_transferred,
        HANDLE event_handle,
        ULONG time_out);

    /** \brief Common code for sync read / write

      @param[in] is_read Read or write selector.
      @param[in,out] buffer Pointer to the buffer for read / write.
      @param[in] bytes_to_transfer Number of bytes to be read / written.
      @param[out] bytes_transferred Number of bytes read / written. Can be NULL.
      @param[in] time_out A timeout (in milliseconds) required for this I/O to
             complete. Zero value in this parameter means that there is no
             timeout set for this I/O.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool CommonSyncReadWrite(bool is_read,
        void* buffer,
        ULONG bytes_to_transfer,
        ULONG* bytes_transferred,
        ULONG time_out);

    //
    // Operations
    //

public:
    /** \brief Opens endpoint and creates a handle to this object

      @param item_path[in] Path to the endpoint on our USB device.
      @param access_type[in] Desired access type. In the current implementation
            this parameter has no effect on the way item is opened. It's
            always read / write access.
      @param sharing_mode[in] Desired share mode. In the current implementation
            this parameter has no effect on the way item is opened. It's
            always shared for read / write.
      @return A handle to this object on success or NULL on an error.
              If NULL is returned GetLastError() provides extended error
              information. ERROR_GEN_FAILURE is set if an attempt was
              made to create already opened object.
    */
    virtual ADBAPIHANDLE CreateHandle(const wchar_t* item_path,
        AdbOpenAccessType access_type,
        AdbOpenSharingMode share_mode);


    /** \brief This method is called when handle to this object gets closed.

      We override this method in order to close handle to the endpoint opened
      in CreateHandle method of this class.
      @return true on success or false if object is already closed. If
              false is returned GetLastError() provides extended error
              information.
    */
    virtual bool CloseHandle();

public:
    /// Gets handle to the endpoint opened on our USB device.
    HANDLE usb_handle() const {
        return usb_handle_;
    }

protected:
    /// Handle to the endpoint opened on our USB device.
    HANDLE              usb_handle_;
};