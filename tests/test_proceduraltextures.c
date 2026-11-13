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
static int fourthPointSkipped;

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
            compareSides(name);
            comparePic(name, w * h);
            compareHolderTail(name, w * h);
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
        /* The frozen copy walks a pointer off the end of one Vec3 local
           through the next and into the matrix the camera is still
           pointing at, which is what the rewrite reads as a four-element
           array. Measured here: points 0, 1 and 2 agree exactly, and the
           third projection lands on the first twelve bytes of that
           matrix, so the fourth reads a matrix the frozen copy has just
           overwritten. That one point is therefore not comparable and is
           cleared on both sides before the arenas are compared; see the
           note in docs for why the array is the reading taken. */
        {
            tTexProcPhoto* p0 = (tTexProcPhoto*)holder[0].st.proceduralTexPtr;
            tTexProcPhoto* p1 = (tTexProcPhoto*)holder[1].st.proceduralTexPtr;

            p0->sa[3] = 0; p1->sa[3] = 0;
            p0->sa[7] = 0; p1->sa[7] = 0;
            ++fourthPointSkipped;
        }
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

static void slotCases(void)
{
    int mask, s;
    char name[80];

    for (mask = 0; mask < 64; ++mask)
    {
        int got[2];
        tBinkUnkStruct blk[2];

        MOCKP_Reset();
        for (s = 0; s < 2; ++s)
        {
            MOCKP_SetSide(s);
            *(s ? &NEW_gBinkReadUsedSlots : &BASE_gBinkReadUsedSlots) = mask;
            got[s] = s ? NEW_TEXPRO_Bink_GetNextFreeSlotID()
                       : BASE_TEXPRO_Bink_GetNextFreeSlotID();
            memset(&blk[s], 0, sizeof(blk[s]));
            blk[s].actSlotID = mask & 7;
            if (s)
                NEW_TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(&blk[s]);
            else
                BASE_TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(&blk[s]);
        }
        sprintf(name, "slot mask %d", mask);
        checkInt(name, "next free", got[0], got[1]);
        checkInt(name, "used slots", BASE_gBinkReadUsedSlots, NEW_gBinkReadUsedSlots);
        checkInt(name, "block state", blk[0].stateMachine2MeansBusy, blk[1].stateMachine2MeansBusy);
        checkInt(name, "block slot", blk[0].actSlotID, blk[1].actSlotID);
        ++cases;
    }
}

static int streamFields(int s, long* out)
{
    tBinkIFPtr* b = &stream[s];
    int n = 0;

    out[n++] = b->binkVideoKey;
    out[n++] = b->filePosInBF;
    out[n++] = b->size;
    out[n++] = b->dataBlockStartCurrOffset;
    out[n++] = b->videofilesize;
    out[n++] = b->dataAndLinkedList ? 1 : 0;
    out[n++] = b->linkedListStart && b->dataAndLinkedList
               ? (long)((char*)b->linkedListStart - b->dataAndLinkedList) : -1;
    out[n++] = b->actualElemInLinkedList && b->dataAndLinkedList
               ? (long)((char*)b->actualElemInLinkedList - b->dataAndLinkedList) : -1;
    return n;
}

static void compareStream(const char* what)
{
    long a[16], b[16];
    int n = streamFields(0, a);
    int i;

    streamFields(1, b);
    for (i = 0; i < n; ++i)
    {
        ++valuesCompared;
        if (a[i] != b[i])
            fail(what, "stream field differs", (int)a[i], (int)b[i]);
    }
}

