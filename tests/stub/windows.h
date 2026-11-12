#pragma once
#include <stdint.h>
#include <stddef.h>
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
typedef struct IDirectSoundNotify { void* p; } IDirectSoundNotify;
typedef struct IDirectSound3DBuffer { void* p; } IDirectSound3DBuffer;
typedef struct IKsPropertySet { void* p; } IKsPropertySet;
typedef struct WAVEFORMATEX { void* p; } WAVEFORMATEX;
