#pragma once

#define MOCKT_LOG_MAX 4096

typedef struct sMockTexCall
{
    const char* fn;
    int a;
    int b;
} tMockTexCall;

extern tMockTexCall MOCKT_log[MOCKT_LOG_MAX];
extern int MOCKT_logLen;
extern int MOCKT_overflow;

void MOCKT_Reset(void);
void MOCKT_Record(const char* fn, int a, int b);
void MOCKT_SetStream(const unsigned char* bytes, int len);
int MOCKT_BlockSpan(void* ptr);
