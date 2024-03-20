// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "adb_winusb_interface.h"

#pragma comment(lib,"AdbWinApi")
#pragma comment(lib,"Winusb")

class CAdbWinApiModule : public CAtlDllModuleT<CAdbWinApiModule>
{
public:

};

CAdbWinApiModule _AtlModule;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return _AtlModule.DllMain(ul_reason_for_call, lpReserved);
}

/** \brief Instantiates interface instance that uses WinUsb API to communicate
  with USB driver.

  This is the only exported routine from this DLL. This routine instantiates an
  object of AdbWinUsbInterfaceObject on request from AdbWinApi.dll when it is
  detected that underlying USB driver is WinUsb.sys.
  @param[in] interface_name Name of the interface.
  @return AdbInterfaceObject - casted instance of AdbWinUsbInterfaceObject
          object on success, or NULL on failure with GetLastError providing
          information on an error that occurred.
*/
extern "C" __declspec(dllexport)
AdbInterfaceObject * __cdecl InstantiateWinUsbInterface(
    const wchar_t* interface_name) {
    // Validate parameter.
    if (NULL == interface_name) {
        return NULL;
    }

    // Instantiate requested object.
    try {
        return new AdbWinUsbInterfaceObject(interface_name);
    }
    catch (...) {
        // We expect only OOM exceptions here.
        SetLastError(ERROR_OUTOFMEMORY);
        return NULL;
    }
}