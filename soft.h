#pragma once
#include "math.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/geo.h"
#include "common.h"

//               // (*((int32_t *)(&(v6->z))))

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
    byte gap[12];
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
    byte data1LightColors[20000];
    byte data2SpecularColors[20000];
    byte data3FinalColorWithShadow[40000];
    byte data4[60000];
    byte data5[60000];
    byte data6[180000];
    byte data7[40000];
    byte data8SpecularVectors[60000];
    byte* UVData;
    int colorMask;
    byte gap[104]; // ennek a kozepeig van memset a GDI_fnpst_CreateDisplayData-ban
    struct sZlistStruct zlists[8];
    byte gap1[64];
    struct sZListElemDataStruct ZListElemData;
    byte gapForTmpMem[44];
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

void SOFT_Project(Vec3* a1, Vec3* a2, int a3, tDDContainerStruct* a4);
void SOFT_TransformAndProject(Vec3* a1, Vec3* a2, int idx, tDDCamStruct* a4);
void SOFT_ZList_Init();
void SOFT_ZList_Clear();
void SOFT_l_MatrixStack_Create(tMatrixStackStruct* mts, int num);
void SOFT_MatrixStack_Reset(tMatrixStackStruct* a1, Mtx4x4* a2);
int SOFT_l_MatrixStack_Push(tMatrixStackStruct* a1, Mtx4x4* a2);
int SOFT_l_MatrixStack_Pop(tMatrixStackStruct* a1);
struct sZListElem* SOFT_pst_ZList_AddNode(float a1, int num, int isObject);
void SOFT_AddCurrentObjectInZList(struct sGdi_DisplayData* a1, struct sGameObjectMain* gao);
void SOFT_TransformInComputingBuffer(tBigBufferStruct* a1, Vec3* vertices, int vertexNum, tDDContainerStruct* a4);
Vec3* SOFT_pst_GetSpecularVectors();
void SOFT_pst_InitSpecularColors(int a1);
void SOFT_l_MatrixStack_Destroy(struct sMatrixStackStruct* a1);
void SOFT_ZList_Lock();
void SOFT_ZList_Unlock();
void SOFT_ZList_Send();