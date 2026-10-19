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
    int* v5;
    int* v6;
    unsigned int v7;
    unsigned int v8;
    tGeoTriangleChunks* v9;
    int v10;
    tGeoTriangleChunks* v11;

    if (pointsNum && pointsNum <= 5000)
    {
        if (useVertexColors)
        {
            if (GFX_ul_MaxRLI < pointsNum)
            {
                if (GFX_pul_RLI)
                    v5 = (int*)MEM_p_Realloc(GFX_pul_RLI, 4 * pointsNum + 4);
                else
                    v5 = (int*)MEM_p_Alloc(4 * pointsNum + 4);
                GFX_pul_RLI = v5;
                GFX_ul_MaxRLI = pointsNum;
            }
            *GFX_pul_RLI = pointsNum;
        }
        if (GFX_gpst_GO)
        {
            v6 = GFX_pul_RLI;
            GFX_gpst_Geo->spritesNum = 0;
            GFX_gpst_Geo->vertexColors = (int*)(useVertexColors != 0 ? (unsigned int)v6 : 0);
            if (pointsNum > GFX_ul_MaxPoints)
            {
                MEM_FreeAlign(GFX_gpst_Geo->vertices);
                GFX_gpst_Geo->vertices = (Vec3*)MEM_p_AllocAligned(sizeof(Vec3) * pointsNum, 16);
                MEM_FreeAlign(GFX_gpst_Geo->normals);
                GFX_gpst_Geo->normals = (Vec3*)MEM_p_AllocAligned(sizeof(Vec3) * pointsNum, 16);
                GFX_ul_MaxPoints = pointsNum;
            }
            v7 = GFX_ul_MaxUVs;
            GFX_gpst_Geo->vertexNum = pointsNum;
            if (UVNum > v7)
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
            v8 = GFX_ul_MaxElements;
            GFX_gpst_Geo->triangleChunks->triangleNum = triangleNum;
            if (triangleChunkNum > v8)
            {
                if (v8)
                {
                    v9 = (tGeoTriangleChunks*)MEM_p_Realloc(GFX_gpst_Geo->triangleChunks, sizeof(tGeoTriangleChunks) * triangleChunkNum);
                    v10 = GFX_ul_MaxElements;
                    GFX_gpst_Geo->triangleChunks = v9;
                    memset(&GFX_gpst_Geo->triangleChunks[v10], 0, sizeof(tGeoTriangleChunks) * (triangleChunkNum - v10));
                    GFX_ul_MaxElements = triangleChunkNum;
                    GFX_gpst_Geo->triangleChunkCount = triangleChunkNum;
                    return;
                }
                GFX_gpst_Geo->triangleChunks = (tGeoTriangleChunks*)MEM_p_Alloc(sizeof(tGeoTriangleChunks) * triangleChunkNum);
                GFX_ul_MaxElements = triangleChunkNum;
            }
            GFX_gpst_Geo->triangleChunkCount = triangleChunkNum;
        }
        else
        {
            memset(GFX_aul_MaxTriangles, 0, sizeof(GFX_aul_MaxTriangles));
            GEO_b_IsInFix = 1;
            GFX_gpst_Geo = GEO_pst_Create(pointsNum, UVNum, triangleChunkNum, 0);
            GEO_b_IsInFix = 0;
            v11 = GFX_gpst_Geo->triangleChunks;
            v11->triangleNum = triangleNum;
            v11->MRMUVIndexElems = 0;
            v11->triangleStripBlock = 0;
            v11->MRMUVIndicesNum = 0;
            GEO_AllocElementContent(v11);
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
        }
    }
}

