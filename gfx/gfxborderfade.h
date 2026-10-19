#pragma once
#include "../gfx.h"

typedef struct sGfxBorderFadeData
{
    int flags;
    int color2;
    int color;
    float f1;
    float f2;
    float f3;
    float f4;
    float startTime;
    float finishTime;
    float totalTime;
    float elapsedTime;
    float vertUnk1;
    float vertUnk2;
    float vertUnk3;
    float vertUnk4;
    float vertUnk5;
} tGfxBorderFadeData;

int GFX_i_BorderFade_Render(struct sGfxBorderFadeData* a1);
struct sGfxBorderFadeData* GFX_BorderFade_Create();