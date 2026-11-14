/* proceduraltextures.c, old against new, in one binary over identical
   state.

   Each case refills two identical arenas, runs the frozen copy over one
   and the rewrite over the other, and then compares the whole 512KB
   arena -- so a stray write anywhere, not just in the output buffer, is
   caught -- along with the ordered sequence of engine calls each side
   made and whatever globals the case touched.

   Three groups:

   - the four procedural kinds, which are arithmetic over a pixel buffer
     and are swept over sizes, seeds and step counts;
   - the Bink block ring and the MPEG header reader, which are state
     machines over a byte stream the mock serves deterministically;
   - the entry points that reach the Bink SDK or the display device.
     Those are measured against logging stubs, not against Bink, so what
     they prove is that the two copies call the same things in the same
     order with the same arguments -- nothing about the SDK itself. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../common.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
/* the Bink types and entry points the frozen copy still uses; the
   engine itself decodes through libav now */
#include "bink.h"
#include "../time.h"
#include "../gdi.h"
#include "../soft.h"
#include "../lights.h"
#include "../inputoutput.h"
#include "../gameobject.h"
#include "proctex_mocks.h"

/* ---- the two copies ---- */

extern struct sTexProcWater* BASE_TEXPRO_pv_WaterLoad(char*, int, int, int);
extern struct sTexProcWater* NEW_TEXPRO_pv_WaterLoad(char*, int, int, int);
extern int BASE_TEXPRO_l_WaterUpdate(tTexProceduralStruct*);
extern int NEW_TEXPRO_l_WaterUpdate(tTexProceduralStruct*);
extern void BASE_TEXPRO_WaterDisrupt(tTexProceduralStruct*);
extern void NEW_TEXPRO_WaterDisrupt(tTexProceduralStruct*);

extern tTexProFire* BASE_TEXPRO_pv_FireLoad(char*, int, int, int);
extern tTexProFire* NEW_TEXPRO_pv_FireLoad(char*, int, int, int);
extern int BASE_TEXPRO_l_FireUpdate(tTexProceduralStruct*);
extern int NEW_TEXPRO_l_FireUpdate(tTexProceduralStruct*);

extern tTexProcPlasma* BASE_TEXPRO_pv_PlasmaLoad(char*, int, int, int);
extern tTexProcPlasma* NEW_TEXPRO_pv_PlasmaLoad(char*, int, int, int);
extern int BASE_TEXPRO_l_PlasmaUpdate(tTexProceduralStruct*);
extern int NEW_TEXPRO_l_PlasmaUpdate(tTexProceduralStruct*);

extern tTexProcPhoto* BASE_TEXPRO_pv_PhotoLoad(char*, int, int, int);
extern tTexProcPhoto* NEW_TEXPRO_pv_PhotoLoad(char*, int, int, int);
extern int BASE_TEXPRO_l_PhotoUpdate(tTexProceduralStruct*);
extern int NEW_TEXPRO_l_PhotoUpdate(tTexProceduralStruct*);
extern void BASE_TEXPRO_Photo_SetMapGao(struct sTexProceduralStruct*, struct sGameObjectMain*);
extern void NEW_TEXPRO_Photo_SetMapGao(struct sTexProceduralStruct*, struct sGameObjectMain*);

extern tTexProcMpeg* BASE_TEXPRO_pv_MpegLoad(char*, int, int, int);
extern tTexProcMpeg* NEW_TEXPRO_pv_MpegLoad(char*, int, int, int);
extern void BASE_TEXPRO_MPEG_Start(void);
extern void NEW_TEXPRO_MPEG_Start(void);
extern int BASE_TEXPRO_MPEG_ReadNext(void*);
extern int NEW_TEXPRO_MPEG_ReadNext(void*);
extern int BASE_TEXPRO_l_MpegUpdate(tTexProceduralStruct*);
extern int NEW_TEXPRO_l_MpegUpdate(tTexProceduralStruct*);
extern void BASE_TEXPRO_MpegFree(tTexProceduralStruct*);
extern void NEW_TEXPRO_MpegFree(tTexProceduralStruct*);

extern void BASE_TEXPRO_Free(tTexProceduralStruct*);
extern void NEW_TEXPRO_Free(tTexProceduralStruct*);
extern void BASE_TEXPRO_PhotoFree(tTexProceduralStruct*);
extern void NEW_TEXPRO_PhotoFree(tTexProceduralStruct*);

extern void BASE_TEX_Procedural_Init(void);
extern void NEW_TEX_Procedural_Init(void);
extern void BASE_TEX_Procedural_Add(int16_t, int, int16_t, void*);
extern void NEW_TEX_Procedural_Add(int16_t, int, int16_t, void*);
extern int BASE_TEX_l_File_LoadProcedural(char*, tEngTexIntermediateStruct*, int);
extern int NEW_TEX_l_File_LoadProcedural(char*, tEngTexIntermediateStruct*, int);

