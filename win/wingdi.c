#include "../profiler.h"
#include "../time.h"
#include "../memory.h"
#include "../textures.h"
#include "../dbg.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/staticlod.h"
#include "../water.h"
#include "../engine.h"
#include "wininputoutput.h"
#include "../resource.h"
#include "wingdi.h"
#include "../profiler.h"

D3DMATRIX d3dProjectionMtx = { 1.4616959f, 0.0, 0.0, 0.0,
                               0.0, -1.9489279f, 0.0, 0.0,
                               0.0, 0.0, 1.000002f, 1.0, 0.0,
                               0.0, -0.050000001f, 0.0 };

D3DMATRIX d3dTransformMatrix = { 1.0, 0.0, 0.0, 0.0,
                                 0.0, 1.0, 0.0, 0.0,
                                 0.0, 0.0, 1.0, 0.0,
                                 0.0, 0.0, 0.0, 1.0 };

D3DMATRIX d3dTransformMatrix2 = { 1.0, 0.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0 };

int engineFogTableModeToD3D[4] = { 3, 1, 2, 0 };

tRasterNames rasterNames[] = {
    {.name = "MisEng", .unk1 = 1},
    {.name = "Lig", .unk1 = 2},
    {.name = "SKN", .unk1 = 2},
    {.name = "SDW", .unk1 = 2},
    {.name = "DRW", .unk1 = 2},
    {.name = "ANIr", .unk1 = 2},
    {.name = "SRS", .unk1 = 2},
    {.name = "GFX", .unk1 = 7},
    {.name = "CVC", .unk1 = 2},
    {.name = "CSpec", .unk1 = 2},
    {.name = "CDAlp", .unk1 = 1},
    {.name = "ANI", .unk1 = 1},
    {.name = "MAT D", .unk1 = 2},
    {.name = "Zlst S", .unk1 = 2},
    {.name = "AI", .unk1 = 1},
    {.name = "INO", .unk1 = 1},
    {.name = "Col", .unk1 = 1},
    {.name = "Rec", .unk1 = 1},
    {.name = "Eng", .unk1 = 5},
    {.name = "Dis", .unk1 = 6},
    {.name = "TxProc", .unk1 = 2},
    {.name = "Flip", .unk1 = 2},
    {.name = "MRM", .unk1 = 2},
    {.name = "1 - DRW", .unk1 = 2},
    {.name = "GO - DRW", .unk1 = 2},
    {.name = "SND", .unk1 = 1},
    {.name = "TAM", .unk1 = 1},
    {.name = "EngRei", .unk1 = 1},
    {.name = "WorAct", .unk1 = 1},
    {.name = "EOT", .unk1 = 1},
    {.name = "SnP", .unk1 = 1},
    {.name = "MdF", .unk1 = 1},
    {.name = "EVE", .unk1 = 1},
    {.name = "HIE", .unk1 = 1},
    {.name = "DYN", .unk1 = 1},
    {.name = "VIEW", .unk1 = 7},
    {.name = "Visi", .unk1 = 7},
    {.name = "Lens F", .unk1 = 2},
    {.name = "FIFO", .unk1 = 6},
    {.name = "AI - SND", .unk1 = 1},
    {.name = "IOP", .unk1 = 1},
    {.name = "VIDEO", .unk1 = 2},
    {.name = "SPG", .unk1 = 2},
    {.name = "BigVB", .unk1 = 2},
    {.name = "GoodVB", .unk1 = 2},
    {.name = "IndP", .unk1 = 2},
    {.name = "Sprite", .unk1 = 2},
    {.name = "Raster", .unk1 = 4},
    {.name = "Water", .unk1 = 2},
    {.name = "?????", .unk1 = 2},
    {.name = "?????", .unk1 = 2},
    {.name = "?????", .unk1 = 2},
    {.name = "Morph", .unk1 = 2},
    {.name = "Total", .unk1 = 7}
};

long long qword_990B48; // rasters

tWorld* worldPtrIfMultiStream = NULL; // TODO FIXME game doesn't set it explicitly to NULL

void PC_SetUVMatrix(Mtx4x4* dest, Vec3* v1, Vec3* v2, Vec3* v3, Vec3* pos, Vec3* scale, int unused, int flags)
{
    dest->v1 = *v1;
    dest->v2 = *v2;
    dest->v3 = *v3;
    dest->_m14 = scale->x;
    dest->_m24 = scale->y;
    dest->_m34 = scale->z;
    dest->Pos = *pos;
    dest->w = 1.0f;
    dest->mtxflag = flags;
}

void MAT_ComputeUV_sub_4521A0(tGEOGRO* geogro, tGeoTriangleChunks* a2, tMultiTextureElemStruct* a3)
{
    profilerRasterStructElem* v3;
    int v4;
    unsigned int v5;
    int v6;
    profilerRasterStructElem* v7;
    __int16 v8;
    __int16 v9;
    tMatDecompressedStruct a2a;
    Vec3 v11;
    Vec3 v12;
    Vec3 v13;
    Vec3 v14;
    Mtx4x4 a1;
    Mtx4x4 a3a;
    Vec3 a2a2; 

    v3 = &gpst_DX_stDD->rasters[38];
    if (!gpst_DX_stDD->rasters[38].insideCounter)
        v3->startTick = TIM_GetTicksAsInt32();
    ++v3->insideCounter;
    v4 = a3->phongOrPlanarParam;
    v5 = (a3->subTextureFlags >> 20) & 0xF;
    D3DRenderParams.textureUVMode = 0;
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
    }
    if (textureStageTypeConfig[0].d3dTextureTransformFlags)
    {
        textureStageTypeConfig[0].d3dTextureTransformFlags = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
    }
    gpst_DX_stDD->flags &= ~0x10u;
    MAT_FuncTable[v5](gpst_DX_stDD, geogro, a2, v4);
    if (!gpst_DX_stDD->bigBuffer->UVData)
    {
        if (D3DRenderParams.textureUVMode)
        {
            MATH_MakeOGLMatrix(&a1, &GLOBAL_UVTransformMatrix);
            memcpy(d3dTransformationMatrix, &a1, sizeof(Mtx4x4WithoutAdditionalData));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX *)&a1);
        }
        return;
    }
    v6 = a3->transform1;
    if (v6 == 0x3F800000 && a3->transform2 == 0x3F800000 || !v6 || !a3->transform2) // TODO: MAT_VUIsIdentity inline
    {
        if (!D3DRenderParams.textureUVMode)
        {
            D3DRenderParams.textureUVMode = 0;
            if (textureStageTypeConfig[0].d3dTexCoordIndex)
            {
                textureStageTypeConfig[0].d3dTexCoordIndex = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
            }
            if (textureStageTypeConfig[0].d3dTextureTransformFlags)
            {
                textureStageTypeConfig[0].d3dTextureTransformFlags = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
            }
            goto LABEL_28;
        }
        MATH_MakeOGLMatrix(&a1, &GLOBAL_UVTransformMatrix);
    }
    else
    {
        MAT_UVDecompress(a3, &a2a);
        v12.x = a2a.f1;
        v12.y = a2a.f2;
        v11.x = a2a.f3;
        v11.y = a2a.f4;
        v14.x = a2a.speedU;
        v14.y = a2a.speedV;
        v12.z = 0.0;
        v11.z = 0.0;
        v13.x = 0.0;
        v13.y = 0.0;
        v13.z = 1.0;
        v14.z = 0.0;
        a2a2.x = 1.0;
        a2a2.y = 1.0;
        a2a2.z = 1.0;
        PC_SetUVMatrix(&a3a, &v12, &v11, &v13, &v14, &a2a2, 0, 6);
        if (D3DRenderParams.textureUVMode)
        {
            MATH_MulMatrixMatrix(&a1, &GLOBAL_UVTransformMatrix, &a3a);
            MATH_MakeOGLMatrix(&a1, &a1);
        }
        else
        {
            D3DRenderParams.textureUVMode = 3;
            if (textureStageTypeConfig[0].d3dTexCoordIndex)
            {
                textureStageTypeConfig[0].d3dTexCoordIndex = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
            }
            if (textureStageTypeConfig[0].d3dTextureTransformFlags != 2)
            {
                textureStageTypeConfig[0].d3dTextureTransformFlags = 2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }
            MATH_MakeOGLMatrix(&a1, &a3a);
            a1.v3.x = a1.Pos.x;
            a1.v3.y = a1.Pos.y;
        }
    }
    memcpy(d3dTransformationMatrix, &a1, sizeof(Mtx4x4WithoutAdditionalData));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&a1);
LABEL_28:
    v7 = &gpst_DX_stDD->rasters[38];
    v8 = gpst_DX_stDD->rasters[38].insideCounter;
    if (v8)
    {
        v9 = v8 - 1;
        gpst_DX_stDD->rasters[38].insideCounter = v9;
        if (!v9)
            v7->sumTime += TIM_GetTicksAsInt32() - v7->startTick;
    }
}

void MAT_UV_Compute_Chrome(tGdi_DisplayData* a1, tGEOGRO* a21, tGeoTriangleChunks* a31, unsigned int a41)
{
    int v1;
    Vec3 a3;
    Vec3 a4;
    Vec3 a5;
    Vec3 a6;
    Vec3 a2;

    D3DRenderParams.textureUVMode = 2;
    if (textureStageTypeConfig[0].d3dTexCoordIndex != D3DTSS_TCI_CAMERASPACENORMAL)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = D3DTSS_TCI_CAMERASPACENORMAL;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
    }
    if (textureStageTypeConfig[0].d3dTextureTransformFlags != D3DTTFF_COUNT2)
    {
        textureStageTypeConfig[0].d3dTextureTransformFlags = D3DTTFF_COUNT2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    }
    v1 = a1->drawMaskAllowedStuffCurrent;
    a2.z = 0.0;
    a2.y = 0.0;
    a2.x = 0.5;
    if ((v1 & 0x8000) != 0)
        a2.x = -0.5;
    a3.x = 0.0;
    a3.y = -0.5;
    a3.z = 0.0;
    a4.x = 0.0;
    a4.y = 0.0;
    a4.z = -0.5;
    a5.x = 0.5;
    a5.y = 0.5;
    a5.z = 0.0;
    a6.x = 1.0;
    a6.y = 1.0;
    a6.z = 1.0;
    PC_SetUVMatrix(&GLOBAL_UVTransformMatrix, &a2, &a3, &a4, &a5, &a6, 0, 4);
    a1->bigBuffer->UVData = a1->bigBuffer->data7;
    a1->flags |= 2u;
}

void MAT_UV_Compute_PLANAR_GIZMO_NOSTORE(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int type)
{
    int elemIdx;
    tMultiPartGameObjectLinks* link;
    Mtx4x4* multiPartMtx;
    //double v14; 
    Mtx4x4* viewMtx;
    Vec3 v1, v2, v3, pos, scale; 
    Mtx4x4 multiPartMtxLocal = {0};
    Mtx4x4 viewMtxStd;
    Mtx4x4 viewMtxStdInv;
    Mtx4x4 modelViewMtx;

    a1->bigBuffer->UVData = a1->bigBuffer->data7;
    a1->flags |= 0x2;
    D3DRenderParams.textureUVMode = 1;
    if (textureStageTypeConfig[0].d3dTexCoordIndex != D3DTSS_TCI_CAMERASPACEPOSITION)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
    }
    if (textureStageTypeConfig[0].d3dTextureTransformFlags != D3DTTFF_COUNT2)
    {
        textureStageTypeConfig[0].d3dTextureTransformFlags = D3DTTFF_COUNT2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    }
    viewMtx = a1->ct.cam.viewMtxPtr_setInGRORender;
    if ((viewMtx->mtxflag & 8) != 0)
    {
        MATH_MakeOGLMatrix(&viewMtxStd, viewMtx);
        viewMtxStdInv.mtxflag = 0x4;
        D3DXMatrixInverse((D3DXMATRIX*)&viewMtxStdInv, NULL, (D3DXMATRIX*)&viewMtxStd);
    }
    else
    {
        MATH_InvertMatrix(&viewMtxStdInv, viewMtx);
    }

    if (!((uint8_t)type >> 6))
    {
        switch ((type >> 4) & 3)
        {
        case 0:
            v1.x = 1.0f;
            v1.y = 1.0f;
            v1.z = 1.0f;
            v2.x = 0.0f;
            v2.y = 0.0f;
            v2.z = 0.0f;
            v3.x = 0.0f;
            v3.y = 1.0f;
            v3.z = 0.0f;
            pos.x = 1.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            scale.x = 0.0f;
            scale.y = 0.0f;
            scale.z = 1.0f;

            PC_SetUVMatrix(&viewMtxStd, &scale, &pos, &v3, &v2, &v1, 0, 4);
            MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
            break;
        case 1:
            v1.x = 1.0f;
            v1.y = 0.0f;
            v1.z = 0.0f;
            v2.x = 0.0f;
            v2.y = 0.0f;
            v2.z = 1.0f;
            v3.x = 0.0f;
            v3.y = 1.0f;
            v3.z = 0.0f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
            PC_SetUVMatrix(&viewMtxStd, &v1, &v2, &v3, &pos, &scale, 0, 4);
            MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
            break;
        case 2:
            memcpy(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, sizeof(GLOBAL_UVTransformMatrix));
            break;
        case 3:
            v1.x = 0.70710677f;
            v1.y = -0.57735026f;
            v1.z = 0.40799999f;
            v2.x = -0.70710677f;
            v2.y = -0.57735026f;
            v2.z = 0.40799999f;
            v3.x = 0.0f;
            v3.y = 0.57735026f;
            v3.z = 0.81599998f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
            PC_SetUVMatrix(&viewMtxStd, &v1, &v2, &v3, &pos, &scale, 0, 0x4);
            MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
            break;
        default:
            return;
        }
        return;
    }

    elemIdx = (type >> 8) & 7;

    if (((uint8_t)type >> 6) != 3 || (a1->currTargetGao->flags & 0x200000) == 0)
    {
        if (((uint8_t)type >> 6) == 2)
        {
            MATH_MakeOGLMatrix(&multiPartMtxLocal, a1->matrixStack.actMatrixElem);
            MATH_VecMultScalarWithoutBuffer(&multiPartMtxLocal.v1, (a1->ct.cam.factorX / a1->ct.cam.centerX));
            MATH_VecMultScalarWithoutBuffer(&multiPartMtxLocal.v2, (a1->ct.cam.factorX / a1->ct.cam.centerX));
            MATH_VecMultScalarWithoutBuffer(&multiPartMtxLocal.v3, (a1->ct.cam.factorX / a1->ct.cam.centerX));            
        }
        else
        {
            MATH_MakeOGLMatrix(&multiPartMtxLocal, a1->currTargetGao->matrices);
        }
    } else if (a1->currTargetGao->basic->multiPartData->elemNum <= elemIdx)
    {
        MATH_MakeOGLMatrix(&multiPartMtxLocal, a1->currTargetGao->matrices);
    } else if ((a1->currTargetGao->flags & 0x1000000) == 0)
    {
        multiPartMtx = ((Mtx4x4*)(a1->currTargetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices));
        MATH_InvertMatrix(&multiPartMtxLocal, &multiPartMtx[elemIdx]);
        MATH_MakeOGLMatrix(&multiPartMtxLocal, &multiPartMtxLocal);
    } else if (link = ((tMultiPartGameObjectLinks*)a1->currTargetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices), link[elemIdx].gao != NULL)
    {
        if ((link[elemIdx].gao->flags & 0x200000) != 0 && (link[elemIdx].gao->flags & 0x1000000) == 0)
        {
            if (!link[elemIdx].mtxPtr)
            {
                MATH_InvertMatrix(&multiPartMtxLocal, &MATH_gst_IdentityMatrix);
            }
            else 
            {
                MATH_InvertMatrix(&multiPartMtxLocal, link[elemIdx].mtxPtr);
            }
            MATH_MakeOGLMatrix(&multiPartMtxLocal, &multiPartMtxLocal);
        }
        else {
            MATH_InvertMatrix(&multiPartMtxLocal, ((tMultiPartGameObjectLinks*)(a1->currTargetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx].gao->matrices);
            MATH_MulMatrixMatrix(&viewMtxStd, a1->currTargetGao->matrices, &multiPartMtxLocal);
            MATH_MakeOGLMatrix(&multiPartMtxLocal, &viewMtxStd);
        }
    }
    else
    {
        MATH_InvertMatrix(&multiPartMtxLocal, &MATH_gst_IdentityMatrix);
        MATH_MakeOGLMatrix(&multiPartMtxLocal, &multiPartMtxLocal);
    }

    switch ((type >> 4) & 3)
    {
    case 0:
        v1.x = 0.0f;
        v1.y = 0.0f;
        v1.z = 1.0f;
        v2.x = 1.0f;
        v2.y = 0.0f;
        v2.z = 0.0f;
        v3.x = 0.0f;
        v3.y = 1.0f;
        v3.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        PC_SetUVMatrix(&viewMtxStd, &v1, &v2, &v3, &pos, &scale, 0, 0x4);
        MATH_MulMatrixMatrix(&modelViewMtx, &multiPartMtxLocal, &viewMtxStd);
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &modelViewMtx);
        break;
    case 1:
        v1.x = 1.0f;
        v1.y = 0.0f;
        v1.z = 0.0f;
        v2.x = 0.0f;
        v2.y = 0.0f;
        v2.z = 1.0f;
        v3.x = 0.0f;
        v3.y = 1.0f;
        v3.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        PC_SetUVMatrix(&modelViewMtx, &v1, &v2, &v3, &pos, &scale, 0, 4);
        MATH_MulMatrixMatrix(&viewMtxStd, &multiPartMtxLocal, &modelViewMtx);
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
        break;
    case 2:
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &multiPartMtxLocal);
        break;
    case 3:
        MATH_TranspEq33Matrix(&multiPartMtxLocal);
        v1.x = 0.70710677f;
        v1.y = -0.57735026f;
        v1.z = 0.40799999f;
        v2.x = -0.70710677f;
        v2.y = -0.57735026f;
        v2.z = 0.40799999f;
        v3.x = 0.0;
        v3.y = 0.57735026f;
        v3.z = 0.81599998f;
        pos.x = 0.0;
        pos.y = 0.0;
        pos.z = 0.0;
        scale.x = 1.0;
        scale.y = 1.0;
        scale.z = 1.0;
        PC_SetUVMatrix(&modelViewMtx, &v1, &v2, &v3, &pos, &scale, 0, 4);
        MATH_MulMatrixMatrix(&viewMtxStd, &multiPartMtxLocal, &modelViewMtx);
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
        break;
    default:
        break;
    }
    a1->bigBuffer->UVData = a1->bigBuffer->data7;
    a1->flags |= 0x2;
}

void D3D_CalcAndSetProjection(float a1, float a2, float a3, float a4, float a5, float a6)
{
    double v6;
    double v7;
    float v8; 
    D3DMATRIX v9;

    v6 = a2 - a1;
    memset(&v9, 0, sizeof(v9));
    v9._44 = 1.0;
    v9._11 = (float)(2.0 / v6);
    v7 = a4 - a3;
    v9._22 = (float)(2.0 / v7);
    v8 = a6 - a5;
    v9._33 = (float)(-4.0 / v8);
    v9._14 = (float)(-((a1 + a2) / v6));
    v9._24 = (float)(-((a3 + a4) / v7));
    v9._34 = (float)(-((a5 + a6) / v8));
    memcpy(&d3dProjectionMtx, &v9, sizeof(D3DMATRIX));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &v9);
}

void D3D_SetViewPort(int a1, int a2, int a3, int a4)
{
    int v4;
    int v5;
    int v6;
    int v7;

    if (D3DRenderParams.d3dDev)
    {
        v4 = a1;
        if (a1 < 0)
            v4 = 0;
        v5 = a2;
        if (a2 < 0)
            v5 = 0;
        v6 = a3;
        if (v4 + a3 > (int)D3DRenderParams.presentParams.BackBufferWidth)
            v6 = D3DRenderParams.presentParams.BackBufferWidth - v4;
        v7 = a4;
        if (v5 + a4 > (int)D3DRenderParams.presentParams.BackBufferHeight)
            v7 = D3DRenderParams.presentParams.BackBufferHeight - v5;
        D3DRenderParams.d3dViewPort.X = v4;
        D3DRenderParams.d3dViewPort.Y = v5;
        D3DRenderParams.d3dViewPort.Width = v6;
        D3DRenderParams.d3dViewPort.Height = v7;
        D3DRenderParams.d3dViewPort.MinZ = 0.0;
        D3DRenderParams.d3dViewPort.MaxZ = 1.0;
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, (&D3DRenderParams.d3dViewPort));
    }
}

int   D3D_gi_Widescreen = 1;
int   D3D_gi_AspectTrace = 0;
int   D3D_gi_Pillarbox2D = 1;
float D3D_gf_RefYoverX = 0.75f;
float D3D_gf_CutsceneBand = 0.10416666f;

float D3D_RealYoverX(void)
{
    // the drawing area as the projection sees it, falling back to
    // the back buffer before the window rectangle is known
    float w = (float)(D3DRenderParams.Rect.right - D3DRenderParams.Rect.left);
    float h = (float)(D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top);

    if (w <= 0.0f || h <= 0.0f)
    {
        w = (float)D3DRenderParams.presentParams.BackBufferWidth;
        h = (float)D3DRenderParams.presentParams.BackBufferHeight;
    }
    if (w <= 0.0f || h <= 0.0f)
        return D3D_gf_RefYoverX;
    return h / w;
}

int D3D_FlatLayerWidth(int w, int h)
{
    int boxW;

    if (!D3D_gi_Pillarbox2D || D3D_gf_RefYoverX <= 0.0f || h <= 0)
        return w;
    boxW = (int)((float)h / D3D_gf_RefYoverX);
    return boxW < w ? boxW : w;
}

void D3D_GetBandFractions(float* top, float* bottom, float* left, float* right)
{
    float g;

    // The vertical field of view is the one the scenes were laid out
    // for, so the full height of the screen is the authored frame and
    // the top and bottom bands keep the size they always had.
    *top = D3D_gf_CutsceneBand;
    *bottom = 1.0f - D3D_gf_CutsceneBand;
    *left = 0.0f;
    *right = 1.0f;
    if (!D3D_gi_Widescreen || D3D_gf_RefYoverX <= 0.0f)
        return;

    // Across, the screen shows more than the authored frame did. The
    // side bands take that back, so the cutscene is framed exactly as
    // laid out however wide the screen is.
    g = D3D_RealYoverX() / D3D_gf_RefYoverX;
    if (g < 1.0f)
    {
        *left = (1.0f - g) * 0.5f;
        *right = 1.0f - *left;
    }
}

void D3D_AspectTrace(const char* where, int viewX, int viewY, int viewW, int viewH)
{
    float t;
    float b;
    float l;
    float r;

    if (!D3D_gi_AspectTrace)
        return;
    D3D_GetBandFractions(&t, &b, &l, &r);
    D3D_OutputDebugString(
        "[aspect] %s back %dx%d rect %dx%d view %d,%d %dx%d "
        "YoverX %.4f bands t%.4f b%.4f l%.4f r%.4f cin %d\n",
        where,
        (int)D3DRenderParams.presentParams.BackBufferWidth,
        (int)D3DRenderParams.presentParams.BackBufferHeight,
        (int)(D3DRenderParams.Rect.right - D3DRenderParams.Rect.left),
        (int)(D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top),
        viewX, viewY, viewW, viewH,
        D3D_RealYoverX(), t, b, l, r, CIN_Bands);
}

void D3D_SetProjectionMatrix(tDDCamStruct* a1)
{
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    double v6;
    double v7;
    int v8;
    int v9;
    int v10;
    int v11;
    BOOL v12; 
    double v13;
    double v14;
    double v16;
    double v19;
    double v20;
    double v22;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    int v43; 
    float v44; 
    int v45;
    float v46;
    float v47;
    int a3; 
    int v49;
    int v50;
    int v51;
    D3DMATRIX v52;
    D3DMATRIX v53;

    D3DRenderParams.projectionMatrix = &a1->projectionMtx;
    v1 = gpst_DX_stDD->ddcs2.selectedScreenRatio;
    if (v1 <= 0 || v1 >= 4)
        v46 = gpst_DX_stDD->ddcs2.manualScreenRatio;
    else
        v46 = GDI_gaf_ScreenRatio[v1];
    if (D3D_gi_Widescreen)
        v46 = D3D_RealYoverX();
    v2 = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
    v3 = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
    v40 = gpst_DX_stDD->ddcs2.pixelYoverX * a1->YoverX * v46;
    v43 = gpst_DX_stDD->ddcs2.flags2;
    a3 = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
    v50 = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
    if ((v43 & 4) != 0)
    {
        v4 = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
        v51 = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
        v5 = (uint32_t)((double)v50 / v40);
        v45 = v5;
        if ((v43 & 1) != 0 && v5 > v2 || (v43 & 2) != 0 && v5 < v2)
        {
            v4 = (uint32_t)((double)a3 * v40);
            v5 = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
        LABEL_16:
            v45 = v5;
            v51 = v4;
            goto LABEL_17;
        }
    }
    else
    {
        v4 = (uint32_t)((double)a3 * v40);
        v5 = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
        v51 = v4;
        v45 = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
        if ((v43 & 1) != 0 && v4 > v3 || (v43 & 2) != 0 && v4 < v3)
        {
            v5 = (uint32_t)((double)v50 / v40);
            v4 = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
            goto LABEL_16;
        }
    }
LABEL_17:
    if (a1->viewPortSizeWidth == 0.0)
    {
        a1->viewPortSizeWidth = 1.0;
        a1->viewPortSizeHeight = 1.0;
        a1->viewPortPosX = 0.0;
        a1->viewPortPosY = 0.0;
    }
    v6 = (double)v45;
    v7 = (double)v51;
    v8 = (int32_t)((double)((v2 - v5) / 2) + v6 * a1->viewPortPosX);
    v9 = (int32_t)((double)((v3 - v4) / 2) + v7 * a1->viewPortPosY);
    v10 = (uint32_t)(v7 * a1->viewPortSizeHeight);
    v49 = (uint32_t)(v6 * a1->viewPortSizeWidth);
    v11 = a1->flags;
    v12 = (a1->flags & 0x400) == 0;
    a1->viewPortRealPosX = v8;
    a1->viewPortRealPosY = v9;
    if (!v12)
    {
        v44 = (float)v49;
        v47 = (float)(0.5 / v44);
        D3DXMatrixTranslation(&v52, v47, v47, 0.0);
        v31 = (float)v10;
        if (D3D_gi_Pillarbox2D)
        {
            // The flat layer is laid out across the width reported by
            // D3D_FlatLayerWidth, which on a wide window is narrower
            // than the window. Shift it so that width sits in the
            // middle, at the size it was drawn.
            float pad = (v44 - (float)D3D_FlatLayerWidth(v49, v10)) * 0.5f;

            D3DXMatrixOrthoOffCenterRH(&v53, -pad, v44 - pad, 0.0, v31, 0.0, 1.0);
        }
        else
        {
            D3DXMatrixOrthoOffCenterRH(&v53, 0.0, v44, 0.0, v31, 0.0, 1.0);
        }
        D3DXMatrixMultiply((D3DMATRIX*)D3DRenderParams.projectionMatrix, &v53, &v52);
        memcpy(&d3dProjectionMtx, D3DRenderParams.projectionMatrix, sizeof(D3DMATRIX));
        D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, (D3DMATRIX*)(D3DRenderParams.projectionMatrix));
        return;
    }
    if ((v11 & 1) != 0)
    {
        v41 = (float)(1.0 / tan(a1->focale * 0.5));
        MATH_SetIdentityMatrix(D3DRenderParams.projectionMatrix);
        if (D3D_gi_Widescreen)
        {
            // the authored vertical half angle, kept whatever the
            // window shape is; the horizontal one follows v46
            float cotY = ((gpst_DX_stDD->ddcs2.flags2 & 4) != 0)
                             ? v41
                             : (float)(v41 / D3D_gf_RefYoverX);

            D3DRenderParams.projectionMatrix->v1.x = (float)(cotY * v46);
            v13 = -cotY;
        }
        else if ((gpst_DX_stDD->ddcs2.flags2 & 4) != 0)
        {
            D3DRenderParams.projectionMatrix->v1.x = v41 * v46;
            v13 = -v41;
        }
        else
        {
            D3DRenderParams.projectionMatrix->v1.x = v41;
            v13 = -(v41 / v46);
        }
        D3DRenderParams.projectionMatrix->v2.y = (float)v13;
        D3DRenderParams.projectionMatrix->v3.z = (a1->nearPlane + a1->farPlane) / (a1->farPlane - a1->nearPlane);
        D3DRenderParams.projectionMatrix->_m34 = 1.0;
        D3DRenderParams.projectionMatrix->Pos.z = -0.050000001f;
        D3DRenderParams.projectionMatrix->w = 0.0;
        memcpy(&d3dProjectionMtx, D3DRenderParams.projectionMatrix, sizeof(D3DMATRIX));
        D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, (D3DMATRIX*)(D3DRenderParams.projectionMatrix));
        goto LABEL_34;
    }
    MATH_SetIdentityMatrix(D3DRenderParams.projectionMatrix);
    v42 = a1->isoZoom * a1->isoFactor;
    if ((gpst_DX_stDD->ddcs2.flags2 & 4) == 0)
    {
        v20 = D3D_gi_Widescreen ? D3D_RealYoverX()
                                : gpst_DX_stDD->ddcs2.manualScreenRatio;
        v22 = -a1->farPlane;
        if (a1->isoFactor == 0.0) 
        {
            v39 = (float)v22;
            v36 = -a1->nearPlane;
            v33 = (float)((v20 + 1.0) * 0.5);
            v30 = (float)((1.0 - v20) * 0.5);
            D3D_CalcAndSetProjection(0.0, 1.0, v30, v33, v36, v39);
            goto LABEL_34;
        }
        v38 = (float)v22;
        v35 = a1->farPlane;
        v32 = (float)(-v20 * v42);
        v29 = (float)(v42 * v20);
        v28 = a1->isoZoom * a1->isoFactor;
        v19 = v42 * -1.0;
    LABEL_33:
        v26 = (float)v19;
        D3D_CalcAndSetProjection(v26, v28, v29, v32, v35, v38);
        goto LABEL_34;
    }
    v14 = 1.0 / (D3D_gi_Widescreen ? D3D_RealYoverX()
                                   : gpst_DX_stDD->ddcs2.manualScreenRatio);
    v16 = -a1->farPlane;
    if (a1->isoFactor != 0.0)     
    {
        v38 = (float)v16;
        v35 = a1->farPlane;
        v32 = a1->isoZoom * a1->isoFactor;
        v29 = (float)(v42 * -1.0);
        v28 = (float)(v42 * v14);
        v19 = -v14 * v42;
        goto LABEL_33;
    }
    v37 = (float)v16;
    v34 = -a1->nearPlane;
    v27 = (float)((v14 + 1.0) * 0.5);
    v25 = (float)((1.0 - v14) * 0.5);
    D3D_CalcAndSetProjection(v25, v27, 0.0, 1.0, v34, v37);
