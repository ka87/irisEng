#pragma once
#include "common.h"

typedef struct sGridPtrElem
{
    float fEval;
    __int16 wayX;
    __int16 wayY;
    byte flags;
    byte __pad[3];
} tGridPtrElem;

typedef struct sWorldGridStruct
{
    int* ptrInts;
    char** ptr;
    byte* compressedGrid;
    char* capasPtr;
    tGridPtrElem* ptrs;
    char* notUsedSecondaryCapasPtr;
    float startX;
    float startY;
    float centerX;
    float centerY;
    __int16 gridCountX;
    __int16 gridCountY;
    __int16 s3Size7LimitMaxX;
    __int16 s4Size7LimitMaxY;
    __int16 s5Limit1MinX;
    __int16 s5Limit1MinY;
    char gridElemSize;
    byte _gap[3];
    int i2;
    int iInSetCenter1;
    int iInSetCenter2;
    int iInSetCenter3;
    int iInSetCenter4;
} tWorldGridStruct;

typedef struct sGridGast
{
    __int16 i1;
    __int16 i2;
} tGridGast;

typedef struct sGridGast2
{
    struct sGridPtrElem* ptrElems;
    struct sGridGast gridShorts;
} tGridGast2;

tGridGast2 GRID_gast_Best[1000];
tGridGast GRID_gast_Explored[1000];

int GRID_gi_Mode;
int GRID_gi_Current;
int GRID_gi_Dest;

extern int GRID_gi_TestWall;

void GRI_Destroy(tWorldGridStruct* a1);
tWorldGridStruct* GRID_ul_CallbackWorldGrid(uint32_t Offset);
int GRI_i_GetCapa(tWorldGridStruct* a1, Vec3* a2);
void GRI_SetCapa(tWorldGridStruct* a1, Vec3* a2, char a3);
int GRI_AddCapa(tWorldGridStruct* a1, Vec3* a2, char a3);
int GRID_ShortestPath(struct sGameObjectMain* gao, struct sWorldGridStruct* a2, struct sWorldGridStruct* a3, Vec3* a4, Vec3* a5, Vec3* a6, int a7, int a8, float a9, int a10, int a11);
int GRI_3Dto2D(tWorldGridStruct* a1, Vec3* pos, int* retPosX, int* retPosY);
void GRID_SetRealCenter(tWorldGridStruct* a1, int a2, int a3);
int GRID_3Dto2D(tWorldGridStruct* a1, Vec3* a2, int32_t* a3, int32_t* a4);
int GRI_Tag(tWorldGridStruct* a1, Vec3* a2, int a3, int a4);
int GRI_TagBox(tWorldGridStruct* a1, Vec3* pos, Vec3* a3, char a4);
void GRI_Reinit(struct sWorldGridStruct* a1);
int GRI_SmoothWay(struct sWorldGridStruct* a1, struct sWorldGridStruct* a2, Vec3* a3, int a4);
int GRI_TagVec(struct sWorldGridStruct* a1, Vec3* pos, int a3, int a4, Vec3* a5);
int GRI_i_ComputeDest(struct sWorldGridStruct* a1, Vec3* pos);
int GRI_TagDyn(struct sWorldGridStruct* a1, Vec3* a2, float a3, Vec3* a4, char a5);
BOOL GRI_Surround(struct sWorldGridStruct* a1, Vec3* a2, Vec3* a3);
int GRI_TagBox(struct sWorldGridStruct* a1, Vec3* pos, Vec3* a3, char a4);