extern int BASE_TEXPRO_Bink_GetNextFreeSlotID(void);
extern int NEW_TEXPRO_Bink_GetNextFreeSlotID(void);
extern void BASE_TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(tBinkUnkStruct*);
extern void NEW_TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(tBinkUnkStruct*);
extern void BASE_BINK_AllocLinkedList(tBinkIFPtr*, int);
extern void NEW_BINK_AllocLinkedList(tBinkIFPtr*, int);
extern void BASE_BINK_releaseLinkedList(tBinkIFPtr*);
extern void NEW_BINK_releaseLinkedList(tBinkIFPtr*);
extern int BASE_TEXPRO_Bink_GetAlreadyProcessedFileSize(tBinkIFPtr*);
extern int NEW_TEXPRO_Bink_GetAlreadyProcessedFileSize(tBinkIFPtr*);
extern int BASE_TEXPRO_Bink_ReadActBlock(tBinkIFPtr*);
extern int NEW_TEXPRO_Bink_ReadActBlock(tBinkIFPtr*);
extern void BASE_TEXPRO_BINK_ReadActBlockAndAdvance(tBinkIFPtr*);
extern void NEW_TEXPRO_BINK_ReadActBlockAndAdvance(tBinkIFPtr*);
extern int BASE_TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(tBinkIFPtr*, char*, int);
extern int NEW_TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(tBinkIFPtr*, char*, int);
extern void BASE_TEXPRO_BINK_Close(tBinkIFPtr*);
extern void NEW_TEXPRO_BINK_Close(tBinkIFPtr*);
extern int BASE_TEXPRO_BINK_ReadNumBytes(BINKIO*, void*, unsigned int);
extern int NEW_TEXPRO_BINK_ReadNumBytes(BINKIO*, void*, unsigned int);
extern S32 BASE_BINK_IO_FUNC(BINKIO*, const char*, U32);
extern S32 NEW_BINK_IO_FUNC(BINKIO*, const char*, U32);
extern U32 BASE_BINK_GetBufferSize(BINKIO*, U32);
extern U32 NEW_BINK_GetBufferSize(BINKIO*, U32);
extern U32 BASE_BINK_ReadHeader(BINKIO*, S32, void*, U32);
extern U32 NEW_BINK_ReadHeader(BINKIO*, S32, void*, U32);
extern U32 BASE_BINK_ReadFrame(BINKIO*, U32, S32, void*, U32);
extern U32 NEW_BINK_ReadFrame(BINKIO*, U32, S32, void*, U32);
extern U32 BASE_BINK_Idle(BINKIO*);
extern U32 NEW_BINK_Idle(BINKIO*);
extern void BASE_BINK_Close(BINKIO*);
extern void NEW_BINK_Close(BINKIO*);
extern void BASE_BINK_SetInfo(BINKIO*, void*, U32, U32, U32);
extern void NEW_BINK_SetInfo(BINKIO*, void*, U32, U32, U32);
extern void BASE_PC_VideoStreamCreate_Bink(tBinkIFPtr*, int);
extern void NEW_PC_VideoStreamCreate_Bink(tBinkIFPtr*, int);
extern void BASE_PC_VideoStreamRestart_Bink(tBinkIFPtr*);
extern void NEW_PC_VideoStreamRestart_Bink(tBinkIFPtr*);
extern void BASE_PC_VideoStreamSync_Bink(tBinkIFPtr*);
extern void NEW_PC_VideoStreamSync_Bink(tBinkIFPtr*);
extern void BASE_BINK_CloseAll(tBinkIFPtr*);
extern void NEW_BINK_CloseAll(tBinkIFPtr*);

/* ---- the globals each side owns ---- */

extern tProceduralIntf BASE_TEX_gast_ProceduralInterface[6];
extern tProceduralIntf NEW_TEX_gast_ProceduralInterface[6];
extern int BASE_TexPro_WaterDistruptCounter, NEW_TexPro_WaterDistruptCounter;
extern int BASE_gBinkReadUsedSlots, NEW_gBinkReadUsedSlots;
extern float BASE_KX_442[2], NEW_KX_442[2];
extern float BASE_KY_443[2], NEW_KY_443[2];
extern float BASE_ALPHA_441[2], NEW_ALPHA_441[2];
extern int BASE_TEX_gi_NbProcedural, NEW_TEX_gi_NbProcedural;
extern tTexProceduralStruct *BASE_TEX_gdst_Procedural, *NEW_TEX_gdst_Procedural;
extern tEngTexListStruct BASE_TEX_gst_GlobalList, NEW_TEX_gst_GlobalList;
extern int BASE_TEXPRO_Cos[256], NEW_TEXPRO_Cos[256];
extern tBinkIFPtr *BASE_BINK_IF_PTR_GLOBAL, *NEW_BINK_IF_PTR_GLOBAL;
extern tTexProcMpeg *BASE_TEXPRO_pst_CurrentMpeg, *NEW_TEXPRO_pst_CurrentMpeg;

extern void MOCKP_SetGro(tGEOGRO*);
extern Mtx4x4* MOCKP_AbsoluteMatrix(void);
extern HBINK MOCKP_BinkHandle(int);
extern void MOCKP_SetBink(int, unsigned int, unsigned int, int);

/* ---- harness ---- */

static int cases;
static int failures;
static int valuesCompared;

static void fail(const char* what, const char* detail, int a, int b)
{
    if (failures < 20)
        printf("  FAIL %-34s %s  base=%d new=%d\n", what, detail, a, b);
    ++failures;
}

static void checkInt(const char* what, const char* detail, int a, int b)
{
    ++valuesCompared;
    if (a != b)
        fail(what, detail, a, b);
}

/* The cases below the differential ones drive one side only, so
   base/new is the wrong label for what they compare. */
static void checkIs(const char* what, const char* detail, int got, int want)
{
    ++valuesCompared;
    if (got != want)
    {
        if (failures < 20)
            printf("  FAIL %-34s %s  got=%d want=%d\n", what, detail, got, want);
        ++failures;
    }
}

static void compareSides(const char* what)
{
    int diff = MOCKP_CompareArenas();
    int logAt = MOCKP_CompareLogs();

    ++cases;
    ++valuesCompared;
    if (diff)
        fail(what, "arena bytes differ", diff, 0);
    if (logAt >= 0)
    {
        const char* an = logAt < MOCKP_logLen[0] ? MOCKP_log[0][logAt].fn : "<end>";
        const char* bn = logAt < MOCKP_logLen[1] ? MOCKP_log[1][logAt].fn : "<end>";
        if (failures < 20)
            printf("  FAIL %-34s call %d: base=%s new=%s\n", what, logAt, an, bn);
        ++failures;
    }
    if (MOCKP_overflow)
        fail(what, "mock overflow", MOCKP_overflow, 0);
}

/* A procedural struct and its picture buffer, one per side, outside the
   arena so the arena holds only what the file allocated. */
/* The picture buffer is far larger than any case needs, because the
   photo reveal walks it with a 1024-word row stride -- see the note on
   that in proceduraltextures.c -- and reaches word 130307 on a 256x128
   procedural. */
#define PIC_WORDS (160 * 1024)

typedef struct
{
    tTexProceduralStruct st;
    uint32_t pic[PIC_WORDS];
} tHolder;

static tHolder holder[2];
static int waterFieldsSkipped;

static void setupHolder(int w, int h, uint32_t seed)
{
    int s, i;

    for (s = 0; s < 2; ++s)
    {
        memset(&holder[s], 0, sizeof(holder[s]));
        holder[s].st.width = (int16_t)w;
        holder[s].st.height = (int16_t)h;
        holder[s].st.proceduralPicData = holder[s].pic;
        /* The whole buffer is seeded, not just width*height: the photo
           reveal writes well past the picture proper, and over a run of
           zeroes a mask defect has nothing to change. */
        for (i = 0; i < PIC_WORDS; ++i)
            holder[s].pic[i] = seed * 2654435761u + (uint32_t)i * 2246822519u;
    }
}

