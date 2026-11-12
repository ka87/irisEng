#include "../memory.h"
#include "../loading.h"
#include "../gameobject.h"
#include "../animation.h"
#include "../world.h"
#include "photomodifier.h"

struct sGameObjectMain* GAO_ModifierPhoto_SnapGOGet(struct sGameObjectMain* a1, struct sPhotoMdfStruct* a2, int a3)
{
    int v3;
    tGameObjectMain* result;
    tGameObjectBasic* v5;
    tGameObjectActorObject* v6;
    tGameObjectMain* v7;

    if (a3)
        v3 = a2->aiCanal2;
    else
        v3 = a2->aiCanal1;
    if (v3 == -1)
        return a1;
    if ((a1->flags & 2) != 0)
    {
        v5 = a1->basic;
        if (v5)
        {
            v6 = v5->actorObjectOrVisualObject;
            if (v6)
            {
                if (v6->skeletonGroup && v6->shape)
                {
                    result = ANI_pst_GetObjectByAICanal(a1, v3);
                    goto LABEL_15;
                }
            }
        }
    }
    if ((a1->flags & 1) != 0)
    {
        v7 = ANI_pst_GetReference(a1);
        if (v7)
        {
            result = ANI_pst_GetObjectByAICanal(v7, v3);
        LABEL_15:
            if (result)
                return result;
            goto LABEL_16;
        }
    }
LABEL_16:
    if (a3)
    {
        a2->aiCanal2 = -1;
        result = a1;
    }
    else
    {
        result = a1;
        a2->aiCanal1 = -1;
    }
    return result;
}

struct sModifier* GAO_ModifierPhoto_Get(struct sGameObjectMain* a1, int a2)
{
    tDDCamStruct* v3;
    tGameObjectExtended* v4;
    tModifier* v5;
    int v6;
    tGameObjectMain* v7;
    tGameObjectExtended* v8;
    int v9;
    tPhotoMdfStruct* v10;
    tGameObjectMain* v11;
    Vec3 v12;

    if (!a1 || !WOR_gpst_CurrentWorld)
        return 0;
    v3 = &WOR_gpst_CurrentWorld->viewPtr->displayData->ct.cam;
    if (WOR_gpst_CurrentWorld->viewPtr->displayData == (tGdi_DisplayData*)0xFFFFFF30)
        return 0;
    if ((a1->flags & 0x2000) != 0)
    {
        v4 = a1->extended;
        if (v4)
        {
            v5 = v4->mdf;
            if (v5)
            {
                while (1)
                {
                    if (v5->modifierIntf->mdfId == 20)
                    {
                        v6 = v5->modifierFlags;
                        if ((v6 & 0xA000000) == 0 && (v6 & 0x1000000) != 0)
                            break;
                    }
                    v5 = v5->nextElem;
                    if (!v5)
                        goto LABEL_12;
                }
                if (!a2)
                    return v5;
                v10 = (tPhotoMdfStruct*)v5->mdfPrivateDataPtr;
                v11 = GAO_ModifierPhoto_SnapGOGet(a1, v10, 0);
                goto LABEL_24;
            }
        }
    }
LABEL_12:
    if ((a1->flags & 2) == 0)
        return 0;
    v7 = ANI_pst_GetObjectByAICanal(a1, 0);
    if (!v7)
        return 0;
    if ((v7->flags & 0x2000) == 0)
        return 0;
    v8 = v7->extended;
    if (!v8)
        return 0;
    v5 = v8->mdf;
    if (!v5)
        return 0;
    while (1)
    {
        if (v5->modifierIntf->mdfId == 20)
        {
            v9 = v5->modifierFlags;
            if ((v9 & 0xA000000) == 0 && (v9 & 0x1000000) != 0)
                break;
        }
        v5 = v5->nextElem;
        if (!v5)
            return 0;
    }
    if (!a2)
        return v5;
    v10 = (tPhotoMdfStruct*)v5->mdfPrivateDataPtr;
    v11 = GAO_ModifierPhoto_SnapGOGet(v7, v10, 0);
LABEL_24:
    MATH_TransformVertex(&v12, v11->matrices, &v10->sphereOffset);
    OBJ_Frame_CullingSphere(
        &v12,
        v10->radius,
        v3,
        512,
        256,
        &v10->frame0_frameCullingSphereRet1,
        &v10->lod0_frameCullingSphereRet2);
    return v5;
}

