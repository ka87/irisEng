#pragma once
#include "common.h"
#include "math.h"
#include "graphicsobjects/shadow.h"

typedef struct sShadowElemStruct
{
    struct sMdfPrivateDataShadow* shadowMdfPtr;
    Vec3 ZVectorInWorldSpace;
    Vec3 centerPosInWorldSpace;
    Vec3 shadowVectorInWorldSpace;
    Vec3 UVectorInWorldSpace;
    Vec3 VVectorInWorldSpace;
    Vec3 posTestVecMax;
    Vec3 posTestVecMin;
    Vec3 coefficients; // U, V, Z
    float ZDepth;
    float startFactor;
    uint32_t color;
    uint32_t sdwTexElemNum;
} tShadowElemStruct;

typedef struct sShadowStruct
{
    int maxShadowNum;
    int actShadowNum;
    struct sShadowElemStruct* shadowElems;
} tShadowStruct;

void SDW_DisplayShadowsOnCurrentObject(struct sGEOGRO* geogro);

