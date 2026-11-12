#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "explodemodifier.h"

void GAO_ModifierExplode_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tGroGeneric* gro;
    tGEOGRO* geo;
    tGEOGRO* newGeo;
    tExplodeMdfStruct* expl;
    tGeoTriangleChunks* src;
    tGeoTriangleChunks* dst;
    tGeoTriangleChunkElem* elem;
    Vec3* newVerts;
    Vec3* newNormals;
    Vec3 sum;
    Vec3 normal;
    int triangles;
    int nextVertex;
    int chunk;
    unsigned int t;

    mdf->modifierFlags |= 0x1000000u;
    gro = OBJ_p_GetGro(gao);
    if (!gro || gro->intf->id != 1)
    {
        mdf->mdfPrivateDataPtr = 0;
        return;
    }

    geo = (tGEOGRO*)gro;
    expl = (tExplodeMdfStruct*)MEM_p_Alloc(sizeof(tExplodeMdfStruct));
    triangles = GEO_l_GetNumberOfTriangles(geo);
    expl->trinagleNum = triangles;
    expl->vertexNum = 3 * triangles;
    expl->triangleChunkNum = geo->triangleChunkCount;
    expl->f1 = 0.0;
    expl->f3 = -0.5;
    expl->f2 = 1.0;

    newGeo = GEO_pst_Create(expl->vertexNum, 0, expl->triangleChunkNum, 0);
    expl->geogroNew = newGeo;
    newGeo->uvNum = geo->uvNum;
    newGeo->UVs = geo->UVs;
    newVerts = newGeo->vertices;
    GEO_UseNormals(newGeo);
    newNormals = newGeo->normals;

    nextVertex = 0;
    for (chunk = 0; chunk < expl->triangleChunkNum; chunk++)
    {
        /* Subscript the chunk array by element, not by byte. This used to
         * index [chunk * 32], sizeof(tGeoTriangleChunks) being 32, which on
         * a typed pointer reaches 32 elements further rather than one and
         * so ran 1KB past the array from the second chunk onwards. The
         * memcpy below always used the element index, so the two disagreed
         * about which chunk they were working on. */
        src = &geo->triangleChunks[chunk];
        dst = &newGeo->triangleChunks[chunk];
        dst->triangleNum = src->triangleNum;
        dst->materialNum = src->materialNum;
        dst->MRMUVIndicesNum = 0;
        GEO_AllocElementContent(dst);
        memcpy(dst->triangleChunkElems, src->triangleChunkElems,
               4 * ((unsigned int)(sizeof(tGeoTriangleChunkElem) * dst->triangleNum) >> 2));

        for (t = 0; t < dst->triangleNum; t++)
        {
            elem = &dst->triangleChunkElems[t];
            newVerts[nextVertex] = geo->vertices[elem->v1];
            newVerts[nextVertex + 1] = geo->vertices[elem->v2];
            newVerts[nextVertex + 2] = geo->vertices[elem->v3];

            sum.x = newVerts[nextVertex].x + newVerts[nextVertex + 1].x + newVerts[nextVertex + 2].x;
            sum.y = newVerts[nextVertex].y + newVerts[nextVertex + 1].y + newVerts[nextVertex + 2].y;
            sum.z = newVerts[nextVertex].z + newVerts[nextVertex + 1].z + newVerts[nextVertex + 2].z;
            MATH_NormalizeVectorWithNullVectorCheck(&normal, &sum);
            newNormals[nextVertex] = normal;
            newNormals[nextVertex + 1] = normal;
            newNormals[nextVertex + 2] = normal;

            elem->v1 = nextVertex;
            elem->v2 = nextVertex + 1;
            elem->v3 = nextVertex + 2;
            nextVertex += 3;
        }
    }

    expl->gaogeogro = geo;
    mdf->mdfPrivateDataPtr = expl;
}

static void GAO_l_ModifierExplode_PosAt(Vec3* out, Vec3* pos, Vec3* normal,
                                        float t, float gravity)
{
    out->x = pos->x + t * normal->x;
    out->y = pos->y + t * normal->y;
    out->z = pos->z + t * normal->z + gravity;
}

void GAO_ModifierExplode_Apply(struct sModifier* mdf, void* unk1)
{
    tExplodeMdfStruct* expl;
    tGameObjectVisualObject* visual;
    tGEOGRO* geo;
    Vec3 at0;
    Vec3 at1;
    float t0;
    float t1;
    float scale;
    unsigned int i;

    expl = (tExplodeMdfStruct*)mdf->mdfPrivateDataPtr;
    if (!expl)
        return;

    visual = (tGameObjectVisualObject*)
             gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject;
    expl->gaogeogro = (tGEOGRO*)visual->GROGEOOrSomethingElse;
    visual->GROGEOOrSomethingElse = (struct sGroGeneric*)expl->geogroNew;

    geo = expl->geogroNew;
    GEO_UseNormals(geo);

    t0 = expl->f1;
    t1 = TIM_gf_dt + t0;
    scale = expl->f2;

    for (i = 0; i < geo->vertexNum; i++)
    {
        GAO_l_ModifierExplode_PosAt(&at0, &geo->vertices[i], &geo->normals[i],
                                    t0, t0 * t0 * expl->f3);
        GAO_l_ModifierExplode_PosAt(&at1, &geo->vertices[i], &geo->normals[i],
                                    t1, t1 * t1 * expl->f3);
        geo->vertices[i].x = geo->vertices[i].x + (at1.x - at0.x) * scale;
        geo->vertices[i].y = geo->vertices[i].y + (at1.y - at0.y) * scale;
        geo->vertices[i].z = geo->vertices[i].z + (at1.z - at0.z) * scale;
    }

    expl->f1 = t1;
}

void GAO_ModifierExplode_Unapply(struct sModifier* mdf, void* unk1)
{
    tExplodeMdfStruct* expl;

    expl = (tExplodeMdfStruct*)mdf->mdfPrivateDataPtr;
    if (expl)
    {
        ((tGameObjectVisualObject*)
         (gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse =
            (struct sGroGeneric*)expl->gaogeogro;
    }
}

int GAO_ModifierExplode_Load(struct sModifier* mdf, char* data)
{
    char* start;

    start = data;
    mdf->modifierFlags |= 0x1000000u;
    LOA_ReadLong(&data, 0, 2);
    return data - start;
}

void GAO_ModifierExplode_Reinit(struct sModifier* mdf)
{
    tExplodeMdfStruct* expl;
    tGeoTriangleChunks* src;
    tGeoTriangleChunkElem* elem;
    Vec3* verts;
    int chunk;
    unsigned int t;

    expl = (tExplodeMdfStruct*)mdf->mdfPrivateDataPtr;
    expl->f1 = 0.0;
    verts = expl->geogroNew->vertices;
    for (chunk = 0; chunk < expl->triangleChunkNum; chunk++)
    {
        src = &expl->gaogeogro->triangleChunks[chunk];
        for (t = 0; t < src->triangleNum; t++)
        {
            elem = &src->triangleChunkElems[t];
            verts[0] = expl->gaogeogro->vertices[elem->v1];
            verts[1] = expl->gaogeogro->vertices[elem->v2];
            verts[2] = expl->gaogeogro->vertices[elem->v3];
            verts += 3;
        }
    }
}

void GAO_ModifierExplode_Destroy(struct sModifier* mdf)
{
}
