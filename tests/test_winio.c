/* Differential test for win/wininputoutput.c.
 *
 * The frozen pre-rewrite copy (tests/baseline_winio.c, BASE_) and the
 * rewrite (win/wininputoutput.c, NEW_) are linked into one binary.
 * Every case installs the same starting state and the same mock
 * script, runs one entry point on the BASE copy, snapshots the state
 * and the ordered engine-call log, restores, runs the same entry
 * point on the NEW copy, and compares.
 *
 * The file's own tables and its two "selected mode" globals are
 * prefixed per side, because both copies define them; everything in
 * win/wininputoutput.h is shared, which is what makes the comparison
 * one of state rather than of two parallel universes. The two
 * pointers that hold a per-side address -- pcINO_keyMaps and
 * sJoystickHandlerFunction -- are compared as table indices rather
 * than as addresses, and so is the string a name lookup returns.
 *
 * No float tolerance anywhere: nothing in the rewrite changed an
 * expression, so every value is expected bit-identical and the
 * comparison is a memcmp of the bits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "winio_mocks.h"
#include "../inputoutput.h"
#include "../dbg.h"
#include "../time.h"

/* ---- the two copies ---------------------------------------------- */

#define SIDE_DECLS(P) \
    void P##pcINO_JoystickPositionAlignSetFromKeyboard(int, uint8_t*); \
    int  P##pcINO_case64StateMachineHandler(int, int*, float*, float); \
    float P##pcINO_case4MouseAmountCalc(float*, float*, float); \
    void P##pcINO_SetMouseSensitivity(float); \
    void P##pcINO_InputHandlerCase1(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerCase2(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerCase4(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerCase8(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerCase32(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerHoverCraft(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerCase100(tInoPad*, float*, uint8_t*); \
    void P##pcINO_InputHandlerCase101(tInoPad*, float*, uint8_t*); \
    int  P##DINPUT_GetProperty(IDirectInputDevice8A*, int, int, const GUID*, int*); \
    void P##DINPUT_InitVariables(int); \
    void P##pcINO_getKeyNames(int, char*); \
    void P##pcINO_getKeyNamesForEverything(void); \
    char* P##pcINO_GetFourKeyNamesCattedTogether(int, int, int, int); \
    void P##pcINO_SetControlSubmode(int); \
    void P##pcINO_SetControlMode(int); \
    void P##pcINO_Joystick_Init(HWND); \
    void P##pcINO_Joystick_Close(void); \
    void P##pcINO_Joystick_Move(Vec3*, int); \
    int  P##pcINO_i_IsMouseBeingUsed(void); \
    int  P##pcINO_i_SetCurrentPad(int); \
    INT_PTR P##DbgDialogFunc(HWND, UINT, WPARAM, LPARAM); \
    void P##pcINO_dinput_mouse_and_keyb_acquire(void); \
    void P##pcINO_dinput_mouse_and_keyb_unacquire(void); \
    void P##pcINO_HandleTweakFlags(HWND); \
    void P##pcINO_Joystick_Update(struct sInoPad*); \
    int P##pcINO_SavModuleInit(void); \
    int P##pcINO_SavModuleClose(void); \
    int P##pcINO_b_AsyncInProgress(void); \
    int P##pcINO_i_TestPresence(void); \
    int P##pcINO_i_MountCard(void); \
    int P##pcINO_i_UnmountCard(void); \
    int P##pcINO_i_CheckCard(void); \
    int P##pcINO_i_FormatCard(void); \
    int P##pcINO_i_TestFreeSpace(void); \
    int P##pcINO_i_Retry(void); \
    int P##pcINO_i_Continue(void); \
    int P##pcINO_i_ActiveConsole(void); \
    int P##pcINO_i_Open(void); \
    int P##pcINO_i_Close(void); \
    int P##pcINO_i_ReadIndex_1(void); \
    int P##pcINO_i_WriteIndex(void); \
    int P##pcINO_i_ReadSlot_1(void); \
    int P##pcINO_i_ReadSlot_2(void); \
    int P##pcINO_i_ReadSlot_3(void); \
    int P##pcINO_i_WriteSlot_C(void); \
    int P##pcINO_i_WriteSlot_E(void); \
    int P##pcINO_i_CheckFreeSpace(void); \
    int P##pcINO_i_TestCard(void); \
    extern int P##pcINO_SelectedControlMode; \
    extern int P##pcINO_SelectedControlSubmode; \
    extern Vec3 P##pcINO_joyPositionBeforeControlSubmodeSet; \
    extern char P##prefixOneSpace[8]; \
    extern char P##prefix_p16_2[7]; \
    extern uint16_t P##pcINO_IntKeybDataLink[36]; \
    extern uint8_t P##pcINO_keyMapsCase1[24]; \
    extern uint8_t P##pcINO_keyMapsCase2[24]; \
    extern uint8_t P##pcINO_keyMapsCase64[24]; \
    extern uint8_t P##pcINO_keyMapsCase32[24]; \
    extern uint8_t P##pcINO_keyMapsCase8[24]; \
    extern uint8_t P##pcINO_keyMapsCase100[24]; \
    extern uint8_t P##pcINO_keyMapsCase101[24]; \
    extern uint8_t P##pcINO_InputHandlerSubModeCaseDef[24]; \
    extern uint8_t P##pcINO_InputHandlerSubModeCase0[24]; \
    extern uint8_t P##pcINO_InputHandlerSubModeCase2[24]; \
    extern uint8_t P##pcINO_InputHandlerSubModeCase7[24]; \
    extern uint8_t P##pcINO_InputHandlerSubModeCase10[24]; \
    extern int P##pcINO_savPresenceFailure; \
    extern int P##pcINO_savMountFailure; \
    extern int P##pcINO_savCheckFailure; \
    extern int P##pcINO_savFormatFailure; \
    extern int P##pcINO_savFreeSpaceFailure; \
    extern int P##pcINO_savCardTestFailure;

SIDE_DECLS(BASE_)
SIDE_DECLS(NEW_)

#define NHANDLER 8
#define NKEYMAP  12
#define NCARD    23

typedef struct
{
    const char* tag;
    void (*align)(int, uint8_t*);
    int  (*stateMachine)(int, int*, float*, float);
    float (*mouseAmount)(float*, float*, float);
    void (*setSensitivity)(float);
    void (*handler[NHANDLER])(tInoPad*, float*, uint8_t*);
    int  (*getProperty)(IDirectInputDevice8A*, int, int, const GUID*, int*);
    void (*initVariables)(int);
    void (*getKeyNames)(int, char*);
    void (*getAllKeyNames)(void);
    char* (*fourNames)(int, int, int, int);
    void (*setSubmode)(int);
    void (*setMode)(int);
    void (*joyInit)(HWND);
    void (*joyClose)(void);
    void (*joyMove)(Vec3*, int);
    int  (*mouseUsed)(void);
    int  (*setPad)(int);
    INT_PTR (*dlgProc)(HWND, UINT, WPARAM, LPARAM);
    void (*acquire)(void);
    void (*unacquire)(void);
    void (*tweakFlags)(HWND);
    void (*joyUpdate)(struct sInoPad*);
    int  (*card[NCARD])(void);
    uint8_t* keyMap[NKEYMAP];
    void* handlerAddr[NHANDLER];
    char* p16_2;
    char* oneSpace;
    int* selMode;
    int* selSubmode;
    Vec3* joyBefore;
    uint16_t* keybLink;
    int* savFail[6];
} Side;

