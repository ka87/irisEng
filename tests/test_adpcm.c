/* Differential test for win/adpcm.c.
 *
 * The frozen pre-rewrite copy (tests/baseline_adpcm.c, BASE_) and the
 * rewrite (win/adpcm.c, NEW_) are linked into one binary and run over
 * identical inputs. adpcm.c calls nothing outside itself, so there is
 * no engine-call sequence to compare: the whole observable result is
 * the decoded samples, the codec state the call leaves behind, the
 * return value, and whether anything was written outside the output
 * buffer. All four are compared, byte for byte, with no tolerance.
 *
 * Two of the three dimensions of pcINO_ApplyADPCMAaptation are swept
 * exhaustively -- all 16 nibbles against all 7 coefficient pairs the
 * engine's own table holds. The three state words cannot be: they are
 * 2^48 combinations. They get a boundary grid and a random sweep.
 *
 * The block and stream entry points are swept exhaustively over every
 * channel count and every block length the format can express up to
 * 80 samples, which includes the one geometry the engine actually
 * uses (mono 36 bytes / stereo 72 bytes, 60 samples per block).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../win/adpcm.h"

int16_t BASE_pcINO_ApplyADPCMAaptation(uint8_t nibble, int16_t* coeffs, int16_t* state);
int16_t NEW_pcINO_ApplyADPCMAaptation(uint8_t nibble, int16_t* coeffs, int16_t* state);
int BASE_pcSND_ADPCMToPCM_ConvertOneBlock(int16_t* dest, tADPCMParamStruct* params, uint8_t* src);
int NEW_pcSND_ADPCMToPCM_ConvertOneBlock(int16_t* dest, tADPCMParamStruct* params, uint8_t* src);
int BASE_pcSND_ADPCMToPCM_Convert(uint8_t* sndData, int16_t* destPtr, unsigned int totalSamples,
                                  uint16_t channels, uint16_t blockBytes, uint16_t samplesPerBlock);
int NEW_pcSND_ADPCMToPCM_Convert(uint8_t* sndData, int16_t* destPtr, unsigned int totalSamples,
                                 uint16_t channels, uint16_t blockBytes, uint16_t samplesPerBlock);

static long cases;
static long diffs;
static const char* group = "?";

/* Coverage counters. A sweep whose guards never take both values is a
   sweep that proves less than its case count suggests, so every one
   derived from the generator is counted and printed. */
static long covTableCoeffs, covWildCoeffs;
static long covBlockSkipped, covStreamSkipped;
static long covMonoChan, covStereoChan;
static long covBodyRan, covBodyEmpty;
static long covOddOverrun;
static long covTail, covNoTail;
static long covNibbleHigh, covNibbleLow;
static long covClampLow, covClampHigh, covDeltaFloor;

static void fail(const char* what, long key, long a, long b)
{
    if (diffs < 20)
        printf("  DIFF %-28s %-22s key=%ld base=%ld new=%ld\n", group, what, key, a, b);
    diffs++;
}

/* ---- the engine's own coefficient table, as pcSND_ADPCMToPCM_Convert
   writes it into the parameter block. Seven pairs; the predictor byte
   in a block header selects one of them. */
static const int16_t coeffTable[14] = {
    256, 0, 512, -256, 0, 0, 192, 64, 240, 0, 460, -208, 392, -232
};

static uint32_t rngState;
static uint32_t rnd(void)
{
    rngState = rngState * 1664525u + 1013904223u;
    return rngState >> 8;
}

/* ------------------------------------------------------------------ */
/* 1. pcINO_ApplyADPCMAaptation                                        */
/* ------------------------------------------------------------------ */

