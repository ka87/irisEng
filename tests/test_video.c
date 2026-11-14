/* A unit test for video.c, run against real libav.
 *
 * video.c is new code with no earlier copy, so this is a direct check
 * of what it does rather than a differential one. It is also the only
 * test here that links a real third-party library instead of mocking
 * it: the point of the module is that libav decodes the picture, and
 * a mocked libav would check nothing.
 *
 * The fixtures are made by the ffmpeg command line in run_video.sh,
 * so the expected values below are properties of those files.
 *
 * The strongest check is not any single expected number: it is that
 * the file path and the reader path, handed the same bytes, produce
 * byte-identical pixels. That needs no magic values and catches most
 * of what can go wrong in between.
 */

#include "../video.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

static void okNear(int got, int want, int slack, const char* what)
{
    checks++;
    if (got < want - slack || got > want + slack)
    {
        printf("  FAIL %s: got %d want %d +/- %d\n", what, got, want, slack);
        fails++;
    }
}

/* ------------------------------------------------------------ a reader */

/* The .bf reader the procedural textures use is exactly this shape:
   bytes in order, from a bounded region, and no seeking. */
typedef struct
{
    const uint8_t* data;
    int            size;
    int            pos;
    int            failAfter;   /* -1 never */
    int            calls;
} tFeed;

static int feedRead(void* user, uint8_t* buf, int size)
{
    tFeed* f = (tFeed*)user;
    int left;

    f->calls++;
    if (f->failAfter >= 0 && f->calls > f->failAfter)
        return -1;
    left = f->size - f->pos;
    if (left <= 0)
        return 0;
    if (size > left)
        size = left;
    memcpy(buf, f->data + f->pos, (size_t)size);
    f->pos += size;
    return size;
}

