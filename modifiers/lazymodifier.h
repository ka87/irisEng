#pragma once
#include "../modifiers.h"
#include "../gameobject.h"

typedef struct sLazyMdfStruct
{
    uint8_t gap[4];
    int upperFlag;
    float f2;
    uint8_t gap1[4];
    Mtx4x4 mtx1;
    int upperFlagBak;
    float ffff2;
    uint8_t gap1qq[8];
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
    uint8_t gap1b[12];
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

void GAO_ModifierLazy_Apply(tModifier* mdf, void* unk1);
void GAO_ModifierLazy_Reinit(tModifier* mdf);
void GAO_ModifierLazy_Create(struct sGameObjectMain* gao, tModifier* mdf, void* initData);
void GAO_ModifierLazy_Destroy(tModifier* mdf);
int GAO_ul_ModifierLazy_Load(tModifier* mdf, char* data);
void GAO_ModifierLazy_Unapply(struct sModifier* mdf, void* unk1);
void GAO_ModifierLazy_UnapplyAll(void);
