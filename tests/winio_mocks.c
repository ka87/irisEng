#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "winio_mocks.h"
#include "sdlpath.h"
#include "../fileio.h"
#include "../dbg.h"
#include "../time.h"

/* Three of the mocks below answer for the platform layer, and the
 * frozen copy's matching device calls are written to log the same
 * line they do.
 *
 * The keyboard and the mouse are no longer two DirectInput devices,
 * so where the frozen copy calls GetDeviceState, Acquire and
 * GetObjectInfo on a device, the rewrite asks the platform layer
 * once. The event is the same event -- read the input, take it or
 * give it back, ask what a key is called -- so both sides log one
 * line for it in the same words, and the differential still compares
 * where in the sequence it happens and with what argument. What it
 * no longer compares is which API was used to do it, which is the
 * whole point of the change.
 *
 * Making the two devices and releasing them are the same event too:
 * the frozen copy took the input by creating a pair of exclusive
 * devices and gave it back by releasing them, which is what the
 * rewrite does in one call. So CreateDevice and Release speak the
 * same two lines.
 *
 * The frozen copy does all of it to the two devices as a pair, and
 * only the keyboard's half logs: one line an event is what the
 * rewrite has one of. Whether the frozen copy still does them as a
 * pair is a property of the frozen copy, and these cases are not
 * about that.
 *
 * Opening DirectInput and releasing it are silent for a different
 * reason: they are not an event the rewrite has at all. The object
 * existed only to make the two devices, so nothing opens it now and
 * there is nothing on the other side to line a line up with.
 *
 * The cases that were about the devices themselves -- input lost and
 * re-acquired, a device present or absent, opening DirectInput
 * failing -- are retired in test_winio.c, where it says so. */
WinioScript winioScript;

/* The keyboard and the mouse arrive through the platform layer now
   rather than through two devices of their own, so this is where the
   script is read instead. It is the same scripted data: the cases
   that drive the handlers go on driving them with it, and only the
   cases that were about a device being lost and taken again have
   gone, since there is no device to lose. */
void PLT_ReadDevices(unsigned char* keys, int* mouse)
{
    WINIO_Log("input read");
    if (keys)
        memcpy(keys, winioScript.keybData, sizeof(winioScript.keybData));
    if (mouse)
        memcpy(mouse, winioScript.mouseData, sizeof(winioScript.mouseData));
}

void PLT_InputCapture(int on)
{
    WINIO_Log("input capture(%d)", on);
}

/* The same name the keyboard device used to give for the same code,
   so that a walk that visits the codes in a different order still
   shows up in what gets built out of the names. */
const char* PLT_sz_KeyName(int dik)
{
    static char name[64];
    WINIO_Log("key name(%u)", (unsigned)dik);
    snprintf(name, sizeof(name), "%s%u", winioScript.objectName, (unsigned)dik);
    return name;
}

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
    if (d == (IDirectInputDevice8A*)&keybDev)
        WINIO_Log("input capture(0)");
    return 0;
}

static HRESULT devGetProperty(IDirectInputDevice8A* d, const GUID* guid, LPDIPROPHEADER h)
{
    DIPROPDWORD* p = (DIPROPDWORD*)h;
    if (d == (IDirectInputDevice8A*)&keybDev)
        WINIO_Log("GetProperty(obj=%u,how=%u,guid=%ld)",
                  (unsigned)h->dwObj, (unsigned)h->dwHow, (long)(size_t)guid);
    p->dwData = winioScript.getPropertyData;
    return winioScript.getPropertyResult;
}

static HRESULT devAcquire(IDirectInputDevice8A* d)
{
    if (d == (IDirectInputDevice8A*)&keybDev)
        WINIO_Log("input capture(1)");
    return d == (IDirectInputDevice8A*)&mouseDev ? winioScript.mouseAcquireResult
                                                 : winioScript.keybAcquireResult;
}

static HRESULT devUnacquire(IDirectInputDevice8A* d)
{
    if (d == (IDirectInputDevice8A*)&keybDev)
        WINIO_Log("input capture(0)");
    return 0;
}

static HRESULT devGetDeviceState(IDirectInputDevice8A* d, DWORD size, void* buf)
{
    int n;
    if (d == (IDirectInputDevice8A*)&keybDev)
        WINIO_Log("input read");
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
    return d == (IDirectInputDevice8A*)&mouseDev ? winioScript.mouseSetFormat
                                                 : winioScript.keybSetFormat;
}

static HRESULT devSetCoop(IDirectInputDevice8A* d, HWND w, DWORD flags)
{
    return d == (IDirectInputDevice8A*)&mouseDev ? winioScript.mouseSetCoop
                                                 : winioScript.keybSetCoop;
}

