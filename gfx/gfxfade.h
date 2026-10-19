#pragma once
#include "../gfx.h"

typedef struct sGfxFadeData
{
    int color1;
    int color2;
    float factors[5];
    float elapsedTime;
    float f;
} tGfxFadeData;

int GFX_i_Fade_Render(struct sGfxFadeData* a1);
struct sGfxFadeData* GFX_Fade_Create();
