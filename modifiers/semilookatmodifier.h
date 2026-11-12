#pragma once
#include "../modifiers.h"

void GAO_ModifierSemiLookAt_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void GAO_ModifierSemiLookAt_Apply(struct sModifier* mdf, void* unk1);
void GAO_ModifierSemiLookAt_Unapply(struct sModifier* mdf, void* unk1);
int GAO_ModifierSemiLookAt_Load(tModifier* mdf, char* data);
void GAO_ModifierSemiLookAt_Destroy(struct sModifier* mdf);
void GAO_ModifierSemiLookAt_Reinit(struct sModifier* mdf);