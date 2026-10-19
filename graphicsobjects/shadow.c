#include "../memory.h"
#include "gro_common.h"
#include "../win/wingdi.h"
#include "../modifiers/shadowmodifier.h"
#include "shadow.h"

float SDW_RDR_XS = 64.0f;
float SDW_RDR_YS = 64.0f;
int SDW_RDR_SHIFT = 2;
int SDW_RDR_SHIFTY = 2;
int SDW_RDR_AND = 3;

tShadowStruct* SDW_Create()
{
    tShadowStruct* ses = (tShadowStruct*)MEM_p_Alloc(sizeof(tShadowStruct));

    ses->maxShadowNum = 32;
    ses->actShadowNum = 0;
    ses->shadowElems = (tShadowElemStruct*)MEM_p_Alloc(ses->maxShadowNum * sizeof(tShadowElemStruct));

    memset(ses->shadowElems, 0, ses->maxShadowNum * sizeof(tShadowElemStruct));

    return ses;
}

int SDW_DispatchTextures()
{
    int shiftNum;
    int shiftNumY;
    float xs;
    float ys;    
    unsigned int i;

    if (!SDW_TextNum) return 0;

    SDW_RDR_XS = 256.0f;
    SDW_RDR_YS = 128.0f;
    SDW_RDR_SHIFT = 0;
    SDW_RDR_SHIFTY = 0;

    shiftNum = SDW_RDR_SHIFT;
    shiftNumY = SDW_RDR_SHIFTY;
    xs = SDW_RDR_XS;
    ys = SDW_RDR_YS;

    if (SDW_TextNum == 1)
    {
        SDW_RDR_AND = 0;
        return SDW_TextNum;
    }

    for (i = SDW_TextNum - 1; i > 0; ) {
        i >>= 1;
        shiftNum++;
        xs *= 0.5f;

        if (!i) break;
        
        shiftNumY++;
        i >>= 1;
        ys *= 0.5f;
        
        if (!i) break;
    }

    SDW_RDR_YS = ys;
    SDW_RDR_XS = xs;
    SDW_RDR_SHIFTY = shiftNumY;
    SDW_RDR_SHIFT = shiftNum;
    SDW_RDR_AND = (1 << shiftNum) - 1;

    return SDW_RDR_AND;
}

void SDW_AddAShadow(struct sShadowElemStruct* ses)
{
    if (!gpst_DX_stDD->shadowStruct)
        gpst_DX_stDD->shadowStruct = SDW_Create();

    if (gpst_DX_stDD->shadowStruct->actShadowNum == gpst_DX_stDD->shadowStruct->maxShadowNum)
    {
        gpst_DX_stDD->shadowStruct->shadowElems = (tShadowElemStruct*)MEM_p_Realloc(gpst_DX_stDD->shadowStruct->shadowElems, 
                                                                                    sizeof(tShadowElemStruct) * (gpst_DX_stDD->shadowStruct->maxShadowNum + 32));
        gpst_DX_stDD->shadowStruct->maxShadowNum += 32;
        memset(&gpst_DX_stDD->shadowStruct->shadowElems[gpst_DX_stDD->shadowStruct->actShadowNum], 0, sizeof(tShadowElemStruct) * 32); // 3712/4/29 = 32
    }
    if ((ses->shadowMdfPtr->flags & 8) != 0)
    {
        ses->sdwTexElemNum = SDW_TextNum | 0x80000000;
        SDW_TextNum++;
        SDW_DispatchTextures();
    }
    memcpy(&gpst_DX_stDD->shadowStruct->shadowElems[gpst_DX_stDD->shadowStruct->actShadowNum++], ses, sizeof(gpst_DX_stDD->shadowStruct->shadowElems[gpst_DX_stDD->shadowStruct->actShadowNum++]));
}

void SDW_Clear()
{
    if (gpst_DX_stDD && gpst_DX_stDD->shadowStruct)
            gpst_DX_stDD->shadowStruct->actShadowNum = 0;
}

BOOL SDW_GET_SF(tGameObjectMain* gao, tGameObjectMain* gaoToBeCheckedAsParent)
{
    int flags;

    while (1)
    {
        flags = gao ? gao->flags : 0;
        if ((flags & 0x400000) == 0)
            break;
        if (!gao->basic)
            break;
        if (!gao->basic->mtx)
            break;
        if (!gao->basic->mtx->refGao2)
            break;
        if (gaoToBeCheckedAsParent == gao)
            return 1;
        gao = gao->basic->mtx->refGao2;
    }
    return gao == gaoToBeCheckedAsParent;
}

void SDW_Destroy()
{
    tShadowStruct* v0;

    if (gpst_DX_stDD)
    {
        SDW_Clear();
        v0 = gpst_DX_stDD->shadowStruct;
        if (v0)
        {
            MEM_Free(v0->shadowElems);
            MEM_Free(v0);
            gpst_DX_stDD->shadowStruct = NULL;
        }
    }
}

uint32_t COLOR_ShadowColorMultiply(uint32_t color, float mult)
{
    unsigned int ret = 0;
    float test;

    if (mult >= 1.0)
        return color | 0x2000000;
    if (mult >= 0.98000002)
        return color | 0x1000000;
    if (mult <= 0.02)
        return 0x1000000;

    test = (float)(mult + 49152.0); // 32768.0 + 16384.0
    if (((*(uint8_t*)(&test)) & 0x80) != 0)
        ret = ((unsigned int)color >> 1) & 0x7F7F7F7F;
    if (((*(uint8_t *)(&test)) & 0x40) != 0)
        ret += ((unsigned int)color >> 2) & 0x3F3F3F3F;
    if (((*(uint8_t*)(&test)) & 0x20) != 0)
        ret += ((unsigned int)color >> 3) & 0x1F1F1F1F;
    if (((*(uint8_t*)(&test)) & 0x10) != 0)
        ret += ((unsigned int)color >> 4) & 0xF0F0F0F;
    if (((*(uint8_t*)(&test)) & 8) != 0)
        ret += ((unsigned int)color >> 5) & 0x7070707;
    if (((*(uint8_t*)(&test)) & 4) != 0)
        ret += ((unsigned int)color >> 6) & 0x3030303;
    if (((*(uint8_t*)(&test)) & 2) != 0)
        ret += ((unsigned int)color >> 7) & 0x1010101;
    
    ret |= 0x1000000;
    return ret;
}

