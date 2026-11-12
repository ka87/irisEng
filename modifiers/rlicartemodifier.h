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

void GEO_ModifierRLICarte_Apply(struct sModifier* mdf, struct sGEOGRO* geo);
void GEO_ModifierRLICarte_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GEO_ModifierRLICarte_Destroy(tModifier* mdf);
void GEO_ModifierRLICarte_Unapply(struct sModifier* mdf, void* unk1);
int GEO_ul_ModifierRLICarte_Load(struct sModifier* mdf, char* data);
void GEO_ModifierRLICarte_Reinit(struct sModifier* mdf);

