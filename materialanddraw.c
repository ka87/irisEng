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

void MAT_SetRenderState(tGEOMAT* geomat, int materialNum)
{
    tGEOMAT* material;
    tMultiTextureElemStruct* texElem;
    int flags;
    int idx;
    int id;

    material = geomat;
    gpst_DX_stDD->bigBuffer->colorMask = 0;
    if (!geomat)
        material = &MAT_gst_DefaultSingleMaterial;
    if (material->intf->id == 4)
    {
        idx = materialNum;
        if (materialNum > material->multiMaterialNumIfType4ThenAmbiantColor - 1)
            idx = material->multiMaterialNumIfType4ThenAmbiantColor - 1;
        material = material->multiMaterialPtrIfType4ThenDiffuseColor[idx];
        if (!material)
            material = &MAT_gst_DefaultSingleMaterial;
    }
    id = material->intf->id;
    if (id == 5) // MTM
    {
        texElem = (tMultiTextureElemStruct*)material->idxOrPtr;
        if (texElem && (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 1) != 0)
        {
            if ((texElem->subTextureFlags & 0x400) != 0)
            {
                gpst_DX_stDD->bigBuffer->colorMask |= (texElem->phongOrPlanarParam & 0xFF00) << 16;
                gpst_DX_stDD->bigBuffer->colorMask ^= 0xFF000000;
            }
            gpst_DX_stDD->TextureInit(texElem->texElemIdx, texElem->subTextureFlags, texElem->phongOrPlanarParam);
        }
        else
        {
            gpst_DX_stDD->TextureInit(-1, 39, 0);
        }
    }
    else if (id == 3) // ST
    {
        flags = 65575;
        if (material->unkFloat2 > 0.98000002)
            flags = 39;
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 1) != 0)
            gpst_DX_stDD->TextureInit(material->idxOrPtr, flags, 0);
        else
            gpst_DX_stDD->TextureInit(-1, flags, 0);
    }
    gpst_DX_stDD->currGEOMAT = material;
}

void MAT_DrawIndexedTriangle_ST(tGdi_DisplayData* dxdd, tGEOGRO* geogro, tGEOMAT* geomat, tGeoTriangleChunks* triangleChunks)
{
    tGeoUVElem* srcUV;
    tGeoUVElem* dstUV;
    uint16_t* uvIdx;
    uint16_t* uvIdxEnd;

    PRO_PerfCounterStartBlock(&dxdd->rasters[29]);
    dxdd->bigBuffer->vertexColorArray = (int*)dxdd->bigBuffer->data3FinalColorWithShadow;
    dxdd->currGEOMAT = geomat;
    PRO_PerfCounterStartBlock(&dxdd->rasters[30]);
    MAT_SetRenderState(geomat, triangleChunks->materialNum);
    PRO_PerfCounterEndBlock(&dxdd->rasters[30]);
    dxdd->flags &= ~0x12u;
    dxdd->bigBuffer->UVData = (uint8_t*)geogro->UVs;
    if (geogro->intf->id == 1 && geogro->mrm)
    {
        dxdd->flags |= 2u;
        srcUV = &geogro->UVs[triangleChunks->MRMUVIndexElems->UVStartOffset];
        dxdd->bigBuffer->UVData = dxdd->bigBuffer->data7;
        uvIdx = triangleChunks->MRMUVIndices;
        uvIdxEnd = &uvIdx[triangleChunks->MRMUVIndicesNum];
        while (uvIdx < uvIdxEnd)
        {
            dstUV = (tGeoUVElem*)dxdd->bigBuffer->UVData;
            dstUV[*uvIdx].u = srcUV->u;
            dstUV[*uvIdx].v = srcUV->v;
            ++uvIdx;
            ++srcUV;
        }
    }
    PRO_PerfCounterStartBlock(&dxdd->rasters[31]);
    gpst_DX_stDD->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(dxdd->bigBuffer->UVData), geogro->vertexNum);
    PRO_PerfCounterEndBlock(&dxdd->rasters[31]);
    PRO_PerfCounterEndBlock(&dxdd->rasters[29]);
}

int MAT_IsObjectTransparent(tGEOMAT* geomat)
{
    tMultiTextureElemStruct* texElem;

    texElem = (tMultiTextureElemStruct*)geomat->idxOrPtr;
    while ((texElem->subTextureFlags & 0xA00) != 0)  // 0x200 - zwriteneable
                                                       // 0x800 - transparent
    {
        texElem = texElem->nextTextureKey;
        if (!texElem || texElem == (tMultiTextureElemStruct*)-1)
            return 0;
    }
    return 1;
}

