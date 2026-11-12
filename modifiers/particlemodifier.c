#include "../memory.h"
#include "../loading.h"
#include "../gameobject.h"
#include "../particlegenerator.h"
#include "../graphicsobjects/particle.h"
#include "../soft.h"
#include "particlemodifier.h"

void MPAG_Modifier_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tMPAGStruct* mpag;

    mpag = (tMPAGStruct*)MEM_p_Alloc(sizeof(tMPAGStruct));
    mdf->mdfPrivateDataPtr = mpag;
    mdf->modifierFlags = 0x1000000;
    if (initData)
        mpag->pag = (tPAGStruct*)PAG_p_Duplicate((tGroGeneric*)(((tMPAGStruct*)(initData))->pag));
    else
        mpag->pag = NULL;
}

void MPAG_Modifier_Destroy(struct sModifier* mdf)
{
    tMPAGStruct* mpag;

    mpag = (tMPAGStruct*)mdf->mdfPrivateDataPtr;
    if (mpag)
    {
        PAG_Free((tGroGeneric*)(mpag->pag));
        MEM_Free(mpag);
    }
}

void MPAG_Modifier_Apply(tModifier* mdf, void* unk1)
{
    tMPAGStruct* mpag;
    Mtx4x4* mtx;

    mpag = (tMPAGStruct*)mdf->mdfPrivateDataPtr;
    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
    mtx = OBJ_pst_GetAbsoluteMatrix(mdf->gao);
    SOFT_l_MatrixStack_Push(&gpst_DX_stDD->matrixStack, mtx);
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    PAG_RenderCommon(mdf->gao, mpag->pag);
    SOFT_l_MatrixStack_Pop(&gpst_DX_stDD->matrixStack);
}

void MPAG_ul_Modifier_Reinit(struct sModifier* mdf)
{
    tMPAGStruct* mpag;

    mpag = (tMPAGStruct*)mdf->mdfPrivateDataPtr;
    if (!mpag->pag)
        mpag->pag = PAG_pst_Create();
    PAG_Reinit((tGroGeneric*)mpag->pag);
}

int MPAG_ul_Modifier_Load(tModifier* mdf, char* data)
{
    char* start;
    tMPAGStruct* mpag;

    start = data;
    mpag = (tMPAGStruct*)mdf->mdfPrivateDataPtr;
    mpag->pag = (tPAGStruct*)PAG_p_CreateFromBuffer(NULL, &data, NULL);
    mdf->modifierFlags = 0x1000000;
    return data - start;
}

void MPAG_Modifier_Unapply(tModifier* mdf, void* unk1)
{
}
