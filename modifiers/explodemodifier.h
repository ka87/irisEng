#pragma once
#include "../modifiers.h"

typedef struct sExplodeMdfStruct
{
    struct sGEOGRO* geogroNew;
    struct sGEOGRO* gaogeogro;
    int trinagleNum;
    int vertexNum;
    int triangleChunkNum;
    byte gasp[4];
    float f1;
    float f2;
    float f3;
} tExplodeMdfStruct;

void GAO_ModifierExplode_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierExplode_Apply(struct sModifier* a1, void* unk1);
void GAO_ModifierExplode_Unapply(struct sModifier* a1, void* unk1);
int GAO_ModifierExplode_Load(struct sModifier* mdf, char* bufferPtr);
void GAO_ModifierExplode_Reinit(struct sModifier* a1);
void GAO_ModifierExplode_Destroy(struct sModifier* a1);