#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "../ai/ai_mathfunctions.h"
#include "snakemodifier.h"

void GAO_ModifierSNAKE_Apply(struct sModifier* mdf, void* unk1)
{
    tSnakeMdfStruct* snake;
    tGameObjectMain* head;
    tGameObjectMain* prev;
    tGameObjectMain* cur;
    Vec3 axis;
    Vec3 target;
    Vec3 blended;
    Vec3 offset;
    Vec3 axisDir;
    Vec3 scale;
    double drop;
    double weight;
    float dt;
    float measured;
    float len;
    float sag;
    int i;

    dt = TIM_gf_dt;
    if (TIM_gf_dt == 0.0)
        dt = 0.02f;

    snake = (tSnakeMdfStruct*)mdf->mdfPrivateDataPtr;
    head = snake->gao;
    if (!head)
        return;

    OBJ_ComputeGlobalWithLocal(head, head->matrices, 1);
    sag = 0.0;

    for (i = 1; i < snake->num; i++)
    {
        prev = snake->gaos[i - 1];
        cur = snake->gaos[i];
        if (!prev || !cur)
            break;

        if ((mdf->modifierFlags & 1) != 0)
        {
            axis = prev->matrices->v3;
            target = cur->matrices->v3;
        }
        else
        {
            MATH_VecInvert(&axis, &prev->matrices->v2);
            MATH_VecInvert(&target, &cur->matrices->v2);
        }

        AI_EvalFunc_MATHVecBlendRotate_C(&target, &axis, dt * snake->inertia, &blended);
        axis = blended;

        offset.x = cur->matrices->Pos.x - prev->matrices->Pos.x;
        offset.y = cur->matrices->Pos.y - prev->matrices->Pos.y;
        offset.z = cur->matrices->Pos.z - prev->matrices->Pos.z;
        measured = MATH_VecLen(&offset);

        if (i < 2)
        {
            if ((mdf->modifierFlags & 1) != 0)
                target = prev->matrices->v2;
            else
                target = prev->matrices->v3;
        }
        else
        {
            target.x = prev->matrices->Pos.x - snake->gaos[i - 2]->matrices->Pos.x;
            target.y = prev->matrices->Pos.y - snake->gaos[i - 2]->matrices->Pos.y;
            target.z = prev->matrices->Pos.z - snake->gaos[i - 2]->matrices->Pos.z;
        }

        MATH_SetNormVector(&blended, &target, snake->distances[i - 1]);
        target = blended;

        AI_EvalFunc_MATHVecBlendRotate_C(&offset, &target, (sag + snake->inertia) * dt, &blended);
        offset = blended;

        drop = sag + snake->f3;
        sag = (float)drop;
        if (drop + snake->inertia < 0.0)
            sag = -snake->inertia;

        weight = dt * snake->f2;
        if (weight < 0.0)
            weight = 0.0;
        else if (weight > 1.0)
            weight = 1.0;

        len = (float)((1.0 - weight) * snake->distances[i - 1] + weight * measured);
        MATH_SetNormVector(&blended, &offset, len);
        offset = blended;

        MATH_NormalizeEqualVector(&blended);
        MATH_NormalizeVector(&axisDir, &axis);

        if ((mdf->modifierFlags & 1) != 0)
        {
            MATH_VecInvert(&blended, &blended);
            OBJ_SightGeneralSet(cur, &blended, &axisDir);
        }
        else
        {
            OBJ_BankingGeneralSet(cur, &axisDir, &blended);
        }

        offset.x = offset.x + prev->matrices->Pos.x;
        offset.y = offset.y + prev->matrices->Pos.y;
        offset.z = offset.z + prev->matrices->Pos.z;
        cur->matrices->Pos = offset;

        MATH_GetScale(&scale, &cur->basic->mtx->mtx);
        OBJMY_ComputeLocalWhenHie(cur);
        MATH_SetScale(&cur->basic->mtx->mtx, &scale);
    }
}

void GAO_ModifierSNAKE_UpdatePointers(tModifier* mdf)
{
    tSnakeMdfStruct* snake;
    tGroup* grp;
    tGameObjectMain* gao;
    int i;

    snake = (tSnakeMdfStruct*)mdf->mdfPrivateDataPtr;
    snake->gao = NULL;
    if (snake->num < 2)
        return;

    grp = MDF_Modifier_GetCurGroup(mdf);
    if (!grp)
        return;

    for (i = 0; i < snake->num; i++)
    {
        gao = OBJ_pst_GroupGetByRank(grp, snake->ranks[i]);
        snake->gaos[i] = gao;
        if (!gao)
            return;
        OBJ_RestoreInitialPos(gao);
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    snake->gao = snake->gaos[0];
    for (i = 1; i < snake->num; i++)
    {
        snake->distances[i - 1] = MATH_DistanceOfTwoPoints(
            &snake->gaos[i - 1]->matrices->Pos,
            &snake->gaos[i]->matrices->Pos);
    }
}

void GAO_ModifierSNAKE_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tSnakeMdfStruct* snake;

    snake = (tSnakeMdfStruct*)MEM_p_Alloc(sizeof(tSnakeMdfStruct));
    mdf->mdfPrivateDataPtr = snake;
    if (initData)
    {
        mdf->modifierFlags = 0x1000000;
        if ((MDF_gpst_GlobalCreate->modifierFlags & 1) != 0)
            mdf->modifierFlags = 0x1000001;
        memcpy(snake, initData, sizeof(tSnakeMdfStruct));
    }
    else
    {
        memset(snake, 0, sizeof(tSnakeMdfStruct));
        mdf->modifierFlags = 0x1000000;
        GAO_ModifierSNAKE_UpdatePointers(mdf);
    }
}

void GAO_ModifierSNAKE_Destroy(struct sModifier* mdf)
{
    if (mdf->mdfPrivateDataPtr)
        MEM_Free(mdf->mdfPrivateDataPtr);
}

int GAO_ModifierSNAKE_Load(struct sModifier* mdf, char* data)
{
    char* start;
    tSnakeMdfStruct* snake;
    int i;

    start = data;
    snake = (tSnakeMdfStruct*)mdf->mdfPrivateDataPtr;
    LOA_ReadLong(&data, 0, 2);
    snake->num = LOA_ReadLong(&data, 0, 1);
    snake->inertia = LOA_ReadFloat(&data, 0, 1);
    snake->f2 = LOA_ReadFloat(&data, 0, 1);
    snake->f3 = LOA_ReadFloat(&data, 0, 1);
    for (i = 0; i < 20; i++)
        snake->ranks[i] = LOA_ReadLong(&data, 0, 1);
    return data - start;
}

void GAO_ModifierSNAKE_Reinit(struct sModifier* mdf)
{
    GAO_ModifierSNAKE_UpdatePointers(mdf);
}

void GAO_ModifierSNAKE_Unapply(struct sModifier* mdf, void* unk1)
{
}
