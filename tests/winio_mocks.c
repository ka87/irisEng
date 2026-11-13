#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "winio_mocks.h"
#include "../dbg.h"
#include "../time.h"

WinioScript winioScript;

static char logLines[WINIO_LOGMAX][WINIO_LINEMAX];
static int logCount;
static int mouseStateCalls;
static int keybStateCalls;

void WINIO_Log(const char* fmt, ...)
{
    va_list ap;
    if (logCount >= WINIO_LOGMAX)
    {
        logCount++;
        return;
    }
    va_start(ap, fmt);
    vsnprintf(logLines[logCount], WINIO_LINEMAX, fmt, ap);
    va_end(ap);
    logCount++;
}

int WINIO_LogCount(void) { return logCount; }
const char* WINIO_LogLine(int i)
{
    return (i >= 0 && i < logCount && i < WINIO_LOGMAX) ? logLines[i] : "<overflow>";
}

/* ------------------------------------------------------------------ */
/* DirectInput                                                        */
/* ------------------------------------------------------------------ */

typedef struct { IDirectInputDevice8AVtbl* lpVtbl; } MockDev;
static MockDev mouseDev, keybDev;
static MockDev* dinputObj;

IDirectInputDevice8A* WINIO_MouseDevice;
IDirectInputDevice8A* WINIO_KeybDevice;
IDirectInput8A* WINIO_DIObject;

static const char* devName(IDirectInputDevice8A* d)
{
    if (d == (IDirectInputDevice8A*)&mouseDev) return "mouse";
    if (d == (IDirectInputDevice8A*)&keybDev) return "keyb";
    return "?";
}

static HRESULT devRelease(IDirectInputDevice8A* d)
{
    WINIO_Log("Release(%s)", devName(d));
    return 0;
}

static HRESULT devGetProperty(IDirectInputDevice8A* d, const GUID* guid, LPDIPROPHEADER h)
{
    DIPROPDWORD* p = (DIPROPDWORD*)h;
    WINIO_Log("GetProperty(%s,obj=%u,how=%u,guid=%ld)", devName(d),
              (unsigned)h->dwObj, (unsigned)h->dwHow, (long)(size_t)guid);
    p->dwData = winioScript.getPropertyData;
    return winioScript.getPropertyResult;
}

static HRESULT devAcquire(IDirectInputDevice8A* d)
{
    WINIO_Log("Acquire(%s)", devName(d));
    return d == (IDirectInputDevice8A*)&mouseDev ? winioScript.mouseAcquireResult
                                                 : winioScript.keybAcquireResult;
}

static HRESULT devUnacquire(IDirectInputDevice8A* d)
{
    WINIO_Log("Unacquire(%s)", devName(d));
    return 0;
}

static HRESULT devGetDeviceState(IDirectInputDevice8A* d, DWORD size, void* buf)
{
    int n;
    WINIO_Log("GetDeviceState(%s,%u)", devName(d), (unsigned)size);
    if (d == (IDirectInputDevice8A*)&mouseDev)
    {
        n = mouseStateCalls < 1 ? 0 : 1;
        mouseStateCalls++;
        if (winioScript.mouseStateResult[n] >= 0)
            memcpy(buf, winioScript.mouseData,
                   size < sizeof(winioScript.mouseData) ? size : sizeof(winioScript.mouseData));
        return winioScript.mouseStateResult[n];
    }
    n = keybStateCalls < 1 ? 0 : 1;
    keybStateCalls++;
    if (winioScript.keybStateResult[n] >= 0)
        memcpy(buf, winioScript.keybData,
               size < sizeof(winioScript.keybData) ? size : sizeof(winioScript.keybData));
    return winioScript.keybStateResult[n];
}

static HRESULT devSetDataFormat(IDirectInputDevice8A* d, const DIDATAFORMAT* f)
{
    WINIO_Log("SetDataFormat(%s)", devName(d));
    return d == (IDirectInputDevice8A*)&mouseDev ? winioScript.mouseSetFormat
                                                 : winioScript.keybSetFormat;
}

static HRESULT devSetCoop(IDirectInputDevice8A* d, HWND w, DWORD flags)
{
    WINIO_Log("SetCooperativeLevel(%s,%u)", devName(d), (unsigned)flags);
    return d == (IDirectInputDevice8A*)&mouseDev ? winioScript.mouseSetCoop
                                                 : winioScript.keybSetCoop;
}

static HRESULT devGetObjectInfo(IDirectInputDevice8A* d, DIDEVICEOBJECTINSTANCEA* inst,
                                DWORD obj, DWORD how)
{
    WINIO_Log("GetObjectInfo(%u,%u)", (unsigned)obj, (unsigned)how);
    /* a name that depends on the offset, so a reordered walk shows up */
    snprintf(inst->tszName, sizeof(inst->tszName), "%s%u", winioScript.objectName,
             (unsigned)obj);
    return 0;
}

