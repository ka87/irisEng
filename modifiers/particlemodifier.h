#pragma once
#include "../modifiers.h"
#include "../particlegenerator.h"

typedef struct sMPAGStruct
{
    struct sPAGStruct* pag;
} tMPAGStruct;

void MPAG_Modifier_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void MPAG_Modifier_Destroy(struct sModifier* a1);
void MPAG_Modifier_Apply(tModifier* a1, void* unk1);
void MPAG_ul_Modifier_Reinit(struct sModifier* a1);
int MPAG_ul_Modifier_Load(tModifier* a1, char* a2);
void MPAG_Modifier_Unapply(tModifier* a1, void* unk1);