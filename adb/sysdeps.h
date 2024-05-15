#pragma once
#include <errno.h>

#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "errors.h"
#include "macros.h"
#include "off64_t.h"
#include "unique_fd.h"
#include "utf8.h"

#include "adb_unique_fd.h"
#include "network.h"
#include "stat.h"

#include <ctype.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <stdint.h>
#include <sys/stat.h>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <windows.h>


#include <memory>   // unique_ptr
#include <string>

#include "dirent-win32.h"



#define OS_PATH_SEPARATORS "\\/"
#define OS_PATH_SEPARATOR '\\'
#define OS_PATH_SEPARATOR_STR "\\"
#define ENV_PATH_SEPARATOR_STR ";"

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _STRING
{
    USHORT Length;
    USHORT MaximumLength;
    _Field_size_bytes_part_opt_(MaximumLength, Length) PCHAR Buffer;
} STRING, * PSTRING, ANSI_STRING, * PANSI_STRING, OEM_STRING, * POEM_STRING;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
    USHORT Flags;
    USHORT Length;
    ULONG TimeStamp;
    STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;

typedef struct _CURDIR
{
    UNICODE_STRING DosPath;
    HANDLE Handle;
} CURDIR, * PCURDIR;

#define RTL_MAX_DRIVE_LETTERS 32

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
    ULONG MaximumLength;
    ULONG Length;

    ULONG Flags;
    ULONG DebugFlags;

    HANDLE ConsoleHandle;
    ULONG ConsoleFlags;
    HANDLE StandardInput;
    HANDLE StandardOutput;
    HANDLE StandardError;

    CURDIR CurrentDirectory;
    UNICODE_STRING DllPath;
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
    PVOID Environment;

    ULONG StartingX;
    ULONG StartingY;
    ULONG CountX;
    ULONG CountY;
    ULONG CountCharsX;
    ULONG CountCharsY;
    ULONG FillAttribute;

    ULONG WindowFlags;
    ULONG ShowWindowFlags;
    UNICODE_STRING WindowTitle;
    UNICODE_STRING DesktopInfo;
    UNICODE_STRING ShellInfo;
    UNICODE_STRING RuntimeData;
    RTL_DRIVE_LETTER_CURDIR CurrentDirectories[RTL_MAX_DRIVE_LETTERS];

    ULONG_PTR EnvironmentSize;
    ULONG_PTR EnvironmentVersion;
    PVOID PackageDependencyData;
    ULONG ProcessGroupId;
    ULONG LoaderThreads;

    UNICODE_STRING RedirectionDllName; // REDSTONE4
    UNICODE_STRING HeapPartitionName; // 19H1
    ULONG_PTR DefaultThreadpoolCpuSetMasks;
    ULONG DefaultThreadpoolCpuSetMaskCount;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

// private
typedef struct _API_SET_NAMESPACE
{
    ULONG Version;      // v2 on Windows 7, v4 on Windows 8.1  and v6 on Windows 10
    ULONG Size;         // apiset map size (usually the .apiset section virtual size)
    ULONG Flags;        // according to Geoff Chappell,  tells if the map is sealed or not.
    ULONG Count;        // hash table entry count
    ULONG EntryOffset;  // Offset to the api set entries values
    ULONG HashOffset;   // Offset to the api set entries hash indexes
    ULONG HashFactor;   // multiplier to use when computing hash 
} API_SET_NAMESPACE, * PAPI_SET_NAMESPACE;

#define GDI_HANDLE_BUFFER_SIZE32 34
#define GDI_HANDLE_BUFFER_SIZE64 60

#ifndef _WIN64
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE32
#else
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE64
#endif

typedef ULONG GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];

