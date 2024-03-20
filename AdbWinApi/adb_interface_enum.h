#pragma once
#include "adb_object_handle.h"


/** \brief Enumerator of USB interfaces available through this API.
*/
class AdbInterfaceEnumObject : public AdbObjectHandle {
public:
    /** \brief Constructs the object.
    */
    AdbInterfaceEnumObject();

protected:
    /** \brief Destructs the object.

     We hide destructor in order to prevent ourseves from accidentaly allocating
     instances on the stack. If such attemp occur, compiler will error.
    */
    virtual ~AdbInterfaceEnumObject();

public:
    /** \brief Enumerates all interfaces for the given device class.

      This routine uses SetupDiGetClassDevs to get our device info and calls
      EnumerateDeviceInterfaces to perform the enumeration.
      @param[in] class_id Device class ID that is specified by our USB driver
      @param[in] exclude_not_present If set include only those devices that are
             currently present.
      @param[in] exclude_removed If true interfaces with SPINT_REMOVED flag set
             will be not included in the enumeration.
      @param[in] active_only If true only active interfaces (with flag
             SPINT_ACTIVE set) will be included in the enumeration.
      @return True on success, false on failure, in which case GetLastError()
              provides extended information about the error that occurred.
    */
    bool InitializeEnum(GUID class_id,
        bool exclude_not_present,
        bool exclude_removed,
        bool active_only);

    /** \brief Gets next enumerated interface information
      @param[out] info Upon successful completion will receive interface
             information. Can be NULL. If it is NULL, upon return from this
             method *size will have memory size required to fit this entry.
      @param[in,out] size On the way in provides size of the memory buffer
             addressed by info param. On the way out (only if buffer is not
             big enough) will provide memory size required to fit this entry.
      @return true on success, false on error. If false is returned
              GetLastError() provides extended information about the error that
              occurred. ERROR_INSUFFICIENT_BUFFER indicates that buffer provided
              in info param was not big enough and *size specifies memory size
              required to fit this entry. ERROR_NO_MORE_ITEMS indicates that
              enumeration is over and there are no more entries to return.
    */
    bool Next(AdbInterfaceInfo** pInfo, ULONG* size);

    /** \brief Makes enumerator to start from the beginning.
      @return true on success, false on error. If false is returned
              GetLastError() provides extended information about the error that
              occurred.
    */
    bool Reset();

    // This is a helper for extracting object from the AdbObjectHandleMap
    static AdbObjectType Type() {
        return AdbObjectTypeInterfaceEnumerator;
    }

protected:
    /// Array of interfaces enumerated with this object
    AdbEnumInterfaceArray           interfaces_;

    /// Current enumerator
    AdbEnumInterfaceArray::iterator current_interface_;
};