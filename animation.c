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


byte ANI_c_DefaultBitFieldLOD = 0x80;

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

void ANI_LimitFrequencyBetween0And255(tAnim* anim, Vec3* v1, Vec3* v2)
{
    float v1Sq, distRatio;


    v1Sq = MATH_VecLenSquared(v1);
    if (v1Sq == 0.0f)
    {
        anim->frequency = (byte)1.0f;
    }
    else
    {
        distRatio = (float)sqrt(MATH_VecLenSquared(v2) / v1Sq);

        if (distRatio >= 0.0f)
        {
            if (distRatio > 255.0f)
                distRatio = 255.0f;
            anim->frequency = (byte)distRatio;
        }
        else
        {
            anim->frequency = (byte)0.0f;
        }
    }
}

void ANI_ClearBlendingFlag(tGameObjectMain* gao)
{
    if (!gao) return;
    if ((gao->flags & 0x2) == 0) return;

    if (gao->basic && gao->basic->actorObjectOrVisualObject)
    {
        ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->flagsEgBlendIw2 &= ~0x2;
    }
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
        for (currElem = grp->tablePF->firstElemPtr; currElem <= ((tableElemStructPF*)(grp->tablePF->nextElemPtr)) - 1; currElem++)
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
            
            if ((gao->flags & 2) != 0)
            {
                if (gao->basic && gao->basic->actorObjectOrVisualObject && (gaoao->flagsEgBlendIw2 & 0x2) != 0)
                {
                    MATH_MulMatrixMatrix(&dest, &gaoao->animPtr[1]->flashMtx, &refInv);
                    MATH_CopyMatrix(&gaoao->animPtr[1]->flashMtx, &dest);
                }
            }
        }
    }
}

int ANI_GaoAnimationIsTransitioning(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGameObjectActorObject* v2;
    int result;

    if (a1 && (a1->flags & 2) != 0 && (v1 = a1->basic) != 0 && (v2 = v1->actorObjectOrVisualObject) != 0)
        result = v2->flagsEgBlendIw2 & 2;
    else
        result = 0;
    return result;
}

void ANI_RecomputeFlash(tGameObjectMain* a1, tAnim* a2)
{
    tGaoMatrix* v2;
    Mtx4x4* v3;
    tGaoMatrix* v4;
    int v5;
    unsigned __int16 v6;
    float v7;
    tGameObjectBasic* v8;
    Mtx4x4* v9;
    Mtx4x4* v10;
    Mtx4x4* v11;
    Mtx4x4* mtx;
    float v13;
    Mtx4x4 a2a;
    Mtx4x4 v15;
    Mtx4x4 a1a;

    if (a1)
    {
        if ((a1->flags & 0x400000) != 0)
        {
            v2 = a1->basic->mtx;
            if (&v15 != (Mtx4x4*)v2)
                memcpy(&v15, v2, sizeof(v15));
            mtx = &v15;
        }
        else
        {
            v3 = a1->matrices;
            if (&a2a != v3)
                memcpy(&a2a, v3, sizeof(a2a));
            mtx = v3;
            a1->matrices = &a2a;
        }
        EVE_gpst_OwnerGAO = a1;
        EVE_gpst_CurrentData = a2->ptr;
        EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
        EVE_gpst_CurrentRefMatrix = &MATH_gst_IdentityMatrix;
        ENG_gb_EVERunning = 0;
        if ((a1->flags & 0x400000) != 0)
            v4 = a1->basic->mtx;
        else
            v4 = (tGaoMatrix*)a1->matrices;
        v5 = v4->mtx.mtxflag;
        v4->mtx._m14 = 0.0;
        v4->mtx._m24 = 0.0;
        v4->mtx._m34 = 0.0;
        v4->mtx.mtxflag = v5 & ~8u;
        if (EVE_gpst_CurrentListTracks)
        {
            v6 = EVE_gpst_CurrentListTracks->num;
            EVE_gpst_CurrentListTracks->num = (EVE_gpst_CurrentListTracks->listElems[1].boneID == 0xFFFF) + 1;
            v7 = TIM_gf_dt;
            TIM_gf_dt = 0.0;
            EVE_gb_CanFlash = 0;
            v13 = v7;
            EVE_b_RunAllTracks(a2->ptr, 0);
            EVE_gb_CanFlash = 1;
            TIM_gf_dt = v13;
            EVE_gpst_CurrentListTracks->num = v6;
        }
        if ((a1->flags & 0x400000) != 0)
        {
            v8 = a1->basic;
            v9 = &v8->mtx->mtx;
            if (&a2a != v9)
                memcpy(&a2a, v9, sizeof(a2a));
            v10 = &v8->mtx->mtx;
            if (v10 != &v15)
                memcpy(v10, &v15, sizeof(Mtx4x4));
        }
        else
        {
            a1->matrices = mtx;
        }
        MATH_InvertMatrix(&a1a, &a2a);
        if (a2 == ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0])
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                v11 = a1->matrices;
                if ((a1->flags & 0x10000) != 0)
                    MATH_MulMatrixMatrix(v11 + 2, &a1a, mtx);
                else
                    MATH_MulMatrixMatrix(v11 + 1, &a1a, mtx);
            }
            else
            {
                MATH_MulMatrixMatrix(0, &a1a, mtx);
            }
        }
        else
        {
            MATH_MulMatrixMatrix(&a2->flashMtx, &a1a, mtx);
        }
    }
}

void ANI_UpdateFlash(struct sGameObjectMain* a1, char a2)
{
    tGameObjectMain* v2;
    tGameObjectBasic* v3;
    tGameObjectActorObject* v4;
    tAnim* v5;
    Mtx4x4* v6;
    tGameObjectEventsList* v7;
    tGameObjectMain* v8;
    tEventData* v9;
    tGameObjectEventsListElem* v10;
    tAnimTracks* v11;
    tGameObjectEventElem* v12;
    int v13;

    v2 = a1;
    if (a1)
    {
        if ((a1->flags & 2) != 0 && (a1->flags & 0x4000000) != 0)
        {
            v3 = a1->basic;
            if (v3)
            {
                v4 = v3->actorObjectOrVisualObject;
                if (v4)
                {
                    v5 = v4->animPtr[0];
                    if (v5)
                    {
                        v10 = EVE_gpst_CurrentTrack;
                        v11 = EVE_gpst_CurrentParam;
                        v6 = EVE_gpst_CurrentRefMatrix;
                        v12 = EVE_gpst_CurrentEvent;
                        v7 = EVE_gpst_CurrentListTracks;
                        v8 = EVE_gpst_OwnerGAO;
                        v13 = ENG_gb_EVERunning;
                        v9 = EVE_gpst_CurrentData;
                        if ((a2 & 1) != 0)
                        {
                            ANI_RecomputeFlash(a1, v5);
                            v2 = a1;
                        }
                        if (ANI_GaoAnimationIsTransitioning(v2))
                        {
                            if ((a2 & 2) != 0)
                                ANI_RecomputeFlash(a1, ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[1]);
                        }
                        EVE_gpst_CurrentData = v9;
                        EVE_gpst_OwnerGAO = v8;
                        EVE_gpst_CurrentListTracks = v7;
                        EVE_gpst_CurrentTrack = v10;
                        EVE_gpst_CurrentRefMatrix = v6;
                        EVE_gpst_CurrentParam = v11;
                        EVE_gpst_CurrentEvent = v12;
                        ENG_gb_EVERunning = v13;
                    }
                }
            }
        }
    }
}

uint8_t ANI_uc_GetEngineCanalByAICanal(struct sGameObjectMain* a1, char a2)
{
    tGameObjectActorObject* v2;
    tGroup* v3;
    tableDescStruct* v4;
    tableElemStructPF* v5;
    tableElemStructPF* v6;
    tableElemStructPF* v7;
    unsigned __int8 result;
    tShape* v9;

    if ((a1->flags & 2) == 0)
        return 0;
    v2 = a1->basic->actorObjectOrVisualObject;
    v3 = v2->skeletonGroup;
    if (!v3)
        return 0;
    v4 = v3->tablePF;
    if (!v3->tablePF)
        return 0;
    v9 = v2->shape;
    if (!v9)
        return 0;
    v5 = v4->nextElemPtr;
    v6 = v4->firstElemPtr;
    v7 = v5 - 1;
    result = 0;
    if (v6 > v7)
        return -1;
    while (v9->shapesOriginal[result] != a2)
    {
        ++v6;
        ++result;
        if (v6 > v7)
            return -1;
    }
    return result;
}

struct sGameObjectMain* ANI_pst_GetObjectByAICanal(struct sGameObjectMain* a1, uint8_t a2)
{
    tGameObjectActorObject* v2;
    tGroup* v3;
    tableDescStruct* v4;
    tShape* v5;
    tableElemStructPF* v6;
    tableElemStructPF* v7;
    tableElemStructPF* v8;
    int v9;

    if ((a1->flags & 2) == 0)
        return 0;
    v2 = a1->basic->actorObjectOrVisualObject;
    v3 = v2->skeletonGroup;
    if (!v3)
        return 0;
    v4 = v3->tablePF;
    if (!v3->tablePF)
        return 0;
    v5 = v2->shape;
    v6 = v4->nextElemPtr;
    v7 = v4->firstElemPtr;
    v8 = v6 - 1;
    v9 = 0;
    if (v7 > v8)
        return 0;
    while (1)
    {
        if (v7->objectPtr && ((int)v7->objectPtr & 3) != 1)
        {
            if (v5 ? v5->shapesOriginal[v9] == a2 : v9 == a2)
                break;
        }
        ++v7;
        ++v9;
        if (v7 > v8)
            return 0;
    }
    return (tGameObjectMain*)v7->objectPtr;
}

