#include "memory.h"
#include "loading.h"
#include "modifiers.h"

#include "modifiers/snapmodifier.h"
#include "modifiers/otcmodifier.h"
#include "modifiers/explodemodifier.h"
#include "modifiers/leglinkmodifier.h"
#include "modifiers/morphmodifier.h"
#include "modifiers/semilookatmodifier.h"
#include "modifiers/shadowmodifier.h"
#include "modifiers/speciallookatmodifier.h"
#include "modifiers/gensoundmodifier.h"
#include "modifiers/xmenmodifier.h"
#include "modifiers/xmecmodifier.h"
#include "modifiers/spritegenmodifier.h"
#include "modifiers/symetriemodifier.h"
#include "modifiers/rotrmodifier.h"
#include "modifiers/snakemodifier.h"
#include "modifiers/soundfxmodifier.h"
#include "modifiers/protexmodifier.h"
#include "modifiers/particlemodifier.h"
#include "modifiers/loadingsoundmodifier.h"
#include "modifiers/photomodifier.h"
#include "modifiers/stpmodifier.h"
#include "modifiers/crushmodifier.h"
#include "modifiers/rlicartemodifier.h"
#include "modifiers/lazymodifier.h"

tModifierInterface MDF_gast_ModifierInterface[25];

void MDF_Reinit(struct sGameObjectMain* a1)
{
    tModifier* i;
    void(*v2)(struct sModifier*);

    if ((a1->flags & 0x2000) != 0)
    {
        for (i = a1->extended->mdf; i; i = i->nextElem)
        {
            v2 = i->modifierIntf->ReInit;
            if (v2)
                v2(i);
        }
    }
}

void MDF_ApplyGen(tGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tModifier* i;
    int v3;

    if ((a1->flags & 0x2000) != 0)
    {
        v1 = a1->extended;
        if (v1)
        {
            for (i = v1->mdf; i; i = i->nextElem)
            {
                v3 = i->modifierFlags;
                if ((v3 & 0x2000000) == 0 && (v3 & 0x4000000) != 0)
                    i->modifierIntf->Apply(i, a1);
            }
        }
    }
}

