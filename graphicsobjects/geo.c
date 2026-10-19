#include "../profiler.h"
#include "../memory.h"
#include "../loading.h"
#include "../gdi.h"
#include "../win/wingdi.h"
#include "../time.h"
#include "../lights.h"
#include "../modifiers/lazymodifier.h"
#include "../modifiers/protexmodifier.h"
#include "../materialanddraw.h"
#include "../animation.h"
#include "../dbg.h"
#include "geo.h"

unsigned int GEO_MRM_GetNumberOfPoints_Curved(tGEOGRO* geo, float quality)
{
    unsigned int pointsNum;
    unsigned int idx;
    double factor;
    double mult;
    float hax;
    int realRoundedValueWithoutIdx;

    if (geo == NULL) // better than null pointer
        return 0;

    if (geo->intf->id != 1 || geo->mrm == NULL)
        return geo->vertexNum;
    
    if (quality >= 1.0f)
        return geo->mrm->endNum;
    
    if (geo->mrm->f[0] != 0.0f)
        quality *= geo->mrm->f[0];
    
    hax = (float)(quality * 8.0 + 49152.0);
    idx = ((*(int32_t*)(&hax)) >> 8) & 0xFFF;
    realRoundedValueWithoutIdx = (*(int32_t*)(&hax)) & 0xFFF000FF;
    factor = (*((float *)(&realRoundedValueWithoutIdx))) - 49152.0;

    if (!idx)
    {
        mult = factor * geo->mrm->f[1];
        pointsNum = (unsigned int)(geo->mrm->endNum * mult);
        if (pointsNum < 8) 
            pointsNum = 8;
        return pointsNum;
    }

    if (idx > 7)
        return geo->mrm->endNum;

    if (idx == 7)
        mult = factor + (1.0f - factor) * geo->mrm->f[7];
    else
        mult = (1.0f - factor) * geo->mrm->f[idx] + factor * geo->mrm->f[idx + 1];

    pointsNum = (unsigned int)(geo->mrm->endNum * mult);
    if (pointsNum < 8)
        pointsNum = 8;
    return pointsNum;
}

void GEO_MRM_SetNumberOfPoints_Number(tGEOGRO* geo, unsigned int num)
{
    int16_t* dataPtr;
    unsigned int numLimited;
    unsigned int i, j;

    if (geo == NULL || geo->intf->id != 1 || geo->mrm == NULL) return;

    if (gpst_DX_stDD != NULL)
    {
        PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[42]);

        dataPtr = (int16_t*)gpst_DX_stDD->bigBuffer->data1LightColors;
    }
    else {
        dataPtr = (int16_t*)MEM_p_Alloc(sizeof(int16_t) * geo->mrm->endNum);
    }

    numLimited = num;
    if (num < 8)
        numLimited = 8;

    if (numLimited > geo->mrm->endNum)
        numLimited = geo->mrm->endNum;

    geo->vertexNum = numLimited;

    if (gpst_DX_stDD)
        gpst_DX_stDD->MRMPtr = geo->mrm->ptr;

    for (i = 0; i < numLimited; i++) {
        dataPtr[i] = i;
    }

    for (; i < geo->mrm->endNum; i++) {
        if (geo->mrm->shorts[i] < numLimited) {
            dataPtr[i] = geo->mrm->shorts[i];
        }
        else {
            dataPtr[i] = dataPtr[geo->mrm->shorts[i]];
        }
    }

    for (i = 0; i < geo->triangleChunkCount; i++) {
        if (geo->triangleChunks[i].MRMUVIndices != NULL) {
            geo->triangleChunks[i].MRMUVIndicesNum = 0;

            for (j = 0; geo->triangleChunks[i].MRMUVIndices[j] < numLimited; j++)
                geo->triangleChunks[i].MRMUVIndicesNum++;
        }

        geo->triangleChunks[i].triangleNum = 0;

        if (geo->triangleChunks[i].MRMUVIndexElems->i2)
        {
            for (j = 0; geo->triangleChunks[i].triangleChunkElems[j].mrmSmoothingGroup < numLimited; j++) {
                geo->triangleChunks[i].triangleChunkElems[j].v1 = dataPtr[geo->triangleChunks[i].triangleChunkElems[j].uv1];
                geo->triangleChunks[i].triangleChunkElems[j].v2 = dataPtr[geo->triangleChunks[i].triangleChunkElems[j].uv2];
                geo->triangleChunks[i].triangleChunkElems[j].v3 = dataPtr[geo->triangleChunks[i].triangleChunkElems[j].uv3];
                geo->triangleChunks[i].triangleNum++;
            }
        }
    }

    if (gpst_DX_stDD) {
        PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[42]);
    }
    else {
        MEM_Free(dataPtr);
    }
}

void GEO_MRM_SetNumberOfPoints_Percent(struct sGEOGRO* geo, float quality)
{
    if (geo == NULL || geo->intf->id != 1 || geo->mrm == NULL) return;

    QueryPerformanceCounter(&PerfCounterStruct[22]);
    GEO_MRM_SetNumberOfPoints_Number(geo, GEO_MRM_GetNumberOfPoints_Curved(geo, quality));
    PRO_QPC_RecordTimeDecRef(22);

}

void GEO_UseNormals(struct sGEOGRO* geogro)
{
    if (!geogro->normals)
    {
        geogro->normals = (Vec3*)MEM_GEO_p_AllocAlign(16 * geogro->vertexNum, 16);
        GEO_ComputeNormals(geogro);
    }
}

void GEO_ComputeNormals(struct sGEOGRO* geo)
{
    GEO_SKN_ComputeNormals(geo, geo->vertices);
}

void GEO_SKN_ComputeNormals(struct sGEOGRO* geo, Vec3* vertices)
{
    unsigned int i, j;
    Vec3 v1v2Dist, v1v3Dist;
    Vec3 cp;

    if (!geo->vertexNum) return;

    QueryPerformanceCounter(&PerfCounterStruct[2]);

    GEO_UseNormals(geo);

    memset(geo->normals, 0, sizeof(Vec3) * geo->vertexNum);

    for (i = 0; i < geo->triangleChunkCount; i++) {
        for (j = 0; j < geo->triangleChunks[i].triangleNum; j++) {
            MATH_SubstractVec(&v1v2Dist, &vertices[geo->triangleChunks[i].triangleChunkElems[j].v1], &vertices[geo->triangleChunks[i].triangleChunkElems[j].v2]);
            MATH_SubstractVec(&v1v3Dist, &vertices[geo->triangleChunks[i].triangleChunkElems[j].v1], &vertices[geo->triangleChunks[i].triangleChunkElems[j].v3]);
            
            if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x8000) == 0)
            {
                v1v2Dist.x = -v1v2Dist.x;
                v1v3Dist.x = -v1v3Dist.x;
            }

            MATH_CrossProduct(&cp, &v1v2Dist, &v1v3Dist);
            MATH_VecAddVecWithoutBuffer(&geo->normals[geo->triangleChunks[i].triangleChunkElems[j].v1], &cp);
            MATH_VecAddVecWithoutBuffer(&geo->normals[geo->triangleChunks[i].triangleChunkElems[j].v2], &cp);
            MATH_VecAddVecWithoutBuffer(&geo->normals[geo->triangleChunks[i].triangleChunkElems[j].v3], &cp);
        }
    }

    for (i = 0; i < geo->vertexNum; i++) {
        MATH_NormalizeEqualVector(&geo->normals[i]);
    }
    
    PRO_QPC_RecordTimeDecRef(2);
}