static void binkRingCases(void)
{
    static const int videoSizes[] = { 0, 1, 4095, 4096, 4097, 12000, 40000 };
    unsigned i;
    int s, round;
    char name[80];

    for (i = 0; i < sizeof(videoSizes) / sizeof(videoSizes[0]); ++i)
    {
        int vs = videoSizes[i];

        MOCKP_Reset();
        for (s = 0; s < 2; ++s)
        {
            MOCKP_SetSide(s);
            *(s ? &NEW_gBinkReadUsedSlots : &BASE_gBinkReadUsedSlots) = 0;
            memset(&stream[s], 0, sizeof(stream[s]));
            stream[s].filePosInBF = 1000;
            stream[s].size = vs;
            if (s)
                NEW_BINK_AllocLinkedList(&stream[s], vs);
            else
                BASE_BINK_AllocLinkedList(&stream[s], vs);
            for (round = 0; round < 3; ++round)
            {
                if (s)
                    NEW_TEXPRO_BINK_ReadActBlockAndAdvance(&stream[s]);
                else
                    BASE_TEXPRO_BINK_ReadActBlockAndAdvance(&stream[s]);
            }
        }
        sprintf(name, "bink ring %d", vs);
        compareSides(name);
        compareStream(name);
        checkInt(name, "used slots", BASE_gBinkReadUsedSlots, NEW_gBinkReadUsedSlots);

        /* read it back out through the copy path */
        {
            int want;

            for (want = 1; want <= 9000; want = want * 7 + 13)
            {
                int rc[2];

                for (s = 0; s < 2; ++s)
                {
                    MOCKP_SetSide(s);
                    memset(readBuf[s], 0, sizeof(readBuf[s]));
                    rc[s] = s ? NEW_TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(&stream[s], readBuf[s], want)
                              : BASE_TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(&stream[s], readBuf[s], want);
                }
                sprintf(name, "bink read %d of %d", want, vs);
                checkInt(name, "return", rc[0], rc[1]);
                if (memcmp(readBuf[0], readBuf[1], sizeof(readBuf[0])) != 0)
                    fail(name, "read buffer differs", 0, 0);
                ++valuesCompared;
                compareStream(name);
                ++cases;
            }
        }

        for (s = 0; s < 2; ++s)
        {
            MOCKP_SetSide(s);
            if (s)
            {
                NEW_TEXPRO_Bink_GetAlreadyProcessedFileSize(&stream[s]);
                NEW_BINK_releaseLinkedList(&stream[s]);
                NEW_TEXPRO_BINK_Close(&stream[s]);
            }
            else
            {
                BASE_TEXPRO_Bink_GetAlreadyProcessedFileSize(&stream[s]);
                BASE_BINK_releaseLinkedList(&stream[s]);
                BASE_TEXPRO_BINK_Close(&stream[s]);
            }
        }
        sprintf(name, "bink close %d", vs);
        compareSides(name);
        compareStream(name);
    }

    /* The same ring with slots already taken, so the block reader's
       slot-id ceiling is reached rather than always coming out zero. */
    {
        static const int masks[] = { 0x1, 0x1FF, 0x3FF, 0x7FF };
        unsigned k;

        for (k = 0; k < sizeof(masks) / sizeof(masks[0]); ++k)
        {
            MOCKP_Reset();
            for (s = 0; s < 2; ++s)
            {
                MOCKP_SetSide(s);
                memset(&stream[s], 0, sizeof(stream[s]));
                stream[s].filePosInBF = 1000;
                stream[s].size = 12000;
                *(s ? &NEW_gBinkReadUsedSlots : &BASE_gBinkReadUsedSlots) = masks[k];
                if (s)
                {
                    NEW_BINK_AllocLinkedList(&stream[s], 12000);
                    NEW_TEXPRO_BINK_ReadActBlockAndAdvance(&stream[s]);
                    NEW_TEXPRO_Bink_ReadActBlock(&stream[s]);
                }
                else
                {
                    BASE_BINK_AllocLinkedList(&stream[s], 12000);
                    BASE_TEXPRO_BINK_ReadActBlockAndAdvance(&stream[s]);
                    BASE_TEXPRO_Bink_ReadActBlock(&stream[s]);
                }
            }
            sprintf(name, "bink slots taken %x", masks[k]);
            compareSides(name);
            compareStream(name);
            checkInt(name, "used slots", BASE_gBinkReadUsedSlots, NEW_gBinkReadUsedSlots);
        }
    }

    /* GetAlreadyProcessedFileSize over hand-built ring states */
    {
        int states, offset;

        for (states = 0; states < 16; ++states)
            for (offset = 0; offset <= 4096; offset += 1024)
            {
                int got[2];
                static tBinkUnkStruct ring[2][4];
                int k;

                MOCKP_Reset();
                for (s = 0; s < 2; ++s)
                {
                    MOCKP_SetSide(s);
                    memset(&stream[s], 0, sizeof(stream[s]));
                    for (k = 0; k < 4; ++k)
                    {
                        ring[s][k].stateMachine2MeansBusy = (states >> k) & 1;
                        ring[s][k].nextDataBlock = &ring[s][(k + 1) & 3];
                    }
                    stream[s].dataAndLinkedList = (char*)ring[s];
                    stream[s].linkedListStart = &ring[s][0];
                    stream[s].actualElemInLinkedList = &ring[s][0];
                    stream[s].size = 1;
                    stream[s].dataBlockStartCurrOffset = offset;
                    got[s] = s ? NEW_TEXPRO_Bink_GetAlreadyProcessedFileSize(&stream[s])
                               : BASE_TEXPRO_Bink_GetAlreadyProcessedFileSize(&stream[s]);
                }
                sprintf(name, "processed size states %d offset %d", states, offset);
                checkInt(name, "size", got[0], got[1]);
                ++cases;
            }
    }
}

