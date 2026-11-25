/* Where a stream starts when it is played from an offset.
 *
 * The start position has to be rounded down to a whole compressed
 * block, or the seek that follows lands mid-block and the decoder
 * reads rubbish. The block is 36 bytes per channel: a property of the
 * format, which is what makes it worth pinning here. It used to be
 * taken from sizeof a struct that happened to match it in one build
 * and in no other.
 *
 * Nothing else exercises this path -- the differential never plays
 * from a non-zero offset -- so these are direct assertions.
 *
 * Only the arithmetic runs: the streaming counter is parked above its
 * limit so the function computes the start position and returns
 * before touching a voice.
 */

#include <stdio.h>
#include "../platform/sndbackend.h"

/* platform/sndbackend.c has no header. */
void pcSND_StreamReinitAndPlay(tSndSoundBufferElem* sbe, int len,
                               int startPos, int from, int to, char flags,
                               int freq, int pan, int att);
unsigned int pcSND_calcMemoryNeededForTheSound(unsigned int fullsoundlen,
                                               int channels);

static int failures;

/* What the function should work out, written out the long way so that
   it does not share a line of arithmetic with the code it checks. */
static unsigned int expected_start(unsigned int from, unsigned int ch)
{
    unsigned int frames = (from >> 1) / ch;
    unsigned int readSize = (ch == 1 ? 36u : 72u) * (frames / 60u);
    unsigned int block = 36u * ch;
    unsigned int aligned = block * (readSize / block);
    return ch == 1 ? 120u * (aligned / 36u) : 120u * ch * (aligned / 72u);
}

static void check(unsigned int from, unsigned int ch, int len)
{
    tSndSoundBufferElem sbe;
    tSbeMiscData misc;
    unsigned int want, got;
    unsigned int full;

    memset(&sbe, 0, sizeof(sbe));
    memset(&misc, 0, sizeof(misc));
    sbe.miscData = &misc;
    sbe.format = (int)ch;

    /* above the limit, so the voice half of the function is skipped */
    gDoSoundGlobal.dword_B303C0 = 64;

    pcSND_StreamReinitAndPlay(&sbe, len, 0, (int)from, 0, 0, 22050, 0, 0);

    want = expected_start(from, ch);
    full = (unsigned int)pcSND_calcMemoryNeededForTheSound((unsigned int)len, (int)ch);
    if (want > full)
        want = 0;                      /* the function's own clamp */
    got = (unsigned int)misc.startDecodedOffset;
    if (got != want)
    {
        printf("  FAIL from=%u ch=%u: start=%u, wanted %u\n", from, ch, got, want);
        ++failures;
    }
}

int main(void)
{
    printf("=== stream start position, played from an offset ===\n");

    check(0, 1, 1 << 20);
    check(0, 2, 1 << 20);
    check(4321, 1, 1 << 20);
    check(4321, 2, 1 << 20);
    check(65536, 1, 1 << 20);
    check(65536, 2, 1 << 20);
    check(1000003, 1, 1 << 22);
    check(1000003, 2, 1 << 22);
    /* past the end of the sound: the function must fall back to zero */
    check(1u << 30, 1, 1 << 16);

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  9 checks, 0 failure(s)\n");
    return 0;
}
