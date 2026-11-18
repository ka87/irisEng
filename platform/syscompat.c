#include "syscompat.h"

#ifdef _WIN32

#include <windows.h>

int PLT_b_DebuggerAttached(void)
{
    return IsDebuggerPresent() ? 1 : 0;
}

int PLT_b_OnlyInstance(void)
{
    /* Never closed on purpose: the mutex has to outlive the call and
       last as long as the process, and the system drops it when the
       process goes. Kept in a static so asking twice answers the same
       thing rather than testing the handle this call just made. */
    static HANDLE held;

    if (held)
        return 1;
    held = CreateMutexA(NULL, TRUE, "IrisEngGame");
    if (!held)
        return 1;
    return GetLastError() == ERROR_ALREADY_EXISTS ? 0 : 1;
}

#else /* not Windows */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <time.h>
#include <unistd.h>

#include "../datapath.h"

/* For LARGE_INTEGER, DWORD and the rest: the implementations below
   have to match the declarations the engine already sees, and off
   Windows those come from whatever windows.h the build supplies --
   the tests have one of their own, and a build that has none cannot
   compile any of the engine, not only this file. */
#include "../common.h"

int PLT_b_OnlyInstance(void)
{
    /* A lock on a file beside the saves. The lock and not the file is
       what matters: the file is left behind and the lock is not, so a
       copy that crashed releases it on the way out and the next one
       starts.

       Beside the saves because the guard is per player rather than
       per machine -- two people logged in at once are two games, and
       a lock in a shared place would have the second refuse to
       start. */
    static int held = -1;
    char       path[512];
    int        fd;

    if (held >= 0)
        return 1;
    if (!DPT_b_UserPath("irisEng.lock", path, sizeof(path)))
        return 1;
    fd = open(path, O_CREAT | O_RDWR, 0644);
    if (fd < 0)
        return 1;
    if (flock(fd, LOCK_EX | LOCK_NB) != 0)
    {
        close(fd);
        return 0;
    }
    /* Held, and not closed: closing it drops the lock. */
    held = fd;
    return 1;
}

int PLT_b_DebuggerAttached(void)
{
    /* TracerPid is the pid of whatever has this process under ptrace,
       and nought when nothing does. Reading it each time it is asked
       rather than once, because a debugger may attach later. */
    FILE* status;
    char  line[256];
    int   tracer = 0;

    status = fopen("/proc/self/status", "r");
    if (!status)
        return 0;
    while (fgets(line, (int)sizeof(line), status))
    {
        if (strncmp(line, "TracerPid:", 10) == 0)
        {
            tracer = atoi(line + 10);
            break;
        }
    }
    fclose(status);
    return tracer != 0;
}

/* ------------------------------------------------------------------
   The Win32 clock and debug output, off Windows.

   The engine says these in Windows' words in about a hundred places,
   and they all mean one of two things: what time is it, and say this
   where a debugger would hear it. So they keep their names and get an
   implementation rather than a hundred call sites being rewritten to
   say the same thing differently.

   One clock underneath all of them: CLOCK_MONOTONIC, which is what
   both of Windows' are. The performance counter is in nanoseconds,
   which is finer than Windows' own and perfectly legal -- the engine
   divides by the frequency it is told.
   ------------------------------------------------------------------ */

static long long PLT_l_ll_Nanoseconds(void)
{
    struct timespec now;

    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0)
        return 0;
    return (long long)now.tv_sec * 1000000000LL + (long long)now.tv_nsec;
}

int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    if (!out)
        return 0;
    out->QuadPart = PLT_l_ll_Nanoseconds();
    return 1;
}

BOOL QueryPerformanceFrequency(LARGE_INTEGER* out)
{
    if (!out)
        return 0;
    out->QuadPart = 1000000000LL;
    return 1;
}

/* Milliseconds since some fixed moment, which is all either of these
   ever meant: the engine only ever subtracts two of them. The first
   call fixes the moment, so the figures stay small and a difference
   is the same whichever is asked. */
static DWORD PLT_l_Milliseconds(void)
{
    static long long base;
    long long now = PLT_l_ll_Nanoseconds();

    if (base == 0)
        base = now;
    return (DWORD)((now - base) / 1000000LL);
}

DWORD timeGetTime(void)  { return PLT_l_Milliseconds(); }
DWORD GetTickCount(void) { return PLT_l_Milliseconds(); }

/* Asking the system for a finer timer, and giving it back.
 *
 * Nothing to do here. Those two ask Windows to run its scheduler's
 * clock at a stated resolution, because the clock the engine reads
 * through them is as coarse as that clock is. The clock above is
 * CLOCK_MONOTONIC, which is as fine as the hardware allows whether
 * anybody asks or not, so there is no resolution to request.
 *
 * Nought is what the real pair answer on success, so a caller that
 * checks -- none of them do -- reads it as having worked, which it
 * did: the engine asked for a millisecond and has better. */
DWORD timeBeginPeriod(UINT period) { (void)period; return 0; }
DWORD timeEndPeriod(UINT period)   { (void)period; return 0; }

void OutputDebugStringA(LPCSTR text)
{
    /* Where a debugger would have heard it. Nothing attaches to a
       stream the way a debugger attaches to a process, so standard
       error is the nearest thing, and it is where the engine's own
       log already goes. */
    if (text)
        fputs(text, stderr);
}

#endif /* not Windows */
