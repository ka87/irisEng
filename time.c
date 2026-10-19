#include "math.h"
#include "time.h"
#include "profiler.h"
#include "engine.h"

float TIM_gf_SpeedFactor = 1.0f;
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
    if (dtTicks > 0.0099999998f)
    {
        if (dtTicks > 0.26666668f)
            dtTicks = 0.26666668f;
    }
    else
    {
        dtTicks = 0.0099999998f;
    }
    TIM_dtTicks = dtTicks;
    PRO_EngineRealDT.unk2 = TIM_dtTicks;
    TIM_gf_dt = TIM_gf_SpeedFactor * dtTicks;
    PRO_EngineDt.unk2 = TIM_gf_dt;
       
    if (MAI_gst_MainHandles.displayData && (MAI_gst_MainHandles.displayData->presentationFlags & 0x800000) != 0)
        TIM_gf_dt = 0.016666666f;
    TIM_gf_MainClock = TIM_gf_MainClock + TIM_gf_dt;
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