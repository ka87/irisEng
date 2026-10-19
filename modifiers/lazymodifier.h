#pragma once
#include "../modifiers.h"
#include "../gameobject.h"

typedef struct sLazyMdfStruct
{
    byte gap[4];
    int upperFlag;
    float f2;
    byte gap1[4];
    Mtx4x4 mtx1;
    int upperFlagBak;
    float ffff2;
    byte gap1qq[8];
    Mtx4x4 mtx2;
    struct sGameObjectMain* gao;
    float ff1;
    float ff2;
    float ff3;
    float upperMax;
    float ff5;
    float ff6;
    float af1;
    float af2;
    float af3;
    float aaf1;
    float aaf2;
    float aaf3;
    float fff1;
    float ffffdf;
    Vec3 vv;
    float fff2;
    float upperFriction;
    struct sLazyMdfStruct* firstToUnapply;
    byte gap1b[12];
} tLazyMdfStruct;

float UpperFriction;
float UpperFactor;
float UpperGravity;
int UpperFlag;
// ezek vektorok?
float UpperMax;
float LazyFF2_dword_98C738;
float LazyFF3_dword_98C73C;
float UpperMin;
float LazyFF5_dword_98C72C;
float LazyFF6_dword_98C730;
// --
tLazyMdfStruct* p_FirsToUnapply;
int bFirst_286_1;

void GAO_ModifierLazy_Apply(tModifier* a1, void *unk1);
void GAO_ModifierLazy_Reinit(tModifier* a1);
void GAO_ModifierLazy_Create(struct sGameObjectMain* a1, tModifier* a2, void* a3);
void GAO_ModifierLazy_Destroy(tModifier* a1);
int GAO_ul_ModifierLazy_Load(tModifier* arg0, char* a1);
void GAO_ModifierLazy_Unapply(struct sModifier* a1, void* unk1);
void GAO_ModifierLazy_UnapplyAll();
