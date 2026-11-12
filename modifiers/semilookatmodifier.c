#include "../loading.h"
#include "semilookatmodifier.h"

void GAO_ModifierSemiLookAt_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    mdf->mdfPrivateDataPtr = NULL;
}

void GAO_ModifierSemiLookAt_Unapply(struct sModifier* mdf, void* unk1)
{
}

void GAO_ModifierSemiLookAt_Apply(struct sModifier* mdf, void* unk1)
{
    tGameObjectMain* gao;
    Mtx4x4* mtx;
    Mtx4x4* objMtx;
    Vec3 camToObj;

    gao = gpst_DX_stDD->currTargetGao;
    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    objMtx = gao->matrices;
    camToObj.x = objMtx->Pos.x - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
    camToObj.y = objMtx->Pos.y - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
    camToObj.z = objMtx->Pos.z - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
    MATH_CrossProduct(&mtx->v1, &camToObj, &mtx->v3);
    MATH_NormalizeEqualVector(&mtx->v1);
    MATH_CrossProduct(&mtx->v2, &mtx->v3, &mtx->v1);
    OBJ_ComputeLocalWhenHie(gao);
}

int GAO_ModifierSemiLookAt_Load(tModifier* mdf, char* data)
{
    char* start;

    start = data;
    mdf->modifierFlags |= 0x1000000u;
    LOA_ReadLong(&data, 0, 2);
    return data - start;
}

void GAO_ModifierSemiLookAt_Destroy(struct sModifier* mdf)
{
}

void GAO_ModifierSemiLookAt_Reinit(struct sModifier* mdf)
{
}