LABEL_34:
    D3D_SetViewPort(v8, v9, v49, v10);
    D3D_AspectTrace("proj", v8, v9, v49, v10);
}


void D3D_GetSnapShot(tD3dRequestSnapshot* a2)
{
    tD3dRequestSnapshot* v1;
    int v2;
    int v3;
    int v4;
    char* v5;
    char* v6;
    int v7;
    int v8;
    int v9;
    char* i;
    char v11;
    char* v12;
    char* v13;
    IDirect3DSurface9* v14;
    IDirect3DSurface9* a2a;
    D3DLOCKED_RECT v15;
    RECT v16;
    D3DSURFACE_DESC v17;

    v1 = a2;
    if ((a2->flags & 0xff) == 0 && ((a2->flags >> 8) & 0xff) != 2)
    {
        v2 = a2->height;
        v3 = a2->cutSizeFromTopAndBottom + a2->width;
        v16.left = a2->cutSizeFromTopAndBottom;
        v4 = a2->cutSizeFromSides + v2;
        v16.top = a2->cutSizeFromSides;
        v16.right = v3;
        v16.bottom = v4;
        if (D3DRenderParams.d3dNeedToCallEndScene)
        {
            D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
            D3DRenderParams.d3dNeedToCallEndScene = 0;
        }
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &v14);
        v14->lpVtbl->GetDesc(v14, &v17);
        
        D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(D3DRenderParams.d3dDev, v17.Width, v17.Height, v17.Format, D3DPOOL_SYSTEMMEM, &a2a, 0);

        D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev, v14, a2a);
        a2a->lpVtbl->LockRect(a2a, &v15, &v16, 0);
        v5 = (char*)v15.pBits;
        v6 = v1->srcData;
        if (v1->height)
        {
            v7 = v1->width;
            v8 = v1->height;
            do
            {
                v9 = 0;
                for (i = v5; v9 < v7; ++v9)
                {
                    *v6 = *i;
                    v11 = i[1];
                    v12 = v6 + 1;
                    v13 = i + 1;
                    *v12++ = v11;
                    *v12 = v13[1];
                    v7 = v1->width;
                    v6 = v12 + 1;
                    i = v13 + 3;
                }
                v5 += v15.Pitch;
                --v8;
            } while (v8);
        }
        a2a->lpVtbl->UnlockRect(a2a);
        a2a->lpVtbl->Release(a2a);
        v14->lpVtbl->Release(v14);
        if (!D3DRenderParams.d3dNeedToCallEndScene)
        {
            D3DRenderParams.d3dDev->lpVtbl->BeginScene(D3DRenderParams.d3dDev);
            D3DRenderParams.d3dNeedToCallEndScene = 1;
        }
    }
}

struct sGdi_D3DRenderParams* D3D_pst_CreateDevice()
{
    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    D3DRenderParams.d3d_if = Direct3DCreate9(D3D9b_SDK_VERSION);
    return &D3DRenderParams;
}

void D3D_DestroyDevice(struct sGdi_D3DRenderParams* a1)
{
    if (a1 == &D3DRenderParams)
    {
        D3DRenderParams.d3d_if->lpVtbl->Release(D3DRenderParams.d3d_if);
        memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    }
}

int D3D_SetViewMatrix(Mtx4x4* a1)
{
    Mtx4x4 a1a;

    MATH_MakeOGLMatrix(&a1a, a1);
    memcpy(&d3dTransformMatrix, &a1a, sizeof(d3dTransformMatrix));
    return D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&a1a);
}

void D3D_TextureStore(tTexUploadRequest* a1)
{
    tTexUploadRequest* v1;
    int v2;
    tEngTexListElemStruct* v3;
    IDirect3DTexture9* v4;
    int v5;
    IDirect3DSurface9* surface;
    RECT v7;
    IDirect3DTexture9* a1a;


    v1 = a1;
    v2 = a1->idx;
    v3 = &TEX_gst_GlobalList.texElems[v2];
    v4 = D3DRenderParams.d3dTextures[v2];
    if (v4)
    {
        a1a = v4;
    }
    else
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, v3->normalData.width, v3->normalData.height, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &a1a, 0);
        v4 = a1a;
    }
    v5 = (int)v1->dataPtr;
    v4->lpVtbl->GetSurfaceLevel(v4, 0, &surface);
    v7.top = 0;
    v7.left = 0;
    v7.right = v3->normalData.width;
    v7.bottom = v3->normalData.height;
    D3DXLoadSurfaceFromMemory(surface, NULL, NULL, (void*)v5, D3DFMT_A8R8G8B8, 4 * v3->normalData.width, NULL, &v7, D3DX_FILTER_POINT, 0);
    D3DXFilterTexture((IDirect3DBaseTexture9*)a1a, NULL, 0, D3DX_FILTER_LINEAR);
    surface->lpVtbl->Release(surface);
    D3DRenderParams.d3dTextures[v1->idx] = a1a;
}

void D3D_ReleaseTextures(struct sGdi_DisplayData* a1)
{
    IDirect3DTexture9** v0;
    IDirect3DTexture9** v1;
    IDirect3DTexture9** v2;
    IDirect3DTexture9** v3;
    IDirect3DTexture9** v4;

    v0 = D3DRenderParams.d3dTextures;
    if (D3DRenderParams.d3dTextures)
    {
        v1 = D3DRenderParams.d3dTextures;
        v2 = &D3DRenderParams.d3dTextures[D3DRenderParams.d3dTextureNum];
        if (D3DRenderParams.d3dTextures < v2)
        {
            do
            {
                if (*v1)
                {
                    (*v1)->lpVtbl->Release(*v1);
                    *v1 = NULL;
                }
                ++v1;
            } while (v1 < v2);
            v0 = D3DRenderParams.d3dTextures;
        }
    }
    v3 = D3DRenderParams.d3dSourceTextures;
    v4 = &D3DRenderParams.d3dSourceTextures[D3DRenderParams.d3dTextureNum];
    if (D3DRenderParams.d3dSourceTextures < v4)
    {
        do
        {
            if (*v3)
            {
                (*v3)->lpVtbl->Release(*v3);
                *v3 = 0;
            }
            ++v3;
        } while (v3 < v4);
        v0 = D3DRenderParams.d3dTextures;
    }
    if (v0)
    {
        MEM_Free(v0);
        D3DRenderParams.d3dTextures = NULL;
    }
    if (D3DRenderParams.d3dSourceTextures)
    {
        MEM_Free(D3DRenderParams.d3dSourceTextures);
        D3DRenderParams.d3dSourceTextures = NULL;
    }
    D3DRenderParams.d3dTextureNum = 0;
}

void D3D_ReleaseShadowTextures()
{
    IDirect3DTexture9** v0;
    IDirect3DTexture9** v1;

    v0 = &D3DRenderParams.d3dShadowTextures[8];
    do
    {
        v1 = v0 - 1;
        --v0;
        if (*v1)
        {
            (*v1)->lpVtbl->Release(*v1);
            (*v1) = NULL;
        }
    } while (v0 != D3DRenderParams.d3dShadowTextures);
}

void D3D_RelaseBlurRenderTargetsAndOthers()
{
    if (D3DRenderParams.blurRenderTarget)
    {
        D3DRenderParams.blurRenderTarget->lpVtbl->Release(D3DRenderParams.blurRenderTarget);
        D3DRenderParams.blurRenderTarget = NULL;
    }
    if (D3DRenderParams.blurRenderTarget2)
    {
        D3DRenderParams.blurRenderTarget2->lpVtbl->Release(D3DRenderParams.blurRenderTarget2);
        D3DRenderParams.blurRenderTarget2 = NULL;
    }
    if (D3DRenderParams.unktex1_dword_B31988)
    {
        D3DRenderParams.unktex1_dword_B31988->lpVtbl->Release(D3DRenderParams.unktex1_dword_B31988);
        D3DRenderParams.unktex1_dword_B31988 = NULL;
    }
    if (D3DRenderParams.unktex2_dword_B3198C)
    {
        D3DRenderParams.unktex2_dword_B3198C->lpVtbl->Release(D3DRenderParams.unktex2_dword_B3198C);
        D3DRenderParams.unktex2_dword_B3198C = NULL;
    }
    if (D3DRenderParams.tmpRenderTarget)
    {
        D3DRenderParams.tmpRenderTarget->lpVtbl->Release(D3DRenderParams.tmpRenderTarget);
        D3DRenderParams.tmpRenderTarget = NULL;
    }
}

void D3D_ReleaseBackBufferAndDSSurace1()
{
    IDirect3DSurface9** v0;

    if (D3DRenderParams.pBackBuffers[0])
    {
        v0 = D3DRenderParams.pBackBuffers;
        do
        {
            (*v0)->lpVtbl->Release(*v0);
            *v0 = NULL;
            v0++;
        } while (v0 <= &D3DRenderParams.pBackBuffers[1]);

        D3DRenderParams.pDepthStencilSurface->lpVtbl->Release(D3DRenderParams.pDepthStencilSurface);
        D3DRenderParams.pDepthStencilSurface = NULL;
    }
}

void D3D_BlurTexRelease() // alloc @D3D_CreateFullScreenTextureAndGetDepthStencilSurface
{
    if (D3DRenderParams.blurTex)
    {
        D3DRenderParams.blurTex->lpVtbl->Release(D3DRenderParams.blurTex);
        D3DRenderParams.blurTex = NULL;
        D3DRenderParams.newZStencil->lpVtbl->Release(D3DRenderParams.newZStencil);
        D3DRenderParams.newZStencil = NULL;
    }
}

void D3D_ReleaseSourceAndDestTexture()
{
    if (D3DRenderParams.pDestTexture)
    {
        D3DRenderParams.pDestTexture->lpVtbl->Release(D3DRenderParams.pDestTexture);
        D3DRenderParams.pDestTexture = NULL;
    }
    if (D3DRenderParams.pSourceTexture)
    {
        D3DRenderParams.pSourceTexture->lpVtbl->Release(D3DRenderParams.pSourceTexture);
        D3DRenderParams.pSourceTexture = NULL;
    }
}

void D3D_ReleaseVBOs()
{
    if (D3DRenderParams.d3dXYZVB)
    {
        D3DRenderParams.d3dXYZVB->lpVtbl->Release(D3DRenderParams.d3dXYZVB);
        D3DRenderParams.d3dXYZVB = 0;
        D3DRenderParams.xyzFVF = 0;
        D3DRenderParams.stride2 = 0;
        D3DRenderParams.prevVertexElemIdx2 = 0;
        D3DRenderParams.startVertexElemIdx2 = 0;
        D3DRenderParams.currTargetGao = 0;
    }
    if (D3DRenderParams.d3dNormalsBuffer)
    {
        D3DRenderParams.d3dNormalsBuffer->lpVtbl->Release(D3DRenderParams.d3dNormalsBuffer);
        D3DRenderParams.d3dNormalsBuffer = 0;
        D3DRenderParams.normalsFVF = 0;
        D3DRenderParams.d3dStride = 0;
        D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = 0;
        D3DRenderParams.currVertexStartOffset = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    }
    if (D3DRenderParams.d3dColorsVB)
    {
        D3DRenderParams.d3dColorsVB->lpVtbl->Release(D3DRenderParams.d3dColorsVB);
        D3DRenderParams.d3dColorsVB = 0;
        D3DRenderParams.colorsFVF = 0;
        D3DRenderParams.stride = 0;
        D3DRenderParams.prevVertexElemIdx = 0;
        D3DRenderParams.startVertexElemIdx = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
    }
    if (VBOs[0])
    {
        VBOs[0]->lpVtbl->Release(VBOs[0]);
        VBOs[1]->lpVtbl->Release(VBOs[1]);
        VBOs[2]->lpVtbl->Release(VBOs[2]);
        VBOs[0] = NULL;
        VBOs[1] = NULL;
        VBOs[2] = NULL;
        d3dStreamCurrentOffset = 0;
    }
}

void D3D_ReleaseEverything()
{
    D3D_ReleaseShadowTextures();
    D3D_RelaseBlurRenderTargetsAndOthers();
    D3D_ReleaseBackBufferAndDSSurace1();
    D3D_BlurTexRelease();
    D3D_ReleaseSourceAndDestTexture();
    D3D_ReleaseVBOs();
    if (D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull)
    {
        D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull->lpVtbl->Release(D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
        D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull = NULL;
    }
    if (D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull)
    {
        D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull->lpVtbl->Release(D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
        D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull = NULL;
    }
    if (d3dWaterRTtoSet)
    {
        d3dWaterRTtoSet->lpVtbl->Release(d3dWaterRTtoSet);
        d3dWaterRTtoSet = NULL;
    }
    if (waterTexture)
    {
        waterTexture->lpVtbl->Release(waterTexture);
        waterTexture = NULL;
    }
    if (d3dDSSurfaceToSet)
    {
        d3dDSSurfaceToSet->lpVtbl->Release(d3dDSSurfaceToSet);
        d3dDSSurfaceToSet = NULL;
    }
}

int D3D_l_Close()
{
    D3D_ReleaseEverything();
    //nullsub_32();
    if (D3DRenderParams.d3dDev)
    {
        D3DRenderParams.d3dDev->lpVtbl->Release(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dDev->lpVtbl->Release(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dDev = NULL;
    }
    return 0;
}

void D3D_CheckReturnAndRunSettingsApplicationIfProblem(int a2)
{
    PROCESS_INFORMATION ProcessInformation;
    STARTUPINFOA StartupInfo;

    if (a2 < 0)
    {
        memset(&StartupInfo, 0, sizeof(StartupInfo));
        ProcessInformation.hProcess = 0;
        ProcessInformation.hThread = 0;
        ProcessInformation.dwProcessId = 0;
        ProcessInformation.dwThreadId = 0;
        CreateProcessA(NULL, (LPSTR)"SettingsApplication.exe -OUTOFVIDEOMEMORY",
            NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, ".", &StartupInfo, &ProcessInformation);
        D3D_l_Close();
        D3DRenderParams.d3d_if->lpVtbl->Release(D3DRenderParams.d3d_if);
        DestroyWindow(MAI_gh_MainWindow);
        exit(0);
    }
}

void D3D_SetVertexShader(IDirect3DVertexShader9* a1)
{
    if (renderStateConfig.vertexShader != a1)
    {
        renderStateConfig.vertexShader = a1;
        renderStateConfig.fvf = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
        D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, renderStateConfig.vertexShader);
    }
}

void D3D_SetInitialRenderStates()
{
    memset(&renderStateConfig, 0, sizeof(renderStateConfig));
    if (renderStateConfig.d3d_zEnable != D3DZB_TRUE)
    {
        renderStateConfig.d3d_zEnable = D3DZB_TRUE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DZB_TRUE);
    }
    if (renderStateConfig.d3d_zFunc != D3DCMP_LESSEQUAL)
    {
        renderStateConfig.d3d_zFunc = D3DCMP_LESSEQUAL;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    }
    if (renderStateConfig.d3d_zWriteEnable != TRUE)
    {
        renderStateConfig.d3d_zWriteEnable = TRUE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, TRUE);
    }
    if (renderStateConfig.d3dFogEnable)
    {
        renderStateConfig.d3dFogEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, FALSE);
    }
    if (renderStateConfig.d3dFogColor)
    {
        renderStateConfig.d3dFogColor = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGCOLOR, 0);
    }
    if (renderStateConfig.d3dFogTableMode)
    {
        renderStateConfig.d3dFogTableMode = D3DFOG_NONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    }
    if (renderStateConfig.d3dFogVertexMode)
    {
        renderStateConfig.d3dFogVertexMode = D3DFOG_NONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    }
    if (renderStateConfig.d3dFogDensity != 1.0f)
    {
        renderStateConfig.d3dFogDensity = 1.0f;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGDENSITY, *(DWORD*)(&(renderStateConfig.d3dFogDensity)));
    }
    if (renderStateConfig.d3dFogStart)
    {
        renderStateConfig.d3dFogStart = 0.0f;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGSTART, *(DWORD*)(&(renderStateConfig.d3dFogStart)));
    }
    if (renderStateConfig.d3dFogEnd != 1.0f)
    {
        renderStateConfig.d3dFogEnd = 1.0f;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGEND, *(DWORD*)(&(renderStateConfig.d3dFogEnd)));
    }
    if (renderStateConfig.d3dFillMode != D3DFILL_SOLID)
    {
        renderStateConfig.d3dFillMode = D3DFILL_SOLID;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_SOLID);
    }
    if (renderStateConfig.d3dCullMode != D3DCULL_CCW)
    {
        renderStateConfig.d3dCullMode = D3DCULL_CCW;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CULLMODE, D3DCULL_CCW);
    }
    if (renderStateConfig.d3dColorWriteEnable != 0xf)
    {
        renderStateConfig.d3dColorWriteEnable = 0xf;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, 0xf);
    }
    if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, 0);
    }
    if (renderStateConfig.d3dAlphaFunc != D3DCMP_ALWAYS)
    {
        renderStateConfig.d3dAlphaFunc = D3DCMP_ALWAYS;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    }
    if (renderStateConfig.d3dAlphaRef)
    {
        renderStateConfig.d3dAlphaRef = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAREF, 0);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    if (renderStateConfig.d3d_BlendOp != D3DBLENDOP_ADD)
    {
        renderStateConfig.d3d_BlendOp = D3DBLENDOP_ADD;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
    if (renderStateConfig.d3dSrcBlend != D3DBLEND_ONE)
    {
        renderStateConfig.d3dSrcBlend = D3DBLEND_ONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_ONE);
    }
    if (renderStateConfig.d3dDestBlend != D3DBLEND_ZERO)
    {
        renderStateConfig.d3dDestBlend = D3DBLEND_ZERO;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_ZERO);
    }
    if (renderStateConfig.d3d_Lighting != TRUE)
    {
        renderStateConfig.d3d_Lighting = TRUE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_LIGHTING, TRUE);
    }
    if (renderStateConfig.d3d_normalizeNormals)
    {
        renderStateConfig.d3d_normalizeNormals = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_NORMALIZENORMALS, FALSE);
    }
    if (renderStateConfig.d3dTextureFactor != 0xffffffff)
    {
        renderStateConfig.d3dTextureFactor = 0xffffffff;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, 0xffffffff);
    }
    if (renderStateConfig.d3dClipPlaneEnable)
    {
        renderStateConfig.d3dClipPlaneEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CLIPPLANEENABLE, 0);
    }
    if (renderStateConfig.fvf)
    {
        renderStateConfig.fvf = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, NULL);
    }
    D3D_SetVertexShader(0);
    renderStateConfig.vertexShader = 0;
    renderStateConfig.fvf = 0;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
}