void GEO_AllocElementContent(struct sGeoTriangleChunks* tc)
{
    if (tc == NULL) return;

    if (tc->triangleNum)
    {
        LOA_ul_FileTypeSize_24 += sizeof(tGeoTriangleChunkElem) * (tc->triangleNum + 1); // had wrong value inside the original exe
        tc->triangleChunkElems = (tGeoTriangleChunkElem*)MEM_GEO_p_AllocAlign(sizeof(tGeoTriangleChunkElem) * (tc->triangleNum + 1), 16);
        tc->triangleChunkElems[tc->triangleNum].mrmSmoothingGroup = 0xffffffff;
    }

    if (tc->MRMUVIndexElems)
        tc->MRMUVIndexElems = (tGeoMRMUVIndexElem*)MEM_GEO_p_Alloc(sizeof(tGeoMRMUVIndexElem));

    tc->MRMUVIndices = NULL;

    if (tc->MRMUVIndicesNum)
    {
        LOA_ul_FileTypeSize_25 += sizeof(uint16_t) * (tc->MRMUVIndicesNum + 1);
        tc->MRMUVIndices = (uint16_t*)MEM_GEO_p_AllocAlign(sizeof(uint16_t) * (tc->MRMUVIndicesNum + 1), 8);
    }
    tc->triangleStripBlock = NULL;
}

void GEO_AllocContent(struct sGEOGRO* geo)
{
    unsigned int i;
    int vertexColorNum;
    int* vertexColors;
    void *mrmMagic;

    if (geo == NULL) return;

    if (geo->vertexNum)
    {
        LOA_ul_FileTypeSize_20 += sizeof(Vec3) * geo->vertexNum;
        LOA_ul_FileTypeSize_21 += sizeof(Vec3) * geo->vertexNum;
        geo->vertices = (Vec3*)MEM_GEO_p_AllocAlign(sizeof(Vec3) * geo->vertexNum, 16);
        geo->normals = (Vec3*)(GEO_b_IsInFix ? MEM_GEO_p_AllocAlign(16 * geo->vertexNum, 16) : 0); // FIXME, ez miert 16?
        vertexColorNum = (int)geo->vertexColors;

        if (vertexColorNum)
        {
            LOA_ul_FileTypeSize_22 += sizeof(uint32_t) * (vertexColorNum + 4);
            vertexColors = ((int*)MEM_GEO_p_AllocAlign(sizeof(uint32_t) * (vertexColorNum + 4), 16)) + 3; // TODO FIXME: ezt miért tolja el 12-vel (3 * 4bytes)? -- lásd GEO_FREE, ide belehány vmi ptr-t is (de mér?!)
            geo->vertexColors = vertexColors;
            vertexColors[0] = vertexColorNum;
        }
    }

    if (geo->mrm)
    {
        mrmMagic = geo->mrm;
        geo->mrm = (tGeoMRM*)MEM_GEO_p_Alloc(sizeof(tGeoMRM));
        geo->mrm->shorts = (uint16_t*)MEM_GEO_p_Alloc(sizeof(uint16_t) * geo->vertexNum);
        if (mrmMagic == (void*)0xc0de0008)
            geo->mrm->ptr = (uint16_t*)MEM_GEO_p_Alloc(sizeof(uint16_t) * geo->vertexNum);
        else
            geo->mrm->ptr = NULL;
        geo->mrm->endNum = geo->vertexNum;
        geo->mrm->b = 0;
    }

    if (geo->uvNum)
    {
        LOA_ul_FileTypeSize_23 += sizeof(tGeoUVElem) * geo->uvNum;
        geo->UVs = (tGeoUVElem*)MEM_GEO_p_AllocAlign(sizeof(tGeoUVElem) * geo->uvNum, 8);
    }

    if (geo->triangleChunkCount)
    {
        geo->triangleChunks = (tGeoTriangleChunks*)MEM_GEO_p_Alloc(sizeof(tGeoTriangleChunks) * geo->triangleChunkCount);
        LOA_ul_FileTypeSize_28 += sizeof(tGeoTriangleChunks) * geo->triangleChunkCount;

        for (i = 0; i < geo->triangleChunkCount; i++) {
            geo->triangleChunks[i].triangleStripBlock = NULL;
        }        
    }
}

struct sGEOGRO* GEO_pst_Create(int vertexNum, int uvNum, int triangleChunkNum, int* vertexColors)
{
    tGEOGRO* geo = (tGEOGRO*)MEM_GEO_p_Alloc(sizeof(tGEOGRO));

    memset(geo, 0, sizeof(tGEOGRO));
    
    GRO_Struct_Init((tGroGeneric*)geo, 1);
    
    geo->vertexNum = vertexNum;
    geo->vertexColors = vertexColors;
    geo->uvNum = uvNum;
    geo->triangleChunkCount = triangleChunkNum;
    geo->mrm = NULL;
    geo->skinning = NULL;
    geo->unkFlags = 0;
    geo->spritesNum = 0;
    
    GEO_AllocContent(geo);

    return geo;
}

void GEO_SKN_DestroyObjPonderation(struct sGeoSkn* skn)
{
    int i;

    if (skn == NULL) return;

    if (skn->num)
    {
        for (i = skn->num - 1; i >= 0; i--) {
            if (skn->ptr[i].ptr != NULL) {
                MEM_FreeAlign(skn->ptr[i].ptr->weightsAndIndicesPtr);
                MEM_Free(skn->ptr[i].ptr);
            }
        }
        MEM_Free(skn->ptr);
    }
    MEM_Free(skn);
}

void GEO_FreeElementStripContent(tGeoTriangleChunks* tc)
{
    if (tc->triangleStripBlock == NULL) return;

    if (tc->triangleStripBlock->triangleStrips)
    {
        MEM_FreeAlign(tc->triangleStripBlock->triangleStripElemsContinousPool);
        MEM_FreeAlign(tc->triangleStripBlock->triangleStrips);
        tc->triangleStripBlock->triangleStrips = NULL;
    }
    MEM_FreeAlign(tc->triangleStripBlock->d3dStreamInfoElems);
    tc->triangleStripBlock->d3dStreamInfoElems = NULL;
    tc->triangleStripBlock->vertexColorArraySecond = 0;
    tc->triangleStripBlock->triangleStripNum = 0;
    tc->triangleStripBlock->unk2 = 0;
    tc->triangleStripBlock->unk1 = 0;
    tc->triangleStripBlock->unk3 = 0;
    MEM_FreeAlign(tc->triangleStripBlock);
    tc->triangleStripBlock = NULL;
}

