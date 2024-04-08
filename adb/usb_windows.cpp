#define TRACE_TAG USB

#include "sysdeps.h"

#include "usb.h"

#include <WinSock2.h>
#include <Windows.h>
#include <usb100.h>
#include <winerror.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <mutex>
#include <thread>

#include "adb_api.h"

