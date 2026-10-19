#pragma once
#include "gameobject.h"
#include "common.h"

typedef struct sModifierInterface
{
    int mdfId;
    void (* Create)(struct sGameObjectMain* gao, struct sModifier* mdfElemBase, void* dataToInit);
    void(* Destroy)(struct sModifier* mdfElemBase);
    void(* Apply)(struct sModifier* mdfElemBase, void* unk1); //az unk1 tGroGeneric (MDF_applyall) és gameobject (MDF_Gen fvk) is lehet
    void(* UnApply)(struct sModifier* mdfElemBase, void* unk1);
    int(* Load)(struct sModifier* mdfElemBase, char* readPtr);
    void(* ReInit)(struct sModifier* mdfElemBase);
    
} tModifierInterface;

typedef struct sModifier
{
    struct sModifierInterface* modifierIntf;
    int modifierFlags;
    void* mdfPrivateDataPtr;
    struct sModifier* nextElem;
    struct sModifier* prevElem;
    struct sGameObjectMain* gao;
    struct sGroup* group;
} tModifier;

tModifier* MDF_gpst_GlobalCreate;
extern tModifierInterface MDF_gast_ModifierInterface[25];

void MDF_Reinit(struct sGameObjectMain* a1);
void MDF_Modifier_Destroy(tModifier* a1);
void MDF_Modifier_DuplicateList(struct sGameObjectMain* a1, struct sModifier* a2);
void UpdateModifiersGroup(struct sGameObjectMain* a1, struct sGroup* grp);
int MDF_ul_Modifier_Load(struct sModifier** a1, char* currReaderPtr, int chainLoad, struct sGameObjectMain* gao);
struct sModifier* MDF_pst_GetByType(struct sGameObjectMain* a1, int mdfId);
void MDF_MainApplyGen(tableDescStruct* a1);
void MDF_MainUnApplyGen(tableDescStruct* a1);
struct sGroup* MDF_Modifier_GetCurGroup(struct sModifier* a1);
void MDF_ApplyAll(struct sGameObjectMain* a1);
void MDF_UnApplyAll(struct sGameObjectMain* a1);
void MDF_ApplyAllGao(struct sGameObjectMain* a1);
void MDF_UnApplyAllGao(struct sGameObjectMain* a1);
void MDF_Modifier_Init();
struct sModifier* MDF_pst_Modifier_Create(struct sGameObjectMain* a1, int idx, void* privateDataPtr);