void SDW_Display1ShadowOn1Object(tGEOGRO* geogro, tShadowElemStruct* sdes)
{   
    unsigned int i, j;
    Vec3* verticesCurr;
    float* colorCurr;
    tGeoUVElem* uvCurr;
    float dp;
    
    int* colorCurrInt;
    tGeoUVElemAsUInt* UVDataCurr;
    int colorsAdded;
    unsigned int baseColor;
    Vec3 shadowVect; 
    Vec3 UVect, VVect, ZVect;
    tGeoTriangleChunks tmpTCO;
    Mtx4x4 gaoMtxInv;
    Vec3 gaoShadowDist;
    tGeoTriangleChunkElem triangleChunkElemsTmp[84];
    tGeoTriangleChunkElem* currTCOE;

    gpst_DX_stDD->bigBuffer->UVData = gpst_DX_stDD->bigBuffer->data7;
    gpst_DX_stDD->flags |= 2;

    MATH_SubstractVec(&gaoShadowDist, &gpst_DX_stDD->currTargetGao->matrices->Pos, &sdes->centerPosInWorldSpace);

    Vec3 shadowDistsWithCoeffs;

    shadowDistsWithCoeffs.x = (float)((sdes->UVectorInWorldSpace.z * gaoShadowDist.z + sdes->UVectorInWorldSpace.y * gaoShadowDist.y + gaoShadowDist.x * sdes->UVectorInWorldSpace.x) * sdes->coefficients.x);
    shadowDistsWithCoeffs.y = (float)((sdes->VVectorInWorldSpace.z * gaoShadowDist.z + sdes->VVectorInWorldSpace.y * gaoShadowDist.y + gaoShadowDist.x * sdes->VVectorInWorldSpace.x) * sdes->coefficients.y);
    shadowDistsWithCoeffs.z = (float)((sdes->ZVectorInWorldSpace.z * gaoShadowDist.z + sdes->ZVectorInWorldSpace.y * gaoShadowDist.y + gaoShadowDist.x * sdes->ZVectorInWorldSpace.x) * sdes->coefficients.z);

    MATH_InvertMatrix(&gaoMtxInv, gpst_DX_stDD->currTargetGao->matrices);

    if ((gaoMtxInv.mtxflag & 8) != 0)
    {
         gaoMtxInv._m14 = MATH_GetInverse(gaoMtxInv._m14);
         gaoMtxInv._m24 = MATH_GetInverse(gaoMtxInv._m24);         
         gaoMtxInv._m34 = MATH_GetInverse(gaoMtxInv._m34);
    }

    MATH_TransformVector(&UVect, &gaoMtxInv, &sdes->UVectorInWorldSpace);
    MATH_TransformVector(&VVect, &gaoMtxInv, &sdes->VVectorInWorldSpace);
    MATH_TransformVector(&ZVect, &gaoMtxInv, &sdes->ZVectorInWorldSpace);
    MATH_TransformVector(&shadowVect, &gaoMtxInv, &sdes->shadowVectorInWorldSpace);
    MATH_NormalizeEqualVector(&shadowVect);    
    MATH_VecMultScalar(&UVect, &UVect, sdes->coefficients.x);
    MATH_VecMultScalar(&VVect, &VVect, sdes->coefficients.y);
    MATH_VecMultScalar(&ZVect, &ZVect, -sdes->coefficients.z);

    baseColor = (~sdes->color & 0x00ffffff) | 0xfc000000;
    verticesCurr = gpst_DX_stDD->verticesCurrent;
    colorCurr = (float*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
    uvCurr = (tGeoUVElem*)gpst_DX_stDD->bigBuffer->data7;
    
    shadowDistsWithCoeffs.x = shadowDistsWithCoeffs.x + 0.5f;
    shadowDistsWithCoeffs.y = shadowDistsWithCoeffs.y + 0.5f;
    shadowDistsWithCoeffs.z = shadowDistsWithCoeffs.z - 1.0f;

    GEO_UseNormals(geogro);
        
    for (i = 0; i < geogro->vertexNum; i++) {
        uvCurr[i].u  = verticesCurr[i].z * UVect.z + UVect.x * verticesCurr[i].x + UVect.y * verticesCurr[i].y + shadowDistsWithCoeffs.x;
        uvCurr[i].v  = verticesCurr[i].z * VVect.z + VVect.x * verticesCurr[i].x + VVect.y * verticesCurr[i].y + shadowDistsWithCoeffs.y;
        colorCurr[i] = verticesCurr[i].z * ZVect.z + ZVect.x * verticesCurr[i].x + ZVect.y * verticesCurr[i].y - shadowDistsWithCoeffs.z;
    }
    
    if ((sdes->shadowMdfPtr->flags & 0x4000) != 0)
    {
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x8000) == 0)
            shadowVect.x = -shadowVect.x;

        MATH_VecInvert(&shadowVect, &shadowVect);
        GEO_UseNormals(geogro);
        
        for (i = 0; i < geogro->vertexNum; i++) {
            dp = MATH_DotProduct(&geogro->normals[i], &shadowVect);
            if (dp <= 0.0f)
            {
                colorCurr[i] = 0.0f;
            }
            else
            {
                if (colorCurr[i] < 1.0f)
                    colorCurr[i] = (1.0f - (1.0f - dp) * (1.0f - dp)) * colorCurr[i];
            }
        }               
    }
        
    colorCurrInt = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
    
    for (i = 0; i < geogro->vertexNum; i++) {
        if (colorCurrInt[i])
        {
            colorCurrInt[i] = COLOR_ShadowColorMultiply(baseColor, *(float*)&colorCurrInt[i]);
        }
    }

    UVDataCurr = (tGeoUVElemAsUInt*)gpst_DX_stDD->bigBuffer->data7;

    GEO_UseNormals(geogro);

    tmpTCO.MRMUVIndices = NULL;
    tmpTCO.MRMUVIndicesNum = 0;
    tmpTCO.MRMUVIndexElems = NULL;
    tmpTCO.triangleStripBlock = 0;
    tmpTCO.triangleNum = 0;
    tmpTCO.triangleChunkElems = triangleChunkElemsTmp;
    //tmpTCO.materialNum = 0;
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

    MATH_VecInvert(&ZVect, &ZVect);

    for (i = 0; i < (int)geogro->triangleChunkCount; i++) {
        for (j = 0; j < geogro->triangleChunks[i].triangleNum; j++) {
            currTCOE = &geogro->triangleChunks[i].triangleChunkElems[j];
            colorsAdded = *(uint32_t*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * currTCOE->v3] |
                          *(uint32_t*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * currTCOE->v2] |
                          *(uint32_t*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * currTCOE->v1];

            if ((colorsAdded & 0x1000000) != 0 && (colorsAdded & 0xffffff) != 0
                &&  ((UVDataCurr[currTCOE->v1].u & 0xff800000 | ((UVDataCurr[currTCOE->v2].u & 0xff8001ff | ((uint32_t)UVDataCurr[currTCOE->v3].u >> 9)) >> 9)) & 0x80402000) != 0x80402000
                && (((UVDataCurr[currTCOE->v1].u & 0xff800000 | ((UVDataCurr[currTCOE->v2].u & 0xff8001ff | ((uint32_t)UVDataCurr[currTCOE->v3].u >> 9)) >> 9)) + 0x804020) & 0xc0603000) != 0x40201000)
            {
                if (((UVDataCurr[currTCOE->v1].v & 0xFF800000 | ((UVDataCurr[currTCOE->v2].v & 0xff8001ff | ((uint32_t)UVDataCurr[currTCOE->v3].v >> 9)) >> 9)) & 0x80402000) != 0x80402000 
                && (((UVDataCurr[currTCOE->v1].v & 0xFF800000 | ((UVDataCurr[currTCOE->v2].v & 0xff8001ff | ((uint32_t)UVDataCurr[currTCOE->v3].v >> 9)) >> 9)) + 0x804020) & 0xc0603000) != 0x40201000)
                {
                    triangleChunkElemsTmp[tmpTCO.triangleNum].v1 = currTCOE->v1;
                    triangleChunkElemsTmp[tmpTCO.triangleNum].v2 = currTCOE->v2;
                    triangleChunkElemsTmp[tmpTCO.triangleNum].v3 = currTCOE->v3;
                    tmpTCO.triangleNum++;
                    if (tmpTCO.triangleNum == 84)
                    {
                        gpst_DX_stDD->DrawElementIndexedTriangles(&tmpTCO, gpst_DX_stDD->verticesCurrent, (tGeoUVElem*)(gpst_DX_stDD->bigBuffer->UVData), geogro->vertexNum);
                        tmpTCO.triangleNum = 0;
                    }
                }
            }
        }
    }
    if (tmpTCO.triangleNum) {
        gpst_DX_stDD->DrawElementIndexedTriangles(&tmpTCO, gpst_DX_stDD->verticesCurrent, (tGeoUVElem*)(gpst_DX_stDD->bigBuffer->UVData), geogro->vertexNum);
    }
}