static uint8_t* slurp(const char* path, int* sizeOut)
{
    FILE* fp;
    uint8_t* buf;
    long size;

    fp = fopen(path, "rb");
    if (!fp)
        return NULL;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = (uint8_t*)malloc((size_t)size);
    if (!buf || fread(buf, 1, (size_t)size, fp) != (size_t)size)
    {
        free(buf);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    *sizeOut = (int)size;
    return buf;
}

/* --------------------------------------------------------------- checks */

#define VT_W 64
#define VT_H 48

static void openAndShape(const char* path)
{
    tVideoStream* v;

    v = VID_pst_OpenFile(path, 0);
    ok(v != NULL, "a file opens");
    if (!v)
        return;
    okEq(VID_i_Width(v), VT_W, "the width comes from the file");
    okEq(VID_i_Height(v), VT_H, "the height comes from the file");
    okEq(VID_i_FrameNum(v), 0, "nothing has been shown yet");
    okEq(VID_b_Ended(v), 0, "a fresh stream has not ended");
    VID_Close(v);

    ok(VID_pst_OpenFile("does-not-exist.avi", 0) == NULL, "a missing file is NULL");
    ok(VID_pst_OpenFile(NULL, 0) == NULL, "a NULL path is NULL");
    ok(VID_pst_OpenFile("", 0) == NULL, "an empty path is NULL");
    ok(VID_pst_OpenReader(NULL, NULL, 0) == NULL, "a NULL reader is NULL");

    /* every accessor survives a NULL stream, because the engine's
       call sites test the handle and then use it */
    okEq(VID_i_Width(NULL), 0, "width of nothing is 0");
    okEq(VID_b_Ended(NULL), 1, "nothing has ended");
    okEq(VID_b_Waiting(NULL), 0, "nothing is not waiting");
    okEq(VID_b_DecodeFrame(NULL), 0, "nothing decodes nothing");
    VID_NextFrame(NULL);
    VID_CopyToBuffer(NULL, NULL, 0, 0, 0);
    VID_Close(NULL);
    ok(1, "the NULL path does not crash");
}

/* The same bytes down both paths must give the same pixels. */
static void bothPathsAgree(const char* path)
{
    tVideoStream* a;
    tVideoStream* b;
    tFeed feed;
    uint8_t* bytes;
    int size = 0;
    uint8_t* pixA;
    uint8_t* pixB;
    int frames = 0;
    int differing = 0;

    bytes = slurp(path, &size);
    ok(bytes != NULL, "the fixture can be read");
    if (!bytes)
        return;

    memset(&feed, 0, sizeof(feed));
    feed.data = bytes;
    feed.size = size;
    feed.failAfter = -1;

    a = VID_pst_OpenFile(path, 0);
    b = VID_pst_OpenReader(feedRead, &feed, 0);
    ok(a != NULL, "the file path opens");
    ok(b != NULL, "the reader path opens");
    if (!a || !b)
    {
        VID_Close(a);
        VID_Close(b);
        free(bytes);
        return;
    }
    okEq(VID_i_Width(b), VID_i_Width(a), "both paths agree on the width");
    okEq(VID_i_Height(b), VID_i_Height(a), "both paths agree on the height");

    pixA = (uint8_t*)malloc((size_t)VT_W * VT_H * 4);
    pixB = (uint8_t*)malloc((size_t)VT_W * VT_H * 4);

    while (VID_b_DecodeFrame(a))
    {
        if (!VID_b_DecodeFrame(b))
        {
            ok(0, "the reader path ran out of frames first");
            break;
        }
        memset(pixA, 0, (size_t)VT_W * VT_H * 4);
        memset(pixB, 0, (size_t)VT_W * VT_H * 4);
        VID_CopyToBuffer(a, pixA, VT_W * 4, VT_H, 0);
        VID_CopyToBuffer(b, pixB, VT_W * 4, VT_H, 0);
        if (memcmp(pixA, pixB, (size_t)VT_W * VT_H * 4) != 0)
            differing++;
        frames++;
        VID_NextFrame(a);
        VID_NextFrame(b);
        if (frames > 200)
            break;
    }
    ok(frames >= 5, "the fixture decodes its frames");
    okEq(differing, 0, "the file path and the reader path give the same pixels");
    okEq(VID_i_FrameNum(a), frames, "the frame number counts what was shown");
    ok(feed.calls > 0, "the reader was actually used");

    free(pixA);
    free(pixB);
    VID_Close(a);
    VID_Close(b);
    free(bytes);
}

/* The fixture's first frame is a known solid colour, which is what
   says the channel order is BGRA and not RGBA. */
static void colourAndAlpha(const char* path)
{
    tVideoStream* v;
    uint8_t* pix;

    v = VID_pst_OpenFile(path, 0);
    if (!v)
    {
        ok(0, "the colour fixture opens");
        return;
    }
    pix = (uint8_t*)malloc((size_t)VT_W * VT_H * 4);
    memset(pix, 0x5A, (size_t)VT_W * VT_H * 4);
    ok(VID_b_DecodeFrame(v), "the first frame decodes");
    VID_CopyToBuffer(v, pix, VT_W * 4, VT_H, 0);

    /* #2040C0: red 0x20, green 0x40, blue 0xC0, through yuv420p */
    okNear(pix[0], 0xC0, 10, "byte 0 is blue");
    okNear(pix[1], 0x40, 10, "byte 1 is green");
    okNear(pix[2], 0x20, 10, "byte 2 is red");

    /* the fourth byte is always written, and a file with no alpha
       plane of its own is opaque either way */
    okEq(pix[3], 0xFF, "without alpha the fourth byte is opaque");
    memset(pix, 0x5A, (size_t)VT_W * VT_H * 4);
    VID_CopyToBuffer(v, pix, VT_W * 4, VT_H, 1);
    okEq(pix[3], 0xFF, "with alpha an opaque file is still opaque");

    free(pix);
    VID_Close(v);
}

/* A surface is wider than the picture and taller than the rows asked
   for, and nothing outside what was asked for may be written. */
static void pitchAndClipping(const char* path)
{
    tVideoStream* v;
    uint8_t* pix;
    int pitch = (VT_W + 11) * 4;
    int surfaceRows = VT_H + 7;
    int askRows = VT_H / 2;
    int i;
    int row;
    int tailDirty = 0;
    int belowDirty = 0;

    v = VID_pst_OpenFile(path, 0);
    if (!v)
    {
        ok(0, "the pitch fixture opens");
        return;
    }
    pix = (uint8_t*)malloc((size_t)pitch * surfaceRows);
    memset(pix, 0xCD, (size_t)pitch * surfaceRows);
    ok(VID_b_DecodeFrame(v), "a frame decodes for the pitch check");
    VID_CopyToBuffer(v, pix, pitch, askRows, 0);

    for (row = 0; row < askRows; row++)
        for (i = VT_W * 4; i < pitch; i++)
            if (pix[row * pitch + i] != 0xCD)
                tailDirty++;
    okEq(tailDirty, 0, "nothing is written past the picture in a row");

    for (row = askRows; row < surfaceRows; row++)
        for (i = 0; i < pitch; i++)
            if (pix[row * pitch + i] != 0xCD)
                belowDirty++;
    okEq(belowDirty, 0, "nothing is written below the rows asked for");

    /* and the rows asked for were written */
    okEq(pix[0] == 0xCD && pix[1] == 0xCD && pix[2] == 0xCD, 0,
         "the first row was written");

    free(pix);
    VID_Close(v);
}

static void endOfStream(const char* path)
{
    tVideoStream* v;
    int frames = 0;

    v = VID_pst_OpenFile(path, 0);
    if (!v)
    {
        ok(0, "the end-of-stream fixture opens");
        return;
    }
    while (VID_b_DecodeFrame(v) && frames < 500)
    {
        frames++;
        VID_NextFrame(v);
    }
    ok(frames > 0, "the stream yielded frames before ending");
    okEq(VID_b_Ended(v), 1, "the stream reports that it ended");
    okEq(VID_b_DecodeFrame(v), 0, "decoding past the end returns nothing");
    okEq(VID_b_Waiting(v), 0, "an ended stream is never waiting");
    VID_NextFrame(v);
    ok(1, "advancing past the end does not crash");
    VID_Close(v);
}

static void brokenInput(const char* path)
{
    tFeed feed;
    uint8_t* bytes;
    int size = 0;
    tVideoStream* v;
    static const uint8_t junk[64] = { 0 };

    bytes = slurp(path, &size);
    if (!bytes)
        return;

    /* a reader that gives nothing */
    memset(&feed, 0, sizeof(feed));
    feed.data = bytes;
    feed.size = 0;
    feed.failAfter = -1;
    ok(VID_pst_OpenReader(feedRead, &feed, 0) == NULL, "an empty reader is NULL");

    /* a reader that fails at once */
    memset(&feed, 0, sizeof(feed));
    feed.data = bytes;
    feed.size = size;
    feed.failAfter = 0;
    ok(VID_pst_OpenReader(feedRead, &feed, 0) == NULL, "a failing reader is NULL");

    /* bytes that are not a video */
    memset(&feed, 0, sizeof(feed));
    feed.data = junk;
    feed.size = (int)sizeof(junk);
    feed.failAfter = -1;
    ok(VID_pst_OpenReader(feedRead, &feed, 0) == NULL, "junk is NULL");

    /* a reader that dies part way through: it must not hang or crash,
       and whatever it does it must end rather than spin */
    memset(&feed, 0, sizeof(feed));
    feed.data = bytes;
    feed.size = size;
    feed.failAfter = 3;
    v = VID_pst_OpenReader(feedRead, &feed, 0);
    if (v)
    {
        int frames = 0;
        while (VID_b_DecodeFrame(v) && frames < 500)
        {
            frames++;
            VID_NextFrame(v);
        }
        okEq(VID_b_Ended(v), 1, "a reader that dies ends the stream");
        VID_Close(v);
    }
    else
    {
        ok(1, "a reader that dies part way is refused at open");
    }
    free(bytes);
}

static void soundTrack(const char* path)
{
    tVideoStream* v;
    short pcm[4096];
    int got;
    int total = 0;
    int nonZero = 0;
    int i;

    v = VID_pst_OpenFile(path, 1);
    if (!v)
    {
        ok(0, "the sound fixture opens");
        return;
    }
    ok(VID_b_HasSound(v), "the sound track is found");
    okEq(VID_i_SoundRate(v), 44100, "the sample rate comes from the file");
    okEq(VID_i_SoundChannels(v), 2, "the channel count comes from the file");

    /* decoding the picture is what pulls the sound packets through */
    for (i = 0; i < 20 && VID_b_DecodeFrame(v); i++)
    {
        VID_NextFrame(v);
        got = VID_i_SoundRead(v, pcm, (int)sizeof(pcm));
        total += got;
        for (int k = 0; k < got / 2; k++)
            if (pcm[k] != 0)
                nonZero++;
    }
    ok(total > 0, "sound comes out of the queue");
    ok(nonZero > 0, "the sound is not silence");
    okEq(total % 4, 0, "the queue hands over whole stereo frames");

    /* a stream opened without sound has none */
    VID_Close(v);
    v = VID_pst_OpenFile(path, 0);
    ok(v != NULL, "the sound fixture opens again");
    if (v)
    {
        okEq(VID_b_HasSound(v), 0, "sound is not decoded when it was not asked for");
        okEq(VID_i_SoundRead(v, pcm, (int)sizeof(pcm)), 0, "and nothing is queued");
        VID_Close(v);
    }

}

/* The PCM queue is a ring, so it has to be driven past its own end,
   and every sample is checked against its own index rather than
   against its neighbour. The fixture is a 1000-sample sawtooth, so
   sample k must be -32768 + (k mod 1000) * 65.536: one sample
   dropped, repeated or read from the wrong side of the wrap shifts
   the phase and everything after it fails. A continuity check
   against the previous sample would miss a whole period going
   astray; this cannot. */
static int rampWant(long k)
{
    return -32768 + (int)((double)(k % 1000) * 65.536);
}

static void soundRingWrap(const char* path)
{
    tVideoStream* v;
    /* 503 shorts is 1006 bytes: it does not divide the queue's
       length, so the read pointer steps over the end rather than
       landing on it, and it is not a whole number of stereo frames
       either, so this also exercises the rounding that keeps the
       channels the right way round. */
    short pcm[503];
    int got;
    long total = 0;
    long index = 0;        /* the sample frame this read starts at */
    int wrong = 0;
    int i;
    int guard;

    v = VID_pst_OpenFile(path, 1);
    if (!v || !VID_b_HasSound(v))
    {
        ok(0, "the ring fixture opens with sound");
        VID_Close(v);
        return;
    }
    for (guard = 0; guard < 4000 && total < 400 * 1024; guard++)
    {
        if (VID_b_DecodeFrame(v))
            VID_NextFrame(v);

        /* drained to empty every time round: the queue drops what it
           cannot hold, so a reader that falls behind would be
           measuring the drop policy rather than the ring */
        for (;;)
        {
            got = VID_i_SoundRead(v, pcm, (int)sizeof(pcm));
            if (got <= 0)
                break;
            total += got;
            for (i = 0; i + 1 < got / 2; i += 2)
            {
                int want = rampWant(index);
                if (pcm[i] < want - 2 || pcm[i] > want + 2)
                    wrong++;
                if (pcm[i + 1] != pcm[i])
                    wrong++;        /* both channels carry the ramp */
                index++;
            }
        }
        if (VID_b_Ended(v))
            break;
    }
    ok(total > 257 * 1024, "the ring is driven past its own length");
    okEq(wrong, 0, "every sample comes out of the ring where it belongs");
    VID_Close(v);
}

/* And the other way: a reader that never drains. The queue is full
   almost at once and the newest sound is dropped, which is the right
   trade -- the decoder must not stall on a sink that has stopped
   asking. What matters is that it keeps going and still hands back
   whole frames. */
static void soundOverflow(const char* path)
{
    tVideoStream* v;
    short pcm[64];
    int frames = 0;
    int got;

    v = VID_pst_OpenFile(path, 1);
    if (!v || !VID_b_HasSound(v))
    {
        ok(0, "the overflow fixture opens with sound");
        VID_Close(v);
        return;
    }
    while (VID_b_DecodeFrame(v) && frames < 200)
    {
        frames++;
        VID_NextFrame(v);
    }
    ok(frames > 5, "a full queue does not stall the picture");
    got = VID_i_SoundRead(v, pcm, (int)sizeof(pcm));
    okEq(got, (int)sizeof(pcm), "a full queue still reads out");
    okEq(got % 4, 0, "and still in whole stereo frames");
    VID_Close(v);
}

/* The clamp in VID_PcmWrite -- what stops a write running over sound
   that is still queued when the sink has stalled -- is NOT covered
   here, and an attempt at it is worth recording so it is not made
   again. A partially drained reader does make the clamp fire on
   nearly every call, but the queue then drops the tail of each
   chunk it cannot fit, so the sound that comes out is discontinuous
   by design: a check for steps the ramp cannot make reports four of
   them on correct code. It is measuring the drop policy, not the
   clamp, exactly as a first attempt at the wrap check did. A test
   that can tell the two apart needs the queue to carry where each
   byte came from, which is more machinery than the forty lines it
   would be guarding. See docs/video.md. */

/* A file that really has an alpha plane, so withAlpha is told apart
   from a file that is opaque whatever is asked for. */
static void alphaPlane(const char* path)
{
    tVideoStream* v;
    uint8_t* pix;

    v = VID_pst_OpenFile(path, 0);
    if (!v)
    {
        ok(0, "the alpha fixture opens");
        return;
    }
    pix = (uint8_t*)malloc((size_t)VT_W * VT_H * 4);
    ok(VID_b_DecodeFrame(v), "the alpha fixture decodes");

    memset(pix, 0x5A, (size_t)VT_W * VT_H * 4);
    VID_CopyToBuffer(v, pix, VT_W * 4, VT_H, 1);
    okNear(pix[3], 0x80, 8, "with alpha the file's own plane comes through");

    memset(pix, 0x5A, (size_t)VT_W * VT_H * 4);
    VID_CopyToBuffer(v, pix, VT_W * 4, VT_H, 0);
    okEq(pix[3], 0xFF, "without alpha the same file is forced opaque");

    /* the colour is still right in both cases */
    okNear(pix[0], 0xC0, 12, "the alpha fixture is still blue-first");

    free(pix);
    VID_Close(v);
}

static void silentFile(const char* path)
{
    tVideoStream* v;
    short pcm[64];

    v = VID_pst_OpenFile(path, 1);
    ok(v != NULL, "a silent file opens with sound asked for");
    if (!v)
        return;
    okEq(VID_b_HasSound(v), 0, "a silent file reports no sound");
    okEq(VID_i_SoundRead(v, pcm, (int)sizeof(pcm)), 0, "a silent file queues nothing");
    ok(VID_b_DecodeFrame(v), "a silent file still decodes its picture");
    VID_Close(v);
}

/* The clock: the first frame is always due, and once a sink reports
   progress it is the sound that decides, not the wall. */
static void clock(const char* path)
{
    tVideoStream* v;

    v = VID_pst_OpenFile(path, 1);
    if (!v)
    {
        ok(0, "the clock fixture opens");
        return;
    }
    okEq(VID_b_Waiting(v), 0, "the first frame is due at once");
    ok(VID_b_DecodeFrame(v), "the first frame decodes");
    VID_NextFrame(v);

    /* nothing has been played, so the second frame is not due yet */
    VID_SoundPlayed(v, 0);
    okEq(VID_b_Waiting(v), 1, "with no sound played the next frame waits");

    /* an hour of sound has been played, so every frame is overdue */
    VID_SoundPlayed(v, (int64_t)VID_i_SoundRate(v) * 3600);
    okEq(VID_b_Waiting(v), 0, "once the sound has run on the frame is due");

    /* and it goes back to waiting when the sound is behind again */
    VID_SoundPlayed(v, 0);
    okEq(VID_b_Waiting(v), 1, "the clock follows the sink both ways");

    /* VID_b_NextAlsoDue answers the same question one frame further
       on, without having to release the frame in hand to ask it */
    ok(VID_b_DecodeFrame(v), "a frame is in hand for the look-ahead");
    VID_SoundPlayed(v, 0);
    okEq(VID_b_NextAlsoDue(v), 0, "with the sound behind, the one after is not due");
    VID_SoundPlayed(v, (int64_t)VID_i_SoundRate(v) * 3600);
    okEq(VID_b_NextAlsoDue(v), 1, "with the sound run on, the one after is due too");
    okEq(VID_b_NextAlsoDue(NULL), 0, "and nothing is never due");
    VID_Close(v);
}

int main(int argc, char** argv)
{
    const char* clip;
    const char* silent;
    const char* alpha;

    if (argc < 4)
    {
        printf("usage: %s <clip-with-sound> <silent-clip> <alpha-clip>\n", argv[0]);
        return 2;
    }
    clip = argv[1];
    silent = argv[2];
    alpha = argv[3];

    printf("=== video unit test ===\n");
    openAndShape(silent);
    bothPathsAgree(silent);
    colourAndAlpha(silent);
    pitchAndClipping(silent);
    endOfStream(silent);
    brokenInput(silent);
    soundTrack(clip);
    soundRingWrap(clip);
    soundOverflow(clip);
    silentFile(silent);
    alphaPlane(alpha);
    clock(clip);

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