static int comparePic(const char* what, int n)
{
    int i, diff = 0;

    for (i = 0; i < n; ++i)
    {
        ++valuesCompared;
        if (holder[0].pic[i] != holder[1].pic[i])
            ++diff;
    }
    if (diff)
        fail(what, "picture words differ", diff, 0);
    return diff;
}

static void compareHolderTail(const char* what, int n)
{
    int i, diff = 0;

    for (i = n; i < PIC_WORDS; ++i)
        if (holder[0].pic[i] != holder[1].pic[i])
            ++diff;
    if (diff)
        fail(what, "picture overrun differs", diff, 0);
}


/* ---- group 1: water ---- */

static void waterCases(void)
{
    static const int sizes[][2] = { { 4, 4 }, { 8, 8 }, { 16, 8 }, { 8, 16 },
                                    { 32, 32 }, { 64, 32 }, { 2, 8 }, { 8, 2 },
                                    { 128, 64 } };
    unsigned i;
    int step, seed;
    char name[80];

    for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i)
    {
        int w = sizes[i][0], h = sizes[i][1];

        for (seed = 1; seed <= 3; ++seed)
        {
            void* tex[2];
            int s;

            MOCKP_Reset();
            setupHolder(w, h, (uint32_t)(seed * 977 + w));
            for (s = 0; s < 2; ++s)
            {
                MOCKP_SetSide(s);
                MOCKP_SeedRand(12345u * seed);
                *(s ? &NEW_TexPro_WaterDistruptCounter : &BASE_TexPro_WaterDistruptCounter) = 0;
                (s ? NEW_KX_442 : BASE_KX_442)[0] = 1.5f;
                (s ? NEW_KX_442 : BASE_KX_442)[1] = -2.5f;
                (s ? NEW_KY_443 : BASE_KY_443)[0] = 3.25f;
                (s ? NEW_KY_443 : BASE_KY_443)[1] = 0.75f;
                (s ? NEW_ALPHA_441 : BASE_ALPHA_441)[0] = 0.3f;
                (s ? NEW_ALPHA_441 : BASE_ALPHA_441)[1] = 1.7f;
                tex[s] = s ? (void*)NEW_TEXPRO_pv_WaterLoad(0, 0, w, h)
                           : (void*)BASE_TEXPRO_pv_WaterLoad(0, 0, w, h);
                holder[s].st.proceduralTexPtr = tex[s];
                for (step = 0; step < 6; ++step)
                {
                    if (w >= 16 && h >= 16)
                    {
                        if (s)
                            NEW_TEXPRO_WaterDisrupt(&holder[s].st);
                        else
                            BASE_TEXPRO_WaterDisrupt(&holder[s].st);
                    }
                    if (s)
                        NEW_TEXPRO_l_WaterUpdate(&holder[s].st);
                    else
                        BASE_TEXPRO_l_WaterUpdate(&holder[s].st);
                }
                if (s)
                    NEW_TEXPRO_Free(&holder[s].st);
                else
                    BASE_TEXPRO_Free(&holder[s].st);
            }
            sprintf(name, "water %dx%d seed %d", w, h, seed);
            /* The two copies cannot be compared on the field itself once
               a ripple is in it. The frozen copy samples the centre cell
               where the row above belongs, so the four-tap kernel is
               really three taps plus the centre; the rewrite reads the
               row above. With no disruption both fields stay flat and
               agree, which is why only the disrupted sizes are skipped.
               waterSymmetryCase below is what checks the rewrite. */
            if (w >= 16 && h >= 16)
            {
                waterFieldsSkipped++;
            }
            else
            {
                compareSides(name);
                comparePic(name, w * h);
                compareHolderTail(name, w * h);
            }
            checkInt(name, "disrupt counter", BASE_TexPro_WaterDistruptCounter,
                     NEW_TexPro_WaterDistruptCounter);
            checkInt(name, "kx0", (int)(BASE_KX_442[0] * 1000), (int)(NEW_KX_442[0] * 1000));
            checkInt(name, "kx1", (int)(BASE_KX_442[1] * 1000), (int)(NEW_KX_442[1] * 1000));
            checkInt(name, "ky0", (int)(BASE_KY_443[0] * 1000), (int)(NEW_KY_443[0] * 1000));
            checkInt(name, "ky1", (int)(BASE_KY_443[1] * 1000), (int)(NEW_KY_443[1] * 1000));
            checkInt(name, "alpha0", (int)(BASE_ALPHA_441[0] * 1000), (int)(NEW_ALPHA_441[0] * 1000));
            checkInt(name, "alpha1", (int)(BASE_ALPHA_441[1] * 1000), (int)(NEW_ALPHA_441[1] * 1000));
        }
    }
}

/* The rewrite reads the row above where the frozen copy re-reads the
   centre cell, so the two fields cannot be compared once a ripple is in
   them. This checks the rewrite on a property the correct kernel has and
   the frozen one does not.

   A single impulse at the centre of a square field, propagated with the
   four-neighbour kernel, stays symmetric about the horizontal axis: the
   row above and the row below contribute the same way. Replacing the
   "above" tap with the centre cell destroys that, because only the row
   below still feeds back. So the rewrite must be symmetric and the
   frozen copy must not be -- the second half is what gives the check
   teeth. */
