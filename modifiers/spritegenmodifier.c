#include "../memory.h"
#include "../loading.h"
#include "../sprites.h"
#include "spritegenmodifier.h"
#include "../materialanddraw.h"

void SPG_Modifier_Unapply(tModifier* mdf, tGEOGRO* geogro)
{
    tSpgMdfStruct* spg;
    float distCalculated;
    int i, j;
    unsigned int k, l;
    float specialFogFarCorrected;
    unsigned int v34;
    tGeoSprites* v35;
    unsigned int v36;
    int v37;
    Vec3* verticesCurrentBackup;
    int v39;
    int v40;
    unsigned int colors[3];
    tSpgTrisToSpritesStruct ttspr;
    Vec3 point1;
    Vec3 point2;
    Vec3 point3;
    tGeoTriangleChunkElem* tcElem;

    spg = (tSpgMdfStruct*)(mdf->mdfPrivateDataPtr);

    distCalculated = (float)mdf->gao->distance * spg->LODCorrectionMultiplier / 64.0f;
    if (distCalculated > 1.0f)
    {
        distCalculated = 1.0f;
    }
    else if (distCalculated <= 0.0f) {
        distCalculated = 0.0f;
    }
    distCalculated *= distCalculated;

    if (distCalculated < 0.5f) return;

    if ((spg->flags & 1) != 0)
    {
        if ((gpst_DX_stDD->flags & 0x4) == 0)
        {
            SOFT_AddCurrentObjectInZList(gpst_DX_stDD, mdf->gao);
            return;
        }
    }
    else if ((gpst_DX_stDD->flags & 0x4) != 0)
    {
        return;
    }

    if (spg->material == 0xffffffff)
        spg->material = 0;

    QueryPerformanceCounter(&PerfCounterStruct[42]);


    pst_CurrentObj = geogro;
    if (spg->material)
        pst_CurrentMat = (tGEOMAT*)(spg->material);
    else
        pst_CurrentMat = (tGEOMAT*)(((tGameObjectVisualObject*)(mdf->gao->basic->actorObjectOrVisualObject))->GROMAT);
    SPG_stIndxSp[0].globalRatio = spg->globalRatio;
    SPG_stIndxSp[0].globalSize = spg->globalSize;
    SPG_stIndxSp[0].num = geogro->vertexNum;
    if (SPG_stIndxSp[0].num > 256)
        SPG_stIndxSp[0].num = 256;
    SPG_stIndxSp[0].matIdx = 0;
    SPG_stIndxSp[0].spriteElems = SPG_SpritesIDX;

    i = geogro->vertexNum;

    if (spg->depth > 8)
        spg->depth = 8;

    spg->depth = spg->depth < 0 ? 0 : spg->depth;

    while (i)
    {
        j = i;
        if (i > 256)
            j = 256;
        i -= j;
        do
        {
            j--;
            SPG_SpritesIDX[j].size = 1.0f;
            SPG_SpritesIDX[j].idx = i + j;
        } while (j);
    }

    SPG_stIndxSp[1].num = 64;
    SPG_stIndxSp[2].num = 64;
    SPG_stIndxSp[3].num = 64;
    SPG_stIndxSp[4].num = 64;
    SPG_stIndxSp[0].matIdx = (uint8_t)spg->subElementData[0].matID;
    SPG_stIndxSp[1].matIdx = (uint8_t)spg->subElementData[1].matID;
    SPG_stIndxSp[2].matIdx = (uint8_t)spg->subElementData[2].matID;
    SPG_stIndxSp[3].matIdx = (uint8_t)spg->subElementData[3].matID;
    SPG_stIndxSp[0].globalSize = spg->subElementData[0].sizeMultiplier * spg->globalSize * distCalculated;
    SPG_stIndxSp[1].globalSize = spg->subElementData[1].sizeMultiplier * spg->globalSize * distCalculated;
    SPG_stIndxSp[2].globalSize = spg->subElementData[2].sizeMultiplier * spg->globalSize * distCalculated;
    SPG_stIndxSp[3].globalSize = spg->subElementData[3].sizeMultiplier * spg->globalSize * distCalculated;
    SPG_stIndxSp[0].globalRatio = spg->subElementData[0].ratioMultiplier * spg->globalRatio;
    SPG_stIndxSp[1].globalRatio = spg->subElementData[1].ratioMultiplier * spg->globalRatio;
    SPG_stIndxSp[2].globalRatio = spg->subElementData[2].ratioMultiplier * spg->globalRatio;
    SPG_stIndxSp[3].globalRatio = spg->subElementData[3].ratioMultiplier * spg->globalRatio;
    SPG_stIndxSp[1].spriteElems = SPG_stIndxSp[0].spriteElems + 64;
    SPG_stIndxSp[2].spriteElems = SPG_stIndxSp[0].spriteElems + 128;
    SPG_stIndxSp[3].spriteElems = SPG_stIndxSp[0].spriteElems + 192;

    for (i = 0; i < 256; i++) {
        SPG_SpritesIDX[i].size = 1.0f;
    }

    for (i = 64; i >= 0; i--)
    {
        SPG_SpritesIDX[i].size = SPG_SpritesIDX[i].size - f_RandomTable[(uint8_t)i] * spg->subElementData[0].noiseMultiplier;
        SPG_SpritesIDX[i + 64].size = SPG_SpritesIDX[i + 64].size - f_RandomTable[(uint8_t)i] * spg->subElementData[1].noiseMultiplier;
        SPG_SpritesIDX[i + 128].size = SPG_SpritesIDX[i + 128].size - f_RandomTable[(uint8_t)i] * spg->subElementData[2].noiseMultiplier;
        SPG_SpritesIDX[i + 192].size = SPG_SpritesIDX[i + 192].size - f_RandomTable[(uint8_t)i] * spg->subElementData[3].noiseMultiplier;

        SPG_SpritesIDX[i].idx = 4 * i + (geogro->vertexNum & 0xffff);
        SPG_SpritesIDX[i + 64].idx = 4 * i + (geogro->vertexNum & 0xffff) + 1;
        SPG_SpritesIDX[i + 128].idx = 4 * i + (geogro->vertexNum & 0xffff) + 2;
        SPG_SpritesIDX[i + 192].idx = 4 * i + (geogro->vertexNum & 0xffff) + 3;
    }

    SPG_gvCurrentStack = SPG_gvStack - geogro->vertexNum;
    SPG_gvCOLORStack = (int*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * geogro->vertexNum];

    if ((spg->flags & 0x10) != 0)
        GDI_gb_WaveSprite = TRUE;

    if ((spg->flags & 4) != 0)
    {
        SPG_gs_SpecialFogCOLOR = spg->fogColor & ~0x100 | 0x1;
        if ((spg->flags & 8) != 0)
            SPG_gs_SpecialFogCOLOR |= 0x100;
        SPG_gs_SpecialFogNear = spg->specialFogNear;
        specialFogFarCorrected = spg->specialFogFar;
        if (specialFogFarCorrected == SPG_gs_SpecialFogNear)
            specialFogFarCorrected = SPG_gs_SpecialFogNear + 0.1f;
        SPG_gs_SpecialFogFar = (1.0f / (specialFogFarCorrected - SPG_gs_SpecialFogNear));
        SPG_gs_SpecialFogAxis.x = SPG_gs_SpecialFogFar * gpst_DX_stDD->matrixStack.actMatrixElem->v1.z;
        SPG_gs_SpecialFogAxis.y = SPG_gs_SpecialFogFar * gpst_DX_stDD->matrixStack.actMatrixElem->v2.z;
        SPG_gs_SpecialFogAxis.z = SPG_gs_SpecialFogFar * gpst_DX_stDD->matrixStack.actMatrixElem->v3.z;
        SPG_gs_SpecialFogCulledPoint.z = 100000.0;
        SPG_gs_SpecialFogCulledPoint.y = 100000.0;
        SPG_gs_SpecialFogCulledPoint.x = 100000.0;
        SPG_gs_SpecialFogDistance = (SPG_gs_SpecialFogNear - gpst_DX_stDD->matrixStack.actMatrixElem->Pos.z) * SPG_gs_SpecialFogFar;
    }
    else
    {
        SPG_gs_SpecialFogCOLOR = 0x00000000;
    }

    if (spg->depth)
    {
        ttspr.f1 = spg->threshold * spg->threshold;
        SPG_FIndx = 10000;
        gvBase = 0;
        ttspr.maxDivide = spg->depth;
        ttspr.noise = spg->noise;

        GEO_UseNormals(geogro);

        for (k = 0; k < geogro->triangleChunkCount; k++)
        {
            if (((1 << geogro->triangleChunks[k].materialNum) & spg->materialMask) != 0)
            {
                for (l = 0; l < geogro->triangleChunks[k].triangleNum; l++)
                {
                    tcElem = &geogro->triangleChunks[k].triangleChunkElems[l];
                    colors[0] = *(uint32_t*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * tcElem->v1];
                    colors[1] = *(uint32_t*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * tcElem->v2];
                    colors[2] = *(uint32_t*)&gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow[4 * tcElem->v3];

                    ttspr.flags = 0;

                    if (tcElem->v1 > tcElem->v2)
                        ttspr.flags = 0x1;
                    if (tcElem->v2 > tcElem->v3)
                        ttspr.flags |= 0x2;
                    if (tcElem->v3 > tcElem->v1)
                        ttspr.flags |= 0x4;

                    point1.x = geogro->normals[tcElem->v1].x * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v1].x;
                    point1.y = geogro->normals[tcElem->v1].y * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v1].y;
                    point1.z = geogro->normals[tcElem->v1].z * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v1].z;
                    point2.x = geogro->normals[tcElem->v2].x * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v2].x;
                    point2.y = geogro->normals[tcElem->v2].y * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v2].y;
                    point2.z = geogro->normals[tcElem->v2].z * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v2].z;
                    point3.x = geogro->normals[tcElem->v3].x * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v3].x;
                    point3.y = geogro->normals[tcElem->v3].y * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v3].y;
                    point3.z = geogro->normals[tcElem->v3].z * spg->extraction + gpst_DX_stDD->verticesCurrent[tcElem->v3].z;
                    SPG_TransformTrisToSprites(&point1, &point2, &point3, colors, &ttspr);
                }
            }
        }
        if (gvBase)
        {
            v34 = gvBase & 3;
            SPG_stIndxSp[0].num = gvBase >> 2;
            SPG_stIndxSp[1].num = gvBase >> 2;
            SPG_stIndxSp[2].num = gvBase >> 2;
            SPG_stIndxSp[3].num = gvBase >> 2;
            if ((gvBase & 3) != 0)
            {
                v35 = &SPG_stIndxSp[v34];
                v36 = gvBase & 3;
                do
                {
                    v37 = v35[-1].num;
                    --v35;
                    --v34;
                    --v36;
                    v35->num = v37 + 1;
                } while (v36);
            }
            verticesCurrentBackup = gpst_DX_stDD->verticesCurrent;

            gvBase = v34 - 1;
            gpst_DX_stDD->verticesCurrent = SPG_gvCurrentStack;

            v39 = SPG_stIndxSp[0].num + SPG_stIndxSp[3].num + SPG_stIndxSp[1].num + SPG_stIndxSp[2].num;
            if (SPG_gs_SpecialFogCOLOR)
            {
                v40 = SPG_stIndxSp[0].num + SPG_stIndxSp[3].num + SPG_stIndxSp[1].num + SPG_stIndxSp[2].num;
                if ((SPG_gs_SpecialFogCOLOR & 0x100) != 0)
                    SPG_ComputeSpriteFog_Cull(SPG_gvStack, &SPG_gvCOLORStack[-v39], v40);
                else
                    SPG_ComputeSpriteFog(SPG_gvStack, &SPG_gvCOLORStack[-v39], v40);
            }
            if (SPG_stIndxSp[0].num)
                MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[0]);
            if (SPG_stIndxSp[1].num)
                MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[1]);
            if (SPG_stIndxSp[2].num)
                MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[2]);
            if (SPG_stIndxSp[3].num)
                MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[3]);
            gpst_DX_stDD->verticesCurrent = verticesCurrentBackup;
        }
    }
    GDI_gb_WaveSprite = FALSE;
    PRO_QPC_RecordTimeDecRef(42);
}

