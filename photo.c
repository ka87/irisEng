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

void IMG_LogOutput(char type, tGameObjectMain* a2, float a3, float a4, float a5, float a6)
{
    char buff[504];

    if (IMG_Log)
    {
        switch (type)
        {
        case 0:
            sprintf(buff, "[MISSED] %%BV[%0.1f%% < 50%%]\n", a3 * 100.0f);
            break;
        case 1:
            sprintf(buff, "[MISSED] %%Screen[%0.1f%% < 2.5%%]", a4 * 100.0f);
            break;
        case 2:
            sprintf(buff, "[PHOTO] %%BV[%0.1f%%], %%Screen[%0.1f%%]\n", a3 * 100.0f, a4 * 100.0f);
            break;
        case 3:
            sprintf(
                buff,
                "[MDF][MISSED] %%BV[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
                a3 * 100.0f,
                a4 * 100.0f,
                a5 * 100.0f,
                a6 * 100.0f);
            break;
        case 4:
            sprintf(
                buff,
                "[MDF][MISSED] %%Screen[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
                a3 * 100.0f,
                a4 * 100.0f,
                a5 * 100.0f,
                a6 * 100.0f);
            break;
        case 5:
            sprintf(buff, "[MDF][Photo] %%BV[%0.1f%%], %%Screen[%0.1f%%]", a3 * 100.0f, a4 * 100.0f);
            break;
        default:
            break;
        }
        if (IMG_Log)
            IMG_Log4Debugging(buff);
    }
}

int IMG_ModifierObjectAnalyser(tGameObjectMain* a1, tModifier* a2, tWorld* a3, tGdi_DisplayData* a4, int a5, int a6, int a7, int a8, int* a9)
{
    tPhotoMdfStruct* v9;
    int v10;
    int v11;
    tGameObjectMain* v12;
    float v13;
    tPhotoMdfStruct* v14;
    tGameObjectMain* v15;
    int result;
    float a5a;
    int v18 = 0;
    float v19;
    Vec3 a3a;
    Vec3 a4a;
    Vec3 a1a;
    char Buffer[504];

    v9 = (tPhotoMdfStruct*)a2->mdfPrivateDataPtr;
    if (a8)
    {
        v10 = v18;
    }
    else
    {
        v10 = v9->info;
        if (!v9->mission)
            return 0;
        if (a5 != -1 && a5)
        {
            if (v9->mission << 24 != a5)
                return 0;
            if (!a7)
                v10 = 1 << v10;
        }
    }
    if (v9->frame0_frameCullingSphereRet1 < (double)v9->frame1
        || v9->frame0_frameCullingSphereRet1 >(double)v9->frame2
        || (v11 = v9->flags, (v11 & 1) != 0))
    {
        if (IMG_Log)
        {
            sprintf(
                Buffer,
                "[MDF][MISSED] %%BV[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
                v9->frame0_frameCullingSphereRet1 * 100.0,
                v9->frame1,
                v9->frame0_frameCullingSphereRet1 * 100.0,
                v9->frame2);
            IMG_Log4Debugging(Buffer);
        }
        if (a9)
            *a9 = 2;
        return 0;
    }
    if (v9->lod0_frameCullingSphereRet2 < (double)v9->lod1
        || v9->lod0_frameCullingSphereRet2 >(double)v9->lod2
        || (v11 & 2) != 0)
    {
        if (IMG_Log)
        {
            sprintf(
                Buffer,
                "[MDF][MISSED] %%Screen[%0.1f%% < %0.1f%%] or [%0.1f%% > %0.1f%%]",
                v9->lod0_frameCullingSphereRet2 * 100.0,
                v9->frame1 * 100.0,
                v9->lod0_frameCullingSphereRet2 * 100.0,
                v9->frame2 * 100.0);
            IMG_Log4Debugging(Buffer);
        }
        if (a9)
        {
            if (v9->lod0_frameCullingSphereRet2 < (double)v9->lod1 || (v9->flags & 2) != 0)
            {
                *a9 = 1;
                result = 0;
            }
            else
            {
                *a9 = 8;
                result = 0;
            }
            return result;
        }
        return 0;
    }
    v12 = GAO_ModifierPhoto_SnapGOGet(a2->gao, v9, 0);
    a3a.x = a4->ct.cam.objectMtx.Pos.x;
    v13 = a4->ct.cam.objectMtx.Pos.z;
    a3a.y = a4->ct.cam.objectMtx.Pos.y;
    v14 = (tPhotoMdfStruct*)a2->mdfPrivateDataPtr;
    a3a.z = v13;
    MATH_TransformVertex(&a1a, v12->matrices, &v14->sphereOffset);
    a4a.x = a1a.x - a3a.x;
    a4a.y = a1a.y - a3a.y;
    a4a.z = (float)(a1a.z - a3a.z + 0.02);
    v19 = MATH_VecLen(&a4a);
    MATH_NormalizeEqualVector(&a4a);
    if (a6)
    {
        a5a = (float)(v19 + 0.0099999998);
        v15 = COL_Visual_RayCast(a3, *AI_MainActor, &a3a, &a4a, a5a, -1, 0, 0, 0, 1);
        if (v15 && v15 != a1)
        {
            if (a9)
            {
                *a9 = 4;
                return 0;
            }
            return 0;
        }
        if (IMG_Log)
        {
            sprintf(
                Buffer,
                "[MDF][Photo] %%BV[%0.1f%%], %%Screen[%0.1f%%]\n",
                v9->frame0_frameCullingSphereRet1 * 100.0,
                v9->lod0_frameCullingSphereRet2 * 100.0);
            IMG_Log4Debugging(Buffer);
        }
    }
    result = 1;
    if (!a8)
        result = v10;
    return result;
}