BOOL MAT_MaterialsHaveAlphaBlendingType3And4(tGEOMAT* geomat)
{
    tMultiTextureElemStruct* currTexElem;
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
            return FALSE;
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
    tMultiTextureElemStruct* texElem;
    unsigned int flags;
    unsigned int blurFlags;
    int drawMask;
    int transparentObject;
    int materialsHaveAlphaBlending;

    bigBuff = dxdd->bigBuffer;
    dxdd->currGEOMAT = gmat;
    texElem = (tMultiTextureElemStruct*)gmat->idxOrPtr;
    drawMask = dxdd->drawMaskAllowedStuffCurrent;
    if (texElem && (drawMask & 1) != 0)
    {
        transparentObject = MAT_IsObjectTransparent(gmat);
        materialsHaveAlphaBlending = MAT_MaterialsHaveAlphaBlendingType3And4(gmat);
        dxdd->drawMaskAllowedStuffCurrent = drawMask | 0x200;
        while (texElem && texElem != (tMultiTextureElemStruct*)-1)
        {
            if (texElem->texElemIdx == -1 || (TEX_gst_GlobalList.texElems[texElem->texElemIdx].texElemFlag & 0x20) == 0)
            {
                if ((texElem->subTextureFlags & 0x800) == 0)
                {
                    bigBuff->colorMask = 0;
                    bigBuff->UVData = 0;
                    bigBuff->vertexColorArray = 0;
                    bigBuff->solidVertexColor = 0;
                    bigBuff->vertexColorArraySecond = 0;
                    switch ((uint8_t)(((texElem->subTextureFlags) & 0xffff) >> 8) >> 4) // elõször 16-tal letolja majd 8-cal vissza (HIBYTE(LOWORD(v6->subTextureFlags)))
                    {
                    case 0:
                        bigBuff->vertexColorArray = (int*)bigBuff->data3FinalColorWithShadow;// finalcolor shadow-val
                        break;
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
                        bigBuff->vertexColorArray = dxdd->RLIPtr;
                        break;
                    case 4:
                        bigBuff->solidVertexColor = 0xFFFFFF;
                        bigBuff->vertexColorArray = 0;
                        break;
                    case 5:
                        bigBuff->colorMask = 0xFFFFFF;
                        bigBuff->vertexColorArray = dxdd->RLIPtr;
                        break;
                    case 6:
                        D3DRenderParams.diffuse2x = 1;
                        bigBuff->vertexColorArray = (int*)bigBuff->data3FinalColorWithShadow;
                        break;
                    default:
                        break;
                    }
                    if ((texElem->subTextureFlags & 0x400) != 0)
                        bigBuff->colorMask = (bigBuff->colorMask | ((texElem->phongOrPlanarParam & 0xFF00) << 16)) ^ 0xFF000000;
                    flags = texElem->subTextureFlags;
                    if (!materialsHaveAlphaBlending)
                        flags |= 0x20u;
                    dxdd->TextureInit(texElem->texElemIdx, flags, 0);
                    MAT_UV_ComputeAndSetTransform(geogro, triangleChunks, texElem);
                    gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
                    dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
                    if (D3DRenderParams.diffuse2x && !D3DRenderParams.tweak_ae_d2x)
                    {
                        blurFlags = (texElem->subTextureFlags & ~0xA0000u) | 0x50000;
                        if (!materialsHaveAlphaBlending)
                            blurFlags = (texElem->subTextureFlags & ~0xA0000u) | 0x50020;
                        dxdd->TextureInit(texElem->texElemIdx, blurFlags, 0);
                        gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
                        dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
                    }
                    D3DRenderParams.diffuse2x = 0;
                }
                if (D3DRenderParams.isDepthBlurOn
                    && !D3DRenderParams.tweak_zonalpha
                    && MAT_IsLastElemInMultiTexture(dxdd, texElem)
                    && (materialsHaveAlphaBlending || transparentObject))
                {
                    dxdd->TextureInit(-1, 0, 1);
                    gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
                    dxdd->DrawElementIndexedTriangles(triangleChunks, dxdd->verticesCurrent, (tGeoUVElem*)(bigBuff->UVData), geogro->vertexNum);
                }
            }
            texElem = texElem->nextTextureKey;
        }
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
    tGEOMAT* selectedMaterial;
    int materialCount;
    int matIdx;
    int renderingFlags;
    int transparent;
    int wantTransparent;

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
    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[28]);
    selectedMaterial = geomat;
    if (geomat && geomat->intf->id == 4)
    {
        materialCount = geomat->multiMaterialNumIfType4ThenAmbiantColor;
        if (materialCount)
        {
            matIdx = materialCount - 1;
            if (triangleChunks->materialNum <= matIdx)
                matIdx = triangleChunks->materialNum;
            selectedMaterial = geomat->multiMaterialPtrIfType4ThenDiffuseColor[matIdx];
        }
        else
        {
            selectedMaterial = NULL;
        }
    }
    if (!selectedMaterial)
        selectedMaterial = &MAT_gst_DefaultSingleMaterial;

    renderingFlags = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    wantTransparent = (gpst_DX_stDD->flags & 4) != 0;
    transparent = MAT_IsMaterialTransparent(selectedMaterial, -1, renderingFlags, 0) != 0;
    if (transparent != wantTransparent)
    {
        PRO_RecordTimeDecRef(&gpst_DX_stDD->rasters[28]);
        PRO_QPC_RecordTimeDecRef(12);
        return;
    }
    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[28]);
    if (selectedMaterial->intf->id == 5)
    {
        MAT_DrawIndexedTriangle_MT(gpst_DX_stDD, geogro, selectedMaterial, triangleChunks);
    }
    else if (selectedMaterial->intf->id == 3)
    {
        gpst_DX_stDD->rasters[9].unk2 += triangleChunks->triangleNum;
        MAT_DrawIndexedTriangle_ST(gpst_DX_stDD, geogro, selectedMaterial, triangleChunks);
    }
    PRO_QPC_RecordTimeDecRef(12);
    gpst_DX_stDD->bigBuffer->colorMask = 0;
}

