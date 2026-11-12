#include "../common.h"
#include "../memory.h"
#include "../gameobject.h"
#include "../graphicsobjects/geo.h"
#include "../gdi.h"
#include "../loading.h"
#include "stpmodifier.h"

// type 21

void GEO_ModifierSTP_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tStpMdfStruct* stp;
    struct sStpMdfStruct* src = (struct sStpMdfStruct*)initData;

    stp = (tStpMdfStruct*)MEM_p_Alloc(sizeof(tStpMdfStruct));
    mdf->mdfPrivateDataPtr = stp;
    if (src)
    {
        *stp = *src;
        if (stp->num)
            stp->verticesStored = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * stp->num);
        else
            stp->verticesStored = 0;
        stp->hasStp = 0;
    }
    else
    {
        stp->num = 0;
        stp->hasStp = 0;
        stp->verticesStored = 0;
    }
}

void GEO_ModifierSTP_Destroy(struct sModifier* mdf)
{
    tStpMdfStruct* stp;

    stp = (tStpMdfStruct*)mdf->mdfPrivateDataPtr;
    if (stp->num)
        MEM_Free(stp->verticesStored);
    MEM_Free(stp);
}

void GEO_ModifierSTP_Apply(struct sModifier* mdf, struct sGEOGRO* geo)
{
    tStpMdfStruct* stp;

    stp = (tStpMdfStruct*)mdf->mdfPrivateDataPtr;
    if (stp->num != geo->vertexNum)
    {
        if (stp->num)
            MEM_Free(stp->verticesStored);
        if (geo->vertexNum)
            stp->verticesStored = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * geo->vertexNum);
        else
            stp->verticesStored = 0;
        stp->num = geo->vertexNum;
    }
    stp->hasStp = 0;
}

void GEO_ModifierSTP_Unapply(struct sModifier* mdf, void* unk1)
{
    tStpMdfStruct* stp;

    stp = (tStpMdfStruct*)mdf->mdfPrivateDataPtr;
    if ((gpst_DX_stDD->flags & 0x400) != 0)
    {
        memcpy(stp->verticesStored, gpst_DX_stDD->bigBuffer->data4, sizeof(Vec3) * stp->num);
        stp->hasStp = 1;
    }
}

int GEO_ul_ModifierSTP_Load(struct sModifier* mdf, char* data)
{
    tStpMdfStruct* stp;
    char* start;
    int count;

    stp = (tStpMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    LOA_ReadLong(&data, 0, 2);
    count = LOA_ReadLong(&data, 0, 1);
    stp->num = count;
    stp->hasStp = 0;
    if (count)
        stp->verticesStored = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * count);
    else
        stp->verticesStored = 0;
    return data - start;
}
