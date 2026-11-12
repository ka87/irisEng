#pragma once
#include "../gfx.h"

typedef struct sGfxRippleInnerData
{
    Vec3 pos;
    Vec3 v1Inv;
    Vec3 v2;
    float f1;
} tGfxRippleInnerData;


typedef struct sGfxRippleData
{
    Vec3 pos;
    Vec3 v1;
    Vec3 v2;
    float f1;
    float f2;
    float f3;
    float f4;
    float f555;
    int num;
    int color2;
    int color1;
    int rippleMaxElemID;
    int flags;
    int numUnk;
    tGfxRippleInnerData* ptr;
    float fff;
} tGfxRippleData;

struct sGfxRippleData* GFX_Ripple_Create(struct sGameObjectMain* gao);
void GFX_Ripple_Destroy(struct sGfxRippleData* ripple);
BOOL GFX_i_Ripple_Render(struct sGfxRippleData* ripple);
void GFX_Ripple_Seti(struct sGfxRippleData* ripple, int id, int value);
void GFX_Ripple_Setf(struct sGfxRippleData* ripple, int id, float value);