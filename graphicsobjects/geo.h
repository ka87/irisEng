#pragma once
#include <d3d9.h>
#include "gro_common.h"
#include "../common.h"

typedef struct sGeoTriangleStripElem
{
    unsigned __int16 triangleIdx;
    unsigned __int16 uvIdx;
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
    byte gap1[4];
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
    byte gap[8];
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
    __int16 triangleChunkID;
    unsigned __int16 num;
    __int16* triangleChunkElemIDs;
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
    IDirect3DVertexBuffer9* d3dvb;
    IDirect3DVertexBuffer9* d3dvb2;
    byte gap4b[4]; // ez mi??? a PC_FreeABackCache-nél ez a release ki van kommentezve mert nemtudom mi TODO FIXME
    int d3dUVVBNum;
    IDirect3DVertexBuffer9** d3dUVVB;
    IDirect3DIndexBuffer9* d3dIndexBuf;
} tGEOGRO;

void GEO_MRM_SetNumberOfPoints_Percent(struct sGEOGRO* a1, float a2);
void GEO_UseNormals(struct sGEOGRO* a1);
void GEO_ComputeNormals(struct sGEOGRO* a1);
void GEO_SKN_ComputeNormals(struct sGEOGRO* a1, Vec3* a2);
struct sGEOGRO* GEO_pst_Create(int a1, int a2, int a3, int* a4);
void GEO_AllocElementContent(struct sGeoTriangleChunks* a1);
void GEO_Free(struct sGroGeneric* a1);
int GEO_l_GetNumberOfTriangles(struct sGEOGRO* a1);
struct sGEOMAT* GEO_GetMaterial(struct sGEOMAT* a1, int a2);
void GEO_Init();