void GEO_FreeElementContent(tGeoTriangleChunks* tc)
{
    tGeoMRMUVIndexElem* mrmuvie;

    if (tc == NULL) return;

    if (tc->triangleNum || (mrmuvie = tc->MRMUVIndexElems) != 0 && mrmuvie->i2)
    {
        MEM_FreeAlign(tc->triangleChunkElems);
        tc->triangleNum = 0;
    }
    if (tc->MRMUVIndexElems)
    {
        MEM_FreeAlign(tc->MRMUVIndexElems);
        tc->MRMUVIndexElems = NULL;
    }
    if (tc->MRMUVIndicesNum)
    {
        MEM_FreeAlign(tc->MRMUVIndices);
        tc->MRMUVIndices = NULL;
    }
    GEO_FreeElementStripContent(tc);
}

void GEO_FreeSpriteElementContent(tGeoSprites* spr)
{
    MEM_FreeAlign(spr->spriteElems);
}

void GEO_FreeContent(tGEOGRO* geo)
{    
    unsigned int i, j;

    if (!geo) return;

    if (geo->vertexNum)
    {
        MEM_FreeAlign(geo->vertices);
        if (geo->normals)
            MEM_FreeAlign(geo->normals);
        if (geo->vertexColors) 
            MEM_FreeAlign(geo->vertexColors - 3); // see GEO_AllocContent FIXME for "- 3"
        geo->vertexNum = 0;
    }

    if (geo->mrm)
    {
        MEM_FreeAlign(geo->mrm->shorts);
        if (geo->mrm->ptr)
            MEM_FreeAlign(geo->mrm->ptr);
        MEM_FreeAlign(geo->mrm);
        geo->mrm = NULL;
    }

    if (geo->skinning)
    {
        GEO_SKN_DestroyObjPonderation(geo->skinning);
        geo->skinning = NULL;
    }

    if (geo->uvNum)
    {
        MEM_FreeAlign(geo->UVs);
        geo->uvNum = 0;
    }

    if (geo->collisionPtr)
    {
        for (i = 0; i < geo->collisionPtr->num; i++) {
            for (j = 0; j < geo->collisionPtr->aaBoxes[i].num; j++) {
                if (geo->collisionPtr->aaBoxes[i].chunkElems[j].triangleChunkElemIDs != NULL)
                    MEM_Free(geo->collisionPtr->aaBoxes[i].chunkElems[j].triangleChunkElemIDs);
            }
            MEM_Free(geo->collisionPtr->aaBoxes[i].chunkElems);
        }
        
        MEM_Free(geo->collisionPtr->aaBoxes);

        if (geo->collisionPtr->faceTagInts)
            MEM_Free(geo->collisionPtr->faceTagInts);

        MEM_Free(geo->collisionPtr);
    }

    PC_FreeABackCache(geo);
    
    if (geo->triangleChunkCount)
    {
        for (i = 0; i < geo->triangleChunkCount; i++) {
            GEO_FreeElementContent(&geo->triangleChunks[i]);
        }

        MEM_FreeAlign(geo->triangleChunks);
        geo->triangleChunkCount = 0;
    }

    if (geo->spritesNum)
    {
        for (i = 0; i < geo->spritesNum; i++) {
            GEO_FreeSpriteElementContent(&geo->spritesPtr[i]);
        }
        MEM_FreeAlign(geo->spritesPtr);
        geo->spritesNum = 0;
    }

    geo->unkFlags = 0;    
}

void GEO_Free(struct sGroGeneric* geo)
{
    if (geo && geo->intf->id == 1 && geo->usageCount <= 0)
    {
        GEO_FreeContent((tGEOGRO*)geo);
        LOA_DeleteAddress((int)geo);
        MEM_FreeAlign(geo);
    }
}

int GEO_l_GetNumberOfTriangles(struct sGEOGRO* geo)
{
    unsigned int i;
    int ret = 0;

    if (geo == NULL) return 0;

    for (i = 0; i < geo->triangleChunkCount; i++) {
        ret += geo->triangleChunks[i].triangleNum;
    }

    return ret;
}

void GEO_Render_InitRLI(tGdi_DisplayData* dd, tGEOGRO* geo, tGameObjectVisualObject* gaovo)
{
    unsigned int i;
    unsigned int* currDestColor;
    int* currSrcColor;
    int rliCalculatedColor;
    

    currDestColor = (unsigned int*)dd->bigBuffer->data1LightColors;

    if (gaovo->RLIVertexColors)
    {
        currSrcColor = gaovo->RLIVertexColors + 1; // 1st one is a count, skipping it
    }
    else if (geo->vertexColors) {
        currSrcColor = geo->vertexColors + 1;      // 1st one is a count, skipping it
    }
    else
    {
        currSrcColor = NULL;
    }

    dd->RLIPtr = currSrcColor;

    if (currSrcColor == NULL || (dd->drawMaskAllowedStuffCurrent & 0x40) == 0)
    {
        memset(currDestColor, 0, sizeof(uint32_t) * geo->vertexNum);
        dd->bigBuffer->flags &= ~4;
        return;
    }

    if ((dd->flags & 0x100) != 0)
    {
        dd->flags &= ~0x100;
        if (dd->MRMPtr)
        {         
            for (i = 0; i < geo->vertexNum; i++) 
            {
                currDestColor[i] = LIGHT_ul_Interpol2Colors(currSrcColor[i], dd->mrmColors[dd->MRMPtr[i]], dd->mrmColorFactor);
            }
                       
            dd->bigBuffer->flags &= ~4;
            return;
        }
        
        // FIXME: broken codepath, mrmColors isn't initialized, and original code increments dd->mrmColors pointer

        for (i = 0; i < geo->vertexNum; i++) 
        {
            currDestColor[i] = LIGHT_ul_Interpol2Colors(currSrcColor[i], dd->mrmColors[i], dd->mrmColorFactor);
        }
        dd->bigBuffer->flags &= ~4;
    }
    else if ((dd->drawMaskAllowedStuffCurrent & 0x2000000) != 0 || dd->RLIFactor == 0.0f || dd->RLIDestColor == 0x00000000)
    {
        if (dd->MRMPtr)
        {
            for (i = 0; i < geo->vertexNum; i++)
            {
                currDestColor[i] = currSrcColor[dd->MRMPtr[i]];
            }
            dd->bigBuffer->flags &= ~4;
        }
        else {
            memcpy(currDestColor, currSrcColor, sizeof(uint32_t) * geo->vertexNum);
            dd->bigBuffer->flags &= ~4;
        }        
    }
    else
    {
        rliCalculatedColor = COLOR_ul_Mul(dd->RLIDestColor, dd->RLIFactor);
        if (dd->MRMPtr)
        {
           
            for (i = 0; i < geo->vertexNum; i++) 
            {            
                currDestColor[i] = currSrcColor[dd->MRMPtr[i]];
                LIGHT_SubColor(&currDestColor[i], rliCalculatedColor);                
            }
            dd->bigBuffer->flags &= ~4;
        }
        else
        {            
            for (i = 0; i < geo->vertexNum; i++) 
            {            
                currDestColor[i] = currSrcColor[i];
                LIGHT_SubColor(&currDestColor[i], rliCalculatedColor);
            }
            dd->bigBuffer->flags &= ~4;
        }
    }
}

