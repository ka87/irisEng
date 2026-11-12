#include "../common.h"
#include "../memory.h"
#include "../gameobject.h"
#include "../graphicsobjects/geo.h"
#include "../gdi.h"
#include "../loading.h"
#include "stpmodifier.h"

// type 21

void GEO_ModifierSTP_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a)
{
    tStpMdfStruct* v3;
    tStpMdfStruct* v4;
    struct sStpMdfStruct* a3 = (struct sStpMdfStruct*)a3a;

    v3 = (tStpMdfStruct*)MEM_p_Alloc(sizeof(tStpMdfStruct));
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        *v3 = *a3;
        v4 = (tStpMdfStruct*)a2->mdfPrivateDataPtr;
        if (v4->num)
            v4->verticesStored = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v4->num);
        else
            v4->verticesStored = 0;
        v4->hasStp = 0;
    }
    else
    {
        v3->num = 0;
        v3->hasStp = 0;
        v3->verticesStored = 0;
    }
}

void GEO_ModifierSTP_Destroy(struct sModifier* a1)
{
    tStpMdfStruct* v1;

    v1 = (tStpMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1->num)
        MEM_Free(v1->verticesStored);
    MEM_Free(v1);
}

void GEO_ModifierSTP_Apply(struct sModifier* a1, struct sGEOGRO* a2)
{
    tStpMdfStruct* v2;
    int v3;

    v2 = (tStpMdfStruct*)a1->mdfPrivateDataPtr;
    if (v2->num != a2->vertexNum)
    {
        if (v2->num)
            MEM_Free(v2->verticesStored);
        v3 = a2->vertexNum;
        if (v3)
        {
            v2->verticesStored = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v3);
            v2->num = a2->vertexNum;
            v2->hasStp = 0;
            return;
        }
        v2->verticesStored = 0;
        v2->num = a2->vertexNum;
    }
    v2->hasStp = 0;
}

void GEO_ModifierSTP_Unapply(struct sModifier* a1, void * unk1)
{
    tStpMdfStruct* v1;

    v1 = (tStpMdfStruct*)a1->mdfPrivateDataPtr;
    if ((gpst_DX_stDD->flags & 0x400) != 0)
    {
        memcpy(v1->verticesStored, gpst_DX_stDD->bigBuffer->data4, sizeof(Vec3) * v1->num);
        v1->hasStp = 1;
    }
}

int GEO_ul_ModifierSTP_Load(struct sModifier* a1, char* a2)
{
    tStpMdfStruct* v2;
    char* v3;
    int v4;
    char* v6;

    v2 = (tStpMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = a2;
    v6 = a2;
    LOA_ReadLong(&v6, 0, 2);
    v4 = LOA_ReadLong(&v6, 0, 1);
    v2->num = v4;
    v2->hasStp = 0;
    if (v4)
        v2->verticesStored = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v4);
    else
        v2->verticesStored = 0;
    return v6 - v3;
}