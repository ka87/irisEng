/* The audio device and the voices over it, against SDL's dummy audio
 * driver.
 *
 * sndplatform.c is included rather than linked, so the voice list and
 * a voice's own fields can be read back after a feed. SDL is real: a
 * real device, real streams bound to it, real conversion. The dummy
 * driver consumes what is given to it and plays nothing, which is
 * exactly what a test wants -- the thing under test is what reaches
 * the stream, not what reaches a speaker.
 *
 * What is checked is the part that had to be built rather than wrapped:
 * the cursor over a buffer, looping and the end of a one-shot, the two
 * attenuations adding, the pan as a pair of channel gains, and a
 * duplicate sharing the sound and nothing else.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../sndplatform.c"

static int checks;
static int failures;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        failures++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        failures++;
    }
}

static void okNear(double got, double want, double tol, const char* what)
{
    checks++;
    if (!(fabs(got - want) <= tol))
    {
        printf("  FAIL %s: got %.6f want %.6f\n", what, got, want);
        failures++;
    }
}

/* A ramp, so that a cursor can be read back out of what was fed. */
static void fillRamp(tSplVoice* v, int bytes)
{
    int16_t* s = (int16_t*)malloc((size_t)bytes);
    int i;

    for (i = 0; i < bytes / 2; i++)
        s[i] = (int16_t)(i + 1);
    ok(SPL_b_VoiceWrite(v, 0, s, bytes) != 0, "the sound is written into the voice");
    free(s);
}

static void theDevice(void)
{
    printf("-- the device\n");
    ok(SPL_b_Open() != 0, "the device opens");
    ok(SPL_b_IsOpen() != 0, "and says so");
    ok(SPL_b_Open() != 0, "opening it again is not a failure");
    okEq(SPL_i_Attenuation(), 0, "it starts untouched");
    okEq(SPL_i_Pan(), 0, "and centred");
}

static void theBuffer(void)
{
    tSplVoice* v;

    printf("-- writing into a voice, and wrapping\n");
    v = SPL_p_VoiceCreate(1, 22050, 64);
    ok(v != NULL, "a voice is made");
    okEq(SPL_i_VoiceBytes(v), 64, "of the size asked for");
    okEq(SPL_i_VoiceChannels(v), 1, "with the channels asked for");

    {
        int16_t in[4] = { 11, 22, 33, 44 };
        int16_t back[4];

        /* A write that runs off the end comes back to the start, which
           is what a circular buffer meant. */
        ok(SPL_b_VoiceWrite(v, 60, in, 8) != 0, "a write wraps");
        memcpy(back, v->pcm + 60, 4);
        memcpy(back + 2, v->pcm, 4);
        okEq(back[0], 11, "the first half lands at the end");
        okEq(back[2], 33, "and the second at the start");

        ok(SPL_b_VoiceSilence(v, 60, 8) != 0, "and silence wraps too");
        okEq(*(int16_t*)(v->pcm + 60), 0, "the end is cleared");
        okEq(*(int16_t*)v->pcm, 0, "and so is the start");

        okEq(SPL_b_VoiceWrite(v, 0, in, 128), 0, "a write longer than the voice is refused");
    }
    SPL_VoiceDestroy(v);
}

