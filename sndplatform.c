/* The audio device and the voices over it. See sndplatform.h for what
   this has to replace and where the two models do not meet. */

#include <SDL3/SDL.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "sndplatform.h"

/* 48 kHz stereo, which is what the primary buffer was set to. Every
   voice's own rate is converted to it by the stream. */
#define SPL_RATE      48000
#define SPL_CHANNELS  2

/* A fed chunk, in frames of the voice's own rate. Short, because the
   pan does not reach audio that is already queued: at 48 kHz this is
   about 10 ms, and four of them may be queued, so a pan change is
   heard within about 40 ms. The engine moves a pan over tenths of a
   second at the fastest. */
#define SPL_CHUNK_FRAMES  480
#define SPL_QUEUE_CHUNKS  4

struct sSplVoice
{
    SDL_AudioStream* stream;
    /* The buffer the engine writes into. Shared with the voices this
       one was duplicated from, which is why it is counted. */
    uint8_t*  pcm;
    int*      pcmUsers;
    int       bytes;
    int       channels;
    int       freq;

    int       cursor;      /* byte offset the feeder has reached */
    int       playing;
    int       looping;
    /* Set when a one-shot has been fed to its end, so the cursor can
       go on reporting the end rather than wrapping. */
    int       drained;

    int       attenuation; /* hundredths of a dB, 0 .. -10000 */
    int       pan;         /* hundredths of a dB, -10000 .. 10000 */

    struct sSplVoice* next;
};

static SDL_AudioDeviceID spl_device;
static SDL_AudioStream*  spl_voiceList;   /* unused: see spl_voices */
static tSplVoice*        spl_voices;
static SDL_Mutex*        spl_lock;
static int               spl_attenuation;
static int               spl_pan;

/* Hundredths of a decibel to a linear gain. DirectSound's definition:
   0 is untouched and -10000 is silence. */
static float SPL_l_f_Gain(int hundredthsDb)
{
    if (hundredthsDb <= -10000)
        return 0.0f;
    if (hundredthsDb >= 0)
        return 1.0f;
    return (float)pow(10.0, (double)hundredthsDb / 2000.0);
}

/* A pan in hundredths of a decibel to the pair of channel gains it
   stands for. DirectSound attenuates one channel and leaves the
   other: positive pan takes the left channel down. */
static void SPL_l_PanGains(int pan, float* left, float* right)
{
    *left = pan > 0 ? SPL_l_f_Gain(-pan) : 1.0f;
    *right = pan < 0 ? SPL_l_f_Gain(pan) : 1.0f;
}

int SPL_b_IsOpen(void)
{
    return spl_device != 0;
}

int SPL_b_Open(void)
{
    SDL_AudioSpec spec;

    if (spl_device)
        return 1;
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
        return 0;
    spec.format = SDL_AUDIO_S16;
    spec.channels = SPL_CHANNELS;
    spec.freq = SPL_RATE;
    spl_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!spl_device)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return 0;
    }
    spl_lock = SDL_CreateMutex();
    if (!spl_lock)
    {
        SDL_CloseAudioDevice(spl_device);
        spl_device = 0;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return 0;
    }
    spl_attenuation = 0;
    spl_pan = 0;
    SDL_ResumeAudioDevice(spl_device);
    return 1;
}