void GEO_SKN_SymmetrizeMatrix(Mtx4x4* mtx)
{
    mtx->v1.x = -mtx->v1.x;
    mtx->v2.x = -mtx->v2.x;
    mtx->v3.x = -mtx->v3.x;
    mtx->Pos.x = -mtx->Pos.x;
}

void GEO_SKN_MulMatrixMatrixSym(Mtx4x4* dest, Mtx4x4* mtx1, Mtx4x4* mtx2)
{
    GEO_SKN_SymmetrizeMatrix(mtx1);
    MATH_MulMatrixMatrix(dest, mtx1, mtx2);
    GEO_SKN_SymmetrizeMatrix(mtx1);
}

void GEO_SKN_Compute4Display(tGameObjectMain* gao, tGEOGRO* geogro)
{
    int i;
    int idx;
    Vec3* destVertices;
    Vec3* srcVertices;
    int skinningNum;
    int* weightsAndIndices;
    tMultiPartGameObjectLinks* gaoLink;
    Mtx4x4* selectedMtx;
    tModifier* currMdf;
    Vec3 currTransformedVertex;
    Mtx4x4 transformMtx;
    Mtx4x4 tmpMtx;
    Mtx4x4 gaoInvMtx;

    if (geogro->skinning && (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x20000) != 0 && (gao->flags & 0x200000) != 0)
    {
        QueryPerformanceCounter(&PerfCounterStruct[2]);

        if ((gpst_DX_stDD->flags & 0x400) != 0)
        {
            destVertices = (Vec3*)gpst_DX_stDD->bigBuffer->data5;
            srcVertices = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        }
        else
        {
            srcVertices = geogro->vertices;
            destVertices = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        }

        memset(destVertices, 0, sizeof(Vec3) * geogro->vertexNum);

        for (idx = geogro->skinning->num; idx > 0; idx--)
        {
            skinningNum = geogro->skinning->ptr[idx - 1].ptr->num;
            weightsAndIndices = (int*)geogro->skinning->ptr[idx - 1].ptr->weightsAndIndicesPtr;

            if (geogro->intf->id == 1 && geogro->mrm)
            {
                if (!geogro->skinning->ptr[idx - 1].ptr->num)
                    continue;

                while (*(uint16_t*)&weightsAndIndices[skinningNum - 1] > geogro->vertexNum)
                {
                    --skinningNum;
                    if (!skinningNum)
                        break;
                }
            }

            if (!skinningNum)
                continue;

            if (gao->basic->multiPartData->elemNum <= geogro->skinning->ptr[idx - 1].ptr->multiPartElemID) // bug corner case
            {
                MATH_SetIdentityMatrix(&transformMtx);
            }
            else {
                if ((gao->flags & 0x1000000) != 0)
                {
                    gaoLink = (tMultiPartGameObjectLinks*)(gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices);
                    selectedMtx = gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].mtxPtr;

                    if (gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].mtxIdx == -1 ||
                        gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao != NULL &&
                        (gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao->flags & 0x1000000) != 0)
                    {
                        if (gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao)
                        {
                            if ((gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao->flags & 0x2000) != 0)
                            {
                                if (gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao->extended)
                                {
                                    for (currMdf = gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao->extended->mdf; currMdf; currMdf = currMdf->nextElem)
                                    {
                                        if (currMdf->modifierIntf->mdfId == 24) //lazy
                                        {
                                            if ((currMdf->modifierFlags & 0xa000000) == 0 && (currMdf->modifierFlags & 0x1000000) != 0)
                                                GAO_ModifierLazy_Apply(currMdf, NULL);
                                        }
                                    }
                                }
                            }
                            MATH_InvertMatrix(&gaoInvMtx, gao->matrices);
                            MATH_MulMatrixMatrix(&tmpMtx, gaoLink[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID].gao->matrices, &gaoInvMtx);
                            selectedMtx = &tmpMtx;
                        }
                        else
                        {
                            MATH_SetIdentityMatrix(&tmpMtx);
                            selectedMtx = &tmpMtx;
                        }
                    }
                }
                else
                {
                    selectedMtx = &(((Mtx4x4*)(gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[geogro->skinning->ptr[idx - 1].ptr->multiPartElemID]);
                }
                if (!selectedMtx)
                {
                    MATH_SetIdentityMatrix(&transformMtx);
                }
                else {

                    if ((((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->objDrawMask & 0x8000) == 0)
                        GEO_SKN_MulMatrixMatrixSym(&transformMtx, &geogro->skinning->ptr[idx - 1].ptr->mtx, selectedMtx);
                    else
                        MATH_MulMatrixMatrix(&transformMtx, &geogro->skinning->ptr[idx - 1].ptr->mtx, selectedMtx);
                }
            }

            for (i = 0; i < skinningNum; i++)
            {
                MATH_TransformVertex(&currTransformedVertex, &transformMtx, &srcVertices[*(uint16_t*)&weightsAndIndices[i]]);
                destVertices[*(uint16_t*)&weightsAndIndices[i]].x = currTransformedVertex.x * *(float*)&weightsAndIndices[i] + destVertices[*(uint16_t*)&weightsAndIndices[i]].x;
                destVertices[*(uint16_t*)&weightsAndIndices[i]].y = currTransformedVertex.y * *(float*)&weightsAndIndices[i] + destVertices[*(uint16_t*)&weightsAndIndices[i]].y;
                destVertices[*(uint16_t*)&weightsAndIndices[i]].z = currTransformedVertex.z * *(float*)&weightsAndIndices[i] + destVertices[*(uint16_t*)&weightsAndIndices[i]].z;
            }
        }

        if ((gpst_DX_stDD->flags & 0x400) != 0)
        {
            memcpy(gpst_DX_stDD->bigBuffer->data4, gpst_DX_stDD->bigBuffer->data5, sizeof(Vec3) * geogro->vertexNum);
        }

        gpst_DX_stDD->flags |= 0x400;
        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;

        PRO_QPC_RecordTimeDecRef(2);
    }
}

struct sGEOMAT* GEO_GetMaterial(struct sGEOMAT* mat, int idx)
{
    tGEOMAT* ret = mat;

    if (mat == NULL)
        return &MAT_gst_DefaultSingleMaterial;

    if (mat->intf->id == 4)
    {
        if (idx > mat->multiMaterialNumIfType4ThenAmbiantColor - 1)
            idx = mat->multiMaterialNumIfType4ThenAmbiantColor - 1;

        if ((ret = mat->multiMaterialPtrIfType4ThenDiffuseColor[idx]) == NULL)
            return &MAT_gst_DefaultSingleMaterial;
    }
    return ret;
}

void GEO_Render(tGameObjectMain* gao)
{
    tGameObjectVisualObject* geovo;
    tGEOGRO* geogro;
    char matValidityMask;    
    tGameObjectMain* aiCBGao; 
    float distTripled; 
    tGeoTriangleChunks* currTriangleChunk;
    tGeoSprites* currSpritePtr;
    int renderingFlags, renderingFlags2; 
    int flags;
   
    /*
    if (gao == (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x620031d4)) {
        DBG_OutputDebugString("rliptr(geo): %p\n", gpst_DX_stDD->RLIPtr);
    }
    */

    QueryPerformanceCounter(&PerfCounterStruct[23]);
    
    geovo = gao->basic->actorObjectOrVisualObject;
    geogro = (tGEOGRO*)(geovo->GROGEOOrSomethingElse);
    gpst_DX_stDD->GROGEO = (tGEOGRO*)(geovo->GROGEOOrSomethingElse);
    gpst_DX_stDD->flags &= ~0x1000;
    matValidityMask = MAT_GetValidityMASK((tGEOMAT*)(geovo->GROMAT), -1, geogro);

    if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x20000000) == 0)
        matValidityMask |= 0xc0;

    if ((gpst_DX_stDD->flags & 8) != 0
        || (matValidityMask & 0xc0) == 0
        || (gpst_DX_stDD->flags & 4) != 0 // &4 = zlistbõl hívott draw, nyilván ha ez teljesül a ZlistObjectes függvény nem hívódik meg
        || (SOFT_AddCurrentObjectInZList(gpst_DX_stDD, gao), (matValidityMask & 0x40) == 0))
    {
        if (gao->extended)
        {
            if (gao->extended->AIInstance)
            {
                aiCBGao = gao;
                AI_C_Callback = TRUE;
                AI_ExecCallback(aiCBGao, 0);
                AI_C_Callback = FALSE;
            }
            else if ((gao->flags & 1) != 0 && (aiCBGao = ANI_pst_GetReferenceInit(gao)) != NULL
                && aiCBGao->extended != NULL && aiCBGao->extended->AIInstance != NULL) {
                AI_C_Callback = TRUE;
                AI_ExecCallback(aiCBGao, 0);
                AI_C_Callback = FALSE;
            }
        }

        gpst_DX_stDD->MRMPtr = NULL;

        if (geogro && geogro->intf->id == 1 && geogro->mrm)
        {
            distTripled = (1.0f - gao->distance * (1.0f/256.0f)) * ((1.0f - gao->distance * (1.0f / 256.0f)) * (1.0f - gao->distance * (1.0f / 256.0f)));
            GEO_MRM_SetNumberOfPoints_Percent(geogro, (1.0f - distTripled * distTripled));
        }
        
        gpst_DX_stDD->flags &= ~0x400;
        gpst_DX_stDD->verticesCurrent = geogro->vertices;

        if (geogro->vertices != NULL)
        {
            if ((gao->flags & 0x2000) != 0 && gao->extended != NULL && gao->extended->mdf != NULL)
                MDF_ApplyAll(gao);          

            if (geogro->skinning)
                GEO_SKN_Compute4Display(gao, geogro);

            if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x800000) == 0 && (gpst_DX_stDD->flags & 0x400) != 0)
                GEO_SKN_ComputeNormals(geogro, gpst_DX_stDD->verticesCurrent);

            QueryPerformanceCounter(&PerfCounterStruct[8]);            
            if ((matValidityMask & 0x13) != 0)
            {
                GEO_Render_InitRLI(gpst_DX_stDD, geogro, geovo);
                if ((matValidityMask & 3) != 0 && (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x8000000) != 0)
                {
                    LIGHT_SendObjectToLight(&gpst_DX_stDD->lightList, gao);
                    gpst_DX_stDD->flags |= 0x2000;
                }
                LIGHT_ComputeVertexColor(geogro->vertexNum);
            }
            PRO_QPC_RecordTimeDecRef(8);

            renderingFlags = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
            if ((renderingFlags & 0x100000) != 0)
            {
                if ((gpst_DX_stDD->flags & 0x200) == 0 || (gpst_DX_stDD->flags & 4) != 0)
                {
                    for (currTriangleChunk = geogro->triangleChunks; currTriangleChunk < &geogro->triangleChunks[geogro->triangleChunkCount]; currTriangleChunk++)
                    {
                        PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[27]);
                        MAT_DrawIndexedTriangle(geogro, (tGEOMAT*)(geovo->GROMAT), currTriangleChunk);
                        PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[27]);

                        gpst_DX_stDD->drawMaskAllowedStuffCurrent = renderingFlags;
                    }
                    if (geogro->spritesNum)
                    {
                        renderingFlags2 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
                     
                        for (currSpritePtr = geogro->spritesPtr; currSpritePtr < &geogro->spritesPtr[geogro->spritesNum]; currSpritePtr++) {
                            MAT_DrawIndexedSprites(geogro, (tGEOMAT*)(geovo->GROMAT), currSpritePtr);
                            gpst_DX_stDD->drawMaskAllowedStuffCurrent = renderingFlags2;
                        }
                    }
                }
                else
                {
                    GEO_Render_TransparentTriangles(gpst_DX_stDD, geogro, geovo);
                }
            }

            if (gpst_DX_stDD->extraMaterialToGeo)
            {
                renderingFlags = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
                flags = gpst_DX_stDD->flags;
                
                if (!MAT_IsMaterialTransparent(gpst_DX_stDD->extraMaterialToGeo, -1, renderingFlags, NULL))
                {
                    gpst_DX_stDD->flags &= ~4;
                }
                else 
                {
                    gpst_DX_stDD->flags |= 4;
                }
                
                for (currTriangleChunk = geogro->triangleChunks; currTriangleChunk < &geogro->triangleChunks[geogro->triangleChunkCount]; currTriangleChunk++) {
                    MAT_DrawIndexedTriangle(geogro, gpst_DX_stDD->extraMaterialToGeo, currTriangleChunk);
                }
                
                gpst_DX_stDD->drawMaskAllowedStuffCurrent = renderingFlags;
                gpst_DX_stDD->flags = flags;
            }

            if (gpst_DX_stDD->shadowStruct && gpst_DX_stDD->shadowStruct->maxShadowNum)
            {
                PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[44]);
                SDW_DisplayShadowsOnCurrentObject(geogro);
                PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[44]);
            }

            if (p_FirstActiveProtex)
                PROTEX_Disturb(gpst_DX_stDD->currTargetGao, geogro);

            if (gao->extended)
            {
                if (gao->extended->mdf)
                    MDF_UnApplyAll(gao);

                if (gao->extended)
                {
                    if (gao->extended->AIInstance)
                    {
                        AI_ExecCallback(gao, 1);
                    }
                    else if ((gao->flags & 1) != 0 && (aiCBGao = ANI_pst_GetReferenceInit(gao)) != NULL
                        && aiCBGao->extended != NULL && aiCBGao->extended->AIInstance != NULL)
                    {
                        AI_ExecCallback(aiCBGao, 1);
                    }
                }
            }
            PRO_QPC_RecordTimeDecRef(23);            
        }
    }
}

