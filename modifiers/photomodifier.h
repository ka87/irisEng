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

struct sGameObjectMain* GAO_ModifierPhoto_SnapGOGet(struct sGameObjectMain* gao, struct sPhotoMdfStruct* photo, int useSecond);
struct sModifier* GAO_ModifierPhoto_Get(struct sGameObjectMain* gao, int wantSphere);
void GAO_ModifierPhoto_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierPhoto_Destroy(struct sModifier* mdf);
int GAO_ModifierPhoto_Load(struct sModifier* mdf, char* data);
void GAO_ModifierPhoto_Apply(tModifier* mdf, void* unk1);
void GAO_ModifierPhoto_Unapply(tModifier* mdf, void* unk1);
void GAO_ModifierPhoto_Reinit(tModifier* mdf);