/* ---- group 8: the MPEG header reader ---- */

static unsigned char mpegStream[256];

static void mpegCases(void)
{
    static const int sizes[] = { 0, 4, 8, 32, 48, 52, 56 };
    static const int positions[] = { -1, 0, 64, 4096 };
    /* 2048 and its two neighbours, because the reader caps its chunk
       there and sets a flag on the short side of the test */
    static const unsigned int lengths[] = { 777, 2047, 2048, 2049, 6000 };
    unsigned i, p, L;
    int s;
    char name[80];

    for (L = 0; L < sizeof(lengths) / sizeof(lengths[0]); ++L)
    for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i)
        for (p = 0; p < sizeof(positions) / sizeof(positions[0]); ++p)
        {
            tTexProcMpeg* mpeg[2];
            int k;

            for (k = 0; k < (int)sizeof(mpegStream); ++k)
                mpegStream[k] = (unsigned char)(k * 11 + 5);
            mpegStream[0] = (unsigned char)sizes[i];
            mpegStream[1] = 0; mpegStream[2] = 0; mpegStream[3] = 0;

            MOCKP_Reset();
            MOCKP_SetStream(mpegStream, sizeof(mpegStream));
            MOCKP_SetBigFile(positions[p], lengths[L]);
            LOA_gb_SpeedMode = (int)(i & 1);
            BIG_gi_ReadMode = (int)(p & 3);
            for (s = 0; s < 2; ++s)
            {
                MOCKP_SetSide(s);
                mpeg[s] = s ? NEW_TEXPRO_pv_MpegLoad((char*)mpegStream, sizes[i], 64, 64)
                            : BASE_TEXPRO_pv_MpegLoad((char*)mpegStream, sizes[i], 64, 64);
                if (s)
                {
                    NEW_TEXPRO_pst_CurrentMpeg = mpeg[s];
                    NEW_TEXPRO_MPEG_Start();
                    NEW_TEXPRO_MPEG_ReadNext(readBuf[s]);
                    NEW_TEXPRO_MPEG_ReadNext(readBuf[s]);
                    NEW_TEXPRO_MPEG_Start();
                    NEW_TEXPRO_MPEG_ReadNext(readBuf[s]);
                }
                else
                {
                    BASE_TEXPRO_pst_CurrentMpeg = mpeg[s];
                    BASE_TEXPRO_MPEG_Start();
                    BASE_TEXPRO_MPEG_ReadNext(readBuf[s]);
                    BASE_TEXPRO_MPEG_ReadNext(readBuf[s]);
                    BASE_TEXPRO_MPEG_Start();
                    BASE_TEXPRO_MPEG_ReadNext(readBuf[s]);
                }
            }
            sprintf(name, "mpeg size %d pos %d len %u", sizes[i], positions[p], lengths[L]);
            compareSides(name);
            if (memcmp(readBuf[0], readBuf[1], 8192) != 0)
                fail(name, "mpeg read buffer differs", 0, 0);
            ++valuesCompared;

            /* and a longer read run, to walk startOffset up to endOffset */
            for (k = 0; k < 6; ++k)
            {
                int rc[2];

                for (s = 0; s < 2; ++s)
                {
                    MOCKP_SetSide(s);
                    if (s)
                    {
                        NEW_TEXPRO_pst_CurrentMpeg = mpeg[s];
                        rc[s] = NEW_TEXPRO_MPEG_ReadNext(readBuf[s]);
                    }
                    else
                    {
                        BASE_TEXPRO_pst_CurrentMpeg = mpeg[s];
                        rc[s] = BASE_TEXPRO_MPEG_ReadNext(readBuf[s]);
                    }
                }
                checkInt(name, "mpeg read return", rc[0], rc[1]);
            }
            compareSides(name);
        }
}