void ANI_ChangeBoneShape(struct sGameObjectMain* a1, unsigned __int8 a2, unsigned __int8 a3, int a4)
{
    tGameObjectBasic* v4;
    tGameObjectActorObject* v5;
    tGameObjectMain* v6;
    unsigned __int8 v7;
    tableDescStruct* v8;
    tGameObjectVisualObject* v9;
    tGameObjectExtended* v10;
    tGroup* v11;
    tableDescStruct* v12;
    tableElemStructPF* v13;
    tableElemStructPF* v14;
    unsigned __int8 v15;
    tGameObjectMain* v16;
    tGameObjectBasic* v17;
    tGameObjectVisualObject* v18;
    int* v19;
    unsigned int v20;
    int* v21;
    tGameObjectBasic* v22;
    tGameObjectVisualObject* v23;
    tGameObjectExtended* v24;
    tModifier* v25;
    tModifier* v26;
    tGroGeneric* v27;
    tGameObjectBasic* v28;
    tGameObjectMultiPartData* v29;
    unsigned int v30;
    unsigned __int8 v31;
    int v32;
    tMultiPartGameObjectLinks* v33;
    tGameObjectMultiPartData* v34;
    void* v35;
    tGameObjectBasic* v36;
    tGameObjectBasic* v37;
    tGameObjectVisualObject* v38;
    tGroGeneric* v39;
    tGroGeneric* v40;
    tGameObjectExtended* v41;
    tModifier* v42;
    tGroup* grp;
    tGameObjectMain* v44;

    if (a3 != 0xFF)
    {
        if (a1)
        {
            v4 = a1->basic;
            if (v4)
            {
                v5 = v4->actorObjectOrVisualObject;
                if (v4->actorObjectOrVisualObject)
                {
                    grp = v5->skeletonGroup;
                    if (a4)
                    {
                        v6 = ANI_pst_GetObjectByAICanal(a1, a2);
                        v7 = ANI_uc_GetEngineCanalByAICanal(a1, a2);
                        if (v7 == 0xFF)
                            return;
                        v5->shapes[v7] = a3;
                    }
                    else
                    {
                        v8 = v5->skeletonGroup->tablePF;
                        if (a2 >= v8->actElemNum)
                            return;
                        v6 = (tGameObjectMain*)((tableElemStructPF*)(v8->firstElemPtr))[a2].objectPtr;
                    }
                    if (v6 && !TAB_TableElemIsInvalid(v6))
                    {
                        if (a3 == 0xFE)
                        {
                            v6->flags2 &= ~0x20200000u;
                        }
                        else if (a3 == 0xFD)
                        {
                            v6->flags2 |= 0x20200000u;
                        }
                        else if ((v6->flags & 0x800000) != 0)
                        {
                            v9 = (tGameObjectVisualObject*)v6->basic->actorObjectOrVisualObject;
                            v10 = v6->extended;
                            if (v10)
                            {
                                v11 = v10->grp;
                                if (v11)
                                {
                                    v12 = v11->tablePF;
                                    v13 = v12->firstElemPtr;
                                    v14 = ((tableElemStructPF*)(v12->nextElemPtr)) - 1;
                                    v15 = 0;
                                    if (a3)
                                    {
                                        while (1)
                                        {
                                            ++v13;
                                            ++v15;
                                            if (v13 > v14)
                                                break;
                                            if (v15 >= a3)
                                                goto LABEL_22;
                                        }
                                    }
                                    else
                                    {
                                    LABEL_22:
                                        v16 = (tGameObjectMain*)v13->objectPtr;
                                        v44 = (tGameObjectMain*)v13->objectPtr;
                                        if (v13->objectPtr)
                                        {
                                            if (!v9)
                                                goto LABEL_35;
                                            if (v9->RLIVertexColors)
                                            {
                                                MEM_Free(v9->RLIVertexColors);
                                                v9->RLIVertexColors = 0;
                                            }
                                            v17 = v16->basic;
                                            if (v17)
                                            {
                                                v18 = (tGameObjectVisualObject*)v17->actorObjectOrVisualObject;
                                                if (v18)
                                                {
                                                    v19 = v18->RLIVertexColors;
                                                    if (v19)
                                                    {
                                                        v20 = 4 * *v19 + 4;
                                                        v21 = (int*)MEM_p_Alloc(v20);
                                                        v9->RLIVertexColors = v21;
                                                        memcpy(v21, ((tGameObjectVisualObject*)(v16->basic->actorObjectOrVisualObject))->RLIVertexColors, v20);
                                                        v16 = v44;
                                                    }
                                                }
                                            }
                                            v22 = v16->basic;
                                            if (!v22
                                                || (v23 = (tGameObjectVisualObject*)v22->actorObjectOrVisualObject) == 0
                                                || v23->GROMAT != v9->GROMAT
                                                || v23->GROGEOOrSomethingElse != v9->GROGEOOrSomethingElse
                                                || (v6->flags2 & 0x200000) != 0)
                                            {
                                            LABEL_35:
                                                v24 = v6->extended;
                                                if (v24)
                                                {
                                                    v25 = v24->mdf;
                                                    if (v25)
                                                    {
                                                        do
                                                        {
                                                            v26 = v25->nextElem;
                                                            MDF_Modifier_Destroy(v25);
                                                            v25 = v26;
                                                        } while (v26);
                                                        v6->extended->mdf = 0;
                                                    }
                                                }
                                                if (v9)
                                                {
                                                    if (v9->GROGEOOrSomethingElse)
                                                    {
                                                        v9->GROGEOOrSomethingElse->intf->AddRef(
                                                            v9->GROGEOOrSomethingElse,
                                                            -1);
                                                        v9->GROGEOOrSomethingElse = 0;
                                                    }
                                                    v27 = v9->GROMAT;
                                                    if (v27)
                                                    {
                                                        v27->intf->AddRef(v27, -1);
                                                        v9->GROMAT = 0;
                                                    }
                                                }
                                                v6->flags |= 0x4000u;
                                                v28 = v16->basic;
                                                if (v28)
                                                {
                                                    v29 = v28->multiPartData;
                                                    if (v29)
                                                    {
                                                        v30 = v29->elemNum;
                                                        if (v29->elemNum)
                                                        {
                                                            OBJ_GameObject_AllocateGizmo(v6, v30, 1);
                                                            memcpy(
                                                                v6->basic->multiPartData->multipartGaoObjectPartsOrMatrices,
                                                                v44->basic->multiPartData->multipartGaoObjectPartsOrMatrices,
                                                                4 * ((sizeof(tMultiPartGameObjectLinks) * v30) >> 2));
                                                            v31 = 0;
                                                            if (v30)
                                                            {
                                                                v32 = 0;
                                                                do
                                                                {
                                                                    //v33 = ((tMultiPartGameObjectLinks**)(v6->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v32];
                                                                    v33 = &((tMultiPartGameObjectLinks*)(v6->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v32];
                                                                    v33->gao = a1;
                                                                    ++v31;
                                                                    //v33->mtxPtr = ((Mtx4x4**)(a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v33->mtxIdx];
                                                                    v33->mtxPtr = &((Mtx4x4*)(a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v33->mtxIdx];
                                                                    v32 = v31;
                                                                } while (v31 < v30);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            v34 = v6->basic->multiPartData;
                                                            if (v34)
                                                            {
                                                                v35 = v34->multipartGaoObjectPartsOrMatrices;
                                                                if (v35)
                                                                {
                                                                    MEM_Free(v35);
                                                                    v6->basic->multiPartData->multipartGaoObjectPartsOrMatrices = 0;
                                                                }
                                                                MEM_Free(v6->basic->multiPartData);
                                                                v6->basic->multiPartData = 0;
                                                                v6->flags &= ~0x1200000u;
                                                            }
                                                        }
                                                        v16 = v44;
                                                    }
                                                }
                                                v36 = v6->basic;
                                                if (v36 && !v36->actorObjectOrVisualObject)
                                                    v6->flags &= ~0x4000u;
                                                OBJ_ChangeIdentityFlags(v6, v6->flags | 0x4000, v6->flags);
                                                v37 = v16->basic;
                                                if (v37)
                                                {
                                                    v38 = (tGameObjectVisualObject*)v37->actorObjectOrVisualObject;
                                                    if (v37->actorObjectOrVisualObject)
                                                    {
                                                        v39 = v38->GROGEOOrSomethingElse;
                                                        if (v38->GROGEOOrSomethingElse)
                                                        {
                                                            v39->intf->AddRef(v39, 1);
                                                            ((tGameObjectVisualObject*)(v6->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = v39;
                                                        }
                                                        v40 = v38->GROMAT;
                                                        if (v40)
                                                        {
                                                            v40->intf->AddRef(v40, 1);
                                                            ((tGameObjectVisualObject*)(v6->basic->actorObjectOrVisualObject))->GROMAT = v40;
                                                        }
                                                        ((tGameObjectVisualObject*)(v6->basic->actorObjectOrVisualObject))->objDrawMask = ((tGameObjectVisualObject*)(v16->basic->actorObjectOrVisualObject))->objDrawMask;
                                                    }
                                                }
                                                v41 = v16->extended;
                                                if (v41)
                                                {
                                                    v42 = v41->mdf;
                                                    if (v42)
                                                    {
                                                        MDF_Modifier_DuplicateList(v6, v42);
                                                        UpdateModifiersGroup(v6, grp);
                                                    }
                                                }
                                                v6->flags2 &= ~0x20200000u;
                                                OBJ_ComputeBV(v6, 0);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ANI_ApplyShape(tGameObjectMain* a1)
{
    tGameObjectMain* v1;
    tGameObjectBasic* v2;
    tGameObjectActorObject* v3;
    tGroup* v4;
    tableDescStruct* v5;
    tableElemStructPF* v6;
    tableElemStructPF* v7;
    uint8_t v8;
    tShape* v9;
    uint8_t i;

    v1 = a1;
    if (a1)
    {
        if ((a1->flags & 2) != 0)
        {
            v2 = a1->basic;
            if (v2)
            {
                v3 = v2->actorObjectOrVisualObject;
                if (v2->actorObjectOrVisualObject)
                {
                    v4 = v3->skeletonGroup;
                    if (v4)
                    {
                        if (v3->shape)
                        {
                            v5 = v4->tablePF;
                            v6 = v5->firstElemPtr;
                            v7 = ((tableElemStructPF*)(v5->nextElemPtr)) - 1;
                            v8 = 0;
                            for (i = 0; v6 <= v7; i = v8)
                            {
                                if (v6->objectPtr && ((int)v6->objectPtr & 3) != 1)
                                {
                                    v9 = v3->shape;
                                    if (v9 == v3->shape2)
                                        ANI_ChangeBoneShape(v1, i, v3->shapes[v8], 0);
                                    else
                                        ANI_ChangeBoneShape(v1, i, v9->shapes[v8], 0);
                                    v1 = a1;
                                }
                                ++v6;
                                ++v8;
                            }
                        }
                    }
                }
            }
        }
    }
}

void ANI_ChangeShape(struct sGameObjectMain* a1, struct sShape* a2)
{
    tGameObjectMain* v2;
    tGameObjectBasic* v3;
    tGameObjectActorObject* v4;
    tShape* v5;
    tShape* v6;
    int v7;
    int v8;
    int v9;
    tShape* v10;
    byte v11;
    byte* v12;
    char v13[64];

    v2 = a1;
    if ((a1->flags & 2) != 0)
    {
        v3 = a1->basic;
        if (v3)
        {
            v4 = v3->actorObjectOrVisualObject;
            if (v3->actorObjectOrVisualObject)
            {
                v5 = a2;
                if (a2 || (v5 = v4->shape2) != 0)
                {
                    v6 = v4->shape;
                    if (v6 != v5)
                    {
                        if (v6)
                        {
                            memcpy(v13, v6->shapesOriginal, sizeof(v13));
                            v2 = a1;
                            v7 = 1;
                        }
                        else
                        {
                            v7 = 0;
                        }
                        v4->shape = v5;
                        if (v7)
                        {
                            v8 = 0;
                            v9 = 64;
                            do
                            {
                                v10 = v4->shape;
                                v11 = v10->shapesOriginal[v8];
                                v12 = &v10->shapesOriginal[v8];
                                if (v11 == 0xFF)
                                    *v12 = v13[v8];
                                ++v8;
                                --v9;
                            } while (v9);
                        }
                        ANI_ApplyShape(v2);
                    }
                }
            }
        }
    }
}

void ANI_SetTrackList(struct sAnim* anims, struct sGameObjectEventsList* a2)
{
    tEventData* v2;
    tAnimTracks* v3;
    unsigned int i;
    tGameObjectEventsListElem* v5;
    tGameObjectEventElem* v6;

    if (anims)
    {
        v2 = anims->ptr;
        if (v2)
        {
            if (a2)
            {
                EVE_InitData(v2, a2);
                v3 = anims->ptr->tracks;
                for (i = 0; i < a2->num; ++i)
                {
                    v5 = a2->listElems;
                    a2->listElems[i].flags |= 3u;
                    v3[i].flag |= 1u;
                    if (!i)
                    {
                        v6 = v5->trackElems;
                        if (v6)
                            v6->flags2 |= 4u;
                    }
                }
            }
        }
    }
}

void ANI_ChangeAction(struct sGameObjectMain* a1, struct sGameObjectActorObject* a2)
{
    tAction* v2;
    int actionItem;
    byte v4;
    int v5;
    int v6;
    BOOL v7;
    tActionElem* v8;
    tAnim* v9;
    BOOL v10;
    byte v11;
    int v12;
    tActionElem *actionElems;

    v2 = a2->actionPtr;
    if (!v2)
        return;
    actionItem = a2->actionItem;
    v4 = a2->callCountForChangeAction + 1;
    a2->callCountForChangeAction = v4;
    actionElems = (tActionElem*)(((char*)v2) + sizeof(tAction));
    if (v4 < actionElems[actionItem].changeActionCallAmountToChange)
        return;
    a2->callCountForChangeAction = 0;
    v5 = v2->actionElemNum;
    v6 = actionItem + 1;
    if (actionItem + 1 >= v5)
    {
        if (a2->transitionElem)
        {
            v12 = a2->currentActionTransition;
            a2->currentActionTransition = -1;
            ACT_SetAction(a1, v12, 0);
            return;
        }
        v6 = v2->actionElemIDForTransition;
        if (v6 == 255)
            goto LABEL_15;
        if (v6 >= v5)
            v6 = v5 - 1;
    }
    if (v6 == 255 || !v2->actionElemNum)
    {
    LABEL_15:
        a2->animPtr[0]->flags |= 0x80u;
        return;
    }
    v7 = actionItem == v6;
    v8 = &(actionElems[v6]);
    v9 = a2->animPtr[0];
    a2->actionItem = v6;
    v10 = v7;
    ANI_SetTrackList(v9, v8->eventLists);
    ANI_ChangeShape(a1, v8->shape);
    a2->callCountForChangeAction = 0;
    v11 = v8->b5;
    if ((a1->flags & 4) != 0)
        a1->basic->dyn->flags ^= (a1->basic->dyn->flags ^ (v11 << 14)) & 0x1C0000;
    a2->animPtr[0]->flags = (a2->animPtr[0]->flags & 0xff00) | (a2->animPtr[0]->flags & 0xF0);
    a2->animPtr[0]->flags = (a2->animPtr[0]->flags & 0xff00) | (a2->animPtr[0]->flags & 0x7F);
    a2->animPtr[0]->flags |= v11 & 0xF;
    if (!v10)
        a2->animPtr[0]->frequency = 60 * v8->actionFrequency / 64;
}

void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* a1, Vec3* a2, char a3)
{
    tGameObjectBasic* v3;
    Mtx4x4* v4;
    tGameObjectBasic* v5;
    tGameObjectActorObject* v6;

    if (a1)
    {
        if ((a1->flags & 2) != 0)
        {
            v3 = a1->basic;
            if (v3)
            {
                if (v3->actorObjectOrVisualObject)
                {
                    if ((a3 & 1) != 0 && (a1->flags & 0x4000000) != 0)
                    {
                        v4 = OBJ_pst_GetFlashMatrix(a1);
                        MATH_VecAddVecWithoutBuffer(&v4->Pos, a2);
                    }
                    if ((a3 & 2) != 0)
                    {
                        if (ANI_GaoAnimationIsTransitioning(a1))
                        {
                            v5 = a1->basic;
                            v6 = v5->actorObjectOrVisualObject;
                            MATH_VecAddVecWithoutBuffer(&((tGameObjectActorObject*)(v5->actorObjectOrVisualObject))->animPtr[1]->flashMtx.Pos, a2);
                            MATH_VecAddVecWithoutBuffer((Vec3*)&v6->animPtr[0]->flashMtx._m14, a2); // TODO FIXME ez mi??? asm-et néztem ott is ez van
                            MATH_VecAddVecWithoutBuffer(&v6->animPtr[0]->flashMtx.Pos, a2);
                        }
                    }
                }
            }
        }
    }
}

void ANI_GetEndSpeedOfMB(struct sAnim* a1, Vec3* a2)
{
    tGameObjectEventsListElem* v3;
    int v4;
    int v5;
    tGameObjectEventElem* v6;
    __int16 v7;
    Vec3* v8;
    Vec3* v9;
    double v10;
    int a2a;

    a2->x = 0.0;
    a2->y = 0.0;
    a2->z = 0.0;
    EVE_gpst_CurrentListTracks = a1->ptr->eventListStruct;
    v3 = EVE_gpst_CurrentListTracks->listElems;
    v4 = 0;
    if (a1->ptr->eventListStruct->num)
    {
        do
        {
            ++v4;
            EVE_gpst_CurrentTrack = v3;
            a2a = v4;
            if (v3->boneID != 0xFFFF)
                break;
            v5 = v3->elemNum;
            v6 = &v3->trackElems[v5 - 1];
            if (v3->elemNum)
            {
                while (1)
                {
                    v7 = v6->flags2 & 0x7C0;
                    --v5;
                    if (v7 == 128)
                        break;
                    if (!v7)
                    {
                        --v6;
                        if (v5)
                            continue;
                    }
                    goto LABEL_10;
                }
                if ((EVE_w_Event_InterpolationKey_GetType(v6) & 3) != 0)
                {
                    if (v5 == v3->elemNum - 1)
                    {
                        v8 = EVE_pst_Event_InterpolationKey_GetPos(v6 - 1);
                        if (v8)
                        {
                            v9 = EVE_pst_Event_InterpolationKey_GetPos(v6);
                            a2->x = v9->x - v8->x;
                            a2->y = v9->y - v8->y;
                            a2->z = v9->z - v8->z;
                            v10 = (double)a1->frequency / (double)v6[-1].duration;
                            a2->x = (float)(v10 * a2->x);
                            a2->y = (float)(v10 * a2->y);
                            a2->z = (float)(v10 * a2->z);
                        }
                    }
                    EVE_gpst_CurrentTrack = 0;
                    return;
                }
                v4 = a2a;
            }
        LABEL_10:
            ++v3;
        } while (v4 < a1->ptr->eventListStruct->num);
    }
    EVE_gpst_CurrentTrack = 0;
}

void ANI_InterpolateMagicBoxPositionAfterEndOfAnim(tGameObjectMain* a1, tGameObjectActorObject* a2, tAnim* a3, float a4)
{
    Mtx4x4* v4;
    float v5;
    float v6;
    tGameObjectBasic* v7;
    tGaoMatrix* v8;
    tGameObjectMain* v9;
    Mtx4x4* v10;
    Vec3* v11;
    Vec3 a1a;
    Vec3 v13;
    Mtx4x4 a2a;

    v4 = &a2->animPtr[0]->flashMtx;
    if ((a3->flags2 & 2) != 0)
    {
        ANI_GetEndSpeedOfMB(a3, &a1a);
        MATH_TransformVector(&a1a, &a3->flashMtx, &a1a);
        v5 = a1a.y;
        v6 = a1a.z;
        v4->v1.x = a1a.x;
        v4->v1.y = v5;
        v4->v1.z = v6;
        a3->flags2 = (a3->flags2 - 2) | 4;
    }
    if ((a3->flags2 & 4) != 0)
    {
        a1a.x = 0.0;
        a1a.y = 0.0;
        a1a.z = 0.0;
        if (a4 != 0.0)
        {
            a1a.x = a4 * v4->v1.x;
            a1a.y = v4->v1.y * a4;
            a1a.z = v4->v1.z * a4;
        }
        if ((a1->flags & 0x400000) != 0)
        {
            v7 = a1->basic;
            v8 = v7->mtx;
            if (v7 && v8)
                v9 = v8->refGao2;
            else
                v9 = 0;
            v10 = OBJ_pst_GetAbsoluteMatrix(v9);
            MATH_InvertMatrix(&a2a, v10);
            MATH_TransformVector(&v13, &a2a, &a1a);
            v8->mtx.Pos.x = ANI_gst_PositionBeforeAnimPlayer.x + v13.x;
            v8->mtx.Pos.y = ANI_gst_PositionBeforeAnimPlayer.y + v13.y;
            v8->mtx.Pos.z = ANI_gst_PositionBeforeAnimPlayer.z + v13.z;
            ANI_UpdateFlashAfterMagicBoxTranslation(a1, &v13, 2);
        }
        else
        {
            v11 = &a1->matrices->Pos;
            v11->x = ANI_gst_PositionBeforeAnimPlayer.x + a1a.x;
            v11->y = ANI_gst_PositionBeforeAnimPlayer.y + a1a.y;
            v11->z = ANI_gst_PositionBeforeAnimPlayer.z + a1a.z;
            ANI_UpdateFlashAfterMagicBoxTranslation(a1, &a1a, 2);
        }
    }
}

void ANI_b_ComputeMagicBoxPositionForBlendAnim(tGameObjectMain* a1, tGameObjectActorObject* a2, tAnim* anims)
{
    tGameObjectMain* v3;
    tGameObjectBasic* v4;
    tGameObjectActorObject* v5;
    uint16_t v6;
    tAction* v7;
    byte v8;
    byte v9;
    byte v10;
    int v11;
    tEventData* v12;
    Mtx4x4* v13;
    float v14;
    byte v15;
    byte v16;
    tGaoMatrix* v17;
    float v18;
    Vec3* v19;
    float v20;
    float v21;
    int(*v22)(tEventData*, float*);
    float v23;
    tAction** actions;
    tActionElem* actionElems;

    v21 = TIM_gf_dt;
    v23 = TIM_gf_dt;
    if (TIM_gf_dt <= 0.0 || ENG_gb_EVERunning)
    {
        v3 = a1;
    LABEL_8:
        v22 = EVE_b_RunAllTracks;
        goto LABEL_9;
    }
    v3 = a1;
    v4 = a1->basic;
    if (!v4)
        goto LABEL_8;
    v5 = v4->actorObjectOrVisualObject;
    if (!v5 || !v5->skeletonGroup)
        goto LABEL_8;
    v22 = EVE_b_RunAnimation;
LABEL_9:
    if ((anims->flags2 & 6) != 0)
    {
        ANI_InterpolateMagicBoxPositionAfterEndOfAnim(v3, a2, anims, v21);
        TIM_gf_dt = v23;
        return;
    }
    EVE_gpst_OwnerGAO = v3;
    EVE_gpst_CurrentGAO = v3;
    EVE_gpst_CurrentData = anims->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    EVE_gpst_CurrentRefMatrix = &anims->flashMtx;
    v6 = EVE_gpst_CurrentListTracks->num;
    EVE_gpst_CurrentListTracks->num = (EVE_gpst_CurrentListTracks->listElems[1].boneID == 0xFFFF) + 1;
    while (1)
    {
        while (1)
        {
            TIM_gf_dt = (float)((double)anims->frequency * v21 * 0.016666668);
            if (v22(anims->ptr, &v21))
                break;
        LABEL_21:
            if (v21 == 0.0)
            {
                v14 = v23;
                EVE_gpst_CurrentListTracks->num = v6;
                TIM_gf_dt = v14;
                return;
            }
        }
        actions = (tAction**)(((char*)(a2->actionKit)) + sizeof(tActionKit));
        v7 = actions[anims->actionForTransition];
        v8 = anims->actionElemForTransition;
        if (v8 != 0xFF)
        {
            v9 = v8 + 1;
            anims->actionElemForTransition = v9;
            if (v9 >= v7->actionElemNum)
            {
                v10 = v7->actionElemIDForTransition;
                anims->actionElemForTransition = v10;
                if (v10 != 0xFF && v10 >= v7->actionElemNum)
                    anims->actionElemForTransition = v7->actionElemNum - 1;
            }
        }
        if (anims->actionElemForTransition == 0xFF)
            break;
        EVE_gpst_CurrentListTracks->num = v6;
        v11 = anims->ptr->tracks->privateData;
        actionElems = (tActionElem*)(((char*)v7) + sizeof(tAction));
        ANI_SetTrackList(anims, actionElems[anims->actionElemForTransition].eventLists);
        anims->ptr->tracks->privateData = v11;
        *(float*)&anims->ptr->tracks[1].privateData = v21;
        v12 = anims->ptr;
        anims->flags2 |= 1u;
        EVE_gpst_CurrentData = v12;
        EVE_gpst_CurrentListTracks = v12->eventListStruct;
        v6 = EVE_gpst_CurrentListTracks->num;
        EVE_gpst_CurrentListTracks->num = (EVE_gpst_CurrentListTracks->listElems[1].boneID == 0xFFFF) + 1;
        if (v21 == 0.0)
        {
            v13 = OBJ_pst_GetAbsoluteMatrix(v3);
            if (&anims->flashMtx != v13)
            {
                memcpy(&anims->flashMtx, v13, sizeof(anims->flashMtx));
                v3 = a1;
            }
            goto LABEL_21;
        }
    }
    v15 = anims->transitionFlags;
    v16 = anims->flags2;
    anims->flags |= 0x80u;
    anims->transitionFlags = v15 | 4;
    anims->flags2 = v16 | 2;
    if ((v3->flags & 0x400000) != 0)
        v17 = v3->basic->mtx;
    else
        v17 = (tGaoMatrix*)v3->matrices;
    v18 = v17->mtx.Pos.x;
    v19 = &v17->mtx.Pos;
    ANI_gst_PositionBeforeAnimPlayer.x = v18;
    ANI_gst_PositionBeforeAnimPlayer.y = v19->y;
    ANI_gst_PositionBeforeAnimPlayer.z = v19->z;
    ANI_InterpolateMagicBoxPositionAfterEndOfAnim(v3, a2, anims, v21);
    v20 = v23;
    EVE_gpst_CurrentListTracks->num = v6;
    TIM_gf_dt = v20;
}

void ANI_b_BlendMagicBox(tGameObjectMain* a1, tGameObjectActorObject* a2, tAnim* anims)
{
    int v3; // edx
    tGaoMatrix* v4; // ebx
    tGameObjectBasic* v5; // ecx
    tGameObjectActorObject* v6; // esi
    Vec3 a2a; // [esp+10h] [ebp-B0h] BYREF
    float v8; // [esp+1Ch] [ebp-A4h]
    Mtx4x4 v9; // [esp+20h] [ebp-A0h] BYREF
    Mtx4x4 v10; // [esp+70h] [ebp-50h] BYREF

    v3 = a1->flags;
    if ((a1->flags & 0x400000) != 0)
        v4 = a1->basic->mtx;
    else
        v4 = (tGaoMatrix*)a1->matrices;
    if (&v10 != (Mtx4x4*)v4)
        memcpy(&v10, v4, sizeof(v10));
    memcpy(&v9, &v10, sizeof(v9));
    if ((anims->transitionFlags & 8) != 0 || (anims->flags & 6) != 0)
    {
        a2a.x = v10.Pos.x - ANI_gst_PositionBeforeAnimPlayer.x;
        a2a.y = v10.Pos.y - ANI_gst_PositionBeforeAnimPlayer.y;
        a2a.z = v10.Pos.z - ANI_gst_PositionBeforeAnimPlayer.z;
        if ((v3 & 2) != 0)
        {
            v5 = a1->basic;
            if (v5)
            {
                v6 = v5->actorObjectOrVisualObject;
                if (v5->actorObjectOrVisualObject)
                {
                    if (ANI_GaoAnimationIsTransitioning(a1))
                    {
                        MATH_VecAddVecWithoutBuffer(&v6->animPtr[1]->flashMtx.Pos, &a2a);
                        MATH_VecAddVecWithoutBuffer((Vec3*)&(v6->animPtr[0]->flashMtx._m14), &a2a); // TODO FIXME ez mi????? asmbe neztem ott is ez van
                        MATH_VecAddVecWithoutBuffer(&v6->animPtr[0]->flashMtx.Pos, &a2a);
                    }
                }
            }
        }
    }
    else
    {
        v8 = TIM_gf_dt;
        ANI_b_ComputeMagicBoxPositionForBlendAnim(a1, a2, anims);
        MATH_BlendVector(&v9.Pos, &v4->mtx.Pos, &v10.Pos, *(float*)&(anims->ptr->tracks->privateData));
        a2a.x = v9.Pos.x - v10.Pos.x;
        a2a.y = v9.Pos.y - v10.Pos.y;
        a2a.z = v9.Pos.z - v10.Pos.z;
        ANI_UpdateFlashAfterMagicBoxTranslation(a1, &a2a, 1);
        a2a.x = v9.Pos.x - v4->mtx.Pos.x;
        a2a.y = v9.Pos.y - v4->mtx.Pos.y;
        a2a.z = v9.Pos.z - v4->mtx.Pos.z;
        ANI_UpdateFlashAfterMagicBoxTranslation(a1, &a2a, 2);
        if (v4 != (tGaoMatrix*)&v9)
            memcpy(v4, &v9, 0x50u);
        TIM_gf_dt = v8;
    }
}

void ANI_PlayLeadAnimation(tGameObjectMain* a1, tGameObjectActorObject* a2, tAnim* a3)
{
    tAnim* v3; // edi
    Mtx4x4* v4; // eax
    Mtx4x4* v5; // eax
    tGameObjectBasic* v6; // eax
    tGameObjectActorObject* v7; // eax
    tGroup* v8; // ecx
    tGameObjectActorObject* v9; // esi
    Mtx4x4* v10; // eax
    Mtx4x4* v11; // eax
    tGameObjectBasic* v12; // eax
    tGameObjectBasic* v13; // eax
    tGameObjectActorObject* v14; // eax
    unsigned __int16 v15; // dx
    Mtx4x4* v16; // eax
    unsigned __int16 v17; // ax
    Mtx4x4* v18; // eax
    double v19; // st7
    Mtx4x4* v20; // eax
    Mtx4x4* v21; // eax
    float a2a; // [esp+0h] [ebp-74h]
    byte v23; // [esp+13h] [ebp-61h]
    float v24; // [esp+14h] [ebp-60h] BYREF
    float v25; // [esp+18h] [ebp-5Ch]
    int(__cdecl * v26)(tEventData*, float*); // [esp+1Ch] [ebp-58h]
    unsigned int v27; // [esp+20h] [ebp-54h]
    char v28[80]; // [esp+24h] [ebp-50h] BYREF

    v3 = a3;
    EVE_gpst_OwnerGAO = a1;
    EVE_gpst_CurrentData = a3->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    if ((a1->flags & 0x4000000) != 0)
    {
        v4 = a1->matrices;
        if ((a1->flags & 0x10000) != 0)
            v5 = v4 + 2;
        else
            v5 = v4 + 1;
    }
    else
    {
        v5 = 0;
    }
    EVE_gpst_CurrentRefMatrix = v5;
    if (TIM_gf_dt <= 0.0
        || ENG_gb_EVERunning
        || (v6 = a1->basic) == 0
        || (v7 = v6->actorObjectOrVisualObject) == 0
        || (v8 = v7->skeletonGroup, v26 = EVE_b_RunAnimation, !v8))
    {
        v26 = EVE_b_RunAllTracks;
    }
    v24 = TIM_gf_dt;
    v25 = TIM_gf_dt;
    while (1)
    {
        v9 = a2;
        v27 = v3->frequency;
        TIM_gf_dt = (float)((double)(int)v27 * v24 * 0.016666668);
        if (a2->eventRunningFrequency)
            TIM_gf_dt = a2->elapsedDT;
        if ((a1->flags & 0x4000000) != 0)
        {
            v10 = a1->matrices;
            if ((a1->flags & 0x10000) != 0)
                v11 = v10 + 2;
            else
                v11 = v10 + 1;
        }
        else
        {
            v11 = 0;
        }
        EVE_gpst_CurrentRefMatrix = v11;

        if ((~((v3->flags & 0xff) >> 7) & 1) != 0)
        {
            if (v26(v3->ptr, &v24))
            {
                v27 = v3->frequency;
                v12 = a1->basic;
                v24 = (float)(v24 * 60.0 / (double)(int)v27);
                ANI_ChangeAction(a1, v12->actorObjectOrVisualObject);
                if ((a1->flags & 2) != 0)
                {
                    v13 = a1->basic;
                    if (v13)
                    {
                        v14 = v13->actorObjectOrVisualObject;
                        if (v14)
                        {
                            if ((v14->flagsEgBlendIw2 & 2) != 0 && v24 != 0.0)
                            {
                                *(float*)&a2->animPtr[1]->ptr->tracks[1].privateData = v24;
                                a2->animPtr[1]->flags2 |= 1u;
                            }
                        }
                    }
                }
            }
        }

        v23 = a2->eventRunningFrequency;
        if (v23)
        {
            v15 = a2->currentActionTransition;
            a2->eventRunningFrequency = 0;
            a2->currentActionTransition = -1;
            v16 = a1->matrices;
            if (v28 != (char*)v16)
            {
                memcpy(v28, v16, sizeof(v28));
                v3 = a3;
                v9 = a2;
            }
            ACT_SetAction(a1, v15, 0);
            v17 = v9->currTime;
            if (v17)
            {
                v27 = v17;
                a2a = (float)((double)v17 * 0.016666668);
                EVE_SetTracksTime(v9->animPtr[0]->ptr, a2a);
            }
            if (v23 != 0xFF)
                v9->animPtr[0]->frequency = v23;
            v18 = a1->matrices;
            if (v18 != (Mtx4x4*)v28)
            {
                memcpy(v18, v28, sizeof(Mtx4x4));
                v3 = a3;
                v9 = a2;
            }
            ANI_UpdateFlash(a1, 3);
            v19 = v25 - v9->elapsedDT;
            v9->elapsedDT = 0.0;
            v24 = (float)v19;
        }
        EVE_gpst_CurrentData = v3->ptr;
        EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
        if ((a1->flags & 0x4000000) != 0)
        {
            v20 = a1->matrices;
            v21 = (a1->flags & 0x10000) != 0 ? &v20[2] : &v20[1];
        }
        else
        {
            v21 = 0;
        }
        EVE_gpst_CurrentRefMatrix = v21;
        if (v24 == 0.0)
            break;
        if ((v3->flags & 0x80) != 0)
        {
            TIM_gf_dt = v25;
            return;
        }
    }
    TIM_gf_dt = v25;
}

void ANI_PlayGameObjectAnimation(tGameObjectMain* a1, tGameObjectActorObject* a2, float a3, Vec3* a4)
{
    tGaoMatrix* v5; // eax
    float v6; // edi
    Vec3* v7; // eax
    Mtx4x4* v8; // eax
    float v9; // ecx
    float v10; // ebx
    float v11; // ebp
    tAnim* v12; // edx
    tGameObjectBasic* v13; // eax
    tGameObjectActorObject* v14; // eax
    tAnim* v15; // edi
    double v16; // st7
    double v17; // st6
    int v18; // ecx
    tGaoMatrix* v19; // eax
    double v20; // st7
    Vec3* v21; // eax
    tGaoMatrix* v22; // eax
    Vec3* v23; // eax
    tGaoMatrix* v24; // eax
    float* v25; // eax
    tGameObjectBasic* v26; // edx
    float v27; // [esp+0h] [ebp-34h]
    float v28; // [esp+14h] [ebp-20h]
    float v29; // [esp+18h] [ebp-1Ch]
    Vec3 a1a; // [esp+1Ch] [ebp-18h] BYREF
    Vec3 a3a; // [esp+28h] [ebp-Ch] BYREF
    tAnim* v32; // [esp+38h] [ebp+4h]

    if ((a1->flags & 0x400000) != 0)
    {
        v5 = a1->basic->mtx;
        v6 = v5->mtx.Pos.z;
        v7 = &v5->mtx.Pos;
        a3a.z = v6;
    }
    else
    {
        v8 = a1->matrices;
        v9 = v8->Pos.z;
        v7 = &v8->Pos;
        a3a.z = v9;
        v6 = v9;
    }
    v10 = v7->x;
    v11 = v7->y;
    ANI_gst_PositionBeforeAnimPlayer.x = v10;
    ANI_gst_PositionBeforeAnimPlayer.y = v11;
    ANI_gst_PositionBeforeAnimPlayer.z = v6;
    v29 = TIM_gf_dt;
    TIM_gf_dt = a3;
    v12 = a2->animPtr[0];
    a3a.x = v10;
    a3a.y = v11;
    v32 = v12;
    if (v12 && v12 != (tAnim*)-1)
    {
        if ((~((unsigned __int16)v12->flags >> 7) & 1) != 0 && a4 && (v12->flags & 0xF) == 2)
        {
            MATH_VecMultScalar(&a1a, a4, a3);
            ANI_LimitFrequencyBetween0And255(v32, &a1a, a4);
            v12 = v32;
        }
        if ((~((v12->flags & 0xff) >> 7) & 1) != 0 || a2->eventRunningFrequency)
        {
            ANI_PlayLeadAnimation(a1, a2, v12);
            v12 = v32;
        }
        if ((a1->flags & 2) != 0)
        {
            v13 = a1->basic;
            if (v13)
            {
                v14 = v13->actorObjectOrVisualObject;
                if (v14)
                {
                    if ((v14->flagsEgBlendIw2 & 2) != 0)
                    {
                        v15 = a2->animPtr[1];
                        if ((v15->flags2 & 1) != 0)
                            TIM_gf_dt = *(float*)&v15->ptr->tracks[1].privateData;
                        v16 = (double)(v15->actionLength + 1) * 0.016666668;
                        v17 = (double)v15->startTime * 0.00006510417 + TIM_gf_dt;
                        v28 = (float)v17;
                        if (v17 >= v16)
                        {
                            ANI_ClearBlendingFlag(a1);
                        }
                        else
                        {
                            *(float*)&v15->ptr->tracks->privateData = (float)(v28 / v16);
                            v15->startTime = (uint16_t)(v28 * 15360.0); // TODO: ez igy jo ?
                            if (a4 && (v15->flags & 0xF) == 2)
                            {
                                MATH_VecMultScalar(&a1a, a4, TIM_gf_dt);
                                ANI_LimitFrequencyBetween0And255(v15, &a1a, a4);
                            }
                            ANI_b_BlendMagicBox(a1, a2, v15);
                        }
                        v6 = a3a.z;
                        v12 = v32;
                    }
                }
            }
        }
        if ((a1->flags & 0x400000) != 0)
        {
            OBJ_ComputeGlobalWhenHie(a1);
            v12 = v32;
        }
        v18 = a1->flags & 0x400000;
        if (v18)
            v19 = a1->basic->mtx;
        else
            v19 = (tGaoMatrix*)a1->matrices;
        v20 = v19->mtx.Pos.x;
        v21 = &v19->mtx.Pos;
        a1a.x = (float)(v20 - a3a.x);
        a1a.y = v21->y - a3a.y;
        a1a.z = v21->z - a3a.z;
        if ((v12->flags & 0xF) == 6)
        {
            if (v18)
                v22 = a1->basic->mtx;
            else
                v22 = (tGaoMatrix*)a1->matrices;
            v23 = &v22->mtx.Pos;
            v23->x = v10;
            v23->y = v11;
            v23->z = v6;
            a3a.x = -a1a.x;
            a3a.y = -a1a.y;
            a3a.z = -a1a.z;
            ANI_UpdateFlashAfterMagicBoxTranslation(a1, &a3a, 3);
            v12 = v32;
        }
        if ((v12->flags & 0xF) == 0 && a4 && a3 != 0.0)
        {
            if ((a1->flags & 0x400000) != 0 && (OBJ_ComputeGlobalWhenHie(a1), (a1->flags & 0x400000) != 0))
                v24 = a1->basic->mtx;
            else
                v24 = (tGaoMatrix*)a1->matrices;
            v25 = &v24->mtx.Pos.x;
            *v25 = v10;
            v25[1] = v11;
            v25[2] = v6;
            MATH_VecInvert(&a3a, &a1a);
            ANI_UpdateFlashAfterMagicBoxTranslation(a1, &a3a, 3);
            if ((a1->flags & 0x400000) != 0)
            {
                a3a.y = a1a.y;
                v26 = a1->basic;
                a3a.x = a1a.x;
                a3a.z = a1a.z;
                MATH_TransformVector(&a1a, v26->mtx->refGao2->matrices, &a3a);
            }
            v27 = (float)(1.0 / a3);
            MATH_VecMultScalar(a4, &a1a, v27);
            DYN_SetHorizontalSpeedComponent(a1->basic->dyn, a4);
        }
        TIM_gf_dt = v29;
    }
}

void ANI_OneCall(struct sGameObjectMain* a1)
{
    tGameObjectMain* v1;
    tDyn* v2;
    double v3;
    Vec3* v4;
    int v5;
    tGameObjectBasic* v6;
    tGameObjectActorObject* v7;
    tAnim* v8;
    tEventData* v9;
    tGameObjectBasic* v10;
    float a4;
    Vec3 v12;

    v1 = a1;
    if ((a1->flags & 4) != 0)
    {
        v2 = a1->basic->dyn;
        v3 = v2->mass;
        v4 = &v12;
        v12.x = (float)(v3 * v2->forceVector.x);
        v12.y = (float)(v2->forceVector.y * v3);
        v12.z = (float)(v2->forceVector.z * v3);
    }
    else
    {
        v4 = 0;
    }
    v5 = a1->flags2;
    if ((v5 & 2) != 0 && (v5 & 4) == 0)
    {
        v6 = a1->basic;
        if (v6)
        {
            v7 = v6->actorObjectOrVisualObject;
            if (v6->actorObjectOrVisualObject)
            {
                v8 = v7->animPtr[0];
                if (v8)
                {
                    if (!v7->initedInANIMainCall)
                    {
                        v9 = v8->ptr;
                        EVE_GetTracksTime(v9, 0, (float*)&a1, &a4);
                        EVE_SetTracksTime(v9, *(float*)&a1);
                        v10 = v1->basic;
                        if (((tGameObjectActorObject*)(v10->actorObjectOrVisualObject))->flagsEgBlendIw2 != 1)
                            ((tGameObjectActorObject*)(v10->actorObjectOrVisualObject))->flagsEgBlendIw2 = 1;
                        ((tGameObjectActorObject*)(v1->basic->actorObjectOrVisualObject))->initedInANIMainCall = 1;
                    }
                }
            }
        }
    }

    ANI_PlayGameObjectAnimation(v1, v1->basic->actorObjectOrVisualObject, TIM_gf_dt, v4);
}


void ANI_InitSkeleton(tGroup* a1, tGameObjectMain* a2)
{
    tableElemStructPF* v2;
    tGameObjectMain* v3;
    tGaoMatrix* v4;
    Mtx4x4* v6;
    tGaoMatrix* v7;
    tGaoMatrix* v8;
    int v9;
    int v10;
    tMultiPartGameObjectLinks* v12;
    Mtx4x4* matrices;
    BOOL v13;
    tableElemStructPF* v14;
    tableElemStructPF* v15;
    int a1a;

    v2 = a1->tablePF->firstElemPtr;
    v15 = v2;
    v14 = ((tableElemStructPF*)(a1->tablePF->nextElemPtr)) - 1;
    a1a = 0;
    if (v2 <= v14)
    {
        do
        {
            v3 = (tGameObjectMain*)v2->objectPtr;
            if (v2->objectPtr && ((uint8_t)v3 & 3) != 1)
            {
                if ((v3->flags & 0x400000) == 0)
                    OBJ_ChangeIdentityFlags(v3, v3->flags | 0x400000, v3->flags);
                v4 = v3->basic->mtx;
                if (!v4->refGao2)
                {
                    v4->refGao = a2;
                    v3->basic->mtx->refGao2 = a2;
                    if (!a1a && (v3->flags & 0x10000) == 0)
                    {
                        OBJ_ChangeIdentityFlags(v3, v3->flags | 0x10000, v3->flags);
                        v6 = v3->matrices + 1;
                        if (v6 != &MATH_gst_IdentityMatrix)
                            memcpy(v6, &MATH_gst_IdentityMatrix, sizeof(Mtx4x4));
                        v3->matrices[1].Pos = MATH_gst_NulVector;
                    }
                    v7 = v3->basic->mtx;
                    v8 = (tGaoMatrix*)&v3->matrices[1];
                    if (v7 != v8)
                        memcpy(v7, v8, 0x50u);
                }
                v9 = v3->flags | 1;
                v3->flags = v9;
                if ((v9 & 0x200000) != 0)
                {
                    v10 = 0;
                    if (v3->basic->multiPartData->elemNum)
                    {
                        v12 = (tMultiPartGameObjectLinks*)(v3->basic->multiPartData->multipartGaoObjectPartsOrMatrices);
                        while (1)
                        {
                            matrices = (Mtx4x4*)(a2->basic->multiPartData->multipartGaoObjectPartsOrMatrices);
                            v12[v10].gao = a2;
                            v12[v10].mtxPtr = &matrices[v12[v10].mtxIdx]; // bug fixed: broken actors; v12[v10].mtxPtr = &matrices[v12[->mtxIdx];
                            ++v10;
                            if (v10 >= v3->basic->multiPartData->elemNum)
                                break;
                        }
                    }
                }
            }
            v2 = v15 + 1;
            v13 = &v15[1] <= v14;
            ++a1a;
            ++v15;
        } while (v13);
    }
}

void ANI_pst_DuplicateGameObjectAnim(struct sWorld* a1, struct sGameObjectMain* a2, struct sGameObjectMain* a3)
{
    tGameObjectActorObject* v3; // ebx
    tGameObjectActorObject* v4; // ebp
    BOOL v5; // zf
    tAnim* v6; // eax
    tGroup* newGroup; // esi
    tGameObjectMain* ggao; // edi
    tShape* v9; // ecx

    v3 = (tGameObjectActorObject*)a3->basic->actorObjectOrVisualObject;
    v4 = (tGameObjectActorObject*)a2->basic->actorObjectOrVisualObject;
    if (v3->actionKit)
        ACT_ActionRestoreInit(v3->actionKit);
    memcpy(v4, v3, sizeof(tGameObjectActorObject));
    v5 = (v4->flagsEgBlendIw2 & ~0xFFFFFF02) == 0;
    v4->flagsEgBlendIw2 &= ~2u;
    v4->animPtr[1] = 0;
    v4->currentAction = -1;
    v4->actionItem = -1;
    if (!v5)
    {
        v6 = ANI_AllocAnim();
        v4->animPtr[0] = v6;
        v6->frequency = v3->animPtr[0]->frequency;
        v4->animPtr[0]->actionElemForTransition = 0;
        v4->animPtr[0]->transitionFlags = 0;
        v4->animPtr[0]->actionLength = 0;
        v4->animPtr[0]->flags2 = 0;
        v4->animPtr[0]->startTime = 0;
        v4->animPtr[0]->actionForTransition = 0;
        v4->animPtr[0]->flags = v3->animPtr[0]->flags;
        ANI_SetTrackList(v4->animPtr[0], v3->animPtr[0]->ptr->eventListStruct);
    }
    if (v3->group)
    {
        newGroup = GRP_pst_CreateNewGroup();
        GRP_DuplicateGroupObjects(a1, v3->group, newGroup);
        ggao = a2;
        UpdateModifiersGroup(a2, newGroup);
        v4->skeletonGroup = newGroup;
        ++newGroup->groupElemNum;
        ++v4->group->groupElemNum;
        ANI_InitSkeleton(v4->skeletonGroup, a2);
    }
    else
    {
        ggao = a2;
    }
    if (v3->shape)
    {
        v9 = v3->shape2;
        v4->shape2 = v9;
        v4->shape = v3->shape;
        ++v9->usageCount;
        ANI_ApplyShape(ggao);
    }
}

struct sShape* ANI_ul_ShapeCallback(uint32_t Offset)
{
    tShape* v1;
    unsigned char v2;
    int v3;
    char* v6;
    int v7;
    int id;

    v6 = BIG_pc_ReadFileTmp(Offset, &v7);
    v1 = (tShape*)MEM_p_Alloc(sizeof(tShape));
    memset(v1, 0xFFu, sizeof(tShape));
    v1->usageCount = 0;
    v2 = LOA_ReadChar(&v6, 0, 1);
    v1->num = (uint8_t)v2;
    if ((uint8_t)v2 <= 64u)
        v2 = (uint8_t)v2;
    else
        v2 = 64;
    v3 = 0;
    v1->num = v2;
    if (v2)
    {
        do
        {
            id = LOA_ReadChar(&v6, 0, 1);
            v1->shapesOriginal[id] = LOA_ReadChar(&v6, 0, 1);
            v1->shapes[id] = LOA_ReadChar(&v6, 0, 1);
            ++v3;
        } while (v3 < v1->num);
    }
    return v1;
}

void ANI_AttachSkeletonModelToWorld(tGroup* a1, tWorld* a2)
{
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    tGameObjectMain* v4;
    tableDescStruct* v5;
    tableElemStructPF* v6;
    tableElemStructPF* i;
    tGameObjectMain* v8;
    BOOL v9;
    tableElemStructPF* v10;
    tableElemStructPF* v11;

    v3 = ((tableElemStructPF*)(a1->tablePF->nextElemPtr)) - 1;
    v11 = a1->tablePF->firstElemPtr;
    v2 = v11;
    v10 = v3;
    if (v11)
    {
        while (v2 <= v3)
        {
            v4 = (tGameObjectMain*)v2->objectPtr;
            if (v2->objectPtr)
            {
                if (((uint8_t)v4 & 3) != 1
                    && TAB_ul_PFtable_GetElemIndexWithPointer(&a2->objectTablePF, v2->objectPtr) == -1)
                {
                    v4->flags |= 1u;
                    WOR_World_JustAfterLoadObject(a2, v4, 0, 1);
                    if ((v4->flags & 0x800000) != 0)
                    {
                        v5 = v4->extended->grp->tablePF;
                        v6 = v5->firstElemPtr;
                        for (i = ((tableElemStructPF*)(v5->nextElemPtr)) - 1; v6; ++v6)
                        {
                            if (v6 > i)
                                break;
                            v8 = (tGameObjectMain*)v6->objectPtr;
                            if (v6->objectPtr
                                && ((uint8_t)v8 & 3) != 1
                                && TAB_ul_PFtable_GetElemIndexWithPointer(&a2->objectTablePF, v6->objectPtr) == -1)
                            {
                                v8->flags |= 1u;
                                WOR_World_JustAfterLoadObject(a2, v8, 1, 1);
                            }
                        }
                        v2 = v11;
                    }
                }
            }
            v9 = &v2[1] == 0;
            v11 = ++v2;
            if (v9)
                break;
            v3 = v10;
        }
    }
}

void ANI_pst_Load(int animation, int shape, int group, tGameObjectMain* gao, tWorld* world)
{
    tGameObjectMain* v5;
    tGameObjectActorObject* gaoActorObject;
    tGroup* v7;
    tAnim* v8;
    int v9;
    Mtx4x4* v10;
    tGroup* groupPtr;
    tShape* shapePtr;
    tGameObjectEventsList* animationPtr;

    v5 = gao;
    gaoActorObject = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
    gaoActorObject->initedInANIMainCall = 1;
    gaoActorObject->shape = 0;
    gaoActorObject->skeletonGroup = 0;
    gaoActorObject->group = 0;
    gaoActorObject->skeletonGroupToRestoreOnReinit = 0;
    v7 = 0;
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
        v7 = GRP_pst_CreateNewGroup();
        ANI_AttachSkeletonModelToWorld(groupPtr, world);
        GRP_DuplicateGroupObjects(world, groupPtr, v7);
        UpdateModifiersGroup(gao, v7);
        ++v7->groupElemNum;
        ++groupPtr->groupElemNum;
        gaoActorObject->group = groupPtr;
        gaoActorObject->skeletonGroup = v7;
    }
    if (shapePtr)
    {
        ++shapePtr->usageCount;
        gaoActorObject->shape = shapePtr;
        gaoActorObject->shape2 = shapePtr;
        memcpy(gaoActorObject->shapes, shapePtr->shapes, sizeof(gaoActorObject->shapes));
        ANI_ApplyShape(gao);
        v5 = gao;
    }
    if (animation == -1 || gaoActorObject->actionKit)
    {
        gaoActorObject->animPtr[0] = 0;
        if (!groupPtr)
            goto LABEL_18;
        v9 = v7->tablePF->actElemNum;
    }
    else
    {
        LOA_MakeFileRef(animation, &animationPtr, EVE_ul_CallbackLoadListTracks, 1);
        LOA_Resolve();
        v8 = ANI_AllocAnim();
        gaoActorObject->animPtr[0] = v8;
        ANI_SetTrackList(v8, animationPtr);
        gaoActorObject->flagsEgBlendIw2 = 1;
        if (!groupPtr)
            goto LABEL_18;
        v9 = v7->tablePF->actElemNum;
    }
    if ((v5->flags & 0x200000) == 0 || v5->basic->multiPartData->elemNum >> 1 < v9)
        OBJ_GameObject_AllocateGizmo(v5, v9, 0);
LABEL_18:
    v10 = OBJ_pst_GetAbsoluteMatrix(v5);
    OBJ_ReinitMatrixStorageByFlags(v5, v10);
    if (groupPtr)
    {
        ANI_InitSkeleton(v7, v5);
        UpdateModifiersGroup(v5, v7);
    }
}

void ANI_ResolveRefs(struct sGameObjectMain* gao)
{
    ANI_pst_Load(
        (int)((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[0], // ezek még csak key-ek azért castolunk int-re
        (int)((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->shape,
        (int)((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->group,
        gao, WOR_gpst_CurrentWorld);
}

void ANI_ReinitAnimation(struct sAnim* a1)
{
    tEventData* v1;

    if (a1)
    {
        v1 = a1->ptr;
        if (v1)
            EVE_ReinitData(v1);
    }
}

void ANI_Reinit(struct sGameObjectMain* a1)
{
    tGameObjectActorObject* v1;
    byte v2;
    unsigned int v3;
    tAnim** v4;
    tActionKit* v5;
    tShape* v6;
    tGroup* v7;

    v1 = (tGameObjectActorObject*)a1->basic->actorObjectOrVisualObject;
    if (v1)
    {
        v2 = ANI_c_DefaultBitFieldLOD;
        v1->transitionElem = 0;
        v1->currentAction = -1;
        v1->actionItem = 0;
        v1->callCountForChangeAction = 0;
        v1->currentActionTransition = -1;
        v1->lod = 0;
        v1->aniBitFieldLOD = v2;
        v3 = 0;
        v4 = v1->animPtr;
        do
        {
            if (((uint8_t)(1 << v3) & v1->flagsEgBlendIw2) != 0)
                ANI_ReinitAnimation(*v4);
            ++v3;
            ++v4;
        } while (v3 < 4);
        v5 = v1->actionKit;
        v1->flagsEgBlendIw2 &= 1u;
        if (v5)
            ACT_ActionRestoreInit(v5);
        v6 = v1->shape2;
        if (v6)
        {
            v1->shape = v6;
            ANI_ApplyShape(a1);
        }
        v7 = v1->skeletonGroupToRestoreOnReinit;
        if (v7)
        {
            v1->skeletonGroup = v7;
            v1->skeletonGroupToRestoreOnReinit = 0;
        }
    }
}

void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* a1)
{
    int v1;
    tGameObjectBasic* v2;
    tGameObjectActorObject* v3;
    Mtx4x4* v4;
    Mtx4x4* v5;
    tGameObjectBasic* v6;
    Mtx4x4* v7;
    Mtx4x4* v8;
    Mtx4x4* v9;
    tGameObjectActorObject* v10;
    tGameObjectMain* v11;
    Mtx4x4 a2;

    v1 = a1->flags;
    if ((a1->flags & 2) != 0 && (v1 & 0x400000) != 0)
    {
        v2 = a1->basic;
        v3 = v2->actorObjectOrVisualObject;
        v10 = v2->actorObjectOrVisualObject;
        if (((tGameObjectActorObject*)(v2->actorObjectOrVisualObject))->animPtr[0])
        {
            v11 = v2->mtx->refGao2;
            if ((v1 & 0x4000000) != 0)
            {
                v4 = OBJ_pst_GetFlashMatrix(a1);
                if (&a2 != v4)
                {
                    memcpy(&a2, v4, sizeof(a2));
                    v3 = v10;
                }
                v9 = OBJ_pst_GetAbsoluteMatrix(v11);
                v5 = OBJ_pst_GetFlashMatrix(a1);
                MATH_MulMatrixMatrix(v5, &a2, v9);
            }
            if ((a1->flags & 2) != 0)
            {
                v6 = a1->basic;
                if (v6)
                {
                    if (v6->actorObjectOrVisualObject && (((tGameObjectActorObject*)(v6->actorObjectOrVisualObject))->flagsEgBlendIw2 & 2) != 0)
                    {
                        v7 = &v3->animPtr[1]->flashMtx;
                        if (&a2 != v7)
                        {
                            memcpy(&a2, v7, sizeof(a2));
                            v3 = v10;
                        }
                        v8 = OBJ_pst_GetAbsoluteMatrix(v11);
                        MATH_MulMatrixMatrix(&v3->animPtr[1]->flashMtx, &a2, v8);
                    }
                }
            }
        }
    }
}

struct sGameObjectMain* ANI_pst_GetReference(struct sGameObjectMain* a1)
{
    tGameObjectMain* result;
    tGameObjectMain* v2;
    tGameObjectBasic* v3;
    tGaoMatrix* v4;

    result = a1;
    if ((a1->flags & 1) != 0)
    {
        v2 = a1;
        while (1)
        {
            v3 = v2->basic;
            if (!v3)
                break;
            v4 = v3->mtx;
            if (!v4)
                break;
            v2 = v4->refGao2;
            if (!v2)
                break;
            if ((v2->flags & 1) == 0)
                return v2;
        }
    }
    return result;
}

void ANI_PlayBlendAnimationWithoutMagicBox(tGameObjectMain* a1, tGameObjectActorObject* a2, tAnim* a3)
{
    tAnim* v3;
    tGameObjectMain* v4;
    tAnimTracks* v5;
    __int16 v6;
    __int16 v7;
    __int16 v8;
    char v9;
    tGameObjectBasic* v10;
    tGameObjectActorObject* v11;
    tGroup* v12;
    int (*v13)(tEventData*, float*);
    byte v14;
    float v15;
    float v16;
    __int16 v17;
    __int16 v18;
    float v19;
    float a3a;
    int a3b;

    v3 = a3;
    v4 = a1;
    EVE_gpst_OwnerGAO = a1;
    EVE_gpst_CurrentGAO = a1;
    EVE_gpst_CurrentData = a3->ptr;
    EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
    EVE_gpst_CurrentRefMatrix = &a3->flashMtx;
    v5 = EVE_gpst_CurrentData->tracks;
    v6 = v5->flag & ~1;
    v7 = v5->flag & 1;
    v8 = v5[1].flag & 1;
    v17 = v5[2].flag & 1;
    v9 = (char)(v5[3].flag & 0xff);
    v5->flag = v6;
    EVE_gpst_CurrentData->tracks[1].flag &= ~1u; //LOBYTE(EVE_gpst_CurrentData->tracks[1].flag) &= ~1u;
    v18 = v9 & 1;
    if (TIM_gf_dt <= 0.0
        || ENG_gb_EVERunning
        || (v10 = v4->basic) == 0
        || (v11 = v10->actorObjectOrVisualObject) == 0
        || (v12 = v11->skeletonGroup, v13 = EVE_b_RunAnimation, !v12))
    {
        v13 = EVE_b_RunAllTracks;
    }
    v14 = v3->transitionFlags;
    if ((v14 & 4) != 0 && (v14 & 8) == 0)
    {
        EVE_GetTracksTime(v3->ptr, 0, &a3a, 0);
        EVE_GetTracksTime(v3->ptr, 2, &v16, 0);
        if (a3a + 0.001 < v16)
            EVE_gpst_CurrentData->tracks[3].flag &= ~1u; //LOBYTE(EVE_gpst_CurrentData->tracks[3].flag) &= ~1u;
    }
    v19 = TIM_gf_dt;
    v16 = TIM_gf_dt;
    do
    {
        a3b = v3->frequency;
        TIM_gf_dt = (float)((double)a3b * v19 * 0.016666668);
        v13(v3->ptr, &v19);
    } while (v19 != 0.0);
    EVE_gpst_CurrentData->tracks->flag |= v7;
    EVE_gpst_CurrentData->tracks[1].flag |= v8;
    EVE_gpst_CurrentData->tracks[2].flag |= v17;
    v15 = v16;
    EVE_gpst_CurrentData->tracks[3].flag |= v18;
    TIM_gf_dt = v15;
}

void ANI_BeforeRender(tGameObjectMain* a1)
{
    tGameObjectMain* v1;
    tGameObjectBasic* v2;
    tGameObjectActorObject* v3;
    tGroup* v4;
    uint8_t v5;
    int v6;
    tAnim* v7;
    double v8;
    double v9;
    tableElemStructPF* v10;
    Mtx4x4* v11;
    tGaoMatrix* v12;
    int v13;
    int v14;
    tGaoMatrix* v15;
    uint8_t v16;
    tableElemStructPF* v17;
    int v18;
    tAnim* v19;
    int v20;
    tGameObjectActorObject* v21;
    tableElemStructPF* i;
    float a4;
    float v24;
    tGroup* v25;
    float currTime;
    float a3;
    Vec3 a2;
    Vec3 scale;
    Mtx4x4 a1a[64];

    v1 = a1;
    v2 = a1->basic;
    v3 = v2->actorObjectOrVisualObject;
    v4 = ((tGameObjectActorObject*)(v2->actorObjectOrVisualObject))->skeletonGroup;
    v21 = v2->actorObjectOrVisualObject;
    v25 = v4;
    if (v4)
    {
        v5 = v3->flagsEgBlendIw2 & 0xF;
        v6 = 0;
        v18 = 0;
        v16 = v5;
        for (i = ((tableElemStructPF*)(v4->tablePF->nextElemPtr)) - 1; v5; v18 = v6)
        {
            if ((v5 & 1) != 0)
            {
                v7 = v3->animPtr[v6];
                v19 = v7;
                if (v6)
                {
                    v24 = TIM_gf_dt;
                    if ((v7->flags2 & 1) != 0)
                    {
                        TIM_gf_dt = *(float*)&v7->ptr->tracks[1].privateData;
                        --v7->flags2;
                    }
                    if ((v7->transitionFlags & 4) != 0
                        || TIM_gf_dt != 0.0
                        && (EVE_GetTracksTime(v7->ptr, 2, &a3, 0),
                            EVE_GetTracksTime(v7->ptr, 0, &currTime, 0),
                            v8 = currTime - a3,
                            TIM_gf_dt = (float)v8,
                            v9 = v8 * 60.0 / (double)v7->frequency,
                            TIM_gf_dt = (float)v9,
                            v9 < 0.001))
                    {
                        TIM_gf_dt = 0.0;
                    }
                    ANI_PlayBlendAnimationWithoutMagicBox(v1, v21, v7);
                    TIM_gf_dt = v24;
                    v5 = v16;
                }
                v10 = v25->tablePF->firstElemPtr;
                v17 = v10;
                v20 = 0;
                if (v10 <= i)
                {
                    v11 = a1a;
                    while (1)
                    {
                        if (v20 >= v1->basic->multiPartData->elemNum)
                        {
                        LABEL_34:
                            v5 = v16;
                            goto LABEL_35;
                        }
                        v12 = OBJ_pst_GetLocalMatrix((tGameObjectMain*)v10->objectPtr);
                        if (v12)
                            break;
                    LABEL_33:
                        v1 = a1;
                        ++v20;
                        ++v11;
                        if (++v17 > i)
                            goto LABEL_34;
                        v10 = v17;
                    }
                    if (v18)
                    {
                        if ((v7->transitionFlags & 0x80u) != 0)
                            goto LABEL_33;
                        v13 = v7->ptr->tracks->privateData;
                        v14 = 0;
                        a4 = *(float*)&v13;
                        if ((v12->mtx.mtxflag & 8) != 0 && (v11->mtxflag & 8) != 0)
                        {
                            MATH_GetScale(&a2, &v12->mtx);
                            MATH_GetScale(&scale, v11);
                            if (fabs(a2.x - scale.x) < 0.0000099999997
                                && fabs(a2.y - scale.y) < 0.0000099999997
                                && fabs(a2.z - scale.z) < 0.0000099999997)
                            {
                                v14 = 1;
                            }
                        }
                        MATH_MatrixBlend(v11, &v12->mtx, v11, a4, 1u);
                        if (v14)
                            MATH_SetScale(v11, &a2);
                        MATH_BlendVector(&v11->Pos, &v12->mtx.Pos, &v11->Pos, a4);
                        if (v12 != (tGaoMatrix*)v11)
                        {
                            v15 = (tGaoMatrix*)v11;
                            goto LABEL_31;
                        }
                    }
                    else
                    {
                        if (v11 == (Mtx4x4*)v12)
                            goto LABEL_33;
                        v15 = v12;
                        v12 = (tGaoMatrix*)v11;
                    LABEL_31:
                        memcpy(v12, v15, sizeof(Mtx4x4));
                    }
                    v7 = v19;
                    goto LABEL_33;
                }
            LABEL_35:
                v6 = v18;
                v3 = v21;
            }
            v5 >>= 1;
            ++v6;
            v16 = v5;
        }
        AI_C_Callback = 1;
        AI_ExecCallback(v1, 3);
        AI_C_Callback = 0;
    }
}

void ANI_BeforeRendering(struct sWorld* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;
    tGameObjectMain* v3;

    v1 = a1->visibleGameObjectsPF.firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->visibleGameObjectsPF.nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        v3 = (tGameObjectMain*)v1->objectPtr;
        if (v1->objectPtr && ((uint8_t)v3 & 3) != 1 && (v3->flags2 & 4) == 0 && (v3->flags & 2) != 0)
        {
            if (((tGameObjectActorObject*)(v3->basic->actorObjectOrVisualObject))->skeletonGroup)
                ANI_BeforeRender((tGameObjectMain*)v1->objectPtr);
        }
    }
}

void ANI_MainCall(struct sWorld* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* v2;
    tGameObjectMain* v3;
    tGameObjectBasic* v4;
    tGameObjectExtended* v5;
    __int16 v6;
    int v7;
    tGameObjectBasic* v8;
    tGameObjectActorObject* v9;
    tEventData* v10;
    tGameObjectBasic* v11;
    tableElemStructPF* i;
    float a1a;

    v1 = a1->eot.animatedObjectsTablePF.firstElemPtr;
    v2 = ((tableElemStructPF*)(a1->eot.animatedObjectsTablePF.nextElemPtr)) - 1;

    for (i = v2; v1 <= v2; ++v1)
    {
        v3 = (tGameObjectMain*)v1->objectPtr;
        if (v1->objectPtr)
        {
            if (((uint8_t)v3 & 3) != 1)
            {
                v4 = v3->basic;
                if (v4)
                {
                    if (v4->actorObjectOrVisualObject)
                    {
                        v5 = v3->extended;
                        if (v5 && (v6 = v5->extraFlag, (v6 & 1) != 0))
                        {
                            v5->extraFlag = v6 & ~1;
                            v7 = v3->flags2;
                            if ((v7 & 2) != 0 && (v7 & 4) == 0)
                            {
                                v8 = v3->basic;
                                if (v8)
                                {
                                    v9 = v8->actorObjectOrVisualObject;
                                    if (v8->actorObjectOrVisualObject)
                                    {
                                        if (v9->animPtr[0] && !v9->initedInANIMainCall)
                                        {
                                            v10 = v9->animPtr[0]->ptr;
                                            EVE_GetTracksTime(v10, 0, &a1a, 0);
                                            EVE_SetTracksTime(v10, a1a);
                                            v11 = v3->basic;
                                            if (((tGameObjectActorObject*)(v11->actorObjectOrVisualObject))->flagsEgBlendIw2 != 1)
                                                ((tGameObjectActorObject*)(v11->actorObjectOrVisualObject))->flagsEgBlendIw2 = 1;
                                            v2 = i;
                                            ((tGameObjectActorObject*)(v3->basic->actorObjectOrVisualObject))->initedInANIMainCall = 1;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            ANI_OneCall((tGameObjectMain*)v1->objectPtr);
                        }
                    }
                }
            }
        }
    }
}

struct sGameObjectMain* ANI_pst_GetReferenceInit(struct sGameObjectMain* a1)
{
    tGameObjectMain* result;
    tGameObjectMain* v2;
    tGameObjectBasic* v3;
    tGaoMatrix* v4;

    result = a1;
    if ((a1->flags & 1) != 0)
    {
        v2 = a1;
        while (1)
        {
            v3 = v2->basic;
            if (!v3)
                break;
            v4 = v3->mtx;
            if (!v4)
                break;
            v2 = v4->refGao;
            if (!v2)
                break;
            if ((v2->flags & 1) == 0)
                return v2;
        }
    }
    return result;
}

void ANI_ComputeBoneMatrix(tGameObjectMain* currentBone, tGameObjectMain* mainGao, Mtx4x4* destMtx, int a4)
{
    tGameObjectMain* v4;
    tGameObjectBasic* v5;
    tGaoMatrix* v6;
    tGameObjectMain* v7;
    tGaoMatrix* v8;
    Mtx4x4* v9;
    tGameObjectBasic* v10;
    tGaoMatrix* v11;
    tGameObjectMain* v12;
    Mtx4x4* v13;
    tGaoMatrix* v14;
    tGaoMatrix* v15;
    Mtx4x4* v16;
    Mtx4x4* v17;
    Mtx4x4* v18;
    Mtx4x4* v19;
    Mtx4x4* v20;
    Vec3 a2a;
    Vec3 v22;
    Vec3 v23;
    Mtx4x4 a3a;

    v20 = OBJ_pst_GetAbsoluteMatrix(mainGao);
    MATH_GetScale(&v23, v20);
    v4 = currentBone;
    if (currentBone && (v5 = currentBone->basic) != 0 && (v6 = v5->mtx) != 0)
        v7 = v6->refGao2;
    else
        v7 = 0;
    if (v7 == mainGao)
    {
        v8 = OBJ_pst_GetLocalMatrix(currentBone);
        v9 = destMtx;
        if (destMtx != (Mtx4x4*)v8)
        {
            memcpy(destMtx, v8, sizeof(Mtx4x4));
            v4 = currentBone;
        }
    }
    else if ((currentBone->flags & 0x400000) != 0 && (v10 = currentBone->basic) != 0 && (v11 = v10->mtx) != 0 && (v12 = v11->refGao2) != 0)
    {
        v13 = &(((Mtx4x4*)(mainGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[TAB_ul_PFtable_GetElemIndexWithPointer(((tGameObjectActorObject*)(mainGao->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF, v12)]);
        MATH_GetScale(&a2a, v13);
        v14 = OBJ_pst_GetLocalMatrix(currentBone);
        MATH_GetScale(&v22, &v14->mtx);
        a2a.x = a2a.x * v22.x;
        a2a.y = a2a.y * v22.y;
        a2a.z = a2a.z * v22.z;
        v15 = OBJ_pst_GetLocalMatrix(currentBone);
        v9 = destMtx;
        MATH_MulMatrixMatrix(destMtx, &v15->mtx, v13);
        MATH_Orthonormalize(destMtx);
        MATH_SetScale(destMtx, &a2a);
    }
    else
    {
        v16 = OBJ_pst_GetAbsoluteMatrix(mainGao);
        MATH_InvertMatrix(&a3a, v16);
        v17 = OBJ_pst_GetAbsoluteMatrix(currentBone);
        v9 = destMtx;
        MATH_MulMatrixMatrix(destMtx, v17, &a3a);
    }
    if (a4 && v4->flags2 >= 0)
    {
        v18 = OBJ_pst_GetAbsoluteMatrix(v4);
        MATH_MulMatrixMatrix(v18, v9, v20);
        v19 = v4->matrices;
        if ((v19->mtxflag & 8) != 0)
            MATH_Orthonormalize(v19);
    }
}

void ANI_Render(struct sGameObjectMain* mainGao)
{
    tGameObjectActorObject* v1;
    tAnim* v2;
    tGroup* v3;
    tEventData* v4;
    float v5;
    tableElemStructPF* v6;
    tableElemStructPF* v7;
    tGameObjectMain* currentBone; 
    tGameObjectExtended* v9; 
    Mtx4x4* v10;
    byte v11; 
    unsigned int v12;
    int v13;
    tGameObjectBasic* v14; 
    int v15;
    BOOL v16; 
    int v17; 
    tGameObjectMain* v18;
    tGameObjectBasic* v19;
    tGameObjectMultiPartData* v20;
    Mtx4x4* v21;
    Mtx4x4* v22;
    Mtx4x4* v23;
    Mtx4x4* v24;
    tAnim* v25;
    float a3;
    int v27;
    int v28;
    tableElemStructPF* v29;
    tGameObjectMain* v30;
    BOOL v31;
    LARGE_INTEGER PerformanceCount;
    Mtx4x4 v33; 
    tGroup* v29a;
    tableElemStructPF* v7a;

    QueryPerformanceCounter(&PerfCounterStruct[5]);
    v1 = (tGameObjectActorObject*)mainGao->basic->actorObjectOrVisualObject;
    v2 = v1->animPtr[0];
    v3 = v1->skeletonGroup;
    v29a = v3;
    v31 = v2 != 0;
    if (!v1->initedInANIMainCall && v2)
    {
        v4 = v2->ptr;
        EVE_GetTracksTime(v4, 0, &a3, 0);
        EVE_SetTracksTime(v4, a3);
        if (v1->flagsEgBlendIw2 != 1)
            v1->flagsEgBlendIw2 = 1;
        v1->initedInANIMainCall = 1;
        v5 = TIM_gf_dt;
        TIM_gf_dt = 0.0;
        v25 = v1->animPtr[0];
        a3 = v5;
        ANI_PlayLeadAnimation(mainGao, v1, v25);
        TIM_gf_dt = a3;
        AI_C_Callback = 1;
        AI_ExecCallback(mainGao, 3);
        v3 = v29a;
        AI_C_Callback = 0;
    }
    if (v3)
    {
        v7 = ((tableElemStructPF*)(v3->tablePF->nextElemPtr)) - 1;
        v29 = v3->tablePF->firstElemPtr;
        v6 = v29;
        v7a = v7;
        v27 = 0;
        if (v29 <= v7)
        {
            v28 = 0;
            while (!v31 || v27 < mainGao->basic->multiPartData->elemNum)
            {
                currentBone = (tGameObjectMain*)(v6->objectPtr);
                v9 = ((tGameObjectMain*)(v6->objectPtr))->extended;
                v10 = &(((Mtx4x4*)(mainGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v28]);
                if (v9 && (currentBone->flags & 0x2000) != 0 && v9->mdf)
                {
                    MDF_ApplyAllGao(currentBone);
                    MDF_UnApplyAllGao(currentBone);
                }
                ANI_ComputeBoneMatrix(currentBone, mainGao, v10, 1);
                v11 = mainGao->distance;
                v12 = currentBone->flags2 & ~8u;
                currentBone->flags2 = v12;
                currentBone->distance = v11;
                v13 = v12 | mainGao->flags2 & 8;
                v14 = currentBone->basic;
                currentBone->flags2 = v13;
                v14->extraMaterial = mainGao->basic->extraMaterial;
                if ((currentBone->flags2 & 0x200000) == 0 && (currentBone->flags & 0x4000) != 0)
                {
                    v15 = PushedStack;
                    pst_RenderingStack[PushedStack + 1] = currentBone;
                    PushedStack = v15 + 1;
                }
                v16 = &v29[1] <= v7a;
                ++v27;
                ++v28;
                ++v29;
                if (!v16)
                    break;
                v6 = v29;
            }
        }
        QueryPerformanceCounter(&PerformanceCount);
        if (PerfCounterStruct[5].QuadPart)
            PerfCountertimeSumArray[5] += PerformanceCount.QuadPart - PerfCounterStruct[5].QuadPart;
        v17 = PushedStack;
        for (PerfCounterStruct[5].QuadPart = 0i64; PushedStack; v17 = PushedStack)
        {
            v18 = pst_RenderingStack[v17];
            PushedStack = v17 - 1;
            v19 = v18->basic;
            v30 = v18;
            if (v19)
            {
                v20 = v19->multiPartData;
                if (v20 && v20->elemNum > 1)
                {
                    v21 = OBJ_pst_GetAbsoluteMatrix(v18);
                    if (&v33 != v21)
                    {
                        memcpy(&v33, v21, sizeof(v33));
                        v18 = v30;
                    }
                    v22 = OBJ_pst_GetAbsoluteMatrix(mainGao);
                    v23 = OBJ_pst_GetAbsoluteMatrix(v18);
                    if (v23 != v22)
                    {
                        memcpy(v23, v22, sizeof(Mtx4x4));
                        v18 = v30;
                    }
                    GRO_Render(v18);
                    v24 = OBJ_pst_GetAbsoluteMatrix(v18);
                    if (v24 != &v33)
                        memcpy(v24, &v33, sizeof(Mtx4x4));
                }
                else
                {
                    GRO_Render(v18);
                }
            }
        }
    }
    else
    {
        PRO_QPC_RecordTimeDecRef(5);
    }
}

void ANI_ChangeDefaultSkeleton(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tGameObjectBasic* v2;
    tGameObjectActorObject* v3;
    tGroup* v4;
    tGameObjectBasic* v5;
    tGameObjectActorObject* v6;
    tGroup* v7;
    tableElemStructPF* v8;
    tableElemStructPF* i;
    tGameObjectMain* v10;
    tGaoMatrix* v11;

    if (a1)
    {
        if ((a1->flags & 2) != 0)
        {
            v2 = a1->basic;
            if (v2)
            {
                v3 = v2->actorObjectOrVisualObject;
                if (v2->actorObjectOrVisualObject)
                {
                    v4 = v3->skeletonGroup;
                    if (v4)
                    {
                        if (a2)
                        {
                            if ((a2->flags & 2) != 0)
                            {
                                v5 = a2->basic;
                                if (v5)
                                {
                                    v6 = v5->actorObjectOrVisualObject;
                                    if (v6)
                                    {
                                        if (v6->skeletonGroup)
                                        {
                                            if (!v3->skeletonGroupToRestoreOnReinit)
                                                v3->skeletonGroupToRestoreOnReinit = v4;
                                            ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup = ((tGameObjectActorObject*)(a2->basic->actorObjectOrVisualObject))->skeletonGroup;
                                            ANI_InitSkeleton(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup, a1);
                                            v7 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup;
                                            v8 = v7->tablePF->firstElemPtr;
                                            for (i = ((tableElemStructPF*)(v7->tablePF->nextElemPtr)) - 1; v8 <= i; ++v8)
                                            {
                                                v10 = (tGameObjectMain*)v8->objectPtr;
                                                if (!TAB_TableElemIsInvalid(v8->objectPtr))
                                                {
                                                    v11 = v10->basic->mtx;
                                                    if (v11->refGao2 == a2)
                                                    {
                                                        v11->refGao = a1;
                                                        v10->basic->mtx->refGao2 = a1;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ANI_RestoreDefaultSkeleton(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tGameObjectBasic* v2;
    tGameObjectActorObject* v3;
    tGroup* v4;
    tGameObjectBasic* v5;
    tGameObjectActorObject* v6;
    tGroup* v7; 
    tableElemStructPF* v8; 
    tableElemStructPF* i;
    tGameObjectMain* v10;
    tGaoMatrix* v11; 

    if (a1)
    {
        if ((a1->flags & 2) != 0)
        {
            v2 = a1->basic;
            if (v2)
            {
                v3 = v2->actorObjectOrVisualObject;
                if (v3)
                {
                    v4 = v3->skeletonGroupToRestoreOnReinit;
                    if (v4)
                    {
                        if (a2)
                        {
                            if ((a2->flags & 2) != 0)
                            {
                                v5 = a2->basic;
                                if (v5)
                                {
                                    v6 = v5->actorObjectOrVisualObject;
                                    if (v6)
                                    {
                                        if (v6->skeletonGroup)
                                        {
                                            v3->skeletonGroup = v4;
                                            ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroupToRestoreOnReinit = 0;
                                            v7 = ((tGameObjectActorObject*)(a2->basic->actorObjectOrVisualObject))->skeletonGroup;
                                            v8 = v7->tablePF->firstElemPtr;
                                            for (i = ((tableElemStructPF*)(v7->tablePF->nextElemPtr)) - 1; v8 <= i; ++v8)
                                            {
                                                v10 = (tGameObjectMain*)v8->objectPtr;
                                                if (!TAB_TableElemIsInvalid(v8->objectPtr))
                                                {
                                                    v11 = v10->basic->mtx;
                                                    if (v11->refGao2 == a1)
                                                    {
                                                        v11->refGao = a2;
                                                        v10->basic->mtx->refGao2 = a2;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ANI_DestroyShape(tShape** a1)
{
    if (!--(*a1)->usageCount)
    {
        MEM_Free(*a1);
        LOA_DeleteAddress((int)*a1);
        *a1 = NULL;
    }
}

void ANI_CloneSet(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3)
{
    tGameObjectBasic* v3;
    tGameObjectActorObject* v4;
    tGameObjectBasic* v5;
    tGameObjectActorObject* v6;
    tGroup* v7;
    tGroup* v8;
    tGameObjectBasic* v9;
    tGameObjectActorObject* v10;
    tShape* v11;
    tShape** v12;
    tAnim* v13;
    tEventData* v14;
    tGameObjectBasic* v15;
    tGroup* v16; 
    tShape* v17; 
    tGroup* v18; 
    tGroup* v19; 
    tGroup* v20; 
    tShape* v21; 
    tActionKit* v22;
    tAction** v23;
    tAction* v24; 
    tActionElem* v25;
    Mtx4x4* v26; 
    float v27; 
    tWorld* v28; 
    float v29;
    tAction** actions;
    tActionElem* actionElems;

    if ((a2->flags & 0x1000) != 0 && (a2->flags & 2) != 0)
    {
        v3 = a2->basic;
        if (v3)
        {
            v4 = v3->actorObjectOrVisualObject;
            if (v4)
            {
                if (v4->skeletonGroup)
                {
                    if ((a1->flags & 0x4000) == 0)
                    {
                        v28 = WOR_gpst_CurrentWorld;
                        if (WOR_gpst_CurrentWorld)
                        {
                            if ((a1->flags & 0x1000) == 0)
                            {
                                v5 = (tGameObjectBasic*)MEM_p_Alloc(sizeof(tGameObjectBasic));
                                a1->basic = v5;
                                v5->actorObjectOrVisualObject = 0;
                                v5->dyn = 0;
                                v5->mtx = 0;
                                v5->multiPartData = 0;
                                v5->extraMaterial = 0;
                                a1->flags |= 0x1000u;
                            }
                            if ((a1->flags & 2) != 0)
                            {
                                v6 = a1->basic->actorObjectOrVisualObject;
                                v7 = v6->skeletonGroup;
                                if (v7)
                                {
                                    v8 = v6->skeletonGroupToRestoreOnReinit;
                                    if (v8)
                                        ANI_DestroySkeleton(v8);
                                    else
                                        ANI_DestroySkeleton(v7);
                                }
                                v9 = a1->basic;
                                if (((tGameObjectActorObject*)(v9->actorObjectOrVisualObject))->group)
                                    ANI_DestroySkeleton(((tGameObjectActorObject*)(v9->actorObjectOrVisualObject))->group);
                                v10 = a1->basic->actorObjectOrVisualObject;
                                v11 = v10->shape2;
                                v12 = &v10->shape2;
                                if (v11)
                                    ANI_DestroyShape(v12);
                                v13 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0];
                                if (v13)
                                {
                                    v14 = v13->ptr;
                                    if (v14)
                                    {
                                        if (v14->eventListStruct)
                                        {
                                            ANI_DestroyAnimation(v13);
                                            ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0] = 0;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                a1->basic->actorObjectOrVisualObject = ANI_pst_CreateGameObjectAnim();
                                memset(a1->basic->actorObjectOrVisualObject, 0, sizeof(tGameObjectActorObject));
                                a1->flags |= 2u;
                            }
                            v15 = a2->basic;
                            v16 = ((tGameObjectActorObject*)(v15->actorObjectOrVisualObject))->group;
                            v17 = ((tGameObjectActorObject*)(v15->actorObjectOrVisualObject))->shape2;
                            if (v16)
                            {
                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->group = v16;
                                v18 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->group;
                                ++v18->groupElemNum;
                                v19 = (tGroup*)GRP_pst_CreateNewGroup();
                                GRP_DuplicateGroupObjects(v28, v16, v19);
                                UpdateModifiersGroup(a1, v19);
                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup = v19;
                                v20 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup;
                                ++v20->groupElemNum;
                                OBJ_GameObject_AllocateGizmo(a1, v16->tablePF->actElemNum, 0);
                                ANI_InitSkeleton(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup, a1);
                            }
                            if (v17)
                            {
                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->shape2 = ((tGameObjectActorObject*)(a2->basic->actorObjectOrVisualObject))->shape2;
                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->shape = ((tGameObjectActorObject*)(a2->basic->actorObjectOrVisualObject))->shape;
                                memcpy(
                                    ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->shapes,
                                    ((tGameObjectActorObject*)(a2->basic->actorObjectOrVisualObject))->shapes,
                                    sizeof(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->shapes));
                                v21 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->shape2;
                                ++v21->usageCount;
                                ANI_ApplyShape(a1);
                            }
                            if (a3 != -1)
                            {
                                v22 = ((tGameObjectActorObject*)(a2->basic->actorObjectOrVisualObject))->actionKit;
                                if (v22)
                                {
                                    v23 = v22->savedActions;
                                    if (v23)
                                    {
                                        v24 = v23[a3];
                                    }
                                    else
                                    {
                                        if (v22 == (tActionKit*)-12)
                                            return;
                                        actions = (tAction**)(((char*)v22) + sizeof(tActionKit));
                                        v24 = actions[a3];
                                    }
                                    if (v24)
                                    {
                                        actionElems = (tActionElem*)(((char*)v24) + sizeof(tAction));
                                        v25 = actionElems;
                                        if (v24 != (tAction*)-4)
                                        {
                                            if (v25->eventLists)
                                            {
                                                if (!((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0])
                                                    ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0] = ANI_AllocAnim();
                                                ++v25->eventLists->usageCount;
                                                ANI_SetTrackList(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0], v25->eventLists);
                                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->flagsEgBlendIw2 = 1;
                                                v26 = OBJ_pst_GetAbsoluteMatrix(a1);
                                                OBJ_ReinitMatrixStorageByFlags(a1, v26);
                                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0]->frequency = 60;
                                                v27 = TIM_gf_dt;
                                                TIM_gf_dt = 0.0;
                                                EVE_gb_ForcePlayerOnAllTracks = 1;
                                                v29 = v27;
                                                ANI_PlayLeadAnimation(
                                                    a1,
                                                    a1->basic->actorObjectOrVisualObject,
                                                    ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0]);
                                                EVE_gb_ForcePlayerOnAllTracks = 0;
                                                TIM_gf_dt = v29;
                                                ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0]->frequency = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}