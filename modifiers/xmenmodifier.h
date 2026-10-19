#pragma once
#include "../graphicsobjects/geomaterial.h"
#include "../modifiers.h"

typedef struct sXmenMdfInner1
{
    Vec3 v1;
    Vec3 v2;
} tXmenMdfInner1;


typedef struct sXmenMdfInner2
{
    //struct sXmenMdfInner1 data[8];
    Vec3 data[16];
} tXmenMdfInner2;

typedef struct sXmenSize
{
    int multipartDataID;
    float size;
} tXmenSize;

typedef struct sXmenMdfStruct
{
    int flags;
    int num;
    int num2;
    byte gap1[4];
    int xmenType;
    int geoKey;
    struct sGEOMAT* geomat;
    struct sXmenSize* sizes;
    tGameObjectMain* gao;
    struct sXmenMdfInner2* ptr1;
    int* ptr2;
    float deathTime;
    float time;
    int id;
    int frameCounter;
} tXmenMdfStruct;

tGEOGRO XMEN_OBJECT;
tGeoTriangleChunks XMEN_ELEMENT[1];
tGeoTriangleChunkElem XMEN_OBJECT_TRIS[64];
tGeoUVElem XMEN_UVs[64];
Vec3 XMEN_OBJECT_Points_Smoothed[64];
tXmenMdfStruct* p_XMEN_STACK[96];
int g_ulFrameCounter;

void GAO_ModifierXMEN_DRAW_ALL();
void GAO_ModifierXMEN_Create(struct sGameObjectMain* gao, struct sModifier* a2, void* a3);
void GAO_ModifierXMEN_Destroy(struct sModifier* a1);
void GAO_ModifierXMEN_Apply(struct sModifier* a1, void* unk1);
int GAO_ModifierXMEN_Load(struct sModifier* arg0, char* a1);
void GAO_ModifierXMEN_Reinit(tModifier* a1);
void GAO_ModifierXMEN_Unapply(struct sModifier* a1, void* unk1);
void XMEN_OnOff(struct sGameObjectMain* a1, int a2, int onOff);
void XMEN_SetSize(struct sGameObjectMain* a1, int id, int multipartDataID, float size);