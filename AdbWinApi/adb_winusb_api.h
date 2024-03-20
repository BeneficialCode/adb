#pragma once

/** \file
  Contains declarations required to link AdbWinApi and AdbWinUsbApi DLLs.
*/

/** \brief Function prototype for InstantiateWinUsbInterface routine exported
  from AdbWinUsbApi.dll

  In order to provide backward compatibility with the systems that still run
  legacy (custom) USB drivers, and have not installed WINUSB.DLL we need to
  split functionality of our ADB API on Windows between two DLLs: AdbWinApi,
  and AdbWinUsbApi. AdbWinApi is fully capable of working on top of the legacy
  driver, but has no traces to WinUsb. AdbWinUsbApi is capable of working on
  top of WinUsb API. We are forced to do this split, because we can have
  dependency on WINUSB.DLL in the DLL that implements legacy API. The problem
  is that customers may have a legacy driver that they don't want to upgrade
  to WinUsb, so they may not have WINUSB.DLL installed on their machines, but
  they still must be able to use ADB. So, the idea behind the split is as
  such. When AdbWinApi.dll is loaded into a process, it will check WINUSB.DLL
  installation (by checking existance of C:\Windows\System32\winusb.dll). If
  WINUSB.DLL is installed, AdbWinApi will also load AdbWinUsbApi.dll (by
  calling LoadLibrary), and will extract address of InstantiateWinUsbInterface
  routine exported from AdbWinUsbApi.dll. Then this routine will be used to
  instantiate AdbInterfaceObject instance on condition that it is confirmed
  that USB driver underneath us is in deed WinUsb.
*/
typedef class AdbInterfaceObject* \
(__cdecl* PFN_INSTWINUSBINTERFACE)(const wchar_t*);