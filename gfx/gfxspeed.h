#pragma once
#include "../math.h"
#include "../gfx.h"

typedef struct sGfxSpeedData
{
    Vec3 v1;
    float timeRemaining;
    float i2;
    int i3;
    Vec3 v2;
} tGfxSpeedData;

int GFX_i_Speed_Render(struct sGfxSpeedData* a1);
struct sGfxSpeedData* GFX_Speed_Create();