/* The mpeg reader caps its chunk at 2048 and sets a flag on the short
   side of that test. A second read sets the same flag whatever the
   first did, so the difference is only visible after exactly one. */
static void mpegBoundaryCases(void)
{
    static const unsigned int lengths[] = { 2047, 2048, 2049 };
    unsigned L;
    int s, k;
    char name[80];

    for (L = 0; L < sizeof(lengths) / sizeof(lengths[0]); ++L)
    {
        int flagsAfter[2];
        int ret[2];
        int startAfter[2];

        for (k = 0; k < (int)sizeof(mpegStream); ++k)
            mpegStream[k] = (unsigned char)(k * 11 + 5);
        mpegStream[0] = 32; mpegStream[1] = 0; mpegStream[2] = 0; mpegStream[3] = 0;

        MOCKP_Reset();
        MOCKP_SetStream(mpegStream, sizeof(mpegStream));
        MOCKP_SetBigFile(64, lengths[L]);
        LOA_gb_SpeedMode = 0;
        BIG_gi_ReadMode = 0;
        for (s = 0; s < 2; ++s)
        {
            tTexProcMpeg* m;

            MOCKP_SetSide(s);
            m = s ? NEW_TEXPRO_pv_MpegLoad((char*)mpegStream, 32, 64, 64)
                  : BASE_TEXPRO_pv_MpegLoad((char*)mpegStream, 32, 64, 64);
            if (s)
            {
                NEW_TEXPRO_pst_CurrentMpeg = m;
                NEW_TEXPRO_MPEG_Start();
                ret[s] = NEW_TEXPRO_MPEG_ReadNext(readBuf[s]);
            }
            else
            {
                BASE_TEXPRO_pst_CurrentMpeg = m;
                BASE_TEXPRO_MPEG_Start();
                ret[s] = BASE_TEXPRO_MPEG_ReadNext(readBuf[s]);
            }
            flagsAfter[s] = m->flags;
            startAfter[s] = m->startOffset;
        }
        sprintf(name, "mpeg one read len %u", lengths[L]);
        compareSides(name);
        checkInt(name, "return", ret[0], ret[1]);
        checkInt(name, "flags after one read", flagsAfter[0], flagsAfter[1]);
        checkInt(name, "startOffset", startAfter[0], startAfter[1]);
    }
}

/* ---- group 9: what reaches the Bink SDK ---- */

static BINKIO binkIO[2];