static void theCursor(void)
{
    tSplVoice* v;
    int chunk;

    printf("-- the cursor over the buffer\n");
    /* Big enough that one chunk does not reach the end. */
    v = SPL_p_VoiceCreate(1, SPL_RATE, SPL_CHUNK_FRAMES * 2 * 8);
    fillRamp(v, SPL_i_VoiceBytes(v));
    chunk = SPL_i_ChunkBytes(v);
    okEq(chunk, SPL_CHUNK_FRAMES * 2, "a chunk is a chunk of frames");

    okEq(SPL_i_VoiceStatus(v), 0, "a voice starts stopped");
    okEq(SPL_i_VoiceCursor(v), 0, "and at the beginning");

    SPL_VoicePlay(v, 0);
    ok((SPL_i_VoiceStatus(v) & SPL_PLAYING) != 0, "playing says so");
    ok((SPL_i_VoiceStatus(v) & SPL_LOOPING) == 0, "and is not looping");

    /* One feed queues the whole watermark, so the feeder's own cursor
       moves by that much while nothing has been heard yet. */
    SPL_Feed();
    okEq(v->cursor, chunk, "one feed takes one chunk");
    okEq(SPL_i_VoiceCursor(v), 0, "and none of it has been heard yet");

    /* Fed again without the device consuming anything, it stops: the
       queue is at its watermark. */
    {
        int before = v->cursor;
        int i;
        for (i = 0; i < SPL_QUEUE_CHUNKS * 2; i++)
            SPL_Feed();
        ok(v->cursor > before, "feeding again queues more");
        /* On what is queued, not on how far the cursor has gone. The
           device is real and consumes in its own time, so the cursor
           depends on how long these calls took -- which made this
           check pass on its own and fail inside the suite, where it
           was slower.
           
           What the watermark bounds is the queue: whatever has been
           consumed, a feed tops up by at most one chunk past it. In
           the queue's own units, which are the stream's input format
           -- stereo, whatever the voice is -- and not the voice's. */
        ok(SDL_GetAudioStreamQueued(v->stream)
               <= SPL_CHUNK_FRAMES * (SPL_QUEUE_CHUNKS + 1) * 2 * SPL_CHANNELS,
           "but never queues more than the watermark and a chunk");
    }

    SPL_VoiceSetCursor(v, chunk * 3);
    okEq(SPL_i_VoiceCursor(v), chunk * 3, "a seek moves the cursor");
    okEq(SDL_GetAudioStreamQueued(v->stream), 0,
         "and drops what was queued from where it used to be");

    /* Paused with audio queued, which is the only case that shows
       what a pause has to do with the cursor: the feeder has run on
       ahead of what has been heard, and the cursor has to come back
       to what has been heard or the sound jumps forward on resuming. */
    SPL_VoicePlay(v, 0);
    SPL_Feed();
    ok(v->cursor > chunk * 3, "the feeder runs ahead of what is heard");
    ok(SDL_GetAudioStreamQueued(v->stream) > 0, "with something queued");
    SPL_VoicePause(v);
    ok((SPL_i_VoiceStatus(v) & SPL_PLAYING) == 0, "a pause stops it");
    okEq(SDL_GetAudioStreamQueued(v->stream), 0, "and is silent at once");
    okEq(v->cursor, chunk * 3, "and the cursor comes back to what was heard");
    okEq(SPL_i_VoiceCursor(v), chunk * 3, "keeping its place");

    SPL_VoicePlay(v, 0);
    SPL_Feed();
    ok(v->cursor > chunk * 3, "and carries on from there on resuming");

    SPL_VoiceStop(v);
    okEq(SPL_i_VoiceCursor(v), 0, "a stop rewinds");
    okEq(SDL_GetAudioStreamQueued(v->stream), 0, "and drops the queue");
    SPL_VoiceDestroy(v);
}

static void theEnd(void)
{
    tSplVoice* v;
    int i;

    printf("-- the end of a sound, and looping past it\n");
    /* Two chunks long, so the end arrives quickly. */
    v = SPL_p_VoiceCreate(1, SPL_RATE, SPL_CHUNK_FRAMES * 2 * 2);
    fillRamp(v, SPL_i_VoiceBytes(v));

    SPL_VoicePlay(v, 0);
    for (i = 0; i < 20; i++)
        SPL_Feed();
    ok(v->drained != 0, "a one-shot runs out");
    ok(v->playing == 0, "and stops being fed");

    /* Still reported as playing while the device has not finished with
       what is queued, which is what GetStatus meant. */
    ok((SPL_i_VoiceStatus(v) & SPL_PLAYING) != 0,
       "but is still playing while anything of it is queued");

    SPL_VoiceStop(v);
    SPL_VoicePlay(v, 1);
    for (i = 0; i < 20; i++)
        SPL_Feed();
    ok(v->drained == 0, "a looping one never runs out");
    ok(v->playing != 0, "and goes on being fed");
    ok((SPL_i_VoiceStatus(v) & SPL_LOOPING) != 0, "and says it is looping");
    SPL_VoiceDestroy(v);
}