void SPG_InitModifier(tSpgMdfStruct* mdf)
{
    mdf->noise = 0.0f;
    mdf->i1 = -1;
    mdf->material = 0xffffffff;
    mdf->materialMask = 0xffffffff;
    mdf->globalRatio = 1.0f;
    mdf->globalSize = 0.0099999998f;
    mdf->depth = 2;

    memset(mdf->spriteMapper, 0, sizeof(mdf->spriteMapper));

    mdf->subElementData[0].noiseMultiplier = 0.0f;
    mdf->subElementData[0].matID = 0;
    mdf->extraction = 0.0f;
    mdf->threshold = 0.0f;
    mdf->subElementData[0].ratioMultiplier = 1.0f;
    mdf->subElementData[0].sizeMultiplier = 1.0f;
    mdf->subElementData[1].matID = mdf->subElementData[0].matID;
    mdf->subElementData[1].sizeMultiplier = mdf->subElementData[0].sizeMultiplier;
    mdf->subElementData[1].noiseMultiplier = mdf->subElementData[0].noiseMultiplier;
    mdf->subElementData[1].ratioMultiplier = mdf->subElementData[0].ratioMultiplier;
    mdf->subElementData[2].matID = mdf->subElementData[0].matID;
    mdf->subElementData[2].sizeMultiplier = mdf->subElementData[0].sizeMultiplier;
    mdf->subElementData[2].noiseMultiplier = mdf->subElementData[0].noiseMultiplier;
    mdf->subElementData[2].ratioMultiplier = mdf->subElementData[0].ratioMultiplier;
    mdf->subElementData[3].matID = mdf->subElementData[0].matID;
    mdf->subElementData[3].sizeMultiplier = mdf->subElementData[0].sizeMultiplier;
    mdf->subElementData[3].noiseMultiplier = mdf->subElementData[0].noiseMultiplier;
    mdf->subElementData[3].ratioMultiplier = mdf->subElementData[0].ratioMultiplier;
    mdf->LODCorrectionMultiplier = 1.0f;
    mdf->flags = 1;
}