// symbols
typedef struct _PEB
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    union
    {
        BOOLEAN BitField;
        struct
        {
            BOOLEAN ImageUsesLargePages : 1;
            BOOLEAN IsProtectedProcess : 1;
            BOOLEAN IsImageDynamicallyRelocated : 1;
            BOOLEAN SkipPatchingUser32Forwarders : 1;
            BOOLEAN IsPackagedProcess : 1;
            BOOLEAN IsAppContainer : 1;
            BOOLEAN IsProtectedProcessLight : 1;
            BOOLEAN IsLongPathAwareProcess : 1;
        };
    };

    HANDLE Mutant;

    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
    PRTL_CRITICAL_SECTION FastPebLock;
    PVOID IFEOKey;
    PSLIST_HEADER AtlThunkSListPtr;
    union
    {
        ULONG CrossProcessFlags;
        struct
        {
            ULONG ProcessInJob : 1;
            ULONG ProcessInitializing : 1;
            ULONG ProcessUsingVEH : 1;
            ULONG ProcessUsingVCH : 1;
            ULONG ProcessUsingFTH : 1;
            ULONG ProcessPreviouslyThrottled : 1;
            ULONG ProcessCurrentlyThrottled : 1;
            ULONG ProcessImagesHotPatched : 1; // REDSTONE5
            ULONG ReservedBits0 : 24;
        };
    };
    union
    {
        PVOID KernelCallbackTable;
        PVOID UserSharedInfoPtr;
    };
    ULONG SystemReserved;
    ULONG AtlThunkSListPtr32;
    PAPI_SET_NAMESPACE ApiSetMap;
    ULONG TlsExpansionCounter;
    PVOID TlsBitmap;
    ULONG TlsBitmapBits[2];

    PVOID ReadOnlySharedMemoryBase;
    PVOID SharedData; // HotpatchInformation
    PVOID* ReadOnlyStaticServerData;

    PVOID AnsiCodePageData; // PCPTABLEINFO
    PVOID OemCodePageData; // PCPTABLEINFO
    PVOID UnicodeCaseTableData; // PNLSTABLEINFO

    ULONG NumberOfProcessors;
    ULONG NtGlobalFlag;

    ULARGE_INTEGER CriticalSectionTimeout;
    SIZE_T HeapSegmentReserve;
    SIZE_T HeapSegmentCommit;
    SIZE_T HeapDeCommitTotalFreeThreshold;
    SIZE_T HeapDeCommitFreeBlockThreshold;

    ULONG NumberOfHeaps;
    ULONG MaximumNumberOfHeaps;
    PVOID* ProcessHeaps; // PHEAP

    PVOID GdiSharedHandleTable;
    PVOID ProcessStarterHelper;
    ULONG GdiDCAttributeList;

    PRTL_CRITICAL_SECTION LoaderLock;

    ULONG OSMajorVersion;
    ULONG OSMinorVersion;
    USHORT OSBuildNumber;
    USHORT OSCSDVersion;
    ULONG OSPlatformId;
    ULONG ImageSubsystem;
    ULONG ImageSubsystemMajorVersion;
    ULONG ImageSubsystemMinorVersion;
    ULONG_PTR ActiveProcessAffinityMask;
    GDI_HANDLE_BUFFER GdiHandleBuffer;
    PVOID PostProcessInitRoutine;

    PVOID TlsExpansionBitmap;
    ULONG TlsExpansionBitmapBits[32];

    ULONG SessionId;

    ULARGE_INTEGER AppCompatFlags;
    ULARGE_INTEGER AppCompatFlagsUser;
    PVOID pShimData;
    PVOID AppCompatInfo; // APPCOMPAT_EXE_DATA

    UNICODE_STRING CSDVersion;

    PVOID ActivationContextData; // ACTIVATION_CONTEXT_DATA
    PVOID ProcessAssemblyStorageMap; // ASSEMBLY_STORAGE_MAP
    PVOID SystemDefaultActivationContextData; // ACTIVATION_CONTEXT_DATA
    PVOID SystemAssemblyStorageMap; // ASSEMBLY_STORAGE_MAP

    SIZE_T MinimumStackCommit;

    PVOID SparePointers[4]; // 19H1 (previously FlsCallback to FlsHighIndex)
    ULONG SpareUlongs[5]; // 19H1
    //PVOID* FlsCallback;
    //LIST_ENTRY FlsListHead;
    //PVOID FlsBitmap;
    //ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof(ULONG) * 8)];
    //ULONG FlsHighIndex;

    PVOID WerRegistrationData;
    PVOID WerShipAssertPtr;
    PVOID pUnused; // pContextData
    PVOID pImageHeaderHash;
    union
    {
        ULONG TracingFlags;
        struct
        {
            ULONG HeapTracingEnabled : 1;
            ULONG CritSecTracingEnabled : 1;
            ULONG LibLoaderTracingEnabled : 1;
            ULONG SpareTracingBits : 29;
        };
    };
    ULONGLONG CsrServerReadOnlySharedMemoryBase;
    PRTL_CRITICAL_SECTION TppWorkerpListLock;
    LIST_ENTRY TppWorkerpList;
    PVOID WaitOnAddressHashTable[128];
    PVOID TelemetryCoverageHeader; // REDSTONE3
    ULONG CloudFileFlags;
    ULONG CloudFileDiagFlags; // REDSTONE4
    CHAR PlaceholderCompatibilityMode;
    CHAR PlaceholderCompatibilityModeReserved[7];
    struct _LEAP_SECOND_DATA* LeapSecondData; // REDSTONE5
    union
    {
        ULONG LeapSecondFlags;
        struct
        {
            ULONG SixtySecondEnabled : 1;
            ULONG Reserved : 31;
        };
    };
    ULONG NtGlobalFlag2;
} PEB, * PPEB;

