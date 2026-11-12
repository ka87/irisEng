#pragma once
#include "../gfx.h"

typedef struct sGfxWater2InnerData
{
    float f1;
    float f2;
    float f3;
    Vec3 v3;
    float ff1;
    float ff2;
    float ff3;
    float ff4;
    float ff5;
    int color;
} tGfxWater2InnerData;


typedef struct sGfxWater2Data
{
    int numToAlloc;
    int i2;
    int num;
    tGfxWater2InnerData* ptr;
} tGfxWater2Data;

struct sGfxWater2Data* GFX_Water2_Create(void);
void GFX_Water2_Destroy(struct sGfxWater2Data* water);
int GFX_i_Water2_Render(struct sGfxWater2Data* water);
void GFX_Water2_Seti(struct sGfxWater2Data* water, int id, int offset, int value);
void GFX_Water2_Setf(struct sGfxWater2Data* water, int id, int offset, float value);
void GFX_Water2_Setv(struct sGfxWater2Data* water, int id, int offset, Vec3* value);