static HRESULT devGetCaps(IDirectInputDevice8A* d, DIDEVCAPS* c)
{
    WINIO_Log("GetCapabilities(%s)", devName(d));
    return 0;
}

static IDirectInputDevice8AVtbl devVtbl = {
    devRelease, devGetProperty, devAcquire, devUnacquire, devGetDeviceState,
    devSetDataFormat, devSetCoop, devGetObjectInfo, devGetCaps
};

typedef struct { IDirectInput8AVtbl* lpVtbl; } MockDI;
static MockDI diObj;

static HRESULT diRelease(IDirectInput8A* p)
{
    WINIO_Log("DI_Release");
    return 0;
}

static HRESULT diCreateDevice(IDirectInput8A* p, const GUID* guid,
                              IDirectInputDevice8A** out, void* unk)
{
    int isMouse = (guid == &GUID_SysMouse);
    WINIO_Log("CreateDevice(%s)", isMouse ? "mouse" : "keyb");
    /* the SDK leaves *out alone when it fails, and the engine relies
       on that: a failed CreateDevice leaves the previous pointer */
    if (isMouse)
    {
        if (winioScript.createMouseResult >= 0)
            *out = (IDirectInputDevice8A*)&mouseDev;
        return winioScript.createMouseResult;
    }
    if (winioScript.createKeybResult >= 0)
        *out = (IDirectInputDevice8A*)&keybDev;
    return winioScript.createKeybResult;
}

static IDirectInput8AVtbl diVtbl = { diRelease, diCreateDevice };

const GUID IID_IDirectInput8A = { 1, 0, 0, { 0 } };
const GUID GUID_SysMouse = { 2, 0, 0, { 0 } };
const GUID GUID_SysKeyboard = { 3, 0, 0, { 0 } };
const DIDATAFORMAT c_dfDIMouse2 = { 1 };
const DIDATAFORMAT c_dfDIKeyboard = { 2 };
const DIDATAFORMAT c_dfDIJoystick2 = { 3 };

HRESULT DirectInput8Create(HINSTANCE inst, DWORD ver, const GUID* iid, void** out, void* unk)
{
    WINIO_Log("DirectInput8Create(ver=%u)", (unsigned)ver);
    *out = &diObj;
    return winioScript.diCreateResult;
}

/* ------------------------------------------------------------------ */
/* Win32                                                              */
/* ------------------------------------------------------------------ */

HMODULE GetModuleHandleA(LPCSTR name)
{
    WINIO_Log("GetModuleHandleA");
    return (HMODULE)0x1234;
}

BOOL SwapMouseButton(BOOL swap)
{
    WINIO_Log("SwapMouseButton(%d)", swap);
    return winioScript.swapMouseButton;
}

SHORT GetKeyState(int vk)
{
    WINIO_Log("GetKeyState(%d)", vk);
    return winioScript.keyState;
}

void keybd_event(BYTE vk, BYTE scan, DWORD flags, UINT_PTR extra)
{
    WINIO_Log("keybd_event(%u,%u)", (unsigned)vk, (unsigned)flags);
}

BOOL GetDiskFreeSpaceExA(LPCSTR dir, ULARGE_INTEGER* freeToCaller,
                         ULARGE_INTEGER* total, ULARGE_INTEGER* free)
{
    WINIO_Log("GetDiskFreeSpaceExA");
    freeToCaller->LowPart = winioScript.diskFreeLow;
    freeToCaller->HighPart = winioScript.diskFreeHigh;
    return winioScript.diskFreeOk;
}

void* WINIO_DlgProc;

INT_PTR DialogBoxIndirectParamA(HINSTANCE inst, LPCDLGTEMPLATEA tmpl, HWND parent,
                                void* proc, long param)
{
    typedef INT_PTR (*DlgProc)(HWND, UINT, WPARAM, LPARAM);
    DlgProc p = (DlgProc)WINIO_DlgProc;

    WINIO_Log("DialogBoxIndirectParamA");
    if (winioScript.dialogDrivesProc && p)
    {
        WINIO_Log("dlg->%ld", (long)p((HWND)0x77, 272, (WPARAM)0x88, 0));
        WINIO_Log("dlg->%ld", (long)p((HWND)0x77, 273, 1, 0));
        WINIO_Log("dlg->%ld", (long)p((HWND)0x77, 273, 2, 0));
        WINIO_Log("dlg->%ld", (long)p((HWND)0x77, 273, 9, 0));
        WINIO_Log("dlg->%ld", (long)p((HWND)0x77, 999, 0, 0));
    }
    strncpy(dbgDialogBuff, winioScript.dialogText, sizeof(dbgDialogBuff) - 1);
    dbgDialogBuff[sizeof(dbgDialogBuff) - 1] = 0;
    return winioScript.dialogResult;
}

