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

static void D3D_RenderStateSet(int* current, DWORD state, int value)
{
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, state, (DWORD)(unsigned int)value);
    }
}

static void D3D_RenderStateSetF(float* current, DWORD state, float value)
{
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, state, *(DWORD*)current);
    }
}

static void D3D_StageStateSet(int stage, int* current, DWORD state, int value)
{
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, state, (DWORD)(unsigned int)value);
    }
}

static void D3D_StageStateSetF(int stage, float* current, DWORD state, float value)
{
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, state, *(DWORD*)current);
    }
}

static void D3D_SamplerStateSet(int sampler, int* current, DWORD state, int value)
{
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, sampler, state, (DWORD)(unsigned int)value);
    }
}

static void D3D_SamplerStateSetF(int sampler, float* current, DWORD state, float value)
{
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, sampler, state, *(DWORD*)current);
    }
}

static void D3D_SetFixedFunctionFVF(int fvf)
{
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = fvf;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
}

void PC_SetUVMatrix(Mtx4x4* dest, Vec3* axisU, Vec3* axisV, Vec3* axisW, Vec3* pos, Vec3* scale, int unused, int flags)
{
    dest->v1 = *axisU;
    dest->v2 = *axisV;
    dest->v3 = *axisW;
    dest->_m14 = scale->x;
    dest->_m24 = scale->y;
    dest->_m34 = scale->z;
    dest->Pos = *pos;
    dest->w = 1.0f;
    dest->mtxflag = flags;
}

void MAT_ComputeUV_sub_4521A0(tGEOGRO* geogro, tGeoTriangleChunks* chunks, tMultiTextureElemStruct* texElem)
{
    int planarParam;
    unsigned int uvFunc;
    int transform1;
    tMatDecompressedStruct decompressed;
    Vec3 axisV;
    Vec3 axisU;
    Vec3 axisW;
    Vec3 speed;
    Mtx4x4 oglMtx;
    Mtx4x4 uvMtx;
    Vec3 scale;

    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[38]);
    planarParam = texElem->phongOrPlanarParam;
    uvFunc = (texElem->subTextureFlags >> 20) & 0xF;
    D3DRenderParams.textureUVMode = 0;
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
    gpst_DX_stDD->flags &= ~0x10u;
    MAT_FuncTable[uvFunc](gpst_DX_stDD, geogro, chunks, planarParam);
    if (!gpst_DX_stDD->bigBuffer->UVData)
    {
        if (D3DRenderParams.textureUVMode)
        {
            MATH_MakeOGLMatrix(&oglMtx, &GLOBAL_UVTransformMatrix);
            memcpy(d3dTransformationMatrix, &oglMtx, sizeof(Mtx4x4WithoutAdditionalData));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX *)&oglMtx);
        }
        // preserved: this return skips the matching end block, so raster 38 stays entered
        return;
    }
    transform1 = texElem->transform1;
    if (transform1 == 0x3F800000 && texElem->transform2 == 0x3F800000 || !transform1 || !texElem->transform2) // TODO: MAT_VUIsIdentity inline
    {
        if (!D3DRenderParams.textureUVMode)
        {
            D3DRenderParams.textureUVMode = 0;
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
            PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[38]);
            return;
        }
        MATH_MakeOGLMatrix(&oglMtx, &GLOBAL_UVTransformMatrix);
    }
    else
    {
        MAT_UVDecompress(texElem, &decompressed);
        axisV.x = decompressed.f1;
        axisV.y = decompressed.f2;
        axisU.x = decompressed.f3;
        axisU.y = decompressed.f4;
        speed.x = decompressed.speedU;
        speed.y = decompressed.speedV;
        axisV.z = 0.0;
        axisU.z = 0.0;
        axisW.x = 0.0;
        axisW.y = 0.0;
        axisW.z = 1.0;
        speed.z = 0.0;
        scale.x = 1.0;
        scale.y = 1.0;
        scale.z = 1.0;
        PC_SetUVMatrix(&uvMtx, &axisV, &axisU, &axisW, &speed, &scale, 0, 6);
        if (D3DRenderParams.textureUVMode)
        {
            MATH_MulMatrixMatrix(&oglMtx, &GLOBAL_UVTransformMatrix, &uvMtx);
            MATH_MakeOGLMatrix(&oglMtx, &oglMtx);
        }
        else
        {
            D3DRenderParams.textureUVMode = 3;
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
            if (textureStageTypeConfig[0].d3dTextureTransformFlags != 2)
            {
                textureStageTypeConfig[0].d3dTextureTransformFlags = 2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }
            MATH_MakeOGLMatrix(&oglMtx, &uvMtx);
            oglMtx.v3.x = oglMtx.Pos.x;
            oglMtx.v3.y = oglMtx.Pos.y;
        }
    }
    memcpy(d3dTransformationMatrix, &oglMtx, sizeof(Mtx4x4WithoutAdditionalData));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&oglMtx);
    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[38]);
}

void MAT_UV_Compute_Chrome(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* chunks, unsigned int type)
{
    int drawMask;
    Vec3 axisV;
    Vec3 axisW;
    Vec3 pos;
    Vec3 scale;
    Vec3 axisU;

    D3DRenderParams.textureUVMode = 2;
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    drawMask = dd->drawMaskAllowedStuffCurrent;
    axisU.z = 0.0;
    axisU.y = 0.0;
    axisU.x = 0.5;
    if ((drawMask & 0x8000) != 0)
        axisU.x = -0.5;
    axisV.x = 0.0;
    axisV.y = -0.5;
    axisV.z = 0.0;
    axisW.x = 0.0;
    axisW.y = 0.0;
    axisW.z = -0.5;
    pos.x = 0.5;
    pos.y = 0.5;
    pos.z = 0.0;
    scale.x = 1.0;
    scale.y = 1.0;
    scale.z = 1.0;
    PC_SetUVMatrix(&GLOBAL_UVTransformMatrix, &axisU, &axisV, &axisW, &pos, &scale, 0, 4);
    dd->bigBuffer->UVData = dd->bigBuffer->data7;
    dd->flags |= 2u;
}

void MAT_UV_Compute_PLANAR_GIZMO_NOSTORE(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* chunks, unsigned int type)
{
    int elemIdx;
    tMultiPartGameObjectLinks* link;
    Mtx4x4* multiPartMtx;
    //double v14; 
    Mtx4x4* viewMtx;
    Vec3 axisU, axisV, axisW, pos, scale;
    Mtx4x4 multiPartMtxLocal = {0};
    Mtx4x4 viewMtxStd;
    Mtx4x4 viewMtxStdInv;
    Mtx4x4 modelViewMtx;

    dd->bigBuffer->UVData = dd->bigBuffer->data7;
    dd->flags |= 0x2;
    D3DRenderParams.textureUVMode = 1;
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    viewMtx = dd->ct.cam.viewMtxPtr_setInGRORender;
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
            axisU.x = 1.0f;
            axisU.y = 1.0f;
            axisU.z = 1.0f;
            axisV.x = 0.0f;
            axisV.y = 0.0f;
            axisV.z = 0.0f;
            axisW.x = 0.0f;
            axisW.y = 1.0f;
            axisW.z = 0.0f;
            pos.x = 1.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            scale.x = 0.0f;
            scale.y = 0.0f;
            scale.z = 1.0f;

            PC_SetUVMatrix(&viewMtxStd, &scale, &pos, &axisW, &axisV, &axisU, 0, 4);
            MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
            break;
        case 1:
            axisU.x = 1.0f;
            axisU.y = 0.0f;
            axisU.z = 0.0f;
            axisV.x = 0.0f;
            axisV.y = 0.0f;
            axisV.z = 1.0f;
            axisW.x = 0.0f;
            axisW.y = 1.0f;
            axisW.z = 0.0f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
            PC_SetUVMatrix(&viewMtxStd, &axisU, &axisV, &axisW, &pos, &scale, 0, 4);
            MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
            break;
        case 2:
            memcpy(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, sizeof(GLOBAL_UVTransformMatrix));
            break;
        case 3:
            axisU.x = 0.70710677f;
            axisU.y = -0.57735026f;
            axisU.z = 0.40799999f;
            axisV.x = -0.70710677f;
            axisV.y = -0.57735026f;
            axisV.z = 0.40799999f;
            axisW.x = 0.0f;
            axisW.y = 0.57735026f;
            axisW.z = 0.81599998f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
            PC_SetUVMatrix(&viewMtxStd, &axisU, &axisV, &axisW, &pos, &scale, 0, 0x4);
            MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
            break;
        default:
            return;
        }
        return;
    }

    elemIdx = (type >> 8) & 7;

    if (((uint8_t)type >> 6) != 3 || (dd->currTargetGao->flags & 0x200000) == 0)
    {
        if (((uint8_t)type >> 6) == 2)
        {
            MATH_MakeOGLMatrix(&multiPartMtxLocal, dd->matrixStack.actMatrixElem);
            MATH_VecMultScalarWithoutBuffer(&multiPartMtxLocal.v1, (dd->ct.cam.factorX / dd->ct.cam.centerX));
            MATH_VecMultScalarWithoutBuffer(&multiPartMtxLocal.v2, (dd->ct.cam.factorX / dd->ct.cam.centerX));
            MATH_VecMultScalarWithoutBuffer(&multiPartMtxLocal.v3, (dd->ct.cam.factorX / dd->ct.cam.centerX));            
        }
        else
        {
            MATH_MakeOGLMatrix(&multiPartMtxLocal, dd->currTargetGao->matrices);
        }
    } else if (dd->currTargetGao->basic->multiPartData->elemNum <= elemIdx)
    {
        MATH_MakeOGLMatrix(&multiPartMtxLocal, dd->currTargetGao->matrices);
    } else if ((dd->currTargetGao->flags & 0x1000000) == 0)
    {
        multiPartMtx = ((Mtx4x4*)(dd->currTargetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices));
        MATH_InvertMatrix(&multiPartMtxLocal, &multiPartMtx[elemIdx]);
        MATH_MakeOGLMatrix(&multiPartMtxLocal, &multiPartMtxLocal);
    } else if (link = ((tMultiPartGameObjectLinks*)dd->currTargetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices), link[elemIdx].gao != NULL)
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
            MATH_InvertMatrix(&multiPartMtxLocal, ((tMultiPartGameObjectLinks*)(dd->currTargetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx].gao->matrices);
            MATH_MulMatrixMatrix(&viewMtxStd, dd->currTargetGao->matrices, &multiPartMtxLocal);
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
        axisU.x = 0.0f;
        axisU.y = 0.0f;
        axisU.z = 1.0f;
        axisV.x = 1.0f;
        axisV.y = 0.0f;
        axisV.z = 0.0f;
        axisW.x = 0.0f;
        axisW.y = 1.0f;
        axisW.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        PC_SetUVMatrix(&viewMtxStd, &axisU, &axisV, &axisW, &pos, &scale, 0, 0x4);
        MATH_MulMatrixMatrix(&modelViewMtx, &multiPartMtxLocal, &viewMtxStd);
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &modelViewMtx);
        break;
    case 1:
        axisU.x = 1.0f;
        axisU.y = 0.0f;
        axisU.z = 0.0f;
        axisV.x = 0.0f;
        axisV.y = 0.0f;
        axisV.z = 1.0f;
        axisW.x = 0.0f;
        axisW.y = 1.0f;
        axisW.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        PC_SetUVMatrix(&modelViewMtx, &axisU, &axisV, &axisW, &pos, &scale, 0, 4);
        MATH_MulMatrixMatrix(&viewMtxStd, &multiPartMtxLocal, &modelViewMtx);
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
        break;
    case 2:
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &multiPartMtxLocal);
        break;
    case 3:
        MATH_TranspEq33Matrix(&multiPartMtxLocal);
        axisU.x = 0.70710677f;
        axisU.y = -0.57735026f;
        axisU.z = 0.40799999f;
        axisV.x = -0.70710677f;
        axisV.y = -0.57735026f;
        axisV.z = 0.40799999f;
        axisW.x = 0.0;
        axisW.y = 0.57735026f;
        axisW.z = 0.81599998f;
        pos.x = 0.0;
        pos.y = 0.0;
        pos.z = 0.0;
        scale.x = 1.0;
        scale.y = 1.0;
        scale.z = 1.0;
        PC_SetUVMatrix(&modelViewMtx, &axisU, &axisV, &axisW, &pos, &scale, 0, 4);
        MATH_MulMatrixMatrix(&viewMtxStd, &multiPartMtxLocal, &modelViewMtx);
        MATH_MulMatrixMatrix(&GLOBAL_UVTransformMatrix, &viewMtxStdInv, &viewMtxStd);
        break;
    default:
        break;
    }
    dd->bigBuffer->UVData = dd->bigBuffer->data7;
    dd->flags |= 0x2;
}

void D3D_CalcAndSetProjection(float left, float right, float bottom, float top,
                              float near, float far)
{
    double width;
    double height;
    float depth;
    D3DMATRIX mtx;

    width = right - left;
    memset(&mtx, 0, sizeof(mtx));
    mtx._44 = 1.0;
    mtx._11 = (float)(2.0 / width);
    height = top - bottom;
    mtx._22 = (float)(2.0 / height);
    depth = far - near;
    mtx._33 = (float)(-4.0 / depth);
    mtx._14 = (float)(-((left + right) / width));
    mtx._24 = (float)(-((bottom + top) / height));
    mtx._34 = (float)(-((near + far) / depth));
    memcpy(&d3dProjectionMtx, &mtx, sizeof(D3DMATRIX));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &mtx);
}

void D3D_SetViewPort(int x, int y, int w, int h)
{
    int clampedX;
    int clampedY;
    int clampedW;
    int clampedH;

    if (D3DRenderParams.d3dDev)
    {
        clampedX = x;
        if (x < 0)
            clampedX = 0;
        clampedY = y;
        if (y < 0)
            clampedY = 0;
        clampedW = w;
        if (clampedX + w > (int)D3DRenderParams.presentParams.BackBufferWidth)
            clampedW = D3DRenderParams.presentParams.BackBufferWidth - clampedX;
        clampedH = h;
        if (clampedY + h > (int)D3DRenderParams.presentParams.BackBufferHeight)
            clampedH = D3DRenderParams.presentParams.BackBufferHeight - clampedY;
        D3DRenderParams.d3dViewPort.X = clampedX;
        D3DRenderParams.d3dViewPort.Y = clampedY;
        D3DRenderParams.d3dViewPort.Width = clampedW;
        D3DRenderParams.d3dViewPort.Height = clampedH;
        D3DRenderParams.d3dViewPort.MinZ = 0.0;
        D3DRenderParams.d3dViewPort.MaxZ = 1.0;
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, (&D3DRenderParams.d3dViewPort));
    }
}

int   D3D_gi_Widescreen = 1;
int   D3D_gi_FlatLayerBox = 1;
int   D3D_gi_AspectTrace = 0;
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

    if (!D3D_gi_FlatLayerBox || !D3D_gi_Widescreen
        || D3D_gf_RefYoverX <= 0.0f || h <= 0)
        return w;
    boxW = (int)((float)h / D3D_gf_RefYoverX);
    return boxW < w ? boxW : w;
}

float D3D_FlatLayerToLayoutX(float windowFraction, int w, int h)
{
    int flat;

    flat = D3D_FlatLayerWidth(w, h);
    if (flat <= 0)
        return windowFraction;
    return (windowFraction * (float)w - (float)D3D_FlatLayerOffsetX(w, h)) / (float)flat;
}

float D3D_FlatLayerFromLayoutX(float layoutFraction, int w, int h)
{
    if (w <= 0)
        return layoutFraction;
    return ((float)D3D_FlatLayerOffsetX(w, h)
            + layoutFraction * (float)D3D_FlatLayerWidth(w, h)) / (float)w;
}

int D3D_FlatLayerOffsetX(int w, int h)
{
    return (w - D3D_FlatLayerWidth(w, h)) / 2;
}

void D3D_GetBandFractions(float* top, float* bottom)
{
    *top = D3D_gf_CutsceneBand;
    *bottom = 1.0f - D3D_gf_CutsceneBand;
}