void D3D_SetInitialSamplerStates()
{
    unsigned int v0;
    int v1; 
    IDirect3DDevice9* v2;
    IDirect3DDevice9* v3;
    IDirect3DDevice9* v4;
    IDirect3DDevice9* v5;
    IDirect3DDevice9* v6;
    IDirect3DDevice9* v7;

    v0 = 0;
    if (D3DRenderParams.d3dcaps.MaxTextureBlendStages)
    {
        v1 = 0;
        do
        {
            if (samplerConfig[v1].d3d_addressu != D3DTADDRESS_WRAP)
            {
                v2 = D3DRenderParams.d3dDev;
                samplerConfig[v1].d3d_addressu = D3DTADDRESS_WRAP;
                v2->lpVtbl->SetSamplerState(v2, v0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            }
            if (samplerConfig[v1].d3d_addressv != D3DTADDRESS_WRAP)
            {
                v3 = D3DRenderParams.d3dDev;
                samplerConfig[v1].d3d_addressv = D3DTADDRESS_WRAP;
                v3->lpVtbl->SetSamplerState(v3, v0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            }
            if (samplerConfig[v1].d3d_minfilter)
            {
                v4 = D3DRenderParams.d3dDev;
                samplerConfig[v1].d3d_minfilter = D3DTEXF_NONE;
                v4->lpVtbl->SetSamplerState(v4, v0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[v1].d3d_magfilter)
            {
                v5 = D3DRenderParams.d3dDev;
                samplerConfig[v1].d3d_magfilter = D3DTEXF_NONE;
                v5->lpVtbl->SetSamplerState(v5, v0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[v1].d3d_mipfilter)
            {
                v6 = D3DRenderParams.d3dDev;
                samplerConfig[v1].d3d_mipfilter = D3DTEXF_NONE;
                v6->lpVtbl->SetSamplerState(v6, v0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[v1].d3d_mipmaplodbias != 0.0f)
            {
                v7 = D3DRenderParams.d3dDev;
                samplerConfig[v1].d3d_mipmaplodbias = 0.0f;
                v7->lpVtbl->SetSamplerState(v7, v0, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&(samplerConfig[v1].d3d_mipmaplodbias));
            }
            ++v0;
            ++v1;
        } while (v0 < D3DRenderParams.d3dcaps.MaxTextureBlendStages);
    }
}

void D3D_SetInitialTextureStageStates()
{
    unsigned int v0;
    int v1;
    BOOL v2;
    IDirect3DDevice9* v3; 
    IDirect3DDevice9* v4; 
    IDirect3DDevice9* v5; 
    IDirect3DDevice9* v6; 
    IDirect3DDevice9* v7; 
    IDirect3DDevice9* v8; 
    IDirect3DDevice9* v9; 
    IDirect3DDevice9* v10;
    IDirect3DDevice9* v11;
    IDirect3DDevice9* v12;
    IDirect3DDevice9* v13;
    IDirect3DDevice9* v14;
    IDirect3DDevice9* v15;
    IDirect3DDevice9* v16;
    IDirect3DDevice9* v17;
    IDirect3DDevice9* v18;
    IDirect3DDevice9* v19;
    IDirect3DDevice9* v20;

    v0 = 0;
    if (D3DRenderParams.d3dcaps.MaxTextureBlendStages)
    {
        v1 = 0;
        do
        {
            v2 = textureStageTypeConfig[v1].d3d_colorOp == D3DTOP_DISABLE;
            textureStageTypeConfig[v1].d3dTextureID = -1;
            if (!v2)
            {
                v3 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_colorOp = D3DTOP_DISABLE;
                v3->lpVtbl->SetTextureStageState(v3, v0, D3DTSS_COLOROP, D3DTOP_DISABLE);
            }
            if (textureStageTypeConfig[v1].d3d_colorarg1 != D3DTA_CURRENT)
            {
                v4 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_colorarg1 = D3DTA_CURRENT;
                v4->lpVtbl->SetTextureStageState(v4, v0, D3DTSS_COLORARG1, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_colorarg2 != D3DTA_CURRENT)
            {
                v5 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_colorarg2 = D3DTA_CURRENT;
                v5->lpVtbl->SetTextureStageState(v5, v0, D3DTSS_COLORARG2, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_alphaOp != D3DTOP_DISABLE)
            {
                v6 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_alphaOp = D3DTOP_DISABLE;
                v6->lpVtbl->SetTextureStageState(v6, v0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            }
            if (textureStageTypeConfig[v1].d3d_alphaArg1 != D3DTA_CURRENT)
            {
                v7 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_alphaArg1 = D3DTA_CURRENT;
                v7->lpVtbl->SetTextureStageState(v7, v0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_alphaArg2 != D3DTA_CURRENT)
            {
                v8 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_alphaArg2 = D3DTA_CURRENT;
                v8->lpVtbl->SetTextureStageState(v8, v0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_bumpenvmat00 != 0.0f)
            {
                v9 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_bumpenvmat00 = 0.0f;
                v9->lpVtbl->SetTextureStageState(v9, v0, D3DTSS_BUMPENVMAT00, *(DWORD*)&(textureStageTypeConfig[v1].d3d_bumpenvmat00));
            }
            if (textureStageTypeConfig[v1].d3d_bumpenvmat01 != 0.0f)
            {
                v10 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_bumpenvmat01 = 0.0f;
                v10->lpVtbl->SetTextureStageState(v10, v0, D3DTSS_BUMPENVMAT01, *(DWORD*)&(textureStageTypeConfig[v1].d3d_bumpenvmat01));
            }
            if (textureStageTypeConfig[v1].d3d_bumpenvmat10 != 0.0f)
            {
                v11 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_bumpenvmat10 = 0.0f;
                v11->lpVtbl->SetTextureStageState(v11, v0, D3DTSS_BUMPENVMAT10, *(DWORD*)&(textureStageTypeConfig[v1].d3d_bumpenvmat10));
            }
            if (textureStageTypeConfig[v1].d3d_bumpenvmat11 != 0.0f)
            {
                v12 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_bumpenvmat11 = 0.0f;
                v12->lpVtbl->SetTextureStageState(v12, v0, D3DTSS_BUMPENVMAT11, *(DWORD*)&(textureStageTypeConfig[v1].d3d_bumpenvmat11));
            }
            if (textureStageTypeConfig[v1].d3dTexCoordIndex != v0)
            {
                v13 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3dTexCoordIndex = v0;
                v13->lpVtbl->SetTextureStageState(v13, v0, D3DTSS_TEXCOORDINDEX, v0);
            }
            if (textureStageTypeConfig[v1].d3d_bumpEnvLScale != 0.0)
            {
                v14 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_bumpEnvLScale = 0.0f;
                v14->lpVtbl->SetTextureStageState(v14, v0, D3DTSS_BUMPENVLSCALE, *(DWORD*)&(textureStageTypeConfig[v1].d3d_bumpEnvLScale));
            }
            if (textureStageTypeConfig[v1].d3d_bumpEnvLOffset != 0.0)
            {
                v15 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_bumpEnvLOffset = 0.0f;
                v15->lpVtbl->SetTextureStageState(v15, v0, D3DTSS_BUMPENVLOFFSET, *(DWORD*)&(textureStageTypeConfig[v1].d3d_bumpEnvLOffset));
            }
            if (textureStageTypeConfig[v1].d3dTextureTransformFlags)
            {
                v16 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3dTextureTransformFlags = 0;
                v16->lpVtbl->SetTextureStageState(v16, v0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
            }
            if (textureStageTypeConfig[v1].d3d_colorArg0 != D3DTA_CURRENT)
            {
                v17 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_colorArg0 = D3DTA_CURRENT;
                v17->lpVtbl->SetTextureStageState(v17, v0, D3DTSS_COLORARG0, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_alphaArg0 != D3DTA_CURRENT)
            {
                v18 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_alphaArg0 = D3DTA_CURRENT;
                v18->lpVtbl->SetTextureStageState(v18, v0, D3DTSS_ALPHAARG0, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_resultArg0 != D3DTA_CURRENT)
            {
                v19 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_resultArg0 = D3DTA_CURRENT;
                v19->lpVtbl->SetTextureStageState(v19, v0, D3DTSS_RESULTARG, D3DTA_CURRENT);
            }
            if (textureStageTypeConfig[v1].d3d_constant)
            {
                v20 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v1].d3d_constant = 0;
                v20->lpVtbl->SetTextureStageState(v20, v0, D3DTSS_CONSTANT, 0);
            }
            ++v0;
            ++v1;
        } while (v0 < D3DRenderParams.d3dcaps.MaxTextureBlendStages);
    }
}

void D3D_RastersCreateFont()
{
    D3DXFONT_DESCA v0;

    v0.Width = 0;
    v0.Italic = 0;
    v0.CharSet = 0;
    v0.OutputPrecision = 0;
    v0.Quality = 0;
    v0.PitchAndFamily = 0;
    memset(v0.FaceName, 0, sizeof(v0.FaceName));
    strcpy(v0.FaceName, "Arial");
    v0.Height = 10;
    v0.Weight = 400;
    v0.MipLevels = 1;
    D3DXCreateFontIndirectA(D3DRenderParams.d3dDev, &v0, &d3dxfontIF);
}

void D3D_RastersDrawText(char* a1, int a2, int a3, D3DCOLOR a4)
{
    ID3DXFont* v4;
    RECT v5;

    v5.left = a2;
    v5.right = 10000;
    v5.bottom = 10000;
    v4 = d3dxfontIF;
    v5.top = a3;
    if (!d3dxfontIF)
    {
        D3D_RastersCreateFont();
        v4 = d3dxfontIF;
    }
    v4->lpVtbl->DrawTextA(v4, NULL, a1, -1, &v5, 0, a4);
}

void D3D_RastersDrawUPStream()
{
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLELIST,
        ((rastersDrawPrimitiveUPDataEnd - rastersDrawPrimitiveUPDataStart) >> 4) / 3,
        rastersDrawPrimitiveUPDataStart,
        16); // 16 = Vec3 + color (int), see D3DRenderParams.fvf
    rastersDrawPrimitiveUPDataEnd = rastersDrawPrimitiveUPDataStart;
}

void D3D_SaveSamplerState(tD3dSamplerConfigStruct* a1)
{
    memcpy(a1, samplerConfig, sizeof(samplerConfig));
}

void D3D_SaveTextureStage(tD3dTextureStageTypeStruct* a1)
{
    memcpy(a1, textureStageTypeConfig, sizeof(textureStageTypeConfig));
}

void D3D_SaveRenderState(tD3dRenderStateConfigStruct *a1)
{
    memcpy(a1, &renderStateConfig, sizeof(tD3dRenderStateConfigStruct));
}

void D3D_SaveEverythingToBackup(int a1)
{
    if (a1 != 3)
    {
        D3D_SaveSamplerState(samplerStateBackup);
        D3D_SaveTextureStage(texturestageBackup);
    }
    if (a1 != 2)
        D3D_SaveRenderState(&renderstateBackup);
    d3dparamsSaveType = a1;
}

void D3D_RenderStateRestore(tD3dRenderStateConfigStruct* a1)
{
    int v1;

    if (renderStateConfig.d3d_zEnable != a1->d3d_zEnable)
    {
        renderStateConfig.d3d_zEnable = a1->d3d_zEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, renderStateConfig.d3d_zEnable);
    }
    if (renderStateConfig.d3d_zFunc != a1->d3d_zFunc)
    {
        renderStateConfig.d3d_zFunc = a1->d3d_zFunc;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZFUNC, renderStateConfig.d3d_zFunc);
    }
    if (renderStateConfig.d3d_zWriteEnable != a1->d3d_zWriteEnable)
    {
        renderStateConfig.d3d_zWriteEnable = a1->d3d_zWriteEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, renderStateConfig.d3d_zWriteEnable);
    }
    if (renderStateConfig.d3dFogEnable != a1->d3dFogEnable)
    {
        renderStateConfig.d3dFogEnable = a1->d3dFogEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, renderStateConfig.d3dFogEnable);
    }
    if (renderStateConfig.d3dFogColor != a1->d3dFogColor)
    {
        renderStateConfig.d3dFogColor = a1->d3dFogColor;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGCOLOR, renderStateConfig.d3dFogColor);
    }
    if (renderStateConfig.d3dFogTableMode != a1->d3dFogTableMode)
    {
        renderStateConfig.d3dFogTableMode = a1->d3dFogTableMode;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGTABLEMODE, renderStateConfig.d3dFogTableMode);
    }
    if (renderStateConfig.d3dFogVertexMode != a1->d3dFogVertexMode)
    {
        renderStateConfig.d3dFogVertexMode = a1->d3dFogVertexMode;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGVERTEXMODE, renderStateConfig.d3dFogVertexMode);
    }
    if (renderStateConfig.d3dFogDensity != a1->d3dFogDensity)
    {
        renderStateConfig.d3dFogDensity = a1->d3dFogDensity;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGDENSITY, *(DWORD*)&(renderStateConfig.d3dFogDensity));
    }
    if (renderStateConfig.d3dFogStart != a1->d3dFogStart)
    {
        renderStateConfig.d3dFogStart = a1->d3dFogStart;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGSTART, *(DWORD*)&(renderStateConfig.d3dFogStart));
    }
    if (renderStateConfig.d3dFogEnd != a1->d3dFogEnd)
    {
        renderStateConfig.d3dFogEnd = a1->d3dFogEnd;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGEND, *(DWORD*)&(renderStateConfig.d3dFogEnd));
    }
    if (renderStateConfig.d3dFillMode != a1->d3dFillMode)
    {
        renderStateConfig.d3dFillMode = a1->d3dFillMode;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, renderStateConfig.d3dFillMode);
    }
    if (renderStateConfig.d3dCullMode != a1->d3dCullMode)
    {
        renderStateConfig.d3dCullMode = a1->d3dCullMode;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CULLMODE, renderStateConfig.d3dCullMode);
    }
    if (renderStateConfig.d3dColorWriteEnable != a1->d3dColorWriteEnable)
    {
        renderStateConfig.d3dColorWriteEnable = a1->d3dColorWriteEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, renderStateConfig.d3dColorWriteEnable);
    }
    if (renderStateConfig.d3dAlphaTest != a1->d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = a1->d3dAlphaTest;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, renderStateConfig.d3dAlphaTest);
    }
    if (renderStateConfig.d3dAlphaFunc != a1->d3dAlphaFunc)
    {
        renderStateConfig.d3dAlphaFunc = a1->d3dAlphaFunc;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAFUNC, renderStateConfig.d3dAlphaFunc);
    }
    if (renderStateConfig.d3dAlphaRef != a1->d3dAlphaRef)
    {
        renderStateConfig.d3dAlphaRef = a1->d3dAlphaRef;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAREF, renderStateConfig.d3dAlphaRef);
    }
    if (renderStateConfig.d3dAlphaBlendEnable != a1->d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = a1->d3dAlphaBlendEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, renderStateConfig.d3dAlphaBlendEnable);
    }
    if (renderStateConfig.d3d_BlendOp != a1->d3d_BlendOp)
    {
        renderStateConfig.d3d_BlendOp = a1->d3d_BlendOp;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_BLENDOP, renderStateConfig.d3d_BlendOp);
    }
    if (renderStateConfig.d3dSrcBlend != a1->d3dSrcBlend)
    {
        renderStateConfig.d3dSrcBlend = a1->d3dSrcBlend;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, renderStateConfig.d3dSrcBlend);
    }
    if (renderStateConfig.d3dDestBlend != a1->d3dDestBlend)
    {
        renderStateConfig.d3dDestBlend = a1->d3dDestBlend;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, renderStateConfig.d3dDestBlend);
    }
    if (renderStateConfig.d3d_Lighting != a1->d3d_Lighting)
    {
        renderStateConfig.d3d_Lighting = a1->d3d_Lighting;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_LIGHTING, renderStateConfig.d3d_Lighting);
    }
    if (renderStateConfig.d3d_normalizeNormals != a1->d3d_normalizeNormals)
    {
        renderStateConfig.d3d_normalizeNormals = a1->d3d_normalizeNormals;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_NORMALIZENORMALS, renderStateConfig.d3d_normalizeNormals);
    }
    if (renderStateConfig.d3dTextureFactor != a1->d3dTextureFactor)
    {
        renderStateConfig.d3dTextureFactor = a1->d3dTextureFactor;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, renderStateConfig.d3dTextureFactor);
    }
    if (renderStateConfig.d3dClipPlaneEnable != a1->d3dClipPlaneEnable)
    {
        renderStateConfig.d3dClipPlaneEnable = a1->d3dClipPlaneEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CLIPPLANEENABLE, renderStateConfig.d3dClipPlaneEnable);
    }
    if (a1->vertexShader)
    {
        renderStateConfig.vertexShader = NULL;
        renderStateConfig.fvf = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
        D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
        if (renderStateConfig.fvf != a1->fvf)
        {
            renderStateConfig.fvf = a1->fvf;
            D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, (IDirect3DVertexDeclaration9 *)(renderStateConfig.fvf)); // TODO FIXME: ez micsoda gányolás de az IDA ezt mutatja, játékban ez van
        }
        D3D_SetVertexShader(a1->vertexShader);
    }
    else
    {
        if (renderStateConfig.fvf)
        {
            renderStateConfig.fvf = 0;
            D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, NULL);
        }
        D3D_SetVertexShader(0);
        v1 = a1->fvf;
        renderStateConfig.vertexShader = NULL;
        renderStateConfig.fvf = v1;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
        D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    }
}

void D3D_SamplerStateRestore(tD3dSamplerConfigStruct* a1)
{
    unsigned int v1;
    tD3dSamplerConfigStruct* v3;
    int v4;
    int v5;
    IDirect3DDevice9* v6;
    int v7;
    IDirect3DDevice9* v8;
    int v9;
    int v10;
    IDirect3DDevice9* v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;

    v1 = 0;
    if (D3DRenderParams.d3dcaps.MaxTextureBlendStages)
    {
        v3 = a1;
        v4 = 0;
        do
        {
            v5 = v3->d3d_addressu;
            if (samplerConfig[v4].d3d_addressu != v5)
            {
                v6 = D3DRenderParams.d3dDev;
                v12 = v3->d3d_addressu;
                samplerConfig[v4].d3d_addressu = v5;
                v6->lpVtbl->SetSamplerState(v6, v1, D3DSAMP_ADDRESSU, v12);
            }
            v7 = v3->d3d_addressv;
            if (samplerConfig[v4].d3d_addressv != v7)
            {
                v8 = D3DRenderParams.d3dDev;
                v13 =v3->d3d_addressv;
                samplerConfig[v4].d3d_addressv = v7;
                v8->lpVtbl->SetSamplerState(v8, v1, D3DSAMP_ADDRESSV, v13);
            }
            if (samplerConfig[v4].d3d_minfilter != v3->d3d_minfilter)
            {
                v14 = v3->d3d_minfilter;
                samplerConfig[v4].d3d_minfilter = v3->d3d_minfilter;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, v1, D3DSAMP_MINFILTER, v14);
            }
            v9 = v3->d3d_magfilter;
            if (samplerConfig[v4].d3d_magfilter != v9)
            {
                v15 = v3->d3d_magfilter;
                samplerConfig[v4].d3d_magfilter = v9;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, v1, D3DSAMP_MAGFILTER, v15);
            }
            v10 = v3->d3d_mipfilter;
            if (samplerConfig[v4].d3d_mipfilter != v10)
            {
                v16 = v3->d3d_mipfilter;
                samplerConfig[v4].d3d_mipfilter = v10;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, v1, D3DSAMP_MIPFILTER, v16);
            }
            if (v3->d3d_mipmaplodbias != samplerConfig[v4].d3d_mipmaplodbias)
            {
                v11 = D3DRenderParams.d3dDev;
                samplerConfig[v4].d3d_mipmaplodbias = v3->d3d_mipmaplodbias;
                v11->lpVtbl->SetSamplerState(v11, v1, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&(samplerConfig[v4].d3d_mipmaplodbias));
            }
            ++v1;
            ++v4;
            ++v3;
        } while (v1 < D3DRenderParams.d3dcaps.MaxTextureBlendStages);
    }
}

void D3D_SetTexture(int textureState, int textureID)
{
    IDirect3DTexture9* d3dTexturePtr;

    if (textureID == -2)
    {
        textureStageTypeConfig[textureState].d3dTextureID = -2;
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, textureState, (IDirect3DBaseTexture9*)D3DRenderParams.pDestTexture);
    }
    if (D3DRenderParams.d3dTextures)
    {
        if (textureID != textureStageTypeConfig[textureState].d3dTextureID)
        {
            if (textureID != -1 && textureID != -2)
            {
                if (textureID >= 0) {
                    d3dTexturePtr = D3DRenderParams.d3dTextures[textureID];
                } 
                else {
                    d3dTexturePtr = D3DRenderParams.d3dShadowTextures[(textureID & ~0x80000000)];
                }
                D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, textureState, (IDirect3DBaseTexture9*)d3dTexturePtr);
            }
            textureStageTypeConfig[textureState].d3dTextureID = textureID;
        }
    }
}

void D3D_TextureStageStateRestore(tD3dTextureStageTypeStruct* a1)
{
    unsigned int v1;
    int v4;
   
    v1 = 0;
    if (D3DRenderParams.d3dcaps.MaxTextureBlendStages)
    {
        v4 = 0;
        do
        {
            textureStageTypeConfig[v4].d3dTextureID = a1[v4].d3dTextureID;
            D3D_SetTexture(v1, a1[v4].d3dTextureID);
            if (textureStageTypeConfig[v4].d3d_colorOp != a1[v4].d3d_colorOp)
            {
                textureStageTypeConfig[v4].d3d_colorOp = a1[v4].d3d_colorOp;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, 1, a1[v4].d3d_colorOp);
            }
            if (textureStageTypeConfig[v4].d3d_colorarg1 != a1[v4].d3d_colorarg1)
            {
                textureStageTypeConfig[v4].d3d_colorarg1 = a1[v4].d3d_colorarg1;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_COLORARG1, a1[v4].d3d_colorarg1);
            }
            if (textureStageTypeConfig[v4].d3d_colorarg2 != a1[v4].d3d_colorarg2)
            {
                textureStageTypeConfig[v4].d3d_colorarg2 = a1[v4].d3d_colorarg2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_COLORARG2, a1[v4].d3d_colorarg2);
            }
            if (textureStageTypeConfig[v4].d3d_alphaOp != a1[v4].d3d_alphaOp)
            {
                textureStageTypeConfig[v4].d3d_alphaOp = a1[v4].d3d_alphaOp;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_ALPHAOP, a1[v4].d3d_alphaOp);
            }
            if (textureStageTypeConfig[v4].d3d_alphaArg1 != a1[v4].d3d_alphaArg1)
            {
                textureStageTypeConfig[v4].d3d_alphaArg1 = a1[v4].d3d_alphaArg1;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_ALPHAARG1, a1[v4].d3d_alphaArg1);
            }
            if (textureStageTypeConfig[v4].d3d_alphaArg2 != a1[v4].d3d_alphaArg2)
            {
                textureStageTypeConfig[v4].d3d_alphaArg2 = a1[v4].d3d_alphaArg2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_ALPHAARG2, a1[v4].d3d_alphaArg2);
            }
            if (textureStageTypeConfig[v4].d3d_bumpenvmat00 != a1[v4].d3d_bumpenvmat00)
            {
                textureStageTypeConfig[v4].d3d_bumpenvmat00 = a1[v4].d3d_bumpenvmat00;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_BUMPENVMAT00, *(DWORD*)&(a1[v4].d3d_bumpenvmat00));
            }
            if (textureStageTypeConfig[v4].d3d_bumpenvmat01 != a1[v4].d3d_bumpenvmat01)
            {
                textureStageTypeConfig[v4].d3d_bumpenvmat01 = a1[v4].d3d_bumpenvmat01;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_BUMPENVMAT01, *(DWORD*)&(a1[v4].d3d_bumpenvmat01));
            }
            if (textureStageTypeConfig[v4].d3d_bumpenvmat10 != a1[v4].d3d_bumpenvmat10)
            {
                textureStageTypeConfig[v4].d3d_bumpenvmat10 = a1[v4].d3d_bumpenvmat10;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_BUMPENVMAT10, *(DWORD*)&(a1[v4].d3d_bumpenvmat10));
            }
            if (textureStageTypeConfig[v4].d3d_bumpenvmat11 != a1[v4].d3d_bumpenvmat11)
            {
                textureStageTypeConfig[v4].d3d_bumpenvmat11 = a1[v4].d3d_bumpenvmat11;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_BUMPENVMAT11, *(DWORD*)&(a1[v4].d3d_bumpenvmat11));
            }
            if (textureStageTypeConfig[v4].d3dTexCoordIndex != a1[v4].d3dTexCoordIndex)
            {
                textureStageTypeConfig[v4].d3dTexCoordIndex = a1[v4].d3dTexCoordIndex;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_TEXCOORDINDEX, a1[v4].d3dTexCoordIndex);
            }
            if (textureStageTypeConfig[v4].d3d_bumpEnvLScale != a1[v4].d3d_bumpEnvLScale)
            {
                textureStageTypeConfig[v4].d3d_bumpEnvLScale = a1[v4].d3d_bumpEnvLScale;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_BUMPENVLSCALE, *(DWORD*)&(a1[v4].d3d_bumpEnvLScale));
            }
            if (textureStageTypeConfig[v4].d3d_bumpEnvLOffset != a1[v4].d3d_bumpEnvLOffset)
            {
                textureStageTypeConfig[v4].d3d_bumpEnvLOffset = a1[v4].d3d_bumpEnvLOffset;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_BUMPENVLOFFSET, *(DWORD*)&(a1[v4].d3d_bumpEnvLOffset));
            }
            if (textureStageTypeConfig[v4].d3dTextureTransformFlags != a1[v4].d3dTextureTransformFlags)
            {
                textureStageTypeConfig[v4].d3dTextureTransformFlags = a1[v4].d3dTextureTransformFlags;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_TEXTURETRANSFORMFLAGS, a1[v4].d3dTextureTransformFlags);
            }
            if (textureStageTypeConfig[v4].d3d_colorArg0 != a1[v4].d3d_colorArg0)
            {
                textureStageTypeConfig[v4].d3d_colorArg0 = a1[v4].d3d_colorArg0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_COLORARG0, a1[v4].d3d_colorArg0);
            }
            if (textureStageTypeConfig[v4].d3d_alphaArg0 != a1[v4].d3d_alphaArg0)
            {
                textureStageTypeConfig[v4].d3d_alphaArg0 = a1[v4].d3d_alphaArg0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_ALPHAARG0, a1[v4].d3d_alphaArg0);
            }
            if (textureStageTypeConfig[v4].d3d_resultArg0 != a1[v4].d3d_resultArg0)
            {
                textureStageTypeConfig[v4].d3d_resultArg0 = a1[v4].d3d_resultArg0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_RESULTARG, a1[v4].d3d_resultArg0);
            }
            if (textureStageTypeConfig[v4].d3d_constant != a1[v4].d3d_constant)
            {
                textureStageTypeConfig[v4].d3d_constant = a1[v4].d3d_constant;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v1, D3DTSS_CONSTANT, a1[v4].d3d_constant);
            }
            ++v1;
            ++v4;
        } while (v1 < D3DRenderParams.d3dcaps.MaxTextureBlendStages);
    }
}

void D3D_RestoreEverythingFromBackup()
{
    if (d3dparamsSaveType != 3)
    {
        D3D_SamplerStateRestore(samplerStateBackup);
        D3D_TextureStageStateRestore(texturestageBackup);
    }
    if (d3dparamsSaveType != 2)
        D3D_RenderStateRestore(&renderstateBackup);
}

void D3D_RastersSetAndDrawAQuad(int a1, float a2, float a3, float a4, float a5, int a6)
{
    tVertexAndColor* v6;
    char* v7;
    double v8;
    double v9;

    v6 = (tVertexAndColor*)rastersDrawPrimitiveUPDataEnd;
    if (!rastersDrawPrimitiveUPDataEnd)
    {
        v7 = (char*)MEM_p_Alloc(16384u);
        memset(v7, 0, 16384u);
        rastersDrawPrimitiveUPDataStart = v7;
        v6 = (tVertexAndColor*)v7;
    }

    v8 = a2 + a4;
    v9 = a3 + a5;

    v6[0].v.x = a2;
    v6[0].v.y = a3;
    v6[0].color = a1;

    v6[1].v.x = (float)v8;
    v6[1].v.y = a3;
    v6[1].color = a1;    
    
    v6[2].v.x = a2;
    v6[2].v.y = (float)v9;
    v6[2].color = a1;

    v6[3].v.x = a2;
    v6[3].v.y = (float)v9;
    v6[3].color = a1;

    v6[4].v.x = (float)v8;
    v6[4].v.y = a3;
    v6[4].color = a1;

    v6[5].v.x = (float)v8;
    v6[5].v.y = (float)v9;
    v6[5].color = a1;
    //rastersDrawPrimitiveUPDataEnd += sizeof(tVertexAndColor) * 6;
    rastersDrawPrimitiveUPDataEnd = (char *)(v6 + 6); // a vége az 6 db tVertexAndColor elemet fog tartalmazni, mivel a v6 az egy tVertexAndColor típusra mutató pointer
    if (a6)
        D3D_RastersDrawUPStream();
}

void D3D_DrawARaster(int color, char* a1, float a2, float a3, float a4)
{
    double v7;
    float v8;
    char Buffer[32];
    float a3a;
    float a4a;

    sprintf(Buffer, "%5.3f", a2);
    D3D_RastersDrawText(a1, 20, (unsigned int)a3, -1);
    D3D_RastersDrawText(Buffer, (unsigned int)(a4 - 50.0), (unsigned int)a3, -1);
    if (a2 > 4.0)
        a2 = 4.0;
    v8 = 70.0;
    for (a4a = (float)((a4 - 140.0) * 0.75294119); a2 > 0.0; a4a = (float)(a4a * 0.25))
    {
        if (a2 >= 1.0)
            v7 = 1.0;
        else
            v7 = a2;
        a3a = (float)(v7 * a4a);
        D3D_RastersSetAndDrawAQuad(color, v8, a3, a3a, 10.0, 0);
        color ^= 0xFFFFu;
        v8 = a3a + v8;
        a2 = (float)(a2 - 1.0);
    }
}

void Rasters_Update()
{
    double v1;
    unsigned int i;
    unsigned int j;
    int v5;
    int* v6;
    int v7;
    unsigned int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;
    int v17;
    int v18;
    int v19;
    double v20;
    unsigned int v21;
    int v22;
    int v23;
    int v24;
    int v25;
    int v26;
    int v27;
    int v28;
    int v29;
    int v30;
    int v31;
    float v32;
    int v33;
    int v34;
    int v35;
    int v36;
    double v37;
    float v38;
    float r;
    float v40;
    float a3;
    LARGE_INTEGER b;
    float a2;
    LARGE_INTEGER Frequency;
    LARGE_INTEGER PerformanceCount;
    D3DMATRIX a1;
    int v47[54];
    float width;

    width = (float)D3DRenderParams.d3dViewPort.Width;
    r = (float)D3DRenderParams.d3dViewPort.Height;

    if (tweak_perf)
    {
        PRO_QPC_RecordTimeDecRef(53);
        PRO_QPC_RecordTimeDecRef(19);
        QueryPerformanceCounter(&PerfCounterStruct[47]);
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&PerformanceCount);
        v1 = (double)(PerformanceCount.QuadPart - qword_990B48);
        qword_990B48 = PerformanceCount.QuadPart;
        a2 = (float)((double)Frequency.QuadPart / v1);
        if (tweak_perf != 3 || a2 > (double)tweak_perf_fps_min)
        {
            D3D_SaveEverythingToBackup(1);
            a1._43 = 0.0;
            a1._42 = 0.0;
            a1._41 = 0.0;
            a1._34 = 0.0;
            a1._32 = 0.0;
            a1._31 = 0.0;
            a1._24 = 0.0;
            a1._23 = 0.0;
            a1._21 = 0.0;
            a1._14 = 0.0;
            a1._13 = 0.0;
            a1._12 = 0.0;
            a1._44 = 1.0;
            a1._33 = 1.0;
            a1._22 = 1.0;
            a1._11 = 1.0;
            memcpy(&d3dTransformMatrix, &a1, sizeof(d3dTransformMatrix));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, 2, &a1);
            D3DXMatrixOrthoOffCenterRH(&a1, 0.0, width, r, 0.0, -1.0, 1.0);
            memcpy(&d3dProjectionMtx, &a1, sizeof(d3dProjectionMtx));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, 3, &a1);
            D3D_SetVertexShader(0);
            renderStateConfig.vertexShader = 0;
            renderStateConfig.fvf = D3DFVF_DIFFUSE | D3DFVF_XYZ; 
            D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
            D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
            if (renderStateConfig.d3d_zEnable)
            {
                renderStateConfig.d3d_zEnable = D3DZB_FALSE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DZB_FALSE);
            }
            if (renderStateConfig.d3dCullMode != D3DCULL_NONE)
            {
                renderStateConfig.d3dCullMode = D3DCULL_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CULLMODE, D3DCULL_NONE);
            }
            if (renderStateConfig.d3d_Lighting)
            {
                renderStateConfig.d3d_Lighting = FALSE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_LIGHTING, FALSE);
            }
            if (renderStateConfig.d3dColorWriteEnable != 0xf)
            {
                renderStateConfig.d3dColorWriteEnable = 0xf;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, 0xf);
            }
            if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
            {
                renderStateConfig.d3dAlphaBlendEnable = TRUE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
            }
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_SRCALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_SRCALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            }
            if (renderStateConfig.d3dDestBlend != D3DBLEND_INVSRCALPHA)
            {
                renderStateConfig.d3dDestBlend = D3DBLEND_INVSRCALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            }
            D3D_SetTexture(0, -1);
            D3D_SetTexture(1, -1);
            D3D_SetTexture(2, -1);
            D3D_SetTexture(3, -1);
            if (a2 > (double)tweak_perf_fps_min)
                D3D_RastersSetAndDrawAQuad(0xFFFF0000, 0.0, 0.0, width, 40.0, 0);
            if (tweak_perf != 3)
            {
                for (i = 0; i < (6 * 9); i += 9) // 6 * 9 = 54
                {
                    rastersUnkTimeFloats_dbl_990B58[i] = (double)PerfCountertimeSumArray[i] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 1] = (double)PerfCountertimeSumArray[i + 1] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 1] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 2] = (double)PerfCountertimeSumArray[i + 2] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 2] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 3] = (double)PerfCountertimeSumArray[i + 3] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 3] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 4] = (double)PerfCountertimeSumArray[i + 4] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 4] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 5] = (double)PerfCountertimeSumArray[i + 5] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 5] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 6] = (double)PerfCountertimeSumArray[i + 6] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 6] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 7] = (double)PerfCountertimeSumArray[i + 7] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 7] * 0.95;
                    rastersUnkTimeFloats_dbl_990B58[i + 8] = (double)PerfCountertimeSumArray[i + 8] * 0.05 + rastersUnkTimeFloats_dbl_990B58[i + 8] * 0.95;
                }
                for (j = 0; j < 54; ++j)
                    v47[j] = j;
                v5 = 0;
                v6 = v47;
                v7 = 53;
                do
                {
                    v8 = v5 + 1;
                    v9 = v5 + 1;
                    if (v7 >= 4)
                    {
                        do
                        {
                            v10 = *v6;
                            v11 = v47[v9];
                            if (rastersUnkTimeFloats_dbl_990B58[*v6] < rastersUnkTimeFloats_dbl_990B58[v11])
                            {
                                *v6 = v11;
                                v47[v9] = v10;
                            }
                            v12 = *v6;
                            v13 = v47[v9 + 1];
                            if (rastersUnkTimeFloats_dbl_990B58[*v6] < rastersUnkTimeFloats_dbl_990B58[v13])
                            {
                                *v6 = v13;
                                v47[v9 + 1] = v12;
                            }
                            v14 = *v6;
                            v15 = v47[v9 + 2];
                            if (rastersUnkTimeFloats_dbl_990B58[*v6] < rastersUnkTimeFloats_dbl_990B58[v15])
                            {
                                *v6 = v15;
                                v47[v9 + 2] = v14;
                            }
                            v16 = *v6;
                            v17 = v47[v9 + 3];
                            if (rastersUnkTimeFloats_dbl_990B58[*v6] < rastersUnkTimeFloats_dbl_990B58[v17])
                            {
                                *v6 = v17;
                                v47[v9 + 3] = v16;
                            }
                            v9 += 4;
                        } while (v9 < 51);
                    }
                    for (; (unsigned int)v9 < 54; ++v9)
                    {
                        v18 = *v6;
                        v19 = v47[v9];
                        if (rastersUnkTimeFloats_dbl_990B58[*v6] < rastersUnkTimeFloats_dbl_990B58[v19])
                        {
                            *v6 = v19;
                            v47[v9] = v18;
                        }
                    }
                    ++v5;
                    --v7;
                    ++v6;
                } while (v8 < 53);
                v20 = 70.0;
                v21 = 0;
                v40 = (float)(tweak_perf_fps_target / (double)Frequency.QuadPart);
                do
                {
                    v22 = v47[v21];
                    if (rasterNames[v22].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v22] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                                break;
                        }
                    }
                    v23 = v47[v21 + 1];
                    if (rasterNames[v23].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v23] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                ++v21;
                                break;
                            }
                        }
                    }
                    v24 = v47[v21 + 2];
                    if (rasterNames[v24].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v24] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 2;
                                break;
                            }
                        }
                    }
                    v25 = v47[v21 + 3];
                    if (rasterNames[v25].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v25] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 3;
                                break;
                            }
                        }
                    }
                    v26 = v47[v21 + 4];
                    if (rasterNames[v26].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v26] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 4;
                                break;
                            }
                        }
                    }
                    v27 = v47[v21 + 5];
                    if (rasterNames[v27].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v27] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 5;
                                break;
                            }
                        }
                    }
                    v28 = v47[v21 + 6];
                    if (rasterNames[v28].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v28] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 6;
                                break;
                            }
                        }
                    }
                    v29 = v47[v21 + 7];
                    if (rasterNames[v29].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v29] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 7;
                                break;
                            }
                        }
                    }
                    v30 = v47[v21 + 8];
                    if (rasterNames[v30].unk1)
                    {
                        if ((double)PerfCountertimeSumArray[v30] * v40 > 0.01)
                        {
                            v20 = v20 + 15.0;
                            if (v20 > r)
                            {
                                v21 += 8;
                                break;
                            }
                        }
                    }
                    v21 += 9;
                } while (v21 < 54);
                v31 = 1;
                if (tweak_perf == 1)
                    v31 = v21;
                v32 = width;
                v38 = (float)((double)v31 * 15.0);
                D3D_RastersSetAndDrawAQuad(0x80000000, 0.0, 50.0, width, v38, 1);
                //nullsub_32();
                v33 = 0;
                for (a3 = 70.0; v33 < v31; ++v33)
                {
                    v34 = v47[v33];
                    v35 = rasterNames[v34].unk1;
                    width = (float)((double)PerfCountertimeSumArray[v34] * v40);
                    if (v35 && width > 0.01)
                    {
                        v36 = 0xF0808020;
                        if ((((byte)v34 - 1) & 1) != 0)
                            v36 = 0xF0808060;
                        if ((((byte)v34 + 1) & 2) != 0)
                            v36 |= 0x4000u;
                        if ((((byte)v34 + 1) & 4) != 0)
                            v36 |= 0x400000u;
                        D3D_DrawARaster(v36, rasterNames[v34].name, width, a3, v32);
                        v37 = a3 + 15.0;
                        a3 = (float)v37;
                        if (v37 > r)
                            break;
                    }
                }
                D3D_DrawARaster(0xFFFFFF, "Frame Rate", a2, 50.0, v32);
                //nullsub_32();
            }
            D3D_RastersDrawUPStream();
            D3D_RestoreEverythingFromBackup();
        }
        memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
        QueryPerformanceCounter(&b);
        if (PerfCounterStruct[47].QuadPart)
            PerfCountertimeSumArray[47] += b.QuadPart - PerfCounterStruct[47].QuadPart;
        PerfCounterStruct[47].QuadPart = 0i64;
        QueryPerformanceCounter(&PerfCounterStruct[19]);
        QueryPerformanceCounter(&PerfCounterStruct[53]);
    }
}

