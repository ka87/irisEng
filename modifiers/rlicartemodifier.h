#pragma once
#include "../modifiers.h"

typedef struct sRliCarteMdfStruct
{
    byte b1;
    byte b2Max3;
    byte b3;
    byte b4;
    int data[64];
    int colors[64];
    int num;
    char* indicesPtr;
    unsigned int* vertexColorDestPtr;
} tRliCarteMdfStruct;

void GEO_ModifierRLICarte_Apply(struct sModifier* a1, struct sGEOGRO* a2);
void GEO_ModifierRLICarte_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GEO_ModifierRLICarte_Destroy(tModifier* a1);
void GEO_ModifierRLICarte_Unapply(struct sModifier* a1, void* unk1);
int GEO_ul_ModifierRLICarte_Load(struct sModifier* a1, char* a2);
void GEO_ModifierRLICarte_Reinit(struct sModifier* a1);

