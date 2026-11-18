#pragma once

#include "common.h"

/* The audio device, and a voice per sound.
 *
 * What this has to replace is not a thin wrapper. A DirectSound
 * buffer was memory the engine wrote into and then told to play,
 * loop, seek, pan and attenuate, and it reported back how far it had
 * got. An SDL audio stream is the other way round: it is a queue that
 * the device pulls from, with no buffer to seek in and no play
 * position to read.
 *
 * So a voice here is the buffer *plus* the cursor over it, and the
 * feed is what turns one into the other. The engine goes on writing
 * into a buffer at an offset and asking where the play position is;
 * underneath, a chunk at a time is pushed into a stream of its own and
 * SDL mixes the streams together.
 *
 * Units are the engine's, which are DirectSound's: attenuation and pan
 * are both in hundredths of a decibel. Attenuation is 0 for untouched
 * and -10000 for silence. Pan is the difference between the two
 * channels -- negative attenuates the right channel, positive the
 * left -- so 0 is centred and +/-10000 is hard over.
 *
 * Where the two ways of doing it cannot be reconciled:
 *
 * - **Attenuation is applied by SDL as it pulls**, so a change is
 *   heard almost at once, which is what the engine's fades expect.
 * - **Pan is applied as the chunk is fed**, because a stream carries
 *   one gain and not two. A change is therefore heard one fed chunk
 *   later. The chunk is deliberately short for that reason.
 * - **Nothing here is spatial.** DirectSound could place a buffer in
 *   a room; the engine never asked it to, beyond deriving a position
 *   from the pan it had already worked out for itself. The pan is
 *   what carries the intent and the pan is what is kept. */

typedef struct sSplVoice tSplVoice;

/* The device. Opening it twice is a no-op that answers true. */
int  SPL_b_Open(void);
void SPL_Close(void);
int  SPL_b_IsOpen(void);

/* How many bytes of a voice are fed at a time, and how many are kept
   queued ahead of the device. Both in bytes of the voice's own format;
   the second is what bounds how stale the pan can be. */
int  SPL_i_ChunkBytes(const tSplVoice* voice);

/* A voice over `bytes` of 16-bit PCM. For a sound held whole that is
   the sound; for one read as it plays it is the window the feeder
   writes into and wraps around, which is what the buffer was. */
tSplVoice* SPL_p_VoiceCreate(int channels, int freq, int bytes);
void SPL_VoiceDestroy(tSplVoice* voice);
/* A second voice over the same sound, sharing nothing else: its own
   cursor, its own mix. This is what Duplicate was for. */
tSplVoice* SPL_p_VoiceDuplicate(const tSplVoice* src);

int  SPL_i_VoiceBytes(const tSplVoice* voice);
int  SPL_i_VoiceChannels(const tSplVoice* voice);

/* Writing into the voice's buffer: what Lock, the memcpy and Unlock
   did, without handing out a pointer into someone else's memory.
   `offset` wraps, and a write may run off the end and come back. */
int  SPL_b_VoiceWrite(tSplVoice* voice, int offset, const void* data, int bytes);
/* And the same the other way, for the one caller that cleared a
   stretch of buffer rather than filling it. */
int  SPL_b_VoiceSilence(tSplVoice* voice, int offset, int bytes);

#define SPL_PLAYING  1
#define SPL_LOOPING  2

void SPL_VoicePlay(tSplVoice* voice, int loop);
/* Stops and rewinds, which is what Stop did. */
void SPL_VoiceStop(tSplVoice* voice);
/* Stops and keeps the cursor, which is what Pause did. */
void SPL_VoicePause(tSplVoice* voice);
int  SPL_i_VoiceStatus(const tSplVoice* voice);

/* The play cursor: how far into the buffer the device has got. The
   write cursor is how far it is safe to write, which is the play
   cursor plus what is already queued -- a stream holds audio the
   device has not played yet, and overwriting it would be heard. */
void SPL_VoiceSetCursor(tSplVoice* voice, int offset);
int  SPL_i_VoiceCursor(const tSplVoice* voice);
int  SPL_i_VoiceWriteCursor(const tSplVoice* voice);

/* How much of this voice is queued ahead of the device, in bytes of
   the stream's own input format. Falling is the device draining it;
   stuck at the full queue is the device not taking it. */
int  SPL_i_VoiceQueued(const tSplVoice* voice);

void SPL_VoiceSetAttenuation(tSplVoice* voice, int hundredthsDb);
int  SPL_i_VoiceAttenuation(const tSplVoice* voice);
void SPL_VoiceSetPan(tSplVoice* voice, int hundredthsDb);
int  SPL_i_VoicePan(const tSplVoice* voice);
void SPL_VoiceSetFrequency(tSplVoice* voice, int hz);
int  SPL_i_VoiceFrequency(const tSplVoice* voice);

/* The whole device's attenuation and pan, which the engine's global
   volume and pan set. Applied on top of every voice's own. */
void SPL_SetAttenuation(int hundredthsDb);
int  SPL_i_Attenuation(void);
void SPL_SetPan(int hundredthsDb);
int  SPL_i_Pan(void);

/* Push whatever the playing voices owe the device, and retire the ones
   that have run out. Called from the sound thread, which is where the
   reading and the decoding already happen. */
void SPL_Feed(void);

/* Whether any voice wants feeding, so the thread can wait rather than
   spin. */
int  SPL_b_FeedWanted(void);
