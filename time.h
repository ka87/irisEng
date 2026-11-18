#pragma once
#include "common.h"
/* Neither the cycle counter's header nor winmm's is wanted here: this
   header declares globals and prototypes and names nothing out of
   either. time.c asks for the counter itself, under whichever of the
   two names the compiler keeps it, and the millisecond timer is
   declared with the rest of what the two libraries spell differently,
   in platform/syscompat.h, which common.h above has already read. */

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

/* The shortest frame the clock will count honestly.
 *
 * A frame that arrives sooner than this is counted as taking this
 * long, so the world advances further than real time did and the game
 * runs fast -- by exactly the ratio between the two. The frame rate
 * follows the display, both renderers presenting on the vertical
 * blank, so on a 120Hz display the game runs a fifth fast and on a
 * 144Hz display very nearly half again.
 *
 * The substitution is not a transcription error: it is what the
 * compiled game does, the same constant and the same branch. It was
 * harmless when a fullscreen mode meant 60Hz or thereabouts and
 * nothing presented faster than a hundred frames a second.
 *
 * So this starts at the value the game has always had and TIM_PaceFrame
 * keeps frames from arriving sooner, which is what makes the clock
 * honest again without changing its arithmetic. Lowering it is the
 * other way out and costs the arithmetic; see TIM_SetFrameLimit. */
extern float TIM_gf_FrameFloor;

/* Hold the frame back until the clock can count it honestly: waits
   until TIM_gf_FrameFloor has passed since the last clock update, so
   the next dt is a measurement rather than the floor. Called once a
   frame, immediately before TIM_Clock_Update. */
void TIM_PaceFrame(void);

/* How many frames a second the engine will run, which is the pacing
   above expressed the way a player thinks of it.

   Nought turns pacing off and leaves the clock exactly as the game
   shipped, fast display and all. Any other rate paces to 1/rate; a
   rate above what the floor allows also lowers the floor to match,
   which is a deliberate departure from the shipped constant and the
   only way to draw more than a hundred frames a second without the
   world running fast. */
void TIM_SetFrameLimit(int fps);

/* What the limit is when nothing says otherwise: the fastest the
   clock's shipped floor can count honestly, which is a hundred frames
   a second to the frame. Chosen so that the default changes the
   arithmetic not at all -- it only stops frames arriving sooner than
   the floor -- and so that a display at 60, 75 or 100 is unaffected,
   every frame on one of those already being longer than the floor. */
#define TIM_DEFAULT_MAX_FPS 100

void TIM_InitModule();
void TIM_MeasureTicksPerSecond();
float TIM_f_Clock_ElapsedTime();
float TIM_f_Clock_TrueRead();
unsigned int TIM_GetTicksAsInt32();
void TIM_Clock_Update();
void TIM_Clock_Reset();
