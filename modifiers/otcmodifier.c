#include "../memory.h"
#include "../time.h"
#include "../loading.h"
#include "otcmodifier.h"

void GEO_ModifierOnduleTonCorps_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tOtcMdfStruct* otc;

    otc = (tOtcMdfStruct*)MEM_p_Alloc(sizeof(tOtcMdfStruct));
    mdf->mdfPrivateDataPtr = otc;
    otc->flags = 0;
    if (initData)
    {
        memcpy(otc, initData, sizeof(tOtcMdfStruct));
    }
    else
    {
        otc->offset = 0.0;
        otc->intensity = 1.0;
        otc->timeMultiplier = 0.1f;
        otc->speed = 1.0;
    }
}

void GEO_ModifierOnduleTonCorps_Destroy(struct sModifier* mdf)
{
    MEM_Free(mdf->mdfPrivateDataPtr);
}

void GEO_ModifierOnduleTonCorps_UnApply(struct sModifier* mdf, struct sGEOGRO* geogro)
{
}

static float GEO_l_ModifierOtc_Amplitude(tOtcMdfStruct* otc, const uint8_t* weight,
                                         int invertWeight)
{
    if (!weight)
        return otc->intensity;
    if (invertWeight)
        return (float)((1.0 - (double)*weight * 0.00390625) * otc->intensity);
    return (float)((double)*weight * otc->intensity * 0.00390625);
}

static void GEO_l_ModifierOtc_Wave(tOtcMdfStruct* otc, Vec3* verts, unsigned int num,
                                   const uint8_t* weight, int invertWeight,
                                   int dstA, int srcA, int dstB, int srcB,
                                   int secondIsCos)
{
    unsigned int i;
    float amp;
    float* v;

    for (i = 0; i < num; i++)
    {
        amp = GEO_l_ModifierOtc_Amplitude(otc, weight, invertWeight);
        v = (float*)&verts[i];
        v[dstA] = (float)(MATH_GetSin(v[srcA] * otc->speed + otc->offset) * amp + v[dstA]);
        if (secondIsCos)
            v[dstB] = MATH_GetCos(v[srcB] * otc->speed + otc->offset) * amp + v[dstB];
        else
            v[dstB] = (float)(MATH_GetSin(v[srcB] * otc->speed + otc->offset) * amp + v[dstB]);
        if (weight)
            weight += 4;
    }
}

static void GEO_l_ModifierOtc_WaveZFromXY(tOtcMdfStruct* otc, Vec3* verts, unsigned int num)
{
    unsigned int i;
    double atOffset;
    float amp;

    atOffset = MATH_GetSin(otc->offset);
    amp = (float)(atOffset * otc->intensity + atOffset * otc->intensity);
    for (i = 0; i < num; i++)
    {
        verts[i].z = (float)((MATH_GetSin(otc->speed * verts[i].x)
                              + MATH_GetSin(verts[i].y * otc->speed)) * amp + verts[i].z);
    }
}

void GEO_ModifierOnduleTonCorps_Apply(struct sModifier* mdf, struct sGEOGRO* geogro)
{
    tOtcMdfStruct* otc;
    tGameObjectMain* gao;
    Vec3* verts;
    int* rliColors;
    const uint8_t* weight;
    unsigned int num;
    int flags;
    int invertWeight;

    otc = (tOtcMdfStruct*)mdf->mdfPrivateDataPtr;
    if ((gpst_DX_stDD->flags & 0x400) == 0)
    {
        gpst_DX_stDD->flags |= 0x400;
        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        memcpy(gpst_DX_stDD->bigBuffer->data4, geogro->vertices,
               sizeof(Vec3) * geogro->vertexNum);
    }

    gao = gpst_DX_stDD->currTargetGao;
    verts = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
    num = geogro->vertexNum;
    flags = otc->flags;
    rliColors = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors;

    if ((flags & 0x10) != 0 && rliColors)
    {
        weight = (const uint8_t*)(rliColors + 1) + 3;
        invertWeight = (flags & 0x20) != 0;
    }
    else
    {
        weight = 0;
        invertWeight = 0;
    }

    if ((flags & 1) != 0)
    {
        if ((flags & 8) != 0)
            GEO_l_ModifierOtc_Wave(otc, verts, num, weight, invertWeight, 0, 1, 0, 2, 0);
        else
            GEO_l_ModifierOtc_Wave(otc, verts, num, weight, invertWeight, 1, 0, 2, 0, 1);
    }
    else if ((flags & 2) != 0)
    {
        if ((flags & 8) != 0)
            GEO_l_ModifierOtc_Wave(otc, verts, num, weight, invertWeight, 1, 0, 1, 2, 0);
        else
            GEO_l_ModifierOtc_Wave(otc, verts, num, weight, invertWeight, 0, 1, 2, 1, 1);
    }
    else if ((flags & 4) != 0)
    {
        if ((flags & 8) != 0)
        {
            if (weight)
                GEO_l_ModifierOtc_Wave(otc, verts, num, weight, invertWeight, 2, 0, 2, 1, 0);
            else
                GEO_l_ModifierOtc_WaveZFromXY(otc, verts, num);
        }
        else
        {
            GEO_l_ModifierOtc_Wave(otc, verts, num, weight, invertWeight, 0, 2, 1, 2, 1);
        }
    }

    otc->offset = (float)(TIM_gf_dt * otc->timeMultiplier * 10.0 + otc->offset);
}

int GEO_ul_ModifierOnduleTonCorps_Load(tModifier* mdf, char* data)
{
    tOtcMdfStruct* otc;
    char* start;

    otc = (tOtcMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    LOA_ReadLong(&data, 0, 2);
    otc->flags = LOA_ReadLong(&data, 0, 1);
    otc->offset = LOA_ReadFloat(&data, 0, 1);
    otc->intensity = LOA_ReadFloat(&data, 0, 1);
    otc->speed = LOA_ReadFloat(&data, 0, 1);
    otc->timeMultiplier = LOA_ReadFloat(&data, 0, 1);
    LOA_ReadLong(&data, 0, 2);
    otc->offset = 0.0;
    return data - start;
}
