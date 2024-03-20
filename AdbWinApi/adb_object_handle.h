#pragma once
#include "adb_api.h"
#include "adb_api_private_defines.h"

/** \brief Defines types of internal API objects
*/
enum AdbObjectType {
	/// Object is AdbInterfaceEnumObject.
	AdbObjectTypeInterfaceEnumerator,

	/// Object is AdbInterfaceObject.
	AdbObjectTypeInterface,

	/// Object is AdbEndpointObject.
	AdbObjectTypeEndpoint,

	/// Object is AdbIOCompletion.
	AdbObjectTypeIoCompletion,

	AdbObjectTypeMax
};

/** \brief Encapsulates an internal API basic object that is visible to the
  outside of the API through a handle.

  In order to prevent crashes when API client tries to access an object through
  an invalid or already closed handle, we keep track of all opened handles in
  AdbObjectHandleMap that maps association between valid ADBAPIHANDLE and
  an object that this handle represents. All objects that are exposed to the
  outside of API via ADBAPIHANDLE are self-destructing referenced objects.
  The reference model for these objects is as such:
  1. When CreateHandle() method is called on an object, a handle (ADBAPIHANDLE
     that is) is assigned to it, a pair <handle, object> is added to the global
     AdbObjectHandleMap instance, object is referenced and then handle is
     returned to the API client.
  2. Every time API is called with a handle, a lookup is performed in
     AdbObjectHandleMap to find an object that is associated with the handle.
     If object is not found then ERROR_INVALID_HANDLE is immediatelly returned
     (via SetLastError() call). If object is found then it is referenced and
     API call is dispatched to appropriate method of the found object. Upon
     return from this method, just before returning from the API call, object
     is dereferenced back to match lookup reference.
  3. When object handle gets closed, assuming object is found in the map, that
     <handle, object> pair is deleted from the map and object's refcount is
     decremented to match refcount increment performed when object has been
     added to the map.
  4. When object's refcount drops to zero, the object commits suicide by
     calling "delete this".
  All API objects that have handles that are sent back to API client must be
  derived from this class.
*/
class ADBWIN_API_CLASS AdbObjectHandle {
public:
    explicit AdbObjectHandle(AdbObjectType obj_type);

protected:
    virtual ~AdbObjectHandle();

public:
    virtual LONG AddRef();
    virtual LONG Release();
    virtual ADBAPIHANDLE CreateHandle();
    virtual bool CloseHandle();
    virtual bool IsObjectOfType(AdbObjectType obj_type) const;
    static AdbObjectHandle* Lookup(ADBAPIHANDLE adb_handle);

protected:
    virtual void LastReferenceReleased();

public:
    ADBAPIHANDLE adb_handle() const {
        return adb_handle_;
    }

    /// Gets type of this object
    AdbObjectType object_type() const {
        return object_type_;
    }

    /// Checks if object is still opened. Note that it is not guaranteed that
    /// object remains opened when this method returns.
    bool IsOpened() const {
        return (nullptr != adb_handle());
    }

protected:
    /// API handle associated with this object
    ADBAPIHANDLE adb_handle_;

    /// Type of this object
    AdbObjectType object_type_;

    /// This object's reference counter
    LONG ref_count_;
};

/// Maps ADBAPIHANDLE to associated AdbObjectHandle object
typedef std::map< ADBAPIHANDLE, AdbObjectHandle* > AdbObjectHandleMap;

/** \brief Template routine that unifies extracting of objects of different
  types from the AdbObjectHandleMap

  @param[in] adb_handle API handle for the object
  @return Object associated with the handle or NULL on error. If NULL is
          returned GetLastError() provides extended error information.
*/
template<class obj_class>
obj_class* LookupObject(ADBAPIHANDLE adb_handle) {
    // Lookup object for the handle in the map
    AdbObjectHandle* adb_object = AdbObjectHandle::Lookup(adb_handle);
    if (nullptr != adb_object) {
        // Make sure it's of the correct type
        if (!adb_object->IsObjectOfType(obj_class::Type())) {
            adb_object->Release();
            adb_object = nullptr;
            SetLastError(ERROR_INVALID_HANDLE);
        }
    }
    else {
        SetLastError(ERROR_INVALID_HANDLE);
    }
    return (adb_object != nullptr) ? reinterpret_cast<obj_class*>(adb_object) : nullptr;
}