int IMG_ObjectAnalyser(struct sGameObjectMain* a1, struct sWorld* a2, struct sGdi_DisplayData* a3, int a4, int a51, int a52, int a6, int* a7)
{
    tModifier* v8;
    int result;
    tGameObjectExtended* v10;
    tGameObjectDesignFields* v11;
    int v12;
    Mtx4x4* v13;
    tGameObjectMain* v14;
    tGameObjectBV* v15;
    Vec3* v16;
    float v17;
    float v18;
    tGameObjectMain* v19;
    float a5a;
    tGameObjectMain* v21;
    float v22;
    int v23;
    float v24;
    float v25;
    Vec3 a3a;
    Vec3 a4a;
    Vec3 a2a;
    Vec3 v29;
    Vec3 v30;
    char Buffer[500];

    v21 = 0;
    if (a7)
        *a7 = 0;
    if ((a1->flags2 & 2) == 0)
        return 0;
    v8 = GAO_ModifierPhoto_Get(a1, 1);
    if (v8)
        return IMG_ModifierObjectAnalyser(a1, v8, a2, a3, a4, a51, a52, a6, a7);
    v10 = a1->extended;
    if (!v10)
        return 0;
    if (!a6)
    {
        v11 = v10->fields;
        if (!v11)
            return 0;
        if ((a1->flags & 0x40) == 0)
            return 0;
        v12 = v11->designInt3;
        v23 = v12;
        if ((v12 & 0xFF000000) == 0)
            return 0;
        if (a4 != -1 && a4)
        {
            if ((v12 & 0xFF000000) != a4)
                return 0;
            if (a52)
                v23 = (uint8_t)v12;
            else
                v23 = 1 << v12;
        }
    }
    v13 = a3->ct.cam.viewMtxPtr_setInGRORender;
    if (v13 != &a3->ct.cam.cameraMtx)
        memcpy(v13, &a3->ct.cam.cameraMtx, sizeof(Mtx4x4));
    if ((a1->flags & 2) == 0)
    {
        if (OBJ_BV_IsAABBox(a1->boundingVolume))
            goto LABEL_26;
        return 0;
    }
    v14 = ANI_pst_GetObjectByAICanal(a1, 0);
    v21 = v14;
    if (v14 && OBJ_BV_IsAABBox(v14->boundingVolume))
    {
        MATH_VecAddVec(&v29, &v14->boundingVolume->BVMax, &v14->matrices->Pos);
        MATH_VecAddVec(&v30, &v14->boundingVolume->BVMin, &v14->matrices->Pos);
        goto LABEL_27;
    }
    if (!OBJ_BV_IsAABBox(a1->boundingVolume))
        return 0;
LABEL_26:
    v15 = a1->boundingVolume;
    v16 = &a1->matrices->Pos;
    MATH_VecAddVec(&v29, &v15->BVMax, v16);
    MATH_VecAddVec(&v30, &v15->BVMin, v16);
    v14 = v21;
LABEL_27:
    OBJ_Frame_CullingAABBox(&v30, &v29, &a3->ct.cam, 512, 256, &v24, &v25);
    if (v24 == 0.0)
        goto LABEL_38;
    if (v25 < 0.025)
    {
        if (IMG_Log)
        {
            sprintf(Buffer, "[MISSED] %%Screen[%0.1f%% < 2.5%%]\n", v25 * 100.0);
            IMG_Log4Debugging(Buffer);
        }
        if (a7)
            *a7 = 1;
        return 0;
    }
    if (v24 < 0.5)
    {
        if (IMG_Log)
        {
            sprintf(Buffer, "[MISSED] %%BV[%0.1f%% < 50%%]\n", v24 * 100.0);
            IMG_Log4Debugging(Buffer);
        }
    LABEL_38:
        if (a7)
        {
            *a7 = 2;
            return 0;
        }
        return 0;
    }
    v17 = a3->ct.cam.objectMtx.Pos.y;
    v18 = a3->ct.cam.objectMtx.Pos.z;
    a3a.x = a3->ct.cam.objectMtx.Pos.x;
    a3a.y = v17;
    a3a.z = v18;
    if (v14 && (a1->flags & 2) != 0)
        OBJ_BV_ComputeCenter(v14, &a2a);
    else
        OBJ_BV_ComputeCenter(a1, &a2a);
    a4a.x = a2a.x - a3a.x;
    a4a.y = a2a.y - a3a.y;
    a4a.z = (float)(a2a.z - a3a.z + 0.02);
    v22 = MATH_VecLen(&a4a);
    MATH_NormalizeEqualVector(&a4a);
    if (a51)
    {
        a5a = (float)(v22 + 0.0099999998);
        v19 = COL_Visual_RayCast(a2, *AI_MainActor, &a3a, &a4a, a5a, -1, 0, 0, 0, 1);
        if (v19 && v19 != a1)
        {
            if (a7)
            {
                *a7 = 4;
                return 0;
            }
            return 0;
        }
        if (IMG_Log)
        {
            sprintf(Buffer, "[Photo] %%BV[%0.1f%%], %%Screen[%0.1f%%]\n", v24 * 100.0, v25 * 100.0);
            IMG_Log4Debugging(Buffer);
        }
    }
    result = 1;
    if (!a6)
        result = v23;
    return result;
}

