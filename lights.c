#include "common.h"
#include "memory.h"
#include "gdi.h"
#include "profiler.h"
#include "time.h"
#include "animation.h"
#include "collision.h"
#include "soft.h"
#include "lights.h"

int NoLIGH = 0; // game doesn't init it explicitly

void LIGHT_List_Reset(struct sLightListStruct* lightList)
{
    lightList->actualLightNum = 0;
}

void LIGHT_List_AddLight(struct sLightListStruct* lightList, struct sGameObjectMain* gao)
{
    lightList->lights[lightList->actualLightNum] = gao;
    ++lightList->actualLightNum;

    if (lightList->actualLightNum >= lightList->maxLightsNum)
        lightList->actualLightNum = 0;
}

void LIGHT_List_Init(tLightListStruct* lightList, int maxLightsNum)
{
    if (lightList)
    {
        lightList->lights = (tGameObjectMain**)MEM_p_Alloc(sizeof(tGameObjectMain**) * maxLightsNum);
        lightList->maxLightsNum = maxLightsNum;
        lightList->actualLightNum = 0;
    }
}

void LIGHT_List_Free(struct sLightListStruct* lightList)
{
    if (lightList)
    {
        if (lightList->lights)
            MEM_Free(lightList->lights);
        lightList->maxLightsNum = 0;
        lightList->actualLightNum = 0;
        lightList->lights = NULL;
    }
}

int LIGHT_ul_MulColor(int color1, float factor)
{
    int ret;
    int colorFromTable;
    float hax;

    if (factor >= 0.98000002f)
        return color1;
    if (factor <= 0.02f)
        return 0;
    hax = (float)(factor + 49152.0);
    colorFromTable = LIGHT_gpul_CurrentTable[(*(char *)(&hax)) & 0xff];
    ret = 0;
    if ((colorFromTable & 0x80) != 0)
        ret =  ((unsigned int)color1 >> 1) & 0x7f7f7f7f;
    if ((colorFromTable & 0x40) != 0)
        ret += ((unsigned int)color1 >> 2) & 0x3f3f3f3f;
    if ((colorFromTable & 0x20) != 0)
        ret += ((unsigned int)color1 >> 3) & 0x1f1f1f1f;
    if ((colorFromTable & 0x10) != 0)
        ret += ((unsigned int)color1 >> 4) & 0xf0f0f0f;
    if ((colorFromTable & 8) != 0)
        ret += ((unsigned int)color1 >> 5) & 0x7070707;
    return ret;
}

