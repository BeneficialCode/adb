#pragma once
#include "..\AdbWinApi\adb_endpoint_object.h"
#include "adb_winusb_interface.h"

/** Class AdbWinUsbEndpointObject encapsulates a handle opened to an endpoint on
  our device.
*/
class AdbWinUsbEndpointObject : public AdbEndpointObject {
public:
    /** \brief Constructs the object

      @param[in] interface Parent WinUsb interface for this object.
      @param[in] endpoint_id Endpoint ID (endpoint address) on the device.
      @param[in] endpoint_index Zero-based endpoint index in the interface's
            array of endpoints.
    */
    AdbWinUsbEndpointObject(AdbWinUsbInterfaceObject* parent_interf,
        UCHAR endpoint_id,
        UCHAR endpoint_index);

protected:
    /** \brief Destructs the object.

      We hide destructor in order to prevent ourseves from accidentaly allocating
      instances on the stack. If such attemp occur, compiler will error.
    */
    virtual ~AdbWinUsbEndpointObject();

    //
    // Virtual overrides
    //

public:
    /** \brief Releases the object.

      If refcount drops to zero as the result of this release, the object is
      destroyed in this method. As a general rule, objects must not be touched
      after this method returns even if returned value is not zero. We override
      this method in order to make sure that objects of this class are deleted
      in contect of the DLL they were created in. The problem is that since
      objects of this class were created in context of AdbWinUsbApi module, they
      are allocated from the heap assigned to that module. Now, if these objects
      are deleted outside of AdbWinUsbApi module, this will lead to the heap
      corruption in the module that deleted these objects. Since all objects of
      this class are deleted in the Release method only, by overriding it we make
      sure that we free memory in the context of the module where it was
      allocated.
      @return Value of the reference counter after object is released in this
              method.
    */
    virtual LONG Release();

    /** \brief This method is called when handle to this object gets closed.

    In this call object is deleted from the AdbObjectHandleMap. We override
    this method in order to abort pending IOs and to prevent new IOs from
    starting up.
    @return true on success or false if object is already closed. If
            false is returned GetLastError() provides extended error
            information.
  */
    virtual bool CloseHandle();

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

protected:
    /** \brief Sets read / write operation timeout.

      @param[in] timeout Timeout value in milliseconds to use for current read
            or write operation. Zero value passed in this parameters indicate
            not timeout at all. Note that timeout that is set with this method is
            global per endpoint (pipe). I.e. once set, it will be used against
            all read / write operations performed on this endpoint, untill
            another call to this method modifies it. This is a WinUsb design
            flaw. Microsoft is aware of this and (hopefuly) future versions of
            WinUsb framework will accept a timeout parameter in WinUsb_Read/Write
            routines. For the purposes of ADB this flaw doesn't apperar to be an
            issue, since we use single-threaded synchronous read / writes, so
            there is no conflict in setting per-endpoint timeouts.
      @return true on success, false on failure. If false is returned
              GetLastError() provides extended error information.
    */
    virtual bool SetTimeout(ULONG timeout);

public:
    /// Gets parent WinUsb interface 
    AdbWinUsbInterfaceObject* parent_winusb_interface() const {
        return reinterpret_cast<AdbWinUsbInterfaceObject*>(parent_interface());
    }

    /// Gets parent interface WinUsb handle
    WINUSB_INTERFACE_HANDLE winusb_handle() const {
        return parent_winusb_interface()->winusb_handle();
    }

protected:
    /// Helper class whose destructor decrements pending_io_count_.
    class DecrementPendingIO {
    public:
        DecrementPendingIO(AdbWinUsbEndpointObject* endpoint)
            : endpoint_(endpoint) {}
        ~DecrementPendingIO() {
            endpoint_->lock_.Lock();
            ATLASSERT(endpoint_->pending_io_count_ > 0);
            --(endpoint_->pending_io_count_);
            endpoint_->lock_.Unlock();
        }
    private:
        AdbWinUsbEndpointObject* endpoint_;
    };

protected:
    /// Protects is_closing_ and pending_io_count_.
    CComAutoCriticalSection lock_;

    /// Once set, prevents new IOs from starting up.
    bool is_closing_;

    /// Count of pending IOs potentially blocked in WinUsb APIs.
    ULONG pending_io_count_;
};