#include "../memory.h"
#include "../loading.h"
#include "../gameobject.h"
#include "../graphicsobjects/staticlod.h"
#include "morphmodifier.h"

struct sMorphMdfStruct* GEO_pst_ModifierMorphing_Get(struct sGameObjectMain* gao)
{
    tModifier* mdf;

    mdf = MDF_pst_GetByType(gao, 4); // morph
    if (!mdf)
        return 0;
    return (tMorphMdfStruct*)mdf->mdfPrivateDataPtr;
}

void GEO_ModifierMorphing_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tMorphMdfStruct* morph;
    struct sMorphMdfStruct* src = (struct sMorphMdfStruct*)initData;
    tGameObjectBasic* basic;
    tGEOGRO* geo;
    int count;
    int i;

    morph = (tMorphMdfStruct*)MEM_p_Alloc(sizeof(tMorphMdfStruct));
    mdf->mdfPrivateDataPtr = morph;
    if (!src)
    {
        memset(morph, 0, sizeof(tMorphMdfStruct));
        basic = gao->basic;
        if (basic->actorObjectOrVisualObject)
        {
            geo = (tGEOGRO*)(((tGameObjectVisualObject*)(basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
            if (geo && geo->intf->id == 1)
                morph->i2 = geo->vertexNum;
        }
        return;
    }

    memcpy(morph, src, sizeof(tMorphMdfStruct));
    if (morph->num)
    {
        morph->ptr = (tMorphMdfInnerStruct*)MEM_p_Alloc(sizeof(tMorphMdfInnerStruct) * morph->num);
        memcpy(morph->ptr, src->ptr, sizeof(tMorphMdfInnerStruct) * morph->num);
        for (i = 0; i < morph->num; i++)
        {
            count = morph->ptr[i].num;
            if (count)
            {
                morph->ptr[i].ptr1 = (int*)MEM_p_Alloc(4 * count);
                memcpy(morph->ptr[i].ptr1, src->ptr[i].ptr1, 4 * count);
                morph->ptr[i].ptr2 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * count);
                memcpy(morph->ptr[i].ptr2, src->ptr[i].ptr2, sizeof(Vec3) * count);
            }
        }
    }

    if (morph->num2)
    {
        morph->ptr2 = (tMorphMdfInner2Struct*)MEM_p_Alloc(sizeof(tMorphMdfInner2Struct) * morph->num2);
        memcpy(morph->ptr2, src->ptr2, sizeof(tMorphMdfInner2Struct) * morph->num2);
        for (i = 0; i < morph->num2; i++)
        {
            count = morph->ptr2[i].num;
            if (count)
            {
                morph->ptr2[i].ptr = (int*)MEM_p_Alloc(4 * count);
                memcpy(morph->ptr2[i].ptr, src->ptr2[i].ptr, 4 * count);
            }
        }
    }
}

void GEO_ModifierMorphing_Destroy(struct sModifier* mdf)
{
    tMorphMdfStruct* morph;
    int i;

    morph = (tMorphMdfStruct*)mdf->mdfPrivateDataPtr;
    if (morph->num)
    {
        for (i = 0; i < morph->num; i++)
        {
            if (morph->ptr[i].num)
            {
                MEM_Free(morph->ptr[i].ptr1);
                MEM_Free(morph->ptr[i].ptr2);
            }
        }
        MEM_Free(morph->ptr);
    }

    if (morph->num2)
    {
        for (i = 0; i < morph->num2; i++)
        {
            if (morph->ptr2[i].num)
                MEM_Free(morph->ptr2[i].ptr);
        }
        MEM_Free(morph->ptr2);
    }

    MEM_Free(morph);
}

void GEO_ModifierMorphing_UnApply(struct sModifier* mdf, struct sGEOGRO* geogro)
{
}

static void GEO_l_ModifierMorphing_AddTarget(Vec3* verts, tMorphMdfInnerStruct* target,
                                             float weight)
{
    Vec3 scaled;
    int k;

    if (weight == 1.0)
    {
        for (k = 0; k < target->num; k++)
            MATH_VecAddVecWithoutBuffer(&verts[target->ptr1[k]], &target->ptr2[k]);
    }
    else
    {
        for (k = 0; k < target->num; k++)
        {
            MATH_VecMultScalar(&scaled, &target->ptr2[k], weight);
            MATH_VecAddVecWithoutBuffer(&verts[target->ptr1[k]], &scaled);
        }
    }
}

void GEO_ModifierMorphing_Apply(struct sModifier* mdf, struct sGEOGRO* geogro)
{
    tMorphMdfStruct* morph;
    tMorphMdfInner2Struct* chan;
    tGEOGRO* geo;
    tGEOSTATICLOD* statlod;
    Vec3* verts;
    int* keys;
    int ch;
    int keyCount;
    int upper;
    int lower;
    float blend;

    morph = (tMorphMdfStruct*)mdf->mdfPrivateDataPtr;
    if (!morph->num2 || !morph->i2 || !geogro)
        return;

    geo = geogro;
    verts = (Vec3*)gpst_DX_stDD->bigBuffer->data4;

    if (geogro->intf->id == 8)
    {
        statlod = (tGEOSTATICLOD*)geogro;
        if (statlod->distance[0] >= statlod->currDistance && statlod->lodNum > 1u)
            return;
        geo = statlod->geo[0];
    }
    if (geo->intf->id != 1)
        return;

    QueryPerformanceCounter(&PerfCounterStruct[52]);
    if ((gpst_DX_stDD->flags & 0x400) == 0)
    {
        gpst_DX_stDD->flags |= 0x400;
        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        memcpy(verts, geo->vertices, sizeof(Vec3) * geo->vertexNum);
    }

    for (ch = 0; ch < morph->num2; ch++)
    {
        chan = &morph->ptr2[ch];
        keyCount = chan->num;
        if (!keyCount || chan->Factor == 0.0 || chan->Prog == 0.0)
            continue;

        keys = chan->ptr;
        lower = (int)(unsigned int)chan->Prog;
        for (upper = lower; upper < keyCount; upper++)
        {
            if ((unsigned int)keys[upper] < (unsigned int)morph->num)
                break;
        }

        if (upper < keyCount)
        {
            lower--;
            while (lower >= 0 && (unsigned int)keys[lower] >= (unsigned int)morph->num)
                lower--;
            blend = (float)((chan->Prog - (double)lower - 1.0) / (double)(upper - lower));
        }
        else
        {
            upper = keyCount - 1;
            blend = 1.0;
            while (upper > 0 && (unsigned int)keys[upper] >= (unsigned int)morph->num)
                upper--;
            if ((unsigned int)keys[upper] >= (unsigned int)morph->num)
                continue;
        }

        if (blend > 0.0)
            GEO_l_ModifierMorphing_AddTarget(verts, &morph->ptr[keys[upper]],
                                             blend * chan->Factor);

        if (lower >= 0 && (float)(1.0 - blend) > 0.0)
            GEO_l_ModifierMorphing_AddTarget(verts, &morph->ptr[keys[lower]],
                                             (float)(1.0 - blend) * chan->Factor);
    }

    PRO_QPC_RecordTimeDecRef(52);
}

int GEO_ul_ModifierMorphing_Load(struct sModifier* mdf, char* data)
{
    tMorphMdfStruct* morph;
    tMorphMdfInnerStruct* target;
    tMorphMdfInner2Struct* chan;
    char* start;
    int i;
    int k;

    morph = (tMorphMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    LOA_ReadLong(&data, 0, 1);
    morph->i1 = LOA_ReadLong(&data, 0, 1);
    morph->i2 = LOA_ReadLong(&data, 0, 1);
    morph->num = LOA_ReadLong(&data, 0, 1);
    morph->num2 = LOA_ReadLong(&data, 0, 1);

    if (morph->num)
        morph->ptr = (tMorphMdfInnerStruct*)MEM_p_Alloc(sizeof(tMorphMdfInnerStruct) * morph->num);
    else
        morph->ptr = 0;
    if (morph->num2)
        morph->ptr2 = (tMorphMdfInner2Struct*)MEM_p_Alloc(sizeof(tMorphMdfInner2Struct) * morph->num2);
    else
        morph->ptr2 = 0;

    for (i = 0; i < morph->num; i++)
    {
        target = &morph->ptr[i];
        target->num = LOA_ReadLong(&data, 0, 1);
        LOA_ReadString(&data, 0, 64, 2);
        if (target->num)
        {
            target->ptr1 = (int*)MEM_p_Alloc(4 * target->num);
            for (k = 0; k < target->num; k++)
                target->ptr1[k] = LOA_ReadLong(&data, 0, 1);
            target->ptr2 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * target->num);
            for (k = 0; k < target->num; k++)
                LOA_ReadVector(&data, &target->ptr2[k], 1);
        }
        else
        {
            target->ptr1 = 0;
            target->ptr2 = 0;
        }
    }

    for (i = 0; i < morph->num2; i++)
    {
        chan = &morph->ptr2[i];
        chan->num = LOA_ReadLong(&data, 0, 1);
        chan->Factor = LOA_ReadFloat(&data, 0, 1);
        chan->Prog = LOA_ReadFloat(&data, 0, 1);
        LOA_ReadString(&data, 0, 64, 2);
        if (chan->num)
        {
            chan->ptr = (int*)MEM_p_Alloc(4 * chan->num);
            for (k = 0; k < chan->num; k++)
                chan->ptr[k] = LOA_ReadLong(&data, 0, 1);
        }
    }

    return data - start;
}