static void binkSdkCases(void)
{
    static const int chunkCases[] = { 0, 8, 100, 9000 };
    unsigned c;
    int s, waits;
    char name[80];

    /* BINK_IO_FUNC, which installs the six callbacks */
    for (c = 0; c < sizeof(chunkCases) / sizeof(chunkCases[0]); ++c)
    {
        S32 rc[2];

        MOCKP_Reset();
        MOCKP_SetBigFile(chunkCases[c] ? 2048 : -1, 0);
        MOCKP_PushWord(chunkCases[c]);
        MOCKP_PushWord(chunkCases[c] * 3);
        for (s = 0; s < 2; ++s)
        {
            MOCKP_SetSide(s);
            memset(&stream[s], 0, sizeof(stream[s]));
            memset(&binkIO[s], 0, sizeof(binkIO[s]));
            stream[s].binkVideoKey = 0x1234;
            if (s)
            {
                NEW_BINK_IF_PTR_GLOBAL = &stream[s];
                rc[s] = NEW_BINK_IO_FUNC(&binkIO[s], "x", 0);
            }
            else
            {
                BASE_BINK_IF_PTR_GLOBAL = &stream[s];
                rc[s] = BASE_BINK_IO_FUNC(&binkIO[s], "x", 0);
            }
        }
        sprintf(name, "bink io func %d", chunkCases[c]);
        compareSides(name);
        compareStream(name);
        checkInt(name, "return", (int)rc[0], (int)rc[1]);
        checkInt(name, "callbacks installed",
                 binkIO[0].ReadHeader != 0, binkIO[1].ReadHeader != 0);

        /* the six callbacks themselves */
        for (s = 0; s < 2; ++s)
        {
            MOCKP_SetSide(s);
            if (s)
            {
                NEW_BINK_IF_PTR_GLOBAL = &stream[s];
                NEW_BINK_GetBufferSize(&binkIO[s], 1);
                NEW_BINK_SetInfo(&binkIO[s], readBuf[s], 1, 2, 3);
                NEW_BINK_Idle(&binkIO[s]);
                NEW_BINK_ReadHeader(&binkIO[s], 0, readBuf[s], 64);
                NEW_BINK_ReadFrame(&binkIO[s], 1, 0, readBuf[s], 64);
                NEW_TEXPRO_BINK_ReadNumBytes(&binkIO[s], readBuf[s], 32);
                NEW_BINK_Close(&binkIO[s]);
            }
            else
            {
                BASE_BINK_IF_PTR_GLOBAL = &stream[s];
                BASE_BINK_GetBufferSize(&binkIO[s], 1);
                BASE_BINK_SetInfo(&binkIO[s], readBuf[s], 1, 2, 3);
                BASE_BINK_Idle(&binkIO[s]);
                BASE_BINK_ReadHeader(&binkIO[s], 0, readBuf[s], 64);
                BASE_BINK_ReadFrame(&binkIO[s], 1, 0, readBuf[s], 64);
                BASE_TEXPRO_BINK_ReadNumBytes(&binkIO[s], readBuf[s], 32);
                BASE_BINK_Close(&binkIO[s]);
            }
        }
        sprintf(name, "bink callbacks %d", chunkCases[c]);
        compareSides(name);
        compareStream(name);
        checkInt(name, "bytes read", (int)binkIO[0].BytesRead, (int)binkIO[1].BytesRead);
        if (memcmp(readBuf[0], readBuf[1], 8192) != 0)
            fail(name, "callback buffer differs", 0, 0);
        ++valuesCompared;
    }

    /* Create / Sync / Restart / CloseAll against the logging stubs */
    for (waits = 0; waits < 8; ++waits)
    {
        int frames;

        for (frames = 0; frames <= 6; frames += 3)
        {
            MOCKP_Reset();
            MOCKP_SetBigFile(2048, 0);
            MOCKP_PushWord(9000);
            MOCKP_PushWord(12000);
            setupHolder(64, 32, (uint32_t)(waits * 3 + frames));
            for (s = 0; s < 2; ++s)
            {
                MOCKP_SetSide(s);
                memset(&stream[s], 0, sizeof(stream[s]));
                stream[s].proceduralStructPtr = &holder[s].st;
                stream[s].videofilesize = frames ? 1 : 1000000;
                MOCKP_SetBink(s, (unsigned)frames + 1, 0, waits);
                if (s)
                {
                    NEW_PC_VideoStreamCreate_Bink(&stream[s], 0x99);
                    stream[s].binkHandle = MOCKP_BinkHandle(s);
                    NEW_PC_VideoStreamSync_Bink(&stream[s]);
                    NEW_PC_VideoStreamRestart_Bink(&stream[s]);
                    NEW_BINK_CloseAll(&stream[s]);
                }
                else
                {
                    BASE_PC_VideoStreamCreate_Bink(&stream[s], 0x99);
                    stream[s].binkHandle = MOCKP_BinkHandle(s);
                    BASE_PC_VideoStreamSync_Bink(&stream[s]);
                    BASE_PC_VideoStreamRestart_Bink(&stream[s]);
                    BASE_BINK_CloseAll(&stream[s]);
                }
            }
            sprintf(name, "bink stream waits %d frames %d", waits, frames);
            compareSides(name);
            compareStream(name);
        }
    }

    /* the MPEG update state machine over its flag byte */
    {
        int flags;

        for (flags = 0; flags < 8; ++flags)
        {
            int state;

            for (state = 0; state < 3; ++state)
            {
                int rc[2];

                MOCKP_Reset();
                MOCKP_SetBigFile(2048, 0);
                MOCKP_PushWord(9000);
                MOCKP_PushWord(12000);
                setupHolder(32, 16, (uint32_t)(flags * 5 + state));
                for (s = 0; s < 2; ++s)
                {
                    tTexProcMpeg* m;

                    MOCKP_SetSide(s);
                    MOCKP_SetBink(s, 3, 0, 0x55);
                    m = (tTexProcMpeg*)MEM_p_Alloc(sizeof(tTexProcMpeg));
                    memset(m, 0, sizeof(*m));
                    m->sub.binkKey = (flags & 4) ? -1 : 0x77;
                    m->flags = ((flags & 1) ? 0x100 : 0) | ((flags & 2) ? 2 : 0)
                             | (state << 16);
                    holder[s].st.proceduralTexPtr = m;
                    rc[s] = s ? NEW_TEXPRO_l_MpegUpdate(&holder[s].st)
                              : BASE_TEXPRO_l_MpegUpdate(&holder[s].st);
                    if (s)
                        NEW_TEXPRO_MpegFree(&holder[s].st);
                    else
                        BASE_TEXPRO_MpegFree(&holder[s].st);
                }
                sprintf(name, "mpeg update flags %d state %d", flags, state);
                compareSides(name);
                checkInt(name, "return", rc[0], rc[1]);
                comparePic(name, 32 * 16);
            }
        }
    }
}

