#pragma once
#include "../modifiers.h"

typedef struct sStpMdfStruct
{
    int num;
    int hasStp;
    Vec3* verticesStored;
} tStpMdfStruct;

void GEO_ModifierSTP_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GEO_ModifierSTP_Destroy(struct sModifier* mdf);
void GEO_ModifierSTP_Apply(struct sModifier* mdf, struct sGEOGRO* geo);
void GEO_ModifierSTP_Unapply(struct sModifier* mdf, void* unk1);
int GEO_ul_ModifierSTP_Load(struct sModifier* mdf, char* data);