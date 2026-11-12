#pragma once
#include "common.h"

typedef struct sGridPtrElem
{
    float fEval;
    int16_t wayX;
    int16_t wayY;
    uint8_t flags;
    uint8_t __pad[3];
} tGridPtrElem;

typedef struct sWorldGridStruct
{
    int* ptrInts;
    char** ptr;
    uint8_t* compressedGrid;
    char* capasPtr;
    tGridPtrElem* ptrs;
    char* notUsedSecondaryCapasPtr;
    float startX;
    float startY;
    float centerX;
    float centerY;
    int16_t gridCountX;
    int16_t gridCountY;
    int16_t s3Size7LimitMaxX;
    int16_t s4Size7LimitMaxY;
    int16_t s5Limit1MinX;
    int16_t s5Limit1MinY;
    char gridElemSize;
    uint8_t _gap[3];
    int i2;
    int iInSetCenter1;
    int iInSetCenter2;
    int iInSetCenter3;
    int iInSetCenter4;
} tWorldGridStruct;

typedef struct sGridGast
{
    int16_t i1;
    int16_t i2;
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

void GRI_Destroy(tWorldGridStruct* grid);
tWorldGridStruct* GRID_ul_CallbackWorldGrid(uint32_t Offset);
int GRI_i_GetCapa(tWorldGridStruct* grid, Vec3* pos);
void GRI_SetCapa(tWorldGridStruct* grid, Vec3* pos, char capability);
int GRI_AddCapa(tWorldGridStruct* grid, Vec3* pos, char capability);
int GRID_ShortestPath(struct sGameObjectMain* gao, struct sWorldGridStruct* ownGrid, struct sWorldGridStruct* colGrid, Vec3* start, Vec3* target, Vec3* pathOut, int unused, int nearLimit, float maxDist, int maxCells, int maxPoints);
int GRI_3Dto2D(tWorldGridStruct* grid, Vec3* pos, int* retPosX, int* retPosY);
void GRID_SetRealCenter(tWorldGridStruct* grid, int worldX, int worldY);
int GRID_3Dto2D(tWorldGridStruct* grid, Vec3* pos, int32_t* cellX, int32_t* cellY);
int GRI_Tag(tWorldGridStruct* grid, Vec3* pos, int shape, int capability);
int GRI_TagBox(struct sWorldGridStruct* grid, Vec3* pos, Vec3* to, char capability);
void GRI_Reinit(struct sWorldGridStruct* grid);
int GRI_SmoothWay(struct sWorldGridStruct* ownGrid, struct sWorldGridStruct* grid, Vec3* path, int num);
int GRI_TagVec(struct sWorldGridStruct* grid, Vec3* pos, int shape, int capability, Vec3* dir);
int GRI_i_ComputeDest(struct sWorldGridStruct* grid, Vec3* pos);
int GRI_TagDyn(struct sWorldGridStruct* grid, Vec3* pos, float distLimit, Vec3* cells, char capability);
BOOL GRI_Surround(tWorldGridStruct* grid, Vec3* pos, Vec3* sight);
int GRI_TagBox(struct sWorldGridStruct* grid, Vec3* pos, Vec3* to, char capability);