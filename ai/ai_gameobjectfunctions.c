#include "ai_gameobjectfunctions.h"
#include "../engine.h"
#include "../memory.h"
#include "../math.h"
#include "../dbg.h"
#include "../snp.h"
#include "../profiler.h"
#include "../animation.h"
#include "../events.h"
#include "../world.h"
#include "../modifiers/morphmodifier.h"
#include "../modifiers/rlicartemodifier.h"
#include "../modifiers/leglinkmodifier.h"
#include "../photo.h"
#include "../modifiers/photomodifier.h"

int AI_EvalFunc_OBJ_HasBeenKilled_C(struct sGameObjectMain* a1)
{
    int i;

    for (i = 0; i < WOR_gi_NumKilledGO; ++i)
    {
        if (WOR_AllKilledGO[i] == a1)
            return 1;
    }
    return 0;
}

tAINode* AI_EvalFunc_OBJ_HasBeenKilled(tAINode* a1)
{
    int v1;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_OBJ_HasBeenKilled_C(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_OutputProgress_C(int a1, void* a2)
{
    FILE* Stream;
    int Buffer;

    Stream = fopen("SFTime.sav", "wb");
    if (Stream)
    {
        Buffer = '.TFS';
        fwrite(&Buffer, 4u, 1u, Stream);
        fwrite(&a1, 4u, 1u, Stream);
        fwrite(a2, 4 * a1, 1u, Stream);
        fclose(Stream);
    }
}

tAINode* AI_EvalFunc_OutputProgress(tAINode* a1)
{
    tVarStackElem a2;
    char dest[52];
    int a1a;

    AI_GetVarFromStack(dest, &a2);
    a1a = AI_GetIntFromStack();
    AI_EvalFunc_OutputProgress_C(a1a, a2.ptr);
    return a1 + 1;
}

void AI_EvalFunc_OBJDrawMaskSet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    if (a1->basic && a1->basic->actorObjectOrVisualObject && (a1->flags & 0x4000) != 0)
    {
        ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask |= a2;
        ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask &= ~a3;
    }
}

tAINode* AI_EvalFunc_OBJDrawMaskSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJDrawMaskSet_C(v2, a2, a3);
    return a1 + 1;
}

int AI_EvalFunc_OBJDrawMaskGet_C(struct sGameObjectMain* a1)
{
    if (!a1->basic)
        return 0;
    if (!a1->basic->actorObjectOrVisualObject)
        return 0;
    if ((a1->flags & 0x4000) != 0)
        return ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask;
    return 0;
}

tAINode* AI_EvalFunc_OBJDrawMaskGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJDrawMaskGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJMe(tAINode* a1)
{
    int v2[3];

    v2[0] = (int)AI_gpst_CurrentGameObject;
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

struct sGameObjectMain* AI_EvalFunc_OBJDuplicate_C(struct sGameObjectMain* srcGao, Vec3* pos)
{
    tGameObjectMain* a1a;

    OBJ_gb_DuplicateAI = 1;
    a1a = OBJ_GameObject_Duplicate(WOR_gpst_CurrentWorld, srcGao, 1, 0, pos);
    OBJ_SetInitialAbsolutePosition(a1a, pos);
    OBJ_RestoreInitialPos(a1a);
    ENG_ReinitOneObject(a1a, 13);
    INT_SnP_AttachObject(a1a, WOR_gpst_CurrentWorld);
    OBJ_gb_DuplicateAI = 0;
    return a1a;
}

tAINode* AI_EvalFunc_OBJDuplicate(tAINode* a1)
{
    int a1a[3];
    tGameObjectMain* v3;
    tGameObjectMain* srcGao;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    srcGao = v3;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a1a[0] = (int)AI_EvalFunc_OBJDuplicate_C(v3, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutObjectToStack(a1a, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

int AI_EvalFunc_OBJDestroy_C(struct sGameObjectMain* a1)
{
    int result;
    unsigned int v2;
    unsigned int v3;
    tWorld* a2;
    tWorld* a2a;

    if (WOR_gi_NumKilledGO < 200)
        WOR_AllKilledGO[WOR_gi_NumKilledGO++] = a1;
    if ((a1->flags & 0x20000) != 0)
    {
        a2a = WOR_gpst_CurrentWorld;
        if (WOR_gpst_CurrentWorld)
        {
            WOR_DelObj(WOR_gpst_CurrentWorld, a1);
            WOR_World_DetachObject(a2a, a1);
            INT_SnP_DetachObject(a1, a2a);
            SND_DetachObject(a1);
        }
        OBJ_GameObject_Remove(a1, 1);
        if (a1 == AI_gpst_CurrentGameObject)
            return 6;
    }
    else
    {
        v3 = a1->flags2 & 0xFFFF0000 | 0x200000;
        a1->flags2 = (uint16_t)a1->flags2;
        a1->flags2 |= v3 & 0xFFFF0000;
        v2 = a1->flags2 & 0xFFFF0000 | 0x400000;
        a1->flags2 = (uint16_t)a1->flags2;
        a1->flags2 |= v2 & 0xFFFF0000;
        a2 = WOR_gpst_CurrentWorld;
        if (WOR_gpst_CurrentWorld)
        {
            WOR_gpst_CurrentWorld->tablesManagerAndEOTNeeded = 1;
            a2->visibilityCallNeeded = 1;
        }
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_DestroyInstanceOfGao(a1);
        PRO_QPC_RecordTimeDecRef(39);
        if ((a1->flags & 0x400000) != 0 && a1->basic && a1->basic->mtx && !a1->basic->mtx->refGao)
        {
            OBJ_ComputeGlobalWhenHie(a1);
            if ((a1->flags & 0x4000000) != 0)
                ANI_RemoveHierarchyOnFlash(a1);
            a1->flags &= ~0x400000u;
            MEM_Free(a1->basic->mtx);
            a1->basic->mtx = 0;
        }
    }
    if (a1 == AI_gpst_CurrentGameObject)
        result = 1;
    else
        result = 1000;
    return result;
}

tAINode* AI_EvalFunc_OBJDestroy(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_EvalFunc_OBJDestroy_C(v2);
    if (v3 == 1)
        return (tAINode*)1;
    if (v3 == 6)
        return (tAINode*)6;
    return a1 + 1;
}

void AI_EvalFunc_OBJMove_C(struct sGameObjectMain* a1, Vec3* a2)
{
    float v2;
    Vec3* v3;
    float v4;

    if (ENG_gb_EVERunning)
    {
        v2 = (float)EVE_gpst_CurrentEvent->duration;
        v4 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0));
        a2->x = v4 * a2->x;
        a2->y = a2->y * v4;
        a2->z = a2->z * v4;
    }
    v3 = &a1->matrices->Pos;
    v3->x = v3->x + a2->x;
    v3->y = v3->y + a2->y;
    v3->z = v3->z + a2->z;
    if ((a1->flags & 4) != 0)
        a1->basic->dyn->flags |= 0x800000u;
}

tAINode* AI_EvalFunc_OBJMove(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJMove_C(v2, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateAround_C(struct sGameObjectMain* a2, Vec3* a3, float a4)
{
    Mtx4x4* v4;
    Mtx4x4 v5;
    Mtx4x4* v6;
    Mtx4x4* v7;
    Mtx4x4 v8;
    int v9;
    int v10;

    v10 = a2->flags & 0x400000;
    v9 = a2->flags & 0x4000000;
    if (v10)
        OBJ_ComputeGlobalWithLocal(a2, a2->matrices, 1);
    MATH_MakeRotationMatrix_AxisAngle(&v8, a3, a4, 0, 1);
    v7 = a2->matrices;
    v6 = v7;
    if (&v5 != v7)
        memcpy(&v5, v7, sizeof(v5));
    MATH_MulMatrixMatrix(a2->matrices, &v8, &v5);
    if (v9)
    {
        if ((a2->flags & 0x4000000) != 0)
        {
            if ((a2->flags & 0x10000) != 0)
                v4 = a2->matrices + 2;
            else
                v4 = a2->matrices + 1;
        }
        else
        {
            v4 = 0;
        }
        if (&v5 != v4)
            memcpy(&v5, v4, sizeof(v5));
        if ((a2->flags & 0x4000000) != 0)
        {
            if ((a2->flags & 0x10000) != 0)
                MATH_MulMatrixMatrix(a2->matrices + 2, &v8, &v5);
            else
                MATH_MulMatrixMatrix(a2->matrices + 1, &v8, &v5);
        }
        else
        {
            MATH_MulMatrixMatrix(0, &v8, &v5);
        }
    }
    if (v10)
        OBJ_ComputeLocalWhenHie(a2);
}

tAINode* AI_EvalFunc_OBJRotateAround(tAINode* a1)
{
    tGameObjectMain* v2;
    float a4;
    Vec3 a1a;
    tGameObjectMain* a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    a4 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_OBJRotateAround_C(a2, &a1a, a4);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateLocal_C(struct sGameObjectMain* a1, Vec3* a2)
{
    float v2;
    float v3;
    int v4;
    int v5;
    Vec3 v6;

    if (ENG_gb_EVERunning)
    {
        v2 = (float)EVE_gpst_CurrentEvent->duration;
        v3 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0));
        a2->x = v3 * a2->x;
        a2->y = v3 * a2->y;
        a2->z = v3 * a2->z;
    }
    v4 = a1->flags & 0x400000;
    v5 = a1->flags & 0x4000000;
    if (v4)
    {
        v6 = a1->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    }
    if (a2->x != 0.0)
    {
        MATH_RotateMatrix_AroundLocalXAxis(a1->matrices, a2->x);
        if (v5)
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundLocalXAxis(a1->matrices + 2, a2->x);
                else
                    MATH_RotateMatrix_AroundLocalXAxis(a1->matrices + 1, a2->x);
            }
            else
            {
                MATH_RotateMatrix_AroundLocalXAxis(0, a2->x);
            }
        }
    }
    if (a2->y != 0.0)
    {
        MATH_RotateMatrix_AroundLocalYAxis(a1->matrices, a2->y);
        if (v5)
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundLocalYAxis(a1->matrices + 2, a2->y);
                else
                    MATH_RotateMatrix_AroundLocalYAxis(a1->matrices + 1, a2->y);
            }
            else
            {
                MATH_RotateMatrix_AroundLocalYAxis(0, a2->y);
            }
        }
    }
    if (a2->z != 0.0)
    {
        MATH_RotateMatrix_AroundLocalZAxis(a1->matrices, a2->z);
        if (v5)
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundLocalZAxis(a1->matrices + 2, a2->z);
                else
                    MATH_RotateMatrix_AroundLocalZAxis(a1->matrices + 1, a2->z);
            }
            else
            {
                MATH_RotateMatrix_AroundLocalZAxis(0, a2->z);
            }
        }
    }
    if (v4)
    {
        OBJ_ComputeLocalWhenHie(a1);
        a1->basic->mtx->mtx.Pos = v6;
    }
}