static void applyCase(int nibble, const int16_t* coeffs, int delta, int s1, int s2, long key)
{
    int16_t baseState[4];
    int16_t newState[4];
    int16_t baseCoeffs[2];
    int16_t newCoeffs[2];
    int16_t baseRet, newRet;
    int i;

    baseState[0] = 0;
    baseState[1] = (int16_t)delta;
    baseState[2] = (int16_t)s1;
    baseState[3] = (int16_t)s2;
    memcpy(newState, baseState, sizeof(baseState));
    baseCoeffs[0] = coeffs[0];
    baseCoeffs[1] = coeffs[1];
    newCoeffs[0] = coeffs[0];
    newCoeffs[1] = coeffs[1];

    baseRet = BASE_pcINO_ApplyADPCMAaptation((uint8_t)nibble, baseCoeffs, baseState);
    newRet = NEW_pcINO_ApplyADPCMAaptation((uint8_t)nibble, newCoeffs, newState);

    if (nibble & 8)
        covNibbleHigh++;
    else
        covNibbleLow++;
    if (baseRet == (int16_t)0x8000)
        covClampLow++;
    if (baseRet == 0x7FFF)
        covClampHigh++;
    if (baseState[1] == 16)
        covDeltaFloor++;

    cases++;
    if (baseRet != newRet)
        fail("return", key, baseRet, newRet);
    for (i = 0; i < 4; i++)
        if (baseState[i] != newState[i])
            fail("state word", key * 4 + i, baseState[i], newState[i]);
    /* the coefficient pair is an input and must come back untouched */
    if (baseCoeffs[0] != newCoeffs[0] || baseCoeffs[1] != newCoeffs[1])
        fail("coeffs clobbered", key, baseCoeffs[0], newCoeffs[0]);
}

static void testApply(void)
{
    static const int deltas[] = { 0, 1, 2, 15, 16, 17, 100, 255, 256, 257,
                                  1000, 4096, 16384, 32767, -32768, -25536, -1 };
    static const int samples[] = { -32768, -32767, -16384, -256, -128, -1, 0,
                                   1, 127, 255, 16384, 32766, 32767 };
    int n, c, d, i, j;
    long key = 0;

    group = "apply/grid";
    for (n = 0; n < 16; n++)
        for (c = 0; c < 7; c++)
            for (d = 0; d < (int)(sizeof(deltas) / sizeof(deltas[0])); d++)
                for (i = 0; i < (int)(sizeof(samples) / sizeof(samples[0])); i++)
                    for (j = 0; j < (int)(sizeof(samples) / sizeof(samples[0])); j++)
                        applyCase(n, &coeffTable[2 * c], deltas[d],
                                  samples[i], samples[j], key++);

    group = "apply/random";
    rngState = 0x12345678u;
    for (i = 0; i < 2000000; i++)
    {
        int16_t coeffs[2];
        int useTable = (rnd() & 3) != 0;
        if (useTable)
        {
            covTableCoeffs++;
            int idx = (int)(rnd() % 7u);
            coeffs[0] = coeffTable[2 * idx];
            coeffs[1] = coeffTable[2 * idx + 1];
        }
        else
        {
            covWildCoeffs++;
            coeffs[0] = (int16_t)rnd();
            coeffs[1] = (int16_t)rnd();
        }
        applyCase((int)(rnd() & 15u), coeffs, (int)(int16_t)rnd(),
                  (int)(int16_t)rnd(), (int)(int16_t)rnd(), key++);
    }
}

/* ------------------------------------------------------------------ */
/* 2. pcSND_ADPCMToPCM_ConvertOneBlock                                 */
/* ------------------------------------------------------------------ */

#define GUARD 64
#define OUTCAP 4096

/* The block writer emits 2*channels header samples plus 2*pairs body
   samples, where pairs = ceil(channels * (samples - 2) / 2). For a
   mono block with an odd sample count that is one sample more than
   channels*samples; the buffer and the comparison allow for it. */
static int blockOutWords(int channels, int blocksize)
{
    int pairs = (channels * (blocksize - 2) + 1) / 2;
    if (pairs < 0)
        pairs = 0;
    return 2 * channels + 2 * pairs;
}