typedef _Success_(return >= 0) LONG NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
typedef LONG KPRIORITY;

typedef struct _PROCESS_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

typedef enum _PROCESSINFOCLASS
{
    ProcessBasicInformation, // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
    ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
    ProcessIoCounters, // q: IO_COUNTERS
    ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
    ProcessTimes, // q: KERNEL_USER_TIMES
    ProcessBasePriority, // s: KPRIORITY
    ProcessRaisePriority, // s: ULONG
    ProcessDebugPort, // q: HANDLE
    ProcessExceptionPort, // s: PROCESS_EXCEPTION_PORT
    ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN
    ProcessLdtInformation, // qs: PROCESS_LDT_INFORMATION // 10
    ProcessLdtSize, // s: PROCESS_LDT_SIZE
    ProcessDefaultHardErrorMode, // qs: ULONG
    ProcessIoPortHandlers, // (kernel-mode only)
    ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS
    ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
    ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS
    ProcessWx86Information,
    ProcessHandleCount, // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
    ProcessAffinityMask, // s: KAFFINITY
    ProcessPriorityBoost, // qs: ULONG
    ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
    ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION
    ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
    ProcessWow64Information, // q: ULONG_PTR
    ProcessImageFileName, // q: UNICODE_STRING
    ProcessLUIDDeviceMapsEnabled, // q: ULONG
    ProcessBreakOnTermination, // qs: ULONG
    ProcessDebugObjectHandle, // q: HANDLE // 30
    ProcessDebugFlags, // qs: ULONG
    ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
    ProcessIoPriority, // qs: IO_PRIORITY_HINT
    ProcessExecuteFlags, // qs: ULONG
    ProcessResourceManagement, // ProcessTlsInformation // PROCESS_TLS_INFORMATION
    ProcessCookie, // q: ULONG
    ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION
    ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
    ProcessPagePriority, // q: PAGE_PRIORITY_INFORMATION
    ProcessInstrumentationCallback, // qs: PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION // 40
    ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
    ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]
    ProcessImageFileNameWin32, // q: UNICODE_STRING
    ProcessImageFileMapping, // q: HANDLE (input)
    ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE
    ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE
    ProcessGroupInformation, // q: USHORT[]
    ProcessTokenVirtualizationEnabled, // s: ULONG
    ProcessConsoleHostProcess, // q: ULONG_PTR // ProcessOwnerInformation
    ProcessWindowInformation, // q: PROCESS_WINDOW_INFORMATION // 50
    ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
    ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION
    ProcessDynamicFunctionTableInformation,
    ProcessHandleCheckingMode, // qs: ULONG; s: 0 disables, otherwise enables
    ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
    ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
    ProcessWorkingSetControl, // s: PROCESS_WORKING_SET_CONTROL
    ProcessHandleTable, // q: ULONG[] // since WINBLUE
    ProcessCheckStackExtentsMode,
    ProcessCommandLineInformation, // q: UNICODE_STRING // 60
    ProcessProtectionInformation, // q: PS_PROTECTION
    ProcessMemoryExhaustion, // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
    ProcessFaultInformation, // PROCESS_FAULT_INFORMATION
    ProcessTelemetryIdInformation, // PROCESS_TELEMETRY_ID_INFORMATION
    ProcessCommitReleaseInformation, // PROCESS_COMMIT_RELEASE_INFORMATION
    ProcessDefaultCpuSetsInformation,
    ProcessAllowedCpuSetsInformation,
    ProcessSubsystemProcess,
    ProcessJobMemoryInformation, // PROCESS_JOB_MEMORY_INFO
    ProcessInPrivate, // since THRESHOLD2 // 70
    ProcessRaiseUMExceptionOnInvalidHandleClose, // qs: ULONG; s: 0 disables, otherwise enables
    ProcessIumChallengeResponse,
    ProcessChildProcessInformation, // PROCESS_CHILD_PROCESS_INFORMATION
    ProcessHighGraphicsPriorityInformation,
    ProcessSubsystemInformation, // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
    ProcessEnergyValues, // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
    ProcessActivityThrottleState, // PROCESS_ACTIVITY_THROTTLE_STATE
    ProcessActivityThrottlePolicy, // PROCESS_ACTIVITY_THROTTLE_POLICY
    ProcessWin32kSyscallFilterInformation,
    ProcessDisableSystemAllowedCpuSets, // 80
    ProcessWakeInformation, // PROCESS_WAKE_INFORMATION
    ProcessEnergyTrackingState, // PROCESS_ENERGY_TRACKING_STATE
    ProcessManageWritesToExecutableMemory, // MANAGE_WRITES_TO_EXECUTABLE_MEMORY // since REDSTONE3
    ProcessCaptureTrustletLiveDump,
    ProcessTelemetryCoverage,
    ProcessEnclaveInformation,
    ProcessEnableReadWriteVmLogging, // PROCESS_READWRITEVM_LOGGING_INFORMATION
    ProcessUptimeInformation, // PROCESS_UPTIME_INFORMATION
    ProcessImageSection, // q: HANDLE
    ProcessDebugAuthInformation, // since REDSTONE4 // 90
    ProcessSystemResourceManagement, // PROCESS_SYSTEM_RESOURCE_MANAGEMENT
    ProcessSequenceNumber, // q: ULONGLONG
    ProcessLoaderDetour, // since REDSTONE5
    ProcessSecurityDomainInformation, // PROCESS_SECURITY_DOMAIN_INFORMATION
    ProcessCombineSecurityDomainsInformation, // PROCESS_COMBINE_SECURITY_DOMAINS_INFORMATION
    ProcessEnableLogging, // PROCESS_LOGGING_INFORMATION
    ProcessLeapSecondInformation, // PROCESS_LEAP_SECOND_INFORMATION
    ProcessFiberShadowStackAllocation, // PROCESS_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION // since 19H1
    ProcessFreeFiberShadowStackAllocation, // PROCESS_FREE_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
    MaxProcessInfoClass
} PROCESSINFOCLASS;

