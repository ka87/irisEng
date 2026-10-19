#include "math.h"
#include "gdi.h"
#include "cameraengine.h"

int CAM_SetObjectMatrixFromCam(Mtx4x4* dest, Mtx4x4* src)
{
    Vec3 bak;

    MATH_GetRotationMatrix(dest, src);

    dest->Pos = src->Pos;
    dest->mtxflag |= 0x2;

    bak = dest->v3;

    dest->v3 = dest->v2;

    MATH_VecInvert(&dest->v3, &dest->v3);

    dest->v2 = bak;

    MATH_VecInvert(&dest->v1, &dest->v1);
    MATH_VecInvert(&dest->v2, &dest->v2);

    return MATH_l_CalculateCorrectType(dest);
}

int CAM_SetCameraMatrix(tDDContainerStruct* ddc, Mtx4x4* mtx)
{
    return CAM_SetObjectMatrixFromCam(&ddc->cam.objectMtx, mtx);
}

void CAM_Inverse(tDDContainerStruct* ddc)
{
    ddc->cam.cameraMtx._m34 = 0.0f;
    ddc->cam.cameraMtx._m24 = 0.0f;
    ddc->cam.cameraMtx._m14 = 0.0f;

    MATH_Transp33MatrixWithoutBuffer(&ddc->cam.cameraMtx, &ddc->cam.objectMtx);

    MATH_VecInvert(&ddc->cam.cameraMtx.Pos, &ddc->cam.objectMtx.Pos);
    MATH_TransformVector(&ddc->cam.cameraMtx.Pos, &ddc->cam.cameraMtx, &ddc->cam.cameraMtx.Pos);

    ddc->cam.cameraMtx.mtxflag = 0x6;
    ddc->cam.cameraMtx.w = 1.0f;
}

void CAM_2Dto3D(struct sDDCamStruct* cam, Vec3* dest, Vec3* src)
{
    Vec3 tmpVec;

    if ((cam->flags & 1) != 0)
    {
        tmpVec.x = (src->x - cam->centerX) * (src->z / cam->factorX);
        tmpVec.y = (src->y - cam->centerY) * (src->z / cam->factorY);
        tmpVec.z = src->z;
    }
    else
    {
        tmpVec.x = (src->x - cam->centerX) / cam->factorX;
        tmpVec.y = (src->y - cam->centerY) / cam->factorY;
        tmpVec.z = src->z;
    }
    MATH_TransformVertex(dest, &cam->objectMtx, &tmpVec);
}

void CAM_2Dto3DCamera(struct sDDCamStruct* cam, Vec3* cam3D, Vec3* cam2D)
{
    if ((cam->flags & 1) != 0)
    {
        cam3D->x = (cam2D->x - cam->centerX) * (cam2D->z / cam->factorX);
        cam3D->y = (cam->centerY - cam2D->y) * (cam2D->z / cam->factorY);
        cam3D->z = cam2D->z;
    }
    else
    {
        cam3D->x = (cam2D->x - cam->centerX) / cam->factorX;
        cam3D->y = (cam->centerY - cam2D->y) / cam->factorY;
        cam3D->z = cam2D->z;
    }
}

void CAM_2Dto3DCamera2(struct sDDCamStruct* cam, Vec3* cam3D, Vec3* cam2D)
{
    if ((cam->flags & 1) != 0)
    {
        cam3D->x = (cam->viewPortRealPosX + cam2D->x - cam->centerX) * (cam2D->z / cam->factorX);
        cam3D->y = (cam->centerY - (cam->viewPortRealPosY + cam2D->y)) * (cam2D->z / cam->factorY);
        cam3D->z = cam2D->z;
    }
    else
    {
        cam3D->x = (cam2D->x - cam->centerX) / cam->factorX;
        cam3D->y = (cam->centerY - cam2D->y) / cam->factorY;
        cam3D->z = cam2D->z;
    }
}