static void theMix(void)
{
    tSplVoice* v;

    printf("-- attenuation, pan and frequency\n");
    v = SPL_p_VoiceCreate(2, 22050, SPL_CHUNK_FRAMES * 4 * 4);

    /* The units are the engine's: hundredths of a decibel, 0 for
       untouched and -10000 for silence. */
    okNear(SPL_l_f_Gain(0), 1.0, 1e-6, "no attenuation is unity gain");
    okNear(SPL_l_f_Gain(-10000), 0.0, 1e-6, "full attenuation is silence");
    okNear(SPL_l_f_Gain(-600), 0.5012, 1e-3, "six decibels is about half");
    okNear(SPL_l_f_Gain(-2000), 0.1, 1e-4, "twenty decibels is a tenth");

    SPL_VoiceSetAttenuation(v, -600);
    okEq(SPL_i_VoiceAttenuation(v), -600, "the voice remembers its attenuation");
    okNear(SDL_GetAudioStreamGain(v->stream), 0.5012, 1e-3,
           "and it reaches the stream");

    /* Two attenuations in decibels add, so half of half is a quarter. */
    SPL_SetAttenuation(-600);
    okNear(SDL_GetAudioStreamGain(v->stream), 0.2512, 1e-3,
           "the device's attenuation adds to the voice's");
    SPL_SetAttenuation(0);
    okNear(SDL_GetAudioStreamGain(v->stream), 0.5012, 1e-3, "and comes back off");

    SPL_VoiceSetAttenuation(v, 500);
    okEq(SPL_i_VoiceAttenuation(v), 0, "a positive attenuation is clamped to none");
    SPL_VoiceSetAttenuation(v, -99999);
    okEq(SPL_i_VoiceAttenuation(v), -10000, "and a huge one to silence");
    SPL_VoiceSetAttenuation(v, 0);

    /* The pan is the difference between the channels: one side is
       taken down and the other left alone. */
    {
        float l, r;

        SPL_l_PanGains(0, &l, &r);
        okNear(l, 1.0, 1e-6, "centred, the left channel is untouched");
        okNear(r, 1.0, 1e-6, "and so is the right");
        SPL_l_PanGains(10000, &l, &r);
        okNear(l, 0.0, 1e-6, "hard right silences the left");
        okNear(r, 1.0, 1e-6, "and leaves the right alone");
        SPL_l_PanGains(-10000, &l, &r);
        okNear(l, 1.0, 1e-6, "hard left leaves the left alone");
        okNear(r, 0.0, 1e-6, "and silences the right");
        SPL_l_PanGains(-600, &l, &r);
        okNear(r, 0.5012, 1e-3, "and six decibels over takes one side to half");
    }

    SPL_VoiceSetPan(v, -4000);
    okEq(SPL_i_VoicePan(v), -4000, "the voice remembers its pan");
    SPL_VoiceSetPan(v, 99999);
    okEq(SPL_i_VoicePan(v), 10000, "clamped hard over");
    SPL_VoiceSetPan(v, 0);

    /* The frequency is a ratio on the stream rather than a new stream,
       which is what SetFrequency on a buffer did. */
    okEq(SPL_i_VoiceFrequency(v), 22050, "the voice knows its rate");
    SPL_VoiceSetFrequency(v, 44100);
    okEq(SPL_i_VoiceFrequency(v), 44100, "and takes a new one");
    okNear(SDL_GetAudioStreamFrequencyRatio(v->stream), 2.0, 1e-4,
           "which reaches the stream as a ratio");
    SPL_VoiceSetFrequency(v, 22050);
    okNear(SDL_GetAudioStreamFrequencyRatio(v->stream), 1.0, 1e-4,
           "and back again");

    SPL_SetPan(-2500);
    okEq(SPL_i_Pan(), -2500, "the device remembers its pan");
    SPL_SetPan(0);
    SPL_VoiceDestroy(v);
}

