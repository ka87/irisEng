#pragma once

/* binkw32 entry points used by the fullscreen video path. Only the
 * handful the playback path calls are declared here, so the Bink SDK
 * headers are not needed to build; binkw32.lib supplies the imports. */

#ifdef IRISENG_HAVE_BINK

#include "../common.h"

/* Leading fields of the Bink handle. The handle is never allocated
 * here, only read through. */
typedef struct sBinkVideo
{
    unsigned int Width;
    unsigned int Height;
    unsigned int Frames;
    unsigned int FrameNum;
} tBinkVideo;

/* Destination format selector for BinkCopyToBuffer: 32 bit BGRA,
 * copy every row. */
#define GFX_FS_VIDEO_COPYFMT   0x80000005

typedef int(__stdcall* tBinkSoundOpen)(void* param);

int __stdcall BinkSetSoundSystem(tBinkSoundOpen open, void* param);
int __stdcall BinkOpenDirectSound(void* param);
tBinkVideo* __stdcall BinkOpen(const char* name, unsigned int flags);
void __stdcall BinkClose(tBinkVideo* bink);
int __stdcall BinkWait(tBinkVideo* bink);
int __stdcall BinkDoFrame(tBinkVideo* bink);
void __stdcall BinkNextFrame(tBinkVideo* bink);
int __stdcall BinkCopyToBuffer(tBinkVideo* bink, void* dest, int destpitch,
    unsigned int destheight, unsigned int destx, unsigned int desty,
    unsigned int flags);

#endif /* IRISENG_HAVE_BINK */
