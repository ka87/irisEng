#include "../memory.h"
#include "../loading.h"
#include "../gameobject.h"
#include "../particlegenerator.h"
#include "../graphicsobjects/particle.h"
#include "../soft.h"
#include "particlemodifier.h"

void MPAG_Modifier_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tMPAGStruct* v3;

    v3 = (tMPAGStruct*)MEM_p_Alloc(sizeof(tMPAGStruct));
    a2->mdfPrivateDataPtr = v3;
    a2->modifierFlags = 0x1000000;
    if (a3)
        v3->pag = (tPAGStruct*)PAG_p_Duplicate((tGroGeneric *)(((tMPAGStruct*)(a3))->pag));
    else
        v3->pag = NULL;
}

void MPAG_Modifier_Destroy(struct sModifier* a1)
{
    tMPAGStruct* v1;

    v1 = (tMPAGStruct*)a1->mdfPrivateDataPtr;
    if (v1)
    {
        PAG_Free((tGroGeneric*)(v1->pag));
        MEM_Free(v1);
    }
}

void MPAG_Modifier_Apply(tModifier* a1, void* unk1)
{
    tMPAGStruct* v1;
    Mtx4x4* v2;

    v1 = (tMPAGStruct*)a1->mdfPrivateDataPtr;
    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
    v2 = OBJ_pst_GetAbsoluteMatrix(a1->gao);
    SOFT_l_MatrixStack_Push(&gpst_DX_stDD->matrixStack, v2);
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    PAG_RenderCommon(a1->gao, v1->pag);
    SOFT_l_MatrixStack_Pop(&gpst_DX_stDD->matrixStack);
}

void MPAG_ul_Modifier_Reinit(struct sModifier* a1)
{
    tMPAGStruct* v1;
    tPAGStruct* a1a; 

    v1 = (tMPAGStruct*)a1->mdfPrivateDataPtr;
    if (!v1->pag)
        v1->pag = PAG_pst_Create();
    a1a = v1->pag;
    PAG_Reinit((tGroGeneric*)a1a);
}

int MPAG_ul_Modifier_Load(tModifier* a1, char* a2)
{
    char* v2;
    tMPAGStruct* v3;
    char* v4;

    v2 = a2;
    v3 = (tMPAGStruct*)a1->mdfPrivateDataPtr;
    v3->pag = (tPAGStruct*)PAG_p_CreateFromBuffer(NULL, &a2, NULL);
    v4 = a2;
    a1->modifierFlags = 0x1000000;
    return v4 - v2;
}

void MPAG_Modifier_Unapply(tModifier* a1, void* unk1) {
}