HWND SetFocus(HWND w)
{
    WINIO_Log("SetFocus(%ld)", (long)(size_t)w);
    return w;
}

BOOL SetWindowTextA(HWND w, LPCSTR s)
{
    WINIO_Log("SetWindowTextA(%ld,%s)", (long)(size_t)w, s);
    return 1;
}

LRESULT SendMessageA(HWND w, UINT msg, WPARAM wp, LPARAM lp)
{
    WINIO_Log("SendMessageA(%ld,%u,%ld)", (long)(size_t)w, msg, (long)lp);
    return 0;
}

UINT GetDlgItemTextA(HWND dlg, int id, LPSTR buf, int cch)
{
    WINIO_Log("GetDlgItemTextA(%d,%d)", id, cch);
    strncpy(buf, winioScript.dialogText, cch - 1);
    buf[cch - 1] = 0;
    return (UINT)strlen(buf);
}

BOOL EndDialog(HWND dlg, INT_PTR result)
{
    WINIO_Log("EndDialog(%ld)", (long)result);
    return 1;
}

double _hypot(double x, double y) { return hypot(x, y); }

/* ------------------------------------------------------------------ */
/* stdio, redirected for both copies                                   */
/* ------------------------------------------------------------------ */

static int fakeFile;

FILE* WINIO_fopen(const char* name, const char* mode)
{
    int ok = (mode[0] == 'r') ? winioScript.fopenReadWriteOk : winioScript.fopenCreateOk;
    WINIO_Log("fopen(%s,%s)->%d", name, mode, ok);
    return ok ? (FILE*)&fakeFile : NULL;
}

int WINIO_fclose(FILE* f)
{
    WINIO_Log("fclose");
    return winioScript.fcloseResult;
}

size_t WINIO_fread(void* buf, size_t size, size_t count, FILE* f)
{
    size_t i;
    WINIO_Log("fread(%u,%u)", (unsigned)size, (unsigned)count);
    for (i = 0; i < count; i++)
        ((unsigned char*)buf)[i] = (unsigned char)(0xA0 + i);
    return (size_t)winioScript.freadResult;
}

size_t WINIO_fwrite(const void* buf, size_t size, size_t count, FILE* f)
{
    unsigned sum = 0;
    size_t i;
    for (i = 0; i < count; i++)
        sum = sum * 31u + ((const unsigned char*)buf)[i];
    WINIO_Log("fwrite(%u,%u,%u)", (unsigned)size, (unsigned)count, sum);
    return (size_t)winioScript.fwriteResult;
}

/* ------------------------------------------------------------------ */
/* engine                                                             */
/* ------------------------------------------------------------------ */

void DBG_OutputDebugString(char* format, ...)
{
    WINIO_Log("DBG:%s", format);
}

int INO_ge_SavSetNextState;
int INO_ge_SavCommand;

float WINIO_tweakFloat;
int32_t WINIO_tweakIntTarget;

void WINIO_TweakCallback(float value)
{
    WINIO_Log("tweakCall(%.6f)", value);
}

struct sTweakElem tweaks[17] = {
    { "aaa_int",   0, &WINIO_tweakIntTarget },
    { "bbb_float", 1, &WINIO_tweakFloat },
    { "ccc_call",  2, (void*)WINIO_TweakCallback },
    { "ddd_none",  3, &WINIO_tweakIntTarget },
    { "eee_int",   0, &WINIO_tweakIntTarget },
    { "fff_float", 1, &WINIO_tweakFloat },
    { "ggg_call",  2, (void*)WINIO_TweakCallback },
    { "hhh_none",  7, &WINIO_tweakIntTarget },
    { "iii_int",   0, &WINIO_tweakIntTarget },
    { "jjj_float", 1, &WINIO_tweakFloat },
    { "kkk_call",  2, (void*)WINIO_TweakCallback },
    { "lll_none",  4, &WINIO_tweakIntTarget },
    { "mmm_int",   0, &WINIO_tweakIntTarget },
    { "nnn_float", 1, &WINIO_tweakFloat },
    { "ooo_call",  2, (void*)WINIO_TweakCallback },
    { "ppp_none",  5, &WINIO_tweakIntTarget },
    { "qqq_int",   0, &WINIO_tweakIntTarget }
};

void WINIO_ResetMocks(void)
{
    logCount = 0;
    mouseStateCalls = 0;
    keybStateCalls = 0;
    WINIO_MouseDevice = (IDirectInputDevice8A*)&mouseDev;
    WINIO_KeybDevice = (IDirectInputDevice8A*)&keybDev;
    mouseDev.lpVtbl = &devVtbl;
    keybDev.lpVtbl = &devVtbl;
    diObj.lpVtbl = &diVtbl;
    dinputObj = &diObj;
    WINIO_DIObject = (IDirectInput8A*)&diObj;
}
