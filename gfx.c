#include "memory.h"
#include "time.h"
#include "gameobject.h"
#include "graphicsobjects/geo.h"
#include "gfx/gfxcarte.h"
#include "gfx/gfxflare.h"
#include "gfx/gfxdesintegrate.h"
#include "gfx/gfxlightning.h"
#include "gfx/gfxsmoke.h"
#include "gfx/gfxspark.h"
#include "gfx/gfxwater2.h"
#include "gfx/gfxprofiler.h"
#include "gfx/gfxexplode.h"
#include "gfx/gfxstr.h"
#include "gfx/gfxripple.h"
#include "gfx/gfxsun.h"
#include "gfx/gfxgao.h"
#include "gfx/gfxline.h"
#include "gfx/gfxtable.h"
#include "gfx/gfxspeed.h"
#include "gfx/gfxfade.h"
#include "gfx/gfxborderfade.h"
#include "gfx/gfxdiaphragm.h"
#include "gfx/gfxanotherflare.h"
#include "soft.h"
#include "gfx.h"

uint16_t GFX_sguw_Id = 1024;

int ga_Paramf_First[] = { 
    0, 0x3E8, 0x7D0, 0x0BB8, 0x0FA0, 
    0x0FA0, 0x1770, 0x1B58, 0x1F40, 0x2328, 
    0x2710,  0x2AF8, 0x2EE0, 0x32C8, 0x36B0, 
    0x3A98, 0x3E80, 0x4268, 0x4650, 0x4A38, 
    0x4E20, 0x5208, 0x55F0, 0 };

int ga_Paramf_Number[] = { 0, 1, 6, 7, 3, 0x15, 4, 0, 5, 0x0B, 4, 6, 0x14, 0x14, 0x0C, 4, 4, 1, 0x0E, 0, 5, 7, 2, 0 };

int ga_Parami_First[] = { 
    0, 0x44C, 0x834, 0x0C1C, 0x1004, //1100, 2100, ... a paraméterek száma, kezdõsorszám
    0x1004, 0x17D4, 0x1BBC, 0x1FA4, 0x238C, 
    0x2774, 0x2B5C, 0x2F44, 0x332C, 0x3714, 
    0x3AFC, 0x3EE4, 0x42CC, 0x46B4, 0x4A9C, 
    0x4E84, 0x526C, 0x5654, 0 };

int ga_Parami_Number[] = { 0, 1, 3, 0, 4, 0x14, 4, 0x0D, 1, 3, 4, 2, 0x14, 0x14, 3, 1, 7, 0, 5, 3, 3, 5, 5, 0 };

int ga_Paramv_First[] = { 
    0, 0x4B0, 0x898, 0x0C80, 0x1068, 
    0x1068, 0x1838, 0x1C20, 0x2008, 0x23F0, 
    0x27D8, 0x2BC0, 0x2FA8, 0x3390, 0x3778, 
    0x3B60, 0x3F48, 0x4330, 0x4718, 0x4B00, 
    0x4EE8, 0x52D0, 0x56B8, 0 };

int ga_Paramv_Number[] = { 0, 3, 3, 1, 2, 2, 2, 4, 1, 3, 2, 0, 0x14, 0x14, 0, 1, 7, 0, 1, 0, 3, 2, 3, 0 };



