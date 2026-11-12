#pragma once
#include "../modifiers.h"

typedef struct sSpecialLookAtMdfStruct
{
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
    int rank;
    struct sGameObjectMain* gao;
    float f1;
    float f2;
    float f3;
} tSpecialLookAtMdfStruct;

void GAO_ModifierSpecialLookAt_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierSpecialLookAt_Apply(struct sModifier* mdf, void* unk1);
int GAO_ModifierSpecialLookAt_Load(struct sModifier* mdf, char* data);
void GAO_ModifierSpecialLookAt_Destroy(tModifier* mdf);
void GAO_ModifierSpecialLookAt_Unapply(struct sModifier* mdf, void* unk1);
void GAO_ModifierSpecialLookAt_Reinit(struct sModifier* mdf);
