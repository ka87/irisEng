#pragma once
#include "../modifiers.h"

typedef struct sSoundFxMdfStruct
{
    int i1;
    int flags;
    int i3;
    float i4;
    float i5;
    int mode;
    int delay;
    int feedback;
    float vol;
    int wetPan;
    byte gap[256];
} tSoundFxMdfStruct;

void GEN_ModifierSoundFx_Apply(struct sModifier* a1, struct sGameObjectMain* a2);
void GEN_ModifierSoundFx_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GEN_ModifierSoundFx_Destroy(struct sModifier* a1);
int GEN_ModifierSoundFx_Load(tModifier* arg0, char* a2);
void GEN_ModifierSoundFx_Reinit(struct sModifier* a1);
void GEN_ModifierSoundFx_Unapply(struct sModifier* a1, struct sGameObjectMain* a2);

