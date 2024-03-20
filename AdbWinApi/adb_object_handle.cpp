#include "pch.h"
#include "adb_object_handle.h"

/// Global ADBAPIHANDLE -> AdbObjectHandle* map
AdbObjectHandleMap      the_map;

/// Locker for the AdbObjectHandleMap instance
CComAutoCriticalSection the_map_locker;

/// Next adb handle value generator
ULONG_PTR               next_adb_handle_value = 0;

AdbObjectHandle::AdbObjectHandle(AdbObjectType obj_type)
	: adb_handle_(nullptr), object_type_(obj_type), ref_count_(1) {
	ATLASSERT(obj_type < AdbObjectTypeMax);
}

AdbObjectHandle::~AdbObjectHandle() {
	ATLASSERT(0 == ref_count_);
	ATLASSERT(nullptr == adb_handle_);
}

LONG AdbObjectHandle::AddRef() {
	ATLASSERT(ref_count_ > 0);
	return InterlockedIncrement(&ref_count_);
}

LONG AdbObjectHandle::Release() {
	ATLASSERT(ref_count_ > 0);
	LONG ret = InterlockedDecrement(&ref_count_);
	ATLASSERT(ret >= 0);
	if (0 == ret) {
		LastReferenceReleased();
		delete this;
	}
	return ret;
}


ADBAPIHANDLE AdbObjectHandle::CreateHandle() {
	ADBAPIHANDLE ret = nullptr;

	// Lock the map
	the_map_locker.Lock();

	ATLASSERT(!IsOpened());

	if (!IsOpened()) {
		try
		{
			// Generate new handle value
			++next_adb_handle_value;
			ret = reinterpret_cast<ADBAPIHANDLE>(next_adb_handle_value);

			// Insert this object into the map
			the_map[ret] = this;

			// Set the handle
			adb_handle_ = ret;
			AddRef();
		}
		catch (...)
		{
			ret = nullptr;
			SetLastError(ERROR_OUTOFMEMORY);
		}
	}
	else {
		SetLastError(ERROR_GEN_FAILURE);
	}

	// Unlock the map
	the_map_locker.Unlock();

	return ret;
}

bool AdbObjectHandle::CloseHandle() {
	bool ret = false;

	// Addref just in case that last reference to this object is being
	// held in the map
	AddRef();

	// Lock the map
	the_map_locker.Lock();

	ATLASSERT(IsOpened());

	if (IsOpened()) {
		try
		{
			// Lookup this object in the map
			AdbObjectHandleMap::iterator it = the_map.find(adb_handle_);
			if ((it != the_map.end()) && (this == it->second)) {
				// Remove this object from the map
				the_map.erase(it);
				adb_handle_ = nullptr;
				Release();
				ret = true;
			}
			else {
				SetLastError(ERROR_INVALID_HANDLE);
			}
		}
		catch (...)
		{
			ret = false;
			SetLastError(ERROR_OUTOFMEMORY);
		}
	}
	else {
		SetLastError(ERROR_INVALID_HANDLE);
	}

	the_map_locker.Unlock();

	Release();

	return ret;
}

bool AdbObjectHandle::IsObjectOfType(AdbObjectType obj_type) const {
	return (obj_type == object_type_);
}

void AdbObjectHandle::LastReferenceReleased() {
	ATLASSERT(!IsOpened());
}

AdbObjectHandle* AdbObjectHandle::Lookup(ADBAPIHANDLE adb_hndl) {
	AdbObjectHandle* ret = nullptr;

	the_map_locker.Lock();

	try {
		// Look us up in the map.
		AdbObjectHandleMap::iterator found = the_map.find(adb_hndl);
		if (found != the_map.end()) {
			ret = found->second;
			ret->AddRef();
		}
	}
	catch (...) {
		SetLastError(ERROR_OUTOFMEMORY);
	}

	the_map_locker.Unlock();

	return ret;
}