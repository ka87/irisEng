#pragma once
#include "../time.h"
#include "../modifiers/snakemodifier.h"

#define MOCK_LOG_MAX 8192
#define MOCK_CALL_ARGS 10

typedef struct sMockCall
{
    const char* fn;
    int gao;
    int nargs;
    float arg[MOCK_CALL_ARGS];
} tMockCall;

extern tMockCall MOCK_log[MOCK_LOG_MAX];
extern int MOCK_logLen;
extern int MOCK_overflow;

void MOCK_Reset(void);
void MOCK_RegisterGaos(tGameObjectMain** gaos, int num);
void MOCK_Record(const char* fn, const void* gao, int nargs, ...);
int MOCK_GaoIndex(const void* gao);

void MOCK_SetGroupRanks(tGameObjectMain** byRank, int num);
void MOCK_SetCurGroup(void* grp);
void MOCK_SetLoadStream(const unsigned char* bytes, int len);
