#pragma once
//#include "../graphicsobjects/geo.h"
#include "../modifiers.h"

typedef struct sSpgSpriteMapperStruct
{
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
    uint8_t b5;
    uint8_t b6;
    uint8_t b7;
    uint8_t b8;
} tSpgSpriteMapperStruct;

typedef struct sSpgUnkStruct
{
    int matID;
    float sizeMultiplier;
    float noiseMultiplier;
    float ratioMultiplier;
} tSpgSubElementDataStruct;


typedef struct sSpgMdfStruct
{
    int i1;
    int material;
    float globalSize;
    float globalRatio;
    float extraction;
    float threshold;
    float noise;
    int depth;
    int materialMask;
    float LODCorrectionMultiplier;
    tSpgSpriteMapperStruct spriteMapper[4];
    tSpgSubElementDataStruct subElementData[4];
    int flags;
    float specialFogNear;
    float specialFogFar;
    int fogColor;
} tSpgMdfStruct;

tGeoSpriteElem SPG_SpritesIDX[256];
float SPG_gs_SpecialFogNear;
float SPG_gs_SpecialFogFar;
short SPG_FIndx;

void SPG_Modifier_Unapply(tModifier* a1, tGEOGRO* a2);
void SPG_Modifier_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void SPG_Modifier_Destroy(tModifier* a1);
int SPG_ul_Modifier_Load(struct sModifier* arg0, char* a2);
void SPG_Modifier_Apply(tModifier* a1, tGEOGRO* a2);