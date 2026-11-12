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

void MDF_Reinit(struct sGameObjectMain* gao)
{
    tModifier* mdf;
    void (*reinit)(struct sModifier*);

    if ((gao->flags & 0x2000) == 0)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        reinit = mdf->modifierIntf->ReInit;
        if (reinit)
            reinit(mdf);
    }
}

void MDF_ApplyGen(tGameObjectMain* gao)
{
    tModifier* mdf;
    int flags;

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        flags = mdf->modifierFlags;
        if ((flags & 0x2000000) == 0 && (flags & 0x4000000) != 0)
            mdf->modifierIntf->Apply(mdf, gao);
    }
}

void MDF_MainApplyGen(tableDescStruct* table)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;

    last = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
    for (elem = table->firstElemPtr; elem <= last; ++elem)
    {
        if (elem->objectPtr && ((uintptr_t)elem->objectPtr & 3) != 1)
            MDF_ApplyGen((tGameObjectMain*)(elem->objectPtr));
    }
}

void MDF_UnApplyGen(tGameObjectMain* gao)
{
    tModifier* mdf;
    int flags;

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        flags = mdf->modifierFlags;
        if ((flags & 0x2000000) == 0 && (flags & 0x4000000) != 0)
            mdf->modifierIntf->UnApply(mdf, gao);
    }
}

void MDF_MainUnApplyGen(tableDescStruct* table)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;

    last = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
    for (elem = table->firstElemPtr; elem <= last; ++elem)
    {
        if (elem->objectPtr && ((uintptr_t)elem->objectPtr & 3) != 1)
            MDF_UnApplyGen((tGameObjectMain*)elem->objectPtr);
    }
}

void MDF_Modifier_Destroy(tModifier* mdf)
{
    mdf->modifierIntf->Destroy(mdf);
    MEM_Free(mdf);
}

struct sModifier* MDF_pst_Modifier_Create(struct sGameObjectMain* gao, int idx, void* privateDataPtr)
{
    tModifier* mdf;

    mdf = (tModifier*)MEM_p_Alloc(sizeof(tModifier));
    mdf->nextElem = 0;
    mdf->prevElem = 0;
    mdf->modifierFlags = 0;
    mdf->group = 0;
    mdf->modifierIntf = &MDF_gast_ModifierInterface[idx];
    mdf->gao = gao;
    mdf->modifierIntf->Create(gao, mdf, privateDataPtr);
    return mdf;
}

int MDF_ul_Modifier_Load(struct sModifier** slot, char* currReaderPtr, int chainLoad,
                         struct sGameObjectMain* gao)
{
    char* start;
    char* peek;
    tModifier* mdf;
    int nextId;
    int idx;

    start = currReaderPtr;
    MDF_gpst_GlobalCreate = 0;
    idx = LOA_ReadLong(&currReaderPtr, 0, 1);
    mdf = MDF_pst_Modifier_Create(gao, idx, 0);
    *slot = mdf;
    mdf->gao = gao;
    mdf->modifierFlags = LOA_ReadLong(&currReaderPtr, 0, 1);
    currReaderPtr += mdf->modifierIntf->Load(mdf, currReaderPtr);

    peek = currReaderPtr;
    LOA_ReadLong(&peek, &nextId, 4);
    if (chainLoad && nextId != -1)
    {
        currReaderPtr += MDF_ul_Modifier_Load(&mdf->nextElem, currReaderPtr, 1, gao);
        if (mdf->nextElem)
            mdf->nextElem->prevElem = mdf;
    }
    return currReaderPtr - start;
}

struct sModifier* MDF_pst_GetByType(struct sGameObjectMain* gao, int mdfId)
{
    tModifier* mdf;

    if (!gao || !gao->extended)
        return NULL;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == mdfId)
            return mdf;
    }
    return NULL;
}

void MDF_Modifier_AddToGameObject(tGameObjectMain* gao, tModifier* mdf)
{
    tModifier* last;

    mdf->gao = gao;
    last = gao->extended->mdf;
    if (!last)
    {
        gao->extended->mdf = mdf;
        return;
    }

    while (last->nextElem)
        last = last->nextElem;
    mdf->prevElem = last;
    last->nextElem = mdf;
}

