#pragma once
#include "../gfx.h"

typedef struct sGfxDiaphragmData
{
    int color;
    float f01[4];
    float elapsedTime;
    Vec3 v1;
} tGfxDiaphragmData;

struct sGfxDiaphragmData* GFX_Diaphragm_Create();
int GFX_i_Diaphragm_Render(struct sGfxDiaphragmData* a1);