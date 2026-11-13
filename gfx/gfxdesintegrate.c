#include "../memory.h"
#include "../modifiers/stpmodifier.h"
#include "../graphicsobjects/particle.h"
#include "../time.h"
#include "gfxdesintegrate.h"

void GFX_Desintegrate_Init(tGfxDesintegrateData* desint)
{
    tGEOGRO* source;
    tGEOGRO* newGeo;
    tGeoTriangleChunks* dstChunk;
    tGeoTriangleChunkElem* elems;
    tPAGElemStruct* particle;
    tModifier* mdf;
    tStpMdfStruct* stp;
    Vec3* world;
    Vec3* srcVerts;
    Vec3* verts;
    Vec3* normals;
    Vec3 centre;
    Vec3 outward;
    Vec3 normal;
    double speed;
    int triangleNum;
    int baseVertex;
    int chunk;
    int t;
    unsigned int i;

    source = desint->geogro;
    desint->flags |= 1u;

    triangleNum = GEO_l_GetNumberOfTriangles(source);
    desint->triangleNum = triangleNum;
    desint->pointsNum = 3 * triangleNum;
    desint->triangleChunkCount = source->triangleChunkCount;

    desint->pagPtr = PAG_pst_Create();
    desint->pagPtr->triangleNum = desint->triangleNum;
    desint->pagPtr->triangleNum2 = desint->pagPtr->triangleNum;
    desint->pagPtr->pagElems = (tPAGElemStruct*)
        MEM_p_Alloc(sizeof(tPAGElemStruct) * desint->pagPtr->triangleNum);
    memset(desint->pagPtr->pagElems, 0,
           sizeof(tPAGElemStruct) * desint->pagPtr->triangleNum);
    desint->pagPtr->flags |= 0x2000u;
    desint->pagPtr->flags |= 0x4000u;
    desint->pagPtr->v1 = desint->v1;
    desint->pagPtr->f1 = desint->dword1C;
    if (desint->f1 != 10000.0f)
    {
        desint->flags |= 2u;
        desint->pagPtr->flags |= 0x400u;
        desint->pagPtr->i3 = desint->dword20;
        desint->pagPtr->ff = desint->f1;
    }

    newGeo = GEO_pst_Create(desint->pointsNum, 0, desint->triangleChunkCount, 0);
    desint->geogro = newGeo;
    newGeo->uvNum = source->uvNum;
    newGeo->UVs = source->UVs;

    world = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * source->vertexNum);
    srcVerts = source->vertices;
    mdf = MDF_pst_GetByType(desint->gao, 21);
    if (mdf)
    {
        stp = (tStpMdfStruct*)mdf->mdfPrivateDataPtr;
        if (stp->hasStp)
            srcVerts = stp->verticesStored;
    }
    for (i = 0; i < source->vertexNum; i++)
        MATH_TransformVertex(&world[i], &desint->mtx, &srcVerts[i]);

    verts = newGeo->vertices;
    normals = newGeo->normals;
    particle = desint->pagPtr->pagElems;
    baseVertex = 0;
    for (chunk = 0; chunk < desint->triangleChunkCount; chunk++)
    {
        if (!source->triangleChunks[chunk].triangleNum)
        {
            memset(&desint->geogro->triangleChunks[chunk], 0, sizeof(tGeoTriangleChunks));
            continue;
        }

        dstChunk = &desint->geogro->triangleChunks[chunk];
        dstChunk->triangleNum = source->triangleChunks[chunk].triangleNum;
        dstChunk->materialNum = source->triangleChunks[chunk].materialNum;
        dstChunk->MRMUVIndicesNum = 0;
        GEO_AllocElementContent(dstChunk);
        // The source is the first chunk's element array indexed by the chunk
        // number, so every chunk past the first copies from one element in
        // rather than from its own array. Left as it stands; see
        // docs/cleanup/gfx.md.
        memcpy(dstChunk->triangleChunkElems,
               &source->triangleChunks->triangleChunkElems[chunk],
               sizeof(tGeoTriangleChunkElem) * dstChunk->triangleNum);

        elems = dstChunk->triangleChunkElems;
        for (t = 0; t < (int)dstChunk->triangleNum; t++)
        {
            verts[0] = world[elems->v1];
            verts[1] = world[elems->v2];
            verts[2] = world[elems->v3];

            centre.x = (float)((verts[0].x + verts[1].x + verts[2].x) * 0.33333334);
            centre.y = (float)((verts[0].y + verts[1].y + verts[2].y) * 0.33333334);
            centre.z = (float)((verts[0].z + verts[1].z + verts[2].z) * 0.33333334);

            if ((desint->flags & 2) != 0)
                particle->particlePos = centre;
            else
                MATH_SetNullVector(&particle->particlePos);

            MATH_SubstractVec(&outward, &centre, &desint->mtx.Pos);
            MATH_NormalizeVectorWithNullVectorCheck(&normal, &outward);
            normals[0] = normal;
            normals[1] = normal;
            normals[2] = normal;

            speed = (double)rand() * 0.000030518509
                  * (desint->rand1Start - desint->rand1End) + desint->rand1End;
            MATH_VecMultScalar(&particle->v1, &normal, (float)speed);
            particle->ff2 = (float)((double)rand() * 0.000030518509
                            * (desint->rand2Start - desint->rand2End) + desint->rand2End);

            elems->v1 = baseVertex;
            elems->v2 = baseVertex + 1;
            elems->v3 = baseVertex + 2;
            baseVertex += 3;

            verts += 3;
            normals += 3;
            elems++;
            particle++;
        }
    }

    MEM_Free(world);
}

