#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "io_mocks.h"
#include "../memory.h"
#include "../dbg.h"
#include "../inputoutput.h"
#include "../win/wininputoutput.h"

tIoMockCall IOMOCK_log[IOMOCK_LOG_MAX];
int IOMOCK_logLen;
unsigned char* IOMOCK_arena;
size_t IOMOCK_used;
int IOMOCK_rets[IOMOCK_RETS_MAX];
int IOMOCK_retIdx;
int IOMOCK_retNum;
int IOMOCK_async;
int IOMOCK_mouseInUse;
int IOMOCK_setPadResult;
float IOMOCK_joyMoveX;
float IOMOCK_joyMoveY;
float IOMOCK_joyMoveZ;
long IOMOCK_now;

static unsigned char* s_arena;

void IOMOCK_Log(const char* name, long a, long b, const char* text)
{
    if (IOMOCK_logLen < IOMOCK_LOG_MAX)
    {
        IOMOCK_log[IOMOCK_logLen].name = name;
        IOMOCK_log[IOMOCK_logLen].a = a;
        IOMOCK_log[IOMOCK_logLen].b = b;
        IOMOCK_log[IOMOCK_logLen].text[0] = 0;
        if (text)
        {
            strncpy(IOMOCK_log[IOMOCK_logLen].text, text,
                    sizeof(IOMOCK_log[IOMOCK_logLen].text) - 1);
            IOMOCK_log[IOMOCK_logLen].text[sizeof(IOMOCK_log[IOMOCK_logLen].text) - 1] = 0;
        }
    }
    ++IOMOCK_logLen;
}

long IOMOCK_Offset(const void* p)
{
    if (!p)
        return -1;
    return (long)((const unsigned char*)p - s_arena);
}

void IOMOCK_Reset(void)
{
    if (!s_arena)
    {
        s_arena = (unsigned char*)malloc(IOMOCK_ARENA_SIZE);
        if (!s_arena)
        {
            fprintf(stderr, "io mock arena allocation failed\n");
            exit(2);
        }
        memset(s_arena, 0xCD, IOMOCK_ARENA_SIZE);
    }
    else
    {
        /* Poison only what the previous run touched; the arena is
           8MB and the cases run in the hundreds. */
        memset(s_arena, 0xCD, IOMOCK_used);
    }
    IOMOCK_arena = s_arena;
    IOMOCK_used = 0;
    IOMOCK_logLen = 0;
    IOMOCK_retIdx = 0;
}

/* Nothing is handed back between the two sides, so the first side's
   state stays readable while the second runs, and every block sits 32
   bytes into a larger one so a copy that writes a little way outside
   it does not take the process with it. */
void* MEM_p_Alloc(unsigned int size)
{
    void* p;

    IOMOCK_Log("MEM_p_Alloc", (long)size, 0, 0);
    if (IOMOCK_used + size + 128 > IOMOCK_ARENA_SIZE)
    {
        fprintf(stderr, "io mock arena exhausted\n");
        exit(2);
    }
    IOMOCK_used += 32;
    p = s_arena + IOMOCK_used;
    IOMOCK_used += (size + 64 + 15) & ~(size_t)15;
    return p;
}

void MEM_Free(void* p)
{
    IOMOCK_Log("MEM_Free", IOMOCK_Offset(p), 0, 0);
}

void DBG_OutputDebugString(char* Format, ...)
{
    char buf[512];
    va_list ap;

    va_start(ap, Format);
    vsnprintf(buf, sizeof(buf), Format, ap);
    va_end(ap);
    IOMOCK_Log("DBG_OutputDebugString", 0, 0, buf);
}

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine,
                const char* error, const char* extra, int showMsgBox)
{
    /* The line number is not compared: the frozen copy and the
       rewrite carry the same assert at different lines, and __LINE__
       is the one thing about it that is expected to move. */
    IOMOCK_Log("DBG_assert", isError, 0, error);
}

/* ------------------------------------------------------------------ */
/* The joystick and mouse back end.                                    */

void pcINO_Joystick_Init(HWND hwnd)
{
    IOMOCK_Log("pcINO_Joystick_Init", (long)(intptr_t)hwnd, 0, 0);
}

void pcINO_Joystick_Close(void)
{
    IOMOCK_Log("pcINO_Joystick_Close", 0, 0, 0);
}

void pcINO_Joystick_Update(struct sInoPad* padData)
{
    IOMOCK_Log("pcINO_Joystick_Update", 0, 0, 0);
}

void pcINO_JoystickPifPafEnable(BOOL b)
{
    IOMOCK_Log("pcINO_JoystickPifPafEnable", b, 0, 0);
}

void pcINO_JoystickPafIntensity(int i1)
{
    IOMOCK_Log("pcINO_JoystickPafIntensity", i1, 0, 0);
}

void pcINO_Joystick_Move(Vec3* v, int stickID)
{
    IOMOCK_Log("pcINO_Joystick_Move", stickID, 0, 0);
    v->x = IOMOCK_joyMoveX;
    v->y = IOMOCK_joyMoveY;
    v->z = IOMOCK_joyMoveZ;
}

