#include "memory.h"
#include "loading.h"
#include "textures.h"
#include "time.h"
#include "win/wingdi.h"
#include "materialanddraw.h"
#include "gdi.h"
#include "water.h"
#include "strings.h"
#include "engine.h"
#include "ai/ai_gfxfunctions.h"

float GDI_gaf_ScreenRatio[4] = { 1.0f, 1.0f,  (3.0f / 4.0f), (9.0f / 16.0f) };
float MAT_C_f_10Bits = 12288.0f;
float MAT_C_f_8Bits = 49152.0f;

float MAT_SinTable[16] = { 0.0f, 0.3826834f, 0.70710671f, 0.9238795f, 
                            1.0f,  0.9238795f,  0.70710671f, 0.3826834f, 
                            0.0f,  -0.3826834f, -0.70710671f, -0.9238795f, 
                            -1.0f, -0.9238795f, -0.70710671f, -0.3826834f };

void(*MAT_FuncTable[16])(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int) = {
                            MAT_UV_Compute_OBJ1,
                            MAT_UV_Compute_OBJ2,
                            MAT_UV_Compute_Chrome,
                            MAT_UV_Compute_DF_GIZMO,
                            MAT_UV_Compute_PHONG_GIZMO,
                            MAT_UV_Compute_Previous,
                            MAT_UV_Compute_PLANAR_GIZMO_NOSTORE,
                            MAT_UV_Compute_FACE_MAP,
                            MAT_UV_Compute_FOGZZ,
                            MAT_UV_Compute_Default,
                            MAT_UV_Compute_Default,
                            MAT_UV_Compute_Default,
                            MAT_UV_Compute_Default,
                            MAT_UV_Compute_Default,
                            MAT_UV_Compute_Default,
                            MAT_UV_Compute_Default
};

BOOL GDI_l_DetachWorld(struct sGdi_DisplayData* dd)
{
    if (!dd->CurrentWorld) return TRUE;

    gpst_DX_stDD = dd;
    dd->D3D_ReleaseTextures(dd);
    TEX_Procedural_Reinit(dd);
    TEX_Anim_Reinit();
    dd->CurrentWorld = NULL;

    return TRUE;
}

void MAT_UVDecompressSpeed(tMultiTextureElemStruct* mte, tMatDecompressedStruct* matds)
{
    int tmp1;
    float tmp2;

    if ((mte->subTextureFlags & 0x40000000) != 0)
    {
        tmp2 = TIM_gf_MainClock * matds->speedU;
        matds->speedU = (float)tmp2;
        matds->speedU = matds->speedU + MAT_C_f_10Bits;
        tmp1 = (*(uint32_t *)&MAT_C_f_10Bits) | (*((uint32_t*)&(matds->speedU))) & 0x3ff;
        matds->speedU = *(float *)&tmp1;
        matds->speedU = matds->speedU - MAT_C_f_10Bits;
    }
    if ((mte->subTextureFlags & 0x80000000) != 0)
    {
        tmp2 = TIM_gf_MainClock * matds->speedV;
        matds->speedV = (float)tmp2;
        matds->speedV = (float)tmp2 + MAT_C_f_10Bits;
        tmp1 = (*(uint32_t*)&MAT_C_f_10Bits) | (*((uint32_t*)&(matds->speedV))) & 0x3ff;
        matds->speedV = *(float*)&tmp1;
        matds->speedV = matds->speedV - MAT_C_f_10Bits;
    }
}

unsigned int MAT_GetRotation_INT(int transform1, unsigned int transform2)
{
    return transform2 & 1 | (4 * (transform1 & 1)) | ((transform1 & 0x10000 | (transform2 >> 2) & 0x4000) >> 13);
}

void MAT_ComputeRotationMatrix(tMultiTextureElemStruct* a1, tMatDecompressedStruct* a2)
{
    char iret;
    float sin1;
    float sin2;
    float f1, f2, f3, f4, f5;

    iret = MAT_GetRotation_INT(a1->transform1, a1->transform2);
    sin1 = MAT_SinTable[iret & 0xF];
    sin2 = MAT_SinTable[(iret + 4) & 0xF];

    f1 = a2->f1;
    f2 = a2->f1;
    f3 = a2->f3;
    f4 = a2->f2;
    f5 = a2->f4;

    a2->f1 = (float)(f1 * sin2 - f3 * sin1);
    a2->f2 = (float)(f4 * sin2 - f5 * sin1);
    a2->f3 = (float)(f3 * sin2 + f2 * sin1);
    a2->f4 = (float)(f5 * sin2 + f4 * sin1);
}

void MAT_UVDecompress(struct sMultiTextureElemStruct* mte, struct sMatDecompressedStruct* matds)
{
    int tmp2 = mte->transform2 & 0xffffe0000;
    int tmp1 = mte->transform1 & 0xffffe0000;

    matds->f1 = *(float*)&tmp1;
    matds->f3 = 0.0f;
    matds->f2 = 0.0f;
    matds->f4 = *(float*)&tmp2;
    matds->speedU = MAT_GetSpeed(mte->transform1) * matds->f1;
    matds->speedV = MAT_GetSpeed(mte->transform2) * matds->f4;
    
    if ((mte->subTextureFlags & 0xc0000000) != 0)
        MAT_UVDecompressSpeed(mte, matds);
    
    MAT_ComputeRotationMatrix(mte, matds);
}