static void thePanReachesTheAudio(void)
{
    tSplVoice* v;
    int16_t out[SPL_CHUNK_FRAMES * 2];
    int got;

    printf("-- the pan reaches the samples\n");
    /* A mono voice of a constant, so the two fed channels can be
       compared against each other rather than against a formula. */
    v = SPL_p_VoiceCreate(1, SPL_RATE, SPL_CHUNK_FRAMES * 2 * 4);
    {
        int16_t* s = (int16_t*)malloc((size_t)SPL_i_VoiceBytes(v));
        int i;
        for (i = 0; i < SPL_i_VoiceBytes(v) / 2; i++)
            s[i] = 10000;
        SPL_b_VoiceWrite(v, 0, s, SPL_i_VoiceBytes(v));
        free(s);
    }

    SPL_VoiceSetPan(v, -10000);     /* hard left */
    SPL_VoicePlay(v, 1);
    SPL_Feed();
    got = SDL_GetAudioStreamData(v->stream, out, (int)sizeof(out));
    ok(got > 0, "something was fed");
    okEq(out[0], 10000, "hard left, the left sample is untouched");
    okEq(out[1], 0, "and the right is silent");

    SDL_ClearAudioStream(v->stream);
    SPL_VoiceSetPan(v, 10000);      /* hard right */
    SPL_Feed();
    got = SDL_GetAudioStreamData(v->stream, out, (int)sizeof(out));
    ok(got > 0, "something was fed again");
    okEq(out[0], 0, "hard right, the left sample is silent");
    okEq(out[1], 10000, "and the right is untouched");

    /* A mono sound reaches both channels, which it has to: the device
       is stereo and a mono sound is not meant to come out of one
       speaker. */
    SDL_ClearAudioStream(v->stream);
    SPL_VoiceSetPan(v, 0);
    SPL_Feed();
    got = SDL_GetAudioStreamData(v->stream, out, (int)sizeof(out));
    ok(got > 0, "and once more");
    okEq(out[0], 10000, "centred, a mono sound reaches the left");
    okEq(out[1], 10000, "and the right");

    /* The device's pan multiplies the voice's rather than replacing
       it, so hard over on both is still hard over. */
    SDL_ClearAudioStream(v->stream);
    SPL_SetPan(10000);
    SPL_Feed();
    got = SDL_GetAudioStreamData(v->stream, out, (int)sizeof(out));
    okEq(out[0], 0, "the device's pan silences the left too");
    SPL_SetPan(0);
    SPL_VoiceDestroy(v);
}

static void theDuplicate(void)
{
    tSplVoice* a;
    tSplVoice* b;

    printf("-- a duplicate shares the sound and nothing else\n");
    a = SPL_p_VoiceCreate(1, SPL_RATE, SPL_CHUNK_FRAMES * 2 * 4);
    fillRamp(a, SPL_i_VoiceBytes(a));
    SPL_VoiceSetAttenuation(a, -300);
    SPL_VoiceSetPan(a, -1000);

    b = SPL_p_VoiceDuplicate(a);
    ok(b != NULL, "a duplicate is made");
    ok(b->pcm == a->pcm, "over the same sound");
    ok(b->stream != a->stream, "with a stream of its own");
    okEq(SPL_i_VoiceAttenuation(b), -300, "the mix is carried over");
    okEq(SPL_i_VoicePan(b), -1000, "pan and all");

    SPL_VoicePlay(a, 0);
    SPL_Feed();
    ok(a->cursor > 0, "the first voice advances");
    okEq(b->cursor, 0, "and the duplicate does not");

    /* Destroying one must not take the sound out from under the other,
       which is what the user count is for. */
    SPL_VoiceDestroy(a);
    okEq(*b->pcmUsers, 1, "destroying one leaves the sound with the other");
    ok(*(int16_t*)b->pcm == 1, "and the sound is still there");
    SPL_VoiceDestroy(b);
}

