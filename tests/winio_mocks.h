/* Mocks for the win/wininputoutput.c differential test.
 *
 * Everything either copy reaches outside itself: the Win32 calls, the
 * four stdio entry points the card manager uses, the debug trace, the
 * platform layer the rewrite reads the input through, and the
 * DirectInput the frozen copy still reads it through. Each one
 * appends to an ordered log, which is compared between the two copies
 * along with the state they leave behind.
 */
#pragma once

#include "../win/wininputoutput.h"
/* DirectInput's types, which left the engine header with the devices
   and are here for the frozen copy and for the mocks that answer it. */
#include "winio_compat.h"

#define WINIO_LOGMAX  8192
#define WINIO_LINEMAX 96

typedef struct
{
    /* DirectInput8Create and the two CreateDevice calls */
    int diCreateResult;
    int createMouseResult;
    int createKeybResult;

    /* device setup */
    int mouseSetFormat;
    int mouseSetCoop;
    int keybSetFormat;
    int keybSetCoop;

    /* GetProperty */
    int getPropertyResult;
    unsigned int getPropertyData;

    /* GetDeviceState: [0] is the first call of a run, [1] the one
       after a successful re-acquire */
    int mouseStateResult[2];
    int mouseAcquireResult;
    int keybStateResult[2];
    int keybAcquireResult;
    int mouseData[5];
    uint8_t keybData[266];

    /* GetObjectInfo */
    char objectName[64];

    /* Win32 */
    int swapMouseButton;
    short keyState;
    int diskFreeOk;
    unsigned int diskFreeLow;
    unsigned int diskFreeHigh;

    /* the overlay's command field */
    int overlayCommandActive;

    /* the tweak dialog */
    int dialogResult;
    char dialogText[128];
    int dialogDrivesProc;

    /* stdio */
    int fopenReadWriteOk;    /* "r+b" */
    int fopenCreateOk;       /* "w+b" */
    int freadResult;
    int fwriteResult;
    int fcloseResult;
} WinioScript;

extern WinioScript winioScript;

void WINIO_ResetMocks(void);

/* Non-zero while the input is taken from the game, as the free
   camera takes it. WINIO_ResetMocks does not clear it, so the one
   case that sets it also clears it. */
extern int WINIO_inputSuspended;
int  WINIO_LogCount(void);
const char* WINIO_LogLine(int i);
void WINIO_Log(const char* fmt, ...);

/* the two device objects the mock DirectInput hands out */
extern IDirectInputDevice8A* WINIO_MouseDevice;
extern IDirectInputDevice8A* WINIO_KeybDevice;
extern IDirectInput8A* WINIO_DIObject;

/* the dialog procedure the DialogBoxIndirectParamA mock should drive,
   set by the driver to whichever side is running */
extern void* WINIO_DlgProc;

/* type-2 tweak target, so a tweak call shows up in the log */
void WINIO_TweakCallback(float value);
extern float WINIO_tweakInt;
extern float WINIO_tweakFloat;
extern int32_t WINIO_tweakIntTarget;

/* win/overlay.c is a translation unit of its own; the command field is
   mocked so the F11 path is observable in the log. */
int  OVL_b_CommandActive(void);
void OVL_CommandShow(void);
void OVL_CommandHide(void);
