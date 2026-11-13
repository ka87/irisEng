#include "../math.h"
#include "../memory.h"
#include "../loading.h"
#include "leglinkmodifier.h"

void GAO_ModifierLegLink_Compute(Mtx4x4* hip, Mtx4x4* knee, Mtx4x4* foot,
                                         float upperLen, float lowerLen)
{
    Vec3 dir;
    Vec3 side;
    Vec3 up;
    Vec3 hipToKnee;
    Vec3 kneeToFoot;
    Vec3 tmp;
    double alongDist;
    float along;
    float dist;
    float height;

    dist = MATH_DistanceOfTwoPoints(&hip->Pos, &foot->Pos);
    alongDist = (upperLen * upperLen - lowerLen * lowerLen + dist * dist) / (dist + dist);
    along = (float)alongDist;
    height = MATH_GetSqrt((float)(upperLen * upperLen - alongDist * along));

    MATH_SubstractVec(&dir, &foot->Pos, &hip->Pos);
    MATH_NormalizeEqualVector(&dir);
    MATH_VecMultScalarWithoutBuffer(&dir, along);

    MATH_CrossProduct(&side, &hip->v3, &dir);
    MATH_CrossProduct(&up, &dir, &side);
    MATH_NormalizeEqualVector(&up);
    MATH_VecMultScalarWithoutBuffer(&up, height);

    MATH_VecAddVec(&knee->Pos, &dir, &hip->Pos);
    MATH_VecAddVec(&knee->Pos, &up, &knee->Pos);

    MATH_SubstractVec(&hipToKnee, &knee->Pos, &hip->Pos);
    MATH_CrossProduct(&tmp, &hip->v2, &hipToKnee);
    MATH_NormalizeVector(&hip->v1, &tmp);
    MATH_NormalizeVector(&hip->v3, &hipToKnee);
    MATH_CrossProduct(&hip->v2, &hip->v3, &hip->v1);

    MATH_SubstractVec(&kneeToFoot, &foot->Pos, &knee->Pos);
    MATH_NormalizeVector(&knee->v3, &kneeToFoot);
    MATH_CrossProduct(&tmp, &knee->v2, &kneeToFoot);
    MATH_NormalizeVector(&knee->v1, &tmp);
    MATH_CrossProduct(&knee->v2, &knee->v3, &knee->v1);
}

void GAO_ModifierLegLink_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tLegLinkMdfStruct* leg;

    leg = (tLegLinkMdfStruct*)MEM_p_Alloc(sizeof(tLegLinkMdfStruct));
    mdf->modifierFlags |= 0x1000000u;
    mdf->mdfPrivateDataPtr = leg;
    if (initData)
    {
        memcpy(leg, initData, sizeof(tLegLinkMdfStruct));
    }
    else
    {
        leg->gao0 = 0;
        leg->gao1 = 0;
        leg->i3 = 5.0f;
        leg->i4 = 5.0f;
        leg->gao2 = 0;
        leg->flags = 0;
    }
}

void GAO_ModifierLegLink_Destroy(struct sModifier* mdf)
{
    MEM_Free(mdf->mdfPrivateDataPtr);
}

void GAO_ModifierLegLink_DoIt(struct sLegLinkMdfStruct* leg, struct sGameObjectMain* gao)
{
    tGameObjectMain* axisGao;
    tGameObjectMain* hipGao;
    Mtx4x4* hipMtx;
    Mtx4x4* kneeMtx;
    Vec3* axis;
    Vec3* hipPos;
    Vec3* kneePos;
    Vec3* footPos;
    Vec3 dir;
    Vec3 side;
    Vec3 up;
    Vec3 hipToKnee;
    Vec3 kneeToFoot;
    Vec3 tmp;
    double alongDist;
    float along;
    float dist;
    float height;
    int flags;

    if (!leg || !leg->gao0 || !leg->gao1 || !gao)
        return;

    OBJ_ComputeGlobalWhenHie(leg->gao0);
    OBJ_ComputeGlobalWhenHie(gao);
    OBJ_ComputeGlobalWhenHie(leg->gao1);

    hipPos = &leg->gao0->matrices->Pos;
    kneePos = &gao->matrices->Pos;
    footPos = &leg->gao1->matrices->Pos;

    axisGao = leg->gao2 ? leg->gao2 : leg->gao0;
    flags = leg->flags;
    if ((flags & 1) != 0)
        axis = &OBJ_pst_GetAbsoluteMatrix(axisGao)->v1;
    else if ((flags & 2) != 0)
        axis = &OBJ_pst_GetAbsoluteMatrix(axisGao)->v2;
    else
        axis = &OBJ_pst_GetAbsoluteMatrix(axisGao)->v3;

    dist = MATH_DistanceOfTwoPoints(hipPos, footPos);
    alongDist = (leg->i3 * leg->i3 - leg->i4 * leg->i4 + dist * dist) / (dist + dist);
    along = (float)alongDist;
    height = MATH_GetSqrt((float)(leg->i3 * leg->i3 - alongDist * along));

    MATH_SubstractVec(&dir, footPos, hipPos);
    MATH_NormalizeEqualVector(&dir);
    MATH_VecMultScalarWithoutBuffer(&dir, along);

    if ((leg->flags & 8) == 0)
        MATH_CrossProduct(&side, axis, &dir);
    else
        MATH_CrossProduct(&side, &dir, axis);
    MATH_CrossProduct(&up, &dir, &side);
    MATH_NormalizeEqualVector(&up);
    MATH_VecMultScalarWithoutBuffer(&up, height);

    MATH_VecAddVec(kneePos, &dir, hipPos);
    MATH_VecAddVec(kneePos, &up, kneePos);

    hipGao = leg->gao0;
    hipMtx = OBJ_pst_GetAbsoluteMatrix(hipGao);
    MATH_SubstractVec(&hipToKnee, kneePos, hipPos);
    MATH_CrossProduct(&tmp, &hipMtx->v2, &hipToKnee);
    MATH_NormalizeVector(&hipMtx->v1, &tmp);
    MATH_NormalizeVector(&hipMtx->v3, &hipToKnee);
    MATH_CrossProduct(&hipMtx->v2, &hipMtx->v3, &hipMtx->v1);
    OBJ_ComputeLocalWhenHie(hipGao);

    kneeMtx = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_SubstractVec(&kneeToFoot, footPos, kneePos);
    MATH_NormalizeVector(&kneeMtx->v3, &kneeToFoot);
    MATH_CrossProduct(&tmp, &kneeMtx->v2, &kneeToFoot);
    MATH_NormalizeVector(&kneeMtx->v1, &tmp);
    MATH_CrossProduct(&kneeMtx->v2, &kneeMtx->v3, &kneeMtx->v1);
    OBJ_ComputeLocalWhenHie(gao);
    OBJ_ComputeLocalWhenHie(leg->gao1);
}

