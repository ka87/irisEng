#include "photo.h"
#include "modifiers.h"
#include "modifiers/photomodifier.h"
#include "gameobject.h"
#include "collision.h"
#include "animation.h"

void IMG_Log4Debugging(char* msg)
{
    FILE* fp;

    fp = fopen("X:/PhotoLog.txt", "ab");
    if (fp)
    {
        fwrite(msg, strlen(msg), 1, fp);
        fclose(fp);
    }
}

void IMG_LogOutput(char type, tGameObjectMain* gao, float pct1, float pct2, float pct3, float pct4)
{
    char msg[504];

    if (!IMG_Log)
        return;
    switch (type)
    {
    case 0:
        sprintf(msg, "[MISSED] %%BV[%0.1f%% < 50%%]\n", pct1 * 100.0f);
        break;
    case 1:
        sprintf(msg, "[MISSED] %%Screen[%0.1f%% < 2.5%%]", pct2 * 100.0f);
        break;
    case 2:
        sprintf(msg, "[PHOTO] %%BV[%0.1f%%], %%Screen[%0.1f%%]\n", pct1 * 100.0f, pct2 * 100.0f);
        break;
    case 3:
        sprintf(
            msg,
            "[MDF][MISSED] %%BV[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
            pct1 * 100.0f,
            pct2 * 100.0f,
            pct3 * 100.0f,
            pct4 * 100.0f);
        break;
    case 4:
        sprintf(
            msg,
            "[MDF][MISSED] %%Screen[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
            pct1 * 100.0f,
            pct2 * 100.0f,
            pct3 * 100.0f,
            pct4 * 100.0f);
        break;
    case 5:
        sprintf(msg, "[MDF][Photo] %%BV[%0.1f%%], %%Screen[%0.1f%%]", pct1 * 100.0f, pct2 * 100.0f);
        break;
    default:
        // preserved: an unknown type leaves msg unwritten and logs it anyway
        break;
    }
    IMG_Log4Debugging(msg);
}

int IMG_ModifierObjectAnalyser(tGameObjectMain* gao, tModifier* mdf, tWorld* world,
                               tGdi_DisplayData* dd, int missionMask, int checkVisibility,
                               int rawInfo, int ignoreMission, int* code)
{
    tPhotoMdfStruct* photo;
    tGameObjectMain* snapGao;
    tGameObjectMain* hit;
    int value;
    int flags;
    float dist;
    Vec3 camPos;
    Vec3 dir;
    Vec3 target;
    char msg[504];

    photo = (tPhotoMdfStruct*)mdf->mdfPrivateDataPtr;
    if (ignoreMission)
    {
        value = 0;
    }
    else
    {
        value = photo->info;
        if (!photo->mission)
            return 0;
        if (missionMask != -1 && missionMask)
        {
            if (photo->mission << 24 != missionMask)
                return 0;
            if (!rawInfo)
                value = 1 << value;
        }
    }
    flags = photo->flags;
    if (photo->frame0_frameCullingSphereRet1 < (double)photo->frame1
        || photo->frame0_frameCullingSphereRet1 >(double)photo->frame2
        || (flags & 1) != 0)
    {
        if (IMG_Log)
        {
            // preserved: the two thresholds print without the x100 the values carry
            sprintf(
                msg,
                "[MDF][MISSED] %%BV[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
                photo->frame0_frameCullingSphereRet1 * 100.0,
                photo->frame1,
                photo->frame0_frameCullingSphereRet1 * 100.0,
                photo->frame2);
            IMG_Log4Debugging(msg);
        }
        if (code)
            *code = 2;
        return 0;
    }
    if (photo->lod0_frameCullingSphereRet2 < (double)photo->lod1
        || photo->lod0_frameCullingSphereRet2 >(double)photo->lod2
        || (flags & 2) != 0)
    {
        if (IMG_Log)
        {
            // preserved: the screen test prints the frame thresholds rather than its own
            sprintf(
                msg,
                "[MDF][MISSED] %%Screen[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
                photo->lod0_frameCullingSphereRet2 * 100.0,
                photo->frame1 * 100.0,
                photo->lod0_frameCullingSphereRet2 * 100.0,
                photo->frame2 * 100.0);
            IMG_Log4Debugging(msg);
        }
        if (code)
        {
            if (photo->lod0_frameCullingSphereRet2 < (double)photo->lod1 || (photo->flags & 2) != 0)
                *code = 1;
            else
                *code = 8;
        }
        return 0;
    }
    snapGao = GAO_ModifierPhoto_SnapGOGet(mdf->gao, photo, 0);
    camPos = dd->ct.cam.objectMtx.Pos;
    MATH_TransformVertex(&target, snapGao->matrices, &photo->sphereOffset);
    dir.x = target.x - camPos.x;
    dir.y = target.y - camPos.y;
    dir.z = (float)(target.z - camPos.z + 0.02);
    dist = MATH_VecLen(&dir);
    MATH_NormalizeEqualVector(&dir);
    if (checkVisibility)
    {
        hit = COL_Visual_RayCast(world, *AI_MainActor, &camPos, &dir,
                                 (float)(dist + 0.0099999998), -1, 0, 0, 0, 1);
        if (hit && hit != gao)
        {
            if (code)
                *code = 4;
            return 0;
        }
        if (IMG_Log)
        {
            sprintf(
                msg,
                "[MDF][Photo] %%BV[%0.1f%%], %%Screen[%0.1f%%]\n",
                photo->frame0_frameCullingSphereRet1 * 100.0,
                photo->lod0_frameCullingSphereRet2 * 100.0);
            IMG_Log4Debugging(msg);
        }
    }
    if (ignoreMission)
        return 1;
    return value;
}

