#include "../time.h"
#include "../memory.h"
#include "../loading.h"
#include "lazymodifier.h"

static void GAO_l_ModifierLazy_BlendVec(Vec3* dest, Vec3* target, float blend)
{
    Vec3 delta;

    MATH_SubstractVec(&delta, target, dest);
    MATH_AddScaleVector(dest, dest, &delta, blend);
}

static void GAO_l_ModifierLazy_ClampToBox(tLazyMdfStruct* lazy, Mtx4x4* target)
{
    Vec3 offset;
    Vec3 inBox;
    float alongU;
    float alongV;
    float alongW;

    MATH_SubstractVec(&offset, &lazy->mtx2.Pos, &target->Pos);

    alongU = MATH_DotProduct(&target->v1, &offset);
    alongV = MATH_DotProduct(&target->v2, &offset);
    alongW = MATH_DotProduct(&target->v3, &offset);
    if (alongU < lazy->ff1)
        alongU = lazy->ff1;
    if (alongV < lazy->ff2)
        alongV = lazy->ff2;
    if (alongW < lazy->ff3)
        alongW = lazy->ff3;
    if (alongU > lazy->upperMax)
        alongU = lazy->upperMax;
    if (alongV > lazy->ff5)
        alongV = lazy->ff5;
    if (alongW > lazy->ff6)
        alongW = lazy->ff6;

    MATH_VecMultScalar(&inBox, &target->v1, alongU);
    MATH_AddScaleVector(&inBox, &inBox, &target->v2, alongV);
    MATH_AddScaleVector(&inBox, &inBox, &target->v3, alongW);
    MATH_VecAddVec(&lazy->mtx2.Pos, &inBox, &target->Pos);
}

static void GAO_l_ModifierLazy_KeepBoneLength(tLazyMdfStruct* lazy, Mtx4x4* mtx, Vec3* anchor)
{
    Vec3 boneVec;
    Vec3 lazyVec;
    Vec3 axis;
    Quat4 rotation;
    Mtx4x4 rotMtx;
    Mtx4x4 boneMtx;
    float boneLen;
    float lazyLen;
    float invLazyLen;

    MATH_SubstractVec(&boneVec, &mtx->Pos, anchor);
    MATH_SubstractVec(&lazyVec, &lazy->mtx2.Pos, anchor);
    boneLen = MATH_VecLen(&boneVec);
    lazyLen = MATH_VecLen(&lazyVec);
    if (lazyLen == 0.0)
        return;

    invLazyLen = (float)(1.0 / lazyLen);
    MATH_AddScaleVector(&lazy->mtx2.Pos, anchor, &lazyVec, invLazyLen * boneLen);
    if (boneLen == 0.0)
        return;

    MATH_VecMultScalarWithoutBuffer(&boneVec, (float)(1.0 / boneLen));
    MATH_VecMultScalarWithoutBuffer(&lazyVec, invLazyLen);
    MATH_VecAddVecWithoutBuffer(&lazyVec, &boneVec);
    MATH_NormalizeVector(&lazyVec, &lazyVec);

    MATH_CrossProduct(&axis, &boneVec, &lazyVec);
    rotation.q1 = axis.x;
    rotation.q2 = axis.y;
    rotation.q3 = axis.z;
    rotation.q4 = MATH_DotProduct(&boneVec, &lazyVec);
    MATH_ConvertQuaternionToMatrix(&rotMtx, &rotation);
    MATH_CopyMatrix(&boneMtx, mtx);
    MATH_Mul33MatrixMatrix(mtx, &boneMtx, &rotMtx, 1);
}

void GAO_ModifierLazy_Compute_SemiMecanical_Model(tLazyMdfStruct* lazy, Mtx4x4* mtx,
                                                  Mtx4x4* parentMtx)
{
    Mtx4x4 target;
    Vec3 prevPos;
    Vec3 pull;

    if (!parentMtx)
        return;

    prevPos = lazy->mtx2.Pos;
    MATH_MulMatrixMatrix(&target, &lazy->mtx1, parentMtx);

    MATH_SubstractVec(&pull, &target.Pos, &lazy->mtx2.Pos);
    MATH_AddScaleVector(&lazy->vv, &lazy->vv, &pull, lazy->f2);
    lazy->vv.z = lazy->fff1 + lazy->vv.z;
    MATH_VecMultScalarWithoutBuffer(&lazy->vv, lazy->fff2);
    MATH_AddScaleVector(&lazy->mtx2.Pos, &lazy->mtx2.Pos, &lazy->vv, TIM_gf_dt);

    if ((lazy->upperFlag & 0x400) != 0)
        GAO_l_ModifierLazy_ClampToBox(lazy, &target);

    GAO_l_ModifierLazy_KeepBoneLength(lazy, mtx, &parentMtx->Pos);

    MATH_SubstractVec(&lazy->vv, &lazy->mtx2.Pos, &prevPos);
    MATH_VecMultScalarWithoutBuffer(&lazy->vv, (float)(1.0 / TIM_gf_dt));
    mtx->Pos = lazy->mtx2.Pos;
}