void SPG_Modifier_Create(struct sGameObjectMain* a1, struct sModifier* mdf, void* modifierDataToBeSet)
{
    int i;

    mdf->mdfPrivateDataPtr = (tSpgMdfStruct*)MEM_p_Alloc(sizeof(tSpgMdfStruct));
    memset(mdf->mdfPrivateDataPtr, 0, sizeof(tSpgMdfStruct));
    
    if (!ulRandomInit)
    {
        ulRandomInit = 1;
        for (i = 255; i >= 0; i--) {
            f_RandomTable[i] = (RND_JADErand() & 0xff) / 256.0f - 0.5f;
        }        
    }

    if (modifierDataToBeSet)
        memcpy(mdf->mdfPrivateDataPtr, modifierDataToBeSet, sizeof(tSpgMdfStruct));
    else
        SPG_InitModifier(mdf->mdfPrivateDataPtr);
}

void SPG_Modifier_Destroy(tModifier* mdf)
{
    MEM_Free(mdf->mdfPrivateDataPtr);
}

int SPG_ul_Modifier_Load(struct sModifier* mdf, char* readptr)
{
    tSpgMdfStruct* spg;
    unsigned int paramVer;
    char* currReadPtr;
    int i;

    spg = (tSpgMdfStruct*)mdf->mdfPrivateDataPtr;
    currReadPtr = readptr;
    
    LOA_ReadLong(&currReadPtr, 0, 1);
    paramVer = LOA_ReadLong(&currReadPtr, 0, 1);
    
    SPG_InitModifier(spg);
    
    spg->globalSize = LOA_ReadFloat(&currReadPtr, 0, 1);
    spg->globalRatio = LOA_ReadFloat(&currReadPtr, 0, 1);
    LOA_ReadLong(&currReadPtr, 0, 1);
    spg->i1 = -1;
    if (paramVer)
    {
        spg->extraction = LOA_ReadFloat(&currReadPtr, 0, 1);
        spg->threshold = LOA_ReadFloat(&currReadPtr, 0, 1);
        spg->depth = LOA_ReadLong(&currReadPtr, 0, 1);
        if (paramVer > 1)
        {
            spg->materialMask = LOA_ReadLong(&currReadPtr, 0, 1);
            if (paramVer > 2)
            {
                spg->noise = LOA_ReadFloat(&currReadPtr, 0, 1);
                if (paramVer > 3)
                {
                    for (i = 0; i < 4; i++)
                    {
                        LOA_ReadSpriteMapper(&currReadPtr, &spg->spriteMapper[i], 1);
                    }
                    if (paramVer > 4)
                    {
                        for (i = 0; i < 4; i++)
                        {
                            spg->subElementData[i].matID = LOA_ReadLong(&currReadPtr, 0, 1);
                            spg->subElementData[i].sizeMultiplier = LOA_ReadFloat(&currReadPtr, 0, 1);
                            spg->subElementData[i].noiseMultiplier = LOA_ReadFloat(&currReadPtr, 0, 1);
                            spg->subElementData[i].ratioMultiplier = LOA_ReadFloat(&currReadPtr, 0, 1);
                        }

                        if (paramVer > 5)
                        {
                            spg->LODCorrectionMultiplier = LOA_ReadFloat(&currReadPtr, 0, 1);
                            if (paramVer > 6)
                            {
                                spg->flags = LOA_ReadLong(&currReadPtr, 0, 1);
                                if (paramVer > 7)
                                {
                                    spg->specialFogNear = LOA_ReadFloat(&currReadPtr, 0, 1);
                                    spg->specialFogFar = LOA_ReadFloat(&currReadPtr, 0, 1);
                                    spg->fogColor = LOA_ReadLong(&currReadPtr, 0, 1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    spg->material = 0xffffffff;

    return currReadPtr - readptr;
}

void SPG_Modifier_Apply(tModifier* a1, tGEOGRO* a2) {
}