void GAO_XMN_Render_TransparentTriangles(struct sGEOGRO* geogro, struct sGEOMAT* geomat)
{
    tGeoTriangleChunks* chunk;
    tGeoTriangleChunks* chunkEnd;
    tGeoTriangleChunkElem* tri;
    tGeoTriangleChunkElem* triEnd;
    tZListElem* node;
    tZListMeshElem* mesh;
    tGeoUVElem* uvs;
    Vec3* points;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    double maxZ;
    float z;
    int savedDrawMask;
    int invZ;

    chunk = geogro->triangleChunks;
    chunkEnd = &chunk[geogro->triangleChunkCount];
    savedDrawMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    for (; chunk < chunkEnd; chunk++)
    {
        SOFT_TransformInComputingBuffer(gpst_DX_stDD->bigBuffer, gpst_DX_stDD->verticesCurrent, geogro->vertexNum, &gpst_DX_stDD->ct);
        points = (Vec3*)gpst_DX_stDD->bigBuffer->data6;
        tri = chunk->triangleChunkElems;
        triEnd = &tri[chunk->triangleNum];
        for (; tri < triEnd; tri++)
        {
            p1 = &points[tri->v1];
            p2 = &points[tri->v2];
            p3 = &points[tri->v3];
            maxZ = p1->z;
            if (maxZ < p2->z)
                maxZ = p2->z;
            if (maxZ < p3->z)
                maxZ = p3->z;
            z = (float)maxZ;
            invZ = MATH_gl_TableInverse[((*((int32_t*)&z)) >> 13) & 0x3FF] - ((*((uint32_t*)&z)) & 0xFF800000) + 0x7E800000;
            node = SOFT_pst_ZList_AddNode(*(float *)&invZ, 3, 0);
            if (node)
            {
                mesh = (tZListMeshElem*)node->data;
                mesh[0].point = *p1;
                mesh[1].point = *p2;
                mesh[2].point = *p3;
                uvs = geogro->UVs;
                if (uvs)
                {
                    mesh[0].uv.u = uvs[tri->uv1].u;
                    mesh[0].uv.v = uvs[tri->uv1].v;
                    mesh[1].uv = uvs[tri->uv2];
                    mesh[2].uv = uvs[tri->uv3];
                }
                mesh[0].vertexColor = 0xFFFFFF;
                mesh[1].vertexColor = 0xFFFFFF;
                mesh[2].vertexColor = 0xFFFFFF;
                node->geomat = geomat;
                node->renderingFlags = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
            }
        }
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedDrawMask;
    }
}

