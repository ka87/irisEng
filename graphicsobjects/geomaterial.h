#pragma once
#include "gro_common.h"
#include "../common.h"

struct sGdi_DisplayData;

typedef struct sMultiTextureElemStruct
{
    int16_t texElemIdx;
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
    // a texture index for a single material, and for a type 5 the head
    // of its tMultiTextureElemStruct chain, so the slot holds an
    // address as well as a number
    intptr_t idxOrPtr;
    int validityFlag;
} tGEOMAT;

// A type 4 material is the first five fields and nothing else: that is
// what its two allocators write and all that anything reads of one, so
// it is allocated to here rather than to the whole of tGEOMAT.
#define MAT_MULTI_BYTES (offsetof(tGEOMAT, solidVertexColor) + sizeof(int))

extern tGEOMAT MAT_gst_DefaultSingleMaterial;

char* MAT_pc_GetAllUsedTextureIndex(struct sGdi_DisplayData* dd, unsigned int texCount);
void MAT_Init();