int IMG_ObjectAnalyser(struct sGameObjectMain* gao, struct sWorld* world,
                       struct sGdi_DisplayData* dd, int missionMask, int checkVisibility,
                       int rawInfo, int ignoreMission, int* code)
{
    tModifier* mdf;
    tGameObjectExtended* ext;
    tGameObjectDesignFields* fields;
    tGameObjectMain* animObj;
    tGameObjectMain* hit;
    Mtx4x4* viewMtx;
    int value;
    int designInfo;
    float bvRatio;
    float screenRatio;
    float dist;
    Vec3 camPos;
    Vec3 dir;
    Vec3 centre;
    Vec3 bvMax;
    Vec3 bvMin;
    char msg[500];

    animObj = NULL;
    value = 0;
    if (code)
        *code = 0;
    if ((gao->flags2 & 2) == 0)
        return 0;
    mdf = GAO_ModifierPhoto_Get(gao, 1);
    if (mdf)
        return IMG_ModifierObjectAnalyser(gao, mdf, world, dd, missionMask, checkVisibility,
                                          rawInfo, ignoreMission, code);
    ext = gao->extended;
    if (!ext)
        return 0;
    if (!ignoreMission)
    {
        fields = ext->fields;
        if (!fields)
            return 0;
        if ((gao->flags & 0x40) == 0)
            return 0;
        designInfo = fields->designInt3;
        value = designInfo;
        if ((designInfo & 0xFF000000) == 0)
            return 0;
        if (missionMask != -1 && missionMask)
        {
            if ((designInfo & 0xFF000000) != missionMask)
                return 0;
            if (rawInfo)
                value = (uint8_t)designInfo;
            else
                value = 1 << designInfo;
        }
    }
    viewMtx = dd->ct.cam.viewMtxPtr_setInGRORender;
    if (viewMtx != &dd->ct.cam.cameraMtx)
        memcpy(viewMtx, &dd->ct.cam.cameraMtx, sizeof(Mtx4x4));
    if ((gao->flags & 2) != 0)
        animObj = ANI_pst_GetObjectByAICanal(gao, 0);
    if (animObj && OBJ_BV_IsAABBox(animObj->boundingVolume))
    {
        MATH_VecAddVec(&bvMax, &animObj->boundingVolume->BVMax, &animObj->matrices->Pos);
        MATH_VecAddVec(&bvMin, &animObj->boundingVolume->BVMin, &animObj->matrices->Pos);
    }
    else
    {
        if (!OBJ_BV_IsAABBox(gao->boundingVolume))
            return 0;
        MATH_VecAddVec(&bvMax, &gao->boundingVolume->BVMax, &gao->matrices->Pos);
        MATH_VecAddVec(&bvMin, &gao->boundingVolume->BVMin, &gao->matrices->Pos);
    }
    OBJ_Frame_CullingAABBox(&bvMin, &bvMax, &dd->ct.cam, 512, 256, &bvRatio, &screenRatio);
    if (bvRatio == 0.0)
    {
        if (code)
            *code = 2;
        return 0;
    }
    if (screenRatio < 0.025)
    {
        if (IMG_Log)
        {
            sprintf(msg, "[MISSED] %%Screen[%0.1f%% < 2.5%%]\n", screenRatio * 100.0);
            IMG_Log4Debugging(msg);
        }
        if (code)
            *code = 1;
        return 0;
    }
    if (bvRatio < 0.5)
    {
        if (IMG_Log)
        {
            sprintf(msg, "[MISSED] %%BV[%0.1f%% < 50%%]\n", bvRatio * 100.0);
            IMG_Log4Debugging(msg);
        }
        if (code)
            *code = 2;
        return 0;
    }
    camPos = dd->ct.cam.objectMtx.Pos;
    if (animObj && (gao->flags & 2) != 0)
        OBJ_BV_ComputeCenter(animObj, &centre);
    else
        OBJ_BV_ComputeCenter(gao, &centre);
    dir.x = centre.x - camPos.x;
    dir.y = centre.y - camPos.y;
    dir.z = (float)(centre.z - camPos.z + 0.02);
    dist = MATH_VecLen(&dir);
    MATH_NormalizeEqualVector(&dir);
    if (checkVisibility)
    {
        hit = COL_Visual_RayCast(world, *AI_MainActor, &camPos, &dir,
                                 (float)(dist + 0.0099999998), -1, 0, 0, 0, 1);
        if (hit && hit != gao)
        {
            if (code)
                *code = 4;
            return 0;
        }
        if (IMG_Log)
        {
            sprintf(msg, "[Photo] %%BV[%0.1f%%], %%Screen[%0.1f%%]\n", bvRatio * 100.0,
                    screenRatio * 100.0);
            IMG_Log4Debugging(msg);
        }
    }
    if (ignoreMission)
        return 1;
    return value;
}

