#pragma once
#include "../modifiers.h"

typedef struct sRotrMdfStruct
{
    struct sGameObjectMain* gao;
    int ii;
    float ff1;
    float timeFactor1;
    float timeFactor2;
    float ff4;
    Vec3 v1;
} tRotrMdfStruct;

void GAO_ModifierROTR_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierROTR_Destroy(struct sModifier* a1);
void GAO_ModifierROTR_Apply(struct sModifier* a1, void* unk1);
int GAO_ModifierROTR_Load(struct sModifier* arg0, char* a1);
void GAO_ModifierROTR_Reinit(struct sModifier* a1);
void GAO_ModifierROTR_Unapply(struct sModifier* a1, void* unk1);