int IMG_Analyser(tWorld* a1, unsigned int a2)
{
    int result;
    int v3;
    tableElemStructPF* v4;
    tableElemStructPF* v5;
    tGameObjectMain* v6;
    int v7;
    int v8;
    int a7;
    tableElemStructPF* v10;
    tGdi_DisplayData* a3;
    char Buffer[500];

    if (a2 == -1)
        return IMG_PhotoSummary;
    result = 0;
    v3 = ((uint64_t)a2 >> 8) & 1;
    v8 = 0;
    if (a1)
    {
        IMG_PhotoSummary = 0;
        if (GetAsyncKeyState(VK_SHIFT) >= 0)
        {
            IMG_Log = 0;
        }
        else
        {
            sprintf(Buffer, "****** Analyse for mission %d *********\n", a2);
            IMG_Log4Debugging(Buffer);
            IMG_Log = 1;
        }
        v4 = a1->visibleGameObjectsPF.firstElemPtr;
        v5 = ((tableElemStructPF*)(a1->visibleGameObjectsPF.nextElemPtr)) - 1;
        a3 = a1->viewPtr->displayData;
        v10 = v5;
        while (v4 <= v5)
        {
            v6 = (tGameObjectMain*)v4->objectPtr;
            if (!v4->objectPtr || ((uint8_t)v6 & 3) == 1 || (v6->flags2 & 4) != 0)
                goto LABEL_20;
            result = IMG_ObjectAnalyser(v6, a1, a3, a2 << 24, 1, v3, 0, &a7);
            if (a7 != 1)
            {
                if (a7 == 2)
                {
                    v7 = IMG_PhotoSummary + 1;
                    goto LABEL_15;
                }
                if (a7 != 8)
                    goto LABEL_16;
            }
            v7 = IMG_PhotoSummary + 256;
        LABEL_15:
            IMG_PhotoSummary = v7;
        LABEL_16:
            if ((result & 0xFF000000) != 0 || v3 && result)
                return result;
            v8 |= result;
            v5 = v10;
        LABEL_20:
            ++v4;
        }
        if (IMG_Log)
        {
            sprintf(Buffer, "==> Result = %08X (%08X)\n", v8, IMG_PhotoSummary);
            IMG_Log4Debugging(Buffer);
        }
        result = v8;
    }
    return result;
}

