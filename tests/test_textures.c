#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
#include "../gdi.h"
#include "../engine.h"
#include "../time.h"
#include "textures_mocks.h"

/* textures.c, old against new, in one binary over identical state.

   What is driven here is everything in the file whose arithmetic or
   control flow was rewritten and that can be stood up without the
   display device: the sixteen scale kernels, the pixel-format and size
   converters, the TGA run-length decoder, the palette loader, the
   compression planner, the animated-texture update and the texture
   list.

   Each case fills two identical arenas, runs one copy over each, and
   then compares the whole arena byte for byte -- so a stray write
   anywhere, not just in the output, is caught -- along with the
   allocator traffic and the texture memory counter. */

extern void BASE_TEX_Compress24_Xo2(char*, char*, int, int);
extern void NEW_TEX_Compress24_Xo2(char*, char*, int, int);
extern void BASE_TEX_Compress24_Yo2(char*, char*, int, unsigned int);
extern void NEW_TEX_Compress24_Yo2(char*, char*, int, unsigned int);
extern void BASE_TEX_Compress_Xo2(char*, char*, int, int);
extern void NEW_TEX_Compress_Xo2(char*, char*, int, int);
extern void BASE_TEX_Compress_Yo2(char*, char*, int, unsigned int);
extern void NEW_TEX_Compress_Yo2(char*, char*, int, unsigned int);
extern void BASE_TEX_Compress8_Xo2(char*, char*, int, int);
extern void NEW_TEX_Compress8_Xo2(char*, char*, int, int);
extern void BASE_TEX_Compress8_Yo2(char*, char*, int, unsigned int);
extern void NEW_TEX_Compress8_Yo2(char*, char*, int, unsigned int);
extern void BASE_TEX_Compress4_Xo2(char*, char*, int, int);
extern void NEW_TEX_Compress4_Xo2(char*, char*, int, int);
extern void BASE_TEX_Compress4_Yo2(char*, char*, unsigned int, unsigned int);
extern void NEW_TEX_Compress4_Yo2(char*, char*, unsigned int, unsigned int);
extern void BASE_TEX_Expand_XBy2(int*, int*, int, int);
extern void NEW_TEX_Expand_XBy2(int*, int*, int, int);
extern void BASE_TEX_Expand_YBy2(char*, char*, int, int);
extern void NEW_TEX_Expand_YBy2(char*, char*, int, int);
extern void BASE_TEX_Expand8_XBy2(char*, char*, int, int);
extern void NEW_TEX_Expand8_XBy2(char*, char*, int, int);
extern void BASE_TEX_Expand8_YBy2(char*, char*, int, int);
extern void NEW_TEX_Expand8_YBy2(char*, char*, int, int);
extern void BASE_TEX_Expand4_XBy2(char*, char*, int, int);
extern void NEW_TEX_Expand4_XBy2(char*, char*, int, int);
extern void BASE_TEX_Expand4_YBy2(char*, char*, unsigned int, int);
extern void NEW_TEX_Expand4_YBy2(char*, char*, unsigned int, int);
extern void BASE_TEX_Expand24_XBy2(char*, char*, int, int);
extern void NEW_TEX_Expand24_XBy2(char*, char*, int, int);
extern void BASE_TEX_Expand24_YBy2(char*, char*, int, int);
extern void NEW_TEX_Expand24_YBy2(char*, char*, int, int);

extern void BASE_TEX_Convert_32SwapRB(int*, int);
extern void NEW_TEX_Convert_32SwapRB(int*, int);
extern void BASE_TEX_Convert_24To32(char*, int);
extern void NEW_TEX_Convert_24To32(char*, int);
extern void BASE_TEX_Convert_24To32SwapRB(char*, int);
extern void NEW_TEX_Convert_24To32SwapRB(char*, int);
extern void BASE_TEX_Convert_4To32(int*, unsigned char*, int*, int);
extern void NEW_TEX_Convert_4To32(int*, unsigned char*, int*, int);
extern void BASE_TEX_Convert_8To32(int*, unsigned char*, int*, int);
extern void NEW_TEX_Convert_8To32(int*, unsigned char*, int*, int);
extern void BASE_TEX_Convert_4To32SwapRB(int*, unsigned char*, int*, int);
extern void NEW_TEX_Convert_4To32SwapRB(int*, unsigned char*, int*, int);
extern void BASE_TEX_Convert_8To32SwapRB(int*, unsigned char*, int*, int);
extern void NEW_TEX_Convert_8To32SwapRB(int*, unsigned char*, int*, int);

extern void BASE_TEX_File_VSwap(char*, char*, int, int, int);
extern void NEW_TEX_File_VSwap(char*, char*, int, int, int);
extern void BASE_TEX_l_File_TgaUncompress(char, char*, char*, int);
extern void NEW_TEX_l_File_TgaUncompress(char, char*, char*, int);

extern unsigned int BASE_TEX_ul_GetPower2Up(unsigned int);
extern unsigned int NEW_TEX_ul_GetPower2Up(unsigned int);
extern char BASE_TEX_uc_GetMaskShift(unsigned int);
extern char NEW_TEX_uc_GetMaskShift(unsigned int);
extern void BASE_TEX_GetMaskParams(uint32_t, uint8_t*, uint8_t*);
extern void NEW_TEX_GetMaskParams(uint32_t, uint8_t*, uint8_t*);
extern void BASE_TEX_GetColorConvertParams(int, unsigned int, int*, unsigned int*);
extern void NEW_TEX_GetColorConvertParams(int, unsigned int, int*, unsigned int*);

extern int BASE_TEX_l_ConvertPF(tEngTexIntermediateStruct*, tEngTexIntermediateStruct*);
extern int NEW_TEX_l_ConvertPF(tEngTexIntermediateStruct*, tEngTexIntermediateStruct*);
extern int BASE_TEX_l_ConvertSize(tEngTexIntermediateStruct*, tEngTexIntermediateStruct*);
extern int NEW_TEX_l_ConvertSize(tEngTexIntermediateStruct*, tEngTexIntermediateStruct*);
extern void BASE_TEX_l_ConvertSize_24To32(tEngTexIntermediateStruct*, tEngTexIntermediateStruct*);
extern void NEW_TEX_l_ConvertSize_24To32(tEngTexIntermediateStruct*, tEngTexIntermediateStruct*);
extern int BASE_TEX_l_ConvertSizeFast(tEngTexIntermediateStruct*);
extern int NEW_TEX_l_ConvertSizeFast(tEngTexIntermediateStruct*);

extern void BASE_TEX_File_DevelopParams(tEngTexIntermediateStruct*);
extern void NEW_TEX_File_DevelopParams(tEngTexIntermediateStruct*);
extern void BASE_TEX_File_TransformParams(tEngTexIntermediateStructTexParams*);
extern void NEW_TEX_File_TransformParams(tEngTexIntermediateStructTexParams*);
extern int BASE_TEX_l_MemoryNeeded(tEngTexIntermediateStruct*);
extern int NEW_TEX_l_MemoryNeeded(tEngTexIntermediateStruct*);

