#pragma once
#include "../sound.h"
#include "../modifiers.h"

unsigned int SND_gst_BankMdFUserList;
tSoundMdfStruct** sndMdfBankUser;

int MDF_SndGetSoundIndexFromBank(tSoundMdfStruct* gsm);
void GEN_ModifierSound_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* readptr);
void GEN_ModifierSound_Destroy(struct sModifier* mdf);
void GEN_ModifierSound_Apply(struct sModifier* mdf, struct sGameObjectMain* gao);
void GEN_ModifierSound_Unapply(struct sModifier* mdf, struct sGameObjectMain* gao);
int GEN_ModifierSound_Load(tModifier* mdf, char* data);
void GEN_ModifierSound_Reinit(tModifier* mdf);