int pcINO_i_IsMouseBeingUsed(void)
{
    IOMOCK_Log("pcINO_i_IsMouseBeingUsed", IOMOCK_mouseInUse, 0, 0);
    return IOMOCK_mouseInUse;
}

int pcINO_i_SetCurrentPad(int pad)
{
    IOMOCK_Log("pcINO_i_SetCurrentPad", pad, 0, 0);
    return IOMOCK_setPadResult;
}

void pcINO_SetMouseSensitivity(float t)
{
    IOMOCK_Log("pcINO_SetMouseSensitivity", (long)(t * 1000.0f), 0, 0);
}

void pcINO_SetControlSubmode(int submode)
{
    IOMOCK_Log("SetControlSubmode_inner", submode, 0, 0);
}

void pcINO_SetControlMode(int mode)
{
    IOMOCK_Log("SetControlMode_inner", mode, 0, 0);
}

/* The frozen tests/baseline_inputoutput.c still calls these two by
   their old spelling. Forwarders rather than second bodies, so both
   sides log the same entry. */
void INO_SetControlSubmode_inner_sub_48A650(int submode)
{
    pcINO_SetControlSubmode(submode);
}

void INO_SetControlMode_inner_sub_48ACB0(int mode)
{
    pcINO_SetControlMode(mode);
}

/* ------------------------------------------------------------------ */
/* The memory-card handlers. Each logs its own name and hands back the
   next scripted state, so the driver drives the whole state machine
   from one table. */

static int nextRet(const char* name)
{
    int r;

    r = IOMOCK_retNum ? IOMOCK_rets[IOMOCK_retIdx % IOMOCK_retNum] : 0;
    ++IOMOCK_retIdx;
    IOMOCK_Log(name, r, 0, 0);
    return r;
}

int pcINO_b_AsyncInProgress(void)
{
    IOMOCK_Log("pcINO_b_AsyncInProgress", IOMOCK_async, 0, 0);
    return IOMOCK_async;
}

int pcINO_SavModuleInit(void) { return nextRet("pcINO_SavModuleInit"); }
int pcINO_SavModuleClose(void) { return nextRet("pcINO_SavModuleClose"); }
int pcINO_i_TestPresence(void) { return nextRet("pcINO_i_TestPresence"); }
int pcINO_i_MountCard(void) { return nextRet("pcINO_i_MountCard"); }
int pcINO_i_UnmountCard(void) { return nextRet("pcINO_i_UnmountCard"); }
int pcINO_i_CheckCard(void) { return nextRet("pcINO_i_CheckCard"); }
int pcINO_i_FormatCard(void) { return nextRet("pcINO_i_FormatCard"); }
int pcINO_i_TestFreeSpace(void) { return nextRet("pcINO_i_TestFreeSpace"); }
int pcINO_i_Retry(void) { return nextRet("pcINO_i_Retry"); }
int pcINO_i_Continue(void) { return nextRet("pcINO_i_Continue"); }
int pcINO_i_ActiveConsole(void) { return nextRet("pcINO_i_ActiveConsole"); }
int pcINO_i_Open(void) { return nextRet("pcINO_i_Open"); }
int pcINO_i_Close(void) { return nextRet("pcINO_i_Close"); }
int pcINO_i_ReadIndex_1(void) { return nextRet("pcINO_i_ReadIndex_1"); }
int pcINO_i_WriteIndex(void) { return nextRet("pcINO_i_WriteIndex"); }
int pcINO_i_ReadSlot_1(void) { return nextRet("pcINO_i_ReadSlot_1"); }
int pcINO_i_ReadSlot_2(void) { return nextRet("pcINO_i_ReadSlot_2"); }
int pcINO_i_ReadSlot_3(void) { return nextRet("pcINO_i_ReadSlot_3"); }
int pcINO_i_WriteSlot_C(void) { return nextRet("pcINO_i_WriteSlot_C"); }
int pcINO_i_WriteSlot_E(void) { return nextRet("pcINO_i_WriteSlot_E"); }
int pcINO_i_CheckFreeSpace(void) { return nextRet("pcINO_i_CheckFreeSpace"); }
int pcINO_i_TestCard(void) { return nextRet("pcINO_i_TestCard"); }

/* ------------------------------------------------------------------ */
/* A fixed clock, so the save-index timestamps are the same on both
   sides however long the run takes. */

time_t IOMOCK_time(time_t* t)
{
    if (t)
        *t = (time_t)IOMOCK_now;
    IOMOCK_Log("time", IOMOCK_now, 0, 0);
    return (time_t)IOMOCK_now;
}

struct tm* IOMOCK_localtime(const time_t* t)
{
    static struct tm tm;
    long v = t ? (long)*t : 0;

    memset(&tm, 0, sizeof(tm));
    tm.tm_sec = (int)(v % 60);
    tm.tm_min = (int)((v / 60) % 60);
    tm.tm_hour = (int)((v / 3600) % 24);
    tm.tm_mday = (int)((v / 86400) % 28) + 1;
    tm.tm_mon = (int)((v / 86400 / 28) % 12);
    tm.tm_year = 70 + (int)(v / 86400 / 365);
    IOMOCK_Log("localtime", v, 0, 0);
    return &tm;
}