extern void BASE_TEX_File_LoadPalette(tEngPaletteElem*, int);
extern void NEW_TEX_File_LoadPalette(tEngPaletteElem*, int);
extern void BASE_TEX_Manager_Init(struct sTexManagerStruct*);
extern void NEW_TEX_Manager_Init(struct sTexManagerStruct*);
extern void BASE_TEX_Manager_Reinit(struct sTexManagerStruct*);
extern void NEW_TEX_Manager_Reinit(struct sTexManagerStruct*);
extern void BASE_TEX_Manager_ComputeCompression(struct sTexManagerStruct*, tEngTexIntermediateStruct*, int, char);
extern void NEW_TEX_Manager_ComputeCompression(struct sTexManagerStruct*, tEngTexIntermediateStruct*, int, char);

extern int BASE_TEX_l_File_LoadTga(char*, tEngTexIntermediateStruct*);
extern int NEW_TEX_l_File_LoadTga(char*, tEngTexIntermediateStruct*);
extern int BASE_TEX_l_File_LoadBmp(tTexWeirdBMPHeader*, tEngTexIntermediateStruct*);
extern int NEW_TEX_l_File_LoadBmp(tTexWeirdBMPHeader*, tEngTexIntermediateStruct*);
extern int BASE_TEX_l_File_LoadRaw(char*, tEngTexIntermediateStruct*);
extern int NEW_TEX_l_File_LoadRaw(char*, tEngTexIntermediateStruct*);
extern int BASE_TEX_l_File_LoadRawPalette(char*, tEngTexIntermediateStruct*, unsigned int);
extern int NEW_TEX_l_File_LoadRawPalette(char*, tEngTexIntermediateStruct*, unsigned int);
extern int BASE_TEX_l_File_LoadAnimated(char*, tEngTexIntermediateStruct*, int);
extern int NEW_TEX_l_File_LoadAnimated(char*, tEngTexIntermediateStruct*, int);

extern int16_t BASE_TEX_w_List_AddTexture(tEngTexListStruct*, int);
extern int16_t NEW_TEX_w_List_AddTexture(tEngTexListStruct*, int);
extern int16_t BASE_TEX_w_List_AddPalette(tEngTexListStruct*, int);
extern int16_t NEW_TEX_w_List_AddPalette(tEngTexListStruct*, int);
extern int16_t BASE_TEX_w_Manager_ChooseGoodInTex(struct sTexManagerStruct*, tEngTexIntermediateStruct*);
extern int16_t NEW_TEX_w_Manager_ChooseGoodInTex(struct sTexManagerStruct*, tEngTexIntermediateStruct*);
extern tEngTexListStruct BASE_TEX_gst_GlobalList;
extern tEngTexListStruct NEW_TEX_gst_GlobalList;

extern void BASE_TEX_Anim_Update(void);
extern void NEW_TEX_Anim_Update(void);
extern struct sTexAnimatedElem* BASE_TEX_pst_Anim_Add(int16_t, char, int16_t, struct sTexAnimatedInnerElem*);
extern struct sTexAnimatedElem* NEW_TEX_pst_Anim_Add(int16_t, char, int16_t, struct sTexAnimatedInnerElem*);
extern tTexAnimatedElem* BASE_TEX_gdst_Animated;
extern tTexAnimatedElem* NEW_TEX_gdst_Animated;
extern int BASE_TEX_gi_NbAnimated;
extern int NEW_TEX_gi_NbAnimated;
extern int BASE_TEX_gi_NbAnimatedMax;
extern int NEW_TEX_gi_NbAnimatedMax;
extern float BASE_sf_TimeLeft_218;
extern float NEW_sf_TimeLeft_218;

/* ---------------------------------------------------------------- */

#define ARENA 32768
#define SRC_AT 4096
#define DST_AT 12288

static unsigned char arenaA[ARENA];
static unsigned char arenaB[ARENA];

static int g_cases;
static int g_fails;
static char g_case[160];

static unsigned s_rng;

static void rngSeed(unsigned seed)
{
    s_rng = seed ? seed : 1u;
}

static unsigned char rngByte(void)
{
    s_rng = s_rng * 1664525u + 1013904223u;
    return (unsigned char)(s_rng >> 16);
}

static void fillArenas(unsigned seed)
{
    int i;

    rngSeed(seed);
    for (i = 0; i < ARENA; i++)
    {
        arenaA[i] = rngByte();
        arenaB[i] = arenaA[i];
    }
}

static void fail(const char* what)
{
    printf("    DIFF %s | %s\n", g_case, what);
    g_fails++;
}

static void compareArenas(void)
{
    int i;
    int shown;

    shown = 0;
    for (i = 0; i < ARENA; i++)
    {
        if (arenaA[i] != arenaB[i])
        {
            if (shown < 4)
                printf("    DIFF %s | arena[%d] (0x%x): old %02x new %02x\n",
                       g_case, i, i, arenaA[i], arenaB[i]);
            shown++;
        }
    }
    if (shown)
    {
        printf("    DIFF %s | %d arena bytes differ\n", g_case, shown);
        g_fails++;
    }
}

static void compareBytes(const void* a, const void* b, int len, const char* what)
{
    const unsigned char* pa = (const unsigned char*)a;
    const unsigned char* pb = (const unsigned char*)b;
    int i;
    int shown = 0;

    for (i = 0; i < len; i++)
    {
        if (pa[i] != pb[i])
        {
            if (shown < 4)
                printf("    DIFF %s | %s[%d]: old %02x new %02x\n", g_case, what, i, pa[i], pb[i]);
            shown++;
        }
    }
    if (shown)
    {
        printf("    DIFF %s | %s: %d of %d bytes differ\n", g_case, what, shown, len);
        g_fails++;
    }
}

static void compareInt(int a, int b, const char* what)
{
    if (a != b)
    {
        printf("    DIFF %s | %s: old %d new %d\n", g_case, what, a, b);
        g_fails++;
    }
}

/* The allocator traffic of the side that ran first, so the second
   side's can be compared against it. */
static tMockTexCall s_log[MOCKT_LOG_MAX];
static int s_logLen;
static unsigned int s_used;

static void snapshotCalls(void)
{
    memcpy(s_log, MOCKT_log, sizeof(tMockTexCall) * MOCKT_logLen);
    s_logLen = MOCKT_logLen;
    s_used = MEM_gst_MemoryInfo.usedMemoryByTextures;
    if (MOCKT_overflow)
        fail("the call log overflowed");
}

static void compareCalls(void)
{
    int i;
    int n;

    if (MOCKT_overflow)
        fail("the call log overflowed");
    if (s_logLen != MOCKT_logLen)
    {
        printf("    DIFF %s | call count: old %d new %d\n", g_case, s_logLen, MOCKT_logLen);
        g_fails++;
    }
    n = s_logLen < MOCKT_logLen ? s_logLen : MOCKT_logLen;
    for (i = 0; i < n; i++)
    {
        if (strcmp(s_log[i].fn, MOCKT_log[i].fn) != 0 || s_log[i].a != MOCKT_log[i].a
            || s_log[i].b != MOCKT_log[i].b)
        {
            printf("    DIFF %s | call %d: old %s(%d,%d) new %s(%d,%d)\n", g_case, i,
                   s_log[i].fn, s_log[i].a, s_log[i].b,
                   MOCKT_log[i].fn, MOCKT_log[i].a, MOCKT_log[i].b);
            g_fails++;
            return;
        }
    }
    compareInt((int)s_used, (int)MEM_gst_MemoryInfo.usedMemoryByTextures, "usedMemoryByTextures");
}

