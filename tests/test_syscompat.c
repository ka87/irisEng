/* The system services the engine asks for by Windows' names, and the
 * answers this platform gives them.
 *
 * syscompat.c is compiled into the game and, until this, into nothing
 * the tests build -- so the implementations the engine would run off
 * Windows had never been run at all. They are small, and small is not
 * the same as right: a clock that does not advance, or a debugger
 * check that says yes when nothing is attached, would each be quiet
 * and wrong.
 *
 * What cannot be checked here is the Windows half, which is the
 * platform's own and is reached by a different branch of the same
 * file. The branch under test is the one that had no implementation
 * before.
 */

#include <stdio.h>
#include <string.h>

#include "../platform/syscompat.c"

static int checks;
static int failures;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        failures++;
    }
}

static void theDebuggerCheck(void)
{
    int first, second;

    printf("-- whether a debugger has this process\n");

    first = PLT_b_DebuggerAttached();
    ok(first == 0 || first == 1, "the answer is yes or no and nothing else");

    /* Nothing is attached to a test run, so the answer is no. If this
       fails, the run really is under a debugger -- or TracerPid is
       being read out of the wrong line. */
    ok(first == 0, "and with nothing attached it is no");

    /* Asked twice because it is read afresh each time rather than
       remembered: a debugger may attach after the first answer. */
    second = PLT_b_DebuggerAttached();
    ok(second == first, "and asking twice gives the same answer");
}

#ifndef _WIN32
static void thePerformanceCounter(void)
{
    LARGE_INTEGER freq, a, b;
    int i;
    volatile double spin = 0.0;

    printf("-- the performance counter\n");

    ok(QueryPerformanceFrequency(&freq) != 0, "the frequency is answered");
    ok(freq.QuadPart > 0, "and is not nought, which would divide badly");

    ok(QueryPerformanceCounter(&a) != 0, "the counter is answered");
    for (i = 0; i < 200000; i++)
        spin += (double)i;
    ok(QueryPerformanceCounter(&b) != 0, "and again");
    ok(b.QuadPart > a.QuadPart, "and it has gone forward");

    /* A counter that runs at its stated frequency: the work above is
       nowhere near a second, so the difference divided by the
       frequency must be well under one. */
    ok((double)(b.QuadPart - a.QuadPart) / (double)freq.QuadPart < 1.0,
       "at about the frequency it claims");

    ok(QueryPerformanceCounter(NULL) == 0, "and nowhere to put it is a failure");
    (void)spin;
}

static void theMillisecondClocks(void)
{
    DWORD a, b;

    printf("-- the millisecond clocks\n");

    a = timeGetTime();
    ok(a < 60000u, "the first reading is small, being measured from the first call");

    /* Both read the same clock from the same moment, so one is never
       wildly ahead of the other. */
    b = GetTickCount();
    ok(b + 1000u >= a && a + 1000u >= b, "and the two of them agree");
}

static void theDebugOutput(void)
{
    printf("-- saying something where a debugger would hear it\n");
    /* No answer to check: what is wanted is that it neither crashes
       nor minds being given nothing. */
    OutputDebugStringA("");
    OutputDebugStringA(NULL);
    ok(1, "it takes a string, and nothing at all");
}
#endif

int main(void)
{
    theDebuggerCheck();
#ifndef _WIN32
    thePerformanceCounter();
    theMillisecondClocks();
    theDebugOutput();
#endif

    printf("  %d checks, %d failure(s)\n", checks, failures);
    if (failures)
        printf("FAIL\n");
    return failures ? 1 : 0;
}