void GFX_NeedGeomExElem(unsigned int triangleChunkIdx, unsigned int triangleNum)
{
    tGeoTriangleChunks* v2;
    unsigned int v3;
    tGeoTriangleChunks* v4;

    if (triangleChunkIdx < GFX_gpst_Geo->triangleChunkCount)
    {
        v2 = GFX_gpst_Geo->triangleChunks;
        v3 = GFX_aul_MaxTriangles[triangleChunkIdx];
        v4 = &v2[triangleChunkIdx];
        if (triangleNum > v3)
        {
            if (v3)
            {
                v4->triangleChunkElems = (tGeoTriangleChunkElem*)MEM_p_ReallocAligned(v4->triangleChunkElems, sizeof(tGeoTriangleChunkElem) * triangleNum, 16);
                GFX_aul_MaxTriangles[triangleChunkIdx] = triangleNum;
                v4->triangleNum = triangleNum;
                return;
            }
            v4->triangleNum = triangleNum;
            v4->MRMUVIndexElems = 0;
            v4->triangleStripBlock = 0;
            v4->MRMUVIndicesNum = 0;
            GEO_AllocElementContent(&v2[triangleChunkIdx]);
            GFX_aul_MaxTriangles[triangleChunkIdx] = triangleNum;
        }
        v4->triangleNum = triangleNum;
    }
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

void GFX_SetCurrent(struct sGfxElem* a1, int16_t id)
{
    tGfxElem* v2;

    if (!GFX_gpst_Current || GFX_gpst_Current->id != id)
    {
        v2 = a1;
        for (GFX_gpst_Current = a1; v2; GFX_gpst_Current = v2)
        {
            if (v2->id == id)
                break;
            v2 = v2->prevGfxElem;
        }
    }
}

void GFX_FlagSet(tGfxElem* a1, int16_t id, char a3, int set)
{
    int v4;
    byte v5;

    GFX_SetCurrent(a1, id);
    if (GFX_gpst_Current)
    {
        v4 = 1 << a3;
        v5 = GFX_gpst_Current->gfxFlags;
        if (set)
            GFX_gpst_Current->gfxFlags = v4 | v5;
        else
            GFX_gpst_Current->gfxFlags = ~(uint8_t)v4 & v5;
    }
}

void GFX_SetMaterial(tGfxElem* a1, int16_t id, struct sGEOMAT* a3)
{
    GFX_SetCurrent(a1, id);
    if (GFX_gpst_Current)
        GFX_gpst_Current->material = a3;
}

void GFX_NeedGeomNbElemSprites(unsigned int a1)
{
    int v1;
    BOOL v2;
    tGeoSprites* v3;
    size_t v4;

    v1 = GFX_ul_MaxElemSprites;
    v2 = a1 < GFX_ul_MaxElemSprites;
    GFX_gpst_Geo->spritesNum = a1;
    if (!v2)
    {
        v4 = sizeof(tGeoSprites) * a1;
        if (v1)
        {
            v3 = (tGeoSprites*)MEM_p_Realloc(GFX_gpst_Geo->spritesPtr, v4);
            GFX_ul_MaxElemSprites = a1;
            GFX_gpst_Geo->spritesPtr = v3;
        }
        else
        {
            GFX_gpst_Geo->spritesPtr = (tGeoSprites*)MEM_p_Alloc(v4);
            GFX_ul_MaxElemSprites = a1;
        }
    }
}

void GFX_NeedGeomElemNbSprites(unsigned int a1, unsigned int a2)
{
    unsigned int v2;
    tGeoSprites* v3;

    if (a1 < GFX_gpst_Geo->spritesNum)
    {
        v2 = GFX_aul_MaxTriangles[a1];
        v3 = &GFX_gpst_Geo->spritesPtr[a1];
        if (a2 > v2)
        {
            if (v2)
            {
                v3->spriteElems = (tGeoSpriteElem*)MEM_p_Realloc(v3->spriteElems, sizeof(tGeoSpriteElem) * a2);
                GFX_aul_MaxSprites[a1] = a2;
                v3->num = a2;
                return;
            }
            v3->spriteElems = (tGeoSpriteElem*)MEM_p_Alloc(sizeof(tGeoSpriteElem) * a2);
            GFX_aul_MaxSprites[a1] = a2;
        }
        v3->num = a2;
    }
}

int GFX_i_Request(int a1)
{
    int result;

    if (a1 < 1000 || a1 >= 2000)
        result = 0;
    else
        result = 0; // return_0_sub_492830(); GFX_i_Line_Request();
    return result;
}

void GFX_NeedGeom(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleNum, int useVertexColors)
{
    GFX_NeedGeomEx(pointsNum, UVNum, 1, triangleNum, useVertexColors);
}

struct sGfxElem* GFX_pst_FindById(struct sGfxElem** a1, int16_t gfxId)
{
    tGfxElem* result;

    result = *a1;
    if (!*a1)
        return 0;
    while (result->id != gfxId)
    {
        result = result->prevGfxElem;
        if (!result)
            return 0;
    }
    return result;
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

struct sGfxElem* GFX_Attached(struct sGfxElem* a1)
{
    tGfxElem* result;

    for (result = a1; result; result = result->prevGfxElem)
    {
        if (result->type == 16)                   // carte
            ((tGfxCarteData*)(result->privateData))->currentPieceStatus = -1;
    }
    return result;
}

void GFX_FreeOne(tGfxElem* a1)
{
    __int8 v1;

    v1 = a1->type;
    if (a1->type == 3)
    {
        GFX_Desintegrate_Destroy((tGfxDesintegrateData*)(a1->privateData));
    }
    else
    {
        switch (v1)
        {
        case 2:
            GFX_Lightning_Destroy((tGfxLightningData*)(a1->privateData));
            break;
        case 6:
            GFX_Smoke_Destroy((tGfxSmokeData*)(a1->privateData));
            break;
        case 9:
            GFX_Spark_Destroy((tGfxSparkData*)(a1->privateData));
            break;
        case 10:
            GFX_Water2_Destroy((tGfxWater2Data*)(a1->privateData));
            break;
        case 12:
            GFX_Profiler_Destroy((tGfxProfilerData*)(a1->privateData));
            break;
        case 13:
            GFX_Explode_Destroy((tGfxExplodeData*)(a1->privateData));
            MEM_FreeAlign(a1->privateData);
            break;
        case 16:
            GFX_Carte_Destroy((tGfxCarteData*)(a1->privateData), a1->material);
            break;
        case 17:
            GFX_Gao_Destroy((tGfxGaoData*)(a1->privateData));
            break;
        case 19:
            GFX_Str_Destroy((tGfxStrData*)(a1->privateData));
            break;
        case 20:
            GFX_Ripple_Destroy((tGfxRippleData*)(a1->privateData));
            break;
        case 18:
            GFX_Sun_Destroy(&(((tGfxSunData*)(a1->privateData))->d3dStuff));
            break;
        }
    }
    if (a1->type != 13)
        MEM_Free(a1->privateData);
    MEM_Free(a1);
}

void GFX_Del(tGfxElem** a1, __int16 id)
{
    tGfxElem* v2;
    tGfxElem* v3;

    v2 = *a1;
    v3 = 0;
    if (*a1)
    {
        while (v2->id != id)
        {
            v3 = v2;
            v2 = v2->prevGfxElem;
            if (!v2)
            {
                GFX_gpst_Current = 0;
                return;
            }
        }
        if (v3)
            v3->prevGfxElem = v2->prevGfxElem;
        else
            *a1 = v2->prevGfxElem;
        GFX_FreeOne(v2);
    }
    GFX_gpst_Current = 0;
}

void GFX_Test(tGfxElem* a1)
{
    tGfxElem* v1;
    unsigned __int16 v2;
    char v3;

    v1 = a1;
    if (a1)
    {
        v2 = gpst_DX_stDD->matrixStack.currElemNum;
        gpst_DX_stDD->matrixStack.currElemNum = 0;
        gpst_DX_stDD->matrixStack.actMatrixElem = gpst_DX_stDD->matrixStack.elems;
        gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
        GFX_gpst_Current = a1;
        do
        {
            v3 = v1->gfxFlags;
            if ((v3 & 1) != 0 && v3 >= 0 && v1->type >= 4 && v1->type <= 5)
            {
                GFX_Flare_Test((tGfxFlareData*)(v1->privateData));
                v1 = GFX_gpst_Current;
            }
            v1 = v1->prevGfxElem;
            GFX_gpst_Current = v1;
        } while (v1);
        gpst_DX_stDD->matrixStack.currElemNum = v2;
        gpst_DX_stDD->matrixStack.actMatrixElem = &gpst_DX_stDD->matrixStack.elems[v2];
        gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    }
}

int GFX_i_SetDeath(struct sGfxElem** a1, struct sGfxElem* a2)
{
    tGfxFlareData* v3;
    int v4;

    if (a2)
    {
        if (a2->type == 4)                        // flare
        {
            v3 = (tGfxFlareData*)(a2->privateData);
            v4 = v3->basic.flags;
            if ((v4 & 0x20) != 0)
            {
                v3->basic.flags = v4 | 0x4000000;
                ((tGfxFlareData*)(a2->privateData))->basic.i15 = -1;
                return 1;
            }
        }
        else if (a2->type == 21)                  // anotherflare
        {
            ((tGfxAnotherFlareData*)(a2->privateData))->flags |= 1u;
            a2->lifeTime = ((tGfxAnotherFlareData*)(a2->privateData))->totalTime + 10.0f;
            return 1;
        }
        GFX_gpst_Current = a2;
        GFX_Del(a1, a2->id);
    }
    return 0;
}

void GFX_Render(tGfxElem** a1, int zlistLock)
{
    tGfxElem* v2;
    profilerRasterStructElem* v3;
    int v4;
    int v5;
    int v6;
    unsigned int v7;
    int v8;
    char v9;
    tGfxElem* v10;
    double v11;
    char v12;
    byte v13;
    int v14; 
    int v15; 
    profilerRasterStructElem* v16;
    __int16 v17;
    __int16 v18;
    int v19;
    int v20;
    int v21;
    int v22;
    unsigned __int16 v23;

    v2 = *a1;
    if (!*a1)
        return;
    QueryPerformanceCounter(&PerfCounterStruct[7]);
    v3 = &gpst_DX_stDD->rasters[45];
    if (!gpst_DX_stDD->rasters[45].insideCounter)
        v3->startTick = TIM_GetTicksAsInt32();
    ++v3->insideCounter;
    GFX_ppst_CurrentList = a1;
    v4 = gpst_DX_stDD->flags;
    v21 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = gpst_DX_stDD->drawMaskAllowedStuff & ~0x10000u;
    v22 = v4;
    v23 = gpst_DX_stDD->matrixStack.currElemNum;
    gpst_DX_stDD->matrixStack.currElemNum = 0;
    gpst_DX_stDD->matrixStack.actMatrixElem = gpst_DX_stDD->matrixStack.elems;
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    gpst_DX_stDD->currTargetGao = GFX_gpst_GO;
    gpst_DX_stDD->extraMaterialToGeo = 0;
    if ((byte)zlistLock)
    {
        SOFT_ZList_Lock();
        v5 = gpst_DX_stDD->flags | 4;
        v6 = v5;
        v7 = gpst_DX_stDD->flags & ~4u;
    }
    else
    {
        gpst_DX_stDD->flags |= 8u;
        v8 = gpst_DX_stDD->flags;
        v6 = v8 | 0x200;
        v5 = v8 | 4;
        v7 = v8 & ~0x200u;
    }
    v19 = v5;
    v20 = v7;
    do
    {
        ++gpst_DX_stDD->rasters[13].unk2;
        v9 = v2->gfxFlags;
        v10 = v2->prevGfxElem;
        if (v9 < 0 && (v9 & 8) != 0)
        {
            GFX_gpst_Current = v2;
            GFX_Del(a1, v2->id);
            goto LABEL_50;
        }
        v11 = v2->lifeTime - TIM_gf_dt;
        v2->lifeTime = (float)v11;
        if (v11 > 0.0 || GFX_i_SetDeath(a1, v2))
        {
            v12 = v2->gfxFlags;
            if ((v12 & 1) != 0 && v12 >= 0)
            {
                if (GFX_gpst_GO)
                    ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROMAT = (tGroGeneric*)(v2->material);
                v13 = v2->gfxFlags;
                if ((v13 & 4) != 0)
                {
                    if ((v13 & 2) == 0)
                    {
                        gpst_DX_stDD->flags = v6;
                    LABEL_23:
                        GFX_gpst_Current = v2;
                        switch (v2->type)
                        {
                        case 1:
                            GFX_Line_Render((tGfxLineData*)v2->privateData);
                            break;
                        case 2:
                            v15 = GFX_i_Lightning_Render((tGfxLightningData*)v2->privateData);
                            goto LABEL_46;
                        case 3:
                            v15 = GFX_i_Desintegrate_Render((tGfxDesintegrateData*)v2->privateData);
                            goto LABEL_46;
                        case 4:
                        case 5:
                            GFX_Flare_Render((tGfxFlareData*)v2->privateData);
                            break;
                        case 6:
                            v15 = GFX_i_Smoke_Render((tGfxSmokeData*)v2->privateData);
                            goto LABEL_46;
                        case 7:
                            v15 = GFX_i_Table_Render((tGfxTableData*)v2->privateData);
                            goto LABEL_46;
                        case 8:
                            v15 = GFX_i_Speed_Render((tGfxSpeedData*)v2->privateData);
                            goto LABEL_46;
                        case 9:
                            v15 = GFX_i_Spark_Render((tGfxSparkData*)v2->privateData);
                            goto LABEL_46;
                        case 10:
                            v15 = GFX_i_Water2_Render((tGfxWater2Data*)v2->privateData);
                            goto LABEL_46;
                        case 11:
                            v15 = GFX_i_Fade_Render((tGfxFadeData*)v2->privateData);
                            goto LABEL_46;
                        case 12:
                            v15 = GFX_i_Profiler_Render((tGfxProfilerData*)v2->privateData);
                            goto LABEL_46;
                        case 13:
                            if (!GFX_i_Explode_Render((tGfxExplodeData*)v2->privateData, v2->material))
                                v2->gfxFlags |= 0x80u;
                            GFX_SetRenderStateForExplode();
                            break;
                        case 14:
                            v15 = GFX_i_BorderFade_Render((tGfxBorderFadeData*)v2->privateData);
                            goto LABEL_46;
                        case 15:
                            v15 = GFX_i_Diaphragm_Render((tGfxDiaphragmData*)v2->privateData);
                            goto LABEL_46;
                        case 16:
                            v15 = GFX_i_Carte_Render((tGfxCarteData*)v2->privateData, v2->material, v2->aiGameObject);
                            goto LABEL_46;
                        case 17:
                            draw_game_object_in_GFX_sub_45DBB0((tGfxGaoData*)v2->privateData);
                            gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
                            gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
                            break;
                        case 18:
                            GFX_Sun_Render((tGfxSunData*)v2->privateData);
                            break;
                        case 19:
                            v15 = GFX_i_Str_Render((tGfxStrData*)v2->privateData);
                            goto LABEL_46;
                        case 20:
                            v15 = GFX_i_Ripple_Render((tGfxRippleData*)v2->privateData);
                            goto LABEL_46;
                        case 21:
                            v15 = GFX_i_AnotherFlare_Render((tGfxAnotherFlareData*)v2->privateData);
                        LABEL_46:
                            if (!v15)
                                v2->gfxFlags |= 0x80u;
                            break;
                        case 22:
                            //nullsub_32();                   // GFX_Tetris_Render
                            break;
                        default:
                            break;
                        }
                        ++gpst_DX_stDD->rasters[14].unk2;
                        goto LABEL_50;
                    }
                    v14 = v19;
                }
                else
                {
                    v14 = v20;
                }
                gpst_DX_stDD->flags = v14;
                goto LABEL_23;
            }
        }
    LABEL_50:
        v2 = v10;
    } while (v10);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v21;
    gpst_DX_stDD->flags = v22;
    gpst_DX_stDD->matrixStack.currElemNum = v23;
    gpst_DX_stDD->matrixStack.actMatrixElem = &gpst_DX_stDD->matrixStack.elems[v23];
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    if ((byte)zlistLock)
        SOFT_ZList_Unlock();
    PRO_QPC_RecordTimeDecRef(7);
    v16 = &gpst_DX_stDD->rasters[45];
    v17 = gpst_DX_stDD->rasters[45].insideCounter;
    if (v17)
    {
        v18 = v17 - 1;
        gpst_DX_stDD->rasters[45].insideCounter = v18;
        if (!v18)
            v16->sumTime += TIM_GetTicksAsInt32() - v16->startTick;
    }
}

void* GFX_Tetris_Dummy_Create() {
    return NULL;
}

int GFX_i_Add(tGfxElem** gfxList, char type, struct sGameObjectMain* aiGao)
{
    byte v3;
    void* v4;
    void* v5;
    int result;
    tGfxElem* newGfxElem;
    __int16 v8;

    if (!GFX_gpst_GO)
        GFX_NeedGeom(4u, 4u, 2u, 1);
    v3 = 0;
    switch (type)
    {
    case 1:
        v4 = GFX_Line_Create();
        goto LABEL_27;
    case 2:
        v4 = GFX_Lightning_Create();
        goto LABEL_27;
    case 3:
        v4 = GFX_Desintegrate_Create(aiGao);
        v3 = 8;
        goto LABEL_27;
    case 4:
        v4 = GFX_Flare_Create(0);
        goto LABEL_27;
    case 5:
        v4 = GFX_Flare_Create(1);
        goto LABEL_27;
    case 6:
        v4 = GFX_Smoke_Create();
        v3 = 8;
        goto LABEL_27;
    case 7:
        v4 = GFX_Table_Create();
        v3 = 8;
        goto LABEL_27;
    case 8:
        v4 = GFX_Speed_Create();
        v3 = 8;
        goto LABEL_27;
    case 9:
        v4 = GFX_Spark_Create();
        v3 = 8;
        goto LABEL_27;
    case 10:
        v4 = GFX_Water2_Create();
        v3 = 8;
        goto LABEL_27;
    case 11:
        v4 = GFX_Fade_Create();
        v3 = 8;
        goto LABEL_27;
    case 12:                                    // watertrace??
        v4 = GFX_Profiler_Create();
        goto LABEL_27;
    case 13:                                    // flames is
        v4 = GFX_Explode_Create();
        v3 = 8;
        goto LABEL_27;
    case 14:
        v4 = GFX_BorderFade_Create();
        v3 = 8;
        goto LABEL_27;
    case 15:
        v4 = GFX_Diaphragm_Create();
        v3 = 8;
        goto LABEL_27;
    case 16:
        v4 = GFX_Carte_Create();
        goto LABEL_27;
    case 17:
        v4 = GFX_Gao_Create(aiGao);
        goto LABEL_26;
    case 18:                                    // flash
        v4 = GFX_Sun_Create();
        goto LABEL_26;
    case 19:
        v4 = GFX_Str_Create(aiGao);
        v3 = 9;
        goto LABEL_27;
    case 20:
        v4 = GFX_Ripple_Create(aiGao);
        v3 = 13;
        goto LABEL_27;
    case 21:                                    // halo
        v4 = GFX_AnotherFlare_Create(aiGao);
        v3 = 13;
        goto LABEL_27;
    case 22:
        v4 = GFX_Tetris_Dummy_Create();
    LABEL_26:
        v3 = 1;
    LABEL_27:
        v5 = v4;
        if (!v4)
            goto LABEL_28;
        if (GFX_sguw_Id < 1024u)
            GFX_sguw_Id = 1024;
        newGfxElem = (tGfxElem*)MEM_p_Alloc(sizeof(tGfxElem));
        newGfxElem->prevGfxElem = *gfxList;
        v8 = GFX_sguw_Id;
        newGfxElem->gfxFlags = v3;
        newGfxElem->privateData = v5;
        newGfxElem->aiGameObject = aiGao;
        newGfxElem->id = v8;
        newGfxElem->type = type;
        newGfxElem->material = 0;
        newGfxElem->lifeTime = 3.402822e38f; // TODO FIXME? float.max?
        GFX_gpst_Current = newGfxElem;
        *gfxList = newGfxElem;
        result = (uint16_t)newGfxElem->id;
        GFX_sguw_Id = v8 + 1;
        break;
    default:
    LABEL_28:
        result = -1;
        break;
    }
    return result;
}

int GFX_i_AddSorted(tGfxElem** a1, char type, unsigned __int16 idx, struct sGameObjectMain* a4)
{
    int result;
    tGfxElem* v5;
    unsigned __int16 v6;
    byte v7;
    void* v8;
    void* v9;
    tGfxElem* v10;
    tGfxElem* i;

    if (!GFX_gpst_GO)
        GFX_NeedGeom(4u, 4u, 2u, 1);
    if (idx >= 1024u)
        return -1;
    v5 = *a1;
    for (i = 0; v5; v5 = v5->prevGfxElem)
    {
        v6 = v5->id;
        if (v6 < idx)
            break;
        if (v6 == idx)
            return -1;
        i = v5;
    }
    v7 = 0;
    switch (type)
    {
    case 1:
        v8 = GFX_Line_Create();
        goto LABEL_34;
    case 2:
        v8 = GFX_Lightning_Create();
        goto LABEL_34;
    case 3:
        v8 = GFX_Desintegrate_Create(a4);
        v7 = 8;
        goto LABEL_34;
    case 4:
        v8 = GFX_Flare_Create(0);
        goto LABEL_34;
    case 5:
        v8 = GFX_Flare_Create(1);
        goto LABEL_34;
    case 6:
        v8 = GFX_Smoke_Create();
        v7 = 8;
        goto LABEL_34;
    case 7:
        v8 = GFX_Table_Create();
        v7 = 8;
        goto LABEL_34;
    case 8:
        v8 = GFX_Speed_Create();
        v7 = 8;
        goto LABEL_34;
    case 9:
        v8 = GFX_Spark_Create();
        v7 = 8;
        goto LABEL_34;
    case 10:
        v8 = GFX_Water2_Create();
        v7 = 8;
        goto LABEL_34;
    case 11:
        v8 = GFX_Fade_Create();
        v7 = 8;
        goto LABEL_34;
    case 12:
        v8 = GFX_Profiler_Create();
        goto LABEL_34;
    case 13:
        v8 = GFX_Explode_Create();
        v7 = 8;
        goto LABEL_34;
    case 14:
        v8 = GFX_BorderFade_Create();
        v7 = 8;
        goto LABEL_34;
    case 15:
        v8 = GFX_Diaphragm_Create();
        v7 = 8;
        goto LABEL_34;
    case 16:
        v8 = GFX_Carte_Create();
        goto LABEL_34;
    case 17:
        v8 = GFX_Gao_Create(a4);
        goto LABEL_33;
    case 18:
        v8 = GFX_Sun_Create();
        goto LABEL_33;
    case 19:
        v8 = GFX_Str_Create(a4);
        v7 = 9;
        goto LABEL_34;
    case 20:
        v8 = GFX_Ripple_Create(a4);
        v7 = 13;
        goto LABEL_34;
    case 21:
        v8 = GFX_AnotherFlare_Create(a4);
        v7 = 13;
        goto LABEL_34;
    case 22:
        v8 = GFX_Tetris_Dummy_Create();
    LABEL_33:
        v7 = 1;
    LABEL_34:
        v9 = v8;
        if (!v8)
            return -1;
        v10 = (tGfxElem*)MEM_p_Alloc(sizeof(tGfxElem));
        v10->type = type;
        v10->prevGfxElem = v5;
        v10->gfxFlags = v7;
        v10->privateData = v9;
        v10->aiGameObject = a4;
        v10->material = 0;
        v10->id = idx;
        v10->lifeTime = 3.402822e38f; // TODO FIXME: float.max?
        GFX_gpst_Current = v10;
        if (i)
            i->prevGfxElem = v10;
        else
            *a1 = v10;
        result = idx;
        break;
    default:
        return -1;
    }
    return result;
}

Vec3* GFX_pst_Getv(tGfxElem* a1, __int16 a2, int a3)
{
    int v3;
    int v4;
    __int16 v5;
    Vec3* result;

    GFX_SetCurrent(a1, a2);
    if (!GFX_gpst_Current
        || (v3 = GFX_gpst_Current->type, v4 = ga_Paramv_First[v3], a3 < v4)
        || a3 >= v4 + ga_Paramv_Number[v3]
        || (v5 = gaa_Paramv_Offset[a3 + 20 * v3 - v4], (v5 & 0x4000) != 0))
    {
        result = NULL;
    }
    else
    {
        result = (Vec3*)((char*)GFX_gpst_Current->privateData + v5);
    }
    return result;
}

float GFX_f_Getf(tGfxElem* a1, __int16 id, int a3)
{
    int v3;
    int v4;
    __int16 v5;
    float result;

    GFX_SetCurrent(a1, id);
    if (!GFX_gpst_Current
        || (v3 = GFX_gpst_Current->type, v4 = ga_Paramf_First[v3], a3 < v4)
        || a3 >= v4 + ga_Paramf_Number[v3]
        || (v5 = gaa_Paramf_Offset[a3 + 21 * v3 - v4], (v5 & 0x4000) != 0))
    {
        result = 0.0;
    }
    else
    {
        result = *(float*)((char*)GFX_gpst_Current->privateData + v5);
    }
    return result;
}

// TOOD FIXME ezek biztos hogy signedek?
int GFX_i_Geti(tGfxElem* a1, __int16 a2, int a3)
{
    int v4;
    int v5;
    __int16 v6;

    GFX_SetCurrent(a1, a2);
    if (!GFX_gpst_Current)
        return 0;
    v4 = GFX_gpst_Current->type;
    v5 = ga_Parami_First[v4];
    if (a3 < v5 || a3 >= v5 + ga_Parami_Number[v4])
        return 0;
    v6 = gaa_Parami_Offset[a3 + 20 * v4 - v5];
    if ((v6 & 0x4000) != 0)
    {
        if (GFX_gpst_Current->type == 13)
            return GFX_i_Explode_Geti((tGfxExplodeData*)GFX_gpst_Current->privateData, a3);
        return 0;
    }
    if ((v6 & 0x1000) != 0)
        return *((int8_t*)GFX_gpst_Current->privateData + (v6 & 0xFFF));
    if ((v6 & 0x2000) != 0)
        return *(int16_t*)((char*)GFX_gpst_Current->privateData + (v6 & 0xFFF));
    return *(int32_t*)((char*)GFX_gpst_Current->privateData + v6);
}

void GFX_Seti(tGfxElem* a1, __int16 id, int a3, int a4)
{
    __int8 v4;
    int v5;
    int v6;
    __int16 v7;
    char* v8;

    GFX_SetCurrent(a1, id);
    if (GFX_gpst_Current)
    {
        v4 = GFX_gpst_Current->type;
        v5 = GFX_gpst_Current->type;
        v6 = ga_Parami_First[v5];
        if (a3 >= v6 && a3 < v6 + ga_Parami_Number[v5])
        {
            v7 = gaa_Parami_Offset[a3 + 20 * v5 - v6];
            if ((v7 & 0x4000) != 0)
            {
                switch (v4)
                {
                case 6:
                    GFX_Smoke_Seti((tGfxSmokeData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 10:
                    GFX_Water2_Seti((tGfxWater2Data*)GFX_gpst_Current->privateData, a3, v7 & ~0x4000u, a4);
                    break;
                case 12:
                    GFX_Profiler_Seti((tGfxProfilerData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 13:
                    GFX_Explode_Seti((tGfxExplodeData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 18:
                    GFX_Sun_Seti((tGfxSunData*)GFX_gpst_Current->privateData, a3);
                    break;
                case 16:
                    GFX_Carte_Seti((tGfxCarteData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 19:
                    GFX_Str_Seti((tGfxStrData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 20:
                    GFX_Ripple_Seti((tGfxRippleData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                }
            }
            else if ((v7 & 0x1000) != 0)
            {
                *((int8_t*)GFX_gpst_Current->privateData + (v7 & 0xFFF)) = a4;
            }
            else
            {
                v8 = (char*)GFX_gpst_Current->privateData;
                if ((v7 & 0x2000) != 0)
                    *(int16_t*)&v8[v7 & 0xFFF] = a4;
                else
                    *(int32_t*)&v8[v7] = a4;
            }
        }
    }
}

void GFX_Setf(tGfxElem* a1, int id, int a2, float a3)
{
    __int8 v4;
    int v5;
    int v6;
    __int16 v7;

    GFX_SetCurrent(a1, id);
    if (GFX_gpst_Current)
    {
        v4 = GFX_gpst_Current->type;
        v5 = GFX_gpst_Current->type;
        v6 = ga_Paramf_First[v5];
        if (a2 >= v6 && a2 < v6 + ga_Paramf_Number[v5])
        {
            v7 = gaa_Paramf_Offset[a2 + 21 * v5 - v6];
            if ((v7 & 0x4000) != 0)
            {
                switch (v4)
                {
                case 10:
                    GFX_Water2_Setf((tGfxWater2Data*)GFX_gpst_Current->privateData, a2, v7 & ~0x4000, a3);
                    break;
                case 12:
                    GFX_Profiler_Setf((tGfxProfilerData*)GFX_gpst_Current->privateData, a2, a3);
                    break;
                case 13:
                    GFX_Explode_Setf((tGfxExplodeData*)GFX_gpst_Current->privateData, a2, a3);
                    break;
                case 20:
                    GFX_Ripple_Setf((tGfxRippleData*)GFX_gpst_Current->privateData, a2, a3);
                    break;
                }
            }
            else
            {
                *(float*)((char*)GFX_gpst_Current->privateData + v7) = a3;
            }
        }
    }
}

void GFX_Setv(tGfxElem* a1, __int16 a2, int a3, Vec3* a4)
{
    __int8 v4;
    int v5;
    int v6;
    __int16 v7;

    GFX_SetCurrent(a1, a2);
    if (GFX_gpst_Current)
    {
        v4 = GFX_gpst_Current->type;
        v5 = GFX_gpst_Current->type;
        v6 = ga_Paramv_First[v5];
        if (a3 >= v6 && a3 < v6 + ga_Paramv_Number[v5])
        {
            v7 = gaa_Paramv_Offset[a3 + 20 * v5 - v6];
            if ((v7 & 0x4000) != 0)
            {
                switch (v4)
                {
                case 10:
                    GFX_Water2_Setv((tGfxWater2Data*)GFX_gpst_Current->privateData, a3, v7 & ~0x4000u, a4);
                    break;
                case 12:
                    GFX_Profiler_Setv((tGfxProfilerData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 13:
                    GFX_Explode_Setv((tGfxExplodeData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                case 7:
                    GFX_Table_Setv((tGfxTableData*)GFX_gpst_Current->privateData, a3, a4);
                    break;
                }
            }
            else
            {
                *(Vec3*)((char*)GFX_gpst_Current->privateData + v7) = *a4;
            }
        }
    }
}

void GFX_CreateExplosion(float a1, float a2, Vec3* a3, struct sGameObjectMain* aiGao, struct sGEOMAT* a5)
{
    tGfxElem** v6;
    int v7;
    double v8;
    double v9;
    int v10;
    int v11;
    int v12;
    float a3a;
    float a3b;
    float a3c;
    float a3d;
    float a3e;
    float a3f;
    float a3g;
    float a3h;
    float a3i;
    float a3j;
    float a3k;
    float a3l;
    float a3m;
    float a3n;
    float a3o;
    float a3p;
    float a3q;
    float a3r;
    float a3s;
    float a3t;
    float a3u;
    float a3v;
    float a3w;
    float a3x;
    float a3y;
    float a3z;
    float a3ba;
    float a3bb;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    float v49;
    float v50;
    float v51;
    Vec3 v52;
    float a4;
    float a4b;

    v41 = (float)(1.0 / a2);
    a3->z = (float)(a3->z + 0.1);
    v6 = &WOR_gpst_CurrentWorld->gfxCurrentList;
    v7 = GFX_i_Add(&WOR_gpst_CurrentWorld->gfxCurrentList, 13, aiGao);
    GFX_SetMaterial(*v6, v7, a5);
    GFX_Seti(*v6, v7, 13100, 10);
    GFX_Seti(*v6, v7, 13106, 10);
    GFX_Seti(*v6, v7, 13101, 1);
    GFX_Seti(*v6, v7, 13103, 0);
    GFX_Seti(*v6, v7, 13104, 0xFFFFFF);
    GFX_Seti(*v6, v7, 13105, 0);
    v8 = v41 * a1;
    a4 = (float)v8;
    a3a = (float)(v8 * 0.80000001);
    GFX_Setf(*v6, v7, 13000, a3a);
    a3b = (float)(a4 * 0.89999998);
    GFX_Setf(*v6, v7, 13001, a3b);
    v42 = (float)(v41 * 0.1);
    GFX_Setf(*v6, v7, 13002, v42);
    v44 = (float)(a2 * 0.029999999);
    GFX_Setf(*v6, v7, 13003, v44);
    GFX_Setf(*v6, v7, 13004, v44);
    v45 = (float)(a1 * 0.039999999);
    GFX_Setf(*v6, v7, 13005, v45);
    GFX_Setf(*v6, v7, 13006, v45);
    v9 = v41 * v41;
    v43 = (float)v9;
    a3c = (float)(v9 * 0.15000001);
    GFX_Setf(*v6, v7, 13007, a3c);
    a3d = (float)(a2 * 0.000049999999);
    GFX_Setf(*v6, v7, 13008, a3d);
    v46 = (float)(a4 * 0.5);
    GFX_Setf(*v6, v7, 13009, v46);
    v50 = (float)(a4 * 0.60000002);
    GFX_Setf(*v6, v7, 13010, v50);
    a3e = (float)(a3->z - 0.1);
    GFX_Setf(*v6, v7, 13011, a3e);
    GFX_Setv(*v6, v7, 13200, a3);
    v52.x = 1.0;
    v52.y = 1.0;
    v52.z = 1.0;
    GFX_Setv(*v6, v7, 13201, &v52);
    v52.x = -1.0;
    v52.y = -1.0;
    v52.z = 0.0;
    GFX_Setv(*v6, v7, 13202, &v52);
    v52.z = (float)(v41 * 0.5);
    v52.x = v42;
    v52.y = v42;
    GFX_Setv(*v6, v7, 13203, &v52);
    GFX_FlagSet(*v6, v7, 0, 1);
    GFX_FlagSet(*v6, v7, 2, 1);
    v10 = GFX_i_Add(v6, 13, aiGao);
    GFX_SetMaterial(*v6, v10, a5);
    GFX_Seti(*v6, v10, 13100, 80);
    GFX_Seti(*v6, v10, 13106, 80);
    GFX_Seti(*v6, v10, 13101, 2);
    GFX_Seti(*v6, v10, 13103, -1);
    GFX_Seti(*v6, v10, 13104, -1);
    GFX_Seti(*v6, v10, 13105, 0);
    v47 = (float)(a4 * 0.0);
    GFX_Setf(*v6, v10, 13000, v47);
    GFX_Setf(*v6, v10, 13001, v47);
    GFX_Setf(*v6, v10, 13002, v42);
    v48 = a2 + a2;
    GFX_Setf(*v6, v10, 13003, v48);
    GFX_Setf(*v6, v10, 13004, v48);
    a3f = (float)(a1 * 0.0020000001);
    GFX_Setf(*v6, v10, 13005, a3f);
    a3g = (float)(a1 * 0.0049999999);
    GFX_Setf(*v6, v10, 13006, a3g);
    a3h = (float)(v43 * -2.5);
    GFX_Setf(*v6, v10, 13007, a3h);
    a3i = (float)(a2 * 0.001);
    GFX_Setf(*v6, v10, 13008, a3i);
    v49 = (float)(a4 * 0.2);
    GFX_Setf(*v6, v10, 13009, v49);
    GFX_Setf(*v6, v10, 13010, v46);
    a3j = (float)(a3->z - 0.5);
    GFX_Setf(*v6, v10, 13011, a3j);
    GFX_Setv(*v6, v10, 13200, a3);
    v52.x = -1.0;
    v52.y = -1.0;
    v52.z = 0.0;
    GFX_Setv(*v6, v10, 13201, &v52);
    v52.x = 1.0;
    v52.y = 1.0;
    v52.z = 2.0;
    GFX_Setv(*v6, v10, 13202, &v52);
    v52.x = (float)(v41 * 0.0099999998);
    v52.y = v52.x;
    v52.z = (float)(v41 * 0.000099999997);
    GFX_Setv(*v6, v10, 13203, &v52);
    GFX_FlagSet(*v6, v10, 0, 1);
    GFX_FlagSet(*v6, v10, 2, 1);
    v11 = GFX_i_Add(v6, 13, aiGao);
    GFX_SetMaterial(*v6, v11, a5);
    GFX_Seti(*v6, v11, 13100, 30);
    GFX_Seti(*v6, v11, 13106, 30);
    GFX_Seti(*v6, v11, 13101, 1);
    GFX_Seti(*v6, v11, 13103, 0);
    GFX_Seti(*v6, v11, 13104, 0xFF000000);
    GFX_Seti(*v6, v11, 13105, 0);
    a3k = (float)(a4 * 0.1);
    GFX_Setf(*v6, v11, 13000, a3k);
    a3l = (float)(a4 * 0.13);
    GFX_Setf(*v6, v11, 13001, a3l);
    a3m = (float)(v41 * 0.02);
    GFX_Setf(*v6, v11, 13002, a3m);
    a3n = (float)(a2 * 0.30000001);
    GFX_Setf(*v6, v11, 13003, a3n);
    a3o = (float)(a2 * 1.4);
    GFX_Setf(*v6, v11, 13004, a3o);
    a3p = (float)(a1 * 0.050000001);
    GFX_Setf(*v6, v11, 13005, a3p);
    a3q = (float)(a1 * 0.059999999);
    GFX_Setf(*v6, v11, 13006, a3q);
    a3r = (float)(v43 * 0.0049999999);
    GFX_Setf(*v6, v11, 13007, a3r);
    a3s = (float)(a2 * 0.0049999999);
    GFX_Setf(*v6, v11, 13008, a3s);
    GFX_Setv(*v6, v11, 13200, a3);
    a3t = (float)(a4 * 0.30000001);
    GFX_Setf(*v6, v11, 13009, a3t);
    GFX_Setf(*v6, v11, 13010, v50);
    v52.x = 1.0;
    v52.y = 1.0;
    v52.z = 0.0;
    GFX_Setv(*v6, v11, 13201, &v52);
    v52.x = -1.0;
    v52.y = -1.0;
    v52.z = 0.0;
    GFX_Setv(*v6, v11, 13202, &v52);
    v52.x = (float)(v41 * 0.050000001);
    v51 = v52.x;
    v52.y = v52.x;
    v52.z = (float)(v41 * 0.0);
    GFX_Setv(*v6, v11, 13203, &v52);
    GFX_FlagSet(*v6, v11, 0, 1);
    GFX_FlagSet(*v6, v11, 2, 1);
    v12 = GFX_i_Add(v6, 13, aiGao);
    GFX_SetMaterial(*v6, v12, a5);
    GFX_Seti(*v6, v12, 13100, 50);
    GFX_Seti(*v6, v12, 13106, 50);
    GFX_Seti(*v6, v12, 13101, 0);
    GFX_Seti(*v6, v12, 13103, 0xFFFFFF);
    GFX_Seti(*v6, v12, 13104, 0xFF080808);
    GFX_Seti(*v6, v12, 13105, 0);
    a3u = (float)(a4 * 0.15000001);
    GFX_Setf(*v6, v12, 13000, a3u);
    GFX_Setf(*v6, v12, 13001, v49);
    a4b = (float)(v41 * 0.029999999);
    GFX_Setf(*v6, v12, 13002, a4b);
    a3v = (float)(a2 * 1.2);
    GFX_Setf(*v6, v12, 13003, a3v);
    a3w = (float)(a2 * 2.2);
    GFX_Setf(*v6, v12, 13004, a3w);
    a3x = (float)(a1 * 0.02);
    GFX_Setf(*v6, v12, 13005, a3x);
    a3y = (float)(a1 * 0.022);
    GFX_Setf(*v6, v12, 13006, a3y);
    a3z = (float)(v43 * 0.02);
    GFX_Setf(*v6, v12, 13007, a3z);
    a3ba = (float)(a2 * 0.0040000002);
    GFX_Setf(*v6, v12, 13008, a3ba);
    GFX_Setv(*v6, v12, 13200, a3);
    GFX_Setf(*v6, v12, 13009, v49);
    GFX_Setf(*v6, v12, 13010, v46);
    a3bb = v41 + v41;
    GFX_Setf(*v6, v12, 13012, a3bb);
    v52.x = 1.0;
    v52.y = 1.0;
    v52.z = (float)1.2;
    GFX_Setv(*v6, v12, 13201, &v52);
    v52.x = -1.0;
    v52.y = -1.0;
    v52.z = 0.0;
    GFX_Setv(*v6, v12, 13202, &v52);
    v52.x = v51;
    v52.y = v51;
    v52.z = a4b;
    GFX_Setv(*v6, v12, 13203, &v52);
    GFX_FlagSet(*v6, v12, 0, 1);
    GFX_FlagSet(*v6, v12, 2, 1);
}

void GFX_Free(struct sGfxElem** a1, int a2)
{
    tGfxElem* v2;
    tGfxElem* v3;
    tGfxElem* v4;
    tGameObjectMain* v5;

    v2 = *a1;
    v3 = 0;
    if (*a1)
    {
        do
        {
            v4 = v2->prevGfxElem;
            if (a2 == 20 && (v5 = v2->aiGameObject) != 0 && (v5->flags2 & 0x4000000) == 0)
            {
                v3 = v2;
            }
            else
            {
                GFX_FreeOne(v2);
                if (v3)
                    v3->prevGfxElem = v4;
                else
                    *a1 = v4;
            }
            v2 = v4;
        } while (v4);
    }
}