void D3D_CycleThroughVBOs()
{
    d3dStreamCurrentOffset = 0;
    currentVBOSelected = (currentVBOSelected + 1) % 3u;
}

void PC_FreeABackCache(struct sGEOGRO* a1)
{
    IDirect3DVertexBuffer9* v1;
    IDirect3DVertexBuffer9* v2;
    //int v3;
    int i;
    IDirect3DVertexBuffer9** v5;
    IDirect3DVertexBuffer9* v6;
    IDirect3DIndexBuffer9* v7;

    v1 = a1->d3dvb;
    if (v1)
    {
        v1->lpVtbl->Release(a1->d3dvb);
        a1->d3dvb = NULL;
    }
    v2 = a1->d3dvb2;
    if (v2)
    {
        v2->lpVtbl->Release(a1->d3dvb2);
        a1->d3dvb2 = NULL;
    }
    /* TODO FIXME
    v3 = *(_DWORD*)a1->gap4b;
    if (v3)
    {
        (*(void(__stdcall**)(_DWORD))(*(_DWORD*)v3 + 8))(*(_DWORD*)a1->gap4b);
        *(_DWORD*)a1->gap4b = 0;
    }
    */
    if (a1->d3dUVVB)
    {
        for (i = 0; i < a1->d3dUVVBNum; ++i)
        {
            v5 = a1->d3dUVVB;
            v6 = v5[i];
            if (v6)
            {
                v6->lpVtbl->Release(v5[i]);
                a1->d3dUVVB[i] = NULL;
            }
        }
        MEM_Free(a1->d3dUVVB);
        a1->d3dUVVB = NULL;
    }
    v7 = a1->d3dIndexBuf;
    if (v7)
    {
        v7->lpVtbl->Release(a1->d3dIndexBuf);
        a1->d3dIndexBuf = NULL;
    }
}

void D3D_ReleaseMeshSpecificData(tGameObjectMain* result)
{
    tGameObjectVisualObject* v1;
    tGEOGRO* v2;
    tGEOSTATICLOD* v2b;
    int v3;
    tGEOGRO** v4;
    tGEOGRO* v5;

    if ((result->flags & 0x4000) != 0)
    {
        v1 = result->basic->actorObjectOrVisualObject;
        v2 = (tGEOGRO*)v1->GROGEOOrSomethingElse;
        v2b = (tGEOSTATICLOD*)v1->GROGEOOrSomethingElse;
        if (v1->GROGEOOrSomethingElse)
        {
            if (v2->intf->id == 1)
            {
                if (v2->triangleChunkCount && v2->triangleChunks && v2->vertexNum)
                    PC_FreeABackCache((tGEOGRO*)v1->GROGEOOrSomethingElse);
            }
            else if (v2b->intf->id == 8)
            {
                v3 = 0;
                if (v2b->lodNum)
                {
                    v4 = v2b->geo;
                    do
                    {
                        v5 = *v4;
                        if ((*v4)->triangleChunkCount && v5->triangleChunks)
                        {
                            if (v5->vertexNum)
                                PC_FreeABackCache(*v4);
                        }
                        ++v3;
                        ++v4;
                    } while (v3 < v2b->lodNum);
                }
            }
        }
    }
}

void D3D_ReleaseMeshSpecificDataOfAWorld(tWorld* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;
    tGameObjectMain* v3;
    tGroup* v4;
    tableDescStruct* v5;
    tableElemStructPF* v6;
    tableElemStructPF* j;
    tGameObjectMain* v8;

    if (a1)
    {
        if (a1->objectTablePF.actElemNum)
        {
            v1 = a1->objectTablePF.firstElemPtr;
            for (i = ((tableElemStructPF*)(a1->objectTablePF.nextElemPtr)) - 1; v1 <= i; ++v1)
            {
                v3 = (tGameObjectMain*)v1->objectPtr;
                if (v1->objectPtr)
                {
                    if (((uint8_t)v3 & 3) != 1)
                    {
                        D3D_ReleaseMeshSpecificData((tGameObjectMain*)v1->objectPtr);
                        if ((v3->flags & 2) != 0)
                        {
                            v4 = ((tGameObjectActorObject*)(v3->basic->actorObjectOrVisualObject))->skeletonGroup;
                            if (v4)
                            {
                                v5 = v4->tablePF;
                                v6 = v5->firstElemPtr;
                                for (j = ((tableElemStructPF*)(v5->nextElemPtr)) - 1; v6 <= j; ++v6)
                                {
                                    v8 = (tGameObjectMain*)v6->objectPtr;
                                    if (v6->objectPtr)
                                    {
                                        if (((uint8_t)v8 & 3) != 1)
                                            D3D_ReleaseMeshSpecificData(v8);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void D3D_ReleaseWaterIndices()
{
    if (waterStreamData)
    {
        waterStreamData->lpVtbl->Release(waterStreamData);
        waterStreamData = NULL;
        waterIndexData->lpVtbl->Release(waterIndexData);
        waterIndexData = NULL;
    }
}

void D3D_ReleaseSunRenderTargetAndTexture(struct sGfxSunD3DSpecificData* a1)
{
    IDirect3DTexture9* v1;

    if (a1->d3dTexture)
    {
        a1->d3dRenderTarget->lpVtbl->Release(a1->d3dRenderTarget);
        v1 = a1->d3dTexture;
        a1->d3dRenderTarget = NULL;
        v1->lpVtbl->Release(v1);
        a1->d3dTexture = NULL;
    }
}

void D3D_ReleaseSunSurfaces()
{
    tGfxSunD3DSpecificData** v0;

    v0 = gD3dSunSurfaces;
    do
    {
        if (*v0)
            D3D_ReleaseSunRenderTargetAndTexture(*v0);
        ++v0;
    } while (v0 <= &(gD3dSunSurfaces[15]));
}

void D3D_ReleaseEveryCache()
{
    IDirect3DTexture9** v0;
    IDirect3DTexture9** v1;

    D3D_ReleaseWaterIndices();
    D3D_ReleaseSunSurfaces();
    D3D_ReleaseEverything();
    D3D_ReleaseMeshSpecificDataOfAWorld(WOR_gpst_CurrentWorld);
    v0 = D3DRenderParams.d3dTextures;
    if (D3DRenderParams.d3dTextures)
    {
        v1 = &D3DRenderParams.d3dTextures[D3DRenderParams.d3dTextureNum];
        if (D3DRenderParams.d3dTextures < v1)
        {
            do
            {
                if (*v0)
                    (*v0)->lpVtbl->Release(*v0);
                ++v0;
            } while (v0 < v1);
            v0 = D3DRenderParams.d3dTextures;
        }
        MEM_Free(v0);
        D3DRenderParams.d3dTextures = NULL;
    }
}

int D3D_CalculateStride(unsigned int a1)
{
    int result;
    unsigned int v2;
    unsigned int v3;
    int v4;

    result = 0;
    switch (a1 & 0x400E)
    {
    case 2u:
        v4 = 12;
        break;
    case 4u:
    case 6u:
        v4 = 16;
        break;
    case 8u:
        v4 = 20;
        break;
    case 0xAu:
        v4 = 24;
        break;
    case 0xCu:
        v4 = 28;
        break;
    case 0xEu:
        v4 = 32;
        break;
    default:
        goto LABEL_15;
    }
    result = v4;
LABEL_15:
    if ((a1 & 0x10) != 0)
        result += 12;
    if ((a1 & 0x20) != 0)
        result += 4;
    if ((a1 & 0x40) != 0)
        result += 4;
    if ((a1 & 0x80u) != 0)
        result += 4;
    v2 = (a1 >> 8) & 0xF;
    v3 = (a1 >> 16) & 0xffff;
    if (v3)
    {
        for (; v2; --v2)
        {
            if ((v3 & 3) != 0)
            {
                switch (v3 & 3)
                {
                case 1u:
                    result += 12;
                    break;
                case 2u:
                    result += 16;
                    break;
                case 3u:
                    result += 4;
                    break;
                }
            }
            else
            {
                result += 8;
            }
            v3 >>= 2;
        }
    }
    else
    {
        result += 8 * v2;
    }
    return result;
}

void D3D_CreateThreeVbos()
{
    int v0;
    int v1;
    int v2;

    v0 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        2097152,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &VBOs[0],
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v0);
    v1 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        2097152,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &VBOs[1],
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v1);
    v2 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        2097152,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &VBOs[2],
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v2);
}

void D3D_CreateVBOs()
{
    int v0;
    int v1;
    int v2;

    D3D_CreateThreeVbos();
    D3DRenderParams.prevVertexElemIdx2 = 0;
    D3DRenderParams.startVertexElemIdx2 = 0;
    D3DRenderParams.currTargetGao = 0;
    D3DRenderParams.xyzFVF = D3DFVF_XYZ;
    D3DRenderParams.stride2 = D3D_CalculateStride(D3DFVF_XYZ);
    v0 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        65535 * D3DRenderParams.stride2,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.xyzFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dXYZVB,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v0);
    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = 0;
    D3DRenderParams.currVertexStartOffset = 0;
    D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    D3DRenderParams.normalsFVF = D3DFVF_NORMAL;
    D3DRenderParams.d3dStride = D3D_CalculateStride(D3DFVF_NORMAL);
    v1 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        65535 * D3DRenderParams.d3dStride,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.normalsFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dNormalsBuffer,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v1);
    D3DRenderParams.prevVertexElemIdx = 0;
    D3DRenderParams.startVertexElemIdx = 0;
    D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
    D3DRenderParams.colorsFVF = D3DFVF_DIFFUSE;
    D3DRenderParams.stride = D3D_CalculateStride(D3DFVF_DIFFUSE);
    v2 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        262143 * D3DRenderParams.stride,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.colorsFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dColorsVB,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v2);
}

void D3D_SetColorWriteEnable(char a1)
{
    int v1;

    v1 = 0;
    if ((a1 & 0x20) == 0)
        v1 = D3DCOLORWRITEENABLE_ALPHA;
    if ((a1 & 0x40) == 0)
        v1 |= (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
    if (renderStateConfig.d3dColorWriteEnable != v1)
    {
        renderStateConfig.d3dColorWriteEnable = v1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, v1);
    }
}

void D3D_SetCullingParams()
{
    int v0;
    BOOL v1;
    int v2;
    int v3;

    v0 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    if ((v0 & 0x800) != 0)
    {
        v1 = (v0 & 0x2000) == 0;
        v2 = renderStateConfig.d3dCullMode;
        if (v1)
            v3 = D3DCULL_CCW;
        else
            v3 = D3DCULL_CW;
    }
    else
    {
        v2 = renderStateConfig.d3dCullMode;
        v3 = 1;
    }
    if (v2 != v3)
    {
        renderStateConfig.d3dCullMode = v3;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CULLMODE, v3);
    }
}

void D3D_SetTextureStageStates(int a1, int a2)
{
    IDirect3DDevice9* v2;
    IDirect3DDevice9* v3;
    IDirect3DDevice9* v4;
    IDirect3DDevice9* v5;
    int v6;
    int v7;
    IDirect3DDevice9* v8;
    IDirect3DDevice9* v9;
    IDirect3DDevice9* v10;
    IDirect3DDevice9* v11;
    IDirect3DDevice9* v12;
    IDirect3DDevice9* v13;
    IDirect3DDevice9* v14;

    if (a2 == -1)
    {
        if (textureStageTypeConfig[a1].d3d_colorarg1)
        { 
            v2 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[a1].d3d_colorarg1 = D3DTA_DIFFUSE;
            v2->lpVtbl->SetTextureStageState(v2, a1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        }
        if (textureStageTypeConfig[a1].d3d_colorOp != D3DTOP_SELECTARG1)
        {
            v3 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[a1].d3d_colorOp = D3DTOP_SELECTARG1;
            v3->lpVtbl->SetTextureStageState(v3, a1, 1, D3DTOP_SELECTARG1);
        }
        if (textureStageTypeConfig[a1].d3d_alphaArg1)
        {
            v4 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[a1].d3d_alphaArg1 = D3DTA_DIFFUSE;
            v4->lpVtbl->SetTextureStageState(v4, a1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        }
        if (textureStageTypeConfig[a1].d3d_alphaOp != D3DTOP_SELECTARG1)
        {
            v5 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[a1].d3d_alphaOp = D3DTOP_SELECTARG1;
            v5->lpVtbl->SetTextureStageState(v5, a1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        }
    }
    else if (a2 != -2)
    {
        if (D3DRenderParams.tweak_ae_d2x && D3DRenderParams.diffuse2x && a2 >= 0)
        {
            v6 = a1;
            v7 = a1;
            if (textureStageTypeConfig[a1].d3d_colorOp != D3DTOP_MODULATE2X)
            {
                v8 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[a1].d3d_colorOp = D3DTOP_MODULATE2X;
                v8->lpVtbl->SetTextureStageState(v8, a1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
            }
        }
        else
        {
            v6 = a1;
            v7 = a1;
            if (textureStageTypeConfig[a1].d3d_colorOp != D3DTOP_MODULATE)
            {
                v9 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[a1].d3d_colorOp = D3DTOP_MODULATE;
                v9->lpVtbl->SetTextureStageState(v9, a1, D3DTSS_COLOROP, D3DTOP_MODULATE);
            }
        }
        if (textureStageTypeConfig[v7].d3d_alphaOp != D3DTOP_MODULATE)
        {
            v10 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v7].d3d_alphaOp = D3DTOP_MODULATE;
            v10->lpVtbl->SetTextureStageState(v10, v6, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        }
        if (textureStageTypeConfig[v7].d3d_colorarg1 != D3DTA_TEXTURE)
        {
            v11 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v7].d3d_colorarg1 = D3DTA_TEXTURE;
            v11->lpVtbl->SetTextureStageState(v11, v6, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        }
        if (textureStageTypeConfig[v7].d3d_colorarg2)
        {
            v12 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v7].d3d_colorarg2 = D3DTA_DIFFUSE;
            v12->lpVtbl->SetTextureStageState(v12, v6, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        }
        if (textureStageTypeConfig[v7].d3d_alphaArg1 != D3DTA_TEXTURE)
        {
            v13 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v7].d3d_alphaArg1 = D3DTA_TEXTURE;
            v13->lpVtbl->SetTextureStageState(v13, v6, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        }
        if (textureStageTypeConfig[v7].d3d_alphaArg2)
        {
            v14 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v7].d3d_alphaArg2 = D3DTA_DIFFUSE;
            v14->lpVtbl->SetTextureStageState(v14, v6, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        }
    }
}

void D3D_SetAlphaTestParams(unsigned int a1, int16_t a2)
{
    int v2;

    if ((a2 & 0x10) != 0)
    {
        if (renderStateConfig.d3dAlphaTest != 1)
        {
            renderStateConfig.d3dAlphaTest = 1;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, TRUE);
        }
        v2 = D3DCMP_LESS;
        if ((a2 & 0x80u) == 0)
            v2 = D3DCMP_GREATER;
        if (renderStateConfig.d3dAlphaFunc != v2)
        {
            renderStateConfig.d3dAlphaFunc = v2;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAFUNC, v2);
        }
        if (renderStateConfig.d3dAlphaRef != ((a1 >> 22) & 0xFC))
        {
            renderStateConfig.d3dAlphaRef = (a1 >> 22) & 0xFC;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAREF, (a1 >> 22) & 0xFC);
        }
    }
    else if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, FALSE);
    }
    D3DRenderParams.colorMaskAlpha = (a2 & 0x400) != 0 ? 0xFF000000 : 0;
}

void D3D_SetAlphaBlendingOptions(unsigned int a1, char a2)
{
    int v2;
    int v3;

    d3d_alphablendIsCase3 = 0;
    switch (((a1 >> 16) & 0xffff) & 0xF)
    {
    case 0:
    case 7:
        if (renderStateConfig.d3dAlphaBlendEnable)
        {
            renderStateConfig.d3dAlphaBlendEnable = FALSE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
        }
        if (renderStateConfig.d3dSrcBlend != D3DBLEND_ONE)
        {
            renderStateConfig.d3dSrcBlend = D3DBLEND_ONE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_ONE);
        }
        if (renderStateConfig.d3dDestBlend != D3DBLEND_ZERO)
        {
            renderStateConfig.d3dDestBlend = D3DBLEND_ZERO;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_ZERO);
        }
        return;
    case 1:
        if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
        {
            renderStateConfig.d3dAlphaBlendEnable = TRUE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
        }
        if (a2 >= 0)
        {
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_SRCALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_SRCALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            }
            v2 = 6;
        }
        else
        {
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_INVSRCALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_INVSRCALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
            }
            v2 = 5;
        }
        goto LABEL_30;
    case 2:
        if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
        {
            renderStateConfig.d3dAlphaBlendEnable = TRUE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
        }
        if (renderStateConfig.d3dSrcBlend != D3DBLEND_ONE)
        {
            renderStateConfig.d3dSrcBlend = D3DBLEND_ONE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_ONE);
        }
        v3 = renderStateConfig.d3dDestBlend;
        if (a2 >= 0)
            v2 = D3DBLEND_INVSRCALPHA;
        else
            v2 = D3DBLEND_SRCALPHA;
        goto LABEL_31;
    case 3:
        d3d_alphablendIsCase3 = 1;
        if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
        {
            renderStateConfig.d3dAlphaBlendEnable = TRUE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
        }
        if (a2 >= 0)
        {
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_DESTALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_DESTALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
            }
            if (renderStateConfig.d3dDestBlend != D3DBLEND_INVDESTALPHA)
            {
                renderStateConfig.d3dDestBlend = D3DBLEND_INVDESTALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
            }
        }
        else
        {
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_INVDESTALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_INVDESTALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
            }
            v2 = 7;
        LABEL_30:
            v3 = renderStateConfig.d3dDestBlend;
        LABEL_31:
            if (v3 != v2)
            {
                renderStateConfig.d3dDestBlend = v2;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, v2);
            }
        }
        return;
    case 4:
        if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
        {
            renderStateConfig.d3dAlphaBlendEnable = TRUE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
        }
        if (a2 >= 0)
        {
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_DESTALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_DESTALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
            }
            if (renderStateConfig.d3dDestBlend != D3DBLEND_ONE)
                goto LABEL_53;
        }
        else
        {
            if (renderStateConfig.d3dSrcBlend != D3DBLEND_INVDESTALPHA)
            {
                renderStateConfig.d3dSrcBlend = D3DBLEND_INVDESTALPHA;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
            }
            if (renderStateConfig.d3dDestBlend != D3DBLEND_ONE)
            {
            LABEL_53:
                renderStateConfig.d3dDestBlend = D3DBLEND_ONE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_ONE);
                return;
            }
        }
        return;
    case 5:
        if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
        {
            renderStateConfig.d3dAlphaBlendEnable = TRUE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
        }
        if (renderStateConfig.d3dSrcBlend != D3DBLEND_ONE)
        {
            renderStateConfig.d3dSrcBlend = D3DBLEND_ONE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_ONE);
        }
        if (renderStateConfig.d3dDestBlend != D3DBLEND_ONE)
            goto LABEL_53;
        return;
    case 6:
    case 8:
        if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
        {
            renderStateConfig.d3dAlphaBlendEnable = TRUE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
        }
        if (renderStateConfig.d3dSrcBlend != D3DBLEND_ZERO)
        {
            renderStateConfig.d3dSrcBlend = D3DBLEND_ZERO;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_ZERO);
        }
        if (renderStateConfig.d3dDestBlend != D3DBLEND_INVSRCCOLOR)
        {
            renderStateConfig.d3dDestBlend = D3DBLEND_INVSRCCOLOR;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
        }
        return;
    default:
        return;
    }
}

void D3D_SetFogParams(unsigned int a1)
{
    int v1;
    int v2;
    int v3;

    v1 = D3DRenderParams.d3dFogModeProbably;
    v2 = (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x4000) != 0 && D3DRenderParams.d3dFogModeProbably;
    if (renderStateConfig.d3dFogEnable != v2)
    {
        renderStateConfig.d3dFogEnable = v2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, v2);
        v1 = D3DRenderParams.d3dFogModeProbably;
    }
    if (v1)
    {
        switch (HIWORD(a1) & 0xF)
        {
        case 0:
        case 1:
        case 3:
        case 7:
            if (v1 != 1)
            {
                v3 = D3DRenderParams.d3dFogColor;
                D3DRenderParams.d3dFogModeProbably = 1;
                goto LABEL_15;
            }
            break;
        case 2:
        case 4:
            if (v1 != 5)
            {
                v3 = D3DRenderParams.d3dFogColor;
                D3DRenderParams.d3dFogModeProbably = 5;
                goto LABEL_15;
            }
            break;
        case 5:
        case 6:
            if (v1 != 3)
            {
                v3 = D3DRenderParams.d3dFogColor2Case5and6;
                D3DRenderParams.d3dFogModeProbably = 3;
            LABEL_15:
                if (renderStateConfig.d3dFogColor != v3)
                {
                    renderStateConfig.d3dFogColor = v3;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGCOLOR, v3);
                }
            }
            break;
        default:
            return;
        }
    }
}

void D3D_l_Texture_Init(int a1, int a2, char a3)
{
    int v3;

    if ((a3 & 1) != 0)
    {
        if (D3DRenderParams.textureUVIsProjected)
        {
            D3DRenderParams.textureUVMode = 4;
            memcpy(d3dTransformationMatrix, &D3DRenderParams.transformationMatrix, sizeof(D3DMATRIX));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&D3DRenderParams.transformationMatrix);
            if (textureStageTypeConfig[0].d3dTexCoordIndex != D3DTSS_TCI_CAMERASPACEPOSITION)
            {
                textureStageTypeConfig[0].d3dTexCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
            }
            if (textureStageTypeConfig[0].d3dTextureTransformFlags != (D3DTTFF_PROJECTED | D3DTTFF_COUNT3))
            {
                textureStageTypeConfig[0].d3dTextureTransformFlags = D3DTTFF_PROJECTED | D3DTTFF_COUNT3;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED | D3DTTFF_COUNT3);
            }
        }
        else
        {
            D3DRenderParams.textureUVMode = 5;
            memcpy(d3dTransformationMatrix, &D3DRenderParams.transformationMatrix, sizeof(D3DMATRIX));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&D3DRenderParams.transformationMatrix);
            if (textureStageTypeConfig[0].d3dTexCoordIndex != D3DTSS_TCI_CAMERASPACEPOSITION)
            {
                textureStageTypeConfig[0].d3dTexCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
            }
            if (textureStageTypeConfig[0].d3dTextureTransformFlags != D3DTTFF_COUNT2)
            {
                textureStageTypeConfig[0].d3dTextureTransformFlags = D3DTTFF_COUNT2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }
        }
        if (samplerConfig[0].d3d_addressu != D3DTADDRESS_CLAMP)
        {
            samplerConfig[0].d3d_addressu = D3DTADDRESS_CLAMP;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        }
        if (samplerConfig[0].d3d_addressv != D3DTADDRESS_CLAMP)
        {
            samplerConfig[0].d3d_addressv = D3DTADDRESS_CLAMP;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        }
        if (samplerConfig[0].d3d_minfilter != D3DTEXF_LINEAR)
        {
            samplerConfig[0].d3d_minfilter = D3DTEXF_LINEAR;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        if (samplerConfig[0].d3d_magfilter != D3DTEXF_LINEAR)
        {
            samplerConfig[0].d3d_magfilter = D3DTEXF_LINEAR;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        }
        if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SELECTARG1)
        {
            textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SELECTARG1;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        }
        if (textureStageTypeConfig[0].d3d_colorarg1)
        {
            textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        }
        if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
        {
            textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        }
        if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TEXTURE)
        {
            textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TEXTURE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        }
        D3D_SetTexture(0, -2);
        if (renderStateConfig.d3dColorWriteEnable != D3DCOLORWRITEENABLE_ALPHA)
        {
            renderStateConfig.d3dColorWriteEnable = D3DCOLORWRITEENABLE_ALPHA;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
        }
        if (renderStateConfig.d3dAlphaTest)
        {
            renderStateConfig.d3dAlphaTest = FALSE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, FALSE);
        }
        if (renderStateConfig.d3d_zWriteEnable)
        {
            renderStateConfig.d3d_zWriteEnable = FALSE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, FALSE);
        }
        if (renderStateConfig.d3dAlphaBlendEnable)
        {
            renderStateConfig.d3dAlphaBlendEnable = FALSE;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
        }
    }
    else if ((a3 & 2) == 0)
    {
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x10) != 0)
        {
            if (renderStateConfig.d3dFillMode != D3DFILL_SOLID)
            {
                renderStateConfig.d3dFillMode = D3DFILL_SOLID;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_SOLID);
            }
        }
        else if (renderStateConfig.d3dFillMode != D3DFILL_WIREFRAME)
        {
            renderStateConfig.d3dFillMode = D3DFILL_WIREFRAME;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        }
        if (renderStateConfig.d3d_zEnable != ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x1000000) != 0 ? D3DRenderParams.d3d_defaultZEnable : 0))
        {
            renderStateConfig.d3d_zEnable = (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x1000000) != 0 ? D3DRenderParams.d3d_defaultZEnable : 0;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, renderStateConfig.d3d_zEnable);
        }
        if (renderStateConfig.d3d_zFunc != 4 - ((a2 & 0x100) != 0))
        {
            renderStateConfig.d3d_zFunc = 4 - ((a2 & 0x100) != 0);
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZFUNC, renderStateConfig.d3d_zFunc);
        }
        if (renderStateConfig.d3d_zWriteEnable != ((a2 & 0x200) == 0))
        {
            renderStateConfig.d3d_zWriteEnable = (a2 & 0x200) == 0;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, renderStateConfig.d3d_zWriteEnable);
        }
        D3D_SetCullingParams();
        D3D_SetTexture(0, a1);
        D3D_SetTextureStageStates(0, a1);
        if (samplerConfig[0].d3d_addressu != (~(2 * (byte)a2) & 2 | 1))
        {
            samplerConfig[0].d3d_addressu = ~(2 * (byte)a2) & 2 | 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSU, samplerConfig[0].d3d_addressu);
        }
        if (samplerConfig[0].d3d_addressv != (~(byte)a2 & 2 | 1))
        {
            samplerConfig[0].d3d_addressv = ~(byte)a2 & 2 | 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSV, samplerConfig[0].d3d_addressv);
        }
        v3 = ((a2 & 4) != 0) + 1;
        if (samplerConfig[0].d3d_minfilter != v3)
        {
            samplerConfig[0].d3d_minfilter = ((a2 & 4) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, samplerConfig[0].d3d_minfilter);
        }
        if (samplerConfig[0].d3d_magfilter != v3)
        {
            samplerConfig[0].d3d_magfilter = ((a2 & 4) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, samplerConfig[0].d3d_magfilter);
        }
        if (samplerConfig[0].d3d_mipfilter != ((a2 & 8) != 0) + 1)
        {
            samplerConfig[0].d3d_mipfilter = ((a2 & 8) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MIPFILTER, samplerConfig[0].d3d_mipfilter);
        }
        if (samplerConfig[0].d3d_mipmaplodbias != tweak_mipmaplodbias)
        {
            samplerConfig[0].d3d_mipmaplodbias = tweak_mipmaplodbias;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MIPMAPLODBIAS, *(DWORD *)(&tweak_mipmaplodbias));
        }
        D3D_SetColorWriteEnable(a2);
        D3D_SetAlphaTestParams(a2, a2 & 0xFFF);
        D3D_SetAlphaBlendingOptions(a2, a2);
        D3D_SetFogParams(a2);
        gpst_DX_stDD->rasters[2].unk1 = a1; // TODO FIXME, itt nem volt az IDAban unk1, jó ez így?
        gpst_DX_stDD->rasters[3].unk1 = a2; // TODO FIXME, itt nem volt az IDAban unk1, jó ez így?
    }
}

