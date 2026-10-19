#include "profiler.h"
#include "gdi.h"
//#include "graphicsobjects/geo.h"
//#include "graphicsobjects/geomaterial.h"
#include "time.h"
#include "textures.h"
#include "gdi.h"
#include "win/wingdi.h"
#include "soft.h"
#include "dbg.h"
#include "materialanddraw.h"

BOOL MAT_IsMaterialTransparent(struct sGEOMAT* geomat, int idx, int renderingFlags, void* unk1)
{
    char mask;

    if (!geomat)
        return FALSE;

    if (geomat->intf->id == 4)
    {
        mask = geomat->intf->GetValidatedMask(geomat, idx);
        if ((mask & 0x40) != 0)
            return TRUE;
        if ((mask & 0x80) == 0)
            return FALSE;
    }
    else
    {
        mask = geomat->intf->GetValidatedMask(geomat, idx);
    }
    return (mask & 0xc0) != 0;
}

void MAT_SetRenderState(tGEOMAT* a1, int a2)
{
    tGEOMAT* currMaterial;
    int v3;
    int v4;
    tMultiTextureElemStruct* v5;
    tGdi_DisplayData* v6;
    tMultiTextureElemStruct* v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;

    currMaterial = a1;
    gpst_DX_stDD->bigBuffer->colorMask = 0;
    if (!a1)
        currMaterial = &MAT_gst_DefaultSingleMaterial;
    if (currMaterial->intf->id == 4)
    {
        v3 = a2;
        if (a2 > currMaterial->multiMaterialNumIfType4ThenAmbiantColor - 1)
            v3 = currMaterial->multiMaterialNumIfType4ThenAmbiantColor - 1;
        currMaterial = currMaterial->multiMaterialPtrIfType4ThenDiffuseColor[v3];
        if (!currMaterial)
            currMaterial = &MAT_gst_DefaultSingleMaterial;
    }
    v4 = currMaterial->intf->id;
    if (v4 == 5) // MTM
    {
        v5 = (tMultiTextureElemStruct*)currMaterial->idxOrPtr;
        v6 = gpst_DX_stDD;
        if (v5 && (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 1) != 0)
        {
            if ((v5->subTextureFlags & 0x400) != 0)
            {
                gpst_DX_stDD->bigBuffer->colorMask |= (v5->phongOrPlanarParam & 0xFF00) << 16;
                gpst_DX_stDD->bigBuffer->colorMask ^= 0xFF000000;
                v6 = gpst_DX_stDD;
            }
            v7 = (tMultiTextureElemStruct*)currMaterial->idxOrPtr;
            v12 = (uint16_t)v7->phongOrPlanarParam;
            v8 = v7->subTextureFlags;
            v9 = v7->texElemIdx;
            v11 = v8;
            goto LABEL_14;
        }
        v11 = 39;
    LABEL_16:
        v6->TextureInit(-1, v11, 0);
        gpst_DX_stDD->currGEOMAT = currMaterial;
        return;
    }
    if (v4 == 3) // ST
    {
        v10 = 65575;
        if (currMaterial->unkFloat2 > 0.98000002)
            v10 = 39;
        v6 = gpst_DX_stDD;
        v12 = 0;
        v11 = v10;
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 1) != 0)
        {
            v9 = (int)(currMaterial->idxOrPtr);
        LABEL_14:
            v6->TextureInit(v9, v11, v12);
            gpst_DX_stDD->currGEOMAT = currMaterial;
            return;
        }
        goto LABEL_16;
    }
    gpst_DX_stDD->currGEOMAT = currMaterial;
}

