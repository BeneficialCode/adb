#pragma once
#include "adb_endpoint_object.h"

/** \brief Encapsulates encapsulates a generic wrapper around OVERLAPPED Win32
  structure returned from asynchronous I/O requests.

  This is an abstract class that implements functionality common for I/O
  performed via WinUsb as well as legacy driver APIs. A handle to this object
  is returned to the caller of each successful asynchronous I/O request. Just
  like all other handles this handle must be closed after it's no longer
  needed.
*/
class ADBWIN_API_CLASS AdbIOCompletion : public AdbObjectHandle {
public:
    /** \brief Constructs the object

      @param[in] parent_io_obj Parent I/O object that created this instance.
             Parent object will be referenced in this object's constructor and
             released in the destructor.
      @param[in] expected_trans_size Number of bytes expected to be transferred
            with the I/O.
      @param[in] event_hndl Event handle that should be signaled when I/O
             completes. Can be NULL. If it's not NULL this handle will be
             used to initialize OVERLAPPED structure for this object.
    */
    AdbIOCompletion(AdbEndpointObject* parent_io_obj,
        ULONG expected_trans_size,
        HANDLE event_hndl);

protected:
    /** \brief Destructs the object.

      We hide destructor in order to prevent ourseves from accidentaly allocating
      instances on the stack. If such attemp occur, compiler will error.
    */
    virtual ~AdbIOCompletion();

    //
    // Abstract
    //

public:
    /** \brief Gets overlapped I/O result

      @param[out] ovl_data Buffer for the copy of this object's OVERLAPPED
             structure. Can be NULL.
      @param[out] bytes_transferred Pointer to a variable that receives the
             number of bytes that were actually transferred by a read or write
             operation. See SDK doc on GetOvelappedResult for more information.
             Unlike regular GetOvelappedResult call this parameter can be NULL.
      @param[in] wait If this parameter is true, the method does not return
             until the operation has been completed. If this parameter is false
             and the operation is still pending, the method returns false and
             the GetLastError function returns ERROR_IO_INCOMPLETE.
      @return true if I/O has been completed or false on failure or if request
             is not yet completed. If false is returned GetLastError() provides
             extended error information. If GetLastError returns
             ERROR_IO_INCOMPLETE it means that I/O is not yet completed.
    */
    virtual bool GetOvelappedIoResult(LPOVERLAPPED ovl_data,
        ULONG* bytes_transferred,
        bool wait) = 0;

    //
    // Operations
    //

public:
    /** \brief Checks if I/O that this object represents has completed.

      @return true if I/O has been completed or false if it's still
              incomplete. Regardless of the returned value, caller should
              check GetLastError to validate that handle was OK.
    */
    virtual bool IsCompleted();

public:
    /// Gets overlapped structure for this I/O
    LPOVERLAPPED overlapped() {
        return &overlapped_;
    }

    /// Gets parent object
    AdbEndpointObject* parent_io_object() const {
        return parent_io_object_;
    }

    /// Gets parent object handle
    ADBAPIHANDLE GetParentObjectHandle() const {
        return (nullptr != parent_io_object()) ? parent_io_object()->adb_handle() :
            nullptr;
    }

    // This is a helper for extracting object from the AdbObjectHandleMap
    static AdbObjectType Type() {
        return AdbObjectTypeIoCompletion;
    }

protected:
    /// Overlapped structure for this I/O
    OVERLAPPED          overlapped_;

    /// Parent I/O object
    AdbEndpointObject* parent_io_object_;

    /// Expected number of bytes transferred in thi I/O
    ULONG               expected_transfer_size_;
};