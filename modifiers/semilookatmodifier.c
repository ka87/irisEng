#include "../loading.h"
#include "semilookatmodifier.h"

void GAO_ModifierSemiLookAt_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    a2->mdfPrivateDataPtr = NULL;
}

void GAO_ModifierSemiLookAt_Unapply(struct sModifier* a1, void* unk1) {

}

void GAO_ModifierSemiLookAt_Apply(struct sModifier* a1, void* unk1)
{
    tGameObjectMain* v0;
    Mtx4x4* v1;
    Mtx4x4* v2;
    double v3;
    double v4;
    double v5;
    double v6;
    long double v7;
    long double v8;
    long double v9;
    long double v10;

    v0 = gpst_DX_stDD->currTargetGao;
    v1 = OBJ_pst_GetAbsoluteMatrix(v0);
    v2 = v0->matrices;
    v3 = v2->Pos.x - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
    v4 = v2->Pos.y - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
    v5 = v2->Pos.z - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
    v1->v1.x = (float)(v4 * v1->v3.z - v5 * v1->v3.y);
    v1->v1.y = (float)(v5 * v1->v3.x - v3 * v1->v3.z);
    v6 = v3 * v1->v3.y - v4 * v1->v3.x;
    v1->v1.z = (float)v6;
    v7 = sqrt(v1->v1.x * v1->v1.x + v1->v1.y * v1->v1.y + v6 * v6);
    v1->v1.x = (float)(1.0 / v7 * v1->v1.x);
    v8 = 1.0 / v7 * v1->v1.y;
    v1->v1.y = (float)v8;
    v9 = v8;
    v10 = 1.0 / v7 * v1->v1.z;
    v1->v1.z = (float)v10;
    v1->v2.x = (float)(v10 * v1->v3.y - v9 * v1->v3.z);
    v1->v2.y = v1->v1.x * v1->v3.z - v1->v3.x * v1->v1.z;
    v1->v2.z = v1->v3.x * v1->v1.y - v1->v3.y * v1->v1.x;
    OBJ_ComputeLocalWhenHie(v0);
}

int GAO_ModifierSemiLookAt_Load(tModifier* arg0, char* a1)
{
    char* v2;

    v2 = a1;
    arg0->modifierFlags |= 0x1000000u;
    a1 = v2;
    LOA_ReadLong(&a1, 0, 2);
    return a1 - v2;
}

void GAO_ModifierSemiLookAt_Destroy(struct sModifier* a1) {
}

void GAO_ModifierSemiLookAt_Reinit(struct sModifier* a1) {
}