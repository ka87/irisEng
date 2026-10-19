#include "../memory.h"
#include "../loading.h"
#include "rlicartemodifier.h"

void GEO_ModifierRLICarte_Apply(struct sModifier* a1, struct sGEOGRO* a2)
{
    tRliCarteMdfStruct* v2;
    int v3;
    byte* v4;
    unsigned int* v5;
    tGameObjectMain* v6;
    tGameObjectBasic* v7;
    tGameObjectVisualObject* v8;
    int* v9;
    byte v10; 
    int v11; 
    unsigned int* v12; 
    int v13;
    int a1a;
    int a1b;

    v2 = (tRliCarteMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = v2->num;
    v2->b3 = 0;
    if (v3)
    {
        v4 = v2->indicesPtr;
        if (v4)
        {
            v5 = v2->vertexColorDestPtr;
            if (v5)
            {
                v6 = a1->gao;
                if (v6)
                {
                    v7 = v6->basic;
                    if (v7)
                    {
                        v8 = v7->actorObjectOrVisualObject;
                        if (v8)
                        {
                            v9 = v8->RLIVertexColors;
                            if (v9)
                            {
                                if ((v6->flags & 0x4000) != 0)
                                {
                                    v10 = v2->b2Max3;
                                    if (v10)
                                    {
                                        v11 = *v9;
                                        if (*v9 >= v3)
                                            v11 = v3;
                                        v12 = (unsigned int*)(v9 + 1);
                                        switch (v10)
                                        {
                                        case 1u:
                                            v2->b3 = 1;
                                            if (v11 > 0)
                                            {
                                                do
                                                {
                                                    *v5 = *v12;
                                                    *v12++ = v2->colors[*v4];
                                                    ++v5;
                                                    ++v4;
                                                    --v11;
                                                } while (v11);
                                            }
                                            break;
                                        case 2u:
                                            v2->b3 = 1;
                                            if (v11 > 0)
                                            {
                                                a1a = v11;
                                                do
                                                {
                                                    *v5 = *v12;
                                                    LIGHT_AddColor(v12++, v2->colors[*v4]);
                                                    ++v5;
                                                    ++v4;
                                                    --a1a;
                                                } while (a1a);
                                            }
                                            break;
                                        case 3u:
                                            v2->b3 = 1;
                                            if (v11 > 0)
                                            {
                                                a1b = v11;
                                                do
                                                {
                                                    *v5 = *v12;
                                                    LIGHT_SubColor(v12++, v2->colors[*v4]);
                                                    ++v5;
                                                    ++v4;
                                                    --a1b;
                                                } while (a1b);
                                            }
                                            break;
                                        }
                                        v13 = gpst_DX_stDD->flags;
                                        if ((v13 & 0x400) == 0)
                                        {
                                            gpst_DX_stDD->flags = v13 | 0x400;
                                            gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
                                            memcpy(gpst_DX_stDD->bigBuffer->data4, a2->vertices, sizeof(Vec3) * a2->vertexNum);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GEO_ModifierRLICarte_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tRliCarteMdfStruct* v3;

    v3 = (tRliCarteMdfStruct*)MEM_p_Alloc(sizeof(tRliCarteMdfStruct));
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        memcpy(v3, a3, sizeof(tRliCarteMdfStruct));
    }
    else
    {
        memset(v3, 0, sizeof(tRliCarteMdfStruct));
        memset(v3->colors, 0xFFu, sizeof(v3->colors));
        memset(v3->data, 0xFFu, sizeof(v3->data));
    }
}

void GEO_ModifierRLICarte_Destroy(tModifier* a1)
{
    tRliCarteMdfStruct* v1;
    unsigned int* v2;

    v1 = (tRliCarteMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1->indicesPtr)
        MEM_Free(v1->indicesPtr);
    v2 = v1->vertexColorDestPtr;
    v1->indicesPtr = 0;
    if (v2)
        MEM_Free(v2);
    v1->vertexColorDestPtr = 0;
    v1->num = 0;
    MEM_Free(v1);
}

void GEO_ModifierRLICarte_Unapply(struct sModifier* a1, void *unk1)
{
    tRliCarteMdfStruct* v1;
    int* v2;
    unsigned int v3;
    unsigned int* v4;
    int v5;

    v1 = (tRliCarteMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1->b3)
    {
        v1->b3 = 0;
        v2 = ((tGameObjectVisualObject*)(a1->gao->basic->actorObjectOrVisualObject))->RLIVertexColors;
        v3 = *v2;
        v4 = v1->vertexColorDestPtr;
        v5 = v1->num;
        if (*v2 >= v5)
            v3 = v5;
        memcpy(v2 + 1, v4, 4 * v3);
    }
}

int GEO_ul_ModifierRLICarte_Load(struct sModifier* a1, char* a2)
{
    tRliCarteMdfStruct* v2;
    int* v3;
    int v4; 
    int v5;
    char* v6; 
    unsigned int* v7; 
    char* v8; 
    int v10;
    char* v11;

    v2 = (tRliCarteMdfStruct*)a1->mdfPrivateDataPtr;
    v11 = a2;
    LOA_ReadLong(&v11, 0, 2);
    v3 = v2->data;
    v4 = 64;
    do
    {
        *v3++ = LOA_ReadLong(&v11, 0, 1);
        --v4;
    } while (v4);
    v2->b1 = LOA_ReadChar(&v11, 0, 1);
    v2->b2Max3 = LOA_ReadChar(&v11, 0, 1);
    v2->b3 = LOA_ReadChar(&v11, 0, 1);
    v2->b4 = LOA_ReadChar(&v11, 0, 1);
    if (v2->b2Max3 > 3u)
        v2->b2Max3 = 0;
    v5 = LOA_ReadLong(&v11, 0, 1);
    v2->num = v5;
    if (v5)
    {
        v6 = (char*)MEM_p_Alloc(v5);
        v10 = v2->num;
        v2->indicesPtr = v6;
        LOA_ReadCharArray(&v11, v6, v10, 1);
        v7 = (unsigned int*)MEM_p_Alloc(4 * v2->num);
        v8 = a2;
        v2->vertexColorDestPtr = v7;
    }
    else
    {
        v8 = a2;
        v2->indicesPtr = 0;
    }
    return v11 - v8;
}

void GEO_ModifierRLICarte_Reinit(struct sModifier* a1)
{
    memcpy(((tRliCarteMdfStruct*)(a1->mdfPrivateDataPtr))->colors, ((tRliCarteMdfStruct*)(a1->mdfPrivateDataPtr))->data, sizeof(((tRliCarteMdfStruct*)(a1->mdfPrivateDataPtr))->colors));
}