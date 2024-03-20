#pragma once
#include "adb_api.h"
#include "adb_api_private_defines.h"

typedef void* ADBAPIINSTANCEHANDLE;

/** Class AdbApiInstance is the main API interbal object representing a device
  interface that is in the interest of the API client. All device (interface)
  related operations go through this class first. So, before doing anything
  meaningfull with the API a client must first create instance of the API
  via CreateAdbApiInstance, select a device interface for that instance and
  then do everything else.
  Objects of this class are globally stored in the map that matches
  ADBAPIINSTANCEHANDLE to the corresponded object.
  This class is self-referenced with the following reference model:
  1. When object of this class is created and added to the map, its recount
     is set to 1.
  2. Every time the client makes an API call that uses ADBAPIINSTANCEHANDLE
     a corresponded AdbApiInstance object is looked up in the table and its
     refcount is incremented. Upon return from the API call that incremented
     the refcount refcount gets decremented.
  3. When the client closes ADBAPIINSTANCEHANDLE via DeleteAdbApiInstance call
     corresponded object gets deleted from the map and its refcount is
     decremented.
  So, at the end, this object destroys itself when refcount drops to zero.
*/
class AdbApiInstance {
public:
    AdbApiInstance();

private:
    ~AdbApiInstance();

    void LastReferenceReleased();

public:
    /// Gets name of the USB interface (device name) for this instance
    const std::wstring& interface_name() const {
        return interface_name_;
    }

    /// References the object and returns number of references
    LONG AddRef() {
        return InterlockedIncrement(&ref_count_);
    }

    LONG Release() {
        LONG ret = InterlockedDecrement(&ref_count_);
        if (0 == ret)
            LastReferenceReleased();

        return ret;
    }

    /// Checks if instance has been initialized
    bool IsInitialized() const {
        return !interface_name_.empty();
    }


private:
    /// Name of the USB interface (device name) for this instance
    std::wstring          interface_name_;

    /// Instance handle for this object
    ADBAPIINSTANCEHANDLE  instance_handle_;

    /// Reference counter for this instance
    LONG                  ref_count_;
};

/// Defines map that matches ADBAPIINSTANCEHANDLE with AdbApiInstance object
typedef std::map<ADBAPIINSTANCEHANDLE, AdbApiInstance*> AdbApiInstanceMap;