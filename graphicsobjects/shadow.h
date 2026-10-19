#pragma once
#include "gro_common.h"
#include "../common.h"
#include "../gameobject.h"

typedef struct sMdfPrivateDataShadow
{
    int flags;
    int texKey;
    float xSizeFactor;
    float ySizeFactor;
    int shadowTextureIdx;
    struct sMdfPrivateDataShadow* nextShadowMdfPrivateData;
    struct sGameObjectMain* gao;
    float zAttenuationFactor;
    int shadowColor;
    float zStart;
    Vec3 centerPos;
    float unkf1;
    int shadowProjectionMode;
    char unk1;
    byte _pad[3];
} tMdfPrivateDataShadow;

int SDW_TextNum;

void SDW_Destroy();
void SDW_Clear();
void SDW_AddAShadow(struct sShadowElemStruct* a1);
void SDW_UpdatePositions();