void GAO_ModifierLazy_Compute(tLazyMdfStruct* lazy, Mtx4x4* mtx, Mtx4x4* parentMtx)
{
    Mtx4x4 target;
    Vec3 scale;
    Vec3 targetScale;
    double decay;
    double elapsed;
    float perStep;
    float remaining;
    float blend;
    int flags;

    flags = lazy->upperFlag;
    if ((flags & 0x1000) != 0)
    {
        GAO_ModifierLazy_Compute_SemiMecanical_Model(lazy, mtx, parentMtx);
        return;
    }

    remaining = 1.0f - lazy->f2;
    if (remaining < 0.0)
        remaining = 0.0;
    perStep = (float)(1.0 - remaining * remaining);

    if (!parentMtx)
        flags &= ~8u;
    if ((flags & 8) != 0)
        flags &= ~2u;

    decay = 1.0;
    elapsed = 0.0;
    if (TIM_gf_dt > 0.0)
    {
        do
        {
            decay = decay * perStep;
            elapsed = elapsed + 0.0033333334;
        } while (elapsed < TIM_gf_dt);
    }
    blend = (float)(1.0 - decay);

    if ((flags & 9) != 0)
    {
        GAO_l_ModifierLazy_BlendVec(&lazy->mtx2.Pos, &mtx->Pos, blend);
        if ((flags & 8) != 0)
        {
            lazy->mtx2.Pos.z = blend * lazy->fff1 + lazy->mtx2.Pos.z;
            if ((flags & 0x400) != 0)
            {
                MATH_MulMatrixMatrix(&target, &lazy->mtx1, parentMtx);
                GAO_l_ModifierLazy_ClampToBox(lazy, &target);
            }
            GAO_l_ModifierLazy_KeepBoneLength(lazy, mtx, &parentMtx->Pos);
        }
        mtx->Pos = lazy->mtx2.Pos;
    }

    if ((flags & 2) != 0)
    {
        GAO_l_ModifierLazy_BlendVec(&lazy->mtx2.v1, &mtx->v1, blend);
        GAO_l_ModifierLazy_BlendVec(&lazy->mtx2.v2, &mtx->v2, blend);
        GAO_l_ModifierLazy_BlendVec(&lazy->mtx2.v3, &mtx->v3, blend);
        MATH_Orthonormalize(&lazy->mtx2);
        mtx->v1 = lazy->mtx2.v1;
        mtx->v2 = lazy->mtx2.v2;
        mtx->v3 = lazy->mtx2.v3;
    }

    if ((flags & 4) != 0 && (mtx->mtxflag & 8) != 0)
    {
        scale.x = 1.0;
        scale.y = 1.0;
        scale.z = 1.0;
        if ((lazy->mtx2.mtxflag & 8) != 0)
        {
            scale.x = lazy->mtx2._m14;
            scale.y = lazy->mtx2._m24;
            scale.z = lazy->mtx2._m34;
        }
        else
        {
            lazy->mtx2.mtxflag |= 8u;
        }

        targetScale.x = mtx->_m14;
        targetScale.y = mtx->_m24;
        targetScale.z = mtx->_m34;
        GAO_l_ModifierLazy_BlendVec(&scale, &targetScale, blend);

        mtx->_m14 = scale.x;
        mtx->_m24 = scale.y;
        mtx->_m34 = scale.z;
        lazy->mtx2._m14 = scale.x;
        lazy->mtx2._m24 = scale.y;
        lazy->mtx2._m34 = scale.z;
    }
}

void GAO_ModifierLazy_Inherit(tLazyMdfStruct* lazy, tGameObjectMain* srcGao,
                              tGameObjectMain* dstGao)
{
    Mtx4x4* src;
    Mtx4x4* dst;

    dst = &dstGao->basic->mtx->mtx;
    if ((srcGao->flags & 0x400000) != 0)
    {
        src = &srcGao->basic->mtx->mtx;
        if ((lazy->upperFlag & 0x10) != 0)
            dst->Pos = src->Pos;
    }
    else
    {
        src = srcGao->matrices;
    }

    if ((lazy->upperFlag & 0x20) != 0)
    {
        dst->v1 = src->v1;
        dst->v2 = src->v2;
        dst->v3 = src->v3;
    }

    if ((lazy->upperFlag & 0x40) != 0)
    {
        if ((src->mtxflag & 8) != 0)
        {
            dst->mtxflag |= 8u;
            dst->_m14 = src->_m14;
            dst->_m24 = src->_m24;
            dst->_m34 = src->_m34;
        }
        else
        {
            dst->mtxflag &= ~8u;
        }
    }
}

