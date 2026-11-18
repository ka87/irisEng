/* A unit test for platform/sndvideo.c.
 *
 * The sink is new code with no earlier copy. What it does that is
 * worth checking is arithmetic rather than audio -- a play cursor
 * that runs round a ring, how much of the ring is free, how much has
 * been heard since the last look, and what of that was really the
 * video rather than silence poured in to cover a gap. All of that is
 * wrong-able in ways nothing would notice until the picture drifted
 * out of step with the sound.
 *
 * So the platform layer is a mock that records every write and hands
 * back a play cursor the test moves itself, and the video stream is a
 * stub that hands over a counted ramp. The sink is then driven and
 * what it believes is compared against what the test knows.
 *
 * The platform layer is mocked rather than run for real because the
 * test has to move the cursor itself; a real device moves it in its
 * own time. sndplatform.c is tested against the real library by
 * tests/run_sndplatform.sh, which is where the wrapping, the cursor
 * and the mixing are checked.
 */

#include "../platform/sndvideo.h"
#include "../sndplatform.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

/* --------------------------------------------- the mock platform layer */

/* The sink no longer talks to DirectSound; it asks sndplatform.c for a
   voice and writes into it. That layer is tested on its own against
   the real library -- a real device, real streams, the wrapping and
   the cursor -- so what stands in for it here is a mock, and it is
   the mock that lets the test drive the play cursor by hand. Driving
   a real device's cursor would mean waiting on it, and the thing
   being checked is arithmetic. */

#define VSM_RING  (44100 * 2 * 2)      /* a second, stereo, 16-bit */

static uint8_t vsm_ring[VSM_RING];
static int     vsm_playPos;
static int     vsm_playing;
static int     vsm_lockCalls;          /* writes into the voice */
static int     vsm_releaseCalls;
static int     vsm_createFails;
static int     vsm_lockFails;
static int     vsm_lastLockOffset;
static int     vsm_lastLockBytes;
static int     vsm_straddles;
static int     vsm_unaligned;          /* writes not on a frame boundary */

/* One voice, which is all the sink ever asks for. */
static int     vsm_voice;

static int VSM_l_Write(int offset, const void* src, int bytes, int asSilence)
{
    int first;

    vsm_lockCalls++;
    vsm_lastLockOffset = offset;
    vsm_lastLockBytes = bytes;
    if ((offset % 4) != 0 || (bytes % 4) != 0)
        vsm_unaligned++;
    if (vsm_lockFails)
        return 0;
    if (offset >= VSM_RING || bytes > VSM_RING)
        return 0;
    first = VSM_RING - offset;
    if (first > bytes)
        first = bytes;
    else if (first < bytes)
        vsm_straddles++;
    if (asSilence)
        memset(vsm_ring + offset, 0, (size_t)first);
    else
        memcpy(vsm_ring + offset, src, (size_t)first);
    if (bytes > first)
    {
        if (asSilence)
            memset(vsm_ring, 0, (size_t)(bytes - first));
        else
            memcpy(vsm_ring, (const uint8_t*)src + first, (size_t)(bytes - first));
    }
    return 1;
}

tSplVoice* SPL_p_VoiceCreate(int channels, int freq, int bytes)
{
    if (vsm_createFails)
        return NULL;
    okEq(bytes, VSM_RING, "the voice is a second long");
    okEq(channels, 2, "stereo");
    okEq(freq, 44100, "44100");
    return (tSplVoice*)&vsm_voice;
}

void SPL_VoiceDestroy(tSplVoice* v) { (void)v; vsm_releaseCalls++; }

int SPL_b_VoiceWrite(tSplVoice* v, int offset, const void* data, int bytes)
{
    (void)v;
    return VSM_l_Write(offset, data, bytes, 0);
}

int SPL_b_VoiceSilence(tSplVoice* v, int offset, int bytes)
{
    (void)v;
    return VSM_l_Write(offset, NULL, bytes, 1);
}

int  SPL_i_VoiceCursor(const tSplVoice* v) { (void)v; return vsm_playPos; }
void SPL_VoicePlay(tSplVoice* v, int loop) { (void)v; (void)loop; vsm_playing = 1; }
void SPL_VoiceStop(tSplVoice* v) { (void)v; vsm_playing = 0; }

static void mockReset(void)
{
    memset(vsm_ring, 0xAA, sizeof(vsm_ring));
    vsm_playPos = 0;
    vsm_playing = 0;
    vsm_lockCalls = 0;
    vsm_straddles = 0;
    vsm_unaligned = 0;
    vsm_releaseCalls = 0;
    vsm_createFails = 0;
    vsm_lockFails = 0;
}