void D3D_AspectTrace(const char* where, int viewX, int viewY, int viewW, int viewH)
{
    float t;
    float b;

    if (!D3D_gi_AspectTrace)
        return;
    D3D_GetBandFractions(&t, &b);
    D3D_OutputDebugString(
        "[aspect] %s back %dx%d rect %dx%d view %d,%d %dx%d "
        "YoverX %.4f bands t%.4f b%.4f cin %d\n",
        where,
        (int)D3DRenderParams.presentParams.BackBufferWidth,
        (int)D3DRenderParams.presentParams.BackBufferHeight,
        (int)(D3DRenderParams.Rect.right - D3DRenderParams.Rect.left),
        (int)(D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top),
        viewX, viewY, viewW, viewH,
        D3D_RealYoverX(), t, b, CIN_Bands);
    if (gpst_DX_stDD)
        D3D_OutputDebugString(
            "[aspect] %s cam %.1fx%.1f centre %.1f,%.1f factor %.2f,%.2f "
            "flat %d glyphscale x%.4f y%.4f\n",
            where,
            gpst_DX_stDD->ct.cam.width, gpst_DX_stDD->ct.cam.height,
            gpst_DX_stDD->ct.cam.centerX, gpst_DX_stDD->ct.cam.centerY,
            gpst_DX_stDD->ct.cam.factorX, gpst_DX_stDD->ct.cam.factorY,
            D3D_FlatLayerWidth((int)gpst_DX_stDD->ct.cam.width,
                               (int)gpst_DX_stDD->ct.cam.height),
            gpst_DX_stDD->ct.cam.width / 640.0f,
            gpst_DX_stDD->ct.cam.height / 480.0f);
}

void D3D_SetProjectionMatrix(tDDCamStruct* cam)
{
    int ratioIdx;
    int rectW;
    int rectH;
    int fitW;
    int fitH;
    int viewX;
    int viewY;
    int viewW;
    int viewH;
    int flags2;
    float screenRatio;
    float fitRatio;
    float cotFov;
    float isoSize;
    double negCot;
    double isoRatio;
    double invRatio;
    double negFar;
    double isoLeft;
    float orthoW;
    float orthoH;
    float halfTexel;
    D3DMATRIX offsetMtx;
    D3DMATRIX orthoMtx;

    D3DRenderParams.projectionMatrix = &cam->projectionMtx;
    ratioIdx = gpst_DX_stDD->ddcs2.selectedScreenRatio;
    if (ratioIdx <= 0 || ratioIdx >= 4)
        screenRatio = gpst_DX_stDD->ddcs2.manualScreenRatio;
    else
        screenRatio = GDI_gaf_ScreenRatio[ratioIdx];
    if (D3D_gi_Widescreen)
        screenRatio = D3D_RealYoverX();
    rectW = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
    rectH = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
    fitRatio = gpst_DX_stDD->ddcs2.pixelYoverX * cam->YoverX * screenRatio;
    flags2 = gpst_DX_stDD->ddcs2.flags2;
    if ((flags2 & 4) != 0)
    {
        fitH = rectH;
        fitW = (uint32_t)((double)rectH / fitRatio);
        if ((flags2 & 1) != 0 && fitW > rectW || (flags2 & 2) != 0 && fitW < rectW)
        {
            fitH = (uint32_t)((double)rectW * fitRatio);
            fitW = rectW;
        }
    }
    else
    {
        fitH = (uint32_t)((double)rectW * fitRatio);
        fitW = rectW;
        if ((flags2 & 1) != 0 && fitH > rectH || (flags2 & 2) != 0 && fitH < rectH)
        {
            fitW = (uint32_t)((double)rectH / fitRatio);
            fitH = rectH;
        }
    }
    if (cam->viewPortSizeWidth == 0.0)
    {
        cam->viewPortSizeWidth = 1.0;
        cam->viewPortSizeHeight = 1.0;
        cam->viewPortPosX = 0.0;
        cam->viewPortPosY = 0.0;
    }
    viewX = (int32_t)((double)((rectW - fitW) / 2) + (double)fitW * cam->viewPortPosX);
    viewY = (int32_t)((double)((rectH - fitH) / 2) + (double)fitH * cam->viewPortPosY);
    viewH = (uint32_t)((double)fitH * cam->viewPortSizeHeight);
    viewW = (uint32_t)((double)fitW * cam->viewPortSizeWidth);
    cam->viewPortRealPosX = viewX;
    cam->viewPortRealPosY = viewY;
    if ((cam->flags & 0x400) != 0)
    {
        orthoW = (float)viewW;
        halfTexel = (float)(0.5 / orthoW);
        D3DXMatrixTranslation(&offsetMtx, halfTexel, halfTexel, 0.0);
        orthoH = (float)viewH;
        D3DXMatrixOrthoOffCenterRH(&orthoMtx, 0.0, orthoW, 0.0, orthoH, 0.0, 1.0);
        D3DXMatrixMultiply((D3DMATRIX*)D3DRenderParams.projectionMatrix, &orthoMtx, &offsetMtx);
        memcpy(&d3dProjectionMtx, D3DRenderParams.projectionMatrix, sizeof(D3DMATRIX));
        D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, (D3DMATRIX*)(D3DRenderParams.projectionMatrix));
        return;
    }
    if ((cam->flags & 1) != 0)
    {
        cotFov = (float)(1.0 / tan(cam->focale * 0.5));
        MATH_SetIdentityMatrix(D3DRenderParams.projectionMatrix);
        if (D3D_gi_Widescreen)
        {
            // the authored vertical half angle, kept whatever the
            // window shape is; the horizontal one follows v46
            float cotY = ((gpst_DX_stDD->ddcs2.flags2 & 4) != 0)
                             ? cotFov
                             : (float)(cotFov / D3D_gf_RefYoverX);

            D3DRenderParams.projectionMatrix->v1.x = (float)(cotY * screenRatio);
            negCot = -cotY;
        }
        else if ((gpst_DX_stDD->ddcs2.flags2 & 4) != 0)
        {
            D3DRenderParams.projectionMatrix->v1.x = cotFov * screenRatio;
            negCot = -cotFov;
        }
        else
        {
            D3DRenderParams.projectionMatrix->v1.x = cotFov;
            negCot = -(cotFov / screenRatio);
        }
        D3DRenderParams.projectionMatrix->v2.y = (float)negCot;
        D3DRenderParams.projectionMatrix->v3.z = (cam->nearPlane + cam->farPlane) / (cam->farPlane - cam->nearPlane);
        D3DRenderParams.projectionMatrix->_m34 = 1.0;
        D3DRenderParams.projectionMatrix->Pos.z = -0.050000001f;
        D3DRenderParams.projectionMatrix->w = 0.0;
        memcpy(&d3dProjectionMtx, D3DRenderParams.projectionMatrix, sizeof(D3DMATRIX));
        D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, (D3DMATRIX*)(D3DRenderParams.projectionMatrix));
    }
    else
    {
        MATH_SetIdentityMatrix(D3DRenderParams.projectionMatrix);
        isoSize = cam->isoZoom * cam->isoFactor;
        negFar = -cam->farPlane;
        if ((gpst_DX_stDD->ddcs2.flags2 & 4) == 0)
        {
            isoRatio = D3D_gi_Widescreen ? D3D_RealYoverX()
                                         : gpst_DX_stDD->ddcs2.manualScreenRatio;
            if (cam->isoFactor == 0.0)
            {
                D3D_CalcAndSetProjection(0.0, 1.0,
                                         (float)((1.0 - isoRatio) * 0.5),
                                         (float)((isoRatio + 1.0) * 0.5),
                                         -cam->nearPlane, (float)negFar);
            }
            else
            {
                isoLeft = isoSize * -1.0;
                D3D_CalcAndSetProjection((float)isoLeft,
                                         cam->isoZoom * cam->isoFactor,
                                         (float)(isoSize * isoRatio),
                                         (float)(-isoRatio * isoSize),
                                         cam->farPlane, (float)negFar);
            }
        }
        else
        {
            invRatio = 1.0 / (D3D_gi_Widescreen ? D3D_RealYoverX()
                                                : gpst_DX_stDD->ddcs2.manualScreenRatio);
            if (cam->isoFactor != 0.0)
            {
                isoLeft = -invRatio * isoSize;
                D3D_CalcAndSetProjection((float)isoLeft,
                                         (float)(isoSize * invRatio),
                                         (float)(isoSize * -1.0),
                                         cam->isoZoom * cam->isoFactor,
                                         cam->farPlane, (float)negFar);
            }
            else
            {
                D3D_CalcAndSetProjection((float)((1.0 - invRatio) * 0.5),
                                         (float)((invRatio + 1.0) * 0.5),
                                         0.0, 1.0,
                                         -cam->nearPlane, (float)negFar);
            }
        }
    }
    D3D_SetViewPort(viewX, viewY, viewW, viewH);
    D3D_AspectTrace("proj", viewX, viewY, viewW, viewH);
}


void D3D_GetSnapShot(tD3dRequestSnapshot* req)
{
    unsigned char* rowStart;
    unsigned char* src;
    unsigned char* dest;
    int row;
    int col;
    IDirect3DSurface9* target;
    IDirect3DSurface9* copy;
    D3DLOCKED_RECT locked;
    RECT area;
    D3DSURFACE_DESC desc;

    if ((req->flags & 0xff) == 0 && ((req->flags >> 8) & 0xff) != 2)
    {
        area.left = req->cutSizeFromTopAndBottom;
        area.top = req->cutSizeFromSides;
        area.right = req->cutSizeFromTopAndBottom + req->width;
        area.bottom = req->cutSizeFromSides + req->height;
        if (D3DRenderParams.d3dNeedToCallEndScene)
        {
            D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
            D3DRenderParams.d3dNeedToCallEndScene = 0;
        }
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &target);
        target->lpVtbl->GetDesc(target, &desc);

        D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(D3DRenderParams.d3dDev, desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &copy, 0);

        D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev, target, copy);
        copy->lpVtbl->LockRect(copy, &locked, &area, 0);
        rowStart = (unsigned char*)locked.pBits;
        dest = (unsigned char*)req->srcData;
        for (row = 0; row < req->height; row++)
        {
            src = rowStart;
            for (col = 0; col < req->width; col++)
            {
                dest[0] = src[0];
                dest[1] = src[1];
                dest[2] = src[2];
                dest += 3;
                src += 4;
            }
            rowStart += locked.Pitch;
        }
        copy->lpVtbl->UnlockRect(copy);
        copy->lpVtbl->Release(copy);
        target->lpVtbl->Release(target);
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

void D3D_DestroyDevice(struct sGdi_D3DRenderParams* renderParams)
{
    if (renderParams == &D3DRenderParams)
    {
        D3DRenderParams.d3d_if->lpVtbl->Release(D3DRenderParams.d3d_if);
        memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    }
}

int D3D_SetViewMatrix(Mtx4x4* mtx)
{
    Mtx4x4 oglMtx;

    MATH_MakeOGLMatrix(&oglMtx, mtx);
    memcpy(&d3dTransformMatrix, &oglMtx, sizeof(d3dTransformMatrix));
    return D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&oglMtx);
}

void D3D_TextureStore(tTexUploadRequest* req)
{
    int idx;
    tEngTexListElemStruct* texElem;
    IDirect3DTexture9* texture;
    IDirect3DSurface9* surface;
    RECT area;

    idx = req->idx;
    texElem = &TEX_gst_GlobalList.texElems[idx];
    texture = D3DRenderParams.d3dTextures[idx];
    if (!texture)
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, texElem->normalData.width, texElem->normalData.height, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &texture, 0);
    texture->lpVtbl->GetSurfaceLevel(texture, 0, &surface);
    area.top = 0;
    area.left = 0;
    area.right = texElem->normalData.width;
    area.bottom = texElem->normalData.height;
    D3DXLoadSurfaceFromMemory(surface, NULL, NULL, req->dataPtr, D3DFMT_A8R8G8B8, 4 * texElem->normalData.width, NULL, &area, D3DX_FILTER_POINT, 0);
    D3DXFilterTexture((IDirect3DBaseTexture9*)texture, NULL, 0, D3DX_FILTER_LINEAR);
    surface->lpVtbl->Release(surface);
    D3DRenderParams.d3dTextures[req->idx] = texture;
}

void D3D_ReleaseTextures(struct sGdi_DisplayData* dd)
{
    int i;

    if (D3DRenderParams.d3dTextures)
    {
        for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
        {
            if (D3DRenderParams.d3dTextures[i])
            {
                D3DRenderParams.d3dTextures[i]->lpVtbl->Release(D3DRenderParams.d3dTextures[i]);
                D3DRenderParams.d3dTextures[i] = NULL;
            }
        }
    }
    // preserved: the source array is walked with no null test, unlike the one above
    for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
    {
        if (D3DRenderParams.d3dSourceTextures[i])
        {
            D3DRenderParams.d3dSourceTextures[i]->lpVtbl->Release(D3DRenderParams.d3dSourceTextures[i]);
            D3DRenderParams.d3dSourceTextures[i] = NULL;
        }
    }
    if (D3DRenderParams.d3dTextures)
    {
        MEM_Free(D3DRenderParams.d3dTextures);
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
    int i;

    for (i = 7; i >= 0; i--)
    {
        if (D3DRenderParams.d3dShadowTextures[i])
        {
            D3DRenderParams.d3dShadowTextures[i]->lpVtbl->Release(D3DRenderParams.d3dShadowTextures[i]);
            D3DRenderParams.d3dShadowTextures[i] = NULL;
        }
    }
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
    int i;

    if (D3DRenderParams.pBackBuffers[0])
    {
        for (i = 0; i < 2; i++)
        {
            D3DRenderParams.pBackBuffers[i]->lpVtbl->Release(D3DRenderParams.pBackBuffers[i]);
            D3DRenderParams.pBackBuffers[i] = NULL;
        }
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

void D3D_CheckReturnAndRunSettingsApplicationIfProblem(int result)
{
    PROCESS_INFORMATION ProcessInformation;
    STARTUPINFOA StartupInfo;

    if (result < 0)
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

void D3D_SetVertexShader(IDirect3DVertexShader9* shader)
{
    if (renderStateConfig.vertexShader != shader)
    {
        renderStateConfig.vertexShader = shader;
        renderStateConfig.fvf = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
        D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, renderStateConfig.vertexShader);
    }
}

void D3D_SetInitialRenderStates()
{
    memset(&renderStateConfig, 0, sizeof(renderStateConfig));
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, D3DZB_TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, 0);
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
    D3D_RenderStateSetF(&renderStateConfig.d3dFogDensity, D3DRS_FOGDENSITY, 1.0f);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogStart, D3DRS_FOGSTART, 0.0f);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogEnd, D3DRS_FOGEND, 1.0f);
    D3D_RenderStateSet(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, D3DFILL_SOLID);
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_CCW);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, 0xf);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, 0);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaRef, D3DRS_ALPHAREF, 0);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ZERO);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals, D3DRS_NORMALIZENORMALS, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0xffffffff);
    D3D_RenderStateSet(&renderStateConfig.d3dClipPlaneEnable, D3DRS_CLIPPLANEENABLE, 0);
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
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
            D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressu, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressv, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            if (samplerConfig[stage].d3d_minfilter)
            {
                samplerConfig[stage].d3d_minfilter = D3DTEXF_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, stage, D3DSAMP_MINFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[stage].d3d_magfilter)
            {
                samplerConfig[stage].d3d_magfilter = D3DTEXF_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, stage, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[stage].d3d_mipfilter)
            {
                samplerConfig[stage].d3d_mipfilter = D3DTEXF_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
            }
        D3D_SamplerStateSetF(stage, &samplerConfig[stage].d3d_mipmaplodbias, D3DSAMP_MIPMAPLODBIAS, 0.0f);
    }
}

void D3D_SetInitialTextureStageStates()
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
        textureStageTypeConfig[stage].d3dTextureID = -1;
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat00, D3DTSS_BUMPENVMAT00, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat01, D3DTSS_BUMPENVMAT01, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat10, D3DTSS_BUMPENVMAT10, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat11, D3DTSS_BUMPENVMAT11, 0.0f);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, stage);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLScale, D3DTSS_BUMPENVLSCALE, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLOffset, D3DTSS_BUMPENVLOFFSET, 0.0f);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorArg0, D3DTSS_COLORARG0, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg0, D3DTSS_ALPHAARG0, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_resultArg0, D3DTSS_RESULTARG, D3DTA_CURRENT);
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_constant, D3DTSS_CONSTANT, 0);
    }
}

void D3D_RastersCreateFont()
{
    D3DXFONT_DESCA desc;

    desc.Width = 0;
    desc.Italic = 0;
    desc.CharSet = 0;
    desc.OutputPrecision = 0;
    desc.Quality = 0;
    desc.PitchAndFamily = 0;
    memset(desc.FaceName, 0, sizeof(desc.FaceName));
    strcpy(desc.FaceName, "Arial");
    desc.Height = 10;
    desc.Weight = 400;
    desc.MipLevels = 1;
    D3DXCreateFontIndirectA(D3DRenderParams.d3dDev, &desc, &d3dxfontIF);
}

void D3D_RastersDrawText(char* text, int x, int y, D3DCOLOR color)
{
    RECT area;

    area.left = x;
    area.right = 10000;
    area.bottom = 10000;
    area.top = y;
    if (!d3dxfontIF)
        D3D_RastersCreateFont();
    d3dxfontIF->lpVtbl->DrawTextA(d3dxfontIF, NULL, text, -1, &area, 0, color);
}