BOOL D3D_CheckDeviceFormatSupported(int a1, int a2, int a3)
{
    return D3DRenderParams.d3d_if->lpVtbl->CheckDeviceFormat(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, D3DRenderParams.d3d_AdapterFormat, a1, a2, a3) >= 0;
}

BOOL D3D_CheckDepthStencilMatch(int a1, int a2)
{
    return D3DRenderParams.d3d_if->lpVtbl->CheckDepthStencilMatch(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, D3DRenderParams.d3d_AdapterFormat, a1, a2) >= 0;
}

int D3D_SelectSupportedDeviceFormat(int format)
{
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32) && D3D_CheckDepthStencilMatch(format, D3DFMT_D32))
        return D3DFMT_D32;
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8) && D3D_CheckDepthStencilMatch(format, D3DFMT_D24X8))
        return D3DFMT_D24X8;
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16) && D3D_CheckDepthStencilMatch(format, D3DFMT_D16))
        return D3DFMT_D16;
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16_LOCKABLE) && D3D_CheckDepthStencilMatch(format, D3DFMT_D16_LOCKABLE))
        return D3DFMT_D16_LOCKABLE;
    return D3DFMT_UNKNOWN;
}

void D3D_CreateWaterTexture()
{
    int v0;
    IDirect3DDevice9* v1;
    int v2;
    int v3;

    v0 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        512,
        512,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &waterTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v0);
    waterTexture->lpVtbl->GetSurfaceLevel(waterTexture, 0, &d3dWaterRTtoSet);
    v1 = D3DRenderParams.d3dDev;
    v2 = D3D_SelectSupportedDeviceFormat(D3DFMT_A8R8G8B8);
    v3 = v1->lpVtbl->CreateDepthStencilSurface(D3DRenderParams.d3dDev,
        512,
        512,
        v2,
        D3DMULTISAMPLE_NONE,
        0,
        FALSE,
        &d3dDSSurfaceToSet,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v3);
}

void D3D_CreateFullScreenTextureAndGetDepthStencilSurface()
{
    int v0;

    v0 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        D3DRenderParams.presentParams.BackBufferWidth,
        D3DRenderParams.presentParams.BackBufferHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DRenderParams.presentParams.BackBufferFormat,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.blurTex,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v0);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &D3DRenderParams.newZStencil);
}

void D3D_PresentAndClear()
{
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &D3DRenderParams.pDepthStencilSurface);
    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &(D3DRenderParams.pBackBuffers[0]));
    D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL);
    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &(D3DRenderParams.pBackBuffers[1]));
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL);
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
}



void D3D_InitEverything(tGdi_DisplayData* a1)
{
    IDirect3DTexture9** v1;
    int v2;
    D3DVERTEXELEMENT9 v5[4];
    IDirect3DSurface9* surf;

    D3D_CreateVBOs();
    v5[0].Stream = 0;
    v5[0].Offset = 0;
    v5[0].Type = 2;
    v5[0].Method = 0;
    v5[0].Usage = 0;
    v5[0].UsageIndex = 0;
    v5[1].Stream = 1;
    v5[1].Offset = 0;
    v5[1].Type = 4;
    v5[1].Method = 0;
    v5[1].Usage = 10;
    v5[1].UsageIndex = 0;
    v5[2].Stream = 255;
    v5[2].Offset = 0;
    v5[2].Type = 17;
    v5[2].Method = 0;
    v5[2].Usage = 0;
    v5[2].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, v5, &D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
    v5[0].Stream = 0;
    v5[0].Offset = 0;
    v5[0].Type = 2;
    v5[0].Method = 0;
    v5[0].Usage = 0;
    v5[0].UsageIndex = 0;
    v5[1].Stream = 1;
    v5[1].Offset = 0;
    v5[1].Type = 4;
    v5[1].Method = 0;
    v5[1].Usage = 10;
    v5[1].UsageIndex = 0;
    v5[2].Stream = 2;
    v5[2].Offset = 0;
    v5[2].Type = 2;
    v5[2].Method = 0;
    v5[2].Usage = 3;
    v5[2].UsageIndex = 0;
    v5[3].Stream = 255;
    v5[3].Offset = 0;
    v5[3].Type = 17;
    v5[3].Method = 0;
    v5[3].Usage = 0;
    v5[3].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, v5, &D3DRenderParams.d3dVertexDeclaration);
    v5[0].Stream = 0;
    v5[0].Offset = 0;
    v5[0].Type = 2;
    v5[0].Method = 0;
    v5[0].Usage = 0;
    v5[0].UsageIndex = 0;
    v5[1].Stream = 1;
    v5[1].Offset = 0;
    v5[1].Type = 4;
    v5[1].Method = 0;
    v5[1].Usage = 10;
    v5[1].UsageIndex = 0;
    v5[2].Stream = 2;
    v5[2].Offset = 0;
    v5[2].Type = 1;
    v5[2].Method = 0;
    v5[2].Usage = 5;
    v5[2].UsageIndex = 0;
    v5[3].Stream = 255;
    v5[3].Offset = 0;
    v5[3].Type = 17;
    v5[3].Method = 0;
    v5[3].Usage = 0;
    v5[3].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, v5, &D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
    gpst_DX_stDD = a1;
    a1->rasters[3].unk1 = 0; // TODO FIXME ez biztos jó így ???? megnézni. az idaban csak siman a1->rasters[3] = 0 van de az nem jó
    D3D_SetViewPort(0,
        (D3DRenderParams.Rect.bottom - (D3DRenderParams.Rect.right - D3DRenderParams.Rect.left) - D3DRenderParams.Rect.top) / 2,
        D3DRenderParams.Rect.right - D3DRenderParams.Rect.left, D3DRenderParams.Rect.right - D3DRenderParams.Rect.left);
    
    if ((D3DRenderParams.d3dcaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0
        && !D3DRenderParams.argbIsSupportedForTextures
        && D3DRenderParams.d3d_if->lpVtbl->CheckDeviceFormat(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, 1, D3DRenderParams.d3d_AdapterFormat,
            D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8) >= 0) 
    {
        D3DRenderParams.argbIsSupportedForTextures = 1;
    }
    D3D_SetInitialRenderStates();
    D3D_SetInitialSamplerStates();
    D3D_SetInitialTextureStageStates();
    D3D_l_Texture_Init(-1, -1, 0);
    D3D_SetProjectionMatrix(&a1->ct.cam);
    D3D_CreateWaterTexture();
    if (renderStateConfig.d3d_zEnable != D3DRenderParams.d3d_defaultZEnable)
    {
        renderStateConfig.d3d_zEnable = D3DRenderParams.d3d_defaultZEnable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DRenderParams.d3d_defaultZEnable);
    }
    D3DRenderParams.tweak_ae_d2x = D3DRenderParams.d3dcaps.TextureOpCaps & D3DTEXOPCAPS_MODULATE2X;
    D3DRenderParams.textureUVMode = 0;
    D3DRenderParams.diffuse2x = 0;
    memset(&D3DRenderParams.transformationMatrix, 0, 64u);
    D3DRenderParams.textureUVIsProjected = 1;
    D3DRenderParams.isDepthBlurOn = 0;
    D3DRenderParams.tweak_zonalpha = 0;
    D3DRenderParams.sunEffectIsOn = 0;
    D3DRenderParams.tweak_ae_noflare = D3DRenderParams.d3dcaps.MaxSimultaneousTextures < 4u;
    D3D_CreateFullScreenTextureAndGetDepthStencilSurface();
    D3D_PresentAndClear();
    D3D_CreateBlurTextures();
    D3D_AETexturesInit();
    //*(_DWORD*)&v5[3].Stream = v4; / ????
    v1 = &(D3DRenderParams.d3dShadowTextures[8]);
    do
    {
        v2 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, 128, 128, 1, 1, 21, 0, --v1, 0);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(v2);
        (*v1)->lpVtbl->GetSurfaceLevel(*v1, 0, &surf);
        D3DRenderParams.d3dDev->lpVtbl->ColorFill(D3DRenderParams.d3dDev, surf, NULL, 0xffffffff);
        surf->lpVtbl->Release(surf);
    } while (v1 != D3DRenderParams.d3dShadowTextures);
}

void D3D_CreateAndCopySourceTexturesToThem()
{
    unsigned int v0;
    IDirect3DTexture9** v1;
    IDirect3DTexture9** v2;
    int v3;
    IDirect3DTexture9* v4;
    int v5;
    int v6;
    IDirect3DTexture9* v7;
    D3DSURFACE_DESC v8;

    v0 = 4 * D3DRenderParams.d3dTextureNum + 4;
    v7 = 0;
    D3DRenderParams.d3dTextures = (IDirect3DTexture9**)MEM_p_Alloc(v0);
    memset(D3DRenderParams.d3dTextures, 0, v0);
    v1 = (IDirect3DTexture9**)D3DRenderParams.d3dSourceTextures;
    v2 = (IDirect3DTexture9**)&D3DRenderParams.d3dSourceTextures[D3DRenderParams.d3dTextureNum];
    if (D3DRenderParams.d3dSourceTextures < v2)
    {
        v3 = 0;
        do
        {
            v4 = 0;
            v7 = 0;
            if (*v1)
            {
                (*v1)->lpVtbl->GetLevelDesc(*v1, 0, &v8);
                v5 = (*v1)->lpVtbl->GetLevelCount(*v1);
                v6 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
                    v8.Width,
                    v8.Height,
                    v5,
                    0,
                    v8.Format,
                    D3DPOOL_DEFAULT,
                    &v7,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(v6);
                (*v1)->lpVtbl->AddDirtyRect(*v1, NULL);
                v4 = v7;
                if (v7)
                {
                    D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)*v1, (IDirect3DBaseTexture9*)v7);
                    v4 = v7;
                }
            }
            D3DRenderParams.d3dTextures[v3] = v4;
            ++v1;
            ++v3;
        } while (v1 < v2);
    }
}
void D3D_CreateIndexAndVertexBuffForElemsIfSuitable(tGameObjectMain* gao, tGEOGRO* geogro)
{
    IDirect3DVertexBuffer9** v3;
    int v4;
    int v5;
    IDirect3DVertexBuffer9** v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
    tGeoWholeTriangleStripBlock* v12;
    int v13;
    tGeoTriangleStrip* v14;
    int v15;
    tGeoTriangleStrip* v16;
    int v17;
    BOOL v18;
    tGeoUVElem* v19;
    tGeoTriangleStripElem* v20;
    int v21;
    tGeoUVElem* v22;
    float v23;
    float* v24;
    BOOL v25;
    tGeoUVElem* v26;
    int v27;
    tGeoTriangleStripElem* v28;
    int v29;
    tGeoUVElem* v30;
    unsigned int v31;
    int v32;
    IDirect3DVertexBuffer9** v33;
    int v34;
    int v35;
    int v36;
    int v37;
    IDirect3DVertexBuffer9* v38;
    tGeoUVElem* v40;
    int v41;
    int v42;
    tGeoTriangleStripElem* v43;
    int v44;
    tGeoUVElem* v45;
    float v46;
    tGeoUVElem* v47;
    BOOL v48; // cc
    int i; // esi
    IDirect3DIndexBuffer9* v50;
    IDirect3DIndexBuffer9** v51;
    int v52;
    int v53;
    tGeoWholeTriangleStripBlock* v54;
    int v55;
    unsigned int v56;
    int v57;
    tGeoTriangleStrip* v58;
    int v59;
    unsigned int v60;
    int v61;
    int v62;
    tGeoWholeTriangleStripBlock* v63;
    int v64;
    int v65;
    int v66;
    int v67;
    tGeoTriangleStrip* v68;
    tD3dStreamInfoElem* v69;
    int v70;
    tD3dStreamInfoElem* v71;
    int v72;
    IDirect3DVertexBuffer9* v73;
    unsigned int v74;
    int v75;
    unsigned int v76;
    int v77;
    unsigned int v78;
    unsigned int v79;
    int v80;
    IDirect3DIndexBuffer9** v81;
    unsigned int v82;
    tGeoWholeTriangleStripBlock* v83;
    int v84;
    tGeoUVElem* v85;
    int v86;
    void* v87;
    tGeoUVElem* v88[50];
    tGeoUVElem** v81b;
    tGeoUVElem* v81c;
    unsigned short* v75b;

    if (worldPtrIfMultiStream) // TODO FIXME, Ennek a függvénynek az elsõ hívásánál ez biztos hogy 0 lesz, szóal csak a másodiknál lesznek ezek a pufferek létrehozva.
    {
        GEO_UseNormals(geogro);
        if ((gao->flags & 2) == 0 || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            v3 = &geogro->d3dvb;
            if (!geogro->d3dvb)
            {
                v4 = D3D_CalculateStride(D3DFVF_XYZ);
                v5 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                    D3DRenderParams.d3dDev,
                    v4 * geogro->vertexNum,
                    D3DUSAGE_WRITEONLY,
                    D3DFVF_XYZ,
                    D3DPOOL_DEFAULT,
                    &geogro->d3dvb,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(v5);
                (*v3)->lpVtbl->Lock(*v3, 0, 0, &v87, 0);
                memcpy(v87, geogro->vertices, v4 * geogro->vertexNum);
                (*v3)->lpVtbl->Unlock(*v3);
            }
            v6 = &geogro->d3dvb2;
            if (!geogro->d3dvb2)
            {
                v7 = D3D_CalculateStride(D3DFVF_NORMAL);
                v8 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                    D3DRenderParams.d3dDev,
                    v7 * geogro->vertexNum,
                    D3DUSAGE_WRITEONLY,
                    D3DFVF_NORMAL,
                    D3DPOOL_DEFAULT,
                    &geogro->d3dvb2,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(v8);
                (*v6)->lpVtbl->Lock(*v6, 0, 0, &v87, 0);
                memcpy(v87, geogro->normals, v7 * geogro->vertexNum);
                (*v6)->lpVtbl->Unlock(*v6);
            }
        }
        if (!geogro->d3dUVVB && geogro->uvNum)
        {
            geogro->d3dUVVBNum = 1;
            geogro->d3dUVVB = (IDirect3DVertexBuffer9**)MEM_p_Alloc(4u);
            v9 = D3D_CalculateStride(D3DFVF_TEX1);
            v10 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                D3DRenderParams.d3dDev,
                v9 * geogro->vertexNum,
                D3DUSAGE_WRITEONLY,
                D3DFVF_TEX1,
                D3DPOOL_DEFAULT,
                &(geogro->d3dUVVB[0]),
                NULL);
            D3D_CheckReturnAndRunSettingsApplicationIfProblem(v10);
            
            geogro->d3dUVVB[0]->lpVtbl->Lock(geogro->d3dUVVB[0], 0, 0, &(v88[0]), 0);

            if (v88[0] < &v88[0][geogro->vertexNum]) {
                for (i = 0; i < (int)((sizeof(tGeoUVElem) * geogro->vertexNum - 1) >> 2) + 1; i++) {
                    ((float*)(v88[0]))[i] = FLT_MAX;
     //               *((int*)(&(v88[0][i]))) = 0x7F7FFFFF; // ez FLOAT_MAX, TODO FIXME
                }
            }
            v11 = geogro->triangleChunkCount;
            v79 = 0;
            if (v11 > 0)
            {
                v76 = 0;
                do
                {
                    v83 = geogro->triangleChunks[v76].triangleStripBlock;
                    v12 = v83;
                    if (v83)
                    {
                        v13 = v83->triangleStripNum;
                        v82 = 0;
                        if (v13)
                        {
                            v75 = 0;
                            do
                            {
                                v14 = v12->triangleStrips;
                                v15 = geogro->d3dUVVBNum;
                                v16 = &(v14[v75]);
                                v17 = 0;
                                v78 = 0;
                                if (v15)
                                {
                                    while (1)
                                    {
                                        v18 = v16->triangleStripElemNum == 0;
                                        v19 = v88[v17];
                                        v80 = 0;
                                        if (v18)
                                            break;
                                        v20 = v16->triangleStripElems;
                                        while (1)
                                        {
                                            v21 = v20->triangleIdx;
                                            v22 = &geogro->UVs[v20->uvIdx];
                                            if (v19[v21].u != FLT_MAX && v19[v21].u != v22->u)
                                                break;
                                            v23 = v19[v21].v;
                                            v24 = &v19[v21].v;
                                            if (v23 != FLT_MAX && *v24 != v22->v)
                                                break;
                                            ++v20;
                                            v25 = ++v80 < v16->triangleStripElemNum;
                                            if (!v25)
                                            {
                                                v17 = v78;
                                                goto LABEL_26;
                                            }
                                        }
                                        v31 = geogro->d3dUVVBNum;
                                        v17 = ++v78;
                                        if (v78 >= v31)
                                            goto LABEL_31;
                                    }
                                LABEL_26:
                                    v26 = v88[v17];
                                    v27 = 0;
                                    if (v16->triangleStripElemNum)
                                    {
                                        do
                                        {
                                            v28 = v16->triangleStripElems;
                                            v29 = v28[v27].triangleIdx;
                                            v30 = &geogro->UVs[v28[v27].uvIdx];
                                            v26[v29].u = v30->u;
                                            v26[v29].v = v30->v;
                                            ++v27;
                                        } while (v27 < v16->triangleStripElemNum);
                                        v17 = v78;
                                    }
                                    v16->vbIdx = v17;
                                }
                                else
                                {
                                LABEL_31:
                                    v32 = geogro->d3dUVVBNum + 1;
                                    geogro->d3dUVVBNum = v32;
                                    v33 = (IDirect3DVertexBuffer9**)MEM_p_Realloc(geogro->d3dUVVB, 4 * v32);
                                    v34 = geogro->d3dUVVBNum;
                                    geogro->d3dUVVB = v33;
                                    v35 = v34 - 1;
                                    v36 = D3D_CalculateStride(D3DFVF_TEX1);
                                    v37 = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                                        D3DRenderParams.d3dDev,
                                        v36 * geogro->vertexNum,
                                        D3DUSAGE_WRITEONLY,
                                        D3DFVF_TEX1,
                                        D3DPOOL_DEFAULT,
                                        &geogro->d3dUVVB[v35],
                                        NULL);
                                    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v37);
                                    v38 = geogro->d3dUVVB[v35];
                                    v81b = &v88[v35];
                                    v38->lpVtbl->Lock(v38, 0, 0, v81b, 0);
                                    v40 = *v81b;
                                    v41 = geogro->vertexNum;
                                    if (v40 < &v40[v41]) {
                                        for (i = 0; i < (int)((sizeof(tGeoUVElem) * v41 - 1) >> 2) + 1; i++) {
                                            ((float*)v40)[i] = FLT_MAX;
                                            //*((int*)(&(v40[i]))) = 0x7F7FFFFF; // ez FLOAT_MAX, TODO FIXME
                                        }
                                    }
                                    v42 = 0;
                                    v18 = v16->triangleStripElemNum == 0;
                                    v81c = *v81b;
                                    if (!v18)
                                    {
                                        do
                                        {
                                            v43 = v16->triangleStripElems;
                                            v44 = v43[v42].triangleIdx;
                                            v45 = &geogro->UVs[v43[v42].uvIdx];
                                            v46 = v45->u;
                                            v85 = v45;
                                            v47 = v81c;
                                            v81c[v44].u = v46;
                                            v47[v44].v = v85->v;
                                            ++v42;
                                        } while (v42 < v16->triangleStripElemNum);
                                    }
                                    v16->vbIdx = v35; 
                                }
                                v12 = v83;
                                v25 = ++v82 < v83->triangleStripNum;
                                v75++;
                            } while (v25);
                        }
                    }
                    v48 = (int)++v79 < geogro->triangleChunkCount;
                    v76++;
                } while (v48);
            }
            for (i = 0; i < geogro->d3dUVVBNum; ++i)
            {
                v73 = geogro->d3dUVVB[i];
                v73->lpVtbl->Unlock(v73);
            }
        }
        v50 = geogro->d3dIndexBuf;
        v51 = &geogro->d3dIndexBuf;
        v81 = &geogro->d3dIndexBuf;
        if (!v50)
        {
            v52 = geogro->triangleChunkCount;
            v53 = 0;
            v78 = 0;
            if (v52 > 0)
            {
                v75 = 0;
                do
                {
                    v54 = geogro->triangleChunks[v75].triangleStripBlock;
                    if (v54)
                    {
                        v55 = geogro->d3dUVVBNum;
                        v56 = 0;
                        v77 = 0;
                        v79 = 0;
                        if (v55)
                        {
                            do
                            {
                                v57 = v53;
                                if (v54->triangleStripNum)
                                {
                                    v58 = v54->triangleStrips;
                                    v59 = v54->triangleStripNum;
                                    do
                                    {
                                        if (v58->vbIdx == v56)
                                        {
                                            if (v57 != v53)
                                            {
                                                v53 += 2;
                                                if ((((byte)v53 - (byte)v57) & 1) != 0) // ez nembiztos hogy jó így TODO IFXME
                                                    ++v53;
                                            }
                                            v53 += v58->triangleStripElemNum;
                                            v56 = v79;
                                        }
                                        ++v58;
                                        --v59;
                                    } while (v59);
                                }
                                if (v53 != v57)
                                    ++v77;
                                v60 = geogro->d3dUVVBNum;
                                v79 = ++v56;
                            } while (v56 < v60);
                        }
                        v51 = v81;
                        v54->streamsNum = v77;
                    }
                    v48 = (int)++v78 < geogro->triangleChunkCount;
                    v75++;
                } while (v48);
                if (v53)
                {
                    v61 = 0;
                    v62 = D3DRenderParams.d3dDev->lpVtbl->CreateIndexBuffer(
                        D3DRenderParams.d3dDev,
                        2 * v53,
                        D3DUSAGE_WRITEONLY,
                        D3DFMT_INDEX16,
                        D3DPOOL_DEFAULT,
                        v51,
                        NULL);
                    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v62);
                    (*v51)->lpVtbl->Lock(*v51, 0, 0, &v75b, 0); // ide uj v75 kéne
                    v48 = geogro->triangleChunkCount <= 0;
                    v78 = 0;
                    if (!v48)
                    {
                        v80 = 0;
                        do
                        {
                            v63 = geogro->triangleChunks[v80].triangleStripBlock;
                            if (v63)
                            {
                                v74 = sizeof(tD3dStreamInfoElem) * v63->streamsNum;
                                v82 = 0;
                                v63->d3dStreamInfoElems = (tD3dStreamInfoElem*)MEM_p_Alloc(v74);
                                v64 = geogro->d3dUVVBNum;
                                v65 = 0;
                                v86 = 0;
                                if (v64)
                                {
                                    v66 = 0;
                                    v84 = 0;
                                    do
                                    {
                                        if (v82 >= v63->streamsNum)
                                            break;
                                        v63->d3dStreamInfoElems[v66].startIndex = v61;
                                        v67 = v63->triangleStripNum;
                                        v79 = 0;
                                        if (v67)
                                        {
                                            v77 = 0;
                                            do
                                            {
                                                //v68 = v63[v77].triangleStrips;
                                                v68 = &(v63->triangleStrips[v77]);
                                                if (v68->vbIdx == v65)
                                                {
                                                    v69 = &(v63->d3dStreamInfoElems[v66]);
                                                    if (v69->startIndex == v61)
                                                    {
                                                        v69->vbIdx = v65;
                                                    }
                                                    else
                                                    {
                                                        *v75b = *(v75b - 1);
                                                        *++v75b = v68->triangleStripElems->triangleIdx;
                                                        v61 += 2;
                                                        ++v75b;
                                                        if ((((byte)v61 - (byte)v63->d3dStreamInfoElems[v66].startIndex) & 1) != 0)
                                                        {
                                                            *v75b++ = v68->triangleStripElems->triangleIdx;
                                                            ++v61;
                                                        }
                                                    }
                                                    v70 = 0;
                                                    if (v68->triangleStripElemNum)
                                                    {
                                                        do
                                                            *v75b++ = v68->triangleStripElems[v70++].triangleIdx;
                                                        while (v70 < v68->triangleStripElemNum);
                                                        v66 = v84;
                                                    }
                                                    v65 = v86;
                                                    v61 += v68->triangleStripElemNum;
                                                }
                                                v77++;
                                                v25 = ++v79 < v63->triangleStripNum;
                                            } while (v25);
                                        }
                                        v71 = &(v63->d3dStreamInfoElems[v66]);
                                        if (v61 != v71->startIndex)
                                        {
                                            v71->primitiveCount = v61 - v71->startIndex;
                                            v66++;
                                            ++v82;
                                            v84 = v66;
                                        }
                                        v72 = geogro->d3dUVVBNum;
                                        v86 = ++v65;
                                    } while (v65 < v72);
                                }
                            }
                            v48 = (int)++v78 < geogro->triangleChunkCount;
                            v80++;
                        } while (v48);
                        v51 = v81;
                    }
                    (*v51)->lpVtbl->Unlock(*v51);
                }
            }
        }
    }
}

void D3D_CreateIndexAndVertexBuffForGameObject(tGameObjectMain* a1)
{
    tGameObjectMain* v1;
    tGameObjectVisualObject* v2;
    tGEOGRO* v3;
    int v4;
    tGEOGRO** v5;
    tGEOGRO* v6;
    tGEOSTATICLOD* v3a;

    v1 = a1;
    if ((a1->flags & 0x4000) != 0)
    {
        v2 = (tGameObjectVisualObject*)a1->basic->actorObjectOrVisualObject;
        v3 = (tGEOGRO*)v2->GROGEOOrSomethingElse;
        v3a = (tGEOSTATICLOD*)v2->GROGEOOrSomethingElse;
        if (v2->GROGEOOrSomethingElse)
        {
            if (v3->intf->id == 1)
            {
                if (v3->triangleChunkCount && v3->triangleChunks && v3->vertexNum)
                    D3D_CreateIndexAndVertexBuffForElemsIfSuitable(a1, (tGEOGRO*)v2->GROGEOOrSomethingElse);
            }
            else if (v3a->intf->id == 8)
            {
                v4 = 0;
                if (v3a->lodNum)
                {
                    v5 = v3a->geo;
                    do
                    {
                        v6 = *v5;
                        if ((*v5)->triangleChunkCount && v6->triangleChunks)
                        {
                            if (v6->vertexNum)
                            {
                                D3D_CreateIndexAndVertexBuffForElemsIfSuitable(v1, v6);
                                v1 = a1;
                            }
                        }
                        ++v4;
                        ++v5;
                    } while (v4 < v3a->lodNum);
                }
            }
        }
    }
}