void MAT_DrawIndexedTriangle_ST(tGdi_DisplayData* a1, tGEOGRO* a2, tGEOMAT* geomat, tGeoTriangleChunks* a4)
{
    profilerRasterStructElem* v4;
    profilerRasterStructElem* v5;
    tGeoTriangleChunks* v6;
    profilerRasterStructElem* v7;
    __int16 v8;
    __int16 v9;
    tGEOGRO* v10;
    tBigBufferStruct* v11;
    tGeoUVElem* v12;
    unsigned __int16* v13;
    unsigned __int16* v14;
    int v15;
    tGeoUVElem* v16;
    float v17;
    profilerRasterStructElem* v18;
    profilerRasterStructElem* v19;
    __int16 v20;
    __int16 v21;
    profilerRasterStructElem* v22;
    __int16 v23;
    __int16 v24;

    v4 = &a1->rasters[29];
    if (!a1->rasters[29].insideCounter)
        v4->startTick = TIM_GetTicksAsInt32();
    ++v4->insideCounter;
    a1->bigBuffer->vertexColorArray = (int*)a1->bigBuffer->data3FinalColorWithShadow;
    v5 = &a1->rasters[30];
    a1->currGEOMAT = geomat;
    if (!v5->insideCounter)
        v5->startTick = TIM_GetTicksAsInt32();
    ++v5->insideCounter;
    v6 = a4;
    MAT_SetRenderState(geomat, a4->materialNum);
    v7 = &a1->rasters[30];
    v8 = a1->rasters[30].insideCounter;
    if (v8)
    {
        v9 = v8 - 1;
        a1->rasters[30].insideCounter = v9;
        if (!v9)
            v7->sumTime += TIM_GetTicksAsInt32() - v7->startTick;
    }
    v10 = a2;
    v11 = a1->bigBuffer;
    a1->flags &= ~0x12u;
    v11->UVData = (byte*)a2->UVs;
    if (a2->intf->id == 1)
    {
        if (a2->mrm)
        {
            a1->flags |= 2u;
            v12 = &a2->UVs[a4->MRMUVIndexElems->UVStartOffset];
            a1->bigBuffer->UVData = a1->bigBuffer->data7;
            v13 = a4->MRMUVIndices;
            v14 = &v13[a4->MRMUVIndicesNum];
            if (v13 < v14)
            {
                do
                {
                    v15 = *v13;
                    v16 = (tGeoUVElem*)a1->bigBuffer->UVData;
                    v16[v15].u = v12->u;
                    v17 = v12->v;
                    ++v13;
                    ++v12;
                    v16[v15].v = v17;
                } while (v13 < v14);
                v10 = a2;
                v6 = a4;
            }
        }
    }
    v18 = &a1->rasters[31];
    if (!a1->rasters[31].insideCounter)
        v18->startTick = TIM_GetTicksAsInt32();
    ++v18->insideCounter;
    gpst_DX_stDD->DrawElementIndexedTriangles(v6, a1->verticesCurrent, (tGeoUVElem*)(a1->bigBuffer->UVData), v10->vertexNum);
    v19 = &a1->rasters[31];
    v20 = a1->rasters[31].insideCounter;
    if (v20)
    {
        v21 = v20 - 1;
        a1->rasters[31].insideCounter = v21;
        if (!v21)
            v19->sumTime += TIM_GetTicksAsInt32() - v19->startTick;
    }
    v22 = &a1->rasters[29];
    v23 = a1->rasters[29].insideCounter;
    if (v23)
    {
        v24 = v23 - 1;
        a1->rasters[29].insideCounter = v24;
        if (!v24)
            v22->sumTime += TIM_GetTicksAsInt32() - v22->startTick;
    }
}

int MAT_IsObjectTransparent(tGEOMAT* a1)
{
    tMultiTextureElemStruct* v1;
    int result;

    v1 = (tMultiTextureElemStruct*)a1->idxOrPtr;
    result = 0;
    while ((v1->subTextureFlags & 0xA00) != 0)  // 0x200 - zwriteneable
                                                  // 0x800 - transparent
    {
        v1 = v1->nextTextureKey;
        if (!v1 || v1 == (tMultiTextureElemStruct*)-1)
            return result;
    }
    return 1;
}

BOOL MAT_MaterialsHaveAlphaBlendingType3And4(tGEOMAT* geomat)
{
    tMultiTextureElemStruct* currTexElem;
    BOOL ret = FALSE;
    int type;

    currTexElem = (tMultiTextureElemStruct*)geomat->idxOrPtr;
    while (1)
    {
        if ((currTexElem->subTextureFlags & 0x800) == 0)
        {
            type = (currTexElem->subTextureFlags >> 16) & 0xf;
            if (type == 3 || type == 4)
                break;
        }
        currTexElem = currTexElem->nextTextureKey;
        if (!currTexElem || currTexElem == (tMultiTextureElemStruct*)-1)
            return ret;
    }
    return TRUE;
}

BOOL MAT_IsLastElemInMultiTexture(tGdi_DisplayData* dxdd, tMultiTextureElemStruct* tmes)
{
    tMultiTextureElemStruct* nextTmes;

    if ((dxdd->flags & 0x8000) != 0 || (nextTmes = tmes->nextTextureKey) == NULL || nextTmes == (tMultiTextureElemStruct*)-1)
        return TRUE;

    return FALSE;
}

