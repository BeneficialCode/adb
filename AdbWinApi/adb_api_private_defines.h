#pragma once
#include "adb_api.h"

/** \brief Encapsulates an entry in the array of enumerated interfaces.
*/
class AdbInstanceEnumEntry {
public:
	AdbInstanceEnumEntry() : flags_(0) {
		ZeroMemory(&class_id_, sizeof(class_id_));
	}

	void Set(const wchar_t* dev_name, GUID cls_id, DWORD flags) {
		device_name_ = dev_name;
		class_id_ = cls_id;
		flags_ = flags;
	}

	AdbInstanceEnumEntry(const AdbInstanceEnumEntry& proto) {
		Set(proto.device_name_.c_str(), proto.class_id_, proto.flags_);
	}

	AdbInstanceEnumEntry(const wchar_t* dev_name, GUID cls_id, DWORD flags) {
		Set(dev_name, cls_id, flags);
	}

	AdbInstanceEnumEntry& operator=(const AdbInstanceEnumEntry& proto) {
		Set(proto.device_name_.c_str(), proto.class_id_, proto.flags_);
		return *this;
	}
	/// Calculates memory size needed to save this entry into AdbInterfaceInfo
	/// structure
	ULONG GetFlatSize() const {
		return static_cast<ULONG>(FIELD_OFFSET(AdbInterfaceInfo, device_name) +
			(device_name_.length() + 1) * sizeof(wchar_t));
	}

	/** \brief Saves this entry into AdbInterfaceInfo structure.

	  @param[in] info Buffer to save this entry to. Must be big enough to fit it.
			 Use GetFlatSize() method to get buffer size needed for that.

	*/
	void Save(AdbInterfaceInfo** pInfo,ULONG* size) const {
		ULONG totalSize = *size;
		AdbInterfaceInfo* info = (AdbInterfaceInfo*)malloc(totalSize);
		ZeroMemory(info, totalSize);
		if (info == NULL) {
			SetLastError(ERROR_OUTOFMEMORY);
			return;
		}
		*pInfo = info;
		info->class_id = class_id_;
		info->flags = flags_;
		ULONG strSize = device_name_.length() + 1;
		wcscpy_s(info->device_name, strSize, device_name_.c_str());
	}

	/// Gets interface's device name
	const std::wstring& device_name() const {
		return device_name_;
	}

	/// Gets inteface's class id
	GUID class_id() const {
		return class_id_;
	}

	/// Gets interface flags
	DWORD flags() const {
		return flags_;
	}

private:
	/// Inteface's class id (see SP_DEVICE_INTERFACE_DATA)
	GUID class_id_;
	/// Interface's device name
	std::wstring device_name_;
	/// Interface flags (see SP_DEVICE_INTERFACE_DATA)
	DWORD flags_;
};

/// Defines array of enumerated interface entries
typedef std::vector<AdbInstanceEnumEntry> AdbEnumInterfaceArray;