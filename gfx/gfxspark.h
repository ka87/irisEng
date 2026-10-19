#pragma once
#include "../gfx.h"

typedef struct sGfxSparkElemData
{
    int i1;
    Vec3 v1;
    float i5;
    float i6;
    float i7;
    float i8;
    float i9;
} tGfxSparkElemData;

typedef struct sGfxSparkData
{
    Vec3 v0;
    Vec3 v1;
    float i4;
    float i5;
    float i6;
    float i7;
    float rand2Max;
    float rand2Min;
    float i10;
    float ii1;
    float rand3Max;
    float rand3Min;
    float ii4;
    float ii5;
    float randMin;
    float randMax;
    int num;
    int iii1;
    int flags;
    int allocdNum;
    int actNum;
    float timeRemaining;
    tGfxSparkElemData* ptr;
} tGfxSparkData;

struct sGfxSparkData* GFX_Spark_Create();
void GFX_Spark_Destroy(struct sGfxSparkData* a1);
int GFX_i_Spark_Render(struct sGfxSparkData* a1);