void MAT_DrawIndexedTriangle_MT(tGdi_DisplayData* dxdd, tGEOGRO* geogro, tGEOMAT* gmat, tGeoTriangleChunks* triangleChunks)
{
    tBigBufferStruct* bigBuff;
    tMultiTextureElemStruct* v6;
    unsigned int v7;
    unsigned int v8;
    int v9;
    int transparentObject;
    int materialsHaveAlphaBlending;

    bigBuff = dxdd->bigBuffer;
    dxdd->currGEOMAT = gmat;
    v6 = (tMultiTextureElemStruct*)gmat->idxOrPtr;
    if (v6 && (v9 = dxdd->drawMaskAllowedStuffCurrent, (v9 & 1) != 0))
    {
        transparentObject = MAT_IsObjectTransparent(gmat);
        materialsHaveAlphaBlending = MAT_MaterialsHaveAlphaBlendingType3And4(gmat);
        dxdd->drawMaskAllowedStuffCurrent = v9 | 0x200;
        do
        {
            if (v6 == (tMultiTextureElemStruct*)-1)
                break;
            if (v6->texElemIdx == -1 || (TEX_gst_GlobalList.texElems[v6->texElemIdx].texElemFlag & 0x20) == 0)
            {
                if ((v6->subTextureFlags & 0x800) == 0)
                {
                    bigBuff->colorMask = 0;
                    bigBuff->UVData = 0;
                    bigBuff->vertexColorArray = 0;
                    bigBuff->solidVertexColor = 0;
                    bigBuff->vertexColorArraySecond = 0;
                    switch ((uint8_t)(((v6->subTextureFlags) & 0xffff) >> 8) >> 4) // elõször 16-tal letolja majd 8-cal vissza (HIBYTE(LOWORD(v6->subTextureFlags)))
                    {
                    case 0:
                        goto LABEL_16;
                    case 1:
                        bigBuff->vertexColorArray = (int*)bigBuff->data2SpecularColors;
                        if ((gpst_DX_stDD->bigBuffer->flags & 4) == 0)
                        {
                            bigBuff->solidVertexColor = 0;
                            bigBuff->vertexColorArray = 0;
                        }
                        break;
                    case 2:
                        bigBuff->solidVertexColor = gmat->solidVertexColor & 0xFFFFFF;
                        break;
                    case 3:
                        goto LABEL_13;
                    case 4:
                        bigBuff->solidVertexColor = 0xFFFFFF;
                        bigBuff->vertexColorArray = 0;
                        break;
                    case 5:
                        bigBuff->colorMask = 0xFFFFFF;
                    LABEL_13:
                        bigBuff->vertexColorArray = dxdd->RLIPtr;
                        break;
                    case 6:
                        D3DRenderParams.diffuse2x = 1;
                    LABEL_16:
                        bigBuff->vertexColorArray = (int*)bigBuff->data3FinalColorWithShadow;// finalcolor shadow-val
                        break;
                    default:
                        break;
                    }
                    if ((v6->subTextureFlags & 0x400) != 0)
                        bigBuff->colorMask = (bigBuff->colorMask | ((v6->phongOrPlanarParam & 0xFF00) << 16)) ^ 0xFF000000;
                    v7 = v6->subTextureFlags;
                    if (!materialsHaveAlphaBlending)
                        v7 |= 0x20u;
                    dxdd->TextureInit(v6->texElemIdx, v7, 0);
                    MAT_ComputeUV_sub_4521A0(geogro, triangleChunks, v6);
                    gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
                    dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
                    if (D3DRenderParams.diffuse2x && !D3DRenderParams.tweak_ae_d2x)
                    {
                        v8 = v6->subTextureFlags & ~0xA0000u | 0x50000;
                        if (!materialsHaveAlphaBlending)
                            v8 = v6->subTextureFlags & ~0xA0000u | 0x50020;
                        dxdd->TextureInit(v6->texElemIdx, v8, 0);
                        gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
                        dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
                    }
                    D3DRenderParams.diffuse2x = 0;
                }
                if (D3DRenderParams.isDepthBlurOn
                    && !D3DRenderParams.tweak_zonalpha
                    && MAT_IsLastElemInMultiTexture(dxdd, v6)
                    && (materialsHaveAlphaBlending || transparentObject))
                {
                    dxdd->TextureInit(-1, 0, 1);
                    gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
                    dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
                }
            }
            v6 = v6->nextTextureKey;
        } while (v6);
    }
    else
    {
        bigBuff->colorMask = 0;
        bigBuff->UVData = 0;
        bigBuff->solidVertexColor = 0;
        bigBuff->vertexColorArraySecond = 0;
        bigBuff->vertexColorArray = (int*)bigBuff->data3FinalColorWithShadow;
        dxdd->currGEOMAT = gmat;
        dxdd->TextureInit(-1, 0, 1);
        gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
        dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
    }
    dxdd->flags &= ~0x12u;
    bigBuff->UVData = 0;
    dxdd->TextureInit(-1, 39, 0);
}

