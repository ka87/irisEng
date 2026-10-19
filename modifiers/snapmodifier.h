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

void GEO_ModifierSnap_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GEO_ModifierSnap_Destroy(struct sModifier* a1);
void GEO_ModifierSnap_Apply(struct sModifier* a1, struct sGEOGRO* a2);
void GEO_ModifierSnap_Unapply(struct sModifier* a1, struct sGEOGRO* a2);
int GEO_ul_ModifierSnap_Load(struct sModifier* a1, char* a2);