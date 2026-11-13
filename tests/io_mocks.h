#pragma once
#include <stdint.h>
#include <stddef.h>
#include <time.h>

/* The mock engine the inputoutput.c differential test runs both copies
   against. Every call is logged in order with its arguments; the
   allocator is a bump allocator over one arena the driver rewinds
   between the two sides, so each copy sees the same addresses and the
   arena can be compared byte for byte.

   The globals of inputoutput.c are NOT split per side. The two copies
   run one after the other with the whole state rebuilt in between, so
   sharing them is what makes the comparison a comparison of state
   rather than of two parallel universes. Only the functions carry the
   BASE_/NEW_ prefixes. */

#define IOMOCK_LOG_MAX 4096
#define IOMOCK_ARENA_SIZE (8u << 20)

typedef struct
{
    const char* name;
    long a;
    long b;
    char text[80];
} tIoMockCall;

extern tIoMockCall IOMOCK_log[IOMOCK_LOG_MAX];
extern int IOMOCK_logLen;
extern unsigned char* IOMOCK_arena;
extern size_t IOMOCK_used;

/* What the card-manager handlers hand back, consumed in order. */
#define IOMOCK_RETS_MAX 64
extern int IOMOCK_rets[IOMOCK_RETS_MAX];
extern int IOMOCK_retIdx;
extern int IOMOCK_retNum;

/* What pcINO_b_AsyncInProgress reports, and what the mouse and pad
   queries report. */
extern int IOMOCK_async;
extern int IOMOCK_mouseInUse;
extern int IOMOCK_setPadResult;
extern float IOMOCK_joyMoveX;
extern float IOMOCK_joyMoveY;
extern float IOMOCK_joyMoveZ;

/* The fixed clock localtime is driven from. */
extern long IOMOCK_now;

void IOMOCK_Reset(void);
void IOMOCK_Log(const char* name, long a, long b, const char* text);
long IOMOCK_Offset(const void* p);

time_t IOMOCK_time(time_t* t);
struct tm* IOMOCK_localtime(const time_t* t);
