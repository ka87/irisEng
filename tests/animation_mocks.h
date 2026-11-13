#pragma once
#include <stddef.h>

#define ANIT_ARENA_SIZE (16 << 20)
#define ANIT_LOG_MAX 40000
#define ANIT_MTX_MAX 4096

typedef struct ANIT_LogEntry
{
    const char* name;
    double a;
    double b;
    double c;
    double d;
} ANIT_LogEntry;

extern char ANIT_arena[ANIT_ARENA_SIZE];
extern size_t ANIT_arenaUsed;
extern ANIT_LogEntry ANIT_log[ANIT_LOG_MAX];
extern int ANIT_logN;
extern int ANIT_seq;

void ANIT_ResetArena(void);
void* ANIT_Alloc(size_t n);
long ANIT_Ref(const void* p);
void ANIT_ResetLog(void);
void ANIT_Log4(const char* name, double a, double b, double c, double d);
void ANIT_MarkMtx(void* p, int n);
int ANIT_SkipWord(size_t off);

extern size_t ANIT_mtxOff[ANIT_MTX_MAX];
extern int ANIT_mtxN;

// What the stand-ins hand back, set up by the fixture.
extern char* ANIT_fileData;
extern int ANIT_fileSize;
extern void* ANIT_keyPos[16];
extern struct sGameObjectEventsList* ANIT_loadedList;
extern void* ANIT_loadedGroup;

struct sGroGeneric* ANIT_MakeGro(int id);