static HRESULT devGetObjectInfo(IDirectInputDevice8A* d, DIDEVICEOBJECTINSTANCEA* inst,
                                DWORD obj, DWORD how)
{
    WINIO_Log("key name(%u)", (unsigned)obj);
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
    return 0;
}

static HRESULT diCreateDevice(IDirectInput8A* p, const GUID* guid,
                              IDirectInputDevice8A** out, void* unk)
{
    int isMouse = (guid == &GUID_SysMouse);
    if (!isMouse)
        WINIO_Log("input capture(1)");
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
    *out = &diObj;
    return winioScript.diCreateResult;
}

/* ------------------------------------------------------------------ */
/* Win32                                                              */
/* ------------------------------------------------------------------ */

/* Silent, like the call it was an argument to: the frozen copy asks
   for the module handle only so that it can open DirectInput. */
HMODULE GetModuleHandleA(LPCSTR name)
{
    return (HMODULE)0x1234;
}

BOOL SwapMouseButton(BOOL swap)
{
    WINIO_Log("SwapMouseButton(%d)", swap);
    return winioScript.swapMouseButton;
}

/* The one site asks after num lock, and the rewrite asks the platform
   layer instead of the keyboard. Same event, so the same word in the
   log -- and the same answer, so the two copies come out of it with
   the same flag. */
SHORT GetKeyState(int vk)
{
    WINIO_Log("numlock");
    return winioScript.keyState;
}

int PLT_b_NumLockOn(void)
{
    WINIO_Log("numlock");
    return winioScript.keyState != 0;
}

void keybd_event(BYTE vk, BYTE scan, DWORD flags, UINT_PTR extra)
{
    WINIO_Log("keybd_event(%u,%u)", (unsigned)vk, (unsigned)flags);
}

/* How much room is left, asked of Windows by the frozen copy and of
   the file-system module by the rewrite. The script says what the
   answer is, in the two halves Windows answers in, and both sides
   hand back the same number. */
BOOL GetDiskFreeSpaceExA(LPCSTR dir, ULARGE_INTEGER* freeToCaller,
                         ULARGE_INTEGER* total, ULARGE_INTEGER* free)
{
    WINIO_Log("freespace");
    freeToCaller->LowPart = winioScript.diskFreeLow;
    freeToCaller->HighPart = winioScript.diskFreeHigh;
    return winioScript.diskFreeOk;
}

long long FIO_i64_FreeSpace(const char* dir)
{
    WINIO_Log("freespace");
    if (!winioScript.diskFreeOk)
        return -1;
    return (long long)(((unsigned long long)winioScript.diskFreeHigh << 32)
                       | (unsigned long long)winioScript.diskFreeLow);
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
    int         ok = (mode[0] == 'r') ? winioScript.fopenReadWriteOk : winioScript.fopenCreateOk;
    const char* leaf = name;
    const char* cut;

    /* The two copies reach the same file by different names -- the
       frozen one opens it in the working directory and the rewrite
       in the player's own -- so the log carries the event in the same
       words for both, which is the file name. Where the path pointed
       is counted instead, and the harness asserts on the counts: a
       difference the log cannot show would otherwise be either a
       false difference on every save, or no check on the move at
       all. */
    for (cut = name; *cut; cut++)
        if (*cut == '/' || *cut == '\\')
            leaf = cut + 1;
    if (leaf == name)
        winioOpensBare++;
    else
        winioOpensWithDir++;

    WINIO_Log("fopen(%s,%s)->%d", leaf, mode, ok);
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

/* This harness's own table, and its own count beside it. */
const int tweaksNum = (int)(sizeof(tweaks) / sizeof(tweaks[0]));

long winioOpensBare;
long winioOpensWithDir;

void WINIO_ResetMocks(void)
{
    logCount = 0;
    winioOpensBare = 0;
    winioOpensWithDir = 0;
    /* The player's own directory, which is where the rewrite looks for
       the save file. A name rather than a real directory: nothing here
       touches the disk. */
    strcpy(dpt_prefPath, "/player/");
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

/* the overlay's command field */
int OVL_b_CommandActive(void)
{
    return winioScript.overlayCommandActive;
}

void OVL_CommandShow(void)
{
    WINIO_Log("OVL_CommandShow");
    winioScript.overlayCommandActive = 1;
}

void OVL_CommandHide(void)
{
    WINIO_Log("OVL_CommandHide");
    winioScript.overlayCommandActive = 0;
}

/* The gate the free camera closes. The differential cases leave it
   open, so the frozen copy and the rewrite see the same world; the
   suspended case in the driver turns it on for the rewrite alone. */
int WINIO_inputSuspended;

int INO_b_InputSuspended(void)
{
    return WINIO_inputSuspended;
}