void SDW_RenderShadow(tGdi_DisplayData* gdd, tGEOGRO* geogro, tShadowElemStruct* ses)
{
    unsigned int i, sdwNum;
    double v5;
    double xs;
    double ys;

    unsigned int clearFlag;
    int zEnableBackup;
    unsigned int v24;
    float unusedShiftParams[4];
    Mtx4x4 viewMtx;
    Mtx4x4 gaoMtxInv;
    Vec3 posDiff;

    memset(&gaoMtxInv, 0, sizeof(gaoMtxInv));
    memset(&viewMtx, 0, sizeof(viewMtx));
    sdwNum = ses->sdwTexElemNum & 0x7fffffff;
    clearFlag = ses->sdwTexElemNum & 0x80000000;
    ses->sdwTexElemNum = sdwNum;
    
    v24 = SDW_RDR_AND & sdwNum;
    v5 = (double)(int)v24;
    
    if ((SDW_RDR_AND & sdwNum & 0x80000000) != 0) 
        v5 = v5 + 4294967300.0;
    
    xs = (v5 + v5 + 1.0) * SDW_RDR_XS;
    v24 = sdwNum >> SDW_RDR_SHIFT;
    ys = ((double)v24 + (double)v24 + 1.0) * SDW_RDR_YS;

    unusedShiftParams[0] = (float)(xs - SDW_RDR_XS);
    unusedShiftParams[1] = (float)(ys - SDW_RDR_YS);
    unusedShiftParams[2] = (float)(SDW_RDR_XS + xs);
    unusedShiftParams[3] = (float)(SDW_RDR_YS + ys + 1.0);
    
    MATH_SubstractVec(&posDiff, &gdd->currTargetGao->matrices->Pos, &ses->centerPosInWorldSpace);

    viewMtx.Pos.x =  (ses->UVectorInWorldSpace.x * posDiff.x + ses->UVectorInWorldSpace.y * posDiff.y + ses->UVectorInWorldSpace.z * posDiff.z) * ses->coefficients.x * 2.0f;
    viewMtx.Pos.y = -(ses->VVectorInWorldSpace.x * posDiff.x + ses->VVectorInWorldSpace.y * posDiff.y + ses->VVectorInWorldSpace.z * posDiff.z) * ses->coefficients.y * 2.0f;
    viewMtx.Pos.z = 0.0f;
    
    MATH_InvertMatrix(&gaoMtxInv, gdd->currTargetGao->matrices);
    MATH_Transp33MatrixWithoutBuffer(&gaoMtxInv, gdd->currTargetGao->matrices);

    if ((gaoMtxInv.mtxflag & 8) != 0)
    {
        gaoMtxInv._m14 = (float)(1.0 / gaoMtxInv._m14);
        gaoMtxInv._m24 = (float)(1.0 / gaoMtxInv._m24);
        gaoMtxInv._m34 = (float)(1.0 / gaoMtxInv._m34);
    }
    else
    {
        gaoMtxInv._m14 = 1.0f;
        gaoMtxInv._m24 = 1.0f;
        gaoMtxInv._m34 = 1.0f;
    }
    MATH_TransformVector(&viewMtx.v1, &gaoMtxInv, &ses->UVectorInWorldSpace);
    MATH_TransformVector(&viewMtx.v2, &gaoMtxInv, &ses->VVectorInWorldSpace);
    MATH_VecMultScalar(&viewMtx.v1, &viewMtx.v1,  ses->coefficients.x * 2.0f);
    MATH_VecMultScalar(&viewMtx.v2, &viewMtx.v2, -ses->coefficients.y * 2.0f);

    viewMtx.v3.x = 0.0f;
    viewMtx.v3.y = 0.0f;
    viewMtx.v3.z = 0.0f;
    MATH_TranspEq33Matrix(&viewMtx);
    viewMtx._m34 = 0.0f;
    viewMtx._m24 = 0.0f;
    viewMtx._m14 = 0.0f;
    viewMtx.w = 1.0f;

    gdd->SetViewMatrixSDW(&viewMtx, unusedShiftParams);
    gdd->TextureInit(-1, 0x2260, 0);
    gdd->D3D_SetRenderTargetAndBackupPreviousSDW(ses->sdwTexElemNum, clearFlag);
    gdd->bigBuffer->vertexColorArray = NULL;
    gdd->bigBuffer->colorMask = 0x00000000;
    gdd->bigBuffer->solidVertexColor = 0xffffffff;
    gpst_DX_stDD->bigBuffer->vertexColorArraySecond = NULL;
    gdd->TextureInit(-1, 0, 0);
    
    zEnableBackup = renderStateConfig.d3d_zEnable;

    if (renderStateConfig.d3d_zEnable)
    {
        renderStateConfig.d3d_zEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, 0);
    }
    if (renderStateConfig.d3d_zWriteEnable)
    {
        renderStateConfig.d3d_zWriteEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, 0);
    }

    for (i = 0; i < geogro->triangleChunkCount; i++) {
        gdd->DrawElementIndexedTriangles(&geogro->triangleChunks[i], gdd->verticesCurrent, NULL, geogro->vertexNum);
    }

    if (renderStateConfig.d3d_zEnable != zEnableBackup)
    {
        renderStateConfig.d3d_zEnable = zEnableBackup;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, zEnableBackup);
    }
    gdd->D3D_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    gdd->bigBuffer->solidVertexColor = 0x00000000;
    gdd->bigBuffer->vertexColorArray = (int*)gdd->bigBuffer->data3FinalColorWithShadow;
    gdd->SetViewMatrix(gdd->ct.cam.viewMtxPtr_setInGRORender);
    gdd->SetProjectionMatrix(&gdd->ct.cam);
}

