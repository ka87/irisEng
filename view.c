#include "memory.h"
#include "gameobject.h"
#include "gdi.h"
#include "view.h"

void WOR_View_SetViewPoint(tViewElem* cameraMtxtPlusViewPointMtx, Mtx4x4* viewPointMatrix, Vec3* unkVec, int unkInt, int onlyPositionSet)
{
    Mtx4x4* prevViewPointMtx;

    prevViewPointMtx = &cameraMtxtPlusViewPointMtx->prevViewPointMtx;
    if (onlyPositionSet)
    {
        if (prevViewPointMtx != &cameraMtxtPlusViewPointMtx->cameraMtx)
            memcpy(prevViewPointMtx, &cameraMtxtPlusViewPointMtx->cameraMtx, sizeof(Mtx4x4));
        cameraMtxtPlusViewPointMtx->cameraMtx.Pos = viewPointMatrix->Pos;
        cameraMtxtPlusViewPointMtx->cameraMtx.mtxflag |= 0x2;
    }
    else
    {
        if (prevViewPointMtx != &cameraMtxtPlusViewPointMtx->cameraMtx)
            memcpy(prevViewPointMtx, &cameraMtxtPlusViewPointMtx->cameraMtx, sizeof(Mtx4x4));
        if (&cameraMtxtPlusViewPointMtx->cameraMtx != viewPointMatrix)
            memcpy(&cameraMtxtPlusViewPointMtx->cameraMtx, viewPointMatrix, sizeof(Mtx4x4));
    }
}

void WOR_View_FollowFather(tViewElem* view)
{
    Mtx4x4 absMtx;

    if (view->assignedGao)
    {
        OBJ_MakeAbsoluteMatrix(view->assignedGao, &absMtx);
        WOR_View_SetViewPoint(view, &absMtx, &MATH_gst_NulVector, 1, 0);
    }
}

void WOR_View_Init(tViewElem* view, const void* cameraMtxToInit, tGameObjectMain* assignedGao)
{
    memset(view, 0, sizeof(tViewElem));

    view->flags |= 0x1;

    if (view != cameraMtxToInit)
        memcpy(&view->cameraMtx, cameraMtxToInit, sizeof(Mtx4x4));

    memcpy(&view->prevViewPointMtx, cameraMtxToInit, sizeof(view->prevViewPointMtx));
    
    view->assignedGao = assignedGao;
    view->viewFunctionPtr = WOR_View_FollowFather;
}

void WOR_View_SetFocale(tViewElem* view, float fov)
{
    view->focale = fov;
}

void WOR_View_SetViewport(struct sViewElem* view, float viewPortPosX, float viewPortPosY, float viewPortSizeWidth, float viewPortSizeHeight)
{
    view->viewPortPosX = viewPortPosX;
    view->viewPortPosY = viewPortPosY;
    view->viewPortSizeWidth = viewPortSizeWidth;
    view->viewPortSizeHeight = viewPortSizeHeight;
}

unsigned int WOR_View_Create(struct sWorld* world)
{
    int i;

    for (i = 0; i < world->viewNum; i++) {
        if (!world->viewPtr[i].viewFunctionPtr) break;
    }

    if (i >= world->viewNum)
    {
        i = world->viewNum;
        if (world->viewNum)
            world->viewPtr = (tViewElem*)MEM_p_Realloc(world->viewPtr, sizeof(tViewElem) * (world->viewNum + 1));
        else
            world->viewPtr = (tViewElem*)MEM_p_Alloc(sizeof(tViewElem));
        ++world->viewNum;
    }

    WOR_View_Init(&world->viewPtr[i], &MATH_gst_IdentityMatrix, 0);
    WOR_View_SetFocale(&world->viewPtr[i], 1.0f);
    WOR_View_SetViewport(&world->viewPtr[i], 0.0f, 0.0f, 1.0f, 1.0f);

    world->viewPtr[i].displayData = fbss;
    world->viewPtr[i].focale = world->viewPtr->focale;

    return i;
}

void WOR_View_Close(struct sViewElem* view)
{
}

