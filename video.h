#pragma once
#include "common.h"

/*
    Video playback through libav (FFmpeg): demux, decode, scale to a
    32-bit surface, and decode the sound track to PCM for whatever
    sink the platform layer has.

    This replaces the Bink SDK. The engine's two consumers both want
    the same four steps per frame, which is the shape the Bink API
    had and the shape kept here:

        VID_b_Waiting       too early for the next frame?
        VID_b_DecodeFrame   decode the frame that is due
        VID_CopyToBuffer    write it into a locked surface
        VID_NextFrame       advance

    A stream is opened either from a path next to the exe, or from a
    read callback, which is how the procedural textures pull a video
    out of the .bf. libav takes the callback through its own buffered
    reader, so a caller supplies bytes in order and nothing else: the
    block list, slot mask and busy flags the Bink I/O hooks needed are
    not part of this.

    Sound is decoded to 16-bit interleaved PCM and queued. The sink
    pulls it with VID_i_AudioRead and reports back how much has been
    heard with VID_AudioPlayed, which is what the frame clock runs on;
    with no sink and no sound track the clock is the wall clock.

    Needs libavformat, libavcodec, libavutil, libswscale and
    libswresample, 5.1 or later (AVChannelLayout).
*/

typedef struct sVideoStream tVideoStream;

/* Bytes in order, at most `size`; the number read, 0 at the end, or
   negative on error. */
typedef int (*tVideoReadFunc)(void* user, uint8_t* buf, int size);

/* Both return NULL when there is nothing playable. wantSound is
   ignored where the file carries no sound track. */
tVideoStream* VID_pst_OpenFile(const char* path, int wantSound);
tVideoStream* VID_pst_OpenReader(tVideoReadFunc read, void* user, int wantSound);
void          VID_Close(tVideoStream* video);

int VID_i_Width(const tVideoStream* video);
int VID_i_Height(const tVideoStream* video);
int VID_i_FrameNum(const tVideoStream* video);     /* 0 before the first */
int VID_i_FrameCount(const tVideoStream* video);   /* 0 when the file does not say */
int VID_b_Ended(const tVideoStream* video);

/* Non-zero while the frame after the current one is not due yet. */
int  VID_b_Waiting(tVideoStream* video);
/* Decodes the current frame if it is not already in hand. Non-zero on
   success, zero at the end of the stream or on a decode error. */
int  VID_b_DecodeFrame(tVideoStream* video);
/* Releases the current frame so the next decode reads the one after. */
void VID_NextFrame(tVideoStream* video);
/* Non-zero when the frame after the one in hand is already due as
   well, which is how a caller that has fallen behind knows to decode
   on without stopping to show what it has. Asking after advancing
   would mean releasing the frame first, and then there is nothing
   left to show if the answer is no. */
int  VID_b_NextAlsoDue(tVideoStream* video);
/* The frame in hand, as 32-bit BGRA rows of `pitch` bytes. At most
   `destHeight` rows are written. withAlpha keeps the file's alpha
   plane where it has one; without it every pixel is opaque. */
void VID_CopyToBuffer(tVideoStream* video, void* dest, int pitch,
                      int destHeight, int withAlpha);

int VID_b_HasSound(const tVideoStream* video);
int VID_i_SoundRate(const tVideoStream* video);
int VID_i_SoundChannels(const tVideoStream* video);
/* Up to `bytes` of 16-bit interleaved PCM; the number written, which
   is short or zero when the queue has run dry. Always a whole number
   of sample frames, so a request that is not a multiple of one comes
   back rounded down. */
int  VID_i_SoundRead(tVideoStream* video, void* dest, int bytes);
/* How many sample frames the sink has actually played, cumulative.
   Calling this at all makes the sound the clock. */
void VID_SoundPlayed(tVideoStream* video, int64_t frames);
