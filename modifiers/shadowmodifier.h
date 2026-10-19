#pragma once
#include "../modifiers.h"

tMdfPrivateDataShadow* pst_FirstShadowMdf;

void GAO_ModifierShadow_Create(struct sGameObjectMain* a1, tModifier* a2, void* a3);
void GAO_ModifierShadow_Destroy(struct sModifier* a1);
int GAO_ModifierShadow_Load(struct sModifier* a1, char* a2);
void GAO_ModifierShadow_Apply(struct sModifier* a1, void* unk1);
void GAO_ModifierShadow_Unapply(struct sModifier* a1, void* unk1);
void GAO_ModifierShadow_Reinit(struct sModifier* a1);
void GAO_ModifierShadow_DetectUsedTextures(char* a1);
void GAO_ModifierShadow_Apply_All();