void SDW_DisplayShadowsOnCurrentObject(struct sGEOGRO* geogro)
{
    int i;
    unsigned int flags = 0x204;

    if (gpst_DX_stDD->shadowStruct && gpst_DX_stDD->shadowStruct->maxShadowNum && (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x10000) != 0
        && gpst_DX_stDD->currTargetGao != NULL && (gpst_DX_stDD->flags & 0x8000) == 0)
    {
        QueryPerformanceCounter(&PerfCounterStruct[3]);
        gpst_DX_stDD->bigBuffer->solidVertexColor = 0x00000000;
        gpst_DX_stDD->bigBuffer->vertexColorArray = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;

        for (i = 0; i < gpst_DX_stDD->shadowStruct->actShadowNum; i++) {
            if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 0x20000) == 0)
            {
                if (!SDW_GET_SF(gpst_DX_stDD->currTargetGao, gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->gao) || // is the shadow owner gao rendered? 
                    (gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 0x1000) == 0)
                {
                    if ((gpst_DX_stDD->flags & 4) == 0)
                    {
                        if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 0x80) != 0 || (gpst_DX_stDD->currTargetGao && ((gpst_DX_stDD->currTargetGao->flags & 0xff) & 5) == 0))
                        {
                            if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 0x100) != 0 || (gpst_DX_stDD->currTargetGao && ((gpst_DX_stDD->currTargetGao->flags & 0xff) & 5) != 0))
                            {
                                if (gpst_DX_stDD->currTargetGao->boundingVolume->BVMax.x + gpst_DX_stDD->currTargetGao->matrices->Pos.x >= gpst_DX_stDD->shadowStruct->shadowElems[i].posTestVecMax.x
                                    && gpst_DX_stDD->currTargetGao->boundingVolume->BVMax.y + gpst_DX_stDD->currTargetGao->matrices->Pos.y >= gpst_DX_stDD->shadowStruct->shadowElems[i].posTestVecMax.y
                                    && gpst_DX_stDD->currTargetGao->boundingVolume->BVMin.x + gpst_DX_stDD->currTargetGao->matrices->Pos.x <= gpst_DX_stDD->shadowStruct->shadowElems[i].posTestVecMin.x
                                    && gpst_DX_stDD->currTargetGao->boundingVolume->BVMin.y + gpst_DX_stDD->currTargetGao->matrices->Pos.y <= gpst_DX_stDD->shadowStruct->shadowElems[i].posTestVecMin.y
                                    && gpst_DX_stDD->currTargetGao->boundingVolume->BVMin.z + gpst_DX_stDD->currTargetGao->matrices->Pos.z <= gpst_DX_stDD->shadowStruct->shadowElems[i].posTestVecMin.z
                                    && gpst_DX_stDD->currTargetGao->boundingVolume->BVMax.z + gpst_DX_stDD->currTargetGao->matrices->Pos.z >= gpst_DX_stDD->shadowStruct->shadowElems[i].posTestVecMax.z)
                                {
                                    if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 8) != 0)
                                    {
                                        flags = flags & ~0x70000 | 0x80020;
                                        gpst_DX_stDD->TextureInit(gpst_DX_stDD->shadowStruct->shadowElems[i].sdwTexElemNum | 0x80000000, flags, 0);
                                    }
                                    else
                                    {
                                        if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 0x80000) != 0)
                                        {
                                            flags = flags & ~0xa0000 | 0x50000;
                                            gpst_DX_stDD->shadowStruct->shadowElems[i].color ^= 0x00ffffff;
                                        }
                                        else
                                        {
                                            flags = flags & ~0x90000 | 0x60000;
                                        }
                                        gpst_DX_stDD->TextureInit(gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->shadowTextureIdx, flags, 0);
                                    }
                                    SDW_Display1ShadowOn1Object(geogro, &gpst_DX_stDD->shadowStruct->shadowElems[i]);
                                    if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 0x80000) != 0)
                                        gpst_DX_stDD->shadowStruct->shadowElems[i].color ^= 0x00ffffff;
                                }
                            }
                        }
                    }
                }
                else if ((gpst_DX_stDD->shadowStruct->shadowElems[i].shadowMdfPtr->flags & 8) != 0)
                {
                    SDW_RenderShadow(gpst_DX_stDD, geogro, &gpst_DX_stDD->shadowStruct->shadowElems[i]); // renders curr gao's shadow to a texture
                }
            }
        }
        PRO_QPC_RecordTimeDecRef(3);
    }
}

