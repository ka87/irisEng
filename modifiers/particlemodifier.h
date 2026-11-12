#pragma once
#include "../modifiers.h"
#include "../particlegenerator.h"

typedef struct sMPAGStruct
{
    struct sPAGStruct* pag;
} tMPAGStruct;

void MPAG_Modifier_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData);
void MPAG_Modifier_Destroy(struct sModifier* mdf);
void MPAG_Modifier_Apply(tModifier* mdf, void* unk1);
void MPAG_ul_Modifier_Reinit(struct sModifier* mdf);
int MPAG_ul_Modifier_Load(tModifier* mdf, char* data);
void MPAG_Modifier_Unapply(tModifier* mdf, void* unk1);