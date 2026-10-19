#pragma once
#include "../modifiers.h"

typedef struct sOtcMdfStruct
{
    int flags;
    float offset;
    float intensity;
    float speed;
    float timeMultiplier;
} tOtcMdfStruct;

void GEO_ModifierOnduleTonCorps_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GEO_ModifierOnduleTonCorps_Destroy(struct sModifier* a1);
void GEO_ModifierOnduleTonCorps_UnApply(struct sModifier* mdf, struct sGEOGRO* geogro);
void GEO_ModifierOnduleTonCorps_Apply(struct sModifier* mdf, struct sGEOGRO* geogro);
int GEO_ul_ModifierOnduleTonCorps_Load(tModifier* a1, char* a2);

