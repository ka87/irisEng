#include "math.h"

/* The processor's own cycle counter, which is what every clock in here
   is built on. Microsoft's compiler has __rdtsc as an intrinsic and
   declares it in <intrin.h>; gcc and clang have the same intrinsic
   under the same name in <x86intrin.h>. One name, two headers.

   x86 either way, as the comment on f2iMagic in math.h is: this is a
   32-bit x86 build. */
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#include "time.h"
#include "profiler.h"
#include "engine.h"
#include "platform.h"

float TIM_gf_SpeedFactor = 1.0f;
/* The shipped value. See time.h for what it does and why it is a
   variable rather than the literal it was. */
float TIM_gf_FrameFloor = 0.0099999998f;
/* What a frame is held back to, and nought when it is not. Separate
   from the floor because the two part company above a hundred frames
   a second: the floor is then lowered to meet this, not the other way
   about. */
static float TIM_l_gf_FramePeriod = 0.0099999998f;
float TIM_PrevCurrClock = 0.0f; // game doesn't explicitly initializes it to 0, but we do it
BOOL TIM_ShouldMeasureTicks = TRUE;
int TIM_PrevTicksPerSecond = 0; // game doesn't explicitly initializes it to 0, but we do it

void TIM_GetTicks(uint64_t* this)
{
    *this = __rdtsc();
}

unsigned int TIM_GetTicksAsInt32()
{
    return (unsigned int)__rdtsc();
}

unsigned int TIM_ul_QuickGetTicksPerSecond()
{
    unsigned int start;
    unsigned int startTick;
    unsigned int endTick;

    timeBeginPeriod(1u);
    start = timeGetTime();
    startTick = TIM_GetTicksAsInt32();
    
    while (timeGetTime() - start < 250);
    
    endTick = TIM_GetTicksAsInt32();
    timeEndPeriod(1u);
    return 4 * (endTick - startTick);
}

float TIM_f_Clock_TrueRead()
{
    uint64_t currTime;

    TIM_GetTicks(&currTime);

    return (float)((((double)(currTime >> 32) - (double)(uint32_t)(TIM_gf_MainClockReal >> 32)) * 4294967300.0
        + (double)(uint32_t)(currTime & 0xffffffff)
        - (double)(uint32_t)(TIM_gf_MainClockReal & 0xffffffff))
        / (double)(uint32_t)TIM_gul_TicksPerSecond);
}

void TIM_Clock_Reset()
{
    TIM_GetTicks(&TIM_gf_MainClockReal);
    TIM_gul_TicksCount = 0.0f;
    TIM_gf_MainClock = 0.0f;
}

float TIM_f_Clock_ElapsedTime()
{
    uint64_t currTime;

    TIM_GetTicks(&currTime);
    return (float)(((double)(currTime >> 32) - (double)(uint32_t)(TIM_gf_MainClockReal >> 32)) * 4294967300.0
        + (double)(uint32_t)(currTime & 0xffffffff)
        - (double)(uint32_t)(TIM_gf_MainClockReal & 0xffffffff));
}

void TIM_InitModule()
{
    TIM_gul_TicksPerSecond = TIM_ul_QuickGetTicksPerSecond();
    TIM_SystemTimeMsAtAppStart = timeGetTime();
    TIM_GetTicks(&TIM_gf_MainClockReal);
    TIM_gul_TicksCount = 0.0;
    TIM_gf_MainClock = 0.0;
    TIM_SpentTimeWithHandlingWinMessages = 0.0;
    TIM_gf_EngineLoopStartTime = TIM_f_Clock_TrueRead();
    TIM_gf_EngineLoopEndTime = TIM_gf_EngineLoopStartTime;
}