int16_t gaa_Paramf_Offset[] = {    0,    0,    0,    0,    0,    0,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x24,    0,    0,    0,    0,    0,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x24, 0x28, 0x2C, 0x30, 0x34, 0x38,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x18, 0x24, 0x28,    0,    0,    0,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x18, 0x24, 0x28, 0x4C, 0x50, 0x54, 0x58, 0x5C, 0x60, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7C, 0x80, 0x84, 0x88, 0x8C, 0x90,
                                0x18, 0x1C, 0x20, 0x24,    0,    0,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,    0,    0,    0,    0,    0,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,    4,    8, 0x0C, 0x10,    0,    0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x24, 0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0,0,0,0,0,0,0,0,0,0,
                                0x4018, 0x401C, 0x4020, 0x4024, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   8, 0x0C, 0x10, 0x14, 0x18, 0x20, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000, 0,0,0,0,0,
                                   0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000, 0,0,0,0,0,
                                0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0,0,0,0,0,0,0,0,0,
                                   4, 8, 0x0C, 0x10, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x54, 0x58, 0x5C, 0x60, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                  4, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x40, 0, 0, 0, 0, 0, 0, 0,
                                  0,    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x24, 0x28, 0x402C, 0x30, 0x34, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x24, 0x28,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

int16_t gaa_Parami_Offset[] = {
                                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0x3C, 0x40, 0x48, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0x201C, 0x48, 0x4C, 0x50, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0x201C, 0x48, 0x94, 0x98, 0x9C, 0x0A0, 0x0A4, 0x0A8, 0x0AC, 0x0B0, 0x0B4, 0x0B8, 0x0BC, 0x0C0, 0x0C4, 0x0C8, 0x0CC, 0x0D0, 0x0D4, 0,
                                 0x4000, 0x28, 0x2C, 0x4000, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0x2000, 0x1002, 0x1003, 4, 8, 0x0C, 0x10, 0x14, 0x18, 0x1C,  0x20, 0x24, 0x28, 0,0,0,0,0,0,0,
                                   0x14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,
                                   0x50, 0x54, 0x58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 4, 0x4000, 0x402C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,
                                 0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000, 0,0,0,0,0,
                                 0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000, 0,0,0,0,0,
                                      0, 4,  8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0x4000, 0x4000, 0x4000, 0x68, 0x1067,  0x1064, 0x1065, 0,0,0,0,0,0,0,0,0,0,0,0,0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0x4000, 0x48, 0x4C,  0x50, 0x1054, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0x4000, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0x4000, 0x3C, 0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0x1034, 0x38, 0x3C, 0x1035, 0x1037, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0x2C, 0x30, 0x34, 0x38, 0x3C, 0x40, 0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

int16_t gaa_Paramv_Offset[] = {
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0x18, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0x18, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x2C,  0x4000, 0x4000, 0x4000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x14, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0x18, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x4000, 0x400C, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,
                                0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0x18, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                  0, 0x0C, 0x18, 0x24, 0x30, 0x3C, 0x48, 0,0,0,0,0,0,0,0,0,0,0,0,0,
                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0x18, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0, 0x0C, 0x18, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


void GFX_NeedGeomEx(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleChunkNum, unsigned int triangleNum, int useVertexColors)
{
    tGeoTriangleChunks* chunk;
    unsigned int oldChunkCount;

    if (!pointsNum || pointsNum > 5000)
        return;

    if (useVertexColors)
    {
        if (GFX_ul_MaxRLI < pointsNum)
        {
            if (GFX_pul_RLI)
                GFX_pul_RLI = (int*)MEM_p_Realloc(GFX_pul_RLI, 4 * pointsNum + 4);
            else
                GFX_pul_RLI = (int*)MEM_p_Alloc(4 * pointsNum + 4);
            GFX_ul_MaxRLI = pointsNum;
        }
        *GFX_pul_RLI = pointsNum;
    }

    if (!GFX_gpst_GO)
    {
        memset(GFX_aul_MaxTriangles, 0, sizeof(GFX_aul_MaxTriangles));
        GEO_b_IsInFix = 1;
        GFX_gpst_Geo = GEO_pst_Create(pointsNum, UVNum, triangleChunkNum, 0);
        GEO_b_IsInFix = 0;
        chunk = GFX_gpst_Geo->triangleChunks;
        chunk->triangleNum = triangleNum;
        chunk->MRMUVIndexElems = 0;
        chunk->triangleStripBlock = 0;
        chunk->MRMUVIndicesNum = 0;
        GEO_AllocElementContent(chunk);
        GFX_gpst_GO = OBJ_GameObject_Create(0x5000);
        ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = (tGroGeneric*)GFX_gpst_Geo;
        gpst_DX_stDD->currTargetGao = GFX_gpst_GO;
        GFX_ul_MaxElements = triangleChunkNum;
        GFX_ul_MaxElemSprites = 0;
        GFX_ul_MaxPoints = pointsNum;
        GFX_ul_MaxUVs = UVNum;
        GFX_aul_MaxTriangles[0] = triangleNum;
        if (useVertexColors)
            GFX_gpst_Geo->vertexColors = GFX_pul_RLI;
        return;
    }

    GFX_gpst_Geo->spritesNum = 0;
    GFX_gpst_Geo->vertexColors = useVertexColors ? GFX_pul_RLI : NULL;
    if (pointsNum > GFX_ul_MaxPoints)
    {
        MEM_FreeAlign(GFX_gpst_Geo->vertices);
        GFX_gpst_Geo->vertices = (Vec3*)MEM_p_AllocAligned(sizeof(Vec3) * pointsNum, 16);
        MEM_FreeAlign(GFX_gpst_Geo->normals);
        GFX_gpst_Geo->normals = (Vec3*)MEM_p_AllocAligned(sizeof(Vec3) * pointsNum, 16);
        GFX_ul_MaxPoints = pointsNum;
    }
    GFX_gpst_Geo->vertexNum = pointsNum;
    if (UVNum > GFX_ul_MaxUVs)
    {
        MEM_FreeAlign(GFX_gpst_Geo->UVs);
        GFX_gpst_Geo->UVs = (tGeoUVElem*)MEM_p_AllocAligned(sizeof(tGeoUVElem) * UVNum, 8);
        GFX_ul_MaxUVs = UVNum;
    }
    GFX_gpst_Geo->uvNum = UVNum;
    if (triangleNum > GFX_aul_MaxTriangles[0])
    {
        MEM_FreeAlign(GFX_gpst_Geo->triangleChunks->triangleChunkElems);
        GFX_gpst_Geo->triangleChunks->triangleChunkElems = (tGeoTriangleChunkElem*)MEM_p_AllocAligned(
            sizeof(tGeoTriangleChunkElem) * triangleNum,
            16);
        GFX_aul_MaxTriangles[0] = triangleNum;
    }
    GFX_gpst_Geo->triangleChunks->triangleNum = triangleNum;
    if (triangleChunkNum > GFX_ul_MaxElements)
    {
        oldChunkCount = GFX_ul_MaxElements;
        if (oldChunkCount)
        {
            GFX_gpst_Geo->triangleChunks = (tGeoTriangleChunks*)MEM_p_Realloc(GFX_gpst_Geo->triangleChunks, sizeof(tGeoTriangleChunks) * triangleChunkNum);
            memset(&GFX_gpst_Geo->triangleChunks[oldChunkCount], 0, sizeof(tGeoTriangleChunks) * (triangleChunkNum - oldChunkCount));
        }
        else
        {
            GFX_gpst_Geo->triangleChunks = (tGeoTriangleChunks*)MEM_p_Alloc(sizeof(tGeoTriangleChunks) * triangleChunkNum);
        }
        GFX_ul_MaxElements = triangleChunkNum;
    }
    GFX_gpst_Geo->triangleChunkCount = triangleChunkNum;
}


void GFX_NeedGeomExElem(unsigned int triangleChunkIdx, unsigned int triangleNum)
{
    tGeoTriangleChunks* chunk;
    unsigned int capacity;

    if (triangleChunkIdx >= GFX_gpst_Geo->triangleChunkCount)
        return;

    chunk = &GFX_gpst_Geo->triangleChunks[triangleChunkIdx];
    capacity = GFX_aul_MaxTriangles[triangleChunkIdx];
    if (triangleNum > capacity)
    {
        if (capacity)
        {
            chunk->triangleChunkElems = (tGeoTriangleChunkElem*)MEM_p_ReallocAligned(chunk->triangleChunkElems, sizeof(tGeoTriangleChunkElem) * triangleNum, 16);
        }
        else
        {
            chunk->triangleNum = triangleNum;
            chunk->MRMUVIndexElems = 0;
            chunk->triangleStripBlock = 0;
            chunk->MRMUVIndicesNum = 0;
            GEO_AllocElementContent(chunk);
        }
        GFX_aul_MaxTriangles[triangleChunkIdx] = triangleNum;
    }
    chunk->triangleNum = triangleNum;
}

void GFX_End()
{
    if (GFX_gpst_GO)
    {
        ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROMAT = 0;
        ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = 0;
        OBJ_GameObject_Remove(GFX_gpst_GO, TRUE);
        GFX_gpst_GO = NULL;
    }
    if (GFX_gpst_Geo)
    {
        GFX_gpst_Geo->vertexColors = 0;
        GFX_gpst_Geo->triangleChunkCount = GFX_ul_MaxElements;
        GFX_gpst_Geo->spritesNum = GFX_ul_MaxElemSprites;
        GEO_Free((tGroGeneric*)GFX_gpst_Geo);
        GFX_gpst_Geo = NULL;
    }
    if (GFX_pul_RLI)
    {
        MEM_Free(GFX_pul_RLI);
        GFX_pul_RLI = NULL;
        GFX_ul_MaxRLI = 0;
    }
}


void GFX_SetCurrent(struct sGfxElem* list, int16_t id)
{
    tGfxElem* elem;

    if (!GFX_gpst_Current || GFX_gpst_Current->id != id)
    {
        elem = list;
        while (elem && elem->id != id)
            elem = elem->prevGfxElem;
        GFX_gpst_Current = elem;
    }
}


void GFX_FlagSet(tGfxElem* list, int16_t id, char bit, int set)
{
    int mask;
    uint8_t flags;

    GFX_SetCurrent(list, id);
    if (GFX_gpst_Current)
    {
        mask = 1 << bit;
        flags = GFX_gpst_Current->gfxFlags;
        if (set)
            GFX_gpst_Current->gfxFlags = mask | flags;
        else
            GFX_gpst_Current->gfxFlags = ~(uint8_t)mask & flags;
    }
}


void GFX_SetMaterial(tGfxElem* list, int16_t id, struct sGEOMAT* material)
{
    GFX_SetCurrent(list, id);
    if (GFX_gpst_Current)
        GFX_gpst_Current->material = material;
}


void GFX_NeedGeomNbElemSprites(unsigned int spriteChunkNum)
{
    unsigned int capacity;

    capacity = GFX_ul_MaxElemSprites;
    GFX_gpst_Geo->spritesNum = spriteChunkNum;
    if (spriteChunkNum >= capacity)
    {
        if (capacity)
            GFX_gpst_Geo->spritesPtr = (tGeoSprites*)MEM_p_Realloc(GFX_gpst_Geo->spritesPtr, sizeof(tGeoSprites) * spriteChunkNum);
        else
            GFX_gpst_Geo->spritesPtr = (tGeoSprites*)MEM_p_Alloc(sizeof(tGeoSprites) * spriteChunkNum);
        GFX_ul_MaxElemSprites = spriteChunkNum;
    }
}


void GFX_NeedGeomElemNbSprites(unsigned int spriteChunkIdx, unsigned int spriteNum)
{
    unsigned int capacity;
    tGeoSprites* chunk;

    if (spriteChunkIdx >= GFX_gpst_Geo->spritesNum)
        return;

    // preserved: the capacity tested is the triangle table's, the one updated is the sprite table's
    capacity = GFX_aul_MaxTriangles[spriteChunkIdx];
    chunk = &GFX_gpst_Geo->spritesPtr[spriteChunkIdx];
    if (spriteNum > capacity)
    {
        if (capacity)
            chunk->spriteElems = (tGeoSpriteElem*)MEM_p_Realloc(chunk->spriteElems, sizeof(tGeoSpriteElem) * spriteNum);
        else
            chunk->spriteElems = (tGeoSpriteElem*)MEM_p_Alloc(sizeof(tGeoSpriteElem) * spriteNum);
        GFX_aul_MaxSprites[spriteChunkIdx] = spriteNum;
    }
    chunk->num = spriteNum;
}


int GFX_i_Request(int id)
{
    int result;

    if (id < 1000 || id >= 2000)
        result = 0;
    else
        result = 0; // return_0_sub_492830(); GFX_i_Line_Request();
    return result;
}

void GFX_NeedGeom(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleNum, int useVertexColors)
{
    GFX_NeedGeomEx(pointsNum, UVNum, 1, triangleNum, useVertexColors);
}

void GFX_SetQuadIndices(void)
{
    tGeoTriangleChunkElem* tris;

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    tris[0].v1 = 0;
    tris[0].v2 = 1;
    tris[0].v3 = 2;
    tris[0].uv1 = 0;
    tris[0].uv2 = 1;
    tris[0].uv3 = 2;
    tris[1].v1 = 2;
    tris[1].v2 = 3;
    tris[1].v3 = 0;
    tris[1].uv1 = 2;
    tris[1].uv2 = 3;
    tris[1].uv3 = 0;
}

void GFX_SetQuadUVs(void)
{
    tGeoUVElem* uvs;

    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;
}

void GFX_RenderGeom(unsigned int maskBitsToClear)
{
    int savedMask;

    savedMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask & ~maskBitsToClear;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask;
}


struct sGfxElem* GFX_pst_FindById(struct sGfxElem** list, int16_t gfxId)
{
    tGfxElem* elem;

    for (elem = *list; elem; elem = elem->prevGfxElem)
    {
        if (elem->id == gfxId)
            return elem;
    }
    return 0;
}

void GFX_SetRenderState()
{
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = gpst_DX_stDD->drawMaskAllowedStuff & 0xFFFEFFFF;
    gpst_DX_stDD->matrixStack.currElemNum = 0;
    gpst_DX_stDD->matrixStack.actMatrixElem = gpst_DX_stDD->matrixStack.elems;
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    gpst_DX_stDD->currTargetGao = GFX_gpst_GO;
    gpst_DX_stDD->extraMaterialToGeo = 0;
}


struct sGfxElem* GFX_Attached(struct sGfxElem* list)
{
    tGfxElem* elem;

    // preserved: the walk never stops early, so the value returned is always NULL
    for (elem = list; elem; elem = elem->prevGfxElem)
    {
        if (elem->type == 16)                   // carte
            ((tGfxCarteData*)(elem->privateData))->currentPieceStatus = -1;
    }
    return elem;
}


void GFX_FreeOne(tGfxElem* elem)
{
    switch (elem->type)
    {
    case 2:
        GFX_Lightning_Destroy((tGfxLightningData*)(elem->privateData));
        break;
    case 3:
        GFX_Desintegrate_Destroy((tGfxDesintegrateData*)(elem->privateData));
        break;
    case 6:
        GFX_Smoke_Destroy((tGfxSmokeData*)(elem->privateData));
        break;
    case 9:
        GFX_Spark_Destroy((tGfxSparkData*)(elem->privateData));
        break;
    case 10:
        GFX_Water2_Destroy((tGfxWater2Data*)(elem->privateData));
        break;
    case 12:
        GFX_Profiler_Destroy((tGfxProfilerData*)(elem->privateData));
        break;
    case 13:
        GFX_Explode_Destroy((tGfxExplodeData*)(elem->privateData));
        MEM_FreeAlign(elem->privateData);
        break;
    case 16:
        GFX_Carte_Destroy((tGfxCarteData*)(elem->privateData), elem->material);
        break;
    case 17:
        GFX_Gao_Destroy((tGfxGaoData*)(elem->privateData));
        break;
    case 18:
        GFX_Sun_Destroy(&(((tGfxSunData*)(elem->privateData))->d3dStuff));
        break;
    case 19:
        GFX_Str_Destroy((tGfxStrData*)(elem->privateData));
        break;
    case 20:
        GFX_Ripple_Destroy((tGfxRippleData*)(elem->privateData));
        break;
    }
    if (elem->type != 13)
        MEM_Free(elem->privateData);
    MEM_Free(elem);
}


void GFX_Del(tGfxElem** list, int16_t id)
{
    tGfxElem* elem;
    tGfxElem* prev;

    elem = *list;
    prev = 0;
    if (elem)
    {
        while (elem->id != id)
        {
            prev = elem;
            elem = elem->prevGfxElem;
            if (!elem)
            {
                GFX_gpst_Current = 0;
                return;
            }
        }
        if (prev)
            prev->prevGfxElem = elem->prevGfxElem;
        else
            *list = elem->prevGfxElem;
        GFX_FreeOne(elem);
    }
    GFX_gpst_Current = 0;
}


void GFX_Test(tGfxElem* list)
{
    tGfxElem* elem;
    uint16_t savedMatrixElem;
    char flags;

    elem = list;
    if (list)
    {
        savedMatrixElem = gpst_DX_stDD->matrixStack.currElemNum;
        gpst_DX_stDD->matrixStack.currElemNum = 0;
        gpst_DX_stDD->matrixStack.actMatrixElem = gpst_DX_stDD->matrixStack.elems;
        gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
        GFX_gpst_Current = list;
        do
        {
            flags = elem->gfxFlags;
            if ((flags & 1) != 0 && flags >= 0 && elem->type >= 4 && elem->type <= 5)
            {
                GFX_Flare_Test((tGfxFlareData*)(elem->privateData));
                elem = GFX_gpst_Current;
            }
            elem = elem->prevGfxElem;
            GFX_gpst_Current = elem;
        } while (elem);
        gpst_DX_stDD->matrixStack.currElemNum = savedMatrixElem;
        gpst_DX_stDD->matrixStack.actMatrixElem = &gpst_DX_stDD->matrixStack.elems[savedMatrixElem];
        gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    }
}


int GFX_i_SetDeath(struct sGfxElem** list, struct sGfxElem* elem)
{
    tGfxFlareData* flare;
    int flareFlags;

    if (elem)
    {
        if (elem->type == 4)                      // flare
        {
            flare = (tGfxFlareData*)(elem->privateData);
            flareFlags = flare->basic.flags;
            if ((flareFlags & 0x20) != 0)
            {
                flare->basic.flags = flareFlags | 0x4000000;
                ((tGfxFlareData*)(elem->privateData))->basic.i15 = -1;
                return 1;
            }
        }
        else if (elem->type == 21)                // anotherflare
        {
            ((tGfxAnotherFlareData*)(elem->privateData))->flags |= 1u;
            elem->lifeTime = ((tGfxAnotherFlareData*)(elem->privateData))->totalTime + 10.0f;
            return 1;
        }
        GFX_gpst_Current = elem;
        GFX_Del(list, elem->id);
    }
    return 0;
}


void GFX_Render(tGfxElem** list, int zlistLock)
{
    tGfxElem* elem;
    tGfxElem* next;
    profilerRasterStructElem* raster;
    int stateZList;
    int stateSorted;
    unsigned int stateDirect;
    int rendered;
    char flags;
    uint8_t renderFlags;
    double remaining;
    int16_t inside;
    int savedDrawMask;
    int savedFlags;
    uint16_t savedMatrixElem;

    elem = *list;
    if (!elem)
        return;
    QueryPerformanceCounter(&PerfCounterStruct[7]);
    raster = &gpst_DX_stDD->rasters[45];
    if (!raster->insideCounter)
        raster->startTick = TIM_GetTicksAsInt32();
    ++raster->insideCounter;
    GFX_ppst_CurrentList = list;
    savedFlags = gpst_DX_stDD->flags;
    savedDrawMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    savedMatrixElem = gpst_DX_stDD->matrixStack.currElemNum;
    GFX_SetRenderState();
    if ((uint8_t)zlistLock)
    {
        SOFT_ZList_Lock();
        stateZList = gpst_DX_stDD->flags | 4;
        stateSorted = stateZList;
        stateDirect = gpst_DX_stDD->flags & ~4u;
    }
    else
    {
        gpst_DX_stDD->flags |= 8u;
        stateZList = gpst_DX_stDD->flags | 4;
        stateSorted = gpst_DX_stDD->flags | 0x200;
        stateDirect = gpst_DX_stDD->flags & ~0x200u;
    }
    for (elem = *list; elem; elem = next)
    {
        ++gpst_DX_stDD->rasters[13].unk2;
        flags = elem->gfxFlags;
        next = elem->prevGfxElem;
        if (flags < 0 && (flags & 8) != 0)
        {
            GFX_gpst_Current = elem;
            GFX_Del(list, elem->id);
            continue;
        }
        remaining = elem->lifeTime - TIM_gf_dt;
        elem->lifeTime = (float)remaining;
        if (remaining <= 0.0 && !GFX_i_SetDeath(list, elem))
            continue;
        flags = elem->gfxFlags;
        if ((flags & 1) == 0 || flags < 0)
            continue;
        if (GFX_gpst_GO)
            ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROMAT = (tGroGeneric*)(elem->material);
        renderFlags = elem->gfxFlags;
        if ((renderFlags & 4) == 0)
            gpst_DX_stDD->flags = stateDirect;
        else if ((renderFlags & 2) != 0)
            gpst_DX_stDD->flags = stateZList;
        else
            gpst_DX_stDD->flags = stateSorted;
        GFX_gpst_Current = elem;
        rendered = 1;
        switch (elem->type)
        {
        case 1:
            GFX_Line_Render((tGfxLineData*)elem->privateData);
            break;
        case 2:
            rendered = GFX_i_Lightning_Render((tGfxLightningData*)elem->privateData);
            break;
        case 3:
            rendered = GFX_i_Desintegrate_Render((tGfxDesintegrateData*)elem->privateData);
            break;
        case 4:
        case 5:
            GFX_Flare_Render((tGfxFlareData*)elem->privateData);
            break;
        case 6:
            rendered = GFX_i_Smoke_Render((tGfxSmokeData*)elem->privateData);
            break;
        case 7:
            rendered = GFX_i_Table_Render((tGfxTableData*)elem->privateData);
            break;
        case 8:
            rendered = GFX_i_Speed_Render((tGfxSpeedData*)elem->privateData);
            break;
        case 9:
            rendered = GFX_i_Spark_Render((tGfxSparkData*)elem->privateData);
            break;
        case 10:
            rendered = GFX_i_Water2_Render((tGfxWater2Data*)elem->privateData);
            break;
        case 11:
            rendered = GFX_i_Fade_Render((tGfxFadeData*)elem->privateData);
            break;
        case 12:
            rendered = GFX_i_Profiler_Render((tGfxProfilerData*)elem->privateData);
            break;
        case 13:
            if (!GFX_i_Explode_Render((tGfxExplodeData*)elem->privateData, elem->material))
                elem->gfxFlags |= 0x80u;
            GFX_SetRenderStateForExplode();
            break;
        case 14:
            rendered = GFX_i_BorderFade_Render((tGfxBorderFadeData*)elem->privateData);
            break;
        case 15:
            rendered = GFX_i_Diaphragm_Render((tGfxDiaphragmData*)elem->privateData);
            break;
        case 16:
            rendered = GFX_i_Carte_Render((tGfxCarteData*)elem->privateData, elem->material, elem->aiGameObject);
            break;
        case 17:
            GFX_Gao_Render((tGfxGaoData*)elem->privateData);
            gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
            gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
            break;
        case 18:
            GFX_Sun_Render((tGfxSunData*)elem->privateData);
            break;
        case 19:
            rendered = GFX_i_Str_Render((tGfxStrData*)elem->privateData);
            break;
        case 20:
            rendered = GFX_i_Ripple_Render((tGfxRippleData*)elem->privateData);
            break;
        case 21:
            rendered = GFX_i_AnotherFlare_Render((tGfxAnotherFlareData*)elem->privateData);
            break;
        case 22:
            //nullsub_32();                   // GFX_Tetris_Render
            break;
        default:
            break;
        }
        if (!rendered)
            elem->gfxFlags |= 0x80u;
        ++gpst_DX_stDD->rasters[14].unk2;
    }
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedDrawMask;
    gpst_DX_stDD->flags = savedFlags;
    gpst_DX_stDD->matrixStack.currElemNum = savedMatrixElem;
    gpst_DX_stDD->matrixStack.actMatrixElem = &gpst_DX_stDD->matrixStack.elems[savedMatrixElem];
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    if ((uint8_t)zlistLock)
        SOFT_ZList_Unlock();
    PRO_QPC_RecordTimeDecRef(7);
    raster = &gpst_DX_stDD->rasters[45];
    inside = gpst_DX_stDD->rasters[45].insideCounter;
    if (inside)
    {
        gpst_DX_stDD->rasters[45].insideCounter = inside - 1;
        if (inside - 1 == 0)
            raster->sumTime += TIM_GetTicksAsInt32() - raster->startTick;
    }
}

void* GFX_Tetris_Dummy_Create() {
    return NULL;
}


int GFX_i_Add(tGfxElem** gfxList, char type, struct sGameObjectMain* aiGao)
{
    uint8_t initialFlags;
    void* privateData;
    tGfxElem* newGfxElem;
    int16_t id;

    if (!GFX_gpst_GO)
        GFX_NeedGeom(4u, 4u, 2u, 1);
    initialFlags = 0;
    switch (type)
    {
    case 1:
        privateData = GFX_Line_Create();
        break;
    case 2:
        privateData = GFX_Lightning_Create();
        break;
    case 3:
        privateData = GFX_Desintegrate_Create(aiGao);
        initialFlags = 8;
        break;
    case 4:
        privateData = GFX_Flare_Create(0);
        break;
    case 5:
        privateData = GFX_Flare_Create(1);
        break;
    case 6:
        privateData = GFX_Smoke_Create();
        initialFlags = 8;
        break;
    case 7:
        privateData = GFX_Table_Create();
        initialFlags = 8;
        break;
    case 8:
        privateData = GFX_Speed_Create();
        initialFlags = 8;
        break;
    case 9:
        privateData = GFX_Spark_Create();
        initialFlags = 8;
        break;
    case 10:
        privateData = GFX_Water2_Create();
        initialFlags = 8;
        break;
    case 11:
        privateData = GFX_Fade_Create();
        initialFlags = 8;
        break;
    case 12:                                    // watertrace??
        privateData = GFX_Profiler_Create();
        break;
    case 13:                                    // flames is
        privateData = GFX_Explode_Create();
        initialFlags = 8;
        break;
    case 14:
        privateData = GFX_BorderFade_Create();
        initialFlags = 8;
        break;
    case 15:
        privateData = GFX_Diaphragm_Create();
        initialFlags = 8;
        break;
    case 16:
        privateData = GFX_Carte_Create();
        break;
    case 17:
        privateData = GFX_Gao_Create(aiGao);
        initialFlags = 1;
        break;
    case 18:                                    // flash
        privateData = GFX_Sun_Create();
        initialFlags = 1;
        break;
    case 19:
        privateData = GFX_Str_Create(aiGao);
        initialFlags = 9;
        break;
    case 20:
        privateData = GFX_Ripple_Create(aiGao);
        initialFlags = 13;
        break;
    case 21:                                    // halo
        privateData = GFX_AnotherFlare_Create(aiGao);
        initialFlags = 13;
        break;
    case 22:
        privateData = GFX_Tetris_Dummy_Create();
        initialFlags = 1;
        break;
    default:
        return -1;
    }
    if (!privateData)
        return -1;
    if (GFX_sguw_Id < 1024u)
        GFX_sguw_Id = 1024;
    newGfxElem = (tGfxElem*)MEM_p_Alloc(sizeof(tGfxElem));
    newGfxElem->prevGfxElem = *gfxList;
    id = GFX_sguw_Id;
    newGfxElem->gfxFlags = initialFlags;
    newGfxElem->privateData = privateData;
    newGfxElem->aiGameObject = aiGao;
    newGfxElem->id = id;
    newGfxElem->type = type;
    newGfxElem->material = 0;
    newGfxElem->lifeTime = 3.402822e38f; // TODO FIXME? float.max?
    GFX_gpst_Current = newGfxElem;
    *gfxList = newGfxElem;
    GFX_sguw_Id = id + 1;
    return (uint16_t)newGfxElem->id;
}


int GFX_i_AddSorted(tGfxElem** gfxList, char type, uint16_t idx, struct sGameObjectMain* aiGao)
{
    tGfxElem* below;
    tGfxElem* above;
    uint16_t elemId;
    uint8_t initialFlags;
    void* privateData;
    tGfxElem* newGfxElem;

    if (!GFX_gpst_GO)
        GFX_NeedGeom(4u, 4u, 2u, 1);
    if (idx >= 1024u)
        return -1;
    above = 0;
    for (below = *gfxList; below; below = below->prevGfxElem)
    {
        elemId = below->id;
        if (elemId < idx)
            break;
        if (elemId == idx)
            return -1;
        above = below;
    }
    initialFlags = 0;
    switch (type)
    {
    case 1:
        privateData = GFX_Line_Create();
        break;
    case 2:
        privateData = GFX_Lightning_Create();
        break;
    case 3:
        privateData = GFX_Desintegrate_Create(aiGao);
        initialFlags = 8;
        break;
    case 4:
        privateData = GFX_Flare_Create(0);
        break;
    case 5:
        privateData = GFX_Flare_Create(1);
        break;
    case 6:
        privateData = GFX_Smoke_Create();
        initialFlags = 8;
        break;
    case 7:
        privateData = GFX_Table_Create();
        initialFlags = 8;
        break;
    case 8:
        privateData = GFX_Speed_Create();
        initialFlags = 8;
        break;
    case 9:
        privateData = GFX_Spark_Create();
        initialFlags = 8;
        break;
    case 10:
        privateData = GFX_Water2_Create();
        initialFlags = 8;
        break;
    case 11:
        privateData = GFX_Fade_Create();
        initialFlags = 8;
        break;
    case 12:
        privateData = GFX_Profiler_Create();
        break;
    case 13:
        privateData = GFX_Explode_Create();
        initialFlags = 8;
        break;
    case 14:
        privateData = GFX_BorderFade_Create();
        initialFlags = 8;
        break;
    case 15:
        privateData = GFX_Diaphragm_Create();
        initialFlags = 8;
        break;
    case 16:
        privateData = GFX_Carte_Create();
        break;
    case 17:
        privateData = GFX_Gao_Create(aiGao);
        initialFlags = 1;
        break;
    case 18:
        privateData = GFX_Sun_Create();
        initialFlags = 1;
        break;
    case 19:
        privateData = GFX_Str_Create(aiGao);
        initialFlags = 9;
        break;
    case 20:
        privateData = GFX_Ripple_Create(aiGao);
        initialFlags = 13;
        break;
    case 21:
        privateData = GFX_AnotherFlare_Create(aiGao);
        initialFlags = 13;
        break;
    case 22:
        privateData = GFX_Tetris_Dummy_Create();
        initialFlags = 1;
        break;
    default:
        return -1;
    }
    if (!privateData)
        return -1;
    newGfxElem = (tGfxElem*)MEM_p_Alloc(sizeof(tGfxElem));
    newGfxElem->type = type;
    newGfxElem->prevGfxElem = below;
    newGfxElem->gfxFlags = initialFlags;
    newGfxElem->privateData = privateData;
    newGfxElem->aiGameObject = aiGao;
    newGfxElem->material = 0;
    newGfxElem->id = idx;
    newGfxElem->lifeTime = 3.402822e38f; // TODO FIXME: float.max?
    GFX_gpst_Current = newGfxElem;
    if (above)
        above->prevGfxElem = newGfxElem;
    else
        *gfxList = newGfxElem;
    return idx;
}


Vec3* GFX_pst_Getv(tGfxElem* list, int16_t id, int param)
{
    int type;
    int first;
    int16_t offset;

    GFX_SetCurrent(list, id);
    if (!GFX_gpst_Current)
        return NULL;
    type = GFX_gpst_Current->type;
    first = ga_Paramv_First[type];
    if (param < first || param >= first + ga_Paramv_Number[type])
        return NULL;
    offset = gaa_Paramv_Offset[param + 20 * type - first];
    if ((offset & 0x4000) != 0)
        return NULL;
    return (Vec3*)((char*)GFX_gpst_Current->privateData + offset);
}


float GFX_f_Getf(tGfxElem* list, int16_t id, int param)
{
    int type;
    int first;
    int16_t offset;

    GFX_SetCurrent(list, id);
    if (!GFX_gpst_Current)
        return 0.0;
    type = GFX_gpst_Current->type;
    first = ga_Paramf_First[type];
    if (param < first || param >= first + ga_Paramf_Number[type])
        return 0.0;
    offset = gaa_Paramf_Offset[param + 21 * type - first];
    if ((offset & 0x4000) != 0)
        return 0.0;
    return *(float*)((char*)GFX_gpst_Current->privateData + offset);
}

// TOOD FIXME ezek biztos hogy signedek?

int GFX_i_Geti(tGfxElem* list, int16_t id, int param)
{
    int type;
    int first;
    int16_t offset;

    GFX_SetCurrent(list, id);
    if (!GFX_gpst_Current)
        return 0;
    type = GFX_gpst_Current->type;
    first = ga_Parami_First[type];
    if (param < first || param >= first + ga_Parami_Number[type])
        return 0;
    offset = gaa_Parami_Offset[param + 20 * type - first];
    if ((offset & 0x4000) != 0)
    {
        if (GFX_gpst_Current->type == 13)
            return GFX_i_Explode_Geti((tGfxExplodeData*)GFX_gpst_Current->privateData, param);
        return 0;
    }
    if ((offset & 0x1000) != 0)
        return *((int8_t*)GFX_gpst_Current->privateData + (offset & 0xFFF));
    if ((offset & 0x2000) != 0)
        return *(int16_t*)((char*)GFX_gpst_Current->privateData + (offset & 0xFFF));
    return *(int32_t*)((char*)GFX_gpst_Current->privateData + offset);
}


void GFX_Seti(tGfxElem* list, int16_t id, int param, int value)
{
    int type;
    int first;
    int16_t offset;
    char* data;

    GFX_SetCurrent(list, id);
    if (!GFX_gpst_Current)
        return;
    type = GFX_gpst_Current->type;
    first = ga_Parami_First[type];
    if (param < first || param >= first + ga_Parami_Number[type])
        return;
    offset = gaa_Parami_Offset[param + 20 * type - first];
    if ((offset & 0x4000) != 0)
    {
        switch (type)
        {
        case 6:
            GFX_Smoke_Seti((tGfxSmokeData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 10:
            GFX_Water2_Seti((tGfxWater2Data*)GFX_gpst_Current->privateData, param, offset & ~0x4000u, value);
            break;
        case 12:
            GFX_Profiler_Seti((tGfxProfilerData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 13:
            GFX_Explode_Seti((tGfxExplodeData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 16:
            GFX_Carte_Seti((tGfxCarteData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 18:
            GFX_Sun_Seti((tGfxSunData*)GFX_gpst_Current->privateData, param);
            break;
        case 19:
            GFX_Str_Seti((tGfxStrData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 20:
            GFX_Ripple_Seti((tGfxRippleData*)GFX_gpst_Current->privateData, param, value);
            break;
        }
    }
    else if ((offset & 0x1000) != 0)
    {
        *((int8_t*)GFX_gpst_Current->privateData + (offset & 0xFFF)) = value;
    }
    else
    {
        data = (char*)GFX_gpst_Current->privateData;
        if ((offset & 0x2000) != 0)
            *(int16_t*)&data[offset & 0xFFF] = value;
        else
            *(int32_t*)&data[offset] = value;
    }
}


void GFX_Setf(tGfxElem* list, int id, int param, float value)
{
    int type;
    int first;
    int16_t offset;

    GFX_SetCurrent(list, id);
    if (!GFX_gpst_Current)
        return;
    type = GFX_gpst_Current->type;
    first = ga_Paramf_First[type];
    if (param < first || param >= first + ga_Paramf_Number[type])
        return;
    offset = gaa_Paramf_Offset[param + 21 * type - first];
    if ((offset & 0x4000) != 0)
    {
        switch (type)
        {
        case 10:
            GFX_Water2_Setf((tGfxWater2Data*)GFX_gpst_Current->privateData, param, offset & ~0x4000, value);
            break;
        case 12:
            GFX_Profiler_Setf((tGfxProfilerData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 13:
            GFX_Explode_Setf((tGfxExplodeData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 20:
            GFX_Ripple_Setf((tGfxRippleData*)GFX_gpst_Current->privateData, param, value);
            break;
        }
    }
    else
    {
        *(float*)((char*)GFX_gpst_Current->privateData + offset) = value;
    }
}


void GFX_Setv(tGfxElem* list, int16_t id, int param, Vec3* value)
{
    int type;
    int first;
    int16_t offset;

    GFX_SetCurrent(list, id);
    if (!GFX_gpst_Current)
        return;
    type = GFX_gpst_Current->type;
    first = ga_Paramv_First[type];
    if (param < first || param >= first + ga_Paramv_Number[type])
        return;
    offset = gaa_Paramv_Offset[param + 20 * type - first];
    if ((offset & 0x4000) != 0)
    {
        switch (type)
        {
        case 7:
            GFX_Table_Setv((tGfxTableData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 10:
            GFX_Water2_Setv((tGfxWater2Data*)GFX_gpst_Current->privateData, param, offset & ~0x4000u, value);
            break;
        case 12:
            GFX_Profiler_Setv((tGfxProfilerData*)GFX_gpst_Current->privateData, param, value);
            break;
        case 13:
            GFX_Explode_Setv((tGfxExplodeData*)GFX_gpst_Current->privateData, param, value);
            break;
        }
    }
    else
    {
        *(Vec3*)((char*)GFX_gpst_Current->privateData + offset) = *value;
    }
}


void GFX_CreateExplosion(float size, float duration, Vec3* pos, struct sGameObjectMain* aiGao, struct sGEOMAT* material)
{
    tGfxElem** list;
    int layer1;
    int layer2;
    int layer3;
    int layer4;
    double growD;
    double gravityD;
    float invDuration;
    float grow;
    float gravityBase;
    float friction;
    float frictionXY;
    float frictionZ;
    float phaseTime;
    float phaseTime2;
    float initialSize;
    float growZero;
    float normSpeed02;
    float normSpeed05;
    float normSpeed06;
    Vec3 vec;

    invDuration = (float)(1.0 / duration);
    pos->z = (float)(pos->z + 0.1);
    list = &WOR_gpst_CurrentWorld->gfxCurrentList;
    layer1 = GFX_i_Add(&WOR_gpst_CurrentWorld->gfxCurrentList, 13, aiGao);
    GFX_SetMaterial(*list, layer1, material);
    GFX_Seti(*list, layer1, 13100, 10);
    GFX_Seti(*list, layer1, 13106, 10);
    GFX_Seti(*list, layer1, 13101, 1);
    GFX_Seti(*list, layer1, 13103, 0);
    GFX_Seti(*list, layer1, 13104, 0xFFFFFF);
    GFX_Seti(*list, layer1, 13105, 0);
    growD = invDuration * size;
    grow = (float)growD;
    GFX_Setf(*list, layer1, 13000, (float)(growD * 0.80000001));
    GFX_Setf(*list, layer1, 13001, (float)(grow * 0.89999998));
    friction = (float)(invDuration * 0.1);
    GFX_Setf(*list, layer1, 13002, friction);
    phaseTime = (float)(duration * 0.029999999);
    GFX_Setf(*list, layer1, 13003, phaseTime);
    GFX_Setf(*list, layer1, 13004, phaseTime);
    initialSize = (float)(size * 0.039999999);
    GFX_Setf(*list, layer1, 13005, initialSize);
    GFX_Setf(*list, layer1, 13006, initialSize);
    gravityD = invDuration * invDuration;
    gravityBase = (float)gravityD;
    GFX_Setf(*list, layer1, 13007, (float)(gravityD * 0.15000001));
    GFX_Setf(*list, layer1, 13008, (float)(duration * 0.000049999999));
    normSpeed05 = (float)(grow * 0.5);
    GFX_Setf(*list, layer1, 13009, normSpeed05);
    normSpeed06 = (float)(grow * 0.60000002);
    GFX_Setf(*list, layer1, 13010, normSpeed06);
    GFX_Setf(*list, layer1, 13011, (float)(pos->z - 0.1));
    GFX_Setv(*list, layer1, 13200, pos);
    vec.x = 1.0;
    vec.y = 1.0;
    vec.z = 1.0;
    GFX_Setv(*list, layer1, 13201, &vec);
    vec.x = -1.0;
    vec.y = -1.0;
    vec.z = 0.0;
    GFX_Setv(*list, layer1, 13202, &vec);
    vec.z = (float)(invDuration * 0.5);
    vec.x = friction;
    vec.y = friction;
    GFX_Setv(*list, layer1, 13203, &vec);
    GFX_FlagSet(*list, layer1, 0, 1);
    GFX_FlagSet(*list, layer1, 2, 1);

    layer2 = GFX_i_Add(list, 13, aiGao);
    GFX_SetMaterial(*list, layer2, material);
    GFX_Seti(*list, layer2, 13100, 80);
    GFX_Seti(*list, layer2, 13106, 80);
    GFX_Seti(*list, layer2, 13101, 2);
    GFX_Seti(*list, layer2, 13103, -1);
    GFX_Seti(*list, layer2, 13104, -1);
    GFX_Seti(*list, layer2, 13105, 0);
    growZero = (float)(grow * 0.0);
    GFX_Setf(*list, layer2, 13000, growZero);
    GFX_Setf(*list, layer2, 13001, growZero);
    GFX_Setf(*list, layer2, 13002, friction);
    phaseTime2 = duration + duration;
    GFX_Setf(*list, layer2, 13003, phaseTime2);
    GFX_Setf(*list, layer2, 13004, phaseTime2);
    GFX_Setf(*list, layer2, 13005, (float)(size * 0.0020000001));
    GFX_Setf(*list, layer2, 13006, (float)(size * 0.0049999999));
    GFX_Setf(*list, layer2, 13007, (float)(gravityBase * -2.5));
    GFX_Setf(*list, layer2, 13008, (float)(duration * 0.001));
    normSpeed02 = (float)(grow * 0.2);
    GFX_Setf(*list, layer2, 13009, normSpeed02);
    GFX_Setf(*list, layer2, 13010, normSpeed05);
    GFX_Setf(*list, layer2, 13011, (float)(pos->z - 0.5));
    GFX_Setv(*list, layer2, 13200, pos);
    vec.x = -1.0;
    vec.y = -1.0;
    vec.z = 0.0;
    GFX_Setv(*list, layer2, 13201, &vec);
    vec.x = 1.0;
    vec.y = 1.0;
    vec.z = 2.0;
    GFX_Setv(*list, layer2, 13202, &vec);
    vec.x = (float)(invDuration * 0.0099999998);
    vec.y = vec.x;
    vec.z = (float)(invDuration * 0.000099999997);
    GFX_Setv(*list, layer2, 13203, &vec);
    GFX_FlagSet(*list, layer2, 0, 1);
    GFX_FlagSet(*list, layer2, 2, 1);

    layer3 = GFX_i_Add(list, 13, aiGao);
    GFX_SetMaterial(*list, layer3, material);
    GFX_Seti(*list, layer3, 13100, 30);
    GFX_Seti(*list, layer3, 13106, 30);
    GFX_Seti(*list, layer3, 13101, 1);
    GFX_Seti(*list, layer3, 13103, 0);
    GFX_Seti(*list, layer3, 13104, 0xFF000000);
    GFX_Seti(*list, layer3, 13105, 0);
    GFX_Setf(*list, layer3, 13000, (float)(grow * 0.1));
    GFX_Setf(*list, layer3, 13001, (float)(grow * 0.13));
    GFX_Setf(*list, layer3, 13002, (float)(invDuration * 0.02));
    GFX_Setf(*list, layer3, 13003, (float)(duration * 0.30000001));
    GFX_Setf(*list, layer3, 13004, (float)(duration * 1.4));
    GFX_Setf(*list, layer3, 13005, (float)(size * 0.050000001));
    GFX_Setf(*list, layer3, 13006, (float)(size * 0.059999999));
    GFX_Setf(*list, layer3, 13007, (float)(gravityBase * 0.0049999999));
    GFX_Setf(*list, layer3, 13008, (float)(duration * 0.0049999999));
    GFX_Setv(*list, layer3, 13200, pos);
    GFX_Setf(*list, layer3, 13009, (float)(grow * 0.30000001));
    GFX_Setf(*list, layer3, 13010, normSpeed06);
    vec.x = 1.0;
    vec.y = 1.0;
    vec.z = 0.0;
    GFX_Setv(*list, layer3, 13201, &vec);
    vec.x = -1.0;
    vec.y = -1.0;
    vec.z = 0.0;
    GFX_Setv(*list, layer3, 13202, &vec);
    vec.x = (float)(invDuration * 0.050000001);
    frictionXY = vec.x;
    vec.y = vec.x;
    vec.z = (float)(invDuration * 0.0);
    GFX_Setv(*list, layer3, 13203, &vec);
    GFX_FlagSet(*list, layer3, 0, 1);
    GFX_FlagSet(*list, layer3, 2, 1);

    layer4 = GFX_i_Add(list, 13, aiGao);
    GFX_SetMaterial(*list, layer4, material);
    GFX_Seti(*list, layer4, 13100, 50);
    GFX_Seti(*list, layer4, 13106, 50);
    GFX_Seti(*list, layer4, 13101, 0);
    GFX_Seti(*list, layer4, 13103, 0xFFFFFF);
    GFX_Seti(*list, layer4, 13104, 0xFF080808);
    GFX_Seti(*list, layer4, 13105, 0);
    GFX_Setf(*list, layer4, 13000, (float)(grow * 0.15000001));
    GFX_Setf(*list, layer4, 13001, normSpeed02);
    frictionZ = (float)(invDuration * 0.029999999);
    GFX_Setf(*list, layer4, 13002, frictionZ);
    GFX_Setf(*list, layer4, 13003, (float)(duration * 1.2));
    GFX_Setf(*list, layer4, 13004, (float)(duration * 2.2));
    GFX_Setf(*list, layer4, 13005, (float)(size * 0.02));
    GFX_Setf(*list, layer4, 13006, (float)(size * 0.022));
    GFX_Setf(*list, layer4, 13007, (float)(gravityBase * 0.02));
    GFX_Setf(*list, layer4, 13008, (float)(duration * 0.0040000002));
    GFX_Setv(*list, layer4, 13200, pos);
    GFX_Setf(*list, layer4, 13009, normSpeed02);
    GFX_Setf(*list, layer4, 13010, normSpeed05);
    GFX_Setf(*list, layer4, 13012, invDuration + invDuration);
    vec.x = 1.0;
    vec.y = 1.0;
    vec.z = 1.2f;
    GFX_Setv(*list, layer4, 13201, &vec);
    vec.x = -1.0;
    vec.y = -1.0;
    vec.z = 0.0;
    GFX_Setv(*list, layer4, 13202, &vec);
    vec.x = frictionXY;
    vec.y = frictionXY;
    vec.z = frictionZ;
    GFX_Setv(*list, layer4, 13203, &vec);
    GFX_FlagSet(*list, layer4, 0, 1);
    GFX_FlagSet(*list, layer4, 2, 1);
}


void GFX_Free(struct sGfxElem** list, int reinitType)
{
    tGfxElem* elem;
    tGfxElem* kept;
    tGfxElem* next;
    tGameObjectMain* gao;

    elem = *list;
    kept = 0;
    while (elem)
    {
        next = elem->prevGfxElem;
        gao = elem->aiGameObject;
        if (reinitType == 20 && gao && (gao->flags2 & 0x4000000) == 0)
        {
            kept = elem;
        }
        else
        {
            GFX_FreeOne(elem);
            if (kept)
                kept->prevGfxElem = next;
            else
                *list = next;
        }
        elem = next;
    }
}