void D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld* a1)
{
    tWorld* v1;
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    tGameObjectMain* v4;
    tGroup* v5;
    tableDescStruct* v6;
    tableElemStructPF* v7;
    tableElemStructPF* i;

    if (!ENG_IsSingleStream)
    {
        v1 = a1;
        if (a1)
        {
            if (a1->objectTablePF.actElemNum)
            {
                v2 = a1->objectTablePF.firstElemPtr;
                v3 = ((tableElemStructPF*)(a1->objectTablePF.nextElemPtr)) - 1;
                if (v2 <= v3)
                {
                    do
                    {
                        v4 = (tGameObjectMain*)v2->objectPtr;
                        if (v2->objectPtr)
                        {
                            if (((uint8_t)v4 & 3) != 1)
                            {
                                D3D_CreateIndexAndVertexBuffForGameObject((tGameObjectMain*)v2->objectPtr);
                                if ((v4->flags & 2) != 0)
                                {
                                    v5 = ((tGameObjectActorObject*)(v4->basic->actorObjectOrVisualObject))->skeletonGroup;
                                    if (v5)
                                    {
                                        v6 = v5->tablePF;
                                        v7 = v6->firstElemPtr;
                                        for (i = ((tableElemStructPF*)(v6->nextElemPtr)) - 1; v7 <= i; ++v7)
                                        {
                                            if (v7->objectPtr && ((int)v7->objectPtr & 3) != 1)
                                                D3D_CreateIndexAndVertexBuffForGameObject((tGameObjectMain*)v7->objectPtr);
                                        }
                                    }
                                }
                            }
                        }
                        ++v2;
                    } while (v2 <= v3);
                    v1 = a1;
                }
            }
        }
        if (!worldPtrIfMultiStream)
            worldPtrIfMultiStream = v1;
    }
}

void D3D_CreateSunTextures(tGfxSunD3DSpecificData* a1)
{
    int v1;

    if (!a1->d3dTexture)
    {
        v1 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
            16,
            1,
            1,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT,
            &a1->d3dTexture,
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(v1);
        a1->d3dTexture->lpVtbl->GetSurfaceLevel(a1->d3dTexture, 0, &a1->d3dRenderTarget);
    }
}

void D3D_CreateSunTexturesForSunSurfaces()
{
    tGfxSunD3DSpecificData** v0;

    v0 = gD3dSunSurfaces;
    do
    {
        if (*v0)
            D3D_CreateSunTextures(*v0);
        ++v0;
    } while (v0 <= &gD3dSunSurfaces[15]);
}

void D3D_WaterBuffersInit()
{
    unsigned int v0;
    int v1;
    short* v2;
    int v3;
    unsigned int v4;
    unsigned int v5;
    short* v6;
    unsigned int v7;
    short* v8;
    unsigned int v9;
    short* v10;
    short* v11;
    int v12;
    int v13;
    short* v14;

    v0 = wtr_NumA;
    v1 = wtr_NumB;
    if (wtr_NumA && wtr_NumB && !waterStreamData)
    {
        wtr_primitiveCount = (wtr_NumA + 3) * (2 * wtr_NumB - 2) - 3;
        D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(D3DRenderParams.d3dDev,
            32 * wtr_NumA * wtr_NumB + 16,
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_SPECULAR | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ,
            D3DPOOL_DEFAULT,
            &waterStreamData,
            NULL);
        D3DRenderParams.d3dDev->lpVtbl->CreateIndexBuffer(D3DRenderParams.d3dDev,
            2 * wtr_primitiveCount + 4,
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &waterIndexData,
            NULL);
        waterIndexData->lpVtbl->Lock(waterIndexData, 0, 0, (void **)&v14, 0);
        v2 = v14;
        v3 = 0;
        v12 = 0;
        if (v1 != 1)
        {
            v13 = v1 - 1;
            do
            {
                if (v12)
                    *v2++ = v3;
                else
                    v12 = 1;
                v4 = 0;
                if (v0 >> 1)
                {
                    v5 = v0 >> 1;
                    v4 = v0 >> 1;
                    do
                    {
                        *v2 = v3;
                        v6 = v2 + 1;
                        *v6 = v0 + v3;
                        v2 = v6 + 1;
                        ++v3;
                        --v5;
                    } while (v5);
                }
                v7 = v4 - 1;
                *v2 = v0 + --v3;
                v8 = v2 + 1;
                if (v7 < v0)
                {
                    v9 = v0 - v7;
                    do
                    {
                        *v8 = v0 + v3;
                        v10 = v8 + 1;
                        *v10 = v3;
                        v8 = v10 + 1;
                        ++v3;
                        --v9;
                    } while (v9);
                }
                *v8 = v3 - 1;
                v11 = v8 + 1;
                *v11 = v3 - 1;
                v2 = v11 + 1;
                --v13;
            } while (v13);
        }
        waterIndexData->lpVtbl->Unlock(waterIndexData);
    }
}

void D3D_ReinitD3DAppBackCache()
{
    D3D_InitEverything(gpst_DX_stDD);
    D3D_CreateAndCopySourceTexturesToThem();
    D3D_CreateGameObjectsD3DStuffForMultiStream(WOR_gpst_CurrentWorld);
    D3D_CreateSunTexturesForSunSurfaces();
    D3D_WaterBuffersInit();
}

void D3D_Flip()
{
    int v0;
    int v1;
    int v2;
    struct tagMSG Msg;

    Rasters_Update();
    if (D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 0;
    }
    v0 = D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL); 
    v1 = v0;
    if (!ENG_gb_ExitApplication && !ENG_gb_ForceEndEngine)
    {
        if (v0 == D3DERR_DEVICELOST) //0x88760868
        {
            do
            {
                D3DRenderParams.dword_B317A0 = 1;
                while (v1 != D3DERR_DEVICENOTRESET) //0x88760869
                {
                    if (PeekMessageA(&Msg, 0, 0, 0, 1u))
                    {
                        TranslateMessage(&Msg);
                        DispatchMessageA(&Msg);
                        if (ENG_gb_ExitApplication || ENG_gb_ForceEndEngine)
                            return;
                    }
                    else
                    {
                        v1 = D3DRenderParams.d3dDev->lpVtbl->TestCooperativeLevel(D3DRenderParams.d3dDev);
                    }
                }
                D3D_ReleaseEveryCache();
                v2 = D3DRenderParams.d3dDev->lpVtbl->Reset(D3DRenderParams.d3dDev, &D3DRenderParams.presentParams); 
                v1 = v2;
            } while (v2 == D3DERR_DEVICELOST);
            if (v2 >= 0)
                D3D_ReinitD3DAppBackCache();
            else
                ENG_gb_ExitApplication = 1;
        }
        D3D_CycleThroughVBOs();
        D3DRenderParams.currTargetGao = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    }
}

void D3D_Clear(int a1, int color) 
{
    if (a1 == 2)
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    else
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL,
            (a1 & D3DCLEAR_ZBUFFER) | D3DCLEAR_TARGET,
            color & 0xFF00FF00 | ((uint8_t)color << 16) | ((color >> 16) & 0xff), 1.0f, 0);
}

BOOL RegQueryVal(HKEY hKey, LPCSTR lpValueName, LPBYTE lpData)
{
    DWORD cbData;
    DWORD Type;

    cbData = 4;
    return !RegQueryValueExA(hKey, lpValueName, 0, &Type, lpData, &cbData) && Type == 4 && cbData == 4;
}

void D3D_SunSurfacesClear()
{
    memset(gD3dSunSurfaces, 0, sizeof(gD3dSunSurfaces));
}

void D3D_ApplyRenderingPreesetsForVariousGraphicsCards()
{
    D3DADAPTER_IDENTIFIER9 v0;

    D3DRenderParams.d3d_if->lpVtbl->GetAdapterIdentifier(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, 0, &v0);
    D3D_OutputDebugString("%d, %d, %d, %d\n", HIWORD(v0.DriverVersion.HighPart), LOWORD(v0.DriverVersion.HighPart), HIWORD(v0.DriverVersion.LowPart), LOWORD(v0.DriverVersion.LowPart));
    if (v0.VendorId == 0x1002)
    {
        ENG_IsDLBFEnabled = 1;
        if (v0.DeviceId > 0x4E45)
        {
            if (v0.DeviceId > 0x5157)
            {
                if (v0.DeviceId < 0x5159 || v0.DeviceId > 0x515A)
                    goto LABEL_27;
            }
            else if (v0.DeviceId != 0x5157)
            {
                if (v0.DeviceId > 0x5147)
                    goto LABEL_27;
                if (v0.DeviceId < 0x5144)
                {
                    if (v0.DeviceId < 0x4E64 || v0.DeviceId > 0x4E65)
                        goto LABEL_27;
                    goto LABEL_23;
                }
            }
        }
        else
        {
            if (v0.DeviceId >= 0x4E44)
                goto LABEL_23;
            if (v0.DeviceId > 0x4C57)
            {
                if (v0.DeviceId < 0x4C59 || v0.DeviceId > 0x4C5A)
                    goto LABEL_27;
            }
            else if (v0.DeviceId != 0x4C57)
            {
                if (v0.DeviceId < 0x4144 || v0.DeviceId > 0x4146 && v0.DeviceId != 16740)
                    goto LABEL_27;
            LABEL_23:
                D3DRenderParams.argbIsSupportedForTextures = -1;
                return;
            }
        }
    LABEL_26:
        ENG_IsSingleStream = 1;
        goto LABEL_27;
    }
    if (v0.VendorId != 4318)
    {
    LABEL_27:
        return;
    }
    switch (v0.DeviceId)
    {
    case 0x110u:
    case 0x111u:
    case 0x112u:
    case 0x113u:
    case 0x174u:
    case 0x175u:
    case 0x176u:
    case 0x179u:
        ENG_IsDLBFEnabled = 1;
        goto LABEL_26;
    case 0x170u:
    case 0x171u:
    case 0x172u:
    case 0x173u:
    case 0x177u:
        ENG_IsSingleStream = 1;
        break;
    default:
        goto LABEL_27;
    }
}

int D3D_l_Init(HWND hWnd, struct sGdi_DisplayData* dd)
{
    BOOL (__stdcall *IsDebuggerPresent)();
    int ret;
    int i;
    int tries;
    HMODULE libHandle;
    void (*DisableD3DSpy)(void);
    HKEY phkResult;
    int adapterMode;
    D3DDISPLAYMODE dispMode;

    if (D3D_l_Close())
        return E_FAIL;
    
    D3DRenderParams.d3d_AdapterFormat = D3DFMT_X8R8G8B8;
    D3DRenderParams.presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\IRISEng\\Basic video", 0, KEY_READ, &phkResult)
        || !RegQueryVal(phkResult, "Video Adapter", (LPBYTE)&D3DRenderParams.d3d_Adapter)
        || !RegQueryVal(phkResult, "Z Buffer depth", (LPBYTE)&D3DRenderParams.ZBufferDepth)
        || !RegQueryVal(phkResult, "Mode", (LPBYTE)&adapterMode)
        || !RegQueryVal(phkResult, "MipMapping", (LPBYTE)&D3DRenderParams.Mipmapping))
    {
        MessageBoxA(hWnd, "Application settings not correctly set or invalid, please re-run settings application", "Beyond Good & Evil", MB_TASKMODAL | MB_ICONERROR);
        exit(1);
    }

    RegCloseKey(phkResult);

    libHandle = LoadLibraryA("kernel32");
    IsDebuggerPresent = GetProcAddress(libHandle, "IsDebuggerPresent");
    
    if (IsDebuggerPresent && IsDebuggerPresent() || ENG_WindowedMode)
    {
        D3DRenderParams.d3d_if->lpVtbl->GetAdapterDisplayMode(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, &dispMode);
        D3DRenderParams.d3d_AdapterFormat = dispMode.Format;
        D3DRenderParams.presentParams.Windowed = 1;
        GetClientRect(hWnd, &D3DRenderParams.Rect);
        if (!IsDebuggerPresent || !IsDebuggerPresent()) 
            tweakFlagsDisabled = 1;
    }
    else
    {
        D3DRenderParams.d3d_if->lpVtbl->EnumAdapterModes(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DRenderParams.d3d_AdapterFormat, adapterMode, &dispMode);
        D3DRenderParams.presentParams.Windowed = 0;
        D3DRenderParams.presentParams.FullScreen_RefreshRateInHz = dispMode.RefreshRate;
        D3DRenderParams.Rect.right = dispMode.Width;
        D3DRenderParams.Rect.bottom = dispMode.Height;
        tweakFlagsDisabled = 1;        
    }
    D3DRenderParams.hwnd2_dword_B3162C = (int)hWnd;
    D3DRenderParams.dword_B317A0 = 0;
    D3DRenderParams.argbIsSupportedForTextures = 0;
    D3DRenderParams.setsDefaultZEnableAtBeginScene = 0;
    D3D_ApplyRenderingPreesetsForVariousGraphicsCards();
    if (ENG_IsDLBFEnabled)
    {
        D3DRenderParams.presentParams.SwapEffect = D3DSWAPEFFECT_COPY;
        D3DRenderParams.presentParams.BackBufferCount = 1;
    }
    else
    {
        D3DRenderParams.presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
        D3DRenderParams.presentParams.BackBufferCount = 2;
    }
    D3DRenderParams.presentParams.BackBufferWidth = D3DRenderParams.Rect.right;
    D3DRenderParams.presentParams.hDeviceWindow = hWnd;
    D3DRenderParams.presentParams.BackBufferHeight = D3DRenderParams.Rect.bottom;
    D3DRenderParams.presentParams.EnableAutoDepthStencil = 1;
    D3DRenderParams.presentParams.AutoDepthStencilFormat = D3DRenderParams.ZBufferDepth;
    D3DRenderParams.presentParams.PresentationInterval = 1;
    ret = E_FAIL;
    for (i = 0; i < 2; ++i)
    {
        if (ret >= 0)
            break; 
        ret = D3DRenderParams.d3d_if->lpVtbl->CreateDevice(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_FPU_PRESERVE | D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING, 
            &D3DRenderParams.presentParams, &D3DRenderParams.d3dDev);
    }
    tries = 0;
    while (ret < 0)
    {
        ret = D3DRenderParams.d3d_if->lpVtbl->CreateDevice(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &D3DRenderParams.presentParams, &D3DRenderParams.d3dDev);
        if (++tries >= 2)
        {
            if (ret < 0)
            {
                MessageBoxA(hWnd, "IDirect3D9_CreateDevice failed, please re-run settings application", "Beyond Good & Evil", MB_TASKMODAL | MB_ICONERROR);
                exit(1);
            }
            break;
        }
    }
    if (!tweakFlagsDisabled && D3DRenderParams.d3dDev->lpVtbl->SetDialogBoxMode(D3DRenderParams.d3dDev, TRUE) < 0)
        OutputDebugStringA("SetDialogBoxMode failed\n");
    D3DRenderParams.d3d_if->lpVtbl->GetDeviceCaps(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, &(D3DRenderParams.d3dcaps));
    D3DRenderParams.setsDefaultZEnableAtBeginScene = 1;
    if (D3DRenderParams.presentParams.BackBufferCount > 1)
    {
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);
        D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, 0, 0, 0, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);
    D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, 0, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    D3D_InitEverything(dd);
    D3D_SunSurfacesClear();
    libHandle = LoadLibraryA("d3d9.dll");
    if (libHandle)
    {
        DisableD3DSpy = (void (*)(void))GetProcAddress(libHandle, "DisableD3DSpy");
        if (DisableD3DSpy)
            DisableD3DSpy();
    }
    return 0;
}

void D3D_SetRenderTargetAndDepthStencilSurface()
{
    IDirect3DSurface9* v1;

    D3DRenderParams.blurTex->lpVtbl->GetSurfaceLevel(D3DRenderParams.blurTex, 0, &v1);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, v1);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, D3DRenderParams.newZStencil);
    v1->lpVtbl->Release(v1);
}

void D3D_BeginSceneCall()
{
    D3D_SetRenderTargetAndDepthStencilSurface();
    if (!D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->BeginScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 1;
    }
    D3DRenderParams.d3d_defaultZEnable = D3DRenderParams.setsDefaultZEnableAtBeginScene;
}

void D3D_Request_SetFogParams(tFogParams* a1)
{
    int fogColor;
    float v2;

    if ((a1->flags & 1) != 0)
    {
        if ((D3DRenderParams.d3dcaps.RasterCaps & 0x100) != 0)
        {
            if (renderStateConfig.d3dFogTableMode != engineFogTableModeToD3D[a1->fogVertexMode])
            {
                renderStateConfig.d3dFogTableMode = engineFogTableModeToD3D[a1->fogVertexMode];
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGTABLEMODE, renderStateConfig.d3dFogTableMode);
            }
        LABEL_8:
            fogColor = (a1->fogColor & 0xFF00FF00 | ((a1->fogColor >> 16) & 0xff) | ((uint8_t)a1->fogColor << 16));
            D3DRenderParams.d3dFogColor = fogColor;
            D3DRenderParams.d3dFogColor2Case5and6 = 0;
            if (renderStateConfig.d3dFogDensity != a1->density)
            {
                renderStateConfig.d3dFogDensity = a1->density;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGDENSITY, *(uint32_t *)&(renderStateConfig.d3dFogDensity));
                fogColor = D3DRenderParams.d3dFogColor;
            }
            D3DRenderParams.d3dFogModeProbably = 1;
            if (renderStateConfig.d3dFogColor != fogColor)
            {
                renderStateConfig.d3dFogColor = fogColor;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGCOLOR, fogColor);
            }
            if (renderStateConfig.d3dFogStart != a1->fogStart)
            {
                renderStateConfig.d3dFogStart = a1->fogStart;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGSTART, *(uint32_t*)&(renderStateConfig.d3dFogStart));
            }
            v2 = a1->fogEnd;
            if (renderStateConfig.d3dFogEnd != v2)
            {
                renderStateConfig.d3dFogEnd = a1->fogEnd;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGEND, *(uint32_t*)&v2);
            }
            if (renderStateConfig.d3dFogEnable != TRUE)
            {
                renderStateConfig.d3dFogEnable = TRUE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, TRUE);
            }
            return;
        }
        if ((D3DRenderParams.d3dcaps.RasterCaps & 0x80) != 0)
        {
            if (renderStateConfig.d3dFogVertexMode != engineFogTableModeToD3D[a1->fogVertexMode])
            {
                renderStateConfig.d3dFogVertexMode = engineFogTableModeToD3D[a1->fogVertexMode];
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGVERTEXMODE, renderStateConfig.d3dFogVertexMode);
            }
            goto LABEL_8;
        }
    }
    D3DRenderParams.d3dFogModeProbably = 0;
    if (renderStateConfig.d3dFogEnable)
    {
        renderStateConfig.d3dFogEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, FALSE);
    }
}

int D3D_FillWaterStreamData(struct sWaterParamsGlobalStruct* a1)
{
    float camPosX;
    float camPosZ;
    int v3;
    struct sWaterParamsGlobalStruct* v4;
    unsigned int verticalVertexNum;
    unsigned int v9;
    double v10;
    float v11;
    float v12;
    double v13;
    int v14;
    int v15;
    uint8_t v16;
    double v17;
    tVertex_type_water* v19;
    unsigned int i;
    float ccamPosX;
    float camPosY;
    float ccamPosZ;
    float v24;
    Vec3* v25;
    tVertex_type_water v26;

    camPosX = gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
    camPosZ = gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
    camPosY = gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
    ccamPosZ = camPosZ;
    ccamPosX = camPosX;
    waterStreamData->lpVtbl->Lock(waterStreamData, 0, 0, &v19, 0x2000);
    v3 = (uint8_t)v19 & 0xF;
    // align 16
    waterOffsetInBytes = 16 - v3;
    if (v3 != 16) 
        v19 = (tVertex_type_water*)((uint8_t*)v19 + 16 - v3);
    v4 = a1;
    Vec3* _ESI = a1->vertices;
    int* _EBX = a1->colors;
    Vec3* _EDI = a1->vecsForUVs;
    v26.specular = 0;
    if (tweak_water_blur_normal)
    {
        WTR_WaterBlurNormal(a1->vecsForUVs, a1->horizontalVertexNum, a1->verticallVertexNum); 
        v4 = a1;
    }
    verticalVertexNum = v4->verticallVertexNum;

    v9 = 0;
    for (i = 0; v9 < verticalVertexNum; i = v9)
    {
        v25 = (Vec3*)verticalVertexNum;
        v10 = (double)v9 * 1.2 / (double)verticalVertexNum;
        if (v10 > 1.0)
            v10 = 1.0;
        v25 = &_ESI[v4->horizontalVertexNum];
        v24 = (float)(48.0 - v10 * 45.0);
        if (_ESI < v25)
        {
            do
            {
                v11 = _ESI->y;
                v12 = _ESI->z;
                v26.v.x = _ESI->x;

                v26.normal.x = v26.v.x - ccamPosX;
                v26.v.y = v11;
                v13 = v11;
                v14 = *((uint8_t*)_EBX + 2);
                v26.v.z = v12;
                v15 = *_EBX;
                v16 = *_EBX;
                v26.normal.y = (float)(v13 - camPosY);
                v17 = _EDI->y + _EDI->x;
                v26.diffuse = v15 & 0xFF00FF00 | v14 | (v16 << 16);
                ++_ESI;
                ++_EBX;
                ++_EDI;
                v26.normal.z = (float)(v26.v.z - ccamPosZ - fabs(v17 * v24));
                memcpy(v19, &v26, sizeof(v26));
                ++v19;
            } while (_ESI < v25);
            v9 = i;
            v4 = a1;
        }
        verticalVertexNum = v4->verticallVertexNum;
        ++v9;
    }
    return waterStreamData->lpVtbl->Unlock(waterStreamData);
}

void D3D_SetTransformAndSaveMatrix(IDirect3DTexture9* a1, int mtxnum)
{
    double v2;
    double v3;
    IDirect3DDevice9* v4;
    float v5;
    IDirect3DSurface9* v6;
    float v7;
    D3DMATRIX v8;
    D3DSURFACE_DESC v9;
    D3DMATRIX v10;
    D3DMATRIX v11;

    memcpy(&v10, &d3dProjectionMtx, sizeof(v10));
    v10._22 = -v10._22;
    memset(&v8, 0, sizeof(v8));
    a1->lpVtbl->GetSurfaceLevel(a1, 0, &v6);
    v6->lpVtbl->GetDesc(v6, &v9);
    v7 = (float)v9.Width;
    v5 = (float)v9.Height;
    v6->lpVtbl->Release(v6);
    v8._33 = 1.0;
    v8._44 = 1.0;
    v8._43 = 0.0;
    v8._11 = (float)(512.0 / v7 * 0.5);
    v2 = 1.0 / v5;
    v3 = 512.0 * v2 * 0.5;
    v8._22 = (float)(v3 - v2);
    v8._41 = v8._11;
    v8._42 = (float)(v3 + v2);
    D3DXMatrixMultiply(&v11, &v10, &v8);
    v4 = D3DRenderParams.d3dDev;
    memcpy(&d3dTransformationMatrix[mtxnum], &v11, sizeof(Mtx4x4WithoutAdditionalData));
    v4->lpVtbl->SetTransform(v4, mtxnum + D3DTS_TEXTURE0, &v11);
}

IDirect3DTexture9* D3D_GetWaterTexture()
{
    water_ClippingHasBeenSet = 0;
    if (gMaxSimultaneousTextures >= 4 && tweak_water_smooth)
        D3D_DoBlurWaterTexture();
    return waterTexture;
}


void D3D_DrawWater_DrawVertexTable(struct sWaterParamsGlobalStruct* a1)
{
    unsigned int v1;
    IDirect3DTexture9* v2;
    Mtx4x4 a2;
    Mtx4x4 a1a;
    Mtx4x4 v5;
    Mtx4x4 v6;

    v1 = a1->horizontalVertexNum;
    wtr_NumB = a1->verticallVertexNum;
    wtr_NumA = v1;
    D3D_WaterBuffersInit();
    D3D_FillWaterStreamData(a1);
    if (&a2 != &gpst_DX_stDD->ct.cam.objectMtx)
        memcpy(&a2, &gpst_DX_stDD->ct.cam.objectMtx, sizeof(a2));
    MATH_InvertMatrix(&a1a, &a2);
    a1a._m34 = 0.0;
    a1a._m24 = 0.0;
    a1a._m14 = 0.0;
    a1a.w = 1.0;
    MATH_MakeOGLMatrix(&v5, &a1a);
    a2.v1.x = 1.0;
    a2.v2.y = 1.0;
    a2.v3.z = 1.0;
    a2.w = 1.0;
    v5.w = 1.0;
    a2.v1.y = 0.0;
    a2.v1.z = 0.0;
    a2._m14 = 0.0;
    a2.v2.x = 0.0;
    a2.v2.z = 0.0;
    a2._m24 = 0.0;
    a2.v3.x = 0.0;
    a2.v3.y = 0.0;
    a2._m34 = 0.0;
    a2.Pos.x = 0.0;
    a2.Pos.y = 0.0;
    a2.Pos.z = 0.0;
    a2.mtxflag = a2.mtxflag & 2;
    MATH_MakeOGLMatrix(&v6, &a2);
    D3D_SaveEverythingToBackup(1);
    v2 = D3D_GetWaterTexture();
    memcpy(&d3dTransformMatrix, &v5, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&v5);
    memcpy(&d3dTransformMatrix2, &v6, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, (D3DMATRIX*)&v6);
    D3D_SetTransformAndSaveMatrix(v2, 0);
    if (textureStageTypeConfig[0].d3dTexCoordIndex != 0x10000)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0x10000;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 11, 0x10000);
    }
    if (textureStageTypeConfig[0].d3dTextureTransformFlags != 0x103)
    {
        textureStageTypeConfig[0].d3dTextureTransformFlags = 0x103;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 24, 0x103);
    }
    if (renderStateConfig.d3d_normalizeNormals)
    {
        renderStateConfig.d3d_normalizeNormals = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 143, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, waterStreamData, waterOffsetInBytes, 32);
    D3DRenderParams.d3dDev->lpVtbl->SetIndices(D3DRenderParams.d3dDev, waterIndexData);
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, 210);
    if (renderStateConfig.d3dAlphaBlendEnable != 1)
    {
        renderStateConfig.d3dAlphaBlendEnable = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 1);
    }
    if (renderStateConfig.d3d_Lighting)
    {
        renderStateConfig.d3d_Lighting = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 137, 0);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 12)
    {
        textureStageTypeConfig[0].d3d_colorOp = 12;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 12);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 1);
    }
    if (samplerConfig[0].d3d_magfilter != 2)
    {
        samplerConfig[0].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    if (samplerConfig[0].d3d_minfilter != 2)
    {
        samplerConfig[0].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 6, 2);
    }
    if (samplerConfig[0].d3d_addressu != 3)
    {
        samplerConfig[0].d3d_addressu = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 1, 3);
    }
    if (samplerConfig[0].d3d_addressv != 3)
    {
        samplerConfig[0].d3d_addressv = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 2, 3);
    }
    if (renderStateConfig.d3dSrcBlend != 2)
    {
        renderStateConfig.d3dSrcBlend = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 2);
    }
    if (renderStateConfig.d3dDestBlend != 1)
    {
        renderStateConfig.d3dDestBlend = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)v2);
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
    if (renderStateConfig.d3dFogEnable != 1)
    {
        renderStateConfig.d3dFogEnable = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 28, 1);
    }
    if (renderStateConfig.d3d_zWriteEnable)
    {
        renderStateConfig.d3d_zWriteEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 14, 0);
    }
    if (renderStateConfig.d3d_zFunc != 4)
    {
        renderStateConfig.d3d_zFunc = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 23, 4);
    }
    //D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    D3DRenderParams.d3dDev->lpVtbl->DrawIndexedPrimitive(D3DRenderParams.d3dDev,
        5,
        0,
        0,
        a1->verticallVertexNum * a1->horizontalVertexNum,
        0,
        wtr_primitiveCount);
    //D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_SOLID);
    D3D_RestoreEverythingFromBackup();
    if (renderStateConfig.d3d_zWriteEnable != 1)
    {
        renderStateConfig.d3d_zWriteEnable = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 14, 1);
    }
    if (renderStateConfig.d3d_zFunc != 4)
    {
        renderStateConfig.d3d_zFunc = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 23, 4);
    }
}