static void blockCase(int channels, int blocksize, int predictorMode, long key)
{
    static int16_t baseOut[GUARD + OUTCAP + GUARD];
    static int16_t newOut[GUARD + OUTCAP + GUARD];
    uint8_t src[OUTCAP];
    tADPCMParamStruct baseParams, newParams;
    int baseRet, newRet;
    int words = blockOutWords(channels, blocksize);
    int i;

    if (words > OUTCAP)
    {
        covBlockSkipped++;
        return;
    }
    if (words > 2 * channels)
        covBodyRan++;
    else
        covBodyEmpty++;
    if (words > channels * blocksize)
        covOddOverrun++;

    for (i = 0; i < OUTCAP; i++)
        src[i] = (uint8_t)rnd();
    /* the predictor byte selects a coefficient pair; index 0..6 is
       what a well-formed block carries, and the loader does not check
       it, so the out-of-range case is driven on purpose too */
    for (i = 0; i < channels; i++)
    {
        if (predictorMode == 0)
            src[i] = (uint8_t)(rnd() % 7u);
        else if (predictorMode == 1)
            src[i] = 0;
        else
            src[i] = 6;
    }

    memset(baseOut, 0x5A, sizeof(baseOut));
    memset(newOut, 0x5A, sizeof(newOut));

    memset(&baseParams, 0, sizeof(baseParams));
    baseParams.channelNum = channels;
    baseParams.blocksize = blocksize;
    memcpy(baseParams.adaptationCoeffs, coeffTable, sizeof(coeffTable));
    newParams = baseParams;

    baseRet = BASE_pcSND_ADPCMToPCM_ConvertOneBlock(baseOut + GUARD, &baseParams, src);
    newRet = NEW_pcSND_ADPCMToPCM_ConvertOneBlock(newOut + GUARD, &newParams, src);

    cases++;
    if (baseRet != newRet)
        fail("return", key, baseRet, newRet);
    if (memcmp(&baseParams, &newParams, sizeof(baseParams)) != 0)
        fail("params mutated", key, 0, 0);
    for (i = 0; i < GUARD + OUTCAP + GUARD; i++)
        if (baseOut[i] != newOut[i])
        {
            fail("out word", key * 10000 + i, baseOut[i], newOut[i]);
            break;
        }
    /* guard bands: the two copies agreeing on a wild write is not the
       same as neither making one */
    for (i = 0; i < GUARD; i++)
        if (newOut[i] != (int16_t)0x5A5A)
            fail("wrote before dest", key, 0, newOut[i]);
    for (i = GUARD + words; i < GUARD + OUTCAP + GUARD; i++)
        if (newOut[i] != (int16_t)0x5A5A)
        {
            fail("wrote past dest", key * 10000 + (i - GUARD), words, i - GUARD);
            break;
        }
}

static void testBlock(void)
{
    int channels, blocksize, rep, mode;
    long key = 0;

    group = "block";
    rngState = 0xC0FFEEu;
    for (channels = 1; channels <= 2; channels++)
        for (blocksize = 0; blocksize <= 80; blocksize++)
            for (mode = 0; mode < 3; mode++)
                for (rep = 0; rep < 8; rep++)
                    blockCase(channels, blocksize, mode, key++);
}

/* ------------------------------------------------------------------ */
/* 3. pcSND_ADPCMToPCM_Convert                                         */
/* ------------------------------------------------------------------ */

#define STREAMCAP 65536

static void streamCase(unsigned totalSamples, int channels, int blockBytes,
                       int samplesPerBlock, long key)
{
    static uint8_t src[STREAMCAP];
    static int16_t baseOut[GUARD + STREAMCAP + GUARD];
    static int16_t newOut[GUARD + STREAMCAP + GUARD];
    int baseRet, newRet;
    unsigned i;
    unsigned blocks = totalSamples / (unsigned)samplesPerBlock;

    if ((blocks + 1) * (unsigned)blockBytes >= STREAMCAP
        || (totalSamples + (unsigned)samplesPerBlock) * (unsigned)channels + 8 >= STREAMCAP)
    {
        covStreamSkipped++;
        return;
    }
    if (channels == 1)
        covMonoChan++;
    else
        covStereoChan++;
    if (totalSamples % (unsigned)samplesPerBlock)
        covTail++;
    else
        covNoTail++;

    for (i = 0; i < STREAMCAP; i++)
        src[i] = (uint8_t)rnd();
    /* keep every block's predictor byte in range */
    for (i = 0; i + (unsigned)blockBytes <= STREAMCAP; i += (unsigned)blockBytes)
    {
        src[i] = (uint8_t)(rnd() % 7u);
        if (channels == 2)
            src[i + 1] = (uint8_t)(rnd() % 7u);
    }

    memset(baseOut, 0x5A, sizeof(baseOut));
    memset(newOut, 0x5A, sizeof(newOut));

    baseRet = BASE_pcSND_ADPCMToPCM_Convert(src, baseOut + GUARD, totalSamples,
                                            (uint16_t)channels, (uint16_t)blockBytes,
                                            (uint16_t)samplesPerBlock);
    newRet = NEW_pcSND_ADPCMToPCM_Convert(src, newOut + GUARD, totalSamples,
                                          (uint16_t)channels, (uint16_t)blockBytes,
                                          (uint16_t)samplesPerBlock);

    cases++;
    if (baseRet != newRet)
        fail("return", key, baseRet, newRet);
    for (i = 0; i < GUARD + STREAMCAP + GUARD; i++)
        if (baseOut[i] != newOut[i])
        {
            fail("out word", key * 100000 + (long)i, baseOut[i], newOut[i]);
            break;
        }
    for (i = 0; i < GUARD; i++)
        if (newOut[i] != (int16_t)0x5A5A)
            fail("wrote before dest", key, 0, newOut[i]);
}

