#pragma once
#include "../gameobject.h"
#include "../modifiers.h"

typedef struct sXmecMdfStruct
{
    int i1;
    struct sGameObjectMain* gao2;
    struct sGameObjectMain* gao;
    int flags;
    float distance;
    float gravity;
    float tension;
    float ground;
    float groundFriction;
    float elasticity;
    Vec3 speedVecs[50];
    Vec3 vvv[50];
    byte gap[8];
    Mtx4x4 mtx;
    int boneNum;
    int rank;
    int indices[50];
    struct sGameObjectMain* gaos[50];
    Mtx4x4* matrices[50];
} tXmecMdfStruct;

void GAO_ModifierXMEC_Apply(struct sModifier* a1, void* unk1);
void GAO_ModifierXMEC_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierXMEC_Destroy(struct sModifier* a1);
int GAO_ModifierXMEC_Load(struct sModifier* arg0, char* a1);
void GAO_ModifierXMEC_Reinit(struct sModifier* a1);
void GAO_ModifierXMEC_Unapply(struct sModifier* a1, void* unk1);
void XMEC_OnOff(struct sGameObjectMain* a1, int a2, int a3);