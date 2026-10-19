#pragma once
#include "gameobject.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/geo.h"
#include "common.h"

typedef struct sGfxElem
{
    __int8 type;
    byte gfxFlags;
    __int16 id;
    void* privateData;
    struct sGEOMAT* material;
    struct sGameObjectMain* aiGameObject;
    struct sGfxElem* prevGfxElem;
    float lifeTime;
} tGfxElem;

int* GFX_pul_RLI;
unsigned int GFX_ul_MaxRLI;
struct sGameObjectMain* GFX_gpst_GO;
struct sGEOGRO* GFX_gpst_Geo;
unsigned int GFX_ul_MaxPoints;
unsigned int GFX_ul_MaxUVs;
unsigned int GFX_aul_MaxTriangles[64];
unsigned int GFX_ul_MaxElements;
unsigned int GFX_ul_MaxElemSprites;

tGfxElem* GFX_gpst_Current;
tGfxElem** GFX_ppst_CurrentList;
int GFX_aul_MaxSprites[32];

int GFX_i_GlobalID;

void GFX_NeedGeom(unsigned int pointsNum, unsigned int UVNum, unsigned int a3, int a4);
void GFX_NeedGeomEx(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleChunkNum, unsigned int triangleNum, int useVertexColors);
void GFX_NeedGeomExElem(unsigned int a1, unsigned int a2);
void GFX_NeedGeomNbElemSprites(unsigned int a1);
void GFX_NeedGeomElemNbSprites(unsigned int a1, unsigned int a2);
void GFX_Del(tGfxElem** a1, __int16 id);
void GFX_End();
struct sGfxElem* GFX_Attached(struct sGfxElem* a1);
void GFX_Test(tGfxElem* a1);
void GFX_SetCurrent(struct sGfxElem* a1, int16_t id);
int GFX_i_Add(tGfxElem** gfxList, char type, struct sGameObjectMain* aiGao);
int GFX_i_AddSorted(tGfxElem** a1, char type, unsigned __int16 idx, struct sGameObjectMain* a4);
struct sGfxElem* GFX_pst_FindById(struct sGfxElem** a1, int16_t gfxId);
int GFX_i_SetDeath(struct sGfxElem** a1, struct sGfxElem* a2);
int GFX_i_Request(int a1);
int GFX_i_Geti(tGfxElem* a1, __int16 a2, int a3);
void GFX_Seti(tGfxElem* a1, __int16 id, int a3, int a4);
void GFX_Setf(tGfxElem* a1, int id, int a2, float a3);
float GFX_f_Getf(tGfxElem* a1, __int16 id, int a3);
Vec3* GFX_pst_Getv(tGfxElem* a1, __int16 a2, int a3);
void GFX_Setv(tGfxElem* a1, __int16 a2, int a3, Vec3* a4);
void GFX_FlagSet(tGfxElem* a1, int16_t id, char a3, int set);
void GFX_SetMaterial(tGfxElem* a1, int16_t id, struct sGEOMAT* a3);
void GFX_Render(tGfxElem** a1, int zlistLock);
void GFX_Free(struct sGfxElem** a1, int a2);
void GFX_CreateExplosion(float a1, float a2, Vec3* a3, struct sGameObjectMain* aiGao, struct sGEOMAT* a5);