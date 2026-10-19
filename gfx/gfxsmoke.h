#pragma once
#include "../math.h"
#include "../gfx.h"

typedef struct sGfxSmokeInnerData
{
    float i1;
    float i2;
    float i3;
    float i4;
    float i5;
    float i6;
    float i7;
} tGfxSmokeInnerData;

typedef struct sGfxSmokeData
{
    float innerData1;
    float innerData2;
    float innerData3;
    float innerData4;
    float innerData5;
    float innerData6;
    float i1;
    float i2;
    float i3;
    float i4;
    int color1;
    int color2;
    int innerDataNum;
    int num;
    byte gap1[4];
    tGfxSmokeInnerData* ptr;
    float timeElapsed;
} tGfxSmokeData;

int GFX_i_Smoke_Render(struct sGfxSmokeData* a1);
struct sGfxSmokeData* GFX_Smoke_Create();
void GFX_Smoke_Destroy(struct sGfxSmokeData* a1);
void GFX_Smoke_Seti(struct sGfxSmokeData* a1, int a2, int num);