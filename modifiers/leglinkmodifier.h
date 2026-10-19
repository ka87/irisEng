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


void GAO_ModifierLegLink_Compute(Mtx4x4* a1, Mtx4x4* a2, Mtx4x4* a3, float a4, float a5);
void GAO_ModifierLegLink_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierLegLink_Destroy(struct sModifier* a1);
void GAO_ModifierLegLink_Apply(struct sModifier* a1, void* unk1);
void GAO_ModifierLegLink_UnApply(struct sModifier* a1, void* unk1);
int GAO_ModifierLegLink_Load(struct sModifier* a1, char* a2);
void GAO_ModifierLegLink_Reinit(struct sModifier* a1);