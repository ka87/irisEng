#include "winvideosound.h"
#include "../memory.h"

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
    IDirectSoundBuffer8* buffer;
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

/* Writes into the ring at writePos, wrapping, and advances it. */
static void WVS_l_Write(tVideoSound* sound, const uint8_t* src, int bytes,
                        int asSilence, int count)
{
    void* first = NULL;
    void* second = NULL;
    DWORD firstBytes = 0;
    DWORD secondBytes = 0;

    if (bytes <= 0)
        return;
    if (FAILED(sound->buffer->lpVtbl->Lock(sound->buffer, (DWORD)sound->writePos,
                                           (DWORD)bytes, &first, &firstBytes,
                                           &second, &secondBytes, 0)))
    {
        return;
    }
    if (first && firstBytes)
    {
        if (asSilence)
            memset(first, 0, firstBytes);
        else
            memcpy(first, src, firstBytes);
    }
    if (second && secondBytes)
    {
        if (asSilence)
            memset(second, 0, secondBytes);
        else
            memcpy(second, src + firstBytes, secondBytes);
    }
    sound->buffer->lpVtbl->Unlock(sound->buffer, first, firstBytes, second, secondBytes);
    sound->writePos = (sound->writePos + (int)(firstBytes + secondBytes)) % sound->bytes;
    if (count)
        sound->writtenBytes += (int)(firstBytes + secondBytes);
}

tVideoSound* WVS_pst_Open(IDirectSound8* device, tVideoStream* video)
{
    tVideoSound* sound;
    WAVEFORMATEX format;
    DSBUFFERDESC desc;
    IDirectSoundBuffer* buffer = NULL;
    int rate;
    int channels;

    if (!device || !video || !VID_b_HasSound(video))
        return NULL;
    rate = VID_i_SoundRate(video);
    channels = VID_i_SoundChannels(video);
    if (rate <= 0 || channels <= 0)
        return NULL;

    memset(&format, 0, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = (WORD)channels;
    format.nSamplesPerSec = (DWORD)rate;
    format.wBitsPerSample = 16;
    format.nBlockAlign = (WORD)(channels * 2);
    format.nAvgBytesPerSec = (DWORD)(rate * channels * 2);
    format.cbSize = 0;

    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS
                 | DSBCAPS_CTRLVOLUME;
    desc.dwBufferBytes = (DWORD)(rate * channels * 2 * WVS_SECONDS);
    desc.lpwfxFormat = &format;

    if (FAILED(device->lpVtbl->CreateSoundBuffer(device, &desc, &buffer, NULL))
        || !buffer)
    {
        return NULL;
    }

    sound = (tVideoSound*)MEM_p_Alloc(sizeof(tVideoSound));
    if (!sound)
    {
        buffer->lpVtbl->Release(buffer);
        return NULL;
    }
    memset(sound, 0, sizeof(*sound));
    sound->buffer = (IDirectSoundBuffer8*)buffer;
    sound->bytes = (int)desc.dwBufferBytes;
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
    if (sound->buffer)
    {
        sound->buffer->lpVtbl->Stop(sound->buffer);
        sound->buffer->lpVtbl->Release(sound->buffer);
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
    DWORD playPos = 0;
    DWORD writeCursor = 0;
    int moved;
    int free;
    int got;
    uint8_t staging[16384];

    if (!sound || !sound->buffer || !video)
        return;

    if (FAILED(sound->buffer->lpVtbl->GetCurrentPosition(sound->buffer,
                                                         &playPos, &writeCursor)))
    {
        return;
    }

    /* The cursor runs round the ring, so the distance it has moved
       since the last look is what was played, and that is what the
       picture's clock is counted in. */
    moved = (int)playPos - sound->lastPlayPos;
    if (moved < 0)
        moved += sound->bytes;
    sound->lastPlayPos = (int)playPos;
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
        sound->buffer->lpVtbl->Play(sound->buffer, 0, 0, DSBPLAY_LOOPING);
        sound->started = 1;
    }
    WVS_l_Report(sound, video);
}