int IMG_ModifierObjectAnalyser_Bis(tGameObjectMain* a1, tModifier* a2, tWorld* a3, tGdi_DisplayData* a4, int a5, int a6, int* a7)
{
    tPhotoMdfStruct* v8;
    int v9;
    int result;
    tGameObjectMain* v11;
    float v12;
    Mtx4x4* v13;
    double v14;
    double v15;
    long double v16;
    long double v17;
    tGameObjectMain* v18;
    float a5a;
    Vec3 a4a;
    Vec3 a3a;
    float v22;

    v8 = (tPhotoMdfStruct*)a2->mdfPrivateDataPtr;
    if (a6)
    {
        v9 = 1;
    }
    else
    {
        result = v8->mission;
        if (!v8->mission)
            return result;
        v9 = v8->info | (result << 24);
    }
    if (v8->frame0_frameCullingSphereRet1 < (double)v8->frame1
        || v8->frame0_frameCullingSphereRet1 >(double)v8->frame2
        || (v8->flags & 1) != 0)
    {
        IMG_LogOutput(
            3,
            a2->gao,
            v8->frame0_frameCullingSphereRet1,
            v8->frame1,
            v8->frame0_frameCullingSphereRet1,
            v8->frame2);
        if (a7)
            *a7 = 2;
        v9 = 0;
    }
    if (v8->lod0_frameCullingSphereRet2 < (double)v8->lod1
        || v8->lod0_frameCullingSphereRet2 >(double)v8->lod2
        || (v8->flags & 2) != 0)
    {
        IMG_LogOutput(4, a2->gao, v8->lod0_frameCullingSphereRet2, v8->frame1, v8->lod0_frameCullingSphereRet2, v8->frame2);
        if (a7)
        {
            if (v8->lod0_frameCullingSphereRet2 < (double)v8->lod1 || (v8->flags & 2) != 0)
                *a7 = 1;
            else
                *a7 = 8;
        }
        v9 = 0;
    }
    if (!a5)
        return v9;
    v11 = GAO_ModifierPhoto_SnapGOGet(a2->gao, v8, 0);
    a3a.x = a4->ct.cam.objectMtx.Pos.x;
    v12 = a4->ct.cam.objectMtx.Pos.z;
    a3a.y = a4->ct.cam.objectMtx.Pos.y;
    a3a.z = v12;
    v13 = v11->matrices;
    v14 = v13->Pos.x;
    a4a.x = (float)(v14 - a3a.x);
    a4a.y = v13->Pos.y - a3a.y;
    v15 = v13->Pos.z - v12 + 0.02;
    v16 = a4a.y * a4a.y + a4a.x * a4a.x + v15 * v15;
    v22 = (float)v16;
    v17 = 1.0 / sqrt(v16);
    a4a.x = (float)(a4a.x * v17);
    a4a.y = (float)(a4a.y * v17);
    a4a.z = (float)(v17 * v15);
    a5a = (float)(sqrt(v22) + 0.0099999998);
    v18 = COL_Visual_RayCast(a3, *AI_MainActor, &a3a, &a4a, a5a, -1, 0, 0, 0, 1);
    if (!v18 || v18 == a1)
    {
        IMG_LogOutput(5, a2->gao, v8->frame0_frameCullingSphereRet1, v8->lod0_frameCullingSphereRet2, 0.0, 0.0);
        return v9;
    }
    if (a7)
        *a7 = 4;
    return 0;
}