void MAT_DrawIndexedTriangle(struct sGEOGRO* geogro, struct sGEOMAT* geomat, struct sGeoTriangleChunks* triangleChunks)
{
    profilerRasterStructElem* v3;
    tGEOMAT* ggeomat;
    int v5;
    int v6;
    profilerRasterStructElem* v7;
    __int16 v8;
    __int16 v9;
    int v10; 
    int v11; 

    /*
    if (gpst_DX_stDD->currTargetGao == (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x620031d4)) {
        DBG_OutputDebugString("rliptr(draw): %p\n", gpst_DX_stDD->RLIPtr);
        //OutputDebugStringA("itt\n");
        //((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->RLIVertexColors
        int iii;
        DBG_OutputDebugString("rli: ");
        for (iii = 0; iii < geogro->vertexNum; iii++) {
            DBG_OutputDebugString("%x ", ((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->RLIVertexColors[iii]);
        }
        DBG_OutputDebugString("\n\n");
    }
    */

    if (!triangleChunks->triangleNum)
        return;
    QueryPerformanceCounter(&PerfCounterStruct[12]);
    v3 = &gpst_DX_stDD->rasters[28];
    if (!gpst_DX_stDD->rasters[28].insideCounter)
        v3->startTick = TIM_GetTicksAsInt32();
    ggeomat = geomat;
    ++v3->insideCounter;
    if (!geomat)
        goto LABEL_11;
    if (geomat->intf->id == 4)
    {
        v5 = geomat->multiMaterialNumIfType4ThenAmbiantColor;
        if (!v5)
        {
        LABEL_11:
            ggeomat = &MAT_gst_DefaultSingleMaterial;
            goto LABEL_12;
        }
        v6 = v5 - 1;
        if (triangleChunks->materialNum <= v6)
            v6 = triangleChunks->materialNum;
        ggeomat = geomat->multiMaterialPtrIfType4ThenDiffuseColor[v6];
    }
    if (!ggeomat)
        goto LABEL_11;
LABEL_12:
    v11 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    if ((gpst_DX_stDD->flags & 4) != 0)
    {
        if (!MAT_IsMaterialTransparent(ggeomat, -1, v11, 0))
            goto LABEL_14;
    }
    else if (MAT_IsMaterialTransparent(ggeomat, -1, v11, 0))
    {
    LABEL_14:
        PRO_RecordTimeDecRef(&gpst_DX_stDD->rasters[28]);
        PRO_QPC_RecordTimeDecRef(12);
        return;
    }
    v7 = &gpst_DX_stDD->rasters[28];
    v8 = gpst_DX_stDD->rasters[28].insideCounter;
    if (v8)
    {
        v9 = v8 - 1;
        gpst_DX_stDD->rasters[28].insideCounter = v9;
        if (!v9)
            v7->sumTime += TIM_GetTicksAsInt32() - v7->startTick;
    }
    v10 = ggeomat->intf->id;
    if (v10 == 5)
    {
        MAT_DrawIndexedTriangle_MT(gpst_DX_stDD, geogro, ggeomat, triangleChunks);
    }
    else if (v10 == 3)
    {
        gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
        MAT_DrawIndexedTriangle_ST(gpst_DX_stDD, geogro, ggeomat, triangleChunks);
    }
    PRO_QPC_RecordTimeDecRef(12);
    gpst_DX_stDD->bigBuffer->colorMask = 0;
}

void GAO_XMN_Render_TransparentTriangles(struct sGEOGRO* geogro, struct sGEOMAT* geomat)
{
    tGEOGRO* v2;
    tGdi_DisplayData* v3;
    tGeoTriangleChunks* v4;
    tGeoTriangleChunks* v5;
    tGeoTriangleChunkElem* v6;
    Vec3* v7;
    Vec3* v8;
    double v9;
    BOOL v10;
    Vec3* v11;
    Vec3* v12;
    tZListElem* v13;
    tGeoUVElem* v14;
    tGeoTriangleChunks* v15;
    Vec3* v16;
    float v17;
    tGeoTriangleChunkElem* v18;
    tGeoTriangleChunks* v19;
    int v20;
    int tmp;

    v2 = geogro;
    v3 = gpst_DX_stDD;
    v4 = geogro->triangleChunks;
    v5 = &v4[geogro->triangleChunkCount];
    v15 = v4;
    v19 = v5;
    v20 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    if (v4 < v5)
    {
        while (1)
        {
            SOFT_TransformInComputingBuffer(v3->bigBuffer, v3->verticesCurrent, v2->vertexNum, &v3->ct);
            v6 = v4->triangleChunkElems;
            v7 = (Vec3*)gpst_DX_stDD->bigBuffer->data6;
            v16 = v7;
            v18 = &v6[v4->triangleNum];
            if (v6 < v18)
            {
                while (1)
                {
                    v8 = &v7[v6->v1];
                    v9 = v8->z;
                    v10 = v9 < v7[v6->v2].z;
                    v11 = &v7[v6->v2];
                    v12 = &v7[v6->v3];
                    if (v10)
                        v9 = v11->z;
                    if (v9 < v12->z)
                        v9 = v12->z;
                    v17 = (float)v9;
                    tmp = MATH_gl_TableInverse[((*((int32_t*)&v17)) >> 13) & 0x3FF] - ((*((uint32_t*)&v17)) & 0xFF800000) + 0x7E800000;
                    v13 = SOFT_pst_ZList_AddNode(*(float *)&tmp, 3, 0);
                    if (v13)
                    {
                        ((tZListMeshElem*)(v13->data))[0].point = *v8;
                        ((tZListMeshElem*)(v13->data))[1].point = *v11;
                        ((tZListMeshElem*)(v13->data))[2].point = *v12;
                        v14 = geogro->UVs;
                        if (v14)
                        {
                            ((tZListMeshElem*)(v13->data))[0].uv.u = v14[v6->uv1].u;
                            ((tZListMeshElem*)(v13->data))[0].uv.v = geogro->UVs[v6->uv1].v;
                            ((tZListMeshElem*)(v13->data))[1].uv = geogro->UVs[v6->uv2];
                            ((tZListMeshElem*)(v13->data))[2].uv = geogro->UVs[v6->uv3];
                        }
                        ((tZListMeshElem*)(v13->data))[0].vertexColor = 0xFFFFFF;
                        ((tZListMeshElem*)(v13->data))[1].vertexColor = 0xFFFFFF;
                        ((tZListMeshElem*)(v13->data))[2].vertexColor = 0xFFFFFF;
                        v13->geomat = geomat;
                        v13->renderingFlags = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
                    }
                    if (++v6 >= v18)
                        break;
                    v7 = v16;
                }
                v5 = v19;
                v4 = v15;
            }
            ++v4;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent = v20;
            v15 = v4;
            if (v4 >= v5)
                break;
            v3 = gpst_DX_stDD;
            v2 = geogro;
        }
    }
}

