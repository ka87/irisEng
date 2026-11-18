#include "projection.h"
#include "gdi.h"
#include "cameraengine.h"

/* The matrix helpers are the renderer's, and the convention they work
   in is the one the engine's projection matrices are already in --
   row-major, as Direct3D has them. A second renderer consumes the
   result rather than needing a convention of its own. */
#include "matrixmath.h"

#include <math.h>
#include <string.h>

/* Documented in projection.h. */
void PRJ_OffCentre(Mtx4x4* out, float left, float right,
                   float bottom, float top, float nearZ, float farZ)
{
    double width;
    double height;
    float  depth;

    width = right - left;
    memset(out, 0, sizeof(*out));
    out->w = 1.0f;
    out->v1.x = (float)(2.0 / width);
    height = top - bottom;
    out->v2.y = (float)(2.0 / height);
    depth = farZ - nearZ;
    out->v3.z = (float)(-4.0 / depth);
    out->_m14 = (float)(-((left + right) / width));
    out->_m24 = (float)(-((bottom + top) / height));
    out->_m34 = (float)(-((nearZ + farZ) / depth));
}

void PRJ_Compute(struct sDDCamStruct* cam, const tPrj_Input* in,
                 tPrj_Output* out)
{
    int    ratioIdx;
    int    rectW;
    int    rectH;
    int    fitW;
    int    fitH;
    int    viewX;
    int    viewY;
    int    viewW;
    int    viewH;
    int    flags2;
    float  screenRatio;
    float  fitRatio;
    float  cotFov;
    float  isoSize;
    double negCot;
    double isoRatio;
    double invRatio;
    double negFar;
    double isoLeft;
    float  orthoW;
    float  orthoH;
    float  halfTexel;
    Mtx4x4 offsetMtx;
    Mtx4x4 orthoMtx;

    Mtx4x4* camMtx = &cam->projectionMtx;

    memset(out, 0, sizeof(*out));

    ratioIdx = in->selectedScreenRatio;
    if (ratioIdx <= 0 || ratioIdx >= 4)
        screenRatio = in->manualScreenRatio;
    else
        screenRatio = GDI_gaf_ScreenRatio[ratioIdx];
    if (in->widescreen)
        screenRatio = in->realYoverX;
    rectW = in->surfaceWidth;
    rectH = in->surfaceHeight;
    fitRatio = in->pixelYoverX * cam->YoverX * screenRatio;
    flags2 = in->flags2;
    if ((flags2 & 4) != 0)
    {
        fitH = rectH;
        fitW = (uint32_t)((double)rectH / fitRatio);
        if ((flags2 & 1) != 0 && fitW > rectW || (flags2 & 2) != 0 && fitW < rectW)
        {
            fitH = (uint32_t)((double)rectW * fitRatio);
            fitW = rectW;
        }
    }
    else
    {
        fitH = (uint32_t)((double)rectW * fitRatio);
        fitW = rectW;
        if ((flags2 & 1) != 0 && fitH > rectH || (flags2 & 2) != 0 && fitH < rectH)
        {
            fitW = (uint32_t)((double)rectH / fitRatio);
            fitH = rectH;
        }
    }
    if (cam->viewPortSizeWidth == 0.0)
    {
        cam->viewPortSizeWidth = 1.0;
        cam->viewPortSizeHeight = 1.0;
        cam->viewPortPosX = 0.0;
        cam->viewPortPosY = 0.0;
    }
    viewX = (int32_t)((double)((rectW - fitW) / 2) + (double)fitW * cam->viewPortPosX);
    viewY = (int32_t)((double)((rectH - fitH) / 2) + (double)fitH * cam->viewPortPosY);
    viewH = (uint32_t)((double)fitH * cam->viewPortSizeHeight);
    viewW = (uint32_t)((double)fitW * cam->viewPortSizeWidth);
    cam->viewPortRealPosX = viewX;
    cam->viewPortRealPosY = viewY;

    out->viewX = viewX;
    out->viewY = viewY;
    out->viewWidth = viewW;
    out->viewHeight = viewH;

    if ((cam->flags & 0x400) != 0)
    {
        /* Drawing into something whose viewport is already chosen, in
           its own pixels, with a half-texel nudge. */
        orthoW = (float)viewW;
        halfTexel = (float)(0.5 / orthoW);
        MTX_p_Translation((Mtx4x4WithoutAdditionalData*)&offsetMtx, halfTexel, halfTexel, 0.0);
        orthoH = (float)viewH;
        MTX_p_OrthoOffCenterRH((Mtx4x4WithoutAdditionalData*)&orthoMtx, 0.0, orthoW, 0.0, orthoH,
                                   0.0, 1.0);
        /* Into the camera's own matrix, which is where the renderer
           built it and where the engine reads it back from. Only the
           sixteen floats: the helpers leave the fields past them
           alone, and that is the shipped behaviour. */
        MTX_p_Multiply((Mtx4x4WithoutAdditionalData*)camMtx, (const Mtx4x4WithoutAdditionalData*)&orthoMtx,
                           (const Mtx4x4WithoutAdditionalData*)&offsetMtx);
        memcpy(&out->matrix, camMtx, sizeof(Mtx4x4WithoutAdditionalData));
        out->setViewport = 0;
        return;
    }

    out->setViewport = 1;

    if ((cam->flags & 1) != 0)
    {
        cotFov = (float)(1.0 / tan(cam->focale * 0.5));

        /* In place, because MATH_SetIdentityMatrix masks the matrix's
           own flag word rather than clearing it -- so which matrix it
           is given is not a detail. */
        MATH_SetIdentityMatrix(camMtx);
        if (in->widescreen)
        {
            /* the authored vertical half angle, kept whatever the
               window shape is; the horizontal one follows from it */
            float cotY = ((in->flags2 & 4) != 0)
                             ? cotFov
                             : (float)(cotFov / in->refYoverX);

            camMtx->v1.x = (float)(cotY * screenRatio);
            negCot = -cotY;
        }
        else if ((in->flags2 & 4) != 0)
        {
            camMtx->v1.x = cotFov * screenRatio;
            negCot = -cotFov;
        }
        else
        {
            camMtx->v1.x = cotFov;
            negCot = -(cotFov / screenRatio);
        }
        camMtx->v2.y = (float)negCot;
        camMtx->v3.z = (cam->nearPlane + cam->farPlane)
                       / (cam->farPlane - cam->nearPlane);
        camMtx->_m34 = 1.0;
        camMtx->Pos.z = -0.050000001f;
        camMtx->w = 0.0;
        memcpy(&out->matrix, camMtx, sizeof(Mtx4x4WithoutAdditionalData));
        return;
    }

    /* Isometric. The camera's matrix is set to the identity and then
       the projection is built somewhere else entirely -- so the
       camera is left holding an identity that is not the projection.
       That is what the renderer did, and something reads it. */
    MATH_SetIdentityMatrix(camMtx);
    isoSize = cam->isoZoom * cam->isoFactor;
    negFar = -cam->farPlane;
    if ((in->flags2 & 4) == 0)
    {
        isoRatio = in->widescreen ? in->realYoverX : in->manualScreenRatio;
        if (cam->isoFactor == 0.0)
        {
            PRJ_OffCentre(&out->matrix, 0.0, 1.0,
                            (float)((1.0 - isoRatio) * 0.5),
                            (float)((isoRatio + 1.0) * 0.5),
                            -cam->nearPlane, (float)negFar);
        }
        else
        {
            isoLeft = isoSize * -1.0;
            PRJ_OffCentre(&out->matrix, (float)isoLeft,
                            cam->isoZoom * cam->isoFactor,
                            (float)(isoSize * isoRatio),
                            (float)(-isoRatio * isoSize),
                            cam->farPlane, (float)negFar);
        }
    }
    else
    {
        invRatio = 1.0 / (in->widescreen ? in->realYoverX : in->manualScreenRatio);
        if (cam->isoFactor != 0.0)
        {
            isoLeft = -invRatio * isoSize;
            PRJ_OffCentre(&out->matrix, (float)isoLeft,
                            (float)(isoSize * invRatio),
                            (float)(isoSize * -1.0),
                            cam->isoZoom * cam->isoFactor,
                            cam->farPlane, (float)negFar);
        }
        else
        {
            PRJ_OffCentre(&out->matrix,
                            (float)((1.0 - invRatio) * 0.5),
                            (float)((invRatio + 1.0) * 0.5),
                            0.0, 1.0,
                            -cam->nearPlane, (float)negFar);
        }
    }
}
