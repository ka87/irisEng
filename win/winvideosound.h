#pragma once
#include <dsound.h>
#include "../common.h"
#include "../video.h"

/*
    The sound half of video playback: a DirectSound streaming buffer
    fed from a video stream's PCM queue.

    The Bink SDK owned an audio device of its own -- the engine handed
    it an IDirectSound8 through BinkSetSoundSystem and Bink ran the
    buffer, the mixing and the clock. libav decodes and nothing else,
    so that part is here.

    It is also what gives the picture its clock. Every WVS_Update
    reads the play cursor, works out how many sample frames have
    actually been heard, and tells the video stream through
    VID_SoundPlayed; the frame pacing then follows the sound rather
    than the wall, which is what Bink did internally.
*/

typedef struct sVideoSound tVideoSound;

/* NULL when there is no sound to play or the device refuses a
   buffer, which is not a reason to stop showing the picture. */
tVideoSound* WVS_pst_Open(IDirectSound8* device, tVideoStream* video);
void         WVS_Close(tVideoSound* sound);

/* Once a frame: top the buffer up from the stream's queue, and
   report back how much has been played. */
void WVS_Update(tVideoSound* sound, tVideoStream* video);