/* ---------------------------------------------------------------- 1
   The sixteen scale kernels. */

typedef void (*tScaleFn)(char*, char*, int, int);

typedef struct sScaleCase
{
    const char* name;
    tScaleFn base;
    tScaleFn live;
} tScaleCase;

static const tScaleCase s_scale[] = {
    { "Compress24_Xo2", (tScaleFn)BASE_TEX_Compress24_Xo2, (tScaleFn)NEW_TEX_Compress24_Xo2 },
    { "Compress24_Yo2", (tScaleFn)BASE_TEX_Compress24_Yo2, (tScaleFn)NEW_TEX_Compress24_Yo2 },
    { "Compress_Xo2",   (tScaleFn)BASE_TEX_Compress_Xo2,   (tScaleFn)NEW_TEX_Compress_Xo2 },
    { "Compress_Yo2",   (tScaleFn)BASE_TEX_Compress_Yo2,   (tScaleFn)NEW_TEX_Compress_Yo2 },
    { "Compress8_Xo2",  (tScaleFn)BASE_TEX_Compress8_Xo2,  (tScaleFn)NEW_TEX_Compress8_Xo2 },
    { "Compress8_Yo2",  (tScaleFn)BASE_TEX_Compress8_Yo2,  (tScaleFn)NEW_TEX_Compress8_Yo2 },
    { "Compress4_Xo2",  (tScaleFn)BASE_TEX_Compress4_Xo2,  (tScaleFn)NEW_TEX_Compress4_Xo2 },
    { "Compress4_Yo2",  (tScaleFn)BASE_TEX_Compress4_Yo2,  (tScaleFn)NEW_TEX_Compress4_Yo2 },
    { "Expand_XBy2",    (tScaleFn)BASE_TEX_Expand_XBy2,    (tScaleFn)NEW_TEX_Expand_XBy2 },
    { "Expand_YBy2",    (tScaleFn)BASE_TEX_Expand_YBy2,    (tScaleFn)NEW_TEX_Expand_YBy2 },
    { "Expand8_XBy2",   (tScaleFn)BASE_TEX_Expand8_XBy2,   (tScaleFn)NEW_TEX_Expand8_XBy2 },
    { "Expand8_YBy2",   (tScaleFn)BASE_TEX_Expand8_YBy2,   (tScaleFn)NEW_TEX_Expand8_YBy2 },
    { "Expand4_XBy2",   (tScaleFn)BASE_TEX_Expand4_XBy2,   (tScaleFn)NEW_TEX_Expand4_XBy2 },
    { "Expand4_YBy2",   (tScaleFn)BASE_TEX_Expand4_YBy2,   (tScaleFn)NEW_TEX_Expand4_YBy2 },
    { "Expand24_XBy2",  (tScaleFn)BASE_TEX_Expand24_XBy2,  (tScaleFn)NEW_TEX_Expand24_XBy2 },
    { "Expand24_YBy2",  (tScaleFn)BASE_TEX_Expand24_YBy2,  (tScaleFn)NEW_TEX_Expand24_YBy2 },
};

static const int s_dims[][2] = {
    { 2, 2 }, { 4, 4 }, { 8, 8 }, { 16, 16 }, { 32, 8 }, { 8, 32 },
    { 16, 2 }, { 2, 16 }, { 6, 4 }, { 4, 6 }, { 10, 10 }
};

static void caseScale(void)
{
    int f;
    int d;
    int inPlace;
    int w;
    int h;
    char* srcA;
    char* dstA;
    char* srcB;
    char* dstB;

    for (f = 0; f < (int)(sizeof(s_scale) / sizeof(s_scale[0])); f++)
    {
        for (d = 0; d < (int)(sizeof(s_dims) / sizeof(s_dims[0])); d++)
        {
            for (inPlace = 0; inPlace < 2; inPlace++)
            {
                w = s_dims[d][0];
                h = s_dims[d][1];
                sprintf(g_case, "%s %dx%d%s", s_scale[f].name, w, h,
                        inPlace ? " in place" : "");
                fillArenas(0x1234u + 31u * f + 7u * d + inPlace);
                srcA = (char*)arenaA + SRC_AT;
                srcB = (char*)arenaB + SRC_AT;
                dstA = inPlace ? srcA : (char*)arenaA + DST_AT;
                dstB = inPlace ? srcB : (char*)arenaB + DST_AT;
                s_scale[f].base(srcA, dstA, w, h);
                s_scale[f].live(srcB, dstB, w, h);
                compareArenas();
                g_cases++;
            }
        }
    }
}

/* ---------------------------------------------------------------- 2
   The pixel converters. */

static void caseConverters(void)
{
    int d;
    int w;
    int h;
    int n;
    int* palA;
    int* palB;

    for (d = 0; d < (int)(sizeof(s_dims) / sizeof(s_dims[0])); d++)
    {
        w = s_dims[d][0];
        h = s_dims[d][1];
        n = w * h;

        sprintf(g_case, "Convert_32SwapRB %d", n);
        fillArenas(0x5100u + d);
        BASE_TEX_Convert_32SwapRB((int*)(arenaA + SRC_AT), n);
        NEW_TEX_Convert_32SwapRB((int*)(arenaB + SRC_AT), n);
        compareArenas();
        g_cases++;

        sprintf(g_case, "Convert_24To32 %d", n);
        fillArenas(0x5200u + d);
        BASE_TEX_Convert_24To32((char*)(arenaA + SRC_AT), n);
        NEW_TEX_Convert_24To32((char*)(arenaB + SRC_AT), n);
        compareArenas();
        g_cases++;

        sprintf(g_case, "Convert_24To32SwapRB %d", n);
        fillArenas(0x5300u + d);
        BASE_TEX_Convert_24To32SwapRB((char*)(arenaA + SRC_AT), n);
        NEW_TEX_Convert_24To32SwapRB((char*)(arenaB + SRC_AT), n);
        compareArenas();
        g_cases++;

        palA = (int*)(arenaA + 1024);
        palB = (int*)(arenaB + 1024);

        sprintf(g_case, "Convert_4To32 %d", n);
        fillArenas(0x5400u + d);
        BASE_TEX_Convert_4To32((int*)(arenaA + DST_AT), arenaA + SRC_AT, palA, n);
        NEW_TEX_Convert_4To32((int*)(arenaB + DST_AT), arenaB + SRC_AT, palB, n);
        compareArenas();
        g_cases++;

        sprintf(g_case, "Convert_8To32 %d", n);
        fillArenas(0x5500u + d);
        BASE_TEX_Convert_8To32((int*)(arenaA + DST_AT), arenaA + SRC_AT, palA, n);
        NEW_TEX_Convert_8To32((int*)(arenaB + DST_AT), arenaB + SRC_AT, palB, n);
        compareArenas();
        g_cases++;

        sprintf(g_case, "Convert_4To32SwapRB %d", n);
        fillArenas(0x5600u + d);
        BASE_TEX_Convert_4To32SwapRB((int*)(arenaA + DST_AT), arenaA + SRC_AT, palA, n);
        NEW_TEX_Convert_4To32SwapRB((int*)(arenaB + DST_AT), arenaB + SRC_AT, palB, n);
        compareArenas();
        g_cases++;

        sprintf(g_case, "Convert_8To32SwapRB %d", n);
        fillArenas(0x5700u + d);
        BASE_TEX_Convert_8To32SwapRB((int*)(arenaA + DST_AT), arenaA + SRC_AT, palA, n);
        NEW_TEX_Convert_8To32SwapRB((int*)(arenaB + DST_AT), arenaB + SRC_AT, palB, n);
        compareArenas();
        g_cases++;

        sprintf(g_case, "File_VSwap %dx%d bpp3", w, h);
        fillArenas(0x5800u + d);
        BASE_TEX_File_VSwap((char*)arenaA + DST_AT, (char*)arenaA + SRC_AT, w, h, 3);
        NEW_TEX_File_VSwap((char*)arenaB + DST_AT, (char*)arenaB + SRC_AT, w, h, 3);
        compareArenas();
        g_cases++;
    }

    /* The null palette guard. */
    sprintf(g_case, "Convert_8To32 null palette");
    fillArenas(0x5900u);
    BASE_TEX_Convert_8To32((int*)(arenaA + DST_AT), arenaA + SRC_AT, NULL, 16);
    NEW_TEX_Convert_8To32((int*)(arenaB + DST_AT), arenaB + SRC_AT, NULL, 16);
    compareArenas();
    g_cases++;
}

