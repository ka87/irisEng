#include "../memory.h"
#include "../loading.h"
#include "../textures.h"
#include "../graphicsobjects/shadow.h"
#include "shadowmodifier.h"

void GAO_ModifierShadow_Create(struct sGameObjectMain* gao, tModifier* mdf, void* privateData)
{

    mdf->mdfPrivateDataPtr = (tMdfPrivateDataShadow*)MEM_p_Alloc(sizeof(tMdfPrivateDataShadow));
    if (privateData)
    {
        memcpy(mdf->mdfPrivateDataPtr, privateData, sizeof(tMdfPrivateDataShadow));
    }
    else
    {
        memset(mdf->mdfPrivateDataPtr, 0, sizeof(tMdfPrivateDataShadow));
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->texKey = -1;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->shadowTextureIdx = -1;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->flags = 0x45184;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->xSizeFactor = 1.0;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->ySizeFactor = 1.0;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->unkf1 = 1.0;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->zAttenuationFactor = 2.0;
        ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->shadowColor = 0;
    }
    
    ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->gao = gao;
    ((tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr))->nextShadowMdfPrivateData = pst_FirstShadowMdf;
    pst_FirstShadowMdf = mdf->mdfPrivateDataPtr;
}

void GAO_ModifierShadow_Destroy(struct sModifier* mdf)
{
    tMdfPrivateDataShadow* shadowMdfPrivateDataPtrToFind;
    tMdfPrivateDataShadow* shadowMdfPrivateDataPtr;
    tMdfPrivateDataShadow** currShadowMdfPrivateDataPPtr;

    shadowMdfPrivateDataPtrToFind = (tMdfPrivateDataShadow*)mdf->mdfPrivateDataPtr;
    currShadowMdfPrivateDataPPtr = &pst_FirstShadowMdf;
    
    // remove desired shadow modifier private data from chained list
    if (pst_FirstShadowMdf)
    {
        if (pst_FirstShadowMdf != shadowMdfPrivateDataPtrToFind)
        {
            do
            {
                shadowMdfPrivateDataPtr = (*currShadowMdfPrivateDataPPtr)->nextShadowMdfPrivateData;
                currShadowMdfPrivateDataPPtr = &(*currShadowMdfPrivateDataPPtr)->nextShadowMdfPrivateData;
            } while (shadowMdfPrivateDataPtr != shadowMdfPrivateDataPtrToFind);
        }
        *currShadowMdfPrivateDataPPtr = (*currShadowMdfPrivateDataPPtr)->nextShadowMdfPrivateData;
    }

    MEM_Free(mdf->mdfPrivateDataPtr);
}

int GAO_ModifierShadow_Load(struct sModifier* mdf, char* readPtr)
{
    tEngTexListElemStruct* texElem;
    tMdfPrivateDataShadow* sdw = (tMdfPrivateDataShadow*)mdf->mdfPrivateDataPtr;
    char* readPtrOrig = readPtr;

    LOA_ReadLong(&readPtr, 0, 2);
    //nextShadowMdfPrivateData and gao get unbound here
    sdw->flags = LOA_ReadLong(&readPtr, 0, 1);
    sdw->texKey = LOA_ReadLong(&readPtr, 0, 1);
    sdw->xSizeFactor = LOA_ReadFloat(&readPtr, 0, 1);
    sdw->ySizeFactor = LOA_ReadFloat(&readPtr, 0, 1);
    sdw->shadowTextureIdx = LOA_ReadLong(&readPtr, 0, 1);
    LOA_ReadLong(&readPtr, 0, 2);
    LOA_ReadLong(&readPtr, 0, 2);
    sdw->zAttenuationFactor = LOA_ReadFloat(&readPtr, 0, 1);
    sdw->shadowColor = LOA_ReadLong(&readPtr, 0, 1);
    sdw->zStart = LOA_ReadFloat(&readPtr, 0, 1);
    LOA_ReadVector(&readPtr, &sdw->centerPos, 1);
    sdw->unkf1 = LOA_ReadFloat(&readPtr, 0, 1);
    sdw->shadowProjectionMode = LOA_ReadLong(&readPtr, 0, 1);
    sdw->unk1 = LOA_ReadChar(&readPtr, 0, 1);
    LOA_ReadChar(&readPtr, 0, 2);
    LOA_ReadChar(&readPtr, 0, 2);
    LOA_ReadChar(&readPtr, 0, 2);
    sdw->flags &= ~0x40;
    sdw->shadowTextureIdx = -1;
    if (sdw->texKey && sdw->texKey != -1)
    {
        if ((texElem = TEX_pst_List_FindTexture(&TEX_gst_GlobalList, sdw->texKey)) != NULL)
        {
            sdw->shadowTextureIdx = texElem->textElemIdx;
            return readPtr - readPtrOrig;
        }
        sdw->shadowTextureIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, sdw->texKey);
    }
    return readPtr - readPtrOrig;
}