void GAO_ModifierLegLink_UnApply(struct sModifier* mdf, void* unk1)
{
}

void GAO_ModifierLegLink_Apply(struct sModifier* mdf, void* unk1)
{
    tLegLinkMdfStruct* leg;
    tGroup* grp;
    tGameObjectMain* savedGao0;
    tGameObjectMain* savedGao1;
    tGameObjectMain* savedGao2;
    tGameObjectMain* hipGao;
    tGameObjectMain* footGao;
    tGameObjectMain* target;
    Vec3* targetPos;
    Vec3* bonePos;
    Vec3 delta;
    int flags;

    leg = (tLegLinkMdfStruct*)mdf->mdfPrivateDataPtr;
    savedGao0 = leg->gao0;
    savedGao1 = leg->gao1;
    savedGao2 = leg->gao2;

    grp = MDF_Modifier_GetCurGroup(mdf);
    if (grp)
    {
        leg->gao0 = OBJ_pst_GroupGetByRank(grp, (int)savedGao0);
        leg->gao1 = OBJ_pst_GroupGetByRank(grp, (int)savedGao1);
        leg->gao2 = OBJ_pst_GroupGetByRank(grp, (int)savedGao2);
    }

    flags = leg->flags;
    if (((flags & 0x10) == 0 || grp) && (flags & 0x20) != 0 && (flags & 0x40) == 0)
    {
        leg->flags = flags | 0x40;
        hipGao = leg->gao0;
        footGao = leg->gao1;
        target = gpst_DX_stDD->currTargetGao;
        if (hipGao && footGao
            && (hipGao->flags & 0x10000) != 0
            && (target->flags & 0x10000) != 0
            && (footGao->flags & 0x10000) != 0)
        {
            targetPos = OBJ_pst_GetPos(target);
            bonePos = OBJ_pst_GetPos(hipGao);
            MATH_SubstractVec(&delta, bonePos, targetPos);
            leg->i3 = MATH_VecLen(&delta);
            targetPos = OBJ_pst_GetPos(target);
            bonePos = OBJ_pst_GetPos(footGao);
            MATH_SubstractVec(&delta, bonePos, targetPos);
            leg->i4 = MATH_VecLen(&delta);
        }
    }

    if ((leg->flags & 0x10) == 0 || grp)
        GAO_ModifierLegLink_DoIt(leg, gpst_DX_stDD->currTargetGao);

    leg->gao0 = savedGao0;
    leg->gao1 = savedGao1;
    leg->gao2 = savedGao2;
}

static void GAO_l_ModifierLegLink_BindRef(tGameObjectMain** slot)
{
    int key;

    key = (int)*slot;
    if (!key || key == -1)
        *slot = 0;
    else
        LOA_MakeFileRef(key, slot, OBJ_ul_GameObjectCallback, 1);
}

int GAO_ModifierLegLink_Load(struct sModifier* mdf, char* data)
{
    tLegLinkMdfStruct* leg;
    char* start;

    leg = (tLegLinkMdfStruct*)mdf->mdfPrivateDataPtr;
    mdf->modifierFlags |= 0x1000000u;
    start = data;
    LOA_ReadLong(&data, 0, 1);
    leg->gao0 = (tGameObjectMain*)LOA_ReadLong(&data, 0, 1);
    leg->gao1 = (tGameObjectMain*)LOA_ReadLong(&data, 0, 1);
    leg->i3 = LOA_ReadFloat(&data, 0, 1);
    leg->i4 = LOA_ReadFloat(&data, 0, 1);
    leg->gao2 = (tGameObjectMain*)LOA_ReadLong(&data, 0, 1);
    leg->flags = LOA_ReadLong(&data, 0, 1) & ~0x40u;
    if ((leg->flags & 0x10) == 0)
    {
        GAO_l_ModifierLegLink_BindRef(&leg->gao0);
        GAO_l_ModifierLegLink_BindRef(&leg->gao1);
        GAO_l_ModifierLegLink_BindRef(&leg->gao2);
    }
    return data - start;
}

void GAO_ModifierLegLink_Reinit(struct sModifier* mdf)
{
    tLegLinkMdfStruct* leg;

    leg = (tLegLinkMdfStruct*)mdf->mdfPrivateDataPtr;
    leg->flags &= ~0x40u;
}
