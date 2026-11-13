#pragma once
#include "gameobject.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/geo.h"
#include "common.h"

typedef struct sGfxElem
{
    int8_t type;
    uint8_t gfxFlags;
    int16_t id;
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

void GFX_NeedGeom(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleNum, int useVertexColors);
void GFX_NeedGeomEx(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleChunkNum, unsigned int triangleNum, int useVertexColors);
void GFX_SetQuadIndices(void);
void GFX_SetQuadUVs(void);
void GFX_RenderGeom(unsigned int maskBitsToClear);
void GFX_NeedGeomExElem(unsigned int triangleChunkIdx, unsigned int triangleNum);
void GFX_NeedGeomNbElemSprites(unsigned int spriteChunkNum);
void GFX_NeedGeomElemNbSprites(unsigned int spriteChunkIdx, unsigned int spriteNum);
void GFX_Del(tGfxElem** list, int16_t id);
void GFX_End();
struct sGfxElem* GFX_Attached(struct sGfxElem* list);
void GFX_Test(tGfxElem* list);
void GFX_SetCurrent(struct sGfxElem* list, int16_t id);
int GFX_i_Add(tGfxElem** gfxList, char type, struct sGameObjectMain* aiGao);
int GFX_i_AddSorted(tGfxElem** gfxList, char type, uint16_t idx, struct sGameObjectMain* aiGao);
struct sGfxElem* GFX_pst_FindById(struct sGfxElem** list, int16_t gfxId);
int GFX_i_SetDeath(struct sGfxElem** list, struct sGfxElem* elem);
int GFX_i_Request(int id);
int GFX_i_Geti(tGfxElem* list, int16_t id, int param);
void GFX_Seti(tGfxElem* list, int16_t id, int param, int value);
void GFX_Setf(tGfxElem* list, int id, int param, float value);
float GFX_f_Getf(tGfxElem* list, int16_t id, int param);
Vec3* GFX_pst_Getv(tGfxElem* list, int16_t id, int param);
void GFX_Setv(tGfxElem* list, int16_t id, int param, Vec3* value);
void GFX_FlagSet(tGfxElem* list, int16_t id, char bit, int set);
void GFX_SetMaterial(tGfxElem* list, int16_t id, struct sGEOMAT* material);
void GFX_Render(tGfxElem** list, int zlistLock);
void GFX_Free(struct sGfxElem** list, int reinitType);
void GFX_CreateExplosion(float size, float duration, Vec3* pos, struct sGameObjectMain* aiGao, struct sGEOMAT* material);