int IMG_ObjectAnalyser_Bis(tGameObjectMain* a1, tWorld* a2, tGdi_DisplayData* a3, int a4, int a5, int* a7)
{
    int* v6;
    tModifier* v7;
    tGameObjectExtended* v9;
    tGameObjectDesignFields* v10;
    Mtx4x4* v11;
    tGameObjectMain* v12;
    tGameObjectMain* v13;
    Mtx4x4* v14;
    tGameObjectBV* v15;
    double v16;
    Mtx4x4* v17;
    tGameObjectBV* v18;
    double v19;
    float v20;
    float v21;
    tGameObjectMain* v22;
    float a5a;
    float v24;
    float v25;
    int v26;
    Vec3 a3a;
    Vec3 a4a;
    Vec3 v29;
    Vec3 v30;
    Vec3 a2a;
    float a7a;

    v6 = a7;
    if (a7)
        *a7 = 0;
    v7 = GAO_ModifierPhoto_Get(a1, 1);
    if (v7)
        return IMG_ModifierObjectAnalyser_Bis(a1, v7, a2, a3, a4, a5, a7);
    v9 = a1->extended;
    if (!v9)
        return 0;
    if (a5)
    {
        v26 = 1;
        goto LABEL_12;
    }
    v10 = v9->fields;
    if (!v10)
        return 0;
    if ((a1->flags & 0x40) == 0)
        return 0;
    v26 = v10->designInt3;
    if ((v26 & 0xFF000000) == 0)
        return 0;
LABEL_12:
    v11 = a3->ct.cam.viewMtxPtr_setInGRORender;
    if (v11 != &a3->ct.cam.cameraMtx)
    {
        memcpy(v11, &a3->ct.cam.cameraMtx, sizeof(Mtx4x4));
        v6 = a7;
    }
    v12 = a1;
    if ((a1->flags & 2) != 0)
    {
        v13 = ANI_pst_GetObjectByAICanal(a1, 0);
        if (v13 && OBJ_BV_IsAABBox(v13->boundingVolume))
            v12 = v13;
        v6 = a7;
    }
    if (!OBJ_BV_IsAABBox(v12->boundingVolume))
        return 0;
    v14 = v12->matrices;
    v15 = v12->boundingVolume;
    v16 = v14->Pos.x + v15->BVMax.x;
    v29.x = (float)v16;
    v29.y = v14->Pos.y + v15->BVMax.y;
    v29.z = v14->Pos.z + v15->BVMax.z;
    v17 = v12->matrices;
    v18 = v12->boundingVolume;
    v19 = v17->Pos.x + v18->BVMin.x;
    v30.x = (float)v19;
    v30.y = v17->Pos.y + v18->BVMin.y;
    v30.z = v17->Pos.z + v18->BVMin.z;
    OBJ_Frame_CullingAABBox(&v30, &v29, &a3->ct.cam, 512, 256, &v24, &v25);
    if (v24 == 0.0)
    {
        if (v6)
            *v6 = 2;
        return 0;
    }
    if (v25 < 0.025)
    {
        IMG_LogOutput(1, a1, v24, v25, 0.0, 0.0);
        if (v6 && (*v6 != 2 || v25 / v24 < 0.025))
            *v6 = 1;
        v26 = 0;
    }
    if (v24 < 0.5)
    {
        IMG_LogOutput(0, a1, v24, v25, 0.0, 0.0);
        if (v6)
            *v6 = 2;
        v26 = 0;
    }
    if (!a4)
        return v26;
    v20 = a3->ct.cam.objectMtx.Pos.y;
    v21 = a3->ct.cam.objectMtx.Pos.z;
    a3a.x = a3->ct.cam.objectMtx.Pos.x;
    a3a.y = v20;
    a3a.z = v21;
    OBJ_BV_ComputeCenter(v12, &a2a);
    a4a.x = a2a.x - a3a.x;
    a4a.y = a2a.y - a3a.y;
    a4a.z = (float)(a2a.z - a3a.z + 0.02);
    a7a = MATH_VecLen(&a4a);
    MATH_NormalizeEqualVector(&a4a);
    a5a = (float)(a7a + 0.0099999998);
    v22 = COL_Visual_RayCast(a2, *AI_MainActor, &a3a, &a4a, a5a, -1, 0, 0, 0, 1);
    if (!v22 || v22 == a1)
    {
        IMG_LogOutput(2, a1, v24, v25, 0.0, 0.0);
        return v26;
    }
    if (v6)
        *v6 = 4;
    return 0;
}

