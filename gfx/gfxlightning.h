#pragma once
#include "../gfx.h"

typedef struct sGfxLightningData
{
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    float f;
    float sampleFractalFloat;
    float timeElapsed;
    float f3;
    float timeStart;
    float deathTime;
    int pointsNum;
    int flags;
    int pointsNumAllocd;
    int vertexColor;
    Vec3* points;
} tGfxLightningData;

struct sGfxLightningData* GFX_Lightning_Create();
void GFX_Lightning_Destroy(struct sGfxLightningData* a1);
void GFX_Lightning_SampleFractal(struct sGfxLightningData* a1);
int GFX_i_Lightning_Render(struct sGfxLightningData* data);
