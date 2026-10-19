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

void GAO_ModifierSNAKE_Apply(struct sModifier* a1, void* unk1);
void GAO_ModifierSNAKE_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierSNAKE_Destroy(struct sModifier* a1);
int GAO_ModifierSNAKE_Load(struct sModifier* arg0, char* a1);
void GAO_ModifierSNAKE_Reinit(struct sModifier* a1);
void GAO_ModifierSNAKE_Unapply(struct sModifier* a1, void* unk1);