tAINode* AI_EvalFunc_OBJRotateLocal(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_OBJRotateLocal_C(v4, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateLocalX_C(struct sGameObjectMain* a1, float a2)
{
    float v2;
    int v3;
    Vec3 v4;

    if (a2 != 0.0)
    {
        if (ENG_gb_EVERunning)
        {
            v2 = (float)EVE_gpst_CurrentEvent->duration;
            a2 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0) * a2);
        }
        v3 = a1->flags & 0x400000;
        if (v3)
        {
            v4 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        }
        if (a2 != 0.0)
        {
            MATH_RotateMatrix_AroundLocalXAxis(a1->matrices, a2);
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundLocalXAxis(a1->matrices + 2, a2);
                else
                    MATH_RotateMatrix_AroundLocalXAxis(a1->matrices + 1, a2);
            }
            if (ANI_GaoAnimationIsTransitioning(a1))
                MATH_RotateMatrix_AroundLocalXAxis(&(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[1]->flashMtx), a2);
        }
        if (v3)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v4;
        }
    }
}

tAINode* AI_EvalFunc_OBJRotateLocalX(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateLocalX_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateLocalY_C(struct sGameObjectMain* a1, float angle)
{
    float v2;
    int v3;
    Vec3 v4;

    if (angle != 0.0)
    {
        if (ENG_gb_EVERunning)
        {
            v2 = (float)EVE_gpst_CurrentEvent->duration;
            angle = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0) * angle);
        }
        v3 = a1->flags & 0x400000;
        if (v3)
        {
            v4 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        }
        if (angle != 0.0)
        {
            MATH_RotateMatrix_AroundLocalYAxis(a1->matrices, angle);
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundLocalYAxis(a1->matrices + 2, angle);
                else
                    MATH_RotateMatrix_AroundLocalYAxis(a1->matrices + 1, angle);
            }
            if (ANI_GaoAnimationIsTransitioning(a1))
                MATH_RotateMatrix_AroundLocalYAxis(&(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[1]->flashMtx), angle);
        }
        if (v3)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v4;
        }
    }
}

tAINode* AI_EvalFunc_OBJRotateLocalY(tAINode* a1)
{
    tGameObjectMain* v2;
    float angle;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateLocalY_C(v2, angle);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateLocalZ_c(struct sGameObjectMain* a1, float a2)
{
    float v2;
    int v3;
    Vec3 v4;

    if (a2 != 0.0)
    {
        if (ENG_gb_EVERunning)
        {
            v2 = (float)EVE_gpst_CurrentEvent->duration;
            a2 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0) * a2);
        }
        v3 = a1->flags & 0x400000;
        if (v3)
        {
            v4 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        }
        if (a2 != 0.0)
        {
            MATH_RotateMatrix_AroundLocalZAxis(a1->matrices, a2);
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundLocalZAxis(a1->matrices + 2, a2);
                else
                    MATH_RotateMatrix_AroundLocalZAxis(a1->matrices + 1, a2);
            }
            if (ANI_GaoAnimationIsTransitioning(a1))
                MATH_RotateMatrix_AroundLocalZAxis(&(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[1]->flashMtx), a2);
        }
        if (v3)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v4;
        }
    }
}

tAINode* AI_EvalFunc_OBJRotateLocalZ(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateLocalZ_c(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateGlobal_C(struct sGameObjectMain* a1, Vec3* a2)
{
    float v2;
    float v3;
    int v4;
    int v5;

    if (ENG_gb_EVERunning)
    {
        v2 = (float)EVE_gpst_CurrentEvent->duration;
        v3 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0));
        a2->x = v3 * a2->x;
        a2->y = v3 * a2->y;
        a2->z = v3 * a2->z;
    }
    v4 = a1->flags & 0x400000;
    v5 = a1->flags & 0x4000000;
    if (v4)
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    if (a2->x != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalXAxis(a1->matrices, a2->x);
        if (v5)
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundGlobalXAxis(a1->matrices + 2, a2->x);
                else
                    MATH_RotateMatrix_AroundGlobalXAxis(a1->matrices + 1, a2->x);
            }
            else
            {
                MATH_RotateMatrix_AroundGlobalXAxis(0, a2->x);
            }
        }
    }
    if (a2->y != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalYAxis(a1->matrices, a2->y);
        if (v5)
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundGlobalYAxis(a1->matrices + 2, a2->y);
                else
                    MATH_RotateMatrix_AroundGlobalYAxis(a1->matrices + 1, a2->y);
            }
            else
            {
                MATH_RotateMatrix_AroundGlobalYAxis(0, a2->y);
            }
        }
    }
    if (a2->z != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalZAxis(a1->matrices, a2->z);
        if (v5)
        {
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundGlobalZAxis(a1->matrices + 2, a2->z);
                else
                    MATH_RotateMatrix_AroundGlobalZAxis(a1->matrices + 1, a2->z);
            }
            else
            {
                MATH_RotateMatrix_AroundGlobalZAxis(0, a2->z);
            }
        }
    }
    if (v4)
        OBJ_ComputeLocalWhenHie(a1);
}

tAINode* AI_EvalFunc_OBJRotateGlobal(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_OBJRotateGlobal_C(v4, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateGlobalX_C(struct sGameObjectMain* a1, float a2)
{
    float v2;
    int v3;
    Vec3 v4;

    if (a2 != 0.0)
    {
        if (ENG_gb_EVERunning)
        {
            v2 = (float)EVE_gpst_CurrentEvent->duration;
            a2 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0) * a2);
        }
        v3 = a1->flags & 0x400000;
        if (v3)
        {
            v4 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        }
        if (a2 != 0.0)
        {
            MATH_RotateMatrix_AroundGlobalXAxis(a1->matrices, a2);
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundGlobalXAxis(a1->matrices + 2, a2);
                else
                    MATH_RotateMatrix_AroundGlobalXAxis(a1->matrices + 1, a2);
            }
        }
        if (v3)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v4;
        }
    }
}

tAINode* AI_EvalFunc_OBJRotateGlobalX(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateGlobalX_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateGlobalY_C(struct sGameObjectMain* a1, float a2)
{
    float v2;
    int v3;
    Vec3 v4;

    if (a2 != 0.0)
    {
        if (ENG_gb_EVERunning)
        {
            v2 = (float)EVE_gpst_CurrentEvent->duration;
            a2 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0) * a2);
        }
        v3 = a1->flags & 0x400000;
        if (v3)
        {
            v4 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        }
        if (a2 != 0.0)
        {
            MATH_RotateMatrix_AroundGlobalYAxis(a1->matrices, a2);
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundGlobalYAxis(a1->matrices + 2, a2);
                else
                    MATH_RotateMatrix_AroundGlobalYAxis(a1->matrices + 1, a2);
            }
        }
        if (v3)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v4;
        }
    }
}

tAINode* AI_EvalFunc_OBJRotateGlobalY(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateGlobalY_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_OBJRotateGlobalZ_C(struct sGameObjectMain* a1, float a2)
{
    float v2;
    int v3;
    Vec3 v4;

    if (a2 != 0.0)
    {
        if (ENG_gb_EVERunning)
        {
            v2 = (float)EVE_gpst_CurrentEvent->duration;
            a2 = (float)(EVE_gf_DT / (v2 * 1.0 / 60.0) * a2);
        }
        v3 = a1->flags & 0x400000;
        if (v3)
        {
            v4 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        }
        if (a2 != 0.0)
        {
            MATH_RotateMatrix_AroundGlobalZAxis(a1->matrices, a2);
            if ((a1->flags & 0x4000000) != 0)
            {
                if ((a1->flags & 0x10000) != 0)
                    MATH_RotateMatrix_AroundGlobalZAxis(a1->matrices + 2, a2);
                else
                    MATH_RotateMatrix_AroundGlobalZAxis(a1->matrices + 1, a2);
            }
        }
        if (v3)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v4;
        }
    }
}

