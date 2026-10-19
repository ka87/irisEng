#pragma once
#include "../modifiers.h"

typedef struct sMorphMdfInnerStruct
{
    int num;
    int* ptr1;
    Vec3* ptr2;
} tMorphMdfInnerStruct;

typedef struct sMorphMdfInner2Struct
{
    int num;
    int* ptr;
    float Factor;
    float Prog;
} tMorphMdfInner2Struct;


typedef struct sMorphMdfStruct
{
    int i1;
    int num;
    struct sMorphMdfInnerStruct* ptr;
    int num2;
    struct sMorphMdfInner2Struct* ptr2;
    int i2;
} tMorphMdfStruct;

struct sMorphMdfStruct* GEO_pst_ModifierMorphing_Get(struct sGameObjectMain* a1);
void GEO_ModifierMorphing_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a);
void GEO_ModifierMorphing_Destroy(struct sModifier* a1);
void GEO_ModifierMorphing_Apply(struct sModifier* a1, struct sGEOGRO* geogro);
void GEO_ModifierMorphing_UnApply(struct sModifier* a1, struct sGEOGRO* geogro);
int GEO_ul_ModifierMorphing_Load(struct sModifier* a1, char* a2);