void GEO_Render_TransparentTriangles(struct sGdi_DisplayData* dxdd, struct sGEOGRO* geogro, struct sGameObjectVisualObject* visual)
{
    tGeoTriangleChunks* chunk;
    tGeoTriangleChunks* chunkEnd;
    tGeoTriangleChunkElem* tri;
    tGeoTriangleChunkElem* triEnd;
    tZListElem* node;
    tZListMeshElem* mesh;
    tGEOMAT* chunkMaterial;
    tGeoUVElem* uvs;
    Vec3* points;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    double maxZ;
    float z;
    int vertexNum;
    int savedDrawMask;
    int invZ;

    vertexNum = geogro->vertexNum;
    if (vertexNum > 5000)
        return;
    chunk = geogro->triangleChunks;
    chunkEnd = &chunk[geogro->triangleChunkCount];
    savedDrawMask = dxdd->drawMaskAllowedStuffCurrent;
    SOFT_TransformInComputingBuffer(dxdd->bigBuffer, dxdd->verticesCurrent, vertexNum, &dxdd->ct);
    for (; chunk < chunkEnd; chunk++)
    {
        dxdd->rasters[9].unk2 += chunk->triangleNum;
        chunkMaterial = GEO_GetMaterial((tGEOMAT*)visual->GROMAT, chunk->materialNum);
        points = (Vec3*)dxdd->bigBuffer->data6;
        tri = chunk->triangleChunkElems;
        triEnd = &tri[chunk->triangleNum];
        for (; tri < triEnd; tri++)
        {
            p1 = &points[tri->v1];
            p2 = &points[tri->v2];
            p3 = &points[tri->v3];
            maxZ = p1->z;
            if (maxZ < p2->z)
                maxZ = p2->z;
            if (maxZ < p3->z)
                maxZ = p3->z;
            if (maxZ > 0.0)
            {
                z = (float)maxZ;
                invZ = MATH_gl_TableInverse[((*((int32_t*)(&z))) >> 13) & 0x3FF] - ((*((uint32_t*)(&z))) & 0xFF800000) + 0x7E800000;
                node = SOFT_pst_ZList_AddNode((*((float*)(&invZ))), 3, 0);
                if (node)
                {
                    mesh = (tZListMeshElem*)node->data;
                    mesh[0].point = *p1;
                    mesh[1].point = *p2;
                    mesh[2].point = *p3;
                    uvs = geogro->UVs;
                    if (uvs)
                    {
                        mesh[0].uv.u = uvs[tri->uv1].u;
                        mesh[0].uv.v = uvs[tri->uv1].v;
                        mesh[1].uv = uvs[tri->uv2];
                        mesh[2].uv = uvs[tri->uv3];
                    }
                    mesh[0].vertexColor = *(uint32_t*)&dxdd->bigBuffer->data3FinalColorWithShadow[4 * tri->v1];
                    mesh[1].vertexColor = *(uint32_t*)&dxdd->bigBuffer->data3FinalColorWithShadow[4 * tri->v2];
                    mesh[2].vertexColor = *(uint32_t*)&dxdd->bigBuffer->data3FinalColorWithShadow[4 * tri->v3];
                    node->geomat = chunkMaterial;
                    node->renderingFlags = dxdd->drawMaskAllowedStuffCurrent;
                }
            }
        }
        dxdd->drawMaskAllowedStuffCurrent = savedDrawMask;
    }
}

