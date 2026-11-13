#pragma once
#include "../profiler.h"
#include "../time.h"
#include "../win/wingdi.h"

#define PROMOCK_LOG_MAX 4096
#define PROMOCK_ARGS 8
#define PROMOCK_TEXT 64

typedef struct sProMockCall
{
    const char* fn;
    int nargs;
    int arg[PROMOCK_ARGS];
    char text[PROMOCK_TEXT];
} tProMockCall;

extern tProMockCall PROMOCK_log[PROMOCK_LOG_MAX];
extern int PROMOCK_logLen;
extern int PROMOCK_overflow;

extern unsigned int PROMOCK_ticks;
extern unsigned int PROMOCK_tickStep;
extern long long PROMOCK_qpc;
extern long long PROMOCK_qpcStep;

void PROMOCK_Reset(void);
void PROMOCK_Record(const char* fn, const char* text, int nargs, ...);
