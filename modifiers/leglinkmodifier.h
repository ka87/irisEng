#pragma once
#include "../modifiers.h"

typedef struct sLegLinkMdfStruct
{
    struct sGameObjectMain* gao0;
    struct sGameObjectMain* gao1;
    float i3;
    float i4;
    struct sGameObjectMain* gao2;
    int flags;
} tLegLinkMdfStruct;


void GAO_ModifierLegLink_Compute(Mtx4x4* hip, Mtx4x4* knee, Mtx4x4* foot, float upperLen, float lowerLen);
void GAO_ModifierLegLink_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierLegLink_Destroy(struct sModifier* mdf);
void GAO_ModifierLegLink_Apply(struct sModifier* mdf, void* unk1);
void GAO_ModifierLegLink_UnApply(struct sModifier* mdf, void* unk1);
int GAO_ModifierLegLink_Load(struct sModifier* mdf, char* data);
void GAO_ModifierLegLink_Reinit(struct sModifier* mdf);