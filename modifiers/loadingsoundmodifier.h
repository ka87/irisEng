#pragma once
#include "../modifiers.h"

typedef struct sLoadingSoundMdfStruct
{
    int i1;
    int i2;
    int ll;
    int i3;
    float vol;
    float i5;
    float ii5;
    float ii6;
    int ii7;
    int freq;
    int ii9;
    int flags;
    int ii11;
    int sndIndex;
    int soundInstanceIndex;
    float ii14;
    float ii15;
    float ii16;
    float ii17;
    float ii18;
    float ii19;
    float ii20;
    float maxDist;
    int ii22;
    float ii23;
    float ii24;
    float ii25;
    float ii26;
    float ii27;
    float ii28;
    byte gap[256];
} tLoadingSoundMdfStruct;

void MDF_LoadingSound_Apply(tModifier* a1, tGameObjectMain* a2);
void MDF_LoadingSound_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void MDF_LoadingSound_Destroy(struct sModifier* mdf);
int MDF_LoadingSound_Load(tModifier* arg0, char* a1);
void MDF_LoadingSound_Reinit(struct sModifier* mdf);
void MDF_LoadingSound_Unapply(tModifier* a1, tGameObjectMain* a2);