static void waterSymmetryCase(void)
{
    enum { W = 32, H = 32 };
    int s, step, row, col;
    int asym[2];

    for (s = 0; s < 2; ++s)
    {
        tTexProcWater* water;
        int16_t* f;

        MOCKP_Reset();
        MOCKP_SetSide(s);
        memset(&holder[s], 0, sizeof(holder[s]));
        holder[s].st.width = (int16_t)W;
        holder[s].st.height = (int16_t)H;
        holder[s].st.proceduralPicData = holder[s].pic;
        water = s ? (tTexProcWater*)NEW_TEXPRO_pv_WaterLoad(0, 0, W, H)
                  : (tTexProcWater*)BASE_TEXPRO_pv_WaterLoad(0, 0, W, H);
        holder[s].st.proceduralTexPtr = water;

        /* one impulse, dead centre, in both buffers so the first step
           has nothing else to work from */
        water->ptr1[(H / 2) * W + W / 2] = 8000;
        water->ptr2[(H / 2) * W + W / 2] = 8000;

        for (step = 0; step < 4; ++step)
        {
            if (s)
                NEW_TEXPRO_l_WaterUpdate(&holder[s].st);
            else
                BASE_TEXPRO_l_WaterUpdate(&holder[s].st);
        }

        /* how far the field is from mirror-symmetric about the centre row */
        asym[s] = 0;
        f = water->ptr2;
        for (row = 1; row < H / 2; ++row)
            for (col = 1; col < W - 1; ++col)
            {
                int a = f[(H / 2 - row) * W + col];
                int b = f[(H / 2 + row) * W + col];
                asym[s] += a > b ? a - b : b - a;
            }

        if (s)
            NEW_TEXPRO_Free(&holder[s].st);
        else
            BASE_TEXPRO_Free(&holder[s].st);
    }

    if (asym[1] != 0)
        fail("water symmetry", "the rewrite is not symmetric about the centre row",
             asym[1], 0);
    if (asym[0] == 0)
        fail("water symmetry", "the frozen copy is symmetric too, so this check has no teeth",
             asym[0], 0);
    printf("  water symmetry: rewrite %d, frozen copy %d\n", asym[1], asym[0]);
    printf("  water fields not compared side by side: %d (the disrupted sizes)\n",
           waterFieldsSkipped);
}

/* ---- group 2: fire ---- */

static void fireCases(void)
{
    static const int sizes[][2] = { { 8, 8 }, { 16, 16 }, { 32, 16 }, { 16, 32 },
                                    { 64, 64 }, { 8, 4 }, { 4, 8 }, { 2, 8 } };
    unsigned i;
    int step, unknum;
    char name[80];

    for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i)
    {
        int w = sizes[i][0], h = sizes[i][1];

        /* The uninitialised branch selector is exercised at the two
           values the code tests plus one that falls through. */
        for (unknum = 0; unknum <= 2; ++unknum)
        {
            int s;

            MOCKP_Reset();
            setupHolder(w, h, (uint32_t)(w * 31 + h + unknum));
            for (s = 0; s < 2; ++s)
            {
                tTexProFire* fire;

                MOCKP_SetSide(s);
                MOCKP_SeedRand(999u);
                fire = s ? NEW_TEXPRO_pv_FireLoad(0, 0, w, h)
                         : BASE_TEXPRO_pv_FireLoad(0, 0, w, h);
                fire->unknum = unknum;
                holder[s].st.proceduralTexPtr = fire;
                /* seed the two buffers so the filter has something to chew */
                {
                    int k;
                    for (k = 0; k < w * h; ++k)
                    {
                        fire->ptr[k] = (uint8_t)(k * 7 + 3);
                        fire->ptr2[k] = (uint8_t)(k * 13 + 11);
                    }
                }
                for (step = 0; step < 4; ++step)
                {
                    if (s)
                        NEW_TEXPRO_l_FireUpdate(&holder[s].st);
                    else
                        BASE_TEXPRO_l_FireUpdate(&holder[s].st);
                }
                if (s)
                    NEW_TEXPRO_Free(&holder[s].st);
                else
                    BASE_TEXPRO_Free(&holder[s].st);
            }
            sprintf(name, "fire %dx%d unknum %d", w, h, unknum);
            compareSides(name);
            comparePic(name, w * h);
            compareHolderTail(name, w * h);
        }
    }
}

/* ---- group 3: plasma ---- */

static void plasmaCases(void)
{
    static const int sizes[][2] = { { 8, 8 }, { 16, 16 }, { 32, 32 }, { 64, 32 },
                                    { 32, 64 }, { 64, 64 }, { 128, 64 } };
    unsigned i;
    int step, seed;
    char name[80];

    for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i)
    {
        int w = sizes[i][0], h = sizes[i][1];

        for (seed = 1; seed <= 2; ++seed)
        {
            int s;

            MOCKP_Reset();
            setupHolder(w, h, (uint32_t)(seed * 613 + w));
            for (s = 0; s < 2; ++s)
            {
                MOCKP_SetSide(s);
                MOCKP_SeedRand(4242u * seed);
                holder[s].st.proceduralTexPtr =
                    s ? (void*)NEW_TEXPRO_pv_PlasmaLoad(0, 0, w, h)
                      : (void*)BASE_TEXPRO_pv_PlasmaLoad(0, 0, w, h);
                for (step = 0; step < 5; ++step)
                {
                    if (s)
                        NEW_TEXPRO_l_PlasmaUpdate(&holder[s].st);
                    else
                        BASE_TEXPRO_l_PlasmaUpdate(&holder[s].st);
                }
                if (s)
                    NEW_TEXPRO_Free(&holder[s].st);
                else
                    BASE_TEXPRO_Free(&holder[s].st);
            }
            sprintf(name, "plasma %dx%d seed %d", w, h, seed);
            compareSides(name);
            comparePic(name, w * h);
            compareHolderTail(name, w * h);
        }
    }
}

/* ---- group 4: photo ---- */

static char photoBytes[4096];