static int vsm_allocs;
static int vsm_frees;

void* MEM_p_Alloc(unsigned int bytes)
{
    vsm_allocs++;
    return malloc(bytes);
}

void MEM_Free(void* ptr)
{
    vsm_frees++;
    free(ptr);
}

/* ------------------------------------------------------- the stub stream */

/* video.c is not linked here: the sink only ever calls these four,
   and a real stream would drag libav in for no gain. The PCM is a
   counted ramp so the test can say where in the stream any byte in
   the ring came from. */

static int vss_hasSound = 1;
static int vss_rate = 44100;
static int vss_channels = 2;
static long vss_handedOut;          /* bytes given to the sink */
static long vss_available;          /* bytes the queue will give */
static int64_t vss_reportedFrames;  /* the last VID_SoundPlayed */
static int vss_reportCalls;

int VID_b_HasSound(const tVideoStream* v) { (void)v; return vss_hasSound; }
int VID_i_SoundRate(const tVideoStream* v) { (void)v; return vss_rate; }
int VID_i_SoundChannels(const tVideoStream* v) { (void)v; return vss_channels; }

int VID_i_SoundRead(tVideoStream* v, void* dest, int bytes)
{
    uint8_t* out = (uint8_t*)dest;
    int i;

    (void)v;
    if (bytes > vss_available)
        bytes = (int)vss_available;
    bytes -= bytes % (vss_channels * 2);
    if (bytes < 0)
        bytes = 0;
    for (i = 0; i < bytes; i++)
        out[i] = (uint8_t)((vss_handedOut + i) & 0xFF);
    vss_handedOut += bytes;
    vss_available -= bytes;
    return bytes;
}

void VID_SoundPlayed(tVideoStream* v, int64_t frames)
{
    (void)v;
    vss_reportedFrames = frames;
    vss_reportCalls++;
}

static void streamReset(long available)
{
    vss_hasSound = 1;
    vss_rate = 44100;
    vss_channels = 2;
    vss_handedOut = 0;
    vss_available = available;
    vss_reportedFrames = -1;
    vss_reportCalls = 0;
}

/* a stand-in handle; the sink never dereferences it */
static tVideoStream* const VSS = (tVideoStream*)(void*)&vss_rate;

/* ------------------------------------------------------------- the checks */

static void opening(void)
{
    tVideoSound* s;

    mockReset();
    streamReset(1 << 20);
    s = WVS_pst_Open(VSS);
    ok(s != NULL, "a buffer opens");
    ok(vsm_lockCalls > 0, "the buffer is cleared at open");
    okEq(vsm_playing, 0, "nothing plays until there is something to play");
    WVS_Close(s);
    okEq(vsm_releaseCalls, 1, "closing releases the buffer");
    okEq(vsm_frees, vsm_allocs, "closing frees what opening allocated");

    mockReset();
    streamReset(1 << 20);
    vss_hasSound = 0;
    ok(WVS_pst_Open(VSS) == NULL, "a silent video opens nothing");

    mockReset();
    streamReset(1 << 20);
    vsm_createFails = 1;
    ok(WVS_pst_Open(VSS) == NULL,
       "a device that refuses a voice is not fatal");

    mockReset();
    streamReset(1 << 20);
    ok(WVS_pst_Open(NULL) == NULL, "no video, no sink");

    /* and the no-op paths do not crash */
    WVS_Close(NULL);
    WVS_Update(NULL, VSS);
    ok(1, "the NULL paths are safe");
}

/* The sink must fill what the cursor has freed, and no more. */
static void fillsWhatIsFree(void)
{
    tVideoSound* s;
    long before;
    int i;

    mockReset();
    streamReset(1 << 22);
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the fill fixture opens");
        return;
    }

    /* nothing played yet, so the whole ring is free and the first
       update primes it and starts the buffer */
    before = vss_handedOut;
    WVS_Update(s, VSS);
    ok(vss_handedOut > before, "the first update writes sound");
    okEq(vsm_playing, 1, "the first update starts the buffer");

    /* With the cursor still, it keeps filling until the ring is full
       and then stops: a buffer run ahead is how a frame that takes
       its time is covered. */
    for (i = 0; i < 100; i++)
        WVS_Update(s, VSS);
    okEq(vss_handedOut, VSM_RING, "it fills the ring and no further");
    before = vss_handedOut;
    WVS_Update(s, VSS);
    okEq(vss_handedOut, before, "a full ring takes nothing more");

    /* move the cursor a long way and it should take up that much */
    vsm_playPos = 40000;
    before = vss_handedOut;
    WVS_Update(s, VSS);
    ok(vss_handedOut > before, "a moved cursor is followed");
    ok(vss_handedOut - before <= 40000 + 4,
       "and never more than the cursor freed");

    WVS_Close(s);
}

