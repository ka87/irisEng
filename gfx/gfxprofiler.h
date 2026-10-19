#pragma once
#include "../gfx.h"

typedef struct sGfxProfilerPtr1Data
{
    __int16 s1;
    __int16 s2;
    __int16 s3;
    byte gap[2];
    float f1;
    float f2;
} tGfxProfilerPtr1Data;

typedef struct sGfxProfilerPtr2Data
{
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    Vec3 v4;
    float f3;
    int i2;
    int i3;
} tGfxProfilerPtr2Data;

typedef struct sGfxProfilerPtr4Data
{
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    int xx;
    float fff1;
} tGfxProfilerPtr4Data;



typedef struct sGfxProfilerData
{
    float f0;
    Vec3 v1;
    int uu;
    float f1;
    unsigned int xx;
    int PPPNum;
    tGfxProfilerPtr2Data* ptr2;
    int LPPNum;
    tGfxProfilerPtr1Data* ptr1;
    int num;
    int a1;
    unsigned int* ptr3;
    tGfxProfilerPtr4Data* ptr4;
    int triangleNum;
    int pointsNum;
    int UVNum;
    Vec3 v2;
    Vec3 v3;
    Vec3 v4;
    Vec3 v5;
    int ptr2CurrID;
    int ptr1CurrID;
    int num2;
    int xxxxx;
} tGfxProfilerData;

struct sGfxProfilerData* GFX_Profiler_Create();
void GFX_Profiler_Destroy(struct sGfxProfilerData* a1);
int GFX_i_Profiler_Render(struct sGfxProfilerData* a1);
void GFX_Profiler_Seti(struct sGfxProfilerData* a1, int a2, int a3);
void GFX_Profiler_Setf(struct sGfxProfilerData* a1, int a2, float a3);
void GFX_Profiler_Setv(struct sGfxProfilerData* a1, int a2, Vec3* a3);