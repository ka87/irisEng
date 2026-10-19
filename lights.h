#pragma once
#include "common.h"
#include "graphicsobjects/light.h"
#include "gameobject.h"

typedef struct sLightListStruct
{
    int maxLightsNum;
    int actualLightNum;
    struct sGameObjectMain** lights;
} tLightListStruct;

Mtx4x4 LIGHT_LightSourceMatrix;
Mtx4x4 LIGHT_gst_NIM;
tGEOGRO* LIGHT_gpst_CurObject;
Vec3* LIGHT_gpst_CurPointsBuffer;
struct sLIGHT* LIGHT_gpst_Cur;
unsigned int LIGHT_gul_Color;
int* LIGHT_gpul_CurrentTable;
int LIGHT_gaul_LinearTable[256];
int LIGHT_gaul_ExponantTable[256];
struct sGameObjectMain* LIGHT_gpst_Bump;

unsigned int LIGHT_ul_Interpol2Colors(int a1, int a2, float a3);
uint32_t COLOR_ul_Mul(uint32_t color, float mult);
void LIGHT_SubColor(unsigned int* a1, int a2);
void LIGHT_AddColor(unsigned int* a1, unsigned int a2);
int LIGHT_ul_MulColor(int a1, float a2);
void LIGHT_SendObjectToLight(struct sLightListStruct* lls, struct sGameObjectMain* gao);
void LIGHT_ComputeVertexColor(int vertexNum);
void LIGHT_ColorAdd(uint32_t colorToAdd, uint32_t* src, int num, uint32_t* dest);
void LIGHT_List_Init(tLightListStruct* a1, int a2);
void LIGHT_List_Reset(struct sLightListStruct* a1);
void LIGHT_List_Free(struct sLightListStruct* a1);
void LIGHT_List_AddLight(struct sLightListStruct* a1, struct sGameObjectMain* a2);