static void testStream(void)
{
    int blocks;
    long key = 0;

    group = "stream/shipped";
    rngState = 0xBEEF01u;
    /* the one geometry win/winsound.c builds: 60 samples a block,
       36 bytes mono and 72 stereo, length always a whole number of
       blocks */
    for (blocks = 0; blocks <= 60; blocks++)
    {
        streamCase(60u * (unsigned)blocks, 1, 36, 60, key++);
        streamCase(60u * (unsigned)blocks, 2, 72, 60, key++);
    }

    group = "stream/tail";
    /* a length that is not a whole number of blocks, which the shipped
       caller cannot produce but the function handles */
    for (blocks = 0; blocks < 400; blocks++)
    {
        unsigned total = (unsigned)(rnd() % 600u);
        int channels = 1 + (int)(rnd() & 1u);
        streamCase(total, channels, channels == 1 ? 36 : 72, 60, key++);
    }

    group = "stream/geometry";
    for (blocks = 0; blocks < 600; blocks++)
    {
        int channels = 1 + (int)(rnd() & 1u);
        int spb = 2 + (int)(rnd() % 40u);
        int bytes = channels * (7 - channels) + ((channels * (spb - 2) + 1) / 2) + (int)(rnd() % 4u);
        unsigned total = (unsigned)(rnd() % 400u);
        if (bytes <= 0)
            continue;
        streamCase(total, channels, bytes, spb, key++);
    }
}

int main(void)
{
    printf("=== adpcm differential test ===\n");

    testApply();
    printf("  after apply:  %ld cases\n", cases);
    testBlock();
    printf("  after block:  %ld cases\n", cases);
    testStream();
    printf("  after stream: %ld cases\n", cases);

    printf("coverage: nibble high/low %ld/%ld  coeffs table/wild %ld/%ld\n",
           covNibbleHigh, covNibbleLow, covTableCoeffs, covWildCoeffs);
    printf("coverage: returned -32768 %ld, returned 32767 %ld, delta hit the floor %ld\n",
           covClampLow, covClampHigh, covDeltaFloor);
    printf("coverage: block body ran/empty %ld/%ld, skipped %ld, "
           "wrote one past channels*samples %ld\n",
           covBodyRan, covBodyEmpty, covBlockSkipped, covOddOverrun);
    printf("coverage: stream mono/stereo %ld/%ld, tail/no tail %ld/%ld, skipped %ld\n",
           covMonoChan, covStereoChan, covTail, covNoTail, covStreamSkipped);

    if (!covNibbleHigh || !covNibbleLow || !covTableCoeffs || !covWildCoeffs
        || !covClampLow || !covClampHigh || !covDeltaFloor
        || !covBodyRan || !covBodyEmpty || !covOddOverrun
        || !covMonoChan || !covStereoChan || !covTail || !covNoTail)
    {
        printf("FAIL: a guard the sweep depends on never took both values\n");
        diffs++;
    }

    printf("%ld cases, %ld differences\n", cases, diffs);
    if (diffs)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