void MAT_DrawIndexedSprites_MT(tGdi_DisplayData* dxdd, tGEOGRO* geogro, tGEOMAT* geomat, tGeoSprites* spr)
{
    tMultiTextureElemStruct* texElem;
    unsigned int flags;
    int isTransparent;
    int materialsHaveAlphaBelnding;

    texElem = (tMultiTextureElemStruct*)geomat->idxOrPtr;
    if (texElem)
    {
        isTransparent = MAT_IsObjectTransparent(geomat);
        materialsHaveAlphaBelnding = MAT_MaterialsHaveAlphaBlendingType3And4(geomat);
        dxdd->bigBuffer->vertexColorArray = (int*)dxdd->bigBuffer->data3FinalColorWithShadow;
        dxdd->currGEOMAT = geomat;
        dxdd->drawMaskAllowedStuffCurrent |= 0x200;
        PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[32]);
        PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[32]);
        dxdd->bigBuffer->solidVertexColor = geomat->solidVertexColor;
        while (texElem && texElem != (tMultiTextureElemStruct*)-1)
        {
            dxdd->bigBuffer->vertexColorArraySecond = NULL;
            if ((texElem->subTextureFlags & 0x800) == 0)
            {
                dxdd->bigBuffer->colorMask = 0;
                switch ((uint8_t)(((texElem->subTextureFlags & 0xffff) >> 8) & 0xff) >> 4) // elõször 16-tal letolja majd 8-cal vissza (HIBYTE(LOWORD(v4->subTextureFlags)))
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
                    dxdd->bigBuffer->solidVertexColor = geomat->solidVertexColor & 0xFFFFFF;
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
                flags = texElem->subTextureFlags;
                if (!materialsHaveAlphaBelnding)
                    flags |= 0x20;
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
                dxdd->TextureInit(texElem->texElemIdx, flags, 0);
                if ((texElem->subTextureFlags & 0x400) != 0)
                    dxdd->bigBuffer->colorMask = (dxdd->bigBuffer->colorMask | ((texElem->phongOrPlanarParam & 0xFF00) << 16)) ^ 0xFF000000;
                dxdd->DrawSprites(spr, dxdd->verticesCurrent, geogro->vertexNum);
            }
            if (D3DRenderParams.isDepthBlurOn
                && !D3DRenderParams.tweak_zonalpha
                && MAT_IsLastElemInMultiTexture(dxdd, texElem)
                && (materialsHaveAlphaBelnding || isTransparent))
            {
                dxdd->TextureInit(-1, 0, 1);
                dxdd->DrawSprites(spr, dxdd->verticesCurrent, geogro->vertexNum);
            }
            texElem = texElem->nextTextureKey;
        }
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

unsigned int MAT_GetValidityMASK(struct sGEOMAT* geomat, int idx, struct sGEOGRO* geogro)
{
    tGeoTriangleChunks* chunk;
    tGeoTriangleChunks* chunkEnd;
    tGeoSprites* sprite;
    tGeoSprites* spriteEnd;
    unsigned int anyMask;
    char allMask;
    int mask;

    if (!geomat)
        return 0x11;
    if (geomat->intf->id != 4 || !geogro || idx != -1)
        return geomat->intf->GetValidatedMask(geomat, idx);
    allMask = -1;
    anyMask = 0;
    chunk = geogro->triangleChunks;
    chunkEnd = &chunk[geogro->triangleChunkCount];
    for (; chunk < chunkEnd; chunk++)
    {
        mask = 0;
        if (geomat->intf->GetValidatedMask)
            mask = geomat->intf->GetValidatedMask(geomat, chunk->materialNum);
        anyMask |= mask;
        allMask &= mask;
    }
    if (geogro->spritesNum)
    {
        sprite = geogro->spritesPtr;
        spriteEnd = &sprite[geogro->spritesNum];
        for (; sprite < spriteEnd; sprite++)
        {
            mask = 0;
            if (geomat->intf->GetValidatedMask)
                mask = geomat->intf->GetValidatedMask(geomat, sprite->matIdx);
            anyMask |= mask;
            allMask &= mask;
        }
    }
    return anyMask ^ ((uint8_t)anyMask ^ (uint8_t)allMask) & 0x40;
}

BOOL MAT_VUISIdentity(struct sMultiTextureElemStruct* texElem)
{
    return texElem->transform1 == 0x3F800000 && texElem->transform2 == 0x3F800000
        || !texElem->transform1
        || !texElem->transform2;
}

