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
    void (*viewFunctionPtr)(struct sViewElem* view);
    uint8_t gap1[12];
} tViewElem;

unsigned int WOR_View_Create(struct sWorld* world);
void WOR_View_Close(struct sViewElem *ve);
void WOR_View_SetViewport(struct sViewElem* view, float viewPortPosX, float viewPortPosY, float viewPortSizeWidth, float viewPortSizeHeight);

/* Sets a view's camera matrix, keeping the previous one. What
   WOR_View_FollowFather calls with the assigned object's absolute
   matrix, and what anything else driving a view calls instead. */
void WOR_View_SetViewPoint(struct sViewElem* view, Mtx4x4* viewPointMatrix, Vec3* unkVec, int unkInt, int onlyPositionSet);