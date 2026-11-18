/* The material's texture-coordinate projection.
 *
 * Sixteen UV modes, thirteen of which have always been here in gdi.c;
 * these three were in the Direct3D renderer because they are the ones
 * that need the device told about the projection as well as the
 * engine. That made the engine's draw path depend on that renderer --
 * materialanddraw.c calls ComputeAndSetTransform straight, where
 * everything around it goes through the renderer's own table -- and
 * it is not a dependency that was ever meant: the OpenGL renderer
 * reads GDI_gi_TextureUVMode and the transform matrix for itself and
 * says so in its own comments.
 *
 * So what is shared is here, and the three device calls go through
 * the renderer like everything else. A renderer that reads the
 * engine's state directly fills them with nothing.
 *
 * The arithmetic is unchanged, including one site that had the
 * transform-flag cache written out by hand where the rest call the
 * helper for it; the two are the same test and the same store. */

#include "common.h"
#include "gdi.h"
#include "math.h"
#include "profiler.h"
#include "graphicsobjects/geo.h"
#include "materialanddraw.h"
#include "matrixmath.h"

/* The matrix a texture-coordinate projection ends up as, laid out from
   the axes, the position and the scale it was computed from. Nothing
   in it is a renderer's business: it sat in the Direct3D renderer's
   file and every one of its callers is here. */
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


void MAT_UV_ComputeAndSetTransform(tGEOGRO* geogro, tGeoTriangleChunks* chunks, tMultiTextureElemStruct* texElem)
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
    GDI_gi_TextureUVMode = 0;
    gpst_DX_stDD->SetTexCoordIndex(0, GDI_UVSRC_STORED);
    gpst_DX_stDD->SetTextureTransformFlags(0, GDI_UVXF_NONE);
    gpst_DX_stDD->flags &= ~0x10u;
    MAT_FuncTable[uvFunc](gpst_DX_stDD, geogro, chunks, planarParam);
    if (!gpst_DX_stDD->bigBuffer->UVData)
    {
        if (GDI_gi_TextureUVMode)
        {
            MATH_MakeOGLMatrix(&oglMtx, &GLOBAL_UVTransformMatrix);
            memcpy(d3dTransformationMatrix, &oglMtx, sizeof(Mtx4x4WithoutAdditionalData));
            /* The matrix above is the engine's, and something outside
               this renderer reads it, so it is computed whether or not
               there is a device to hand it to. Only the handing over
               is guarded. */
            gpst_DX_stDD->SetTexCoordMatrix(0, &oglMtx);
        }
        // preserved: this return skips the matching end block, so raster 38 stays entered
        return;
    }
    transform1 = texElem->transform1;
    if (transform1 == 0x3F800000 && texElem->transform2 == 0x3F800000 || !transform1 || !texElem->transform2) // TODO: MAT_VUIsIdentity inline
    {
        if (!GDI_gi_TextureUVMode)
        {
            GDI_gi_TextureUVMode = 0;
            gpst_DX_stDD->SetTexCoordIndex(0, GDI_UVSRC_STORED);
            gpst_DX_stDD->SetTextureTransformFlags(0, GDI_UVXF_NONE);
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
        if (GDI_gi_TextureUVMode)
        {
            MATH_MulMatrixMatrix(&oglMtx, &GLOBAL_UVTransformMatrix, &uvMtx);
            MATH_MakeOGLMatrix(&oglMtx, &oglMtx);
        }
        else
        {
            GDI_gi_TextureUVMode = 3;
            gpst_DX_stDD->SetTexCoordIndex(0, GDI_UVSRC_STORED);
            gpst_DX_stDD->SetTextureTransformFlags(0, GDI_UVXF_2D);
            MATH_MakeOGLMatrix(&oglMtx, &uvMtx);
            oglMtx.v3.x = oglMtx.Pos.x;
            oglMtx.v3.y = oglMtx.Pos.y;
        }
    }
    memcpy(d3dTransformationMatrix, &oglMtx, sizeof(Mtx4x4WithoutAdditionalData));
    /* As above: the matrix is the engine's and is computed either
       way; only handing it to a device is guarded. */
    gpst_DX_stDD->SetTexCoordMatrix(0, &oglMtx);
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

    GDI_gi_TextureUVMode = 2;
    gpst_DX_stDD->SetTexCoordIndex(0, GDI_UVSRC_NORMAL);
    gpst_DX_stDD->SetTextureTransformFlags(0, GDI_UVXF_2D);
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
    GDI_gi_TextureUVMode = 1;
    gpst_DX_stDD->SetTexCoordIndex(0, GDI_UVSRC_POSITION);
    gpst_DX_stDD->SetTextureTransformFlags(0, GDI_UVXF_2D);
    viewMtx = dd->ct.cam.viewMtxPtr_setInGRORender;
    if ((viewMtx->mtxflag & 8) != 0)
    {
        MATH_MakeOGLMatrix(&viewMtxStd, viewMtx);
        viewMtxStdInv.mtxflag = 0x4;
        MTX_p_Inverse((Mtx4x4WithoutAdditionalData*)&viewMtxStdInv, NULL,
                            (const Mtx4x4WithoutAdditionalData*)&viewMtxStd);
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
