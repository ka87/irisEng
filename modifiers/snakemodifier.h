#pragma once
#include "../modifiers.h"

typedef struct sSnakeMdfStruct
{
    tGameObjectMain* gaos[20];
    tGameObjectMain* gao;
    float distances[20];
    int num;
    int ranks[20];
    float inertia;
    float f2;
    float f3;
} tSnakeMdfStruct;

void GAO_ModifierSNAKE_Apply(struct sModifier* mdf, void* unk1);
void GAO_ModifierSNAKE_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierSNAKE_Destroy(struct sModifier* mdf);
int GAO_ModifierSNAKE_Load(struct sModifier* mdf, char* data);
void GAO_ModifierSNAKE_Reinit(struct sModifier* mdf);
void GAO_ModifierSNAKE_Unapply(struct sModifier* mdf, void* unk1);