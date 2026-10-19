#pragma once
#include "../modifiers.h"

typedef struct sPhotoMdfStruct
{
    int mission;
    int info;
    int aiCanal1;
    int aiCanal2;
    Vec3 sphereOffset;
    Vec3 infoOffset;
    float lod1;
    float lod2;
    float lod0_frameCullingSphereRet2;
    float frame1;
    float frame2;
    float frame0_frameCullingSphereRet1;
    float radius;
    int flags;
} tPhotoMdfStruct;

struct sGameObjectMain* GAO_ModifierPhoto_SnapGOGet(struct sGameObjectMain* a1, struct sPhotoMdfStruct* a2, int a3);
struct sModifier* GAO_ModifierPhoto_Get(struct sGameObjectMain* a1, int a2);
void GAO_ModifierPhoto_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a);
void GAO_ModifierPhoto_Destroy(struct sModifier* a1);
int GAO_ModifierPhoto_Load(struct sModifier* a1, char* a2);
void GAO_ModifierPhoto_Apply(tModifier* a1, void* unk1);
void GAO_ModifierPhoto_Unapply(tModifier* a1, void* unk1);
void GAO_ModifierPhoto_Reinit(tModifier* a1);