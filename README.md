# adb

Porting Android Debug Bridge (HOST) to Microsoft Visual C++.


## Prequisites

* Visual Studio 2022

Desktop development with C++ （MFC x86 x64)

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
vcpkg install zlib:x86-windows-static
vcpkg install getopt:x86-windows-static
```

3. generate the proto headers
```
protoc.exe -I=D:\Data\dev\C++\adb\proto -I=D:\Data\dev\C++\adb\fastdeploy\proto --cpp_out=D:\Data\dev\C++\adb\adb D:\Data\dev\C++\adb\proto\adb_known_hosts.proto D:\Data\dev\C++\adb\proto\app_processes.proto D:\Data\dev\C++\adb\proto\key_type.proto D:\Data\dev\C++\adb\proto\pairing.proto D:\Data\dev\C++\adb\fastdeploy\proto\ApkEntry.proto
```

4. Build AdbWinApi, AdbWinUsbApi and Adb

Enjoy it :)
