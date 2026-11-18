#include "d3dvideo.h"
#include "../gdi.h"

#include <string.h>

/* The picture the engine writes frames into, movie-sized and in
   system memory so it can be locked, and the texture it is copied
   into to be stretched onto the screen. */
static IDirect3DSurface9* d3d_videoPicture;
static IDirect3DTexture9* d3d_videoStageTex;
static IDirect3DSurface9* d3d_videoStage;

/* The picture's own size, and the screen's as it was when the
   display was made. */
static int d3d_videoWidth;
static int d3d_videoHeight;
static int d3d_videoScreenW;
static int d3d_videoScreenH;

void D3D_VideoDisplayClose(void)
{
    if (d3d_videoStage)
    {
        d3d_videoStage->lpVtbl->Release(d3d_videoStage);
        d3d_videoStage = NULL;
    }
    if (d3d_videoStageTex)
    {
        d3d_videoStageTex->lpVtbl->Release(d3d_videoStageTex);
        d3d_videoStageTex = NULL;
    }
    if (d3d_videoPicture)
    {
        d3d_videoPicture->lpVtbl->Release(d3d_videoPicture);
        d3d_videoPicture = NULL;
    }
    d3d_videoWidth = 0;
    d3d_videoHeight = 0;
}

int D3D_b_VideoDisplayCreate(int width, int height)
{
    IDirect3DSurface9* back;
    D3DSURFACE_DESC desc;
    D3DLOCKED_RECT lr;
    HRESULT hr;

    D3D_VideoDisplayClose();

    if (!D3DRenderParams.d3dDev || width <= 0 || height <= 0)
        return 0;

    /* The back buffer decides the format to match and the size to
       fit to. */
    if (FAILED(D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(
            D3DRenderParams.d3dDev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &back)))
        return 0;
    back->lpVtbl->GetDesc(back, &desc);
    back->lpVtbl->Release(back);

    d3d_videoScreenW = desc.Width;
    d3d_videoScreenH = desc.Height;

    if (FAILED(D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(
            D3DRenderParams.d3dDev, width, height, desc.Format,
            D3DPOOL_SYSTEMMEM, &d3d_videoPicture, NULL)))
    {
        d3d_videoPicture = NULL;
        return 0;
    }

    /* Black, so a frame that does not cover the picture shows black
       rather than whatever the memory held. */
    if (SUCCEEDED(d3d_videoPicture->lpVtbl->LockRect(d3d_videoPicture, &lr, NULL,
                                                     D3DLOCK_NOSYSLOCK)))
    {
        memset(lr.pBits, 0, (size_t)lr.Pitch * (size_t)height);
        d3d_videoPicture->lpVtbl->UnlockRect(d3d_videoPicture);
    }

    hr = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
        D3DRenderParams.d3dDev, width, height, 1, D3DUSAGE_RENDERTARGET,
        desc.Format, D3DPOOL_DEFAULT, &d3d_videoStageTex, NULL);
    if (FAILED(hr))
    {
        d3d_videoStageTex = NULL;
        D3D_VideoDisplayClose();
        return 0;
    }

    if (FAILED(d3d_videoStageTex->lpVtbl->GetSurfaceLevel(d3d_videoStageTex, 0,
                                                          &d3d_videoStage)))
    {
        d3d_videoStage = NULL;
        D3D_VideoDisplayClose();
        return 0;
    }

    d3d_videoWidth = width;
    d3d_videoHeight = height;
    return 1;
}

int D3D_b_VideoFrameLock(void** bits, int* pitch, int* height)
{
    D3DLOCKED_RECT lr;

    if (!d3d_videoPicture)
        return 0;
    if (FAILED(d3d_videoPicture->lpVtbl->LockRect(d3d_videoPicture, &lr, NULL,
                                                  D3DLOCK_NOSYSLOCK)))
        return 0;

    if (bits)
        *bits = lr.pBits;
    if (pitch)
        *pitch = lr.Pitch;
    /* The picture's own height, not the screen's. Whatever fills it
       clamps to what it is told, so telling it anything else is how
       the bottom of a frame goes missing. */
    if (height)
        *height = d3d_videoHeight;
    return 1;
}

void D3D_VideoFrameUnlock(void)
{
    if (d3d_videoPicture)
        d3d_videoPicture->lpVtbl->UnlockRect(d3d_videoPicture);
}

void D3D_VideoFramePresent(int width, int height, int fit)
{
    IDirect3DSurface9* back;
    RECT src;
    RECT dst;
    unsigned int scaledW;
    unsigned int scaledH;
    unsigned int left;
    unsigned int top;
    float scale;
    D3DTEXTUREFILTERTYPE filter;

    if (!d3d_videoPicture || !d3d_videoStage || !D3DRenderParams.d3dDev)
        return;
    if (width <= 0 || height <= 0)
        return;

    if (FAILED(D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(
            D3DRenderParams.d3dDev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &back)))
        return;

    D3DRenderParams.d3dDev->lpVtbl->UpdateSurface(D3DRenderParams.d3dDev,
                                                  d3d_videoPicture, NULL,
                                                  d3d_videoStage, NULL);

    switch (fit)
    {
    case GDI_VIDEOFIT_SMOOTH:
        scale = (float)d3d_videoScreenW / (float)width;
        filter = D3DTEXF_LINEAR;
        break;
    case GDI_VIDEOFIT_SHARP:
        scale = (float)d3d_videoScreenW / (float)width;
        filter = D3DTEXF_POINT;
        break;
    default:
        scale = 1.0f;
        filter = D3DTEXF_NONE;
        break;
    }

    src.left = 0;
    src.top = 0;
    src.right = width;
    src.bottom = height;

    scaledW = (unsigned int)((float)width * scale);
    left = (d3d_videoScreenW - scaledW) / 2;
    scaledH = (unsigned int)((float)height * scale);
    top = (d3d_videoScreenH - scaledH) / 2;
    dst.left = left;
    dst.top = top;
    dst.right = left + scaledW;
    dst.bottom = top + scaledH;

    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL,
                                          D3DCLEAR_TARGET, 0, 0.0f, 0);
    D3DRenderParams.d3dDev->lpVtbl->StretchRect(D3DRenderParams.d3dDev,
                                                d3d_videoStage, &src,
                                                back, &dst, filter);
    back->lpVtbl->Release(back);
}