void D3D_RastersDrawUPStream()
{
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLELIST,
        ((rastersDrawPrimitiveUPDataEnd - rastersDrawPrimitiveUPDataStart) >> 4) / 3,
        rastersDrawPrimitiveUPDataStart,
        16); // 16 = Vec3 + color (int), see D3DRenderParams.fvf
    rastersDrawPrimitiveUPDataEnd = rastersDrawPrimitiveUPDataStart;
}

void D3D_SaveSamplerState(tD3dSamplerConfigStruct* dest)
{
    memcpy(dest, samplerConfig, sizeof(samplerConfig));
}

void D3D_SaveTextureStage(tD3dTextureStageTypeStruct* dest)
{
    memcpy(dest, textureStageTypeConfig, sizeof(textureStageTypeConfig));
}

void D3D_SaveRenderState(tD3dRenderStateConfigStruct *dest)
{
    memcpy(dest, &renderStateConfig, sizeof(tD3dRenderStateConfigStruct));
}

void D3D_SaveEverythingToBackup(int saveType)
{
    if (saveType != 3)
    {
        D3D_SaveSamplerState(samplerStateBackup);
        D3D_SaveTextureStage(texturestageBackup);
    }
    if (saveType != 2)
        D3D_SaveRenderState(&renderstateBackup);
    d3dparamsSaveType = saveType;
}

void D3D_RenderStateRestore(tD3dRenderStateConfigStruct* cfg)
{
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, cfg->d3d_zEnable);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, cfg->d3d_zFunc);
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, cfg->d3d_zWriteEnable);
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, cfg->d3dFogEnable);
    D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, cfg->d3dFogColor);
    D3D_RenderStateSet(&renderStateConfig.d3dFogTableMode, D3DRS_FOGTABLEMODE, cfg->d3dFogTableMode);
    D3D_RenderStateSet(&renderStateConfig.d3dFogVertexMode, D3DRS_FOGVERTEXMODE, cfg->d3dFogVertexMode);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogDensity, D3DRS_FOGDENSITY, cfg->d3dFogDensity);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogStart, D3DRS_FOGSTART, cfg->d3dFogStart);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogEnd, D3DRS_FOGEND, cfg->d3dFogEnd);
    D3D_RenderStateSet(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, cfg->d3dFillMode);
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, cfg->d3dCullMode);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, cfg->d3dColorWriteEnable);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, cfg->d3dAlphaTest);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, cfg->d3dAlphaFunc);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaRef, D3DRS_ALPHAREF, cfg->d3dAlphaRef);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, cfg->d3dAlphaBlendEnable);
    D3D_RenderStateSet(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, cfg->d3d_BlendOp);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, cfg->d3dSrcBlend);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, cfg->d3dDestBlend);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, cfg->d3d_Lighting);
    D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals, D3DRS_NORMALIZENORMALS, cfg->d3d_normalizeNormals);
    D3D_RenderStateSet(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, cfg->d3dTextureFactor);
    D3D_RenderStateSet(&renderStateConfig.d3dClipPlaneEnable, D3DRS_CLIPPLANEENABLE, cfg->d3dClipPlaneEnable);
    if (cfg->vertexShader)
    {
        D3D_SetFixedFunctionFVF(0);
        if (renderStateConfig.fvf != cfg->fvf)
        {
            renderStateConfig.fvf = cfg->fvf;
            D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, (IDirect3DVertexDeclaration9 *)(renderStateConfig.fvf)); // TODO FIXME: ez micsoda gányolás de az IDA ezt mutatja, játékban ez van
        }
        D3D_SetVertexShader(cfg->vertexShader);
    }
    else
    {
        if (renderStateConfig.fvf)
        {
            renderStateConfig.fvf = 0;
            D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, NULL);
        }
        D3D_SetVertexShader(0);
        D3D_SetFixedFunctionFVF(cfg->fvf);
    }
}

void D3D_SamplerStateRestore(tD3dSamplerConfigStruct* cfg)
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressu, D3DSAMP_ADDRESSU, cfg[stage].d3d_addressu);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressv, D3DSAMP_ADDRESSV, cfg[stage].d3d_addressv);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_minfilter, D3DSAMP_MINFILTER, cfg[stage].d3d_minfilter);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_magfilter, D3DSAMP_MAGFILTER, cfg[stage].d3d_magfilter);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_mipfilter, D3DSAMP_MIPFILTER, cfg[stage].d3d_mipfilter);
        D3D_SamplerStateSetF(stage, &samplerConfig[stage].d3d_mipmaplodbias, D3DSAMP_MIPMAPLODBIAS, cfg[stage].d3d_mipmaplodbias);
    }
}

void D3D_SetTexture(int textureState, int textureID)
{
    IDirect3DTexture9* texture;

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
                    texture = D3DRenderParams.d3dTextures[textureID];
                } 
                else {
                    texture = D3DRenderParams.d3dShadowTextures[(textureID & ~0x80000000)];
                }
                D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, textureState, (IDirect3DBaseTexture9*)texture);
            }
            textureStageTypeConfig[textureState].d3dTextureID = textureID;
        }
    }
}

void D3D_TextureStageStateRestore(tD3dTextureStageTypeStruct* cfg)
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
            textureStageTypeConfig[stage].d3dTextureID = cfg[stage].d3dTextureID;
            D3D_SetTexture(stage, cfg[stage].d3dTextureID);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, 1, cfg[stage].d3d_colorOp);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg1, D3DTSS_COLORARG1, cfg[stage].d3d_colorarg1);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg2, D3DTSS_COLORARG2, cfg[stage].d3d_colorarg2);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, cfg[stage].d3d_alphaOp);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg1, D3DTSS_ALPHAARG1, cfg[stage].d3d_alphaArg1);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg2, D3DTSS_ALPHAARG2, cfg[stage].d3d_alphaArg2);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat00, D3DTSS_BUMPENVMAT00, cfg[stage].d3d_bumpenvmat00);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat01, D3DTSS_BUMPENVMAT01, cfg[stage].d3d_bumpenvmat01);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat10, D3DTSS_BUMPENVMAT10, cfg[stage].d3d_bumpenvmat10);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat11, D3DTSS_BUMPENVMAT11, cfg[stage].d3d_bumpenvmat11);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, cfg[stage].d3dTexCoordIndex);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLScale, D3DTSS_BUMPENVLSCALE, cfg[stage].d3d_bumpEnvLScale);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLOffset, D3DTSS_BUMPENVLOFFSET, cfg[stage].d3d_bumpEnvLOffset);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, cfg[stage].d3dTextureTransformFlags);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorArg0, D3DTSS_COLORARG0, cfg[stage].d3d_colorArg0);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg0, D3DTSS_ALPHAARG0, cfg[stage].d3d_alphaArg0);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_resultArg0, D3DTSS_RESULTARG, cfg[stage].d3d_resultArg0);
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_constant, D3DTSS_CONSTANT, cfg[stage].d3d_constant);
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

void D3D_RastersSetAndDrawAQuad(int color, float x, float y, float w, float h, int flush)
{
    tVertexAndColor* quad;
    char* buffer;
    double right;
    double bottom;

    quad = (tVertexAndColor*)rastersDrawPrimitiveUPDataEnd;
    if (!rastersDrawPrimitiveUPDataEnd)
    {
        buffer = (char*)MEM_p_Alloc(16384u);
        memset(buffer, 0, 16384u);
        rastersDrawPrimitiveUPDataStart = buffer;
        quad = (tVertexAndColor*)buffer;
    }

    right = x + w;
    bottom = y + h;

    quad[0].v.x = x;
    quad[0].v.y = y;
    quad[0].color = color;

    quad[1].v.x = (float)right;
    quad[1].v.y = y;
    quad[1].color = color;    
    
    quad[2].v.x = x;
    quad[2].v.y = (float)bottom;
    quad[2].color = color;

    quad[3].v.x = x;
    quad[3].v.y = (float)bottom;
    quad[3].color = color;

    quad[4].v.x = (float)right;
    quad[4].v.y = y;
    quad[4].color = color;

    quad[5].v.x = (float)right;
    quad[5].v.y = (float)bottom;
    quad[5].color = color;
    //rastersDrawPrimitiveUPDataEnd += sizeof(tVertexAndColor) * 6;
    rastersDrawPrimitiveUPDataEnd = (char *)(quad + 6); // a vége az 6 db tVertexAndColor elemet fog tartalmazni, mivel a v6 az egy tVertexAndColor típusra mutató pointer
    if (flush)
        D3D_RastersDrawUPStream();
}

void D3D_DrawARaster(int color, char* name, float value, float y, float width)
{
    double nextY;
    float x;
    char text[32];
    float barWidth;
    float segment;

    sprintf(text, "%5.3f", value);
    D3D_RastersDrawText(name, 20, (unsigned int)y, -1);
    D3D_RastersDrawText(text, (unsigned int)(width - 50.0), (unsigned int)y, -1);
    if (value > 4.0)
        value = 4.0;
    x = 70.0;
    for (segment = (float)((width - 140.0) * 0.75294119); value > 0.0; segment = (float)(segment * 0.25))
    {
        if (value >= 1.0)
            nextY = 1.0;
        else
            nextY = value;
        barWidth = (float)(nextY * segment);
        D3D_RastersSetAndDrawAQuad(color, x, y, barWidth, 10.0, 0);
        color ^= 0xFFFFu;
        x = barWidth + x;
        value = (float)(value - 1.0);
    }
}

void Rasters_Update()
{
    double elapsed;
    unsigned int i;
    unsigned int j;
    int swapped;
    int shown;
    int row;
    int raster;
    int kind;
    int barColor;
    double nextY;
    float barY;
    float y;
    float value;
    float scale;
    float viewHeight;
    float viewWidth;
    float fps;
    float barHeight;
    LARGE_INTEGER after;
    LARGE_INTEGER Frequency;
    LARGE_INTEGER PerformanceCount;
    D3DMATRIX mtx;
    int order[54];

    viewWidth = (float)D3DRenderParams.d3dViewPort.Width;
    viewHeight = (float)D3DRenderParams.d3dViewPort.Height;

    if (tweak_perf)
    {
        PRO_QPC_RecordTimeDecRef(53);
        PRO_QPC_RecordTimeDecRef(19);
        QueryPerformanceCounter(&PerfCounterStruct[47]);
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&PerformanceCount);
        elapsed = (double)(PerformanceCount.QuadPart - qword_990B48);
        qword_990B48 = PerformanceCount.QuadPart;
        fps = (float)((double)Frequency.QuadPart / elapsed);
        if (tweak_perf != 3 || fps > (double)tweak_perf_fps_min)
        {
            D3D_SaveEverythingToBackup(1);
            mtx._43 = 0.0;
            mtx._42 = 0.0;
            mtx._41 = 0.0;
            mtx._34 = 0.0;
            mtx._32 = 0.0;
            mtx._31 = 0.0;
            mtx._24 = 0.0;
            mtx._23 = 0.0;
            mtx._21 = 0.0;
            mtx._14 = 0.0;
            mtx._13 = 0.0;
            mtx._12 = 0.0;
            mtx._44 = 1.0;
            mtx._33 = 1.0;
            mtx._22 = 1.0;
            mtx._11 = 1.0;
            memcpy(&d3dTransformMatrix, &mtx, sizeof(d3dTransformMatrix));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, 2, &mtx);
            D3DXMatrixOrthoOffCenterRH(&mtx, 0.0, viewWidth, viewHeight, 0.0, -1.0, 1.0);
            memcpy(&d3dProjectionMtx, &mtx, sizeof(d3dProjectionMtx));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, 3, &mtx);
            D3D_SetVertexShader(0);
            D3D_SetFixedFunctionFVF(D3DFVF_DIFFUSE | D3DFVF_XYZ);
            if (renderStateConfig.d3d_zEnable)
            {
                renderStateConfig.d3d_zEnable = D3DZB_FALSE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DZB_FALSE);
            }
            D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_NONE);
            D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, FALSE);
            D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, 0xf);
            D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            D3D_SetTexture(0, -1);
            D3D_SetTexture(1, -1);
            D3D_SetTexture(2, -1);
            D3D_SetTexture(3, -1);
            if (fps > (double)tweak_perf_fps_min)
                D3D_RastersSetAndDrawAQuad(0xFFFF0000, 0.0, 0.0, viewWidth, 40.0, 0);
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
                    order[j] = j;
                for (i = 0; i < 53; i++)
                {
                    for (j = i + 1; j < 54; j++)
                    {
                        if (rastersUnkTimeFloats_dbl_990B58[order[i]]
                            < rastersUnkTimeFloats_dbl_990B58[order[j]])
                        {
                            swapped = order[i];
                            order[i] = order[j];
                            order[j] = swapped;
                        }
                    }
                }
                barY = 70.0;
                scale = (float)(tweak_perf_fps_target / (double)Frequency.QuadPart);
                for (i = 0; i < 54; i++)
                {
                    raster = order[i];
                    if (rasterNames[raster].unk1
                        && (double)PerfCountertimeSumArray[raster] * scale > 0.01)
                    {
                        barY = barY + 15.0;
                        if (barY > viewHeight)
                            break;
                    }
                }
                shown = 1;
                if (tweak_perf == 1)
                    shown = (int)i;
                barHeight = (float)((double)shown * 15.0);
                D3D_RastersSetAndDrawAQuad(0x80000000, 0.0, 50.0, viewWidth, barHeight, 1);
                //nullsub_32();
                row = 0;
                for (y = 70.0; row < shown; ++row)
                {
                    raster = order[row];
                    kind = rasterNames[raster].unk1;
                    value = (float)((double)PerfCountertimeSumArray[raster] * scale);
                    if (kind && value > 0.01)
                    {
                        barColor = 0xF0808020;
                        if ((((uint8_t)raster - 1) & 1) != 0)
                            barColor = 0xF0808060;
                        if ((((uint8_t)raster + 1) & 2) != 0)
                            barColor |= 0x4000u;
                        if ((((uint8_t)raster + 1) & 4) != 0)
                            barColor |= 0x400000u;
                        D3D_DrawARaster(barColor, rasterNames[raster].name, value, y, viewWidth);
                        nextY = y + 15.0;
                        y = (float)nextY;
                        if (nextY > viewHeight)
                            break;
                    }
                }
                D3D_DrawARaster(0xFFFFFF, "Frame Rate", fps, 50.0, viewWidth);
                //nullsub_32();
            }
            D3D_RastersDrawUPStream();
            D3D_RestoreEverythingFromBackup();
        }
        memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
        QueryPerformanceCounter(&after);
        if (PerfCounterStruct[47].QuadPart)
            PerfCountertimeSumArray[47] += after.QuadPart - PerfCounterStruct[47].QuadPart;
        PerfCounterStruct[47].QuadPart = 0LL;
        QueryPerformanceCounter(&PerfCounterStruct[19]);
        QueryPerformanceCounter(&PerfCounterStruct[53]);
    }
}

void D3D_CycleThroughVBOs()
{
    d3dStreamCurrentOffset = 0;
    currentVBOSelected = (currentVBOSelected + 1) % 3u;
}

void PC_FreeABackCache(struct sGEOGRO* geogro)
{
    //int v3;
    int i;

    if (geogro->d3dvb)
    {
        geogro->d3dvb->lpVtbl->Release(geogro->d3dvb);
        geogro->d3dvb = NULL;
    }
    if (geogro->d3dvb2)
    {
        geogro->d3dvb2->lpVtbl->Release(geogro->d3dvb2);
        geogro->d3dvb2 = NULL;
    }
    /* TODO FIXME
    v3 = *(_DWORD*)a1->gap4b;
    if (v3)
    {
        (*(void(__stdcall**)(_DWORD))(*(_DWORD*)v3 + 8))(*(_DWORD*)a1->gap4b);
        *(_DWORD*)a1->gap4b = 0;
    }
    */
    if (geogro->d3dUVVB)
    {
        for (i = 0; i < geogro->d3dUVVBNum; ++i)
        {
            if (geogro->d3dUVVB[i])
            {
                geogro->d3dUVVB[i]->lpVtbl->Release(geogro->d3dUVVB[i]);
                geogro->d3dUVVB[i] = NULL;
            }
        }
        MEM_Free(geogro->d3dUVVB);
        geogro->d3dUVVB = NULL;
    }
    if (geogro->d3dIndexBuf)
    {
        geogro->d3dIndexBuf->lpVtbl->Release(geogro->d3dIndexBuf);
        geogro->d3dIndexBuf = NULL;
    }
}

