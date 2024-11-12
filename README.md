# adb

Porting Android Debug Bridge (HOST) to Microsoft Visual C++.


## Prequisites

* Visual Studio 2022

## Build

1. Getting source code from the git repository:
```
git clone https://github.com/BeneficialCode/adb
```

2. install some dependencies

```
vcpkg install abseil:x86-windows-static
vcpkg install boringssl:x86-windows-static
vcpkg install protobuf:x86-windows-static
vcpkg install brotli:x86-windows-static
vcpkg install libusb:x86-windows-static
vcpkg install lz4:x86-windows-static
vcpkg install mdnsresponder:x86-windows-static
vcpkg install zstd:x86-windows-static
```

3. Build AdbWinApi, AdbWinUsbApi and Adb

Enjoy it :)