#define MAKE_SIDE(P, NAME) { NAME, \
    P##pcINO_JoystickPositionAlignSetFromKeyboard, \
    P##pcINO_case64StateMachineHandler, P##pcINO_case4MouseAmountCalc, \
    P##pcINO_SetMouseSensitivity, \
    { P##pcINO_InputHandlerCase1, P##pcINO_InputHandlerCase2, \
      P##pcINO_InputHandlerCase4, P##pcINO_InputHandlerCase8, \
      P##pcINO_InputHandlerCase32, P##pcINO_InputHandlerHoverCraft, \
      P##pcINO_InputHandlerCase100, P##pcINO_InputHandlerCase101 }, \
    P##DINPUT_GetProperty, P##DINPUT_InitVariables, \
    P##pcINO_getKeyNames, P##pcINO_getKeyNamesForEverything, \
    P##pcINO_GetFourKeyNamesCattedTogether, \
    P##pcINO_SetControlSubmode, \
    P##pcINO_SetControlMode, \
    P##pcINO_Joystick_Init, P##pcINO_Joystick_Close, P##pcINO_Joystick_Move, \
    P##pcINO_i_IsMouseBeingUsed, P##pcINO_i_SetCurrentPad, P##DbgDialogFunc, \
    P##pcINO_dinput_mouse_and_keyb_acquire, \
    P##pcINO_dinput_mouse_and_keyb_unacquire, \
    P##pcINO_HandleTweakFlags, P##pcINO_Joystick_Update, \
    { P##pcINO_SavModuleInit, P##pcINO_SavModuleClose, P##pcINO_b_AsyncInProgress, \
      P##pcINO_i_TestPresence, P##pcINO_i_MountCard, P##pcINO_i_UnmountCard, \
      P##pcINO_i_CheckCard, P##pcINO_i_FormatCard, P##pcINO_i_TestFreeSpace, \
      P##pcINO_i_Retry, P##pcINO_i_Continue, P##pcINO_i_ActiveConsole, \
      P##pcINO_i_Open, P##pcINO_i_Close, P##pcINO_i_ReadIndex_1, \
      P##pcINO_i_WriteIndex, P##pcINO_i_ReadSlot_1, P##pcINO_i_ReadSlot_2, \
      P##pcINO_i_ReadSlot_3, P##pcINO_i_WriteSlot_C, P##pcINO_i_WriteSlot_E, \
      P##pcINO_i_CheckFreeSpace, P##pcINO_i_TestCard }, \
    { P##pcINO_keyMapsCase1, P##pcINO_keyMapsCase2, P##pcINO_keyMapsCase64, \
      P##pcINO_keyMapsCase32, P##pcINO_keyMapsCase8, P##pcINO_keyMapsCase100, \
      P##pcINO_keyMapsCase101, P##pcINO_InputHandlerSubModeCaseDef, \
      P##pcINO_InputHandlerSubModeCase0, P##pcINO_InputHandlerSubModeCase2, \
      P##pcINO_InputHandlerSubModeCase7, P##pcINO_InputHandlerSubModeCase10 }, \
    { P##pcINO_InputHandlerCase1, P##pcINO_InputHandlerCase2, \
      P##pcINO_InputHandlerCase4, P##pcINO_InputHandlerCase8, \
      P##pcINO_InputHandlerCase32, P##pcINO_InputHandlerHoverCraft, \
      P##pcINO_InputHandlerCase100, P##pcINO_InputHandlerCase101 }, \
    P##prefix_p16_2, P##prefixOneSpace, \
    &P##pcINO_SelectedControlMode, &P##pcINO_SelectedControlSubmode, \
    &P##pcINO_joyPositionBeforeControlSubmodeSet, P##pcINO_IntKeybDataLink, \
    { &P##pcINO_savPresenceFailure, &P##pcINO_savMountFailure, \
      &P##pcINO_savCheckFailure, &P##pcINO_savFormatFailure, \
      &P##pcINO_savFreeSpaceFailure, &P##pcINO_savCardTestFailure } }

static const Side baseSide = MAKE_SIDE(BASE_, "BASE");
static const Side newSide = MAKE_SIDE(NEW_, "NEW");

/* ---- the state both copies share --------------------------------- */

typedef struct
{
    Vec3 joy[2];
    Vec3 joyBefore;
    int stateMachineFlags;
    float mouseSensitivity;
    float case1DividedTime, case1DividedTime2, case2DividedTime;
    int case2Scroll;
    float case4X, case4Y, case4XD, case4YD;
    int granularity;
    int selMode, selSubmode;
    int keyMapIdx, handlerIdx;
    uint8_t keybBackup[266];
    uint16_t keybLink[36];
    int savButton[8];
    int savNext, savCommand, freeSpace, lastError;
    int joyEnable, currentPad;
    int savFail[6];
    char keyNames[256];
    short numLock;
    int haveMouse, haveKeyb, haveDI;
    void* savFp;
    int savSize;
    char savBuffer[64];
    char buttonNames[18][64];
    tInoPad pad;
    Vec3 moveOut;
    int propOut;
    int retInt;
    float retFloat;
    float retFloatArgs[2];
    int retStrTag;
    char retStr[256];
    int32_t tweakInt;
    float tweakFloat;
    char dlgBuff[128];
} State;

static char savBufferLive[64];

static int keyMapIndexOf(const void* p)
{
    int i;
    if (!p) return -1;
    for (i = 0; i < NKEYMAP; i++)
    {
        if (p == (const void*)baseSide.keyMap[i]) return i;
        if (p == (const void*)newSide.keyMap[i]) return i;
    }
    return -2;
}

static int handlerIndexOf(const void* p)
{
    int i;
    if (!p) return -1;
    for (i = 0; i < NHANDLER; i++)
    {
        if (p == baseSide.handlerAddr[i]) return i;
        if (p == newSide.handlerAddr[i]) return i;
    }
    return -2;
}

static int stringTagOf(const char* p)
{
    if (!p) return -1;
    if (p == baseSide.p16_2 || p == newSide.p16_2) return 1;
    if (p == baseSide.oneSpace || p == newSide.oneSpace) return 2;
    if (p == pcINO_KeyNamesToRet) return 3;
    return 0;
}

static void installState(const Side* s, const State* st)
{
    int i;

    memcpy(pcINO_joystickPosition, st->joy, sizeof(st->joy));
    *s->joyBefore = st->joyBefore;
    pcINO_stateMachineFlags = st->stateMachineFlags;
    pcINO_mouseSensitivity = st->mouseSensitivity;
    pcINO_case1DividedTime = st->case1DividedTime;
    pcINO_case1DividedTime2 = st->case1DividedTime2;
    pcINO_case2DividedTime = st->case2DividedTime;
    pcINO_case2_mouseScroll = st->case2Scroll;
    pcINO_case4_mouseMoveX = st->case4X;
    pcINO_case4_mouseMoveY = st->case4Y;
    pcINO_case4_mouseMoveXDeadline = st->case4XD;
    pcINO_case4_mouseMoveYDeadline = st->case4YD;
    mouseGranularity = st->granularity;
    *s->selMode = st->selMode;
    *s->selSubmode = st->selSubmode;
    pcINO_keyMaps = st->keyMapIdx < 0 ? NULL : (char*)s->keyMap[st->keyMapIdx];
    sJoystickHandlerFunction = st->handlerIdx < 0 ? NULL : s->handler[st->handlerIdx];
    memcpy(pcINO_keyboardDataBackup, st->keybBackup, sizeof(st->keybBackup));
    memcpy(s->keybLink, st->keybLink, sizeof(st->keybLink));
    memcpy(INO_gi_SavButton, st->savButton, sizeof(st->savButton));
    INO_ge_SavSetNextState = st->savNext;
    INO_ge_SavCommand = st->savCommand;
    INO_gi_FreeSpace = st->freeSpace;
    pcINO_gi_LastError = st->lastError;
    INO_l_Joystick_Enable = st->joyEnable;
    INO_gi_CurrentPadId = st->currentPad;
    for (i = 0; i < 6; i++)
        *s->savFail[i] = st->savFail[i];
    memcpy(pcINO_KeyNamesToRet, st->keyNames, sizeof(st->keyNames));
    pcINO_numLockAtAcquire = st->numLock;
    pcINO_mouseDevice = st->haveMouse ? WINIO_MouseDevice : NULL;
    pcINO_keybDevice = st->haveKeyb ? WINIO_KeybDevice : NULL;
    dinput_if = st->haveDI ? WINIO_DIObject : NULL;
    pcINO_Hwnd = (HWND)0x4242;
    INO_gst_SavFileDesc.fp = (FILE*)st->savFp;
    INO_gst_SavFileDesc.size = st->savSize;
    memcpy(savBufferLive, st->savBuffer, sizeof(savBufferLive));
    INO_gst_SavFileDesc.saveBuffer = savBufferLive;
    for (i = 0; i < 18; i++)
        memcpy(buttonNames[i].str, st->buttonNames[i], 64);
    WINIO_tweakIntTarget = st->tweakInt;
    WINIO_tweakFloat = st->tweakFloat;
    memcpy(dbgDialogBuff, st->dlgBuff, sizeof(dbgDialogBuff));
    tweakFlagsDisabled = 0;
}

static void captureState(const Side* s, State* st)
{
    int i;

    memcpy(st->joy, pcINO_joystickPosition, sizeof(st->joy));
    st->joyBefore = *s->joyBefore;
    st->stateMachineFlags = pcINO_stateMachineFlags;
    st->mouseSensitivity = pcINO_mouseSensitivity;
    st->case1DividedTime = pcINO_case1DividedTime;
    st->case1DividedTime2 = pcINO_case1DividedTime2;
    st->case2DividedTime = pcINO_case2DividedTime;
    st->case2Scroll = pcINO_case2_mouseScroll;
    st->case4X = pcINO_case4_mouseMoveX;
    st->case4Y = pcINO_case4_mouseMoveY;
    st->case4XD = pcINO_case4_mouseMoveXDeadline;
    st->case4YD = pcINO_case4_mouseMoveYDeadline;
    st->granularity = mouseGranularity;
    st->selMode = *s->selMode;
    st->selSubmode = *s->selSubmode;
    st->keyMapIdx = keyMapIndexOf(pcINO_keyMaps);
    st->handlerIdx = handlerIndexOf((void*)sJoystickHandlerFunction);
    memcpy(st->keybBackup, pcINO_keyboardDataBackup, sizeof(st->keybBackup));
    memcpy(st->keybLink, s->keybLink, sizeof(st->keybLink));
    memcpy(st->savButton, INO_gi_SavButton, sizeof(st->savButton));
    st->savNext = INO_ge_SavSetNextState;
    st->savCommand = INO_ge_SavCommand;
    st->freeSpace = INO_gi_FreeSpace;
    st->lastError = pcINO_gi_LastError;
    st->joyEnable = INO_l_Joystick_Enable;
    st->currentPad = INO_gi_CurrentPadId;
    for (i = 0; i < 6; i++)
        st->savFail[i] = *s->savFail[i];
    memcpy(st->keyNames, pcINO_KeyNamesToRet, sizeof(st->keyNames));
    st->numLock = pcINO_numLockAtAcquire;
    st->haveMouse = pcINO_mouseDevice != NULL;
    st->haveKeyb = pcINO_keybDevice != NULL;
    st->haveDI = dinput_if != NULL;
    st->savFp = INO_gst_SavFileDesc.fp;
    st->savSize = INO_gst_SavFileDesc.size;
    memcpy(st->savBuffer, savBufferLive, sizeof(st->savBuffer));
    for (i = 0; i < 18; i++)
        memcpy(st->buttonNames[i], buttonNames[i].str, 64);
    st->tweakInt = WINIO_tweakIntTarget;
    st->tweakFloat = WINIO_tweakFloat;
    memcpy(st->dlgBuff, dbgDialogBuff, sizeof(dbgDialogBuff));
}

/* ---- one case ---------------------------------------------------- */

enum {
    OP_SENSITIVITY, OP_MOUSEAMOUNT, OP_STATEMACHINE, OP_ALIGN, OP_HANDLER,
    OP_INITVARS, OP_SETMODE, OP_SETSUBMODE, OP_FOURNAMES, OP_ALLKEYNAMES,
    OP_KEYNAMES, OP_UPDATE, OP_JOYINIT, OP_JOYCLOSE, OP_JOYMOVE, OP_SETPAD,
    OP_ACQUIRE, OP_UNACQUIRE, OP_DLGPROC, OP_TWEAKFLAGS, OP_CARD,
    OP_GETPROPERTY, OP_MOUSEUSED, OP_NUM
};

typedef struct
{
    int op;
    int i0, i1, i2, i3;
    float f0, f1, f2;
    float clock;
    float mouse[3];
    uint8_t keyboard[266];
} Case;

static void runOp(const Side* s, const Case* c, State* st)
{
    TIM_gf_MainClock = c->clock;
    WINIO_DlgProc = (void*)s->dlgProc;
    st->retInt = 0;
    st->retFloat = 0.0f;
    st->retStrTag = -9;
    /* 0xBB, not 0: a zeroed destination hides a missing terminator */
    memset(st->retStr, 0xBB, sizeof(st->retStr));
    st->retStr[sizeof(st->retStr) - 1] = 0;
    memset(&st->pad, 0, sizeof(st->pad));
    memset(&st->moveOut, 0, sizeof(st->moveOut));
    st->propOut = 0;

    switch (c->op)
    {
    case OP_SENSITIVITY:
        s->setSensitivity(c->f0);
        break;
    case OP_MOUSEAMOUNT:
        st->retFloat = s->mouseAmount(&pcINO_case4_mouseMoveX,
                                      &pcINO_case4_mouseMoveXDeadline, c->f0);
        st->retFloatArgs[0] = pcINO_case4_mouseMoveX;
        st->retFloatArgs[1] = pcINO_case4_mouseMoveXDeadline;
        break;
    case OP_STATEMACHINE:
        st->retInt = s->stateMachine(c->i0, &pcINO_stateMachineFlags,
                                     &pcINO_case1DividedTime2, c->f0);
        break;
    case OP_ALIGN:
        s->align(c->i0, (uint8_t*)c->keyboard);
        break;
    case OP_HANDLER:
        s->handler[c->i0](&st->pad, (float*)c->mouse, (uint8_t*)c->keyboard);
        break;
    case OP_INITVARS:
        s->initVariables(c->i0);
        break;
    case OP_SETMODE:
        s->setMode(c->i0);
        break;
    case OP_SETSUBMODE:
        s->setSubmode(c->i0);
        break;
    case OP_FOURNAMES:
    {
        char* r = s->fourNames(c->i0, c->i1, c->i2, c->i3);
        st->retStrTag = stringTagOf(r);
        if (r)
            memcpy(st->retStr, r, 7);
        break;
    }
    case OP_ALLKEYNAMES:
        s->getAllKeyNames();
        break;
    case OP_KEYNAMES:
        s->getKeyNames(c->i0, st->retStr);
        break;
    case OP_UPDATE:
        s->joyUpdate(&st->pad);
        break;
    case OP_JOYINIT:
        s->joyInit((HWND)0x4242);
        break;
    case OP_JOYCLOSE:
        s->joyClose();
        break;
    case OP_JOYMOVE:
        s->joyMove(&st->moveOut, c->i0);
        break;
    case OP_SETPAD:
        st->retInt = s->setPad(c->i0);
        break;
    case OP_ACQUIRE:
        s->acquire();
        break;
    case OP_UNACQUIRE:
        s->unacquire();
        break;
    case OP_DLGPROC:
        st->retInt = (int)s->dlgProc((HWND)0x55, (UINT)c->i0, (WPARAM)c->i1, (LPARAM)c->i2);
        break;
    case OP_TWEAKFLAGS:
        s->tweakFlags((HWND)0x4242);
        break;
    case OP_CARD:
        st->retInt = s->card[c->i0]();
        break;
    case OP_GETPROPERTY:
        st->retInt = s->getProperty(c->i0 ? WINIO_MouseDevice : WINIO_KeybDevice,
                                    c->i1, c->i2, (const GUID*)(size_t)c->i3, &st->propOut);
        break;
    case OP_MOUSEUSED:
        st->retInt = s->mouseUsed();
        break;
    default:
        break;
    }
}

/* ---- comparison -------------------------------------------------- */

static long cases, diffs;
static const char* group = "?";
static long opRan[OP_NUM];

static char baseLog[WINIO_LOGMAX][WINIO_LINEMAX];
static int baseLogCount;

static void report(long key, const char* what)
{
    if (diffs < 25)
        printf("  DIFF %-16s %-24s case=%ld\n", group, what, key);
    diffs++;
}

#define CMP(field, name) \
    if (memcmp(&a.field, &b.field, sizeof(a.field)) != 0) report(key, name)


/* ---- coverage ----------------------------------------------------
   Every guard the sweeps depend on, counted from the fixture rather
   than from inside the file under test. A guard that never takes both
   values is a sweep that proves less than its case count suggests, so
   the run fails when one of these is zero. */

typedef struct { const char* name; long n; } Cov;
static Cov cov[] = {
    {"update: mouse device present", 0},      /* 0 */
    {"update: mouse device absent", 0},
    {"update: keyboard present", 0},
    {"update: keyboard absent", 0},
    {"update: mouse state INPUTLOST", 0},     /* 4 */
    {"update: mouse state ok", 0},
    {"update: mouse state failed", 0},
    {"update: mouse re-acquire ok", 0},
    {"update: mouse re-acquire failed", 0},
    {"update: keyboard state INPUTLOST", 0},  /* 9 */
    {"update: keyboard state ok", 0},
    {"update: keyboard state failed", 0},
    {"update: keyboard re-acquire ok", 0},
    {"update: keyboard re-acquire failed", 0},
    {"update: alt+f4 key combination", 0},    /* 14 */
    {"update: tweak key edge", 0},
    {"update: wheel granularity zero", 0},
    {"update: wheel granularity non-zero", 0},
    {"card: file handle null", 0},            /* 18 */
    {"card: file handle set", 0},
    {"card: command 16", 0},
    {"card: command 10", 0},
    {"card: command other", 0},
    {"card: free space below photo buffer", 0},
    {"card: free space at or above it", 0},
    {"card: free space below 1024", 0},
    {"card: free space at or above 1024", 0},
    {"card: a failure flag set", 0},          /* 27 */
    {"card: every failure flag clear", 0},
    {"card: fopen r+b succeeds", 0},
    {"card: fopen r+b fails", 0},
    {"card: fopen w+b succeeds", 0},
    {"card: fopen w+b fails", 0},
    {"tweak: dialog returned 1", 0},          /* 33 */
    {"tweak: dialog returned other", 0},
    {"tweak: entry type 0", 0},
    {"tweak: entry type 1", 0},
    {"tweak: entry type 2", 0},
    {"tweak: entry type out of range", 0},
    {"tweak: name not in the table", 0},
    {"init: DirectInput8Create ok", 0},       /* 40 */
    {"init: DirectInput8Create failed", 0},
    {"init: mouse CreateDevice ok", 0},
    {"init: mouse CreateDevice failed", 0},
    {"init: buttons already swapped", 0},
    {"init: buttons not swapped", 0},
    {"mode: a mode the switch names", 0},     /* 46 */
    {"mode: default arm", 0},
    {"mode: already that mode", 0},
    {"submode: negative", 0},                 /* 49 */
    {"submode: under control mode 4", 0},
    {"submode: mode 64, submode 4", 0},
    {"submode: mode 64, other submode", 0},
    {"submode: mode 1", 0},
    {"submode: already that submode", 0},
    {"fournames: returned prefix_p16_2", 0},  /* 55 */
    {"fournames: returned prefixOneSpace", 0},
    {"fournames: returned the catted buffer", 0},
    {"getproperty: succeeded", 0},            /* 58 */
    {"getproperty: failed", 0},
    {"handler: at least one key down", 0},    /* 60 */
    {"handler: no key down", 0},
};
#define NCOV ((int)(sizeof(cov) / sizeof(cov[0])))

static void countCoverage(const Case* c, const State* st)
{
    int i, any;

    switch (c->op)
    {
    case OP_UPDATE:
        cov[st->haveMouse ? 0 : 1].n++;
        cov[st->haveKeyb ? 2 : 3].n++;
        if (st->haveMouse)
        {
            if (winioScript.mouseStateResult[0] == (int)0x8007001E)
            {
                cov[4].n++;
                cov[winioScript.mouseAcquireResult < 0 ? 8 : 7].n++;
            }
            else
                cov[winioScript.mouseStateResult[0] < 0 ? 6 : 5].n++;
        }
        if (st->haveKeyb)
        {
            if (winioScript.keybStateResult[0] == (int)0x8007001E)
            {
                cov[9].n++;
                cov[winioScript.keybAcquireResult < 0 ? 13 : 12].n++;
            }
            else
                cov[winioScript.keybStateResult[0] < 0 ? 11 : 10].n++;
            if ((winioScript.keybData[56] || winioScript.keybData[184])
                && winioScript.keybData[62])
                cov[14].n++;
            if (winioScript.keybData[88] && !st->keybBackup[88])
                cov[15].n++;
        }
        cov[st->granularity ? 17 : 16].n++;
        break;

    case OP_CARD:
        cov[st->savFp ? 19 : 18].n++;
        cov[st->savCommand == 16 ? 20 : st->savCommand == 10 ? 21 : 22].n++;
        cov[st->freeSpace < SAV_PHOTO_BUFFER_SIZE ? 23 : 24].n++;
        cov[st->freeSpace < 1024 ? 25 : 26].n++;
        any = 0;
        for (i = 0; i < 6; i++)
            any |= st->savFail[i] != 0;
        cov[any ? 27 : 28].n++;
        cov[winioScript.fopenReadWriteOk ? 29 : 30].n++;
        cov[winioScript.fopenCreateOk ? 31 : 32].n++;
        break;

    case OP_TWEAKFLAGS:
        cov[winioScript.dialogResult == 1 ? 33 : 34].n++;
        {
            char key[64];
            int found = 0;
            sscanf(winioScript.dialogText, "%63s", key);
            for (i = 0; i < 17; i++)
                if (strcmp(tweaks[i].elemname, key) == 0)
                {
                    found = 1;
                    if (tweaks[i].type >= 0 && tweaks[i].type <= 2)
                        cov[35 + tweaks[i].type].n++;
                    else
                        cov[38].n++;
                }
            if (!found)
                cov[39].n++;
        }
        break;

    case OP_JOYINIT:
        cov[winioScript.diCreateResult < 0 ? 41 : 40].n++;
        if (winioScript.diCreateResult >= 0)
        {
            cov[winioScript.createMouseResult < 0 ? 43 : 42].n++;
            cov[winioScript.swapMouseButton ? 44 : 45].n++;
        }
        break;

    case OP_SETMODE:
        if (st->selMode == c->i0)
            cov[48].n++;
        else if (c->i0 == 1 || c->i0 == 2 || c->i0 == 4 || c->i0 == 8
                 || c->i0 == 32 || c->i0 == 64 || c->i0 == 128 || c->i0 == 129)
            cov[46].n++;
        else
            cov[47].n++;
        break;

    case OP_SETSUBMODE:
        if (c->i0 < 0)
            cov[49].n++;
        else if (st->selSubmode == c->i0)
            cov[54].n++;
        else if (st->selMode == 4)
            cov[50].n++;
        else if (st->selMode == 64)
            cov[c->i0 == 4 ? 51 : 52].n++;
        else if (st->selMode == 1)
            cov[53].n++;
        break;

    case OP_GETPROPERTY:
        cov[winioScript.getPropertyResult < 0 ? 59 : 58].n++;
        break;

    case OP_HANDLER:
        any = 0;
        for (i = 0; i < 266; i++)
            any |= c->keyboard[i] != 0;
        cov[any ? 60 : 61].n++;
        break;

    default:
        break;
    }
}

/* The two copies run one after the other from this same frame, so an
   uninitialised local in the second lands on the bytes the first left
   behind and reads back as correct. Dropping Joystick_Update's clear
   of mouseData scored clean until this was here. */
static void dirtyStack(void)
{
    volatile unsigned char scratch[3072];
    int i;
    for (i = 0; i < 3072; i++)
        scratch[i] = (unsigned char)(0xC3 + i * 7);
}

static void runCase(const Case* c, const State* start, long key)
{
    State a, b;
    int i, n;

    opRan[c->op]++;
    cases++;
    countCoverage(c, start);

    dirtyStack();
    WINIO_ResetMocks();
    installState(&baseSide, start);
    a = *start;
    runOp(&baseSide, c, &a);
    captureState(&baseSide, &a);
    baseLogCount = WINIO_LogCount();
    n = baseLogCount < WINIO_LOGMAX ? baseLogCount : WINIO_LOGMAX;
    for (i = 0; i < n; i++)
        memcpy(baseLog[i], WINIO_LogLine(i), WINIO_LINEMAX);

    dirtyStack();
    WINIO_ResetMocks();
    installState(&newSide, start);
    b = *start;
    runOp(&newSide, c, &b);
    captureState(&newSide, &b);

    if (WINIO_LogCount() != baseLogCount)
    {
        report(key, "engine-call count");
        return;
    }
    for (i = 0; i < n; i++)
        if (strcmp(baseLog[i], WINIO_LogLine(i)) != 0)
        {
            if (diffs < 25)
                printf("  DIFF %-16s log[%d] base=%s new=%s case=%ld\n",
                       group, i, baseLog[i], WINIO_LogLine(i), key);
            diffs++;
            return;
        }

    CMP(joy, "joystickPosition");
    CMP(joyBefore, "joyBeforeSubmodeSet");
    CMP(stateMachineFlags, "stateMachineFlags");
    CMP(mouseSensitivity, "mouseSensitivity");
    CMP(case1DividedTime, "case1DividedTime");
    CMP(case1DividedTime2, "case1DividedTime2");
    CMP(case2DividedTime, "case2DividedTime");
    CMP(case2Scroll, "case2_mouseScroll");
    CMP(case4X, "case4_mouseMoveX");
    CMP(case4Y, "case4_mouseMoveY");
    CMP(case4XD, "case4_mouseMoveXDeadline");
    CMP(case4YD, "case4_mouseMoveYDeadline");
    CMP(granularity, "mouseGranularity");
    CMP(selMode, "SelectedControlMode");
    CMP(selSubmode, "SelectedControlSubmode");
    CMP(keyMapIdx, "keyMaps table");
    CMP(handlerIdx, "handler function");
    CMP(keybBackup, "keyboardDataBackup");
    CMP(keybLink, "IntKeybDataLink");
    CMP(savButton, "INO_gi_SavButton");
    CMP(savNext, "SavSetNextState");
    CMP(savCommand, "SavCommand");
    CMP(freeSpace, "INO_gi_FreeSpace");
    CMP(lastError, "pcINO_gi_LastError");
    CMP(joyEnable, "INO_l_Joystick_Enable");
    CMP(currentPad, "INO_gi_CurrentPadId");
    CMP(savFail, "card failure flags");
    CMP(keyNames, "KeyNamesToRet");
    CMP(numLock, "numLockAtAcquire");
    CMP(haveMouse, "mouse device");
    CMP(haveKeyb, "keyboard device");
    CMP(haveDI, "dinput object");
    CMP(savFp, "SavFileDesc.fp");
    CMP(savSize, "SavFileDesc.size");
    CMP(savBuffer, "SavFileDesc.saveBuffer");
    CMP(buttonNames, "buttonNames");
    CMP(pad, "pad data");
    CMP(moveOut, "Joystick_Move out");
    CMP(propOut, "GetProperty out");
    CMP(retInt, "int return");
    CMP(retFloat, "float return");
    CMP(retFloatArgs, "mouseAmount accum/deadline");
    if (c->op == OP_FOURNAMES && a.retStrTag >= 1 && a.retStrTag <= 3)
        cov[54 + a.retStrTag].n++;
    CMP(retStrTag, "returned string identity");
    CMP(retStr, "returned string");
    CMP(tweakInt, "tweak int target");
    CMP(tweakFloat, "tweak float target");
    CMP(dlgBuff, "dbgDialogBuff");
}

/* ---- fixtures ---------------------------------------------------- */

static uint32_t rngState;
static uint32_t rnd(void)
{
    rngState = rngState * 1664525u + 1013904223u;
    return rngState >> 8;
}

static float rndFloat(float lo, float hi)
{
    return lo + (hi - lo) * ((float)(rnd() & 0xFFFF) / 65535.0f);
}

static const uint16_t linkDefault[36] = { 258, 0, 259, 0, 16, 211, 18, 207,
                                          42, 82, 46, 76, 57, 157, 29, 221,
                                          3, 201, 4, 209, 17, 200, 31, 208,
                                          30, 203, 32, 205, 15, 14, 1, 197,
                                          259, 1, 57, 260 };

static void baseState(State* st)
{
    int i;
    memset(st, 0, sizeof(*st));
    st->joy[0].x = 1.0f;
    st->mouseSensitivity = 0.5f;
    st->granularity = 1;
    st->selMode = -1;
    st->selSubmode = -1;
    st->keyMapIdx = 0;
    st->handlerIdx = 0;
    memcpy(st->keybLink, linkDefault, sizeof(linkDefault));
    st->savNext = -1;
    st->savCommand = -1;
    st->haveMouse = 1;
    st->haveKeyb = 1;
    st->haveDI = 1;
    st->savSize = 32;
    for (i = 0; i < 18; i++)
        memset(st->buttonNames[i], 0, 64);
}

static void randomState(State* st)
{
    int i;
    baseState(st);
    for (i = 0; i < 2; i++)
    {
        st->joy[i].x = rndFloat(-2.0f, 2.0f);
        st->joy[i].y = rndFloat(-2.0f, 2.0f);
        st->joy[i].z = rndFloat(-2.0f, 2.0f);
    }
    st->joyBefore.x = rndFloat(-1.0f, 1.0f);
    st->joyBefore.y = rndFloat(-1.0f, 1.0f);
    st->stateMachineFlags = (int)(rnd() % 40u);
    st->mouseSensitivity = rndFloat(0.05f, 2.5f);
    st->case1DividedTime = rndFloat(-1.0f, 3.0f);
    st->case1DividedTime2 = rndFloat(-1.0f, 3.0f);
    st->case2DividedTime = rndFloat(-1.0f, 3.0f);
    st->case2Scroll = (int)(rnd() % 7u) - 3;
    st->case4X = rndFloat(-5.0f, 5.0f);
    st->case4Y = rndFloat(-5.0f, 5.0f);
    st->case4XD = rndFloat(-1.0f, 3.0f);
    st->case4YD = rndFloat(-1.0f, 3.0f);
    st->granularity = (int)(rnd() % 4u);
    st->selMode = (int)(rnd() % 8u);
    st->selSubmode = (int)(rnd() % 12u) - 1;
    st->keyMapIdx = (int)(rnd() % NKEYMAP);
    st->handlerIdx = (int)(rnd() % NHANDLER);
    for (i = 0; i < 266; i++)
        st->keybBackup[i] = (rnd() & 7u) ? 0 : 0x80;
    for (i = 0; i < 8; i++)
        st->savButton[i] = (int)(rnd() % 30u);
    st->savNext = (rnd() & 1u) ? -1 : (int)(rnd() % 30u);
    st->savCommand = (int)(rnd() % 20u);
    st->freeSpace = (int)(rnd() % 4u) == 0 ? 0 : (int)(rnd() % 4000000u);
    st->lastError = (int)(rnd() % 800u);
    for (i = 0; i < 6; i++)
        st->savFail[i] = (int)(rnd() % 4u);
    st->savSize = 1 + (int)(rnd() % 48u);
    for (i = 0; i < 64; i++)
        st->savBuffer[i] = (char)rnd();
}

static void randomScript(void)
{
    int i;
    memset(&winioScript, 0, sizeof(winioScript));
    winioScript.diCreateResult = (rnd() & 7u) ? 0 : -1;
    winioScript.createMouseResult = (rnd() & 7u) ? 0 : -1;
    winioScript.createKeybResult = (rnd() & 7u) ? 0 : -1;
    winioScript.mouseSetFormat = (rnd() & 7u) ? 0 : -1;
    winioScript.mouseSetCoop = (rnd() & 7u) ? 0 : -1;
    winioScript.keybSetFormat = (rnd() & 7u) ? 0 : -1;
    winioScript.keybSetCoop = (rnd() & 7u) ? 0 : -1;
    winioScript.getPropertyResult = (rnd() & 3u) ? 0 : -1;
    winioScript.getPropertyData = rnd() % 10u;
    for (i = 0; i < 2; i++)
    {
        switch (rnd() % 3u)
        {
        case 0: winioScript.mouseStateResult[i] = 0; break;
        case 1: winioScript.mouseStateResult[i] = (int)0x8007001E; break;
        default: winioScript.mouseStateResult[i] = -1; break;
        }
        switch (rnd() % 3u)
        {
        case 0: winioScript.keybStateResult[i] = 0; break;
        case 1: winioScript.keybStateResult[i] = (int)0x8007001E; break;
        default: winioScript.keybStateResult[i] = -1; break;
        }
    }
    winioScript.mouseAcquireResult = (rnd() & 1u) ? 0 : -1;
    winioScript.keybAcquireResult = (rnd() & 1u) ? 0 : -1;
    for (i = 0; i < 5; i++)
        winioScript.mouseData[i] = (int)(rnd() % 0x1000u) - 0x800;
    for (i = 0; i < 266; i++)
        winioScript.keybData[i] = (rnd() & 3u) ? 0 : 0x80;
    strcpy(winioScript.objectName, "KEY");
    winioScript.swapMouseButton = (int)(rnd() & 1u);
    winioScript.keyState = (short)(rnd() & 1u);
    winioScript.diskFreeOk = (rnd() & 3u) != 0;
    winioScript.diskFreeHigh = (rnd() & 3u) == 0 ? 1 : 0;
    winioScript.diskFreeLow = (rnd() & 1u) ? 0x90000000u : (rnd() % 4000000u);
    winioScript.dialogResult = (int)(rnd() % 3u);
    winioScript.dialogDrivesProc = 1;
    winioScript.fopenReadWriteOk = (int)(rnd() & 1u);
    winioScript.fopenCreateOk = (int)(rnd() & 1u);
    winioScript.freadResult = (int)(rnd() % 40u);
    winioScript.fwriteResult = (int)(rnd() % 40u);
    winioScript.fcloseResult = (int)(rnd() % 3u);
    {
        static const char* names[6] = { "aaa_int 3.5", "bbb_float 2.25",
                                        "ccc_call 7", "ddd_none 1",
                                        "zzz_missing 4", "qqq_int -8.5" };
        strcpy(winioScript.dialogText, names[rnd() % 6u]);
    }
}

static void randomCase(Case* c)
{
    int i;
    memset(c, 0, sizeof(*c));
    c->clock = rndFloat(-1.0f, 4.0f);
    for (i = 0; i < 3; i++)
        c->mouse[i] = rndFloat(-40.0f, 40.0f);
    for (i = 0; i < 266; i++)
        c->keyboard[i] = (rnd() & 3u) ? 0 : 0x80;
}

/* ------------------------------------------------------------------ */

static void sweepPure(void)
{
    Case c;
    State st;
    long key = 0;
    int i, j;

    group = "sensitivity";
    for (i = -40; i <= 240; i++)
    {
        baseState(&st);
        memset(&winioScript, 0, sizeof(winioScript));
        randomCase(&c);
        c.op = OP_SENSITIVITY;
        c.f0 = (float)i / 100.0f;
        runCase(&c, &st, key++);
    }

    group = "statemachine";
    /* every state the dispatch can be in, against both inputs and a
       clock on either side of the deadline */
    for (i = 0; i <= 40; i++)
        for (j = 0; j < 8; j++)
        {
            baseState(&st);
            memset(&winioScript, 0, sizeof(winioScript));
            randomCase(&c);
            c.op = OP_STATEMACHINE;
            c.i0 = j & 1;
            c.f0 = (j & 2) ? 0.2f : 0.0f;
            st.stateMachineFlags = i;
            st.case1DividedTime2 = 1.0f;
            c.clock = (j & 4) ? 2.0f : 0.5f;
            runCase(&c, &st, key++);
        }

    group = "statemachine/ontime";
    /* the clock exactly on the deadline, which is the only input that
       separates > from >= there */
    for (i = 0; i <= 40; i++)
        for (j = 0; j < 4; j++)
        {
            baseState(&st);
            memset(&winioScript, 0, sizeof(winioScript));
            randomCase(&c);
            c.op = OP_STATEMACHINE;
            c.i0 = j & 1;
            c.f0 = (j & 2) ? 0.2f : 0.0f;
            st.stateMachineFlags = i;
            st.case1DividedTime2 = 1.5f;
            c.clock = 1.5f;
            runCase(&c, &st, key++);
        }

    group = "mouseamount";
    for (i = 0; i < 4000; i++)
    {
        baseState(&st);
        memset(&winioScript, 0, sizeof(winioScript));
        randomCase(&c);
        c.op = OP_MOUSEAMOUNT;
        st.case4X = rndFloat(-6.0f, 6.0f);
        st.case4XD = rndFloat(0.0f, 2.0f);
        c.f0 = rndFloat(0.0f, 4.0f);
        c.clock = rndFloat(0.0f, 2.0f);
        runCase(&c, &st, key++);
    }

    group = "initvars";
    for (i = -2; i <= 7; i++)
        for (j = 0; j <= 40; j++)
        {
            baseState(&st);
            memset(&winioScript, 0, sizeof(winioScript));
            randomCase(&c);
            c.op = OP_INITVARS;
            c.i0 = i;
            st.stateMachineFlags = j;
            st.joy[0].x = 3.0f; st.joy[0].y = 4.0f; st.joy[1].x = 5.0f;
            runCase(&c, &st, key++);
        }
}

static void sweepHandlers(void)
{
    Case c;
    State st;
    long key = 100000;
    int h, i, sub;

    group = "align";
    rngState = 0x1111u;
    for (i = 0; i < 2000; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = OP_ALIGN;
        c.i0 = (int)(rnd() & 1u);
        runCase(&c, &st, key++);
    }

    group = "handlers";
    rngState = 0x2222u;
    for (h = 0; h < NHANDLER; h++)
        for (sub = -1; sub <= 11; sub++)
            for (i = 0; i < 160; i++)
            {
                randomState(&st);
                randomScript();
                randomCase(&c);
                c.op = OP_HANDLER;
                c.i0 = h;
                st.selSubmode = sub;
                runCase(&c, &st, key++);
            }

    /* the keyboard-driven arms need keys that are actually down, which
       a 1-in-4 random fill reaches too rarely to rely on */
    group = "handlers/keys";
    rngState = 0x3333u;
    for (h = 0; h < NHANDLER; h++)
        for (i = 0; i < 266; i++)
            for (sub = 0; sub < 4; sub++)
            {
                randomState(&st);
                randomScript();
                randomCase(&c);
                c.op = OP_HANDLER;
                c.i0 = h;
                memset(c.keyboard, 0, sizeof(c.keyboard));
                c.keyboard[i] = 0x80;
                if (sub & 1)
                    c.keyboard[67] = c.keyboard[68] = 0x80;
                st.selSubmode = (int)(rnd() % 12u) - 1;
                runCase(&c, &st, key++);
            }

    /* nothing pressed at all. A 1-in-4 fill over 266 bytes never
       produces it, and it is the common case at run time. Its own
       seed base, so adding it moves nothing above. */
    group = "handlers/idle";
    rngState = 0x3A3Au;
    for (h = 0; h < NHANDLER; h++)
        for (sub = -1; sub <= 11; sub++)
            for (i = 0; i < 12; i++)
            {
                randomState(&st);
                randomScript();
                randomCase(&c);
                memset(c.keyboard, 0, sizeof(c.keyboard));
                c.op = OP_HANDLER;
                c.i0 = h;
                st.selSubmode = sub;
                runCase(&c, &st, key++);
            }
}

/* pcINO_InputHandlerCase4 at submode 0 and pcINO_InputHandlerCase100
   both normalise the left stick only when its length passes 0.1. The
   random sweep's magnitudes put the length in the tens, so the band
   around the bound needs driving on purpose. */
static void sweepNormalise(void)
{
    Case c;
    State st;
    long key = 300000;
    int h, i;

    group = "normalise";
    rngState = 0x7A7Au;
    for (h = 0; h < 2; h++)
        for (i = 0; i < 4000; i++)
        {
            randomState(&st);
            randomScript();
            randomCase(&c);
            c.op = OP_HANDLER;
            c.i0 = h ? 6 : 2;              /* Case100, Case4 */
            st.selSubmode = 0;
            st.mouseSensitivity = 1.0f;
            st.joy[0].x = rndFloat(-0.25f, 0.25f);
            st.joy[0].y = rndFloat(-0.25f, 0.25f);
            c.mouse[0] = rndFloat(-1.0f, 1.0f);
            c.mouse[1] = rndFloat(-1.0f, 1.0f);
            runCase(&c, &st, key++);
        }
}

static void sweepModes(void)
{
    Case c;
    State st;
    long key = 400000;
    int m, s2, i;

    group = "setmode";
    rngState = 0x4444u;
    for (m = -3; m <= 140; m++)
        for (i = 0; i < 3; i++)
        {
            randomState(&st);
            randomScript();
            randomCase(&c);
            c.op = OP_SETMODE;
            c.i0 = m;
            st.selMode = (int)(rnd() % 8u) == 0 ? m : (int)(rnd() % 140u);
            runCase(&c, &st, key++);
        }

    group = "setsubmode";
    rngState = 0x5555u;
    for (s2 = -6; s2 <= 14; s2++)
        for (m = 0; m < 8; m++)
            for (i = 0; i < 8; i++)
            {
                static const int modes[8] = { -1, 1, 2, 4, 8, 32, 64, 128 };
                randomState(&st);
                randomScript();
                randomCase(&c);
                c.op = OP_SETSUBMODE;
                c.i0 = s2;
                st.selMode = modes[m];
                st.selSubmode = (i & 1) ? s2 : (int)(rnd() % 12u);
                runCase(&c, &st, key++);
            }

    /* the eight arms of the mode switch, driven on purpose: the sweep
       above reaches each of them two or three times by accident */
    group = "setmode/named";
    rngState = 0x4A4Au;
    for (m = 0; m < 8; m++)
        for (i = 0; i < 120; i++)
        {
            static const int modes[8] = { 1, 2, 4, 8, 32, 64, 128, 129 };
            randomState(&st);
            randomScript();
            randomCase(&c);
            c.op = OP_SETMODE;
            c.i0 = modes[m];
            st.selMode = modes[(m + 1 + (int)(rnd() % 7u)) % 8];
            runCase(&c, &st, key++);
        }

    group = "setsubmode/named";
    rngState = 0x5A5Au;
    for (m = 0; m < 8; m++)
        for (s2 = 0; s2 <= 11; s2++)
            for (i = 0; i < 12; i++)
            {
                static const int modes[8] = { -1, 1, 2, 4, 8, 32, 64, 128 };
                randomState(&st);
                randomScript();
                randomCase(&c);
                c.op = OP_SETSUBMODE;
                c.i0 = s2;
                st.selMode = modes[m];
                st.selSubmode = s2 == 0 ? 1 : 0;
                runCase(&c, &st, key++);
            }
}

static void sweepNames(void)
{
    Case c;
    State st;
    long key = 600000;
    int i, j;

    group = "fournames";
    rngState = 0x6666u;
    for (i = 0; i < 6000; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = OP_FOURNAMES;
        c.i0 = (int)(rnd() % 22u);
        c.i1 = (int)(rnd() % 22u);
        c.i2 = (int)(rnd() % 22u);
        c.i3 = (int)(rnd() % 22u);
        for (j = 0; j < 18; j++)
            snprintf(st.buttonNames[j], 64, "n%d", j);
        runCase(&c, &st, key++);
    }

    group = "keynames";
    rngState = 0x7777u;
    for (i = 0; i < 300; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = OP_KEYNAMES;
        c.i0 = (int)(rnd() % 300u);
        st.haveKeyb = 1;
        runCase(&c, &st, key++);
    }

    group = "allkeynames";
    rngState = 0x8888u;
    for (i = 0; i < 400; i++)
    {
        int j;
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = OP_ALLKEYNAMES;
        st.haveKeyb = 1;
        for (j = 0; j < 18; j++)
            memset(st.buttonNames[j], 0xBB, 63);
        /* drive the five special table entries on purpose as well as
           at random */
        if (i < 100)
        {
            memcpy(st.keybLink, linkDefault, sizeof(linkDefault));
            st.keybLink[(i % 18) * 2] = (uint16_t)((const uint16_t[]){ 57, 256, 257, 258, 259, 260, 16 })[i % 7];
        }
        runCase(&c, &st, key++);
    }
}

static void sweepDevices(void)
{
    Case c;
    State st;
    long key = 800000;
    int i, dev;

    group = "update";
    rngState = 0x9999u;
    for (i = 0; i < 8000; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = OP_UPDATE;
        st.haveMouse = (int)(rnd() % 4u) != 0;
        st.haveKeyb = (int)(rnd() % 4u) != 0;
        /* drive the two trailing key combinations deliberately */
        if (st.haveKeyb)
        {
            if ((rnd() & 3u) == 0) winioScript.keybData[56] = 0x80;
            if ((rnd() & 3u) == 0) winioScript.keybData[184] = 0x80;
            if ((rnd() & 3u) == 0) winioScript.keybData[62] = 0x80;
            if ((rnd() & 3u) == 0) winioScript.keybData[88] = 0x80;
            if ((rnd() & 1u) == 0) st.keybBackup[88] = 0x80;
        }
        st.handlerIdx = (int)(rnd() % NHANDLER);
        runCase(&c, &st, key++);
    }

    group = "init/close";
    rngState = 0xAAAAu;
    for (i = 0; i < 3000; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        /* Two unconditional dereferences in pcINO_Joystick_Init are
           reachable and would crash both copies identically, so the
           scripts that reach them are kept out: the GetProperty on a
           mouse device the failure path has just set to NULL, and the
           key-name walk, which runs whether or not the keyboard
           device was created. */
        if (winioScript.diCreateResult >= 0)
        {
            if (winioScript.createMouseResult >= 0)
                winioScript.mouseSetFormat = winioScript.mouseSetCoop = 0;
            winioScript.createKeybResult = 0;
            winioScript.keybSetFormat = winioScript.keybSetCoop = 0;
        }
        c.op = (i & 1) ? OP_JOYCLOSE : OP_JOYINIT;
        st.haveMouse = (int)(rnd() & 1u);
        st.haveKeyb = (int)(rnd() & 1u);
        st.haveDI = (int)(rnd() & 1u);
        runCase(&c, &st, key++);
    }

    group = "acquire";
    rngState = 0xBBBBu;
    for (i = 0; i < 400; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = (i & 1) ? OP_ACQUIRE : OP_UNACQUIRE;
        st.haveMouse = (int)(rnd() & 1u);
        st.haveKeyb = (int)(rnd() & 1u);
        runCase(&c, &st, key++);
    }

    group = "misc";
    rngState = 0xCCCCu;
    for (i = 0; i < 600; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        switch (i % 4)
        {
        case 0: c.op = OP_JOYMOVE; c.i0 = (int)(rnd() & 1u); break;
        case 1: c.op = OP_SETPAD;  c.i0 = (int)(rnd() % 4u) - 1; break;
        case 2: c.op = OP_MOUSEUSED; break;
        default:
            c.op = OP_GETPROPERTY;
            c.i0 = (int)(rnd() & 1u);
            c.i1 = (int)(rnd() % 20u);
            c.i2 = (int)(rnd() % 4u);
            c.i3 = (int)(rnd() % 8u);
            break;
        }
        runCase(&c, &st, key++);
    }

    group = "dialog";
    rngState = 0xDDDDu;
    for (dev = 0; dev < 6; dev++)
        for (i = 0; i < 200; i++)
        {
            randomState(&st);
            randomScript();
            randomCase(&c);
            c.op = OP_DLGPROC;
            c.i0 = ((const int[6]){ 272, 273, 0, 16, 274, 273 })[dev];
            c.i1 = (int)(rnd() % 5u);
            c.i2 = (int)rnd();
            runCase(&c, &st, key++);
        }

    group = "getproperty";
    rngState = 0xC1C1u;
    for (i = 0; i < 400; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        winioScript.getPropertyResult = (i & 1) ? 0 : -1;
        winioScript.getPropertyData = (unsigned)(rnd() % 20u);
        c.op = OP_GETPROPERTY;
        c.i0 = (int)(rnd() & 1u);
        c.i1 = (int)(rnd() % 20u);
        c.i2 = (int)(rnd() % 4u);
        c.i3 = (int)(rnd() % 8u);
        runCase(&c, &st, key++);
    }

    group = "tweakflags";
    rngState = 0xEEEEu;
    for (i = 0; i < 2000; i++)
    {
        randomState(&st);
        randomScript();
        randomCase(&c);
        c.op = OP_TWEAKFLAGS;
        st.haveMouse = (int)(rnd() & 1u);
        st.haveKeyb = (int)(rnd() & 1u);
        runCase(&c, &st, key++);
    }
}

static void sweepCard(void)
{
    Case c;
    State st;
    long key = 1000000;
    int f, i;

    group = "card";
    rngState = 0xF0F0u;
    for (f = 0; f < NCARD; f++)
        for (i = 0; i < 400; i++)
        {
            randomState(&st);
            randomScript();
            randomCase(&c);
            c.op = OP_CARD;
            c.i0 = f;
            /* the open path turns on the two command values and the
               free-space boundary that select its arms */
            if ((i & 3) == 0)
                st.savCommand = 16;
            else if ((i & 3) == 1)
                st.savCommand = 10;
            if ((i & 7) == 0)
                st.freeSpace = SAV_PHOTO_BUFFER_SIZE;
            else if ((i & 7) == 1)
                st.freeSpace = SAV_PHOTO_BUFFER_SIZE - 1;
            else if ((i & 7) == 2)
                st.freeSpace = 1024;
            else if ((i & 7) == 3)
                st.freeSpace = 1023;
            st.savFp = (i & 4) ? (void*)&st : NULL;
            runCase(&c, &st, key++);
        }

    /* every failure flag clear, which is what the shipped build has:
       nothing in the tree writes any of the six. The random sweep
       above sets each from rnd()%4, so all six clear at once is a
       1-in-4096 event it did not once produce. */
    group = "card/noflags";
    rngState = 0xF1F1u;
    for (f = 0; f < NCARD; f++)
        for (i = 0; i < 40; i++)
        {
            int j;
            randomState(&st);
            randomScript();
            randomCase(&c);
            for (j = 0; j < 6; j++)
                st.savFail[j] = 0;
            c.op = OP_CARD;
            c.i0 = f;
            st.savCommand = ((const int[4]){ 16, 10, 0, 23 })[i & 3];
            st.freeSpace = ((const int[4]){ SAV_PHOTO_BUFFER_SIZE,
                                            SAV_PHOTO_BUFFER_SIZE - 1,
                                            1024, 1023 })[(i >> 2) & 3];
            st.savFp = (i & 16) ? (void*)&st : NULL;
            runCase(&c, &st, key++);
        }
}

int main(void)
{
    int i;

    printf("=== wininputoutput differential test ===\n");

    sweepPure();
    printf("  after pure:     %ld cases\n", cases);
    sweepHandlers();
    printf("  after handlers: %ld cases\n", cases);
    sweepNormalise();
    printf("  after normalise:%ld cases\n", cases);
    sweepModes();
    printf("  after modes:    %ld cases\n", cases);
    sweepNames();
    printf("  after names:    %ld cases\n", cases);
    sweepDevices();
    printf("  after devices:  %ld cases\n", cases);
    sweepCard();
    printf("  after card:     %ld cases\n", cases);

    for (i = 0; i < NCOV; i++)
    {
        printf("  cov %-42s %ld%s\n", cov[i].name, cov[i].n,
               cov[i].n ? "" : "   <- NEVER TAKEN");
        if (!cov[i].n)
            diffs++;
    }

    printf("coverage by entry point:");
    for (i = 0; i < OP_NUM; i++)
    {
        printf(" %d:%ld", i, opRan[i]);
        if (!opRan[i])
        {
            printf(" <- NEVER RUN");
            diffs++;
        }
    }
    printf("\n");

    printf("%ld cases, %ld differences\n", cases, diffs);
    if (diffs)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