void MDF_Modifier_DuplicateList(struct sGameObjectMain* gao, struct sModifier* src)
{
    tModifier* mdf;
    tModifier* copy;

    MDF_gpst_GlobalCreate = 0;
    if (!gao || !gao->extended)
        return;

    if (!src)
    {
        gao->extended->mdf = 0;
        return;
    }

    for (mdf = src; mdf; mdf = mdf->nextElem)
    {
        MDF_gpst_GlobalCreate = mdf;
        copy = MDF_pst_Modifier_Create(gao, mdf->modifierIntf->mdfId, mdf->mdfPrivateDataPtr);
        MDF_Modifier_AddToGameObject(gao, copy);
    }
}

static void MDF_l_SetGroupAndReinit(tGameObjectMain* gao, struct sGroup* grp)
{
    tModifier* mdf;
    void (*reinit)(tModifier*);

    if (!gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        mdf->group = grp;
        reinit = mdf->modifierIntf->ReInit;
        if (reinit)
            reinit(mdf);
    }
}

void UpdateModifiersGroup(struct sGameObjectMain* gao, struct sGroup* grp)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* member;

    MDF_l_SetGroupAndReinit(gao, grp);

    last = ((tableElemStructPF*)(grp->tablePF->nextElemPtr)) - 1;
    for (elem = grp->tablePF->firstElemPtr; elem <= last; ++elem)
    {
        member = (tGameObjectMain*)elem->objectPtr;
        if (member && ((uintptr_t)member & 3) != 1)
            MDF_l_SetGroupAndReinit(member, grp);
    }
}

struct sGroup* MDF_Modifier_GetCurGroup(struct sModifier* mdf)
{
    tGroup* grp;
    tGameObjectExtended* extended;

    grp = mdf->group;
    if (!grp)
    {
        extended = mdf->gao->extended;
        if (extended)
            grp = extended->grp;
    }
    return grp;
}

void MDF_ApplyAll(struct sGameObjectMain* gao)
{
    tGroGeneric* gro;
    tGameObjectVisualObject* visual;
    tModifier* mdf;

    gro = 0;
    if (gao->basic)
    {
        visual = gao->basic->actorObjectOrVisualObject;
        if (visual)
            gro = visual->GROGEOOrSomethingElse;
    }

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if ((mdf->modifierFlags & 0xF000000) == 0)
            mdf->modifierIntf->Apply(mdf, (void*)gro);
    }
}

void MDF_UnApplyAll(struct sGameObjectMain* gao)
{
    tGroGeneric* gro;
    tGameObjectVisualObject* visual;
    tModifier* mdf;

    gro = NULL;
    if (gao->basic)
    {
        visual = gao->basic->actorObjectOrVisualObject;
        if (visual)
            gro = visual->GROGEOOrSomethingElse;
    }

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if ((mdf->modifierFlags & 0xF000000) == 0)
            mdf->modifierIntf->UnApply(mdf, (void*)gro);
    }
}

void MDF_ApplyAllGao(struct sGameObjectMain* gao)
{
    tGEOGRO* geo;
    tGameObjectVisualObject* visual;
    tModifier* mdf;
    int flags;

    geo = 0;
    if ((gao->flags & 0x4000) != 0)
    {
        visual = gao->basic->actorObjectOrVisualObject;
        geo = (tGEOGRO*)(visual->GROGEOOrSomethingElse);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent =
            gpst_DX_stDD->drawMaskAllowedStuff & visual->objDrawMask;
    }
    else
    {
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = gpst_DX_stDD->drawMaskAllowedStuff;
    }

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        flags = mdf->modifierFlags;
        if ((flags & 0xA000000) == 0 && (flags & 0x1000000) != 0)
        {
            gpst_DX_stDD->currTargetGao = gao;
            mdf->modifierIntf->Apply(mdf, geo);
        }
    }
}

void MDF_UnApplyAllGao(struct sGameObjectMain* gao)
{
    tGEOGRO* geo;
    tGameObjectVisualObject* visual;
    tModifier* mdf;
    int flags;

    geo = 0;
    if (gao->basic)
    {
        visual = gao->basic->actorObjectOrVisualObject;
        if (visual)
            geo = (tGEOGRO*)(visual->GROGEOOrSomethingElse);
    }

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        flags = mdf->modifierFlags;
        if ((flags & 0xA000000) == 0 && (flags & 0x1000000) != 0)
        {
            gpst_DX_stDD->currTargetGao = gao;
            mdf->modifierIntf->UnApply(mdf, geo);
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