static void photoCases(void)
{
    static const int quads[][8] = {
        /* x0 x1 x2 x3  y0 y1 y2 y3 */
        {  10, 200, 190,  20,    5,  10, 120, 115 },
        {   2, 254, 254,   2,    0,   0, 127, 127 },
        {  60,  70, 200, 190,   20,  20,  20,  20 },
        {   0, 255, 128,  64,    3, 126,  64,  64 },
        { 100, 110, 120, 130,   60,  60,  60,  60 },
        {  40, 210, 215,  35,    2,   2, 125, 125 },
        { 250, 260, 255, 253,   10,  20,  30,  40 },
        {  -5,  10, 300,  12,    1, 100,  50,  99 },
    };
    unsigned q;
    int s, k;
    char name[80];

    for (k = 0; k < (int)sizeof(photoBytes); ++k)
        photoBytes[k] = (char)(k * 5 + 1);

    /* the scanline reveal, driven from a given x */
    for (q = 0; q < sizeof(quads) / sizeof(quads[0]); ++q)
    {
        int startX;

        for (startX = 4; startX <= 128; startX += 31)
        {
            MOCKP_Reset();
            MOCKP_SetStream((const unsigned char*)photoBytes, sizeof(photoBytes));
            MOCKP_SetPhoto(photoBytes, 256);
            setupHolder(256, 128, (uint32_t)(q * 101 + startX));
            TIM_gf_dt = 0.05f;
            for (s = 0; s < 2; ++s)
            {
                tTexProcPhoto* photo;
                int step;

                MOCKP_SetSide(s);
                MOCKP_SeedRand(7u);
                photo = s ? NEW_TEXPRO_pv_PhotoLoad(0, 0, 256, 128)
                          : BASE_TEXPRO_pv_PhotoLoad(0, 0, 256, 128);
                holder[s].st.proceduralTexPtr = photo;
                for (k = 0; k < 4; ++k)
                {
                    photo->sa[k] = (int16_t)quads[q][k];
                    photo->sa[k + 4] = (int16_t)quads[q][k + 4];
                }
                photo->x = startX;
                photo->f1 = 1.0f;
                photo->remainingTime = 1.0f;
                for (step = 0; step < 6; ++step)
                {
                    if (s)
                        NEW_TEXPRO_l_PhotoUpdate(&holder[s].st);
                    else
                        BASE_TEXPRO_l_PhotoUpdate(&holder[s].st);
                }
                if (s)
                    NEW_TEXPRO_PhotoFree(&holder[s].st);
                else
                    BASE_TEXPRO_PhotoFree(&holder[s].st);
            }
            sprintf(name, "photo reveal quad %u from %d", q, startX);
            compareSides(name);
            comparePic(name, PIC_WORDS);
        }
    }

    /* the load path: b4/b3/b2/b1 state machine and the border draw */
    {
        int b2, hasPhoto;

        for (b2 = 0; b2 <= 2; ++b2)
            for (hasPhoto = 0; hasPhoto <= 1; ++hasPhoto)
            {
                MOCKP_Reset();
                MOCKP_SetStream((const unsigned char*)photoBytes, sizeof(photoBytes));
                MOCKP_SetPhoto(hasPhoto ? photoBytes : 0, hasPhoto ? 256 : 0);
                setupHolder(64, 32, (uint32_t)(b2 * 7 + hasPhoto));
                TIM_gf_dt = 0.05f;
                for (s = 0; s < 2; ++s)
                {
                    tTexProcPhoto* photo;
                    int step;

                    MOCKP_SetSide(s);
                    photo = s ? NEW_TEXPRO_pv_PhotoLoad(0, 0, 64, 32)
                              : BASE_TEXPRO_pv_PhotoLoad(0, 0, 64, 32);
                    holder[s].st.proceduralTexPtr = photo;
                    photo->b2 = (uint8_t)(b2 == 0 ? 0xFF : (b2 == 1 ? 3 : 200));
                    for (step = 0; step < 3; ++step)
                    {
                        if (s)
                            NEW_TEXPRO_l_PhotoUpdate(&holder[s].st);
                        else
                            BASE_TEXPRO_l_PhotoUpdate(&holder[s].st);
                    }
                    if (s)
                        NEW_TEXPRO_PhotoFree(&holder[s].st);
                    else
                        BASE_TEXPRO_PhotoFree(&holder[s].st);
                }
                sprintf(name, "photo load b2=%d photo=%d", b2, hasPhoto);
                compareSides(name);
                comparePic(name, PIC_WORDS);
            }
    }
}

/* ---- group 5: SetMapGao ---- */

static tGdi_DisplayData displayData;
static tGroIntf groIntf;
static Vec3 groVerts[2][8];
static tGEOGRO gro[2];
static void setMapGaoCases(void)
{
    static const float quads[][12] = {
        { 0,0,0,  1,0,0,  1,1,0,  0,1,0 },
        { 0,0,0,  0,2,0,  3,2,1,  3,0,1 },
        { 1,1,1,  -1,1,2, -1,-1,3, 1,-1,0 },
        { 0,0,0,  5,0,0,  5,0.5f,0, 0,0.5f,0 },
    };
    unsigned q;
    int s, v, i;
    char name[80];
    float* m = (float*)MOCKP_AbsoluteMatrix();

    groIntf.id = 1;
    gpst_DX_stDD = &displayData;
    displayData.winWidth = 1024;
    displayData.winHeight = 768;
    for (i = 0; i < 16; ++i)
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    m[0] = 2.0f; m[5] = 1.5f; m[10] = 0.5f; m[15] = 1.0f;
    m[12] = 7.0f; m[13] = -3.0f;
    {
        float* c = (float*)&displayData.ct.cam.cameraMtx;
        for (i = 0; i < 16; ++i)
            c[i] = 0.0f;
        c[0] = 1.0f; c[5] = 1.0f; c[10] = 1.0f; c[15] = 1.0f;
        c[12] = 100.0f; c[13] = 50.0f;
    }

    for (q = 0; q < sizeof(quads) / sizeof(quads[0]); ++q)
    {
        MOCKP_Reset();
        setupHolder(256, 128, (uint32_t)q);
        for (s = 0; s < 2; ++s)
        {
            tTexProcPhoto* photo;

            MOCKP_SetSide(s);
            photo = s ? NEW_TEXPRO_pv_PhotoLoad(0, 0, 256, 128)
                      : BASE_TEXPRO_pv_PhotoLoad(0, 0, 256, 128);
            holder[s].st.proceduralTexPtr = photo;
            for (v = 0; v < 4; ++v)
            {
                groVerts[s][v].x = quads[q][v * 3 + 0];
                groVerts[s][v].y = quads[q][v * 3 + 1];
                groVerts[s][v].z = quads[q][v * 3 + 2];
            }
            gro[s].intf = &groIntf;
            gro[s].vertexNum = 4;
            gro[s].vertices = groVerts[s];
            MOCKP_SetGro(&gro[s]);
            if (s)
                NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            else
                BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
        }
        /* Every one of the four projected points is comparable. The walk
           in TEXPRO_Photo_SetMapGao runs past its two scratch vectors
           into the view matrix, which is host-layout dependent -- but
           it is written the same way on both sides now, so the two
           agree and nothing has to be excluded. The pass originally
           gave the walk an array of its own, which made the fourth
           point incomparable; that was reverted, because the two
           scratch vectors are used individually by the edge search
           above and so are not slices of one array. */
        sprintf(name, "setmapgao quad %u", q);
        compareSides(name);
    }

    /* the four guards */
    {
        MOCKP_Reset();
        setupHolder(256, 128, 0);
        for (s = 0; s < 2; ++s)
        {
            MOCKP_SetSide(s);
            holder[s].st.proceduralTexPtr = s ? (void*)NEW_TEXPRO_pv_PhotoLoad(0, 0, 256, 128)
                                              : (void*)BASE_TEXPRO_pv_PhotoLoad(0, 0, 256, 128);
            MOCKP_SetGro(&gro[s]);
            gro[s].vertexNum = 5;
            if (s) NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            else   BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            gro[s].vertexNum = 4;
            groIntf.id = 2;
            if (s) NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            else   BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            groIntf.id = 1;
            MOCKP_SetGro(0);
            if (s) NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            else   BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            MOCKP_SetGro(&gro[s]);
            if (s) NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, 0);
            else   BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, 0);
            if (s) NEW_TEXPRO_Photo_SetMapGao(0, (struct sGameObjectMain*)1);
            else   BASE_TEXPRO_Photo_SetMapGao(0, (struct sGameObjectMain*)1);
            displayData.winWidth = 320;
            if (s) NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            else   BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            displayData.winWidth = 1024;
            displayData.winHeight = 200;
            if (s) NEW_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            else   BASE_TEXPRO_Photo_SetMapGao(&holder[s].st, (struct sGameObjectMain*)1);
            displayData.winHeight = 768;
        }
        compareSides("setmapgao guards");
    }
}