/* ---------------------------------------------------------------- 3
   The mask helpers. */

static void caseMasks(void)
{
    static const unsigned int masks[] = {
        0, 1, 0xFF, 0xFF00, 0xFF0000, 0xFF000000u, 0x7C00, 0x3E0, 0x1F,
        0xF000, 0xF00, 0xF0, 0xF, 0x8000, 0xFFFF, 0xFFFFFFFFu, 0x5555
    };
    int i;
    int j;
    unsigned int n;
    uint8_t bitsA;
    uint8_t shiftA;
    uint8_t bitsB;
    uint8_t shiftB;
    int sA;
    int sB;
    unsigned int mA;
    unsigned int mB;

    for (i = 0; i < (int)(sizeof(masks) / sizeof(masks[0])); i++)
    {
        sprintf(g_case, "GetMaskShift 0x%x", masks[i]);
        compareInt(BASE_TEX_uc_GetMaskShift(masks[i]), NEW_TEX_uc_GetMaskShift(masks[i]), "shift");
        g_cases++;

        sprintf(g_case, "GetMaskParams 0x%x", masks[i]);
        bitsA = 0xAA; shiftA = 0xAA; bitsB = 0xAA; shiftB = 0xAA;
        BASE_TEX_GetMaskParams(masks[i], &bitsA, &shiftA);
        NEW_TEX_GetMaskParams(masks[i], &bitsB, &shiftB);
        compareInt(bitsA, bitsB, "bits");
        compareInt(shiftA, shiftB, "shift");
        g_cases++;

        for (j = 0; j < (int)(sizeof(masks) / sizeof(masks[0])); j++)
        {
            sprintf(g_case, "GetColorConvertParams 0x%x 0x%x", masks[i], masks[j]);
            sA = 0x5A5A5A; mA = 0x5A5A5A; sB = 0x5A5A5A; mB = 0x5A5A5A;
            BASE_TEX_GetColorConvertParams(masks[i], masks[j], &sA, &mA);
            NEW_TEX_GetColorConvertParams(masks[i], masks[j], &sB, &mB);
            compareInt(sA, sB, "shift");
            compareInt((int)mA, (int)mB, "mask");
            g_cases++;
        }
    }

    for (n = 0; n < 1100; n++)
    {
        sprintf(g_case, "GetPower2Up %u", n);
        compareInt((int)BASE_TEX_ul_GetPower2Up(n), (int)NEW_TEX_ul_GetPower2Up(n), "value");
        g_cases++;
    }
}

/* ---------------------------------------------------------------- 4
   The TGA run-length decoder. */

static void caseTga(void)
{
    static const int depths[] = { 24, 32 };
    int d;
    int k;
    int len;
    int i;
    unsigned char* streamA;
    unsigned char* streamB;

    for (d = 0; d < 2; d++)
    {
        for (k = 0; k < 6; k++)
        {
            sprintf(g_case, "TgaUncompress depth %d stream %d", depths[d], k);
            fillArenas(0x7000u + 16u * d + k);
            streamA = arenaA + SRC_AT;
            streamB = arenaB + SRC_AT;
            /* Packets whose decoded length is bounded: alternate a
               literal run and a repeat run of a few pixels each. */
            for (i = 0; i < 256; i += 8)
            {
                streamA[i] = (unsigned char)((i & 8) ? 0x80 + (k & 3) : (k & 3));
                streamB[i] = streamA[i];
            }
            len = 32 + 16 * k;
            BASE_TEX_l_File_TgaUncompress((char)depths[d], (char*)arenaA + DST_AT,
                                          (char*)streamA, len);
            NEW_TEX_l_File_TgaUncompress((char)depths[d], (char*)arenaB + DST_AT,
                                         (char*)streamB, len);
            compareArenas();
            g_cases++;
        }
    }
}

/* ---------------------------------------------------------------- 5
   The pixel format and size converters. */

static void setTex(tEngTexIntermediateStruct* tex, int w, int h, int depth,
                   unsigned int r, unsigned int g, unsigned int b, unsigned int a,
                   char* data)
{
    memset(tex, 0, sizeof(*tex));
    tex->realWidth = w;
    tex->realHeight = h;
    tex->realWidthResizeToThis = w;
    tex->realHeightResizeToThis = h;
    tex->colorDepth1 = depth;
    tex->colorDepth2 = depth;
    tex->redMask = r;
    tex->greenMask = g;
    tex->blueMask = b;
    tex->alphaMask = a;
    tex->imgDataPtr = data;
}

/* TEX_GetColorConvertParams writes neither of its outputs when the
   source channel is narrower than the destination one, so the caller
   reads uninitialized locals and the two copies cannot be compared.
   Those format pairs are skipped and counted. */
static int convertPFIsDefined(const unsigned int* sf, const unsigned int* df)
{
    int c;
    uint8_t srcBits;
    uint8_t srcShift;
    uint8_t destBits;
    uint8_t destShift;

    for (c = 1; c <= 4; c++)
    {
        if (!sf[c] || !df[c])
            continue;
        NEW_TEX_GetMaskParams(sf[c], &srcBits, &srcShift);
        NEW_TEX_GetMaskParams(df[c], &destBits, &destShift);
        if (srcBits < destBits)
            return 0;
    }
    return 1;
}

static int g_pfSkipped;

