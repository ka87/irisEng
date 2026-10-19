#include "../memory.h"
#include "../loading.h"
#include "symetriemodifier.h"

void GEO_ModifierSymetrie_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a)
{
    tSymMdfStruct* v3;
    struct sSymMdfStruct* a3 = (struct sSymMdfStruct*)a3a;

    v3 = (tSymMdfStruct*)MEM_p_Alloc(sizeof(tSymMdfStruct));
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        *v3 = *a3;
    }
    else
    {
        v3->flags = 1;
        v3->i2 = 0.0;
    }
}

void GEO_ModifierSymetrie_Destroy(struct sModifier* a1)
{
    MEM_Free(a1->mdfPrivateDataPtr);
}

void GEO_ModifierSymetrie_Apply(tModifier* a1, tGEOGRO* a2)
{
    tGEOGRO* v2;
    tSymMdfStruct* v3;
    byte* v4;
    Vec3* v5;
    Vec3* v6;
    Vec3* v7;
    unsigned int v8;
    double v9;
    unsigned int v10;
    float a1a;

    v2 = a2;
    v3 = (tSymMdfStruct*)a1->mdfPrivateDataPtr;
    v4 = gpst_DX_stDD->bigBuffer->data4;
    GEO_UseNormals(a2);
    v5 = a2->normals;
    if ((gpst_DX_stDD->flags & 0x400) != 0)
    {
        v6 = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
    }
    else
    {
        v6 = a2->vertices;
        memcpy(v4, v6, 12 * a2->vertexNum);
        v2 = a2;
    }
    v7 = &v6[v2->vertexNum];
    if ((v3->flags & 2) != 0)
    {
        v6 = (Vec3*)((char*)v6 + 4);
        v4 += 4;
        v5 = (Vec3*)((char*)v5 + 4);
    }
    else if ((v3->flags & 4) != 0)
    {
        v6 = (Vec3*)((char*)v6 + 8);
        v4 += 8;
        v5 = (Vec3*)((char*)v5 + 8);
    }
    if (v3->i2 == 0.0)
    {
        if (v6 < v7)
        {
            v8 = ((char*)v7 - (char*)v6 - 1) / sizeof(Vec3) + 1;
            do
            {
                v9 = *(float*)v4;
                *((float*)v4) = (float)-v9;
                v5->x = -v5->x;
                v4 += sizeof(Vec3);
                ++v5;
                --v8;
            } while (v8);
        }
    }
    else
    {
        a1a = v3->i2 + v3->i2;
        if (v6 < v7)
        {
            v10 = ((char*)v7 - (char*)v6 - 1) / sizeof(Vec3) + 1;
            do
            {
                *((float*)v4) = a1a - *((float*)v4);
                v5->x = -v5->x;
                v4 += sizeof(Vec3);
                ++v5;
                --v10;
            } while (v10);
        }
    }
    gpst_DX_stDD->flags |= 0x400u;
    gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
}

void GEO_ModifierSymetrie_Unapply(struct sModifier* a1, struct sGEOGRO* a2)
{
    tSymMdfStruct* v2;
    Vec3* v3;
    Vec3* v4;
    double v5;

    v2 = (tSymMdfStruct*)a1->mdfPrivateDataPtr;
    GEO_UseNormals(a2);
    v3 = a2->normals;
    v4 = &v3[a2->vertexNum];
    if ((v2->flags & 2) != 0)
    {
        v3 = (Vec3*)((char*)v3 + 4);
    }
    else if ((v2->flags & 4) != 0)
    {
        v3 = (Vec3*)((char*)v3 + 8);
    }
    for (; v3 < v4; v3++)
    {
        v5 = v3->x;
        v3->x = (float)-v5;
    }
}

int GEO_ul_ModifierSymetrie_Load(struct sModifier* a1, char* a2a)
{
    tSymMdfStruct* v2;
    char* v3;

    v2 = (tSymMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = a2a;
    LOA_ReadLong(&a2a, 0, 2);
    v2->flags = LOA_ReadLong(&a2a, 0, 1);
    v2->i2 = LOA_ReadFloat(&a2a, 0, 1);
    return a2a - v3;
}