int IMG_Analyser(tWorld* world, unsigned int mission)
{
    tableElemStructPF* cursor;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGdi_DisplayData* dd;
    int rawInfo;
    int result;
    int accumulated;
    int code;
    char msg[500];

    if (mission == -1)
        return IMG_PhotoSummary;
    result = 0;
    rawInfo = (mission >> 8) & 1;
    accumulated = 0;
    if (!world)
        return result;
    IMG_PhotoSummary = 0;
    if (GetAsyncKeyState(VK_SHIFT) >= 0)
    {
        IMG_Log = 0;
    }
    else
    {
        sprintf(msg, "****** Analyse for mission %d *********\n", mission);
        IMG_Log4Debugging(msg);
        IMG_Log = 1;
    }
    cursor = world->visibleGameObjectsPF.firstElemPtr;
    last = ((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr)) - 1;
    dd = world->viewPtr->displayData;
    for (; cursor <= last; cursor++)
    {
        gao = (tGameObjectMain*)cursor->objectPtr;
        if (!cursor->objectPtr || ((uint8_t)gao & 3) == 1 || (gao->flags2 & 4) != 0)
            continue;
        result = IMG_ObjectAnalyser(gao, world, dd, mission << 24, 1, rawInfo, 0, &code);
        if (code == 2)
            IMG_PhotoSummary = IMG_PhotoSummary + 1;
        else if (code == 1 || code == 8)
            IMG_PhotoSummary = IMG_PhotoSummary + 256;
        if ((result & 0xFF000000) != 0 || rawInfo && result)
            return result;
        accumulated |= result;
    }
    if (IMG_Log)
    {
        sprintf(msg, "==> Result = %08X (%08X)\n", accumulated, IMG_PhotoSummary);
        IMG_Log4Debugging(msg);
    }
    return accumulated;
}

