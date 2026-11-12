#include "../memory.h"
#include "../loading.h"
#include "symetriemodifier.h"

void GEO_ModifierSymetrie_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tSymMdfStruct* sym;
    struct sSymMdfStruct* src = (struct sSymMdfStruct*)initData;

    sym = (tSymMdfStruct*)MEM_p_Alloc(sizeof(tSymMdfStruct));
    mdf->mdfPrivateDataPtr = sym;
    if (src)
    {
        *sym = *src;
    }
    else
    {
        sym->flags = 1;
        sym->i2 = 0.0;
    }
}

void GEO_ModifierSymetrie_Destroy(struct sModifier* mdf)
{
    MEM_Free(mdf->mdfPrivateDataPtr);
}

static int GEO_l_ModifierSymetrie_Axis(int flags)
{
    if ((flags & 2) != 0)
        return 1;
    if ((flags & 4) != 0)
        return 2;
    return 0;
}

void GEO_ModifierSymetrie_Apply(tModifier* mdf, tGEOGRO* geo)
{
    tSymMdfStruct* sym;
    Vec3* work;
    float* vert;
    float* norm;
    float mirror;
    int axis;
    unsigned int i;

    sym = (tSymMdfStruct*)mdf->mdfPrivateDataPtr;
    GEO_UseNormals(geo);
    work = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
    if ((gpst_DX_stDD->flags & 0x400) == 0)
        memcpy(work, geo->vertices, sizeof(Vec3) * geo->vertexNum);

    axis = GEO_l_ModifierSymetrie_Axis(sym->flags);
    vert = (float*)work + axis;
    norm = (float*)geo->normals + axis;

    if (sym->i2 == 0.0)
    {
        for (i = 0; i < geo->vertexNum; i++)
        {
            vert[i * 3] = -vert[i * 3];
            norm[i * 3] = -norm[i * 3];
        }
    }
    else
    {
        mirror = sym->i2 + sym->i2;
        for (i = 0; i < geo->vertexNum; i++)
        {
            vert[i * 3] = mirror - vert[i * 3];
            norm[i * 3] = -norm[i * 3];
        }
    }

    gpst_DX_stDD->flags |= 0x400u;
    gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
}

void GEO_ModifierSymetrie_Unapply(struct sModifier* mdf, struct sGEOGRO* geo)
{
    tSymMdfStruct* sym;
    float* norm;
    unsigned int i;

    sym = (tSymMdfStruct*)mdf->mdfPrivateDataPtr;
    GEO_UseNormals(geo);
    norm = (float*)geo->normals + GEO_l_ModifierSymetrie_Axis(sym->flags);
    for (i = 0; i < geo->vertexNum; i++)
        norm[i * 3] = -norm[i * 3];
}

int GEO_ul_ModifierSymetrie_Load(struct sModifier* mdf, char* data)
{
    tSymMdfStruct* sym;
    char* start;

    sym = (tSymMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    LOA_ReadLong(&data, 0, 2);
    sym->flags = LOA_ReadLong(&data, 0, 1);
    sym->i2 = LOA_ReadFloat(&data, 0, 1);
    return data - start;
}
