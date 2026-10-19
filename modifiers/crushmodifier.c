#include "../memory.h"
#include "../loading.h"
#include "crushmodifier.h"

void GEO_ModifierCrush_Apply(struct sModifier* mdf, struct sGEOGRO* geogro)
{
    tCrushMdfStruct* crushMdf;
    Vec3* currPtr;
    Vec3* endPtr;
    float limit;

    crushMdf = ((tCrushMdfStruct*)mdf->mdfPrivateDataPtr);
    if ((gpst_DX_stDD->flags & 0x400) == 0)
    {
        gpst_DX_stDD->flags |= 0x400;
        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        memcpy(gpst_DX_stDD->bigBuffer->data4, geogro->vertices, sizeof(Vec3) * geogro->vertexNum);
    }

    currPtr = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
    endPtr = &currPtr[geogro->vertexNum];

    if ((crushMdf->flags & 0x8) != 0)
    {
        limit = (crushMdf->max - crushMdf->min) * crushMdf->factor + crushMdf->min;
        if ((crushMdf->flags & 0x4) != 0)
        {
            for (; currPtr < endPtr; ++currPtr)
            {
                if (currPtr->z < limit)
                    currPtr->z = limit;
            }
        }
        else if ((crushMdf->flags & 0x2) != 0)
        {
            for (; currPtr < endPtr; ++currPtr)
            {
                if (currPtr->y < limit)
                    currPtr->y = limit;
            }
        }
        else
        {
            for (; currPtr < endPtr; ++currPtr)
            {
                if (currPtr->x < limit)
                    currPtr->x = limit;
            }
        }
    }
    else
    {
        limit = (crushMdf->min - crushMdf->max) * crushMdf->factor + crushMdf->max;
        if ((crushMdf->flags & 0x4) != 0)
        {
            for (; currPtr < endPtr; ++currPtr)
            {
                if (currPtr->z > limit)
                    currPtr->z = limit;
            }
        }
        else if ((crushMdf->flags & 0x2) != 0)
        {
            for (; currPtr < endPtr; ++currPtr)
            {
                if (currPtr->y > limit)
                    currPtr->y = limit;
            }
        }
        else
        {
            for (; currPtr < endPtr; ++currPtr)
            {
                if (currPtr->x > limit)
                    currPtr->x = limit;
            }
        }
    }
}

void GEO_ModifierCrush_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* privateData)
{
    tCrushMdfStruct* crushMdf = (tCrushMdfStruct*)MEM_p_Alloc(sizeof(tCrushMdfStruct));

    mdf->mdfPrivateDataPtr = crushMdf;

    if (privateData)
    {
        memcpy(crushMdf, privateData, sizeof(tCrushMdfStruct));
    }
    else
    {
        crushMdf->flags = 0;
        crushMdf->min = 0.0f;
        crushMdf->max = 0.0f;
        crushMdf->factorDefault = 0.0f;
        crushMdf->factor = 0.0f;
    }
}

void GEO_ModifierCrush_Destroy(struct sModifier* mdf)
{
    MEM_Free(mdf->mdfPrivateDataPtr);
}

int GEO_ul_ModifierCrush_Load(struct sModifier* mdf, char *readPtr)
{
    tCrushMdfStruct* crushMdf;
    char *readPtrBak;
    double factor;

    crushMdf = (tCrushMdfStruct*)mdf->mdfPrivateDataPtr;
    readPtrBak = readPtr;

    LOA_ReadLong(&readPtr, 0, 2);
    crushMdf->max = LOA_ReadFloat(&readPtr, 0, 1);
    crushMdf->min = LOA_ReadFloat(&readPtr, 0, 1);
    
    factor = LOA_ReadFloat(&readPtr, 0, 1);
    crushMdf->factorDefault = (float)factor;
    crushMdf->factor = (float)factor;
    
    return readPtr - readPtrBak;
}

void GEO_ModifierCrush_Reinit(struct sModifier* mdf)
{
    ((tCrushMdfStruct*)(mdf->mdfPrivateDataPtr))->factor = ((tCrushMdfStruct*)(mdf->mdfPrivateDataPtr))->factorDefault;
}

void GEO_ModifierCrush_Unapply(struct sModifier* mdf, struct sGEOGRO* geogro) {
}