#pragma once
#include "../modifiers.h"

typedef struct sSymMdfStruct
{
    int flags;
    float i2;
} tSymMdfStruct;

void GEO_ModifierSymetrie_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GEO_ModifierSymetrie_Destroy(struct sModifier* mdf);
void GEO_ModifierSymetrie_Apply(tModifier* mdf, tGEOGRO* geo);
void GEO_ModifierSymetrie_Unapply(struct sModifier* mdf, struct sGEOGRO* geo);
int GEO_ul_ModifierSymetrie_Load(struct sModifier* mdf, char* data);