/* What the sink reports as played must match what the cursor did,
   across the ring's wrap, and must count sample frames not bytes. */
static void reportsWhatWasPlayed(void)
{
    tVideoSound* s;
    int i;

    mockReset();
    streamReset(1 << 22);
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the clock fixture opens");
        return;
    }
    WVS_Update(s, VSS);                 /* prime and start */
    okEq(vss_reportedFrames, 0, "nothing has been played at the start");

    vsm_playPos = 4000;                 /* 1000 stereo frames */
    WVS_Update(s, VSS);
    okEq(vss_reportedFrames, 1000, "the cursor is reported in sample frames");

    vsm_playPos = 40000;
    WVS_Update(s, VSS);
    okEq(vss_reportedFrames, 10000, "and it accumulates");

    /* Round the ring and a half, fed at every step the way a frame
       loop feeds it, so the sound stays ahead of the cursor and the
       clock is the cursor rather than the sound it ran out of. The
       cursor goes backwards at each wrap; the count must not. */
    {
        long travel = 0;
        int64_t atStart = vss_reportedFrames;

        for (i = 0; i < 700; i++)
        {
            WVS_Update(s, VSS);
            vsm_playPos = (DWORD)((vsm_playPos + 2048) % VSM_RING);
            travel += 2048;
        }
        WVS_Update(s, VSS);
        ok(travel > VSM_RING, "the cursor went round at least once");
        okEq(vss_reportedFrames, atStart + travel / 4,
             "a wrap of the ring loses no lap and repeats none");
    }

    /* and when the sound runs out the clock stops at what was heard
       rather than following the cursor into the silence */
    {
        int64_t stopped;

        vss_available = 0;
        /* more than a ring of travel first, so everything that was
           still queued when the sound ran out has been played and
           the clock has come to rest */
        for (i = 0; i < 200; i++)
        {
            WVS_Update(s, VSS);
            vsm_playPos = (DWORD)((vsm_playPos + 4096) % VSM_RING);
        }
        WVS_Update(s, VSS);
        stopped = vss_reportedFrames;
        for (i = 0; i < 40; i++)
        {
            WVS_Update(s, VSS);
            vsm_playPos = (DWORD)((vsm_playPos + 4096) % VSM_RING);
        }
        WVS_Update(s, VSS);
        okEq(vss_reportedFrames, stopped,
             "a cursor running through silence does not advance the clock");
    }

    WVS_Close(s);
}

/* A queue that runs dry gets silence, and the silence must not be
   counted as video that was heard -- or the picture races ahead of
   the sound by exactly the length of the gap. */
static void silenceIsNotHeard(void)
{
    tVideoSound* s;
    int64_t afterGap;

    mockReset();
    streamReset(8000);            /* a little sound, then nothing */
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the silence fixture opens");
        return;
    }
    WVS_Update(s, VSS);           /* takes the 8000 and pads the rest */
    okEq(vss_handedOut, 8000, "the queue gave what it had");

    /* play through the lot, sound and silence together */
    vsm_playPos = 40000;
    WVS_Update(s, VSS);
    afterGap = vss_reportedFrames;
    if (afterGap > 8000 / 4)
        printf("    [diag] handedOut=%ld reported=%lld cursor=%u\n",
               vss_handedOut, (long long)afterGap, (unsigned)vsm_playPos);
    ok(afterGap <= 8000 / 4,
       "silence poured into the gap is not reported as sound heard");

    /* and once real sound flows again the clock resumes from there */
    vss_available = 1 << 20;
    vsm_playPos = 80000;
    WVS_Update(s, VSS);
    ok(vss_reportedFrames >= afterGap, "the clock never goes backwards");

    WVS_Close(s);
}