extern "C"
NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryInformationProcess(
    _In_ HANDLE ProcessHandle,
    _In_ PROCESSINFOCLASS ProcessInformationClass,
    _Out_writes_bytes_(ProcessInformationLength) PVOID ProcessInformation,
    _In_ ULONG ProcessInformationLength,
    _Out_opt_ PULONG ReturnLength
);

bool GetProcessPeb(HANDLE hProcess, PPEB peb);

HANDLE _get_console_handle(borrowed_fd fd, DWORD* mode = nullptr);

static inline auto& g_environ_utf8 = *new std::unordered_map<std::string, char*>();

static inline bool adb_is_separator(char c) {
	return c == '\\' || c == '/';
}

extern int adb_thread_setname(const std::string& name);

static inline void close_on_exec(borrowed_fd fd) {
	/* nothing really */
}

extern int adb_unlink(const char* path);
#undef unlink
#define unlink ___xxx_unlink

extern int adb_mkdir(const std::string& path, int mode);
#undef mkdir
#define mkdir ___xxx_mkdir

extern int adb_rename(const char* oldpath, const char* newpath);

// See the comments for the !defined(_WIN32) versions of adb_*().
extern int adb_open(const char* path, int options);
extern int adb_creat(const char* path, int mode);
extern int adb_read(borrowed_fd fd, void* buf, int len);
extern int adb_pread(borrowed_fd fd, void* buf, int len, off64_t offset);
extern int adb_write(borrowed_fd fd, const void* buf, int len);
extern int adb_pwrite(borrowed_fd fd, const void* buf, int len, off64_t offset);
extern int64_t adb_lseek(borrowed_fd fd, int64_t pos, int where);
extern int adb_shutdown(borrowed_fd fd, int direction = SD_BOTH); // shutdown both send and receive by default
extern int adb_close(int fd);
extern int adb_register_socket(SOCKET s);
extern HANDLE adb_get_os_handle(borrowed_fd fd);