int GEO_l_HasSomethingToRender(struct sGameObjectVisualObject* gaovo, struct sGroGeneric** gro)
{
    *gro = (tGroGeneric*)gaovo->GROGEOOrSomethingElse;
    return gpst_DX_stDD->drawMaskAllowedStuffCurrent & 8;
}

int GEO_SKN_Load(char* readPtr, tGEOGRO* geogro)
{
    int i, j;
    char* currReadPtr = readPtr;
    tGeoSkn* skn;
    int* currSknWeightAndIndex;
    
    skn = (tGeoSkn*)MEM_p_Alloc(sizeof(tGeoSkn));
    LOA_ul_FileTypeSize_26 += sizeof(tGeoSkn);
    
    skn->s1 = LOA_ReadShort(&currReadPtr, 0, 1);
    skn->num = LOA_ReadShort(&currReadPtr, 0, 1);
    skn->ptr = (tGeoSknSub1*)MEM_p_Alloc(sizeof(tGeoSknSub1) * skn->num);
    LOA_ul_FileTypeSize_26 += sizeof(tGeoSknSub1) * skn->num;

    for (i = skn->num - 1; i >= 0; i--)
    {
        skn->ptr[i].ptr = (tGeoSknElem*)MEM_p_AllocAligned(sizeof(tGeoSknElem), 16);
        LOA_ul_FileTypeSize_26 += sizeof(tGeoSknElem);

        skn->ptr[i].ptr->multiPartElemID = LOA_ReadShort(&currReadPtr, 0, 1);
        skn->ptr[i].ptr->num = LOA_ReadShort(&currReadPtr, 0, 1);

        LOA_ReadMatrix(&currReadPtr, &skn->ptr[i].ptr->mtx, 1);

        // skn matrix cleanup
        /*
        if ((skn->ptr[i].ptr->mtx.mtxflag & 0x8) == 0) {
            skn->ptr[i].ptr->mtx._m14 = 0.0f;
            skn->ptr[i].ptr->mtx._m24 = 0.0f;
            skn->ptr[i].ptr->mtx._m34 = 0.0f;
            skn->ptr[i].ptr->mtx.w = 1.0f;
        }
        */

        if (skn->ptr[i].ptr->num)
        {
            skn->ptr[i].ptr->weightsAndIndicesPtr = (int*)MEM_p_AllocAligned(sizeof(int32_t) * (skn->ptr[i].ptr->num + 4), 16); // 4 pcs for the last one, see below [0,0,0,0]
            LOA_ul_FileTypeSize_26 += sizeof(int32_t) * (skn->ptr[i].ptr->num + 4);

            for (currSknWeightAndIndex = skn->ptr[i].ptr->weightsAndIndicesPtr, j = skn->ptr[i].ptr->num; j; j--)
                *currSknWeightAndIndex++ = LOA_ReadLong(&currReadPtr, 0, 1);

            currSknWeightAndIndex[0] = 0;
            currSknWeightAndIndex[1] = 0;
            currSknWeightAndIndex[2] = 0;
            currSknWeightAndIndex[3] = 0;
        }
    }

    geogro->skinning = skn;
    return currReadPtr - readPtr;
}

