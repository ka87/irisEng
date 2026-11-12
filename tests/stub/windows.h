#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef long                LONG;
typedef unsigned int        UINT;
typedef const char*         LPCSTR;
typedef char*               LPSTR;
typedef void*               LPVOID;
typedef void*               HANDLE;
typedef void*               HMODULE;
typedef void*               HINSTANCE;
typedef void*               HWND;
typedef void*               HDC;
typedef void*               HRESULT_PTR;
typedef long                HRESULT;
typedef struct tagRECT { LONG left, top, right, bottom; } RECT;
typedef struct tagPOINT { LONG x, y; } POINT;
typedef struct _FILETIME { DWORD dwLowDateTime, dwHighDateTime; } FILETIME;
typedef struct _CRITICAL_SECTION { void* p; } CRITICAL_SECTION;
typedef struct _LARGE_INTEGER_STUB { long long QuadPart; } LARGE_INTEGER;
typedef union _ULARGE_INTEGER_STUB { struct { DWORD LowPart; DWORD HighPart; }; unsigned long long QuadPart; } ULARGE_INTEGER;
typedef long INT_PTR;
typedef unsigned long UINT_PTR;
typedef struct DLGTEMPLATE { DWORD style, dwExtendedStyle; WORD cdit; short x, y, cx, cy; } DLGTEMPLATE;
typedef const DLGTEMPLATE* LPCDLGTEMPLATEA;
int QueryPerformanceCounter(LARGE_INTEGER* out);
BOOL GetDiskFreeSpaceExA(LPCSTR, ULARGE_INTEGER*, ULARGE_INTEGER*, ULARGE_INTEGER*);
INT_PTR DialogBoxIndirectParamA(HINSTANCE, LPCDLGTEMPLATEA, HWND, void*, long);
#define TRUE  1
#define FALSE 0
#define WINAPI
#define CALLBACK
#define FAILED(hr) ((HRESULT)(hr) < 0)
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define __int8  char
#define __int16 short
#define __int32 int
#define __int64 long long
typedef unsigned char       byte;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned char       uchar;
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define __forceinline inline
typedef char                CHAR;
typedef unsigned char       UCHAR;
typedef short               SHORT;
typedef unsigned short      USHORT;
typedef unsigned long       ULONG;
typedef unsigned long*      LPDWORD;
typedef unsigned char*      LPBYTE;
typedef void*               HKEY;
typedef HKEY*               PHKEY;
typedef void*               HBRUSH;
typedef void*               HICON;
typedef void*               HCURSOR;
typedef void*               HMENU;
#define ERROR_SUCCESS            0
#define HKEY_CLASSES_ROOT        ((HKEY)0x80000000)
#define HKEY_CURRENT_USER        ((HKEY)0x80000001)
#define HKEY_LOCAL_MACHINE       ((HKEY)0x80000002)
#define KEY_READ                 0x20019
#define KEY_QUERY_VALUE          0x0001
#define REG_SZ                   1
#define REG_DWORD                4
#define INVALID_HANDLE_VALUE     ((HANDLE)(long)-1)
#define INVALID_FILE_ATTRIBUTES  ((DWORD)-1)
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_NORMAL    0x00000080
#define MAX_FIND_PATH            260

typedef void*               LPARAM_PTR;
typedef long                LPARAM;
typedef unsigned int        WPARAM;
typedef long                LRESULT;
typedef void*               HFONT;
typedef void*               HBITMAP;
typedef void*               HMENU;
typedef void*               HICON;
typedef void*               HCURSOR;
typedef void*               HBRUSH;
#include <limits.h>
#include <float.h>
#define MAX_PATH 260
typedef DWORD COLORREF;
typedef unsigned int SIZE_T;
typedef struct _GUID { DWORD Data1; WORD Data2, Data3; uint8_t Data4[8]; } GUID;
#define DEFINE_GUID(n,a,b,c,d,e,f,g,h,i,j,k) extern const GUID n
#define ERROR_ALREADY_EXISTS 183
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_ESCAPE 0x1B
#define VK_RETURN 0x0D
#define VK_SPACE 0x20
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28

/* The Windows API, the MSVC built-ins and the CRT spellings the engine
   names. Declarations only: the differential tests mock what they need.
   Enough of these that -Werror=implicit-function-declaration holds over
   the whole corpus; see docs/cleanup/README.md. */
#ifndef LOBYTE
#define LOBYTE(w)  ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)  ((BYTE)((((DWORD)(w)) >> 8) & 0xff))
#define LOWORD(l)  ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)  ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#endif
#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#endif

int      MessageBoxA(HWND hwnd, LPCSTR text, LPCSTR caption, UINT type);
void     OutputDebugStringA(LPCSTR text);
BOOL     GetClientRect(HWND hwnd, RECT* rect);
BOOL     DestroyWindow(HWND hwnd);
DWORD    GetLastError(void);
HANDLE   CreateMutexA(void* attributes, BOOL initialOwner, LPCSTR name);
HANDLE   CreateEventA(void* attributes, BOOL manualReset, BOOL initialState, LPCSTR name);
BOOL     SetEvent(HANDLE event);
DWORD    WaitForSingleObject(HANDLE handle, DWORD milliseconds);
HANDLE   GetProcessHeap(void);
LPVOID   HeapAlloc(HANDLE heap, DWORD flags, size_t bytes);
BOOL     HeapFree(HANDLE heap, DWORD flags, LPVOID mem);
short    GetAsyncKeyState(int key);

int      _strcmpi(const char* a, const char* b);
int      _stricmp(const char* a, const char* b);
int      _vsnprintf(char* buf, size_t count, const char* format, va_list args);

unsigned long long __rdtsc(void);
DWORD    timeGetTime(void);
DWORD    timeBeginPeriod(UINT period);
DWORD    timeEndPeriod(UINT period);

typedef struct WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
    DWORD nFileSizeHigh, nFileSizeLow, dwReserved0, dwReserved1;
    CHAR cFileName[260];
    CHAR cAlternateFileName[14];
} WIN32_FIND_DATAA;

typedef struct WNDCLASSA {
    UINT style;
    void* lpfnWndProc;
    int cbClsExtra, cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCSTR lpszMenuName, lpszClassName;
} WNDCLASSA;

DWORD  GetFileAttributesA(LPCSTR);
HANDLE FindFirstFileA(LPCSTR, WIN32_FIND_DATAA*);
BOOL   FindNextFileA(HANDLE, WIN32_FIND_DATAA*);
BOOL   FindClose(HANDLE);
LONG   RegOpenKeyExA(HKEY, LPCSTR, DWORD, DWORD, PHKEY);
LONG   RegQueryValueExA(HKEY, LPCSTR, DWORD*, DWORD*, LPBYTE, DWORD*);
LONG   RegCloseKey(HKEY);
LONG   DefWindowProcA(HWND, UINT, WPARAM, LPARAM);
WORD   RegisterClassA(const WNDCLASSA*);
HWND   CreateWindowExA(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, void*);
BOOL   ShowWindow(HWND, int);
BOOL   UpdateWindow(HWND);