void D3D_PrepareWaterRendering()
{
    float v0[4];

    if (pst_GlobalsWaterParams)
    {
        QueryPerformanceCounter(&PerfCounterStruct[48]);
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &d3dRTToGet);
        D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &d3dWaterDSSurfaceToGet);
        D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dWaterRTtoSet);
        D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dDSSurfaceToSet);
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, 0, 3, pst_GlobalsWaterParams->waterEngine.bottomColor, 1.0f, 0);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dRTToGet);
        D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dWaterDSSurfaceToGet);
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
        if (d3dRTToGet)
        {
            d3dRTToGet->lpVtbl->Release(d3dRTToGet);
            d3dRTToGet = NULL;
        }
        if (d3dWaterDSSurfaceToGet)
        {
            d3dWaterDSSurfaceToGet->lpVtbl->Release(d3dWaterDSSurfaceToGet);
            d3dWaterDSSurfaceToGet = NULL;
        }
        MATH_MakeOGLMatrix(&d3dCameraMtx, &gpst_DX_stDD->ct.cam.cameraMtx);
        if (tweak_water_clip)
        {
            v0[3] = tweak_water_off + waterHeight;
            v0[0] = 0.0;
            v0[1] = 0.0;
            v0[2] = -1.0;
            D3DRenderParams.d3dDev->lpVtbl->SetClipPlane(D3DRenderParams.d3dDev, 0, v0);
        }
        PRO_QPC_RecordTimeDecRef(48);
        water_ClippingHasBeenSet = 1;
    }
    else
    {
        water_ClippingHasBeenSet = 0;
    }
}

int D3D_setWaterRenderParamsIfWaterRenderingIsActive()
{
    tGameObjectBV* v0;
    long double v1;
    Mtx4x4 a1;

    if (!water_ClippingHasBeenSet)
        return 0;
    v0 = gpst_DX_stDD->currTargetGao->boundingVolume;
    v1 = OBJ_BV_IsSphere(v0) ? v0->BVMax.x : fabs(v0->BVMax.y - v0->BVMin.y)
        + fabs(v0->BVMax.x - v0->BVMin.x);
    if (v1 < tweak_water_min_bv)
        return 0;
    QueryPerformanceCounter(&PerfCounterStruct[48]);
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &d3dRTToGet);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &d3dWaterDSSurfaceToGet);
    D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dWaterRTtoSet);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dDSSurfaceToSet);
    d3dCullModeBackup = renderStateConfig.d3dCullMode;
    if (renderStateConfig.d3dCullMode == 2)
    {
        if (renderStateConfig.d3dCullMode != 3)
        {
            renderStateConfig.d3dCullMode = 3;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 22, 3);
        }
    }
    else if (renderStateConfig.d3dCullMode == 3 && renderStateConfig.d3dCullMode != 2)
    {
        renderStateConfig.d3dCullMode = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 22, 2);
    }
    memcpy(&d3dTransformMatrixBackup, &d3dTransformMatrix, sizeof(d3dTransformMatrix));
    memcpy(&d3dTransformMatrix2Backup, &d3dTransformMatrix2, sizeof(d3dTransformMatrix2));
    MATH_MakeOGLMatrix(&a1, gpst_DX_stDD->currTargetGao->matrices);
    a1.v1.z = -a1.v1.z;
    a1.v2.z = -a1.v2.z;
    a1.v3.z = -a1.v3.z;
    a1.Pos.z = a1.Pos.z - (a1.Pos.z - waterHeight + a1.Pos.z - waterHeight);
    memcpy(&d3dTransformMatrix2, &a1, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, (D3DMATRIX*)&a1);
    memcpy(&d3dTransformMatrix, &d3dCameraMtx, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&d3dCameraMtx);
    if (tweak_water_clip)
    {
        if (renderStateConfig.d3dClipPlaneEnable != 1)
        {
            renderStateConfig.d3dClipPlaneEnable = 1;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 152, 1);
        }
    }
    return 1;
}

void D3D_RestoreRenderParamsAfterWaterRendering()
{
    if (tweak_water_clip && renderStateConfig.d3dClipPlaneEnable)
    {
        renderStateConfig.d3dClipPlaneEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 152, 0);
    }
    memcpy(&d3dTransformMatrix, &d3dTransformMatrixBackup, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, &d3dTransformMatrixBackup);
    memcpy(&d3dTransformMatrix2, &d3dTransformMatrix2Backup, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, &d3dTransformMatrix2Backup);
    if (renderStateConfig.d3dCullMode != d3dCullModeBackup)
    {
        renderStateConfig.d3dCullMode = d3dCullModeBackup;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 22, d3dCullModeBackup);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dRTToGet);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dWaterDSSurfaceToGet);
    D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
    if (d3dRTToGet)
    {
        d3dRTToGet->lpVtbl->Release(d3dRTToGet);
        d3dRTToGet = NULL;
    }
    if (d3dWaterDSSurfaceToGet)
    {
        d3dWaterDSSurfaceToGet->lpVtbl->Release(d3dWaterDSSurfaceToGet);
        d3dWaterDSSurfaceToGet = NULL;
    }
    PRO_QPC_RecordTimeDecRef(48);
}

tVertex_type_142_generic* D3D_LockCurrentVBO(int a1)
{
    int v1;
    int v2;
    int v3;
    tVertex_type_142_generic* result;

    v1 = d3dStreamCurrentOffset;
    v2 = 24 * a1;
    v3 = 4096;
    if ((unsigned int)(d3dStreamCurrentOffset + sizeof(tVertex_type_142_generic) * a1) >= 2097152)
    {
        v1 = 0;
        d3dStreamCurrentOffset = 0;
        v3 = 8192;
    }
    VBOs[currentVBOSelected]->lpVtbl->Lock(VBOs[currentVBOSelected], v1, sizeof(tVertex_type_142_generic) * a1, (void **)&result, v3);
    d3dStreamActDataSize = v2;
    return result;
}

void D3D_FillCurrentVBO(int a1)
{
    VBOs[currentVBOSelected]->lpVtbl->Unlock(VBOs[currentVBOSelected]);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, a1, VBOs[currentVBOSelected], d3dStreamCurrentOffset, sizeof(tVertex_type_142_generic));
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ);
    d3dStreamCurrentOffset += d3dStreamActDataSize;
    d3dStreamActDataSize = 0;
}

void D3D_Request_SpriteDraw()
{
    if (d3d_requestDrawSelectedVBO)
    {
        d3d_requestDrawSelectedVBO = NULL;
        D3D_FillCurrentVBO(0);
        D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, d3dRequestPrimitiveCount);
    }
}

void D3D_Request_PrepareForDrawing(int a1)
{
    if (a1)
    {
        if (renderStateConfig.d3dFillMode != 3)
        {
            renderStateConfig.d3dFillMode = 3;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 8, 3);
        }
        if (renderStateConfig.d3dCullMode != 2)
        {
            renderStateConfig.d3dCullMode = 2;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 22, 2);
        }
        d3dRequestPrimitiveCount = 2 * a1;
        d3dRequestPrimitiveCount2 = 2 * a1;
        d3d_requestDrawSelectedVBO = D3D_LockCurrentVBO(6 * a1);
    }
}

void D3D_FillSelectedVBOWithSprite(tD3dSpriteData* a1)
{
    tVertex_type_142_generic* v1;
    unsigned int v2;
    tVertex_type_142_generic* v3;

    v1 = d3d_requestDrawSelectedVBO;
    if (d3d_requestDrawSelectedVBO)
    {
        if (d3dRequestPrimitiveCount2)
        {
            v2 = a1->color & 0xFF00FF00 | ((a1->color >> 16) & 0xff) | ((uint8_t)a1->color << 16);
            d3d_requestDrawSelectedVBO->v1.x = a1->v1.x;
            v1->v1.y = a1->v1.y;
            v1->v1.z = a1->v1.z;
            v1->color = v2;
            v1->uvSet1.u = 1.0f;
            v1->uvSet1.v = 1.0f;
            v1[1].v1 = a1->v2;
            v1[1].color = v2;
            v1[1].uvSet1.u = 0.0f;
            v1[1].uvSet1.v = 1.0f;
            v1[2].v1 = a1->v3;
            v1[2].color = v2;
            v1[2].uvSet1.u = 0.0f;
            v1[2].uvSet1.v = 0.0f;
            v3 = v1 + 3;
            v3->v1.x = a1->v1.x;
            v3->v1.y = a1->v1.y;
            v3->v1.z = a1->v1.z;
            v3->color = v2;
            v3->uvSet1.u = 1.0f;
            v3->uvSet1.v = 1.0f;
            v3[1].v1 = a1->v3;
            v3[1].color = v2;
            v3[1].uvSet1.u = 0.0f;
            v3[1].uvSet1.v = 0.0f;
            v3[2].v1 = a1->v4;
            v3[2].color = v2;
            v3[2].uvSet1.u = 1.0f;
            v3[2].uvSet1.v = 0.0f;
            d3d_requestDrawSelectedVBO = v3 + 3;
            d3dRequestPrimitiveCount2 -= 2;
        }
    }
}

int D3D_l_Request(int type, void *param)
{
    int result;

    switch (type)
    {
    case 1:
        D3D_Request_SetFogParams((tFogParams*)param);
        result = 0;
        break;
    case 12:
        D3D_TextureStore((tTexUploadRequest*)param);
        result = 0;
        break;
    case 18:
        D3D_Request_PrepareForDrawing((int)param);
        result = 0;
        break;
    case 19:
    case 33:
        D3D_FillSelectedVBOWithSprite((tD3dSpriteData*)param);
        result = 0;
        break;
    case 20:
        D3D_Request_SpriteDraw();
        result = 0;
        break;
    case 24:
        D3D_GetSnapShot((tD3dRequestSnapshot*)param);
        result = 0;
        break;
    case 25:
        if (renderStateConfig.d3dFogEnable != (int)param)
        {
            renderStateConfig.d3dFogEnable = (int)param;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, (int)param);
        }
        goto LABEL_10;
    default:
    LABEL_10:
        result = 0;
        break;
    }
    return result;
}

int D3D_l_Texture_Store(struct sGdi_DisplayData* a1, int a2)
{
    int v2;
    unsigned int v3;

    v2 = a2;
    if (!a2)
        v2 = 1;
    v3 = 4 * (v2 + 1);
    D3DRenderParams.d3dTextureNum = v2;
    D3DRenderParams.d3dTextures = (IDirect3DTexture9 **)MEM_p_Alloc(v3);
    memset(D3DRenderParams.d3dTextures, 0, v3);
    D3DRenderParams.d3dSourceTextures = (IDirect3DTexture9**)MEM_p_Alloc(v3);
    memset(D3DRenderParams.d3dSourceTextures, 0, v3);
    return 1;
}

void D3D_Palette_Load(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* a3, int idx)
{
    tEngTexIntermediateStruct* v4;
    tEngTexListElemStruct* v5;
    int v6;
    int v7;
    int v8;
    __int16 v9;
    int v10;
    int v11;
    int v12;
    IDirect3DTexture9* v13;
    IDirect3DTexture9* v14;
    char* v15;
    char* v16;
    unsigned int v17;
    int v18;
    IDirect3DSurface9* v19;
    char* v20;
    D3DLOCKED_RECT v21; 
    IDirect3DTexture9* v22;
    IDirect3DTexture9* v23;
    int v20a;

    v4 = a3;
    v5 = tle;
    v6 = a3->colorDepth2;
    v7 = tle->normalData.width;
    v20 = a3->imgDataPtr;
    v8 = tle->normalData.height;
    if (!D3DRenderParams.Mipmapping
        || (v9 = a3->tp.flagsUnkAndProceduralUnkb1, (v9 & 0x40) != 0)
        || (v9 & 0x200) != 0
        || a3->tp.type == 5)
    {
        a3->bmpUnkb1 &= ~0x10u;
        v10 = 1;
    }
    else
    {
        a3->bmpUnkb1 |= 0x10u;
        v10 = 0;
    }
    v5->texElemFlag = v4->bmpUnkb1;
    v22 = 0;
    v23 = 0;
    if (v6 != 32 && v6 != 24)
    {
        v11 = idx;
        D3DRenderParams.d3dTextures[idx] = NULL;
        D3DRenderParams.d3dSourceTextures[v11] = NULL;
        return;
    }
    if (D3DRenderParams.Mipmapping && D3DRenderParams.argbIsSupportedForTextures == 1)
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, v8, v7, 1, 0, 21, 2, &v22, 0);
        v12 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev,
            v8,
            v7,
            v10,
            0x400,
            21,
            0,
            &v23,
            0);
    }
    else
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, v8, v7, v10, 0, 21, 2, &v22, 0);
        v12 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev,
            v8,
            v7,
            v10,
            0,
            21,
            0,
            &v23,
            0);
    }
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v12);
    v13 = v23;
    if (v23)
    {
        v14 = v22;
        if (v22)
        {
            if (v6 == 32)
                dd->texManager.paletteUsedMemory += 4 * v8 * v7;
            else
                dd->texManager.paletteUsedMemory += 3 * v8 * v7;
            v14->lpVtbl->GetSurfaceLevel(v14, 0, &v19);
            v19->lpVtbl->LockRect(v19, &v21, 0, 0);
            v15 = (char*)v21.pBits;
            v16 = v20;
            if (v7)
            {
                v17 = 4 * v8;
                v20a = v7;
                do
                {
                    memcpy(v15, v16, v17);
                    v16 += v17;
                    v15 += v21.Pitch;
                    --v20a;
                } while (v20a);
            }
            v19->lpVtbl->UnlockRect(v19);
            v19->lpVtbl->Release(v19);
            if (D3DRenderParams.Mipmapping)
            {
                if (D3DRenderParams.argbIsSupportedForTextures != 1)
                    D3DXFilterTexture((IDirect3DBaseTexture9*)v22, NULL, D3DX_DEFAULT, D3DX_DEFAULT);
            }
            D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)v22, (IDirect3DBaseTexture9*)v23);
            goto LABEL_29;
        }
        v23->lpVtbl->Release(v23);
        v13 = NULL;
        v23 = NULL;
    }
    if (v22)
    {
        v22->lpVtbl->Release(v22);
        v22 = NULL;
    LABEL_29:
        v13 = v23;
    }
    v18 = idx;
    D3DRenderParams.d3dTextures[idx] = v13;
    D3DRenderParams.d3dSourceTextures[v18] = v22;
}

int D3D_DrawSprites(struct sGeoSprites* a1, void* a2, int x)
{
    tBigBufferStruct* v3;
    int* v4;
    int* v5;
    int v6; 
    int v7; 
    Mtx4x4* v8;
    int v9;
    tGeoSpriteElem* v10;
    tGeoSpriteElem* v11;
    int v12;
    tSpriteC0DEData* v13;
    int* v14;
    double v15;
    int v16;
    double v17;
    Vec3* v18;
    double v20;
    double v22;
    float v24;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    int* v32; 
    tSpriteC0DEData* v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    tGdi_DisplayData* v45;
    float v46;
    float v47;
    float v48;
    float v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    int v60;
    int v61;
    tD3dSpriteData v62;

    v3 = gpst_DX_stDD->bigBuffer;
    v4 = v3->vertexColorArray;
    v5 = v3->vertexColorArraySecond;
    v6 = v3->solidVertexColor;
    v7 = v3->colorMask;
    v8 = gpst_DX_stDD->matrixStack.actMatrixElem;
    v32 = v4;
    v61 = v7;
    v9 = v7 ^ (D3DRenderParams.colorMaskAlpha | v6);
    v45 = gpst_DX_stDD;
    v60 = D3DRenderParams.colorMaskAlpha;
    v29 = -(v8->v1.x * a1->globalSize * a1->globalRatio);
    v30 = -(v8->v2.x * a1->globalSize * a1->globalRatio);
    v31 = -(v8->v3.x * a1->globalSize * a1->globalRatio);
    v24 = v8->v1.y * a1->globalSize;
    v27 = v8->v2.y * a1->globalSize;
    v28 = v8->v3.y * a1->globalSize;
    gpst_DX_stDD->Request(18, (void *) a1->num);
    v10 = a1->spriteElems;
    v11 = &v10[a1->num];
    if (v10->idx == 0xC0DE) // see gfxexplode.c GFX_SetNumberOfSprites
    {
        v12 = v10[1].idx;
        v13 = &((tSpriteC0DEData *)a2)[v12];
        v14 = v32;
        v33 = &v13[a1->num];
        if (v32)
        {
            v14 = &v32[v12];
            v32 = v14;
        }
        if (v5)
            v5 += v12;
        if (v13 < &v13[a1->num])
        {
            while (1)
            {
                if (v14)
                {
                    v9 = v61 ^ (v60 | *v14);
                    if (v5)
                        v9 = v5[v10->idx] | v9 & 0xFFFFFF;
                    v32 = v14 + 1;
                }
                else if (v5)
                {
                    v9 = v5[v10->idx] | v9 & 0xFFFFFF;
                }
                v62.color = v9;
                v15 = v13->f1;
                v46 = -v29 - v24;
                v62.v1.x = (float)(v46 * v15 + v13->v1.x);
                v41 = -v30 - v27;
                v62.v1.y = (float)(v41 * v15 + v13->v1.y);
                v58 = -v31 - v28;
                v62.v1.z = (float)(v58 * v15 + v13->v1.z);
                v56 = v29 - v24;
                v62.v2.x = (float)(v56 * v15 + v13->v1.x);
                v39 = v30 - v27;
                v62.v2.y = (float)(v39 * v15 + v13->v1.y);
                v52 = v31 - v28;
                v62.v2.z = (float)(v52 * v15 + v13->v1.z);
                v43 = v24 + v29;
                v62.v3.x = (float)(v43 * v15 + v13->v1.x);
                v48 = v27 + v30;
                v62.v3.y = (float)(v48 * v15 + v13->v1.y);
                v35 = v28 + v31;
                v62.v3.z = (float)(v35 * v15 + v13->v1.z);
                v50 = v24 - v29;
                v62.v4.x = (float)(v50 * v15 + v13->v1.x);
                v54 = v27 - v30;
                v62.v4.y = (float)(v54 * v15 + v13->v1.y);
                v37 = v28 - v31;
                v62.v4.z = (float)(v37 * v15 + v13->v1.z);
                v45->Request(33, &v62);
                v13++;
                if (v13 >= v33)
                    break;
                v14 = v32;
            }
        }
    }
    else
    {
        v34 = 0.0;
        if (v10 < v11)
        {
            v47 = -v29 - v24;
            v42 = -v30 - v27;
            v57 = v29 - v24;
            v40 = v30 - v27;
            v44 = v24 + v29;
            v49 = v27 + v30;
            v51 = v24 - v29;
            v55 = v27 - v30;
            do
            {
                v34 = v34 + 1.0f;
                if (v32)
                {
                    v16 = v10->idx;
                    v9 = v61 ^ (v60 | v32[v16]);
                    if (v5)
                        v9 = v5[v16] | v9 & 0xFFFFFF;
                }
                else if (v5)
                {
                    v9 = v5[v10->idx] | v9 & 0xFFFFFF;
                }
                v62.color = v9;
                v17 = v10->size;
                v18 = &(((Vec3*)a2)[v10->idx]);
                v62.v1.x = (float)(v47 * v17 + v18->x);
                v62.v1.y = (float)(v42 * v17 + v18->y);
                v59 = -v31 - v28;
                v62.v1.z = (float)(v59 * v17 + v18->z);
                v62.v2.x = (float)(v57 * v17 + v18->x);
                v62.v2.y = (float)(v40 * v17 + v18->y);
                v53 = v31 - v28;
                v62.v2.z = (float)(v53 * v17 + v18->z);
                v62.v3.x = (float)(v44 * v17 + v18->x);
                v62.v3.y = (float)(v49 * v17 + v18->y);
                v36 = v28 + v31;
                v62.v3.z = (float)(v36 * v17 + v18->z);
                v62.v4.x = (float)(v51 * v17 + v18->x);
                v62.v4.y = (float)(v55 * v17 + v18->y);
                v38 = v28 - v31;
                v62.v4.z = (float)(v38 * v17 + v18->z);
                if (GDI_gb_WaveSprite)
                {
                    v20 = MATH_GetSin(v34);
                    v22 = v20 * MATH_GetSin((float)(v20 * TIM_gul_TicksCount * 2.5)) * 0.2;

                    v62.v1.x = (float)(v62.v1.x + v22);
                    v62.v2.x = (float)(v22 + v62.v2.x);
                }
                v45->Request(33, &v62);
                ++v10;
            } while (v10 < v11);
        }
    }
    v45->Request(20, 0);
    return 0;
}