struct sGfxDesintegrateData* GFX_Desintegrate_Create(struct sGameObjectMain* gao)
{
    tGEOGRO* geo;
    tGfxDesintegrateData* desint;

    geo = (tGEOGRO*)OBJ_p_GetGro(gao);
    if (!geo || geo->intf->id != 1)
        return 0;

    desint = (tGfxDesintegrateData*)MEM_p_Alloc(sizeof(tGfxDesintegrateData));
    desint->flags = 0;
    MATH_CopyMatrix(&desint->mtx, OBJ_pst_GetAbsoluteMatrix(gao));
    desint->gao = gao;
    desint->geogro = geo;
    desint->geomat = OBJ_p_GetGrm(gao);
    desint->timeElapsed = 0.0;
    desint->v1.x = 0.0;
    desint->v1.y = 0.0;
    desint->v1.z = -0.5f;
    desint->dword1C = 1.0;
    desint->rand1Start = 2.0f;
    desint->rand1End = 1.0;
    desint->rand2Start = 5.0f;
    desint->f1 = 10000.0f;
    return desint;
}

void GFX_Desintegrate_Destroy(struct sGfxDesintegrateData* desint)
{
    if (!desint)
        return;

    desint->geogro->uvNum = 0;
    desint->geogro->UVs = 0;
    GEO_Free((tGroGeneric*)(desint->geogro));
    PAG_Free((tGroGeneric*)(desint->pagPtr));
}

int GFX_i_Desintegrate_Render(struct sGfxDesintegrateData* desint)
{
    tGameObjectVisualObject* visual;
    tPAGElemStruct* particles;
    tGEOGRO* savedGeo;
    Vec3* verts;
    double elapsed;
    int clearParticlePos;
    int count;
    int i;

    if (!desint)
        return 0;

    elapsed = TIM_gf_dt + desint->timeElapsed;
    desint->timeElapsed = (float)elapsed;
    if (elapsed > desint->rand2Start)
        return 0;

    if ((desint->flags & 1) == 0)
        GFX_Desintegrate_Init(desint);

    count = desint->pagPtr->triangleNum2;
    if ((desint->flags & 2) != 0 && count > 0)
    {
        particles = desint->pagPtr->pagElems;
        verts = desint->geogro->vertices;
        for (i = 0; i < count; i++)
        {
            MATH_SubstractVec(&verts[0], &verts[0], &particles[i].particlePos);
            MATH_SubstractVec(&verts[1], &verts[1], &particles[i].particlePos);
            MATH_SubstractVec(&verts[2], &verts[2], &particles[i].particlePos);
            verts += 3;
        }
    }

    PAG_Update(0, desint->pagPtr, TIM_gf_dt);

    // the pass that reads the particles back resets them only when the
    // positions were never seeded, which is the flag-2-clear case
    clearParticlePos = ((desint->flags & 2) == 0);
    particles = desint->pagPtr->pagElems;
    verts = desint->geogro->vertices;
    for (i = 0; i < count; i++)
    {
        if (particles[i].ff2 >= 0.0)
        {
            MATH_VecAddVec(&verts[0], &verts[0], &particles[i].particlePos);
            MATH_VecAddVec(&verts[1], &verts[1], &particles[i].particlePos);
            MATH_VecAddVec(&verts[2], &verts[2], &particles[i].particlePos);
        }
        else
        {
            MATH_SetNullVector(&verts[0]);
            MATH_SetNullVector(&verts[1]);
            MATH_SetNullVector(&verts[2]);
        }
        if (clearParticlePos)
            MATH_SetNullVector(&particles[i].particlePos);
        verts += 3;
    }

    GFX_NeedGeom(4u, 4u, 4u, 0);

    visual = (tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject);
    savedGeo = (tGEOGRO*)(visual->GROGEOOrSomethingElse);
    visual->GROGEOOrSomethingElse = (tGroGeneric*)(desint->geogro);
    visual->GROMAT = (tGroGeneric*)(desint->geomat);
    visual->objDrawMask &= ~0x800u;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x800u;

    desint->geogro->intf->Render(GFX_gpst_GO);

    visual->objDrawMask |= 0x800u;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent |= 0x800u;
    visual->GROGEOOrSomethingElse = (tGroGeneric*)savedGeo;
    return 1;
}