void GEO_Render_TransparentTriangles(struct sGdi_DisplayData* a1, struct sGEOGRO* a2, struct sGameObjectVisualObject* a3)
{
    int v3;
    tGdi_DisplayData* v4;
    tGeoTriangleChunks* v5;
    tGeoTriangleChunks* v6;
    tGEOMAT* v7;
    tGeoTriangleChunkElem* v8;
    Vec3* v9;
    double v10;
    Vec3* v11;
    Vec3* v12;
    Vec3* v13;
    tZListElem* v14;
    tZListMeshElem* v15;
    tGeoUVElem* v16;
    float v17;
    float v18;
    Vec3* v19;
    tGeoTriangleChunks* v20;
    float v21;
    tGEOMAT* v22;
    tGeoTriangleChunkElem* v23;
    tGeoTriangleChunks* v24;
    int v25;
    int tmp;

    v3 = a2->vertexNum;
    if (v3 <= 5000)
    {
        v4 = a1;
        v5 = a2->triangleChunks;
        v25 = a1->drawMaskAllowedStuffCurrent;
        v6 = &v5[a2->triangleChunkCount];
        v20 = v5;
        v24 = v6;
        SOFT_TransformInComputingBuffer(a1->bigBuffer, a1->verticesCurrent, v3, &a1->ct);
        for (; v5 < v6; v20 = v5)
        {
            v4->rasters[9].unk2 += v5->triangleNum;
            v7 = GEO_GetMaterial((tGEOMAT*)a3->GROMAT, v5->materialNum);
            v8 = v5->triangleChunkElems;
            v22 = v7;
            v9 = (Vec3*)v4->bigBuffer->data6;
            v19 = v9;
            v23 = &v8[v5->triangleNum];
            if (v8 < v23)
            {
                do
                {
                    v10 = v9[v8->v1].z;
                    v11 = &v9[v8->v2];
                    v12 = &v9[v8->v1];
                    v17 = v11->z;
                    v13 = &v9[v8->v3];
                    if (v10 < v17)
                        v10 = v17;
                    v18 = v13->z;
                    if (v10 < v18)
                        v10 = v18;
                    if (v10 > 0.0)
                    {
                        v21 = (float)v10;
                        tmp = MATH_gl_TableInverse[((*((int32_t*)(&v21))) >> 13) & 0x3FF] - ((*((uint32_t*)(&v21))) & 0xFF800000) + 0x7E800000;
                        v14 = SOFT_pst_ZList_AddNode((*((float*)(&tmp))), 3, 0);
                        if (v14)
                        {
                            v15 = v14->data;
                            v15->point.x = v12->x;
                            v15->point.y = v12->y;
                            v15->point.z = v12->z;
                            ((tZListMeshElem*)(v14->data))[1].point = *v11;
                            ((tZListMeshElem*)(v14->data))[2].point = *v13;
                            v16 = a2->UVs;
                            if (v16)
                            {
                                ((tZListMeshElem*)(v14->data))[0].uv.u = v16[v8->uv1].u;
                                ((tZListMeshElem*)(v14->data))[0].uv.v = a2->UVs[v8->uv1].v;
                                ((tZListMeshElem*)(v14->data))[1].uv = a2->UVs[v8->uv2];
                                ((tZListMeshElem*)(v14->data))[2].uv = a2->UVs[v8->uv3];
                            }
                            ((tZListMeshElem*)(v14->data))[0].vertexColor = *(uint32_t*)&a1->bigBuffer->data3FinalColorWithShadow[4 * v8->v1];
                            ((tZListMeshElem*)(v14->data))[1].vertexColor = *(uint32_t*)&a1->bigBuffer->data3FinalColorWithShadow[4 * v8->v2];
                            ((tZListMeshElem*)(v14->data))[2].vertexColor = *(uint32_t*)&a1->bigBuffer->data3FinalColorWithShadow[4 * v8->v3];
                            v14->geomat = v22;
                            v14->renderingFlags = a1->drawMaskAllowedStuffCurrent;
                        }
                        v9 = v19;
                    }
                    ++v8;
                } while (v8 < v23);
                v6 = v24;
                v5 = v20;
                v4 = a1;
            }
            ++v5;
            v4->drawMaskAllowedStuffCurrent = v25;
        }
    }
}