extern int adb_gethostname(char* name, size_t len);
extern int adb_getlogin_r(char* buf, size_t bufsize);

// See the comments for the !defined(_WIN32) version of unix_close().
static inline int unix_close(int fd) {
	return _close(fd);
}
#undef close
#define close ____xxx_close

// Like unix_read(), but may return EINTR.
extern int unix_read_interruptible(borrowed_fd fd, void* buf, size_t len);

// See the comments for the !defined(_WIN32) version of unix_read().
static inline int unix_read(borrowed_fd fd, void* buf, size_t len) {
	return TEMP_FAILURE_RETRY(unix_read_interruptible, fd, buf, len);
}

#undef   read
#define  read  ___xxx_read

#undef pread
#define pread ___xxx_pread

// See the comments for the !defined(_WIN32) version of unix_write().
static inline int unix_write(borrowed_fd fd, const void* buf, size_t len) {
	return _write(fd.get(), buf, len);
}
#undef   write
#define  write  ___xxx_write

#undef pwrite
#define pwrite ___xxx_pwrite

// See the comments for the !defined(_WIN32) version of unix_lseek().
static inline int unix_lseek(borrowed_fd fd, int pos, int where) {
	return _lseek(fd.get(), pos, where);
}
#undef lseek
#define lseek ___xxx_lseek

// See the comments for the !defined(_WIN32) version of adb_open_mode().
static inline int adb_open_mode(const char* path, int options, int mode) {
	return adb_open(path, options);
}

// See the comments for the !defined(_WIN32) version of unix_open().
extern int unix_open(std::string_view path, int options, ...);
#define  open    ___xxx_unix_open

// Checks if |fd| corresponds to a console.
// Standard Windows isatty() returns 1 for both console FDs and character
// devices like NUL. unix_isatty() performs some extra checking to only match
// console FDs.
// |fd| must be a real file descriptor, meaning STDxx_FILENO or unix_open() FDs
// will work but adb_open() FDs will not. Additionally the OS handle associated
// with |fd| must have GENERIC_READ access (which console FDs have by default).
// Returns 1 if |fd| is a console FD, 0 otherwise. The value of errno after
// calling this function is unreliable and should not be used.
int unix_isatty(borrowed_fd fd);
#define  isatty  ___xxx_isatty

int network_inaddr_any_server(int port, int type, std::string* error);

inline int network_local_client(const char* name, int namespace_id, int type, std::string* error) {
	abort();
}

inline int network_local_server(const char* name, int namespace_id, int type, std::string* error) {
	abort();
}