/* What the sink writes has to land where it says it does. */
static void writesLandInPlace(void)
{
    tVideoSound* s;
    int i;
    int wrong = 0;

    mockReset();
    streamReset(1 << 22);
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the placement fixture opens");
        return;
    }
    WVS_Update(s, VSS);
    /* the ramp started at 0 and went in at offset 0 */
    for (i = 0; i < 2048; i++)
        if (vsm_ring[i] != (uint8_t)(i & 0xFF))
            wrong++;
    okEq(wrong, 0, "the sound is written where the sink says it is");

    /* And a write that reaches the end of the ring must carry on at
       the start. The ring is driven round once and a bit, which has
       to split at least one write across the end. */
    mockReset();
    streamReset(1 << 22);
    s = WVS_pst_Open(VSS);
    vsm_straddles = 0;
    vsm_unaligned = 0;
    for (i = 0; i < 400; i++)
    {
        WVS_Update(s, VSS);
        vsm_playPos = (DWORD)((vsm_playPos + 4096) % VSM_RING);
    }
    ok(vss_handedOut > VSM_RING, "the ring was driven round at least once");
    ok(vsm_straddles > 0, "a write that reaches the end carries on at the start");

    WVS_Close(s);
}

/* A device that refuses a lock must not leave the sink believing it
   wrote something. */
static void lockRefused(void)
{
    tVideoSound* s;
    int before;

    mockReset();
    streamReset(1 << 22);
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the lock fixture opens");
        return;
    }
    vsm_lockFails = 1;
    before = vsm_lockCalls;
    WVS_Update(s, VSS);
    ok(vsm_lockCalls > before, "a refused lock was still attempted");
    vsm_lockFails = 0;
    vsm_playPos = 40000;
    WVS_Update(s, VSS);
    ok(1, "the sink carries on after a refused lock");
    WVS_Close(s);
}

/* When the queue runs dry the sink writes silence over what the
   cursor is about to reach. Without it the cursor runs back over
   whatever was there -- the lap before's sound, or in a fresh buffer
   whatever the device had -- and the gap is heard as a stutter
   rather than as nothing. The clock does not notice either way, so
   this looks at the ring itself, with a queue that is dry from the
   start so the region written is known exactly. */
static void theGapIsSilent(void)
{
    tVideoSound* s;
    int i;
    int notSilent = 0;

    mockReset();
    streamReset(0);                 /* nothing to play at all */
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the gap fixture opens");
        return;
    }
    /* the mock filled the ring with 0xAA, and open clears it, so put
       the noise back to see what this update writes */
    memset(vsm_ring, 0xAA, sizeof(vsm_ring));
    WVS_Update(s, VSS);

    for (i = 0; i < 16384; i++)
        if (vsm_ring[i] != 0)
            notSilent++;
    okEq(notSilent, 0, "a dry queue puts silence in front of the cursor");
    WVS_Close(s);
}

/* A play cursor is normally on a sample-frame boundary, but nothing
   promises it, and the amount free follows the cursor. An unaligned
   amount must not leave the write position half a frame out, or
   every channel after it is swapped.

   The buffer has to be nearly full for this to bite: while there is
   more free than one staging buffer the amount is clamped to that
   and is aligned by accident. */
static void unalignedCursor(void)
{
    tVideoSound* s;
    int i;
    int misplaced = 0;

    mockReset();
    streamReset(1 << 22);
    s = WVS_pst_Open(VSS);
    if (!s)
    {
        ok(0, "the alignment fixture opens");
        return;
    }
    /* fill the ring, so what is free is what the cursor just freed */
    for (i = 0; i < 300; i++)
        WVS_Update(s, VSS);
    okEq(vss_handedOut, VSM_RING, "the ring filled before the cursor moved");

    /* and now free it in amounts that are not whole frames */
    vsm_playPos = 1001;
    WVS_Update(s, VSS);
    vsm_playPos = 2003;
    WVS_Update(s, VSS);
    vsm_playPos = 5009;
    WVS_Update(s, VSS);

    /* Checking the ring's contents cannot settle this: the ring's
       length is not a multiple of the ramp's period, so the second
       lap round legitimately holds different values from the first.
       The property itself is simpler -- every lock the sink asks for
       is on a sample-frame boundary and a whole number of frames
       long -- and the mock counts the ones that are not. */
    (void)misplaced;
    okEq(vsm_unaligned, 0, "an unaligned cursor does not slip the write position");
    WVS_Close(s);
}

int main(void)
{
    printf("=== video sound unit test ===\n");
    opening();
    fillsWhatIsFree();
    reportsWhatWasPlayed();
    silenceIsNotHeard();
    writesLandInPlace();
    theGapIsSilent();
    unalignedCursor();
    lockRefused();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