void MAT_DrawIndexedSprites_MT(tGdi_DisplayData* dxdd, tGEOGRO* a2, tGEOMAT* a1, tGeoSprites* a4)
{
    tMultiTextureElemStruct* v4;
    unsigned int v12;
    int isTransparent;
    int materialsHaveAlphaBelnding;

    v4 = (tMultiTextureElemStruct*)a1->idxOrPtr;
    if (v4)
    {
        isTransparent = MAT_IsObjectTransparent(a1);
        materialsHaveAlphaBelnding = MAT_MaterialsHaveAlphaBlendingType3And4(a1);
        dxdd->bigBuffer->vertexColorArray = (int*)dxdd->bigBuffer->data3FinalColorWithShadow;
        dxdd->currGEOMAT = a1;
        dxdd->drawMaskAllowedStuffCurrent |= 0x200;
        PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[32]);
        PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[32]);
        dxdd->bigBuffer->solidVertexColor = a1->solidVertexColor;
        do
        {
            if (v4 == (tMultiTextureElemStruct*)-1)
                break;
            dxdd->bigBuffer->vertexColorArraySecond = NULL;
            if ((v4->subTextureFlags & 0x800) == 0)
            {
                dxdd->bigBuffer->colorMask = 0;
                switch ((uint8_t)(((v4->subTextureFlags & 0xffff) >> 8) & 0xff) >> 4) // elõször 16-tal letolja majd 8-cal vissza (HIBYTE(LOWORD(v4->subTextureFlags)))
                {
                case 0:
                case 6:
                    dxdd->bigBuffer->vertexColorArray = (int*)dxdd->bigBuffer->data3FinalColorWithShadow;
                    break;
                case 1:
                    dxdd->bigBuffer->vertexColorArray = (int*)dxdd->bigBuffer->data2SpecularColors;
                    if ((gpst_DX_stDD->bigBuffer->flags & 4) == 0)
                    {
                        dxdd->bigBuffer->solidVertexColor = 0;
                        dxdd->bigBuffer->vertexColorArray = NULL;
                    }
                    break;
                case 2:
                    dxdd->bigBuffer->solidVertexColor = a1->solidVertexColor & 0xFFFFFF;
                    dxdd->bigBuffer->vertexColorArray = NULL;
                    break;
                case 3:
                    dxdd->bigBuffer->vertexColorArray = dxdd->RLIPtr;
                    break;
                case 4:
                    dxdd->bigBuffer->solidVertexColor = 0xFFFFFF;
                    dxdd->bigBuffer->vertexColorArray = NULL;
                    break;
                case 5:
                    dxdd->bigBuffer->colorMask = 0xFFFFFF;
                    dxdd->bigBuffer->vertexColorArray = dxdd->RLIPtr;
                    break;
                default:
                    break;
                }
                dxdd->bigBuffer->UVData = NULL;
                v12 = v4->subTextureFlags;
                if (!materialsHaveAlphaBelnding)
                    v12 |= 0x20;
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
                dxdd->TextureInit(v4->texElemIdx, v12, 0);
                if ((v4->subTextureFlags & 0x400) != 0)
                    dxdd->bigBuffer->colorMask = (dxdd->bigBuffer->colorMask | ((v4->phongOrPlanarParam & 0xFF00) << 16)) ^ 0xFF000000;
                dxdd->DrawSprites(a4, dxdd->verticesCurrent, a2->vertexNum);
            }
            if (D3DRenderParams.isDepthBlurOn
                && !D3DRenderParams.tweak_zonalpha
                && MAT_IsLastElemInMultiTexture(dxdd, v4)
                && (materialsHaveAlphaBelnding || isTransparent))
            {
                dxdd->TextureInit(-1, 0, 1);
                dxdd->DrawSprites(a4, dxdd->verticesCurrent, a2->vertexNum);
            }
            v4 = v4->nextTextureKey;
        } while (v4);
    }
    dxdd->flags &= ~0x12;
    dxdd->bigBuffer->UVData = NULL;
    dxdd->TextureInit(-1, 39, 0);
    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[32]);
}

