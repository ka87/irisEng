#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "rotrmodifier.h"

void GAO_ModifierROTR_UpdatePointers(tModifier* mdf)
{
    tRotrMdfStruct* rotr;
    tGaoMatrix* objMtx;

    rotr = (tRotrMdfStruct*)mdf->mdfPrivateDataPtr;
    rotr->gao = 0;
    if (!mdf->gao || !mdf->gao->basic)
        return;

    objMtx = mdf->gao->basic->mtx;
    if (!objMtx || !objMtx->refGao2)
        return;

    rotr->gao = objMtx->refGao2;
    rotr->v1 = objMtx->mtx.v3;
}

void GAO_ModifierROTR_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tRotrMdfStruct* rotr;

    rotr = (tRotrMdfStruct*)MEM_p_Alloc(sizeof(tRotrMdfStruct));
    mdf->mdfPrivateDataPtr = rotr;
    mdf->modifierFlags = 0x1000000;
    if (initData)
    {
        memcpy(rotr, initData, sizeof(tRotrMdfStruct));
    }
    else
    {
        memset(rotr, 0, sizeof(tRotrMdfStruct));
        GAO_ModifierROTR_UpdatePointers(mdf);
    }
    rotr->gao = 0;
}

void GAO_ModifierROTR_Destroy(struct sModifier* mdf)
{
    if (mdf->mdfPrivateDataPtr)
        MEM_Free(mdf->mdfPrivateDataPtr);
}

static double GAO_l_ModifierROTR_BlendFactor(float speed)
{
    double t;

    t = TIM_gf_dt * speed;
    if (t < 0.0)
        return 0.0;
    if (t > 1.0)
        return 1.0;
    return t;
}

void GAO_ModifierROTR_Apply(struct sModifier* mdf, void* unk1)
{
    tRotrMdfStruct* rotr;
    tGameObjectMain* gao;
    Mtx4x4* after;
    Mtx4x4 before;
    tGaoMatrix* objMtx;
    Vec3 dir;
    Vec3 blended;
    double tx;
    double ty;
    double tz;

    rotr = (tRotrMdfStruct*)mdf->mdfPrivateDataPtr;
    if (!rotr->gao)
        return;

    gao = mdf->gao;
    before = *gao->matrices;
    OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    after = mdf->gao->matrices;

    dir.x = before.v3.x + (before.Pos.x - after->Pos.x);
    dir.y = before.v3.y + (before.Pos.y - after->Pos.y);
    dir.z = before.v3.z + (before.Pos.z - after->Pos.z);
    MATH_SetNormVector(&dir, &dir, 1.0);
    MATH_VectorGlobalToLocal(&dir, rotr->gao->matrices, &dir);

    tx = GAO_l_ModifierROTR_BlendFactor(rotr->ff1);
    ty = GAO_l_ModifierROTR_BlendFactor(rotr->timeFactor1);
    tz = GAO_l_ModifierROTR_BlendFactor(rotr->timeFactor2);
    blended.x = (float)((1.0 - tx) * dir.x + tx * rotr->v1.x);
    blended.y = (float)((1.0 - ty) * dir.y + ty * rotr->v1.y);
    blended.z = (float)((1.0 - tz) * dir.z + tz * rotr->v1.z);
    MATH_SetNormVector(&blended, &blended, 1.0);

    objMtx = mdf->gao->basic->mtx;
    objMtx->mtx.v3 = blended;
    MATH_CrossProduct(&objMtx->mtx.v2, &objMtx->mtx.v3, &objMtx->mtx.v1);
    MATH_NormalizeEqualVector(&objMtx->mtx.v2);
    MATH_CrossProduct(&objMtx->mtx.v1, &objMtx->mtx.v2, &objMtx->mtx.v3);
    MATH_NormalizeEqualVector(&objMtx->mtx.v1);
}

int GAO_ModifierROTR_Load(struct sModifier* mdf, char* data)
{
    tRotrMdfStruct* rotr;
    char* start;

    rotr = (tRotrMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    LOA_ReadLong(&data, 0, 2);
    rotr->ii = LOA_ReadLong(&data, 0, 1);
    rotr->ff1 = LOA_ReadFloat(&data, 0, 1);
    rotr->timeFactor1 = LOA_ReadFloat(&data, 0, 1);
    rotr->timeFactor2 = LOA_ReadFloat(&data, 0, 1);
    rotr->ff4 = LOA_ReadFloat(&data, 0, 1);
    rotr->gao = 0;
    return data - start;
}

void GAO_ModifierROTR_Reinit(struct sModifier* mdf)
{
    GAO_ModifierROTR_UpdatePointers(mdf);
}

void GAO_ModifierROTR_Unapply(struct sModifier* mdf, void* unk1)
{
}