int IMG_ModifierObjectAnalyser_Bis(tGameObjectMain* gao, tModifier* mdf, tWorld* world,
                                   tGdi_DisplayData* dd, int checkVisibility, int ignoreMission,
                                   int* code)
{
    tPhotoMdfStruct* photo;
    tGameObjectMain* snapGao;
    tGameObjectMain* hit;
    Mtx4x4* snapMtx;
    int value;
    double dx;
    double dz;
    long double lenSq;
    long double invLen;
    float lenSqF;
    Vec3 dir;
    Vec3 camPos;

    photo = (tPhotoMdfStruct*)mdf->mdfPrivateDataPtr;
    if (ignoreMission)
    {
        value = 1;
    }
    else
    {
        if (!photo->mission)
            return 0;
        value = photo->info | (photo->mission << 24);
    }
    if (photo->frame0_frameCullingSphereRet1 < (double)photo->frame1
        || photo->frame0_frameCullingSphereRet1 >(double)photo->frame2
        || (photo->flags & 1) != 0)
    {
        IMG_LogOutput(
            3,
            mdf->gao,
            photo->frame0_frameCullingSphereRet1,
            photo->frame1,
            photo->frame0_frameCullingSphereRet1,
            photo->frame2);
        if (code)
            *code = 2;
        value = 0;
    }
    if (photo->lod0_frameCullingSphereRet2 < (double)photo->lod1
        || photo->lod0_frameCullingSphereRet2 >(double)photo->lod2
        || (photo->flags & 2) != 0)
    {
        // preserved: the screen test reports the frame thresholds rather than its own
        IMG_LogOutput(4, mdf->gao, photo->lod0_frameCullingSphereRet2, photo->frame1,
                      photo->lod0_frameCullingSphereRet2, photo->frame2);
        if (code)
        {
            if (photo->lod0_frameCullingSphereRet2 < (double)photo->lod1
                || (photo->flags & 2) != 0)
                *code = 1;
            else
                *code = 8;
        }
        value = 0;
    }
    if (!checkVisibility)
        return value;
    snapGao = GAO_ModifierPhoto_SnapGOGet(mdf->gao, photo, 0);
    camPos = dd->ct.cam.objectMtx.Pos;
    snapMtx = snapGao->matrices;
    dx = snapMtx->Pos.x;
    dir.x = (float)(dx - camPos.x);
    dir.y = snapMtx->Pos.y - camPos.y;
    dz = snapMtx->Pos.z - camPos.z + 0.02;
    lenSq = dir.y * dir.y + dir.x * dir.x + dz * dz;
    lenSqF = (float)lenSq;
    invLen = 1.0 / sqrt(lenSq);
    dir.x = (float)(dir.x * invLen);
    dir.y = (float)(dir.y * invLen);
    dir.z = (float)(invLen * dz);
    hit = COL_Visual_RayCast(world, *AI_MainActor, &camPos, &dir,
                             (float)(sqrt(lenSqF) + 0.0099999998), -1, 0, 0, 0, 1);
    if (!hit || hit == gao)
    {
        IMG_LogOutput(5, mdf->gao, photo->frame0_frameCullingSphereRet1,
                      photo->lod0_frameCullingSphereRet2, 0.0, 0.0);
        return value;
    }
    if (code)
        *code = 4;
    return 0;
}

int IMG_ObjectAnalyser_Bis(tGameObjectMain* gao, tWorld* world, tGdi_DisplayData* dd,
                           int checkVisibility, int ignoreMission, int* code)
{
    tModifier* mdf;
    tGameObjectExtended* ext;
    tGameObjectDesignFields* fields;
    tGameObjectMain* target;
    tGameObjectMain* animObj;
    tGameObjectMain* hit;
    Mtx4x4* viewMtx;
    int value;
    float bvRatio;
    float screenRatio;
    float dist;
    Vec3 camPos;
    Vec3 dir;
    Vec3 bvMax;
    Vec3 bvMin;
    Vec3 centre;

    if (code)
        *code = 0;
    mdf = GAO_ModifierPhoto_Get(gao, 1);
    if (mdf)
        return IMG_ModifierObjectAnalyser_Bis(gao, mdf, world, dd, checkVisibility,
                                              ignoreMission, code);
    ext = gao->extended;
    if (!ext)
        return 0;
    if (ignoreMission)
    {
        value = 1;
    }
    else
    {
        fields = ext->fields;
        if (!fields)
            return 0;
        if ((gao->flags & 0x40) == 0)
            return 0;
        value = fields->designInt3;
        if ((value & 0xFF000000) == 0)
            return 0;
    }
    viewMtx = dd->ct.cam.viewMtxPtr_setInGRORender;
    if (viewMtx != &dd->ct.cam.cameraMtx)
        memcpy(viewMtx, &dd->ct.cam.cameraMtx, sizeof(Mtx4x4));
    target = gao;
    if ((gao->flags & 2) != 0)
    {
        animObj = ANI_pst_GetObjectByAICanal(gao, 0);
        if (animObj && OBJ_BV_IsAABBox(animObj->boundingVolume))
            target = animObj;
    }
    if (!OBJ_BV_IsAABBox(target->boundingVolume))
        return 0;
    MATH_VecAddVec(&bvMax, &target->matrices->Pos, &target->boundingVolume->BVMax);
    MATH_VecAddVec(&bvMin, &target->matrices->Pos, &target->boundingVolume->BVMin);
    OBJ_Frame_CullingAABBox(&bvMin, &bvMax, &dd->ct.cam, 512, 256, &bvRatio, &screenRatio);
    if (bvRatio == 0.0)
    {
        if (code)
            *code = 2;
        return 0;
    }
    if (screenRatio < 0.025)
    {
        IMG_LogOutput(1, gao, bvRatio, screenRatio, 0.0, 0.0);
        // preserved: *code is still 0 here on every path, so the ratio test can never be reached
        if (code && (*code != 2 || screenRatio / bvRatio < 0.025))
            *code = 1;
        value = 0;
    }
    if (bvRatio < 0.5)
    {
        IMG_LogOutput(0, gao, bvRatio, screenRatio, 0.0, 0.0);
        if (code)
            *code = 2;
        value = 0;
    }
    if (!checkVisibility)
        return value;
    camPos = dd->ct.cam.objectMtx.Pos;
    OBJ_BV_ComputeCenter(target, &centre);
    dir.x = centre.x - camPos.x;
    dir.y = centre.y - camPos.y;
    dir.z = (float)(centre.z - camPos.z + 0.02);
    dist = MATH_VecLen(&dir);
    MATH_NormalizeEqualVector(&dir);
    hit = COL_Visual_RayCast(world, *AI_MainActor, &camPos, &dir,
                             (float)(dist + 0.0099999998), -1, 0, 0, 0, 1);
    if (!hit || hit == gao)
    {
        IMG_LogOutput(2, gao, bvRatio, screenRatio, 0.0, 0.0);
        return value;
    }
    if (code)
        *code = 4;
    return 0;
}

