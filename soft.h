#pragma once
#include "math.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/geo.h"
#include "common.h"


typedef struct sZListElem
{
    struct sZListElem* currZElemGroup;
    void* data;
    int renderingFlags;
    struct sGEOMAT* geomat;
} tZListElem;

typedef struct sZListMeshElem
{
    Vec3 point;
    int vertexColor;
    struct sGeoUVElem uv;
} tZListMeshElem;

typedef struct sZListObjElem
{
    struct sGameObjectMain* gao;
    uint8_t gap[12];
    Mtx4x4 worldMtx;
    Mtx4x4 gaoMatrix;
} tZListObjElem;


typedef struct sZListElemDataStruct
{
    struct sZListElem elems[1024];
    struct sZListElem* currPtr;
    struct sZListMeshElem* currMeshOffset;
    struct sZListObjElem* currObjOffset;
    struct sZListMeshElem meshElems[16384];
    int _pad;
    struct sZListObjElem objElems[128];
} tZListElemDataStruct;

typedef struct sZlistStruct
{
    int minZ;
    int maxZ;
    struct sZListElem* zElemGroup[16384];
} tZlistStruct;

typedef struct sBigBufferStruct
{
    int flags;
    int solidVertexColor;
    int* vertexColorArray;
    int* vertexColorArraySecond;
    uint8_t data1LightColors[20000];
    uint8_t data2SpecularColors[20000];
    uint8_t data3FinalColorWithShadow[40000];
    uint8_t data4[60000];
    uint8_t data5[60000];
    uint8_t data6[180000];
    uint8_t data7[40000];
    uint8_t data8SpecularVectors[60000];
    uint8_t* UVData;
    int colorMask;
    uint8_t gap[104]; // ennek a kozepeig van memset a GDI_fnpst_CreateDisplayData-ban
    struct sZlistStruct zlists[8];
    uint8_t gap1[64];
    struct sZListElemDataStruct ZListElemData;
    uint8_t gapForTmpMem[44];
    int lastElem;
} tBigBufferStruct;

struct sBigBufferStruct SOFT_gpc_BigBufferOnStack;
struct sZlistStruct* p_Current_SOFT_gst_ZList;

extern tBigBufferStruct* SOFT_gpc_BigBuffer;
extern tZlistStruct* SOFT_gpc_BigBufferZlists;
extern tZListElemDataStruct* zListData;

tGEOGRO ZList_Obj;
tGeoTriangleChunks ZList_Element;
Vec3 ZList_Points[3];
tGeoUVElem ZListUVs[3];
int ZListVertexColors[4]; // lehet csak három kellene, de úgy emlékszem az elsõ int a daramszám
tGeoTriangleChunkElem ZListTriangleChunkElem;
BOOL ZList_Locked;
tGeoTriangleChunkElem* ZList_T;
Vec3* ZList_V;
tGeoUVElem* ZList_UVs;
int* ZList_RLIs;
tGEOMAT* ZList_Mat;
BOOL i_Identity;

void SOFT_Project(Vec3* dstVecs, Vec3* srcVecs, int verticesNum, tDDContainerStruct* cam);
void SOFT_TransformAndProject(Vec3* dstVecs, Vec3* srcVecs, int verticesNum, tDDCamStruct* cam);
void SOFT_ZList_Init();
void SOFT_ZList_Clear();
void SOFT_l_MatrixStack_Create(tMatrixStackStruct* mts, int num);
void SOFT_MatrixStack_Reset(tMatrixStackStruct* mts, Mtx4x4* mtx);
int SOFT_l_MatrixStack_Push(tMatrixStackStruct* mts, Mtx4x4* mtx);
int SOFT_l_MatrixStack_Pop(tMatrixStackStruct* mts);
struct sZListElem* SOFT_pst_ZList_AddNode(float z, int num, int isObject);
void SOFT_AddCurrentObjectInZList(struct sGdi_DisplayData* dd, struct sGameObjectMain* gao);
void SOFT_TransformInComputingBuffer(tBigBufferStruct* bigBuf, Vec3* vertices, int vertexNum, tDDContainerStruct* container);
Vec3* SOFT_pst_GetSpecularVectors();
void SOFT_pst_InitSpecularColors(int num);
void SOFT_l_MatrixStack_Destroy(struct sMatrixStackStruct* mts);
void SOFT_ZList_Lock();
void SOFT_ZList_Unlock();
void SOFT_ZList_Send();