struct sGroGeneric* GEO_p_CreateFromBuffer(struct sGroGeneric* gro, char** readPtr, struct sWorld* world)
{
    unsigned int i, j;
    int k;
    tGEOGRO* geogro;
    uint32_t flags;
    int triangleStripElemNum;
    int triangleStripActBlockStartElemIdx;
    int triangleStripElemIdx, maxTriangleStripElemIdx;
    int triangleStripElemNumAlreadyCopied;
    unsigned int vertexColorNum, vertexColorNumLimited;
    char* currReadPtr = *readPtr;
    
    geogro = GEO_pst_Create(0, 0, 0, 0);
    geogro->vertexNum = LOA_ReadLong(&currReadPtr, 0, 1);
    geogro->vertexColors = (int*)LOA_ReadLong(&currReadPtr, 0, 1);
    geogro->uvNum = LOA_ReadLong(&currReadPtr, 0, 1);
    geogro->triangleChunkCount = LOA_ReadLong(&currReadPtr, 0, 1);
    
    LOA_ReadLong(&currReadPtr, 0, 2);

    vertexColorNum = (unsigned int)geogro->vertexColors;
    if (vertexColorNum)
        geogro->vertexColors = (int*)geogro->vertexNum;

    geogro->mrm = (tGeoMRM*)LOA_ReadLong(&currReadPtr, 0, 1);
    flags = LOA_ReadLong(&currReadPtr, 0, 1);
    geogro->skinning = NULL;

    // skinning
    if ((flags & 0xc0de2002) == 0xc0de2002)
    {
        currReadPtr += GEO_SKN_Load(currReadPtr, geogro);
    }

    // collision
    if ((flags & 1) != 0)
    {
        geogro->collisionPtr = (tCobOK3*)MEM_p_Alloc(sizeof(tCobOK3));
        
        geogro->collisionPtr->faceTagInts = NULL;
        geogro->collisionPtr->num = LOA_ReadLong(&currReadPtr, 0, 1);
        geogro->collisionPtr->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(sizeof(tCobOK3AABBoxElem) * geogro->collisionPtr->num);

        for (i = 0; i < geogro->collisionPtr->num; i++)
        {
            geogro->collisionPtr->aaBoxes[i].num = LOA_ReadLong(&currReadPtr, 0, 1);
            LOA_ReadVector(&currReadPtr, &geogro->collisionPtr->aaBoxes[i].vec2, 1);
            LOA_ReadVector(&currReadPtr, &geogro->collisionPtr->aaBoxes[i].vec1, 1);

            geogro->collisionPtr->aaBoxes[i].chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(sizeof(tCobOK3AABBoxElemChunkElem) * geogro->collisionPtr->aaBoxes[i].num);
            
            for (j = 0; j < geogro->collisionPtr->aaBoxes[i].num; j++)
            {
                geogro->collisionPtr->aaBoxes[i].chunkElems[j].triangleChunkID = LOA_ReadShort(&currReadPtr, 0, 1);
                geogro->collisionPtr->aaBoxes[i].chunkElems[j].num = LOA_ReadShort(&currReadPtr, 0, 1);
                geogro->collisionPtr->aaBoxes[i].chunkElems[j].triangleChunkElemIDs = (uint16_t*)MEM_p_Alloc(2 * geogro->collisionPtr->aaBoxes[i].chunkElems[j].num);

                for (k = 0; k < geogro->collisionPtr->aaBoxes[i].chunkElems[j].num; k++) {
                    geogro->collisionPtr->aaBoxes[i].chunkElems[j].triangleChunkElemIDs[k] = LOA_ReadShort(&currReadPtr, 0, 1);
                }
            }
        }
    }

