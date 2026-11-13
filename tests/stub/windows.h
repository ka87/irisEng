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

/* GDI and the system-colour calls the profiler window draws with. */
HMODULE  GetModuleHandleA(LPCSTR);
HDC      GetDC(HWND);
int      ReleaseDC(HWND, HDC);
DWORD    GetSysColor(int);
COLORREF SetBkColor(HDC, COLORREF);
COLORREF SetTextColor(HDC, COLORREF);
HBRUSH   CreateSolidBrush(COLORREF);
BOOL     ExtTextOutA(HDC, int, int, UINT, const RECT*, LPCSTR, UINT, const int*);
int      DrawTextA(HDC, LPCSTR, int, RECT*, UINT);

/* ---- messages, keys and window/queue flags for the win/ directory.
   Distinct values, not the SDK's; see tests/stub/d3d9.h. */
/* QS */
#define QS_HOTKEY                          0x00000001
#define QS_KEY                             0x00000002
#define QS_MOUSE                           0x00000004
#define QS_PAINT                           0x00000008
#define QS_POSTMESSAGE                     0x00000010
#define QS_SENDMESSAGE                     0x00000020
#define QS_TIMER                           0x00000040
/* WS */
#define WS_BORDER                          0x00000001
#define WS_DLGFRAME                        0x00000002
#define WS_MAXIMIZEBOX                     0x00000004
#define WS_MINIMIZEBOX                     0x00000008
#define WS_SYSMENU                         0x00000010
#define WS_THICKFRAME                      0x00000020
/* MB */
#define MB_ICONERROR                       0x00000001
#define MB_TASKMODAL                       0x00000002
/* MWMO */
#define MWMO_ALERTABLE                     0x00000001
#define MWMO_INPUTAVAILABLE                0x00000002
/* PM */
#define PM_REMOVE                          0x00000001
/* WM */
#define WM_ACTIVATEAPP                     16385
#define WM_CLOSE                           16386
#define WM_CREATE                          16387
#define WM_DESTROY                         16388
#define WM_ERASEBKGND                      16389
#define WM_MOUSEFIRST                      16390
#define WM_NCLBUTTONDOWN                   16391
#define WM_QUIT                            16392
#define WM_SYSCOMMAND                      16393
/* VK */
#define VK_F1                              16394
#define VK_F2                              16395
#define VK_F3                              16396
#define VK_F4                              16397
#define VK_F5                              16398
#define VK_F6                              16399
#define VK_F7                              16400
#define VK_F10                             16401
/* SC */
#define SC_CLOSE                           16402
/* SW */
#define SW_SHOW                            16403
/* MISC */
#define NORMAL_PRIORITY_CLASS              16404
#define E_FAIL                             16405
#define E_INVALIDARG                       16406

/* Dialogs, focus, the keyboard and the mouse-button swap, for the
   win/ input path. Real prototypes, not invented ones. */
HWND    SetFocus(HWND hWnd);
BOOL    SetWindowTextA(HWND hWnd, LPCSTR lpString);
LRESULT SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
UINT    GetDlgItemTextA(HWND hDlg, int nIDDlgItem, LPSTR lpString, int cchMax);
BOOL    EndDialog(HWND hDlg, INT_PTR nResult);
SHORT   GetKeyState(int nVirtKey);
BOOL    SwapMouseButton(BOOL fSwap);
void    keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, UINT_PTR dwExtraInfo);
double  _hypot(double x, double y);

/* CreateProcessA and friends, for the win/ relauncher. */
typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess, hThread; DWORD dwProcessId, dwThreadId;
} PROCESS_INFORMATION;
typedef struct _STARTUPINFOA {
    DWORD cb; LPSTR lpReserved, lpDesktop, lpTitle;
    DWORD dwX, dwY, dwXSize, dwYSize, dwXCountChars, dwYCountChars;
    DWORD dwFillAttribute, dwFlags; WORD wShowWindow, cbReserved2;
    BYTE* lpReserved2; HANDLE hStdInput, hStdOutput, hStdError;
} STARTUPINFOA;
BOOL  CreateProcessA(LPCSTR, LPSTR, void*, void*, BOOL, DWORD, void*, LPCSTR,
                     STARTUPINFOA*, PROCESS_INFORMATION*);

/* The message loop and the clock the win/ directory uses. */
typedef struct tagMSG {
    HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam;
    DWORD time; POINT pt;
} MSG;
typedef struct _SYSTEMTIME {
    WORD wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
} SYSTEMTIME;
void  GetLocalTime(SYSTEMTIME*);
BOOL  PeekMessageA(MSG*, HWND, UINT, UINT, UINT);
BOOL  TranslateMessage(const MSG*);
LRESULT DispatchMessageA(const MSG*);
DWORD MsgWaitForMultipleObjects(DWORD, const HANDLE*, BOOL, DWORD, DWORD);