void SDW_UpdatePositions()
{
    int i, j, k;
    float uCoeff;
    float objVLen;
    float vVectorPart1XBackup;
    float vVectorPart1YBackup;
    float vVectorPart1ZBackup;
    float dp, vdp, udp, uvNormDP;
    Vec3 uVectorPart1, uVectorPart2;
    Vec3 vVectorPart1, vVectorPart2;
    Vec3 uVectorPart1Norm, vVectorPart1Norm;
    int bitMask;
    float vCoeff;
    float shadowMaxX;
    float shadowMaxY;
    float shadowMaxZ;
    float ySizeFactor;
    Vec3 objSize;
    Vec3 planes[5];
    float planeDPs[5];
    float uCoeffBackup, vecLen;
    tShadowElemStruct* currShadowElem;
    
    Vec3 tmpv = { 0.0f, 0.0f, 1.0f };

    SDW_TextNum = 0;
    GAO_ModifierShadow_Apply_All();

    if (!gpst_DX_stDD->shadowStruct) return;

        MATH_TransformVector(&planes[0], &gpst_DX_stDD->ct.cam.objectMtx, &gpst_DX_stDD->ct.cam.planeUp);
        MATH_TransformVector(&planes[1], &gpst_DX_stDD->ct.cam.objectMtx, &gpst_DX_stDD->ct.cam.planeDown);
        MATH_TransformVector(&planes[2], &gpst_DX_stDD->ct.cam.objectMtx, &gpst_DX_stDD->ct.cam.planeLeft);
        MATH_TransformVector(&planes[3], &gpst_DX_stDD->ct.cam.objectMtx, &gpst_DX_stDD->ct.cam.planeRight);
        MATH_TransformVector(&planes[4], &gpst_DX_stDD->ct.cam.objectMtx, &tmpv);

        for (i = 0; i < 5; i++) {
            MATH_VecInvert(&planes[i], &planes[i]);
            planeDPs[i] = gpst_DX_stDD->ct.cam.objectMtx.Pos.x * planes[i].x + gpst_DX_stDD->ct.cam.objectMtx.Pos.z * planes[i].z + gpst_DX_stDD->ct.cam.objectMtx.Pos.y * planes[i].y;
        }

        for (i = 0; i < gpst_DX_stDD->shadowStruct->actShadowNum; i++)
        {
            currShadowElem = &gpst_DX_stDD->shadowStruct->shadowElems[i];
            currShadowElem->shadowMdfPtr->flags |= 0x20000;
            currShadowElem->shadowMdfPtr->flags |= 0x40;
            vCoeff = currShadowElem->shadowMdfPtr->xSizeFactor;
            ySizeFactor = currShadowElem->shadowMdfPtr->ySizeFactor;
            objSize.x = 1.0f;
            objSize.y = 1.0f;
            objSize.z = 1.0f;

            if ((currShadowElem->shadowMdfPtr->gao->flags & 0x80000) != 0) {
                objSize.x = currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMaxLocal.x - currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMinLocal.x;
                objSize.y = currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMaxLocal.y - currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMinLocal.y;
                objSize.z = currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMaxLocal.z - currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMinLocal.z;
            }
            else if (OBJ_BV_IsAABBox(currShadowElem->shadowMdfPtr->gao->boundingVolume))
            {
                objSize.x = currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMax.x - currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMin.x;
                objSize.y = currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMax.y - currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMin.y;
                objSize.z = currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMax.z - currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMin.z;
            }
            if ((currShadowElem->shadowMdfPtr->gao->flags & 0x80000) != 0)
            {
                MATH_BlendVector(&currShadowElem->centerPosInWorldSpace, &currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMinLocal, &currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMaxLocal, 0.5f);
                MATH_TransformVertex(&currShadowElem->centerPosInWorldSpace, currShadowElem->shadowMdfPtr->gao->matrices, &currShadowElem->centerPosInWorldSpace);
            }
            else
            {
                if (OBJ_BV_IsAABBox(currShadowElem->shadowMdfPtr->gao->boundingVolume))
                {
                    MATH_BlendVector(&currShadowElem->centerPosInWorldSpace, &currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMin, &currShadowElem->shadowMdfPtr->gao->boundingVolume->BVMax, 0.5f);
                    currShadowElem->centerPosInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->Pos.x + currShadowElem->centerPosInWorldSpace.x;
                    currShadowElem->centerPosInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->Pos.y + currShadowElem->centerPosInWorldSpace.y;
                    currShadowElem->centerPosInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->Pos.z + currShadowElem->centerPosInWorldSpace.z;
                }
                else
                {
                    currShadowElem->centerPosInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->Pos.x;
                    currShadowElem->centerPosInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->Pos.y;
                    currShadowElem->centerPosInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->Pos.z;
                }
            }

            currShadowElem->centerPosInWorldSpace.x = -currShadowElem->shadowMdfPtr->zStart * currShadowElem->shadowVectorInWorldSpace.x + currShadowElem->centerPosInWorldSpace.x;
            currShadowElem->centerPosInWorldSpace.y = -currShadowElem->shadowMdfPtr->zStart * currShadowElem->shadowVectorInWorldSpace.y + currShadowElem->centerPosInWorldSpace.y;
            currShadowElem->centerPosInWorldSpace.z = -currShadowElem->shadowMdfPtr->zStart * currShadowElem->shadowVectorInWorldSpace.z + currShadowElem->centerPosInWorldSpace.z;
            currShadowElem->centerPosInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->v1.x * currShadowElem->shadowMdfPtr->centerPos.x + currShadowElem->centerPosInWorldSpace.x;
            currShadowElem->centerPosInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->v1.y * currShadowElem->shadowMdfPtr->centerPos.x + currShadowElem->centerPosInWorldSpace.y;
            currShadowElem->centerPosInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->v1.z * currShadowElem->shadowMdfPtr->centerPos.x + currShadowElem->centerPosInWorldSpace.z;
            currShadowElem->centerPosInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->v2.x * currShadowElem->shadowMdfPtr->centerPos.y + currShadowElem->centerPosInWorldSpace.x;
            currShadowElem->centerPosInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->v2.y * currShadowElem->shadowMdfPtr->centerPos.y + currShadowElem->centerPosInWorldSpace.y;
            currShadowElem->centerPosInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->v2.z * currShadowElem->shadowMdfPtr->centerPos.y + currShadowElem->centerPosInWorldSpace.z;
            currShadowElem->centerPosInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->v3.x * currShadowElem->shadowMdfPtr->centerPos.z + currShadowElem->centerPosInWorldSpace.x;
            currShadowElem->centerPosInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->v3.y * currShadowElem->shadowMdfPtr->centerPos.z + currShadowElem->centerPosInWorldSpace.y;
            currShadowElem->centerPosInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->v3.z * currShadowElem->shadowMdfPtr->centerPos.z + currShadowElem->centerPosInWorldSpace.z;

            switch (currShadowElem->shadowMdfPtr->shadowProjectionMode)
            {
            case 0:
                vVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v1.x;
                vCoeff = objSize.x * vCoeff;
                uCoeff = objSize.y * ySizeFactor;
                vVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v1.y;
                vVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v1.z;
                uVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v2.x;
                uVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v2.y;
                uVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v2.z;
                currShadowElem->ZVectorInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->v3.x;
                currShadowElem->ZVectorInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->v3.y;
                currShadowElem->ZVectorInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->v3.z;
                break;
            case 1:
                vVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v1.x;
                vCoeff = objSize.x * vCoeff;
                uCoeff = objSize.z * ySizeFactor;
                vVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v1.y;
                vVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v1.z;
                uVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v3.x;
                uVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v3.y;
                uVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v3.z;
                currShadowElem->ZVectorInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->v2.x;
                currShadowElem->ZVectorInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->v2.y;
                currShadowElem->ZVectorInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->v2.z;
                break;
            case 2:
                vVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v2.x;
                vCoeff = objSize.y * vCoeff;
                uCoeff = objSize.z * ySizeFactor;
                vVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v2.y;
                vVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v2.z;
                uVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v3.x;
                uVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v3.y;
                uVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v3.z;
                currShadowElem->ZVectorInWorldSpace.x = currShadowElem->shadowMdfPtr->gao->matrices->v1.x;
                currShadowElem->ZVectorInWorldSpace.y = currShadowElem->shadowMdfPtr->gao->matrices->v1.y;
                currShadowElem->ZVectorInWorldSpace.z = currShadowElem->shadowMdfPtr->gao->matrices->v1.z;
                break;
            case 3:
                vCoeff = objSize.x * vCoeff;
                vVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v1.x;
                vVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v1.y;
                vVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v1.z;
                uVectorPart1.x = vVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.z - vVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.y;
                uVectorPart1.y = vVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.x - vVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.z;
                uVectorPart1.z = vVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.y - vVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.x;
                MATH_NormalizeEqualVector(&uVectorPart1);
                currShadowElem->ZVectorInWorldSpace.x = (float)(uVectorPart1.z * vVectorPart1.y - uVectorPart1.y * vVectorPart1.z);
                currShadowElem->ZVectorInWorldSpace.y = vVectorPart1.z * uVectorPart1.x - uVectorPart1.z * vVectorPart1.x;
                currShadowElem->ZVectorInWorldSpace.z = uVectorPart1.y * vVectorPart1.x - vVectorPart1.y * uVectorPart1.x;
                dp = MATH_DotProduct(&currShadowElem->shadowMdfPtr->gao->matrices->v2, &uVectorPart1);
                if (dp < 0.0f)
                    dp = -dp;
                uCoeff = ((1.0f - dp) * objSize.z + dp * objSize.y) * ySizeFactor;  // linear interpolate
                break;
            case 4:
                vCoeff = objSize.y * vCoeff;
                vVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v2.y;
                vVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v2.z;
                vVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v2.x;
                uVectorPart1.x = vVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.z - vVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.y;
                uVectorPart1.y = vVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.x - vVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.z;
                uVectorPart1.z = vVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.y - vVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.x;
                MATH_NormalizeEqualVector(&uVectorPart1);
                currShadowElem->ZVectorInWorldSpace.x = (float)(uVectorPart1.z * vVectorPart1.y - uVectorPart1.y * vVectorPart1.z);
                currShadowElem->ZVectorInWorldSpace.y = vVectorPart1.z * uVectorPart1.x - uVectorPart1.z * vVectorPart1.x;
                currShadowElem->ZVectorInWorldSpace.z = uVectorPart1.y * vVectorPart1.x - vVectorPart1.y * uVectorPart1.x;
                dp = MATH_DotProduct(&currShadowElem->shadowMdfPtr->gao->matrices->v1, &uVectorPart1);
                if (dp < 0.0f)
                    dp = -dp;
                uCoeff = ((1.0f - dp) * objSize.z + dp * objSize.x) * ySizeFactor; // linear interpolate
                break;
            case 5:
                vCoeff = objSize.z * vCoeff;
                vVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v3.y;
                vVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v3.z;
                vVectorPart1.x = currShadowElem->shadowMdfPtr->gao->matrices->v3.x;
                uVectorPart1.x = vVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.z - vVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.y;
                uVectorPart1.y = vVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.x - vVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.z;
                uVectorPart1.z = vVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.y - vVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.x;
                MATH_NormalizeEqualVector(&uVectorPart1);
                currShadowElem->ZVectorInWorldSpace.x = (float)(uVectorPart1.z * vVectorPart1.y - uVectorPart1.y * vVectorPart1.z);
                currShadowElem->ZVectorInWorldSpace.y = vVectorPart1.z * uVectorPart1.x - uVectorPart1.z * vVectorPart1.x;
                currShadowElem->ZVectorInWorldSpace.z = uVectorPart1.y * vVectorPart1.x - vVectorPart1.y * uVectorPart1.x;
                dp = MATH_DotProduct(&currShadowElem->shadowMdfPtr->gao->matrices->v1, &uVectorPart1);
                if (dp < 0.0f)
                    dp = -dp;
                uCoeff = ((1.0f - dp) * objSize.y + dp * objSize.x) * ySizeFactor;  // linear interpolate
                break;
            case 6:
                uVectorPart2.x = currShadowElem->shadowVectorInWorldSpace.x;
                uVectorPart2.y = currShadowElem->shadowVectorInWorldSpace.y;
                uVectorPart2.z = currShadowElem->shadowVectorInWorldSpace.z;
                
                if (uVectorPart2.z < 0.0)
                    uVectorPart2.z = -uVectorPart2.z;
                
                objVLen = (float)sqrt(objSize.y * objSize.y + objSize.x * objSize.x);
                uCoeff = objVLen * ySizeFactor;
                ySizeFactor = uCoeff;
                vCoeff = (float)((sqrt(1.0f - uVectorPart2.z * uVectorPart2.z) * objSize.z + objVLen * uVectorPart2.z) * vCoeff);
                dp = MATH_DotProduct(&currShadowElem->shadowMdfPtr->gao->matrices->v3, &currShadowElem->shadowVectorInWorldSpace);
                                
                if (dp >= 0.99900001f || dp <= -0.99900001f)
                {
                    uVectorPart1.x = currShadowElem->shadowVectorInWorldSpace.z * currShadowElem->shadowMdfPtr->gao->matrices->v1.y - currShadowElem->shadowVectorInWorldSpace.y * currShadowElem->shadowMdfPtr->gao->matrices->v1.z;
                    uVectorPart1.y = currShadowElem->shadowMdfPtr->gao->matrices->v1.z * currShadowElem->shadowVectorInWorldSpace.x - currShadowElem->shadowMdfPtr->gao->matrices->v1.x * currShadowElem->shadowVectorInWorldSpace.z;
                    uVectorPart1.z = currShadowElem->shadowMdfPtr->gao->matrices->v1.x * currShadowElem->shadowVectorInWorldSpace.y - currShadowElem->shadowMdfPtr->gao->matrices->v1.y * currShadowElem->shadowVectorInWorldSpace.x;
                }
                else
                {
                    MATH_CrossProduct(&uVectorPart1, &currShadowElem->shadowMdfPtr->gao->matrices->v3, &currShadowElem->shadowVectorInWorldSpace);
                    uCoeff = ySizeFactor;
                }

                MATH_NormalizeEqualVector(&uVectorPart1);
                
                vVectorPart1.x = uVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.z - uVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.y;
                vVectorPart1.y = uVectorPart1.z * currShadowElem->shadowVectorInWorldSpace.x - uVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.z;
                vVectorPart1.z = uVectorPart1.x * currShadowElem->shadowVectorInWorldSpace.y - uVectorPart1.y * currShadowElem->shadowVectorInWorldSpace.x;
                currShadowElem->ZVectorInWorldSpace.x = uVectorPart1.y * vVectorPart1.z - uVectorPart1.z * vVectorPart1.y;
                currShadowElem->ZVectorInWorldSpace.y = uVectorPart1.z * vVectorPart1.x - vVectorPart1.z * uVectorPart1.x;
                currShadowElem->ZVectorInWorldSpace.z = vVectorPart1.y * uVectorPart1.x - uVectorPart1.y * vVectorPart1.x;
                break;
            default:
                uCoeff = ySizeFactor;
                break;
            }
            if ((currShadowElem->shadowMdfPtr->flags & 0x800) != 0)
            {
                MATH_VecInvert(&uVectorPart1, &uVectorPart1);
                MATH_VecInvert(&vVectorPart1, &vVectorPart1);
            }
            if ((currShadowElem->shadowMdfPtr->flags & 0x400) != 0)
            {
                uCoeffBackup = uCoeff;
                uCoeff = vCoeff;
                vCoeff = uCoeffBackup;
                vVectorPart1XBackup = vVectorPart1.x;
                vVectorPart1YBackup = vVectorPart1.y;
                vVectorPart1ZBackup = vVectorPart1.z;
                vVectorPart1.x = -uVectorPart1.x;
                vVectorPart1.y = -uVectorPart1.y;
                vVectorPart1.z = -uVectorPart1.z;
                uVectorPart2.y = vVectorPart1YBackup;
                uVectorPart2.z = vVectorPart1.z;
                uVectorPart2.x = vVectorPart1XBackup;
                uVectorPart1.x = vVectorPart1XBackup;
                uVectorPart1.y = vVectorPart1YBackup;
                uVectorPart1.z = vVectorPart1ZBackup;
            }

            udp = -MATH_DotProduct(&uVectorPart1, &currShadowElem->shadowVectorInWorldSpace);
            vdp = -MATH_DotProduct(&vVectorPart1, &currShadowElem->shadowVectorInWorldSpace);

            uVectorPart1.x = (udp * currShadowElem->shadowVectorInWorldSpace.x + uVectorPart1.x) * uCoeff;
            uVectorPart1.y = (udp * currShadowElem->shadowVectorInWorldSpace.y + uVectorPart1.y) * uCoeff;
            uVectorPart1.z = (udp * currShadowElem->shadowVectorInWorldSpace.z + uVectorPart1.z) * uCoeff;
            vVectorPart1.x = (vdp * currShadowElem->shadowVectorInWorldSpace.x + vVectorPart1.x) * vCoeff;
            vVectorPart1.y = (vdp * currShadowElem->shadowVectorInWorldSpace.y + vVectorPart1.y) * vCoeff;
            vVectorPart1.z = (vdp * currShadowElem->shadowVectorInWorldSpace.z + vVectorPart1.z) * vCoeff;

            if (currShadowElem->ZVectorInWorldSpace.x * currShadowElem->shadowVectorInWorldSpace.x
                + currShadowElem->shadowVectorInWorldSpace.z * currShadowElem->ZVectorInWorldSpace.z
                + currShadowElem->shadowVectorInWorldSpace.y * currShadowElem->ZVectorInWorldSpace.y < 0.0f)
            {
                currShadowElem->ZVectorInWorldSpace.x = -currShadowElem->ZVectorInWorldSpace.x;
                currShadowElem->ZVectorInWorldSpace.y = -currShadowElem->ZVectorInWorldSpace.y;
                currShadowElem->ZVectorInWorldSpace.z = -currShadowElem->ZVectorInWorldSpace.z;
            }

            MATH_NormalizeVector(&uVectorPart1Norm, &uVectorPart1);
            MATH_NormalizeVector(&vVectorPart1Norm, &vVectorPart1);

            uvNormDP = vVectorPart1Norm.x * uVectorPart1Norm.x + vVectorPart1Norm.z * uVectorPart1Norm.z + vVectorPart1Norm.y * uVectorPart1Norm.y;

            MATH_VecMultScalar(&uVectorPart2, &vVectorPart1Norm, MATH_VecLen(&uVectorPart1)* uvNormDP);
            MATH_VecMultScalar(&vVectorPart2, &uVectorPart1Norm, MATH_VecLen(&vVectorPart1)* uvNormDP);

            currShadowElem->UVectorInWorldSpace.x = uVectorPart1.x - uVectorPart2.x;
            currShadowElem->UVectorInWorldSpace.y = uVectorPart1.y - uVectorPart2.y;
            currShadowElem->UVectorInWorldSpace.z = uVectorPart1.z - uVectorPart2.z;
            currShadowElem->VVectorInWorldSpace.x = vVectorPart1.x - vVectorPart2.x;
            currShadowElem->VVectorInWorldSpace.y = vVectorPart1.y - vVectorPart2.y;
            currShadowElem->VVectorInWorldSpace.z = vVectorPart1.z - vVectorPart2.z;
            vecLen = (float)(sqrt(currShadowElem->VVectorInWorldSpace.x * currShadowElem->VVectorInWorldSpace.x + currShadowElem->VVectorInWorldSpace.z * currShadowElem->VVectorInWorldSpace.z + currShadowElem->VVectorInWorldSpace.y * currShadowElem->VVectorInWorldSpace.y));
            currShadowElem->coefficients.y = MATH_GetInverse(vecLen);
            currShadowElem->VVectorInWorldSpace.x = currShadowElem->coefficients.y * currShadowElem->VVectorInWorldSpace.x;
            currShadowElem->VVectorInWorldSpace.y = currShadowElem->coefficients.y * currShadowElem->VVectorInWorldSpace.y;
            currShadowElem->VVectorInWorldSpace.z = currShadowElem->coefficients.y * currShadowElem->VVectorInWorldSpace.z;
            vecLen = (float)(sqrt(currShadowElem->UVectorInWorldSpace.x * currShadowElem->UVectorInWorldSpace.x + currShadowElem->UVectorInWorldSpace.z * currShadowElem->UVectorInWorldSpace.z + currShadowElem->UVectorInWorldSpace.y * currShadowElem->UVectorInWorldSpace.y));
            currShadowElem->coefficients.x = MATH_GetInverse(vecLen);
            currShadowElem->UVectorInWorldSpace.x = currShadowElem->coefficients.x * currShadowElem->UVectorInWorldSpace.x;
            currShadowElem->UVectorInWorldSpace.y = currShadowElem->coefficients.x * currShadowElem->UVectorInWorldSpace.y;
            currShadowElem->UVectorInWorldSpace.z = currShadowElem->coefficients.x * currShadowElem->UVectorInWorldSpace.z;
            currShadowElem->coefficients.z = MATH_GetInverse(currShadowElem->ZDepth);
            
            currShadowElem->posTestVecMax.x = FLT_MAX;
            currShadowElem->posTestVecMax.y = FLT_MAX;
            currShadowElem->posTestVecMax.z = FLT_MAX;
            currShadowElem->posTestVecMin.x = FLT_MAX;
            currShadowElem->posTestVecMin.y = FLT_MAX;
            currShadowElem->posTestVecMin.z = FLT_MAX;
            
            bitMask = 31;
            for (j = 16 - 1; j != 0; j--)
            {
                shadowMaxX = currShadowElem->centerPosInWorldSpace.x;
                shadowMaxY = currShadowElem->centerPosInWorldSpace.y;
                shadowMaxZ = currShadowElem->centerPosInWorldSpace.z;

                if ((j & 1) != 0)
                {
                    shadowMaxX += uVectorPart1.x;
                    shadowMaxY += uVectorPart1.y;
                    shadowMaxZ += uVectorPart1.z;
                }
                else
                {
                    shadowMaxX -= uVectorPart1.x;
                    shadowMaxY -= uVectorPart1.y;
                    shadowMaxZ -= uVectorPart1.z;
                }
                if ((j & 2) != 0)
                {
                    shadowMaxX += vVectorPart1.x;
                    shadowMaxY += vVectorPart1.y;
                    shadowMaxZ += vVectorPart1.z;
                }
                else
                {
                    shadowMaxX -= vVectorPart1.x;
                    shadowMaxY -= vVectorPart1.y;
                    shadowMaxZ -= vVectorPart1.z;
                }
                if ((j & 4) != 0)
                {
                    shadowMaxX += currShadowElem->ZVectorInWorldSpace.x;
                    shadowMaxY += currShadowElem->ZVectorInWorldSpace.y;
                    shadowMaxZ += currShadowElem->ZVectorInWorldSpace.z;
                }
                else
                {
                    shadowMaxX -= currShadowElem->ZVectorInWorldSpace.x;
                    shadowMaxY -= currShadowElem->ZVectorInWorldSpace.y;
                    shadowMaxZ -= currShadowElem->ZVectorInWorldSpace.z;
                }
                if ((j & 8) != 0)
                {
                    shadowMaxX += (currShadowElem->ZDepth * currShadowElem->shadowVectorInWorldSpace.x);
                    shadowMaxY += (currShadowElem->ZDepth * currShadowElem->shadowVectorInWorldSpace.y);
                    shadowMaxZ += (currShadowElem->ZDepth * currShadowElem->shadowVectorInWorldSpace.z);
                }

                for (k = 0; k < 5; k++) {
                    if (planes[k].z * shadowMaxZ + shadowMaxX * planes[k].x + shadowMaxY * planes[k].y - planeDPs[k] < 0.0f)
                    {
                        bitMask &= ~(1 << k);
                    }
                }

                if (shadowMaxX < currShadowElem->posTestVecMax.x)
                    currShadowElem->posTestVecMax.x = shadowMaxX;
                if (shadowMaxY < currShadowElem->posTestVecMax.y)
                    currShadowElem->posTestVecMax.y = shadowMaxY;
                if (shadowMaxZ < currShadowElem->posTestVecMax.z)
                    currShadowElem->posTestVecMax.z = shadowMaxZ;
                if (shadowMaxX > currShadowElem->posTestVecMin.x)
                    currShadowElem->posTestVecMin.x = shadowMaxX;
                if (shadowMaxY > currShadowElem->posTestVecMin.y)
                    currShadowElem->posTestVecMin.y = shadowMaxY;
                if (shadowMaxZ > currShadowElem->posTestVecMin.z)
                    currShadowElem->posTestVecMin.z = shadowMaxZ;
            } 

            if (!bitMask)
                currShadowElem->shadowMdfPtr->flags &= ~0x20000;
        }
}