static void feedWanted(void)
{
    tSplVoice* v;
    int i;

    printf("-- whether the thread has anything to do\n");
    v = SPL_p_VoiceCreate(1, SPL_RATE, SPL_CHUNK_FRAMES * 2 * 8);
    fillRamp(v, SPL_i_VoiceBytes(v));
    okEq(SPL_b_FeedWanted(), 0, "a stopped voice wants nothing");
    SPL_VoicePlay(v, 1);
    ok(SPL_b_FeedWanted() != 0, "a playing one wants feeding");
    for (i = 0; i < SPL_QUEUE_CHUNKS + 2; i++)
        SPL_Feed();
    okEq(SPL_b_FeedWanted(), 0, "and stops wanting it once it is full");
    SPL_VoiceDestroy(v);
}

/* The streaming ring, driven the way win/winsound.c drives it.
 *
 * A streamed sound is a ring of four blocks played looping, which the
 * sound thread refills a block behind the cursor. Everything the
 * engine decides -- when to refill, how far the sound has got, whether
 * it is still playing -- it works out from SPL_i_VoiceCursor and
 * nothing else, so that arithmetic has to hold over many passes of the
 * ring and not just the first.
 *
 * This is the loop out of pcSND_l_ServiceStream, with the file and the
 * decoder taken out: the cursor, the block comparison and the running
 * total, against a real device. */
static void theStreamingRing(void)
{
    const int block = 8192;                 /* sizeToUpload */
    const int ring = 4 * block;             /* soundBufferSize */
    tSplVoice* v;
    int currentPlayPosition, lastPlayedBlock, currOffset;
    long long currSize;
    int tick, refills, biggestAdvance, wentBackwards, everStopped;
    int16_t* payload;

    printf("-- the streaming ring over several passes\n");

    v = SPL_p_VoiceCreate(1, 22050, ring);
    ok(v != 0, "a ring of four blocks");
    if (!v)
        return;
    fillRamp(v, ring);

    payload = (int16_t*)malloc((size_t)block);
    memset(payload, 0x11, (size_t)block);

    SPL_VoicePlay(v, 1);                    /* streams are played looping */

    currentPlayPosition = 0;
    lastPlayedBlock = SPL_i_VoiceCursor(v) / block;
    currOffset = 0;
    currSize = 0;
    refills = 0;
    biggestAdvance = 0;
    wentBackwards = 0;
    everStopped = 0;

    /* Long enough to go round the ring several times: the ring is
       16384 bytes of mono 22050, which is a bit under a second, and a
       tick is the thread's own 5 ms. */
    for (tick = 0; tick < 500; tick++)
    {
        int playPos, advanced, blockNow;

        if ((SPL_i_VoiceStatus(v) & SPL_PLAYING) == 0)
            everStopped = 1;

        playPos = SPL_i_VoiceCursor(v);
        if (playPos >= currentPlayPosition)
            advanced = playPos - currentPlayPosition;
        else
        {
            advanced = playPos + ring - currentPlayPosition;
            /* A real wrap crosses the end of the ring, so the position
               before it was near the top and the one after it near the
               bottom. Anything else is the cursor going backwards, and
               this arm charges a whole ring for it. */
            if (!(currentPlayPosition > ring - block && playPos < block))
                wentBackwards++;
        }
        if (advanced > biggestAdvance)
            biggestAdvance = advanced;
        currSize += advanced;
        currentPlayPosition = playPos;

        blockNow = currentPlayPosition / block;
        if (blockNow != lastPlayedBlock)
        {
            lastPlayedBlock = blockNow;
            SPL_b_VoiceWrite(v, currOffset, payload, block);
            currOffset += block;
            if (currOffset >= ring)
                currOffset -= ring;
            refills++;
        }

        SPL_Feed();
        SDL_Delay(5);
    }

    ok(everStopped == 0, "a looping ring never reads as stopped");
    okEq(wentBackwards, 0, "the play cursor never goes backwards");
    ok(biggestAdvance < block, "no single tick is charged a whole block");
    /* 500 ticks of 5 ms is about 2.5 s, and the ring is 16384 bytes at
       44100 bytes a second -- so a bit over six passes, or about 25
       refills. Generous bounds: the point is that refills keep coming
       and are not wildly over. */
    ok(refills >= 8, "the refills keep coming");
    ok(refills <= 64, "and there is about one per block, not many");
    /* What the engine reports as progress has to be what was actually
       consumed, within a tick's worth. */
    ok(currSize > 4 * (long long)block, "the running total advances");
    ok(currSize < 40 * (long long)block, "and does not run away");
    printf("   %d refills, total %lld bytes, largest advance %d of %d\n",
           refills, currSize, biggestAdvance, block);

    SPL_VoiceDestroy(v);
    free(payload);
}

