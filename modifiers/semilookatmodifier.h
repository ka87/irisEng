#pragma once
#include "../modifiers.h"

void GAO_ModifierSemiLookAt_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GAO_ModifierSemiLookAt_Apply(struct sModifier* a1, void* unk1);
void GAO_ModifierSemiLookAt_Unapply(struct sModifier* a1, void* unk1);
int GAO_ModifierSemiLookAt_Load(tModifier* arg0, char* a1);
void GAO_ModifierSemiLookAt_Destroy(struct sModifier* a1);
void GAO_ModifierSemiLookAt_Reinit(struct sModifier* a1);