void SPL_Close(void)
{
    if (!spl_device)
        return;
    /* The voices go first: each holds a stream bound to the device. */
    while (spl_voices)
        SPL_VoiceDestroy(spl_voices);
    SDL_CloseAudioDevice(spl_device);
    spl_device = 0;
    if (spl_lock)
    {
        SDL_DestroyMutex(spl_lock);
        spl_lock = NULL;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

int SPL_i_ChunkBytes(const tSplVoice* voice)
{
    if (!voice)
        return 0;
    return SPL_CHUNK_FRAMES * 2 * voice->channels;
}

tSplVoice* SPL_p_VoiceCreate(int channels, int freq, int bytes)
{
    SDL_AudioSpec src, dst;
    tSplVoice* voice;

    if (!spl_device || bytes <= 0 || (channels != 1 && channels != 2) || freq <= 0)
        return NULL;

    voice = (tSplVoice*)calloc(1, sizeof(*voice));
    if (!voice)
        return NULL;
    voice->pcm = (uint8_t*)calloc(1, (size_t)bytes);
    voice->pcmUsers = (int*)calloc(1, sizeof(int));
    if (!voice->pcm || !voice->pcmUsers)
    {
        free(voice->pcm);
        free(voice->pcmUsers);
        free(voice);
        return NULL;
    }
    *voice->pcmUsers = 1;
    voice->bytes = bytes;
    voice->channels = channels;
    voice->freq = freq;

    /* Fed as stereo whatever the sound is, because the pan is applied
       on the way in and needs two channels to apply to. */
    src.format = SDL_AUDIO_S16;
    src.channels = SPL_CHANNELS;
    src.freq = freq;
    dst.format = SDL_AUDIO_S16;
    dst.channels = SPL_CHANNELS;
    dst.freq = SPL_RATE;
    voice->stream = SDL_CreateAudioStream(&src, &dst);
    if (!voice->stream || !SDL_BindAudioStream(spl_device, voice->stream))
    {
        if (voice->stream)
            SDL_DestroyAudioStream(voice->stream);
        free(voice->pcm);
        free(voice->pcmUsers);
        free(voice);
        return NULL;
    }

    SDL_LockMutex(spl_lock);
    voice->next = spl_voices;
    spl_voices = voice;
    SDL_UnlockMutex(spl_lock);
    return voice;
}

tSplVoice* SPL_p_VoiceDuplicate(const tSplVoice* src)
{
    tSplVoice* voice;

    if (!src || !spl_device)
        return NULL;
    voice = SPL_p_VoiceCreate(src->channels, src->freq, src->bytes);
    if (!voice)
        return NULL;

    /* The sound itself is shared; everything that says how it is being
       played is this voice's own and starts where a new voice starts. */
    free(voice->pcm);
    free(voice->pcmUsers);
    voice->pcm = src->pcm;
    voice->pcmUsers = src->pcmUsers;
    (*voice->pcmUsers)++;
    voice->attenuation = src->attenuation;
    voice->pan = src->pan;
    return voice;
}

void SPL_VoiceDestroy(tSplVoice* voice)
{
    tSplVoice** link;

    if (!voice)
        return;
    SDL_LockMutex(spl_lock);
    for (link = &spl_voices; *link; link = &(*link)->next)
        if (*link == voice)
        {
            *link = voice->next;
            break;
        }
    SDL_UnlockMutex(spl_lock);

    if (voice->stream)
    {
        SDL_UnbindAudioStream(voice->stream);
        SDL_DestroyAudioStream(voice->stream);
    }
    if (voice->pcmUsers && --(*voice->pcmUsers) == 0)
    {
        free(voice->pcm);
        free(voice->pcmUsers);
    }
    free(voice);
}

int SPL_i_VoiceBytes(const tSplVoice* voice)
{
    return voice ? voice->bytes : 0;
}

int SPL_i_VoiceChannels(const tSplVoice* voice)
{
    return voice ? voice->channels : 0;
}

int SPL_b_VoiceWrite(tSplVoice* voice, int offset, const void* data, int bytes)
{
    int first;

    if (!voice || !data || bytes <= 0 || bytes > voice->bytes)
        return 0;
    offset %= voice->bytes;
    if (offset < 0)
        offset += voice->bytes;
    first = voice->bytes - offset;
    if (first > bytes)
        first = bytes;
    memcpy(voice->pcm + offset, data, (size_t)first);
    if (bytes > first)
        memcpy(voice->pcm, (const uint8_t*)data + first, (size_t)(bytes - first));
    return 1;
}

int SPL_b_VoiceSilence(tSplVoice* voice, int offset, int bytes)
{
    int first;

    if (!voice || bytes <= 0 || bytes > voice->bytes)
        return 0;
    offset %= voice->bytes;
    if (offset < 0)
        offset += voice->bytes;
    first = voice->bytes - offset;
    if (first > bytes)
        first = bytes;
    memset(voice->pcm + offset, 0, (size_t)first);
    if (bytes > first)
        memset(voice->pcm, 0, (size_t)(bytes - first));
    return 1;
}

static void SPL_l_ApplyGain(tSplVoice* voice)
{
    int total;

    if (!voice->stream)
        return;
    /* The device's attenuation and the voice's add, which is what two
       attenuations in decibels do. */
    total = voice->attenuation + spl_attenuation;
    if (total < -10000)
        total = -10000;
    SDL_SetAudioStreamGain(voice->stream, SPL_l_f_Gain(total));
}

void SPL_VoicePlay(tSplVoice* voice, int loop)
{
    if (!voice)
        return;
    voice->playing = 1;
    voice->looping = loop ? 1 : 0;
    voice->drained = 0;
    SPL_l_ApplyGain(voice);
}

void SPL_VoiceStop(tSplVoice* voice)
{
    if (!voice)
        return;
    voice->playing = 0;
    voice->looping = 0;
    voice->drained = 0;
    voice->cursor = 0;
    if (voice->stream)
        SDL_ClearAudioStream(voice->stream);
}

void SPL_VoicePause(tSplVoice* voice)
{
    if (!voice)
        return;
    voice->playing = 0;
    /* What is already queued would otherwise go on being heard, and a
       pause is meant to be silent at once. The cursor stays where the
       device had got to, so resuming carries on from there. */
    voice->cursor = SPL_i_VoiceCursor(voice);
    if (voice->stream)
        SDL_ClearAudioStream(voice->stream);
}

int SPL_i_VoiceStatus(const tSplVoice* voice)
{
    int status = 0;

    if (!voice)
        return 0;
    /* Still playing while anything of it is queued: the device has not
       finished with it even once the feeder has. */
    if (voice->playing || (voice->stream && SDL_GetAudioStreamQueued(voice->stream) > 0))
        status |= SPL_PLAYING;
    if (voice->looping)
        status |= SPL_LOOPING;
    return status;
}

void SPL_VoiceSetCursor(tSplVoice* voice, int offset)
{
    if (!voice)
        return;
    offset %= voice->bytes;
    if (offset < 0)
        offset += voice->bytes;
    voice->cursor = offset;
    voice->drained = 0;
    /* Whatever was queued came from where the cursor used to be. */
    if (voice->stream)
        SDL_ClearAudioStream(voice->stream);
}

int SPL_i_VoiceCursor(const tSplVoice* voice)
{
    int queued, frame, behind;

    if (!voice)
        return 0;
    if (!voice->stream)
        return voice->cursor;
    /* The feeder's cursor runs ahead of what has been heard by exactly
       what is still queued. Queued bytes are in the stream's input
       format, which is stereo 16-bit at the voice's own rate. */
    queued = SDL_GetAudioStreamQueued(voice->stream);
    if (queued <= 0)
        return voice->cursor;
    frame = 2 * SPL_CHANNELS;
    behind = (queued / frame) * (2 * voice->channels);
    if (behind >= voice->bytes)
        behind %= voice->bytes;
    if (behind > voice->cursor)
        return voice->cursor + voice->bytes - behind;
    return voice->cursor - behind;
}

int SPL_i_VoiceQueued(const tSplVoice* voice)
{
    if (!voice || !voice->stream)
        return 0;
    return SDL_GetAudioStreamQueued(voice->stream);
}

int SPL_i_VoiceWriteCursor(const tSplVoice* voice)
{
    if (!voice)
        return 0;
    /* Where it is safe to write: the feeder has taken everything up to
       its own cursor, so that is the first byte nobody is holding. */
    return voice->cursor;
}

void SPL_VoiceSetAttenuation(tSplVoice* voice, int hundredthsDb)
{
    if (!voice)
        return;
    if (hundredthsDb > 0)
        hundredthsDb = 0;
    if (hundredthsDb < -10000)
        hundredthsDb = -10000;
    voice->attenuation = hundredthsDb;
    SPL_l_ApplyGain(voice);
}

int SPL_i_VoiceAttenuation(const tSplVoice* voice)
{
    return voice ? voice->attenuation : 0;
}

void SPL_VoiceSetPan(tSplVoice* voice, int hundredthsDb)
{
    if (!voice)
        return;
    if (hundredthsDb > 10000)
        hundredthsDb = 10000;
    if (hundredthsDb < -10000)
        hundredthsDb = -10000;
    voice->pan = hundredthsDb;
}

int SPL_i_VoicePan(const tSplVoice* voice)
{
    return voice ? voice->pan : 0;
}

void SPL_VoiceSetFrequency(tSplVoice* voice, int hz)
{
    if (!voice || hz <= 0)
        return;
    voice->freq = hz;
    if (voice->stream)
    {
        /* The stream was made at the voice's original rate, so a new
           rate is that ratio rather than a new stream. */
        SDL_AudioSpec src, dst;
        if (SDL_GetAudioStreamFormat(voice->stream, &src, &dst))
            SDL_SetAudioStreamFrequencyRatio(voice->stream, (float)hz / (float)src.freq);
    }
}

int SPL_i_VoiceFrequency(const tSplVoice* voice)
{
    return voice ? voice->freq : 0;
}

void SPL_SetAttenuation(int hundredthsDb)
{
    tSplVoice* voice;

    if (hundredthsDb > 0)
        hundredthsDb = 0;
    if (hundredthsDb < -10000)
        hundredthsDb = -10000;
    spl_attenuation = hundredthsDb;
    SDL_LockMutex(spl_lock);
    for (voice = spl_voices; voice; voice = voice->next)
        SPL_l_ApplyGain(voice);
    SDL_UnlockMutex(spl_lock);
}

int SPL_i_Attenuation(void)
{
    return spl_attenuation;
}

void SPL_SetPan(int hundredthsDb)
{
    if (hundredthsDb > 10000)
        hundredthsDb = 10000;
    if (hundredthsDb < -10000)
        hundredthsDb = -10000;
    spl_pan = hundredthsDb;
}

int SPL_i_Pan(void)
{
    return spl_pan;
}

/* One voice's worth of feeding: take a chunk from the buffer, widen it
   to stereo, apply the pan, and hand it over. */
static void SPL_l_FeedVoice(tSplVoice* voice)
{
    int16_t out[SPL_CHUNK_FRAMES * SPL_CHANNELS];
    int frameBytes, chunk, queued, want, frames, i, taken;
    float left, right, deviceLeft, deviceRight;

    if (!voice->playing || !voice->stream)
        return;

    frameBytes = 2 * voice->channels;
    chunk = SPL_CHUNK_FRAMES * frameBytes;
    want = chunk * SPL_QUEUE_CHUNKS;
    queued = SDL_GetAudioStreamQueued(voice->stream);
    /* Queued bytes are stereo; what is owed is measured in the voice's
       own frames, so compare in frames. */
    if (queued / (2 * SPL_CHANNELS) >= want / frameBytes)
        return;

    SPL_l_PanGains(voice->pan, &left, &right);
    SPL_l_PanGains(spl_pan, &deviceLeft, &deviceRight);
    left *= deviceLeft;
    right *= deviceRight;

    frames = SPL_CHUNK_FRAMES;
    taken = 0;
    for (i = 0; i < frames; i++)
    {
        int16_t l, r;
        const int16_t* src;

        if (voice->cursor + frameBytes > voice->bytes)
        {
            /* The last frame of the buffer. A looping voice carries on
               from the top; a one-shot stops being fed. */
            voice->cursor = 0;
            if (!voice->looping)
            {
                voice->drained = 1;
                voice->playing = 0;
                break;
            }
        }
        src = (const int16_t*)(voice->pcm + voice->cursor);
        l = src[0];
        r = voice->channels == 2 ? src[1] : src[0];
        out[2 * i] = (int16_t)((float)l * left);
        out[2 * i + 1] = (int16_t)((float)r * right);
        voice->cursor += frameBytes;
        if (voice->cursor >= voice->bytes)
        {
            voice->cursor = 0;
            if (!voice->looping)
            {
                voice->drained = 1;
                voice->playing = 0;
                taken = i + 1;
                break;
            }
        }
        taken = i + 1;
    }
    if (taken > 0)
        SDL_PutAudioStreamData(voice->stream, out,
                               taken * (int)(2 * SPL_CHANNELS));
}

void SPL_Feed(void)
{
    tSplVoice* voice;

    if (!spl_device)
        return;
    SDL_LockMutex(spl_lock);
    for (voice = spl_voices; voice; voice = voice->next)
        SPL_l_FeedVoice(voice);
    SDL_UnlockMutex(spl_lock);
}

int SPL_b_FeedWanted(void)
{
    tSplVoice* voice;
    int wanted = 0;

    if (!spl_device)
        return 0;
    SDL_LockMutex(spl_lock);
    for (voice = spl_voices; voice && !wanted; voice = voice->next)
    {
        int frameBytes, want;

        if (!voice->playing || !voice->stream)
            continue;
        frameBytes = 2 * voice->channels;
        want = SPL_CHUNK_FRAMES * SPL_QUEUE_CHUNKS;
        if (SDL_GetAudioStreamQueued(voice->stream) / (2 * SPL_CHANNELS) < want)
            wanted = 1;
        (void)frameBytes;
    }
    SDL_UnlockMutex(spl_lock);
    return wanted;
}

/* ------------------------------------------------------------------
   The sound thread and its mailbox. See the note in sndplatform.h.
   ------------------------------------------------------------------ */

struct sSplThread
{
    SDL_Thread* thread;
    int (*body)(void*);
    void* arg;
};

static SDL_Mutex*     spl_cmdLock;
static SDL_Condition* spl_cmdPosted;    /* a command is waiting */
static SDL_Condition* spl_cmdAnswered;  /* the thread has done it */
static int            spl_cmd = SPL_CMD_NONE;
static int            spl_cmdDone;

static int SPL_l_b_CmdOpen(void)
{
    if (spl_cmdLock)
        return 1;
    spl_cmdLock = SDL_CreateMutex();
    spl_cmdPosted = SDL_CreateCondition();
    spl_cmdAnswered = SDL_CreateCondition();
    if (spl_cmdLock && spl_cmdPosted && spl_cmdAnswered)
        return 1;
    if (spl_cmdAnswered) SDL_DestroyCondition(spl_cmdAnswered);
    if (spl_cmdPosted)   SDL_DestroyCondition(spl_cmdPosted);
    if (spl_cmdLock)     SDL_DestroyMutex(spl_cmdLock);
    spl_cmdAnswered = 0;
    spl_cmdPosted = 0;
    spl_cmdLock = 0;
    return 0;
}

static int SDLCALL SPL_l_i_ThreadBody(void* data)
{
    tSplThread* t = (tSplThread*)data;
    return t->body(t->arg);
}

tSplThread* SPL_p_ThreadStart(int (*body)(void*), void* arg, const char* name)
{
    tSplThread* t;

    if (!body || !SPL_l_b_CmdOpen())
        return 0;
    t = (tSplThread*)calloc(1, sizeof(*t));
    if (!t)
        return 0;
    t->body = body;
    t->arg = arg;
    t->thread = SDL_CreateThread(SPL_l_i_ThreadBody, name ? name : "sound", t);
    if (!t->thread)
    {
        free(t);
        return 0;
    }
    return t;
}

int SPL_b_ThreadWait(tSplThread* thread, int timeoutMs)
{
    int status = 0;

    if (!thread)
        return 1;
    /* SDL waits without a bound, and the engine's own bound is what
       stops a wedged thread holding the shutdown. A thread that has
       been told to exit answers long before this. */
    (void)timeoutMs;
    SDL_WaitThread(thread->thread, &status);
    free(thread);
    return 1;
}

int SPL_b_Command(int command, int timeoutMs)
{
    int answered;

    if (!SPL_l_b_CmdOpen())
        return 0;
    SDL_LockMutex(spl_cmdLock);
    spl_cmd = command;
    spl_cmdDone = 0;
    SDL_SignalCondition(spl_cmdPosted);
    /* Waited rather than slept on, so the answer is taken as soon as
       it is given; the bound is only there for a thread that never
       answers at all. */
    while (!spl_cmdDone)
    {
        if (!SDL_WaitConditionTimeout(spl_cmdAnswered, spl_cmdLock, timeoutMs))
            break;
    }
    answered = spl_cmdDone;
    SDL_UnlockMutex(spl_cmdLock);
    return answered;
}

int SPL_i_TakeCommand(int timeoutMs)
{
    int command;

    if (!SPL_l_b_CmdOpen())
        return SPL_CMD_ERROR;
    SDL_LockMutex(spl_cmdLock);
    if (spl_cmd == SPL_CMD_NONE)
        SDL_WaitConditionTimeout(spl_cmdPosted, spl_cmdLock, timeoutMs);
    command = spl_cmd;
    spl_cmd = SPL_CMD_NONE;
    SDL_UnlockMutex(spl_cmdLock);
    return command;
}

void SPL_CommandDone(void)
{
    if (!spl_cmdLock)
        return;
    SDL_LockMutex(spl_cmdLock);
    spl_cmdDone = 1;
    SDL_SignalCondition(spl_cmdAnswered);
    SDL_UnlockMutex(spl_cmdLock);
}