/* ---- group 6: the procedural table and the loader ---- */

static tEngTexListElemStruct texElems[2][8];
static unsigned char procStream[64];

static void tableCases(void)
{
    int s, i, idx;
    char name[80];

    BASE_TEX_gst_GlobalList.texElems = texElems[0];
    NEW_TEX_gst_GlobalList.texElems = texElems[1];
    for (s = 0; s < 2; ++s)
        for (i = 0; i < 8; ++i)
        {
            texElems[s][i].normalData.width = (int16_t)(32 + i * 8);
            texElems[s][i].normalData.height = (int16_t)(16 + i * 4);
            texElems[s][i].textElemIdx = (int16_t)i;
        }

    MOCKP_Reset();
    MOCKP_SetSide(0);
    BASE_TEX_Procedural_Init();
    MOCKP_SetSide(1);
    NEW_TEX_Procedural_Init();
    compareSides("procedural init");
    for (i = 0; i < 256; ++i)
        checkInt("procedural init", "cos table", BASE_TEXPRO_Cos[i], NEW_TEXPRO_Cos[i]);
    for (i = 0; i < 6; ++i)
        checkInt("procedural init", "interface id",
                 BASE_TEX_gast_ProceduralInterface[i].id,
                 NEW_TEX_gast_ProceduralInterface[i].id);

    /* 18 adds, three past the 16 the table holds */
    MOCKP_Reset();
    BASE_TEX_gi_NbProcedural = 0;
    NEW_TEX_gi_NbProcedural = 0;
    for (s = 0; s < 2; ++s)
    {
        MOCKP_SetSide(s);
        for (i = 0; i < 18; ++i)
        {
            if (s)
                NEW_TEX_Procedural_Add((int16_t)(i & 7), i % 6, (int16_t)(i * 3), (void*)(size_t)(i + 1));
            else
                BASE_TEX_Procedural_Add((int16_t)(i & 7), i % 6, (int16_t)(i * 3), (void*)(size_t)(i + 1));
        }
    }
    compareSides("procedural add");
    checkInt("procedural add", "count", BASE_TEX_gi_NbProcedural, NEW_TEX_gi_NbProcedural);
    for (i = 0; i < 16; ++i)
    {
        checkInt("procedural add", "elemIdx",
                 BASE_TEX_gdst_Procedural[i].elemIdx, NEW_TEX_gdst_Procedural[i].elemIdx);
        checkInt("procedural add", "s2",
                 BASE_TEX_gdst_Procedural[i].s2, NEW_TEX_gdst_Procedural[i].s2);
        checkInt("procedural add", "width",
                 BASE_TEX_gdst_Procedural[i].width, NEW_TEX_gdst_Procedural[i].width);
        checkInt("procedural add", "height",
                 BASE_TEX_gdst_Procedural[i].height, NEW_TEX_gdst_Procedural[i].height);
        checkInt("procedural add", "intf index",
                 (int)(BASE_TEX_gdst_Procedural[i].proceduralIntf - BASE_TEX_gast_ProceduralInterface),
                 (int)(NEW_TEX_gdst_Procedural[i].proceduralIntf - NEW_TEX_gast_ProceduralInterface));
        checkInt("procedural add", "texPtr",
                 (int)(size_t)BASE_TEX_gdst_Procedural[i].proceduralTexPtr,
                 (int)(size_t)NEW_TEX_gdst_Procedural[i].proceduralTexPtr);
    }

    /* the loader, over every interface index and both the sized and the
       default branch */
    for (idx = 0; idx < 6; ++idx)
        for (i = 0; i <= 1; ++i)
        {
            tEngTexIntermediateStruct tex[2];
            int size = i ? 44 : 0;
            int k;
            int rc[2];

            for (k = 0; k < (int)sizeof(procStream); ++k)
                procStream[k] = (unsigned char)(k * 3 + idx);
            /* first long is the amount already consumed, then the flags,
               width, height and interface index */
            procStream[0] = 12; procStream[1] = 0; procStream[2] = 0; procStream[3] = 0;
            procStream[4] = (unsigned char)(i ? 1 : 0); procStream[5] = 0;
            procStream[6] = 16; procStream[7] = 0;
            procStream[8] = 8; procStream[9] = 0;
            procStream[10] = (unsigned char)idx; procStream[11] = 0;
            /* the length word the mpeg Load compares its size argument
               against, so that size - used is separated from size + used */
            procStream[12] = 32; procStream[13] = 0;
            procStream[14] = 0; procStream[15] = 0;

            MOCKP_Reset();
            MOCKP_SetStream(procStream, sizeof(procStream));
            MOCKP_SetBigFile(-1, 0);
            for (s = 0; s < 2; ++s)
            {
                MOCKP_SetSide(s);
                MOCKP_SeedRand(5u);
                memset(&tex[s], 0, sizeof(tex[s]));
                rc[s] = s ? NEW_TEX_l_File_LoadProcedural((char*)procStream, &tex[s], size)
                          : BASE_TEX_l_File_LoadProcedural((char*)procStream, &tex[s], size);
            }
            sprintf(name, "load procedural idx %d size %d", idx, size);
            compareSides(name);
            checkInt(name, "return", rc[0], rc[1]);
            checkInt(name, "flags", tex[0].flags, tex[1].flags);
            checkInt(name, "realWidth", tex[0].realWidth, tex[1].realWidth);
            checkInt(name, "realHeight", tex[0].realHeight, tex[1].realHeight);
            checkInt(name, "tp flags", tex[0].tp.flagsUnkAndProceduralUnkb1,
                     tex[1].tp.flagsUnkAndProceduralUnkb1);
            checkInt(name, "interfaceIdx", tex[0].tp.proceduralParams.interfaceIdx,
                     tex[1].tp.proceduralParams.interfaceIdx);
            checkInt(name, "params flags", tex[0].tp.proceduralParams.flags,
                     tex[1].tp.proceduralParams.flags);
            checkInt(name, "ptr set",
                     tex[0].tp.proceduralParams.interfacePtr != 0,
                     tex[1].tp.proceduralParams.interfacePtr != 0);
        }
}

