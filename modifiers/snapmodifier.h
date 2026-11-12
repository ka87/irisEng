#pragma once
#include "../gameobject.h"
#include "../modifiers.h"

typedef struct sSnapMdfInnerStruct
{
    int vec1Idx;
    int vec2Idx;
    Vec3 v;
} tSnapMdfInnerStruct;


typedef struct sSnapMdfStruct
{
    struct sGameObjectMain* gao;
    int num;
    tSnapMdfInnerStruct* ptr;
} tSnapMdfStruct;

void GEO_ModifierSnap_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GEO_ModifierSnap_Destroy(struct sModifier* mdf);
void GEO_ModifierSnap_Apply(struct sModifier* mdf, struct sGEOGRO* geo);
void GEO_ModifierSnap_Unapply(struct sModifier* mdf, struct sGEOGRO* geo);
int GEO_ul_ModifierSnap_Load(struct sModifier* mdf, char* data);