void GAO_ModifierPhoto_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a)
{
    tPhotoMdfStruct* result;
    int v4;
    int v5;
    int v6;
    int v7;
    double v8;
    double v9;
    double v10;
    struct sPhotoMdfStruct* a3 = (struct sPhotoMdfStruct*) a3a;

    a2->modifierFlags |= 0x1000000u;
    result = (tPhotoMdfStruct*)MEM_p_Alloc(sizeof(tPhotoMdfStruct));
    if (a3)
        v4 = a3->mission;
    else
        v4 = 0;
    result->mission = v4;
    if (a3)
        v5 = a3->info;
    else
        v5 = 0;
    result->info = v5;
    if (a3)
        v6 = a3->aiCanal1;
    else
        v6 = -1;
    result->aiCanal1 = v6;
    if (a3)
        v7 = a3->aiCanal2;
    else
        v7 = -1;
    result->aiCanal2 = v7;
    result->frame0_frameCullingSphereRet1 = 0.0;
    if (a3)
        v8 = a3->frame1;
    else
        v8 = 0.5;
    result->frame1 = (float)v8;
    result->frame2 = 1.001f;
    result->flags = 0;
    if (a3)
    {
        result->sphereOffset = a3->sphereOffset;
        result->infoOffset = a3->infoOffset;
    }
    else
    {
        result->sphereOffset.x = 0.0;
        result->sphereOffset.y = 0.0;
        result->sphereOffset.z = 0.0;
        result->infoOffset.x = 0.0;
        result->infoOffset.y = 0.0;
        result->infoOffset.z = 0.0;
    }
    result->lod0_frameCullingSphereRet2 = 0.0;
    if (a3)
        v9 = a3->lod1;
    else
        v9 = 0.050000001;
    result->lod1 = (float)v9;
    if (a3)
        v10 = a3->lod2;
    else
        v10 = 1.0;
    result->lod2 = (float)v10;
    if (a3)
        result->radius = a3->radius;
    else
        result->radius = 0.5;
    a2->mdfPrivateDataPtr = result;
}

void GAO_ModifierPhoto_Destroy(struct sModifier* a1)
{
    if (a1->mdfPrivateDataPtr)
        MEM_Free(a1->mdfPrivateDataPtr);
}

int GAO_ModifierPhoto_Load(struct sModifier* a1, char* a2)
{
    tPhotoMdfStruct* v2;
    char* v3;
    float v4; // double
    char* v6;

    v2 = (tPhotoMdfStruct*)a1->mdfPrivateDataPtr;
    a1->modifierFlags |= 0x1000000u;
    v3 = a2;
    v6 = a2;
    v2->mission = LOA_ReadLong(&v6, 0, 1);
    v2->info = LOA_ReadLong(&v6, 0, 1);
    v2->aiCanal1 = LOA_ReadLong(&v6, 0, 1);
    v2->aiCanal2 = LOA_ReadLong(&v6, 0, 1);
    v2->lod1 = LOA_ReadFloat(&v6, 0, 1);
    v2->lod2 = LOA_ReadFloat(&v6, 0, 1);
    v2->frame1 = LOA_ReadFloat(&v6, 0, 1);
    v4 = LOA_ReadFloat(&v6, 0, 1);
    v2->frame2 = v4;
    v2->flags = 0;
    //if (*(uint32_t*)&v4 == 0x3F8020C5) // FIXME
    if (v4 == 1.001f)
    {
        LOA_ReadVector(&v6, &v2->sphereOffset, 1);
        LOA_ReadVector(&v6, &v2->infoOffset, 1);
    }
    else
    {
        v2->sphereOffset.x = 0.0;
        v2->sphereOffset.y = 0.0;
        v2->sphereOffset.z = 0.0;
        v2->infoOffset.x = 0.0;
        v2->infoOffset.y = 0.0;
        v2->infoOffset.z = 0.0;
    }
    v2->radius = LOA_ReadFloat(&v6, 0, 1);
    return v6 - v3;
}

void GAO_ModifierPhoto_Apply(tModifier* a1, void* unk1) {
}

void GAO_ModifierPhoto_Unapply(tModifier* a1, void* unk1) {
}

void GAO_ModifierPhoto_Reinit(tModifier* a1) {
}