void MDF_MainApplyGen(tableDescStruct* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;

    v1 = a1->firstElemPtr;
    for (i = ((tableElemStructPF *)(a1->nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        if (v1->objectPtr)
        {
            if (((int)v1->objectPtr & 3) != 1)
                MDF_ApplyGen((tGameObjectMain*)(v1->objectPtr));
        }
    }
}

void MDF_UnApplyGen(tGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tModifier* i;
    int v3;

    if ((a1->flags & 0x2000) != 0)
    {
        v1 = a1->extended;
        if (v1)
        {
            for (i = v1->mdf; i; i = i->nextElem)
            {
                v3 = i->modifierFlags;
                if ((v3 & 0x2000000) == 0 && (v3 & 0x4000000) != 0)
                    i->modifierIntf->UnApply(i, a1);
            }
        }
    }
}

void MDF_MainUnApplyGen(tableDescStruct* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;

    v1 = a1->firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        if (v1->objectPtr)
        {
            if (((int)v1->objectPtr & 3) != 1)
                MDF_UnApplyGen((tGameObjectMain*)v1->objectPtr);
        }
    }
}

void MDF_Modifier_Destroy(tModifier* a1)
{
    a1->modifierIntf->Destroy(a1);
    MEM_Free(a1);
}

struct sModifier* MDF_pst_Modifier_Create(struct sGameObjectMain* a1, int idx, void* privateDataPtr)
{
    tModifier* v3;
    tModifierInterface* v4;

    v3 = (tModifier*)MEM_p_Alloc(sizeof(tModifier));
    v3->nextElem = 0;
    v3->prevElem = 0;
    v3->modifierFlags = 0;
    v3->group = 0;
    v4 = &MDF_gast_ModifierInterface[idx];
    v3->modifierIntf = v4;
    v3->gao = a1;
    v4->Create(a1, v3, privateDataPtr);
    return v3;
}

int MDF_ul_Modifier_Load(struct sModifier** a1, char *currReaderPtr, int chainLoad, struct sGameObjectMain* gao)
{
    char *v4;
    int v5;
    tGameObjectMain* v6;
    tModifier* mdf;
    int v8;
    int v9;
    tModifier* v10;
    tModifier* v11;
    char* v13;
    int a2;

    v4 = currReaderPtr;
    MDF_gpst_GlobalCreate = 0;
    v5 = LOA_ReadLong(&currReaderPtr, 0, 1);
    v6 = gao;
    mdf = MDF_pst_Modifier_Create(gao, v5, 0);
    *a1 = mdf;
    mdf->gao = v6;
    (*a1)->modifierFlags = LOA_ReadLong(&currReaderPtr, 0, 1);
    v8 = (*a1)->modifierIntf->Load(*a1, currReaderPtr);
    currReaderPtr += v8;
    v13 =currReaderPtr;
    LOA_ReadLong(&v13, &a2, 4);
    if (chainLoad)
    {
        if (a2 != -1)
        {
            v9 = MDF_ul_Modifier_Load(&(*a1)->nextElem, currReaderPtr, 1, v6);
            v10 = *a1;
            currReaderPtr += v9;
            v11 = v10->nextElem;
            if (v11)
                v11->prevElem = v10;
        }
    }
    return currReaderPtr - v4;
}

struct sModifier* MDF_pst_GetByType(struct sGameObjectMain* a1, int mdfId)
{
    tGameObjectExtended* v2;
    tModifier* result;

    if (!a1)
        return NULL;
    v2 = a1->extended;
    if (!v2)
        return NULL;
    result = v2->mdf;
    if (!result)
        return 0;
    while (result->modifierIntf->mdfId != mdfId)
    {
        result = result->nextElem;
        if (!result)
            return NULL;
    }
    return result;
}

void MDF_Modifier_AddToGameObject(tGameObjectMain* a1, tModifier* a2)
{
    tGameObjectExtended* v2;
    tModifier* v3;

    a2->gao = a1;
    v2 = a1->extended;
    v3 = v2->mdf;
    if (v3)
    {
        for (; v3->nextElem; v3 = v3->nextElem);
        a2->prevElem = v3;
        v3->nextElem = a2;
    }
    else
    {
        v2->mdf = a2;
    }
}

void MDF_Modifier_DuplicateList(struct sGameObjectMain* a1, struct sModifier* a2)
{
    tGameObjectExtended* v2;
    tModifier* v3;
    tModifier* v4; 

    MDF_gpst_GlobalCreate = 0;
    if (a1)
    {
        v2 = a1->extended;
        if (v2)
        {
            v3 = a2;
            if (a2)
            {
                do
                {
                    MDF_gpst_GlobalCreate = v3;
                    v4 = MDF_pst_Modifier_Create(a1, v3->modifierIntf->mdfId, v3->mdfPrivateDataPtr);
                    MDF_Modifier_AddToGameObject(a1, v4);
                    v3 = v3->nextElem;
                } while (v3);
            }
            else
            {
                v2->mdf = 0;
            }
        }
    }
}

void UpdateModifiersGroup(struct sGameObjectMain* a1, struct sGroup* grp)
{
    tGameObjectExtended* v2;
    tModifier* i;
    tModifierInterface* v4;
    void(*v5)(tModifier*);
    tableElemStructPF* v6;
    tableElemStructPF* j; 
    tGameObjectMain* v8; 
    tGameObjectExtended* v9;
    tModifier* v10;
    tModifier* v11;
    tModifierInterface* v12; 
    void(*v13)(tModifier*);

    v2 = a1->extended;
    if (v2)
    {
        for (i = v2->mdf; i; i = i->nextElem)
        {
            v4 = i->modifierIntf;
            i->group = grp;
            v5 = v4->ReInit;
            if (v5)
                v5(i);
        }
    }
    v6 = grp->tablePF->firstElemPtr;
    for (j = ((tableElemStructPF*)(grp->tablePF->nextElemPtr)) - 1; v6 <= j; ++v6)
    {
        v8 = (tGameObjectMain*)v6->objectPtr;
        if (v6->objectPtr)
        {
            if (((uint8_t)v8 & 3) != 1)
            {
                v9 = v8->extended;
                if (v9)
                {
                    v10 = v9->mdf;
                    if (v10)
                    {
                        v11 = v10;
                        do
                        {
                            v12 = v11->modifierIntf;
                            v11->group = grp;
                            v13 = v12->ReInit;
                            if (v13)
                                v13(v11);
                            v11 = v11->nextElem;
                        } while (v11);
                    }
                }
            }
        }
    }
}

struct sGroup* MDF_Modifier_GetCurGroup(struct sModifier* a1)
{
    tGroup* result;
    tGameObjectExtended* result1;

    result = a1->group;
    if (!result)
    {
        result1 = a1->gao->extended;
        if (result1)
            result = result1->grp;
    }
    return result;
}

void MDF_ApplyAll(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGroGeneric* v2;
    tGameObjectVisualObject* v3;
    tGameObjectExtended* v4;
    tModifier* i;

    v1 = a1->basic;
    v2 = 0;
    if (v1)
    {
        v3 = v1->actorObjectOrVisualObject;
        if (v3)
            v2 = v3->GROGEOOrSomethingElse;
    }
    if ((a1->flags & 0x2000) != 0)
    {
        v4 = a1->extended;
        if (v4)
        {
            for (i = v4->mdf; i; i = i->nextElem)
            {
                if ((i->modifierFlags & 0xF000000) == 0)
                    i->modifierIntf->Apply(i, (void*)v2);
            }
        }
    }
}

void MDF_UnApplyAll(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGroGeneric* v2;
    tGameObjectVisualObject* v3;
    tGameObjectExtended* v4;
    tModifier* i;

    v1 = a1->basic;
    v2 = NULL;
    if (v1)
    {
        v3 = v1->actorObjectOrVisualObject;
        if (v3)
            v2 = v3->GROGEOOrSomethingElse;
    }
    if ((a1->flags & 0x2000) != 0)
    {
        v4 = a1->extended;
        if (v4)
        {
            for (i = v4->mdf; i; i = i->nextElem)
            {
                if ((i->modifierFlags & 0xF000000) == 0)
                    i->modifierIntf->UnApply(i, (void*)v2);
            }
        }
    }
}

void MDF_ApplyAllGao(struct sGameObjectMain* a1)
{
    tGEOGRO* v1;
    tGameObjectVisualObject* v2;
    tGameObjectExtended* v3;
    tModifier* i;
    int v5;

    v1 = 0;
    if ((a1->flags & 0x4000) != 0)
    {
        v2 = a1->basic->actorObjectOrVisualObject;
        v1 = (tGEOGRO *)(v2->GROGEOOrSomethingElse);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = gpst_DX_stDD->drawMaskAllowedStuff & v2->objDrawMask;
    }
    else
    {
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = gpst_DX_stDD->drawMaskAllowedStuff;
    }
    if ((a1->flags & 0x2000) != 0)
    {
        v3 = a1->extended;
        if (v3)
        {
            for (i = v3->mdf; i; i = i->nextElem)
            {
                v5 = i->modifierFlags;
                if ((v5 & 0xA000000) == 0 && (v5 & 0x1000000) != 0)
                {
                    gpst_DX_stDD->currTargetGao = a1;
                    i->modifierIntf->Apply(i, v1);
                }
            }
        }
    }
}

void MDF_UnApplyAllGao(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGEOGRO* v2;
    tGameObjectVisualObject* v3;
    tGameObjectExtended* v4;
    tModifier* i;
    int v6;

    v1 = a1->basic;
    v2 = 0;
    if (v1)
    {
        v3 = v1->actorObjectOrVisualObject;
        if (v3)
            v2 = (tGEOGRO*)(v3->GROGEOOrSomethingElse);
    }
    if ((a1->flags & 0x2000) != 0)
    {
        v4 = a1->extended;
        if (v4)
        {
            for (i = v4->mdf; i; i = i->nextElem)
            {
                v6 = i->modifierFlags;
                if ((v6 & 0xA000000) == 0 && (v6 & 0x1000000) != 0)
                {
                    gpst_DX_stDD->currTargetGao = a1;
                    i->modifierIntf->UnApply(i, v2);
                }
            }
        }
    }
}

void MDF_Modifier_Init()
{
    MDF_gast_ModifierInterface[0].mdfId = 0;
    MDF_gast_ModifierInterface[0].Create = GEO_ModifierSnap_Create;
    MDF_gast_ModifierInterface[0].Destroy = GEO_ModifierSnap_Destroy;
    MDF_gast_ModifierInterface[0].Apply = GEO_ModifierSnap_Apply;
    MDF_gast_ModifierInterface[0].UnApply = GEO_ModifierSnap_Unapply;
    MDF_gast_ModifierInterface[0].Load = GEO_ul_ModifierSnap_Load;
    MDF_gast_ModifierInterface[0].ReInit = NULL;
    MDF_gast_ModifierInterface[1].mdfId = 1;
    MDF_gast_ModifierInterface[1].Create = GEO_ModifierOnduleTonCorps_Create;
    MDF_gast_ModifierInterface[1].Destroy = GEO_ModifierOnduleTonCorps_Destroy;
    MDF_gast_ModifierInterface[1].Apply = GEO_ModifierOnduleTonCorps_Apply;
    MDF_gast_ModifierInterface[1].UnApply = GEO_ModifierOnduleTonCorps_UnApply;
    MDF_gast_ModifierInterface[1].Load = GEO_ul_ModifierOnduleTonCorps_Load;
    MDF_gast_ModifierInterface[1].ReInit = NULL;
    
    MDF_gast_ModifierInterface[2].mdfId = 2;
    MDF_gast_ModifierInterface[2].Create = GAO_ModifierExplode_Create;
    MDF_gast_ModifierInterface[2].Destroy = GAO_ModifierExplode_Destroy;
    MDF_gast_ModifierInterface[2].Apply =  GAO_ModifierExplode_Apply;
    MDF_gast_ModifierInterface[2].UnApply = GAO_ModifierExplode_Unapply;
    MDF_gast_ModifierInterface[2].Load = GAO_ModifierExplode_Load;
    MDF_gast_ModifierInterface[2].ReInit = GAO_ModifierExplode_Reinit;

    MDF_gast_ModifierInterface[3].mdfId = 3;
    MDF_gast_ModifierInterface[3].Create = GAO_ModifierLegLink_Create;
    MDF_gast_ModifierInterface[3].Destroy = GAO_ModifierLegLink_Destroy;
    MDF_gast_ModifierInterface[3].Apply = GAO_ModifierLegLink_Apply;
    MDF_gast_ModifierInterface[3].UnApply = GAO_ModifierLegLink_UnApply;
    MDF_gast_ModifierInterface[3].Load = GAO_ModifierLegLink_Load;
    MDF_gast_ModifierInterface[3].ReInit = GAO_ModifierLegLink_Reinit;
    
    MDF_gast_ModifierInterface[4].mdfId = 4;
    MDF_gast_ModifierInterface[4].Create = GEO_ModifierMorphing_Create;
    MDF_gast_ModifierInterface[4].Destroy = GEO_ModifierMorphing_Destroy;
    MDF_gast_ModifierInterface[4].Apply = GEO_ModifierMorphing_Apply;
    MDF_gast_ModifierInterface[4].UnApply = GEO_ModifierMorphing_UnApply;
    MDF_gast_ModifierInterface[4].Load = GEO_ul_ModifierMorphing_Load;
    MDF_gast_ModifierInterface[4].ReInit = NULL;
    
    MDF_gast_ModifierInterface[5].mdfId = 5;
    MDF_gast_ModifierInterface[5].Create = GAO_ModifierSemiLookAt_Create;
    MDF_gast_ModifierInterface[5].Destroy = GAO_ModifierSemiLookAt_Destroy;
    MDF_gast_ModifierInterface[5].Apply = GAO_ModifierSemiLookAt_Apply;
    MDF_gast_ModifierInterface[5].UnApply = GAO_ModifierSemiLookAt_Unapply;
    MDF_gast_ModifierInterface[5].Load = GAO_ModifierSemiLookAt_Load;
    MDF_gast_ModifierInterface[5].ReInit = GAO_ModifierSemiLookAt_Reinit;
    
    MDF_gast_ModifierInterface[6].mdfId = 6;
    MDF_gast_ModifierInterface[6].Create = GAO_ModifierShadow_Create;
    MDF_gast_ModifierInterface[6].Destroy = GAO_ModifierShadow_Destroy;
    MDF_gast_ModifierInterface[6].Apply = GAO_ModifierShadow_Apply;
    MDF_gast_ModifierInterface[6].UnApply = GAO_ModifierShadow_Unapply;
    MDF_gast_ModifierInterface[6].Load = GAO_ModifierShadow_Load;
    MDF_gast_ModifierInterface[6].ReInit = GAO_ModifierShadow_Reinit;
    MDF_gast_ModifierInterface[7].mdfId = 7;
    
    MDF_gast_ModifierInterface[7].Create = GAO_ModifierSpecialLookAt_Create;
    MDF_gast_ModifierInterface[7].Destroy = GAO_ModifierSpecialLookAt_Destroy;
    MDF_gast_ModifierInterface[7].Apply = GAO_ModifierSpecialLookAt_Apply;
    MDF_gast_ModifierInterface[7].UnApply = GAO_ModifierSpecialLookAt_Unapply;
    MDF_gast_ModifierInterface[7].Load = GAO_ModifierSpecialLookAt_Load;
    MDF_gast_ModifierInterface[7].ReInit = GAO_ModifierSpecialLookAt_Reinit;

    MDF_gast_ModifierInterface[8].mdfId = 8;
    MDF_gast_ModifierInterface[8].Create = GEN_ModifierSound_Create;
    MDF_gast_ModifierInterface[8].Destroy = GEN_ModifierSound_Destroy;
    MDF_gast_ModifierInterface[8].Apply = GEN_ModifierSound_Apply;
    MDF_gast_ModifierInterface[8].UnApply = GEN_ModifierSound_Unapply;
    MDF_gast_ModifierInterface[8].Load = GEN_ModifierSound_Load;
    MDF_gast_ModifierInterface[8].ReInit = GEN_ModifierSound_Reinit;

    MDF_gast_ModifierInterface[9].mdfId = 9;
    MDF_gast_ModifierInterface[9].Create = GAO_ModifierXMEN_Create;
    MDF_gast_ModifierInterface[9].Destroy = GAO_ModifierXMEN_Destroy;
    MDF_gast_ModifierInterface[9].Apply = GAO_ModifierXMEN_Apply;
    MDF_gast_ModifierInterface[9].UnApply = GAO_ModifierXMEN_Unapply;
    MDF_gast_ModifierInterface[9].Load = GAO_ModifierXMEN_Load;
    MDF_gast_ModifierInterface[9].ReInit = GAO_ModifierXMEN_Reinit;

    MDF_gast_ModifierInterface[10].mdfId = 10;
    MDF_gast_ModifierInterface[10].Create = GAO_ModifierXMEC_Create;
    MDF_gast_ModifierInterface[10].Destroy = GAO_ModifierXMEC_Destroy;
    MDF_gast_ModifierInterface[10].Apply = GAO_ModifierXMEC_Apply;
    MDF_gast_ModifierInterface[10].UnApply = GAO_ModifierXMEC_Unapply;
    MDF_gast_ModifierInterface[10].Load = GAO_ModifierXMEC_Load;
    MDF_gast_ModifierInterface[10].ReInit = GAO_ModifierXMEC_Reinit;

    MDF_gast_ModifierInterface[11].mdfId = 11;
    MDF_gast_ModifierInterface[11].Create = SPG_Modifier_Create;
    MDF_gast_ModifierInterface[11].Destroy = SPG_Modifier_Destroy;
    MDF_gast_ModifierInterface[11].Apply = SPG_Modifier_Apply;
    MDF_gast_ModifierInterface[11].UnApply = SPG_Modifier_Unapply;
    MDF_gast_ModifierInterface[11].Load = SPG_ul_Modifier_Load;
    MDF_gast_ModifierInterface[11].ReInit = NULL;

    MDF_gast_ModifierInterface[12].mdfId = 12;
    MDF_gast_ModifierInterface[12].Create = GEO_ModifierSymetrie_Create;
    MDF_gast_ModifierInterface[12].Destroy = GEO_ModifierSymetrie_Destroy;
    MDF_gast_ModifierInterface[12].Apply = GEO_ModifierSymetrie_Apply;
    MDF_gast_ModifierInterface[12].UnApply = GEO_ModifierSymetrie_Unapply;
    MDF_gast_ModifierInterface[12].Load = GEO_ul_ModifierSymetrie_Load;
    MDF_gast_ModifierInterface[12].ReInit = NULL;
    
    MDF_gast_ModifierInterface[13].mdfId = 13;
    MDF_gast_ModifierInterface[13].Create = GAO_ModifierROTR_Create;
    MDF_gast_ModifierInterface[13].Destroy = GAO_ModifierROTR_Destroy;
    MDF_gast_ModifierInterface[13].Apply = GAO_ModifierROTR_Apply;
    MDF_gast_ModifierInterface[13].UnApply = GAO_ModifierROTR_Unapply;
    MDF_gast_ModifierInterface[13].Load = GAO_ModifierROTR_Load;
    MDF_gast_ModifierInterface[13].ReInit = GAO_ModifierROTR_Reinit;
    
    MDF_gast_ModifierInterface[14].mdfId = 14;
    MDF_gast_ModifierInterface[14].Create = GAO_ModifierSNAKE_Create;
    MDF_gast_ModifierInterface[14].Destroy = GAO_ModifierSNAKE_Destroy;
    MDF_gast_ModifierInterface[14].Apply = GAO_ModifierSNAKE_Apply;
    MDF_gast_ModifierInterface[14].UnApply = GAO_ModifierSNAKE_Unapply;
    MDF_gast_ModifierInterface[14].Load = GAO_ModifierSNAKE_Load;
    MDF_gast_ModifierInterface[14].ReInit = GAO_ModifierSNAKE_Reinit;

    MDF_gast_ModifierInterface[15].mdfId = 15;
    MDF_gast_ModifierInterface[15].Create = GEN_ModifierSoundFx_Create;
    MDF_gast_ModifierInterface[15].Destroy = GEN_ModifierSoundFx_Destroy;
    MDF_gast_ModifierInterface[15].Apply = GEN_ModifierSoundFx_Apply;
    MDF_gast_ModifierInterface[15].UnApply = GEN_ModifierSoundFx_Unapply;
    MDF_gast_ModifierInterface[15].Load = GEN_ModifierSoundFx_Load;
    MDF_gast_ModifierInterface[15].ReInit = GEN_ModifierSoundFx_Reinit;

    MDF_gast_ModifierInterface[16].mdfId = 16;
    MDF_gast_ModifierInterface[16].Create = PROTEX_Modifier_Create;
    MDF_gast_ModifierInterface[16].Destroy = PROTEX_Modifier_Destroy;
    MDF_gast_ModifierInterface[16].Apply = PROTEX_Modifier_Apply;
    MDF_gast_ModifierInterface[16].UnApply = PROTEX_Modifier_Unapply;
    MDF_gast_ModifierInterface[16].Load = PROTEX_ul_Modifier_Load;
    MDF_gast_ModifierInterface[16].ReInit = PROTEX_Modifier_Reinit;

    MDF_gast_ModifierInterface[17].mdfId = 17;
    MDF_gast_ModifierInterface[17].Create = NULL;
    MDF_gast_ModifierInterface[17].Destroy = NULL;
    MDF_gast_ModifierInterface[17].Load = NULL;
    MDF_gast_ModifierInterface[17].Apply = NULL;
    MDF_gast_ModifierInterface[17].UnApply = NULL;
    MDF_gast_ModifierInterface[17].ReInit = NULL;

    MDF_gast_ModifierInterface[18].mdfId = 18;
    MDF_gast_ModifierInterface[18].Create = MPAG_Modifier_Create;
    MDF_gast_ModifierInterface[18].Destroy = MPAG_Modifier_Destroy;
    MDF_gast_ModifierInterface[18].Apply = MPAG_Modifier_Apply;
    MDF_gast_ModifierInterface[18].UnApply = MPAG_Modifier_Unapply;
    MDF_gast_ModifierInterface[18].Load = MPAG_ul_Modifier_Load;
    MDF_gast_ModifierInterface[18].ReInit = MPAG_ul_Modifier_Reinit;

    MDF_gast_ModifierInterface[19].mdfId = 19;
    MDF_gast_ModifierInterface[19].Create = MDF_LoadingSound_Create;
    MDF_gast_ModifierInterface[19].Destroy = MDF_LoadingSound_Destroy;
    MDF_gast_ModifierInterface[19].Apply = MDF_LoadingSound_Apply;
    MDF_gast_ModifierInterface[19].UnApply = MDF_LoadingSound_Unapply;
    MDF_gast_ModifierInterface[19].Load = MDF_LoadingSound_Load;
    MDF_gast_ModifierInterface[19].ReInit = MDF_LoadingSound_Reinit;

    MDF_gast_ModifierInterface[20].mdfId = 20;
    MDF_gast_ModifierInterface[20].Create = GAO_ModifierPhoto_Create;
    MDF_gast_ModifierInterface[20].Destroy = GAO_ModifierPhoto_Destroy;
    MDF_gast_ModifierInterface[20].Apply = GAO_ModifierPhoto_Apply;
    MDF_gast_ModifierInterface[20].UnApply = GAO_ModifierPhoto_Unapply;
    MDF_gast_ModifierInterface[20].Load = GAO_ModifierPhoto_Load;
    MDF_gast_ModifierInterface[20].ReInit = GAO_ModifierPhoto_Reinit;

    MDF_gast_ModifierInterface[21].mdfId = 21;
    MDF_gast_ModifierInterface[21].Create = GEO_ModifierSTP_Create;
    MDF_gast_ModifierInterface[21].Destroy = GEO_ModifierSTP_Destroy;
    MDF_gast_ModifierInterface[21].Apply = GEO_ModifierSTP_Apply;
    MDF_gast_ModifierInterface[21].UnApply = GEO_ModifierSTP_Unapply;
    MDF_gast_ModifierInterface[21].Load = GEO_ul_ModifierSTP_Load;
    MDF_gast_ModifierInterface[21].ReInit = NULL;

    MDF_gast_ModifierInterface[22].mdfId = 22;
    MDF_gast_ModifierInterface[22].Create = GEO_ModifierCrush_Create;
    MDF_gast_ModifierInterface[22].Destroy = GEO_ModifierCrush_Destroy;
    MDF_gast_ModifierInterface[22].Apply = GEO_ModifierCrush_Apply;
    MDF_gast_ModifierInterface[22].UnApply = GEO_ModifierCrush_Unapply;
    MDF_gast_ModifierInterface[22].Load = GEO_ul_ModifierCrush_Load;
    MDF_gast_ModifierInterface[22].ReInit = GEO_ModifierCrush_Reinit;

    MDF_gast_ModifierInterface[23].mdfId = 23;
    MDF_gast_ModifierInterface[23].Create = GEO_ModifierRLICarte_Create;
    MDF_gast_ModifierInterface[23].Destroy = GEO_ModifierRLICarte_Destroy;
    MDF_gast_ModifierInterface[23].Apply = GEO_ModifierRLICarte_Apply;
    MDF_gast_ModifierInterface[23].UnApply = GEO_ModifierRLICarte_Unapply;
    MDF_gast_ModifierInterface[23].Load = GEO_ul_ModifierRLICarte_Load;
    MDF_gast_ModifierInterface[23].ReInit = GEO_ModifierRLICarte_Reinit;

    MDF_gast_ModifierInterface[24].mdfId = 24;
    MDF_gast_ModifierInterface[24].Create = GAO_ModifierLazy_Create;
    MDF_gast_ModifierInterface[24].Destroy = GAO_ModifierLazy_Destroy;
    MDF_gast_ModifierInterface[24].Apply = GAO_ModifierLazy_Apply;
    MDF_gast_ModifierInterface[24].UnApply = GAO_ModifierLazy_Unapply;
    MDF_gast_ModifierInterface[24].Load = GAO_ul_ModifierLazy_Load;
    MDF_gast_ModifierInterface[24].ReInit = GAO_ModifierLazy_Reinit;
}