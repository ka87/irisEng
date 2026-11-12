#pragma once
#include "../modifiers.h"

typedef struct sExplodeMdfStruct
{
    struct sGEOGRO* geogroNew;
    struct sGEOGRO* gaogeogro;
    int trinagleNum;
    int vertexNum;
    int triangleChunkNum;
    uint8_t gasp[4];
    float f1;
    float f2;
    float f3;
} tExplodeMdfStruct;

void GAO_ModifierExplode_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierExplode_Apply(struct sModifier* mdf, void* unk1);
void GAO_ModifierExplode_Unapply(struct sModifier* mdf, void* unk1);
int GAO_ModifierExplode_Load(struct sModifier* mdf, char* data);
void GAO_ModifierExplode_Reinit(struct sModifier* mdf);
void GAO_ModifierExplode_Destroy(struct sModifier* mdf);