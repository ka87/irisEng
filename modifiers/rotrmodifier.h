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

void GAO_ModifierROTR_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierROTR_Destroy(struct sModifier* mdf);
void GAO_ModifierROTR_Apply(struct sModifier* mdf, void* unk1);
int GAO_ModifierROTR_Load(struct sModifier* mdf, char* data);
void GAO_ModifierROTR_Reinit(struct sModifier* mdf);
void GAO_ModifierROTR_Unapply(struct sModifier* mdf, void* unk1);