/* ---- group 7: the Bink block ring ---- */

static tBinkIFPtr stream[2];
static char readBuf[2][16384];

/* The video half of this file used to be checked here: the slot
   mask, the block ring, the six Bink I/O callbacks and the MPEG state
   machine driving them. All of that apparatus is gone -- libav pulls
   its input through one callback -- so there is nothing on the
   rewrite's side for those cases to be compared against, and they
   have been removed rather than left comparing a frozen copy with
   itself.
   
   What replaces them is below: the new path driven directly, which
   is the same treatment the overlay's F12 edge got when it stopped
   being comparable. The reader is the part worth checking here --
   where it reads from, how much, and that it stops at the end of the
   .bf member rather than running on into whatever follows. The
   decoding beyond it is tests/test_video.c's job.
   
   PC_VideoStreamCreate hands the reader to libav, which will not
   recognise a mocked .bf as a video and returns NULL; that is the
   case the engine already handles, and it is the one reachable here
   without a real video in the archive. */

extern int NEW_TEXPRO_i_ReadFromBF(void* user, uint8_t* buf, int size);
extern void NEW_PC_VideoStreamCreate(tVideoStreamBF* stream, int videoKey);
extern void NEW_PC_VideoStreamClose(tVideoStreamBF* stream);
extern void NEW_PC_VideoStreamSync(tVideoStreamBF* stream);
extern void NEW_PC_VideoStreamRestart(tVideoStreamBF* stream);

static void videoReaderCases(void)
{
    /* these drive the rewrite only, so the mocks' side is fixed */
    MOCKP_SetSide(0);
    tVideoStreamBF st;
    uint8_t buf[64];
    int got;

    /* a plain read advances the position and shrinks what is left */
    memset(&st, 0, sizeof(st));
    st.filePosInBF = 1000;
    st.remaining = 100;
    MOCKP_Reset();
    got = NEW_TEXPRO_i_ReadFromBF(&st, buf, 32);
    checkIs("reader", "a read returns what was asked", got, 32);
    checkIs("reader", "the position advances", st.filePosInBF, 1032);
    checkIs("reader", "what is left shrinks", st.remaining, 68);
    cases++;

    /* a read that still fits is not clamped */
    got = NEW_TEXPRO_i_ReadFromBF(&st, buf, 64);
    checkIs("reader", "a read that fits is whole", got, 64);
    checkIs("reader", "and leaves the remainder", st.remaining, 4);
    cases++;

    /* one that does not fit is cut to the end of the member, rather
       than reading on into whatever follows it in the archive */
    got = NEW_TEXPRO_i_ReadFromBF(&st, buf, 16);
    checkIs("reader", "a read past the end is clamped", got, 4);
    checkIs("reader", "the member is then spent", st.remaining, 0);
    checkIs("reader", "and the position is at its end", st.filePosInBF, 1100);
    cases++;

    /* and a spent member reports the end for ever after */
    got = NEW_TEXPRO_i_ReadFromBF(&st, buf, 16);
    checkIs("reader", "a spent member reads nothing", got, 0);
    checkIs("reader", "and does not move", st.filePosInBF, 1100);
    cases++;

    /* a zero-length read asks the archive for nothing */
    memset(&st, 0, sizeof(st));
    st.filePosInBF = 7;
    st.remaining = 10;
    got = NEW_TEXPRO_i_ReadFromBF(&st, buf, 0);
    checkIs("reader", "nothing asked for is nothing read", got, 0);
    checkIs("reader", "and nothing moves", st.filePosInBF, 7);
    cases++;
}

static void videoStreamCases(void)
{
    /* these drive the rewrite only, so the mocks' side is fixed */
    MOCKP_SetSide(0);
    tVideoStreamBF st;

    /* a key the archive does not have leaves no stream */
    memset(&st, 0, sizeof(st));
    MOCKP_Reset();
    MOCKP_SetBigFile(-1, 0);
    NEW_PC_VideoStreamCreate(&st, 0x1234);
    checkIs("stream", "a missing key opens nothing", st.video != NULL, 0);
    checkIs("stream", "and leaves no position", st.filePosInBF, 0);
    checkIs("stream", "and nothing to restart from", st.startPosInBF, 0);
    cases++;

    /* the empty member the archive uses for a video that was never
       built: a length of 8 and nothing after it */
    memset(&st, 0, sizeof(st));
    MOCKP_Reset();
    MOCKP_SetBigFile(2048, 0);
    MOCKP_PushWord(8);           /* the member's length word */
    NEW_PC_VideoStreamCreate(&st, 0x1234);
    checkIs("stream", "an empty member opens nothing", st.video != NULL, 0);
    checkIs("stream", "and is marked as having no position", st.filePosInBF, 0);
    cases++;

    /* a member with a length: the video starts four bytes in, and
       that is what a restart has to come back to */
    memset(&st, 0, sizeof(st));
    MOCKP_Reset();
    MOCKP_SetBigFile(4096, 0);
    MOCKP_PushWord(50000);       /* the member's length word */
    NEW_PC_VideoStreamCreate(&st, 0x1234);
    checkIs("stream", "the video starts past the length word",
             st.startPosInBF, 4100);
    checkIs("stream", "the whole member is available", st.startSize, 50000);
    checkIs("stream", "the key is kept for a restart", st.videoKey, 0x1234);
    cases++;

    /* closing is safe whether or not anything opened, and twice */
    NEW_PC_VideoStreamClose(&st);
    checkIs("stream", "closing clears the position", st.filePosInBF, 0);
    NEW_PC_VideoStreamClose(&st);
    checkIs("stream", "closing twice is safe", st.video != NULL, 0);
    cases++;

    /* and a sync with nothing open does nothing rather than crashing,
       which is the state a texture naming a missing video sits in */
    memset(&st, 0, sizeof(st));
    NEW_PC_VideoStreamSync(&st);
    NEW_PC_VideoStreamRestart(&st);
    checkIs("stream", "a stream that never opened is inert", st.video != NULL, 0);
    cases++;
}

