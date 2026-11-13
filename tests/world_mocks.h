#pragma once

#define WMOCK_LOG_MAX  60000
#define WMOCK_LOG_LINE 112

void WMOCK_LogReset(void);
int WMOCK_LogNum(void);
const char* WMOCK_LogLine(int i);
int WMOCK_LogOverflowed(void);
void WMOCK_Logf(const char* fmt, ...);

void WMOCK_HeapReset(void);
void* WMOCK_HeapBase(void);
unsigned int WMOCK_HeapUsed(void);

/* the mocked engine's own clock, so both sides see the same ticks */
void WMOCK_ClockReset(void);

/* what OBJ_CullingObject and OBJ_uc_CalcLODVis hand back, keyed off the
   object so the driver can steer a case without the two sides diverging */
extern int WMOCK_cullSeed;
