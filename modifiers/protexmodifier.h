#pragma once
#include "../common.h"

typedef struct sProtexMdfStruct
{
    int maxMaterialID;
    int unkMatCnt;
    int i1;
    int i2;
    char* ptrCharArray;
    char* ptrCharArray2;
    int flags;
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    int unkProbablyDim1;
    int unkProbablyDim2;
    byte gap[4];
    int texElemsI22field;
    int texElemIdx;
    int16_t* unkPtrFreedInUnapply;
    int16_t* unkPtr2;
    unsigned __int8* unkNumProtexWater;
    tGameObjectMain* gao;
    struct sProtexMdfStruct* prevProtex;
    float* ptr3;
} tProtexMdfStruct;

tProtexMdfStruct* p_FirstActiveProtex;

void PROTEX_Disturb(struct sGameObjectMain* gao, struct sGEOGRO* geogro);
void PROTEX_Modifier_Reinit(struct sModifier* a1);
void PROTEX_Modifier_Destroy(struct sModifier* a1);
int PROTEX_ul_Modifier_Load(tModifier* arg0, char* a1);
void PROTEX_Modifier_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void PROTEX_Modifier_Unapply(tModifier* a1, void* unk1);
void PROTEX_Modifier_Apply(tModifier* a1, void* unk1);
void PROTEX_BeforeDraw();