void MAT_UV_Compute_OBJ1(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{
    unsigned int i;

    if (geogro && geogro->intf->id == 1 && geogro->mrm)
    {
        dd->bigBuffer->UVData = dd->bigBuffer->data7;
        dd->flags |= 0x2;

        if (geogro->UVs)
        {
            for (i = 0; i < gtc->MRMUVIndicesNum; i++) {
                ((tGeoUVElem*)dd->bigBuffer->UVData)[gtc->MRMUVIndices[i]].u = geogro->UVs[gtc->MRMUVIndexElems->UVStartOffset + i].u;
                ((tGeoUVElem*)dd->bigBuffer->UVData)[gtc->MRMUVIndices[i]].v = geogro->UVs[gtc->MRMUVIndexElems->UVStartOffset + i].v;
            }
        }
    }
    else
    {
        dd->flags &= ~0x2;
        dd->bigBuffer->UVData = (byte*)geogro->UVs;
    }
}

void MAT_UV_Compute_OBJ2(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{
    unsigned int i;

    if (geogro->UVs)
    {
        if (geogro->intf->id == 1 && geogro->mrm)
        {
            dd->bigBuffer->UVData = dd->bigBuffer->data7;
            dd->flags |= 0x2;

            for (i = 0; i < gtc->MRMUVIndicesNum; i++) {
                ((tGeoUVElem*)dd->bigBuffer->UVData)[gtc->MRMUVIndices[i]].u = geogro->UVs[gtc->MRMUVIndexElems->UVStartOffset + i].u;
                ((tGeoUVElem*)dd->bigBuffer->UVData)[gtc->MRMUVIndices[i]].v = geogro->UVs[gtc->MRMUVIndexElems->UVStartOffset + i].v;
            }           
        }
        else
        {
            if (geogro->uvNum < geogro->vertexNum)
                dd->flags &= ~0x2;
            else
                dd->flags |= 0x2;

            dd->bigBuffer->UVData = (byte*)geogro->UVs;
        }
    }
}

void MAT_UV_Compute_Previous(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{
    if (!dd->bigBuffer->UVData)
        MAT_UV_Compute_OBJ1(dd, geogro, gtc, unk);
}

void MAT_UV_Compute_FACE_MAP(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{
    tGeoUVElem* uv = ((tGeoUVElem*)dd->bigBuffer->data7);
    dd->flags = dd->flags & ~2 | 0x10;
    uv[0].u = 0.0f;
    uv[0].v = 0.0f;
    uv[1].u = 0.0f;
    uv[1].v = 1.0f;
    uv[2].u = 1.0f;
    uv[2].v = 0.0f;
    uv[3].u = 1.0f;
    uv[3].v = 1.0f;
    dd->bigBuffer->UVData = dd->bigBuffer->data7;
}

void MAT_UV_Compute_Default(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{
    dd->flags &= ~0x2;
    dd->bigBuffer->UVData = (byte*)geogro->UVs;
}

void MAT_UV_Compute_DF_GIZMO(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{

    unsigned int i, j;
    tGeoUVElem* UVs;
    Mtx4x4 invMtx;
    Vec3 distV;
    Vec3 mtxInLocal = {
        .x = 0.0f, 
        .y = 0.0f, 
        .z = 0.0f
    };    

    dd->bigBuffer->UVData = (byte*)geogro->UVs;

    MATH_InvertMatrix(&invMtx, dd->matrixStack.actMatrixElem);
    MATH_TransformVertex(&mtxInLocal, &invMtx, &mtxInLocal);

    UVs = (tGeoUVElem*)dd->bigBuffer->data7;

    for (i = 0, j = 0; i < geogro->vertexNum; i++, j += 4) {
        MATH_SubstractVec(&distV, &mtxInLocal, &geogro->vertices[i]);
        UVs[i].u = (((float)dd->bigBuffer->data1LightColors[j + 0] + 
                     (float)dd->bigBuffer->data1LightColors[j + 1] + 
                     (float)dd->bigBuffer->data1LightColors[j + 2]) * (1.0f / (256.0f * 3.0f)));
        UVs[i].v = 1.0f - MATH_GetInverseSqrt(MATH_VecLenSquared(&distV)) * MATH_DotProduct(&distV, &geogro->normals[i]);
        if (UVs[i].v < 0.1f)
            UVs[i].v = 0.1f;
        if (UVs[i].v > 0.89999998f)
            UVs[i].v = 0.89999998f;
    }

    dd->bigBuffer->UVData = dd->bigBuffer->data7;
    dd->flags |= 0x2;
}

void MAT_UV_Compute_FOGZZ(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk)
{
    unsigned int i;
    float invSqLenZ, dp;
    tGeoUVElem* uvBuf;
    Vec3 zNormalVecToLocal; 
    Vec3 nullVecToLocal; 
    Mtx4x4 invMtx;

    nullVecToLocal.z = 0.0;
    nullVecToLocal.y = 0.0;
    nullVecToLocal.x = 0.0;
    zNormalVecToLocal.y = 0.0;
    zNormalVecToLocal.x = 0.0;
    zNormalVecToLocal.z = 1.0;
    MATH_InvertMatrix(&invMtx, dd->matrixStack.actMatrixElem);
    MATH_TransformVertex(&nullVecToLocal, &invMtx, &nullVecToLocal);
    MATH_TransformVector(&zNormalVecToLocal, &invMtx, &zNormalVecToLocal);

    invSqLenZ = MATH_GetInverseSqrt(MATH_VecLenSquared(&zNormalVecToLocal));

    MATH_VecMultScalar(&zNormalVecToLocal, &zNormalVecToLocal, (invSqLenZ * invSqLenZ));

    dd->bigBuffer->UVData = dd->bigBuffer->data7;
    dd->flags |= 0x2;
    
    dp = MATH_DotProduct(&zNormalVecToLocal, &nullVecToLocal);
    MATH_MakeOGLMatrix(&invMtx, dd->currTargetGao->matrices);

    uvBuf = (tGeoUVElem*)&dd->bigBuffer->data7;

    if (gtc->MRMUVIndices)
    {
        for (i = 0; i < gtc->MRMUVIndicesNum; i++) {
            uvBuf[gtc->MRMUVIndices[i]].v = invMtx.v1.z * dd->verticesCurrent[gtc->MRMUVIndices[i]].x +
                                            invMtx.v2.z * dd->verticesCurrent[gtc->MRMUVIndices[i]].y +
                                            invMtx.v3.z * dd->verticesCurrent[gtc->MRMUVIndices[i]].z + invMtx.Pos.z;
            uvBuf[gtc->MRMUVIndices[i]].u = zNormalVecToLocal.x * dd->verticesCurrent[gtc->MRMUVIndices[i]].x +
                                            zNormalVecToLocal.y * dd->verticesCurrent[gtc->MRMUVIndices[i]].y +
                                            zNormalVecToLocal.z * dd->verticesCurrent[gtc->MRMUVIndices[i]].z - dp;
        }
    }
    else
    {
        for (i = 0; i < geogro->vertexNum; i++) {
            uvBuf[i].u = zNormalVecToLocal.x * dd->verticesCurrent[i].x + 
                         zNormalVecToLocal.y * dd->verticesCurrent[i].y +
                         zNormalVecToLocal.z * dd->verticesCurrent[i].z - dp;
            uvBuf[i].v = invMtx.v1.z * dd->verticesCurrent[i].x + 
                         invMtx.v2.z * dd->verticesCurrent[i].y +
                         invMtx.v3.z * dd->verticesCurrent[i].z + invMtx.Pos.z;
        }        
    }
}

void MAT_UV_Compute_PHONG_GIZMO(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int type)
{
    unsigned int i;
    tGeoUVElem* uvBuf;
    Vec3* specularVecs;
    Vec3 uVec, vVec;
    Mtx4x4 currGaoLocalMtx;

    dd->bigBuffer->UVData = (byte*)geogro->UVs;
    
    uVec.z = 0.0f;
    uVec.y = 0.0f;
    uVec.x = 0.0f;
    vVec.z = 0.0f;
    vVec.y = 0.0f;
    vVec.x = 0.0f;

    switch ((type >> 4) & 3)
    {
    case 0:
        uVec.y = 1.0f;
        vVec.z = 1.0f;
        break;
    case 1:
        uVec.x = 1.0f;
        vVec.z = 1.0f;
    case 2:
    case 3:
        uVec.x = 1.0f;
        vVec.y = 1.0f;
        break;
    default:
        break;
    }


    MATH_InvertMatrix(&currGaoLocalMtx, dd->currTargetGao->matrices);    
    currGaoLocalMtx.Pos.z = 0.0f;
    currGaoLocalMtx.Pos.y = 0.0f;
    currGaoLocalMtx.Pos.x = 0.0f;
    
    MATH_TransformVertex(&uVec, &currGaoLocalMtx, &uVec);
    MATH_TransformVertex(&vVec, &currGaoLocalMtx, &vVec);
    
    vVec.x = vVec.x + vVec.x;
    vVec.y = vVec.y + vVec.y;
    vVec.z = vVec.z + vVec.z;
    uVec.x = uVec.x + uVec.x;
    uVec.y = uVec.y + uVec.y;
    uVec.z = uVec.z + uVec.z;

    specularVecs = SOFT_pst_GetSpecularVectors();
    uvBuf = (tGeoUVElem*)dd->bigBuffer->data7;

    for (i = 0; i < geogro->vertexNum; i++) {
        uvBuf[i].u = MATH_DotProduct(&uVec, &specularVecs[i]) + 0.5f;
        uvBuf[i].v = MATH_DotProduct(&vVec, &specularVecs[i]) + 0.5f;
    }

    dd->bigBuffer->UVData = dd->bigBuffer->data7;
    dd->flags |= 0x2;
}

void GDI_CloseModule()
{
    STR_Close();
    GFX_End();
    TEX_List_Free(&TEX_gst_GlobalList);
    MAT_SpriteGen_Close();
}

struct sGdi_DisplayData* GDI_fnpst_CreateDisplayData()
{
    tGdi_DisplayData* dd = (tGdi_DisplayData*)MEM_p_AllocAligned(sizeof(tGdi_DisplayData), 64);

    memset(dd, 0, sizeof(tGdi_DisplayData));

    WTR_SetDefaulExp(&stExportWaterParrams);
    dd->drawMaskAllowedStuffCurrent = 0xffffff7f;
    dd->drawMaskAllowedStuff = 0xffffff7f;
    dd->RLIFactor = 1.0f;
    dd->ddcs2.pixelYoverX = 1.0f;
    dd->ddcs2.manualScreenRatio = 1.0f;
    dd->presentationFlags = 0;
    dd->shadowStruct = NULL;
    dd->extraMaterialToGeo = NULL;
    dd->ddcs2.flags2 = 8; // 8 width szerint stretch, 4 height szerint, 8 + 1: keretben van de a méretezés jó
    dd->ddcs2.selectedScreenRatio = 2;

    CAM_Engine_Init(&dd->ct);
    SOFT_l_MatrixStack_Create(&dd->matrixStack, 16);
    LIGHT_List_Init(&dd->lightList, 100);
    TEX_Manager_Init(&dd->texManager);
    
    dd->bigBuffer = SOFT_gpc_BigBuffer;
    memset(dd->bigBuffer, 0, offsetof(struct sBigBufferStruct, zlists)); // a tBigBufferStruct gap elemenek kozepeig van memset (eredeti: 480112), mi zlists-ig (480128-ig) cleareljük
    dd->rasters = (profilerRasterStructElem*)MEM_p_Alloc(47 * sizeof(profilerRasterStructElem));
    return dd;
}

void GDI_TransformUnsupportedRawPal(tGdi_DisplayData* dd, tEngTexListElemStruct* texListElem, tEngTexIntermediateStruct* dest, tEngTexIntermediateStruct* src)
{
    if ((src->tp.flagsUnkAndProceduralUnkb1 & 0x40) != 0)
        STR_FontAddAlternativeKey(TEX_gst_GlobalList.texElems[dest->textureID].texKey, texListElem->texKey);
    if (TEX_l_File_GetContent(TEX_gst_GlobalList.texElems[dest->textureID].texKey, src))
    {
        dest->imgDataPtr = (char*)MEM_p_AllocAligned(sizeof(uint32_t) * dest->tp.imageDataWidth * dest->tp.imageDataHeight, 64);
        MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetAligned(dest->imgDataPtr);

        if ((dd->texManager.flags & 0x100000) != 0)
        {
            if (src->colorDepth1 == 4)
            {
                TEX_Convert_4To32SwapRB((int*)dest->imgDataPtr, (unsigned char*)src->imgDataPtr, (int*)TEX_gst_GlobalList.palettes[dest->paletteID].paletteData, 
                    dest->tp.imageDataWidth * dest->tp.imageDataHeight);
            }
            else if (src->colorDepth1 == 8)
            {
                TEX_Convert_8To32SwapRB((int*)dest->imgDataPtr, (unsigned char*)src->imgDataPtr, (int*)TEX_gst_GlobalList.palettes[dest->paletteID].paletteData, 
                    dest->tp.imageDataWidth * dest->tp.imageDataHeight);
            }
        }
        else if (src->colorDepth1 == 4)
        {
            TEX_Convert_4To32((int*)dest->imgDataPtr, (unsigned char*)src->imgDataPtr, (int*)TEX_gst_GlobalList.palettes[dest->paletteID].paletteData,
                dest->tp.imageDataWidth * dest->tp.imageDataHeight);
        }
        else if (src->colorDepth1 == 8)
        {
            TEX_Convert_8To32((int*)dest->imgDataPtr, (unsigned char*)src->imgDataPtr, (int*)TEX_gst_GlobalList.palettes[dest->paletteID].paletteData, 
                dest->tp.imageDataWidth * dest->tp.imageDataHeight);
        }

        dest->colorDepth1 = 32;
        
        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(src->imgDataPtr);
        MEM_FreeAlign(src->imgDataPtr);
    }
}

void GDI_l_AttachWorld(struct sGdi_DisplayData* dxdd, struct sWorld* world)
{
    tMatSpriteElem* v9;
    tTexAnimatedElem* animTex;
    int16_t texIdx;     
    tEngTexIntermediateStruct* texElemsIntermediateStore;
    int elemNum;
    byte* usedTextureIndices;
    int16_t selectedTexIdx;
    int startIdx;
    BOOL restartNeeded = FALSE;
    int originalActTexElemNum;
    BOOL wasNewTextureAdded;
    int i, j;

    if (!dxdd) return;

    if (dxdd->CurrentWorld)
        GDI_l_DetachWorld(dxdd);

    gpst_DX_stDD = dxdd;

    dxdd->CurrentWorld = world;
    dxdd->fog.flags = 0;

    SOFT_ZList_Clear();

    if (!world) return;

    elemNum = TEX_gst_GlobalList.actTexElemNum;

    if (TEX_gst_GlobalList.actTexElemNum)
    {
        LOA_BeginSpeedMode(0xff800000);
        texElemsIntermediateStore = (tEngTexIntermediateStruct*)MEM_p_Alloc(sizeof(tEngTexIntermediateStruct) * TEX_gst_GlobalList.actTexElemNum);
        memset(texElemsIntermediateStore, 0, 4 * ((unsigned int)(sizeof(tEngTexIntermediateStruct) * TEX_gst_GlobalList.actTexElemNum) >> 2));
        GDI_f_Delay_AttachWorld_TextureUsed = 0.0f;
        GDI_f_Delay_AttachWorld_TextureUsed = 0.0f - TIM_f_Clock_TrueRead();
        usedTextureIndices = MAT_pc_GetAllUsedTextureIndex(dxdd, TEX_gst_GlobalList.actTexElemNum);
        originalActTexElemNum = TEX_gst_GlobalList.actTexElemNum;

        GDI_f_Delay_AttachWorld_TextureInfos = 0.0f;
        GDI_f_Delay_AttachWorld_TextureUsed = TIM_f_Clock_TrueRead() + GDI_f_Delay_AttachWorld_TextureUsed;
        GDI_f_Delay_AttachWorld_TextureInfos = 0.0f - TIM_f_Clock_TrueRead();

        startIdx = 0;
        wasNewTextureAdded = FALSE;
        selectedTexIdx = -1;

RESTART:
        for (i = startIdx; i < elemNum; i++)
        {
            if (wasNewTextureAdded || usedTextureIndices[i] != 2) {
                texElemsIntermediateStore[i].flags = 0x10;
            }
            if (!wasNewTextureAdded)
            {
                if (usedTextureIndices[i] != 1)
                    continue;
                usedTextureIndices[i] = 2;
            }
            if (TEX_l_File_GetInfo(TEX_gst_GlobalList.texElems[i].texKey, &texElemsIntermediateStore[i], 1))
            {
                if (texElemsIntermediateStore[i].tp.type == 4)
                {
                    TEX_gst_GlobalList.texElems[i].texElemFlag |= 0x20;
                    v9 = texElemsIntermediateStore[i].tp.matSpriteParams.matSprite;
                    TEX_gst_GlobalList.texElems[i].imgLink.ptr = v9;
                    selectedTexIdx = v9->texIdx;
                }
                else {
                    texElemsIntermediateStore[i].flags &= ~0x10;

                    if ((dxdd->texManager.flags & 0x8) == 0)
                        texElemsIntermediateStore[i].tp.flagsUnkAndProceduralUnkb1 &= ~0x8;

                    if (texElemsIntermediateStore[i].tp.type == 5)
                    {
                        TEX_gst_GlobalList.texElems[i].normalData.width = texElemsIntermediateStore[i].realWidth;
                        TEX_gst_GlobalList.texElems[i].normalData.height = texElemsIntermediateStore[i].realHeight;
                        TEX_Procedural_Add(TEX_gst_GlobalList.texElems[i].textElemIdx, texElemsIntermediateStore[i].tp.proceduralParams.interfaceIdx,
                            texElemsIntermediateStore[i].tp.proceduralParams.flags, texElemsIntermediateStore[i].tp.proceduralParams.interfacePtr);
                    }
                    else {
                        if (texElemsIntermediateStore[i].tp.type != 9)
                        {
                            if (texElemsIntermediateStore[i].tp.type == 7)
                            {
                                TEX_gst_GlobalList.texElems[i].texElemFlag |= 0x40;
                                selectedTexIdx = TEX_w_Manager_ChooseGoodInTex(&dxdd->texManager, &texElemsIntermediateStore[i]);
                            }
                        }
                        else {
                            TEX_gst_GlobalList.texElems[i].texElemFlag |= 0x80;
                            animTex = TEX_pst_Anim_Add(TEX_gst_GlobalList.texElems[i].textElemIdx, texElemsIntermediateStore[i].tp.animatedParams.num,
                                texElemsIntermediateStore[i].tp.proceduralParams.flags, texElemsIntermediateStore[i].tp.animatedParams.aiePtr);
                            if (animTex->texNum) {

                                for (j = 0; j < animTex->texNum; j++) {
                                    texIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, animTex->texPtr[j].key);
                                    animTex->texPtr[j].texIdx = texIdx;
                                    if (texIdx < originalActTexElemNum && !usedTextureIndices[texIdx])
                                    {
                                        usedTextureIndices[texIdx] = 1;
                                        if (texIdx < i)
                                            restartNeeded = TRUE;
                                    }
                                }

                                TEX_gst_GlobalList.texElems[i].animatedData.textureID = animTex->texPtr->texIdx;
                                selectedTexIdx = -1;
                                if (restartNeeded)
                                    goto RESTART;
                            }
                        }
                    }
                }

                if (selectedTexIdx != -1)
                {
                    if (selectedTexIdx < originalActTexElemNum && !usedTextureIndices[selectedTexIdx])
                    {
                        usedTextureIndices[selectedTexIdx] = 1;
                        if (selectedTexIdx < i)
                            goto RESTART;
                    }
                    selectedTexIdx = -1;
                }
            }
        }

        if (elemNum != TEX_gst_GlobalList.actTexElemNum) {
            // -- Zero out space for new textures --
            texElemsIntermediateStore = (tEngTexIntermediateStruct*)MEM_p_Realloc(texElemsIntermediateStore, sizeof(tEngTexIntermediateStruct) * TEX_gst_GlobalList.actTexElemNum);

            for (i = elemNum; i < TEX_gst_GlobalList.actTexElemNum; i++) {
                memset(&texElemsIntermediateStore[i], 0, sizeof(tEngTexIntermediateStruct));
            }

            wasNewTextureAdded = TRUE;
            startIdx = elemNum;
            elemNum = TEX_gst_GlobalList.actTexElemNum;

            goto RESTART;
        }


        // -- Palette load --
        for (i = 0; i < TEX_gst_GlobalList.paletteNum; i++) {
            TEX_File_LoadPalette(&TEX_gst_GlobalList.palettes[i], dxdd->texManager.flags & 0x100000);
        }

        // TODO: / FIXME: width/height is not "width" and "height" in this case. maybe create a struct for this version
        for (i = 0; i < TEX_gst_GlobalList.actTexElemNum; i++) {
            if ((texElemsIntermediateStore[i].flags & 0x10) != 0 || texElemsIntermediateStore[i].tp.type != 7)
                continue;

            if ((texElemsIntermediateStore[i].probablyType & 2) != 0)
            {
                TEX_gst_GlobalList.texElems[i].palettedData.textureID = texElemsIntermediateStore[i].texIdx;
                TEX_gst_GlobalList.texElems[i].palettedData.paletteID = -1;
            }
            else
            {
                TEX_gst_GlobalList.texElems[i].palettedData.textureID = texElemsIntermediateStore[i].textureID;
                TEX_gst_GlobalList.texElems[i].palettedData.paletteID = texElemsIntermediateStore[i].paletteID;
                if ((texElemsIntermediateStore[i].probablyType & 4) != 0)
                {
                    if (TEX_gst_GlobalList.texElems[i].palettedData.textureID != -1)
                    {
                        texElemsIntermediateStore[TEX_gst_GlobalList.texElems[i].palettedData.textureID].flags |= 0x10;
                        texElemsIntermediateStore[i].tp.imageDataColorDepth = texElemsIntermediateStore[TEX_gst_GlobalList.texElems[i].palettedData.textureID].tp.imageDataColorDepth;
                        texElemsIntermediateStore[i].tp.imageDataWidth = texElemsIntermediateStore[TEX_gst_GlobalList.texElems[i].palettedData.textureID].tp.imageDataWidth;
                        texElemsIntermediateStore[i].tp.imageDataHeight = texElemsIntermediateStore[TEX_gst_GlobalList.texElems[i].palettedData.textureID].tp.imageDataHeight;
                        TEX_File_DevelopParams(&texElemsIntermediateStore[i]);
                        continue;
                    }
                }
            }
            texElemsIntermediateStore[i].flags |= 0x10;
        }

        GDI_f_Delay_AttachWorld_TextureManager = 0.0f;
        GDI_f_Delay_AttachWorld_TextureInfos = TIM_f_Clock_TrueRead() + GDI_f_Delay_AttachWorld_TextureInfos;
        GDI_f_Delay_AttachWorld_TextureManager = 0.0f - TIM_f_Clock_TrueRead();
        TEX_Manager_Reinit(&dxdd->texManager);
        TEX_Manager_ComputeCompression(&dxdd->texManager, texElemsIntermediateStore, elemNum, 0);
        TEX_Manager_ComputeCompression(&dxdd->texManager, texElemsIntermediateStore, elemNum, 1);
        GDI_f_Delay_AttachWorld_TextureCreate = 0.0f;
        GDI_f_Delay_AttachWorld_TextureCreate_GetContent = 0.0f;
        GDI_f_Delay_AttachWorld_TextureManager = TIM_f_Clock_TrueRead() + GDI_f_Delay_AttachWorld_TextureManager;
        GDI_f_Delay_AttachWorld_TextureCreate_Compress = 0.0f;
        GDI_f_Delay_AttachWorld_TextureCreate_Convert24To32 = 0.0f;
        GDI_f_Delay_AttachWorld_TextureCreate_Convert32Colors = 0.0f;
        dword_B09A28 = 0;
        dword_B09A24 = 0;
        GDI_f_Delay_AttachWorld_TextureCreate = 0.0f - TIM_f_Clock_TrueRead();

        if (dxdd->TextureStore(dxdd, elemNum))
        {
            // -- Texture load --
            for (i = 0; i < elemNum; i++) {
                if ((texElemsIntermediateStore[i].flags & 0x10) == 0 && (TEX_gst_GlobalList.texElems[i].texElemFlag & 0x80) == 0)
                {
                    if ((texElemsIntermediateStore[i].tp.flagsUnkAndProceduralUnkb1 & 0x200) != 0 && (dxdd->texManager.flags & 0x200000) != 0)
                        dxdd->texManager.elemsFlagsWithFlag2++;
                    else
                        dxdd->texManager.elemsFlagsWithoutFlag2++;
                    if (texElemsIntermediateStore[i].tp.type == 7 && (texElemsIntermediateStore[i].flags & 0x20) == 0) {
                        GDI_TransformUnsupportedRawPal(dxdd, &TEX_gst_GlobalList.texElems[i], &texElemsIntermediateStore[i],
                            &texElemsIntermediateStore[texElemsIntermediateStore[i].textureID]);
                    }
                    TEX_Hardware_Load(dxdd, &TEX_gst_GlobalList.texElems[i], &texElemsIntermediateStore[i], i);
                }
            }

            // -- Texture unload (after conversion) --
            for (i = 0; i < TEX_gst_GlobalList.paletteNum; i++) {
                dxdd->TextureUnload(dxdd, &TEX_gst_GlobalList.palettes[i], i);
            }

            // -- Set Textures --
            for (i = 0; i < elemNum; i++) {
                if (texElemsIntermediateStore[i].colorDepth2 <= 8 && (TEX_gst_GlobalList.texElems[i].texElemFlag & 0x40) != 0)
                    dxdd->SetTextureTarget(dxdd, i, TEX_gst_GlobalList.texElems[i].normalData.width, TEX_gst_GlobalList.texElems[i].normalData.height);
            }

            TEX_Procedural_AfterLoad(dxdd);
        }

        GDI_f_Delay_AttachWorld_TextureCreate = TIM_f_Clock_TrueRead() + GDI_f_Delay_AttachWorld_TextureCreate;
        MEM_Free(usedTextureIndices);
        MEM_Free(texElemsIntermediateStore);

        // -- Free palette data --
        for (i = 0; i < TEX_gst_GlobalList.paletteNum; i++) {
            if ((TEX_gst_GlobalList.palettes[i].flags & 0x10) == 0)
            {
                if (TEX_gst_GlobalList.palettes[i].paletteData)
                {
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(TEX_gst_GlobalList.palettes[i].paletteData);
                    MEM_FreeAlign(TEX_gst_GlobalList.palettes[i].paletteData);
                }
                TEX_gst_GlobalList.palettes[i].paletteData = NULL;
            }
        }

        STR_FontResolveAlternativeKey();
        WOR_World_Attached(world);
        LOA_EndSpeedMode();
    }
}

void DisplayAttach(struct sGdi_DisplayData* dd)
{
    if (MAI_gst_MainHandles.currentWorld)
    {
        if (!dd->CurrentWorld
            || dd->CurrentWorld != MAI_gst_MainHandles.currentWorld
            || strcmp(MAI_gst_MainHandles.currentWorld->worldName, asz_PrevWorldName)
            || ENG_gb_ForceAttach)
        {
            ENG_gb_ForceAttach = FALSE;
            strcpy(asz_PrevWorldName, MAI_gst_MainHandles.currentWorld->worldName);           
            GDI_l_AttachWorld(dd, MAI_gst_MainHandles.currentWorld);                // texture load
            dd->CurrentWorld = MAI_gst_MainHandles.currentWorld;
        }
    }
}

void GDI_AfterDisplay(tGdi_DisplayData* dxdd)
{
    PRO_PerfCounterStartBlock(&dxdd->rasters[16]);

    gpst_DX_stDD = dxdd;

    if ((dxdd->presentationFlags & 0x20000000) != 0)
    {
        if (dxdd->CurrentWorld)
            GFX_Test(dxdd->CurrentWorld->gfxCurrentList);
    }

    if ((dxdd->presentationFlags & 0x200080) != 0)
        dxdd->presentationFlags &= ~0x200000;
    else
        dxdd->Flip();

    PRO_PerfCounterEndBlock(&dxdd->rasters[16]);
}

void GDI_BeforeDisplay(tGdi_DisplayData* dxdd)
{
    int clearColor;

    PRO_PerfCounterStartBlock(&dxdd->rasters[15]);

    gpst_DX_stDD = dxdd;
    D3D_BeginSceneCall();

    if ((dxdd->presentationFlags & 0x80) == 0)
    {
        if (dxdd->CurrentWorld)
            clearColor = dxdd->CurrentWorld->clearColor;
        else
            clearColor = 0x00000000;
     
        if (GFX_gl_FS_VideoPlaying)
        {
            /* The video covers the frame later on, so clear to black
             * and let what is left over read as the border. */
            dxdd->Clear(3, 0x00000000);
            dxdd->presentationFlags &= ~0x400000;
        }
        else if ((dxdd->presentationFlags & 0x400000) != 0)
        {
            dxdd->Clear(2, clearColor);
            dxdd->presentationFlags &= ~0x400000;
        }
        else
        {
            dxdd->Clear(3, clearColor);
        }
        
        dxdd->Request(1, &dxdd->fog);        
        ++dxdd->rasters[4].unk1;
    }
    // TODO FIXME biztos jók ezek?
    dxdd->rasters[0].unk2 = (float)dxdd->winWidth;
    dxdd->rasters[1].unk2 = (float)dxdd->winHeight;
    dxdd->rasters[2].unk2 = (float)dxdd->rasters[4].unk1;

    CAM_AssignCameraToDevice(&dxdd->ddcs2, &dxdd->ct, dxdd->winWidth, dxdd->winHeight);
    dxdd->SetProjectionMatrix(&dxdd->ct.cam);
    CAM_Inverse(&dxdd->ct);
    SOFT_MatrixStack_Reset(&dxdd->matrixStack, &dxdd->ct.cam.cameraMtx);
    LIGHT_List_Reset(&dxdd->lightList);

    PRO_PerfCounterEndBlock(&dxdd->rasters[15]);    
}

void GDI_fnl_InitInterface(struct sGdi_DisplayData* dd)
{
    dd->CreateDevice = D3D_pst_CreateDevice;
    dd->DestroyDevice = D3D_DestroyDevice;
    dd->Init = D3D_l_Init;
    dd->Close = D3D_l_Close;
    dd->ReadaptDisplay = D3D_l_ReadaptDisplay;
    dd->Clear = D3D_Clear;
    dd->Flip = D3D_Flip;
    dd->SetViewMatrix = D3D_SetViewMatrix;
    dd->SetProjectionMatrix = D3D_SetProjectionMatrix;
    dd->DrawElementIndexedTriangles = D3D_l_DrawElementIndexedTriangles;
    dd->DrawSprites = D3D_DrawSprites;
    dd->Request = D3D_l_Request;
    dd->TextureInit = D3D_l_Texture_Init;
    dd->TextureStore = D3D_l_Texture_Store;
    dd->PaletteLoad = D3D_Palette_Load;
    dd->SetTexturePalette = D3D_Set_Texture_Palette;
    dd->TextureUnload = D3D_Texture_Unload;
    dd->SetTextureTarget = D3D_SetTextureTarget;
    dd->D3D_ReleaseTextures = D3D_ReleaseTextures;
    dd->D3D_SetRenderTargetAndBackupPreviousSDW = D3D_SetRenderTargetAndBackupPreviousSDW;
    dd->SetViewMatrixSDW = D3D_SetViewMatrix_SDW;
}

void GDI_DetachDisplay(struct sGdi_DisplayData* dd)
{
    RECT rect;

    if (GDI_gl_ReadaptOperation)
        GetClientRect(dd->hwnd, &rect);

    dd->hwnd = NULL;

    if (!GDI_gl_ReadaptOperation)
    {
        GDI_l_DetachWorld(dd);
        dd->Close(dd);
    }
}

void GDI_AttachDisplay(tGdi_DisplayData* dd, HWND hwnd)
{
    float biggerDimension;

    if (dd->hwnd)
        GDI_DetachDisplay(dd);
    
    dd->hwnd = hwnd;
    dd->Init(dd->hwnd, dd);
    dd->winWidth = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
    dd->winHeight = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;

    if (dd->winWidth <= dd->winHeight)
        biggerDimension = (float)dd->winHeight;
    else
        biggerDimension = (float)dd->winWidth;

    dd->NormalizedWidthOrHeightMultBy255 = (1.0f / biggerDimension * 255.0f);
    CAM_AssignCameraToDevice(&dd->ddcs2, &dd->ct, dd->winWidth, dd->winHeight);
}

BOOL GDI_AttachDisplayAndReadaptDisplay(struct sGdi_DisplayData* gdd, HWND hwnd)
{
    tWorld* worldBackup;
    int ret;

    GDI_gl_ReadaptOperation = TRUE;
    GDI_AttachDisplay(gdd, hwnd);
    GDI_gl_ReadaptOperation = FALSE;
    
    worldBackup = gdd->CurrentWorld;
    
    GDI_l_DetachWorld(gdd);
    
    ret = gdd->ReadaptDisplay(hwnd, gdd);
    if (ret)
    {
        GDI_l_AttachWorld(gdd, worldBackup);
        return TRUE;
    }
    
    return ret;
}

void GDI_DestroyDisplayData(struct sGdi_DisplayData* dd)
{
    if (!dd) return;

    gpst_DX_stDD = dd;
    SDW_Destroy();
    SOFT_l_MatrixStack_Destroy(&dd->matrixStack);
    LIGHT_List_Free(&dd->lightList);
    PRO_ClearAllRasters(dd->rasters);
    MEM_Free(dd->rasters);
    MEM_FreeAlign(dd);

}

void s_Display(HWND a1, tGdi_DisplayData* dxdd)
{
    DisplayAttach(dxdd);
    GDI_BeforeDisplay(dxdd);

    dxdd->CurrentWorld = MAI_gst_MainHandles.currentWorld;

    if (MAI_gst_MainHandles.currentWorld != NULL)
    {
        dxdd->CurrentWorld->viewPtr->displayData = dxdd;
        WOR_Render(dxdd->CurrentWorld, dxdd);
    }

    /* After the world and before the flip, so the frame goes out over
     * whatever was drawn. */
    GFX_FS_VideoUpdate();

    GDI_AfterDisplay(dxdd);
}

void GDI_InitModule()
{
    GRO_Struct_InitInterfaces();
    MAT_Init();
    LIGHT_Init();
    GEO_Init();
    GEO_StaticLOD_Init();
    CAM_Init();
    WAY_Init();
    STR_Init();
    PAG_Init();
    TEX_List_Init(&TEX_gst_GlobalList);
    TEX_Procedural_Init();
    SOFT_ZList_Init();
}
