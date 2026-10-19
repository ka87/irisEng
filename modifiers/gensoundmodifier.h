#pragma once
#include "../sound.h"
#include "../modifiers.h"

unsigned int SND_gst_BankMdFUserList;
tSoundMdfStruct** sndMdfBankUser;

int MDF_SndGetSoundIndexFromBank(tSoundMdfStruct* a1);
void GEN_ModifierSound_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3);
void GEN_ModifierSound_Destroy(struct sModifier* a1);
void GEN_ModifierSound_Apply(struct sModifier* a1, struct sGameObjectMain* a2);
void GEN_ModifierSound_Unapply(struct sModifier* a1, struct sGameObjectMain* a2);
int GEN_ModifierSound_Load(tModifier* arg0, char* a2);
void GEN_ModifierSound_Reinit(tModifier* a1);