void LIGHT_AddColor(unsigned int* retColor, unsigned int color2)
{
    *retColor = (((((((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 1) | 
                    (((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 2) | 
                  (((((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 1) | 
                    (((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080        | 
               (2 * (((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)))                    | 
              (((((((((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 1) | 
                    (((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 2) | 
                  (((((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 1) | 
                    (((color2 >> 1) & 0x7f7f7f7f) + ((*retColor >> 1) & 0x7f7f7f7f)) & 0x80808080) >> 4);
}

void LIGHT_SubColor(unsigned int* retColor, int color2)
{
    *retColor = ~*retColor;
    LIGHT_AddColor(retColor, color2);
    *retColor = ~*retColor;
}

void LIGHT_SendObjectToSpherical(unsigned int* colors, Vec3* normals)
{
    unsigned int i;
    float avgScale;
    float avgDist;
    float sqAvgDist;
    float invLSPosLen;
    float dpNeg;
    float currDistX, currDistY, currDistZ;
    float factor;
    Vec3 scales; 
    float invSqAvgDist;
    float sqAvgNear;
    float invAvgDist;
    float avgFar;
    float avgNear;
    float sqAvgFar;
    Vec3 invLSPos;
    Vec3 invLSPosNorm;
    Vec3 pointToLightSourceDist;
    float pointToLightDistSq;
    float pointToLightDist;
    float pointToLightDistLen;
    float dpSum;

    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[20]);
    gpst_DX_stDD->rasters[6].unk2 += LIGHT_gpst_CurObject->vertexNum;
    
    MATH_GetScale(&scales, &LIGHT_gst_NIM);
    avgScale = (scales.z + scales.y + scales.x) * 0.333f; // 1 / 3 = 0.333... [fast division]
    avgFar =  (avgScale * LIGHT_gpst_Cur->ffar);
    avgNear = (avgScale * LIGHT_gpst_Cur->fnear);
    avgDist = avgFar - avgNear;
    
    invAvgDist = avgDist;
    if (avgDist != 0.0f)
    {
        invAvgDist = MATH_GetInverse(invAvgDist);
    }

    sqAvgFar = avgFar * avgFar;
    sqAvgNear = avgNear * avgNear;
    sqAvgDist = sqAvgFar - sqAvgNear;
    
    invSqAvgDist = sqAvgDist;
    if (sqAvgDist != 0.0f)
    {
        invSqAvgDist = MATH_GetInverse(invSqAvgDist);
    }

    if ((LIGHT_gpst_Cur->flags & 0x80) == 0)
    {
        if ((LIGHT_gpst_Cur->flags & 0x10) != 0)
        {
            for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
            {
                MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);
                pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y +
                    pointToLightSourceDist.z * pointToLightSourceDist.z;
                if (pointToLightDistSq <= sqAvgFar)
                {
                    pointToLightDistLen = MATH_GetSqrt(pointToLightDistSq);
                    if (pointToLightDistSq <= sqAvgNear)
                        factor = 1.0f;
                    else
                        factor = (avgFar - pointToLightDistLen) * invAvgDist;

                    LIGHT_AddColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                }
            }

        }
        else if ((LIGHT_gpst_Cur->flags & 0x100000) != 0)
        {
            MATH_VecInvert(&invLSPos, &LIGHT_LightSourceMatrix.Pos);
            
            invLSPosLen = MATH_VecLen(&invLSPos);

            if (invLSPosLen <= avgFar)
            {
                MATH_VecMultScalar(&invLSPosNorm, &invLSPos, (1.0f / invLSPosLen));

                invSqAvgDist = invLSPosLen <= avgNear ? 1.0f : ((sqAvgFar - invLSPosLen * invLSPosLen) * invSqAvgDist);

                for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
                {
                    if (normals[i].x != 0.0f || normals[i].y != 0.0f || normals[i].z != 0.0f)
                    {
                        dpNeg = -(normals[i].x * invLSPosNorm.x + normals[i].y * invLSPosNorm.y + normals[i].z * invLSPosNorm.z);
                        if (dpNeg > 0.0f)
                        {
                            LIGHT_AddColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, (invSqAvgDist * dpNeg)));
                        }
                    }
                }

            }
        }
        else
        {
            for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
            {
                if (normals[i].x != 0.0f || normals[i].y != 0.0f || normals[i].z != 0.0f)
                {
                    currDistX = LIGHT_gpst_CurPointsBuffer[i].x - LIGHT_LightSourceMatrix.Pos.x;
                    dpSum = currDistX * currDistX;
                    if (dpSum <= sqAvgFar)
                    {
                        currDistY = LIGHT_gpst_CurPointsBuffer[i].y - LIGHT_LightSourceMatrix.Pos.y;
                        dpSum += currDistY * currDistY;

                        if (dpSum <= sqAvgFar)
                        {
                            currDistZ = LIGHT_gpst_CurPointsBuffer[i].z - LIGHT_LightSourceMatrix.Pos.z;
                            dpSum += currDistZ * currDistZ;
                            if (dpSum <= sqAvgFar)
                            {
                                factor = -(normals[i].x * currDistX + normals[i].y * currDistY + normals[i].z * currDistZ);
                                if (factor > 0.0f)
                                {
                                    if (dpSum > sqAvgNear)
                                        factor = (sqAvgFar - dpSum) * invSqAvgDist * factor;

                                    factor = MATH_GetInverseSqrt(dpSum) * factor;
                                    LIGHT_AddColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if ((LIGHT_gpst_Cur->flags & 0x10) != 0)
    {
        for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
        {
            MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);

            pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y +
                pointToLightSourceDist.z * pointToLightSourceDist.z;

            if (pointToLightDistSq <= sqAvgFar)
            {
                if (pointToLightDistSq <= sqAvgNear)
                    factor = 1.0f;
                else
                    factor = (avgFar - MATH_GetSqrt(pointToLightDistSq)) * invAvgDist;

                LIGHT_SubColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
            }
        }
    }
    else
    {
        for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
        {
            MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);

            pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y +
                pointToLightSourceDist.z * pointToLightSourceDist.z;

            if (pointToLightDistSq <= sqAvgFar)
            {
                factor = -(normals[i].x * pointToLightSourceDist.x + normals[i].y * pointToLightSourceDist.y + normals[i].z * pointToLightSourceDist.z);
                if (factor >= 0.0f)
                {
                    pointToLightDist = MATH_GetSqrt(pointToLightDistSq);
                    invSqAvgDist = MATH_GetInverse(pointToLightDist);
                    factor = invSqAvgDist * factor;

                    if (pointToLightDistSq > sqAvgNear)
                        factor = (avgFar - pointToLightDist) * invAvgDist * factor;

                    LIGHT_SubColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                }
            }
        }
    }

    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[20]);
}

void LIGHT_SendObjectToDirect(unsigned int* colors, Vec3* normals)
{
    float sqNearFarDist;
    float sinDiff;
    float dpNeg;
    float invPointToLightDist;
    float pointToLightDistSq;
    float is8Min;
    float densitySin; 
    float sqNear; 
    float invSinDiff; 
    float invNearFarDist; 
    float is8Sin; 
    float sqFar; 
    float factor; 
    Vec3 pointToLightSourceDist;
    Vec3 distCP;
    unsigned int i;

    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[21]);
    gpst_DX_stDD->rasters[7].unk2 += LIGHT_gpst_CurObject->vertexNum;

    sqFar = LIGHT_gpst_Cur->fnear * LIGHT_gpst_Cur->fnear;
    sqNear = LIGHT_gpst_Cur->ffar * LIGHT_gpst_Cur->ffar;
    sqNearFarDist = sqNear - sqFar;
    
    invNearFarDist = sqNearFarDist;
    if (sqNearFarDist != 0.0f) {
        invNearFarDist = MATH_GetInverse(invNearFarDist);
    }

    densitySin = MATH_GetSin(LIGHT_gpst_Cur->density);
    is8Sin = MATH_GetSin(LIGHT_gpst_Cur->i8);
    sinDiff = is8Sin - densitySin;
    
    invSinDiff = (float)sinDiff;
    if (sinDiff != 0.0f) {
        invSinDiff = MATH_GetInverse(invSinDiff);
    }

    if ((LIGHT_gpst_Cur->flags & 0x80) == 0)
    {
        if ((LIGHT_gpst_Cur->flags & 0x10) != 0)
        {

                for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
                {
                    MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);

                    if (pointToLightSourceDist.x * LIGHT_LightSourceMatrix.v2.x
                        + pointToLightSourceDist.y * LIGHT_LightSourceMatrix.v2.y
                        + pointToLightSourceDist.z * LIGHT_LightSourceMatrix.v2.z <= 0.0f)
                    {
                        pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y + 
                                             pointToLightSourceDist.z * pointToLightSourceDist.z;
                        if (pointToLightDistSq <= sqNear)
                        {
                            factor = 1.0f;
                            if (pointToLightDistSq > sqFar)
                                factor = (1.0f - (pointToLightDistSq - sqFar) * invNearFarDist);
                            MATH_CrossProduct(&distCP, &LIGHT_LightSourceMatrix.v2, &pointToLightSourceDist);
                            is8Min = MATH_VecLen(&distCP) / MATH_GetSqrt(pointToLightDistSq);
                            if (is8Min <= is8Sin)
                            {
                                if (is8Min > densitySin)
                                    factor = (is8Sin - is8Min) * factor * invSinDiff;
                                LIGHT_AddColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                            }
                        }
                    }
                }
        }
        else
        {
            for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)            
            {
                MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);
                if (pointToLightSourceDist.x * LIGHT_LightSourceMatrix.v2.x
                    + pointToLightSourceDist.y * LIGHT_LightSourceMatrix.v2.y
                    + pointToLightSourceDist.z * LIGHT_LightSourceMatrix.v2.z <= 0.0f)
                {
                    pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y +
                                         pointToLightSourceDist.z * pointToLightSourceDist.z;

                    if (pointToLightDistSq <= sqNear)
                    {
                        dpNeg = -(normals[i].x * pointToLightSourceDist.x + normals[i].y * pointToLightSourceDist.y + normals[i].z * pointToLightSourceDist.z);
                        if (dpNeg > 0.0f)
                        {
                            invPointToLightDist = 1.0f / MATH_GetSqrt(pointToLightDistSq);
                            factor = dpNeg * invPointToLightDist;

                            if (pointToLightDistSq > sqFar)
                                factor = (1.0f - (pointToLightDistSq - sqFar) * invNearFarDist) * factor;

                            MATH_CrossProduct(&distCP, &LIGHT_LightSourceMatrix.v2, &pointToLightSourceDist);

                            is8Min = MATH_VecLen(&distCP) * invPointToLightDist;
                            if (is8Min <= is8Sin)
                            {
                                if (is8Min > densitySin)
                                    factor = (is8Sin - is8Min) * factor * invSinDiff;
                                LIGHT_AddColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                            }
                        }
                    }
                }
            }
        }
    }
    else if ((LIGHT_gpst_Cur->flags & 0x10) != 0)
    {
        for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
        {
            MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);
            if (pointToLightSourceDist.x * LIGHT_LightSourceMatrix.v2.x
                + pointToLightSourceDist.y * LIGHT_LightSourceMatrix.v2.y
                + pointToLightSourceDist.z * LIGHT_LightSourceMatrix.v2.z <= 0.0f)
            {
                pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y +
                    pointToLightSourceDist.z * pointToLightSourceDist.z;

                if (pointToLightDistSq <= sqNear)
                {
                    factor = 1.0f;
                    if (pointToLightDistSq > sqFar)
                        factor = (float)(1.0f - (pointToLightDistSq - sqFar) * invNearFarDist);

                    MATH_CrossProduct(&distCP, &LIGHT_LightSourceMatrix.v2, &pointToLightSourceDist);
                    is8Min = MATH_VecLen(&distCP) / MATH_GetSqrt(pointToLightDistSq);
                    if (is8Min <= is8Sin)
                    {
                        if (is8Min > densitySin)
                            factor = (is8Sin - is8Min) * factor * invSinDiff;
                        LIGHT_SubColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                    }
                }
            }
        }
    }
    else 
    {
        for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++)
        {
            MATH_SubstractVec(&pointToLightSourceDist, &LIGHT_gpst_CurPointsBuffer[i], &LIGHT_LightSourceMatrix.Pos);
            if (pointToLightSourceDist.x * LIGHT_LightSourceMatrix.v2.x
                + pointToLightSourceDist.y * LIGHT_LightSourceMatrix.v2.y
                + pointToLightSourceDist.z * LIGHT_LightSourceMatrix.v2.z <= 0.0f)
            {
                pointToLightDistSq = pointToLightSourceDist.x * pointToLightSourceDist.x + pointToLightSourceDist.y * pointToLightSourceDist.y +
                    pointToLightSourceDist.z * pointToLightSourceDist.z;
                if (pointToLightDistSq <= sqNear)
                {
                    dpNeg = -(normals[i].x * LIGHT_gpst_CurPointsBuffer[i].x + normals[i].y * LIGHT_gpst_CurPointsBuffer[i].y + normals[i].z * LIGHT_gpst_CurPointsBuffer[i].z);
                    if (dpNeg > 0.0f)
                    {
                        invPointToLightDist = 1.0f / MATH_GetSqrt(pointToLightDistSq);
                        factor = dpNeg * invPointToLightDist;
                        if (pointToLightDistSq > sqFar)
                            factor = (1.0f - (pointToLightDistSq - sqFar) * invNearFarDist) * factor;
                        MATH_CrossProduct(&distCP, &LIGHT_LightSourceMatrix.v2, &pointToLightSourceDist);
                        is8Min = MATH_VecLen(&distCP) * invPointToLightDist;
                        if (is8Min <= is8Sin)
                        {
                            if (is8Min > densitySin)
                                factor = (is8Sin - is8Min) * factor * invSinDiff;
                            LIGHT_SubColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
                        }
                    }
                }
            }
        }
    }
    
    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[21]);
}

void LIGHT_SendObjectToSpot(unsigned int* colors, Vec3* normals)
{
    unsigned int i;
    float factor;

    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[22]);
    gpst_DX_stDD->rasters[8].unk2 += LIGHT_gpst_CurObject->vertexNum;

    MATH_VecInvert(&LIGHT_LightSourceMatrix.v2, &LIGHT_LightSourceMatrix.v2);

    if ((LIGHT_gpst_Cur->flags & 0x80) == 0)
    {
        if ((LIGHT_gpst_Cur->flags & 0x10) != 0)
        {
            for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++) 
            {
                LIGHT_AddColor(&colors[i], LIGHT_gul_Color);
            }
        }
        else
        {
            for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++) 
            {                
                factor = -(normals[i].x * LIGHT_LightSourceMatrix.v2.x +
                           normals[i].y * LIGHT_LightSourceMatrix.v2.y +
                           normals[i].z * LIGHT_LightSourceMatrix.v2.z);
                
                if (factor < 0.0f)
                    factor = 0.0f;

                LIGHT_AddColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
            }            
        }
    }
    else if ((LIGHT_gpst_Cur->flags & 0x10) != 0)
    {
        for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++) 
        {
            LIGHT_SubColor(&colors[i], LIGHT_gul_Color);
        }               
    }
    else 
    {
        for (i = 0; i < LIGHT_gpst_CurObject->vertexNum; i++) 
        {
            factor = -(normals[i].x * LIGHT_LightSourceMatrix.v2.x +
                       normals[i].y * LIGHT_LightSourceMatrix.v2.y +
                       normals[i].z * LIGHT_LightSourceMatrix.v2.z);
            if (factor < 0.0f)
                factor = 0.0f;

            LIGHT_SubColor(&colors[i], LIGHT_ul_MulColor(LIGHT_gul_Color, factor));
        }
    }

    MATH_VecInvert(&LIGHT_LightSourceMatrix.v2, &LIGHT_LightSourceMatrix.v2);

    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[22]);
}

BOOL LIGHT_l_IsNodeLighted(tGameObjectMain* baseGao, tGameObjectMain* gao, tLIGHT* light, int checkBV)
{
    Vec3* gaoPos;
    Vec3 tmpVec; 

    if ((baseGao->flags2 & 8) != 0)
    {
        if ((light->flags & 0x100) == 0)
            return FALSE;
    }
    else if ((light->flags & 0x200) == 0)
    {
        return FALSE;
    }

    if (light->lightGao && light->lightGao != ANI_pst_GetReferenceInit(baseGao))
        return FALSE;

    if (checkBV && baseGao->boundingVolume && ((light->flags & 7) == 0 || (light->flags & 7) == 2))
    {
        if ((baseGao->flags & 0x600001) == 0x600001 && (baseGao->basic->multiPartData->elemNum > 1)
            && baseGao->basic->mtx->refGao2 != NULL)
        {
            MATH_TransformVertex(&tmpVec, OBJ_pst_GetAbsoluteMatrix(baseGao->basic->mtx->refGao2), &baseGao->basic->mtx->mtx.Pos);
            gaoPos = &tmpVec;
        }
        else
        {
            gaoPos = &baseGao->matrices->Pos;
        }

        MATH_SubstractVec(&tmpVec, &gao->matrices->Pos, gaoPos);
                
        if (!INT_SphereAABBox(&tmpVec, light->ffar, &baseGao->boundingVolume->BVMin, &baseGao->boundingVolume->BVMax))
            return FALSE;
    }
    return TRUE;
}

void LIGHT_Invert(Vec3* normals, int vertexNum, int vecComponentsSelected, int YZorX)
{
    int i;

    if (vecComponentsSelected)
    {
        if (YZorX)
        {
            for (i = 0; i < vertexNum; i++) {
                normals[i].y = -normals[i].y;
                normals[i].z = -normals[i].z;
            }
        }
        else {
            for (i = 0; i < vertexNum; i++) {
                normals[i].x = -normals[i].x;
            }
        }                        
    } else if (YZorX)
    {
        for (i = 0; i < vertexNum; i++) {
            normals[i].x = -normals[i].x;
            normals[i].y = -normals[i].y;
            normals[i].z = -normals[i].z;
        }
    }
}

void LIGHT_SendObjectToLight(struct sLightListStruct* lls, struct sGameObjectMain* gao)
{
    int i;
    int initObjBVCenter;
    int checkBVOnLitTest;
    float lightMultiplicationFactor;
    int vecComponentsSelected, YZorX;
    tGameObjectVisualObject* gaovo;
    Vec3 objectBVCenter, objBVInWorldSpace;
    BOOL attenuationOn;

    if (NoLIGH) return;

    LIGHT_gpst_Bump = NULL;

    if (!lls->actualLightNum) return;

    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[19]);
    QueryPerformanceCounter(&PerfCounterStruct[1]);

    YZorX = ((gpst_DX_stDD->drawMaskAllowedStuffCurrent >> 13) & 1) == 0;
    vecComponentsSelected = FALSE;
    
    if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x8000) == 0)
    {
        vecComponentsSelected = TRUE;
        YZorX = ((gpst_DX_stDD->drawMaskAllowedStuffCurrent >> 13) & 1) != 0;
    }
    
    initObjBVCenter = TRUE;
    gpst_DX_stDD->bigBuffer->solidVertexColor = 0x00000000;
    LIGHT_gpst_CurObject = (tGEOGRO*)((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
    LIGHT_gpst_CurPointsBuffer = gpst_DX_stDD->verticesCurrent;
    
    GEO_UseNormals(LIGHT_gpst_CurObject);

    if (!LIGHT_gpst_CurObject->normals) return;

    checkBVOnLitTest = gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x200000;
    attenuationOn = ((gpst_DX_stDD->drawMaskAllowedStuffCurrent >> 30) & 1) == 0;

    if (attenuationOn)
        lightMultiplicationFactor = ((127.0f - (float)((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->vmilyenDistanceLightAttenuationFactor)
              * 0.0078740157f * (1.0f - gpst_DX_stDD->lightAttenuationFactor));

    for (i = 0; i < lls->actualLightNum; i++)
    {
        LIGHT_gpst_Cur = (tLIGHT*)(lls->lights[i]->extended->light);

        if (!LIGHT_l_IsNodeLighted(gao, lls->lights[i], LIGHT_gpst_Cur, checkBVOnLitTest))
            continue;

        if ((LIGHT_gpst_Cur->flags & 0x10000000) != 0)
            LIGHT_gpst_Bump = lls->lights[i];

        if ((LIGHT_gpst_Cur->flags & 7) == 4)
        {
            if (!gpst_DX_stDD->extraMaterialToGeo)
            {
                if (lls->lights[i]->basic != NULL && (gaovo = lls->lights[i]->basic->actorObjectOrVisualObject) != NULL && gaovo->GROMAT != NULL)
                {
                    if ((LIGHT_gpst_Cur->flags & 0x2000000) != 0 ? COL_b_BVIn(lls->lights[i], gao) : COL_b_BVOverlap(gao, lls->lights[i], TRUE, TRUE, FALSE))
                    {
                        gpst_DX_stDD->extraMaterialToGeo = (tGEOMAT*)(((tGameObjectVisualObject*)(lls->lights[i]->basic->actorObjectOrVisualObject))->GROMAT);
                        if ((LIGHT_gpst_Cur->flags & 0x1000000) != 0)
                            gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x100000;
                        if (gpst_DX_stDD->extraMaterialToGeo->intf->id == 4)
                            gpst_DX_stDD->extraMaterialToGeo = gpst_DX_stDD->extraMaterialToGeo->multiMaterialPtrIfType4ThenDiffuseColor[(*(int32_t*)&(LIGHT_gpst_Cur->fnear)) % gpst_DX_stDD->extraMaterialToGeo->multiMaterialNumIfType4ThenAmbiantColor];
                    }
                }
            }
            continue;
        }

        if (initObjBVCenter)
        {
            initObjBVCenter = FALSE;
            MATH_InvertMatrix(&LIGHT_gst_NIM, gao->matrices);
            if (gao->boundingVolume != NULL)
            {
                MATH_VecAddVec(&objBVInWorldSpace, &gao->boundingVolume->BVMin, &gao->boundingVolume->BVMax);
                MATH_VecMultScalar(&objBVInWorldSpace, &objBVInWorldSpace, 0.5f);
                MATH_TransformVector(&objectBVCenter, &LIGHT_gst_NIM, &objBVInWorldSpace);
            }
            else
            {
                objectBVCenter.x = 0.0f;
                objectBVCenter.y = 0.0f;
                objectBVCenter.z = 0.0f;
            }
            LIGHT_Invert(LIGHT_gpst_CurObject->normals, LIGHT_gpst_CurObject->vertexNum, vecComponentsSelected, YZorX);
        }

        MATH_MulMatrixMatrix(&LIGHT_LightSourceMatrix, lls->lights[i]->matrices, &LIGHT_gst_NIM);

        if ((LIGHT_gpst_Cur->flags & 0x100007) == 0x100000)
        {
            MATH_SubstractVec(&LIGHT_LightSourceMatrix.Pos, &LIGHT_LightSourceMatrix.Pos, &objectBVCenter);
        }
        LIGHT_gul_Color = LIGHT_gpst_Cur->color;

        if ((LIGHT_gpst_Cur->flags & 0x20) == 0)
            LIGHT_gul_Color &= 0xff000000;

        if ((LIGHT_gpst_Cur->flags & 0x40) == 0)
            LIGHT_gul_Color &= 0x00ffffff;

        if (attenuationOn && (LIGHT_gpst_Cur->flags & 0x200000) == 0)
        {
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_gul_Color = LIGHT_ul_MulColor(LIGHT_gul_Color, lightMultiplicationFactor);
        }

        if ((LIGHT_gpst_Cur->flags & 0x1000) != 0 && (gpst_DX_stDD->bigBuffer->flags & 4) == 0)
        {
            SOFT_pst_InitSpecularColors(LIGHT_gpst_CurObject->vertexNum);
        }

        switch (LIGHT_gpst_Cur->flags & 0x1007) {
        case 0:
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_SendObjectToSpherical((unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors, LIGHT_gpst_CurObject->normals);
            break;
        case 1:
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_SendObjectToSpot((unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors, LIGHT_gpst_CurObject->normals);
            break;
        case 2:
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_SendObjectToDirect((unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors, LIGHT_gpst_CurObject->normals);
            break;
        case 0x1000:
            LIGHT_gpul_CurrentTable = LIGHT_gaul_ExponantTable;
            LIGHT_gpst_Cur->ffar = LIGHT_gpst_Cur->ffar + LIGHT_gpst_Cur->ffar;
            LIGHT_gpst_Cur->fnear = LIGHT_gpst_Cur->fnear * 0.25f;
            LIGHT_SendObjectToSpherical((unsigned int*)gpst_DX_stDD->bigBuffer->data2SpecularColors, SOFT_pst_GetSpecularVectors());
            LIGHT_gpst_Cur->ffar = LIGHT_gpst_Cur->ffar * 0.5f;
            LIGHT_gpst_Cur->fnear = LIGHT_gpst_Cur->fnear * 4.0f;
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_SendObjectToSpherical((unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors, LIGHT_gpst_CurObject->normals);
            break;
        case 0x1001:
            LIGHT_gpul_CurrentTable = LIGHT_gaul_ExponantTable;
            LIGHT_SendObjectToSpot((unsigned int*)gpst_DX_stDD->bigBuffer->data2SpecularColors, SOFT_pst_GetSpecularVectors());
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_SendObjectToSpot((unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors, LIGHT_gpst_CurObject->normals);
            break;
        case 0x1002:
            LIGHT_gpul_CurrentTable = LIGHT_gaul_ExponantTable;
            LIGHT_gpst_Cur->ffar = LIGHT_gpst_Cur->ffar + LIGHT_gpst_Cur->ffar;
            LIGHT_SendObjectToDirect((unsigned int*)gpst_DX_stDD->bigBuffer->data2SpecularColors, SOFT_pst_GetSpecularVectors());
            LIGHT_gpst_Cur->ffar = LIGHT_gpst_Cur->ffar * 0.5f;
            LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
            LIGHT_SendObjectToDirect((unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors, LIGHT_gpst_CurObject->normals);
            break;
        default:
            break;
        }
    }

    if (!initObjBVCenter)
        LIGHT_Invert(LIGHT_gpst_CurObject->normals, LIGHT_gpst_CurObject->vertexNum, vecComponentsSelected, YZorX);
    
    PRO_QPC_RecordTimeDecRef(1);
    PRO_RecordTimeDecRef(&gpst_DX_stDD->rasters[19]);
}

unsigned int LIGHT_ul_Interpol2Colors(int color1, int color2, float factor)
{
    unsigned int tmpColor;
    unsigned int colorToAdd;
    float hax;

    if (factor >= 0.98000002f)
        return color2;
    if (factor <= 0.02f)
        return color1;

    hax = (float)(factor + 49152.0);
    tmpColor = color2;
    if (((*(char *)(&hax)) & 0x80) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 1) & 0x7f7f7f7f);
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x40) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 2) & 0x3f3f3f3f) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x20) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 3) & 0x1f1f1f1f) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x10) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 4) & 0x0f0f0f0f) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 8) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 5) & 0x07070707) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 4) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 6) & 0x03030303) + colorToAdd;
    if (((*(char*)(&hax)) & 2) != 0)
        tmpColor = ((unsigned int)color2 >> 7) & 0x01010101;
    else
        tmpColor = ((unsigned int)color1 >> 7) & 0x01010101;
    return tmpColor + colorToAdd;
}

uint32_t COLOR_ul_Mul(uint32_t color, float mult)
{
    int ret = 0;
    float test;

    if (mult >= 0.98000002f)
        return color;
    if (mult <= 0.02f)
        return 0;

    test = (float)(mult + 49152.0);                            // 32768.0 + 16384.0
    if (((*(uint8_t*)(&test)) & 0x80) != 0)
        ret = ((unsigned int)color >> 1) & 0x7f7f7f7f;
    if (((*(uint8_t*)(&test)) & 0x40) != 0)
        ret += ((unsigned int)color >> 2) & 0x3f3f3f3f;
    if (((*(uint8_t*)(&test)) & 0x20) != 0)
        ret += ((unsigned int)color >> 3) & 0x1f1f1f1f;
    if (((*(uint8_t*)(&test)) & 0x10) != 0)
        ret += ((unsigned int)color >> 4) & 0x0f0f0f0f;
    if (((*(uint8_t*)(&test)) & 8) != 0)
        ret += ((unsigned int)color >> 5) & 0x07070707;
    return ret;
}

// MMX-es kód van a játékban, azt nem használom, itt egy sima implementáció
void LIGHT_ColorAdd(uint32_t colorToAdd, uint32_t* src, int num, uint32_t* dest)
{
    unsigned int b1, b2, b3, b4;
    unsigned int b1d, b2d, b3d, b4d;
    int i;

    b1 = (colorToAdd & 0xff);
    b2 = ((colorToAdd >> 8) & 0xff);
    b3 = ((colorToAdd >> 16) & 0xff);
    b4 = ((colorToAdd >> 24) & 0xff);

    for (i = 0; i < num; i++) {
        b1d = (*src & 0xff);
        b2d = ((*src >> 8) & 0xff);
        b3d = ((*src >> 16) & 0xff);
        b4d = ((*src >> 24) & 0xff);

        b1d = min(b1 + b1d, 255);
        b2d = min(b2 + b2d, 255);
        b3d = min(b3 + b3d, 255);
        b4d = min(b4 + b4d, 255);

        *dest = (b1d & 0xff) | ((b2d & 0xff) << 8) | ((b3d & 0xff) << 16) | ((b4d & 0xff) << 24);

        src++;
        dest++;
    }
}

void LIGHT_ComputeVertexColor(int vertexNum)
{
    int i;
    int vertexNumLimited = vertexNum;
    unsigned int* srcColorArr, *destColorArr;
    int selectedAmbientColor;

    gpst_DX_stDD->rasters[5].unk2 += vertexNum;
    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[18]);

    if (vertexNum > 5000)
        vertexNumLimited = 5000;

    gpst_DX_stDD->bigBuffer->vertexColorArray = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
    srcColorArr = (unsigned int*)gpst_DX_stDD->bigBuffer->data1LightColors;
    destColorArr = (unsigned int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;

    if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x100) != 0)
    {
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x400000) != 0)
            selectedAmbientColor = gpst_DX_stDD->CurrentWorld->ambientColor1;
        else
            selectedAmbientColor = gpst_DX_stDD->CurrentWorld->ambientColor2;
        LIGHT_ColorAdd(selectedAmbientColor & 0xffffff, srcColorArr, vertexNumLimited, destColorArr);
    }
    else
    {
        for (i = 0; i < vertexNumLimited; i++) {
            *destColorArr++ = *srcColorArr++;
        }
    }

    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[18]);
}