void D3D_ReleaseMeshSpecificData(tGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;
    tGEOGRO* geogro;
    tGEOSTATICLOD* lod;
    int i;

    if ((gao->flags & 0x4000) != 0)
    {
        visual = gao->basic->actorObjectOrVisualObject;
        geogro = (tGEOGRO*)visual->GROGEOOrSomethingElse;
        lod = (tGEOSTATICLOD*)visual->GROGEOOrSomethingElse;
        if (visual->GROGEOOrSomethingElse)
        {
            if (geogro->intf->id == 1)
            {
                if (geogro->triangleChunkCount && geogro->triangleChunks && geogro->vertexNum)
                    PC_FreeABackCache(geogro);
            }
            else if (lod->intf->id == 8)
            {
                for (i = 0; i < lod->lodNum; i++)
                {
                    if (lod->geo[i]->triangleChunkCount && lod->geo[i]->triangleChunks
                        && lod->geo[i]->vertexNum)
                        PC_FreeABackCache(lod->geo[i]);
                }
            }
        }
    }
}

void D3D_ReleaseMeshSpecificDataOfAWorld(tWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGroup* skeleton;
    tableDescStruct* table;
    tableElemStructPF* bone;
    tableElemStructPF* lastBone;

    if (world && world->objectTablePF.actElemNum)
    {
        last = ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1;
        for (elem = world->objectTablePF.firstElemPtr; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (!gao || ((uint8_t)(size_t)gao & 3) == 1)
                continue;
            D3D_ReleaseMeshSpecificData(gao);
            if ((gao->flags & 2) == 0)
                continue;
            skeleton = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup;
            if (!skeleton)
                continue;
            table = skeleton->tablePF;
            lastBone = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
            for (bone = table->firstElemPtr; bone <= lastBone; ++bone)
            {
                if (bone->objectPtr && ((uint8_t)(size_t)bone->objectPtr & 3) != 1)
                    D3D_ReleaseMeshSpecificData((tGameObjectMain*)bone->objectPtr);
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

void D3D_ReleaseSunRenderTargetAndTexture(struct sGfxSunD3DSpecificData* sun)
{
    if (sun->d3dTexture)
    {
        sun->d3dRenderTarget->lpVtbl->Release(sun->d3dRenderTarget);
        sun->d3dRenderTarget = NULL;
        sun->d3dTexture->lpVtbl->Release(sun->d3dTexture);
        sun->d3dTexture = NULL;
    }
}

void D3D_ReleaseSunSurfaces()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i])
            D3D_ReleaseSunRenderTargetAndTexture(gD3dSunSurfaces[i]);
    }
}

void D3D_ReleaseEveryCache()
{
    int i;

    D3D_ReleaseWaterIndices();
    D3D_ReleaseSunSurfaces();
    D3D_ReleaseEverything();
    D3D_ReleaseMeshSpecificDataOfAWorld(WOR_gpst_CurrentWorld);
    if (D3DRenderParams.d3dTextures)
    {
        for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
        {
            if (D3DRenderParams.d3dTextures[i])
                D3DRenderParams.d3dTextures[i]->lpVtbl->Release(D3DRenderParams.d3dTextures[i]);
        }
        MEM_Free(D3DRenderParams.d3dTextures);
        D3DRenderParams.d3dTextures = NULL;
    }
}

int D3D_CalculateStride(unsigned int fvf)
{
    int stride;
    unsigned int texCount;
    unsigned int texSizes;

    stride = 0;
    switch (fvf & 0x400E)
    {
    case 2u:
        stride = 12;
        break;
    case 4u:
    case 6u:
        stride = 16;
        break;
    case 8u:
        stride = 20;
        break;
    case 0xAu:
        stride = 24;
        break;
    case 0xCu:
        stride = 28;
        break;
    case 0xEu:
        stride = 32;
        break;
    default:
        break;
    }
    if ((fvf & 0x10) != 0)
        stride += 12;
    if ((fvf & 0x20) != 0)
        stride += 4;
    if ((fvf & 0x40) != 0)
        stride += 4;
    if ((fvf & 0x80u) != 0)
        stride += 4;
    texCount = (fvf >> 8) & 0xF;
    texSizes = (fvf >> 16) & 0xffff;
    if (texSizes)
    {
        for (; texCount; --texCount)
        {
            switch (texSizes & 3)
            {
            case 0u:
                stride += 8;
                break;
            case 1u:
                stride += 12;
                break;
            case 2u:
                stride += 16;
                break;
            case 3u:
                stride += 4;
                break;
            }
            texSizes >>= 2;
        }
    }
    else
    {
        stride += 8 * texCount;
    }
    return stride;
}

void D3D_CreateThreeVbos()
{
    int i;
    int result;

    for (i = 0; i < 3; i++)
    {
        result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
            D3DRenderParams.d3dDev,
            2097152,
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
            D3DPOOL_DEFAULT,
            &VBOs[i],
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    }
}

void D3D_CreateVBOs()
{
    int result;

    D3D_CreateThreeVbos();
    D3DRenderParams.prevVertexElemIdx2 = 0;
    D3DRenderParams.startVertexElemIdx2 = 0;
    D3DRenderParams.currTargetGao = 0;
    D3DRenderParams.xyzFVF = D3DFVF_XYZ;
    D3DRenderParams.stride2 = D3D_CalculateStride(D3DFVF_XYZ);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        65535 * D3DRenderParams.stride2,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.xyzFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dXYZVB,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = 0;
    D3DRenderParams.currVertexStartOffset = 0;
    D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    D3DRenderParams.normalsFVF = D3DFVF_NORMAL;
    D3DRenderParams.d3dStride = D3D_CalculateStride(D3DFVF_NORMAL);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        65535 * D3DRenderParams.d3dStride,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.normalsFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dNormalsBuffer,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    D3DRenderParams.prevVertexElemIdx = 0;
    D3DRenderParams.startVertexElemIdx = 0;
    D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
    D3DRenderParams.colorsFVF = D3DFVF_DIFFUSE;
    D3DRenderParams.stride = D3D_CalculateStride(D3DFVF_DIFFUSE);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        262143 * D3DRenderParams.stride,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.colorsFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dColorsVB,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
}

void D3D_SetColorWriteEnable(char flags)
{
    int mask;

    mask = 0;
    if ((flags & 0x20) == 0)
        mask = D3DCOLORWRITEENABLE_ALPHA;
    if ((flags & 0x40) == 0)
        mask |= (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, mask);
}

void D3D_SetCullingParams()
{
    int drawMask;
    int cullMode;

    drawMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    if ((drawMask & 0x800) != 0)
    {
        if ((drawMask & 0x2000) == 0)
            cullMode = D3DCULL_CCW;
        else
            cullMode = D3DCULL_CW;
    }
    else
    {
        cullMode = 1;
    }
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, cullMode);
}

void D3D_SetTextureStageStates(int stage, int textureID)
{
    if (textureID == -1)
    {
        if (textureStageTypeConfig[stage].d3d_colorarg1)
        { 
            textureStageTypeConfig[stage].d3d_colorarg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, 1, D3DTOP_SELECTARG1);
        if (textureStageTypeConfig[stage].d3d_alphaArg1)
        {
            textureStageTypeConfig[stage].d3d_alphaArg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    else if (textureID != -2)
    {
        if (D3DRenderParams.tweak_ae_d2x && D3DRenderParams.diffuse2x && textureID >= 0)
        {
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
        }
        else
        {
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        if (textureStageTypeConfig[stage].d3d_colorarg2)
        {
            textureStageTypeConfig[stage].d3d_colorarg2 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        if (textureStageTypeConfig[stage].d3d_alphaArg2)
        {
            textureStageTypeConfig[stage].d3d_alphaArg2 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        }
    }
}

void D3D_SetAlphaTestParams(unsigned int mask, int16_t flags)
{
    int compare;

    if ((flags & 0x10) != 0)
    {
        if (renderStateConfig.d3dAlphaTest != 1)
        {
            renderStateConfig.d3dAlphaTest = 1;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, TRUE);
        }
        compare = D3DCMP_LESS;
        if ((flags & 0x80u) == 0)
            compare = D3DCMP_GREATER;
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, compare);
        if (renderStateConfig.d3dAlphaRef != ((mask >> 22) & 0xFC))
        {
            renderStateConfig.d3dAlphaRef = (mask >> 22) & 0xFC;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAREF, (mask >> 22) & 0xFC);
        }
    }
    else if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, FALSE);
    }
    D3DRenderParams.colorMaskAlpha = (flags & 0x400) != 0 ? 0xFF000000 : 0;
}

void D3D_SetAlphaBlendingOptions(unsigned int mask, char flags)
{
    int destBlend;

    d3d_alphablendIsCase3 = 0;
    switch (((mask >> 16) & 0xffff) & 0xF)
    {
    case 0:
    case 7:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ZERO);
        return;
    case 1:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        if (flags >= 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            destBlend = 6;
        }
        else
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
            destBlend = 5;
        }
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, destBlend);
        return;
    case 2:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
        if (flags >= 0)
            destBlend = D3DBLEND_INVSRCALPHA;
        else
            destBlend = D3DBLEND_SRCALPHA;
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, destBlend);
        return;
    case 3:
        d3d_alphablendIsCase3 = 1;
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        if (flags >= 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
            D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
        }
        else
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
            D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, 7);
        }
        return;
    case 4:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        if (flags >= 0)
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
        else
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
        return;
    case 5:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
        return;
    case 6:
    case 8:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ZERO);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
        return;
    default:
        return;
    }
}

void D3D_SetFogParams(unsigned int mask)
{
    int mode;
    int enable;
    int color;

    mode = D3DRenderParams.d3dFogModeProbably;
    enable = (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x4000) != 0 && D3DRenderParams.d3dFogModeProbably;
    if (renderStateConfig.d3dFogEnable != enable)
    {
        renderStateConfig.d3dFogEnable = enable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, enable);
        mode = D3DRenderParams.d3dFogModeProbably;
    }
    if (!mode)
        return;
    switch (HIWORD(mask) & 0xF)
    {
    case 0:
    case 1:
    case 3:
    case 7:
        if (mode == 1)
            return;
        color = D3DRenderParams.d3dFogColor;
        D3DRenderParams.d3dFogModeProbably = 1;
        break;
    case 2:
    case 4:
        if (mode == 5)
            return;
        color = D3DRenderParams.d3dFogColor;
        D3DRenderParams.d3dFogModeProbably = 5;
        break;
    case 5:
    case 6:
        if (mode == 3)
            return;
        color = D3DRenderParams.d3dFogColor2Case5and6;
        D3DRenderParams.d3dFogModeProbably = 3;
        break;
    default:
        return;
    }
    D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, color);
}

void D3D_l_Texture_Init(int textureID, int flags, char mode)
{
    int filter;

    if ((mode & 1) != 0)
    {
        if (D3DRenderParams.textureUVIsProjected)
        {
            D3DRenderParams.textureUVMode = 4;
            memcpy(d3dTransformationMatrix, &D3DRenderParams.transformationMatrix, sizeof(D3DMATRIX));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&D3DRenderParams.transformationMatrix);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
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
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        }
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressu, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressv, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        if (textureStageTypeConfig[0].d3d_colorarg1)
        {
            textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        D3D_SetTexture(0, -2);
        D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, FALSE);
        D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, FALSE);
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    else if ((mode & 2) == 0)
    {
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x10) != 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, D3DFILL_SOLID);
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
        D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, 4 - ((flags & 0x100) != 0));
        if (renderStateConfig.d3d_zWriteEnable != ((flags & 0x200) == 0))
        {
            renderStateConfig.d3d_zWriteEnable = (flags & 0x200) == 0;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, renderStateConfig.d3d_zWriteEnable);
        }
        D3D_SetCullingParams();
        D3D_SetTexture(0, textureID);
        D3D_SetTextureStageStates(0, textureID);
        if (samplerConfig[0].d3d_addressu != (~(2 * (uint8_t)flags) & 2 | 1))
        {
            samplerConfig[0].d3d_addressu = ~(2 * (uint8_t)flags) & 2 | 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSU, samplerConfig[0].d3d_addressu);
        }
        if (samplerConfig[0].d3d_addressv != (~(uint8_t)flags & 2 | 1))
        {
            samplerConfig[0].d3d_addressv = ~(uint8_t)flags & 2 | 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSV, samplerConfig[0].d3d_addressv);
        }
        filter = ((flags & 4) != 0) + 1;
        if (samplerConfig[0].d3d_minfilter != filter)
        {
            samplerConfig[0].d3d_minfilter = ((flags & 4) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, samplerConfig[0].d3d_minfilter);
        }
        if (samplerConfig[0].d3d_magfilter != filter)
        {
            samplerConfig[0].d3d_magfilter = ((flags & 4) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, samplerConfig[0].d3d_magfilter);
        }
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_mipfilter, D3DSAMP_MIPFILTER, ((flags & 8) != 0) + 1);
        D3D_SamplerStateSetF(0, &samplerConfig[0].d3d_mipmaplodbias, D3DSAMP_MIPMAPLODBIAS, tweak_mipmaplodbias);
        D3D_SetColorWriteEnable(flags);
        D3D_SetAlphaTestParams(flags, flags & 0xFFF);
        D3D_SetAlphaBlendingOptions(flags, flags);
        D3D_SetFogParams(flags);
        gpst_DX_stDD->rasters[2].unk1 = textureID; // TODO FIXME, itt nem volt az IDAban unk1, jó ez így?
        gpst_DX_stDD->rasters[3].unk1 = flags; // TODO FIXME, itt nem volt az IDAban unk1, jó ez így?
    }
}

BOOL D3D_CheckDeviceFormatSupported(int usage, int resourceType, int format)
{
    return D3DRenderParams.d3d_if->lpVtbl->CheckDeviceFormat(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, D3DRenderParams.d3d_AdapterFormat, usage, resourceType, format) >= 0;
}

BOOL D3D_CheckDepthStencilMatch(int renderFormat, int depthFormat)
{
    return D3DRenderParams.d3d_if->lpVtbl->CheckDepthStencilMatch(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, D3DRenderParams.d3d_AdapterFormat, renderFormat, depthFormat) >= 0;
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
    int result;
    int format;

    result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        512,
        512,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &waterTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    waterTexture->lpVtbl->GetSurfaceLevel(waterTexture, 0, &d3dWaterRTtoSet);
    format = D3D_SelectSupportedDeviceFormat(D3DFMT_A8R8G8B8);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateDepthStencilSurface(D3DRenderParams.d3dDev,
        512,
        512,
        format,
        D3DMULTISAMPLE_NONE,
        0,
        FALSE,
        &d3dDSSurfaceToSet,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
}

void D3D_CreateFullScreenTextureAndGetDepthStencilSurface()
{
    int result;

    result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        D3DRenderParams.presentParams.BackBufferWidth,
        D3DRenderParams.presentParams.BackBufferHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DRenderParams.presentParams.BackBufferFormat,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.blurTex,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
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



void D3D_InitEverything(tGdi_DisplayData* dd)
{
    int i;
    int result;
    D3DVERTEXELEMENT9 decl[4];
    IDirect3DSurface9* surf;

    D3D_CreateVBOs();
    decl[0].Stream = 0;
    decl[0].Offset = 0;
    decl[0].Type = 2;
    decl[0].Method = 0;
    decl[0].Usage = 0;
    decl[0].UsageIndex = 0;
    decl[1].Stream = 1;
    decl[1].Offset = 0;
    decl[1].Type = 4;
    decl[1].Method = 0;
    decl[1].Usage = 10;
    decl[1].UsageIndex = 0;
    decl[2].Stream = 255;
    decl[2].Offset = 0;
    decl[2].Type = 17;
    decl[2].Method = 0;
    decl[2].Usage = 0;
    decl[2].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, decl, &D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
    decl[0].Stream = 0;
    decl[0].Offset = 0;
    decl[0].Type = 2;
    decl[0].Method = 0;
    decl[0].Usage = 0;
    decl[0].UsageIndex = 0;
    decl[1].Stream = 1;
    decl[1].Offset = 0;
    decl[1].Type = 4;
    decl[1].Method = 0;
    decl[1].Usage = 10;
    decl[1].UsageIndex = 0;
    decl[2].Stream = 2;
    decl[2].Offset = 0;
    decl[2].Type = 2;
    decl[2].Method = 0;
    decl[2].Usage = 3;
    decl[2].UsageIndex = 0;
    decl[3].Stream = 255;
    decl[3].Offset = 0;
    decl[3].Type = 17;
    decl[3].Method = 0;
    decl[3].Usage = 0;
    decl[3].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, decl, &D3DRenderParams.d3dVertexDeclaration);
    decl[0].Stream = 0;
    decl[0].Offset = 0;
    decl[0].Type = 2;
    decl[0].Method = 0;
    decl[0].Usage = 0;
    decl[0].UsageIndex = 0;
    decl[1].Stream = 1;
    decl[1].Offset = 0;
    decl[1].Type = 4;
    decl[1].Method = 0;
    decl[1].Usage = 10;
    decl[1].UsageIndex = 0;
    decl[2].Stream = 2;
    decl[2].Offset = 0;
    decl[2].Type = 1;
    decl[2].Method = 0;
    decl[2].Usage = 5;
    decl[2].UsageIndex = 0;
    decl[3].Stream = 255;
    decl[3].Offset = 0;
    decl[3].Type = 17;
    decl[3].Method = 0;
    decl[3].Usage = 0;
    decl[3].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, decl, &D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
    gpst_DX_stDD = dd;
    dd->rasters[3].unk1 = 0; // TODO FIXME ez biztos jó így ???? megnézni. az idaban csak siman a1->rasters[3] = 0 van de az nem jó
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
    D3D_SetProjectionMatrix(&dd->ct.cam);
    D3D_CreateWaterTexture();
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, D3DRenderParams.d3d_defaultZEnable);
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
    for (i = 7; i >= 0; i--)
    {
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, 128, 128, 1, 1, 21, 0, &D3DRenderParams.d3dShadowTextures[i], 0);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
        D3DRenderParams.d3dShadowTextures[i]->lpVtbl->GetSurfaceLevel(D3DRenderParams.d3dShadowTextures[i], 0, &surf);
        D3DRenderParams.d3dDev->lpVtbl->ColorFill(D3DRenderParams.d3dDev, surf, NULL, 0xffffffff);
        surf->lpVtbl->Release(surf);
    }
}

