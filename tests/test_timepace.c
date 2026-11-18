/* What the clock counts a frame as, against how long it really took.
 *
 * The thing under test is a property of real elapsed time, so this
 * harness runs real frames and waits real waits rather than driving a
 * clock it has injected. It costs about a second and a half, most of
 * it the quarter second the clock spends measuring the processor at
 * startup and the three timed runs below.
 *
 * The property, in one line: **the world should advance by as much
 * time as actually passed.** The clock counts any frame shorter than
 * its floor as the floor, so a loop offering frames faster than that
 * advances the world faster than real time -- which is the bug this
 * is here to pin down. The cases drive the same loop at four limits
 * and compare the world's clock against a clock the harness keeps
 * itself.
 *
 * The tolerances are loose on purpose. Both clocks are real, the
 * machine is shared, and a sleep may be late; what is being checked is
 * a ratio that is either about one or about a hundred, so a few per
 * cent either way decides nothing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* The four things time.c reaches for. timeGetTime has to be real: the
   clock measures the processor's counter against it at startup and
   every reading afterwards is scaled by what that measurement said. */
static unsigned long tp_MsNow(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000ull + ts.tv_nsec / 1000000ull);
}

unsigned int timeGetTime(void) { return (unsigned int)tp_MsNow(); }
unsigned int timeBeginPeriod(unsigned int p) { (void)p; return 0; }
unsigned int timeEndPeriod(unsigned int p) { (void)p; return 0; }

/* A real sleep, for the same reason: a stub that returned at once
   would leave the pacing spinning and still pass, which would make
   the test say nothing about whether the wait works. */
void PLT_SleepMs(int ms)
{
    struct timespec ts;

    if (ms <= 0)
        return;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

#include "../time.h"

/* The harness's own clock, which the thing under test cannot touch. */
static double tp_Seconds(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static int tp_checks;
static int tp_failures;

static void tp_Check(int ok, const char* what, const char* detail)
{
    tp_checks++;
    if (!ok)
    {
        printf("FAIL: %s -- %s\n", what, detail);
        tp_failures++;
    }
}

/* One run of the loop: frames offered as fast as they can be, which is
   far faster than any display, and nothing in them but the clock. That
   is the case that matters -- a frame the loop can produce in
   microseconds is what a 144Hz display asks for and then some.

   Gives back how much the world advanced, how long that really took,
   and how many frames the clock had to put the floor under because
   they came in shorter than it. */
static void tp_Run(int frames, double* world, double* real, int* floored)
{
    double start;
    int    i;

    *floored = 0;
    TIM_Clock_Reset();
    /* One update to set the baseline the first paced frame measures
       against, and not counted: its own dt spans the reset. */
    TIM_Clock_Update();
    *world = 0.0;
    start = tp_Seconds();
    for (i = 0; i < frames; i++)
    {
        TIM_PaceFrame();
        TIM_Clock_Update();
        /* Exactly the floor means the substitution fired: a measured
           frame landing on it to the bit does not happen. */
        if (TIM_dtTicks == TIM_gf_FrameFloor)
            (*floored)++;
        *world += TIM_gf_dt;
    }
    *real = tp_Seconds() - start;
}

int main(void)
{
    double world, real;
    int    floored;
    char   msg[256];

    TIM_InitModule();

    /* ---- the floor is the shipped constant until something says
            otherwise ------------------------------------------------ */
    tp_Check(TIM_gf_FrameFloor == 0.0099999998f,
             "the floor starts at the value the game shipped with", "");

    /* ---- no limit: the bug, reproduced ------------------------------
       Frames arrive in microseconds, every one of them is counted as a
       hundredth of a second, and the world runs away. This case is
       here so that the ones below are measuring something: if this
       ever stops being fast, the loop has slowed down and the other
       cases prove nothing. */
    printf("-- with no limit, which is what the game shipped as\n");
    TIM_SetFrameLimit(0);
    tp_Run(400, &world, &real, &floored);
    sprintf(msg, "world %.4fs against real %.4fs, ratio %.1f",
            world, real, real > 0.0 ? world / real : 0.0);
    printf("   %s\n", msg);
    tp_Check(world > real * 2.0, "the world runs away when nothing holds frames back", msg);
    sprintf(msg, "%d of 400 frames took the floor", floored);
    tp_Check(floored > 390, "and nearly every frame is counted as the floor", msg);

    /* ---- the default limit ---------------------------------------- */
    printf("-- held back to the default, a hundred frames a second\n");
    TIM_SetFrameLimit(TIM_DEFAULT_MAX_FPS);
    tp_Check(TIM_gf_FrameFloor == 0.0099999998f,
             "the default limit leaves the shipped floor alone", "");
    tp_Run(40, &world, &real, &floored);
    sprintf(msg, "world %.4fs against real %.4fs, ratio %.3f",
            world, real, real > 0.0 ? world / real : 0.0);
    printf("   %s\n", msg);
    tp_Check(world > real * 0.85 && world < real * 1.15,
             "the world advances by about as long as really passed", msg);
    sprintf(msg, "%d of 40 frames took the floor", floored);
    tp_Check(floored == 0, "and no frame has the floor put under it", msg);
    sprintf(msg, "40 frames in %.4fs", real);
    tp_Check(real > 0.30, "which takes the time a hundred frames a second takes", msg);

    /* ---- a limit slower than the floor needs ----------------------
       Nothing to do with the bug, and the case that says so: the
       shipped floor already satisfies 50 frames a second, so it must
       be left exactly as it is. */
    printf("-- held back to fifty, which the shipped floor already allows\n");
    TIM_SetFrameLimit(50);
    tp_Check(TIM_gf_FrameFloor == 0.0099999998f,
             "a limit the floor already allows does not move it", "");
    tp_Run(20, &world, &real, &floored);
    sprintf(msg, "world %.4fs against real %.4fs, ratio %.3f",
            world, real, real > 0.0 ? world / real : 0.0);
    printf("   %s\n", msg);
    tp_Check(world > real * 0.85 && world < real * 1.15,
             "the world still advances by about as long as really passed", msg);
    tp_Check(floored == 0, "and still no frame takes the floor", "");

    /* ---- a limit faster than the floor allows ---------------------
       The other way out, and the one that costs the shipped constant:
       pacing alone cannot keep 144 frames a second honest, because the
       floor is longer than one of them. */
    printf("-- held back to 144, which the shipped floor does not allow\n");
    TIM_SetFrameLimit(144);
    sprintf(msg, "floor is now %.8f", (double)TIM_gf_FrameFloor);
    tp_Check(TIM_gf_FrameFloor < 0.0099999998f
             && TIM_gf_FrameFloor > 0.0069f && TIM_gf_FrameFloor < 0.0070f,
             "a limit faster than the floor lowers it to one frame", msg);
    tp_Run(72, &world, &real, &floored);
    sprintf(msg, "world %.4fs against real %.4fs, ratio %.3f",
            world, real, real > 0.0 ? world / real : 0.0);
    printf("   %s\n", msg);
    tp_Check(world > real * 0.85 && world < real * 1.15,
             "so the world keeps time at 144 frames a second too", msg);

    /* ---- and back ------------------------------------------------- */
    TIM_SetFrameLimit(0);
    tp_Check(TIM_gf_FrameFloor == 0.0099999998f,
             "turning the limit off puts the shipped floor back", "");

    printf("  %d checks, %d failure(s)\n", tp_checks, tp_failures);
    return tp_failures ? 1 : 0;
}