/* The frame pacing: what used to be a Bink wait, three decodes and a
   copy. A texture that is up to date shows nothing new; one that has
   fallen behind decodes on without copying, up to three frames; and
   one that runs off the end starts the video again, which is what
   makes a video texture loop. */
static int countRecorded(const char* fn)
{
    int i;
    int n = 0;

    for (i = 0; i < MOCKP_logLen[0]; i++)
        if (strcmp(MOCKP_log[0][i].fn, fn) == 0)
            n++;
    return n;
}

/* the third argument of the first call to fn, or -1 */
static long recordedArg3(const char* fn)
{
    int i;

    for (i = 0; i < MOCKP_logLen[0]; i++)
        if (strcmp(MOCKP_log[0][i].fn, fn) == 0)
            return MOCKP_log[0][i].c;
    return -1;
}

static void videoPacingCases(void)
{
    /* these drive the rewrite only, so the mocks' side is fixed */
    MOCKP_SetSide(0);
    tVideoStreamBF st;
    tTexProceduralStruct holderOne;

    memset(&holderOne, 0, sizeof(holderOne));
    holderOne.width = 64;
    holderOne.height = 48;

    /* up to date: nothing is decoded and nothing is shown */
    MOCKP_Reset();
    MOCKP_SetVideo(0, 100, 1, 0, 0);     /* Waiting says yes */
    MOCKP_SetBigFile(4096, 0);
    MOCKP_PushWord(50000);
    memset(&st, 0, sizeof(st));
    st.proceduralStructPtr = &holderOne;
    NEW_PC_VideoStreamCreate(&st, 0x1234);
    checkIs("pacing", "the stream opened", st.video != NULL, 1);
    MOCKP_Reset();
    NEW_PC_VideoStreamSync(&st);
    checkIs("pacing", "a stream that is up to date decodes nothing",
            countRecorded("VID_b_DecodeFrame"), 0);
    checkIs("pacing", "and copies nothing", countRecorded("VID_CopyToBuffer"), 0);
    cases++;

    /* one frame behind: decode it, show it, advance once */
    MOCKP_Reset();
    MOCKP_SetVideo(0, 100, 0, 0, 0);     /* due, and the next is not */
    NEW_PC_VideoStreamSync(&st);
    checkIs("pacing", "a frame behind decodes once",
            countRecorded("VID_b_DecodeFrame"), 1);
    checkIs("pacing", "and shows it", countRecorded("VID_CopyToBuffer"), 1);
    checkIs("pacing", "and advances once", countRecorded("VID_NextFrame"), 1);
    cases++;

    /* a long way behind: catching up stops at three decodes, and only
       the last of them is shown */
    MOCKP_Reset();
    MOCKP_SetVideo(0, 100, 0, ~0, 0);    /* every next frame also due */
    NEW_PC_VideoStreamSync(&st);
    checkIs("pacing", "catching up stops at three frames",
            countRecorded("VID_b_DecodeFrame"), 3);
    checkIs("pacing", "and only the last is shown",
            countRecorded("VID_CopyToBuffer"), 1);
    checkIs("pacing", "and every one is advanced past",
            countRecorded("VID_NextFrame"), 3);
    cases++;

    /* The end of the video: the stream is reopened, which is the
       loop. The reader is wound forward first, so that coming back
       to the start of the member is something the check can see. */
    MOCKP_Reset();
    MOCKP_SetVideo(0, 0, 0, 0, 0);       /* no frames left */
    MOCKP_SetBigFile(4096, 0);
    st.filePosInBF = 4100 + 12345;
    st.remaining = 50000 - 12345;
    NEW_PC_VideoStreamSync(&st);
    checkIs("pacing", "running out closes the stream",
            countRecorded("VID_Close"), 1);
    checkIs("pacing", "and opens it again",
            countRecorded("VID_pst_OpenReader"), 1);
    checkIs("pacing", "from the start of the member", st.filePosInBF, 4100);
    checkIs("pacing", "with the whole member to read", st.remaining, 50000);
    cases++;

    /* the sound is not asked for: a video texture is silent, as the
       Bink path's BinkSetSoundTrack(0, 0) said */
    checkIs("pacing", "a video texture asks for no sound",
            (int)recordedArg3("VID_pst_OpenReader"), 0);
    cases++;
}

int main(void)
{
    MOCKP_AddPair((const void*)BASE_TEXPRO_MPEG_Start, (const void*)NEW_TEXPRO_MPEG_Start);
    MOCKP_AddPair((const void*)BASE_TEXPRO_MPEG_ReadNext, (const void*)NEW_TEXPRO_MPEG_ReadNext);
    MOCKP_AddRange(BASE_TEX_gast_ProceduralInterface, NEW_TEX_gast_ProceduralInterface,
                   sizeof(BASE_TEX_gast_ProceduralInterface));
    MOCKP_AddRange(&holder[0], &holder[1], sizeof(holder[0]));

    printf("proceduraltextures.c: frozen copy against the rewrite\n");
    waterCases();
    waterSymmetryCase();
    fireCases();
    plasmaCases();
    photoCases();
    setMapGaoCases();
    tableCases();
    videoReaderCases();
    videoStreamCases();
    videoPacingCases();

    printf("  %d cases, %d values compared, %d failures\n",
           cases, valuesCompared, failures);
    return failures ? 1 : 0;
}