void D3D_CreateAndCopySourceTexturesToThem()
{
    unsigned int bytes;
    int i;
    int levels;
    int result;
    IDirect3DTexture9* copy;
    D3DSURFACE_DESC desc;

    bytes = 4 * D3DRenderParams.d3dTextureNum + 4;
    D3DRenderParams.d3dTextures = (IDirect3DTexture9**)MEM_p_Alloc(bytes);
    memset(D3DRenderParams.d3dTextures, 0, bytes);
    for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
    {
        copy = 0;
        if (D3DRenderParams.d3dSourceTextures[i])
        {
            D3DRenderParams.d3dSourceTextures[i]->lpVtbl->GetLevelDesc(D3DRenderParams.d3dSourceTextures[i], 0, &desc);
            levels = D3DRenderParams.d3dSourceTextures[i]->lpVtbl->GetLevelCount(D3DRenderParams.d3dSourceTextures[i]);
            result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
                desc.Width,
                desc.Height,
                levels,
                0,
                desc.Format,
                D3DPOOL_DEFAULT,
                &copy,
                NULL);
            D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
            D3DRenderParams.d3dSourceTextures[i]->lpVtbl->AddDirtyRect(D3DRenderParams.d3dSourceTextures[i], NULL);
            if (copy)
                D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)D3DRenderParams.d3dSourceTextures[i], (IDirect3DBaseTexture9*)copy);
        }
        D3DRenderParams.d3dTextures[i] = copy;
    }
}
void D3D_CreateIndexAndVertexBuffForElemsIfSuitable(tGameObjectMain* gao, tGEOGRO* geogro)
{
    int stride;
    int result;
    int chunk;
    int strip;
    int elem;
    int i;
    int buf;
    int found;
    int fits;
    int newBuf;
    int total;
    int pos;
    int streamStart;
    int streams;
    unsigned int word;
    tGeoWholeTriangleStripBlock* block;
    tGeoTriangleStrip* strips;
    tGeoTriangleStripElem* elems;
    tGeoUVElem* uv;
    tGeoUVElem* src;
    tD3dStreamInfoElem* info;
    void* locked;
    unsigned short* out;
    tGeoUVElem* uvBufs[50];

    if (worldPtrIfMultiStream) // TODO FIXME, Ennek a függvénynek az elsõ hívásánál ez biztos hogy 0 lesz, szóal csak a másodiknál lesznek ezek a pufferek létrehozva.
    {
        GEO_UseNormals(geogro);
        if ((gao->flags & 2) == 0 || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            if (!geogro->d3dvb)
            {
                stride = D3D_CalculateStride(D3DFVF_XYZ);
                result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                    D3DRenderParams.d3dDev,
                    stride * geogro->vertexNum,
                    D3DUSAGE_WRITEONLY,
                    D3DFVF_XYZ,
                    D3DPOOL_DEFAULT,
                    &geogro->d3dvb,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
                geogro->d3dvb->lpVtbl->Lock(geogro->d3dvb, 0, 0, &locked, 0);
                memcpy(locked, geogro->vertices, stride * geogro->vertexNum);
                geogro->d3dvb->lpVtbl->Unlock(geogro->d3dvb);
            }
            if (!geogro->d3dvb2)
            {
                stride = D3D_CalculateStride(D3DFVF_NORMAL);
                result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                    D3DRenderParams.d3dDev,
                    stride * geogro->vertexNum,
                    D3DUSAGE_WRITEONLY,
                    D3DFVF_NORMAL,
                    D3DPOOL_DEFAULT,
                    &geogro->d3dvb2,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
                geogro->d3dvb2->lpVtbl->Lock(geogro->d3dvb2, 0, 0, &locked, 0);
                memcpy(locked, geogro->normals, stride * geogro->vertexNum);
                geogro->d3dvb2->lpVtbl->Unlock(geogro->d3dvb2);
            }
        }
        if (!geogro->d3dUVVB && geogro->uvNum)
        {
            geogro->d3dUVVBNum = 1;
            geogro->d3dUVVB = (IDirect3DVertexBuffer9**)MEM_p_Alloc(4u);
            stride = D3D_CalculateStride(D3DFVF_TEX1);
            result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                D3DRenderParams.d3dDev,
                stride * geogro->vertexNum,
                D3DUSAGE_WRITEONLY,
                D3DFVF_TEX1,
                D3DPOOL_DEFAULT,
                &(geogro->d3dUVVB[0]),
                NULL);
            D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);

            geogro->d3dUVVB[0]->lpVtbl->Lock(geogro->d3dUVVB[0], 0, 0, (void**)&uvBufs[0], 0);

            if (geogro->vertexNum)
            {
                for (i = 0; i < (int)((sizeof(tGeoUVElem) * geogro->vertexNum - 1) >> 2) + 1; i++) {
                    ((float*)uvBufs[0])[i] = FLT_MAX;
     //               *((int*)(&(v88[0][i]))) = 0x7F7FFFFF; // ez FLOAT_MAX, TODO FIXME
                }
            }
            for (chunk = 0; chunk < (int)geogro->triangleChunkCount; chunk++)
            {
                block = geogro->triangleChunks[chunk].triangleStripBlock;
                if (!block)
                    continue;
                for (strip = 0; strip < (int)block->triangleStripNum; strip++)
                {
                    strips = &block->triangleStrips[strip];
                    found = -1;
                    for (buf = 0; buf < geogro->d3dUVVBNum; buf++)
                    {
                        uv = uvBufs[buf];
                        if (strips->triangleStripElemNum == 0)
                        {
                            found = buf;
                            break;
                        }
                        fits = 1;
                        for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                        {
                            i = strips->triangleStripElems[elem].triangleIdx;
                            src = &geogro->UVs[strips->triangleStripElems[elem].uvIdx];
                            if (uv[i].u != FLT_MAX && uv[i].u != src->u)
                            {
                                fits = 0;
                                break;
                            }
                            if (uv[i].v != FLT_MAX && uv[i].v != src->v)
                            {
                                fits = 0;
                                break;
                            }
                        }
                        if (fits)
                        {
                            found = buf;
                            break;
                        }
                    }
                    if (found >= 0)
                    {
                        uv = uvBufs[found];
                        for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                        {
                            elems = strips->triangleStripElems;
                            i = elems[elem].triangleIdx;
                            src = &geogro->UVs[elems[elem].uvIdx];
                            uv[i].u = src->u;
                            uv[i].v = src->v;
                        }
                        strips->vbIdx = found;
                        continue;
                    }
                    newBuf = geogro->d3dUVVBNum;
                    geogro->d3dUVVBNum = newBuf + 1;
                    geogro->d3dUVVB = (IDirect3DVertexBuffer9**)MEM_p_Realloc(geogro->d3dUVVB, 4 * (newBuf + 1));
                    stride = D3D_CalculateStride(D3DFVF_TEX1);
                    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                        D3DRenderParams.d3dDev,
                        stride * geogro->vertexNum,
                        D3DUSAGE_WRITEONLY,
                        D3DFVF_TEX1,
                        D3DPOOL_DEFAULT,
                        &geogro->d3dUVVB[newBuf],
                        NULL);
                    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
                    geogro->d3dUVVB[newBuf]->lpVtbl->Lock(geogro->d3dUVVB[newBuf], 0, 0, (void**)&uvBufs[newBuf], 0);
                    uv = uvBufs[newBuf];
                    if (geogro->vertexNum)
                    {
                        for (i = 0; i < (int)((sizeof(tGeoUVElem) * geogro->vertexNum - 1) >> 2) + 1; i++) {
                            ((float*)uv)[i] = FLT_MAX;
                                            //*((int*)(&(v40[i]))) = 0x7F7FFFFF; // ez FLOAT_MAX, TODO FIXME
                        }
                    }
                    for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                    {
                        elems = strips->triangleStripElems;
                        i = elems[elem].triangleIdx;
                        src = &geogro->UVs[elems[elem].uvIdx];
                        uv[i].u = src->u;
                        uv[i].v = src->v;
                    }
                    strips->vbIdx = newBuf;
                }
            }
            for (i = 0; i < geogro->d3dUVVBNum; ++i)
                geogro->d3dUVVB[i]->lpVtbl->Unlock(geogro->d3dUVVB[i]);
        }
        if (geogro->d3dIndexBuf || (int)geogro->triangleChunkCount <= 0)
            return;
        total = 0;
        for (chunk = 0; chunk < (int)geogro->triangleChunkCount; chunk++)
        {
            block = geogro->triangleChunks[chunk].triangleStripBlock;
            if (!block)
                continue;
            streams = 0;
            for (buf = 0; buf < geogro->d3dUVVBNum; buf++)
            {
                streamStart = total;
                for (strip = 0; strip < (int)block->triangleStripNum; strip++)
                {
                    strips = &block->triangleStrips[strip];
                    if (strips->vbIdx != buf)
                        continue;
                    if (streamStart != total)
                    {
                        total += 2;
                        if ((((uint8_t)total - (uint8_t)streamStart) & 1) != 0) // ez nembiztos hogy jó így TODO IFXME
                            ++total;
                    }
                    total += strips->triangleStripElemNum;
                }
                if (total != streamStart)
                    ++streams;
            }
            block->streamsNum = streams;
        }
        if (!total)
            return;
        pos = 0;
        result = D3DRenderParams.d3dDev->lpVtbl->CreateIndexBuffer(
            D3DRenderParams.d3dDev,
            2 * total,
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &geogro->d3dIndexBuf,
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
        geogro->d3dIndexBuf->lpVtbl->Lock(geogro->d3dIndexBuf, 0, 0, (void**)&out, 0); // ide uj v75 kéne
        for (chunk = 0; chunk < (int)geogro->triangleChunkCount; chunk++)
        {
            block = geogro->triangleChunks[chunk].triangleStripBlock;
            if (!block)
                continue;
            word = sizeof(tD3dStreamInfoElem) * block->streamsNum;
            streams = 0;
            block->d3dStreamInfoElems = (tD3dStreamInfoElem*)MEM_p_Alloc(word);
            for (buf = 0; buf < geogro->d3dUVVBNum; buf++)
            {
                if ((unsigned int)streams >= block->streamsNum)
                    break;
                block->d3dStreamInfoElems[streams].startIndex = pos;
                for (strip = 0; strip < (int)block->triangleStripNum; strip++)
                {
                    //v68 = v63[v77].triangleStrips;
                    strips = &block->triangleStrips[strip];
                    if (strips->vbIdx != buf)
                        continue;
                    info = &block->d3dStreamInfoElems[streams];
                    if (info->startIndex == pos)
                    {
                        info->vbIdx = buf;
                    }
                    else
                    {
                        *out = *(out - 1);
                        *++out = strips->triangleStripElems->triangleIdx;
                        pos += 2;
                        ++out;
                        if ((((uint8_t)pos - (uint8_t)block->d3dStreamInfoElems[streams].startIndex) & 1) != 0)
                        {
                            *out++ = strips->triangleStripElems->triangleIdx;
                            ++pos;
                        }
                    }
                    for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                        *out++ = strips->triangleStripElems[elem].triangleIdx;
                    pos += strips->triangleStripElemNum;
                }
                info = &block->d3dStreamInfoElems[streams];
                if (pos != info->startIndex)
                {
                    info->primitiveCount = pos - info->startIndex;
                    streams++;
                }
            }
        }
        geogro->d3dIndexBuf->lpVtbl->Unlock(geogro->d3dIndexBuf);
    }
}

void D3D_CreateIndexAndVertexBuffForGameObject(tGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;
    tGEOGRO* geogro;
    tGEOSTATICLOD* lod;
    int i;

    if ((gao->flags & 0x4000) != 0)
    {
        visual = (tGameObjectVisualObject*)gao->basic->actorObjectOrVisualObject;
        geogro = (tGEOGRO*)visual->GROGEOOrSomethingElse;
        lod = (tGEOSTATICLOD*)visual->GROGEOOrSomethingElse;
        if (visual->GROGEOOrSomethingElse)
        {
            if (geogro->intf->id == 1)
            {
                if (geogro->triangleChunkCount && geogro->triangleChunks && geogro->vertexNum)
                    D3D_CreateIndexAndVertexBuffForElemsIfSuitable(gao, geogro);
            }
            else if (lod->intf->id == 8)
            {
                for (i = 0; i < lod->lodNum; i++)
                {
                    if (lod->geo[i]->triangleChunkCount && lod->geo[i]->triangleChunks
                        && lod->geo[i]->vertexNum)
                        D3D_CreateIndexAndVertexBuffForElemsIfSuitable(gao, lod->geo[i]);
                }
            }
        }
    }
}

void D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGroup* skeleton;
    tableDescStruct* table;
    tableElemStructPF* bone;
    tableElemStructPF* lastBone;

    if (ENG_IsSingleStream)
        return;
    if (world && world->objectTablePF.actElemNum)
    {
        last = ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1;
        for (elem = world->objectTablePF.firstElemPtr; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (!gao || ((uint8_t)(size_t)gao & 3) == 1)
                continue;
            D3D_CreateIndexAndVertexBuffForGameObject(gao);
            if ((gao->flags & 2) == 0)
                continue;
            skeleton = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup;
            if (!skeleton)
                continue;
            table = skeleton->tablePF;
            lastBone = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
            for (bone = table->firstElemPtr; bone <= lastBone; ++bone)
            {
                if (bone->objectPtr && ((int)(size_t)bone->objectPtr & 3) != 1)
                    D3D_CreateIndexAndVertexBuffForGameObject((tGameObjectMain*)bone->objectPtr);
            }
        }
    }
    if (!worldPtrIfMultiStream)
        worldPtrIfMultiStream = world;
}

void D3D_CreateSunTextures(tGfxSunD3DSpecificData* sun)
{
    int result;

    if (!sun->d3dTexture)
    {
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
            16,
            1,
            1,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT,
            &sun->d3dTexture,
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
        sun->d3dTexture->lpVtbl->GetSurfaceLevel(sun->d3dTexture, 0, &sun->d3dRenderTarget);
    }
}

void D3D_CreateSunTexturesForSunSurfaces()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i])
            D3D_CreateSunTextures(gD3dSunSurfaces[i]);
    }
}

