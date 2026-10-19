#pragma once
#include "../gfx.h"

typedef struct sGfxAnotherFlareData
{
    Vec3 pos;
    Vec3 v2;
    float timeMultiplier;
    float f2;
    float i1;
    float f3;
    float f4;
    float totalTime;
    float angle;
    byte flags;
    byte b2;
    byte inited;
    char extraFlareElems;
    int color2;
    int color;
    float remainingTime;
} tGfxAnotherFlareData;

struct sGfxAnotherFlareData* GFX_AnotherFlare_Create(struct sGameObjectMain* a1);
int GFX_i_AnotherFlare_Render(struct sGfxAnotherFlareData* a1);