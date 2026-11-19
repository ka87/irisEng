#pragma once
#include "gro_common.h"
#include "../common.h"

/* Named below in a parameter list and nowhere else, so without this
   each prototype would declare a type of its own, scoped to itself:
   two functions taking "the same" pointer would not agree, and
   assigning one to the other is a type error rather than the
   intended opaque handle. */
struct sGroGeneric;

/* The renderer's buffers for a piece of geometry live in tGEOGRO
   below. They belong to whichever renderer made them, they mean
   nothing to anyone else, and nothing in the engine does anything
   with them but hand them back -- so they are opaque handles here,
   and this header, which most of the engine includes, names no
   renderer's types and needs no renderer's API. Each renderer casts
   them to its own. */

typedef struct sGeoTriangleStripElem
{
    uint16_t triangleIdx;
    uint16_t uvIdx;
} tGeoTriangleStripElem;

typedef struct sGeoTriangleStrip
{
    int triangleStripElemNum;
    struct sGeoTriangleStripElem* triangleStripElems;
    int vbIdx;
} tGeoTriangleStrip;

typedef struct sGeoTriangleChunkElem
{
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;
    uint16_t uv1;
    uint16_t uv2;
    uint16_t uv3;
    uint32_t mrmSmoothingGroup;
    int unk2;
} tGeoTriangleChunkElem;

typedef struct sGeoMRMUVIndexElem
{
    int UVStartOffset;
    int i2;
    int i3;
} tGeoMRMUVIndexElem;

typedef struct sD3dStreamInfoElem
{
    int primitiveCount;
    int startIndex;
    int vbIdx;
} tD3dStreamInfoElem;


typedef struct sGeoWholeTriangleStripBlock
{
    int vertexColorArraySecond;
    unsigned int triangleStripNum;
    struct sGeoTriangleStrip* triangleStrips;
    tGeoTriangleStripElem* triangleStripElemsContinousPool;
    int unk1;
    int unk2;
    int unk3;
    struct sD3dStreamInfoElem* d3dStreamInfoElems;
    unsigned int streamsNum;
} tGeoWholeTriangleStripBlock;

typedef struct sGeoTriangleChunks
{
    uint32_t triangleNum;
    int materialNum;
    struct sGeoTriangleChunkElem* triangleChunkElems;
    uint32_t MRMUVIndicesNum;
    uint16_t* MRMUVIndices;
    struct sGeoMRMUVIndexElem* MRMUVIndexElems;
    struct sGeoWholeTriangleStripBlock* triangleStripBlock;
    uint8_t gap1[4];
} tGeoTriangleChunks;

typedef struct sGeoUVElem
{
    float u;
    float v;
} tGeoUVElem;

typedef struct sGeoUVElemAsUInt
{
    uint32_t u;
    uint32_t v;
} tGeoUVElemAsUInt;

typedef struct sGeoMRM
{
    uint16_t* shorts;
    unsigned int endNum;
    int b;
    float f[16];
    uint16_t* ptr;
} tGeoMRM;

typedef struct sGeoSknElem
{
    Mtx4x4 mtx;
    uint16_t multiPartElemID;
    uint16_t num;
    int32_t* weightsAndIndicesPtr;
    uint8_t gap[8];
} tGeoSknElem;

typedef struct sGeoSknSub1
{
    struct sGeoSknElem* ptr;
} tGeoSknSub1;

typedef struct sGeoSkn
{
    int16_t s1;
    uint16_t num;
    struct sGeoSknSub1* ptr;
} tGeoSkn;

/*
typedef struct sGeoSub6
{
    int16_t triangleChunkID;
    uint16_t num;
    int16_t* triangleChunkElemIDs;
} tGeoSub6;

typedef struct sGeoSub5
{
    Vec3 v1;
    Vec3 v2;
    int num;
    struct sGeoSub6* ptr;
} tGeoSub5;


typedef struct sGeoCollisionMain
{
    int num;
    struct sGeoSub5* ptr;
    int unk2;
} tGeoCollisionMain;
*/
typedef struct sGEOGRO
{
    struct sGroIntf* intf;
    int usageCount;
    uint32_t vertexNum;
    Vec3* vertices;
    Vec3* normals;
    unsigned int* vertexColors; // lehet unsignedre kéne áttenni, + 3 int-et belefoglal még, és oda beletesz vmilyen pointereket, GFX_SetNumberOfSprites is rak bele gfx darabszamot
    uint32_t uvNum;
    struct sGeoUVElem* UVs;
    unsigned int triangleChunkCount;
    struct sGeoTriangleChunks* triangleChunks;
    struct sGameObjectBV* bv;
    struct sGeoMRM* mrm;
    struct sGeoSkn* skinning;
    int unkFlags;
    unsigned int spritesNum;
    struct sGeoSprites* spritesPtr;
    struct sCobOK3* collisionPtr; /* struct sGeoCollisionMain */
    /* the renderer's own, opaque to everything outside it */
    void* vertexBuffer;
    void* vertexBuffer2;
    uint8_t gap4b[4]; // ez mi??? a PC_FreeABackCache-nél ez a release ki van kommentezve mert nemtudom mi TODO FIXME
    int uvBufferNum;
    void** uvBuffers;
    void* indexBuffer;
} tGEOGRO;

void GEO_MRM_SetNumberOfPoints_Percent(struct sGEOGRO* geo, float quality);
void GEO_UseNormals(struct sGEOGRO* geogro);
void GEO_ComputeNormals(struct sGEOGRO* geo);
void GEO_SKN_ComputeNormals(struct sGEOGRO* geo, Vec3* vertices);
struct sGEOGRO* GEO_pst_Create(int vertexNum, int uvNum, int triangleChunkNum, int* vertexColors);
void GEO_AllocElementContent(struct sGeoTriangleChunks* tc);
void GEO_Free(struct sGroGeneric* geo);
int GEO_l_GetNumberOfTriangles(struct sGEOGRO* geo);
struct sGEOMAT* GEO_GetMaterial(struct sGEOMAT* mat, int idx);
void GEO_Init();