void MAT_DrawIndexedSprites(struct sGEOGRO* geogro, struct sGEOMAT* geomat, struct sGeoSprites* spr)
{
    tGEOMAT* selectedMaterial;
    int matIdx;
    if (spr->num)
    {
        QueryPerformanceCounter(&PerfCounterStruct[12]);
        PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[28]);        
        selectedMaterial = geomat;
        if (geomat != NULL && geomat->intf->id == 4)
        {
            matIdx = geomat->multiMaterialNumIfType4ThenAmbiantColor - 1;
            if (spr->matIdx <= matIdx)
                matIdx = spr->matIdx;
            selectedMaterial = geomat->multiMaterialPtrIfType4ThenDiffuseColor[matIdx];
        }
        if (!selectedMaterial)
            selectedMaterial = &MAT_gst_DefaultSingleMaterial;
        PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[28]);

        if (selectedMaterial->intf->id == 5)
            MAT_DrawIndexedSprites_MT(gpst_DX_stDD, geogro, selectedMaterial, spr);
        
        gpst_DX_stDD->bigBuffer->colorMask = 0x00000000;
        PRO_QPC_RecordTimeDecRef(12);
    }
}

unsigned int MAT_GetValidityMASK(struct sGEOMAT* a1, int a2, struct sGEOGRO* a3)
{
    tGEOGRO* v4;
    char v5;
    tGeoTriangleChunks* v6;
    tGeoTriangleChunks* v7;
    unsigned int v8;
    tGEOMAT* v9;
    int(*v10)(tGEOMAT*, int);
    int v11;
    int v12;
    tGeoSprites* v13;
    tGeoSprites* i;
    int(*v15)(tGEOMAT*, int);
    int v16;

    if (!a1)
        return 0x11;
    if (a1->intf->id != 4)
        return a1->intf->GetValidatedMask(a1, a2);
    v4 = a3;
    if (!a3 || a2 != -1)
        return a1->intf->GetValidatedMask(a1, a2);
    v5 = -1;
    v6 = a3->triangleChunks;
    v7 = &v6[a3->triangleChunkCount];
    v8 = 0;
    if (v6 >= v7)
    {
        v9 = a1;
    }
    else
    {
        do
        {
            v9 = a1;
            v10 = a1->intf->GetValidatedMask;
            if (v10)
            {
                v11 = v10(a1, v6->materialNum);
                v4 = a3;
                v9 = a1;
            }
            else
            {
                v11 = 0;
            }
            ++v6;
            v8 |= v11;
            v5 &= v11;
        } while (v6 < v7);
    }
    v12 = v4->spritesNum;
    if (v12)
    {
        v13 = v4->spritesPtr;
        for (i = &v13[v12]; v13 < i; v5 &= v16)
        {
            v15 = v9->intf->GetValidatedMask;
            if (v15)
            {
                v16 = v15(v9, v13->matIdx);
                v9 = a1;
            }
            else
            {
                v16 = 0;
            }
            ++v13;
            v8 |= v16;
        }
    }
    return v8 ^ ((uint8_t)v8 ^ (uint8_t)v5) & 0x40;
}

BOOL MAT_VUISIdentity(struct sMultiTextureElemStruct* a1)
{
    int v1;

    v1 = a1->transform1;
    return v1 == 0x3F800000 && a1->transform2 == 0x3F800000 || !v1 || !a1->transform2;
}

