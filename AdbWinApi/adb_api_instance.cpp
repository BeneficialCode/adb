#include "pch.h"
#include "adb_api_instance.h"

/// Map that holds all instances of this object
AdbApiInstanceMap adb_app_instance_map;
ULONG_PTR adb_app_instance_id = 0;
CComAutoCriticalSection adb_app_instance_map_locker;

AdbApiInstance::AdbApiInstance()
    : ref_count_(1) {
    // Generate inteface handle
    adb_app_instance_map_locker.Lock();
    adb_app_instance_id++;
    adb_app_instance_map_locker.Unlock();
    instance_handle_ =
        reinterpret_cast<ADBAPIINSTANCEHANDLE>(adb_app_instance_id);
}

AdbApiInstance::~AdbApiInstance() {
}

void AdbApiInstance::LastReferenceReleased() {
}