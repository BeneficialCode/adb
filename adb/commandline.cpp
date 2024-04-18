#define TRACE_TAG ADB

#include "sysdeps.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "file.h"
#include "logging.h"
#include "parseint.h"
#include "stringprintf.h"
#include "strings.h"


#if !defined(_WIN32)
#include <sys/ioctl.h>
#include <termios.h>
#else
#define _POSIX
#include <signal.h>
#endif

#include <google/protobuf/text_format.h>

#include "adb.h"
#include "adb_auth.h"
#include "adb_client.h"