int network_connect(const std::string& host, int port, int type, int timeout,
	std::string* error);

std::optional<size_t> network_peek(borrowed_fd fd);

extern int adb_socket_accept(borrowed_fd serverfd, struct sockaddr* addr, socklen_t* addrlen);



int adb_getsockname(borrowed_fd fd, struct sockaddr* sockaddr, socklen_t* addrlen);

// Returns the local port number of a bound socket, or -1 on failure.
int adb_socket_get_local_port(borrowed_fd fd);

extern int adb_setsockopt(borrowed_fd fd, int level, int optname, const void* optval,
	socklen_t optlen);



// Wrapper around socket() call. On Windows, ADB has an indirection layer for file descriptors.
extern int adb_socket(int domain, int type, int protocol);

// Wrapper around bind() call, as Windows has indirection layer.
extern int adb_bind(borrowed_fd fd, const sockaddr* addr, int namelen);

extern int adb_socketpair(int sv[2]);

// Posix compatibility layer for msghdr
struct adb_msghdr {
	void* msg_name;
	socklen_t msg_namelen;
	struct adb_iovec* msg_iov;
	size_t msg_iovlen;
	void* msg_control;
	size_t msg_controllen;
	int msg_flags;
};

size_t adb_sendmsg(borrowed_fd fd, const adb_msghdr* msg, int flags);
size_t adb_recvmsg(borrowed_fd fd, adb_msghdr* msg, int flags);

using adb_cmsghdr = WSACMSGHDR;

extern adb_cmsghdr* adb_CMSG_FIRSTHDR(adb_msghdr* msgh);
extern adb_cmsghdr* adb_CMSG_NXTHDR(adb_msghdr* msgh, adb_cmsghdr* cmsg);
extern unsigned char* adb_CMSG_DATA(adb_cmsghdr* cmsg);

struct adb_pollfd {
	int fd;
	short events;
	short revents;
};
extern int adb_poll(adb_pollfd* fds, size_t nfds, int timeout);
#define poll ___xxx_poll

static inline int adb_is_absolute_host_path(const char* path) {
	return isalpha(path[0]) && path[1] == ':' && path[2] == '\\';
}

// UTF-8 versions of POSIX APIs.
extern DIR* adb_opendir(const char* dirname);
extern struct dirent* adb_readdir(DIR* dir);
extern int adb_closedir(DIR* dir);

extern int adb_utime(const char*, struct utimbuf*);
extern int adb_chmod(const char*, int);

extern int adb_vfprintf(FILE* stream, const char* format, va_list ap);
extern int adb_vprintf(const char* format, va_list ap);
extern int adb_fprintf(FILE* stream, const char* format, ...);
extern int adb_printf(const char* format, ...);
extern int adb_fputs(const char* buf, FILE* stream);
extern int adb_fputc(int ch, FILE* stream);
extern int adb_putchar(int ch);
extern int adb_puts(const char* buf);
extern size_t adb_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

extern FILE* adb_fopen(const char* f, const char* m);

extern char* adb_getenv(const char* name);

extern char* adb_getcwd(char* buf, int size);

// Remap calls to POSIX APIs to our UTF-8 versions.
#define opendir adb_opendir
#define readdir adb_readdir
#define closedir adb_closedir
#define rewinddir rewinddir_utf8_not_yet_implemented
#define telldir telldir_utf8_not_yet_implemented
// Some compiler's C++ headers have members named seekdir, so we can't do the
// macro technique and instead cause a link error if seekdir is called.
inline void seekdir(DIR*, long) {
	extern int seekdir_utf8_not_yet_implemented;
	seekdir_utf8_not_yet_implemented = 1;
}

#define chmod adb_chmod

#define vfprintf adb_vfprintf
#define vprintf adb_vprintf
#define fprintf adb_fprintf
#define printf adb_printf
#define fputs adb_fputs
#define fputc adb_fputc
// putc may be a macro, so if so, undefine it, so that we can redefine it.
#undef putc
#define putc(c, s) adb_fputc(c, s)
#define putchar adb_putchar
#define puts adb_puts
#define fwrite adb_fwrite