void D3D_WaterBuffersInit()
{
    unsigned int columns;
    int rows;
    short* idx;
    int vertex;
    unsigned int half;
    unsigned int rest;
    unsigned int i;
    int started;
    int row;
    short* indices;

    columns = wtr_NumA;
    rows = wtr_NumB;
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
        waterIndexData->lpVtbl->Lock(waterIndexData, 0, 0, (void **)&indices, 0);
        idx = indices;
        vertex = 0;
        started = 0;
        for (row = 0; row < rows - 1; row++)
        {
            if (started)
                *idx++ = vertex;
            else
                started = 1;
            half = columns >> 1;
            for (i = 0; i < half; i++)
            {
                *idx++ = vertex;
                *idx++ = columns + vertex;
                ++vertex;
            }
            rest = half - 1;
            --vertex;
            *idx++ = columns + vertex;
            if (rest < columns)
            {
                for (i = columns - rest; i; i--)
                {
                    *idx++ = columns + vertex;
                    *idx++ = vertex;
                    ++vertex;
                }
            }
            *idx++ = vertex - 1;
            *idx++ = vertex - 1;
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
    int result;
    int status;
    int resetResult;
    struct tagMSG Msg;

    Rasters_Update();
    if (D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 0;
    }
    result = D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL); 
    status = result;
    if (!ENG_gb_ExitApplication && !ENG_gb_ForceEndEngine)
    {
        if (result == D3DERR_DEVICELOST) //0x88760868
        {
            do
            {
                D3DRenderParams.dword_B317A0 = 1;
                while (status != D3DERR_DEVICENOTRESET) //0x88760869
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
                        status = D3DRenderParams.d3dDev->lpVtbl->TestCooperativeLevel(D3DRenderParams.d3dDev);
                    }
                }
                D3D_ReleaseEveryCache();
                resetResult = D3DRenderParams.d3dDev->lpVtbl->Reset(D3DRenderParams.d3dDev, &D3DRenderParams.presentParams); 
                status = resetResult;
            } while (resetResult == D3DERR_DEVICELOST);
            if (resetResult >= 0)
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

void D3D_Clear(int flags, int color) 
{
    if (flags == 2)
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    else
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL,
            (flags & D3DCLEAR_ZBUFFER) | D3DCLEAR_TARGET,
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
    D3DADAPTER_IDENTIFIER9 adapter;
    DWORD device;

    D3DRenderParams.d3d_if->lpVtbl->GetAdapterIdentifier(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, 0, &adapter);
    D3D_OutputDebugString("%d, %d, %d, %d\n", HIWORD(adapter.DriverVersion.HighPart), LOWORD(adapter.DriverVersion.HighPart), HIWORD(adapter.DriverVersion.LowPart), LOWORD(adapter.DriverVersion.LowPart));
    device = adapter.DeviceId;
    if (adapter.VendorId == 0x1002)
    {
        ENG_IsDLBFEnabled = 1;
        if (device >= 0x4144 && device <= 0x4146
            || device == 16740
            || device >= 0x4E44 && device <= 0x4E45
            || device >= 0x4E64 && device <= 0x4E65)
        {
            D3DRenderParams.argbIsSupportedForTextures = -1;
        }
        else if (device == 0x4C57
                 || device >= 0x4C59 && device <= 0x4C5A
                 || device >= 0x5144 && device <= 0x5147
                 || device == 0x5157
                 || device >= 0x5159 && device <= 0x515A)
        {
            ENG_IsSingleStream = 1;
        }
        return;
    }
    if (adapter.VendorId != 4318)
        return;
    switch (device)
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
        ENG_IsSingleStream = 1;
        break;
    case 0x170u:
    case 0x171u:
    case 0x172u:
    case 0x173u:
    case 0x177u:
        ENG_IsSingleStream = 1;
        break;
    default:
        break;
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
        MessageBoxA(hWnd, "Application settings is not correctly set or invalid, please re-run settings application.", "IRISEngine", MB_TASKMODAL | MB_ICONERROR);
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
    }
    else
    {
        D3DRenderParams.d3d_if->lpVtbl->EnumAdapterModes(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DRenderParams.d3d_AdapterFormat, adapterMode, &dispMode);
        D3DRenderParams.presentParams.Windowed = 0;
        D3DRenderParams.presentParams.FullScreen_RefreshRateInHz = dispMode.RefreshRate;
        D3DRenderParams.Rect.right = dispMode.Width;
        D3DRenderParams.Rect.bottom = dispMode.Height;
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
                MessageBoxA(hWnd, "IDirect3D9_CreateDevice failed, please re-run settings application.", "IRISEngine", MB_TASKMODAL | MB_ICONERROR);
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
    IDirect3DSurface9* surface;

    D3DRenderParams.blurTex->lpVtbl->GetSurfaceLevel(D3DRenderParams.blurTex, 0, &surface);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surface);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, D3DRenderParams.newZStencil);
    surface->lpVtbl->Release(surface);
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

void D3D_Request_SetFogParams(tFogParams* fog)
{
    int fogColor;

    if ((fog->flags & 1) != 0)
    {
        if ((D3DRenderParams.d3dcaps.RasterCaps & 0x100) != 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dFogTableMode, D3DRS_FOGTABLEMODE, engineFogTableModeToD3D[fog->fogVertexMode]);
        }
        else if ((D3DRenderParams.d3dcaps.RasterCaps & 0x80) != 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dFogVertexMode, D3DRS_FOGVERTEXMODE, engineFogTableModeToD3D[fog->fogVertexMode]);
        }
        else
        {
            D3DRenderParams.d3dFogModeProbably = 0;
            D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
            return;
        }
        fogColor = (fog->fogColor & 0xFF00FF00 | ((fog->fogColor >> 16) & 0xff) | ((uint8_t)fog->fogColor << 16));
        D3DRenderParams.d3dFogColor = fogColor;
        D3DRenderParams.d3dFogColor2Case5and6 = 0;
        D3D_RenderStateSetF(&renderStateConfig.d3dFogDensity, D3DRS_FOGDENSITY, fog->density);
        D3DRenderParams.d3dFogModeProbably = 1;
        D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, D3DRenderParams.d3dFogColor);
        D3D_RenderStateSetF(&renderStateConfig.d3dFogStart, D3DRS_FOGSTART, fog->fogStart);
        D3D_RenderStateSetF(&renderStateConfig.d3dFogEnd, D3DRS_FOGEND, fog->fogEnd);
        D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, TRUE);
        return;
    }
    D3DRenderParams.d3dFogModeProbably = 0;
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
}

int D3D_FillWaterStreamData(struct sWaterParamsGlobalStruct* water)
{
    float camX;
    float camY;
    float camZ;
    int align;
    unsigned int rows;
    unsigned int row;
    double fade;
    double height;
    double ripple;
    int packed;
    int green;
    uint8_t blue;
    float scale;
    Vec3* pos;
    int* colors;
    Vec3* uvVec;
    Vec3* rowEnd;
    tVertex_type_water vertex;
    tVertex_type_water* out;

    camX = gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
    camZ = gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
    camY = gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
    waterStreamData->lpVtbl->Lock(waterStreamData, 0, 0, (void**)&out, 0x2000);
    // align 16
    align = (uint8_t)(size_t)out & 0xF;
    waterOffsetInBytes = 16 - align;
    // preserved: align is masked to 0..15, so this test can never fail
    if (align != 16)
        out = (tVertex_type_water*)((uint8_t*)out + 16 - align);
    pos = water->vertices;
    colors = water->colors;
    uvVec = water->vecsForUVs;
    vertex.specular = 0;
    if (tweak_water_blur_normal)
        WTR_WaterBlurNormal(water->vecsForUVs, water->horizontalVertexNum, water->verticallVertexNum);
    rows = water->verticallVertexNum;
    for (row = 0; row < rows; row++)
    {
        fade = (double)row * 1.2 / (double)rows;
        if (fade > 1.0)
            fade = 1.0;
        rowEnd = &pos[water->horizontalVertexNum];
        scale = (float)(48.0 - fade * 45.0);
        while (pos < rowEnd)
        {
            vertex.v.x = pos->x;
            vertex.normal.x = vertex.v.x - camX;
            vertex.v.y = pos->y;
            height = pos->y;
            green = *((uint8_t*)colors + 2);
            vertex.v.z = pos->z;
            packed = *colors;
            blue = *colors;
            vertex.normal.y = (float)(height - camY);
            ripple = uvVec->y + uvVec->x;
            vertex.diffuse = packed & 0xFF00FF00 | green | (blue << 16);
            ++pos;
            ++colors;
            ++uvVec;
            vertex.normal.z = (float)(vertex.v.z - camZ - fabs(ripple * scale));
            memcpy(out, &vertex, sizeof(vertex));
            ++out;
        }
        rows = water->verticallVertexNum;
    }
    return waterStreamData->lpVtbl->Unlock(waterStreamData);
}

void D3D_SetTransformAndSaveMatrix(IDirect3DTexture9* texture, int mtxnum)
{
    double invHeight;
    double halfHeight;
    float width;
    float height;
    IDirect3DSurface9* surface;
    D3DMATRIX scaleMtx;
    D3DSURFACE_DESC desc;
    D3DMATRIX flipped;
    D3DMATRIX result;

    memcpy(&flipped, &d3dProjectionMtx, sizeof(flipped));
    flipped._22 = -flipped._22;
    memset(&scaleMtx, 0, sizeof(scaleMtx));
    texture->lpVtbl->GetSurfaceLevel(texture, 0, &surface);
    surface->lpVtbl->GetDesc(surface, &desc);
    width = (float)desc.Width;
    height = (float)desc.Height;
    surface->lpVtbl->Release(surface);
    scaleMtx._33 = 1.0;
    scaleMtx._44 = 1.0;
    scaleMtx._43 = 0.0;
    scaleMtx._11 = (float)(512.0 / width * 0.5);
    invHeight = 1.0 / height;
    halfHeight = 512.0 * invHeight * 0.5;
    scaleMtx._22 = (float)(halfHeight - invHeight);
    scaleMtx._41 = scaleMtx._11;
    scaleMtx._42 = (float)(halfHeight + invHeight);
    D3DXMatrixMultiply(&result, &flipped, &scaleMtx);
    memcpy(&d3dTransformationMatrix[mtxnum], &result, sizeof(Mtx4x4WithoutAdditionalData));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, mtxnum + D3DTS_TEXTURE0, &result);
}

IDirect3DTexture9* D3D_GetWaterTexture()
{
    water_ClippingHasBeenSet = 0;
    if (gMaxSimultaneousTextures >= 4 && tweak_water_smooth)
        D3D_DoBlurWaterTexture();
    return waterTexture;
}


void D3D_DrawWater_DrawVertexTable(struct sWaterParamsGlobalStruct* water)
{
    IDirect3DTexture9* texture;
    Mtx4x4 camMtx;
    Mtx4x4 invCamMtx;
    Mtx4x4 viewMtx;
    Mtx4x4 worldMtx;

    wtr_NumB = water->verticallVertexNum;
    wtr_NumA = water->horizontalVertexNum;
    D3D_WaterBuffersInit();
    D3D_FillWaterStreamData(water);
    if (&camMtx != &gpst_DX_stDD->ct.cam.objectMtx)
        memcpy(&camMtx, &gpst_DX_stDD->ct.cam.objectMtx, sizeof(camMtx));
    MATH_InvertMatrix(&invCamMtx, &camMtx);
    invCamMtx._m34 = 0.0;
    invCamMtx._m24 = 0.0;
    invCamMtx._m14 = 0.0;
    invCamMtx.w = 1.0;
    MATH_MakeOGLMatrix(&viewMtx, &invCamMtx);
    camMtx.v1.x = 1.0;
    camMtx.v2.y = 1.0;
    camMtx.v3.z = 1.0;
    camMtx.w = 1.0;
    viewMtx.w = 1.0;
    camMtx.v1.y = 0.0;
    camMtx.v1.z = 0.0;
    camMtx._m14 = 0.0;
    camMtx.v2.x = 0.0;
    camMtx.v2.z = 0.0;
    camMtx._m24 = 0.0;
    camMtx.v3.x = 0.0;
    camMtx.v3.y = 0.0;
    camMtx._m34 = 0.0;
    camMtx.Pos.x = 0.0;
    camMtx.Pos.y = 0.0;
    camMtx.Pos.z = 0.0;
    camMtx.mtxflag = camMtx.mtxflag & 2;
    MATH_MakeOGLMatrix(&worldMtx, &camMtx);
    D3D_SaveEverythingToBackup(1);
    texture = D3D_GetWaterTexture();
    memcpy(&d3dTransformMatrix, &viewMtx, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&viewMtx);
    memcpy(&d3dTransformMatrix2, &worldMtx, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, (D3DMATRIX*)&worldMtx);
    D3D_SetTransformAndSaveMatrix(texture, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, 11, 0x10000);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, 24, 0x103);
    D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals, 143, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, waterStreamData, waterOffsetInBytes, 32);
    D3DRenderParams.d3dDev->lpVtbl->SetIndices(D3DRenderParams.d3dDev, waterIndexData);
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, 210);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, 27, 1);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, 137, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorOp, 1, 12);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1, 2, 2);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2, 3, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaOp, 4, 1);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_magfilter, 5, 2);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_minfilter, 6, 2);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressu, 1, 3);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressv, 2, 3);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, 19, 2);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, 20, 1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, 168, 15);
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, 28, 1);
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, 14, 0);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, 23, 4);
    //D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    D3DRenderParams.d3dDev->lpVtbl->DrawIndexedPrimitive(D3DRenderParams.d3dDev,
        5,
        0,
        0,
        water->verticallVertexNum * water->horizontalVertexNum,
        0,
        wtr_primitiveCount);
    //D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_SOLID);
    D3D_RestoreEverythingFromBackup();
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, 14, 1);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, 23, 4);
}

void D3D_PrepareWaterRendering()
{
    float clipPlane[4];

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
            clipPlane[3] = tweak_water_off + waterHeight;
            clipPlane[0] = 0.0;
            clipPlane[1] = 0.0;
            clipPlane[2] = -1.0;
            D3DRenderParams.d3dDev->lpVtbl->SetClipPlane(D3DRenderParams.d3dDev, 0, clipPlane);
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
    tGameObjectBV* bv;
    long double extent;
    Mtx4x4 worldMtx;

    if (!water_ClippingHasBeenSet)
        return 0;
    bv = gpst_DX_stDD->currTargetGao->boundingVolume;
    extent = OBJ_BV_IsSphere(bv) ? bv->BVMax.x : fabs(bv->BVMax.y - bv->BVMin.y)
        + fabs(bv->BVMax.x - bv->BVMin.x);
    if (extent < tweak_water_min_bv)
        return 0;
    QueryPerformanceCounter(&PerfCounterStruct[48]);
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &d3dRTToGet);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &d3dWaterDSSurfaceToGet);
    D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dWaterRTtoSet);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dDSSurfaceToSet);
    d3dCullModeBackup = renderStateConfig.d3dCullMode;
    if (renderStateConfig.d3dCullMode == 2)
        D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, 3);
    else if (renderStateConfig.d3dCullMode == 3)
        D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, 2);
    memcpy(&d3dTransformMatrixBackup, &d3dTransformMatrix, sizeof(d3dTransformMatrix));
    memcpy(&d3dTransformMatrix2Backup, &d3dTransformMatrix2, sizeof(d3dTransformMatrix2));
    MATH_MakeOGLMatrix(&worldMtx, gpst_DX_stDD->currTargetGao->matrices);
    worldMtx.v1.z = -worldMtx.v1.z;
    worldMtx.v2.z = -worldMtx.v2.z;
    worldMtx.v3.z = -worldMtx.v3.z;
    worldMtx.Pos.z = worldMtx.Pos.z - (worldMtx.Pos.z - waterHeight + worldMtx.Pos.z - waterHeight);
    memcpy(&d3dTransformMatrix2, &worldMtx, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, (D3DMATRIX*)&worldMtx);
    memcpy(&d3dTransformMatrix, &d3dCameraMtx, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&d3dCameraMtx);
    if (tweak_water_clip)
    {
        D3D_RenderStateSet(&renderStateConfig.d3dClipPlaneEnable, 152, 1);
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
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, d3dCullModeBackup);
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

tVertex_type_142_generic* D3D_LockCurrentVBO(int vertexNum)
{
    int offset;
    int flags;
    tVertex_type_142_generic* locked;

    offset = d3dStreamCurrentOffset;
    flags = 4096;
    if ((unsigned int)(d3dStreamCurrentOffset + sizeof(tVertex_type_142_generic) * vertexNum) >= 2097152)
    {
        offset = 0;
        d3dStreamCurrentOffset = 0;
        flags = 8192;
    }
    VBOs[currentVBOSelected]->lpVtbl->Lock(VBOs[currentVBOSelected], offset, sizeof(tVertex_type_142_generic) * vertexNum, (void **)&locked, flags);
    d3dStreamActDataSize = sizeof(tVertex_type_142_generic) * vertexNum;
    return locked;
}

void D3D_FillCurrentVBO(int stream)
{
    VBOs[currentVBOSelected]->lpVtbl->Unlock(VBOs[currentVBOSelected]);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, stream, VBOs[currentVBOSelected], d3dStreamCurrentOffset, sizeof(tVertex_type_142_generic));
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

void D3D_Request_PrepareForDrawing(int spriteNum)
{
    if (spriteNum)
    {
        D3D_RenderStateSet(&renderStateConfig.d3dFillMode, 8, 3);
        D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, 2);
        d3dRequestPrimitiveCount = 2 * spriteNum;
        d3dRequestPrimitiveCount2 = 2 * spriteNum;
        d3d_requestDrawSelectedVBO = D3D_LockCurrentVBO(6 * spriteNum);
    }
}

