#include "winvideosound.h"
#include "../memory.h"
#include "../sndplatform.h"

#include <string.h>

/* A second of sound in the ring. Long enough that a frame that takes
   its time does not leave the buffer empty, short enough that
   stopping the video does not leave a second of it still to play. */
#define WVS_SECONDS   1

/* The buffer is not refilled until this much of it has been played,
   so a frame does not spend its time locking and unlocking a few
   hundred bytes at a time. */
#define WVS_MINWRITE  4096

struct sVideoSound
{
    tSplVoice* voice;
    int   bytes;          /* the whole ring */
    int   frameBytes;     /* one sample frame */
    int   writePos;       /* where the next write goes */
    int   lastPlayPos;    /* the cursor the last update saw */
    /* Cumulative, both of them, because the two positions on their
       own cannot tell a full ring from an empty one: they are equal
       in both cases. The difference between these two can. */
    int64_t writtenBytes;
    int64_t playedBytes;
    int64_t soundBytes;   /* of the written, how much was really video */
    int   started;        /* Play has been called */
};

/* Writes into the ring at writePos, wrapping, and advances it.
   Where this used to lock a stretch of the buffer and be handed one
   or two pointers into it, the voice takes the bytes and does its own
   wrapping -- so the write is always the whole of what was asked
   for, and there is no short write to account for. */
static void WVS_l_Write(tVideoSound* sound, const uint8_t* src, int bytes,
                        int asSilence, int count)
{
    int ok;

    if (bytes <= 0)
        return;
    if (asSilence)
        ok = SPL_b_VoiceSilence(sound->voice, sound->writePos, bytes);
    else
        ok = SPL_b_VoiceWrite(sound->voice, sound->writePos, src, bytes);
    if (!ok)
        return;
    sound->writePos = (sound->writePos + bytes) % sound->bytes;
    if (count)
        sound->writtenBytes += bytes;
}

tVideoSound* WVS_pst_Open(tVideoStream* video)
{
    tVideoSound* sound;
    tSplVoice* voice;
    int rate;
    int channels;
    int bytes;

    if (!video || !VID_b_HasSound(video))
        return NULL;
    rate = VID_i_SoundRate(video);
    channels = VID_i_SoundChannels(video);
    if (rate <= 0 || channels <= 0)
        return NULL;

    bytes = rate * channels * 2 * WVS_SECONDS;
    voice = SPL_p_VoiceCreate(channels, rate, bytes);
    if (!voice)
        return NULL;

    sound = (tVideoSound*)MEM_p_Alloc(sizeof(tVideoSound));
    if (!sound)
    {
        SPL_VoiceDestroy(voice);
        return NULL;
    }
    memset(sound, 0, sizeof(*sound));
    sound->voice = voice;
    sound->bytes = bytes;
    sound->frameBytes = channels * 2;

    /* Start silent, so the first update writes into a known buffer
       rather than whatever the device had. */
    WVS_l_Write(sound, NULL, sound->bytes, 1, 0);
    sound->writePos = 0;
    return sound;
}

void WVS_Close(tVideoSound* sound)
{
    if (!sound)
        return;
    if (sound->voice)
    {
        SPL_VoiceStop(sound->voice);
        SPL_VoiceDestroy(sound->voice);
    }
    MEM_Free(sound);
}

/* The clock the picture runs on: how much of the video's own sound
   has been heard.
   
   Not simply how far the cursor has run, because some of what it ran
   over is silence -- poured in to cover a gap, or left from the clear
   at open -- and counting that would put the clock ahead of the
   picture by the length of the gap. Since the buffer plays in the
   order it was written, the sound heard is whichever is the smaller
   of what has been played and what real sound went in: with sound
   written but not yet played the cursor is the limit, and with the
   cursor past the end of the sound the sound is.
   
   Only once the buffer is actually playing: before that the cursor
   means nothing. */
static void WVS_l_Report(const tVideoSound* sound, tVideoStream* video)
{
    int64_t heard;

    if (!sound->started)
        return;
    heard = sound->playedBytes < sound->soundBytes
          ? sound->playedBytes : sound->soundBytes;
    VID_SoundPlayed(video, heard / sound->frameBytes);
}

void WVS_Update(tVideoSound* sound, tVideoStream* video)
{
    int playPos;
    int moved;
    int free;
    int got;
    uint8_t staging[16384];

    if (!sound || !sound->voice || !video)
        return;

    playPos = SPL_i_VoiceCursor(sound->voice);

    /* The cursor runs round the ring, so the distance it has moved
       since the last look is what was played, and that is what the
       picture's clock is counted in. */
    moved = playPos - sound->lastPlayPos;
    if (moved < 0)
        moved += sound->bytes;
    sound->lastPlayPos = playPos;
    sound->playedBytes += moved;

    /* What is free is the ring less what is written and not yet
       played. Taking it from the two positions instead would read an
       empty ring as a full one at the start, when both are zero. */
    free = sound->bytes - (int)(sound->writtenBytes - sound->playedBytes);
    if (free > (int)sizeof(staging))
        free = (int)sizeof(staging);
    free -= free % sound->frameBytes;
    if ((free < WVS_MINWRITE && sound->started) || free <= 0)
    {
        WVS_l_Report(sound, video);
        return;
    }

    got = VID_i_SoundRead(video, staging, free);
    if (got > 0)
    {
        WVS_l_Write(sound, staging, got, 0, 1);
        sound->soundBytes += got;
    }
    if (got < free)
    {
        /* The queue ran dry. Silence keeps the cursor moving, which
           keeps the picture moving; stalling the buffer instead
           would freeze both. */
        WVS_l_Write(sound, NULL, free - got, 1, 1);
    }

    if (!sound->started)
    {
        SPL_VoicePlay(sound->voice, 1);
        sound->started = 1;
    }
    WVS_l_Report(sound, video);
}
