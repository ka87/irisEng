#pragma once
#include "../gameobject.h"
#include "../gfx.h"

typedef struct sGfxSunDataInner
{
    float i1;
    float i2;
    int i3;
    float i4;
    float i5;
    float colorFactor;
    uint8_t colorID;
    uint8_t colorID2;
    uint8_t _gap[2];
} tGfxSunDataInner;

typedef struct sGfxSunData
{
    Vec3 v1;
    float i4;
    float i5;
    float i6;
    float i7;
    float i8;
    float i9;
    float i10;
    float i11;
    float i12;
    float i13;
    float i14;
    float multiplier;
    float i16;
    float i17;
    uint8_t gap1[4];
    int num;
    int color;
    int flags;
    uint8_t b1;
    uint8_t b2;
    tGfxSunDataInner elems[32];
    uint8_t gap2[2688];
    float rayLums[8];
    int alphaThresholdFlags;
    Vec3 v2;
    /* The renderer's own state for this sun's occlusion test: it
       made it, it owns it, and nothing here looks inside. */
    void* sampler;
} tGfxSunData;

/* One of the ring of offsets the occlusion test looks at, around the
   sun and scaled by its multiplier. */
typedef struct sGfxSunRingOffset
{
    float x;
    float y;
} tGfxSunRingOffset;

int sunUnkCnt;

struct sGfxSunData* GFX_Sun_Create(void);
void GFX_Sun_Destroy(struct sGfxSunData* sun);
void GFX_Sun_Render(struct sGfxSunData* sun);
void GFX_Sun_Seti(struct sGfxSunData* sun, int id);