static void caseConvertPF(void)
{
    static const unsigned int fmt[][5] = {
        /* depth, r, g, b, a */
        { 32, 0xFF0000, 0xFF00, 0xFF, 0xFF000000u },
        { 32, 0xFF, 0xFF00, 0xFF0000, 0 },
        { 16, 0x7C00, 0x3E0, 0x1F, 0x8000 },
        { 16, 0xF00, 0xF0, 0xF, 0xF000 },
        { 24, 0xFF0000, 0xFF00, 0xFF, 0 }
    };
    int i;
    int j;
    int d;
    tEngTexIntermediateStruct srcA;
    tEngTexIntermediateStruct dstA;
    tEngTexIntermediateStruct srcB;
    tEngTexIntermediateStruct dstB;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (!convertPFIsDefined(fmt[i], fmt[j]))
            {
                g_pfSkipped++;
                continue;
            }
            for (d = 0; d < 4; d++)
            {
                sprintf(g_case, "ConvertPF fmt %d to %d %dx%d", i, j,
                        s_dims[d][0], s_dims[d][1]);
                fillArenas(0x9000u + 97u * i + 13u * j + d);
                setTex(&srcA, s_dims[d][0], s_dims[d][1], fmt[i][0], fmt[i][1],
                       fmt[i][2], fmt[i][3], fmt[i][4], (char*)arenaA + SRC_AT);
                setTex(&dstA, s_dims[d][0], s_dims[d][1], fmt[j][0], fmt[j][1],
                       fmt[j][2], fmt[j][3], fmt[j][4], (char*)arenaA + DST_AT);
                setTex(&srcB, s_dims[d][0], s_dims[d][1], fmt[i][0], fmt[i][1],
                       fmt[i][2], fmt[i][3], fmt[i][4], (char*)arenaB + SRC_AT);
                setTex(&dstB, s_dims[d][0], s_dims[d][1], fmt[j][0], fmt[j][1],
                       fmt[j][2], fmt[j][3], fmt[j][4], (char*)arenaB + DST_AT);
                compareInt(BASE_TEX_l_ConvertPF(&srcA, &dstA),
                           NEW_TEX_l_ConvertPF(&srcB, &dstB), "return");
                compareArenas();
                compareBytes(&srcA, &srcB, (int)((char*)&srcA.imgDataPtr - (char*)&srcA),
                             "src struct");
                g_cases++;
            }
        }
    }
}

static void caseConvertSize(void)
{
    static const int sizes[][4] = {
        /* srcW, srcH, dstW, dstH */
        { 8, 8, 4, 4 }, { 4, 4, 8, 8 }, { 16, 8, 4, 8 }, { 8, 16, 8, 4 },
        { 10, 6, 5, 3 }, { 5, 3, 10, 6 }, { 16, 16, 16, 16 }, { 12, 4, 7, 9 }
    };
    static const unsigned int fmt[][5] = {
        { 32, 0xFF0000, 0xFF00, 0xFF, 0xFF000000u },
        { 32, 0xFF, 0xFF00, 0xFF0000, 0 },
        { 16, 0x7C00, 0x3E0, 0x1F, 0x8000 }
    };
    int s;
    int i;
    int j;
    tEngTexIntermediateStruct srcA;
    tEngTexIntermediateStruct dstA;
    tEngTexIntermediateStruct srcB;
    tEngTexIntermediateStruct dstB;

    for (s = 0; s < 8; s++)
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                sprintf(g_case, "ConvertSize %dx%d to %dx%d fmt %d to %d",
                        sizes[s][0], sizes[s][1], sizes[s][2], sizes[s][3], i, j);
                fillArenas(0xA000u + 211u * s + 17u * i + j);
                setTex(&srcA, sizes[s][0], sizes[s][1], fmt[i][0], fmt[i][1],
                       fmt[i][2], fmt[i][3], fmt[i][4], (char*)arenaA + SRC_AT);
                setTex(&dstA, sizes[s][2], sizes[s][3], fmt[j][0], fmt[j][1],
                       fmt[j][2], fmt[j][3], fmt[j][4], (char*)arenaA + DST_AT);
                setTex(&srcB, sizes[s][0], sizes[s][1], fmt[i][0], fmt[i][1],
                       fmt[i][2], fmt[i][3], fmt[i][4], (char*)arenaB + SRC_AT);
                setTex(&dstB, sizes[s][2], sizes[s][3], fmt[j][0], fmt[j][1],
                       fmt[j][2], fmt[j][3], fmt[j][4], (char*)arenaB + DST_AT);
                compareInt(BASE_TEX_l_ConvertSize(&srcA, &dstA),
                           NEW_TEX_l_ConvertSize(&srcB, &dstB), "return");
                compareArenas();
                g_cases++;
            }
        }

        sprintf(g_case, "ConvertSize_24To32 %dx%d to %dx%d",
                sizes[s][0], sizes[s][1], sizes[s][2], sizes[s][3]);
        fillArenas(0xB000u + s);
        setTex(&srcA, sizes[s][0], sizes[s][1], 24, 0xFF0000, 0xFF00, 0xFF, 0,
               (char*)arenaA + SRC_AT);
        setTex(&dstA, sizes[s][2], sizes[s][3], 32, 0xFF0000, 0xFF00, 0xFF,
               0xFF000000u, (char*)arenaA + DST_AT);
        setTex(&srcB, sizes[s][0], sizes[s][1], 24, 0xFF0000, 0xFF00, 0xFF, 0,
               (char*)arenaB + SRC_AT);
        setTex(&dstB, sizes[s][2], sizes[s][3], 32, 0xFF0000, 0xFF00, 0xFF,
               0xFF000000u, (char*)arenaB + DST_AT);
        BASE_TEX_l_ConvertSize_24To32(&srcA, &dstA);
        NEW_TEX_l_ConvertSize_24To32(&srcB, &dstB);
        compareArenas();
        g_cases++;
    }
}

/* ---------------------------------------------------------------- 6
   The power-of-two scaler, which drives all sixteen kernels through
   the loops and the temporary buffer. */

