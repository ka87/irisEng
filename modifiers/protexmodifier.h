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
    uint8_t gap[4];
    int texElemsI22field;
    int texElemIdx;
    int16_t* unkPtrFreedInUnapply;
    int16_t* unkPtr2;
    uint8_t* unkNumProtexWater;
    tGameObjectMain* gao;
    struct sProtexMdfStruct* prevProtex;
    float* ptr3;
} tProtexMdfStruct;

tProtexMdfStruct* p_FirstActiveProtex;

void PROTEX_Disturb(struct sGameObjectMain* gao, struct sGEOGRO* geogro);
void PROTEX_Modifier_Reinit(struct sModifier* mdf);
void PROTEX_Modifier_Destroy(struct sModifier* mdf);
int PROTEX_ul_Modifier_Load(tModifier* mdf, char* data);
void PROTEX_Modifier_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void PROTEX_Modifier_Unapply(tModifier* mdf, void* unk1);
void PROTEX_Modifier_Apply(tModifier* mdf, void* unk1);
void PROTEX_BeforeDraw(void);