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

void GEO_ModifierCrush_Apply(struct sModifier* a1, struct sGEOGRO* a2);
void GEO_ModifierCrush_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GEO_ModifierCrush_Destroy(struct sModifier* a1);
int GEO_ul_ModifierCrush_Load(struct sModifier* a1, char* a2);
void GEO_ModifierCrush_Reinit(struct sModifier* a1);
void GEO_ModifierCrush_Unapply(struct sModifier* a1, struct sGEOGRO* a2);
