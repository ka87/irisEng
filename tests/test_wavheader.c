/* Reading a sound file's header, for each of the three shapes.
 *
 * Which shape applies is decided by the format chunk's size, and the
 * data tag that follows sits at a different place in each -- two bytes
 * later once a bit depth is present, two later again once an extension
 * size is. Reading it at the wrong one does not produce a wrong
 * answer: the tag fails to match and the whole file is rejected, so a
 * sound simply never loads.
 *
 * Nothing else covers this. The differential harness skips the
 * function on purpose, because it keeps a pointer in an int and so
 * cannot be handed a stack buffer in a 64-bit build. The one entry
 * that takes the caller's own pointer is used here instead, with the
 * buffer placed low enough to survive the round trip.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include "../sound.h"
#include "../bigfiles.h"

static int failures;

static void fail(const char* what, int got, int want)
{
    printf("  FAIL %s: got %d, wanted %d\n", what, got, want);
    ++failures;
}

static void put32(unsigned char* p, unsigned int v)
{
    p[0]=(unsigned char)v; p[1]=(unsigned char)(v>>8);
    p[2]=(unsigned char)(v>>16); p[3]=(unsigned char)(v>>24);
}
static void put16(unsigned char* p, unsigned int v)
{
    p[0]=(unsigned char)v; p[1]=(unsigned char)(v>>8);
}

/* Lays out a header of the shape the given format-chunk size selects,
   and returns how long it is. */
static int build(unsigned char* h, int fmtSize)
{
    unsigned char* p = h;
    memcpy(p, "RIFF", 4);           p += 4;
    put32(p, 0);                    p += 4;   /* riff chunk size */
    memcpy(p, "WAVE", 4);           p += 4;
    memcpy(p, "fmt ", 4);           p += 4;
    put32(p, (unsigned)fmtSize);    p += 4;
    put16(p, 2);                    p += 2;   /* format tag */
    put16(p, 2);                    p += 2;   /* channels */
    put32(p, 22050);                p += 4;   /* samples per second */
    put32(p, 11025);                p += 4;   /* average bytes per second */
    put16(p, 72);                   p += 2;   /* block align */
    if (fmtSize >= 16) { put16(p, 4); p += 2; }   /* bits per sample */
    if (fmtSize >= 18) { put16(p, 0); p += 2; }   /* extension size */
    memcpy(p, "data", 4);           p += 4;
    put32(p, 123456);               p += 4;   /* data chunk size */
    return (int)(p - h);
}

static void check_shape(int fmtSize, int wantLen, int wantBits)
{
    unsigned char* buf;
    tSndWavHdrStruct hdr;
    int r;

    buf = (unsigned char*)mmap(NULL, 4096, PROT_READ|PROT_WRITE,
                               MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT, -1, 0);
    if (buf == MAP_FAILED || (uintptr_t)buf > 0x7fffffffu)
    {
        printf("  SKIP fmt=%d: no buffer reachable through an int\n", fmtSize);
        return;
    }
    memset(buf, 0, 4096);
    if (build(buf + 4, fmtSize) != 28 + fmtSize)
        fail("header length as laid out", build(buf + 4, fmtSize), 28 + fmtSize);
    put32(buf + 50, 0);                       /* the key read on this path */

    memset(&hdr, 0, sizeof(hdr));
    r = SND_i_GetWaveInfo((int)(uintptr_t)buf, &hdr, 4);
    /* the result counts from where the sound sits in the file */
    wantLen += (int)BIG_ul_SearchKeyToPos(0) + 4;

    if (r == -1)
        fail("fmt chunk accepted", -1, wantLen);
    else if (r != wantLen)
        fail("header length reported", r, wantLen);

    if (r != -1)
    {
        if (hdr.wfex.nChannels != 2)        fail("channels", hdr.wfex.nChannels, 2);
        if (hdr.wfex.nSamplesPerSec != 22050) fail("sample rate", (int)hdr.wfex.nSamplesPerSec, 22050);
        if (hdr.wfex.nBlockAlign != 72)     fail("block align", hdr.wfex.nBlockAlign, 72);
        if (hdr.fullSoundSize != 123456)    fail("data size", hdr.fullSoundSize, 123456);
        if (hdr.wfex.wBitsPerSample != wantBits)
            fail("bits per sample", hdr.wfex.wBitsPerSample, wantBits);
    }
    munmap(buf, 4096);
}

int main(void)
{
    printf("=== sound file header, all three shapes ===\n");

    /* 4 is what the header carries; the 14-byte shape has no room for
       it and the engine derives a value of its own, which is kept as
       it is however odd it looks. */
    check_shape(14, 42, 8 * (11025 / 72 / 2));
    check_shape(16, 44, 4);
    check_shape(18, 46, 4);

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  3 shapes, 0 failure(s)\n");
    return 0;
}
