#pragma once
#include "common.h"
#include <intrin.h>
#include <timeapi.h>

unsigned int TIM_gul_TicksPerSecond;
unsigned int TIM_SystemTimeMsAtAppStart;
uint64_t TIM_gf_MainClockReal;
float TIM_gul_TicksCount;
float TIM_gf_MainClock;
float TIM_gf_LockTime; // TODO FIXME: initelni nem kéne? játék nem csinálja
float TIM_gf_LockBegTime;
float TIM_SpentTimeWithHandlingWinMessages;
float TIM_gf_EngineLoopStartTime;
float TIM_gf_EngineLoopEndTime;
float TIM_dtTicks;
float TIM_gf_dt;

extern float TIM_gf_SpeedFactor;

void TIM_InitModule();
void TIM_MeasureTicksPerSecond();
float TIM_f_Clock_ElapsedTime();
float TIM_f_Clock_TrueRead();
unsigned int TIM_GetTicksAsInt32();
void TIM_Clock_Update();
void TIM_Clock_Reset();
