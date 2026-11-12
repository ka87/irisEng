#pragma once
#include <windows.h>

typedef struct BINK
{
    uint32_t Width;
    uint32_t Height;
    uint32_t Frames;
    uint32_t FrameNum;
    uint32_t LastFrameNum;
    uint32_t FrameRate;
    uint32_t FrameRateDiv;
    uint32_t ReadError;
    uint32_t OpenFlags;
    uint32_t BinkType;
    uint32_t Size;
    uint32_t FrameSize;
    uint32_t SndSize;
    void*    Reserved[32];
} BINK;

typedef BINK* HBINK;
typedef void* HBINKTRACK;

#define BINKSURFACE8P       0
#define BINKSURFACE24       1
#define BINKSURFACE24R      2
#define BINKSURFACE32       3
#define BINKSURFACE32R      4
#define BINKSURFACE32A      5
#define BINKSURFACE32RA     6
#define BINKCOPYALL         0x80000000

HBINK BinkOpen(const char* name, uint32_t flags);
void  BinkClose(HBINK bink);
int   BinkWait(HBINK bink);
int   BinkDoFrame(HBINK bink);
void  BinkNextFrame(HBINK bink);
int   BinkPause(HBINK bink, int pause);
int   BinkCopyToBuffer(HBINK bink, void* dest, int destpitch,
                       uint32_t destheight, uint32_t destx, uint32_t desty,
                       uint32_t flags);
int   BinkSetSoundSystem(void* open, uint32_t param);
int   BinkOpenDirectSound(uint32_t param);
void  BinkSetIO(void* io);
int   BinkSetSoundTrack(uint32_t track);
#define RADLINK
#define RADEXPLINK
#define PTR4
typedef uint32_t U32;
typedef int32_t  S32;
typedef uint8_t  U8;
typedef void*    VOIDPTR;