void D3D_FillSelectedVBOWithSprite(tD3dSpriteData* sprite)
{
    tVertex_type_142_generic* out;
    unsigned int color;

    out = d3d_requestDrawSelectedVBO;
    if (d3d_requestDrawSelectedVBO && d3dRequestPrimitiveCount2)
    {
        color = sprite->color & 0xFF00FF00 | ((sprite->color >> 16) & 0xff) | ((uint8_t)sprite->color << 16);
        out[0].v1 = sprite->v1;
        out[0].color = color;
        out[0].uvSet1.u = 1.0f;
        out[0].uvSet1.v = 1.0f;
        out[1].v1 = sprite->v2;
        out[1].color = color;
        out[1].uvSet1.u = 0.0f;
        out[1].uvSet1.v = 1.0f;
        out[2].v1 = sprite->v3;
        out[2].color = color;
        out[2].uvSet1.u = 0.0f;
        out[2].uvSet1.v = 0.0f;
        out[3].v1 = sprite->v1;
        out[3].color = color;
        out[3].uvSet1.u = 1.0f;
        out[3].uvSet1.v = 1.0f;
        out[4].v1 = sprite->v3;
        out[4].color = color;
        out[4].uvSet1.u = 0.0f;
        out[4].uvSet1.v = 0.0f;
        out[5].v1 = sprite->v4;
        out[5].color = color;
        out[5].uvSet1.u = 1.0f;
        out[5].uvSet1.v = 0.0f;
        d3d_requestDrawSelectedVBO = out + 6;
        d3dRequestPrimitiveCount2 -= 2;
    }
}

int D3D_l_Request(int type, void *param)
{
    switch (type)
    {
    case 1:
        D3D_Request_SetFogParams((tFogParams*)param);
        break;
    case 12:
        D3D_TextureStore((tTexUploadRequest*)param);
        break;
    case 18:
        D3D_Request_PrepareForDrawing((int)(size_t)param);
        break;
    case 19:
    case 33:
        D3D_FillSelectedVBOWithSprite((tD3dSpriteData*)param);
        break;
    case 20:
        D3D_Request_SpriteDraw();
        break;
    case 24:
        D3D_GetSnapShot((tD3dRequestSnapshot*)param);
        break;
    case 25:
        D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, (int)(size_t)param);
        break;
    default:
        break;
    }
    return 0;
}

int D3D_l_Texture_Store(struct sGdi_DisplayData* dd, int textureNum)
{
    int count;
    unsigned int bytes;

    count = textureNum;
    if (!textureNum)
        count = 1;
    bytes = 4 * (count + 1);
    D3DRenderParams.d3dTextureNum = count;
    D3DRenderParams.d3dTextures = (IDirect3DTexture9 **)MEM_p_Alloc(bytes);
    memset(D3DRenderParams.d3dTextures, 0, bytes);
    D3DRenderParams.d3dSourceTextures = (IDirect3DTexture9**)MEM_p_Alloc(bytes);
    memset(D3DRenderParams.d3dSourceTextures, 0, bytes);
    return 1;
}

void D3D_Palette_Load(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* inter, int idx)
{
    int depth;
    int width;
    int height;
    int16_t texFlags;
    int mipLevels;
    int result;
    IDirect3DTexture9* source;
    IDirect3DTexture9* dest;
    char* srcRow;
    char* destRow;
    unsigned int rowBytes;
    int row;
    IDirect3DSurface9* surface;
    char* pixels;
    D3DLOCKED_RECT locked;

    depth = inter->colorDepth2;
    width = tle->normalData.width;
    pixels = inter->imgDataPtr;
    height = tle->normalData.height;
    if (!D3DRenderParams.Mipmapping
        || (texFlags = inter->tp.flagsUnkAndProceduralUnkb1, (texFlags & 0x40) != 0)
        || (texFlags & 0x200) != 0
        || inter->tp.type == 5)
    {
        inter->bmpUnkb1 &= ~0x10u;
        mipLevels = 1;
    }
    else
    {
        inter->bmpUnkb1 |= 0x10u;
        mipLevels = 0;
    }
    tle->texElemFlag = inter->bmpUnkb1;
    source = 0;
    dest = 0;
    if (depth != 32 && depth != 24)
    {
        D3DRenderParams.d3dTextures[idx] = NULL;
        D3DRenderParams.d3dSourceTextures[idx] = NULL;
        return;
    }
    if (D3DRenderParams.Mipmapping && D3DRenderParams.argbIsSupportedForTextures == 1)
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, height, width, 1, 0, 21, 2, &source, 0);
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev,
            height,
            width,
            mipLevels,
            0x400,
            21,
            0,
            &dest,
            0);
    }
    else
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, height, width, mipLevels, 0, 21, 2, &source, 0);
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev,
            height,
            width,
            mipLevels,
            0,
            21,
            0,
            &dest,
            0);
    }
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    if (dest && source)
    {
        if (depth == 32)
            dd->texManager.paletteUsedMemory += 4 * height * width;
        else
            dd->texManager.paletteUsedMemory += 3 * height * width;
        source->lpVtbl->GetSurfaceLevel(source, 0, &surface);
        surface->lpVtbl->LockRect(surface, &locked, 0, 0);
        destRow = (char*)locked.pBits;
        srcRow = pixels;
        rowBytes = 4 * height;
        for (row = 0; row < width; row++)
        {
            memcpy(destRow, srcRow, rowBytes);
            srcRow += rowBytes;
            destRow += locked.Pitch;
        }
        surface->lpVtbl->UnlockRect(surface);
        surface->lpVtbl->Release(surface);
        if (D3DRenderParams.Mipmapping && D3DRenderParams.argbIsSupportedForTextures != 1)
            D3DXFilterTexture((IDirect3DBaseTexture9*)source, NULL, D3DX_DEFAULT, D3DX_DEFAULT);
        D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)source, (IDirect3DBaseTexture9*)dest);
    }
    else
    {
        if (dest)
        {
            dest->lpVtbl->Release(dest);
            dest = NULL;
        }
        if (source)
        {
            source->lpVtbl->Release(source);
            source = NULL;
        }
    }
    D3DRenderParams.d3dTextures[idx] = dest;
    D3DRenderParams.d3dSourceTextures[idx] = source;
}

int D3D_DrawSprites(struct sGeoSprites* sprites, void* positions, int num)
{
    tBigBufferStruct* bb;
    int* colors;
    int* second;
    int mask;
    int alpha;
    int color;
    int idx;
    Mtx4x4* mtx;
    tGeoSpriteElem* elem;
    tGeoSpriteElem* elemEnd;
    int offset;
    tSpriteC0DEData* c0de;
    tSpriteC0DEData* c0deEnd;
    double size;
    Vec3* base;
    double sine;
    double wobble;
    float rightX, rightY, rightZ;
    float upX, upY, upZ;
    float c1x, c1y, c1z;
    float c2x, c2y, c2z;
    float c3x, c3y, c3z;
    float c4x, c4y, c4z;
    float wave;
    tGdi_DisplayData* dd;
    tD3dSpriteData out;

    bb = gpst_DX_stDD->bigBuffer;
    colors = bb->vertexColorArray;
    second = bb->vertexColorArraySecond;
    mask = bb->colorMask;
    mtx = gpst_DX_stDD->matrixStack.actMatrixElem;
    alpha = D3DRenderParams.colorMaskAlpha;
    color = mask ^ (alpha | bb->solidVertexColor);
    dd = gpst_DX_stDD;
    rightX = -(mtx->v1.x * sprites->globalSize * sprites->globalRatio);
    rightY = -(mtx->v2.x * sprites->globalSize * sprites->globalRatio);
    rightZ = -(mtx->v3.x * sprites->globalSize * sprites->globalRatio);
    upX = mtx->v1.y * sprites->globalSize;
    upY = mtx->v2.y * sprites->globalSize;
    upZ = mtx->v3.y * sprites->globalSize;
    c1x = -rightX - upX;
    c1y = -rightY - upY;
    c1z = -rightZ - upZ;
    c2x = rightX - upX;
    c2y = rightY - upY;
    c2z = rightZ - upZ;
    c3x = upX + rightX;
    c3y = upY + rightY;
    c3z = upZ + rightZ;
    c4x = upX - rightX;
    c4y = upY - rightY;
    c4z = upZ - rightZ;
    gpst_DX_stDD->Request(18, (void *) (size_t) sprites->num);
    elem = sprites->spriteElems;
    elemEnd = &elem[sprites->num];
    if (elem->idx == 0xC0DE) // see gfxexplode.c GFX_SetNumberOfSprites
    {
        offset = elem[1].idx;
        c0de = &((tSpriteC0DEData *)positions)[offset];
        c0deEnd = &c0de[sprites->num];
        if (colors)
            colors = &colors[offset];
        if (second)
            second += offset;
        for (; c0de < c0deEnd; c0de++)
        {
            if (colors)
            {
                color = mask ^ (alpha | *colors);
                // preserved: elem still points at the 0xC0DE marker, so this reads second[49374]
                if (second)
                    color = second[elem->idx] | color & 0xFFFFFF;
                colors++;
            }
            else if (second)
            {
                color = second[elem->idx] | color & 0xFFFFFF;
            }
            out.color = color;
            size = c0de->f1;
            out.v1.x = (float)(c1x * size + c0de->v1.x);
            out.v1.y = (float)(c1y * size + c0de->v1.y);
            out.v1.z = (float)(c1z * size + c0de->v1.z);
            out.v2.x = (float)(c2x * size + c0de->v1.x);
            out.v2.y = (float)(c2y * size + c0de->v1.y);
            out.v2.z = (float)(c2z * size + c0de->v1.z);
            out.v3.x = (float)(c3x * size + c0de->v1.x);
            out.v3.y = (float)(c3y * size + c0de->v1.y);
            out.v3.z = (float)(c3z * size + c0de->v1.z);
            out.v4.x = (float)(c4x * size + c0de->v1.x);
            out.v4.y = (float)(c4y * size + c0de->v1.y);
            out.v4.z = (float)(c4z * size + c0de->v1.z);
            dd->Request(33, &out);
        }
    }
    else
    {
        wave = 0.0;
        for (; elem < elemEnd; ++elem)
        {
            wave = wave + 1.0f;
            if (colors)
            {
                idx = elem->idx;
                color = mask ^ (alpha | colors[idx]);
                if (second)
                    color = second[idx] | color & 0xFFFFFF;
            }
            else if (second)
            {
                color = second[elem->idx] | color & 0xFFFFFF;
            }
            out.color = color;
            size = elem->size;
            base = &(((Vec3*)positions)[elem->idx]);
            out.v1.x = (float)(c1x * size + base->x);
            out.v1.y = (float)(c1y * size + base->y);
            out.v1.z = (float)(c1z * size + base->z);
            out.v2.x = (float)(c2x * size + base->x);
            out.v2.y = (float)(c2y * size + base->y);
            out.v2.z = (float)(c2z * size + base->z);
            out.v3.x = (float)(c3x * size + base->x);
            out.v3.y = (float)(c3y * size + base->y);
            out.v3.z = (float)(c3z * size + base->z);
            out.v4.x = (float)(c4x * size + base->x);
            out.v4.y = (float)(c4y * size + base->y);
            out.v4.z = (float)(c4z * size + base->z);
            if (GDI_gb_WaveSprite)
            {
                sine = MATH_GetSin(wave);
                wobble = sine * MATH_GetSin((float)(sine * TIM_gul_TicksCount * 2.5)) * 0.2;

                out.v1.x = (float)(out.v1.x + wobble);
                out.v2.x = (float)(wobble + out.v2.x);
            }
            dd->Request(33, &out);
        }
    }
    dd->Request(20, 0);
    return 0;
}

static unsigned int D3D_VertexColor(int* colors, int* second, int mask, int alpha,
                                    int idx, int secondIdx)
{
    unsigned int packed;

    packed = mask ^ (alpha | colors[idx]);
    if (second)
        packed = second[secondIdx] | packed & 0xFFFFFF;
    return packed & 0xFF00FF00 | ((uint8_t)packed << 16) | ((packed >> 16) & 0xff);
}

static void D3D_DrawIndexedStreams(tGEOGRO* geogro, tGeoWholeTriangleStripBlock* block)
{
    unsigned int i;
    tD3dStreamInfoElem* info;

    for (i = 0; i < block->streamsNum; i++)
    {
        info = &block->d3dStreamInfoElems[i];
        if (D3DRenderParams.textureUVMode != 2)
        {
            if (geogro->d3dUVVB)
            {
                D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, geogro->d3dUVVB[info->vbIdx], 0, 8);
                D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                    D3DRenderParams.d3dDev,
                    D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
            }
            else
            {
                D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, 0, 0, 0);
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
            geogro->vertexNum,
            info->startIndex,
            info->primitiveCount - 2);
    }
}

