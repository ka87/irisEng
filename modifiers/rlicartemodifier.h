#pragma once
#include "../modifiers.h"

typedef struct sRliCarteMdfStruct
{
    uint8_t b1;
    uint8_t b2Max3;
    uint8_t b3;
    uint8_t b4;
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

