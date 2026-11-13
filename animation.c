#include "gameobject.h"
#include "memory.h"
#include "loading.h"
#include "events.h"
#include "math.h"
#include "time.h"
#include "gameobject.h"
#include "events.h"
#include "bigfiles.h"
#include "dbg.h"
#include "animation.h"


uint8_t ANI_c_DefaultBitFieldLOD = 0x80;

typedef int (*tANITrackRunner)(tEventData*, float*);

// An animated game object keeps its animation state in the actor behind it.
static tGameObjectActorObject* ANI_l_Actor(tGameObjectMain* gao)
{
    return (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
}

static tGameObjectActorObject* ANI_l_ActorOrNull(tGameObjectMain* gao)
{
    if (!gao || !gao->basic || !gao->basic->actorObjectOrVisualObject)
        return NULL;
    return ANI_l_Actor(gao);
}

static tGameObjectActorObject* ANI_l_AnimatedActor(tGameObjectMain* gao)
{
    if (!gao || (gao->flags & 2) == 0)
        return NULL;
    return ANI_l_ActorOrNull(gao);
}

// The matrix the object's own position lives in: the local one while it
// hangs in a hierarchy, the absolute one otherwise.
static tGaoMatrix* ANI_l_PosMatrix(tGameObjectMain* gao)
{
    if ((gao->flags & 0x400000) != 0)
        return gao->basic->mtx;
    return (tGaoMatrix*)gao->matrices;
}

static tGameObjectMain* ANI_l_RefGao(tGameObjectMain* gao)
{
    if (!gao || !gao->basic || !gao->basic->mtx)
        return NULL;
    return gao->basic->mtx->refGao2;
}

static tableElemStructPF* ANI_l_LastElem(tableDescStruct* tab)
{
    return ((tableElemStructPF*)(tab->nextElemPtr)) - 1;
}

// Only the magic box tracks are wanted; returns the track count to put back.
static uint16_t ANI_l_LimitTracksToMagicBox()
{
    uint16_t num;

    num = EVE_gpst_CurrentListTracks->num;
    EVE_gpst_CurrentListTracks->num = (EVE_gpst_CurrentListTracks->listElems[1].boneID == 0xFFFF) + 1;
    return num;
}

static tANITrackRunner ANI_l_TrackRunner(tGameObjectMain* gao)
{
    tGameObjectActorObject* actor;

    if (TIM_gf_dt <= 0.0 || ENG_gb_EVERunning)
        return EVE_b_RunAllTracks;
    actor = ANI_l_ActorOrNull(gao);
    if (!actor || !actor->skeletonGroup)
        return EVE_b_RunAllTracks;
    return EVE_b_RunAnimation;
}

static void ANI_l_InitTracksTime(tGameObjectActorObject* actor, float* fullTime)
{
    tEventData* data;
    float currTime;

    data = actor->animPtr[0]->ptr;
    EVE_GetTracksTime(data, 0, &currTime, fullTime);
    EVE_SetTracksTime(data, currTime);
    if (actor->flagsEgBlendIw2 != 1)
        actor->flagsEgBlendIw2 = 1;
    actor->initedInANIMainCall = 1;
}

tGameObjectActorObject* ANI_pst_CreateGameObjectAnim()
{
    tGameObjectActorObject* gaoao = (tGameObjectActorObject*)MEM_p_Alloc(sizeof(tGameObjectActorObject));;

    LOA_ul_FileTypeSize_37 += sizeof(tGameObjectActorObject);
    memset(gaoao, 0, sizeof(tGameObjectActorObject));

    gaoao->transitionElem = 0;
    gaoao->lod = 0;
    gaoao->ANILOD = 0;
    gaoao->aniPelvisOffset = 0;
    gaoao->currentAction = -1;
    gaoao->actionItem = -1;
    gaoao->aniBitFieldLOD = ANI_c_DefaultBitFieldLOD;
    return gaoao;
}

void ANI_LimitFrequencyBetween0And255(tAnim* anim, Vec3* step, Vec3* speed)
{
    float stepLenSq, ratio;


    stepLenSq = MATH_VecLenSquared(step);
    if (stepLenSq == 0.0f)
    {
        anim->frequency = (uint8_t)1.0f;
    }
    else
    {
        ratio = (float)sqrt(MATH_VecLenSquared(speed) / stepLenSq);

        if (ratio >= 0.0f)
        {
            if (ratio > 255.0f)
                ratio = 255.0f;
            anim->frequency = (uint8_t)ratio;
        }
        else
        {
            anim->frequency = (uint8_t)0.0f;
        }
    }
}

void ANI_ClearBlendingFlag(tGameObjectMain* gao)
{
    tGameObjectActorObject* actor;

    actor = ANI_l_AnimatedActor(gao);
    if (actor)
        actor->flagsEgBlendIw2 &= ~0x2;
}

void ANI_DestroyAnimation(tAnim* anim)
{
    if (!anim) return;

    EVE_DeleteListTracks(anim->ptr->eventListStruct);

    if (anim->ptr->tracks)
        MEM_Free(anim->ptr->tracks);

    MEM_Free(anim->ptr);
    MEM_Free(anim);
}

void ANI_DestroySkeleton(tGroup* grp)
{
    tableElemStructPF* currElem;
    tGameObjectMain* gao;

    if (grp->groupElemNum-- == 1)
    {
        for (currElem = grp->tablePF->firstElemPtr; currElem <= ANI_l_LastElem(grp->tablePF); currElem++)
        {
            gao = (tGameObjectMain*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(gao))
            {
                if ((gao->flags & 1) == 0)
                    break;

                WOR_World_DetachObject(WOR_gpst_CurrentWorld, (tGameObjectMain*)currElem->objectPtr);
                OBJ_GameObject_Remove(gao, TRUE);
            }
        }
        OBJ_FreeGroup(grp);
    }
}

void ANI_Free(tGameObjectActorObject* gaoao)
{
    int i;

    if (gaoao->actionKit)
    {
        ACT_FreeActionKit(&gaoao->actionKit);
        for (i = 0; i < 4; i++)
        {
            if (gaoao->animPtr[i] && gaoao->animPtr[i] != (tAnim*)-1)
            {
                if (gaoao->animPtr[i]->ptr->tracks)
                    MEM_Free(gaoao->animPtr[i]->ptr->tracks);
                MEM_Free(gaoao->animPtr[i]->ptr);
                MEM_Free(gaoao->animPtr[i]);
            }
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            if (gaoao->animPtr[i] && gaoao->animPtr[i] != (tAnim*)-1)
                ANI_DestroyAnimation(gaoao->animPtr[i]);
        }
    }
    if (gaoao->skeletonGroup)
    {
        if (gaoao->skeletonGroupToRestoreOnReinit)
            ANI_DestroySkeleton(gaoao->skeletonGroupToRestoreOnReinit);
        else
            ANI_DestroySkeleton(gaoao->skeletonGroup);
    }
    if (gaoao->group)
        ANI_DestroySkeleton(gaoao->group);

    if (gaoao->shape2)
    {
        gaoao->shape2->usageCount--;

        if (!gaoao->shape2->usageCount)
        {
            MEM_Free(gaoao->shape2);
            LOA_DeleteAddress((int)gaoao->shape2);
            gaoao->shape2 = NULL;
        }
    }
    MEM_Free(gaoao);
}

struct sAnim* ANI_AllocAnim()
{
    tAnim* anim;

    anim = (tAnim*)MEM_p_AllocAligned(sizeof(tAnim), 16);
    memset(anim, 0, sizeof(tAnim));

    anim->ptr = (tEventData*)MEM_p_AllocAligned(sizeof(tEventData), 16);;
    anim->ptr->eventListStruct = NULL;
    anim->ptr->tracks = NULL;

    return anim;
}

void ANI_ApplyHierarchyOnFlash(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* gaoao;
    Mtx4x4 dest, refInv;

    if ((gao->flags & 2) != 0 && (gao->flags & 0x400000) != 0)
    {
        gaoao = gao->basic->actorObjectOrVisualObject;
        if (gaoao->animPtr[0])
        {
            MATH_InvertMatrix(&refInv, OBJ_pst_GetAbsoluteMatrix(gao->basic->mtx->refGao2));

            if ((gao->flags & 0x4000000) != 0)
            {
                MATH_MulMatrixMatrix(&dest, OBJ_pst_GetFlashMatrix(gao), &refInv);
                MATH_CopyMatrix(OBJ_pst_GetFlashMatrix(gao), &dest);
            }

            if (ANI_GaoAnimationIsTransitioning(gao))
            {
                MATH_MulMatrixMatrix(&dest, &gaoao->animPtr[1]->flashMtx, &refInv);
                MATH_CopyMatrix(&gaoao->animPtr[1]->flashMtx, &dest);
            }
        }
    }
}

int ANI_GaoAnimationIsTransitioning(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor;

    actor = ANI_l_AnimatedActor(gao);
    if (!actor)
        return 0;
    return actor->flagsEgBlendIw2 & 2;
}

void ANI_RecomputeFlash(tGameObjectMain* gao, tAnim* anim)
{
    tGaoMatrix* posMtx;
    Mtx4x4* baseMtx;
    uint32_t mtxflag;
    uint16_t trackNum;
    float savedDt;
    Mtx4x4 animMtx;
    Mtx4x4 savedMtx;
    Mtx4x4 invAnimMtx;

    if (!gao)
        return;

    if ((gao->flags & 0x400000) != 0)
    {
        MATH_CopyMatrix(&savedMtx, &gao->basic->mtx->mtx);
        baseMtx = &savedMtx;
    }
    else
    {
        baseMtx = gao->matrices;
        MATH_CopyMatrix(&animMtx, baseMtx);
        gao->matrices = &animMtx;
    }
    EVE_gpst_OwnerGAO = gao;
    EVE_gpst_CurrentData = anim->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    EVE_gpst_CurrentRefMatrix = &MATH_gst_IdentityMatrix;
    ENG_gb_EVERunning = 0;
    posMtx = ANI_l_PosMatrix(gao);
    mtxflag = posMtx->mtx.mtxflag;
    posMtx->mtx._m14 = 0.0;
    posMtx->mtx._m24 = 0.0;
    posMtx->mtx._m34 = 0.0;
    posMtx->mtx.mtxflag = mtxflag & ~8u;
    if (EVE_gpst_CurrentListTracks)
    {
        trackNum = ANI_l_LimitTracksToMagicBox();
        savedDt = TIM_gf_dt;
        TIM_gf_dt = 0.0;
        EVE_gb_CanFlash = 0;
        EVE_b_RunAllTracks(anim->ptr, 0);
        EVE_gb_CanFlash = 1;
        TIM_gf_dt = savedDt;
        EVE_gpst_CurrentListTracks->num = trackNum;
    }
    if ((gao->flags & 0x400000) != 0)
    {
        MATH_CopyMatrix(&animMtx, &gao->basic->mtx->mtx);
        MATH_CopyMatrix(&gao->basic->mtx->mtx, &savedMtx);
    }
    else
    {
        gao->matrices = baseMtx;
    }
    MATH_InvertMatrix(&invAnimMtx, &animMtx);
    if (anim == ANI_l_Actor(gao)->animPtr[0])
        MATH_MulMatrixMatrix(OBJ_pst_GetFlashMatrix(gao), &invAnimMtx, baseMtx);
    else
        MATH_MulMatrixMatrix(&anim->flashMtx, &invAnimMtx, baseMtx);
}

void ANI_UpdateFlash(struct sGameObjectMain* gao, char which)
{
    tGameObjectActorObject* actor;
    tGameObjectEventsListElem* savedTrack;
    tAnimTracks* savedParam;
    Mtx4x4* savedRefMtx;
    tGameObjectEventElem* savedEvent;
    tGameObjectEventsList* savedListTracks;
    tGameObjectMain* savedOwner;
    tEventData* savedData;
    int savedRunning;

    if (!gao || (gao->flags & 0x4000000) == 0)
        return;
    actor = ANI_l_AnimatedActor(gao);
    if (!actor || !actor->animPtr[0])
        return;

    savedTrack = EVE_gpst_CurrentTrack;
    savedParam = EVE_gpst_CurrentParam;
    savedRefMtx = EVE_gpst_CurrentRefMatrix;
    savedEvent = EVE_gpst_CurrentEvent;
    savedListTracks = EVE_gpst_CurrentListTracks;
    savedOwner = EVE_gpst_OwnerGAO;
    savedRunning = ENG_gb_EVERunning;
    savedData = EVE_gpst_CurrentData;

    if ((which & 1) != 0)
        ANI_RecomputeFlash(gao, actor->animPtr[0]);
    if (ANI_GaoAnimationIsTransitioning(gao) && (which & 2) != 0)
        ANI_RecomputeFlash(gao, actor->animPtr[1]);

    EVE_gpst_CurrentData = savedData;
    EVE_gpst_OwnerGAO = savedOwner;
    EVE_gpst_CurrentListTracks = savedListTracks;
    EVE_gpst_CurrentTrack = savedTrack;
    EVE_gpst_CurrentRefMatrix = savedRefMtx;
    EVE_gpst_CurrentParam = savedParam;
    EVE_gpst_CurrentEvent = savedEvent;
    ENG_gb_EVERunning = savedRunning;
}

uint8_t ANI_uc_GetEngineCanalByAICanal(struct sGameObjectMain* gao, char aiCanal)
{
    tGameObjectActorObject* actor;
    tableDescStruct* tab;
    tShape* shape;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    uint8_t idx;

    if ((gao->flags & 2) == 0)
        return 0;
    actor = ANI_l_Actor(gao);
    if (!actor->skeletonGroup)
        return 0;
    tab = actor->skeletonGroup->tablePF;
    if (!tab)
        return 0;
    shape = actor->shape;
    if (!shape)
        return 0;
    lastElem = ANI_l_LastElem(tab);
    idx = 0;
    for (elem = tab->firstElemPtr; elem <= lastElem; elem++)
    {
        if (shape->shapesOriginal[idx] == aiCanal)
            return idx;
        idx++;
    }
    return -1;
}

struct sGameObjectMain* ANI_pst_GetObjectByAICanal(struct sGameObjectMain* gao, uint8_t aiCanal)
{
    tGameObjectActorObject* actor;
    tableDescStruct* tab;
    tShape* shape;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    int idx;

    if ((gao->flags & 2) == 0)
        return 0;
    actor = ANI_l_Actor(gao);
    if (!actor->skeletonGroup)
        return 0;
    tab = actor->skeletonGroup->tablePF;
    if (!tab)
        return 0;
    shape = actor->shape;
    lastElem = ANI_l_LastElem(tab);
    idx = 0;
    for (elem = tab->firstElemPtr; elem <= lastElem; elem++, idx++)
    {
        if (!TAB_TableElemIsInvalid(elem->objectPtr)
            && (shape ? shape->shapesOriginal[idx] == aiCanal : idx == aiCanal))
            return (tGameObjectMain*)elem->objectPtr;
    }
    return 0;
}

// The bone drops its own geometry, material and modifiers and takes the
// source object's.
static void ANI_l_ReplaceBoneVisual(tGameObjectMain* gao, tGameObjectMain* bone,
    tGameObjectVisualObject* boneVis, tGameObjectMain* src, tGroup* grp)
{
    tGameObjectVisualObject* srcVis;
    tModifier* mdf;
    tModifier* nextMdf;
    tGroGeneric* gro;
    tMultiPartGameObjectLinks* link;
    unsigned int partNum;
    uint8_t part;

    if (bone->extended && bone->extended->mdf)
    {
        mdf = bone->extended->mdf;
        do
        {
            nextMdf = mdf->nextElem;
            MDF_Modifier_Destroy(mdf);
            mdf = nextMdf;
        } while (nextMdf);
        bone->extended->mdf = 0;
    }
    if (boneVis)
    {
        if (boneVis->GROGEOOrSomethingElse)
        {
            boneVis->GROGEOOrSomethingElse->intf->AddRef(
                boneVis->GROGEOOrSomethingElse,
                -1);
            boneVis->GROGEOOrSomethingElse = 0;
        }
        gro = boneVis->GROMAT;
        if (gro)
        {
            gro->intf->AddRef(gro, -1);
            boneVis->GROMAT = 0;
        }
    }
    bone->flags |= 0x4000u;
    if (src->basic && src->basic->multiPartData)
    {
        partNum = src->basic->multiPartData->elemNum;
        if (partNum)
        {
            OBJ_GameObject_AllocateGizmo(bone, partNum, 1);
            memcpy(
                bone->basic->multiPartData->multipartGaoObjectPartsOrMatrices,
                src->basic->multiPartData->multipartGaoObjectPartsOrMatrices,
                4 * ((sizeof(tMultiPartGameObjectLinks) * partNum) >> 2));
            part = 0;
            do
            {
                link = &((tMultiPartGameObjectLinks*)(bone->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[part];
                link->gao = gao;
                ++part;
                link->mtxPtr = &((Mtx4x4*)(gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[link->mtxIdx];
            } while (part < partNum);
        }
        else if (bone->basic->multiPartData)
        {
            if (bone->basic->multiPartData->multipartGaoObjectPartsOrMatrices)
            {
                MEM_Free(bone->basic->multiPartData->multipartGaoObjectPartsOrMatrices);
                bone->basic->multiPartData->multipartGaoObjectPartsOrMatrices = 0;
            }
            MEM_Free(bone->basic->multiPartData);
            bone->basic->multiPartData = 0;
            bone->flags &= ~0x1200000u;
        }
    }
    if (bone->basic && !bone->basic->actorObjectOrVisualObject)
        bone->flags &= ~0x4000u;
    OBJ_ChangeIdentityFlags(bone, bone->flags | 0x4000, bone->flags);
    if (src->basic && src->basic->actorObjectOrVisualObject)
    {
        srcVis = (tGameObjectVisualObject*)src->basic->actorObjectOrVisualObject;
        if (srcVis->GROGEOOrSomethingElse)
        {
            srcVis->GROGEOOrSomethingElse->intf->AddRef(srcVis->GROGEOOrSomethingElse, 1);
            ((tGameObjectVisualObject*)(bone->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = srcVis->GROGEOOrSomethingElse;
        }
        if (srcVis->GROMAT)
        {
            srcVis->GROMAT->intf->AddRef(srcVis->GROMAT, 1);
            ((tGameObjectVisualObject*)(bone->basic->actorObjectOrVisualObject))->GROMAT = srcVis->GROMAT;
        }
        ((tGameObjectVisualObject*)(bone->basic->actorObjectOrVisualObject))->objDrawMask = srcVis->objDrawMask;
    }
    if (src->extended && src->extended->mdf)
    {
        MDF_Modifier_DuplicateList(bone, src->extended->mdf);
        UpdateModifiersGroup(bone, grp);
    }
    bone->flags2 &= ~0x20200000u;
    OBJ_ComputeBV(bone, 0);
}

void ANI_ChangeBoneShape(struct sGameObjectMain* gao, uint8_t canal, uint8_t shapeIdx, int byAICanal)
{
    tGameObjectActorObject* actor;
    tGameObjectMain* bone;
    tGameObjectMain* src;
    tGameObjectVisualObject* boneVis;
    tGameObjectVisualObject* srcVis;
    tGroup* grp;
    tableDescStruct* tab;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    int* colors;
    int* newColors;
    unsigned int colorBytes;
    uint8_t engineCanal;
    uint8_t idx;

    if (shapeIdx == 0xFF)
        return;
    actor = ANI_l_ActorOrNull(gao);
    if (!actor)
        return;
    grp = actor->skeletonGroup;
    if (byAICanal)
    {
        bone = ANI_pst_GetObjectByAICanal(gao, canal);
        engineCanal = ANI_uc_GetEngineCanalByAICanal(gao, canal);
        if (engineCanal == 0xFF)
            return;
        actor->shapes[engineCanal] = shapeIdx;
    }
    else
    {
        tab = actor->skeletonGroup->tablePF;
        if (canal >= tab->actElemNum)
            return;
        bone = (tGameObjectMain*)((tableElemStructPF*)(tab->firstElemPtr))[canal].objectPtr;
    }
    if (TAB_TableElemIsInvalid(bone))
        return;
    if (shapeIdx == 0xFE)
    {
        bone->flags2 &= ~0x20200000u;
        return;
    }
    if (shapeIdx == 0xFD)
    {
        bone->flags2 |= 0x20200000u;
        return;
    }
    if ((bone->flags & 0x800000) == 0)
        return;
    boneVis = (tGameObjectVisualObject*)bone->basic->actorObjectOrVisualObject;
    if (!bone->extended || !bone->extended->grp)
        return;
    tab = bone->extended->grp->tablePF;
    lastElem = ANI_l_LastElem(tab);
    elem = tab->firstElemPtr;
    for (idx = 0; idx < shapeIdx; idx++)
    {
        elem++;
        if (elem > lastElem)
            return;
    }
    if (!elem->objectPtr)
        return;
    src = (tGameObjectMain*)elem->objectPtr;
    if (boneVis)
    {
        if (boneVis->RLIVertexColors)
        {
            MEM_Free(boneVis->RLIVertexColors);
            boneVis->RLIVertexColors = 0;
        }
        if (src->basic && src->basic->actorObjectOrVisualObject)
        {
            colors = ((tGameObjectVisualObject*)(src->basic->actorObjectOrVisualObject))->RLIVertexColors;
            if (colors)
            {
                colorBytes = 4 * *colors + 4;
                newColors = (int*)MEM_p_Alloc(colorBytes);
                boneVis->RLIVertexColors = newColors;
                memcpy(newColors, ((tGameObjectVisualObject*)(src->basic->actorObjectOrVisualObject))->RLIVertexColors, colorBytes);
            }
        }
        if (src->basic)
        {
            srcVis = (tGameObjectVisualObject*)src->basic->actorObjectOrVisualObject;
            if (srcVis
                && srcVis->GROMAT == boneVis->GROMAT
                && srcVis->GROGEOOrSomethingElse == boneVis->GROGEOOrSomethingElse
                && (bone->flags2 & 0x200000) == 0)
                return;
        }
    }
    ANI_l_ReplaceBoneVisual(gao, bone, boneVis, src, grp);
}

void ANI_ApplyShape(tGameObjectMain* gao)
{
    tGameObjectActorObject* actor;
    tableDescStruct* tab;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    uint8_t idx;

    actor = ANI_l_AnimatedActor(gao);
    if (!actor || !actor->skeletonGroup || !actor->shape)
        return;
    tab = actor->skeletonGroup->tablePF;
    lastElem = ANI_l_LastElem(tab);
    idx = 0;
    for (elem = tab->firstElemPtr; elem <= lastElem; elem++, idx++)
    {
        if (!TAB_TableElemIsInvalid(elem->objectPtr))
        {
            if (actor->shape == actor->shape2)
                ANI_ChangeBoneShape(gao, idx, actor->shapes[idx], 0);
            else
                ANI_ChangeBoneShape(gao, idx, actor->shape->shapes[idx], 0);
        }
    }
}

void ANI_ChangeShape(struct sGameObjectMain* gao, struct sShape* shape)
{
    tGameObjectActorObject* actor;
    tShape* oldShape;
    BOOL hadShape;
    int i;
    char savedOriginals[64];

    if ((gao->flags & 2) == 0 || !gao->basic || !gao->basic->actorObjectOrVisualObject)
        return;
    actor = ANI_l_Actor(gao);
    if (!shape)
    {
        shape = actor->shape2;
        if (!shape)
            return;
    }
    oldShape = actor->shape;
    if (oldShape == shape)
        return;
    hadShape = oldShape != 0;
    if (hadShape)
        memcpy(savedOriginals, oldShape->shapesOriginal, sizeof(savedOriginals));
    actor->shape = shape;
    if (hadShape)
    {
        for (i = 0; i < 64; i++)
        {
            if (actor->shape->shapesOriginal[i] == 0xFF)
                actor->shape->shapesOriginal[i] = savedOriginals[i];
        }
    }
    ANI_ApplyShape(gao);
}

void ANI_SetTrackList(struct sAnim* anim, struct sGameObjectEventsList* list)
{
    tAnimTracks* tracks;
    unsigned int i;

    if (!anim || !anim->ptr || !list)
        return;
    EVE_InitData(anim->ptr, list);
    tracks = anim->ptr->tracks;
    for (i = 0; i < list->num; ++i)
    {
        list->listElems[i].flags |= 3u;
        tracks[i].flag |= 1u;
        if (!i && list->listElems->trackElems)
            list->listElems->trackElems->flags2 |= 4u;
    }
}

void ANI_ChangeAction(struct sGameObjectMain* gao, struct sGameObjectActorObject* actor)
{
    tAction* action;
    tActionElem* actionElems;
    tActionElem* next;
    tAnim* anim;
    int actionItem;
    int nextItem;
    int elemNum;
    BOOL sameItem;
    uint8_t calls;
    uint8_t bits;
    int transition;

    action = actor->actionPtr;
    if (!action)
        return;
    actionItem = actor->actionItem;
    calls = actor->callCountForChangeAction + 1;
    actor->callCountForChangeAction = calls;
    actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));
    if (calls < actionElems[actionItem].changeActionCallAmountToChange)
        return;
    actor->callCountForChangeAction = 0;
    elemNum = action->actionElemNum;
    nextItem = actionItem + 1;
    if (nextItem >= elemNum)
    {
        if (actor->transitionElem)
        {
            transition = actor->currentActionTransition;
            actor->currentActionTransition = -1;
            ACT_SetAction(gao, transition, 0);
            return;
        }
        nextItem = action->actionElemIDForTransition;
        if (nextItem != 255 && nextItem >= elemNum)
            nextItem = elemNum - 1;
    }
    if (nextItem == 255 || !action->actionElemNum)
    {
        actor->animPtr[0]->flags |= 0x80u;
        return;
    }
    sameItem = actionItem == nextItem;
    next = &(actionElems[nextItem]);
    anim = actor->animPtr[0];
    actor->actionItem = nextItem;
    ANI_SetTrackList(anim, next->eventLists);
    ANI_ChangeShape(gao, next->shape);
    actor->callCountForChangeAction = 0;
    bits = next->b5;
    if ((gao->flags & 4) != 0)
        gao->basic->dyn->flags ^= (gao->basic->dyn->flags ^ (bits << 14)) & 0x1C0000;
    anim = actor->animPtr[0];
    anim->flags = (anim->flags & 0xff00) | (anim->flags & 0xF0);
    anim->flags = (anim->flags & 0xff00) | (anim->flags & 0x7F);
    anim->flags |= bits & 0xF;
    if (!sameItem)
        actor->animPtr[0]->frequency = 60 * next->actionFrequency / 64;
}

void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* gao, Vec3* offset, char which)
{
    tGameObjectActorObject* actor;

    actor = ANI_l_AnimatedActor(gao);
    if (!actor)
        return;
    if ((which & 1) != 0 && (gao->flags & 0x4000000) != 0)
        MATH_VecAddVecWithoutBuffer(&OBJ_pst_GetFlashMatrix(gao)->Pos, offset);
    if ((which & 2) != 0 && ANI_GaoAnimationIsTransitioning(gao))
    {
        MATH_VecAddVecWithoutBuffer(&actor->animPtr[1]->flashMtx.Pos, offset);
        MATH_VecAddVecWithoutBuffer((Vec3*)&actor->animPtr[0]->flashMtx._m14, offset); // TODO FIXME ez mi??? asm-et néztem ott is ez van
        MATH_VecAddVecWithoutBuffer(&actor->animPtr[0]->flashMtx.Pos, offset);
    }
}

void ANI_GetEndSpeedOfMB(struct sAnim* anim, Vec3* speed)
{
    tGameObjectEventsListElem* track;
    tGameObjectEventElem* key;
    Vec3* from;
    Vec3* to;
    double scale;
    int trackIdx;
    int keyIdx;
    int16_t keyType;

    MATH_SetNullVector(speed);
    EVE_gpst_CurrentListTracks = anim->ptr->eventListStruct;
    track = EVE_gpst_CurrentListTracks->listElems;
    for (trackIdx = 0; trackIdx < anim->ptr->eventListStruct->num; trackIdx++, track++)
    {
        EVE_gpst_CurrentTrack = track;
        if (track->boneID != 0xFFFF)
            break;
        key = 0;
        for (keyIdx = (int)track->elemNum - 1; keyIdx >= 0; keyIdx--)
        {
            keyType = track->trackElems[keyIdx].flags2 & 0x7C0;
            if (keyType != 0)
            {
                if (keyType == 128)
                    key = &track->trackElems[keyIdx];
                break;
            }
        }
        if (!key || (EVE_w_Event_InterpolationKey_GetType(key) & 3) == 0)
            continue;
        if (keyIdx == track->elemNum - 1)
        {
            from = EVE_pst_Event_InterpolationKey_GetPos(key - 1);
            if (from)
            {
                to = EVE_pst_Event_InterpolationKey_GetPos(key);
                MATH_SubstractVec(speed, to, from);
                scale = (double)anim->frequency / (double)key[-1].duration;
                speed->x = (float)(scale * speed->x);
                speed->y = (float)(scale * speed->y);
                speed->z = (float)(scale * speed->z);
            }
        }
        EVE_gpst_CurrentTrack = 0;
        return;
    }
    EVE_gpst_CurrentTrack = 0;
}

void ANI_InterpolateMagicBoxPositionAfterEndOfAnim(tGameObjectMain* gao, tGameObjectActorObject* actor, tAnim* anim, float dt)
{
    Mtx4x4* leadFlash;
    tGaoMatrix* posMtx;
    Vec3 step;
    Vec3 localStep;
    Mtx4x4 refInv;

    leadFlash = &actor->animPtr[0]->flashMtx;
    if ((anim->flags2 & 2) != 0)
    {
        ANI_GetEndSpeedOfMB(anim, &step);
        MATH_TransformVector(&step, &anim->flashMtx, &step);
        leadFlash->v1 = step;
        anim->flags2 = (anim->flags2 - 2) | 4;
    }
    if ((anim->flags2 & 4) != 0)
    {
        MATH_SetNullVector(&step);
        if (dt != 0.0)
            MATH_VecMultScalar(&step, &leadFlash->v1, dt);
        if ((gao->flags & 0x400000) != 0)
        {
            posMtx = gao->basic->mtx;
            MATH_InvertMatrix(&refInv, OBJ_pst_GetAbsoluteMatrix(ANI_l_RefGao(gao)));
            MATH_TransformVector(&localStep, &refInv, &step);
            MATH_VecAddVec(&posMtx->mtx.Pos, &ANI_gst_PositionBeforeAnimPlayer, &localStep);
            ANI_UpdateFlashAfterMagicBoxTranslation(gao, &localStep, 2);
        }
        else
        {
            MATH_VecAddVec(&gao->matrices->Pos, &ANI_gst_PositionBeforeAnimPlayer, &step);
            ANI_UpdateFlashAfterMagicBoxTranslation(gao, &step, 2);
        }
    }
}

void ANI_b_ComputeMagicBoxPositionForBlendAnim(tGameObjectMain* gao, tGameObjectActorObject* actor, tAnim* anim)
{
    tANITrackRunner run;
    tAction** actions;
    tAction* action;
    tActionElem* actionElems;
    tGaoMatrix* posMtx;
    uint16_t trackNum;
    uint8_t elem;
    uint8_t nextElem;
    int privateData;
    float remain;
    float savedDt;

    remain = TIM_gf_dt;
    savedDt = TIM_gf_dt;
    run = ANI_l_TrackRunner(gao);
    if ((anim->flags2 & 6) != 0)
    {
        ANI_InterpolateMagicBoxPositionAfterEndOfAnim(gao, actor, anim, remain);
        TIM_gf_dt = savedDt;
        return;
    }
    EVE_gpst_OwnerGAO = gao;
    EVE_gpst_CurrentGAO = gao;
    EVE_gpst_CurrentData = anim->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    EVE_gpst_CurrentRefMatrix = &anim->flashMtx;
    trackNum = ANI_l_LimitTracksToMagicBox();
    while (1)
    {
        while (1)
        {
            TIM_gf_dt = (float)((double)anim->frequency * remain * 0.016666668);
            if (run(anim->ptr, &remain))
                break;
            if (remain == 0.0)
            {
                EVE_gpst_CurrentListTracks->num = trackNum;
                TIM_gf_dt = savedDt;
                return;
            }
        }
        actions = (tAction**)(((char*)(actor->actionKit)) + sizeof(tActionKit));
        action = actions[anim->actionForTransition];
        elem = anim->actionElemForTransition;
        if (elem != 0xFF)
        {
            nextElem = elem + 1;
            anim->actionElemForTransition = nextElem;
            if (nextElem >= action->actionElemNum)
            {
                nextElem = action->actionElemIDForTransition;
                anim->actionElemForTransition = nextElem;
                if (nextElem != 0xFF && nextElem >= action->actionElemNum)
                    anim->actionElemForTransition = action->actionElemNum - 1;
            }
        }
        if (anim->actionElemForTransition == 0xFF)
            break;
        EVE_gpst_CurrentListTracks->num = trackNum;
        privateData = anim->ptr->tracks->privateData;
        actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));
        ANI_SetTrackList(anim, actionElems[anim->actionElemForTransition].eventLists);
        anim->ptr->tracks->privateData = privateData;
        *(float*)&anim->ptr->tracks[1].privateData = remain;
        anim->flags2 |= 1u;
        EVE_gpst_CurrentData = anim->ptr;
        EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
        trackNum = ANI_l_LimitTracksToMagicBox();
        if (remain == 0.0)
        {
            MATH_CopyMatrix(&anim->flashMtx, OBJ_pst_GetAbsoluteMatrix(gao));
            EVE_gpst_CurrentListTracks->num = trackNum;
            TIM_gf_dt = savedDt;
            return;
        }
    }
    anim->flags |= 0x80u;
    anim->transitionFlags |= 4;
    anim->flags2 |= 2;
    posMtx = ANI_l_PosMatrix(gao);
    ANI_gst_PositionBeforeAnimPlayer = posMtx->mtx.Pos;
    ANI_InterpolateMagicBoxPositionAfterEndOfAnim(gao, actor, anim, remain);
    EVE_gpst_CurrentListTracks->num = trackNum;
    TIM_gf_dt = savedDt;
}

void ANI_b_BlendMagicBox(tGameObjectMain* gao, tGameObjectActorObject* actor, tAnim* anim)
{
    tGameObjectActorObject* gaoActor;
    tGaoMatrix* posMtx;
    Vec3 delta;
    float savedDt;
    Mtx4x4 blendMtx;
    Mtx4x4 beforeMtx;

    posMtx = ANI_l_PosMatrix(gao);
    MATH_CopyMatrix(&beforeMtx, &posMtx->mtx);
    memcpy(&blendMtx, &beforeMtx, sizeof(blendMtx));
    if ((anim->transitionFlags & 8) != 0 || (anim->flags & 6) != 0)
    {
        MATH_SubstractVec(&delta, &beforeMtx.Pos, &ANI_gst_PositionBeforeAnimPlayer);
        gaoActor = ANI_l_AnimatedActor(gao);
        if (gaoActor && ANI_GaoAnimationIsTransitioning(gao))
        {
            MATH_VecAddVecWithoutBuffer(&gaoActor->animPtr[1]->flashMtx.Pos, &delta);
            MATH_VecAddVecWithoutBuffer((Vec3*)&(gaoActor->animPtr[0]->flashMtx._m14), &delta); // TODO FIXME ez mi????? asmbe neztem ott is ez van
            MATH_VecAddVecWithoutBuffer(&gaoActor->animPtr[0]->flashMtx.Pos, &delta);
        }
    }
    else
    {
        savedDt = TIM_gf_dt;
        ANI_b_ComputeMagicBoxPositionForBlendAnim(gao, actor, anim);
        MATH_BlendVector(&blendMtx.Pos, &posMtx->mtx.Pos, &beforeMtx.Pos, *(float*)&(anim->ptr->tracks->privateData));
        MATH_SubstractVec(&delta, &blendMtx.Pos, &beforeMtx.Pos);
        ANI_UpdateFlashAfterMagicBoxTranslation(gao, &delta, 1);
        MATH_SubstractVec(&delta, &blendMtx.Pos, &posMtx->mtx.Pos);
        ANI_UpdateFlashAfterMagicBoxTranslation(gao, &delta, 2);
        MATH_CopyMatrix(&posMtx->mtx, &blendMtx);
        TIM_gf_dt = savedDt;
    }
}

void ANI_PlayLeadAnimation(tGameObjectMain* gao, tGameObjectActorObject* actor, tAnim* anim)
{
    tANITrackRunner run;
    uint16_t transition;
    uint16_t currTime;
    uint8_t frequency;
    float remain;
    float savedDt;
    Mtx4x4 savedAbsMtx;

    EVE_gpst_OwnerGAO = gao;
    EVE_gpst_CurrentData = anim->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    EVE_gpst_CurrentRefMatrix = OBJ_pst_GetFlashMatrix(gao);
    run = ANI_l_TrackRunner(gao);
    remain = TIM_gf_dt;
    savedDt = TIM_gf_dt;
    while (1)
    {
        TIM_gf_dt = (float)((double)anim->frequency * remain * 0.016666668);
        if (actor->eventRunningFrequency)
            TIM_gf_dt = actor->elapsedDT;
        EVE_gpst_CurrentRefMatrix = OBJ_pst_GetFlashMatrix(gao);

        if ((anim->flags & 0x80) == 0)
        {
            if (run(anim->ptr, &remain))
            {
                remain = (float)(remain * 60.0 / (double)anim->frequency);
                ANI_ChangeAction(gao, gao->basic->actorObjectOrVisualObject);
                if (ANI_GaoAnimationIsTransitioning(gao) && remain != 0.0)
                {
                    *(float*)&actor->animPtr[1]->ptr->tracks[1].privateData = remain;
                    actor->animPtr[1]->flags2 |= 1u;
                }
            }
        }

        frequency = actor->eventRunningFrequency;
        if (frequency)
        {
            transition = actor->currentActionTransition;
            actor->eventRunningFrequency = 0;
            actor->currentActionTransition = -1;
            MATH_CopyMatrix(&savedAbsMtx, gao->matrices);
            ACT_SetAction(gao, transition, 0);
            currTime = actor->currTime;
            if (currTime)
                EVE_SetTracksTime(actor->animPtr[0]->ptr, (float)((double)currTime * 0.016666668));
            if (frequency != 0xFF)
                actor->animPtr[0]->frequency = frequency;
            MATH_CopyMatrix(gao->matrices, &savedAbsMtx);
            ANI_UpdateFlash(gao, 3);
            remain = (float)(savedDt - actor->elapsedDT);
            actor->elapsedDT = 0.0;
        }
        EVE_gpst_CurrentData = anim->ptr;
        EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
        EVE_gpst_CurrentRefMatrix = OBJ_pst_GetFlashMatrix(gao);
        if (remain == 0.0)
            break;
        if ((anim->flags & 0x80) != 0)
        {
            TIM_gf_dt = savedDt;
            return;
        }
    }
    TIM_gf_dt = savedDt;
}

void ANI_PlayGameObjectAnimation(tGameObjectMain* gao, tGameObjectActorObject* actor, float dt, Vec3* speed)
{
    tAnim* anim;
    tAnim* blend;
    tGaoMatrix* posMtx;
    double blendLen;
    double blendTime;
    float t;
    float savedDt;
    Vec3 posBefore;
    Vec3 moved;
    Vec3 tmp;

    posBefore = ANI_l_PosMatrix(gao)->mtx.Pos;
    ANI_gst_PositionBeforeAnimPlayer = posBefore;
    savedDt = TIM_gf_dt;
    TIM_gf_dt = dt;
    anim = actor->animPtr[0];
    // TIM_gf_dt keeps dt on this path: the original does not restore it either
    if (!anim || anim == (tAnim*)-1)
        return;

    if ((anim->flags & 0x80) == 0 && speed && (anim->flags & 0xF) == 2)
    {
        MATH_VecMultScalar(&moved, speed, dt);
        ANI_LimitFrequencyBetween0And255(anim, &moved, speed);
    }
    if ((anim->flags & 0x80) == 0 || actor->eventRunningFrequency)
        ANI_PlayLeadAnimation(gao, actor, anim);
    if (ANI_GaoAnimationIsTransitioning(gao))
    {
        blend = actor->animPtr[1];
        if ((blend->flags2 & 1) != 0)
            TIM_gf_dt = *(float*)&blend->ptr->tracks[1].privateData;
        blendLen = (double)(blend->actionLength + 1) * 0.016666668;
        blendTime = (double)blend->startTime * 0.00006510417 + TIM_gf_dt;
        t = (float)blendTime;
        if (blendTime >= blendLen)
        {
            ANI_ClearBlendingFlag(gao);
        }
        else
        {
            *(float*)&blend->ptr->tracks->privateData = (float)(t / blendLen);
            blend->startTime = (uint16_t)(t * 15360.0); // TODO: ez igy jo ?
            if (speed && (blend->flags & 0xF) == 2)
            {
                MATH_VecMultScalar(&moved, speed, TIM_gf_dt);
                ANI_LimitFrequencyBetween0And255(blend, &moved, speed);
            }
            ANI_b_BlendMagicBox(gao, actor, blend);
        }
    }
    if ((gao->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWhenHie(gao);
    posMtx = ANI_l_PosMatrix(gao);
    MATH_SubstractVec(&moved, &posMtx->mtx.Pos, &posBefore);
    if ((anim->flags & 0xF) == 6)
    {
        posMtx = ANI_l_PosMatrix(gao);
        posMtx->mtx.Pos = posBefore;
        MATH_VecInvert(&tmp, &moved);
        ANI_UpdateFlashAfterMagicBoxTranslation(gao, &tmp, 3);
    }
    if ((anim->flags & 0xF) == 0 && speed && dt != 0.0)
    {
        if ((gao->flags & 0x400000) != 0)
            OBJ_ComputeGlobalWhenHie(gao);
        posMtx = ANI_l_PosMatrix(gao);
        posMtx->mtx.Pos = posBefore;
        MATH_VecInvert(&tmp, &moved);
        ANI_UpdateFlashAfterMagicBoxTranslation(gao, &tmp, 3);
        if ((gao->flags & 0x400000) != 0)
        {
            tmp = moved;
            MATH_TransformVector(&moved, gao->basic->mtx->refGao2->matrices, &tmp);
        }
        MATH_VecMultScalar(speed, &moved, (float)(1.0 / dt));
        DYN_SetHorizontalSpeedComponent(gao->basic->dyn, speed);
    }
    TIM_gf_dt = savedDt;
}

void ANI_OneCall(struct sGameObjectMain* gao)
{
    tDyn* dyn;
    tGameObjectActorObject* actor;
    Vec3* force;
    float fullTime;
    Vec3 forceVec;

    if ((gao->flags & 4) != 0)
    {
        dyn = gao->basic->dyn;
        force = &forceVec;
        MATH_VecMultScalar(&forceVec, &dyn->forceVector, dyn->mass);
    }
    else
    {
        force = 0;
    }
    if ((gao->flags2 & 2) != 0 && (gao->flags2 & 4) == 0)
    {
        actor = ANI_l_ActorOrNull(gao);
        if (actor && actor->animPtr[0] && !actor->initedInANIMainCall)
            ANI_l_InitTracksTime(actor, &fullTime);
    }

    ANI_PlayGameObjectAnimation(gao, gao->basic->actorObjectOrVisualObject, TIM_gf_dt, force);
}


void ANI_InitSkeleton(tGroup* grp, tGameObjectMain* gao)
{
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tGameObjectMain* bone;
    tGaoMatrix* boneMtx;
    tMultiPartGameObjectLinks* links;
    Mtx4x4* matrices;
    int idx;
    int i;

    lastElem = ANI_l_LastElem(grp->tablePF);
    idx = 0;
    for (elem = grp->tablePF->firstElemPtr; elem <= lastElem; elem++, idx++)
    {
        bone = (tGameObjectMain*)elem->objectPtr;
        if (TAB_TableElemIsInvalid(bone))
            continue;
        if ((bone->flags & 0x400000) == 0)
            OBJ_ChangeIdentityFlags(bone, bone->flags | 0x400000, bone->flags);
        boneMtx = bone->basic->mtx;
        if (!boneMtx->refGao2)
        {
            boneMtx->refGao = gao;
            bone->basic->mtx->refGao2 = gao;
            if (!idx && (bone->flags & 0x10000) == 0)
            {
                OBJ_ChangeIdentityFlags(bone, bone->flags | 0x10000, bone->flags);
                MATH_CopyMatrix(bone->matrices + 1, &MATH_gst_IdentityMatrix);
                bone->matrices[1].Pos = MATH_gst_NulVector;
            }
            MATH_CopyMatrix(&bone->basic->mtx->mtx, &bone->matrices[1]);
        }
        bone->flags |= 1;
        if ((bone->flags & 0x200000) != 0 && bone->basic->multiPartData->elemNum)
        {
            links = (tMultiPartGameObjectLinks*)(bone->basic->multiPartData->multipartGaoObjectPartsOrMatrices);
            for (i = 0; i < bone->basic->multiPartData->elemNum; i++)
            {
                matrices = (Mtx4x4*)(gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices);
                links[i].gao = gao;
                links[i].mtxPtr = &matrices[links[i].mtxIdx]; // bug fixed: broken actors; v12[v10].mtxPtr = &matrices[v12[->mtxIdx];
            }
        }
    }
}

void ANI_pst_DuplicateGameObjectAnim(struct sWorld* world, struct sGameObjectMain* dstGao, struct sGameObjectMain* srcGao)
{
    tGameObjectActorObject* src;
    tGameObjectActorObject* dst;
    tGroup* newGroup;
    tAnim* anim;
    tShape* shape;
    BOOL hasTracks;

    src = (tGameObjectActorObject*)srcGao->basic->actorObjectOrVisualObject;
    dst = (tGameObjectActorObject*)dstGao->basic->actorObjectOrVisualObject;
    if (src->actionKit)
        ACT_ActionRestoreInit(src->actionKit);
    memcpy(dst, src, sizeof(tGameObjectActorObject));
    hasTracks = (dst->flagsEgBlendIw2 & ~0x02) != 0;
    dst->flagsEgBlendIw2 &= ~2u;
    dst->animPtr[1] = 0;
    dst->currentAction = -1;
    dst->actionItem = -1;
    if (hasTracks)
    {
        anim = ANI_AllocAnim();
        dst->animPtr[0] = anim;
        anim->frequency = src->animPtr[0]->frequency;
        dst->animPtr[0]->actionElemForTransition = 0;
        dst->animPtr[0]->transitionFlags = 0;
        dst->animPtr[0]->actionLength = 0;
        dst->animPtr[0]->flags2 = 0;
        dst->animPtr[0]->startTime = 0;
        dst->animPtr[0]->actionForTransition = 0;
        dst->animPtr[0]->flags = src->animPtr[0]->flags;
        ANI_SetTrackList(dst->animPtr[0], src->animPtr[0]->ptr->eventListStruct);
    }
    if (src->group)
    {
        newGroup = GRP_pst_CreateNewGroup();
        GRP_DuplicateGroupObjects(world, src->group, newGroup);
        UpdateModifiersGroup(dstGao, newGroup);
        dst->skeletonGroup = newGroup;
        ++newGroup->groupElemNum;
        ++dst->group->groupElemNum;
        ANI_InitSkeleton(dst->skeletonGroup, dstGao);
    }
    if (src->shape)
    {
        shape = src->shape2;
        dst->shape2 = shape;
        dst->shape = src->shape;
        ++shape->usageCount;
        ANI_ApplyShape(dstGao);
    }
}

struct sShape* ANI_ul_ShapeCallback(uint32_t Offset)
{
    tShape* shape;
    char* data;
    unsigned char num;
    int size;
    int i;
    int id;

    data = BIG_pc_ReadFileTmp(Offset, &size);
    shape = (tShape*)MEM_p_Alloc(sizeof(tShape));
    memset(shape, 0xFFu, sizeof(tShape));
    shape->usageCount = 0;
    num = LOA_ReadChar(&data, 0, 1);
    shape->num = num;
    if (num > 64u)
        num = 64;
    shape->num = num;
    for (i = 0; i < shape->num; i++)
    {
        id = LOA_ReadChar(&data, 0, 1);
        shape->shapesOriginal[id] = LOA_ReadChar(&data, 0, 1);
        shape->shapes[id] = LOA_ReadChar(&data, 0, 1);
    }
    return shape;
}

void ANI_AttachSkeletonModelToWorld(tGroup* grp, tWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tableElemStructPF* child;
    tableElemStructPF* lastChild;
    tableDescStruct* childTab;
    tGameObjectMain* gao;
    tGameObjectMain* childGao;

    lastElem = ANI_l_LastElem(grp->tablePF);
    elem = grp->tablePF->firstElemPtr;
    if (!elem)
        return;
    for (; elem <= lastElem; elem++)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (TAB_TableElemIsInvalid(gao)
            || TAB_ul_PFtable_GetElemIndexWithPointer(&world->objectTablePF, elem->objectPtr) != -1)
            continue;
        gao->flags |= 1u;
        WOR_World_JustAfterLoadObject(world, gao, 0, 1);
        if ((gao->flags & 0x800000) == 0)
            continue;
        childTab = gao->extended->grp->tablePF;
        lastChild = ANI_l_LastElem(childTab);
        for (child = childTab->firstElemPtr; child <= lastChild; child++)
        {
            childGao = (tGameObjectMain*)child->objectPtr;
            if (!TAB_TableElemIsInvalid(childGao)
                && TAB_ul_PFtable_GetElemIndexWithPointer(&world->objectTablePF, child->objectPtr) == -1)
            {
                childGao->flags |= 1u;
                WOR_World_JustAfterLoadObject(world, childGao, 1, 1);
            }
        }
    }
}

void ANI_pst_Load(int animation, int shape, int group, tGameObjectMain* gao, tWorld* world)
{
    tGameObjectActorObject* gaoActorObject;
    tGroup* skeleton;
    tAnim* anim;
    Mtx4x4* mtx;
    int boneNum;
    tGroup* groupPtr;
    tShape* shapePtr;
    tGameObjectEventsList* animationPtr;

    gaoActorObject = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
    gaoActorObject->initedInANIMainCall = 1;
    gaoActorObject->shape = 0;
    gaoActorObject->skeletonGroup = 0;
    gaoActorObject->group = 0;
    gaoActorObject->skeletonGroupToRestoreOnReinit = 0;
    skeleton = 0;
    animationPtr = 0;
    shapePtr = 0;
    groupPtr = 0;
    if (shape != -1)
        LOA_MakeFileRef(shape, &shapePtr, ANI_ul_ShapeCallback, 1);
    if (group != -1)
        LOA_MakeFileRef(group, &groupPtr, GRP_ul_LoadGrpCallback, 1);
    LOA_Resolve();
    if (groupPtr)
    {
        skeleton = GRP_pst_CreateNewGroup();
        ANI_AttachSkeletonModelToWorld(groupPtr, world);
        GRP_DuplicateGroupObjects(world, groupPtr, skeleton);
        UpdateModifiersGroup(gao, skeleton);
        ++skeleton->groupElemNum;
        ++groupPtr->groupElemNum;
        gaoActorObject->group = groupPtr;
        gaoActorObject->skeletonGroup = skeleton;
    }
    if (shapePtr)
    {
        ++shapePtr->usageCount;
        gaoActorObject->shape = shapePtr;
        gaoActorObject->shape2 = shapePtr;
        memcpy(gaoActorObject->shapes, shapePtr->shapes, sizeof(gaoActorObject->shapes));
        ANI_ApplyShape(gao);
    }
    if (animation == -1 || gaoActorObject->actionKit)
    {
        gaoActorObject->animPtr[0] = 0;
    }
    else
    {
        LOA_MakeFileRef(animation, &animationPtr, EVE_ul_CallbackLoadListTracks, 1);
        LOA_Resolve();
        anim = ANI_AllocAnim();
        gaoActorObject->animPtr[0] = anim;
        ANI_SetTrackList(anim, animationPtr);
        gaoActorObject->flagsEgBlendIw2 = 1;
    }
    if (groupPtr)
    {
        boneNum = skeleton->tablePF->actElemNum;
        if ((gao->flags & 0x200000) == 0 || gao->basic->multiPartData->elemNum >> 1 < boneNum)
            OBJ_GameObject_AllocateGizmo(gao, boneNum, 0);
    }
    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    OBJ_ReinitMatrixStorageByFlags(gao, mtx);
    if (groupPtr)
    {
        ANI_InitSkeleton(skeleton, gao);
        UpdateModifiersGroup(gao, skeleton);
    }
}

void ANI_ResolveRefs(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor;

    actor = ANI_l_Actor(gao);
    ANI_pst_Load(
        (int)actor->animPtr[0], // ezek még csak key-ek azért castolunk int-re
        (int)actor->shape,
        (int)actor->group,
        gao, WOR_gpst_CurrentWorld);
}

void ANI_ReinitAnimation(struct sAnim* anim)
{
    if (anim && anim->ptr)
        EVE_ReinitData(anim->ptr);
}

void ANI_Reinit(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor;
    tActionKit* kit;
    tShape* shape;
    tGroup* grp;
    unsigned int i;

    actor = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
    if (!actor)
        return;
    actor->transitionElem = 0;
    actor->currentAction = -1;
    actor->actionItem = 0;
    actor->callCountForChangeAction = 0;
    actor->currentActionTransition = -1;
    actor->lod = 0;
    actor->aniBitFieldLOD = ANI_c_DefaultBitFieldLOD;
    for (i = 0; i < 4; i++)
    {
        if (((uint8_t)(1 << i) & actor->flagsEgBlendIw2) != 0)
            ANI_ReinitAnimation(actor->animPtr[i]);
    }
    kit = actor->actionKit;
    actor->flagsEgBlendIw2 &= 1u;
    if (kit)
        ACT_ActionRestoreInit(kit);
    shape = actor->shape2;
    if (shape)
    {
        actor->shape = shape;
        ANI_ApplyShape(gao);
    }
    grp = actor->skeletonGroupToRestoreOnReinit;
    if (grp)
    {
        actor->skeletonGroup = grp;
        actor->skeletonGroupToRestoreOnReinit = 0;
    }
}

void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor;
    tGameObjectMain* refGao;
    Mtx4x4 tmpMtx;

    if ((gao->flags & 2) == 0 || (gao->flags & 0x400000) == 0)
        return;
    actor = ANI_l_Actor(gao);
    if (!actor->animPtr[0])
        return;
    refGao = gao->basic->mtx->refGao2;
    if ((gao->flags & 0x4000000) != 0)
    {
        MATH_CopyMatrix(&tmpMtx, OBJ_pst_GetFlashMatrix(gao));
        MATH_MulMatrixMatrix(OBJ_pst_GetFlashMatrix(gao), &tmpMtx, OBJ_pst_GetAbsoluteMatrix(refGao));
    }
    if (ANI_GaoAnimationIsTransitioning(gao))
    {
        MATH_CopyMatrix(&tmpMtx, &actor->animPtr[1]->flashMtx);
        MATH_MulMatrixMatrix(&actor->animPtr[1]->flashMtx, &tmpMtx, OBJ_pst_GetAbsoluteMatrix(refGao));
    }
}

struct sGameObjectMain* ANI_pst_GetReference(struct sGameObjectMain* gao)
{
    tGameObjectMain* cur;
    tGameObjectMain* ref;

    if ((gao->flags & 1) == 0)
        return gao;
    cur = gao;
    while ((ref = ANI_l_RefGao(cur)) != 0)
    {
        cur = ref;
        if ((cur->flags & 1) == 0)
            return cur;
    }
    return gao;
}

void ANI_PlayBlendAnimationWithoutMagicBox(tGameObjectMain* gao, tGameObjectActorObject* actor, tAnim* anim)
{
    tANITrackRunner run;
    tAnimTracks* tracks;
    int16_t savedBit[4];
    uint8_t transitionFlags;
    float currTime;
    float blendEnd;
    float remain;
    float savedDt;

    EVE_gpst_OwnerGAO = gao;
    EVE_gpst_CurrentGAO = gao;
    EVE_gpst_CurrentData = anim->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    EVE_gpst_CurrentRefMatrix = &anim->flashMtx;
    tracks = EVE_gpst_CurrentData->tracks;
    savedBit[0] = tracks[0].flag & 1;
    savedBit[1] = tracks[1].flag & 1;
    savedBit[2] = tracks[2].flag & 1;
    savedBit[3] = tracks[3].flag & 1;
    tracks[0].flag &= ~1;
    tracks[1].flag &= ~1u; //LOBYTE(EVE_gpst_CurrentData->tracks[1].flag) &= ~1u;
    run = ANI_l_TrackRunner(gao);
    transitionFlags = anim->transitionFlags;
    if ((transitionFlags & 4) != 0 && (transitionFlags & 8) == 0)
    {
        EVE_GetTracksTime(anim->ptr, 0, &currTime, 0);
        EVE_GetTracksTime(anim->ptr, 2, &blendEnd, 0);
        if (currTime + 0.001 < blendEnd)
            EVE_gpst_CurrentData->tracks[3].flag &= ~1u; //LOBYTE(EVE_gpst_CurrentData->tracks[3].flag) &= ~1u;
    }
    remain = TIM_gf_dt;
    savedDt = TIM_gf_dt;
    do
    {
        TIM_gf_dt = (float)((double)anim->frequency * remain * 0.016666668);
        run(anim->ptr, &remain);
    } while (remain != 0.0);
    EVE_gpst_CurrentData->tracks[0].flag |= savedBit[0];
    EVE_gpst_CurrentData->tracks[1].flag |= savedBit[1];
    EVE_gpst_CurrentData->tracks[2].flag |= savedBit[2];
    EVE_gpst_CurrentData->tracks[3].flag |= savedBit[3];
    TIM_gf_dt = savedDt;
}

void ANI_BeforeRender(tGameObjectMain* gao)
{
    tGameObjectActorObject* actor;
    tGroup* skeleton;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tAnim* anim;
    tGaoMatrix* local;
    Mtx4x4* acc;
    double remain;
    double frames;
    float savedDt;
    float currTime;
    float blendEnd;
    int slot;
    int idx;
    BOOL keepScale;
    float factor;
    uint8_t mask;
    Vec3 localScale;
    Vec3 accScale;
    Mtx4x4 blended[64];

    actor = ANI_l_Actor(gao);
    skeleton = actor->skeletonGroup;
    if (!skeleton)
        return;
    lastElem = ANI_l_LastElem(skeleton->tablePF);
    mask = actor->flagsEgBlendIw2 & 0xF;
    for (slot = 0; mask; mask >>= 1, slot++)
    {
        if ((mask & 1) == 0)
            continue;
        anim = actor->animPtr[slot];
        if (slot)
        {
            savedDt = TIM_gf_dt;
            if ((anim->flags2 & 1) != 0)
            {
                TIM_gf_dt = *(float*)&anim->ptr->tracks[1].privateData;
                --anim->flags2;
            }
            if ((anim->transitionFlags & 4) != 0
                || TIM_gf_dt != 0.0
                && (EVE_GetTracksTime(anim->ptr, 2, &blendEnd, 0),
                    EVE_GetTracksTime(anim->ptr, 0, &currTime, 0),
                    remain = currTime - blendEnd,
                    TIM_gf_dt = (float)remain,
                    frames = remain * 60.0 / (double)anim->frequency,
                    TIM_gf_dt = (float)frames,
                    frames < 0.001))
            {
                TIM_gf_dt = 0.0;
            }
            ANI_PlayBlendAnimationWithoutMagicBox(gao, actor, anim);
            TIM_gf_dt = savedDt;
        }
        idx = 0;
        acc = blended;
        for (elem = skeleton->tablePF->firstElemPtr; elem <= lastElem; elem++, idx++, acc++)
        {
            if (idx >= gao->basic->multiPartData->elemNum)
                break;
            local = OBJ_pst_GetLocalMatrix((tGameObjectMain*)elem->objectPtr);
            if (!local)
                continue;
            if (slot)
            {
                if ((anim->transitionFlags & 0x80u) != 0)
                    continue;
                factor = *(float*)&anim->ptr->tracks->privateData;
                keepScale = 0;
                if ((local->mtx.mtxflag & 8) != 0 && (acc->mtxflag & 8) != 0)
                {
                    MATH_GetScale(&localScale, &local->mtx);
                    MATH_GetScale(&accScale, acc);
                    if (fabs(localScale.x - accScale.x) < 0.0000099999997
                        && fabs(localScale.y - accScale.y) < 0.0000099999997
                        && fabs(localScale.z - accScale.z) < 0.0000099999997)
                    {
                        keepScale = 1;
                    }
                }
                MATH_MatrixBlend(acc, &local->mtx, acc, factor, 1u);
                if (keepScale)
                    MATH_SetScale(acc, &localScale);
                MATH_BlendVector(&acc->Pos, &local->mtx.Pos, &acc->Pos, factor);
                MATH_CopyMatrix(&local->mtx, acc);
            }
            else
            {
                MATH_CopyMatrix(acc, &local->mtx);
            }
        }
    }
    AI_C_Callback = 1;
    AI_ExecCallback(gao, 3);
    AI_C_Callback = 0;
}

void ANI_BeforeRendering(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tGameObjectMain* gao;

    lastElem = ANI_l_LastElem(&world->visibleGameObjectsPF);
    for (elem = world->visibleGameObjectsPF.firstElemPtr; elem <= lastElem; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (!TAB_TableElemIsInvalid(gao) && (gao->flags2 & 4) == 0 && (gao->flags & 2) != 0)
        {
            if (ANI_l_Actor(gao)->skeletonGroup)
                ANI_BeforeRender((tGameObjectMain*)elem->objectPtr);
        }
    }
}

void ANI_MainCall(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tGameObjectMain* gao;
    tGameObjectActorObject* actor;
    tGameObjectExtended* ext;

    lastElem = ANI_l_LastElem(&world->eot.animatedObjectsTablePF);

    for (elem = world->eot.animatedObjectsTablePF.firstElemPtr; elem <= lastElem; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (TAB_TableElemIsInvalid(gao))
            continue;
        if (!gao->basic || !gao->basic->actorObjectOrVisualObject)
            continue;
        ext = gao->extended;
        if (!ext || (ext->extraFlag & 1) == 0)
        {
            ANI_OneCall((tGameObjectMain*)elem->objectPtr);
            continue;
        }
        ext->extraFlag &= ~1;
        if ((gao->flags2 & 2) != 0 && (gao->flags2 & 4) == 0)
        {
            actor = ANI_l_ActorOrNull(gao);
            if (actor && actor->animPtr[0] && !actor->initedInANIMainCall)
                ANI_l_InitTracksTime(actor, 0);
        }
    }
}

struct sGameObjectMain* ANI_pst_GetReferenceInit(struct sGameObjectMain* gao)
{
    tGameObjectMain* cur;

    if ((gao->flags & 1) == 0)
        return gao;
    cur = gao;
    while (cur->basic && cur->basic->mtx && cur->basic->mtx->refGao)
    {
        cur = cur->basic->mtx->refGao;
        if ((cur->flags & 1) == 0)
            return cur;
    }
    return gao;
}

void ANI_ComputeBoneMatrix(tGameObjectMain* currentBone, tGameObjectMain* mainGao, Mtx4x4* destMtx, int updateAbsolute)
{
    tGameObjectMain* refGao;
    tGaoMatrix* localMtx;
    Mtx4x4* mainMtx;
    Mtx4x4* boneMtx;
    Mtx4x4* refMtx;
    Vec3 scale;
    Vec3 localScale;
    Vec3 mainScale;
    Mtx4x4 mainInv;

    mainMtx = OBJ_pst_GetAbsoluteMatrix(mainGao);
    MATH_GetScale(&mainScale, mainMtx); // mainScale is never read, the original computes it anyway
    if (ANI_l_RefGao(currentBone) == mainGao)
    {
        MATH_CopyMatrix(destMtx, &OBJ_pst_GetLocalMatrix(currentBone)->mtx);
    }
    else if ((currentBone->flags & 0x400000) != 0 && (refGao = ANI_l_RefGao(currentBone)) != 0)
    {
        refMtx = &(((Mtx4x4*)(mainGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[TAB_ul_PFtable_GetElemIndexWithPointer(ANI_l_Actor(mainGao)->skeletonGroup->tablePF, refGao)]);
        MATH_GetScale(&scale, refMtx);
        localMtx = OBJ_pst_GetLocalMatrix(currentBone);
        MATH_GetScale(&localScale, &localMtx->mtx);
        scale.x = scale.x * localScale.x;
        scale.y = scale.y * localScale.y;
        scale.z = scale.z * localScale.z;
        localMtx = OBJ_pst_GetLocalMatrix(currentBone);
        MATH_MulMatrixMatrix(destMtx, &localMtx->mtx, refMtx);
        MATH_Orthonormalize(destMtx);
        MATH_SetScale(destMtx, &scale);
    }
    else
    {
        MATH_InvertMatrix(&mainInv, OBJ_pst_GetAbsoluteMatrix(mainGao));
        MATH_MulMatrixMatrix(destMtx, OBJ_pst_GetAbsoluteMatrix(currentBone), &mainInv);
    }
    if (updateAbsolute && currentBone->flags2 >= 0)
    {
        MATH_MulMatrixMatrix(OBJ_pst_GetAbsoluteMatrix(currentBone), destMtx, mainMtx);
        boneMtx = currentBone->matrices;
        if ((boneMtx->mtxflag & 8) != 0)
            MATH_Orthonormalize(boneMtx);
    }
}

void ANI_Render(struct sGameObjectMain* mainGao)
{
    tGameObjectActorObject* actor;
    tAnim* anim;
    tGroup* skeleton;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tGameObjectMain* bone;
    tGameObjectMain* gao;
    Mtx4x4* boneMtx;
    Mtx4x4* absMtx;
    BOOL hasLead;
    float savedDt;
    int idx;
    Mtx4x4 savedAbsMtx;

    QueryPerformanceCounter(&PerfCounterStruct[5]);
    actor = (tGameObjectActorObject*)mainGao->basic->actorObjectOrVisualObject;
    anim = actor->animPtr[0];
    skeleton = actor->skeletonGroup;
    hasLead = anim != 0;
    if (!actor->initedInANIMainCall && anim)
    {
        ANI_l_InitTracksTime(actor, 0);
        savedDt = TIM_gf_dt;
        TIM_gf_dt = 0.0;
        ANI_PlayLeadAnimation(mainGao, actor, actor->animPtr[0]);
        TIM_gf_dt = savedDt;
        AI_C_Callback = 1;
        AI_ExecCallback(mainGao, 3);
        AI_C_Callback = 0;
    }
    if (!skeleton)
    {
        PRO_QPC_RecordTimeDecRef(5);
        return;
    }
    lastElem = ANI_l_LastElem(skeleton->tablePF);
    idx = 0;
    for (elem = skeleton->tablePF->firstElemPtr; elem <= lastElem; elem++, idx++)
    {
        if (hasLead && idx >= mainGao->basic->multiPartData->elemNum)
            break;
        bone = (tGameObjectMain*)(elem->objectPtr);
        boneMtx = &(((Mtx4x4*)(mainGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[idx]);
        if (bone->extended && (bone->flags & 0x2000) != 0 && bone->extended->mdf)
        {
            MDF_ApplyAllGao(bone);
            MDF_UnApplyAllGao(bone);
        }
        ANI_ComputeBoneMatrix(bone, mainGao, boneMtx, 1);
        bone->flags2 &= ~8u;
        bone->distance = mainGao->distance;
        bone->flags2 |= mainGao->flags2 & 8;
        bone->basic->extraMaterial = mainGao->basic->extraMaterial;
        if ((bone->flags2 & 0x200000) == 0 && (bone->flags & 0x4000) != 0)
        {
            pst_RenderingStack[PushedStack + 1] = bone;
            PushedStack = PushedStack + 1;
        }
    }
    PRO_QPC_RecordTimeDecRef(5);
    while (PushedStack)
    {
        gao = pst_RenderingStack[PushedStack];
        PushedStack = PushedStack - 1;
        if (!gao->basic)
            continue;
        if (gao->basic->multiPartData && gao->basic->multiPartData->elemNum > 1)
        {
            MATH_CopyMatrix(&savedAbsMtx, OBJ_pst_GetAbsoluteMatrix(gao));
            absMtx = OBJ_pst_GetAbsoluteMatrix(mainGao);
            MATH_CopyMatrix(OBJ_pst_GetAbsoluteMatrix(gao), absMtx);
            GRO_Render(gao);
            MATH_CopyMatrix(OBJ_pst_GetAbsoluteMatrix(gao), &savedAbsMtx);
        }
        else
        {
            GRO_Render(gao);
        }
    }
}

void ANI_ChangeDefaultSkeleton(struct sGameObjectMain* gao, struct sGameObjectMain* srcGao)
{
    tGameObjectActorObject* actor;
    tGameObjectActorObject* srcActor;
    tGroup* grp;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tGameObjectMain* bone;
    tGaoMatrix* boneMtx;

    actor = ANI_l_AnimatedActor(gao);
    if (!actor)
        return;
    grp = actor->skeletonGroup;
    if (!grp)
        return;
    srcActor = ANI_l_AnimatedActor(srcGao);
    if (!srcActor || !srcActor->skeletonGroup)
        return;
    if (!actor->skeletonGroupToRestoreOnReinit)
        actor->skeletonGroupToRestoreOnReinit = grp;
    ANI_l_Actor(gao)->skeletonGroup = ANI_l_Actor(srcGao)->skeletonGroup;
    ANI_InitSkeleton(ANI_l_Actor(gao)->skeletonGroup, gao);
    lastElem = ANI_l_LastElem(ANI_l_Actor(gao)->skeletonGroup->tablePF);
    for (elem = ANI_l_Actor(gao)->skeletonGroup->tablePF->firstElemPtr; elem <= lastElem; ++elem)
    {
        bone = (tGameObjectMain*)elem->objectPtr;
        if (!TAB_TableElemIsInvalid(elem->objectPtr))
        {
            boneMtx = bone->basic->mtx;
            if (boneMtx->refGao2 == srcGao)
            {
                boneMtx->refGao = gao;
                bone->basic->mtx->refGao2 = gao;
            }
        }
    }
}

void ANI_RestoreDefaultSkeleton(struct sGameObjectMain* gao, struct sGameObjectMain* srcGao)
{
    tGameObjectActorObject* actor;
    tGameObjectActorObject* srcActor;
    tGroup* grp;
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tGameObjectMain* bone;
    tGaoMatrix* boneMtx;

    actor = ANI_l_AnimatedActor(gao);
    if (!actor)
        return;
    grp = actor->skeletonGroupToRestoreOnReinit;
    if (!grp)
        return;
    srcActor = ANI_l_AnimatedActor(srcGao);
    if (!srcActor || !srcActor->skeletonGroup)
        return;
    actor->skeletonGroup = grp;
    ANI_l_Actor(gao)->skeletonGroupToRestoreOnReinit = 0;
    lastElem = ANI_l_LastElem(ANI_l_Actor(srcGao)->skeletonGroup->tablePF);
    for (elem = ANI_l_Actor(srcGao)->skeletonGroup->tablePF->firstElemPtr; elem <= lastElem; ++elem)
    {
        bone = (tGameObjectMain*)elem->objectPtr;
        if (!TAB_TableElemIsInvalid(elem->objectPtr))
        {
            boneMtx = bone->basic->mtx;
            if (boneMtx->refGao2 == gao)
            {
                boneMtx->refGao = srcGao;
                bone->basic->mtx->refGao2 = srcGao;
            }
        }
    }
}

void ANI_DestroyShape(tShape** shape)
{
    if (!--(*shape)->usageCount)
    {
        MEM_Free(*shape);
        LOA_DeleteAddress((int)*shape);
        *shape = NULL;
    }
}

void ANI_CloneSet(struct sGameObjectMain* gao, struct sGameObjectMain* srcGao, int actionId)
{
    tGameObjectActorObject* srcActor;
    tGameObjectActorObject* actor;
    tGameObjectBasic* basic;
    tGroup* grp;
    tGroup* srcGrp;
    tGroup* newGroup;
    tShape* srcShape;
    tAnim* anim;
    tActionKit* kit;
    tAction** actions;
    tAction* action;
    tActionElem* actionElems;
    Mtx4x4* mtx;
    float savedDt;
    tWorld* world;

    if ((srcGao->flags & 0x1000) == 0 || (srcGao->flags & 2) == 0)
        return;
    srcActor = ANI_l_ActorOrNull(srcGao);
    if (!srcActor || !srcActor->skeletonGroup)
        return;
    if ((gao->flags & 0x4000) != 0)
        return;
    world = WOR_gpst_CurrentWorld;
    if (!world)
        return;
    if ((gao->flags & 0x1000) == 0)
    {
        basic = (tGameObjectBasic*)MEM_p_Alloc(sizeof(tGameObjectBasic));
        gao->basic = basic;
        basic->actorObjectOrVisualObject = 0;
        basic->dyn = 0;
        basic->mtx = 0;
        basic->multiPartData = 0;
        basic->extraMaterial = 0;
        gao->flags |= 0x1000u;
    }
    if ((gao->flags & 2) != 0)
    {
        actor = gao->basic->actorObjectOrVisualObject;
        grp = actor->skeletonGroup;
        if (grp)
        {
            if (actor->skeletonGroupToRestoreOnReinit)
                ANI_DestroySkeleton(actor->skeletonGroupToRestoreOnReinit);
            else
                ANI_DestroySkeleton(grp);
        }
        if (ANI_l_Actor(gao)->group)
            ANI_DestroySkeleton(ANI_l_Actor(gao)->group);
        actor = ANI_l_Actor(gao);
        if (actor->shape2)
            ANI_DestroyShape(&actor->shape2);
        anim = ANI_l_Actor(gao)->animPtr[0];
        if (anim && anim->ptr && anim->ptr->eventListStruct)
        {
            ANI_DestroyAnimation(anim);
            ANI_l_Actor(gao)->animPtr[0] = 0;
        }
    }
    else
    {
        gao->basic->actorObjectOrVisualObject = ANI_pst_CreateGameObjectAnim();
        memset(gao->basic->actorObjectOrVisualObject, 0, sizeof(tGameObjectActorObject));
        gao->flags |= 2u;
    }
    srcGrp = ANI_l_Actor(srcGao)->group;
    srcShape = ANI_l_Actor(srcGao)->shape2;
    if (srcGrp)
    {
        ANI_l_Actor(gao)->group = srcGrp;
        ++ANI_l_Actor(gao)->group->groupElemNum;
        newGroup = GRP_pst_CreateNewGroup();
        GRP_DuplicateGroupObjects(world, srcGrp, newGroup);
        UpdateModifiersGroup(gao, newGroup);
        ANI_l_Actor(gao)->skeletonGroup = newGroup;
        ++ANI_l_Actor(gao)->skeletonGroup->groupElemNum;
        OBJ_GameObject_AllocateGizmo(gao, srcGrp->tablePF->actElemNum, 0);
        ANI_InitSkeleton(ANI_l_Actor(gao)->skeletonGroup, gao);
    }
    if (srcShape)
    {
        ANI_l_Actor(gao)->shape2 = ANI_l_Actor(srcGao)->shape2;
        ANI_l_Actor(gao)->shape = ANI_l_Actor(srcGao)->shape;
        memcpy(
            ANI_l_Actor(gao)->shapes,
            ANI_l_Actor(srcGao)->shapes,
            sizeof(ANI_l_Actor(gao)->shapes));
        ++ANI_l_Actor(gao)->shape2->usageCount;
        ANI_ApplyShape(gao);
    }
    if (actionId == -1)
        return;
    kit = ANI_l_Actor(srcGao)->actionKit;
    if (!kit)
        return;
    if (kit->savedActions)
    {
        action = kit->savedActions[actionId];
    }
    else
    {
        if (kit == (tActionKit*)-12)
            return;
        actions = (tAction**)(((char*)kit) + sizeof(tActionKit));
        action = actions[actionId];
    }
    if (!action || action == (tAction*)-4)
        return;
    actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));
    if (!actionElems->eventLists)
        return;
    if (!ANI_l_Actor(gao)->animPtr[0])
        ANI_l_Actor(gao)->animPtr[0] = ANI_AllocAnim();
    ++actionElems->eventLists->usageCount;
    ANI_SetTrackList(ANI_l_Actor(gao)->animPtr[0], actionElems->eventLists);
    ANI_l_Actor(gao)->flagsEgBlendIw2 = 1;
    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    OBJ_ReinitMatrixStorageByFlags(gao, mtx);
    ANI_l_Actor(gao)->animPtr[0]->frequency = 60;
    savedDt = TIM_gf_dt;
    TIM_gf_dt = 0.0;
    EVE_gb_ForcePlayerOnAllTracks = 1;
    ANI_PlayLeadAnimation(
        gao,
        gao->basic->actorObjectOrVisualObject,
        ANI_l_Actor(gao)->animPtr[0]);
    EVE_gb_ForcePlayerOnAllTracks = 0;
    TIM_gf_dt = savedDt;
    ANI_l_Actor(gao)->animPtr[0]->frequency = 0;
}