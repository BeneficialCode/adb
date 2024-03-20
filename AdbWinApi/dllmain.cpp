// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "adb_api.h"
#include "adb_winusb_api.h"

using namespace ATL;

/// References InstantiateWinUsbInterface declared in adb_api.cpp
extern PFN_INSTWINUSBINTERFACE InstantiateWinUsbInterface;

class CAdbWinApiModule : public CAtlDllModuleT<CAdbWinApiModule> {
public:
    CAdbWinApiModule() 
        :CAtlDllModuleT<CAdbWinApiModule>(),
        adbwinusbapi_handle_(nullptr), is_initialized_(false) {
	}

    ~CAdbWinApiModule(){
        // Unload AdbWinUsbApi.dll before we exit
        if (adbwinusbapi_handle_ != nullptr) {
			FreeLibrary(adbwinusbapi_handle_);
			adbwinusbapi_handle_ = nullptr;
		}
    }

    /** \brief Loads AdbWinUsbApi.dll and caches its InstantiateWinUsbInterface
    export.

    This method is called from DllMain on DLL_PROCESS_ATTACH event. In this
    method we will check if WINUSB.DLL required by AdbWinUsbApi.dll is
    installed, and if it is we will load AdbWinUsbApi.dll and cache address of
    InstantiateWinUsbInterface routine exported from AdbWinUsbApi.dll
    */
    void AttachToAdbWinUsbApi() {
        // We only need to run this only once.
        if (is_initialized_) {
			return;
		}

        // Just mark that we have ran initialization.
        is_initialized_ = true;

        // Before we can load AdbWinUsbApi.dll we need to check if WINUSB.DLL is
        // installed.
        wchar_t path_to_winusb_dll[MAX_PATH];
        if (GetSystemDirectory(path_to_winusb_dll, MAX_PATH) == 0) {
            return;
        }

        wcscat_s(path_to_winusb_dll, MAX_PATH, L"\\winusb.dll");

        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(path_to_winusb_dll)) {
            // WINUSB.DLL is not installed. We can't use AdbWinUsbApi.dll
			return;
		}

        // WINUSB.DLL is installed. Lets load AdbWinUsbApi.dll and cache its
        // InstantiateWinUsbInterface export.
        // We require that AdbWinUsbApi.dll is located in the same folder
        // where AdbWinApi.dll and adb.exe are located, so by Windows
        // conventions we can pass just module name, and not the full path.
        adbwinusbapi_handle_ = LoadLibrary(L"AdbWinUsbApi.dll");
        if (adbwinusbapi_handle_ == nullptr) {
			return;
		}
        // Cache the address of InstantiateWinUsbInterface export.
        InstantiateWinUsbInterface = reinterpret_cast<PFN_INSTWINUSBINTERFACE>(
			GetProcAddress(adbwinusbapi_handle_, "InstantiateWinUsbInterface"));
    }

protected:
    /// Handle to the loaded AdbWinUsbApi.dll
    HINSTANCE adbwinusbapi_handle_;

    /// Flags whether or not this module has been initialized.
    bool      is_initialized_;
};

CAdbWinApiModule _AtlModule;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Lets see if we need to initialize InstantiateWinUsbInterface
    // variable. We do that only once, on condition that this DLL is
    // being attached to the process and InstantiateWinUsbInterface
    // address has not been calculated yet.
    if (DLL_PROCESS_ATTACH == ul_reason_for_call) {
        _AtlModule.AttachToAdbWinUsbApi();
    }
    return _AtlModule.DllMain(ul_reason_for_call, lpReserved);
}

