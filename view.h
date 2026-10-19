#pragma once
#include "math.h"
#include "gameobject.h"
#include "gdi.h"
#include "common.h"

typedef struct sViewElem
{
    Mtx4x4 cameraMtx;
    Mtx4x4 prevViewPointMtx; // gets set in WOR_View_FollowFather
    struct sGameObjectMain* assignedGao;
    int flags;
    struct sGdi_DisplayData* displayData;
    float focale;
    float viewPortPosX;
    float viewPortPosY;
    float viewPortSizeWidth;
    float viewPortSizeHeight;
    void (*viewFunctionPtr)(struct sViewElem* a1);
    byte gap1[12];
} tViewElem;

unsigned int WOR_View_Create(struct sWorld* world);
void WOR_View_Close(struct sViewElem *ve);
void WOR_View_SetViewport(struct sViewElem* a1, float a2, float a3, float a4, float a5);