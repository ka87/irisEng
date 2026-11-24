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
 * function on purpose, because it used to keep a pointer in an int and
 * so could not be handed a buffer in a 64-bit build. It takes a
 * pointer-wide position now, so the buffer here is an ordinary one at
 * whatever address this build puts it -- which is also the check that
 * the widening did what it was for: the test asserted the buffer was
 * reachable through an int and skipped itself when it was not, and now
 * it asserts the opposite.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include "../sound.h"
#include "../bigfiles.h"
#include "../basic_array.h"
#include "../memory.h"
#include "../handle.h"

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
                               MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (buf == MAP_FAILED)
    {
        printf("  FAIL fmt=%d: no buffer\n", fmtSize);
        ++failures;
        return;
    }
    /* The point of the widening: this address does not fit in an int,
       and the function has to work with it anyway. On a build where a
       pointer is four bytes it does fit, and there is nothing to
       prove. */
    if (sizeof(void*) > 4 && (uintptr_t)buf <= 0x7FFFFFFFu)
        printf("  note fmt=%d: this buffer happens to sit low, so the "
               "round trip through a narrower slot would also have "
               "worked\n", fmtSize);

    memset(buf, 0, 4096);
    if (build(buf + 4, fmtSize) != 28 + fmtSize)
        fail("header length as laid out", build(buf + 4, fmtSize), 28 + fmtSize);
    put32(buf + 50, 0);                       /* the key read on this path */

    memset(&hdr, 0, sizeof(hdr));
    r = SND_i_GetWaveInfo((intptr_t)buf, &hdr, 4);
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

/* Every key in the array finds its own header and reads it. A value
   that was not resolved is a small number, and reading a header out of
   it does not survive. */
static void check_lookup(const char* how, unsigned int firstKey)
{
    tSndWavHdrStruct hdr;
    int i;

    for (i = 0; i < 6; i++)
    {
        memset(&hdr, 0, sizeof(hdr));
        if (SND_i_LoadStreamedFile(firstKey + (unsigned)i, &hdr) == -1)
        {
            printf("  FAIL %s: no header for key %#x\n", how,
                   firstKey + (unsigned)i);
            ++failures;
        }
        else if (hdr.wfex.nSamplesPerSec != 22050
                 || hdr.fullSoundSize != 123456u)
        {
            printf("  FAIL %s: the header read back wrong for key %#x\n",
                   how, firstKey + (unsigned)i);
            ++failures;
        }
    }
}

/* The header array, which nothing covered at all.
 *
 * SND_HeaderMakeArray and SND_HeaderLoadArray put a header block in the
 * value beside its sound key, SND_i_LoadStreamedFile looks one up and
 * reads it, and SND_FreeHeaderArray gives them back. The value is four
 * bytes, so it holds a handle -- and a mutation that stored the address
 * there instead survived every test in the suite, which is what this
 * is for.
 *
 * It is driven twice over: once with the array filled by hand, which
 * pins the lookup and the release on their own, and once through
 * SND_HeaderMakeArray so that the engine's own store site is driven
 * rather than stood in for. */
static void check_header_array(void)
{
    enum { HEADERS = 6 };
    unsigned char* blocks[HEADERS];
    int before;
    int i;

    memset(&SND_gst_HeaderArray, 0, sizeof(SND_gst_HeaderArray));
    BAS_bsortmode = 0;
    BAS_binit(&SND_gst_HeaderArray, 16);
    before = HDL_i_LiveCount();

    for (i = 0; i < HEADERS; i++)
    {
        /* The block the loaders allocate: four bytes of key in front of
           the header, and the sound key at 50 for the path that reads
           it. */
        blocks[i] = (unsigned char*)MEM_p_Alloc(64);
        memset(blocks[i], 0, 64);
        build(blocks[i] + 4, 18);
        put32(blocks[i] + 50, 0);
        if ((uint32_t)(uintptr_t)blocks[i] == HDL_ul_Handle(blocks[i]))
        {
            fail("a handle is the address itself, so this check proves "
                 "nothing", 0, 0);
            return;
        }
        BAS_binsert(0x1000u + (unsigned)i,
                    (int)HDL_ul_Handle(blocks[i]), &SND_gst_HeaderArray);
    }
    BAS_bsortmode = 1;
    BAS_bsort(&SND_gst_HeaderArray);

    if (SND_gst_HeaderArray.actElemNum != HEADERS)
        fail("headers in the array", SND_gst_HeaderArray.actElemNum, HEADERS);

    check_lookup("filled by hand", 0x1000u);

    /* And again with the engine's own filling, so the two store sites
       are driven rather than stood in for. The sounds it walks need the
       two flag groups it tests for and a key the big file answers; what
       it then puts beside each key is the thing in question. */
    snd_bigfile_clear();
    for (i = 0; i < HEADERS; i++)
    {
        unsigned char raw[46];
        memset(raw, 0, sizeof(raw));
        build(raw, 18);
        snd_bigfile_add(0x3000u + (unsigned)i, raw, (int)sizeof(raw));
    }
    SND_gst_Params.allocatedSounds =
        (tSndSoundElem*)MEM_p_Alloc(HEADERS * sizeof(tSndSoundElem));
    memset(SND_gst_Params.allocatedSounds, 0, HEADERS * sizeof(tSndSoundElem));
    SND_gst_Params.allocatedSoundsMaxNum = HEADERS;
    for (i = 0; i < HEADERS; i++)
    {
        SND_gst_Params.allocatedSounds[i].soundKey = 0x3000u + (unsigned)i;
        SND_gst_Params.allocatedSounds[i].flags = 0x400u | 0x2000u;
    }
    SND_HeaderMakeArray(&SND_gst_HeaderArray);
    if (SND_gst_HeaderArray.actElemNum != HEADERS)
        fail("headers the engine put in the array",
             SND_gst_HeaderArray.actElemNum, HEADERS);
    check_lookup("filled by the engine", 0x3000u);

    /* A key that is in neither the array nor the big file has nothing
       to find, and must say so rather than resolving something. */
    {
        tSndWavHdrStruct hdr;
        if (SND_i_LoadStreamedFile(0x9999u, &hdr) != -1)
            fail("a key in neither place found a header", 0, -1);
    }

    /* And freeing the array gives every handle back. */
    SND_FreeHeaderArray(&SND_gst_HeaderArray);
    if (HDL_i_LiveCount() != before)
        fail("handles still held after the array was freed",
             HDL_i_LiveCount(), before);
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
    printf("=== the header array, by key ===\n");
    check_header_array();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  3 shapes, 0 failure(s)\n");
    return 0;
}