static void caseConvertSizeFast(void)
{
    static const int sizes[][4] = {
        { 8, 8, 4, 4 }, { 4, 4, 8, 8 }, { 16, 4, 4, 16 }, { 4, 16, 16, 4 },
        { 8, 8, 8, 8 }, { 32, 32, 8, 8 }, { 2, 2, 16, 16 }, { 16, 16, 2, 2 },
        { 6, 8, 4, 4 }, { 8, 8, 4, 3 }
    };
    static const int depths[] = { 4, 8, 24, 32, 16 };
    int s;
    int d;
    int bytes;
    int retA;
    int retB;
    tEngTexIntermediateStruct texA;
    tEngTexIntermediateStruct texB;
    char* dataA;
    char* dataB;
    int i;

    for (s = 0; s < 10; s++)
    {
        for (d = 0; d < 5; d++)
        {
            sprintf(g_case, "ConvertSizeFast %dx%d to %dx%d depth %d",
                    sizes[s][0], sizes[s][1], sizes[s][2], sizes[s][3], depths[d]);
            bytes = sizes[s][0] * sizes[s][1];
            if (sizes[s][2] * sizes[s][3] > bytes)
                bytes = sizes[s][2] * sizes[s][3];
            bytes = bytes * 8 + 256;

            MOCKT_Reset();
            dataA = (char*)MEM_p_AllocAligned(bytes, 64);
            rngSeed(0xC000u + 31u * s + d);
            for (i = 0; i < bytes; i++)
                dataA[i] = (char)rngByte();
            setTex(&texA, sizes[s][0], sizes[s][1], depths[d], 0xFF0000, 0xFF00,
                   0xFF, 0xFF000000u, dataA);
            texA.realWidthResizeToThis = sizes[s][2];
            texA.realHeightResizeToThis = sizes[s][3];
            MOCKT_Reset();
            retA = BASE_TEX_l_ConvertSizeFast(&texA);
            snapshotCalls();

            MOCKT_Reset();
            dataB = (char*)MEM_p_AllocAligned(bytes, 64);
            rngSeed(0xC000u + 31u * s + d);
            for (i = 0; i < bytes; i++)
                dataB[i] = (char)rngByte();
            setTex(&texB, sizes[s][0], sizes[s][1], depths[d], 0xFF0000, 0xFF00,
                   0xFF, 0xFF000000u, dataB);
            texB.realWidthResizeToThis = sizes[s][2];
            texB.realHeightResizeToThis = sizes[s][3];
            MOCKT_Reset();
            retB = NEW_TEX_l_ConvertSizeFast(&texB);
            compareCalls();

            compareInt(retA, retB, "return");
            compareInt(texA.realWidth, texB.realWidth, "realWidth");
            compareInt(texA.realHeight, texB.realHeight, "realHeight");
            compareInt(*((uint32_t*)texA.imgDataPtr - 1), *((uint32_t*)texB.imgDataPtr - 1),
                       "block size");
            compareBytes(texA.imgDataPtr, texB.imgDataPtr,
                         MOCKT_BlockSpan(texA.imgDataPtr), "image");
            g_cases++;
        }
    }
}

/* ---------------------------------------------------------------- 7
   The palette loader. */

static void casePalette(void)
{
    static const int sizes[] = { 1024, 768, 64, 48, 256 };
    static unsigned char stream[2048];
    int s;
    int swap;
    int i;
    tEngPaletteElem palA;
    tEngPaletteElem palB;
    unsigned int blockA;

    for (s = 0; s < 5; s++)
    {
        for (swap = 0; swap < 2; swap++)
        {
            sprintf(g_case, "File_LoadPalette %d bytes swap %d", sizes[s], swap);
            rngSeed(0xD000u + 17u * s + swap);
            for (i = 0; i < 2048; i++)
                stream[i] = rngByte();
            MOCKT_SetStream(stream, sizes[s]);

            memset(&palA, 0, sizeof(palA));
            memset(&palB, 0, sizeof(palB));
            palA.key = 0x1234;
            palB.key = 0x1234;

            MOCKT_Reset();
            BASE_TEX_File_LoadPalette(&palA, swap);
            snapshotCalls();
            MOCKT_Reset();
            NEW_TEX_File_LoadPalette(&palB, swap);
            compareCalls();

            compareInt(palA.flags, palB.flags, "flags");
            compareInt(palA.paletteData != NULL, palB.paletteData != NULL, "data");
            if (palA.paletteData && palB.paletteData)
            {
                blockA = *((uint32_t*)palA.paletteData - 1);
                compareInt((int)blockA, (int)*((uint32_t*)palB.paletteData - 1), "block size");
                compareBytes(palA.paletteData, palB.paletteData,
                             MOCKT_BlockSpan(palA.paletteData), "palette");
            }
            g_cases++;
        }
    }
    MOCKT_SetStream(NULL, 0);
}

/* ---------------------------------------------------------------- 8
   The compression planner. */

static void caseCompression(void)
{
    static const int flagSets[] = { 0x10d, 0x10c, 0x30d, 0x7ff, 0x20010d };
    int f;
    int budget;
    int i;
    int n;
    tTexManagerStruct mgrA;
    tTexManagerStruct mgrB;
    tEngTexIntermediateStruct timsA[8];
    tEngTexIntermediateStruct timsB[8];

    for (f = 0; f < 5; f++)
    {
        for (budget = 0; budget < 4; budget++)
        {
            for (n = 0; n <= 8; n += 4)
            {
                sprintf(g_case, "ComputeCompression flags 0x%x budget %d n %d",
                        flagSets[f], budget, n);
                BASE_TEX_Manager_Init(&mgrA);
                NEW_TEX_Manager_Init(&mgrB);
                mgrA.flags = flagSets[f];
                mgrB.flags = flagSets[f];
                BASE_TEX_Manager_Reinit(&mgrA);
                NEW_TEX_Manager_Reinit(&mgrB);
                mgrA.texMemoryNeededCompressedCase1Backup = 1 << (10 + 4 * budget);
                mgrB.texMemoryNeededCompressedCase1Backup = mgrA.texMemoryNeededCompressedCase1Backup;
                mgrA.texMemoryNeededCompressedCase2Backup = 1 << (10 + 4 * budget);
                mgrB.texMemoryNeededCompressedCase2Backup = mgrA.texMemoryNeededCompressedCase2Backup;

                rngSeed(0xE000u + 71u * f + 7u * budget + n);
                memset(timsA, 0, sizeof(timsA));
                for (i = 0; i < n; i++)
                {
                    timsA[i].realWidth = 1 << (2 + (rngByte() % 6));
                    timsA[i].realHeight = 1 << (2 + (rngByte() % 6));
                    timsA[i].colorDepth1 = (rngByte() & 1) ? 32 : 24;
                    timsA[i].colorDepth2 = timsA[i].colorDepth1;
                    timsA[i].flags = (rngByte() & 7) == 0 ? 0x10 : 0;
                    timsA[i].tp.flagsUnkAndProceduralUnkb1 = rngByte() & 0x20F;
                    timsA[i].tp.type = rngByte() & 7;
                }
                memcpy(timsB, timsA, sizeof(timsA));

                BASE_TEX_Manager_ComputeCompression(&mgrA, timsA, n, 1);
                NEW_TEX_Manager_ComputeCompression(&mgrB, timsB, n, 1);
                BASE_TEX_Manager_ComputeCompression(&mgrA, timsA, n, 0);
                NEW_TEX_Manager_ComputeCompression(&mgrB, timsB, n, 0);

                compareBytes(&mgrA, &mgrB, sizeof(mgrA), "manager");
                compareBytes(timsA, timsB, sizeof(timsA), "tims");
                for (i = 0; i < n; i++)
                    compareInt(BASE_TEX_l_MemoryNeeded(&timsA[i]),
                               NEW_TEX_l_MemoryNeeded(&timsB[i]), "memoryNeeded");
                g_cases++;
            }
        }
    }
}

/* ---------------------------------------------------------------- 9
   The file loaders that need only the stream reader. */