void MAT_UV_COMPUTE_GetMAtrix(struct sGdi_DisplayData* dxdd, struct sGameObjectMain* gao, unsigned int uvMode, Mtx4x4* mtx)
{
    tGameObjectMain* targetGao;
    tGameObjectBasic* basic;
    tGameObjectMultiPartData* multiPart;
    tMultiPartGameObjectLinks* link;
    Mtx4x4* src;
    signed int partIdx;
    int mode;
    int linkFlags;
    double scale;
    Mtx4x4 partMtx;

    mode = (uint8_t)uvMode >> 6;
    if (mode == 3 && (targetGao = dxdd->currTargetGao, (targetGao->flags & 0x200000) != 0))
    {
        basic = gao->basic;
        multiPart = basic->multiPartData;
        partIdx = (uvMode >> 8) & 7;
        if (multiPart->elemNum <= partIdx)
        {
            src = gao->matrices;
        }
        else if ((gao->flags & 0x1000000) == 0)
        {
            MATH_InvertMatrix(mtx, &(((Mtx4x4*)(multiPart->multipartGaoObjectPartsOrMatrices))[partIdx]));
            MATH_MakeOGLMatrix(mtx, mtx);
            MATH_TranspEq33Matrix(mtx);
            return;
        }
        else
        {
            link = &(((tMultiPartGameObjectLinks*)(multiPart->multipartGaoObjectPartsOrMatrices))[partIdx]);
            src = &MATH_gst_IdentityMatrix;
            if (link->gao)
            {
                linkFlags = ((tMultiPartGameObjectLinks*)(targetGao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[partIdx].gao->flags;
                if ((linkFlags & 0x200000) == 0 || (linkFlags & 0x1000000) != 0)
                {
                    MATH_InvertMatrix(mtx, ((tMultiPartGameObjectLinks*)(basic->multiPartData->multipartGaoObjectPartsOrMatrices))[partIdx].gao->matrices);
                    MATH_MulMatrixMatrix(&partMtx, gao->matrices, mtx);
                    MATH_MakeOGLMatrix(mtx, &partMtx);
                    MATH_TranspEq33Matrix(mtx);
                    return;
                }
                if (link->mtxPtr)
                {
                    MATH_InvertMatrix(mtx, link->mtxPtr);
                    MATH_MakeOGLMatrix(mtx, mtx);
                    MATH_TranspEq33Matrix(mtx);
                    return;
                }
            }
        }
    }
    else if (mode == 2)
    {
        MATH_MakeOGLMatrix(mtx, dxdd->matrixStack.actMatrixElem);
        scale = dxdd->ct.cam.factorX / dxdd->ct.cam.centerX;
        mtx->v1.x = (float)(scale * mtx->v1.x);
        mtx->v1.y = (float)(scale * mtx->v1.y);
        mtx->v1.z = (float)(scale * mtx->v1.z);
        mtx->v2.x = (float)(scale * mtx->v2.x);
        mtx->v2.y = (float)(scale * mtx->v2.y);
        mtx->v2.z = (float)(scale * mtx->v2.z);
        mtx->v3.x = (float)(scale * mtx->v3.x);
        mtx->v3.y = (float)(scale * mtx->v3.y);
        mtx->v3.z = (float)(scale * mtx->v3.z);
        mtx->Pos.x = (float)(scale * mtx->Pos.x);
        mtx->Pos.y = (float)(scale * mtx->Pos.y);
        mtx->Pos.z = (float)(scale * mtx->Pos.z);
        MATH_TranspEq33Matrix(mtx);
        return;
    }
    else
    {
        src = gao->matrices;
    }
    MATH_MakeOGLMatrix(mtx, src);
    MATH_TranspEq33Matrix(mtx);
}

void MAT_SetScale(int* transform, float scale)
{
    *transform = (*(uint32_t *)&scale) ^ (((*(uint32_t*)&scale) ^ *transform) & 0x1FFFF);
}

void MAT_SetSpeed(int* transform, float speed)
{
    *transform ^= ((uint16_t)*transform ^ (((*(uint32_t*)&speed) >> 16) & 0xFFFF)) & 0xFFFE;
}

void MAT_SetPos(int* transform, float pos)
{
    MAT_SetSpeed(transform, pos);
}

void MAT_SetRotation(int* transformU, int* transformV, float angle)
{
    float biased;

    *transformU &= ~0x10001u;
    *transformV &= ~0x10001u;
    biased = MAT_C_f_8Bits + angle;
    *transformU |= ((*(uint32_t*)&biased) >> 6) & 1 | (((*(uint8_t*)&biased) & 0x80) << 9);
    *transformV |= ((*(uint32_t*)&biased) >> 4) & 1 | (((*(uint8_t*)&biased) & 0x20) << 11);
}

float MAT_GetSpeed(int transform)
{
    float result;
    int tmp;

    if (transform && transform != 0x8001000)
    {
        tmp = transform << 16;
        result = *(float*)&tmp;
    }
    else {
        result = 0.0;
    }
    return result;
}


float MAT_GetScale(int transform)
{
    float result;
    unsigned int tmp;

    if (transform && transform != 0x8001000) {
        tmp = transform & 0xFFFE0000;
        result = *(float*)&tmp;
    }
    else
        result = 1.0;
    return result;
}

float MAT_GetRotation(int transformU, unsigned int transformV)
{
    unsigned int tmp = (*(uint32_t*)&MAT_C_f_8Bits) | (16 * (transformV & 1 | (4 * (transformU & 1)))) | ((transformU & 0x10000 | (transformV >> 2) & 0x4000) >> 9);
    return *(float*)&tmp - MAT_C_f_8Bits;
}
