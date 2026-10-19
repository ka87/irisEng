#pragma once
#include "../modifiers.h"

typedef struct sSymMdfStruct
{
    int flags;
    float i2;
} tSymMdfStruct;

void GEO_ModifierSymetrie_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a);
void GEO_ModifierSymetrie_Destroy(struct sModifier* a1);
void GEO_ModifierSymetrie_Apply(tModifier* a1, tGEOGRO* a2);
void GEO_ModifierSymetrie_Unapply(struct sModifier* a1, struct sGEOGRO* a2);
int GEO_ul_ModifierSymetrie_Load(struct sModifier* a1, char* a2a);