void CAM_AssignCameraToDevice(tDDContainerStruct2ForIDA* srcdd, tDDContainerStruct* dstdd, int winWidth, int winHeight)
{
    float screenRatio;
    int calculatedWidth, calculatedHeight;
    float finalHalfWidth, finalHalfHeight;
    float tmp;
    float factor;

    //if (winWidth / (float)winHeight == (16.0f/9.0f)) {
    //    srcdd->selectedScreenRatio = 3;
    //    tweak_ae_nobands = 1;
    //}

    if ((dstdd->cam.flags & 1) != 0)
    {
        if (srcdd->selectedScreenRatio <= 0 || srcdd->selectedScreenRatio >= 4)
            screenRatio = srcdd->manualScreenRatio;
        else
            screenRatio = GDI_gaf_ScreenRatio[srcdd->selectedScreenRatio];
    }
    else
    {
        screenRatio = GDI_gaf_ScreenRatio[1];
    }

    factor = srcdd->pixelYoverX * dstdd->cam.YoverX * screenRatio;

    if ((srcdd->flags2 & 4) != 0)
    {
        calculatedHeight = winHeight;
        calculatedWidth = (unsigned int)(winHeight / factor);
        if ((srcdd->flags2 & 1) != 0 && (int)calculatedWidth > winWidth || (srcdd->flags2 & 0x2) != 0 && (int)calculatedWidth < winWidth)
        {
            calculatedHeight = (unsigned int)(winWidth * factor);
            calculatedWidth = winWidth;
        }
    }
    else
    {
        calculatedHeight = (unsigned int)(winWidth * factor);
        calculatedWidth = winWidth;
        if ((srcdd->flags2 & 1) != 0 && (int)calculatedHeight > winHeight || (srcdd->flags2 & 0x2) != 0 && (int)calculatedHeight < winHeight)
        {
            calculatedWidth = (unsigned int)(winHeight / factor);
            calculatedHeight = winHeight;
        }
    }

    finalHalfHeight = (float)((int)(calculatedHeight * dstdd->cam.viewPortSizeHeight) / 2);
    finalHalfWidth = (float)((int)(calculatedWidth * dstdd->cam.viewPortSizeWidth) / 2);
    dstdd->cam.width = finalHalfWidth + finalHalfWidth;
    dstdd->cam.height = finalHalfHeight + finalHalfHeight;

    if ((dstdd->cam.flags & 0x1) == 0)
    {
        tmp = dstdd->cam.isoZoom * dstdd->cam.isoFactor;
        if ((srcdd->flags2 & 0x4) != 0)
        {
            dstdd->cam.factorX = finalHalfWidth * screenRatio / tmp;
            dstdd->cam.factorY = finalHalfHeight / tmp;
            dstdd->cam.centerX = (float)(winWidth / 2);
            dstdd->cam.centerY = (float)(winHeight / 2);
            return;
        }
        dstdd->cam.factorX = (float)(finalHalfWidth / tmp);
        dstdd->cam.factorY = (float)(finalHalfHeight / (tmp * screenRatio));
        dstdd->cam.centerX = (float)(winWidth / 2);
        dstdd->cam.centerY = (float)(winHeight / 2);
        return;
    }

    if ((srcdd->flags2 & 0x4) == 0)
    {
        tmp = (float)tan(dstdd->cam.focale * 0.5f);

        dstdd->cam.factorX = (float)(finalHalfWidth / tmp);
        dstdd->cam.factorY = (float)(finalHalfHeight / (tmp * screenRatio));
        dstdd->cam.centerX = (float)(winWidth / 2);
        dstdd->cam.centerY = (float)(winHeight / 2);
        return;
    }
    
    tmp = (float)tan(dstdd->cam.focale * 0.5f);

    dstdd->cam.factorX = (float)(finalHalfWidth * screenRatio / tmp);
    dstdd->cam.factorY = finalHalfHeight / tmp;
    dstdd->cam.centerX = (float)(winWidth / 2);
    dstdd->cam.centerY = (float)(winHeight / 2);
}

void CAM_Engine_ComputePlans(tDDContainerStruct* dd)
{
    float tmp;
    float halfFOV;

    if ((dd->cam.flags & 1) != 0)
    {
        halfFOV = dd->cam.focale * 0.5f;
        tmp = MATH_GetCos(halfFOV);

        dd->cam.planeLeft.x = tmp;
        dd->cam.planeRight.x = -tmp;
        dd->cam.planeLeft.y = 0.0f;
        dd->cam.planeRight.y = 0.0f;

        tmp = MATH_GetSin(halfFOV);

        dd->cam.planeLeft.z = tmp;
        dd->cam.planeRight.z = tmp;
        dd->cam.planeUp.x = 0.0f;
        dd->cam.planeDown.x = 0.0f;

        tmp = MATH_GetCos(halfFOV);

        dd->cam.planeUp.y = tmp;
        dd->cam.planeDown.y = -tmp;

        tmp = MATH_GetSin(halfFOV);

        dd->cam.planeUp.z = tmp;
        dd->cam.planeDown.z = tmp;
    }
    else
    {
        dd->cam.planeLeft.x = 1.0f;
        dd->cam.planeLeft.y = 0.0f;
        dd->cam.planeLeft.z = 0.0f;
        dd->cam.planeRight.x = -1.0f;
        dd->cam.planeRight.y = 0.0f;
        dd->cam.planeRight.z = 0.0f;
        dd->cam.planeUp.x = 0.0f;
        dd->cam.planeUp.y = 1.0f;
        dd->cam.planeUp.z = 0.0f;
        dd->cam.planeDown.x = 0.0f;
        dd->cam.planeDown.y = -1.0f;
        dd->cam.planeDown.z = 0.0f;
    }
}

void CAM_Engine_Init(tDDContainerStruct* dd)
{
    float factor;

    MATH_SetIdentityMatrix(&dd->cam.objectMtx);
    MATH_SetIdentityMatrix(&dd->cam.cameraMtx);
    MATH_SetIdentityMatrix(&dd->cam.projectionMtx);

    dd->cam.objectMtx.v2.y = 0.0f;
    dd->cam.objectMtx.v3.z = 0.0f;
    dd->cam.objectMtx.v2.z = -1.0f;
    dd->cam.objectMtx.v3.y = 1.0f;

    dd->cam.objectMtx.mtxflag |= 0x2;
    dd->cam.objectMtx.mtxflag |= 0x4;
    dd->cam.objectMtx.Pos.y = -20.0f;
    dd->cam.objectMtx.Pos.z = 2.0f;
    dd->cam.flags = 0x1;
    dd->cam.nearPlane = 1.0f;
    dd->cam.farPlane = 1000000.0f;
    dd->cam.focale = MATH_f_PiBy2;

    factor = (float)(1.0f / tan(0.7853981852531433)); //MATH_f_PiBy4
    
    dd->cam.YoverX = 1.0f;
    dd->cam.isoFactor = 10.0f;
    dd->cam.isoZoom = 1.0f;
    dd->cam.factorX = factor;
    dd->cam.factorY = factor;
    CAM_Engine_ComputePlans(dd);
}