tAINode* AI_EvalFunc_OBJRotateGlobalZ(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateGlobalZ_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_OBJGetPos_C(struct sGameObjectMain* a2, Vec3* a3)
{
    tGaoMatrix* v3;
    Mtx4x4 v4;
    tGameObjectMain* v5;
    Mtx4x4 v6;
    tGameObjectMain* v7;
    Vec3* v8;
    Vec3* v9;
    Vec3* v10;

    if ((a2->flags & 1) != 0)
    {
        v7 = ANI_pst_GetReference(a2);
        MATH_SetIdentityMatrix(&v6);
        v5 = a2;
        while (v5 != v7)
        {
            v3 = OBJ_pst_GetLocalMatrix(v5);
            MATH_MulMatrixMatrix(&v4, &v6, &v3->mtx);
            if (&v6 != &v4)
                memcpy(&v6, &v4, sizeof(v6));
            if (v5 && v5->basic && v5->basic->mtx)
                v5 = v5->basic->mtx->refGao2;
            else
                v5 = 0;
        }
        MATH_MulMatrixMatrix(&v4, &v6, v7->matrices);
        *a3 = v4.Pos;
    }
    else
    {
        v10 = &a2->matrices->Pos;
        v9 = a3;
        v8 = v10;
        a3->x = v10->x;
        v9->y = v8->y;
        v9->z = v8->z;
    }
}

void AI_EvalFunc_OBJGetPos_C_CURRENT(Vec3* a1)
{
    AI_EvalFunc_OBJGetPos_C(AI_gpst_CurrentGameObject, a1);
}

tAINode* AI_EvalFunc_OBJGetPos(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a3;
    tGameObjectMain* a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    AI_EvalFunc_OBJGetPos_C(v2, &a3);
    AI_PutVecToStack(&a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJRestoreInitMatrix(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    OBJ_RestoreInitialPos(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJGetPosInit_C(struct sGameObjectMain* a1, Vec3* a2)
{
    Vec3* v2;

    if ((a1->flags & 0x10000) != 0)
        v2 = &a1->matrices[1].Pos;
    else
        v2 = &a1->matrices->Pos;
    *a2 = *v2;
}

tAINode* AI_EvalFunc_OBJGetPosInit(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_OBJGetPosInit_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJSetPos_C(struct sGameObjectMain* a1, Vec3* a2)
{
    int v2;

    v2 = a1->flags & 0x400000;
    if (v2)
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    OBJ_SetAbsolutePosition(a1, a2);
    if (v2)
        OBJ_ComputeLocalWhenHie(a1);
    if ((a1->flags & 0x4000000) != 0)
        ANI_UpdateFlash(a1, 3);
    if ((a1->flags & 4) != 0)
        a1->basic->dyn->flags |= 0x800000u;
}

void AI_EvalFunc_OBJSetPos_C_CURRENT(Vec3* a1)
{
    AI_EvalFunc_OBJSetPos_C(AI_gpst_CurrentGameObject, a1);
}

tAINode* AI_EvalFunc_OBJSetPos(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJSetPos_C(v2, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_OBJSetLocalPos_C(struct sGameObjectMain* a1, Vec3* a2)
{
    OBJ_SetLocalPosition(a1, a2);
    if ((a1->flags & 0x4000000) != 0)
        ANI_UpdateFlash(a1, 3);
    if ((a1->flags & 4) != 0)
        a1->basic->dyn->flags |= 0x800000u;
}

tAINode* AI_EvalFunc_OBJSetLocalPos(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJSetLocalPos_C(v2, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_Rejoin_C(tGameObjectMain* a1, tGameObjectMain* a2)
{
    int v2;

    if (a2)
    {
        v2 = a1->flags & 0x400000;
        if (v2)
            OBJ_ComputeGlobalWhenHie(a2);
        OBJ_SetAbsolutePosition(a1, &a2->matrices->Pos);
        if (v2)
            OBJ_ComputeLocalWhenHie(a1);
        if ((a1->flags & 0x4000000) != 0)
            ANI_UpdateFlash(a1, 3);
    }
}

tAINode* AI_EvalFunc_OBJ_Rejoin(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_Rejoin_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_RejoinAndOrient_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    Mtx4x4* v2;
    int v3;

    if (a2)
    {
        v3 = a1->flags & 0x400000;
        if (v3)
            OBJ_ComputeGlobalWhenHie(a2);
        v2 = OBJ_pst_GetAbsoluteMatrix(a2);
        OBJ_SetAbsoluteMatrix(a1, v2);
        if (v3)
            OBJ_ComputeLocalWhenHie(a1);
        if ((a1->flags & 0x4000000) != 0)
            ANI_UpdateFlash(a1, 3);
        if ((a1->flags & 4) != 0)
            a1->basic->dyn->flags |= 0x800000u;
    }
}

tAINode* AI_EvalFunc_OBJ_RejoinAndOrient(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_RejoinAndOrient_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_OBJReinit_C(struct sGameObjectMain* a1, int a2)
{
    byte v2;

    v2 = a1->if_this_not0_wont_render_gao;
    ENG_ReinitOneObject(a1, a2);
    if (a2 == 1000)
        a1->if_this_not0_wont_render_gao = v2;
    INT_SnP_AttachObject(a1, WOR_gpst_CurrentWorld);
}

tAINode* AI_EvalFunc_OBJReinit(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJReinit_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_ExtraFlagsSet_C(struct sGameObjectMain* a1, __int16 a2, __int16 a3)
{
    if (a1->extended)
        a1->extended->extraFlag = a2 | ~a3 & a1->extended->extraFlag;
}

tAINode* AI_EvalFunc_OBJ_ExtraFlagsSet(tAINode* a1)
{
    tGameObjectMain* v2;
    __int16 a3;
    __int16 a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_ExtraFlagsSet_C(v2, a2, a3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_AILodSet_C(struct sGameObjectMain* a1, float a2)
{
    if (a1->extended)
        a1->extended->aiLod = a2;
}

tAINode* AI_EvalFunc_OBJ_AILodSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_OBJ_AILodSet_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_AILodBV_C(tGameObjectMain* a1)
{
    float v1;
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    Vec3 v16;
    Vec3 a1a;
    Vec3 v18;
    float v19;

    if (a1->extended)
    {
        if (OBJ_BV_IsAABBox(a1->boundingVolume))
            v18 = a1->boundingVolume->BVMinOriginal;
        else
            v18 = MATH_gst_NulVector;
        if (OBJ_BV_IsAABBox(a1->boundingVolume))
            v16 = a1->boundingVolume->BVMaxOriginal;
        else
            v16 = MATH_gst_NulVector;
        v15 = (float)fabs(v18.x);
        v14 = (float)fabs(v16.x);
        if (v14 >= (double)v15)
        {
            v11 = (float)fabs(v16.x);
            v12 = v11;
        }
        else
        {
            v13 = (float)fabs(v18.x);
            v12 = v13;
        }
        a1a.x = v12;
        v10 = (float)fabs(v18.y);
        v9 = (float)fabs(v16.y);
        if (v9 >= (double)v10)
        {
            v6 = (float)fabs(v16.y);
            v7 = v6;
        }
        else
        {
            v8 = (float)fabs(v18.y);
            v7 = v8;
        }
        a1a.y = v7;
        v5 = (float)fabs(v18.z);
        v4 = (float)fabs(v16.z);
        if (v4 >= (double)v5)
        {
            v1 = (float)fabs(v16.z);
            v2 = v1;
        }
        else
        {
            v3 = (float)fabs(v18.z);
            v2 = v3;
        }
        a1a.z = v2;
        v19 = MATH_VecLen(&a1a);
        v19 = v19 * v19;
        v19 = (float)(v19 + 8.0);
        a1->extended->extraFlag |= 8u;
        a1->extended->aiLod = v19;
    }
}

tAINode* AI_EvalFunc_OBJ_AILodBV(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_AILodBV_C(v2);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_SqrDist_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    Vec3 dest;

    MATH_SubstractVec(&dest, &a2->matrices->Pos, &a1->matrices->Pos);
    return dest.z * dest.z + dest.y * dest.y + dest.x * dest.x;
}

tAINode* AI_EvalFunc_OBJ_SqrDist(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = AI_EvalFunc_OBJ_SqrDist_C(v3, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_SqrDistHorz_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    Vec3 dest;

    MATH_SubstractVec(&dest, &a2->matrices->Pos, &a1->matrices->Pos);
    return 0.0f * 0.0f + dest.y * dest.y + dest.x * dest.x;
}

tAINode* AI_EvalFunc_OBJ_SqrDistHorz(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(v3, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

unsigned int AI_EvalFunc_OBJ_FlagsControlSet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    unsigned int v4;
    tWorld* v5;

    v4 = a2 | ~a3 & a1->flags2 & 0xFFFF0000;
    a1->flags2 = (uint16_t)a1->flags2;
    a1->flags2 |= v4 & 0xFFFF0000;
    if ((a2 & 0x400000) != 0)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_RegisterInactiveGAO(a1);
        PRO_QPC_RecordTimeDecRef(39);
    }
    if ((a2 & 0x400000) != 0 || (a3 & 0x400000) != 0)
    {
        v5 = WOR_gpst_CurrentWorld;
        WOR_gpst_CurrentWorld->tablesManagerAndEOTNeeded = 1;
        v5->visibilityCallNeeded = 1;
        v5->SnPRefreshNeeded = TRUE;
    }
    if ((a2 & 0x200000) != 0 || (a3 & 0x200000) != 0)
        WOR_gpst_CurrentWorld->visibilityCallNeeded = 1;
    return v4 & 0xFFFF0000;
}

tAINode* AI_EvalFunc_OBJ_FlagsControlSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a3;
    int a2;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

unsigned int AI_EvalFunc_OBJ_FlagsControlGet_C(struct sGameObjectMain* a1)
{
    unsigned int v2;

    v2 = 0;
    if (a1)
        v2 = a1->flags2 & 0xFFFF0000;
    return v2;
}

tAINode* AI_EvalFunc_OBJ_FlagsControlGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_FlagInvisibleSet_C(struct sGameObjectMain* a1, int a2)
{
    unsigned int v2;

    if (a2)
    {
        if ((a1->flags2 & 0x200000) != 0)
            return;
        v2 = a1->flags2 & 0xFFFF0000 | 0x200000;
    }
    else
    {
        if ((a1->flags2 & 0x200000) == 0)
            return;
        v2 = a1->flags2 & 0xFFDF0000;
    }
    a1->flags2 = (uint16_t)a1->flags2;
    a1->flags2 |= v2 & 0xFFFF0000;
    WOR_gpst_CurrentWorld->visibilityCallNeeded = 1;
}

tAINode* AI_EvalFunc_OBJ_FlagInvisibleSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_FlagInactiveSet_C(struct sGameObjectMain* a1, int a2)
{
    unsigned int v2;
    tWorld* v3;

    if (a2)
    {
        if ((a1->flags2 & 0x400000) != 0)
            return;
        v2 = a1->flags2 & 0xFFFF0000 | 0x400000;
    }
    else
    {
        if ((a1->flags2 & 0x400000) == 0)
            return;
        v2 = a1->flags2 & ~0x40FFFFu;
    }
    a1->flags2 = (uint16_t)a1->flags2;
    a1->flags2 |= v2 & 0xFFFF0000;
    v3 = WOR_gpst_CurrentWorld;
    WOR_gpst_CurrentWorld->tablesManagerAndEOTNeeded = 1;
    v3->visibilityCallNeeded = 1;
    v3->SnPRefreshNeeded = TRUE;
    if (a2 && (a1->flags & 0x400000) != 0 && a1->basic && a1->basic->mtx && !a1->basic->mtx->refGao)
    {
        OBJ_ComputeGlobalWhenHie(a1);
        if ((a1->flags & 0x4000000) != 0)
            ANI_RemoveHierarchyOnFlash(a1);
        a1->flags &= ~0x400000u;
        MEM_Free(a1->basic->mtx);
        a1->basic->mtx = 0;
    }
    if (a2)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_RegisterInactiveGAO(a1);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_OBJ_FlagInactiveSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagInactiveSet_C(v3, v1);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_FlagsStatusGet_C(tGameObjectMain* a1)
{
    int v2;

    v2 = 0;
    if (a1)
        v2 = (uint16_t)a1->flags2;
    return v2;
}

tAINode* AI_EvalFunc_OBJ_FlagsStatusGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_FlagsStatusGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_CustomBitsSet_C(struct sGameObjectMain* a1, __int16 a2, __int16 a3)
{
    a1->flags2 &= ~(a3 & 0xFF00);
    a1->flags2 |= a2 & 0xFF00;
}

void AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(__int16 a1, __int16 a2)
{
    AI_gpst_CurrentGameObject->flags2 &= ~(a2 & 0xFF00);
    AI_gpst_CurrentGameObject->flags2 |= a1 & 0xFF00;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsSet(tAINode* a1)
{
    tGameObjectMain* v2;
    __int16 a2;
    __int16 a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_CustomBitsSet_C(v2, a2, a3);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_CustomBitsGet_C(struct sGameObjectMain* a1)
{
    return a1->flags2 & 0xFF00;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_CustomBitsGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_OBJ_CustomBitsTest_C(struct sGameObjectMain* a1, __int16 a2)
{
    return (a2 & 0xFF00 & a1->flags2 & 0xFF00) != 0;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsTest(tAINode* a1)
{
    __int16 v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    if (AI_EvalFunc_OBJ_CustomBitsTest_C(v3, v1))
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_CustomBitsNot_C(tGameObjectMain* a1, __int16 a2)
{
    a1->flags2 ^= a2 & 0xFF00;
    return a1->flags2;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsNot(tAINode* a1)
{
    __int16 v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_CustomBitsNot_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_CapaSet_C(struct sGameObjectMain* a1, int16_t set, int16_t remove)
{
    a1->extended->capa &= ~remove;
    a1->extended->capa |= set;
}

void AI_EvalFunc_OBJ_CapaSet_C_CURRENT(int16_t set, int16_t remove)
{
    AI_gpst_CurrentGameObject->extended->capa &= ~remove;
    AI_gpst_CurrentGameObject->extended->capa |= set;
}

tAINode* AI_EvalFunc_OBJ_CapaSet(tAINode* a1)
{
    tGameObjectMain* v2;
    __int16 a2;
    __int16 a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_CapaSet_C(v2, a2, a3);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_CapaGet_C(struct sGameObjectMain* a1)
{
    return a1->extended->capa;
}

tAINode* AI_EvalFunc_OBJ_CapaGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_CapaGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_OBJ_CapaTest_C(struct sGameObjectMain* a1, __int16 a2)
{
    BOOL result;

    if (a1->extended)
        result = (uint16_t)(a2 & a1->extended->capa) != 0;
    else
        result = 0;
    return result;
}

BOOL AI_EvalFunc_OBJ_CapaTest_C_CURRENT(__int16 a1)
{
    BOOL result;

    if (AI_gpst_CurrentGameObject->extended)
        result = (uint16_t)(a1 & AI_gpst_CurrentGameObject->extended->capa) != 0;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_OBJ_CapaTest(tAINode* a1)
{
    tAINode* result;
    __int16 v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (v3->extended)
    {
        v2 = AI_GetIntFromStack();
        if (AI_EvalFunc_OBJ_CapaTest_C(v3, v2))
            AI_PutIntToStack(1);
        else
            AI_PutIntToStack(0);
        result = a1 + 1;
    }
    else
    {
        AI_PutIntToStack(0);
        result = a1 + 1;
    }
    return result;
}

int AI_EvalFunc_OBJ_CapaNot_C(struct sGameObjectMain* a1, __int16 a2)
{
    a1->extended->capa ^= a2;
    return a1->flags2;
}

tAINode* AI_EvalFunc_OBJ_CapaNot(tAINode* a1)
{
    __int16 v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_CapaNot_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SightGeneralSet_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    int v3;
    Vec3 v4;

    v3 = a1->flags & 0x400000;
    if (v3)
    {
        v4 = a1->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    }
    OBJ_SightGeneralSet(a1, a2, a3);
    if (v3)
    {
        OBJ_ComputeLocalWhenHie(a1);
        a1->basic->mtx->mtx.Pos = v4;
    }
    if ((a1->flags & 0x4000000) != 0)
        ANI_UpdateFlash(a1, 3);
}

tAINode* AI_EvalFunc_OBJ_SightGeneralSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    Vec3 a1a;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_OBJ_SightGeneralSet_C(v5, &a2, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SightSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    int v2;
    Vec3 v3;

    v2 = a1->flags & 0x400000;
    if (v2)
    {
        v3 = a1->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    }
    OBJ_SightSet(a1, a2);
    if (v2)
    {
        OBJ_ComputeLocalWhenHie(a1);
        a1->basic->mtx->mtx.Pos = v3;
    }
    if ((a1->flags & 0x4000000) != 0)
        ANI_UpdateFlash(a1, 3);
}

tAINode* AI_EvalFunc_OBJ_SightSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_OBJ_SightSet_C(v4, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_BoneDisplayOrder_C(struct sGameObjectMain* a1, int a2, char a3)
{
    if (a2 != -1)
        a1 = ANI_pst_GetObjectByAICanal(a1, a2);
    if (a1 && (a1->flags & 0x4000) != 0 && a1->basic)
    {
        if (a1->basic->actorObjectOrVisualObject)
            ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum = a3;
    }
}

tAINode* AI_EvalFunc_OBJ_BoneDisplayOrder(tAINode* a1)
{
    tGameObjectMain* v2;
    char a3;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_BoneDisplayOrder_C(v2, a2, a3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_LocalSightSet_C(tGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    if ((a1->flags & 0x400000) != 0)
    {
        OBJ_SightGeneralSet(a1, a2, a3);
        MATH_Copy33Matrix(&a1->basic->mtx->mtx, a1->matrices);
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
        if ((a1->flags & 0x4000000) != 0)
            ANI_UpdateFlash(a1, 3);
    }
}

tAINode* AI_EvalFunc_OBJ_LocalSightSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    Vec3 a1a;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_OBJ_LocalSightSet_C(v5, &a2, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_OrientTo_C(tGameObjectMain* a1, tGameObjectMain* a2)
{
    int v2;
    Vec3 dest;
    Vec3 a3;
    Vec3 v5;

    if (a2)
    {
        v2 = a1->flags & 0x400000;
        if (v2)
        {
            v5 = a1->basic->mtx->mtx.Pos;
            OBJ_ComputeGlobalWhenHie(a1);
        }
        if ((a2->flags & 0x400000) != 0)
            OBJ_ComputeGlobalWhenHie(a2);
        dest = a2->matrices->Pos;
        MATH_SubstractVec(&dest, &dest, &a1->matrices->Pos);
        a3.x = 0.0;
        a3.y = 0.0;
        a3.z = 1.0;
        OBJ_SightGeneralSet(a1, &dest, &a3);
        if (v2)
        {
            OBJ_ComputeLocalWhenHie(a1);
            a1->basic->mtx->mtx.Pos = v5;
        }
        if ((a1->flags & 0x4000000) != 0)
            ANI_UpdateFlash(a1, 3);
    }
}

tAINode* AI_EvalFunc_OBJ_OrientTo(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_OrientTo_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SightGet_C(struct sGameObjectMain* a2, Vec3* a3)
{
    Vec3* v3;
    Mtx4x4 v4;

    if ((a2->flags & 0x400000) != 0)
    {
        OBJ_ComputeGlobalWithLocal(a2, &v4, 1);
        a3->x = -v4.v2.x;
        a3->y = -v4.v2.y;
        a3->z = -v4.v2.z;
    }
    else
    {
        v3 = &a2->matrices->v2;
        a3->x = -v3->x;
        a3->y = -v3->y;
        a3->z = -v3->z;
    }
}

void AI_EvalFunc_OBJ_SightGet_C_CURRENT(Vec3* a1)
{
    AI_EvalFunc_OBJ_SightGet_C(AI_gpst_CurrentGameObject, a1);
}

tAINode* AI_EvalFunc_OBJ_SightGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a3;
    tGameObjectMain* a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    AI_EvalFunc_OBJ_SightGet_C(v2, &a3);
    AI_PutVecToStack(&a3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SightInitGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    if ((a1->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWhenHie(a1);
    OBJ_SightInitGet(a1, a2);
}

tAINode* AI_EvalFunc_OBJ_SightInitGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_OBJ_SightInitGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_BankingGeneralSet_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    int v3;
    Vec3 v4;

    v3 = a1->flags & 0x400000;
    if (v3)
    {
        v4 = a1->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    }
    OBJ_BankingGeneralSet(a1, a2, a3);
    if (v3)
    {
        OBJ_ComputeLocalWhenHie(a1);
        a1->basic->mtx->mtx.Pos = v4;
    }
    if ((a1->flags & 0x4000000) != 0)
        ANI_UpdateFlash(a1, 3);
}

void AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(Vec3* a1, Vec3* a3)
{
    AI_EvalFunc_OBJ_BankingGeneralSet_C(AI_gpst_CurrentGameObject, a1, a3);
}

tAINode* AI_EvalFunc_OBJ_BankingGeneralSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    Vec3 a1a;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_OBJ_BankingGeneralSet_C(v5, &a2, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_BankingSet_C(struct sGameObjectMain* a1, Vec3* a3)
{
    int v2;
    Vec3 v3;

    v2 = a1->flags & 0x400000;
    if (v2)
    {
        v3 = a1->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    }
    OBJ_BankingSet(a1, a3);
    if (v2)
    {
        OBJ_ComputeLocalWhenHie(a1);
        a1->basic->mtx->mtx.Pos = v3;
    }
    if ((a1->flags & 0x4000000) != 0)
        ANI_UpdateFlash(a1, 3);
}

tAINode* AI_EvalFunc_OBJ_BankingSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_OBJ_BankingSet_C(v4, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_BankingGet_C(struct sGameObjectMain* a2, Vec3* a3)
{
    Mtx4x4 v3;

    if ((a2->flags & 0x400000) != 0)
    {
        OBJ_ComputeGlobalWithLocal(a2, &v3, 1);
        *a3 = v3.v3;
    }
    else
    {
        *a3 = a2->matrices->v3;
    }
}

void AI_EvalFunc_OBJ_BankingGet_C_CURRENT(Vec3* a1)
{
    AI_EvalFunc_OBJ_BankingGet_C(AI_gpst_CurrentGameObject, a1);
}

tAINode* AI_EvalFunc_OBJ_BankingGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a3;
    tGameObjectMain* a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    AI_EvalFunc_OBJ_BankingGet_C(v2, &a3);
    AI_PutVecToStack(&a3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_HorizonGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    if ((a1->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    OBJ_HorizonGet(a1, a2);
}

void AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(Vec3* a2)
{
    if ((AI_gpst_CurrentGameObject->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWithLocal(AI_gpst_CurrentGameObject, AI_gpst_CurrentGameObject->matrices, 1);
    OBJ_HorizonGet(AI_gpst_CurrentGameObject, a2);
}

tAINode* AI_EvalFunc_OBJ_HorizonGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_OBJ_HorizonGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_ScaleSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    AI_GetVecFromStack(&a1a);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    OBJ_ScaleSet(v2, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_ScaleGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    MATH_GetScale(a2, a1->matrices);
}

tAINode* AI_EvalFunc_OBJ_ScaleGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_OBJ_ScaleGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_ZoomSet_C(struct sGameObjectMain* a1, float zoom)
{
    int v2;

    v2 = a1->flags & 0x400000;
    if (v2)
        OBJ_ComputeGlobalWithLocal(a1, a1->matrices, 1);
    OBJ_ZoomSet(a1, zoom);
    if (v2)
        OBJ_ComputeLocalWhenHie(a1);
}

tAINode* AI_EvalFunc_OBJ_ZoomSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float zoom;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    zoom = AI_GetFloatFromStack();
    AI_EvalFunc_OBJ_ZoomSet_C(v2, zoom);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_ZoomGet_C(struct sGameObjectMain* a2)
{
    Vec3 a1;

    MATH_GetScale(&a1, a2->matrices);
    return float_get_max_of_3vals_sub_48BBB0(a1.x, a1.y, a1.z);
}

tAINode* AI_EvalFunc_OBJ_ZoomGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_OBJ_ZoomGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_FlagsIdentityGet_C(struct sGameObjectMain* a1)
{
    int v2;

    if (a1)
        v2 = a1->flags;
    else
        v2 = 0;
    return v2;
}

tAINode* AI_EvalFunc_OBJ_FlagsIdentityGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_FlagsIdentitySet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    int a2a;
    int a2b;

    if (a1)
        a2a = a1->flags;
    else
        a2a = 0;
    a2b = a2 | ~a3 & a2a;
    if (a1)
        OBJ_ChangeIdentityFlags(a1, a2b, a1->flags);
    else
        OBJ_ChangeIdentityFlags(0, a2b, 0);
}

tAINode* AI_EvalFunc_OBJ_FlagsIdentitySet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(v2, a2, a3);
    return a1 + 1;
}

BOOL AI_EvalFunc_OBJ_FlagsIdentityTest_C(struct sGameObjectMain* a1, int a2)
{
    int v3;

    if (a1)
        v3 = a1->flags;
    else
        v3 = 0;
    return (a2 & v3) != 0;
}

tAINode* AI_EvalFunc_OBJ_FlagsIdentityTest(tAINode* a1)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_FlagsDesignGet_C(struct sGameObjectMain* a1)
{
    byte v2;

    v2 = 0;
    if (a1)
        v2 = a1->designFlags;
    return v2;
}

tAINode* AI_EvalFunc_OBJ_FlagsDesignGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_FlagsDesignGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_FlagsDesignSet_C(struct sGameObjectMain* a1, char a2, char a3)
{
    byte v4;

    if (!a1)
        return -1;
    v4 = a2 | ~a3 & a1->designFlags;
    a1->designFlags = v4;
    return v4;
}

tAINode* AI_EvalFunc_OBJ_FlagsDesignSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char v4;
    char v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_OBJ_FlagsDesignSet_C(v3, v4, v5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tGameObjectMain* AI_EvalFunc_OBJ_FlagsFixSet_C(tGameObjectMain* a1, char a2, char a3)
{
    tGameObjectMain* result; // eax

    a1->process &= ~a3;
    result = a1;
    a1->process |= a2;
    return result;
}

tAINode* AI_EvalFunc_OBJ_FlagsFixSet(tAINode* a1)
{
    tGameObjectMain* v2;
    char a3;
    char a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagsFixSet_C(v2, a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_FlagsFixGet(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(v2->process);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_FlagsFixSetInit(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2->process |= AI_GetIntFromStack() & 0xC;
    return a1 + 1;
}

void AI_EvalFunc_OBJ_MorphCopy_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    int v3;
    int v4;
    tMorphMdfStruct* v5;
    int j;
    int v7;
    tMorphMdfStruct* v8;
    tMorphMdfInner2Struct* v9;
    tMorphMdfInner2Struct* i;

    v8 = GEO_pst_ModifierMorphing_Get(a1);
    if (v8)
    {
        v5 = GEO_pst_ModifierMorphing_Get(a2);
        if (v5)
        {
            if (v8->num2 <= v5->num2)
                v4 = v8->num2;
            else
                v4 = v5->num2;
            v7 = v4;
            v9 = v8->ptr2;
            for (i = v5->ptr2; v7--; ++i)
            {
                v9->Factor = i->Factor;
                v9->Prog = i->Prog;
                if (v9->num >= i->num)
                    v3 = i->num;
                else
                    v3 = v9->num;
                for (j = 0; j < v3; ++j)
                {
                    v9->ptr[j] = i->ptr[j];
                    if (v9->ptr[j] >= v8->num)
                        v9->ptr[j] = -1;
                }
                ++v9;
            }
        }
    }
}

tAINode* AI_EvalFunc_OBJ_MorphCopy(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_MorphCopy_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_NumberOfData_C(tGameObjectMain* a1)
{
    int result;
    tMorphMdfStruct* v2;

    v2 = GEO_pst_ModifierMorphing_Get(a1);
    if (v2)
        result = v2->num;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_OBJ_NumberOfData(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_NumberOfData_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_NumberOfChannel_C(tGameObjectMain* a1)
{
    int result;
    tMorphMdfStruct* v2;

    v2 = GEO_pst_ModifierMorphing_Get(a1);
    if (v2)
        result = v2->num2;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_OBJ_NumberOfChannel(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_NumberOfChannel_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_GetChannelFactor_C(struct sGameObjectMain* a1, int a2)
{
    tMorphMdfStruct* v3;

    v3 = GEO_pst_ModifierMorphing_Get(a1);
    if (!v3)
        return 0.0;
    if (a2 >= 0 && a2 < v3->num2)
        return v3->ptr2[a2].Factor;
    return 0.0;
}

tAINode* AI_EvalFunc_OBJ_GetChannelFactor(tAINode* a1)
{
    int v1;
    float v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v3 = AI_EvalFunc_OBJ_GetChannelFactor_C(v4, v1);
    AI_PutFloatToStack(v3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SetChannelFactor_C(struct sGameObjectMain* a1, int a2, float a3)
{
    tMorphMdfStruct* v3;

    v3 = GEO_pst_ModifierMorphing_Get(a1);
    if (v3 && a2 >= 0 && a2 < v3->num2)
        v3->ptr2[a2].Factor = a3;
}

tAINode* AI_EvalFunc_OBJ_SetChannelFactor(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetChannelFactor_C(v3, v1, a3);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_GetChannelProg_C(struct sGameObjectMain* a1, int a2)
{
    tMorphMdfStruct* v3;

    v3 = GEO_pst_ModifierMorphing_Get(a1);
    if (!v3)
        return 0.0;
    if (a2 >= 0 && a2 < v3->num2)
        return v3->ptr2[a2].Prog;
    return 0.0;
}

tAINode* AI_EvalFunc_OBJ_GetChannelProg(tAINode* a1)
{
    int v1;
    float v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v3 = AI_EvalFunc_OBJ_GetChannelProg_C(v4, v1);
    AI_PutFloatToStack(v3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SetChannelProg_C(struct sGameObjectMain* a1, int a2, float a3)
{
    tMorphMdfStruct* v3;

    v3 = GEO_pst_ModifierMorphing_Get(a1);
    if (v3 && a2 >= 0 && a2 < v3->num2)
        v3->ptr2[a2].Prog = a3;
}

tAINode* AI_EvalFunc_OBJ_SetChannelProg(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetChannelProg_C(v3, v1, a3);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_GetChannelData_C(struct sGameObjectMain* a1, int a2, int a3)
{
    int result;
    tMorphMdfStruct* v4;
    tMorphMdfInner2Struct* v5;

    v4 = GEO_pst_ModifierMorphing_Get(a1);
    if (!v4)
        return 0;
    if (a2 < 0 || a2 >= v4->num2)
        return 0;
    v5 = &v4->ptr2[a2];
    if (a3 >= 0 && a3 < v5->num)
        result = v5->ptr[a3];
    else
        result = v5->num;
    return result;
}

tAINode* AI_EvalFunc_OBJ_GetChannelData(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_GetChannelData_C(v5, v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SetChannelData_C(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tMorphMdfStruct* v4;
    tMorphMdfInner2Struct* v5;

    v4 = GEO_pst_ModifierMorphing_Get(a1);
    if (v4)
    {
        if (a2 >= 0 && a2 < v4->num2)
        {
            v5 = &v4->ptr2[a2];
            if (a4 >= 0 && a4 < v5->num && a3 < v4->num)
                v5->ptr[a4] = a3;
        }
    }
}

tAINode* AI_EvalFunc_OBJ_SetChannelData(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetChannelData_C(v4, v1, v3, a4);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_SetModuleChannelData_C(tGameObjectMain* a1, int a2, int a3, int a4, int a5)
{
    tMorphMdfStruct* v5;
    tMorphMdfInner2Struct* v6;

    if (a2 == -1 || (a1 = ANI_pst_GetObjectByAICanal(a1, a2)) != 0)
    {
        v5 = GEO_pst_ModifierMorphing_Get(a1);
        if (v5)
        {
            if (a3 >= 0 && a3 < v5->num2)
            {
                v6 = &v5->ptr2[a3];
                if (a4 >= 0 && a4 < v6->num && a5 < v5->num)
                    v6->ptr[a4] = a5;
            }
        }
    }
}

tAINode* AI_EvalFunc_OBJ_SetModuleChannelData(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;
    int a5;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetModuleChannelData_C(v4, v1, v3, a4, a5);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_ResetModuleChannelData_C(tGameObjectMain* a1, int a2, int a3)
{
    tMorphMdfStruct* v3;
    tMorphMdfInner2Struct* v4;
    int i;

    if (a2 == -1 || (a1 = ANI_pst_GetObjectByAICanal(a1, a2)) != 0)
    {
        v3 = GEO_pst_ModifierMorphing_Get(a1);
        if (v3 && a3 >= 0 && a3 < v3->num2)
        {
            v4 = &v3->ptr2[a3];
            for (i = 0; i < v4->num; ++i)
                v4->ptr[i] = -1;
        }
    }
}

tAINode* AI_EvalFunc_OBJ_ResetModuleChannelData(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_ResetModuleChannelData_C(v4, v1, v3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_AnimMorphFactor_C(tGameObjectMain* a1, tGameObjectMain* a2, int a3, int a4, int a5, int a6)
{
    if (a3 == -1 || (a2 = ANI_pst_GetObjectByAICanal(a2, a3)) != 0)
    {
        AI_EvalFunc_OBJ_SetChannelData_C(a2, a4, a5, 0);
        AI_EvalFunc_OBJ_SetChannelData_C(a2, a4, a6, 1);
        AI_EvalFunc_OBJ_SetChannelFactor_C(a2, a4, a1->matrices->Pos.x);
        AI_EvalFunc_OBJ_SetChannelProg_C(a2, a4, a1->matrices->Pos.y);
    }
}

tAINode* AI_EvalFunc_OBJ_AnimMorphFactor(tAINode* a1)
{
    tGameObjectMain* v2;
    int a5;
    int a6;
    int a3;
    int a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_AnimMorphFactor_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3, a4, a5, a6);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_AnimMorphFactorNoKey_C(tGameObjectMain* a1, tGameObjectMain* a2, int a3, int a4)
{
    float v4;

    if (a3 == -1 || (a2 = ANI_pst_GetObjectByAICanal(a2, a3)) != 0)
    {
        AI_EvalFunc_OBJ_SetChannelFactor_C(a2, a4, a1->matrices->Pos.x);
        v4 = (float)(a1->matrices->Pos.y + 1.0);
        AI_EvalFunc_OBJ_SetChannelProg_C(a2, a4, v4);
    }
}

tAINode* AI_EvalFunc_OBJ_AnimMorphFactorNoKey(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    tGameObjectMain* a2;
    int a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    if (a2)
        AI_EvalFunc_OBJ_AnimMorphFactorNoKey_C(v2, a2, a3, a4);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_RLICarteSet_C(struct sGameObjectMain* a1, unsigned int a2, int a3)
{
    tRliCarteMdfStruct* v3;
    tModifier* v4;

    v4 = MDF_pst_GetByType(a1, 23);
    if (v4 && v4->mdfPrivateDataPtr)
    {
        v3 = (tRliCarteMdfStruct*)v4->mdfPrivateDataPtr;
        if (a2 == -1)
        {
            v3->b1 = a3;
        }
        else if (a2 == -2)
        {
            v3->b2Max3 = a3;
        }
        else if (a2 < 64)
        {
            v3->colors[a2] = a3;
        }
    }
}

tAINode* AI_EvalFunc_OBJ_RLICarteSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_RLICarteSet_C(v4, v1, v3);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_LodVisGet_C(struct sGameObjectMain* a1)
{
    float v2;

    v2 = (float)a1->distance;
    return (float)(v2 * 0.0039215689);
}

tAINode* AI_EvalFunc_OBJ_LodVisGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_OBJ_LodVisGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_LodDistGet_c_inner(uint8_t a1)
{
    double result;
    float v2;
    float v3;

    if (a1 <= 127u)
    {
        v2 = (float)(8 * a1);
        result = v2;
    }
    else
    {
        v3 = (float)(((a1 - 128) << 6) + 1024);
        result = v3;
    }
    return (float)result;
}

float AI_EvalFunc_OBJ_LodDistGet_C(tGameObjectMain* a1)
{
    return AI_EvalFunc_OBJ_LodDistGet_c_inner(a1->LODDist);
}

tAINode* AI_EvalFunc_OBJ_LodDistGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_OBJ_LodDistGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

struct sGameObjectMain* AI_EvalFunc_OBJ_HierarchyGet_C(struct sGameObjectMain* a1)
{
    tGameObjectMain* v2;

    v2 = 0;
    if ((a1->flags & 0x400000) != 0)
        v2 = a1->basic->mtx->refGao2;
    return v2;
}

tAINode* AI_EvalFunc_OBJ_HierarchyGet(tAINode* a1)
{
    int v2[3];
    tGameObjectMain* v3;
    tGameObjectMain* v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v3;
    v2[0] = (int)AI_EvalFunc_OBJ_HierarchyGet_C(v3);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_HierarchyGetChildren_C(tGameObjectMain* a1, tGameObjectMain** ret)
{
    return WOR_i_GetAllChildrenOf(WOR_gpst_CurrentWorld, a1, ret);
}

tAINode* AI_EvalFunc_OBJ_HierarchyGetChilds(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v6;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v3;
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_OBJ_HierarchyGetChildren_C(v6, (tGameObjectMain**)a2.ptr);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_HierarchyReset_C(struct sGameObjectMain* a2)
{

    int v2;
    int v3;
    Mtx4x4* v4;
    Mtx4x4 v5;

    if (a2->basic && a2->basic->mtx)
    {
        if ((a2->flags & 0x4000000) != 0)
            ANI_RemoveHierarchyOnFlash(a2);
        if ((a2->flags & 0x600) != 0)
        {
            MATH_MulMatrixMatrix(&v5, a2->extended->coll->colInstance->mtx, a2->basic->mtx->refGao2->matrices);
            v4 = a2->extended->coll->colInstance->mtx;
            if (v4 != &v5)
                memcpy(v4, &v5, sizeof(Mtx4x4));
        }
        if (a2)
            v3 = a2->flags;
        else
            v3 = 0;
        if (a2)
            v2 = a2->flags;
        else
            v2 = 0;
        OBJ_ChangeIdentityFlags(a2, v2 & ~0x400000u, v3);
        TAB_PFtable_RemoveElemWithPointer(&WOR_gpst_CurrentWorld->eot.objectsWithHiearchyTablePF, a2);
    }
}

tAINode* AI_EvalFunc_OBJ_HierarchyReset(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_HierarchyReset_C(v2);
    return a1 + 1;
}

struct sGameObjectMain* AI_EvalFunc_OBJ_HierarchySet_C(struct sGameObjectMain* a2, struct sGameObjectMain* a3)
{
    Mtx4x4* v4;
    Mtx4x4 v5;
    Mtx4x4 v6;
    tGameObjectMain* v7;
    int v8;
    int v9;
    tWorld* v10;

    if (!a3 || a3 == a2)
        return 0;
    v10 = WOR_gpst_CurrentWorld;
    AI_EvalFunc_OBJ_HierarchyReset_C(a2);
    if (a2)
        v9 = a2->flags;
    else
        v9 = 0;
    if (a2)
        v8 = a2->flags;
    else
        v8 = 0;
    OBJ_ChangeIdentityFlags(a2, v8 | 0x400000, v9);
    v7 = a2->basic->mtx->refGao2;
    a2->basic->mtx->refGao2 = a3;
    OBJ_ComputeLocalWhenHie(a2);
    if ((a2->flags & 0x600) != 0)
    {
        MATH_InvertMatrix(&v6, a3->matrices);
        MATH_MulMatrixMatrix(&v5, a2->extended->coll->colInstance->mtx, &v6);
        v4 = a2->extended->coll->colInstance->mtx;
        if (v4 != &v5)
            memcpy(v4, &v5, sizeof(Mtx4x4));
    }
    if ((a2->flags & 0x4000000) != 0)
        ANI_ApplyHierarchyOnFlash(a2);
    if (v10)
        v10->tablesManagerAndEOTNeeded = 1;
    return v7;
}

tAINode* AI_EvalFunc_OBJ_HierarchySet(tAINode* a1)
{
    int v2[3];
    tGameObjectMain* v3;
    tGameObjectMain* a3;
    tGameObjectMain* a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v3;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_EvalFunc_OBJ_HierarchyReset_C(v3);
    v2[0] = (int)AI_EvalFunc_OBJ_HierarchySet_C(v3, a3);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

struct sGameObjectMain* AI_EvalFunc_OBJ_HierarchySetFather_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tGameObjectMain* v3;

    if (a2 == a1)
        return 0;
    if ((a1->flags & 0x400000) == 0)
        return 0;
    v3 = a1->basic->mtx->refGao2;
    a1->basic->mtx->refGao2 = a2;
    OBJ_ComputeLocalWhenHie(a1);
    return v3;
}

tAINode* AI_EvalFunc_OBJ_HierarchySetFather(tAINode* a1)
{
    int v2[3];
    tGameObjectMain* v3;
    tGameObjectMain* a2;
    tGameObjectMain* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v3;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2[0] = (int)AI_EvalFunc_OBJ_HierarchySetFather_C(v3, a2);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_HierarchyMoveFather_C(tGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2;
    Vec3* v3;
    BOOL v4;
    tGameObjectMain* v5;
    tGameObjectMain* a1a;
    Vec3 a2a;
    tableElemStructPF* v8;
    tableElemStructPF* v9;
    tWorld* v10;

    if ((a1->flags & 0x400000) != 0)
    {
        a1a = a1->basic->mtx->refGao2;
        if (a1a)
        {
            v3 = &a1->basic->mtx->mtx.Pos;
            v2 = OBJ_pst_GetAbsoluteMatrix(a1a);
            MATH_TransformVector(&a2a, v2, v3);
            MATH_SubstractVec(&a2a, a2, &a2a);
            OBJ_SetAbsolutePosition(a1a, &a2a);
            v10 = WOR_gpst_CurrentWorld;
            v9 = WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr;
            v8 = WOR_gpst_CurrentWorld->objectTablePF.nextElemPtr;
            while (v9 < v8)
            {
                v5 = (tGameObjectMain*)v9->objectPtr;
                v4 = !v9->objectPtr || ((int)v9->objectPtr & 3) == 1;
                if (!v4 && (v5->flags & 0x400000) != 0 && v5->basic && v5->basic->mtx && v5->basic->mtx->refGao2 == a1a)
                {
                    if (v5 == a1)
                        a2a.x = 0.0;
                    OBJ_ComputeGlobalWhenHie(v5);
                }
                ++v9;
            }
        }
    }
}

tAINode* AI_EvalFunc_OBJ_HierarchyMoveFather(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_HierarchyMoveFather_C(v2, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_IsValidGAO(tAINode* a1)
{
    tGameObjectMain* v2;
    tableElemStructPF* i;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    for (i = WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr; i < (tableElemStructPF*)(WOR_gpst_CurrentWorld->objectTablePF.nextElemPtr); ++i)
    {
        if (i->objectPtr == v2)
        {
            AI_PutIntToStack(1);
            return a1 + 1;
        }
    }
    AI_PutIntToStack(0);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_AddMaterial_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3)
{
    tGEOMAT* v3;

    if ((a1->flags & 0x1000) != 0 && (a2->flags & 0x4000) != 0)
    {
        if (a3 == -1)
        {
            a1->basic->extraMaterial = 0;
        }
        else
        {
            v3 = (tGEOMAT*)(((tGameObjectVisualObject*)(a2->basic->actorObjectOrVisualObject))->GROMAT);
            if (v3)
            {
                if (v3->intf->id == 4)
                    v3 = v3->multiMaterialPtrIfType4ThenDiffuseColor[a3 % v3->multiMaterialNumIfType4ThenAmbiantColor];
            }
            a1->basic->extraMaterial = v3;
        }
    }
}

tAINode* AI_EvalFunc_OBJ_AddMaterial(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJ_AddMaterial_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_RegisterKill_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int* a3)
{
    WOR_AddDelObj(WOR_gpst_CurrentWorld, a2, a3);
}

tAINode* AI_EvalFunc_OBJ_RegisterKill(tAINode* a1)
{
    tGameObjectMain* v2;
    char dest[52];
    int* a3;
    tVarStackElem a2;
    tGameObjectMain* v6;

    AI_GetVarFromStack(dest, &a2);
    a3 = (int*)a2.ptr;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v6 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_RegisterKill_C(v2, v6, (int*)a2.ptr);
    return a1 + 1;
}

float AI_EvalFunc_OBJIsInCone_C(struct sGameObjectMain* a1, Vec3* a2, float a3, float a4, int a5)
{
    double result;
    float v6;
    Vec3 v7;
    Vec3 dest;
    float v9;

    MATH_SubstractVec(&dest, a2, &a1->matrices->Pos);
    if (a5)
        dest.z = 0.0;
    v6 = MATH_VecLen(&dest);
    if (v6 > (double)a4)
        return 0.0;
    MATH_NormalizeVector(&dest, &dest);
    OBJ_SightGet(a1, &v7);
    if (a5)
    {
        v7.z = 0.0;
        MATH_NormalizeVector(&v7, &v7);
    }
    v9 = v7.z * dest.z + v7.y * dest.y + dest.x * v7.x;
    if (v9 <= (double)a3)
        result = 0.0;
    else
        result = v6;
    return (float)result;
}

tAINode* AI_EvalFunc_OBJIsInCone(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;
    float a4;
    Vec3 a1a;
    int a5;
    float a3;
    tGameObjectMain* v8;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v3;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetFloatFromStack();
    a3 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1a);
    v2 = AI_EvalFunc_OBJIsInCone_C(v8, &a1a, a3, a4, a5);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_OBJIKFatherPosSet(tGameObjectMain* a2, tGameObjectMain* a3, tGameObjectMain* a4, float a5, float a6)
{

    Mtx4x4* v6;
    Mtx4x4* v7;
    Mtx4x4 v8;
    Mtx4x4 v9;
    tGameObjectMain* v10;
    Mtx4x4* v11;
    Mtx4x4* v12;
    Mtx4x4 v13;
    Mtx4x4 v14;
    Mtx4x4 v15;

    OBJ_ComputeGlobalWithLocal(a3, &v15, 1);
    OBJ_ComputeGlobalWithLocal(a4, &v14, 1);
    OBJ_ComputeGlobalWithLocal(a2, &v13, 1);
    if (a5 == 0.0)
    {
        if ((a4->flags & 0x10000) != 0)
            v12 = a4->matrices + 1;
        else
            v12 = a4->matrices;
        a5 = MATH_VecLen(&v12->Pos);
    }
    if (a6 == 0.0)
    {
        if ((a2->flags & 0x10000) != 0)
            v11 = a2->matrices + 1;
        else
            v11 = a2->matrices;
        a6 = MATH_VecLen(&v11->Pos);
    }
    GAO_ModifierLegLink_Compute(&v15, &v14, &v13, a5, a6);
    if (a3 && a3->basic && a3->basic->mtx)
        v10 = a3->basic->mtx->refGao2;
    else
        v10 = 0;
    if (v10)
    {
        OBJ_ComputeGlobalWithLocal(v10, &v9, 1);
        MATH_InvertMatrix(&v8, &v9);
        MATH_MulMatrixMatrix(&a3->basic->mtx->mtx, &v15, &v8);
        MATH_Orthonormalize(&a3->basic->mtx->mtx);
    }
    else
    {
        v7 = a3->matrices;
        if (v7 != &v15)
            memcpy(v7, &v15, sizeof(Mtx4x4));
    }
    if (a4 && a4->basic && a4->basic->mtx)
        v10 = a4->basic->mtx->refGao2;
    else
        v10 = 0;
    if (v10)
    {
        MATH_InvertMatrix(&v8, &v15);
        MATH_MulMatrixMatrix(&a4->basic->mtx->mtx, &v14, &v8);
        MATH_Orthonormalize(&a4->basic->mtx->mtx);
    }
    else
    {
        v6 = a4->matrices;
        if (v6 != &v14)
            memcpy(v6, &v14, sizeof(Mtx4x4));
    }
    if (a2 && a2->basic && a2->basic->mtx)
        v10 = a2->basic->mtx->refGao2;
    else
        v10 = 0;
    if (v10)
    {
        OBJ_ComputeGlobalWithLocal(v10, &v9, 1);
        MATH_InvertMatrix(&v8, &v9);
        MATH_MulMatrixMatrix(&a2->basic->mtx->mtx, &v13, &v8);
        MATH_Orthonormalize(&a2->basic->mtx->mtx);
    }
}

void AI_EvalFunc_OBJIKFatherPosSetExt_C(tGameObjectMain* a1, tGameObjectMain* a2, tGameObjectMain* a3, float a4, float a5)
{
    AI_OBJIKFatherPosSet(a1, a3, a2, a5, a4);
}

tAINode* AI_EvalFunc_OBJIKFatherPosSetExt(tAINode* a1)
{
    tGameObjectMain* v2;
    tGameObjectMain* a3;
    float a5;
    float a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetFloatFromStack();
    a4 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_OBJIKFatherPosSetExt_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3, a4, a5);
    return a1 + 1;
}

void AI_EvalFunc_OBJIKFatherPosSet_C(tGameObjectMain* a1)
{
    tGameObjectMain* a3;
    tGameObjectMain* a4;

    if (a1 && a1->basic && a1->basic->mtx)
        a4 = a1->basic->mtx->refGao2;
    else
        a4 = 0;
    if (a4)
    {
        if (a4->basic && a4->basic->mtx)
            a3 = a4->basic->mtx->refGao2;
        else
            a3 = 0;
        if (a3)
            AI_OBJIKFatherPosSet(a1, a3, a4, 0.0, 0.0);
    }
}

tAINode* AI_EvalFunc_OBJIKFatherPosSet(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJIKFatherPosSet_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_EyeFollowParamSet_C(float a1, float a2, float a3, float a4, float a5, float a6, float a7)
{
    OBJ_gf_EYEFOLLOW_HeadZMax = a1;
    OBJ_gf_EYEFOLLOW_HeadZMin = a2;
    OBJ_gf_EYEFOLLOW_HeadXMax = a3;
    OBJ_gf_EYEFOLLOW_HeadXMin = a4;
    OBJ_gf_EYEFOLLOW_TorsoZMax = a5;
    OBJ_gf_EYEFOLLOW_TorsoZMin = a6;
    OBJ_gf_EYEFOLLOW_HeadXOffset = a7;
}

tAINode* AI_EvalFunc_OBJ_EyeFollowParamSet(tAINode* a1)
{
    float a5;
    float a2;
    float a7;
    float a1a;
    float a3;
    float a4;
    float a6;

    a7 = AI_GetFloatFromStack();
    a6 = AI_GetFloatFromStack();
    a5 = AI_GetFloatFromStack();
    a4 = AI_GetFloatFromStack();
    a3 = AI_GetFloatFromStack();
    a2 = AI_GetFloatFromStack();
    a1a = AI_GetFloatFromStack();
    AI_EvalFunc_OBJ_EyeFollowParamSet_C(a1a, a2, a3, a4, a5, a6, a7);
    return a1 + 1;
}

void AI_EvalFunc_OBJEyeFollow_C(struct sGameObjectMain* a1, Vec3* a2, float a3, int a4)
{
    OBJ_EyeFollow(
        a1,
        a2,
        a3,
        a4 & 0x3FFFFFF,
        a4 & 0x80000000,
        a4 & 0x40000000,
        a4 & 0x20000000,
        a4 & 0x10000000,
        a4 & 0x8000000,
        a4 & 0x4000000);
}

tAINode* AI_EvalFunc_OBJEyeFollow(tAINode* a1)
{
    tGameObjectMain* v2;
    float a3;
    Vec3 a1a;
    tGameObjectMain* v5;
    int a4;

    a4 = AI_GetIntFromStack();
    a3 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1a);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_OBJEyeFollow_C(v2, &a1a, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJFreezeModifier(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    OBJ_FreezeModifier_C(v2, a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJModifierType(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int mdfId;

    mdfId = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (!v3->extended || !v3->extended->mdf)
        AI_PutIntToStack(0);
    v1 = OBJ_ModifierType_C(v3, mdfId);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_ColMapOptimize_C(struct sGameObjectMain* a1)
{
    int result;

    if ((a1->flags & 0x100) != 0
        && a1->extended
        && a1->extended->coll->colMap
        && a1->extended->coll->colMap->collisionObjectPtr)
    {
        result = (*a1->extended->coll->colMap->collisionObjectPtr)->flags & 2;
    }
    else
    {
        result = 0;
    }
    return result;
}

tAINode* AI_EvalFunc_OBJ_ColMapOptimize(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_ColMapOptimize_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_OBJEngineCall_C(struct sGameObjectMain* a1)
{
    int a2;
    tWorld* v2;

    v2 = WOR_gpst_CurrentWorld;
    if (WOR_gpst_CurrentWorld)
    {
        a2 = WOR_gpst_CurrentWorld->recordCount;
        DYN_OneCall(a1);
        COL_OneCall(a1, a2);
        REC_MainCall(v2, a2);
    }
}

tAINode* AI_EvalFunc_OBJEngineCall(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJEngineCall_C(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_OBJHasPhotoValue_C(struct sGameObjectMain* a1, int a2, int* a7)
{
    int v4;
    tGdi_DisplayData* a3;

    if (!WOR_gpst_CurrentWorld)
        return 0;
    a3 = WOR_gpst_CurrentWorld->viewPtr->displayData;
    if (a2 == -1)
        v4 = IMG_ObjectAnalyser(a1, WOR_gpst_CurrentWorld, a3, -1, 1, 0, 0, a7);
    else
        v4 = IMG_ObjectAnalyser(a1, WOR_gpst_CurrentWorld, a3, a2 << 24, 1, 0, 0, a7);
    return v4 != 0;
}

tAINode* AI_EvalFunc_OBJHasPhotoValue(tAINode* a1)
{
    tGameObjectMain* v2;
    tVarStackElem a2;
    char dest[52];
    int v5;
    tGameObjectMain* v6;

    AI_GetVarFromStack(dest, &a2);
    v5 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v2;
    if (AI_EvalFunc_OBJHasPhotoValue_C(v2, v5, (int*)a2.ptr))
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return a1 + 1;
}

BOOL AI_EvalFunc_OBJCentred_C(tGameObjectMain* a1, int* a7)
{
    BOOL result;

    if (WOR_gpst_CurrentWorld)
        result = IMG_ObjectAnalyser(a1, WOR_gpst_CurrentWorld, WOR_gpst_CurrentWorld->viewPtr->displayData, -1, 1, 0, 1, a7) != 0;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_OBJCentred(tAINode* a1)
{
    tGameObjectMain* v2;
    tVarStackElem a2;
    char dest[52];
    tGameObjectMain* a1a;

    AI_GetVarFromStack(dest, &a2);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v2;
    if (AI_EvalFunc_OBJCentred_C(v2, (int*)a2.ptr))
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectMain* v2;
    tGameObjectMain* v3;
    tModifier* v4;

    v4 = GAO_ModifierPhoto_Get(a1, 0);
    if (v4)
    {
        v3 = GAO_ModifierPhoto_SnapGOGet(a1, (tPhotoMdfStruct*)v4->mdfPrivateDataPtr, 1);
        MATH_TransformVertex(a2, v3->matrices, &(((tPhotoMdfStruct*)(v4->mdfPrivateDataPtr))->infoOffset));
    }
    else if ((a1->flags & 2) != 0 && (v2 = ANI_pst_GetObjectByAICanal(a1, 0)) != 0)
    {
        OBJ_BV_ComputeCenter(v2, a2);
    }
    else
    {
        OBJ_BV_ComputeCenter(a1, a2);
    }
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoPivotGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(v2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

int AI_EvalFunc_OBJ_InfoPhotoMissionGet_C(struct sGameObjectMain* a1)
{
    int result;
    tModifier* v2;

    v2 = GAO_ModifierPhoto_Get(a1, 0);
    if (v2)
        result = ((tPhotoMdfStruct*)(v2->mdfPrivateDataPtr))->mission;
    else
        result = -1;
    return result;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoMissionGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_InfoPhotoMissionGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_InfoPhotoInfoGet_C(struct sGameObjectMain* a1)
{
    int result;
    tModifier* v2;

    v2 = GAO_ModifierPhoto_Get(a1, 0);
    if (v2)
        result = ((tPhotoMdfStruct*)(v2->mdfPrivateDataPtr))->info;
    else
        result = -1;
    return result;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoInfoGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_OBJ_InfoPhotoInfoGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_InfoPhotoLODGet_C(struct sGameObjectMain* a1, int a2)
{
    tModifier* v3;

    v3 = GAO_ModifierPhoto_Get(a1, 0);
    if (!v3)
        return -1.0f;
    switch (a2)
    {
    case 0:
        return ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->lod0_frameCullingSphereRet2;
    case 1:
        return ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->lod1;
    case 2:
        return ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->lod2;
    }
    return -1.0f;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoLODGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_OBJ_InfoPhotoLODGet_C(v3, a2);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_OBJ_InfoPhotoFrameGet_C(struct sGameObjectMain* a1, int a2)
{
    tModifier* v3;

    v3 = GAO_ModifierPhoto_Get(a1, 0);
    if (!v3)
        return -1.0f;
    switch (a2)
    {
    case 0:
        return ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->frame0_frameCullingSphereRet1;
    case 1:
        return ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->frame1;
    case 2:
        return ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->frame2;
    }
    return -1.0f;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoFrameGet(tAINode* a1)
{
    float v2; 
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_OBJ_InfoPhotoFrameGet_C(v3, a2);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tModifier* v2;

    v2 = GAO_ModifierPhoto_Get(a1, 0);
    if (v2)
        ((tPhotoMdfStruct*)(v2->mdfPrivateDataPtr))->sphereOffset = *a2;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet_C(a1, &a2);
    return arg0 + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tModifier* v2;

    v2 = GAO_ModifierPhoto_Get(a1, 0);
    if (v2)
        ((tPhotoMdfStruct*)(v2->mdfPrivateDataPtr))->infoOffset = *a2;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet_C(a1, &a2);
    return arg0 + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoParamSet_C(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5, float a6, float a7, float a8, float a9)
{
    tModifier** i;
    tModifier* v10;

    v10 = GAO_ModifierPhoto_Get(a1, 0);
    if (!v10)
    {
        OBJ_GameObject_CreateExtendedIfNot(a1);
        for (i = &a1->extended->mdf; *i; i = &(*i)->nextElem)
            ;
        *i = MDF_pst_Modifier_Create(a1, 20, 0);
        v10 = *i;
        v10->modifierFlags |= 0x1000000u;
        v10->gao = a1;
    }
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->radius = a9;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->frame1 = a8;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->frame2 = 1.001f;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->lod1 = a6;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->lod2 = a7;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->aiCanal1 = a4;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->aiCanal2 = a5;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->info = a3;
    ((tPhotoMdfStruct*)(v10->mdfPrivateDataPtr))->mission = a2;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoParamSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a9;
    float a8;
    float a6;
    int a5;
    int a2;
    int a3;
    float a7;
    int a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a9 = AI_GetFloatFromStack();
    a8 = AI_GetFloatFromStack();
    a7 = AI_GetFloatFromStack();
    a6 = AI_GetFloatFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_InfoPhotoParamSet_C(v2, a2, a3, a4, a5, a6, a7, a8, a9);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    tModifier* v3;

    v3 = GAO_ModifierPhoto_Get(a1, 0);
    if (v3)
    {
        ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->flags |= a2;
        ((tPhotoMdfStruct*)(v3->mdfPrivateDataPtr))->flags &= ~a3;
    }
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoFlagsSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(v2, a2, a3);
    return a1 + 1;
}