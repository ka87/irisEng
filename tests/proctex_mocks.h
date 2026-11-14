#pragma once
#include <stddef.h>

/* Shared state for the proceduraltextures.c differential test. */

#define MOCKP_ARENA   (512 * 1024)
#define MOCKP_LOG_MAX 20000

typedef struct
{
    const char* fn;
    long a;
    long b;
    long c;
} tMockProcCall;

extern unsigned char MOCKP_arena[2][MOCKP_ARENA];
extern tMockProcCall MOCKP_log[2][MOCKP_LOG_MAX];
extern int MOCKP_logLen[2];
extern int MOCKP_overflow;

/* Which side's arena and call log the mocks use. 0 = frozen, 1 = rewrite. */
void MOCKP_SetSide(int side);
int MOCKP_Side(void);

/* Refill both arenas with the same deterministic pattern and clear the
   logs. Every case starts here, so an uninitialised read is a defined
   and identical value on both sides rather than noise. */
void MOCKP_Reset(void);

/* The deterministic generators. Both sides are reset to the same seed. */
void MOCKP_SeedRand(unsigned int seed);

/* The byte stream LOA_Read* walks and BINK_SeekAndReadFromBF serves. */
void MOCKP_SetStream(const unsigned char* data, int len);

/* BIG_ul_SearchKeyToPos / BIG_ul_GetLengthFile answers. */
void MOCKP_SetBigFile(int pos, unsigned int len);

/* What SAV_PhotoGet hands back. */
void MOCKP_SetPhoto(char* data, int len);

void MOCKP_Record(const char* fn, long a, long b, long c);

/* Compare the two arenas, normalising any word that is a pointer into
   the caller's own arena or one of the known paired function addresses.
   Returns the number of differing bytes. */
int MOCKP_CompareArenas(void);

/* Compare the two call logs. Returns the index of the first difference
   or -1. */
int MOCKP_CompareLogs(void);

void MOCKP_AddPair(const void* base, const void* new_);

/* A pair of equivalent objects outside the arena -- the two copies' own
   globals -- so a pointer into one compares equal to the matching
   pointer into the other. */
void MOCKP_AddRange(const void* base, const void* new_, size_t size);

/* The next values a four-byte BINK_SeekAndReadFromBF should hand back,
   so BINK_IO_FUNC reads a chunk size and a video size rather than the
   filler pattern. */
void MOCKP_PushWord(int v);

/* video.c's answers: how many frames are left, a bit per call for
   VID_b_Waiting and for VID_b_NextAlsoDue, and whether opening
   fails. */
void MOCKP_SetVideo(int s, int frames, int waitScript, int dueScript, int openFails);
int  MOCKP_VideoOpened(int s);