#define fopen adb_fopen
#define freopen freopen_utf8_not_yet_implemented

#define getenv adb_getenv
#define putenv putenv_utf8_not_yet_implemented
#define setenv setenv_utf8_not_yet_implemented
#define unsetenv unsetenv_utf8_not_yet_implemented

#define getcwd adb_getcwd

// A very simple wrapper over a launched child process
class Process {
public:
	constexpr explicit Process(HANDLE h = nullptr) : h_(h) {}
	constexpr Process(Process&& other) : h_(std::exchange(other.h_, nullptr)) {}
	~Process() { close(); }
	constexpr explicit operator bool() const { return h_ != nullptr; }

	void wait() {
		if (*this) {
			::WaitForSingleObject(h_, INFINITE);
			close();
		}
	}
	void kill() {
		if (*this) {
			::TerminateProcess(h_, -1);
		}
	}

private:
	DISALLOW_COPY_AND_ASSIGN(Process);

	void close() {
		if (*this) {
			::CloseHandle(h_);
			h_ = nullptr;
		}
	}

	HANDLE h_;
};

Process adb_launch_process(std::string_view executable, std::vector<std::string> args,
	std::initializer_list<int> fds_to_inherit = {});

// Helper class to convert UTF-16 argv from wmain() to UTF-8 args that can be
// passed to main().
class NarrowArgs {
public:
	NarrowArgs(int argc, wchar_t** argv);
	~NarrowArgs();

	inline char** data() {
		return narrow_args;
	}

private:
	char** narrow_args;
};

// Windows HANDLE values only use 32-bits of the type, even on 64-bit machines,
// so they can fit in an int. To convert back, we just need to sign-extend.
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa384203%28v=vs.85%29.aspx
// Note that this does not make a HANDLE value work with APIs like open(), nor
// does this make a value from open() passable to APIs taking a HANDLE. This
// just lets you take a HANDLE, pass it around as an int, and then use it again
// as a HANDLE.
inline int cast_handle_to_int(const HANDLE h) {
	// truncate
	return static_cast<int>(reinterpret_cast<INT_PTR>(h));
}

inline HANDLE cast_int_to_handle(const int fd) {
	// sign-extend
	return reinterpret_cast<HANDLE>(static_cast<INT_PTR>(fd));
}

// Deleter for unique_handle. Adapted from many sources, including:
// http://stackoverflow.com/questions/14841396/stdunique-ptr-deleters-and-the-win32-api
// https://visualstudiomagazine.com/articles/2013/09/01/get-a-handle-on-the-windows-api.aspx
class handle_deleter {
public:
	typedef HANDLE pointer;

	void operator()(HANDLE h);
};

// Like std::unique_ptr, but for Windows HANDLE objects that should be
// CloseHandle()'d. Operator bool() only checks if the handle != nullptr,
// but does not check if the handle != INVALID_HANDLE_VALUE.
typedef std::unique_ptr<HANDLE, handle_deleter> unique_handle;

namespace internal {

	size_t ParseCompleteUTF8(const char* first, const char* last, std::vector<char>* remaining_bytes);

}


static inline void disable_tcp_nagle(borrowed_fd fd) {
	int off = 1;
	adb_setsockopt(fd.get(), IPPROTO_TCP, TCP_NODELAY, &off, sizeof(off));
}

// Sets TCP socket |fd| to send a keepalive TCP message every |interval_sec| seconds. Set
// |interval_sec| to 0 to disable keepalives. If keepalives are enabled, the connection will be
// configured to drop after 10 missed keepalives. Returns true on success.
bool set_tcp_keepalive(borrowed_fd fd, int interval_sec);

// Returns a human-readable OS version string.
extern std::string GetOSVersion(void);

#if defined(_WIN32)
// Win32 defines ERROR, which we don't need, but which conflicts with google3 logging.
#undef ERROR
#endif