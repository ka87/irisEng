#include "../memory.h"
#include "../loading.h"
#include "rlicartemodifier.h"

void GEO_ModifierRLICarte_Apply(struct sModifier* mdf, struct sGEOGRO* geo)
{
    tRliCarteMdfStruct* rli;
    tGameObjectMain* gao;
    tGameObjectVisualObject* visual;
    int* rliColors;
    unsigned int* live;
    unsigned int* saved;
    uint8_t* indices;
    int count;
    int i;

    rli = (tRliCarteMdfStruct*)mdf->mdfPrivateDataPtr;
    rli->b3 = 0;
    if (!rli->num || !rli->indicesPtr || !rli->vertexColorDestPtr)
        return;

    gao = mdf->gao;
    if (!gao || !gao->basic)
        return;

    visual = (tGameObjectVisualObject*)gao->basic->actorObjectOrVisualObject;
    if (!visual || !visual->RLIVertexColors)
        return;
    if ((gao->flags & 0x4000) == 0 || !rli->b2Max3)
        return;

    rliColors = visual->RLIVertexColors;
    count = *rliColors;
    if (count >= rli->num)
        count = rli->num;
    live = (unsigned int*)(rliColors + 1);
    saved = rli->vertexColorDestPtr;
    indices = (uint8_t*)rli->indicesPtr;

    switch (rli->b2Max3)
    {
    case 1:
        rli->b3 = 1;
        for (i = 0; i < count; i++)
        {
            saved[i] = live[i];
            live[i] = rli->colors[indices[i]];
        }
        break;
    case 2:
        rli->b3 = 1;
        for (i = 0; i < count; i++)
        {
            saved[i] = live[i];
            LIGHT_AddColor(&live[i], rli->colors[indices[i]]);
        }
        break;
    case 3:
        rli->b3 = 1;
        for (i = 0; i < count; i++)
        {
            saved[i] = live[i];
            LIGHT_SubColor(&live[i], rli->colors[indices[i]]);
        }
        break;
    }

    if ((gpst_DX_stDD->flags & 0x400) == 0)
    {
        gpst_DX_stDD->flags |= 0x400;
        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        memcpy(gpst_DX_stDD->bigBuffer->data4, geo->vertices, sizeof(Vec3) * geo->vertexNum);
    }
}

void GEO_ModifierRLICarte_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tRliCarteMdfStruct* rli;

    rli = (tRliCarteMdfStruct*)MEM_p_Alloc(sizeof(tRliCarteMdfStruct));
    mdf->mdfPrivateDataPtr = rli;
    if (initData)
    {
        memcpy(rli, initData, sizeof(tRliCarteMdfStruct));
    }
    else
    {
        memset(rli, 0, sizeof(tRliCarteMdfStruct));
        memset(rli->colors, 0xFFu, sizeof(rli->colors));
        memset(rli->data, 0xFFu, sizeof(rli->data));
    }
}

void GEO_ModifierRLICarte_Destroy(tModifier* mdf)
{
    tRliCarteMdfStruct* rli;

    rli = (tRliCarteMdfStruct*)mdf->mdfPrivateDataPtr;
    if (rli->indicesPtr)
        MEM_Free(rli->indicesPtr);
    rli->indicesPtr = 0;
    if (rli->vertexColorDestPtr)
        MEM_Free(rli->vertexColorDestPtr);
    rli->vertexColorDestPtr = 0;
    rli->num = 0;
    MEM_Free(rli);
}

void GEO_ModifierRLICarte_Unapply(struct sModifier* mdf, void* unk1)
{
    tRliCarteMdfStruct* rli;
    int* rliColors;
    unsigned int count;

    rli = (tRliCarteMdfStruct*)mdf->mdfPrivateDataPtr;
    if (rli->b3)
    {
        rli->b3 = 0;
        rliColors = ((tGameObjectVisualObject*)(mdf->gao->basic->actorObjectOrVisualObject))->RLIVertexColors;
        count = *rliColors;
        if (*rliColors >= rli->num)
            count = rli->num;
        memcpy(rliColors + 1, rli->vertexColorDestPtr, 4 * count);
    }
}

int GEO_ul_ModifierRLICarte_Load(struct sModifier* mdf, char* data)
{
    tRliCarteMdfStruct* rli;
    char* start;
    int count;
    int i;

    rli = (tRliCarteMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    LOA_ReadLong(&data, 0, 2);
    for (i = 0; i < 64; i++)
        rli->data[i] = LOA_ReadLong(&data, 0, 1);
    rli->b1 = LOA_ReadChar(&data, 0, 1);
    rli->b2Max3 = LOA_ReadChar(&data, 0, 1);
    rli->b3 = LOA_ReadChar(&data, 0, 1);
    rli->b4 = LOA_ReadChar(&data, 0, 1);
    if (rli->b2Max3 > 3u)
        rli->b2Max3 = 0;
    count = LOA_ReadLong(&data, 0, 1);
    rli->num = count;
    if (count)
    {
        rli->indicesPtr = (char*)MEM_p_Alloc(count);
        LOA_ReadCharArray(&data, rli->indicesPtr, rli->num, 1);
        rli->vertexColorDestPtr = (unsigned int*)MEM_p_Alloc(4 * rli->num);
    }
    else
    {
        rli->indicesPtr = 0;
    }
    return data - start;
}

void GEO_ModifierRLICarte_Reinit(struct sModifier* mdf)
{
    tRliCarteMdfStruct* rli;

    rli = (tRliCarteMdfStruct*)mdf->mdfPrivateDataPtr;
    memcpy(rli->colors, rli->data, sizeof(rli->colors));
}
