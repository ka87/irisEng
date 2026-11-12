#pragma once
#include "../modifiers.h"

tMdfPrivateDataShadow* pst_FirstShadowMdf;

void GAO_ModifierShadow_Create(struct sGameObjectMain* gao, tModifier* mdf, void* initData);
void GAO_ModifierShadow_Destroy(struct sModifier* mdf);
int GAO_ModifierShadow_Load(struct sModifier* mdf, char* data);
void GAO_ModifierShadow_Apply(struct sModifier* mdf, void* unk1);
void GAO_ModifierShadow_Unapply(struct sModifier* mdf, void* unk1);
void GAO_ModifierShadow_Reinit(struct sModifier* mdf);
void GAO_ModifierShadow_DetectUsedTextures(char* data);
void GAO_ModifierShadow_Apply_All();