unsigned int IMG_Analyser_Bis(tWorld* world, int mission)
{
    tableElemStructPF* cursor;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGdi_DisplayData* dd;
    unsigned int result;
    int flags2;
    int slot;
    int i;
    int packed;
    int summary;
    int answer;
    int code;
    char seen[4];
    char msg[500];

    if (!world)
        return 0;
    if (mission == -1)
        return IMG_PhotoSummary;
    if (mission == -2)
        return (unsigned int)IMG_Gao;
    IMG_Gao = 0;
    IMG_PhotoSummary = 0;
    packed = 0;
    dd = world->viewPtr->displayData;
    seen[2] = 0;
    seen[1] = 0;
    seen[0] = 0;
    IMG_Log = 0;
    if (GetAsyncKeyState(VK_SHIFT) < 0)
    {
        IMG_Log4Debugging("****** photo analyser *********\n");
        IMG_Log = 1;
    }
    cursor = world->visibleGameObjectsPF.firstElemPtr;
    last = ((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr)) - 1;
    for (; cursor <= last; cursor++)
    {
        gao = (tGameObjectMain*)cursor->objectPtr;
        if (!cursor->objectPtr)
            continue;
        if (((uint8_t)gao & 3) == 1)
            continue;
        flags2 = gao->flags2;
        if ((flags2 & 4) != 0 || (flags2 & 2) == 0)
            continue;
        result = IMG_ObjectAnalyser_Bis(gao, world, dd, 1, 0, &code);
        if (code == 2)
            IMG_PhotoSummary = IMG_PhotoSummary + 1;
        else if (code == 1 || code == 8)
            IMG_PhotoSummary = IMG_PhotoSummary + 256;
        if (!result)
            continue;
        if (((result >> 24) & 0xff) >= 100u) // HIBYTE
        {
            IMG_Gao = gao;
            return result;
        }
        if (((result >> 24) & 0xff) == 4) // HIBYTE
            return result;
        for (slot = 0; slot < 3; slot++)
        {
            if (!seen[slot])
            {
                seen[slot] = result;
                break;
            }
            if ((result & 0xff) == seen[slot])
                break;
        }
    }
    for (i = 2; i >= 0; --i)
        packed = (packed << 8) | seen[i];
    if (packed)
    {
        answer = packed | 0x1000000;
        summary = IMG_PhotoSummary;
    }
    else
    {
        summary = IMG_PhotoSummary;
        if (IMG_PhotoSummary)
            answer = IMG_PhotoSummary - 0x1000000;
        else
            answer = 0;
    }
    if (IMG_Log)
    {
        sprintf(msg, "==> Result = %08X (%08X)\n", answer, summary);
        IMG_Log4Debugging(msg);
    }
    return answer;
}
