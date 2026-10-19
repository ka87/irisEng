#pragma once
#include "../modifiers.h"

typedef struct sStpMdfStruct
{
    int num;
    int hasStp;
    Vec3* verticesStored;
} tStpMdfStruct;

void GEO_ModifierSTP_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a);
void GEO_ModifierSTP_Destroy(struct sModifier* a1);
void GEO_ModifierSTP_Apply(struct sModifier* a1, struct sGEOGRO* a2);
void GEO_ModifierSTP_Unapply(struct sModifier* a1, void* unk1);
int GEO_ul_ModifierSTP_Load(struct sModifier* a1, char* a2);