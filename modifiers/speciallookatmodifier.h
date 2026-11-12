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

void GAO_ModifierSpecialLookAt_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierSpecialLookAt_Apply(struct sModifier* a1, void* a2);
int GAO_ModifierSpecialLookAt_Load(struct sModifier* a1, char* a2);
void GAO_ModifierSpecialLookAt_Destroy(tModifier* a1);
void GAO_ModifierSpecialLookAt_Unapply(struct sModifier* a1, void* a2);
void GAO_ModifierSpecialLookAt_Reinit(struct sModifier* a1);