int D3D_l_DrawElementIndexedTriangles(struct sGeoTriangleChunks* triangleChunks, Vec3* verticesIN, struct sGeoUVElem* UVs, int vertexNum)
{
    int v3;
    tGEOGRO* v4;
    Vec3* v5;
    int v6;
    int v7;
    tBigBufferStruct* v8;
    int v9;
    int* v10;
    BOOL v11;
    int v12;
    tGameObjectMain* v13;
    unsigned int* v14;
    int* v15;
    int* v16;
    int v17; 
    int v18;
    unsigned int v19;
    tGeoWholeTriangleStripBlock* v20;
    tD3dStreamInfoElem* v21;
    IDirect3DVertexBuffer9** v22;
    IDirect3DDevice9* v23;
    unsigned int v24;
    tGeoWholeTriangleStripBlock* v25;
    tD3dStreamInfoElem* v26;
    IDirect3DVertexBuffer9** v27;
    IDirect3DDevice9* v28;
    tVertex_type_142_generic* v30;
    tBigBufferStruct* v31;
    int v33;
    int* v34;
    unsigned int v35;
    tGeoTriangleChunkElem* v36;
    tGeoTriangleChunkElem* v37;
    char v38;
    Vec3* v39;
    int v40;
    int v41;
    unsigned int v42;
    Vec3* v43;
    tVertex_type_142_generic* v44;
    int v45;
    int v46;
    unsigned int v47;
    Vec3* v48;
    tVertex_type_142_generic* v49;
    int v50;
    int v51;
    unsigned int v52;
    Vec3* v53;
    int v54;
    int v55;
    unsigned int v56;
    Vec3* v57;
    tVertex_type_142_generic* v58;
    int v59;
    int v60;
    unsigned int v61;
    Vec3* v62;
    tVertex_type_142_generic* v63;
    int v64;
    int v65;
    unsigned int v66;
    Vec3* v67;
    Vec3* v68;
    tVertex_type_142_generic* v69;
    Vec3* v70;
    Vec3* v71;
    Vec3* v72;
    tVertex_type_142_generic* v73;
    Vec3* v74;
    Vec3* v75;
    int v76;
    int v77;
    unsigned int v78;
    Vec3* v79;
    tVertex_type_142_generic* v80;
    int v81;
    int v82;
    unsigned int v83;
    Vec3* v84;
    tVertex_type_142_generic* v85;
    int v86;
    unsigned int v87;
    Vec3* v88;
    Vec3* v89;
    Vec3* v90;
    unsigned int v91 = 0;// TODO FIXME ???????? játék nem initeli
    uint8_t v92;
    uint8_t v93;
    uint8_t v94;
    uint8_t v95;
    uint8_t v96;
    uint8_t v97;
    uint8_t v98;
    uint8_t v99;
    uint8_t v100;
    int* v101;
    int v102;
    unsigned int v103;
    tGeoWholeTriangleStripBlock* v104;
    unsigned int v105;
    int v106;
    int v107;
    int triangleChunksa;
    int triangleChunksb;
    int triangleChunksc;
    tGeoTriangleChunkElem* triangleChunksd;
    int* v32a;
    int* v104a;
    void* v103a;
    unsigned int* v102a;
    int *v105a;
    unsigned int i;

    v3 = 0;
    v105 = triangleChunks->triangleNum;
    if (!v105)
        return 0;
    v4 = gpst_DX_stDD->GROGEO;
    if (!v4)
        goto LABEL_66;
    if (v4->UVs != UVs && D3DRenderParams.textureUVMode != 2)
        v3 = 1;
    v5 = v4->vertices;
    if (verticesIN == v5 && (gpst_DX_stDD->flags & 0x400) != 0)
        v3 = 1;
    if (!v4->d3dIndexBuf || (v104 = triangleChunks->triangleStripBlock) == 0 || v3)
    {
    LABEL_66:
        v106 = 0;
        v107 = 0;
        v30 = D3D_LockCurrentVBO(3 * v105);
        v31 = gpst_DX_stDD->bigBuffer;
        v32a = v31->vertexColorArraySecond;
        v33 = gpst_DX_stDD->pd3drp->colorMaskAlpha;
        v103 = v31->colorMask;
        v34 = v31->vertexColorArray;
        v101 = v34;
        v104a = v32a;
        v102 = v33;
        if (v34)
        {
            v35 = v91;
        }
        else
        {
            v35 = (v103 ^ (v33 | v31->solidVertexColor)) & 0xFF00FF00 | ((uint8_t)(v103 ^ (v33 | (v31->solidVertexColor & 0xff))) << 16) | (uint8_t)((v103 ^ (v33 | v31->solidVertexColor)) >> 16);
            v91 = v35;
        }
        if (D3DRenderParams.textureUVMode == 2)
        {
            v106 = textureStageTypeConfig[0].d3d_colorarg1;
            v107 = textureStageTypeConfig[0].d3d_colorarg1;
            if (textureStageTypeConfig[0].d3d_colorarg1)
            {
                textureStageTypeConfig[0].d3d_colorarg1 = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 0);
            }
            if (textureStageTypeConfig[0].d3d_colorarg2)
            {
                textureStageTypeConfig[0].d3d_colorarg2 = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
            }
            v35 = 0;
            v101 = 0;
            v91 = 0;
        }
        v36 = triangleChunks->triangleChunkElems;
        v37 = &v36[v105];
        triangleChunksd = v37;
        if (UVs)
        {
            v38 = gpst_DX_stDD->flags;
            if (v101)
            {
                if ((v38 & 2) != 0)
                {
                    if (v36 < v37)
                    {
                        do
                        {
                            v39 = &verticesIN[v36->v1];
                            v30->v1.x = v39->x;
                            v30->v1.y = v39->y;
                            v40 = v102;
                            v30->v1.z = v39->z;
                            v41 = v36->v1;
                            v42 = v103 ^ (v40 | v101[v41]);
                            v92 = (v42 >> 16) & 0xff;
                            if (v104a)
                            {
                                v42 = v104a[v41] | v42 & 0xFFFFFF;
                                v92 = (v42 >> 16) & 0xff;
                            }
                            v30->color = v42 & 0xFF00FF00 | ((uint8_t)v42 << 16) | v92;
                            v30->uvSet1 = UVs[v36->v1];
                            v43 = &verticesIN[v36->v2];
                            v44 = v30 + 1;
                            v44->v1.x = v43->x;
                            v44->v1.y = v43->y;
                            v45 = v102;
                            v44->v1.z = v43->z;
                            v46 = v36->v2;
                            v47 = v103 ^ (v45 | v101[v46]);
                            v93 = (v47 >> 16) & 0xff;
                            if (v104a)
                            {
                                v47 = v104a[v46 * 4] | v47 & 0xFFFFFF;
                                v93 = (v47 >> 16) & 0xff;
                            }
                            v44->color = v47 & 0xFF00FF00 | ((uint8_t)v47 << 16) | v93;
                            v44->uvSet1 = UVs[v36->v2];
                            v48 = &verticesIN[v36->v3];
                            v49 = v44 + 1;
                            v49->v1.x = v48->x;
                            v49->v1.y = v48->y;
                            v50 = v102;
                            v49->v1.z = v48->z;
                            v51 = v36->v3;
                            v52 = v103 ^ (v50 | v101[v51]);
                            v94 = (v52 >> 16) & 0xff;
                            if (v104a)
                            {
                                v52 = v104a[v51] | v52 & 0xFFFFFF;
                                v94 = (v52 >> 16) & 0xff;
                            }
                            v49->color = v52 & 0xFF00FF00 | ((uint8_t)v52 << 16) | v94;
                            v49->uvSet1 = UVs[v36->v3];
                            ++v36;
                            v30 = v49 + 1;
                        } while (v36 < triangleChunksd);
                    }
                }
                else if (v36 < v37)
                {
                    do
                    {
                        v53 = &verticesIN[v36->v1];
                        v30->v1.x = v53->x;
                        v30->v1.y = v53->y;
                        v54 = v102;
                        v30->v1.z = v53->z;
                        v55 = v36->v1;
                        v56 = v103 ^ (v54 | v101[v55]);
                        v95 = (v56 >> 16) & 0xff;
                        if (v104a)
                        {
                            v56 = v104a[v55] | v56 & 0xFFFFFF;
                            v95 = (v56 >> 16) & 0xff;
                        }
                        v30->color = v56 & 0xFF00FF00 | ((uint8_t)v56 << 16) | v95;
                        v30->uvSet1 = UVs[v36->uv1];
                        v57 = &verticesIN[v36->v2];
                        v58 = v30 + 1;
                        v58->v1.x = v57->x;
                        v58->v1.y = v57->y;
                        v59 = v102;
                        v58->v1.z = v57->z;
                        v60 = v36->v2;
                        v61 = v103 ^ (v59 | v101[v60]);
                        v96 = (v61 >> 16) & 0xff;
                        if (v104a)
                        {
                            v61 = v104a[v60] | v61 & 0xFFFFFF;
                            v96 = (v61 >> 16) & 0xff;
                        }
                        v58->color = v61 & 0xFF00FF00 | ((uint8_t)v61 << 16) | v96;
                        v58->uvSet1 = UVs[v36->uv2];
                        v62 = &verticesIN[v36->v3];
                        v63 = v58 + 1;
                        v63->v1.x = v62->x;
                        v63->v1.y = v62->y;
                        v64 = v102;
                        v63->v1.z = v62->z;
                        v65 = v36->v3;
                        v66 = v103 ^ (v64 | v101[v65]);
                        v97 = (v66 >> 16) & 0xff;
                        if (v104a)
                        {
                            v66 = v104a[v65] | v66 & 0xFFFFFF;
                            v97 = (v66 >> 16) & 0xff;
                        }
                        v63->color = v66 & 0xFF00FF00 | ((uint8_t)v66 << 16) | v97;
                        v63->uvSet1 = UVs[v36->uv3];
                        ++v36;
                        v30 = v63 + 1;
                    } while (v36 < triangleChunksd);
                }
            }
            else if ((v38 & 2) != 0)
            {
                if (v36 < v37)
                {
                    do
                    {
                        v67 = &verticesIN[v36->v1];
                        v30->v1.x = v67->x;
                        v30->v1.y = v67->y;
                        v30->v1.z = v67->z;
                        v30->color = v91;
                        v30->uvSet1 = UVs[v36->v1];
                        v68 = &verticesIN[v36->v2];
                        v69 = v30 + 1;
                        v69->v1.x = v68->x;
                        v69->v1.y = v68->y;
                        v69->v1.z = v68->z;
                        v69->color = v91;
                        v69->uvSet1 = UVs[v36->v2];
                        v70 = &verticesIN[v36->v3];
                        ++v69;
                        v69->v1.x = v70->x;
                        v69->v1.y = v70->y;
                        v69->v1.z = v70->z;
                        v69->color = v91;
                        v69->uvSet1 = UVs[v36->v3];
                        ++v36;
                        v30 = v69 + 1;
                    } while (v36 < triangleChunksd);
                }
            }
            else if (v36 < v37)
            {
                do
                {
                    v71 = &verticesIN[v36->v1];
                    v30->v1.x = v71->x;
                    v30->v1.y = v71->y;
                    v30->v1.z = v71->z;
                    v30->color = v91;
                    v30->uvSet1 = UVs[v36->uv1];
                    v72 = &verticesIN[v36->v2];
                    v73 = v30 + 1;
                    v73->v1.x = v72->x;
                    v73->v1.y = v72->y;
                    v73->v1.z = v72->z;
                    v73->color = v91;
                    v73->uvSet1 = UVs[v36->uv2];
                    v74 = &verticesIN[v36->v3];
                    ++v73;
                    v73->v1.x = v74->x;
                    v73->v1.y = v74->y;
                    v73->v1.z = v74->z;
                    v73->color = v91;
                    v73->uvSet1 = UVs[v36->uv3];
                    ++v36;
                    v30 = v73 + 1;
                } while (v36 < triangleChunksd);
            }
        }
        else if (v101)
        {
            if (v36 < v37)
            {
                do
                {
                    v75 = &verticesIN[v36->v1];
                    v30->v1.x = v75->x;
                    v30->v1.y = v75->y;
                    v76 = v102;
                    v30->v1.z = v75->z;
                    v77 = v36->v1;
                    v78 = v103 ^ (v76 | v101[v77]);
                    v98 = (v78 >> 16) & 0xff;
                    if (v32a)
                    {
                        v78 = v32a[v77] | v78 & 0xFFFFFF;
                        v98 = (v78 >> 16) & 0xff;
                    }
                    v30->color = v78 & 0xFF00FF00 | ((uint8_t)v78 << 16) | v98;
                    v79 = &verticesIN[v36->v2];
                    v80 = v30 + 1;
                    v80->v1.x = v79->x;
                    v80->v1.y = v79->y;
                    v81 = v102;
                    v80->v1.z = v79->z;
                    v82 = v36->v2;
                    v83 = v103 ^ (v81 | v101[v82]);
                    v99 = (v83 >> 16) & 0xff;
                    if (v32a)
                    {
                        v83 = v32a[v82] | v83 & 0xFFFFFF;
                        v99 = (v83 >> 16) & 0xff;
                    }
                    v80->color = v83 & 0xFF00FF00 | ((uint8_t)v83 << 16) | v99;
                    v84 = &verticesIN[v36->v3];
                    v85 = v80 + 1;
                    v85->v1.x = v84->x;
                    v85->v1.y = v84->y;
                    v86 = v102;
                    v85->v1.z = v84->z;
                    v87 = v103 ^ (v86 | v101[v36->v3]);
                    v100 = (v87 >> 16) & 0xff;
                    if (v32a)
                    {
                        v87 = v32a[v36->v3] | v87 & 0xFFFFFF;
                        v100 = (v87 >> 16) & 0xff;
                    }
                    v85->color = v87 & 0xFF00FF00 | ((uint8_t)v87 << 16) | v100;
                    ++v36;
                    v30 = v85 + 1;
                } while (v36 < triangleChunksd);
            }
        }
        else if (v36 < v37)
        {
            do
            {
                v88 = &verticesIN[v36->v1];
                v30->v1.x = v88->x;
                v30->v1.y = v88->y;
                v30->v1.z = v88->z;
                v30->color = v35;
                v89 = &verticesIN[v36->v2];
                v30[1].v1.x = v89->x;
                v30[1].v1.y = v89->y;
                v30[1].v1.z = v89->z;
                v30[1].color = v35;
                v90 = &verticesIN[v36->v3];
                v30[2].v1.x = v90->x;
                v30[2].v1.y = v90->y;
                v30[2].v1.z = v90->z;
                v30[2].color = v35;
                ++v36;
                v30 += 3;
            } while (v36 < triangleChunksd);
        }
        D3D_FillCurrentVBO(0);
        D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, v105);
        if (D3DRenderParams.textureUVMode != 5
            && D3DRenderParams.textureUVMode != 4
            && !D3DRenderParams.renderTargetBackup
            && D3D_setWaterRenderParamsIfWaterRenderingIsActive())
        {
            D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, v105);
            D3D_RestoreRenderParamsAfterWaterRendering();
        }
        if (D3DRenderParams.textureUVMode == 2)
        {
            if (textureStageTypeConfig[0].d3d_colorarg1 != v106)
            {
                textureStageTypeConfig[0].d3d_colorarg1 = v106;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, v106);
            }
            if (textureStageTypeConfig[0].d3d_colorarg2 != v107)
            {
                textureStageTypeConfig[0].d3d_colorarg2 = v107;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, v107);
            }
        }
        return 0;
    }
    if (verticesIN == v5)
    {
        D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, v4->d3dvb, 0, 12);
    }
    else
    {
        if (D3DRenderParams.currTargetGao != gpst_DX_stDD->currTargetGao)
        {
            if ((unsigned int)(D3DRenderParams.prevVertexElemIdx2 + v4->vertexNum) <= 0xFFFF)
            {
                v6 = D3DRenderParams.prevVertexElemIdx2;
                D3DRenderParams.startVertexElemIdx2 = D3DRenderParams.prevVertexElemIdx2;
                D3DRenderParams.prevVertexElemIdx2 += v4->vertexNum;
                v7 = 4096;
            }
            else
            {
                v6 = 0;
                D3DRenderParams.startVertexElemIdx2 = 0;
                D3DRenderParams.prevVertexElemIdx2 = v4->vertexNum;
                v7 = 0x2000;
            }
            D3DRenderParams.currTargetGao = gpst_DX_stDD->currTargetGao;
            D3DRenderParams.d3dXYZVB->lpVtbl->Lock(
                D3DRenderParams.d3dXYZVB,
                v6 * D3DRenderParams.stride2,
                D3DRenderParams.stride2 * v4->vertexNum,
                &v103a,
                v7);
            memcpy(v103a, verticesIN, sizeof(Vec3) * v4->vertexNum);
            D3DRenderParams.d3dXYZVB->lpVtbl->Unlock(D3DRenderParams.d3dXYZVB);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
            D3DRenderParams.d3dDev,
            0,
            D3DRenderParams.d3dXYZVB,
            D3DRenderParams.startVertexElemIdx2 * D3DRenderParams.stride2,
            D3DRenderParams.stride2);
    }
    v8 = gpst_DX_stDD->bigBuffer;
    v9 = v8->colorMask;
    v10 = v8->vertexColorArray;
    v105 = v8->solidVertexColor;
    v11 = D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 == gpst_DX_stDD->currTargetGao;
    v107 = v9;
    triangleChunksa = D3DRenderParams.colorMaskAlpha;
    if (!v11
        || D3DRenderParams.vertexColorArray != v10
        || D3DRenderParams.colorMask != v9
        || D3DRenderParams.colorMaskAlpha2 != D3DRenderParams.colorMaskAlpha
        || D3DRenderParams.solidVertexColor != v105)
    {
        if ((unsigned int)(D3DRenderParams.prevVertexElemIdx + v4->vertexNum) <= 0x3FFFF)
        {
            D3DRenderParams.startVertexElemIdx = D3DRenderParams.prevVertexElemIdx;
            D3DRenderParams.prevVertexElemIdx += v4->vertexNum;
            v12 = 4096;
        }
        else
        {
            D3DRenderParams.startVertexElemIdx = 0;
            D3DRenderParams.prevVertexElemIdx = v4->vertexNum;
            v12 = 0x2000;
        }
        v13 = gpst_DX_stDD->currTargetGao;
        D3DRenderParams.colorMaskAlpha2 = D3DRenderParams.colorMaskAlpha;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = v13;
        D3DRenderParams.vertexColorArray = v10;
        D3DRenderParams.colorMask = v9;
        D3DRenderParams.solidVertexColor = v105;
        D3DRenderParams.d3dColorsVB->lpVtbl->Lock(D3DRenderParams.d3dColorsVB, D3DRenderParams.startVertexElemIdx * D3DRenderParams.stride, D3DRenderParams.stride * v4->vertexNum, &v102a, v12);

        /*
        if (gpst_DX_stDD->currTargetGao == (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x620031d4)) {
            v10 = gpst_DX_stDD->RLIPtr;
            //OutputDebugStringA("itt\n");
            //((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->RLIVertexColors
            int iii;
            //if (v10) v10 = ((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->RLIVertexColors + 1;
            //DBG_OutputDebugString("buf: %p\n", v10);
            if (v10) {
                DBG_OutputDebugString("buf: ");
                for (iii = 0; iii < v4->vertexNum; iii++) {
                    DBG_OutputDebugString("%x ", v10[iii]);
                }
                DBG_OutputDebugString("\n\n");
            }
        }
        */

        if (v10)
        {
            v14 = v102a;
            v15 = v10;
            v105a = &v102a[v4->vertexNum];
            if (v102a < v105a)
            {
                while (1)
                {
                    v106 = v9 ^ (triangleChunksa | *v15);
                    v16 = v105a;
                    *v14++ = v106 & 0xFF00FF00 | ((uint8_t)v106 << 16) | ((v106 >> 16) & 0xff);
                    ++v15;
                    if (v14 >= v16)
                        break;
                    v9 = v107;
                }
            }
        }
        else if (v102a < &v102a[v4->vertexNum])
        {
            for (i = 0; i < (((4 * v4->vertexNum - 1) >> 2) + 1); i++) {
                v102a[i] = (v9 ^ (triangleChunksa | gpst_DX_stDD->bigBuffer->solidVertexColor)) & 0xFF00FF00 | ((uint8_t)(v9 ^ (triangleChunksa | (gpst_DX_stDD->bigBuffer->solidVertexColor & 0xff))) << 16) | (uint8_t)((v9 ^ ((unsigned int)triangleChunksa | gpst_DX_stDD->bigBuffer->solidVertexColor)) >> 16);
            }
        }
        D3DRenderParams.d3dColorsVB->lpVtbl->Unlock(D3DRenderParams.d3dColorsVB);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
        D3DRenderParams.d3dDev,
        1,
        D3DRenderParams.d3dColorsVB,
        D3DRenderParams.startVertexElemIdx * D3DRenderParams.stride,
        D3DRenderParams.stride);
    if (D3DRenderParams.textureUVMode == 2)
    {
        if (verticesIN == v4->vertices)
        {
            D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, v4->d3dvb2, 0, 12);
        }
        else
        {
            if (D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle != gpst_DX_stDD->currTargetGao)
            {
                if ((unsigned int)(D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle + v4->vertexNum) <= 0xFFFF)
                {
                    v17 = D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle;
                    D3DRenderParams.currVertexStartOffset = D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle;
                    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle += v4->vertexNum;
                    v18 = 4096;
                }
                else
                {
                    v17 = 0;
                    D3DRenderParams.currVertexStartOffset = 0;
                    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = v4->vertexNum;
                    v18 = 8192;
                }
                D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = gpst_DX_stDD->currTargetGao;
                D3DRenderParams.d3dNormalsBuffer->lpVtbl->Lock(D3DRenderParams.d3dNormalsBuffer, v17 * D3DRenderParams.d3dStride, D3DRenderParams.d3dStride * v4->vertexNum, &v103a, v18);
                memcpy(v103a, v4->normals, 12 * v4->vertexNum);
                D3DRenderParams.d3dNormalsBuffer->lpVtbl->Unlock(D3DRenderParams.d3dNormalsBuffer);
            }
            D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
                D3DRenderParams.d3dDev,
                2,
                D3DRenderParams.d3dNormalsBuffer,
                D3DRenderParams.currVertexStartOffset * D3DRenderParams.d3dStride,
                D3DRenderParams.d3dStride);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
            D3DRenderParams.d3dDev,
            D3DRenderParams.d3dVertexDeclaration);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 3, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetIndices(D3DRenderParams.d3dDev, v4->d3dIndexBuf);
    v19 = 0;
    if (v104->streamsNum)
    {
        triangleChunksb = 0;
        v20 = v104;
        do
        {
            v21 = &v20->d3dStreamInfoElems[triangleChunksb];
            if (D3DRenderParams.textureUVMode != 2)
            {
                v22 = v4->d3dUVVB;
                v23 = D3DRenderParams.d3dDev;
                if (v22)
                {
                    v23->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, v22[v21->vbIdx], 0, 8);
                    D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                        D3DRenderParams.d3dDev,
                        D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
                    v20 = v104;
                }
                else
                {
                    v23->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, 0, 0, 0);
                    D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                        D3DRenderParams.d3dDev,
                        D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
                }
            }
            D3DRenderParams.d3dDev->lpVtbl->DrawIndexedPrimitive(
                D3DRenderParams.d3dDev,
                5,
                0,
                0,
                v4->vertexNum,
                v21->startIndex,
                v21->primitiveCount - 2);
            ++v19;
            ++triangleChunksb;
        } while (v19 < v20->streamsNum);
    }
    if (D3DRenderParams.textureUVMode != 5 && D3DRenderParams.textureUVMode != 4)
    {
        v24 = 0;
        if (!D3DRenderParams.renderTargetBackup)
        {
            if (D3D_setWaterRenderParamsIfWaterRenderingIsActive())
            {
                v25 = v104;
                if (v104->streamsNum)
                {
                    triangleChunksc = 0;
                    do
                    {
                        v26 = &v25->d3dStreamInfoElems[triangleChunksc];
                        if (D3DRenderParams.textureUVMode != 2)
                        {
                            v27 = v4->d3dUVVB;
                            v28 = D3DRenderParams.d3dDev;
                            if (v27)
                            {
                                v28->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, v27[v26->vbIdx], 0, 8);
                                D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                                    D3DRenderParams.d3dDev,
                                    D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
                                v25 = v104;
                            }
                            else
                            {
                                v28->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, 0, 0, 0);
                                D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                                    D3DRenderParams.d3dDev,
                                    D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
                            }
                        }
                        D3DRenderParams.d3dDev->lpVtbl->DrawIndexedPrimitive(
                            D3DRenderParams.d3dDev,
                            5,
                            0,
                            0,
                            v4->vertexNum,
                            v26->startIndex,
                            v26->primitiveCount - 2);
                        ++v24;
                        ++triangleChunksc;
                    } while (v24 < v25->streamsNum);
                }
                D3D_RestoreRenderParamsAfterWaterRendering();
            }
        }
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 1, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 3, 0, 0, 0);
    return 0;
}

void D3D_SetRenderTargetAndBackupPreviousSDW(int textureID, int clear)
{
    IDirect3DSurface9* shadowTexSurface;
    D3DVIEWPORT9 viewPort = {
        .X = 1, 
        .Y = 1, 
        .Width = 126, 
        .Height = 126, 
        .MinZ = 0.0f, 
        .MaxZ = 1.0f
    };

    if (textureID == -1)
    {
        if (D3DRenderParams.renderTargetBackup)
        {
            D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, D3DRenderParams.renderTargetBackup);
            D3DRenderParams.renderTargetBackup->lpVtbl->Release(D3DRenderParams.renderTargetBackup);
            D3DRenderParams.renderTargetBackup = NULL;
            memcpy(&d3dProjectionMtx, &(D3DRenderParams.projectionMatrixBackup), sizeof(d3dProjectionMtx));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &(D3DRenderParams.projectionMatrixBackup));
            D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &(D3DRenderParams.pViewPort));
        }
    }
    else if (!D3DRenderParams.renderTargetBackup)
    {
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &(D3DRenderParams.renderTargetBackup));
        D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &(D3DRenderParams.pViewPort));
        D3DRenderParams.d3dShadowTextures[textureID]->lpVtbl->GetSurfaceLevel(D3DRenderParams.d3dShadowTextures[textureID], 0, &shadowTexSurface);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, shadowTexSurface);
        shadowTexSurface->lpVtbl->Release(shadowTexSurface);
        
        if (clear)
            D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, 0, 1, 0, 1.0f, 0);
        
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &viewPort);
    }
}

void D3D_SetViewMatrix_SDW(Mtx4x4* mtx, float* unkVals)
{
    D3DMATRIX identityMtx = { ._11 = 1.0f, ._12 = 0.0f, ._13 = 0.0f, ._14 = 0.0f,
                              ._21 = 0.0f, ._22 = 1.0f, ._23 = 0.0f, ._24 = 0.0f,
                              ._31 = 0.0f, ._32 = 0.0f, ._33 = 1.0f, ._34 = 0.0f,
                              ._41 = 0.0f, ._42 = 0.0f, ._43 = 0.0f, ._44 = 1.0f
                            };

    memcpy(&(D3DRenderParams.projectionMatrixBackup), &d3dProjectionMtx, sizeof(D3DRenderParams.projectionMatrixBackup));
    memcpy(&d3dTransformMatrix2, &identityMtx, sizeof(d3dTransformMatrix2));

    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, &identityMtx);
    memcpy(&d3dProjectionMtx, &identityMtx, sizeof(d3dProjectionMtx));

    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &identityMtx);
    D3D_SetViewMatrix(mtx);
}

void D3D_Texture_Unload(struct sGdi_DisplayData* a1, struct sEngPaletteElem* a2, int a3) {
}

int D3D_l_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* gdd) {
    return 0;
}

int  D3D_Set_Texture_Palette(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4) {
    return 0;
}

void D3D_SetTextureTarget(struct sGdi_DisplayData* a1, int a2, int a3, int a4) {
}

int PC_HandleWindowOpenAndCloseMessages(HWND a1, GLOBAL_MainHandles* mainHandles, unsigned int Msg)
{
    int result;

    if (Msg > WM_CLOSE)
    {
        if (Msg != WM_MOUSEFIRST)
            return 0;
        SetCursor(0);
        result = 0;
    }
    else
    {
        if (Msg == WM_CLOSE)
        {
        LABEL_8:
            if (!mainHandles || !mainHandles->displayData)
                return 0;
            GDI_DetachDisplay(mainHandles->displayData);
            mainHandles->displayData->DestroyDevice(mainHandles->displayData->pd3drp);
            GDI_DestroyDisplayData(mainHandles->displayData);
            return 0;
        }
        if (Msg != WM_CREATE)
        {
            if (Msg != WM_DESTROY)
                return 0;
            goto LABEL_8;
        }
        if (!mainHandles)
            return 0;
        mainHandles->hwnd = a1;
        fbss = GDI_fnpst_CreateDisplayData();
        mainHandles->displayData = fbss;
        GDI_fnl_InitInterface(mainHandles->displayData);
        mainHandles->displayData->pd3drp = (tGdi_D3DRenderParams*)mainHandles->displayData->CreateDevice();
        PRO_InitRasters(mainHandles->displayData->rasters, "Display Data");
        GDI_AttachDisplayAndReadaptDisplay(mainHandles->displayData, a1);
        result = 0;
    }
    return result;
}

int PC_HandleWindowCreateAndEraseBackgroundMessages(HWND hwnd, GLOBAL_MainHandles* globalHandles, int msg, int wparam, int lparam, int* plparam)
{
    if (msg == WM_CREATE)
    {
        globalHandles->hwndSetAfterCreateWindow = hwnd;
    }
    else if (msg == WM_ERASEBKGND)
    {
        *plparam = 1;
        return 1;
    }
    return 0;
}

int PC_WNDMsgHandler(HWND hWnd, int msg, int wParam, int a4, int* ret)
{
    CHAR stats[512];

    if ((unsigned int)msg > WM_ACTIVATEAPP)
    {
        if (msg == WM_NCLBUTTONDOWN)
        {
            ENG_GetStatsText(stats);
            SetWindowTextA(hWnd, stats);
            return 0;
        }
        if (msg == WM_SYSCOMMAND)
        {
            if (wParam == SC_CLOSE && ENG_gb_EngineRunning)
            {
                ENG_gb_ExitApplication = 1;
                *ret = 0;
                return 1;
            }
            return 0;
        }

        if (!ENG_WIN_AutoPlaySupressorRegistered)
        {
            ENG_WIN_AutoPlaySupressorRegistered = RegisterWindowMessageA("QueryCancelAutoPlay");
        }

        if (msg == ENG_WIN_AutoPlaySupressorRegistered)
        {
            *ret = 1;
            return 1;
        }
        return 0;
    }
    if (msg != WM_ACTIVATEAPP)
    {
        if (msg == WM_CLOSE || msg == WM_QUIT)
        {
            if (ENG_gb_EngineRunning)
            {
                ENG_gb_ExitApplication = 1;
                *ret = 0;
                return 1;
            }
            return 0;
        }

        if (!ENG_WIN_AutoPlaySupressorRegistered)
        {
            ENG_WIN_AutoPlaySupressorRegistered = RegisterWindowMessageA("QueryCancelAutoPlay");
        }

        if (msg == ENG_WIN_AutoPlaySupressorRegistered)
        {
            *ret = 1;
            return 1;
        }
        return 0;
    }
    if (wParam)
    {
        ENG_gb_ForcePauseEngine = 0;
        pcINO_dinput_mouse_and_keyb_acquire();
    }
    else
    {
        ENG_gb_ForcePauseEngine = 1;
        pcINO_dinput_mouse_and_keyb_unacquire();
    }
    return 0;
}

LRESULT WINAPI PC_WNDProcHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (PC_WNDMsgHandler(hWnd, Msg, wParam, lParam, &lParam)
        || PC_HandleWindowCreateAndEraseBackgroundMessages(hWnd, &MAI_gst_MainHandles, Msg, wParam, lParam, &lParam)
        || PC_HandleWindowOpenAndCloseMessages(hWnd, &MAI_gst_MainHandles, Msg))
    {
        return lParam;
    }
    else
    {
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
}

void ENG_WinGameWindowInit(HINSTANCE a1)
{
    struct tagRECT rect;
    WNDCLASSA wndClass;

    wndClass.style = 0;
    wndClass.lpfnWndProc = PC_WNDProcHandler;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = a1;
    wndClass.hIcon = LoadIconA(a1, MAKEINTRESOURCEA(IDI_MAIN));
    wndClass.hCursor = NULL;
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "BGEMain";
    RegisterClassA(&wndClass);
    rect.left = 0;
    rect.top = 0;
    rect.right = 640;
    rect.bottom = 480;
    AdjustWindowRect(&rect, WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_SYSMENU | WS_DLGFRAME | WS_BORDER, 0);
    MAI_gst_MainHandles.hwndSetAfterCreateWindow = CreateWindowExA(
        0,
        "BGEMain",
        "IRISEngine",
        WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_SYSMENU | WS_DLGFRAME | WS_BORDER,
        20,
        20,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        a1,
        NULL);
    MAI_gh_MainWindow = MAI_gst_MainHandles.hwndSetAfterCreateWindow;
    MAI_gst_MainHandles.hwnd = MAI_gst_MainHandles.hwndSetAfterCreateWindow;
    ShowWindow(MAI_gst_MainHandles.hwndSetAfterCreateWindow, SW_SHOW);
}

void s_HandleWinMessages()
{
    float startTimeStamp;
    MSG Msg;

    PRO_PerfCounterStartBlock(&PRO_WinMsg);

    startTimeStamp = TIM_f_Clock_TrueRead();
    if (!MsgWaitForMultipleObjectsEx(0, 0, 0, MY_QS_ALLINPUT_COMPATIBLE, MWMO_INPUTAVAILABLE | MWMO_ALERTABLE))
    {
        while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        };
    }
    TIM_SpentTimeWithHandlingWinMessages = TIM_f_Clock_TrueRead() - startTimeStamp + TIM_SpentTimeWithHandlingWinMessages;
    
    PRO_PerfCounterEndBlock(&PRO_WinMsg);
}

void PC_EngineCheatFinal()
{
    tableElemStructPF* currWorld;
    profilerRasterStructElem* currRaster;

    if (GetAsyncKeyState(VK_F6) < 0)
    {
        SOFT_ZList_Clear();
        AI_ReinitUniverse();

        for (currWorld = WOR_gst_Universe.univWorldTablePF.firstElemPtr; currWorld <= ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1; currWorld++)
        {
            if (!TAB_TableElemIsInvalid(currWorld->objectPtr)) {
                ENG_ReinitOneWorld((tWorld*)currWorld->objectPtr, 15);
            }
        }

        while (GetAsyncKeyState(VK_F6) < 0); 
    }
    if (GetAsyncKeyState(VK_F10) < 0 && GetAsyncKeyState(VK_SHIFT) < 0 && GetAsyncKeyState(VK_CONTROL) < 0)
    {
        AI_CompiledAIEnabled = AI_CompiledAIEnabled == 0;
        
        while (GetAsyncKeyState(VK_F10) < 0);
    }
    if (GetAsyncKeyState(VK_F1) < 0)
    {
        if (PRO_rasterWindowIsOn)
        {
            PRO_rasterDetailPage = PRO_rasterDetailPage == 0;
            if (PRO_rasterDetailPage != 0)
                PRO_rasterWindowIsOn = PRO_rasterDetailPage;
            while (GetAsyncKeyState(VK_F1) < 0);

            PRO_FirstRasterPtr = DBG_profiler_first_raster;
            pointsToUnkRaster_dword_98F944 = NULL;
            PRO_CurrRaster = NULL;
            
            if (PRO_rasterHWND)
                InvalidateRect(PRO_rasterHWND, NULL, TRUE);
        }
        else
        {
            PRO_rasterWindowIsOn = 1;
            while (GetAsyncKeyState(VK_F1) < 0);
        }
    }
    if (GetAsyncKeyState(VK_F2) < 0)
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff & 0x800) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff &= ~0x800;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff |= 0x800;
            }
        }
        while (GetAsyncKeyState(VK_F2) < 0);
    }
    if (GetAsyncKeyState(VK_F3) < 0)
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff & 0x10) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff &= ~0x10;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff |= 0x10;
            }
        }
        while (GetAsyncKeyState(VK_F3) < 0)
            ;
    }
    if (GetAsyncKeyState(VK_F5) < 0)
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags & 0x20000000) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags &= ~0x20000000;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags |= 0x20000000;
            }
        }
        while (GetAsyncKeyState(VK_F5) < 0);
    }
    if (GetAsyncKeyState(VK_F4) < 0)
    {
        if (PRO_rasterDetailPage)
        {
            currRaster = PRO_CurrRaster;
            if (PRO_CurrRaster)
            {
                while (1)
                {
                    currRaster = currRaster->prev;
                    if (!currRaster)
                        currRaster = DBG_profiler_first_raster;
                    if (currRaster == PRO_CurrRaster || _stricmp(currRaster->module, PRO_CurrRaster->module))
                        break;
                }
            }
            else
            {
                currRaster = DBG_profiler_first_raster;
            }
            PRO_CurrRaster = currRaster;
        }
        while (GetAsyncKeyState(VK_F4) < 0);

        if (PRO_rasterHWND)
            InvalidateRect(PRO_rasterHWND, 0, 1);
    }
}