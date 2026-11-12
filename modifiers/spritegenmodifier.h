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

void SPG_Modifier_Unapply(tModifier* mdf, tGEOGRO* geogro);
void SPG_Modifier_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* modifierDataToBeSet);
void SPG_Modifier_Destroy(tModifier* mdf);
int SPG_ul_Modifier_Load(struct sModifier* mdf, char* readptr);
void SPG_Modifier_Apply(tModifier* mdf, tGEOGRO* geogro);