int D3D_l_DrawElementIndexedTriangles(struct sGeoTriangleChunks* triangleChunks, Vec3* verticesIN, struct sGeoUVElem* UVs, int vertexNum)
{
    tGEOGRO* geogro;
    tGeoWholeTriangleStripBlock* block;
    tBigBufferStruct* bb;
    tGeoTriangleChunkElem* tri;
    tGeoTriangleChunkElem* triEnd;
    tVertex_type_142_generic* out;
    unsigned int triangleNum;
    int immediate;
    int mismatch;
    int useVertexUv;
    int* colors;
    int* second;
    int mask;
    int alpha;
    int solid;
    int savedArg1;
    int savedArg2;
    int start;
    int lockFlags;
    int idx;
    int secondIdx;
    int k;
    unsigned int i;
    void* locked;
    unsigned int* lockedColors;
    unsigned int solidColor = 0;// TODO FIXME ???????? játék nem initeli

    triangleNum = triangleChunks->triangleNum;
    if (!triangleNum)
        return 0;
    geogro = gpst_DX_stDD->GROGEO;
    block = NULL;
    immediate = 1;
    if (geogro)
    {
        mismatch = 0;
        if (geogro->UVs != UVs && D3DRenderParams.textureUVMode != 2)
            mismatch = 1;
        if (verticesIN == geogro->vertices && (gpst_DX_stDD->flags & 0x400) != 0)
            mismatch = 1;
        block = triangleChunks->triangleStripBlock;
        immediate = !geogro->d3dIndexBuf || !block || mismatch;
    }
    if (immediate)
    {
        savedArg1 = 0;
        savedArg2 = 0;
        out = D3D_LockCurrentVBO(3 * triangleNum);
        bb = gpst_DX_stDD->bigBuffer;
        second = bb->vertexColorArraySecond;
        alpha = gpst_DX_stDD->pd3drp->colorMaskAlpha;
        mask = bb->colorMask;
        colors = bb->vertexColorArray;
        if (!colors)
        {
            solidColor = (mask ^ (alpha | bb->solidVertexColor)) & 0xFF00FF00 | ((uint8_t)(mask ^ (alpha | (bb->solidVertexColor & 0xff))) << 16) | (uint8_t)((mask ^ (alpha | bb->solidVertexColor)) >> 16);
        }
        if (D3DRenderParams.textureUVMode == 2)
        {
            // preserved: the colour arg 2 backup is taken from arg 1
            savedArg1 = textureStageTypeConfig[0].d3d_colorarg1;
            savedArg2 = textureStageTypeConfig[0].d3d_colorarg1;
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1, 2, 0);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2, 3, 0);
            colors = 0;
            solidColor = 0;
        }
        useVertexUv = (gpst_DX_stDD->flags & 2) != 0;
        tri = triangleChunks->triangleChunkElems;
        triEnd = &tri[triangleNum];
        for (; tri < triEnd; tri++)
        {
            for (k = 0; k < 3; k++)
            {
                if (k == 0)
                    idx = tri->v1;
                else if (k == 1)
                    idx = tri->v2;
                else
                    idx = tri->v3;
                out->v1 = verticesIN[idx];
                if (colors)
                {
                    secondIdx = idx;
                    // preserved: this one of nine second-colour lookups scales its index by four
                    if (UVs && useVertexUv && k == 1)
                        secondIdx = idx * 4;
                    out->color = D3D_VertexColor(colors, second, mask, alpha, idx, secondIdx);
                }
                else
                {
                    out->color = solidColor;
                }
                if (UVs)
                {
                    if (useVertexUv)
                        out->uvSet1 = UVs[idx];
                    else if (k == 0)
                        out->uvSet1 = UVs[tri->uv1];
                    else if (k == 1)
                        out->uvSet1 = UVs[tri->uv2];
                    else
                        out->uvSet1 = UVs[tri->uv3];
                }
                out++;
            }
        }
        D3D_FillCurrentVBO(0);
        D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, triangleNum);
        if (D3DRenderParams.textureUVMode != 5
            && D3DRenderParams.textureUVMode != 4
            && !D3DRenderParams.renderTargetBackup
            && D3D_setWaterRenderParamsIfWaterRenderingIsActive())
        {
            D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, triangleNum);
            D3D_RestoreRenderParamsAfterWaterRendering();
        }
        if (D3DRenderParams.textureUVMode == 2)
        {
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1, 2, savedArg1);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2, 3, savedArg2);
        }
        return 0;
    }
    if (verticesIN == geogro->vertices)
    {
        D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, geogro->d3dvb, 0, 12);
    }
    else
    {
        if (D3DRenderParams.currTargetGao != gpst_DX_stDD->currTargetGao)
        {
            if ((unsigned int)(D3DRenderParams.prevVertexElemIdx2 + geogro->vertexNum) <= 0xFFFF)
            {
                start = D3DRenderParams.prevVertexElemIdx2;
                D3DRenderParams.startVertexElemIdx2 = D3DRenderParams.prevVertexElemIdx2;
                D3DRenderParams.prevVertexElemIdx2 += geogro->vertexNum;
                lockFlags = 4096;
            }
            else
            {
                start = 0;
                D3DRenderParams.startVertexElemIdx2 = 0;
                D3DRenderParams.prevVertexElemIdx2 = geogro->vertexNum;
                lockFlags = 0x2000;
            }
            D3DRenderParams.currTargetGao = gpst_DX_stDD->currTargetGao;
            D3DRenderParams.d3dXYZVB->lpVtbl->Lock(
                D3DRenderParams.d3dXYZVB,
                start * D3DRenderParams.stride2,
                D3DRenderParams.stride2 * geogro->vertexNum,
                &locked,
                lockFlags);
            memcpy(locked, verticesIN, sizeof(Vec3) * geogro->vertexNum);
            D3DRenderParams.d3dXYZVB->lpVtbl->Unlock(D3DRenderParams.d3dXYZVB);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
            D3DRenderParams.d3dDev,
            0,
            D3DRenderParams.d3dXYZVB,
            D3DRenderParams.startVertexElemIdx2 * D3DRenderParams.stride2,
            D3DRenderParams.stride2);
    }
    bb = gpst_DX_stDD->bigBuffer;
    mask = bb->colorMask;
    colors = bb->vertexColorArray;
    solid = bb->solidVertexColor;
    alpha = D3DRenderParams.colorMaskAlpha;
    if (D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 != gpst_DX_stDD->currTargetGao
        || D3DRenderParams.vertexColorArray != colors
        || D3DRenderParams.colorMask != mask
        || D3DRenderParams.colorMaskAlpha2 != D3DRenderParams.colorMaskAlpha
        || D3DRenderParams.solidVertexColor != solid)
    {
        if ((unsigned int)(D3DRenderParams.prevVertexElemIdx + geogro->vertexNum) <= 0x3FFFF)
        {
            D3DRenderParams.startVertexElemIdx = D3DRenderParams.prevVertexElemIdx;
            D3DRenderParams.prevVertexElemIdx += geogro->vertexNum;
            lockFlags = 4096;
        }
        else
        {
            D3DRenderParams.startVertexElemIdx = 0;
            D3DRenderParams.prevVertexElemIdx = geogro->vertexNum;
            lockFlags = 0x2000;
        }
        D3DRenderParams.colorMaskAlpha2 = D3DRenderParams.colorMaskAlpha;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = gpst_DX_stDD->currTargetGao;
        D3DRenderParams.vertexColorArray = colors;
        D3DRenderParams.colorMask = mask;
        D3DRenderParams.solidVertexColor = solid;
        D3DRenderParams.d3dColorsVB->lpVtbl->Lock(D3DRenderParams.d3dColorsVB, D3DRenderParams.startVertexElemIdx * D3DRenderParams.stride, D3DRenderParams.stride * geogro->vertexNum, (void**)&lockedColors, lockFlags);

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

        if (colors)
        {
            for (i = 0; i < geogro->vertexNum; i++)
            {
                unsigned int packed = mask ^ (alpha | colors[i]);
                lockedColors[i] = packed & 0xFF00FF00 | ((uint8_t)packed << 16) | ((packed >> 16) & 0xff);
            }
        }
        else if (geogro->vertexNum)
        {
            for (i = 0; i < (((4 * geogro->vertexNum - 1) >> 2) + 1); i++)
            {
                lockedColors[i] = (mask ^ (alpha | gpst_DX_stDD->bigBuffer->solidVertexColor)) & 0xFF00FF00 | ((uint8_t)(mask ^ (alpha | (gpst_DX_stDD->bigBuffer->solidVertexColor & 0xff))) << 16) | (uint8_t)((mask ^ ((unsigned int)alpha | gpst_DX_stDD->bigBuffer->solidVertexColor)) >> 16);
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
        if (verticesIN == geogro->vertices)
        {
            D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, geogro->d3dvb2, 0, 12);
        }
        else
        {
            if (D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle != gpst_DX_stDD->currTargetGao)
            {
                if ((unsigned int)(D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle + geogro->vertexNum) <= 0xFFFF)
                {
                    start = D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle;
                    D3DRenderParams.currVertexStartOffset = D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle;
                    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle += geogro->vertexNum;
                    lockFlags = 4096;
                }
                else
                {
                    start = 0;
                    D3DRenderParams.currVertexStartOffset = 0;
                    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = geogro->vertexNum;
                    lockFlags = 8192;
                }
                D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = gpst_DX_stDD->currTargetGao;
                D3DRenderParams.d3dNormalsBuffer->lpVtbl->Lock(D3DRenderParams.d3dNormalsBuffer, start * D3DRenderParams.d3dStride, D3DRenderParams.d3dStride * geogro->vertexNum, &locked, lockFlags);
                memcpy(locked, geogro->normals, 12 * geogro->vertexNum);
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
    D3DRenderParams.d3dDev->lpVtbl->SetIndices(D3DRenderParams.d3dDev, geogro->d3dIndexBuf);
    D3D_DrawIndexedStreams(geogro, block);
    if (D3DRenderParams.textureUVMode != 5 && D3DRenderParams.textureUVMode != 4
        && !D3DRenderParams.renderTargetBackup
        && D3D_setWaterRenderParamsIfWaterRenderingIsActive())
    {
        D3D_DrawIndexedStreams(geogro, block);
        D3D_RestoreRenderParamsAfterWaterRendering();
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

void D3D_Texture_Unload(struct sGdi_DisplayData* dd, struct sEngPaletteElem* palette, int idx) {
}

int D3D_l_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* gdd) {
    return 0;
}

int  D3D_Set_Texture_Palette(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* inter, int idx) {
    return 0;
}

void D3D_SetTextureTarget(struct sGdi_DisplayData* dd, int target, int width, int height) {
}

int PC_HandleWindowOpenAndCloseMessages(HWND hwnd, GLOBAL_MainHandles* mainHandles, unsigned int msg)
{
    if (msg > WM_CLOSE)
    {
        if (msg == WM_MOUSEFIRST)
            SetCursor(0);
        return 0;
    }
    if (msg == WM_CLOSE || msg == WM_DESTROY)
    {
        if (!mainHandles || !mainHandles->displayData)
            return 0;
        GDI_DetachDisplay(mainHandles->displayData);
        mainHandles->displayData->DestroyDevice(mainHandles->displayData->pd3drp);
        GDI_DestroyDisplayData(mainHandles->displayData);
        return 0;
    }
    if (msg == WM_CREATE)
    {
        if (!mainHandles)
            return 0;
        mainHandles->hwnd = hwnd;
        fbss = GDI_fnpst_CreateDisplayData();
        mainHandles->displayData = fbss;
        GDI_fnl_InitInterface(mainHandles->displayData);
        mainHandles->displayData->pd3drp = (tGdi_D3DRenderParams*)mainHandles->displayData->CreateDevice();
        PRO_InitRasters(mainHandles->displayData->rasters, "Display Data");
        GDI_AttachDisplayAndReadaptDisplay(mainHandles->displayData, hwnd);
    }
    return 0;
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

int PC_WNDMsgHandler(HWND hWnd, int msg, int wParam, int lParam, int* ret)
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

void ENG_WinGameWindowInit(HINSTANCE instance)
{
    struct tagRECT rect;
    WNDCLASSA wndClass;
    HKEY videoKey;
    int regVal;
    int windowWidth;
    int windowHeight;

    wndClass.style = 0;
    wndClass.lpfnWndProc = PC_WNDProcHandler;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = instance;
    wndClass.hIcon = LoadIconA(instance, MAKEINTRESOURCEA(IDI_MAIN));
    wndClass.hCursor = NULL;
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "BGEMain";
    RegisterClassA(&wndClass);
    windowWidth = 640;
    windowHeight = 480;
    if (!RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\IRISEng\\Basic video", 0, KEY_READ, &videoKey))
    {
        if (RegQueryVal(videoKey, "Windowed", (LPBYTE)&regVal) && regVal)
            ENG_WindowedMode = 1;
        if (RegQueryVal(videoKey, "Width", (LPBYTE)&regVal) && regVal > 0)
            windowWidth = regVal;
        if (RegQueryVal(videoKey, "Height", (LPBYTE)&regVal) && regVal > 0)
            windowHeight = regVal;
        RegCloseKey(videoKey);
    }
    rect.left = 0;
    rect.top = 0;
    rect.right = windowWidth;
    rect.bottom = windowHeight;
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
        instance,
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

static unsigned int PNG_l_CrcTable[256];
static int PNG_l_CrcReady;

static void PNG_l_CrcInit(void)
{
    unsigned int c;
    int i;
    int k;

    if (PNG_l_CrcReady)
        return;
    for (i = 0; i < 256; i++)
    {
        c = (unsigned int)i;
        for (k = 0; k < 8; k++)
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        PNG_l_CrcTable[i] = c;
    }
    PNG_l_CrcReady = 1;
}

typedef struct sPngWriter
{
    FILE* file;
    unsigned int crc;
    unsigned int adlerA;
    unsigned int adlerB;
    int blockLeft;
    int rawLeft;
} tPngWriter;

static void PNG_l_Raw(tPngWriter* w, int uint8_t)
{
    fputc(uint8_t & 0xFF, w->file);
}

static void PNG_l_Chk(tPngWriter* w, int uint8_t)
{
    unsigned int b;

    b = (unsigned int)(uint8_t & 0xFF);
    fputc((int)b, w->file);
    w->crc = PNG_l_CrcTable[(w->crc ^ b) & 0xFF] ^ (w->crc >> 8);
}

static void PNG_l_Data(tPngWriter* w, int uint8_t)
{
    int n;

    if (w->blockLeft == 0)
    {
        n = w->rawLeft < 65535 ? w->rawLeft : 65535;
        PNG_l_Chk(w, w->rawLeft <= 65535 ? 1 : 0);
        PNG_l_Chk(w, n);
        PNG_l_Chk(w, n >> 8);
        PNG_l_Chk(w, ~n);
        PNG_l_Chk(w, ~n >> 8);
        w->blockLeft = n;
    }
    PNG_l_Chk(w, uint8_t);
    w->adlerA = (w->adlerA + (unsigned int)(uint8_t & 0xFF)) % 65521u;
    w->adlerB = (w->adlerB + w->adlerA) % 65521u;
    w->blockLeft--;
    w->rawLeft--;
}

static void PNG_l_BeginChunk(tPngWriter* w, int length, const char* type)
{
    PNG_l_Raw(w, length >> 24);
    PNG_l_Raw(w, length >> 16);
    PNG_l_Raw(w, length >> 8);
    PNG_l_Raw(w, length);
    w->crc = 0xFFFFFFFFu;
    PNG_l_Chk(w, type[0]);
    PNG_l_Chk(w, type[1]);
    PNG_l_Chk(w, type[2]);
    PNG_l_Chk(w, type[3]);
}

static void PNG_l_EndChunk(tPngWriter* w)
{
    unsigned int c;

    c = ~w->crc;
    PNG_l_Raw(w, (int)(c >> 24));
    PNG_l_Raw(w, (int)(c >> 16));
    PNG_l_Raw(w, (int)(c >> 8));
    PNG_l_Raw(w, (int)c);
}

static int PNG_WriteBgra(const char* path, const unsigned char* pixels,
                         int pitch, int width, int height)
{
    static const unsigned char signature[8] =
        { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    tPngWriter w;
    const unsigned char* row;
    unsigned int adler;
    int blocks;
    int raw;
    int i;
    int x;
    int y;

    PNG_l_CrcInit();
    memset(&w, 0, sizeof(w));
    w.file = fopen(path, "wb");
    if (!w.file)
        return 0;
    w.adlerA = 1;
    w.adlerB = 0;

    for (i = 0; i < 8; i++)
        PNG_l_Raw(&w, signature[i]);

    PNG_l_BeginChunk(&w, 13, "IHDR");
    PNG_l_Chk(&w, width >> 24);
    PNG_l_Chk(&w, width >> 16);
    PNG_l_Chk(&w, width >> 8);
    PNG_l_Chk(&w, width);
    PNG_l_Chk(&w, height >> 24);
    PNG_l_Chk(&w, height >> 16);
    PNG_l_Chk(&w, height >> 8);
    PNG_l_Chk(&w, height);
    PNG_l_Chk(&w, 8);
    PNG_l_Chk(&w, 2);
    PNG_l_Chk(&w, 0);
    PNG_l_Chk(&w, 0);
    PNG_l_Chk(&w, 0);
    PNG_l_EndChunk(&w);

    raw = height * (1 + width * 3);
    blocks = (raw + 65534) / 65535;
    if (blocks < 1)
        blocks = 1;
    w.rawLeft = raw;
    w.blockLeft = 0;

    PNG_l_BeginChunk(&w, 2 + blocks * 5 + raw + 4, "IDAT");
    PNG_l_Chk(&w, 0x78);
    PNG_l_Chk(&w, 0x01);
    for (y = 0; y < height; y++)
    {
        row = pixels + (size_t)y * (size_t)pitch;
        PNG_l_Data(&w, 0);
        for (x = 0; x < width; x++)
        {
            PNG_l_Data(&w, row[x * 4 + 2]);
            PNG_l_Data(&w, row[x * 4 + 1]);
            PNG_l_Data(&w, row[x * 4 + 0]);
        }
    }
    adler = (w.adlerB << 16) | w.adlerA;
    PNG_l_Chk(&w, (int)(adler >> 24));
    PNG_l_Chk(&w, (int)(adler >> 16));
    PNG_l_Chk(&w, (int)(adler >> 8));
    PNG_l_Chk(&w, (int)adler);
    PNG_l_EndChunk(&w);

    PNG_l_BeginChunk(&w, 0, "IEND");
    PNG_l_EndChunk(&w);

    fclose(w.file);
    return 1;
}

void D3D_SaveScreenshot(void)
{
    IDirect3DSurface9* target;
    IDirect3DSurface9* copy;
    D3DSURFACE_DESC desc;
    D3DLOCKED_RECT locked;
    SYSTEMTIME now;
    CHAR path[MAX_PATH];

    if (!D3DRenderParams.d3dDev)
        return;
    if (D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 0;
    }
    target = NULL;
    if (D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &target) < 0 || !target)
        return;
    target->lpVtbl->GetDesc(target, &desc);
    if (desc.Format != D3DFMT_A8R8G8B8 && desc.Format != D3DFMT_X8R8G8B8)
    {
        D3D_OutputDebugString("screenshot: back buffer format %d is not 32 bit\n", (int)desc.Format);
        target->lpVtbl->Release(target);
        return;
    }
    copy = NULL;
    if (D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(D3DRenderParams.d3dDev,
            desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &copy, 0) < 0 || !copy)
    {
        target->lpVtbl->Release(target);
        return;
    }
    if (D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev, target, copy) < 0
        || copy->lpVtbl->LockRect(copy, &locked, NULL, D3DLOCK_READONLY) < 0)
    {
        D3D_OutputDebugString("screenshot: could not read the back buffer\n");
        copy->lpVtbl->Release(copy);
        target->lpVtbl->Release(target);
        return;
    }
    CreateDirectoryA("screenshots", NULL);
    GetLocalTime(&now);
    sprintf(path, "screenshots\\%04d%02d%02d_%02d%02d%02d_%03d.png",
            now.wYear, now.wMonth, now.wDay,
            now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);
    if (PNG_WriteBgra(path, (const unsigned char*)locked.pBits, locked.Pitch,
                      (int)desc.Width, (int)desc.Height))
        D3D_OutputDebugString("screenshot: %s\n", path);
    else
        D3D_OutputDebugString("screenshot: could not write %s\n", path);
    copy->lpVtbl->UnlockRect(copy);
    copy->lpVtbl->Release(copy);
    target->lpVtbl->Release(target);
}

void PC_EngineCheatFinal()
{
    tableElemStructPF* currWorld;
    profilerRasterStructElem* currRaster;

    if (GetAsyncKeyState(VK_F7) < 0)
    {
        D3D_SaveScreenshot();
        while (GetAsyncKeyState(VK_F7) < 0);
    }
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