/* The sound thread and the mailbox the engine hands it work through.
 *
 * What this has to behave like is four Windows events and a fifth for
 * the answer: one command outstanding at a time, the poster waiting
 * for the answer, and a timeout on the thread's own wait that is its
 * clock rather than an error. */

static SDL_AtomicInt theTicks;
static SDL_AtomicInt theSeen;
static SDL_AtomicInt theStop;

static int splTestThread(void* arg)
{
    (void)arg;
    while (!SDL_GetAtomicInt(&theStop))
    {
        int cmd = SPL_i_TakeCommand(5);

        if (cmd == SPL_CMD_ERROR)
            continue;
        if (cmd == SPL_CMD_NONE)
        {
            SDL_AddAtomicInt(&theTicks, 1);
            continue;
        }
        SDL_SetAtomicInt(&theSeen, cmd);
        SPL_CommandDone();
    }
    return 0;
}

static void theMailbox(void)
{
    tSplThread* t;
    int i;
    int ticksBefore;

    printf("-- the thread and the commands handed to it\n");

    SDL_SetAtomicInt(&theTicks, 0);
    SDL_SetAtomicInt(&theSeen, -99);
    SDL_SetAtomicInt(&theStop, 0);

    t = SPL_p_ThreadStart(splTestThread, 0, "spl test");
    ok(t != 0, "the thread starts");
    if (!t)
        return;

    /* Every command gets through, and the poster is not let go until
       the thread has actually had it. */
    for (i = 0; i < 4; i++)
    {
        ok(SPL_b_Command(i, 2000) != 0, "the command is answered");
        okEq(SDL_GetAtomicInt(&theSeen), i, "and it is the one that was sent");
    }

    /* The timeout is the thread's clock, not an error: with nothing
       posted it must keep ticking. */
    ticksBefore = SDL_GetAtomicInt(&theTicks);
    SDL_Delay(60);
    ok(SDL_GetAtomicInt(&theTicks) > ticksBefore,
       "the wait times out and goes round again");

    /* And a command still gets through once it has been ticking. */
    ok(SPL_b_Command(2, 2000) != 0, "a command after a quiet spell is answered");
    okEq(SDL_GetAtomicInt(&theSeen), 2, "and is still the right one");

    SDL_SetAtomicInt(&theStop, 1);
    ok(SPL_b_ThreadWait(t, 2000) != 0, "and the thread is waited for");
}

int main(void)
{
    SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy");

    theDevice();
    theBuffer();
    theCursor();
    theEnd();
    theMix();
    thePanReachesTheAudio();
    theDuplicate();
    feedWanted();
    theStreamingRing();
    theMailbox();

    SPL_Close();
    ok(SPL_b_IsOpen() == 0, "closing gives the device back");
    okEq((long)(size_t)spl_voices, 0, "and takes every voice with it");

    printf("  %d checks, %d failure(s)\n", checks, failures);
    if (failures)
        printf("FAIL\n");
    return failures ? 1 : 0;
}