void TIM_Clock_Update()
{
    float ticksCount;
    float dtTicks;
    float prevTicksCount;

    prevTicksCount = TIM_gul_TicksCount;
    ticksCount = TIM_f_Clock_TrueRead() - TIM_SpentTimeWithHandlingWinMessages;
    TIM_gul_TicksCount = ticksCount;
    dtTicks = ticksCount - prevTicksCount;
    if (dtTicks > TIM_gf_FrameFloor)
    {
        if (dtTicks > 0.26666668f)
            dtTicks = 0.26666668f;
    }
    else
    {
        dtTicks = TIM_gf_FrameFloor;
    }
    TIM_dtTicks = dtTicks;
    PRO_EngineRealDT.unk2 = TIM_dtTicks;
    TIM_gf_dt = TIM_gf_SpeedFactor * dtTicks;
    PRO_EngineDt.unk2 = TIM_gf_dt;
       
    if (MAI_gst_MainHandles.displayData && (MAI_gst_MainHandles.displayData->presentationFlags & 0x800000) != 0)
        TIM_gf_dt = 0.016666666f;
    TIM_gf_MainClock = TIM_gf_MainClock + TIM_gf_dt;
}

void TIM_SetFrameLimit(int fps)
{
    if (fps <= 0)
    {
        /* The floor goes back with it. Turning the limit off is meant
           to leave the clock exactly as the game shipped, and a floor
           left where an earlier limit had put it would not be that. */
        TIM_l_gf_FramePeriod = 0.0f;
        TIM_gf_FrameFloor = 0.0099999998f;
        return;
    }
    TIM_l_gf_FramePeriod = 1.0f / (float)fps;
    /* Drawing faster than the floor allows means the floor has to give
       way, or the world would run fast however well the frames are
       paced. The other direction leaves it alone: a slower limit is
       satisfied by the shipped constant already. */
    if (TIM_l_gf_FramePeriod < 0.0099999998f)
        TIM_gf_FrameFloor = TIM_l_gf_FramePeriod;
    else
        TIM_gf_FrameFloor = 0.0099999998f;
}

void TIM_PaceFrame(void)
{
    float elapsed;
    float left;

    if (TIM_l_gf_FramePeriod <= 0.0f)
        return;
    for (;;)
    {
        /* The same reading the clock is about to take, against the one
           it took last time, so what is waited out is exactly what
           would otherwise have been counted as the floor. */
        elapsed = TIM_f_Clock_TrueRead() - TIM_SpentTimeWithHandlingWinMessages
                  - TIM_gul_TicksCount;
        /* A frame already past the period is nothing to wait for, and
           neither is a reading that makes no sense: written this way
           round so that a reading which has gone backwards -- the
           clock re-based under us -- and one that is not a number at
           all both leave rather than spin. The second cannot happen
           while the clock is sound, and a wait that could hang the
           game on an unsound one is not worth the risk. */
        if (!(elapsed >= 0.0f) || elapsed >= TIM_l_gf_FramePeriod)
            return;
        left = TIM_l_gf_FramePeriod - elapsed;
        /* Give the rest of the slice back while there is a whole
           millisecond of it to give, and spin out the remainder: a
           sleep cannot be trusted to wake inside a millisecond, and
           overshooting here would cost a frame rather than save one. */
        if (left > 0.0015f)
            PLT_SleepMs((int)((left - 0.0005f) * 1000.0f));
    }
}

int TIM_TimeDriftPerSecond()
{
    float timeElapsed;

    timeElapsed = (float)(timeGetTime() - TIM_SystemTimeMsAtAppStart);
    
    return f2iMagic(TIM_f_Clock_ElapsedTime() / timeElapsed * 1000.0f);
}

void TIM_MeasureTicksPerSecond()
{
    int newTicksPerSecond;
    int dtTiskPerLoop;

    if (TIM_ShouldMeasureTicks && TIM_f_Clock_TrueRead() - TIM_PrevCurrClock > 5.0f)
    {
        TIM_PrevCurrClock = TIM_f_Clock_TrueRead();
        TIM_PrevTicksPerSecond = TIM_gul_TicksPerSecond;
        newTicksPerSecond = TIM_TimeDriftPerSecond();
        TIM_gul_TicksPerSecond = newTicksPerSecond;
        if (newTicksPerSecond - TIM_PrevTicksPerSecond < 0)
            dtTiskPerLoop = TIM_PrevTicksPerSecond - newTicksPerSecond;
        else
            dtTiskPerLoop = newTicksPerSecond - TIM_PrevTicksPerSecond;
        if (dtTiskPerLoop < 40000)
            TIM_ShouldMeasureTicks = FALSE;
    }
}