void GAO_ModifierLazy_Apply(tModifier* mdf, void* unk1)
{
    tLazyMdfStruct* lazy;
    tGameObjectMain* gao;
    tGaoMatrix* gaoMtx;
    tGameObjectMain* refGao;
    tModifier* other;
    Mtx4x4 refInverse;
    int flags;

    lazy = (tLazyMdfStruct*)mdf->mdfPrivateDataPtr;

    UpperFriction = lazy->fff2;
    UpperGravity = lazy->fff1;
    UpperFactor = lazy->f2;
    UpperFlag = lazy->upperFlag;
    UpperMax = lazy->upperMax;
    LazyFF5_dword_98C72C = lazy->ff5;
    LazyFF6_dword_98C730 = lazy->ff6;
    UpperMin = lazy->ff1;
    LazyFF2_dword_98C738 = lazy->ff2;
    LazyFF3_dword_98C73C = lazy->ff3;

    lazy->upperFriction = lazy->fff2;
    lazy->ffffdf = lazy->fff1;
    lazy->ffff2 = lazy->f2;
    lazy->upperFlagBak = lazy->upperFlag;
    lazy->aaf1 = lazy->upperMax;
    lazy->aaf2 = lazy->ff5;
    lazy->aaf3 = lazy->ff6;
    lazy->af1 = lazy->ff1;
    lazy->af2 = lazy->ff2;
    lazy->af3 = lazy->ff3;

    if ((lazy->upperFlag & 0x10000000) != 0)
        return;

    gao = mdf->gao;
    if ((gao->flags & 0x400000) == 0)
    {
        MATH_CopyMatrix(&lazy->mtx1, gao->matrices);
        GAO_ModifierLazy_Compute(lazy, mdf->gao->matrices, NULL);
    }
    else
    {
        gaoMtx = gao->basic->mtx;
        refGao = gaoMtx->refGao2;
        if (refGao)
        {
            if ((refGao->flags & 0x2000) != 0 && refGao->extended)
            {
                for (other = refGao->extended->mdf; other; other = other->nextElem)
                {
                    if (other->modifierIntf->mdfId != 24)        // lazy
                        continue;
                    flags = other->modifierFlags;
                    if ((flags & 0xA000000) == 0 && (flags & 0x1000000) != 0)
                        GAO_ModifierLazy_Apply(other, NULL);
                }
            }

            if ((lazy->upperFlag & 0x80) != 0)
                lazy->upperFlag = UpperFlag;

            flags = lazy->upperFlag;
            if ((flags & 0x100) != 0)
            {
                lazy->f2 = UpperFactor;
                lazy->fff1 = UpperGravity;
                lazy->fff2 = UpperFriction;
            }
            if ((flags & 0x800) != 0)
            {
                lazy->upperMax = UpperMax;
                lazy->ff5 = LazyFF5_dword_98C72C;
                lazy->ff6 = LazyFF6_dword_98C730;
                lazy->ff1 = UpperMin;
                lazy->ff2 = LazyFF2_dword_98C738;
                lazy->ff3 = LazyFF3_dword_98C73C;
            }
            if ((flags & 0x70) != 0)
                GAO_ModifierLazy_Inherit(lazy, refGao, mdf->gao);

            OBJ_ComputeGlobalWhenHie(mdf->gao);
            MATH_CopyMatrix(&lazy->mtx1, &gaoMtx->mtx);
            GAO_ModifierLazy_Compute(lazy, mdf->gao->matrices, refGao->matrices);
            MATH_InvertMatrix(&refInverse, OBJ_pst_GetAbsoluteMatrix(refGao));
            MATH_MulMatrixMatrix(&gaoMtx->mtx, mdf->gao->matrices, &refInverse);
        }
    }

    lazy->upperFlag |= 0x10000000u;
    lazy->gao = mdf->gao;
    lazy->firstToUnapply = p_FirsToUnapply;
    p_FirsToUnapply = lazy;
}

int Lazy_FirstInit(void)
{
    int wasInitialised;

    wasInitialised = bFirst_286_1;
    if (!bFirst_286_1)
        bFirst_286_1 = 1;
    return wasInitialised;
}