void GAO_ModifierShadow_Apply(struct sModifier* mdf, void* unk1) {
}

void GAO_ModifierShadow_Unapply(struct sModifier* mdf, void* unk1) {
}

void GAO_ModifierShadow_Reinit(struct sModifier* mdf) {
}

void GAO_ModifierShadow_DetectUsedTextures(char* texArray)
{
    tMdfPrivateDataShadow* currPtr;

    for (currPtr = pst_FirstShadowMdf; currPtr; currPtr = currPtr->nextShadowMdfPrivateData)
    {
        if (currPtr->shadowTextureIdx != -1)
            texArray[currPtr->shadowTextureIdx] = 1;
    }
}

void GAO_ModifierShadow_Apply_All()
{
    int i;
    tMdfPrivateDataShadow* privDataShadow;
    tGameObjectMain* currLightGao;
    tLIGHT* currLight;
    float colorFactor;
    float shadowLightSrcDist;
    tShadowElemStruct sdwelem;

    SDW_Clear();

    for (privDataShadow = pst_FirstShadowMdf; privDataShadow != NULL; privDataShadow = privDataShadow->nextShadowMdfPrivateData)
    {
        if ((privDataShadow->gao->flags2 & 1) != 0 && (privDataShadow->gao->flags2 & 2) != 0 && (privDataShadow->flags & 0x40000) != 0)
        {
            sdwelem.startFactor = 1.0f;

            if ((privDataShadow->flags & 4) != 0)
            {
                sdwelem.shadowMdfPtr = privDataShadow;
                sdwelem.ZDepth = privDataShadow->zAttenuationFactor;
                sdwelem.shadowVectorInWorldSpace.x = 0.0f;
                sdwelem.shadowVectorInWorldSpace.y = 0.0f;
                sdwelem.shadowVectorInWorldSpace.z = -1.0f;
                sdwelem.color = privDataShadow->shadowColor;
                SDW_AddAShadow(&sdwelem);
            }
            if ((privDataShadow->flags & 0x100000) != 0)
            {
                sdwelem.shadowMdfPtr = privDataShadow;
                sdwelem.ZDepth = privDataShadow->zAttenuationFactor;
                sdwelem.shadowVectorInWorldSpace = privDataShadow->gao->matrices->v1;
                sdwelem.color = privDataShadow->shadowColor;

                if ((privDataShadow->flags & 0x800000) != 0)
                {
                    MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                }
                SDW_AddAShadow(&sdwelem);
            }
            if ((privDataShadow->flags & 0x200000) != 0)
            {
                sdwelem.shadowMdfPtr = privDataShadow;
                sdwelem.ZDepth = privDataShadow->zAttenuationFactor;
                sdwelem.shadowVectorInWorldSpace.x = privDataShadow->gao->matrices->v2.x;
                sdwelem.shadowVectorInWorldSpace.y = privDataShadow->gao->matrices->v2.y;
                sdwelem.shadowVectorInWorldSpace.z = privDataShadow->gao->matrices->v2.z;
                sdwelem.color = privDataShadow->shadowColor;
                if ((privDataShadow->flags & 0x800000) != 0)
                {
                    MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                }
                SDW_AddAShadow(&sdwelem);
            }
            if ((privDataShadow->flags & 0x400000) != 0)
            {
                sdwelem.shadowMdfPtr = privDataShadow;
                sdwelem.ZDepth = privDataShadow->zAttenuationFactor;
                sdwelem.shadowVectorInWorldSpace.x = privDataShadow->gao->matrices->v3.x;
                sdwelem.shadowVectorInWorldSpace.y = privDataShadow->gao->matrices->v3.y;
                sdwelem.shadowVectorInWorldSpace.z = privDataShadow->gao->matrices->v3.z;
                sdwelem.color = privDataShadow->shadowColor;
                if ((privDataShadow->flags & 0x800000) != 0)
                {
                    MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                }
                SDW_AddAShadow(&sdwelem);
            }
            if ((privDataShadow->flags & 3) != 0)
            {
                sdwelem.shadowMdfPtr = privDataShadow;
             
                for (i = gpst_DX_stDD->lightList.actualLightNum; i > 0; i--) {
                    currLightGao = gpst_DX_stDD->lightList.lights[i - 1];
                    currLight = (tLIGHT*)(currLightGao->extended->light);
                    if ((privDataShadow->flags & 0x1000000) != 0)
                        sdwelem.color = privDataShadow->shadowColor;
                    else
                        sdwelem.color = currLight->color;
                    sdwelem.ZDepth = sdwelem.shadowMdfPtr->zAttenuationFactor;
                    
                    if ((sdwelem.color & 0xf0f0f0) == 0)
                        continue;
                    if ((currLight->flags & 0x4000) == 0 || (currLight->flags & 8) == 0)
                        continue;
                    switch (currLight->flags & 7) {
                    case 0:
                        if ((privDataShadow->flags & 1) != 0)
                        {
                            MATH_SubstractVec(&sdwelem.shadowVectorInWorldSpace, &currLightGao->matrices->Pos, &privDataShadow->gao->matrices->Pos);
                            shadowLightSrcDist = MATH_VecLen(&sdwelem.shadowVectorInWorldSpace);
                            if (shadowLightSrcDist < currLight->ffar)
                            {
                                if (shadowLightSrcDist > (double)currLight->fnear)
                                {
                                    sdwelem.startFactor = (shadowLightSrcDist - currLight->fnear) / (currLight->ffar - currLight->fnear);
                                    sdwelem.color = LIGHT_ul_Interpol2Colors(currLight->color, 0, sdwelem.startFactor);
                                    sdwelem.startFactor = (float)(1.0 - sdwelem.startFactor * sdwelem.startFactor);
                                }

                                if (sdwelem.ZDepth > (currLight->ffar - shadowLightSrcDist))
                                    sdwelem.ZDepth = currLight->ffar - shadowLightSrcDist;

                                sdwelem.color = sdwelem.color ^ 0xffffff;

                                MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                                MATH_NormalizeVector(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);

                                if ((privDataShadow->flags & 0x800000) != 0)
                                {
                                    MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                                }
                                SDW_AddAShadow(&sdwelem);
                                sdwelem.startFactor = 1.0;
                            }
                        }
                        break;
                    case 1:
                        if ((privDataShadow->flags & 2) == 0)
                            continue;
                        sdwelem.color = sdwelem.color ^ 0xffffff;
                        MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &currLightGao->matrices->v2);
                        MATH_NormalizeVector(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                        if ((privDataShadow->flags & 0x800000) != 0)
                        {
                            MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                        }
                        SDW_AddAShadow(&sdwelem);
                        break;
                    case 2:
                        if ((privDataShadow->flags & 1) == 0)
                            continue;
                        MATH_SubstractVec(&sdwelem.shadowVectorInWorldSpace, &currLightGao->matrices->Pos, &privDataShadow->gao->matrices->Pos);
                        shadowLightSrcDist = MATH_VecLen(&sdwelem.shadowVectorInWorldSpace);
                        if (shadowLightSrcDist >= currLight->ffar)
                            continue;
                        if (shadowLightSrcDist > currLight->fnear)
                        {
                            colorFactor = (shadowLightSrcDist - currLight->fnear) / (currLight->ffar - currLight->fnear);
                            sdwelem.color = LIGHT_ul_Interpol2Colors(currLight->color, 0, colorFactor);
                        }

                        if (sdwelem.ZDepth > currLight->ffar - shadowLightSrcDist)
                            sdwelem.ZDepth = currLight->ffar - shadowLightSrcDist;

                        sdwelem.color = sdwelem.color ^ 0xffffff;
                        MATH_SubstractVec(&sdwelem.shadowVectorInWorldSpace, &currLightGao->matrices->Pos, &privDataShadow->gao->matrices->Pos);
                        MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                        MATH_NormalizeVector(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                        if ((privDataShadow->flags & 0x800000) != 0)
                        {
                            MATH_VecInvert(&sdwelem.shadowVectorInWorldSpace, &sdwelem.shadowVectorInWorldSpace);
                        }
                        SDW_AddAShadow(&sdwelem);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}