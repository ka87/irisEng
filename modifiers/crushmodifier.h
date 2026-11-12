#pragma once
#include "../modifiers.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/geomaterial.h"

typedef struct sCrushMdfStruct
{
    int flags;
    float min;
    float max;
    float factorDefault;
    float factor;
} tCrushMdfStruct;

void GEO_ModifierCrush_Apply(struct sModifier* mdf, struct sGEOGRO* geogro);
void GEO_ModifierCrush_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GEO_ModifierCrush_Destroy(struct sModifier* mdf);
int GEO_ul_ModifierCrush_Load(struct sModifier* mdf, char* data);
void GEO_ModifierCrush_Reinit(struct sModifier* mdf);
void GEO_ModifierCrush_Unapply(struct sModifier* mdf, struct sGEOGRO* geogro);
