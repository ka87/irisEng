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
    uint8_t gap[256];
} tSoundFxMdfStruct;

void GEN_ModifierSoundFx_Apply(struct sModifier* mdf, struct sGameObjectMain* gao);
void GEN_ModifierSoundFx_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GEN_ModifierSoundFx_Destroy(struct sModifier* mdf);
int GEN_ModifierSoundFx_Load(tModifier* mdf, char* data);
void GEN_ModifierSoundFx_Reinit(struct sModifier* mdf);
void GEN_ModifierSoundFx_Unapply(struct sModifier* mdf, struct sGameObjectMain* gao);