void GAO_ModifierLazy_Reinit(tModifier* mdf)
{
    tGameObjectMain* gao;
    tLazyMdfStruct* lazy;

    gao = mdf->gao;
    lazy = (tLazyMdfStruct*)mdf->mdfPrivateDataPtr;
    if (!gao)
        return;

    if ((gao->flags & 0x400000) == 0)
    {
        MATH_CopyMatrix(&lazy->mtx2, gao->matrices);
        return;
    }

    if (gao->basic->mtx->refGao2)
    {
        OBJ_ComputeGlobalWhenHie(mdf->gao);
        MATH_CopyMatrix(&lazy->mtx2, mdf->gao->matrices);
    }
}

void GAO_ModifierLazy_Create(struct sGameObjectMain* gao, tModifier* mdf, void* initData)
{
    tLazyMdfStruct* lazy;

    Lazy_FirstInit();
    lazy = (tLazyMdfStruct*)MEM_p_AllocAligned(sizeof(tLazyMdfStruct), 16);
    mdf->mdfPrivateDataPtr = lazy;
    mdf->modifierFlags = 0x1000000;
    if (initData)
    {
        memcpy(lazy, initData, sizeof(tLazyMdfStruct));
    }
    else
    {
        memset(lazy, 0, sizeof(tLazyMdfStruct));
        lazy->f2 = 0.89999998f;
        lazy->ff1 = -0.1f;
        lazy->ff2 = -0.1f;
        lazy->ff3 = -0.1f;
        lazy->upperMax = 0.1f;
        lazy->ff5 = 0.1f;
        lazy->ff6 = 0.1f;
    }
    GAO_ModifierLazy_Reinit(mdf);
}

void GAO_ModifierLazy_Destroy(tModifier* mdf)
{
    if (mdf->mdfPrivateDataPtr)
        MEM_FreeAlign(mdf->mdfPrivateDataPtr);
}

int GAO_ul_ModifierLazy_Load(tModifier* mdf, char* data)
{
    char* start;
    tLazyMdfStruct* lazy;
    unsigned int version;

    start = data;
    lazy = (tLazyMdfStruct*)mdf->mdfPrivateDataPtr;
    LOA_ReadLong(&data, 0, 2);
    version = LOA_ReadLong(&data, 0, 1);
    lazy->f2 = LOA_ReadFloat(&data, 0, 1);
    lazy->upperFlag = LOA_ReadLong(&data, 0, 1);

    if (!version)
    {
        lazy->ff1 = -1.0;
        lazy->ff2 = -1.0;
        lazy->ff3 = -1.0;
        lazy->upperMax = 1.0;
        lazy->ff5 = 1.0;
        lazy->ff6 = 1.0;
        return data - start;
    }

    lazy->ff1 = LOA_ReadFloat(&data, 0, 1);
    lazy->ff2 = LOA_ReadFloat(&data, 0, 1);
    lazy->ff3 = LOA_ReadFloat(&data, 0, 1);
    lazy->upperMax = LOA_ReadFloat(&data, 0, 1);
    lazy->ff5 = LOA_ReadFloat(&data, 0, 1);
    lazy->ff6 = LOA_ReadFloat(&data, 0, 1);
    if (version > 1)
    {
        lazy->fff1 = LOA_ReadFloat(&data, 0, 1);
        if (version > 2)
            lazy->fff2 = LOA_ReadFloat(&data, 0, 1);
    }
    return data - start;
}

void GAO_ModifierLazy_Unapply(struct sModifier* mdf, void* unk1)
{
}

void GAO_ModifierLazy_UnApply_Local(tLazyMdfStruct* lazy)
{
    tGameObjectMain* gao;
    Mtx4x4* dst;

    lazy->fff1 = lazy->ffffdf;
    lazy->fff2 = lazy->upperFriction;
    lazy->f2 = lazy->ffff2;
    lazy->upperMax = lazy->aaf1;
    lazy->ff5 = lazy->aaf2;
    lazy->ff6 = lazy->aaf3;
    lazy->ff1 = lazy->af1;
    lazy->ff2 = lazy->af2;
    lazy->ff3 = lazy->af3;
    lazy->upperFlag = lazy->upperFlagBak & ~0x10000000u;

    gao = lazy->gao;
    if ((gao->flags & 0x400000) != 0)
        dst = &gao->basic->mtx->mtx;
    else
        dst = gao->matrices;
    MATH_CopyMatrix(dst, &lazy->mtx1);
}

void GAO_ModifierLazy_UnapplyAll(void)
{
    tLazyMdfStruct* lazy;

    gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    CAM_Inverse(&gpst_DX_stDD->ct);
    gpst_DX_stDD->SetViewMatrix(&gpst_DX_stDD->ct.cam.cameraMtx);
    for (lazy = p_FirsToUnapply; lazy; lazy = lazy->firstToUnapply)
        GAO_ModifierLazy_UnApply_Local(lazy);
    p_FirsToUnapply = NULL;
}