unsigned int IMG_Analyser_Bis(tWorld* a1, int a2)
{
    unsigned int result;
    tableElemStructPF* v3;
    tableElemStructPF* v4;
    tGameObjectMain* v5;
    int v6;
    int v7;
    int v8;
    char v9;
    int i;
    int v11;
    int v12;
    int v13;
    int v14;
    char v15[4];
    tGdi_DisplayData* v16;
    int v17;
    char Buffer[500];

    if (!a1)
        return 0;
    if (a2 == -1)
        return IMG_PhotoSummary;
    if (a2 == -2)
        return (unsigned int)IMG_Gao;
    IMG_Gao = 0;
    IMG_PhotoSummary = 0;
    v13 = 0;
    v16 = a1->viewPtr->displayData;
    v15[2] = 0;
    v15[1] = 0;
    v15[0] = 0;
    IMG_Log = 0;
    if (GetAsyncKeyState(VK_SHIFT) < 0)
    {
        IMG_Log4Debugging("****** photo analyser *********\n");
        IMG_Log = 1;
    }
    v3 = a1->visibleGameObjectsPF.firstElemPtr;
    v4 = ((tableElemStructPF*)(a1->visibleGameObjectsPF.nextElemPtr)) - 1;
    while (v3 <= v4)
    {
        v5 = (tGameObjectMain*)v3->objectPtr;
        if (!v3->objectPtr)
            goto LABEL_29;
        if (((uint8_t)v5 & 3) == 1)
            goto LABEL_29;
        v6 = v5->flags2;
        if ((v6 & 4) != 0 || (v6 & 2) == 0)
            goto LABEL_29;
        result = IMG_ObjectAnalyser_Bis(v5, a1, v16, 1, 0, &v17);
        if (v17 != 1)
        {
            if (v17 == 2)
            {
                v7 = IMG_PhotoSummary + 1;
                goto LABEL_18;
            }
            if (v17 != 8)
                goto LABEL_19;
        }
        v7 = IMG_PhotoSummary + 256;
    LABEL_18:
        IMG_PhotoSummary = v7;
    LABEL_19:
        if (result)
        {
            if (((result >> 24) & 0xff) >= 100u) // HIBYTE
            {
                IMG_Gao = v5;
                return result;
            }
            if (((result >> 24) & 0xff) == 4) // HIBYTE
                return result;
            v8 = 0;
            while (1)
            {
                v9 = v15[v8];
                if (!v9)
                    break;
                if ((result & 0xff) != v9 && ++v8 < 3)
                    continue;
                goto LABEL_29;
            }
            v15[v8] = result;
        }
    LABEL_29:
        ++v3;
    }
    for (i = 2; i >= 0; --i)
    {
        v11 = (v13 << 8) | v15[i];
        v13 = v11;
    }
    if (v11)
    {
        v14 = v11 | 0x1000000;
        v12 = IMG_PhotoSummary;
    }
    else
    {
        v12 = IMG_PhotoSummary;
        if (IMG_PhotoSummary)
            v14 = IMG_PhotoSummary - 0x1000000;
        else
            v14 = 0;
    }
    if (IMG_Log)
    {
        sprintf(Buffer, "==> Result = %08X (%08X)\n", v14, v12);
        IMG_Log4Debugging(Buffer);
    }
    return v14;
}