#include "../memory.h"
#include "../loading.h"
#include "../gameobject.h"
#include "../animation.h"
#include "../world.h"
#include "photomodifier.h"

static struct sGameObjectMain* GAO_l_ModifierPhoto_SnapGOGiveUp(struct sGameObjectMain* gao,
                                                                struct sPhotoMdfStruct* photo,
                                                                int useSecond)
{
    if (useSecond)
        photo->aiCanal2 = -1;
    else
        photo->aiCanal1 = -1;
    return gao;
}

struct sGameObjectMain* GAO_ModifierPhoto_SnapGOGet(struct sGameObjectMain* gao,
                                                    struct sPhotoMdfStruct* photo,
                                                    int useSecond)
{
    int canal;
    tGameObjectActorObject* actor;
    tGameObjectMain* ref;
    tGameObjectMain* found;

    canal = useSecond ? photo->aiCanal2 : photo->aiCanal1;
    if (canal == -1)
        return gao;

    if ((gao->flags & 2) != 0 && gao->basic)
    {
        actor = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
        if (actor && actor->skeletonGroup && actor->shape)
        {
            found = ANI_pst_GetObjectByAICanal(gao, canal);
            if (found)
                return found;
            return GAO_l_ModifierPhoto_SnapGOGiveUp(gao, photo, useSecond);
        }
    }

    if ((gao->flags & 1) != 0)
    {
        ref = ANI_pst_GetReference(gao);
        if (ref)
        {
            found = ANI_pst_GetObjectByAICanal(ref, canal);
            if (found)
                return found;
        }
    }

    return GAO_l_ModifierPhoto_SnapGOGiveUp(gao, photo, useSecond);
}

static struct sModifier* GAO_l_ModifierPhoto_FindOn(struct sGameObjectMain* gao)
{
    tModifier* mdf;
    int flags;

    if ((gao->flags & 0x2000) == 0 || !gao->extended)
        return 0;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == 20)
        {
            flags = mdf->modifierFlags;
            if ((flags & 0xA000000) == 0 && (flags & 0x1000000) != 0)
                return mdf;
        }
    }
    return 0;
}

struct sModifier* GAO_ModifierPhoto_Get(struct sGameObjectMain* gao, int wantSphere)
{
    tDDCamStruct* cam;
    tModifier* mdf;
    tPhotoMdfStruct* photo;
    tGameObjectMain* owner;
    tGameObjectMain* snap;
    Vec3 center;

    if (!gao || !WOR_gpst_CurrentWorld)
        return 0;

    cam = &WOR_gpst_CurrentWorld->viewPtr->displayData->ct.cam;
    if (WOR_gpst_CurrentWorld->viewPtr->displayData == (tGdi_DisplayData*)0xFFFFFF30)
        return 0;

    owner = gao;
    mdf = GAO_l_ModifierPhoto_FindOn(gao);
    if (!mdf)
    {
        if ((gao->flags & 2) == 0)
            return 0;
        owner = ANI_pst_GetObjectByAICanal(gao, 0);
        if (!owner)
            return 0;
        mdf = GAO_l_ModifierPhoto_FindOn(owner);
        if (!mdf)
            return 0;
    }

    if (!wantSphere)
        return mdf;

    photo = (tPhotoMdfStruct*)mdf->mdfPrivateDataPtr;
    snap = GAO_ModifierPhoto_SnapGOGet(owner, photo, 0);
    MATH_TransformVertex(&center, snap->matrices, &photo->sphereOffset);
    OBJ_Frame_CullingSphere(
        &center,
        photo->radius,
        cam,
        512,
        256,
        &photo->frame0_frameCullingSphereRet1,
        &photo->lod0_frameCullingSphereRet2);
    return mdf;
}

void GAO_ModifierPhoto_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tPhotoMdfStruct* photo;
    struct sPhotoMdfStruct* src = (struct sPhotoMdfStruct*)initData;

    mdf->modifierFlags |= 0x1000000u;
    photo = (tPhotoMdfStruct*)MEM_p_Alloc(sizeof(tPhotoMdfStruct));

    photo->frame0_frameCullingSphereRet1 = 0.0;
    photo->lod0_frameCullingSphereRet2 = 0.0;
    photo->frame2 = 1.001f;
    photo->flags = 0;

    if (src)
    {
        photo->mission = src->mission;
        photo->info = src->info;
        photo->aiCanal1 = src->aiCanal1;
        photo->aiCanal2 = src->aiCanal2;
        photo->frame1 = src->frame1;
        photo->sphereOffset = src->sphereOffset;
        photo->infoOffset = src->infoOffset;
        photo->lod1 = src->lod1;
        photo->lod2 = src->lod2;
        photo->radius = src->radius;
    }
    else
    {
        photo->mission = 0;
        photo->info = 0;
        photo->aiCanal1 = -1;
        photo->aiCanal2 = -1;
        photo->frame1 = 0.5f;
        photo->sphereOffset.x = 0.0;
        photo->sphereOffset.y = 0.0;
        photo->sphereOffset.z = 0.0;
        photo->infoOffset.x = 0.0;
        photo->infoOffset.y = 0.0;
        photo->infoOffset.z = 0.0;
        photo->lod1 = 0.050000001f;
        photo->lod2 = 1.0f;
        photo->radius = 0.5f;
    }

    mdf->mdfPrivateDataPtr = photo;
}

void GAO_ModifierPhoto_Destroy(struct sModifier* mdf)
{
    if (mdf->mdfPrivateDataPtr)
        MEM_Free(mdf->mdfPrivateDataPtr);
}

int GAO_ModifierPhoto_Load(struct sModifier* mdf, char* data)
{
    tPhotoMdfStruct* photo;
    char* start;

    photo = (tPhotoMdfStruct*)mdf->mdfPrivateDataPtr;
    mdf->modifierFlags |= 0x1000000u;
    start = data;
    photo->mission = LOA_ReadLong(&data, 0, 1);
    photo->info = LOA_ReadLong(&data, 0, 1);
    photo->aiCanal1 = LOA_ReadLong(&data, 0, 1);
    photo->aiCanal2 = LOA_ReadLong(&data, 0, 1);
    photo->lod1 = LOA_ReadFloat(&data, 0, 1);
    photo->lod2 = LOA_ReadFloat(&data, 0, 1);
    photo->frame1 = LOA_ReadFloat(&data, 0, 1);
    photo->frame2 = LOA_ReadFloat(&data, 0, 1);
    photo->flags = 0;
    //if (*(uint32_t*)&photo->frame2 == 0x3F8020C5) // FIXME
    if (photo->frame2 == 1.001f)
    {
        LOA_ReadVector(&data, &photo->sphereOffset, 1);
        LOA_ReadVector(&data, &photo->infoOffset, 1);
    }
    else
    {
        photo->sphereOffset.x = 0.0;
        photo->sphereOffset.y = 0.0;
        photo->sphereOffset.z = 0.0;
        photo->infoOffset.x = 0.0;
        photo->infoOffset.y = 0.0;
        photo->infoOffset.z = 0.0;
    }
    photo->radius = LOA_ReadFloat(&data, 0, 1);
    return data - start;
}

void GAO_ModifierPhoto_Apply(tModifier* mdf, void* unk1)
{
}

void GAO_ModifierPhoto_Unapply(tModifier* mdf, void* unk1)
{
}

void GAO_ModifierPhoto_Reinit(tModifier* mdf)
{
}