    GEO_AllocContent(geogro);

    // vertices
    for (i = 0; i < geogro->vertexNum; i++)
    {
        LOA_ReadVector(&currReadPtr, &geogro->vertices[i], 1);
    }
    
    // unknown
    for (i = 0; i < geogro->vertexNum; i++) {
        LOA_ReadVector(&currReadPtr, 0, 2);
    }

    // mrm (?)
    if (geogro->mrm)
    {
        for (i = 0; i < geogro->vertexNum; i++)
        {
            geogro->mrm->shorts[i] = LOA_ReadShort(&currReadPtr, 0, 1);
        }

        if (geogro->mrm->ptr)
        {
            for (i = 0; i < geogro->vertexNum; i++)
            {
                geogro->mrm->ptr[i] = LOA_ReadShort(&currReadPtr, 0, 1);
            }
        }
        geogro->mrm->endNum = geogro->vertexNum;
        geogro->mrm->b = LOA_ReadLong(&currReadPtr, 0, 1);

        for (i = 0; i < 8; i++)
            geogro->mrm->f[i] = LOA_ReadFloat(&currReadPtr, 0, 1);
    }

    // vertexcolors (vertexpaint)
    vertexColorNumLimited = vertexColorNum;
    if (vertexColorNumLimited)
    {
        if (vertexColorNumLimited > geogro->vertexNum)
            vertexColorNumLimited = geogro->vertexNum;

        for (i = 0; i < vertexColorNumLimited; i++) {
            geogro->vertexColors[i + 1] = LOA_ReadLong(&currReadPtr, 0, 1);
        }        
    }

    // UVs
    for (i = 0; i < geogro->uvNum; i++)
    {
        geogro->UVs[i].u = LOA_ReadFloat(&currReadPtr, 0, 1);
        geogro->UVs[i].v = LOA_ReadFloat(&currReadPtr, 0, 1);
    }

    // triangle chunks
    for (i = 0; i < geogro->triangleChunkCount; i++)
    {
        geogro->triangleChunks[i].triangleNum = LOA_ReadLong(&currReadPtr, 0, 1);
        geogro->triangleChunks[i].materialNum = LOA_ReadLong(&currReadPtr, 0, 1);
        geogro->triangleChunks[i].MRMUVIndexElems = (tGeoMRMUVIndexElem*)LOA_ReadLong(&currReadPtr, 0, 1);
        geogro->triangleChunks[i].MRMUVIndicesNum = LOA_ReadLong(&currReadPtr, 0, 1);

        for (j = 0; j < 6; j++) {
            LOA_ReadLong(&currReadPtr, 0, 2);
        }        
    }