int main(void)
{
    MOCKP_AddPair((const void*)BASE_TEXPRO_MPEG_Start, (const void*)NEW_TEXPRO_MPEG_Start);
    MOCKP_AddPair((const void*)BASE_TEXPRO_MPEG_ReadNext, (const void*)NEW_TEXPRO_MPEG_ReadNext);
    MOCKP_AddRange(BASE_TEX_gast_ProceduralInterface, NEW_TEX_gast_ProceduralInterface,
                   sizeof(BASE_TEX_gast_ProceduralInterface));
    MOCKP_AddRange(&holder[0], &holder[1], sizeof(holder[0]));
    MOCKP_AddRange(MOCKP_BinkHandle(0), MOCKP_BinkHandle(1), sizeof(BINK));
    MOCKP_AddRange(&stream[0], &stream[1], sizeof(stream[0]));
    MOCKP_AddRange(&binkIO[0], &binkIO[1], sizeof(binkIO[0]));

    printf("proceduraltextures.c: frozen copy against the rewrite\n");
    waterCases();
    fireCases();
    plasmaCases();
    photoCases();
    setMapGaoCases();
    tableCases();
    slotCases();
    binkRingCases();
    mpegCases();
    mpegBoundaryCases();
    binkSdkCases();

    printf("  %d cases, %d values compared, %d failures\n",
           cases, valuesCompared, failures);
    printf("  (%d SetMapGao cases compare three of four projected corners;\n"
           "   the fourth is not comparable -- see the note at that case)\n",
           fourthPointSkipped);
    return failures ? 1 : 0;
}