void MAT_UV_COMPUTE_GetMAtrix(struct sGdi_DisplayData* a1, struct sGameObjectMain* a2, unsigned int a3, Mtx4x4* a4)
{
    int v4;
    tGameObjectMain* v5;
    tGameObjectBasic* v6;
    tGameObjectMultiPartData* v7;
    signed int v8;
    int v9;
    tMultiPartGameObjectLinks* v10;
    int v11;
    Mtx4x4* v12;
    Mtx4x4* v13;
    Mtx4x4* v14;
    double v15; 
    Mtx4x4* v16;
    Mtx4x4 a2a; 

    v4 = (uint8_t)a3 >> 6;
    if (v4 != 3 || (v5 = a1->currTargetGao, (v5->flags & 0x200000) == 0))
    {
        if (v4 == 2)
        {
            MATH_MakeOGLMatrix(a4, a1->matrixStack.actMatrixElem);
            v15 = a1->ct.cam.factorX / a1->ct.cam.centerX;
            a4->v1.x = (float)(v15 * a4->v1.x);
            a4->v1.y = (float)(v15 * a4->v1.y);
            a4->v1.z = (float)(v15 * a4->v1.z);
            a4->v2.x = (float)(v15 * a4->v2.x);
            a4->v2.y = (float)(v15 * a4->v2.y);
            a4->v2.z = (float)(v15 * a4->v2.z);
            a4->v3.x = (float)(v15 * a4->v3.x);
            a4->v3.y = (float)(v15 * a4->v3.y);
            a4->v3.z = (float)(v15 * a4->v3.z);
            a4->Pos.x = (float)(v15 * a4->Pos.x);
            a4->Pos.y = (float)(v15 * a4->Pos.y);
            a4->Pos.z = (float)(v15 * a4->Pos.z);
            MATH_TranspEq33Matrix(a4);
            return;
        }
        v14 = a2->matrices;
        goto LABEL_18;
    }
    v6 = a2->basic;
    v7 = v6->multiPartData;
    v8 = (a3 >> 8) & 7;
    if (v7->elemNum <= v8)
    {
        v14 = a2->matrices;
    LABEL_18:
        v16 = v14;
        goto LABEL_19;
    }
    if ((a2->flags & 0x1000000) == 0)
    {
        v12 = &(((Mtx4x4*)(v7->multipartGaoObjectPartsOrMatrices))[v8]);
        v13 = a4;
    LABEL_13:
        MATH_InvertMatrix(v13, v12);
        MATH_MakeOGLMatrix(v13, v13);
        MATH_TranspEq33Matrix(v13);
        return;
    }
    v9 = v8;
    v10 = &(((tMultiPartGameObjectLinks*)(v7->multipartGaoObjectPartsOrMatrices))[v9]);
    if (!v10->gao)
    {
        v16 = &MATH_gst_IdentityMatrix;
    LABEL_19:
        v13 = a4;
        MATH_MakeOGLMatrix(a4, v16);
        goto LABEL_20;
    }
    v11 = ((tMultiPartGameObjectLinks*)(v5->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v9].gao->flags;
    if ((v11 & 0x200000) == 0 || (v11 & 0x1000000) != 0)
    {
        MATH_InvertMatrix(a4, ((tMultiPartGameObjectLinks*)(v6->multiPartData->multipartGaoObjectPartsOrMatrices))[v9].gao->matrices);
        MATH_MulMatrixMatrix(&a2a, a2->matrices, a4);
        MATH_MakeOGLMatrix(a4, &a2a);
        MATH_TranspEq33Matrix(a4);
        return;
    }
    v12 = v10->mtxPtr;
    v13 = a4;
    if (v12)
        goto LABEL_13;
    MATH_MakeOGLMatrix(a4, &MATH_gst_IdentityMatrix);
LABEL_20:
    MATH_TranspEq33Matrix(v13);
}

void MAT_SetScale(int* a1, float a2)
{
    *a1 = (*(uint32_t *)&a2) ^ (((*(uint32_t*)&a2) ^ *a1) & 0x1FFFF);
}

void MAT_SetSpeed(int* a1, float a2)
{    
    *a1 ^= ((uint16_t)*a1 ^ (((*(uint32_t*)&a2) >> 16) & 0xFFFF)) & 0xFFFE;
}

void MAT_SetPos(int* a1, float a2)
{
    MAT_SetSpeed(a1, a2);
}

void MAT_SetRotation(int* a1, int* a2, float a3)
{
    float a1a;

    *a1 &= ~0x10001u;
    *a2 &= ~0x10001u;
    a1a = MAT_C_f_8Bits + a3;
    *a1 |= ((*(uint32_t*)&a1a) >> 6) & 1 | (((*(uint8_t*)&a1a) & 0x80) << 9);
    *a2 |= ((*(uint32_t*)&a1a) >> 4) & 1 | (((*(uint8_t*)&a1a) & 0x20) << 11);
}

float MAT_GetSpeed(int a1)
{
    float result;
    int tmp;

    if (a1 && a1 != 0x8001000)
    {
        tmp = a1 << 16;
        result = *(float*)&tmp;
    }
    else {
        result = 0.0;
    }
    return result;
}


float MAT_GetScale(int a1)
{
    double result;
    unsigned int tmp;

    if (a1 && a1 != 0x8001000) {
        tmp = a1 & 0xFFFE0000;
        result = *(float*)&tmp;
    }
    else
        result = 1.0;
    return (float)result;
}

float MAT_GetRotation(int a1, unsigned int a2)
{
    unsigned int tmp = (*(uint32_t*)&MAT_C_f_8Bits) | (16 * (a2 & 1 | (4 * (a1 & 1)))) | ((a1 & 0x10000 | (a2 >> 2) & 0x4000) >> 9);
    return *(float*)&tmp - MAT_C_f_8Bits;
}