static void caseLoaders(void)
{
    static unsigned char stream[4096];
    static const int depths[] = { 8, 16, 24, 32, 12 };
    static const int types[] = { 1, 2, 9, 10 };
    int d;
    int t;
    int i;
    int flags;
    int retA;
    int retB;
    tEngTexIntermediateStruct texA;
    tEngTexIntermediateStruct texB;

    for (d = 0; d < 5; d++)
    {
        for (t = 0; t < 4; t++)
        {
            for (flags = 1; flags <= 3; flags++)
            {
                sprintf(g_case, "LoadTga depth %d type %d flags %d", depths[d], types[t], flags);
                rngSeed(0xF000u + 31u * d + 5u * t + flags);
                for (i = 0; i < 4096; i++)
                    stream[i] = rngByte();
                stream[2] = (unsigned char)types[t];
                stream[5] = 4;              /* colour map length */
                stream[6] = 0;
                stream[7] = 24;             /* colour map entry bits */
                stream[12] = 8;             /* width */
                stream[13] = 0;
                stream[14] = 4;             /* height */
                stream[15] = 0;
                stream[16] = (unsigned char)depths[d];

                MOCKT_Reset();
                memset(&texA, 0, sizeof(texA));
                texA.flags = flags;
                retA = BASE_TEX_l_File_LoadTga((char*)stream, &texA);
                snapshotCalls();
                MOCKT_Reset();
                memset(&texB, 0, sizeof(texB));
                texB.flags = flags;
                retB = NEW_TEX_l_File_LoadTga((char*)stream, &texB);
                compareCalls();
                compareInt(retA, retB, "return");

                compareBytes(&texA, &texB, (int)((char*)&texA.imgDataPtr - (char*)&texA),
                             "tex head");
                compareInt(texA.tgaDataStartPtr, texB.tgaDataStartPtr, "tgaDataStartPtr");
                compareInt(texA.tgaDataEndPtr, texB.tgaDataEndPtr, "tgaDataEndPtr");
                if (texA.imgDataPtr && texB.imgDataPtr)
                    compareBytes(texA.imgDataPtr, texB.imgDataPtr,
                                 MOCKT_BlockSpan(texA.imgDataPtr), "image");
                g_cases++;
            }
        }

        for (flags = 1; flags <= 3; flags++)
        {
            sprintf(g_case, "LoadBmp depth %d flags %d", depths[d], flags);
            rngSeed(0xF800u + 31u * d + flags);
            for (i = 0; i < 4096; i++)
                stream[i] = rngByte();
            ((tTexWeirdBMPHeader*)stream)->width = 8;
            ((tTexWeirdBMPHeader*)stream)->height = 4;
            ((tTexWeirdBMPHeader*)stream)->colorDepth = (uint8_t)depths[d];

            MOCKT_Reset();
            memset(&texA, 0, sizeof(texA));
            texA.flags = flags;
            retA = BASE_TEX_l_File_LoadBmp((tTexWeirdBMPHeader*)stream, &texA);
            snapshotCalls();
            MOCKT_Reset();
            memset(&texB, 0, sizeof(texB));
            texB.flags = flags;
            retB = NEW_TEX_l_File_LoadBmp((tTexWeirdBMPHeader*)stream, &texB);
            compareCalls();
            compareInt(retA, retB, "return");
            compareBytes(&texA, &texB, (int)((char*)&texA.imgDataPtr - (char*)&texA),
                         "tex head");
            if (texA.imgDataPtr && texB.imgDataPtr)
                compareBytes(texA.imgDataPtr, texB.imgDataPtr,
                             MOCKT_BlockSpan(texA.imgDataPtr), "image");
            g_cases++;
        }
    }

    for (d = 0; d < 5; d++)
    {
        sprintf(g_case, "LoadRaw depth %d", depths[d]);
        rngSeed(0x10000u + d);
        for (i = 0; i < 4096; i++)
            stream[i] = rngByte();
        MOCKT_Reset();
        setTex(&texA, 8, 4, depths[d], 0, 0, 0, 0, NULL);
        texA.flags = 2;
        BASE_TEX_l_File_LoadRaw((char*)stream, &texA);
        snapshotCalls();
        MOCKT_Reset();
        setTex(&texB, 8, 4, depths[d], 0, 0, 0, 0, NULL);
        texB.flags = 2;
        NEW_TEX_l_File_LoadRaw((char*)stream, &texB);
        compareCalls();
        if (texA.imgDataPtr && texB.imgDataPtr)
            compareBytes(texA.imgDataPtr, texB.imgDataPtr,
                         MOCKT_BlockSpan(texA.imgDataPtr), "image");
        g_cases++;
    }

    for (d = 32; d <= 84; d += 4)
    {
        sprintf(g_case, "LoadRawPalette size %d", d);
        rngSeed(0x11000u + d);
        for (i = 0; i < 4096; i++)
            stream[i] = rngByte();
        memset(&texA, 0, sizeof(texA));
        memset(&texB, 0, sizeof(texB));
        compareInt(BASE_TEX_l_File_LoadRawPalette((char*)stream, &texA, d),
                   NEW_TEX_l_File_LoadRawPalette((char*)stream, &texB, d), "return");
        compareBytes(&texA, &texB, sizeof(texA), "tex");
        g_cases++;
    }

    for (d = 0; d < 6; d++)
    {
        sprintf(g_case, "LoadAnimated size %d", d);
        rngSeed(0x12000u + d);
        for (i = 0; i < 4096; i++)
            stream[i] = rngByte();
        stream[6] = (unsigned char)d;       /* texture count */
        MOCKT_Reset();
        memset(&texA, 0, sizeof(texA));
        BASE_TEX_l_File_LoadAnimated((char*)stream, &texA, d ? 64 : 0);
        snapshotCalls();
        MOCKT_Reset();
        memset(&texB, 0, sizeof(texB));
        NEW_TEX_l_File_LoadAnimated((char*)stream, &texB, d ? 64 : 0);
        compareCalls();
        compareInt(texA.tp.animatedParams.num, texB.tp.animatedParams.num, "num");
        compareInt(texA.tp.animatedParams.flags, texB.tp.animatedParams.flags, "flags");
        if (texA.tp.animatedParams.aiePtr && texB.tp.animatedParams.aiePtr)
            compareBytes(texA.tp.animatedParams.aiePtr, texB.tp.animatedParams.aiePtr,
                         (int)(sizeof(tTexAnimatedInnerElem) * texA.tp.animatedParams.num),
                         "elems");
        g_cases++;
    }
}

/* ---------------------------------------------------------------- 10
   The texture list and the animated-texture clock. */