    // triangle chunk elems
    for (i = 0; i < geogro->triangleChunkCount; i++) {
        GEO_AllocElementContent(&geogro->triangleChunks[i]);

        for (j = 0; j < geogro->triangleChunks[i].triangleNum; j++) {
            LOA_ReadShort(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].v1, 1);
            LOA_ReadShort(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].v2, 1);
            LOA_ReadShort(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].v3, 1);
            LOA_ReadShort(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].uv1, 1);
            LOA_ReadShort(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].uv2, 1);
            LOA_ReadShort(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].uv3, 1);
            LOA_ReadLong(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].mrmSmoothingGroup, 1);
            LOA_ReadLong(&currReadPtr, &geogro->triangleChunks[i].triangleChunkElems[j].unk2, 1);
            geogro->triangleChunks[i].triangleChunkElems[j].unk2 &= 0x7fffffff;
        }

        if (geogro->triangleChunks[i].MRMUVIndexElems != NULL)
        {
            geogro->triangleChunks[i].MRMUVIndexElems->UVStartOffset = LOA_ReadLong(&currReadPtr, 0, 1);
            geogro->triangleChunks[i].MRMUVIndexElems->i2 = LOA_ReadLong(&currReadPtr, 0, 1);
            geogro->triangleChunks[i].MRMUVIndexElems->i3 = 0;
            LOA_ReadLong(&currReadPtr, 0, 2);
        }

        if (geogro->triangleChunks[i].MRMUVIndicesNum)
        {
            for (j = 0; j < geogro->triangleChunks[i].MRMUVIndicesNum; j++)
            {
                geogro->triangleChunks[i].MRMUVIndices[j] = LOA_ReadShort(&currReadPtr, 0, 1);
            }

            geogro->triangleChunks[i].MRMUVIndices[geogro->triangleChunks[i].MRMUVIndicesNum] = -1;
        }

        for (j = 0; j < geogro->triangleChunks[i].triangleNum; j++)
        {
            if (geogro->triangleChunks[i].triangleChunkElems[j].v1 >= geogro->vertexNum)
                geogro->triangleChunks[i].triangleChunkElems[j].v1 = geogro->vertexNum - 1;

            if (geogro->uvNum)
            {
                if (geogro->triangleChunks[i].triangleChunkElems[j].uv1 >= geogro->uvNum)
                    geogro->triangleChunks[i].triangleChunkElems[j].uv1 = geogro->uvNum - 1;
            }

            if (geogro->triangleChunks[i].triangleChunkElems[j].v2 >= geogro->vertexNum)
                geogro->triangleChunks[i].triangleChunkElems[j].v2 = geogro->vertexNum - 1;

            if (geogro->uvNum)
            {
                if (geogro->triangleChunks[i].triangleChunkElems[j].uv2 >= geogro->uvNum)
                    geogro->triangleChunks[i].triangleChunkElems[j].uv2 = geogro->uvNum - 1;
            }

            if (geogro->triangleChunks[i].triangleChunkElems[j].v3 >= geogro->vertexNum)
                geogro->triangleChunks[i].triangleChunkElems[j].v3 = geogro->vertexNum - 1;

            if (geogro->uvNum)
            {
                if (geogro->triangleChunks[i].triangleChunkElems[j].uv3 >= geogro->uvNum)
                    geogro->triangleChunks[i].triangleChunkElems[j].uv3 = geogro->uvNum - 1;
            }           
        }
    }

    geogro->unkFlags = LOA_ReadLong(&currReadPtr, 0, 1);
    
    // striangle stripes for speedup (static models)
    // triangleStripBlock =>
    // D3D_CreateIndexAndVertexBuffForElemsIfSuitable refers to it
    // and D3D_l_DrawElementIndexedTriangles does it too
    if ((geogro->unkFlags & 1) != 0)
    {
        for (i = 0; i < geogro->triangleChunkCount; i++) {
            geogro->triangleChunks[i].triangleStripBlock = (tGeoWholeTriangleStripBlock*)MEM_GEO_p_Alloc(sizeof(tGeoWholeTriangleStripBlock));
            memset(geogro->triangleChunks[i].triangleStripBlock, 0, sizeof(tGeoWholeTriangleStripBlock));

            geogro->triangleChunks[i].triangleStripBlock->vertexColorArraySecond = LOA_ReadLong(&currReadPtr, 0, 1);
            geogro->triangleChunks[i].triangleStripBlock->triangleStripNum = LOA_ReadLong(&currReadPtr, 0, 1);
            geogro->triangleChunks[i].triangleStripBlock->unk2 = 0;
            geogro->triangleChunks[i].triangleStripBlock->unk1 = 0;
            geogro->triangleChunks[i].triangleStripBlock->unk3 = 0;
            
            geogro->triangleChunks[i].triangleStripBlock->triangleStrips = 
                (tGeoTriangleStrip*)MEM_GEO_p_Alloc(sizeof(tGeoTriangleStrip) * geogro->triangleChunks[i].triangleStripBlock->triangleStripNum);
            memset(geogro->triangleChunks[i].triangleStripBlock->triangleStrips, 0, sizeof(tGeoTriangleStrip)* geogro->triangleChunks[i].triangleStripBlock->triangleStripNum);
                                    
            for (j = 0, triangleStripElemIdx = 0; j < geogro->triangleChunks[i].triangleStripBlock->triangleStripNum; j++)
            {
                triangleStripElemNum = LOA_ReadLong(&currReadPtr, 0, 1);
                geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElemNum = triangleStripElemNum;
                triangleStripElemIdx += (triangleStripElemNum & 0x7fffffff);

                if (triangleStripElemNum < 0)
                {
                    geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElemNum = triangleStripElemNum & 0x7FFFFFFF;
                }
                else
                {
                    for (k = 0; k < triangleStripElemNum; k++) {
                        LOA_ReadLong(&currReadPtr, 0, 2);
                    }
                }
                // temporary allocation
                geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElems = (tGeoTriangleStripElem*)MEM_p_AllocFromEnd(sizeof(tGeoTriangleStripElem) * geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElemNum);
                if (geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElemNum != 0)
                {
                    for (k = 0; k < geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElemNum; k++) {
                        geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElems[k].triangleIdx = LOA_ReadShort(&currReadPtr, 0, 1);
                        geogro->triangleChunks[i].triangleStripBlock->triangleStrips[j].triangleStripElems[k].uvIdx = LOA_ReadShort(&currReadPtr, 0, 1);
                    }
                }
            }
            
            // concatenate every trianglestrip block data into one big buffer for memory saving (defragmentation overhead)
            geogro->triangleChunks[i].triangleStripBlock->triangleStripElemsContinousPool = (tGeoTriangleStripElem*)MEM_GEO_p_Alloc(sizeof(tGeoTriangleStripElem) * triangleStripElemIdx);
            triangleStripElemNumAlreadyCopied = 0;
            maxTriangleStripElemIdx = triangleStripElemIdx;

            for (k = geogro->triangleChunks[i].triangleStripBlock->triangleStripNum - 1; k >= 0; k--)
            {
                triangleStripActBlockStartElemIdx = maxTriangleStripElemIdx - 
                    (geogro->triangleChunks[i].triangleStripBlock->triangleStrips[k].triangleStripElemNum + triangleStripElemNumAlreadyCopied);
                
                memcpy(&geogro->triangleChunks[i].triangleStripBlock->triangleStripElemsContinousPool[triangleStripActBlockStartElemIdx],
                    geogro->triangleChunks[i].triangleStripBlock->triangleStrips[k].triangleStripElems,
                    sizeof(tGeoTriangleStripElem) * geogro->triangleChunks[i].triangleStripBlock->triangleStrips[k].triangleStripElemNum);
                
                triangleStripElemNumAlreadyCopied += geogro->triangleChunks[i].triangleStripBlock->triangleStrips[k].triangleStripElemNum;
                
                MEM_FreeFromEnd(geogro->triangleChunks[i].triangleStripBlock->triangleStrips[k].triangleStripElems);

                geogro->triangleChunks[i].triangleStripBlock->triangleStrips[k].triangleStripElems = 
                    &(geogro->triangleChunks[i].triangleStripBlock->triangleStripElemsContinousPool[triangleStripActBlockStartElemIdx]);
            }
        }        
    }
    else
    {
        for (i = 0; i < geogro->triangleChunkCount; i++)
            geogro->triangleChunks[i].triangleStripBlock = NULL;
    }

    // sprites
    geogro->spritesNum = LOA_ReadLong(&currReadPtr, 0, 1);
    if (!geogro->spritesNum || (geogro->spritesNum & 0xffffff00) != 0)
    {
        geogro->spritesNum = 0;
    }
    else
    {
        LOA_ReadLong(&currReadPtr, 0, 1);

        geogro->spritesPtr = (tGeoSprites*)MEM_GEO_p_Alloc(sizeof(tGeoSprites) * geogro->spritesNum);
        memset(geogro->spritesPtr, 0, sizeof(tGeoSprites) * geogro->spritesNum);

        for (k = geogro->spritesNum - 1; k >= 0; k--) {
            geogro->spritesPtr[k].globalRatio = LOA_ReadFloat(&currReadPtr, 0, 1);
            geogro->spritesPtr[k].globalSize = LOA_ReadFloat(&currReadPtr, 0, 1);
            geogro->spritesPtr[k].matIdx = LOA_ReadLong(&currReadPtr, 0, 1);
            geogro->spritesPtr[k].num = LOA_ReadLong(&currReadPtr, 0, 1);
            
            LOA_ReadLong(&currReadPtr, 0, 2);
            
            geogro->spritesPtr[k].spriteElems = NULL;

            if (geogro->spritesPtr[k].num) {
                geogro->spritesPtr[k].spriteElems = (tGeoSpriteElem*)MEM_GEO_p_Alloc(sizeof(tGeoSpriteElem) * geogro->spritesPtr[i].num);
            }

            for (j = 0; j < geogro->spritesPtr[k].num; j++) {
                geogro->spritesPtr[k].spriteElems[j].idx = LOA_ReadShort(&currReadPtr, 0, 1);
                geogro->spritesPtr[k].spriteElems[j].dummy = LOA_ReadShort(&currReadPtr, 0, 1);
            }            
        }        
    }

    *readPtr = currReadPtr;
    
    if (GEO_b_IsInFix)
        GEO_ComputeNormals(geogro);
    
    return (tGroGeneric*)geogro;
}

struct sGroGeneric* GEO_p_Duplicate(struct sGroGeneric* gro) {
    return NULL;
}

void GEO_Init()
{
    GRO_gast_Interface[1].CreateFromBuffer = GEO_p_CreateFromBuffer;
    GRO_gast_Interface[1].Duplicate = GEO_p_Duplicate;
    GRO_gast_Interface[1].Destroy = GEO_Free;
    GRO_gast_Interface[1].HasSomethingToRender = GEO_l_HasSomethingToRender;
    GRO_gast_Interface[1].Render = GEO_Render;
}