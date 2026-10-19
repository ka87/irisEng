#include "../memory.h"
#include "../time.h"
#include "gfxanotherflare.h"

// eg. green circles around falling meteorites at intro
int GFX_i_AnotherFlare_Render(struct sGfxAnotherFlareData* afd)
{
    int i, j;
    int color2;
    int extraFlareNum;
    int drawMaskBackup; 
    int interpolatedColor;
    char extraFlareElems;
    float currTimeMult;
    float dptmp;
    float timeRatio;
    float dpOrig;
    float currAngle;
    float scalec;
    float angleAddAmount;
    Vec3 sight; 
    Vec3 banking; 
    Vec3 distance;
    Vec3 sightInterpolated;
    Vec3 invBanking; 
    Vec3 distAndBankingAngle;
    Vec3 pointsToBanking;
    Vec3 bankingAndSightInterpolated;
    Vec3 distanceNormalized;

    interpolatedColor = afd->color2;
    color2 = afd->color2;

    if (interpolatedColor == 255)
        interpolatedColor = 255;
    
    currTimeMult = afd->timeMultiplier;
    if ((afd->inited & 1) != 0)
    {
        afd->remainingTime = afd->remainingTime - TIM_gf_dt;
        
        if (afd->remainingTime < 0.0f)
            return 0;

        timeRatio = afd->remainingTime / afd->totalTime;
        interpolatedColor = LIGHT_ul_Interpol2Colors(afd->color, afd->color2, timeRatio);
        color2 = interpolatedColor;

        if ((afd->flags & 0x10) != 0)
            currTimeMult = timeRatio * currTimeMult;
    }
    else
    {
        if ((afd->flags & 1) != 0)
        {
            if (afd->totalTime <= 0.0f)
                return 0;

            afd->inited |= 0x1;
            afd->remainingTime = afd->totalTime;
        }
    }
    if (GFX_gpst_Current->aiGameObject->distance)
    {
        extraFlareNum = 0;
        MATH_SubstractVec(&distance, &afd->pos, &gpst_DX_stDD->ct.cam.objectMtx.Pos);
        MATH_NormalizeVector(&distanceNormalized, &distance);
        
        if (afd->b2 == 1)
        {
            MATH_VecMultScalar(&sight, &distanceNormalized, -afd->i1);
            MATH_VecMultScalar(&sightInterpolated, &gpst_DX_stDD->ct.cam.objectMtx.v2, currTimeMult);
            MATH_VecMultScalar(&banking, &gpst_DX_stDD->ct.cam.objectMtx.v1, currTimeMult);
            MATH_VecInvert(&invBanking, &banking);
        }
        else
        {
            MATH_CrossProduct(&distance, &afd->v2, &distanceNormalized);
            MATH_NormalizeEqualVector(&distance);
            MATH_VecMultScalar(&sightInterpolated, &distance, currTimeMult);
            MATH_CrossProduct(&banking, &afd->v2, &sightInterpolated);
            MATH_VecInvert(&invBanking, &banking);
            dptmp = MATH_DotProduct(&afd->v2, &distanceNormalized) + 1.0f;
            dpOrig = dptmp;
            
            if (dptmp > afd->f2)
                dptmp = afd->f2;

            if ((afd->flags & 2) != 0)
            {
                MATH_VecMultScalar(&sight, &afd->v2, dptmp);
                MATH_AddScaleVector(&banking, &banking, &sight, -afd->f3);
                MATH_VecMultScalar(&sight, &afd->v2, afd->i1);
            }
            else
            {
                MATH_VecMultScalar(&sight, &afd->v2, dptmp * afd->i1);
                MATH_AddScaleVector(&banking, &banking, &sight, -afd->f3);
            }

            MATH_AddScaleVector(&invBanking, &invBanking, &afd->v2, (dptmp * afd->f4 * currTimeMult));
            
            if (MATH_DotProduct(&banking, &distanceNormalized) > 0.0f)
            {
                MATH_VecInvert(&sightInterpolated, &sightInterpolated);
            }
            if ((afd->flags & 0x4) != 0 && dpOrig > 1.0)
            {
                MATH_CrossProduct(&distAndBankingAngle, &distance, &afd->v2);
                MATH_VecMultScalar(&distAndBankingAngle, &distAndBankingAngle, currTimeMult);
                MATH_SubstractVec(&pointsToBanking, &distAndBankingAngle, &banking);
                MATH_CrossProduct(&bankingAndSightInterpolated, &sightInterpolated, &pointsToBanking);
                
                if (MATH_DotProduct(&bankingAndSightInterpolated, &distanceNormalized) < 0.0f)
                {
                    extraFlareNum = 1;
                    MATH_SubstractVec(&pointsToBanking, &banking, &invBanking);
                    MATH_CrossProduct(&bankingAndSightInterpolated, &sightInterpolated, &pointsToBanking);
                    if (MATH_DotProduct(&bankingAndSightInterpolated, &distanceNormalized) >= 0.0f)
                    {
                        invBanking.x = 0.0f;
                        invBanking.y = 0.0f;
                        invBanking.z = 0.0f;
                    }
                    else
                    {
                        banking.x = 0.0f;
                        banking.y = 0.0f;
                        banking.z = 0.0f;
                    }
                }
            }
        }

        MATH_VecAddVec(&sight, &sight, &afd->pos);

        if (afd->b2 == 1 || (extraFlareElems = afd->extraFlareElems, extraFlareElems <= 1))
        {
            if ((afd->flags & 0x2) != 0 && dptmp * afd->f3 > afd->i1)
            {
                GFX_NeedGeom(6, 6, 4, 1);

                scalec = afd->i1 / (dptmp * afd->f3);

                MATH_VecMultScalar(&distanceNormalized, &sightInterpolated, 1.0f - scalec);
                MATH_VecAddVec(&GFX_gpst_Geo->vertices[3], &invBanking, &sight);
                MATH_AddScaleVector(&GFX_gpst_Geo->vertices[4], &sight, &banking, scalec);
                MATH_SubstractVec(&GFX_gpst_Geo->vertices[5], &GFX_gpst_Geo->vertices[4], &distanceNormalized);
                MATH_VecAddVec(&GFX_gpst_Geo->vertices[4], &GFX_gpst_Geo->vertices[4], &distanceNormalized);

                MATH_VecMultScalar(&distanceNormalized, &afd->v2, dptmp * (afd->f3 - afd->i1));
                GFX_gpst_Geo->vertexColors[4] = color2;
                GFX_gpst_Geo->vertexColors[5] = color2;
                GFX_gpst_Geo->UVs[4].u = scalec;
                GFX_gpst_Geo->UVs[4].v = 0.0f;
                GFX_gpst_Geo->UVs[5].u = 1.0f;
                GFX_gpst_Geo->UVs[5].v = 1.0f - scalec;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v1 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v2 = 4;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv1 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv2 = 4;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v1 = 4;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v2 = 5;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv1 = 4;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv2 = 5;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].v1 = 4;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].v2 = 1;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].v3 = 5;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].uv1 = 4;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].uv2 = 1;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].uv3 = 5;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].v1 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].v2 = 3;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].v3 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].uv1 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].uv2 = 3;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].uv3 = 0;
                banking.x = banking.x + distanceNormalized.x;
                banking.y = banking.y + distanceNormalized.y;
                banking.z = banking.z + distanceNormalized.z;
            }
            else
            {
                GFX_NeedGeom(4, 4, 2, 1);

                GFX_gpst_Geo->vertices[3].x = invBanking.x + sight.x;
                GFX_gpst_Geo->vertices[3].y = invBanking.y + sight.y;
                GFX_gpst_Geo->vertices[3].z = invBanking.z + sight.z;
                GFX_gpst_Geo->vertexColors[4] = color2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v1 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v2 = 1;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv1 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv2 = 1;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v1 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v2 = 3;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v3 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv1 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv2 = 3;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv3 = 0;
            }
        }
        else
        {
            GFX_NeedGeom(extraFlareElems + extraFlareNum + 3, 4, extraFlareElems * (extraFlareNum + 2), 1);

            angleAddAmount = ((afd->angle + afd->angle) / (float)(afd->extraFlareElems - 1));
            currAngle = -afd->angle;
            
            MATH_VecMultScalar(&distance, &distance, (dptmp* afd->f4* currTimeMult));

            if (afd->extraFlareElems > 0)
            {
                color2 = interpolatedColor;
            }
            for (i = 0, j = 0; i < afd->extraFlareElems; i++, j += 2)
            {
                MATH_VecMultScalar(&bankingAndSightInterpolated, &invBanking, MATH_GetCos(currAngle));
                MATH_VecMultScalar(&pointsToBanking, &distance, MATH_GetSin(currAngle));

                GFX_gpst_Geo->vertices[i + 3].x = bankingAndSightInterpolated.x + sight.x;
                GFX_gpst_Geo->vertices[i + 3].y = bankingAndSightInterpolated.y + sight.y;
                GFX_gpst_Geo->vertices[i + 3].z = bankingAndSightInterpolated.z + sight.z;
                GFX_gpst_Geo->vertices[i + 3].x = GFX_gpst_Geo->vertices[i + 3].x + pointsToBanking.x;
                GFX_gpst_Geo->vertices[i + 3].y = GFX_gpst_Geo->vertices[i + 3].y + pointsToBanking.y;
                GFX_gpst_Geo->vertices[i + 3].z = GFX_gpst_Geo->vertices[i + 3].z + pointsToBanking.z;
                GFX_gpst_Geo->vertexColors[i + 4] = interpolatedColor;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j].v1 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j].v2 = 1;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j].v3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j].uv1 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j].uv2 = 1;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j].uv3 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].v1 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].v2 = i + 3;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].v3 = 0;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].uv1 = 2;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].uv2 = 3;
                GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].uv3 = 0;
                currAngle = angleAddAmount + currAngle;
            }
            if (extraFlareNum)
            {
                GFX_gpst_Geo->vertices[afd->extraFlareElems + 3].x = distAndBankingAngle.x + sight.x;
                GFX_gpst_Geo->vertices[afd->extraFlareElems + 3].y = distAndBankingAngle.y + sight.y;
                GFX_gpst_Geo->vertices[afd->extraFlareElems + 3].z = distAndBankingAngle.z + sight.z;
                GFX_gpst_Geo->vertexColors[afd->extraFlareElems + 4] = color2;

                for (i = 0; i < afd->extraFlareElems; i++)
                {
                    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2 * afd->extraFlareElems + i].v1 = 2;
                    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2 * afd->extraFlareElems + i].v2 = afd->extraFlareElems + 3;
                    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2 * afd->extraFlareElems + i].v3 = 0;
                    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2 * afd->extraFlareElems + i].uv1 = 2;
                    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2 * afd->extraFlareElems + i].uv2 = 3;
                    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2 * afd->extraFlareElems + i].uv3 = 0;
                }
            }
        }
        GFX_gpst_Geo->UVs[0].u = 0.0f;
        GFX_gpst_Geo->UVs[0].v = 0.0f;
        GFX_gpst_Geo->UVs[1].u = 1.0f;
        GFX_gpst_Geo->UVs[1].v = 0.0f;
        GFX_gpst_Geo->UVs[2].u = 1.0f;
        GFX_gpst_Geo->UVs[2].v = 1.0f;
        GFX_gpst_Geo->UVs[3].u = 0.0f;
        GFX_gpst_Geo->UVs[3].v = 1.0f;
        GFX_gpst_Geo->vertices[0].x = sightInterpolated.x + sight.x;
        GFX_gpst_Geo->vertices[0].y = sightInterpolated.y + sight.y;
        GFX_gpst_Geo->vertices[0].z = sightInterpolated.z + sight.z;
        GFX_gpst_Geo->vertices[1].x = banking.x + sight.x;
        GFX_gpst_Geo->vertices[1].y = banking.y + sight.y;
        GFX_gpst_Geo->vertices[1].z = banking.z + sight.z;
        GFX_gpst_Geo->vertices[2].x = sight.x - sightInterpolated.x;
        GFX_gpst_Geo->vertices[2].y = sight.y - sightInterpolated.y;
        GFX_gpst_Geo->vertices[2].z = sight.z - sightInterpolated.z;
        GFX_gpst_Geo->vertexColors[1] = color2;
        GFX_gpst_Geo->vertexColors[2] = color2;
        GFX_gpst_Geo->vertexColors[3] = color2;
        
        drawMaskBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup & ~0x8000900;
        
        if ((afd->flags & 8) != 0)
            gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x4000;
        
        GFX_gpst_Geo->triangleChunks[0].materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup;
    }
    return 1;
}

struct sGfxAnotherFlareData* GFX_AnotherFlare_Create(struct sGameObjectMain* gao)
{
    tGfxAnotherFlareData* afd = (tGfxAnotherFlareData*)MEM_p_Alloc(sizeof(tGfxAnotherFlareData));
    Mtx4x4* gaoMtx = OBJ_pst_GetAbsoluteMatrix(gao);

    afd->pos.x = gaoMtx->Pos.x;
    afd->pos.y = gaoMtx->Pos.y;
    afd->pos.z = gaoMtx->Pos.z;
    afd->v2.x = -gaoMtx->v2.x;
    afd->v2.y = -gaoMtx->v2.y;
    afd->v2.z = -gaoMtx->v2.z;
    afd->i1 = 0.0f;
    afd->totalTime = 0.0f;
    afd->flags = 0;
    afd->b2 = 0;
    afd->inited = 0;
    afd->extraFlareElems = 0;
    afd->color = 0;
    afd->timeMultiplier = 1.0f;
    afd->f2 = 0.80000001f;
    afd->f3 = 1.0f;
    afd->f4 = 1.0f;
    afd->color2 = 0xffffffff;

    return afd;
}