static void caseList(void)
{
    static const int keys[] = {
        0x1F0016B6, 0x62000872, 0x720006F4, 0x62000F6D, 0x00000010, 0x00000090,
        0x11112222, -1, 0x1F0016B6
    };
    int p;
    int i;
    int n;
    int16_t got[16];

    n = (int)(sizeof(keys) / sizeof(keys[0]));
    for (p = 1; p <= 3; p++)
    {
        sprintf(g_case, "List_AddTexture platform %d", p);
        memset(&BASE_TEX_gst_GlobalList, 0, sizeof(tEngTexListStruct));
        memset(&NEW_TEX_gst_GlobalList, 0, sizeof(tEngTexListStruct));
        ENG_PlatformID = p;

        MOCKT_Reset();
        for (i = 0; i < n; i++)
            got[i] = BASE_TEX_w_List_AddTexture(&BASE_TEX_gst_GlobalList, keys[i]);
        snapshotCalls();
        MOCKT_Reset();
        for (i = 0; i < n; i++)
            compareInt(got[i], NEW_TEX_w_List_AddTexture(&NEW_TEX_gst_GlobalList, keys[i]),
                       "index");
        compareCalls();
        compareInt(BASE_TEX_gst_GlobalList.actTexElemNum,
                   NEW_TEX_gst_GlobalList.actTexElemNum, "num");
        compareBytes(BASE_TEX_gst_GlobalList.texElems, NEW_TEX_gst_GlobalList.texElems,
                     (int)(sizeof(tEngTexListElemStruct) * BASE_TEX_gst_GlobalList.actTexElemNum),
                     "texElems");
        g_cases++;
    }

    sprintf(g_case, "List_AddPalette");
    memset(&BASE_TEX_gst_GlobalList, 0, sizeof(tEngTexListStruct));
    memset(&NEW_TEX_gst_GlobalList, 0, sizeof(tEngTexListStruct));
    for (i = 0; i < 300; i++)
        compareInt(BASE_TEX_w_List_AddPalette(&BASE_TEX_gst_GlobalList, i / 2),
                   NEW_TEX_w_List_AddPalette(&NEW_TEX_gst_GlobalList, i / 2), "index");
    compareInt(BASE_TEX_gst_GlobalList.paletteNum, NEW_TEX_gst_GlobalList.paletteNum, "num");
    compareInt(BASE_TEX_gst_GlobalList.unk1, NEW_TEX_gst_GlobalList.unk1, "capacity");
    compareBytes(BASE_TEX_gst_GlobalList.palettes, NEW_TEX_gst_GlobalList.palettes,
                 (int)(sizeof(tEngPaletteElem) * BASE_TEX_gst_GlobalList.paletteNum),
                 "palettes");
    g_cases++;
}

static void caseAnim(void)
{
    static tTexAnimatedInnerElem elems[4];
    int i;
    int step;
    int which;

    for (which = 0; which < 6; which++)
    {
        sprintf(g_case, "Anim_Update variant %d", which);

        memset(&BASE_TEX_gst_GlobalList, 0, sizeof(tEngTexListStruct));
        memset(&NEW_TEX_gst_GlobalList, 0, sizeof(tEngTexListStruct));
        BASE_TEX_gst_GlobalList.texElems = (tEngTexListElemStruct*)calloc(8, sizeof(tEngTexListElemStruct));
        NEW_TEX_gst_GlobalList.texElems = (tEngTexListElemStruct*)calloc(8, sizeof(tEngTexListElemStruct));
        BASE_TEX_gst_GlobalList.actTexElemNum = 8;
        NEW_TEX_gst_GlobalList.actTexElemNum = 8;
        BASE_TEX_gdst_Animated = NULL;
        NEW_TEX_gdst_Animated = NULL;
        BASE_TEX_gi_NbAnimated = 0;
        NEW_TEX_gi_NbAnimated = 0;
        BASE_TEX_gi_NbAnimatedMax = 0;
        NEW_TEX_gi_NbAnimatedMax = 0;
        BASE_sf_TimeLeft_218 = 0.0f;
        NEW_sf_TimeLeft_218 = 0.0f;

        /* both sides share the frame table: they only read it, and an
           address of their own would make the stored pointer differ */
        for (i = 0; i < 4; i++)
        {
            elems[i].key = i;
            elems[i].texTime = (int16_t)(1 + i);
            elems[i].texIdx = (int16_t)(10 + i);
        }

        MOCKT_Reset();
        for (i = 0; i < 3; i++)
        {
            BASE_TEX_pst_Anim_Add((int16_t)i, (char)(which == 5 ? 0 : 4),
                                  (int16_t)i, elems);
            NEW_TEX_pst_Anim_Add((int16_t)i, (char)(which == 5 ? 0 : 4),
                                 (int16_t)i, elems);
        }
        for (i = 0; i < 3; i++)
        {
            BASE_TEX_gdst_Animated[i].animationFlags = (uint8_t)which;
            NEW_TEX_gdst_Animated[i].animationFlags = (uint8_t)which;
            BASE_TEX_gdst_Animated[i].b3 = (uint8_t)(which + i);
            NEW_TEX_gdst_Animated[i].b3 = (uint8_t)(which + i);
        }

        for (step = 0; step < 12; step++)
        {
            TIM_gf_dt = 0.02f + 0.01f * step;
            BASE_TEX_Anim_Update();
            NEW_TEX_Anim_Update();
            compareBytes(BASE_TEX_gdst_Animated, NEW_TEX_gdst_Animated,
                         (int)(sizeof(tTexAnimatedElem) * 3), "animated");
            compareBytes(BASE_TEX_gst_GlobalList.texElems, NEW_TEX_gst_GlobalList.texElems,
                         (int)(sizeof(tEngTexListElemStruct) * 8), "texElems");
            compareBytes(&BASE_sf_TimeLeft_218, &NEW_sf_TimeLeft_218, sizeof(float),
                         "timeLeft");
        }
        g_cases++;
    }
}

/* ---------------------------------------------------------------- 11
   The parameter decoders. */

static void caseParams(void)
{
    int d;
    tEngTexIntermediateStruct texA;
    tEngTexIntermediateStruct texB;
    tEngTexIntermediateStructTexParams tpA;
    tEngTexIntermediateStructTexParams tpB;
    int i;

    for (d = 0; d < 100; d++)
    {
        sprintf(g_case, "File_DevelopParams colorDepth %d", d);
        memset(&texA, 0, sizeof(texA));
        memset(&texB, 0, sizeof(texB));
        texA.tp.imageDataColorDepth = (uint8_t)d;
        texA.tp.imageDataWidth = 64;
        texA.tp.imageDataHeight = 32;
        texA.colorDepth1 = 7;
        texB.tp = texA.tp;
        texB.colorDepth1 = 7;
        BASE_TEX_File_DevelopParams(&texA);
        NEW_TEX_File_DevelopParams(&texB);
        compareBytes(&texA, &texB, sizeof(texA), "tex");
        g_cases++;
    }

    rngSeed(0x13000u);
    for (i = 0; i < 64; i++)
    {
        sprintf(g_case, "File_TransformParams %d", i);
        memset(&tpA, 0, sizeof(tpA));
        tpA.uuuu = (int)((unsigned)rngByte() << 24 | (unsigned)rngByte() << 16
                         | (unsigned)rngByte() << 8 | rngByte());
        tpA.flagsUnkAndProceduralUnkb1 = (int16_t)(rngByte() << 8 | rngByte());
        tpA.type = rngByte();
        tpA.imageDataColorDepth = rngByte();
        tpB = tpA;
        BASE_TEX_File_TransformParams(&tpA);
        NEW_TEX_File_TransformParams(&tpB);
        compareBytes(&tpA, &tpB, sizeof(tpA), "params");
        g_cases++;
    }
}

int main(void)
{
    MOCKT_Reset();

    caseScale();
    caseConverters();
    caseMasks();
    caseTga();
    caseConvertPF();
    caseConvertSize();
    caseConvertSizeFast();
    casePalette();
    caseCompression();
    caseLoaders();
    caseList();
    caseAnim();
    caseParams();

    printf("textures: %d cases, %d differences"
           " (%d ConvertPF format pairs skipped as undefined)\n",
           g_cases, g_fails, g_pfSkipped);
    return g_fails ? 1 : 0;
}
