#pragma once
#include "gro_common.h"
#include "../common.h"

typedef struct sMultiTextureElemStruct
{
    __int16 texElemIdx;
    uint16_t phongOrPlanarParam;
    unsigned int subTextureFlags;
    int transform1;
    int transform2;
    struct sMultiTextureElemStruct* nextTextureKey;
} tMultiTextureElemStruct;

typedef struct sGEOMAT
{
    struct sGroIntf* intf;
    int usageCount;
    int multiMaterialNumIfType4ThenAmbiantColor;
    struct sGEOMAT** multiMaterialPtrIfType4ThenDiffuseColor;
    int solidVertexColor;
    float unkFloat1;
    float unkFloat2;
    int negativeZeroOrOneUnknown;
    int idxOrPtr; // type 5-nél tMultiTextureElemStruct
    int validityFlag;
} tGEOMAT;

extern tGEOMAT MAT_gst_DefaultSingleMaterial;

char* MAT_pc_GetAllUsedTextureIndex(struct sGdi_DisplayData* a1, unsigned int a2);
void MAT_Init();