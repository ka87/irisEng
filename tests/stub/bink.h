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
#define RADLINK
#define RADEXPLINK
#define PTR4
typedef uint32_t U32;
typedef int32_t  S32;
typedef uint8_t  U8;
typedef void*    VOIDPTR;

/* The custom-I/O half of the SDK, as proceduraltextures.c names it.
   Only the members and entry points the engine uses are here, and the
   signatures are taken from the engine's own definitions of the six
   callbacks it installs. The four BinkOpen flag values below are NOT
   from the SDK -- they are distinct placeholder bits, because the
   engine only ORs them into BinkOpen and never tests them, and the
   real build compiles against the real bink.h. Do not read a value
   out of this file. */

#define BINKIOSIZE          0x00000001
#define BINKIOPROCESSOR     0x00000002
#define BINKSNDTRACK        0x00000004
#define BINKNOTHREADEDIO    0x00000008

typedef struct BINKIO
{
    U32  (RADLINK* ReadHeader)(struct BINKIO PTR4* Bnkio, S32 Offset,
                               void PTR4* Dest, U32 Size);
    U32  (RADLINK* ReadFrame)(struct BINKIO PTR4* Bnkio, U32 Framenum,
                              S32 origofs, void PTR4* Dest, U32 Size);
    U32  (RADLINK* GetBufferSize)(struct BINKIO PTR4* Bnkio, U32 Size);
    void (RADLINK* SetInfo)(struct BINKIO PTR4* Bnkio, void PTR4* Buf,
                            U32 Size, U32 FileSize, U32 simulate);
    U32  (RADLINK* Idle)(struct BINKIO PTR4* Bnkio);
    void (RADLINK* Close)(struct BINKIO PTR4* Bnkio);
    U32  BytesRead;
    void* Reserved[32];
} BINKIO;

typedef S32  (RADLINK* BINKIOOPEN)(BINKIO PTR4* Bnkio,
                                   const char PTR4* name, U32 flags);
typedef void PTR4* (RADLINK* BINKMEMALLOC)(U32 bytes);
typedef void (RADLINK* BINKMEMFREE)(void PTR4* ptr);

void  BinkSetIO(BINKIOOPEN io);
void  BinkSetIOSize(U32 iosize);
void  BinkSetMemory(BINKMEMALLOC alloc, BINKMEMFREE free);
int   BinkSetSoundTrack(U32 total_tracks, U32 PTR4* tracks);
