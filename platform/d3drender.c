#include "../profiler.h"
#include "../perfbars.h"
#include "../pngsave.h"
#include "../datapath.h"
#include "../time.h"
#include "../memory.h"
#include "../textures.h"
#include "../dbg.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/staticlod.h"
#include "../water.h"
#include "../watermesh.h"
#include "../waterreflect.h"
#include "../engine.h"
#include "inputbackend.h"
#include "../resource.h"
#include "d3drender.h"
#include "syscompat.h"
#include "../settings.h"
#include "../platform.h"
#include "../font.h"
#include "d3dmath.h"
#include "d3dtex.h"
#include "../fog.h"
#include "../projection.h"
#include "../geovertex.h"
#include "../geosprite.h"
#include "overlay.h"
#include "../editorcamera.h"
#include "../profiler.h"

D3DMATRIX d3dProjectionMtx = { 1.4616959f, 0.0, 0.0, 0.0,
                               0.0, -1.9489279f, 0.0, 0.0,
                               0.0, 0.0, 1.000002f, 1.0, 0.0,
                               0.0, -0.050000001f, 0.0 };

D3DMATRIX d3dTransformMatrix = { 1.0, 0.0, 0.0, 0.0,
                                 0.0, 1.0, 0.0, 0.0,
                                 0.0, 0.0, 1.0, 0.0,
                                 0.0, 0.0, 0.0, 1.0 };

D3DMATRIX d3dTransformMatrix2 = { 1.0, 0.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0 };

int engineFogTableModeToD3D[4] = { 3, 1, 2, 0 };

tRasterNames rasterNames[] = {
    {.name = "MisEng", .unk1 = 1},
    {.name = "Lig", .unk1 = 2},
    {.name = "SKN", .unk1 = 2},
    {.name = "SDW", .unk1 = 2},
    {.name = "DRW", .unk1 = 2},
    {.name = "ANIr", .unk1 = 2},
    {.name = "SRS", .unk1 = 2},
    {.name = "GFX", .unk1 = 7},
    {.name = "CVC", .unk1 = 2},
    {.name = "CSpec", .unk1 = 2},
    {.name = "CDAlp", .unk1 = 1},
    {.name = "ANI", .unk1 = 1},
    {.name = "MAT D", .unk1 = 2},
    {.name = "Zlst S", .unk1 = 2},
    {.name = "AI", .unk1 = 1},
    {.name = "INO", .unk1 = 1},
    {.name = "Col", .unk1 = 1},
    {.name = "Rec", .unk1 = 1},
    {.name = "Eng", .unk1 = 5},
    {.name = "Dis", .unk1 = 6},
    {.name = "TxProc", .unk1 = 2},
    {.name = "Flip", .unk1 = 2},
    {.name = "MRM", .unk1 = 2},
    {.name = "1 - DRW", .unk1 = 2},
    {.name = "GO - DRW", .unk1 = 2},
    {.name = "SND", .unk1 = 1},
    {.name = "TAM", .unk1 = 1},
    {.name = "EngRei", .unk1 = 1},
    {.name = "WorAct", .unk1 = 1},
    {.name = "EOT", .unk1 = 1},
    {.name = "SnP", .unk1 = 1},
    {.name = "MdF", .unk1 = 1},
    {.name = "EVE", .unk1 = 1},
    {.name = "HIE", .unk1 = 1},
    {.name = "DYN", .unk1 = 1},
    {.name = "VIEW", .unk1 = 7},
    {.name = "Visi", .unk1 = 7},
    {.name = "Lens F", .unk1 = 2},
    {.name = "FIFO", .unk1 = 6},
    {.name = "AI - SND", .unk1 = 1},
    {.name = "IOP", .unk1 = 1},
    {.name = "VIDEO", .unk1 = 2},
    {.name = "SPG", .unk1 = 2},
    {.name = "BigVB", .unk1 = 2},
    {.name = "GoodVB", .unk1 = 2},
    {.name = "IndP", .unk1 = 2},
    {.name = "Sprite", .unk1 = 2},
    {.name = "Raster", .unk1 = 4},
    {.name = "Water", .unk1 = 2},
    {.name = "?????", .unk1 = 2},
    {.name = "?????", .unk1 = 2},
    {.name = "?????", .unk1 = 2},
    {.name = "Morph", .unk1 = 2},
    {.name = "Total", .unk1 = 7}
};

long long qword_990B48; // rasters

tWorld* worldPtrIfMultiStream = NULL; // TODO FIXME game doesn't set it explicitly to NULL

/* Each of these six is the file's one way of setting a piece of
   device state, so the check for a device belongs here rather than
   at every call site. It can only ever fire when this renderer was
   not the one chosen -- with a device the behaviour is unchanged,
   which is why the differential comparison cannot see it. */
static void D3D_RenderStateSet(int* current, DWORD state, int value)
{
    if (!D3DRenderParams.d3dDev)
        return;
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, state, (DWORD)(unsigned int)value);
    }
}

static void D3D_RenderStateSetF(float* current, DWORD state, float value)
{
    if (!D3DRenderParams.d3dDev)
        return;
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, state, *(DWORD*)current);
    }
}

static void D3D_StageStateSet(int stage, int* current, DWORD state, int value)
{
    if (!D3DRenderParams.d3dDev)
        return;
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, state, (DWORD)(unsigned int)value);
    }
}

static void D3D_StageStateSetF(int stage, float* current, DWORD state, float value)
{
    if (!D3DRenderParams.d3dDev)
        return;
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, state, *(DWORD*)current);
    }
}

static void D3D_SamplerStateSet(int sampler, int* current, DWORD state, int value)
{
    if (!D3DRenderParams.d3dDev)
        return;
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, sampler, state, (DWORD)(unsigned int)value);
    }
}

static void D3D_SamplerStateSetF(int sampler, float* current, DWORD state, float value)
{
    if (!D3DRenderParams.d3dDev)
        return;
    if (*current != value)
    {
        *current = value;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, sampler, state, *(DWORD*)current);
    }
}

/* The drawing state the engine asks for by meaning, as the interface
   in tGdi_DisplayData carries it. Each one is the cached setter the
   rest of this file already uses, so a value that is already in
   force costs nothing and the device sees exactly the calls it saw
   when these were written out at the call sites. */
/* What this renderer can do. All four are worked out at device
   creation from d3dcaps and can be forced from the tweak console, so
   they are read out of the params each time rather than answered
   from anything cached here. */
/* The size the renderer is actually drawing at, taken from the rect
   it set up rather than from the window, because a device does not
   always get the size it asked for. */
/* The tweak console's way in: the same three flags, written. They
   are worked out from the device at startup and these let that be
   overridden from the console, which is the whole reason they are
   tweaks at all. */
void D3D_SetCanDiffuse2x(int on)
{
    D3DRenderParams.tweak_ae_d2x = on;
}

void D3D_SetWantsNoFlare(int on)
{
    D3DRenderParams.tweak_ae_noflare = on;
}

void D3D_SetNoZonAlpha(int on)
{
    D3DRenderParams.tweak_zonalpha = on;
}

void D3D_GetDrawingSize(int* width, int* height)
{
    if (width)
        *width = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
    if (height)
        *height = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
}

int D3D_b_CanDiffuse2x(void)
{
    return D3DRenderParams.tweak_ae_d2x != 0;
}

int D3D_b_WantsNoFlare(void)
{
    return D3DRenderParams.tweak_ae_noflare != 0;
}

int D3D_b_NoZonAlpha(void)
{
    return D3DRenderParams.tweak_zonalpha != 0;
}

int D3D_b_DepthBlurOn(void)
{
    return D3DRenderParams.isDepthBlurOn != 0;
}

void D3D_SetZEnable(int on)
{
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, on);
}

int D3D_i_ZEnable(void)
{
    return renderStateConfig.d3d_zEnable;
}

void D3D_SetZWriteEnable(int on)
{
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE,
                       on);
}

/* The engine's GDI_UVSRC_ and GDI_UVXF_ turned into Direct3D's own.
   The translation is here and not in the engine because Direct3D's
   numbers are not the same in a real d3d9.h as in the stub the tests
   build against. */
void D3D_SetTexCoordIndex(int stage, int source)
{
    int index = source == GDI_UVSRC_NORMAL   ? D3DTSS_TCI_CAMERASPACENORMAL
              : source == GDI_UVSRC_POSITION ? D3DTSS_TCI_CAMERASPACEPOSITION
              : 0;

    D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTexCoordIndex,
                      D3DTSS_TEXCOORDINDEX, index);
}

void D3D_SetTextureTransformFlags(int stage, int transform)
{
    D3D_StageStateSet(stage,
                      &textureStageTypeConfig[stage].d3dTextureTransformFlags,
                      D3DTSS_TEXTURETRANSFORMFLAGS,
                      transform == GDI_UVXF_2D ? D3DTTFF_COUNT2 : 0);
}

/* The transform itself. Uncached, as it always was: the engine only
   sets it when it has worked out a new one. */
void D3D_SetTexCoordMatrix(int stage, const Mtx4x4* matrix)
{
    (void)stage;
    if (matrix && D3DRenderParams.d3dDev)
        D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev,
                                                     D3DTS_TEXTURE0,
                                                     (D3DMATRIX*)matrix);
}

static void D3D_SetFixedFunctionFVF(int fvf)
{
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = fvf;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
}

void PC_SetUVMatrix(Mtx4x4* dest, Vec3* axisU, Vec3* axisV, Vec3* axisW, Vec3* pos, Vec3* scale, int unused, int flags)
{
    dest->v1 = *axisU;
    dest->v2 = *axisV;
    dest->v3 = *axisW;
    dest->_m14 = scale->x;
    dest->_m24 = scale->y;
    dest->_m34 = scale->z;
    dest->Pos = *pos;
    dest->w = 1.0f;
    dest->mtxflag = flags;
}


// nearZ and farZ, not near and far: windef.h defines both of those as
// empty macros, so a parameter of either name vanishes
/* The arithmetic lives in projection.c now, shared with the camera
   fitting that uses the same projection; this is the renderer's half
   of it. */
void D3D_CalcAndSetProjection(float left, float right, float bottom, float top,
                              float nearZ, float farZ)
{
    Mtx4x4 mtx;

    PRJ_OffCentre(&mtx, left, right, bottom, top, nearZ, farZ);
    memcpy(&d3dProjectionMtx, &mtx, sizeof(D3DMATRIX));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev,
        D3DTS_PROJECTION, (D3DMATRIX*)&mtx);
}

void D3D_SetViewPort(int x, int y, int w, int h)
{
    int clampedX;
    int clampedY;
    int clampedW;
    int clampedH;

    if (D3DRenderParams.d3dDev)
    {
        clampedX = x;
        if (x < 0)
            clampedX = 0;
        clampedY = y;
        if (y < 0)
            clampedY = 0;
        clampedW = w;
        if (clampedX + w > (int)D3DRenderParams.presentParams.BackBufferWidth)
            clampedW = D3DRenderParams.presentParams.BackBufferWidth - clampedX;
        clampedH = h;
        if (clampedY + h > (int)D3DRenderParams.presentParams.BackBufferHeight)
            clampedH = D3DRenderParams.presentParams.BackBufferHeight - clampedY;
        D3DRenderParams.d3dViewPort.X = clampedX;
        D3DRenderParams.d3dViewPort.Y = clampedY;
        D3DRenderParams.d3dViewPort.Width = clampedW;
        D3DRenderParams.d3dViewPort.Height = clampedH;
        D3DRenderParams.d3dViewPort.MinZ = 0.0;
        D3DRenderParams.d3dViewPort.MaxZ = 1.0;
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, (&D3DRenderParams.d3dViewPort));
    }
}

int   D3D_gi_FlatLayerBox = 1;
int   D3D_gi_AspectTrace = 0;
float D3D_gf_CutsceneBand = 0.10416666f;

float D3D_RealYoverX(void)
{
    // the drawing area as the projection sees it, falling back to
    // the back buffer before the window rectangle is known
    float w = (float)(D3DRenderParams.Rect.right - D3DRenderParams.Rect.left);
    float h = (float)(D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top);

    if (w <= 0.0f || h <= 0.0f)
    {
        w = (float)D3DRenderParams.presentParams.BackBufferWidth;
        h = (float)D3DRenderParams.presentParams.BackBufferHeight;
    }
    if (w <= 0.0f || h <= 0.0f)
        return GDI_gf_RefYoverX;
    return h / w;
}

int D3D_FlatLayerWidth(int w, int h)
{
    int boxW;

    if (!D3D_gi_FlatLayerBox || !GDI_gi_Widescreen
        || GDI_gf_RefYoverX <= 0.0f || h <= 0)
        return w;
    boxW = (int)((float)h / GDI_gf_RefYoverX);
    return boxW < w ? boxW : w;
}

float D3D_FlatLayerToLayoutX(float windowFraction, int w, int h)
{
    int flat;

    flat = D3D_FlatLayerWidth(w, h);
    if (flat <= 0)
        return windowFraction;
    return (windowFraction * (float)w - (float)D3D_FlatLayerOffsetX(w, h)) / (float)flat;
}

float D3D_FlatLayerFromLayoutX(float layoutFraction, int w, int h)
{
    if (w <= 0)
        return layoutFraction;
    return ((float)D3D_FlatLayerOffsetX(w, h)
            + layoutFraction * (float)D3D_FlatLayerWidth(w, h)) / (float)w;
}

int D3D_FlatLayerOffsetX(int w, int h)
{
    return (w - D3D_FlatLayerWidth(w, h)) / 2;
}

void D3D_GetBandFractions(float* top, float* bottom)
{
    *top = D3D_gf_CutsceneBand;
    *bottom = 1.0f - D3D_gf_CutsceneBand;
}

void D3D_AspectTrace(const char* where, int viewX, int viewY, int viewW, int viewH)
{
    float t;
    float b;

    if (!D3D_gi_AspectTrace)
        return;
    D3D_GetBandFractions(&t, &b);
    GDI_OutputDebugString(
        "[aspect] %s back %dx%d rect %dx%d view %d,%d %dx%d "
        "YoverX %.4f bands t%.4f b%.4f cin %d\n",
        where,
        (int)D3DRenderParams.presentParams.BackBufferWidth,
        (int)D3DRenderParams.presentParams.BackBufferHeight,
        (int)(D3DRenderParams.Rect.right - D3DRenderParams.Rect.left),
        (int)(D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top),
        viewX, viewY, viewW, viewH,
        D3D_RealYoverX(), t, b, CIN_Bands);
    if (gpst_DX_stDD)
        GDI_OutputDebugString(
            "[aspect] %s cam %.1fx%.1f centre %.1f,%.1f factor %.2f,%.2f "
            "flat %d glyphscale x%.4f y%.4f\n",
            where,
            gpst_DX_stDD->ct.cam.width, gpst_DX_stDD->ct.cam.height,
            gpst_DX_stDD->ct.cam.centerX, gpst_DX_stDD->ct.cam.centerY,
            gpst_DX_stDD->ct.cam.factorX, gpst_DX_stDD->ct.cam.factorY,
            D3D_FlatLayerWidth((int)gpst_DX_stDD->ct.cam.width,
                               (int)gpst_DX_stDD->ct.cam.height),
            gpst_DX_stDD->ct.cam.width / 640.0f,
            gpst_DX_stDD->ct.cam.height / 480.0f);
}

void D3D_SetProjectionMatrix(tDDCamStruct* cam)
{
    tPrj_Input  in;
    tPrj_Output out;

    D3DRenderParams.projectionMatrix = &cam->projectionMtx;

    in.surfaceWidth = D3DRenderParams.Rect.right - D3DRenderParams.Rect.left;
    in.surfaceHeight = D3DRenderParams.Rect.bottom - D3DRenderParams.Rect.top;
    in.selectedScreenRatio = gpst_DX_stDD->ddcs2.selectedScreenRatio;
    in.manualScreenRatio = gpst_DX_stDD->ddcs2.manualScreenRatio;
    in.pixelYoverX = gpst_DX_stDD->ddcs2.pixelYoverX;
    in.flags2 = gpst_DX_stDD->ddcs2.flags2;
    in.widescreen = GDI_gi_Widescreen;
    in.realYoverX = D3D_RealYoverX();
    in.refYoverX = GDI_gf_RefYoverX;

    /* This writes the camera's own matrix as well, which is where the
       renderer built it. */
    PRJ_Compute(cam, &in, &out);

    memcpy(&d3dProjectionMtx, &out.matrix, sizeof(D3DMATRIX));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev,
        D3DTS_PROJECTION, (D3DMATRIX*)&out.matrix);

    if (out.setViewport)
    {
        D3D_SetViewPort(out.viewX, out.viewY, out.viewWidth, out.viewHeight);
        D3D_AspectTrace("proj", out.viewX, out.viewY, out.viewWidth,
                        out.viewHeight);
    }
}


void D3D_GetSnapShot(tGdi_SnapshotRequest* req)
{
    unsigned char* rowStart;
    unsigned char* src;
    unsigned char* dest;
    int row;
    int col;
    IDirect3DSurface9* target;
    IDirect3DSurface9* copy;
    D3DLOCKED_RECT locked;
    RECT area;
    D3DSURFACE_DESC desc;

    if ((req->flags & 0xff) == 0 && ((req->flags >> 8) & 0xff) != 2)
    {
        area.left = req->cutSizeFromTopAndBottom;
        area.top = req->cutSizeFromSides;
        area.right = req->cutSizeFromTopAndBottom + req->width;
        area.bottom = req->cutSizeFromSides + req->height;
        if (D3DRenderParams.d3dNeedToCallEndScene)
        {
            D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
            D3DRenderParams.d3dNeedToCallEndScene = 0;
        }
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &target);
        target->lpVtbl->GetDesc(target, &desc);

        D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(D3DRenderParams.d3dDev, desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &copy, 0);

        D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev, target, copy);
        copy->lpVtbl->LockRect(copy, &locked, &area, 0);
        rowStart = (unsigned char*)locked.pBits;
        dest = (unsigned char*)req->srcData;
        for (row = 0; row < req->height; row++)
        {
            src = rowStart;
            for (col = 0; col < req->width; col++)
            {
                dest[0] = src[0];
                dest[1] = src[1];
                dest[2] = src[2];
                dest += 3;
                src += 4;
            }
            rowStart += locked.Pitch;
        }
        copy->lpVtbl->UnlockRect(copy);
        copy->lpVtbl->Release(copy);
        target->lpVtbl->Release(target);
        if (!D3DRenderParams.d3dNeedToCallEndScene)
        {
            D3DRenderParams.d3dDev->lpVtbl->BeginScene(D3DRenderParams.d3dDev);
            D3DRenderParams.d3dNeedToCallEndScene = 1;
        }
    }
}

struct sGdi_D3DRenderParams* D3D_pst_CreateDevice()
{
    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    D3DRenderParams.d3d_if = Direct3DCreate9(D3D9b_SDK_VERSION);
    return &D3DRenderParams;
}

void D3D_DestroyDevice(struct sGdi_D3DRenderParams* renderParams)
{
    if (renderParams == &D3DRenderParams)
    {
        D3DRenderParams.d3d_if->lpVtbl->Release(D3DRenderParams.d3d_if);
        memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    }
}

int D3D_SetViewMatrix(Mtx4x4* mtx)
{
    Mtx4x4 oglMtx;

    MATH_MakeOGLMatrix(&oglMtx, mtx);
    memcpy(&d3dTransformMatrix, &oglMtx, sizeof(d3dTransformMatrix));
    return D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&oglMtx);
}

void D3D_TextureStore(tTexUploadRequest* req)
{
    int idx;
    tEngTexListElemStruct* texElem;
    IDirect3DTexture9* texture;
    IDirect3DSurface9* surface;
    RECT area;

    idx = req->idx;
    texElem = &TEX_gst_GlobalList.texElems[idx];
    texture = D3DRenderParams.d3dTextures[idx];
    if (!texture)
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, texElem->normalData.width, texElem->normalData.height, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &texture, 0);
    texture->lpVtbl->GetSurfaceLevel(texture, 0, &surface);
    area.top = 0;
    area.left = 0;
    area.right = texElem->normalData.width;
    area.bottom = texElem->normalData.height;
    /* Reported rather than discarded: a failure here leaves the
       texture showing whatever was in it, which on a video screen
       looks like the video simply not playing. */
    if (!D3D_b_SurfaceLoadFromMemory(surface, req->dataPtr,
                                     4 * texElem->normalData.width,
                                     area.right, area.bottom))
        GDI_OutputDebugString("texture %d: %dx%d upload refused\n", idx,
                              area.right, area.bottom);
    /* Kept because the texture may be one that already existed and
       carries levels; the one created just above has a single level,
       where this does nothing, exactly as the call it replaces did. */
    D3D_b_TextureBuildMipLevels(texture);
    surface->lpVtbl->Release(surface);
    D3DRenderParams.d3dTextures[req->idx] = texture;
}

void D3D_ReleaseTextures(struct sGdi_DisplayData* dd)
{
    int i;

    if (D3DRenderParams.d3dTextures)
    {
        for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
        {
            if (D3DRenderParams.d3dTextures[i])
            {
                D3DRenderParams.d3dTextures[i]->lpVtbl->Release(D3DRenderParams.d3dTextures[i]);
                D3DRenderParams.d3dTextures[i] = NULL;
            }
        }
    }
    // preserved: the source array is walked with no null test, unlike the one above
    for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
    {
        if (D3DRenderParams.d3dSourceTextures[i])
        {
            D3DRenderParams.d3dSourceTextures[i]->lpVtbl->Release(D3DRenderParams.d3dSourceTextures[i]);
            D3DRenderParams.d3dSourceTextures[i] = NULL;
        }
    }
    if (D3DRenderParams.d3dTextures)
    {
        MEM_Free(D3DRenderParams.d3dTextures);
        D3DRenderParams.d3dTextures = NULL;
    }
    if (D3DRenderParams.d3dSourceTextures)
    {
        MEM_Free(D3DRenderParams.d3dSourceTextures);
        D3DRenderParams.d3dSourceTextures = NULL;
    }
    D3DRenderParams.d3dTextureNum = 0;
}

void D3D_ReleaseShadowTextures()
{
    int i;

    for (i = 7; i >= 0; i--)
    {
        if (D3DRenderParams.d3dShadowTextures[i])
        {
            D3DRenderParams.d3dShadowTextures[i]->lpVtbl->Release(D3DRenderParams.d3dShadowTextures[i]);
            D3DRenderParams.d3dShadowTextures[i] = NULL;
        }
    }
}

void D3D_RelaseBlurRenderTargetsAndOthers()
{
    if (D3DRenderParams.blurRenderTarget)
    {
        D3DRenderParams.blurRenderTarget->lpVtbl->Release(D3DRenderParams.blurRenderTarget);
        D3DRenderParams.blurRenderTarget = NULL;
    }
    if (D3DRenderParams.blurRenderTarget2)
    {
        D3DRenderParams.blurRenderTarget2->lpVtbl->Release(D3DRenderParams.blurRenderTarget2);
        D3DRenderParams.blurRenderTarget2 = NULL;
    }
    if (D3DRenderParams.unktex1_dword_B31988)
    {
        D3DRenderParams.unktex1_dword_B31988->lpVtbl->Release(D3DRenderParams.unktex1_dword_B31988);
        D3DRenderParams.unktex1_dword_B31988 = NULL;
    }
    if (D3DRenderParams.unktex2_dword_B3198C)
    {
        D3DRenderParams.unktex2_dword_B3198C->lpVtbl->Release(D3DRenderParams.unktex2_dword_B3198C);
        D3DRenderParams.unktex2_dword_B3198C = NULL;
    }
    if (D3DRenderParams.tmpRenderTarget)
    {
        D3DRenderParams.tmpRenderTarget->lpVtbl->Release(D3DRenderParams.tmpRenderTarget);
        D3DRenderParams.tmpRenderTarget = NULL;
    }
}

void D3D_ReleaseBackBufferAndDSSurace1()
{
    int i;

    if (D3DRenderParams.pBackBuffers[0])
    {
        for (i = 0; i < 2; i++)
        {
            D3DRenderParams.pBackBuffers[i]->lpVtbl->Release(D3DRenderParams.pBackBuffers[i]);
            D3DRenderParams.pBackBuffers[i] = NULL;
        }
        D3DRenderParams.pDepthStencilSurface->lpVtbl->Release(D3DRenderParams.pDepthStencilSurface);
        D3DRenderParams.pDepthStencilSurface = NULL;
    }
}

void D3D_BlurTexRelease() // alloc @D3D_CreateFullScreenTextureAndGetDepthStencilSurface
{
    if (D3DRenderParams.blurTex)
    {
        D3DRenderParams.blurTex->lpVtbl->Release(D3DRenderParams.blurTex);
        D3DRenderParams.blurTex = NULL;
        D3DRenderParams.newZStencil->lpVtbl->Release(D3DRenderParams.newZStencil);
        D3DRenderParams.newZStencil = NULL;
    }
}

void D3D_ReleaseSourceAndDestTexture()
{
    if (D3DRenderParams.pDestTexture)
    {
        D3DRenderParams.pDestTexture->lpVtbl->Release(D3DRenderParams.pDestTexture);
        D3DRenderParams.pDestTexture = NULL;
    }
    if (D3DRenderParams.pSourceTexture)
    {
        D3DRenderParams.pSourceTexture->lpVtbl->Release(D3DRenderParams.pSourceTexture);
        D3DRenderParams.pSourceTexture = NULL;
    }
}

void D3D_ReleaseVBOs()
{
    if (D3DRenderParams.d3dXYZVB)
    {
        D3DRenderParams.d3dXYZVB->lpVtbl->Release(D3DRenderParams.d3dXYZVB);
        D3DRenderParams.d3dXYZVB = 0;
        D3DRenderParams.xyzFVF = 0;
        D3DRenderParams.stride2 = 0;
        D3DRenderParams.prevVertexElemIdx2 = 0;
        D3DRenderParams.startVertexElemIdx2 = 0;
        D3DRenderParams.currTargetGao = 0;
    }
    if (D3DRenderParams.d3dNormalsBuffer)
    {
        D3DRenderParams.d3dNormalsBuffer->lpVtbl->Release(D3DRenderParams.d3dNormalsBuffer);
        D3DRenderParams.d3dNormalsBuffer = 0;
        D3DRenderParams.normalsFVF = 0;
        D3DRenderParams.d3dStride = 0;
        D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = 0;
        D3DRenderParams.currVertexStartOffset = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    }
    if (D3DRenderParams.d3dColorsVB)
    {
        D3DRenderParams.d3dColorsVB->lpVtbl->Release(D3DRenderParams.d3dColorsVB);
        D3DRenderParams.d3dColorsVB = 0;
        D3DRenderParams.colorsFVF = 0;
        D3DRenderParams.stride = 0;
        D3DRenderParams.prevVertexElemIdx = 0;
        D3DRenderParams.startVertexElemIdx = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
    }
    if (VBOs[0])
    {
        VBOs[0]->lpVtbl->Release(VBOs[0]);
        VBOs[1]->lpVtbl->Release(VBOs[1]);
        VBOs[2]->lpVtbl->Release(VBOs[2]);
        VBOs[0] = NULL;
        VBOs[1] = NULL;
        VBOs[2] = NULL;
        d3dStreamCurrentOffset = 0;
    }
}

void D3D_ReleaseEverything()
{
    D3D_ReleaseShadowTextures();
    D3D_RelaseBlurRenderTargetsAndOthers();
    D3D_ReleaseBackBufferAndDSSurace1();
    D3D_BlurTexRelease();
    D3D_ReleaseSourceAndDestTexture();
    D3D_ReleaseVBOs();
    if (D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull)
    {
        D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull->lpVtbl->Release(D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
        D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull = NULL;
    }
    if (D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull)
    {
        D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull->lpVtbl->Release(D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
        D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull = NULL;
    }
    if (d3dWaterRTtoSet)
    {
        d3dWaterRTtoSet->lpVtbl->Release(d3dWaterRTtoSet);
        d3dWaterRTtoSet = NULL;
    }
    if (waterTexture)
    {
        waterTexture->lpVtbl->Release(waterTexture);
        waterTexture = NULL;
    }
    if (d3dDSSurfaceToSet)
    {
        d3dDSSurfaceToSet->lpVtbl->Release(d3dDSSurfaceToSet);
        d3dDSSurfaceToSet = NULL;
    }
}

int D3D_l_Close()
{
    D3D_ReleaseEverything();
    //nullsub_32();
    if (D3DRenderParams.d3dDev)
    {
        D3DRenderParams.d3dDev->lpVtbl->Release(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dDev->lpVtbl->Release(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dDev = NULL;
    }
    return 0;
}

void D3D_CheckReturnAndRunSettingsApplicationIfProblem(int result)
{
    if (result < 0)
    {
        /* The settings application, told why it was called for. The
           engine does not wait for it and never read anything back
           from it: it starts it and goes, which is what the handle
           pair being dropped on the floor meant. */
        {
            static const char* const args[] = {
                "SettingsApplication.exe", "-OUTOFVIDEOMEMORY", NULL
            };

            PLT_b_Launch(args);
        }
        D3D_l_Close();
        D3DRenderParams.d3d_if->lpVtbl->Release(D3DRenderParams.d3d_if);
        DestroyWindow(MAI_gh_MainWindow);
        exit(0);
    }
}

void D3D_SetVertexShader(IDirect3DVertexShader9* shader)
{
    if (renderStateConfig.vertexShader != shader)
    {
        renderStateConfig.vertexShader = shader;
        renderStateConfig.fvf = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
        D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
        D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, renderStateConfig.vertexShader);
    }
}

void D3D_SetInitialRenderStates()
{
    memset(&renderStateConfig, 0, sizeof(renderStateConfig));
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, D3DZB_TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, 0);
    if (renderStateConfig.d3dFogTableMode)
    {
        renderStateConfig.d3dFogTableMode = D3DFOG_NONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    }
    if (renderStateConfig.d3dFogVertexMode)
    {
        renderStateConfig.d3dFogVertexMode = D3DFOG_NONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    }
    D3D_RenderStateSetF(&renderStateConfig.d3dFogDensity, D3DRS_FOGDENSITY, 1.0f);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogStart, D3DRS_FOGSTART, 0.0f);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogEnd, D3DRS_FOGEND, 1.0f);
    D3D_RenderStateSet(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, D3DFILL_SOLID);
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_CCW);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, 0xf);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, 0);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaRef, D3DRS_ALPHAREF, 0);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ZERO);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals, D3DRS_NORMALIZENORMALS, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0xffffffff);
    D3D_RenderStateSet(&renderStateConfig.d3dClipPlaneEnable, D3DRS_CLIPPLANEENABLE, 0);
    if (renderStateConfig.fvf)
    {
        renderStateConfig.fvf = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, NULL);
    }
    D3D_SetVertexShader(0);
    renderStateConfig.vertexShader = 0;
    renderStateConfig.fvf = 0;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
}

void D3D_SetInitialSamplerStates()
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
            D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressu, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressv, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            if (samplerConfig[stage].d3d_minfilter)
            {
                samplerConfig[stage].d3d_minfilter = D3DTEXF_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, stage, D3DSAMP_MINFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[stage].d3d_magfilter)
            {
                samplerConfig[stage].d3d_magfilter = D3DTEXF_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, stage, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
            }
            if (samplerConfig[stage].d3d_mipfilter)
            {
                samplerConfig[stage].d3d_mipfilter = D3DTEXF_NONE;
                D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
            }
        D3D_SamplerStateSetF(stage, &samplerConfig[stage].d3d_mipmaplodbias, D3DSAMP_MIPMAPLODBIAS, 0.0f);
    }
}

void D3D_SetInitialTextureStageStates()
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
        textureStageTypeConfig[stage].d3dTextureID = -1;
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat00, D3DTSS_BUMPENVMAT00, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat01, D3DTSS_BUMPENVMAT01, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat10, D3DTSS_BUMPENVMAT10, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat11, D3DTSS_BUMPENVMAT11, 0.0f);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, stage);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLScale, D3DTSS_BUMPENVLSCALE, 0.0f);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLOffset, D3DTSS_BUMPENVLOFFSET, 0.0f);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorArg0, D3DTSS_COLORARG0, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg0, D3DTSS_ALPHAARG0, D3DTA_CURRENT);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_resultArg0, D3DTSS_RESULTARG, D3DTA_CURRENT);
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_constant, D3DTSS_CONSTANT, 0);
    }
}

void D3D_RastersDrawUPStream()
{
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLELIST,
        ((rastersDrawPrimitiveUPDataEnd - rastersDrawPrimitiveUPDataStart) >> 4) / 3,
        rastersDrawPrimitiveUPDataStart,
        16); // 16 = Vec3 + color (int), see D3DRenderParams.fvf
    rastersDrawPrimitiveUPDataEnd = rastersDrawPrimitiveUPDataStart;
}

void D3D_SaveSamplerState(tD3dSamplerConfigStruct* dest)
{
    memcpy(dest, samplerConfig, sizeof(samplerConfig));
}

void D3D_SaveTextureStage(tD3dTextureStageTypeStruct* dest)
{
    memcpy(dest, textureStageTypeConfig, sizeof(textureStageTypeConfig));
}

void D3D_SaveRenderState(tD3dRenderStateConfigStruct *dest)
{
    memcpy(dest, &renderStateConfig, sizeof(tD3dRenderStateConfigStruct));
}

void D3D_SaveEverythingToBackup(int saveType)
{
    if (saveType != 3)
    {
        D3D_SaveSamplerState(samplerStateBackup);
        D3D_SaveTextureStage(texturestageBackup);
    }
    if (saveType != 2)
        D3D_SaveRenderState(&renderstateBackup);
    d3dparamsSaveType = saveType;
}

void D3D_RenderStateRestore(tD3dRenderStateConfigStruct* cfg)
{
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, cfg->d3d_zEnable);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, cfg->d3d_zFunc);
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, cfg->d3d_zWriteEnable);
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, cfg->d3dFogEnable);
    D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, cfg->d3dFogColor);
    D3D_RenderStateSet(&renderStateConfig.d3dFogTableMode, D3DRS_FOGTABLEMODE, cfg->d3dFogTableMode);
    D3D_RenderStateSet(&renderStateConfig.d3dFogVertexMode, D3DRS_FOGVERTEXMODE, cfg->d3dFogVertexMode);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogDensity, D3DRS_FOGDENSITY, cfg->d3dFogDensity);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogStart, D3DRS_FOGSTART, cfg->d3dFogStart);
    D3D_RenderStateSetF(&renderStateConfig.d3dFogEnd, D3DRS_FOGEND, cfg->d3dFogEnd);
    D3D_RenderStateSet(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, cfg->d3dFillMode);
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, cfg->d3dCullMode);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, cfg->d3dColorWriteEnable);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, cfg->d3dAlphaTest);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, cfg->d3dAlphaFunc);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaRef, D3DRS_ALPHAREF, cfg->d3dAlphaRef);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, cfg->d3dAlphaBlendEnable);
    D3D_RenderStateSet(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, cfg->d3d_BlendOp);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, cfg->d3dSrcBlend);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, cfg->d3dDestBlend);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, cfg->d3d_Lighting);
    D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals, D3DRS_NORMALIZENORMALS, cfg->d3d_normalizeNormals);
    D3D_RenderStateSet(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, cfg->d3dTextureFactor);
    D3D_RenderStateSet(&renderStateConfig.d3dClipPlaneEnable, D3DRS_CLIPPLANEENABLE, cfg->d3dClipPlaneEnable);
    if (cfg->vertexShader)
    {
        D3D_SetFixedFunctionFVF(0);
        if (renderStateConfig.fvf != cfg->fvf)
        {
            renderStateConfig.fvf = cfg->fvf;
            D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, (IDirect3DVertexDeclaration9 *)(renderStateConfig.fvf)); // TODO FIXME: ez micsoda gányolás de az IDA ezt mutatja, játékban ez van
        }
        D3D_SetVertexShader(cfg->vertexShader);
    }
    else
    {
        if (renderStateConfig.fvf)
        {
            renderStateConfig.fvf = 0;
            D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(D3DRenderParams.d3dDev, NULL);
        }
        D3D_SetVertexShader(0);
        D3D_SetFixedFunctionFVF(cfg->fvf);
    }
}

void D3D_SamplerStateRestore(tD3dSamplerConfigStruct* cfg)
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressu, D3DSAMP_ADDRESSU, cfg[stage].d3d_addressu);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_addressv, D3DSAMP_ADDRESSV, cfg[stage].d3d_addressv);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_minfilter, D3DSAMP_MINFILTER, cfg[stage].d3d_minfilter);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_magfilter, D3DSAMP_MAGFILTER, cfg[stage].d3d_magfilter);
        D3D_SamplerStateSet(stage, &samplerConfig[stage].d3d_mipfilter, D3DSAMP_MIPFILTER, cfg[stage].d3d_mipfilter);
        D3D_SamplerStateSetF(stage, &samplerConfig[stage].d3d_mipmaplodbias, D3DSAMP_MIPMAPLODBIAS, cfg[stage].d3d_mipmaplodbias);
    }
}

void D3D_SetTexture(int textureState, int textureID)
{
    IDirect3DTexture9* texture;

    if (textureID == -2)
    {
        textureStageTypeConfig[textureState].d3dTextureID = -2;
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, textureState, (IDirect3DBaseTexture9*)D3DRenderParams.pDestTexture);
    }
    if (D3DRenderParams.d3dTextures)
    {
        if (textureID != textureStageTypeConfig[textureState].d3dTextureID)
        {
            if (textureID != -1 && textureID != -2)
            {
                if (textureID >= 0) {
                    texture = D3DRenderParams.d3dTextures[textureID];
                } 
                else {
                    texture = D3DRenderParams.d3dShadowTextures[(textureID & ~0x80000000)];
                }
                D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, textureState, (IDirect3DBaseTexture9*)texture);
            }
            textureStageTypeConfig[textureState].d3dTextureID = textureID;
        }
    }
}

void D3D_TextureStageStateRestore(tD3dTextureStageTypeStruct* cfg)
{
    unsigned int stage;

    for (stage = 0; stage < D3DRenderParams.d3dcaps.MaxTextureBlendStages; stage++)
    {
            textureStageTypeConfig[stage].d3dTextureID = cfg[stage].d3dTextureID;
            D3D_SetTexture(stage, cfg[stage].d3dTextureID);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, 1, cfg[stage].d3d_colorOp);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg1, D3DTSS_COLORARG1, cfg[stage].d3d_colorarg1);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg2, D3DTSS_COLORARG2, cfg[stage].d3d_colorarg2);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, cfg[stage].d3d_alphaOp);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg1, D3DTSS_ALPHAARG1, cfg[stage].d3d_alphaArg1);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg2, D3DTSS_ALPHAARG2, cfg[stage].d3d_alphaArg2);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat00, D3DTSS_BUMPENVMAT00, cfg[stage].d3d_bumpenvmat00);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat01, D3DTSS_BUMPENVMAT01, cfg[stage].d3d_bumpenvmat01);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat10, D3DTSS_BUMPENVMAT10, cfg[stage].d3d_bumpenvmat10);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpenvmat11, D3DTSS_BUMPENVMAT11, cfg[stage].d3d_bumpenvmat11);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, cfg[stage].d3dTexCoordIndex);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLScale, D3DTSS_BUMPENVLSCALE, cfg[stage].d3d_bumpEnvLScale);
            D3D_StageStateSetF(stage, &textureStageTypeConfig[stage].d3d_bumpEnvLOffset, D3DTSS_BUMPENVLOFFSET, cfg[stage].d3d_bumpEnvLOffset);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, cfg[stage].d3dTextureTransformFlags);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorArg0, D3DTSS_COLORARG0, cfg[stage].d3d_colorArg0);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg0, D3DTSS_ALPHAARG0, cfg[stage].d3d_alphaArg0);
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_resultArg0, D3DTSS_RESULTARG, cfg[stage].d3d_resultArg0);
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_constant, D3DTSS_CONSTANT, cfg[stage].d3d_constant);
    }
}

void D3D_RestoreEverythingFromBackup()
{
    if (d3dparamsSaveType != 3)
    {
        D3D_SamplerStateRestore(samplerStateBackup);
        D3D_TextureStageStateRestore(texturestageBackup);
    }
    if (d3dparamsSaveType != 2)
        D3D_RenderStateRestore(&renderstateBackup);
}

// The overlay's screen-space pass. The render states live here
// because two of the setters are file-local; the overlay itself only
// builds geometry and strings.
void D3D_OverlayBegin(void)
{
    D3DMATRIX mtx;
    float viewWidth;
    float viewHeight;

    viewWidth = (float)D3DRenderParams.d3dViewPort.Width;
    viewHeight = (float)D3DRenderParams.d3dViewPort.Height;

    D3D_SaveEverythingToBackup(1);

    memset(&mtx, 0, sizeof(mtx));
    mtx._11 = 1.0f;
    mtx._22 = 1.0f;
    mtx._33 = 1.0f;
    mtx._44 = 1.0f;
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, &mtx);
    D3D_MatrixOrthoOffCenterRH(&mtx, 0.0, viewWidth, viewHeight, 0.0, -1.0, 1.0);
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &mtx);

    D3D_SetVertexShader(0);
    D3D_SetFixedFunctionFVF(D3DFVF_DIFFUSE | D3DFVF_XYZ);
    if (renderStateConfig.d3d_zEnable)
    {
        renderStateConfig.d3d_zEnable = D3DZB_FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DZB_FALSE);
    }
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_NONE);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, FALSE);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, 0xf);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    D3D_SetTexture(0, -1);
    D3D_SetTexture(1, -1);
    D3D_SetTexture(2, -1);
    D3D_SetTexture(3, -1);
}

/* A glyph atlas. The coverage arrives as one byte per pixel; this
   becomes an A8R8G8B8 texture with white where the glyph is and the
   coverage in alpha, so a MODULATE stage takes the colour from the
   vertex and the shape from here. A8 would be half the memory and is
   a format a card may decline, which is not worth finding out about
   on somebody else's machine. */
void* D3D_p_CreateAlphaTexture(int width, int height, const uint8_t* coverage)
{
    IDirect3DTexture9* tex = NULL;
    D3DLOCKED_RECT     locked;
    int                row, col;

    if (!D3DRenderParams.d3dDev || width <= 0 || height <= 0 || !coverage)
        return NULL;
    if (FAILED(D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev, (UINT)width, (UINT)height, 1, 0,
            D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL)))
        return NULL;
    if (FAILED(tex->lpVtbl->LockRect(tex, 0, &locked, NULL, 0)))
    {
        tex->lpVtbl->Release(tex);
        return NULL;
    }
    for (row = 0; row < height; row++)
    {
        uint32_t*      out = (uint32_t*)((char*)locked.pBits + (size_t)row * locked.Pitch);
        const uint8_t* in  = coverage + (size_t)row * width;

        for (col = 0; col < width; col++)
            out[col] = ((uint32_t)in[col] << 24) | 0x00FFFFFF;
    }
    tex->lpVtbl->UnlockRect(tex, 0);
    return tex;
}

void D3D_DestroyAlphaTexture(void* texture)
{
    IDirect3DTexture9* tex = (IDirect3DTexture9*)texture;

    if (tex)
        tex->lpVtbl->Release(tex);
}

/* Textured quads inside the overlay pass. The atlas is not one of the
   engine's textures, so it is bound straight to stage 0 and the cached
   id is left saying nothing is bound -- which is true when this
   returns, and is what makes the next D3D_SetTexture bind again
   rather than trusting the cache. */
void D3D_OverlayDrawTexturedQuads(void* texture, const void* vertices,
                                  int triangleCount)
{
    IDirect3DTexture9* tex = (IDirect3DTexture9*)texture;

    if (!tex || !vertices || triangleCount <= 0)
        return;

    D3D_SetFixedFunctionFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0,
                                               (IDirect3DBaseTexture9*)tex);

    /* Colour from the vertex, shape from the atlas: the texture is
       white, so modulating leaves the colour alone, and the alphas
       multiply so a translucent request stays translucent. */
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorOp,
                      D3DTSS_COLOROP, D3DTOP_MODULATE);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1,
                      D3DTSS_COLORARG1, D3DTA_TEXTURE);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2,
                      D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaOp,
                      D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaArg1,
                      D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaArg2,
                      D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags,
                      D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(
        D3DRenderParams.d3dDev, D3DPT_TRIANGLELIST, triangleCount,
        vertices, sizeof(tGdi_TexturedVertex));

    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    textureStageTypeConfig[0].d3dTextureID = -1;
}

void D3D_OverlayDrawQuads(const void* verts, int triangleCount)
{
    if (triangleCount <= 0)
        return;
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(
        D3DRenderParams.d3dDev, D3DPT_TRIANGLELIST, triangleCount,
        verts, sizeof(tGdi_Vertex));
}

/* Whether this renderer has a device to draw through. The overlay
   runs from the message loop and can be asked to draw before one
   exists or after one has gone. */
int D3D_b_Ready(void)
{
    return D3DRenderParams.d3dDev != NULL;
}

const char* D3D_sz_Name(void)
{
    return "Direct3D 9";
}

void D3D_OverlayEnd(void)
{
    D3D_RestoreEverythingFromBackup();
}

void D3D_RastersSetAndDrawAQuad(int color, float x, float y, float w, float h, int flush)
{
    tVertexAndColor* quad;
    char* buffer;
    double right;
    double bottom;

    quad = (tVertexAndColor*)rastersDrawPrimitiveUPDataEnd;
    if (!rastersDrawPrimitiveUPDataEnd)
    {
        buffer = (char*)MEM_p_Alloc(16384u);
        memset(buffer, 0, 16384u);
        rastersDrawPrimitiveUPDataStart = buffer;
        quad = (tVertexAndColor*)buffer;
    }

    right = x + w;
    bottom = y + h;

    quad[0].v.x = x;
    quad[0].v.y = y;
    quad[0].color = color;

    quad[1].v.x = (float)right;
    quad[1].v.y = y;
    quad[1].color = color;    
    
    quad[2].v.x = x;
    quad[2].v.y = (float)bottom;
    quad[2].color = color;

    quad[3].v.x = x;
    quad[3].v.y = (float)bottom;
    quad[3].color = color;

    quad[4].v.x = (float)right;
    quad[4].v.y = y;
    quad[4].color = color;

    quad[5].v.x = (float)right;
    quad[5].v.y = (float)bottom;
    quad[5].color = color;
    //rastersDrawPrimitiveUPDataEnd += sizeof(tVertexAndColor) * 6;
    rastersDrawPrimitiveUPDataEnd = (char *)(quad + 6); // a vége az 6 db tVertexAndColor elemet fog tartalmazni, mivel a v6 az egy tVertexAndColor típusra mutató pointer
    if (flush)
        D3D_RastersDrawUPStream();
}

void D3D_DrawARaster(int color, char* name, float value, float y, float width)
{
    char text[32];

    tPfb_Quad quads[8];
    int       n;
    int       i;

    sprintf(text, "%5.3f", value);
    FNT_DrawText(FNT_RASTERS, name, 20, (unsigned int)y, 0xFFFFFFFF);
    FNT_DrawText(FNT_RASTERS, text, (unsigned int)(width - 50.0),
                 (unsigned int)y, 0xFFFFFFFF);
    /* The segments from perfbars.c, which the other renderer's display
       takes them from too. */
    n = PFB_i_BarQuads(quads, 8, (unsigned int)color, value, y, width);
    for (i = 0; i < n; i++)
        D3D_RastersSetAndDrawAQuad((int)quads[i].color, quads[i].x,
                                   quads[i].y, quads[i].w, quads[i].h, 0);
}

void Rasters_Update()
{
    double elapsed;
    int shown;
    int row;
    int raster;
    int kind;
    int barColor;
    double nextY;
    float y;
    float value;
    float scale;
    float viewHeight;
    float viewWidth;
    float fps;
    float barHeight;
    LARGE_INTEGER after;
    LARGE_INTEGER Frequency;
    LARGE_INTEGER PerformanceCount;
    D3DMATRIX mtx;
    int order[54];

    viewWidth = (float)D3DRenderParams.d3dViewPort.Width;
    viewHeight = (float)D3DRenderParams.d3dViewPort.Height;

    if (tweak_perf)
    {
        PRO_QPC_RecordTimeDecRef(53);
        PRO_QPC_RecordTimeDecRef(19);
        QueryPerformanceCounter(&PerfCounterStruct[47]);
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&PerformanceCount);
        elapsed = (double)(PerformanceCount.QuadPart - qword_990B48);
        qword_990B48 = PerformanceCount.QuadPart;
        fps = PFB_f_Fps((int64_t)elapsed, Frequency.QuadPart);
        if (PFB_b_On(tweak_perf, fps, tweak_perf_fps_min))
        {
            D3D_SaveEverythingToBackup(1);
            mtx._43 = 0.0;
            mtx._42 = 0.0;
            mtx._41 = 0.0;
            mtx._34 = 0.0;
            mtx._32 = 0.0;
            mtx._31 = 0.0;
            mtx._24 = 0.0;
            mtx._23 = 0.0;
            mtx._21 = 0.0;
            mtx._14 = 0.0;
            mtx._13 = 0.0;
            mtx._12 = 0.0;
            mtx._44 = 1.0;
            mtx._33 = 1.0;
            mtx._22 = 1.0;
            mtx._11 = 1.0;
            memcpy(&d3dTransformMatrix, &mtx, sizeof(d3dTransformMatrix));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, 2, &mtx);
            D3D_MatrixOrthoOffCenterRH(&mtx, 0.0, viewWidth, viewHeight, 0.0, -1.0, 1.0);
            memcpy(&d3dProjectionMtx, &mtx, sizeof(d3dProjectionMtx));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, 3, &mtx);
            D3D_SetVertexShader(0);
            D3D_SetFixedFunctionFVF(D3DFVF_DIFFUSE | D3DFVF_XYZ);
            if (renderStateConfig.d3d_zEnable)
            {
                renderStateConfig.d3d_zEnable = D3DZB_FALSE;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DZB_FALSE);
            }
            D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_NONE);
            D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, FALSE);
            D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, 0xf);
            D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            D3D_SetTexture(0, -1);
            D3D_SetTexture(1, -1);
            D3D_SetTexture(2, -1);
            D3D_SetTexture(3, -1);
            if (PFB_b_Band(fps, tweak_perf_fps_min))
                D3D_RastersSetAndDrawAQuad(0xFFFF0000, 0.0, 0.0, viewWidth,
                                           PFB_BAND_H, 0);
            if (PFB_b_Rows(tweak_perf))
            {
                PFB_Smooth(rastersUnkTimeFloats_dbl_990B58,
                           PerfCountertimeSumArray, PFB_RASTERS);
                PFB_Order(order, rastersUnkTimeFloats_dbl_990B58,
                          PFB_RASTERS);
                /* Narrowed on purpose, and the other display does
                   the same: the bars are measured from this number and
                   the two have to measure them from the same one. */
                scale = (float)PFB_d_Scale(tweak_perf_fps_target,
                                           Frequency.QuadPart);
                shown = PFB_i_Rows(tweak_perf,
                                   PFB_i_Fitting(PerfCountertimeSumArray,
                                                 rasterNames, order,
                                                 PFB_RASTERS, scale,
                                                 viewHeight));
                barHeight = (float)((double)shown * (double)PFB_ROW_H);
                D3D_RastersSetAndDrawAQuad(0x80000000, 0.0, PFB_FPS_Y,
                                           viewWidth, barHeight, 1);
                //nullsub_32();
                row = 0;
                for (y = PFB_FIRST_Y; row < shown; ++row)
                {
                    raster = order[row];
                    kind = rasterNames[raster].unk1;
                    value = PFB_f_Value(PerfCountertimeSumArray[raster],
                                        scale);
                    if (kind && value > 0.01)
                    {
                        barColor = (int)PFB_u_RowColour(raster);
                        D3D_DrawARaster(barColor, rasterNames[raster].name, value, y, viewWidth);
                        nextY = y + (double)PFB_ROW_H;
                        y = (float)nextY;
                        if (nextY > viewHeight)
                            break;
                    }
                }
                D3D_DrawARaster(0xFFFFFF, "Frame Rate", fps, PFB_FPS_Y,
                                viewWidth);
                //nullsub_32();
            }
            D3D_RastersDrawUPStream();
            D3D_RestoreEverythingFromBackup();
        }
        memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
        QueryPerformanceCounter(&after);
        if (PerfCounterStruct[47].QuadPart)
            PerfCountertimeSumArray[47] += after.QuadPart - PerfCounterStruct[47].QuadPart;
        PerfCounterStruct[47].QuadPart = 0LL;
        QueryPerformanceCounter(&PerfCounterStruct[19]);
        QueryPerformanceCounter(&PerfCounterStruct[53]);
    }
}

void D3D_CycleThroughVBOs()
{
    d3dStreamCurrentOffset = 0;
    currentVBOSelected = (currentVBOSelected + 1) % 3u;
}

/* Whether this renderer has anything cached for a piece of geometry.
   The test is the index buffer alone, which is what the one caller
   that asks has always tested -- and is narrower than what gets
   freed: PC_FreeABackCache below also releases the vertex buffers and
   the UV buffer array. A geometry holding those but no index buffer
   would therefore be left holding them. Kept as it was; see
   docs/renderers.md. */
int D3D_b_GeometryHasCache(struct sGEOGRO* gro)
{
    return gro && gro->indexBuffer != NULL;
}

void D3D_DiscardGeometryCache(struct sGEOGRO* gro)
{
    if (gro)
        PC_FreeABackCache(gro);
}

void PC_FreeABackCache(struct sGEOGRO* geogro)
{
    //int v3;
    int i;

    if (geogro->vertexBuffer)
    {
        D3D_GEO_VB(geogro)->lpVtbl->Release(D3D_GEO_VB(geogro));
        geogro->vertexBuffer = NULL;
    }
    if (geogro->vertexBuffer2)
    {
        D3D_GEO_VB2(geogro)->lpVtbl->Release(D3D_GEO_VB2(geogro));
        geogro->vertexBuffer2 = NULL;
    }
    /* TODO FIXME
    v3 = *(_DWORD*)a1->gap4b;
    if (v3)
    {
        (*(void(__stdcall**)(_DWORD))(*(_DWORD*)v3 + 8))(*(_DWORD*)a1->gap4b);
        *(_DWORD*)a1->gap4b = 0;
    }
    */
    if (geogro->uvBuffers)
    {
        for (i = 0; i < geogro->uvBufferNum; ++i)
        {
            if (geogro->uvBuffers[i])
            {
                D3D_GEO_UVVB(geogro)[i]->lpVtbl->Release(D3D_GEO_UVVB(geogro)[i]);
                geogro->uvBuffers[i] = NULL;
            }
        }
        MEM_Free(geogro->uvBuffers);
        geogro->uvBuffers = NULL;
    }
    if (geogro->indexBuffer)
    {
        D3D_GEO_IB(geogro)->lpVtbl->Release(D3D_GEO_IB(geogro));
        geogro->indexBuffer = NULL;
    }
}

void D3D_ReleaseMeshSpecificData(tGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;
    tGEOGRO* geogro;
    tGEOSTATICLOD* lod;
    int i;

    if ((gao->flags & 0x4000) != 0)
    {
        visual = gao->basic->actorObjectOrVisualObject;
        geogro = (tGEOGRO*)visual->GROGEOOrSomethingElse;
        lod = (tGEOSTATICLOD*)visual->GROGEOOrSomethingElse;
        if (visual->GROGEOOrSomethingElse)
        {
            if (geogro->intf->id == 1)
            {
                if (geogro->triangleChunkCount && geogro->triangleChunks && geogro->vertexNum)
                    PC_FreeABackCache(geogro);
            }
            else if (lod->intf->id == 8)
            {
                for (i = 0; i < lod->lodNum; i++)
                {
                    if (lod->geo[i]->triangleChunkCount && lod->geo[i]->triangleChunks
                        && lod->geo[i]->vertexNum)
                        PC_FreeABackCache(lod->geo[i]);
                }
            }
        }
    }
}

void D3D_ReleaseMeshSpecificDataOfAWorld(tWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGroup* skeleton;
    tableDescStruct* table;
    tableElemStructPF* bone;
    tableElemStructPF* lastBone;

    if (world && world->objectTablePF.actElemNum)
    {
        last = ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1;
        for (elem = world->objectTablePF.firstElemPtr; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (!gao || ((uint8_t)(size_t)gao & 3) == 1)
                continue;
            D3D_ReleaseMeshSpecificData(gao);
            if ((gao->flags & 2) == 0)
                continue;
            skeleton = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup;
            if (!skeleton)
                continue;
            table = skeleton->tablePF;
            lastBone = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
            for (bone = table->firstElemPtr; bone <= lastBone; ++bone)
            {
                if (bone->objectPtr && ((uint8_t)(size_t)bone->objectPtr & 3) != 1)
                    D3D_ReleaseMeshSpecificData((tGameObjectMain*)bone->objectPtr);
            }
        }
    }
}

void D3D_ReleaseWaterIndices()
{
    if (waterStreamData)
    {
        waterStreamData->lpVtbl->Release(waterStreamData);
        waterStreamData = NULL;
        waterIndexData->lpVtbl->Release(waterIndexData);
        waterIndexData = NULL;
    }
}

void D3D_ReleaseSunRenderTargetAndTexture(struct sGfxSunD3DSpecificData* sun)
{
    if (sun->d3dTexture)
    {
        sun->d3dRenderTarget->lpVtbl->Release(sun->d3dRenderTarget);
        sun->d3dRenderTarget = NULL;
        sun->d3dTexture->lpVtbl->Release(sun->d3dTexture);
        sun->d3dTexture = NULL;
    }
}

void D3D_ReleaseSunSurfaces()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i])
            D3D_ReleaseSunRenderTargetAndTexture(gD3dSunSurfaces[i]);
    }
}

void D3D_ReleaseEveryCache()
{
    int i;

    D3D_ReleaseWaterIndices();
    D3D_ReleaseSunSurfaces();
    D3D_ReleaseEverything();
    D3D_ReleaseMeshSpecificDataOfAWorld(WOR_gpst_CurrentWorld);
    if (D3DRenderParams.d3dTextures)
    {
        for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
        {
            if (D3DRenderParams.d3dTextures[i])
                D3DRenderParams.d3dTextures[i]->lpVtbl->Release(D3DRenderParams.d3dTextures[i]);
        }
        MEM_Free(D3DRenderParams.d3dTextures);
        D3DRenderParams.d3dTextures = NULL;
    }
}

int D3D_CalculateStride(unsigned int fvf)
{
    int stride;
    unsigned int texCount;
    unsigned int texSizes;

    stride = 0;
    switch (fvf & 0x400E)
    {
    case 2u:
        stride = 12;
        break;
    case 4u:
    case 6u:
        stride = 16;
        break;
    case 8u:
        stride = 20;
        break;
    case 0xAu:
        stride = 24;
        break;
    case 0xCu:
        stride = 28;
        break;
    case 0xEu:
        stride = 32;
        break;
    default:
        break;
    }
    if ((fvf & 0x10) != 0)
        stride += 12;
    if ((fvf & 0x20) != 0)
        stride += 4;
    if ((fvf & 0x40) != 0)
        stride += 4;
    if ((fvf & 0x80u) != 0)
        stride += 4;
    texCount = (fvf >> 8) & 0xF;
    texSizes = (fvf >> 16) & 0xffff;
    if (texSizes)
    {
        for (; texCount; --texCount)
        {
            switch (texSizes & 3)
            {
            case 0u:
                stride += 8;
                break;
            case 1u:
                stride += 12;
                break;
            case 2u:
                stride += 16;
                break;
            case 3u:
                stride += 4;
                break;
            }
            texSizes >>= 2;
        }
    }
    else
    {
        stride += 8 * texCount;
    }
    return stride;
}

void D3D_CreateThreeVbos()
{
    int i;
    int result;

    for (i = 0; i < 3; i++)
    {
        result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
            D3DRenderParams.d3dDev,
            2097152,
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
            D3DPOOL_DEFAULT,
            &VBOs[i],
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    }
}

void D3D_CreateVBOs()
{
    int result;

    D3D_CreateThreeVbos();
    D3DRenderParams.prevVertexElemIdx2 = 0;
    D3DRenderParams.startVertexElemIdx2 = 0;
    D3DRenderParams.currTargetGao = 0;
    D3DRenderParams.xyzFVF = D3DFVF_XYZ;
    D3DRenderParams.stride2 = D3D_CalculateStride(D3DFVF_XYZ);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        65535 * D3DRenderParams.stride2,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.xyzFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dXYZVB,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = 0;
    D3DRenderParams.currVertexStartOffset = 0;
    D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    D3DRenderParams.normalsFVF = D3DFVF_NORMAL;
    D3DRenderParams.d3dStride = D3D_CalculateStride(D3DFVF_NORMAL);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        65535 * D3DRenderParams.d3dStride,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.normalsFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dNormalsBuffer,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    D3DRenderParams.prevVertexElemIdx = 0;
    D3DRenderParams.startVertexElemIdx = 0;
    D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
    D3DRenderParams.colorsFVF = D3DFVF_DIFFUSE;
    D3DRenderParams.stride = D3D_CalculateStride(D3DFVF_DIFFUSE);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
        D3DRenderParams.d3dDev,
        262143 * D3DRenderParams.stride,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DRenderParams.colorsFVF,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.d3dColorsVB,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
}

void D3D_SetColorWriteEnable(char flags)
{
    int mask;

    mask = 0;
    if ((flags & 0x20) == 0)
        mask = D3DCOLORWRITEENABLE_ALPHA;
    if ((flags & 0x40) == 0)
        mask |= (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, mask);
}

void D3D_SetCullingParams()
{
    int drawMask;
    int cullMode;

    drawMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    if ((drawMask & 0x800) != 0)
    {
        if ((drawMask & 0x2000) == 0)
            cullMode = D3DCULL_CCW;
        else
            cullMode = D3DCULL_CW;
    }
    else
    {
        cullMode = 1;
    }
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, cullMode);
}

void D3D_SetTextureStageStates(int stage, int textureID)
{
    if (textureID == -1)
    {
        if (textureStageTypeConfig[stage].d3d_colorarg1)
        { 
            textureStageTypeConfig[stage].d3d_colorarg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, 1, D3DTOP_SELECTARG1);
        if (textureStageTypeConfig[stage].d3d_alphaArg1)
        {
            textureStageTypeConfig[stage].d3d_alphaArg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    else if (textureID != -2)
    {
        if (D3DRenderParams.tweak_ae_d2x && GDI_gi_Diffuse2x && textureID >= 0)
        {
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
        }
        else
        {
            D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        if (textureStageTypeConfig[stage].d3d_colorarg2)
        {
            textureStageTypeConfig[stage].d3d_colorarg2 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(stage, &textureStageTypeConfig[stage].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        if (textureStageTypeConfig[stage].d3d_alphaArg2)
        {
            textureStageTypeConfig[stage].d3d_alphaArg2 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, stage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        }
    }
}

void D3D_SetAlphaTestParams(unsigned int mask, int16_t flags)
{
    int compare;

    if ((flags & 0x10) != 0)
    {
        if (renderStateConfig.d3dAlphaTest != 1)
        {
            renderStateConfig.d3dAlphaTest = 1;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, TRUE);
        }
        compare = D3DCMP_LESS;
        if ((flags & 0x80u) == 0)
            compare = D3DCMP_GREATER;
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, compare);
        if (renderStateConfig.d3dAlphaRef != ((mask >> 22) & 0xFC))
        {
            renderStateConfig.d3dAlphaRef = (mask >> 22) & 0xFC;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAREF, (mask >> 22) & 0xFC);
        }
    }
    else if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, FALSE);
    }
    D3DRenderParams.colorMaskAlpha = (flags & 0x400) != 0 ? 0xFF000000 : 0;
}

void D3D_SetAlphaBlendingOptions(unsigned int mask, char flags)
{
    int destBlend;

    d3d_alphablendIsCase3 = 0;
    switch (((mask >> 16) & 0xffff) & 0xF)
    {
    case 0:
    case 7:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ZERO);
        return;
    case 1:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        if (flags >= 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            destBlend = 6;
        }
        else
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
            destBlend = 5;
        }
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, destBlend);
        return;
    case 2:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
        if (flags >= 0)
            destBlend = D3DBLEND_INVSRCALPHA;
        else
            destBlend = D3DBLEND_SRCALPHA;
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, destBlend);
        return;
    case 3:
        d3d_alphablendIsCase3 = 1;
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        if (flags >= 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
            D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
        }
        else
        {
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
            D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, 7);
        }
        return;
    case 4:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        if (flags >= 0)
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
        else
            D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
        return;
    case 5:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
        return;
    case 6:
    case 8:
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
        D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ZERO);
        D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
        return;
    default:
        return;
    }
}

void D3D_SetFogParams(unsigned int mask)
{
    int mode;
    int enable;
    int color;

    mode = D3DRenderParams.d3dFogModeProbably;
    enable = (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x4000) != 0 && D3DRenderParams.d3dFogModeProbably;
    if (renderStateConfig.d3dFogEnable != enable)
    {
        renderStateConfig.d3dFogEnable = enable;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, enable);
        mode = D3DRenderParams.d3dFogModeProbably;
    }
    if (!mode)
        return;
    switch (HIWORD(mask) & 0xF)
    {
    case 0:
    case 1:
    case 3:
    case 7:
        if (mode == 1)
            return;
        color = D3DRenderParams.d3dFogColor;
        D3DRenderParams.d3dFogModeProbably = 1;
        break;
    case 2:
    case 4:
        if (mode == 5)
            return;
        color = D3DRenderParams.d3dFogColor;
        D3DRenderParams.d3dFogModeProbably = 5;
        break;
    case 5:
    case 6:
        if (mode == 3)
            return;
        color = D3DRenderParams.d3dFogColor2Case5and6;
        D3DRenderParams.d3dFogModeProbably = 3;
        break;
    default:
        return;
    }
    D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, color);
}

void D3D_l_Texture_Init(int textureID, int flags, char mode)
{
    int filter;

    if ((mode & 1) != 0)
    {
        if (D3DRenderParams.textureUVIsProjected)
        {
            GDI_gi_TextureUVMode = 4;
            memcpy(d3dTransformationMatrix, &D3DRenderParams.transformationMatrix, sizeof(D3DMATRIX));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&D3DRenderParams.transformationMatrix);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
            if (textureStageTypeConfig[0].d3dTextureTransformFlags != (D3DTTFF_PROJECTED | D3DTTFF_COUNT3))
            {
                textureStageTypeConfig[0].d3dTextureTransformFlags = D3DTTFF_PROJECTED | D3DTTFF_COUNT3;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED | D3DTTFF_COUNT3);
            }
        }
        else
        {
            GDI_gi_TextureUVMode = 5;
            memcpy(d3dTransformationMatrix, &D3DRenderParams.transformationMatrix, sizeof(D3DMATRIX));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_TEXTURE0, (D3DMATRIX*)&D3DRenderParams.transformationMatrix);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        }
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressu, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressv, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        if (textureStageTypeConfig[0].d3d_colorarg1)
        {
            textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_DIFFUSE;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        }
        D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        D3D_SetTexture(0, -2);
        D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, FALSE);
        D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, FALSE);
        D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    else if ((mode & 2) == 0)
    {
        if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x10) != 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, D3DFILL_SOLID);
        }
        else if (renderStateConfig.d3dFillMode != D3DFILL_WIREFRAME)
        {
            renderStateConfig.d3dFillMode = D3DFILL_WIREFRAME;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        }
        if (renderStateConfig.d3d_zEnable != ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x1000000) != 0 ? D3DRenderParams.d3d_defaultZEnable : 0))
        {
            renderStateConfig.d3d_zEnable = (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x1000000) != 0 ? D3DRenderParams.d3d_defaultZEnable : 0;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, renderStateConfig.d3d_zEnable);
        }
        D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, 4 - ((flags & 0x100) != 0));
        if (renderStateConfig.d3d_zWriteEnable != ((flags & 0x200) == 0))
        {
            renderStateConfig.d3d_zWriteEnable = (flags & 0x200) == 0;
            D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, renderStateConfig.d3d_zWriteEnable);
        }
        D3D_SetCullingParams();
        D3D_SetTexture(0, textureID);
        D3D_SetTextureStageStates(0, textureID);
        if (samplerConfig[0].d3d_addressu != (~(2 * (uint8_t)flags) & 2 | 1))
        {
            samplerConfig[0].d3d_addressu = ~(2 * (uint8_t)flags) & 2 | 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSU, samplerConfig[0].d3d_addressu);
        }
        if (samplerConfig[0].d3d_addressv != (~(uint8_t)flags & 2 | 1))
        {
            samplerConfig[0].d3d_addressv = ~(uint8_t)flags & 2 | 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_ADDRESSV, samplerConfig[0].d3d_addressv);
        }
        filter = ((flags & 4) != 0) + 1;
        if (samplerConfig[0].d3d_minfilter != filter)
        {
            samplerConfig[0].d3d_minfilter = ((flags & 4) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, samplerConfig[0].d3d_minfilter);
        }
        if (samplerConfig[0].d3d_magfilter != filter)
        {
            samplerConfig[0].d3d_magfilter = ((flags & 4) != 0) + 1;
            D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, samplerConfig[0].d3d_magfilter);
        }
        D3D_SamplerStateSet(0, &samplerConfig[0].d3d_mipfilter, D3DSAMP_MIPFILTER, ((flags & 8) != 0) + 1);
        D3D_SamplerStateSetF(0, &samplerConfig[0].d3d_mipmaplodbias, D3DSAMP_MIPMAPLODBIAS, tweak_mipmaplodbias);
        D3D_SetColorWriteEnable(flags);
        D3D_SetAlphaTestParams(flags, flags & 0xFFF);
        D3D_SetAlphaBlendingOptions(flags, flags);
        D3D_SetFogParams(flags);
        gpst_DX_stDD->rasters[2].unk1 = textureID; // TODO FIXME, itt nem volt az IDAban unk1, jó ez így?
        gpst_DX_stDD->rasters[3].unk1 = flags; // TODO FIXME, itt nem volt az IDAban unk1, jó ez így?
    }
}

BOOL D3D_CheckDeviceFormatSupported(int usage, int resourceType, int format)
{
    return D3DRenderParams.d3d_if->lpVtbl->CheckDeviceFormat(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, D3DRenderParams.d3d_AdapterFormat, usage, resourceType, format) >= 0;
}

BOOL D3D_CheckDepthStencilMatch(int renderFormat, int depthFormat)
{
    return D3DRenderParams.d3d_if->lpVtbl->CheckDepthStencilMatch(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, D3DRenderParams.d3d_AdapterFormat, renderFormat, depthFormat) >= 0;
}

int D3D_SelectSupportedDeviceFormat(int format)
{
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32) && D3D_CheckDepthStencilMatch(format, D3DFMT_D32))
        return D3DFMT_D32;
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8) && D3D_CheckDepthStencilMatch(format, D3DFMT_D24X8))
        return D3DFMT_D24X8;
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16) && D3D_CheckDepthStencilMatch(format, D3DFMT_D16))
        return D3DFMT_D16;
    if (D3D_CheckDeviceFormatSupported(D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16_LOCKABLE) && D3D_CheckDepthStencilMatch(format, D3DFMT_D16_LOCKABLE))
        return D3DFMT_D16_LOCKABLE;
    return D3DFMT_UNKNOWN;
}

void D3D_CreateWaterTexture()
{
    int result;
    int format;

    result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        512,
        512,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &waterTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    waterTexture->lpVtbl->GetSurfaceLevel(waterTexture, 0, &d3dWaterRTtoSet);
    format = D3D_SelectSupportedDeviceFormat(D3DFMT_A8R8G8B8);
    result = D3DRenderParams.d3dDev->lpVtbl->CreateDepthStencilSurface(D3DRenderParams.d3dDev,
        512,
        512,
        format,
        D3DMULTISAMPLE_NONE,
        0,
        FALSE,
        &d3dDSSurfaceToSet,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
}

void D3D_CreateFullScreenTextureAndGetDepthStencilSurface()
{
    int result;

    result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        D3DRenderParams.presentParams.BackBufferWidth,
        D3DRenderParams.presentParams.BackBufferHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DRenderParams.presentParams.BackBufferFormat,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.blurTex,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &D3DRenderParams.newZStencil);
}

void D3D_PresentAndClear()
{
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &D3DRenderParams.pDepthStencilSurface);
    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &(D3DRenderParams.pBackBuffers[0]));
    D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL);
    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &(D3DRenderParams.pBackBuffers[1]));
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL);
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
}



void D3D_InitEverything(tGdi_DisplayData* dd)
{
    int i;
    int result;
    D3DVERTEXELEMENT9 decl[4];
    IDirect3DSurface9* surf;

    D3D_CreateVBOs();
    decl[0].Stream = 0;
    decl[0].Offset = 0;
    decl[0].Type = 2;
    decl[0].Method = 0;
    decl[0].Usage = 0;
    decl[0].UsageIndex = 0;
    decl[1].Stream = 1;
    decl[1].Offset = 0;
    decl[1].Type = 4;
    decl[1].Method = 0;
    decl[1].Usage = 10;
    decl[1].UsageIndex = 0;
    decl[2].Stream = 255;
    decl[2].Offset = 0;
    decl[2].Type = 17;
    decl[2].Method = 0;
    decl[2].Usage = 0;
    decl[2].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, decl, &D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
    decl[0].Stream = 0;
    decl[0].Offset = 0;
    decl[0].Type = 2;
    decl[0].Method = 0;
    decl[0].Usage = 0;
    decl[0].UsageIndex = 0;
    decl[1].Stream = 1;
    decl[1].Offset = 0;
    decl[1].Type = 4;
    decl[1].Method = 0;
    decl[1].Usage = 10;
    decl[1].UsageIndex = 0;
    decl[2].Stream = 2;
    decl[2].Offset = 0;
    decl[2].Type = 2;
    decl[2].Method = 0;
    decl[2].Usage = 3;
    decl[2].UsageIndex = 0;
    decl[3].Stream = 255;
    decl[3].Offset = 0;
    decl[3].Type = 17;
    decl[3].Method = 0;
    decl[3].Usage = 0;
    decl[3].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, decl, &D3DRenderParams.d3dVertexDeclaration);
    decl[0].Stream = 0;
    decl[0].Offset = 0;
    decl[0].Type = 2;
    decl[0].Method = 0;
    decl[0].Usage = 0;
    decl[0].UsageIndex = 0;
    decl[1].Stream = 1;
    decl[1].Offset = 0;
    decl[1].Type = 4;
    decl[1].Method = 0;
    decl[1].Usage = 10;
    decl[1].UsageIndex = 0;
    decl[2].Stream = 2;
    decl[2].Offset = 0;
    decl[2].Type = 1;
    decl[2].Method = 0;
    decl[2].Usage = 5;
    decl[2].UsageIndex = 0;
    decl[3].Stream = 255;
    decl[3].Offset = 0;
    decl[3].Type = 17;
    decl[3].Method = 0;
    decl[3].Usage = 0;
    decl[3].UsageIndex = 0;
    D3DRenderParams.d3dDev->lpVtbl->CreateVertexDeclaration(D3DRenderParams.d3dDev, decl, &D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
    gpst_DX_stDD = dd;
    dd->rasters[3].unk1 = 0; // TODO FIXME ez biztos jó így ???? megnézni. az idaban csak siman a1->rasters[3] = 0 van de az nem jó
    D3D_SetViewPort(0,
        (D3DRenderParams.Rect.bottom - (D3DRenderParams.Rect.right - D3DRenderParams.Rect.left) - D3DRenderParams.Rect.top) / 2,
        D3DRenderParams.Rect.right - D3DRenderParams.Rect.left, D3DRenderParams.Rect.right - D3DRenderParams.Rect.left);
    
    if ((D3DRenderParams.d3dcaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0
        && !D3DRenderParams.argbIsSupportedForTextures
        && D3DRenderParams.d3d_if->lpVtbl->CheckDeviceFormat(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, 1, D3DRenderParams.d3d_AdapterFormat,
            D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8) >= 0) 
    {
        D3DRenderParams.argbIsSupportedForTextures = 1;
    }
    D3D_SetInitialRenderStates();
    D3D_SetInitialSamplerStates();
    D3D_SetInitialTextureStageStates();
    D3D_l_Texture_Init(-1, -1, 0);
    D3D_SetProjectionMatrix(&dd->ct.cam);
    D3D_CreateWaterTexture();
    D3D_RenderStateSet(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, D3DRenderParams.d3d_defaultZEnable);
    D3DRenderParams.tweak_ae_d2x = D3DRenderParams.d3dcaps.TextureOpCaps & D3DTEXOPCAPS_MODULATE2X;
    GDI_gi_TextureUVMode = 0;
    GDI_gi_Diffuse2x = 0;
    memset(&D3DRenderParams.transformationMatrix, 0, 64u);
    D3DRenderParams.textureUVIsProjected = 1;
    D3DRenderParams.isDepthBlurOn = 0;
    D3DRenderParams.tweak_zonalpha = 0;
    D3DRenderParams.sunEffectIsOn = 0;
    D3DRenderParams.tweak_ae_noflare = D3DRenderParams.d3dcaps.MaxSimultaneousTextures < 4u;
    D3D_CreateFullScreenTextureAndGetDepthStencilSurface();
    D3D_PresentAndClear();
    D3D_CreateBlurTextures();
    D3D_AETexturesInit();
    //*(_DWORD*)&v5[3].Stream = v4; / ????
    for (i = 7; i >= 0; i--)
    {
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, 128, 128, 1, 1, 21, 0, &D3DRenderParams.d3dShadowTextures[i], 0);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
        D3DRenderParams.d3dShadowTextures[i]->lpVtbl->GetSurfaceLevel(D3DRenderParams.d3dShadowTextures[i], 0, &surf);
        D3DRenderParams.d3dDev->lpVtbl->ColorFill(D3DRenderParams.d3dDev, surf, NULL, 0xffffffff);
        surf->lpVtbl->Release(surf);
    }
}

void D3D_CreateAndCopySourceTexturesToThem()
{
    unsigned int bytes;
    int i;
    int levels;
    int result;
    IDirect3DTexture9* copy;
    D3DSURFACE_DESC desc;

    bytes = 4 * D3DRenderParams.d3dTextureNum + 4;
    D3DRenderParams.d3dTextures = (IDirect3DTexture9**)MEM_p_Alloc(bytes);
    memset(D3DRenderParams.d3dTextures, 0, bytes);
    for (i = 0; i < D3DRenderParams.d3dTextureNum; i++)
    {
        copy = 0;
        if (D3DRenderParams.d3dSourceTextures[i])
        {
            D3DRenderParams.d3dSourceTextures[i]->lpVtbl->GetLevelDesc(D3DRenderParams.d3dSourceTextures[i], 0, &desc);
            levels = D3DRenderParams.d3dSourceTextures[i]->lpVtbl->GetLevelCount(D3DRenderParams.d3dSourceTextures[i]);
            result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
                desc.Width,
                desc.Height,
                levels,
                0,
                desc.Format,
                D3DPOOL_DEFAULT,
                &copy,
                NULL);
            D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
            D3DRenderParams.d3dSourceTextures[i]->lpVtbl->AddDirtyRect(D3DRenderParams.d3dSourceTextures[i], NULL);
            if (copy)
                D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)D3DRenderParams.d3dSourceTextures[i], (IDirect3DBaseTexture9*)copy);
        }
        D3DRenderParams.d3dTextures[i] = copy;
    }
}
void D3D_CreateIndexAndVertexBuffForElemsIfSuitable(tGameObjectMain* gao, tGEOGRO* geogro)
{
    int stride;
    int result;
    int chunk;
    int strip;
    int elem;
    int i;
    int buf;
    int found;
    int fits;
    int newBuf;
    int total;
    int pos;
    int streamStart;
    int streams;
    unsigned int word;
    tGeoWholeTriangleStripBlock* block;
    tGeoTriangleStrip* strips;
    tGeoTriangleStripElem* elems;
    tGeoUVElem* uv;
    tGeoUVElem* src;
    tD3dStreamInfoElem* info;
    void* locked;
    unsigned short* out;
    tGeoUVElem* uvBufs[50];

    if (worldPtrIfMultiStream) // TODO FIXME, Ennek a függvénynek az elsõ hívásánál ez biztos hogy 0 lesz, szóal csak a másodiknál lesznek ezek a pufferek létrehozva.
    {
        GEO_UseNormals(geogro);
        if ((gao->flags & 2) == 0 || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            if (!geogro->vertexBuffer)
            {
                stride = D3D_CalculateStride(D3DFVF_XYZ);
                result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                    D3DRenderParams.d3dDev,
                    stride * geogro->vertexNum,
                    D3DUSAGE_WRITEONLY,
                    D3DFVF_XYZ,
                    D3DPOOL_DEFAULT,
                    (IDirect3DVertexBuffer9**)&geogro->vertexBuffer,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
                D3D_GEO_VB(geogro)->lpVtbl->Lock(D3D_GEO_VB(geogro), 0, 0, &locked, 0);
                memcpy(locked, geogro->vertices, stride * geogro->vertexNum);
                D3D_GEO_VB(geogro)->lpVtbl->Unlock(D3D_GEO_VB(geogro));
            }
            if (!geogro->vertexBuffer2)
            {
                stride = D3D_CalculateStride(D3DFVF_NORMAL);
                result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                    D3DRenderParams.d3dDev,
                    stride * geogro->vertexNum,
                    D3DUSAGE_WRITEONLY,
                    D3DFVF_NORMAL,
                    D3DPOOL_DEFAULT,
                    (IDirect3DVertexBuffer9**)&geogro->vertexBuffer2,
                    NULL);
                D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
                D3D_GEO_VB2(geogro)->lpVtbl->Lock(D3D_GEO_VB2(geogro), 0, 0, &locked, 0);
                memcpy(locked, geogro->normals, stride * geogro->vertexNum);
                D3D_GEO_VB2(geogro)->lpVtbl->Unlock(D3D_GEO_VB2(geogro));
            }
        }
        if (!geogro->uvBuffers && geogro->uvNum)
        {
            geogro->uvBufferNum = 1;
            geogro->uvBuffers = (void**)MEM_p_Alloc(4u);
            stride = D3D_CalculateStride(D3DFVF_TEX1);
            result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                D3DRenderParams.d3dDev,
                stride * geogro->vertexNum,
                D3DUSAGE_WRITEONLY,
                D3DFVF_TEX1,
                D3DPOOL_DEFAULT,
                &(D3D_GEO_UVVB(geogro)[0]),
                NULL);
            D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);

            D3D_GEO_UVVB(geogro)[0]->lpVtbl->Lock(D3D_GEO_UVVB(geogro)[0], 0, 0, (void**)&uvBufs[0], 0);

            if (geogro->vertexNum)
            {
                for (i = 0; i < (int)((sizeof(tGeoUVElem) * geogro->vertexNum - 1) >> 2) + 1; i++) {
                    ((float*)uvBufs[0])[i] = FLT_MAX;
     //               *((int*)(&(v88[0][i]))) = 0x7F7FFFFF; // ez FLOAT_MAX, TODO FIXME
                }
            }
            for (chunk = 0; chunk < (int)geogro->triangleChunkCount; chunk++)
            {
                block = geogro->triangleChunks[chunk].triangleStripBlock;
                if (!block)
                    continue;
                for (strip = 0; strip < (int)block->triangleStripNum; strip++)
                {
                    strips = &block->triangleStrips[strip];
                    found = -1;
                    for (buf = 0; buf < geogro->uvBufferNum; buf++)
                    {
                        uv = uvBufs[buf];
                        if (strips->triangleStripElemNum == 0)
                        {
                            found = buf;
                            break;
                        }
                        fits = 1;
                        for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                        {
                            i = strips->triangleStripElems[elem].triangleIdx;
                            src = &geogro->UVs[strips->triangleStripElems[elem].uvIdx];
                            if (uv[i].u != FLT_MAX && uv[i].u != src->u)
                            {
                                fits = 0;
                                break;
                            }
                            if (uv[i].v != FLT_MAX && uv[i].v != src->v)
                            {
                                fits = 0;
                                break;
                            }
                        }
                        if (fits)
                        {
                            found = buf;
                            break;
                        }
                    }
                    if (found >= 0)
                    {
                        uv = uvBufs[found];
                        for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                        {
                            elems = strips->triangleStripElems;
                            i = elems[elem].triangleIdx;
                            src = &geogro->UVs[elems[elem].uvIdx];
                            uv[i].u = src->u;
                            uv[i].v = src->v;
                        }
                        strips->vbIdx = found;
                        continue;
                    }
                    newBuf = geogro->uvBufferNum;
                    geogro->uvBufferNum = newBuf + 1;
                    geogro->uvBuffers = (void**)MEM_p_Realloc(geogro->uvBuffers, 4 * (newBuf + 1));
                    stride = D3D_CalculateStride(D3DFVF_TEX1);
                    result = D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(
                        D3DRenderParams.d3dDev,
                        stride * geogro->vertexNum,
                        D3DUSAGE_WRITEONLY,
                        D3DFVF_TEX1,
                        D3DPOOL_DEFAULT,
                        &D3D_GEO_UVVB(geogro)[newBuf],
                        NULL);
                    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
                    D3D_GEO_UVVB(geogro)[newBuf]->lpVtbl->Lock(D3D_GEO_UVVB(geogro)[newBuf], 0, 0, (void**)&uvBufs[newBuf], 0);
                    uv = uvBufs[newBuf];
                    if (geogro->vertexNum)
                    {
                        for (i = 0; i < (int)((sizeof(tGeoUVElem) * geogro->vertexNum - 1) >> 2) + 1; i++) {
                            ((float*)uv)[i] = FLT_MAX;
                                            //*((int*)(&(v40[i]))) = 0x7F7FFFFF; // ez FLOAT_MAX, TODO FIXME
                        }
                    }
                    for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                    {
                        elems = strips->triangleStripElems;
                        i = elems[elem].triangleIdx;
                        src = &geogro->UVs[elems[elem].uvIdx];
                        uv[i].u = src->u;
                        uv[i].v = src->v;
                    }
                    strips->vbIdx = newBuf;
                }
            }
            for (i = 0; i < geogro->uvBufferNum; ++i)
                D3D_GEO_UVVB(geogro)[i]->lpVtbl->Unlock(D3D_GEO_UVVB(geogro)[i]);
        }
        if (geogro->indexBuffer || (int)geogro->triangleChunkCount <= 0)
            return;
        total = 0;
        for (chunk = 0; chunk < (int)geogro->triangleChunkCount; chunk++)
        {
            block = geogro->triangleChunks[chunk].triangleStripBlock;
            if (!block)
                continue;
            streams = 0;
            for (buf = 0; buf < geogro->uvBufferNum; buf++)
            {
                streamStart = total;
                for (strip = 0; strip < (int)block->triangleStripNum; strip++)
                {
                    strips = &block->triangleStrips[strip];
                    if (strips->vbIdx != buf)
                        continue;
                    if (streamStart != total)
                    {
                        total += 2;
                        if ((((uint8_t)total - (uint8_t)streamStart) & 1) != 0) // ez nembiztos hogy jó így TODO IFXME
                            ++total;
                    }
                    total += strips->triangleStripElemNum;
                }
                if (total != streamStart)
                    ++streams;
            }
            block->streamsNum = streams;
        }
        if (!total)
            return;
        pos = 0;
        result = D3DRenderParams.d3dDev->lpVtbl->CreateIndexBuffer(
            D3DRenderParams.d3dDev,
            2 * total,
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            (IDirect3DIndexBuffer9**)&geogro->indexBuffer,
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
        D3D_GEO_IB(geogro)->lpVtbl->Lock(D3D_GEO_IB(geogro), 0, 0, (void**)&out, 0); // ide uj v75 kéne
        for (chunk = 0; chunk < (int)geogro->triangleChunkCount; chunk++)
        {
            block = geogro->triangleChunks[chunk].triangleStripBlock;
            if (!block)
                continue;
            word = sizeof(tD3dStreamInfoElem) * block->streamsNum;
            streams = 0;
            block->d3dStreamInfoElems = (tD3dStreamInfoElem*)MEM_p_Alloc(word);
            for (buf = 0; buf < geogro->uvBufferNum; buf++)
            {
                if ((unsigned int)streams >= block->streamsNum)
                    break;
                block->d3dStreamInfoElems[streams].startIndex = pos;
                for (strip = 0; strip < (int)block->triangleStripNum; strip++)
                {
                    //v68 = v63[v77].triangleStrips;
                    strips = &block->triangleStrips[strip];
                    if (strips->vbIdx != buf)
                        continue;
                    info = &block->d3dStreamInfoElems[streams];
                    if (info->startIndex == pos)
                    {
                        info->vbIdx = buf;
                    }
                    else
                    {
                        *out = *(out - 1);
                        *++out = strips->triangleStripElems->triangleIdx;
                        pos += 2;
                        ++out;
                        if ((((uint8_t)pos - (uint8_t)block->d3dStreamInfoElems[streams].startIndex) & 1) != 0)
                        {
                            *out++ = strips->triangleStripElems->triangleIdx;
                            ++pos;
                        }
                    }
                    for (elem = 0; elem < strips->triangleStripElemNum; elem++)
                        *out++ = strips->triangleStripElems[elem].triangleIdx;
                    pos += strips->triangleStripElemNum;
                }
                info = &block->d3dStreamInfoElems[streams];
                if (pos != info->startIndex)
                {
                    info->primitiveCount = pos - info->startIndex;
                    streams++;
                }
            }
        }
        D3D_GEO_IB(geogro)->lpVtbl->Unlock(D3D_GEO_IB(geogro));
    }
}

void D3D_CreateIndexAndVertexBuffForGameObject(tGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;
    tGEOGRO* geogro;
    tGEOSTATICLOD* lod;
    int i;

    if ((gao->flags & 0x4000) != 0)
    {
        visual = (tGameObjectVisualObject*)gao->basic->actorObjectOrVisualObject;
        geogro = (tGEOGRO*)visual->GROGEOOrSomethingElse;
        lod = (tGEOSTATICLOD*)visual->GROGEOOrSomethingElse;
        if (visual->GROGEOOrSomethingElse)
        {
            if (geogro->intf->id == 1)
            {
                if (geogro->triangleChunkCount && geogro->triangleChunks && geogro->vertexNum)
                    D3D_CreateIndexAndVertexBuffForElemsIfSuitable(gao, geogro);
            }
            else if (lod->intf->id == 8)
            {
                for (i = 0; i < lod->lodNum; i++)
                {
                    if (lod->geo[i]->triangleChunkCount && lod->geo[i]->triangleChunks
                        && lod->geo[i]->vertexNum)
                        D3D_CreateIndexAndVertexBuffForElemsIfSuitable(gao, lod->geo[i]);
                }
            }
        }
    }
}

void D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGroup* skeleton;
    tableDescStruct* table;
    tableElemStructPF* bone;
    tableElemStructPF* lastBone;

    /* Without a device this renderer was not the one chosen. The
       engine calls in here directly rather than through the
       interface, so this runs whichever renderer is in force, and
       the only right answer is to do nothing. */
    if (!D3DRenderParams.d3dDev)
        return;

    if (ENG_IsSingleStream)
        return;
    if (world && world->objectTablePF.actElemNum)
    {
        last = ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1;
        for (elem = world->objectTablePF.firstElemPtr; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (!gao || ((uint8_t)(size_t)gao & 3) == 1)
                continue;
            D3D_CreateIndexAndVertexBuffForGameObject(gao);
            if ((gao->flags & 2) == 0)
                continue;
            skeleton = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup;
            if (!skeleton)
                continue;
            table = skeleton->tablePF;
            lastBone = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
            for (bone = table->firstElemPtr; bone <= lastBone; ++bone)
            {
                if (bone->objectPtr && ((int)(size_t)bone->objectPtr & 3) != 1)
                    D3D_CreateIndexAndVertexBuffForGameObject((tGameObjectMain*)bone->objectPtr);
            }
        }
    }
    if (!worldPtrIfMultiStream)
        worldPtrIfMultiStream = world;
}

void D3D_CreateSunTextures(tGfxSunD3DSpecificData* sun)
{
    int result;

    if (!sun->d3dTexture)
    {
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
            16,
            1,
            1,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT,
            &sun->d3dTexture,
            NULL);
        D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
        sun->d3dTexture->lpVtbl->GetSurfaceLevel(sun->d3dTexture, 0, &sun->d3dRenderTarget);
    }
}

void D3D_CreateSunTexturesForSunSurfaces()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i])
            D3D_CreateSunTextures(gD3dSunSurfaces[i]);
    }
}

void D3D_WaterBuffersInit()
{
    short* indices;

    if (wtr_NumA && wtr_NumB && !waterStreamData)
    {
        wtr_primitiveCount = WMS_i_PrimitiveCount((unsigned int)wtr_NumA,
                                                  wtr_NumB);
        D3DRenderParams.d3dDev->lpVtbl->CreateVertexBuffer(D3DRenderParams.d3dDev,
            32 * wtr_NumA * wtr_NumB + 16,
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_SPECULAR | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ,
            D3DPOOL_DEFAULT,
            &waterStreamData,
            NULL);
        D3DRenderParams.d3dDev->lpVtbl->CreateIndexBuffer(D3DRenderParams.d3dDev,
            2 * wtr_primitiveCount + 4,
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &waterIndexData,
            NULL);
        waterIndexData->lpVtbl->Lock(waterIndexData, 0, 0, (void **)&indices, 0);
        /* The strip itself has no device in it, so it is built by the
           shared mesh code that the other renderer also draws from. */
        WMS_i_StripIndices((unsigned int)wtr_NumA, wtr_NumB,
                           (unsigned short*)indices,
                           WMS_i_IndexCount((unsigned int)wtr_NumA, wtr_NumB));
        waterIndexData->lpVtbl->Unlock(waterIndexData);
    }
}

void D3D_ReinitD3DAppBackCache()
{
    D3D_InitEverything(gpst_DX_stDD);
    D3D_CreateAndCopySourceTexturesToThem();
    D3D_CreateGameObjectsD3DStuffForMultiStream(WOR_gpst_CurrentWorld);
    D3D_CreateSunTexturesForSunSurfaces();
    D3D_WaterBuffersInit();
}

void D3D_Flip()
{
    int result;
    int status;
    int resetResult;
    struct tagMSG Msg;

    Rasters_Update();
    OVL_Draw();           // the overlay goes last, on top of everything
    if (D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 0;
    }
    result = D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, NULL, NULL, NULL, NULL); 
    status = result;
    if (!ENG_gb_ExitApplication && !ENG_gb_ForceEndEngine)
    {
        if (result == D3DERR_DEVICELOST) //0x88760868
        {
            do
            {
                D3DRenderParams.dword_B317A0 = 1;
                while (status != D3DERR_DEVICENOTRESET) //0x88760869
                {
                    if (PeekMessageA(&Msg, 0, 0, 0, 1u))
                    {
                        TranslateMessage(&Msg);
                        DispatchMessageA(&Msg);
                        if (ENG_gb_ExitApplication || ENG_gb_ForceEndEngine)
                            return;
                    }
                    else
                    {
                        status = D3DRenderParams.d3dDev->lpVtbl->TestCooperativeLevel(D3DRenderParams.d3dDev);
                    }
                }
                D3D_ReleaseEveryCache();
                resetResult = D3DRenderParams.d3dDev->lpVtbl->Reset(D3DRenderParams.d3dDev, &D3DRenderParams.presentParams); 
                status = resetResult;
            } while (resetResult == D3DERR_DEVICELOST);
            if (resetResult >= 0)
                D3D_ReinitD3DAppBackCache();
            else
                ENG_gb_ExitApplication = 1;
        }
        D3D_CycleThroughVBOs();
        D3DRenderParams.currTargetGao = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = 0;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = 0;
    }
}

void D3D_Clear(int flags, int color) 
{
    if (flags == 2)
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    else
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL,
            (flags & D3DCLEAR_ZBUFFER) | D3DCLEAR_TARGET,
            color & 0xFF00FF00 | ((uint8_t)color << 16) | ((color >> 16) & 0xff), 1.0f, 0);
}

void D3D_SunSurfacesClear()
{
    memset(gD3dSunSurfaces, 0, sizeof(gD3dSunSurfaces));
}

/* What the driver says it is: the same line the other renderer logs
   when its context comes up, which is the first question about a
   renderer that draws nothing.
 *
   Once per device, not once per run -- this sits where the card's
   presets are chosen, which is itself part of making a device, and a
   device made a second time has a driver worth naming again. The
   identifier is the caller's, already fetched: asking for it again
   would be a second call for something it is holding. */
static void D3D_l_SayWhatThisIs(const D3DADAPTER_IDENTIFIER9* adapter)
{
    if (!adapter)
        return;
    GDI_OutputDebugString("D3D: %s, %s %d.%d.%d.%d\n",
                          adapter->Description, adapter->Driver,
                          HIWORD(adapter->DriverVersion.HighPart),
                          LOWORD(adapter->DriverVersion.HighPart),
                          HIWORD(adapter->DriverVersion.LowPart),
                          LOWORD(adapter->DriverVersion.LowPart));
}

void D3D_ApplyRenderingPreesetsForVariousGraphicsCards()
{
    D3DADAPTER_IDENTIFIER9 adapter;
    DWORD device;

    D3DRenderParams.d3d_if->lpVtbl->GetAdapterIdentifier(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, 0, &adapter);
    D3D_l_SayWhatThisIs(&adapter);
    device = adapter.DeviceId;
    if (adapter.VendorId == 0x1002)
    {
        ENG_IsDLBFEnabled = 1;
        if (device >= 0x4144 && device <= 0x4146
            || device == 16740
            || device >= 0x4E44 && device <= 0x4E45
            || device >= 0x4E64 && device <= 0x4E65)
        {
            D3DRenderParams.argbIsSupportedForTextures = -1;
        }
        else if (device == 0x4C57
                 || device >= 0x4C59 && device <= 0x4C5A
                 || device >= 0x5144 && device <= 0x5147
                 || device == 0x5157
                 || device >= 0x5159 && device <= 0x515A)
        {
            ENG_IsSingleStream = 1;
        }
        return;
    }
    if (adapter.VendorId != 4318)
        return;
    switch (device)
    {
    case 0x110u:
    case 0x111u:
    case 0x112u:
    case 0x113u:
    case 0x174u:
    case 0x175u:
    case 0x176u:
    case 0x179u:
        ENG_IsDLBFEnabled = 1;
        ENG_IsSingleStream = 1;
        break;
    case 0x170u:
    case 0x171u:
    case 0x172u:
    case 0x173u:
    case 0x177u:
        ENG_IsSingleStream = 1;
        break;
    default:
        break;
    }
}

int D3D_l_Init(HWND hWnd, struct sGdi_DisplayData* dd)
{
    int ret;
    int i;
    int tries;
    HMODULE libHandle;
    void (*DisableD3DSpy)(void);
    int adapterMode;
    D3DDISPLAYMODE dispMode;

    if (D3D_l_Close())
        return E_FAIL;
    
    D3DRenderParams.d3d_AdapterFormat = D3DFMT_X8R8G8B8;
    D3DRenderParams.presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;

    /* The four the renderer cannot start without, out of [display]
       in engine.ini beside the executable. They were four registry
       values under Software\IRISEng\Basic video, and the message
       below has always been what the player sees when they are not
       set -- which is now a missing or incomplete ini rather than a
       missing registry key. */
    if (!SET_b_DisplayAdapter(&D3DRenderParams.d3d_Adapter)
        || !SET_b_DisplayZBuffer(&D3DRenderParams.ZBufferDepth)
        || !SET_b_DisplayMode(&adapterMode)
        || !SET_b_DisplayMipMapping(&D3DRenderParams.Mipmapping))
    {
        MessageBoxA(hWnd, "Application settings is not correctly set or invalid, please re-run settings application.", "IRISEngine", MB_TASKMODAL | MB_ICONERROR);
        exit(1);
    }

    /* An exclusive display mode under a debugger wedges the machine,
       so a debugged run presents in a window whatever the settings
       say. The question used to be asked by loading kernel32 and
       looking the answer up in it; it is asked by name now, and the
       platform answers it however it can. */
    if (PLT_b_DebuggerAttached() || ENG_WindowedMode)
    {
        D3DRenderParams.d3d_if->lpVtbl->GetAdapterDisplayMode(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, &dispMode);
        D3DRenderParams.d3d_AdapterFormat = dispMode.Format;
        D3DRenderParams.presentParams.Windowed = 1;
        GetClientRect(hWnd, &D3DRenderParams.Rect);
    }
    else
    {
        D3DRenderParams.d3d_if->lpVtbl->EnumAdapterModes(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DRenderParams.d3d_AdapterFormat, adapterMode, &dispMode);
        D3DRenderParams.presentParams.Windowed = 0;
        D3DRenderParams.presentParams.FullScreen_RefreshRateInHz = dispMode.RefreshRate;
        D3DRenderParams.Rect.right = dispMode.Width;
        D3DRenderParams.Rect.bottom = dispMode.Height;
    }
    D3DRenderParams.hwnd2_dword_B3162C = (int)hWnd;
    D3DRenderParams.dword_B317A0 = 0;
    D3DRenderParams.argbIsSupportedForTextures = 0;
    D3DRenderParams.setsDefaultZEnableAtBeginScene = 0;
    D3D_ApplyRenderingPreesetsForVariousGraphicsCards();
    if (ENG_IsDLBFEnabled)
    {
        D3DRenderParams.presentParams.SwapEffect = D3DSWAPEFFECT_COPY;
        D3DRenderParams.presentParams.BackBufferCount = 1;
    }
    else
    {
        D3DRenderParams.presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
        D3DRenderParams.presentParams.BackBufferCount = 2;
    }
    D3DRenderParams.presentParams.BackBufferWidth = D3DRenderParams.Rect.right;
    D3DRenderParams.presentParams.hDeviceWindow = hWnd;
    D3DRenderParams.presentParams.BackBufferHeight = D3DRenderParams.Rect.bottom;
    D3DRenderParams.presentParams.EnableAutoDepthStencil = 1;
    D3DRenderParams.presentParams.AutoDepthStencilFormat = D3DRenderParams.ZBufferDepth;
    D3DRenderParams.presentParams.PresentationInterval = 1;
    ret = E_FAIL;
    for (i = 0; i < 2; ++i)
    {
        if (ret >= 0)
            break; 
        ret = D3DRenderParams.d3d_if->lpVtbl->CreateDevice(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_FPU_PRESERVE | D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING, 
            &D3DRenderParams.presentParams, &D3DRenderParams.d3dDev);
    }
    tries = 0;
    while (ret < 0)
    {
        ret = D3DRenderParams.d3d_if->lpVtbl->CreateDevice(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &D3DRenderParams.presentParams, &D3DRenderParams.d3dDev);
        if (++tries >= 2)
        {
            if (ret < 0)
            {
                MessageBoxA(hWnd, "IDirect3D9_CreateDevice failed, please re-run settings application.", "IRISEngine", MB_TASKMODAL | MB_ICONERROR);
                exit(1);
            }
            break;
        }
    }
    if (!tweakFlagsDisabled && D3DRenderParams.d3dDev->lpVtbl->SetDialogBoxMode(D3DRenderParams.d3dDev, TRUE) < 0)
        OutputDebugStringA("SetDialogBoxMode failed\n");
    D3DRenderParams.d3d_if->lpVtbl->GetDeviceCaps(D3DRenderParams.d3d_if, D3DRenderParams.d3d_Adapter, D3DDEVTYPE_HAL, &(D3DRenderParams.d3dcaps));
    D3DRenderParams.setsDefaultZEnableAtBeginScene = 1;
    if (D3DRenderParams.presentParams.BackBufferCount > 1)
    {
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);
        D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, 0, 0, 0, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);
    D3DRenderParams.d3dDev->lpVtbl->Present(D3DRenderParams.d3dDev, 0, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    D3D_InitEverything(dd);
    D3D_SunSurfacesClear();
    libHandle = LoadLibraryA("d3d9.dll");
    if (libHandle)
    {
        DisableD3DSpy = (void (*)(void))GetProcAddress(libHandle, "DisableD3DSpy");
        if (DisableD3DSpy)
            DisableD3DSpy();
    }
    return 0;
}

void D3D_SetRenderTargetAndDepthStencilSurface()
{
    IDirect3DSurface9* surface;

    /* Without a device this renderer was not the one chosen. The
       engine calls in here directly rather than through the
       interface, so this runs whichever renderer is in force, and
       the only right answer is to do nothing. */
    if (!D3DRenderParams.d3dDev)
        return;

    D3DRenderParams.blurTex->lpVtbl->GetSurfaceLevel(D3DRenderParams.blurTex, 0, &surface);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surface);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, D3DRenderParams.newZStencil);
    surface->lpVtbl->Release(surface);
}

void D3D_BeginSceneCall()
{
    /* Without a device this renderer was not the one chosen. The
       engine calls in here directly rather than through the
       interface, so this runs whichever renderer is in force, and
       the only right answer is to do nothing. */
    if (!D3DRenderParams.d3dDev)
        return;

    D3D_SetRenderTargetAndDepthStencilSurface();
    if (!D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->BeginScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 1;
    }
    D3DRenderParams.d3d_defaultZEnable = D3DRenderParams.setsDefaultZEnableAtBeginScene;
}

void D3D_Request_SetFogParams(tFogParams* fog)
{
    tFog_Source fogSrc;
    tFog_State  fogState;

    fogSrc.b_enabled = (fog->flags & 1) != 0;
    fogSrc.engineCurve = fog->fogVertexMode;
    fogSrc.start = fog->fogStart;
    fogSrc.end = fog->fogEnd;
    fogSrc.density = fog->density;
    fogSrc.engineColor = (unsigned int)fog->fogColor;

    if ((fog->flags & 1) != 0)
    {
        if ((D3DRenderParams.d3dcaps.RasterCaps & 0x100) != 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dFogTableMode, D3DRS_FOGTABLEMODE, engineFogTableModeToD3D[fog->fogVertexMode]);
        }
        else if ((D3DRenderParams.d3dcaps.RasterCaps & 0x80) != 0)
        {
            D3D_RenderStateSet(&renderStateConfig.d3dFogVertexMode, D3DRS_FOGVERTEXMODE, engineFogTableModeToD3D[fog->fogVertexMode]);
        }
        else
        {
            D3DRenderParams.d3dFogModeProbably = 0;
            D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
            return;
        }
        /* The curve and the colour come from fog.c, so the engine's
           numbering and its byte order are read in one place rather
           than once per renderer. Which device constant the curve
           becomes is still this renderer's own table. */
        FOG_Decode(&fogSrc, &fogState);
        D3DRenderParams.d3dFogColor = fogState.color;
        D3DRenderParams.d3dFogColor2Case5and6 = fogState.color2;
        D3D_RenderStateSetF(&renderStateConfig.d3dFogDensity, D3DRS_FOGDENSITY, fog->density);
        D3DRenderParams.d3dFogModeProbably = 1;
        D3D_RenderStateSet(&renderStateConfig.d3dFogColor, D3DRS_FOGCOLOR, D3DRenderParams.d3dFogColor);
        D3D_RenderStateSetF(&renderStateConfig.d3dFogStart, D3DRS_FOGSTART, fog->fogStart);
        D3D_RenderStateSetF(&renderStateConfig.d3dFogEnd, D3DRS_FOGEND, fog->fogEnd);
        D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, TRUE);
        return;
    }
    D3DRenderParams.d3dFogModeProbably = 0;
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
}

int D3D_FillWaterStreamData(struct sWaterParamsGlobalStruct* water)
{
    int align;
    tWms_Source src;
    tVertex_type_water* out;

    waterStreamData->lpVtbl->Lock(waterStreamData, 0, 0, (void**)&out, 0x2000);
    // align 16
    align = (uint8_t)(size_t)out & 0xF;
    waterOffsetInBytes = 16 - align;
    // preserved: align is masked to 0..15, so this test can never fail
    if (align != 16)
        out = (tVertex_type_water*)((uint8_t*)out + 16 - align);

    /* The blur runs over the wave array before anything reads it, so
       it stays ahead of the fill rather than inside the shared code,
       which takes the array as it finds it. */
    if (tweak_water_blur_normal)
        WTR_WaterBlurNormal(water->vecsForUVs, water->horizontalVertexNum, water->verticallVertexNum);

    src.positions = water->vertices;
    src.waves = water->vecsForUVs;
    src.colors = water->colors;
    src.columns = water->horizontalVertexNum;
    src.rows = water->verticallVertexNum;
    src.camera = gpst_DX_stDD->ct.cam.objectMtx.Pos;
    WMS_i_Vertices(&src, (tWms_Vertex*)out,
                   (int)water->horizontalVertexNum * water->verticallVertexNum);

    return waterStreamData->lpVtbl->Unlock(waterStreamData);
}

void D3D_SetTransformAndSaveMatrix(IDirect3DTexture9* texture, int mtxnum)
{
    IDirect3DSurface9* surface;
    D3DSURFACE_DESC desc;
    D3DMATRIX result;

    texture->lpVtbl->GetSurfaceLevel(texture, 0, &surface);
    surface->lpVtbl->GetDesc(surface, &desc);
    surface->lpVtbl->Release(surface);
    /* The remap has no device in it either, so the other renderer
       projects its reflection through the same arithmetic. */
    WMS_ProjectionToTexture((Mtx4x4WithoutAdditionalData*)&result,
                            (const Mtx4x4WithoutAdditionalData*)&d3dProjectionMtx,
                            (int)desc.Width, (int)desc.Height);
    memcpy(&d3dTransformationMatrix[mtxnum], &result, sizeof(Mtx4x4WithoutAdditionalData));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, mtxnum + D3DTS_TEXTURE0, &result);
}

IDirect3DTexture9* D3D_GetWaterTexture()
{
    water_ClippingHasBeenSet = 0;
    if (gMaxSimultaneousTextures >= 4 && tweak_water_smooth)
        D3D_DoBlurWaterTexture();
    return waterTexture;
}


void D3D_DrawWater_DrawVertexTable(struct sWaterParamsGlobalStruct* water)
{
    IDirect3DTexture9* texture;
    Mtx4x4 camMtx;
    Mtx4x4 invCamMtx;
    Mtx4x4 viewMtx;
    Mtx4x4 worldMtx;

    /* Without a device this renderer was not the one chosen. The
       engine calls in here directly rather than through the
       interface, so this runs whichever renderer is in force, and
       the only right answer is to do nothing. */
    if (!D3DRenderParams.d3dDev)
        return;

    wtr_NumB = water->verticallVertexNum;
    wtr_NumA = water->horizontalVertexNum;
    D3D_WaterBuffersInit();
    D3D_FillWaterStreamData(water);
    if (&camMtx != &gpst_DX_stDD->ct.cam.objectMtx)
        memcpy(&camMtx, &gpst_DX_stDD->ct.cam.objectMtx, sizeof(camMtx));
    MATH_InvertMatrix(&invCamMtx, &camMtx);
    invCamMtx._m34 = 0.0;
    invCamMtx._m24 = 0.0;
    invCamMtx._m14 = 0.0;
    invCamMtx.w = 1.0;
    MATH_MakeOGLMatrix(&viewMtx, &invCamMtx);
    camMtx.v1.x = 1.0;
    camMtx.v2.y = 1.0;
    camMtx.v3.z = 1.0;
    camMtx.w = 1.0;
    viewMtx.w = 1.0;
    camMtx.v1.y = 0.0;
    camMtx.v1.z = 0.0;
    camMtx._m14 = 0.0;
    camMtx.v2.x = 0.0;
    camMtx.v2.z = 0.0;
    camMtx._m24 = 0.0;
    camMtx.v3.x = 0.0;
    camMtx.v3.y = 0.0;
    camMtx._m34 = 0.0;
    camMtx.Pos.x = 0.0;
    camMtx.Pos.y = 0.0;
    camMtx.Pos.z = 0.0;
    camMtx.mtxflag = camMtx.mtxflag & 2;
    MATH_MakeOGLMatrix(&worldMtx, &camMtx);
    D3D_SaveEverythingToBackup(1);
    texture = D3D_GetWaterTexture();
    memcpy(&d3dTransformMatrix, &viewMtx, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&viewMtx);
    memcpy(&d3dTransformMatrix2, &worldMtx, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, (D3DMATRIX*)&worldMtx);
    D3D_SetTransformAndSaveMatrix(texture, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTexCoordIndex, 11, 0x10000);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3dTextureTransformFlags, 24, 0x103);
    D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals, 143, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, waterStreamData, waterOffsetInBytes, 32);
    D3DRenderParams.d3dDev->lpVtbl->SetIndices(D3DRenderParams.d3dDev, waterIndexData);
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, 210);
    D3D_RenderStateSet(&renderStateConfig.d3dAlphaBlendEnable, 27, 1);
    D3D_RenderStateSet(&renderStateConfig.d3d_Lighting, 137, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorOp, 1, 12);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1, 2, 2);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2, 3, 0);
    D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_alphaOp, 4, 1);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_magfilter, 5, 2);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_minfilter, 6, 2);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressu, 1, 3);
    D3D_SamplerStateSet(0, &samplerConfig[0].d3d_addressv, 2, 3);
    D3D_RenderStateSet(&renderStateConfig.d3dSrcBlend, 19, 2);
    D3D_RenderStateSet(&renderStateConfig.d3dDestBlend, 20, 1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    D3D_RenderStateSet(&renderStateConfig.d3dColorWriteEnable, 168, 15);
    D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, 28, 1);
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, 14, 0);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, 23, 4);
    //D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    D3DRenderParams.d3dDev->lpVtbl->DrawIndexedPrimitive(D3DRenderParams.d3dDev,
        5,
        0,
        0,
        water->verticallVertexNum * water->horizontalVertexNum,
        0,
        wtr_primitiveCount);
    //D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_SOLID);
    D3D_RestoreEverythingFromBackup();
    D3D_RenderStateSet(&renderStateConfig.d3d_zWriteEnable, 14, 1);
    D3D_RenderStateSet(&renderStateConfig.d3d_zFunc, 23, 4);
}

void D3D_PrepareWaterRendering(void)
{
    float clipPlane[4];

    /* Without a device this renderer was not the one chosen. The
       engine calls in here directly rather than through the
       interface, so this runs whichever renderer is in force, and
       the only right answer is to do nothing. */
    if (!D3DRenderParams.d3dDev)
        return;

    if (pst_GlobalsWaterParams)
    {
        QueryPerformanceCounter(&PerfCounterStruct[48]);
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &d3dRTToGet);
        D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &d3dWaterDSSurfaceToGet);
        D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dWaterRTtoSet);
        D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dDSSurfaceToSet);
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, 0, 3, pst_GlobalsWaterParams->waterEngine.bottomColor, 1.0f, 0);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dRTToGet);
        D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dWaterDSSurfaceToGet);
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
        if (d3dRTToGet)
        {
            d3dRTToGet->lpVtbl->Release(d3dRTToGet);
            d3dRTToGet = NULL;
        }
        if (d3dWaterDSSurfaceToGet)
        {
            d3dWaterDSSurfaceToGet->lpVtbl->Release(d3dWaterDSSurfaceToGet);
            d3dWaterDSSurfaceToGet = NULL;
        }
        MATH_MakeOGLMatrix(&d3dCameraMtx, &gpst_DX_stDD->ct.cam.cameraMtx);
        if (tweak_water_clip)
        {
            /* Shared with the other renderer: the plane has no device
               in it either. */
            WRF_ClipPlane(clipPlane, waterHeight, tweak_water_off);
            D3DRenderParams.d3dDev->lpVtbl->SetClipPlane(D3DRenderParams.d3dDev, 0, clipPlane);
        }
        PRO_QPC_RecordTimeDecRef(48);
        water_ClippingHasBeenSet = 1;
    }
    else
    {
        water_ClippingHasBeenSet = 0;
    }
}

int D3D_setWaterRenderParamsIfWaterRenderingIsActive()
{
    tGameObjectBV* bv;
    Mtx4x4 worldMtx;

    if (!water_ClippingHasBeenSet)
        return 0;
    bv = gpst_DX_stDD->currTargetGao->boundingVolume;
    /* Whether this object earns a second draw has no device in it, so
       it is the shared reflection code's answer and the other renderer
       asks the same question the same way. */
    if (!WRF_b_WorthReflecting(bv, OBJ_BV_IsSphere(bv), tweak_water_min_bv))
        return 0;
    QueryPerformanceCounter(&PerfCounterStruct[48]);
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &d3dRTToGet);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &d3dWaterDSSurfaceToGet);
    D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dWaterRTtoSet);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dDSSurfaceToSet);
    d3dCullModeBackup = renderStateConfig.d3dCullMode;
    if (renderStateConfig.d3dCullMode == 2)
        D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, 3);
    else if (renderStateConfig.d3dCullMode == 3)
        D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, 2);
    memcpy(&d3dTransformMatrixBackup, &d3dTransformMatrix, sizeof(d3dTransformMatrix));
    memcpy(&d3dTransformMatrix2Backup, &d3dTransformMatrix2, sizeof(d3dTransformMatrix2));
    /* Shared with the other renderer, for the same reason. */
    WRF_Mirror(&worldMtx, gpst_DX_stDD->currTargetGao->matrices, waterHeight);
    memcpy(&d3dTransformMatrix2, &worldMtx, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, (D3DMATRIX*)&worldMtx);
    memcpy(&d3dTransformMatrix, &d3dCameraMtx, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, (D3DMATRIX*)&d3dCameraMtx);
    if (tweak_water_clip)
    {
        D3D_RenderStateSet(&renderStateConfig.d3dClipPlaneEnable, 152, 1);
    }
    return 1;
}

void D3D_RestoreRenderParamsAfterWaterRendering()
{
    if (tweak_water_clip && renderStateConfig.d3dClipPlaneEnable)
    {
        renderStateConfig.d3dClipPlaneEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 152, 0);
    }
    memcpy(&d3dTransformMatrix, &d3dTransformMatrixBackup, sizeof(d3dTransformMatrix));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_VIEW, &d3dTransformMatrixBackup);
    memcpy(&d3dTransformMatrix2, &d3dTransformMatrix2Backup, sizeof(d3dTransformMatrix2));
    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, &d3dTransformMatrix2Backup);
    D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, d3dCullModeBackup);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, d3dRTToGet);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, d3dWaterDSSurfaceToGet);
    D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &d3dViewPort);
    if (d3dRTToGet)
    {
        d3dRTToGet->lpVtbl->Release(d3dRTToGet);
        d3dRTToGet = NULL;
    }
    if (d3dWaterDSSurfaceToGet)
    {
        d3dWaterDSSurfaceToGet->lpVtbl->Release(d3dWaterDSSurfaceToGet);
        d3dWaterDSSurfaceToGet = NULL;
    }
    PRO_QPC_RecordTimeDecRef(48);
}

tVertex_type_142_generic* D3D_LockCurrentVBO(int vertexNum)
{
    int offset;
    int flags;
    tVertex_type_142_generic* locked;

    offset = d3dStreamCurrentOffset;
    flags = 4096;
    if ((unsigned int)(d3dStreamCurrentOffset + sizeof(tVertex_type_142_generic) * vertexNum) >= 2097152)
    {
        offset = 0;
        d3dStreamCurrentOffset = 0;
        flags = 8192;
    }
    VBOs[currentVBOSelected]->lpVtbl->Lock(VBOs[currentVBOSelected], offset, sizeof(tVertex_type_142_generic) * vertexNum, (void **)&locked, flags);
    d3dStreamActDataSize = sizeof(tVertex_type_142_generic) * vertexNum;
    return locked;
}

void D3D_FillCurrentVBO(int stream)
{
    VBOs[currentVBOSelected]->lpVtbl->Unlock(VBOs[currentVBOSelected]);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, stream, VBOs[currentVBOSelected], d3dStreamCurrentOffset, sizeof(tVertex_type_142_generic));
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ);
    d3dStreamCurrentOffset += d3dStreamActDataSize;
    d3dStreamActDataSize = 0;
}

void D3D_Request_SpriteDraw()
{
    if (d3d_requestDrawSelectedVBO)
    {
        d3d_requestDrawSelectedVBO = NULL;
        D3D_FillCurrentVBO(0);
        D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, d3dRequestPrimitiveCount);
    }
}

void D3D_Request_PrepareForDrawing(int spriteNum)
{
    if (spriteNum)
    {
        D3D_RenderStateSet(&renderStateConfig.d3dFillMode, 8, 3);
        D3D_RenderStateSet(&renderStateConfig.d3dCullMode, 22, 2);
        d3dRequestPrimitiveCount = 2 * spriteNum;
        d3dRequestPrimitiveCount2 = 2 * spriteNum;
        d3d_requestDrawSelectedVBO = D3D_LockCurrentVBO(6 * spriteNum);
    }
}

void D3D_FillSelectedVBOWithSprite(tD3dSpriteData* sprite)
{
    tVertex_type_142_generic* out;
    unsigned int color;

    out = d3d_requestDrawSelectedVBO;
    if (d3d_requestDrawSelectedVBO && d3dRequestPrimitiveCount2)
    {
        color = sprite->color & 0xFF00FF00 | ((sprite->color >> 16) & 0xff) | ((uint8_t)sprite->color << 16);
        out[0].v1 = sprite->v1;
        out[0].color = color;
        out[0].uvSet1.u = 1.0f;
        out[0].uvSet1.v = 1.0f;
        out[1].v1 = sprite->v2;
        out[1].color = color;
        out[1].uvSet1.u = 0.0f;
        out[1].uvSet1.v = 1.0f;
        out[2].v1 = sprite->v3;
        out[2].color = color;
        out[2].uvSet1.u = 0.0f;
        out[2].uvSet1.v = 0.0f;
        out[3].v1 = sprite->v1;
        out[3].color = color;
        out[3].uvSet1.u = 1.0f;
        out[3].uvSet1.v = 1.0f;
        out[4].v1 = sprite->v3;
        out[4].color = color;
        out[4].uvSet1.u = 0.0f;
        out[4].uvSet1.v = 0.0f;
        out[5].v1 = sprite->v4;
        out[5].color = color;
        out[5].uvSet1.u = 1.0f;
        out[5].uvSet1.v = 0.0f;
        d3d_requestDrawSelectedVBO = out + 6;
        d3dRequestPrimitiveCount2 -= 2;
    }
}

int D3D_l_Request(int type, void *param)
{
    switch (type)
    {
    case 1:
        D3D_Request_SetFogParams((tFogParams*)param);
        break;
    case 12:
        D3D_TextureStore((tTexUploadRequest*)param);
        break;
    case 18:
        D3D_Request_PrepareForDrawing((int)(size_t)param);
        break;
    case 19:
    case 33:
        D3D_FillSelectedVBOWithSprite((tD3dSpriteData*)param);
        break;
    case 20:
        D3D_Request_SpriteDraw();
        break;
    case 24:
        D3D_GetSnapShot((tGdi_SnapshotRequest*)param);
        break;
    case 26:
        D3D_SaveScreenshot();
        break;
    case 25:
        D3D_RenderStateSet(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, (int)(size_t)param);
        break;
    default:
        break;
    }
    return 0;
}

int D3D_l_Texture_Store(struct sGdi_DisplayData* dd, int textureNum)
{
    int count;
    unsigned int bytes;

    count = textureNum;
    if (!textureNum)
        count = 1;
    bytes = 4 * (count + 1);
    D3DRenderParams.d3dTextureNum = count;
    D3DRenderParams.d3dTextures = (IDirect3DTexture9 **)MEM_p_Alloc(bytes);
    memset(D3DRenderParams.d3dTextures, 0, bytes);
    D3DRenderParams.d3dSourceTextures = (IDirect3DTexture9**)MEM_p_Alloc(bytes);
    memset(D3DRenderParams.d3dSourceTextures, 0, bytes);
    return 1;
}

void D3D_Palette_Load(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* inter, int idx)
{
    int depth;
    int width;
    int height;
    int16_t texFlags;
    int mipLevels;
    int result;
    IDirect3DTexture9* source;
    IDirect3DTexture9* dest;
    char* srcRow;
    char* destRow;
    unsigned int rowBytes;
    int row;
    IDirect3DSurface9* surface;
    char* pixels;
    D3DLOCKED_RECT locked;

    depth = inter->colorDepth2;
    width = tle->normalData.width;
    pixels = inter->imgDataPtr;
    height = tle->normalData.height;
    if (!D3DRenderParams.Mipmapping
        || (texFlags = inter->tp.flagsUnkAndProceduralUnkb1, (texFlags & 0x40) != 0)
        || (texFlags & 0x200) != 0
        || inter->tp.type == 5)
    {
        inter->bmpUnkb1 &= ~0x10u;
        mipLevels = 1;
    }
    else
    {
        inter->bmpUnkb1 |= 0x10u;
        mipLevels = 0;
    }
    tle->texElemFlag = inter->bmpUnkb1;
    source = 0;
    dest = 0;
    if (depth != 32 && depth != 24)
    {
        D3DRenderParams.d3dTextures[idx] = NULL;
        D3DRenderParams.d3dSourceTextures[idx] = NULL;
        return;
    }
    if (D3DRenderParams.Mipmapping && D3DRenderParams.argbIsSupportedForTextures == 1)
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, height, width, 1, 0, 21, 2, &source, 0);
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev,
            height,
            width,
            mipLevels,
            0x400,
            21,
            0,
            &dest,
            0);
    }
    else
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev, height, width, mipLevels, 0, 21, 2, &source, 0);
        result = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
            D3DRenderParams.d3dDev,
            height,
            width,
            mipLevels,
            0,
            21,
            0,
            &dest,
            0);
    }
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(result);
    if (dest && source)
    {
        if (depth == 32)
            dd->texManager.paletteUsedMemory += 4 * height * width;
        else
            dd->texManager.paletteUsedMemory += 3 * height * width;
        source->lpVtbl->GetSurfaceLevel(source, 0, &surface);
        surface->lpVtbl->LockRect(surface, &locked, 0, 0);
        destRow = (char*)locked.pBits;
        srcRow = pixels;
        rowBytes = 4 * height;
        for (row = 0; row < width; row++)
        {
            memcpy(destRow, srcRow, rowBytes);
            srcRow += rowBytes;
            destRow += locked.Pitch;
        }
        surface->lpVtbl->UnlockRect(surface);
        surface->lpVtbl->Release(surface);
        if (D3DRenderParams.Mipmapping && D3DRenderParams.argbIsSupportedForTextures != 1)
            D3D_b_TextureBuildMipLevels(source);
        D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)source, (IDirect3DBaseTexture9*)dest);
    }
    else
    {
        if (dest)
        {
            dest->lpVtbl->Release(dest);
            dest = NULL;
        }
        if (source)
        {
            source->lpVtbl->Release(source);
            source = NULL;
        }
    }
    D3DRenderParams.d3dTextures[idx] = dest;
    D3DRenderParams.d3dSourceTextures[idx] = source;
}

/* Each quad goes straight into the buffer the device has locked, as
   the walk finishes it. */
static void D3D_l_EmitSprite(void* user, const tGdi_SpriteQuad* quad)
{
    tGdi_DisplayData* dd = (tGdi_DisplayData*)user;

    dd->Request(33, (void*)quad);
}

int D3D_DrawSprites(struct sGeoSprites* sprites, void* positions, int num)
{
    tBigBufferStruct* bb;
    tGsp_Source       src;

    (void)num;

    bb = gpst_DX_stDD->bigBuffer;
    src.positions = positions;
    src.colors = bb->vertexColorArray;
    src.second = bb->vertexColorArraySecond;
    src.mask = bb->colorMask;
    src.alpha = D3DRenderParams.colorMaskAlpha;
    src.solidColor = (unsigned int)bb->solidVertexColor;
    src.matrix = gpst_DX_stDD->matrixStack.actMatrixElem;
    src.waveSprite = GDI_gb_WaveSprite;

    gpst_DX_stDD->Request(18, (void*)(size_t)sprites->num);
    GSP_Walk(sprites, &src, D3D_l_EmitSprite, gpst_DX_stDD);
    gpst_DX_stDD->Request(20, 0);
    return 0;
}

/* The packing lives in geovertex.c now, shared with the vertex
   building that uses it; the other callers in this file reach it
   through here. */
static unsigned int D3D_VertexColor(int* colors, int* second, int mask, int alpha,
                                    int idx, int secondIdx)
{
    return GVX_ui_Color(colors, second, mask, alpha, idx, secondIdx);
}

static void D3D_DrawIndexedStreams(tGEOGRO* geogro, tGeoWholeTriangleStripBlock* block)
{
    unsigned int i;
    tD3dStreamInfoElem* info;

    for (i = 0; i < block->streamsNum; i++)
    {
        info = &block->d3dStreamInfoElems[i];
        if (GDI_gi_TextureUVMode != 2)
        {
            if (geogro->uvBuffers)
            {
                D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, geogro->uvBuffers[info->vbIdx], 0, 8);
                D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                    D3DRenderParams.d3dDev,
                    D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull);
            }
            else
            {
                D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, 0, 0, 0);
                D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
                    D3DRenderParams.d3dDev,
                    D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull);
            }
        }
        D3DRenderParams.d3dDev->lpVtbl->DrawIndexedPrimitive(
            D3DRenderParams.d3dDev,
            5,
            0,
            0,
            geogro->vertexNum,
            info->startIndex,
            info->primitiveCount - 2);
    }
}

int D3D_l_DrawElementIndexedTriangles(struct sGeoTriangleChunks* triangleChunks, Vec3* verticesIN, struct sGeoUVElem* UVs, int vertexNum)
{
    tGEOGRO* geogro;
    tGeoWholeTriangleStripBlock* block;
    tBigBufferStruct* bb;
    tVertex_type_142_generic* out;
    unsigned int triangleNum;
    int immediate;
    int mismatch;
    int useVertexUv;
    int* colors;
    int* second;
    int mask;
    int alpha;
    int solid;
    int savedArg1;
    int savedArg2;
    int start;
    int lockFlags;
    unsigned int i;
    void* locked;
    unsigned int* lockedColors;
    unsigned int solidColor = 0;// TODO FIXME ???????? játék nem initeli

    triangleNum = triangleChunks->triangleNum;
    if (!triangleNum)
        return 0;
    geogro = gpst_DX_stDD->GROGEO;
    block = NULL;
    immediate = 1;
    if (geogro)
    {
        mismatch = 0;
        if (geogro->UVs != UVs && GDI_gi_TextureUVMode != 2)
            mismatch = 1;
        if (verticesIN == geogro->vertices && (gpst_DX_stDD->flags & 0x400) != 0)
            mismatch = 1;
        block = triangleChunks->triangleStripBlock;
        immediate = !geogro->indexBuffer || !block || mismatch;
    }
    /* The chrome mapping reads the normal after the matrix that places
       the object has been applied to it, and that matrix carries the
       object's scale -- so what arrives is no longer a unit vector,
       and the mapping is then read off a length as much as off a
       direction. The hardware renormalises only when it is asked to,
       and only this mode needs it; the other renderer does the same
       thing per pixel in its shader. Both ways of drawing generate the
       coordinate in hardware, so this sits above the branch. */
    if (tweak_d3d_chrome)
        D3D_RenderStateSet(&renderStateConfig.d3d_normalizeNormals,
                           D3DRS_NORMALIZENORMALS,
                           GDI_gi_TextureUVMode == 2);
    if (immediate)
    {
        tGvx_Source src;

        savedArg1 = 0;
        savedArg2 = 0;
        out = D3D_LockCurrentVBO(3 * triangleNum);
        bb = gpst_DX_stDD->bigBuffer;
        second = bb->vertexColorArraySecond;
        /* the engine holds the renderer's state opaquely; inside the
           renderer it is this renderer's own struct */
        alpha = ((tGdi_D3DRenderParams*)gpst_DX_stDD->rendererState)->colorMaskAlpha;
        mask = bb->colorMask;
        colors = bb->vertexColorArray;
        if (!colors)
            solidColor = GVX_ui_SolidColor(mask, alpha, bb->solidVertexColor);
        if (GDI_gi_TextureUVMode == 2)
        {
            // preserved: the colour arg 2 backup is taken from arg 1
            savedArg1 = textureStageTypeConfig[0].d3d_colorarg1;
            savedArg2 = textureStageTypeConfig[0].d3d_colorarg1;
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1, 2, 0);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2, 3, 0);
            colors = 0;
            solidColor = 0;
        }
        useVertexUv = (gpst_DX_stDD->flags & 2) != 0;

        /* The vertex building is geovertex.c's, because none of it is
           this renderer's: the engine's arrays are separate and the
           vertices interleaved, and the colour packing is not
           something to write twice. */
        src.vertices = verticesIN;
        src.uvs = UVs;
        src.colors = colors;
        src.second = second;
        src.mask = mask;
        src.alpha = alpha;
        src.solidColor = solidColor;
        src.useVertexUv = useVertexUv;
        GVX_BuildTriangles((tGdi_TexturedVertex*)out, triangleChunks, &src);

        D3D_FillCurrentVBO(0);
        D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, triangleNum);
        if (GDI_gi_TextureUVMode != 5
            && GDI_gi_TextureUVMode != 4
            && !D3DRenderParams.renderTargetBackup
            && D3D_setWaterRenderParamsIfWaterRenderingIsActive())
        {
            D3DRenderParams.d3dDev->lpVtbl->DrawPrimitive(D3DRenderParams.d3dDev, 4, 0, triangleNum);
            D3D_RestoreRenderParamsAfterWaterRendering();
        }
        if (GDI_gi_TextureUVMode == 2)
        {
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg1, 2, savedArg1);
            D3D_StageStateSet(0, &textureStageTypeConfig[0].d3d_colorarg2, 3, savedArg2);
        }
        return 0;
    }
    if (verticesIN == geogro->vertices)
    {
        D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, geogro->vertexBuffer, 0, 12);
    }
    else
    {
        if (D3DRenderParams.currTargetGao != gpst_DX_stDD->currTargetGao)
        {
            if ((unsigned int)(D3DRenderParams.prevVertexElemIdx2 + geogro->vertexNum) <= 0xFFFF)
            {
                start = D3DRenderParams.prevVertexElemIdx2;
                D3DRenderParams.startVertexElemIdx2 = D3DRenderParams.prevVertexElemIdx2;
                D3DRenderParams.prevVertexElemIdx2 += geogro->vertexNum;
                lockFlags = 4096;
            }
            else
            {
                start = 0;
                D3DRenderParams.startVertexElemIdx2 = 0;
                D3DRenderParams.prevVertexElemIdx2 = geogro->vertexNum;
                lockFlags = 0x2000;
            }
            D3DRenderParams.currTargetGao = gpst_DX_stDD->currTargetGao;
            D3DRenderParams.d3dXYZVB->lpVtbl->Lock(
                D3DRenderParams.d3dXYZVB,
                start * D3DRenderParams.stride2,
                D3DRenderParams.stride2 * geogro->vertexNum,
                &locked,
                lockFlags);
            memcpy(locked, verticesIN, sizeof(Vec3) * geogro->vertexNum);
            D3DRenderParams.d3dXYZVB->lpVtbl->Unlock(D3DRenderParams.d3dXYZVB);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
            D3DRenderParams.d3dDev,
            0,
            D3DRenderParams.d3dXYZVB,
            D3DRenderParams.startVertexElemIdx2 * D3DRenderParams.stride2,
            D3DRenderParams.stride2);
    }
    bb = gpst_DX_stDD->bigBuffer;
    mask = bb->colorMask;
    colors = bb->vertexColorArray;
    solid = bb->solidVertexColor;
    alpha = D3DRenderParams.colorMaskAlpha;
    if (D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 != gpst_DX_stDD->currTargetGao
        || D3DRenderParams.vertexColorArray != colors
        || D3DRenderParams.colorMask != mask
        || D3DRenderParams.colorMaskAlpha2 != D3DRenderParams.colorMaskAlpha
        || D3DRenderParams.solidVertexColor != solid)
    {
        if ((unsigned int)(D3DRenderParams.prevVertexElemIdx + geogro->vertexNum) <= 0x3FFFF)
        {
            D3DRenderParams.startVertexElemIdx = D3DRenderParams.prevVertexElemIdx;
            D3DRenderParams.prevVertexElemIdx += geogro->vertexNum;
            lockFlags = 4096;
        }
        else
        {
            D3DRenderParams.startVertexElemIdx = 0;
            D3DRenderParams.prevVertexElemIdx = geogro->vertexNum;
            lockFlags = 0x2000;
        }
        D3DRenderParams.colorMaskAlpha2 = D3DRenderParams.colorMaskAlpha;
        D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle2 = gpst_DX_stDD->currTargetGao;
        D3DRenderParams.vertexColorArray = colors;
        D3DRenderParams.colorMask = mask;
        D3DRenderParams.solidVertexColor = solid;
        D3DRenderParams.d3dColorsVB->lpVtbl->Lock(D3DRenderParams.d3dColorsVB, D3DRenderParams.startVertexElemIdx * D3DRenderParams.stride, D3DRenderParams.stride * geogro->vertexNum, (void**)&lockedColors, lockFlags);

        /*
        if (gpst_DX_stDD->currTargetGao == (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x620031d4)) {
            v10 = gpst_DX_stDD->RLIPtr;
            //OutputDebugStringA("itt\n");
            //((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->RLIVertexColors
            int iii;
            //if (v10) v10 = ((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->RLIVertexColors + 1;
            //DBG_OutputDebugString("buf: %p\n", v10);
            if (v10) {
                DBG_OutputDebugString("buf: ");
                for (iii = 0; iii < v4->vertexNum; iii++) {
                    DBG_OutputDebugString("%x ", v10[iii]);
                }
                DBG_OutputDebugString("\n\n");
            }
        }
        */

        if (colors)
        {
            for (i = 0; i < geogro->vertexNum; i++)
            {
                unsigned int packed = mask ^ (alpha | colors[i]);
                lockedColors[i] = packed & 0xFF00FF00 | ((uint8_t)packed << 16) | ((packed >> 16) & 0xff);
            }
        }
        else if (geogro->vertexNum)
        {
            for (i = 0; i < (((4 * geogro->vertexNum - 1) >> 2) + 1); i++)
            {
                lockedColors[i] = (mask ^ (alpha | gpst_DX_stDD->bigBuffer->solidVertexColor)) & 0xFF00FF00 | ((uint8_t)(mask ^ (alpha | (gpst_DX_stDD->bigBuffer->solidVertexColor & 0xff))) << 16) | (uint8_t)((mask ^ ((unsigned int)alpha | gpst_DX_stDD->bigBuffer->solidVertexColor)) >> 16);
            }
        }
        D3DRenderParams.d3dColorsVB->lpVtbl->Unlock(D3DRenderParams.d3dColorsVB);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
        D3DRenderParams.d3dDev,
        1,
        D3DRenderParams.d3dColorsVB,
        D3DRenderParams.startVertexElemIdx * D3DRenderParams.stride,
        D3DRenderParams.stride);
    if (GDI_gi_TextureUVMode == 2)
    {
        if (verticesIN == geogro->vertices)
        {
            D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, geogro->vertexBuffer2, 0, 12);
        }
        else
        {
            if (D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle != gpst_DX_stDD->currTargetGao)
            {
                if ((unsigned int)(D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle + geogro->vertexNum) <= 0xFFFF)
                {
                    start = D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle;
                    D3DRenderParams.currVertexStartOffset = D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle;
                    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle += geogro->vertexNum;
                    lockFlags = 4096;
                }
                else
                {
                    start = 0;
                    D3DRenderParams.currVertexStartOffset = 0;
                    D3DRenderParams.sumVertexNumInDrawElementIndexedTriangle = geogro->vertexNum;
                    lockFlags = 8192;
                }
                D3DRenderParams.currTargetGaoInDrawElementIndexedTriangle = gpst_DX_stDD->currTargetGao;
                D3DRenderParams.d3dNormalsBuffer->lpVtbl->Lock(D3DRenderParams.d3dNormalsBuffer, start * D3DRenderParams.d3dStride, D3DRenderParams.d3dStride * geogro->vertexNum, &locked, lockFlags);
                memcpy(locked, geogro->normals, 12 * geogro->vertexNum);
                D3DRenderParams.d3dNormalsBuffer->lpVtbl->Unlock(D3DRenderParams.d3dNormalsBuffer);
            }
            D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(
                D3DRenderParams.d3dDev,
                2,
                D3DRenderParams.d3dNormalsBuffer,
                D3DRenderParams.currVertexStartOffset * D3DRenderParams.d3dStride,
                D3DRenderParams.d3dStride);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetVertexDeclaration(
            D3DRenderParams.d3dDev,
            D3DRenderParams.d3dVertexDeclaration);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 3, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetIndices(D3DRenderParams.d3dDev, geogro->indexBuffer);
    D3D_DrawIndexedStreams(geogro, block);
    if (GDI_gi_TextureUVMode != 5 && GDI_gi_TextureUVMode != 4
        && !D3DRenderParams.renderTargetBackup
        && D3D_setWaterRenderParamsIfWaterRenderingIsActive())
    {
        D3D_DrawIndexedStreams(geogro, block);
        D3D_RestoreRenderParamsAfterWaterRendering();
    }
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 0, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 1, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 2, 0, 0, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetStreamSource(D3DRenderParams.d3dDev, 3, 0, 0, 0);
    return 0;
}

void D3D_SetRenderTargetAndBackupPreviousSDW(int textureID, int clear)
{
    IDirect3DSurface9* shadowTexSurface;
    D3DVIEWPORT9 viewPort = {
        .X = 1, 
        .Y = 1, 
        .Width = 126, 
        .Height = 126, 
        .MinZ = 0.0f, 
        .MaxZ = 1.0f
    };

    if (textureID == -1)
    {
        if (D3DRenderParams.renderTargetBackup)
        {
            D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, D3DRenderParams.renderTargetBackup);
            D3DRenderParams.renderTargetBackup->lpVtbl->Release(D3DRenderParams.renderTargetBackup);
            D3DRenderParams.renderTargetBackup = NULL;
            memcpy(&d3dProjectionMtx, &(D3DRenderParams.projectionMatrixBackup), sizeof(d3dProjectionMtx));
            D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &(D3DRenderParams.projectionMatrixBackup));
            D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &(D3DRenderParams.pViewPort));
        }
    }
    else if (!D3DRenderParams.renderTargetBackup)
    {
        D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &(D3DRenderParams.renderTargetBackup));
        D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &(D3DRenderParams.pViewPort));
        D3DRenderParams.d3dShadowTextures[textureID]->lpVtbl->GetSurfaceLevel(D3DRenderParams.d3dShadowTextures[textureID], 0, &shadowTexSurface);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, shadowTexSurface);
        shadowTexSurface->lpVtbl->Release(shadowTexSurface);
        
        if (clear)
            D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 0, 0, 1, 0, 1.0f, 0);
        
        D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &viewPort);
    }
}

void D3D_SetViewMatrix_SDW(Mtx4x4* mtx, float* unkVals)
{
    D3DMATRIX identityMtx = { ._11 = 1.0f, ._12 = 0.0f, ._13 = 0.0f, ._14 = 0.0f,
                              ._21 = 0.0f, ._22 = 1.0f, ._23 = 0.0f, ._24 = 0.0f,
                              ._31 = 0.0f, ._32 = 0.0f, ._33 = 1.0f, ._34 = 0.0f,
                              ._41 = 0.0f, ._42 = 0.0f, ._43 = 0.0f, ._44 = 1.0f
                            };

    memcpy(&(D3DRenderParams.projectionMatrixBackup), &d3dProjectionMtx, sizeof(D3DRenderParams.projectionMatrixBackup));
    memcpy(&d3dTransformMatrix2, &identityMtx, sizeof(d3dTransformMatrix2));

    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_WORLD, &identityMtx);
    memcpy(&d3dProjectionMtx, &identityMtx, sizeof(d3dProjectionMtx));

    D3DRenderParams.d3dDev->lpVtbl->SetTransform(D3DRenderParams.d3dDev, D3DTS_PROJECTION, &identityMtx);
    D3D_SetViewMatrix(mtx);
}

void D3D_Texture_Unload(struct sGdi_DisplayData* dd, struct sEngPaletteElem* palette, int idx) {
}

int D3D_l_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* gdd) {
    return 0;
}

int  D3D_Set_Texture_Palette(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* inter, int idx) {
    return 0;
}

void D3D_SetTextureTarget(struct sGdi_DisplayData* dd, int target, int width, int height) {
}

/* The window, its procedure and the message loop used to be here --
   six functions, none of them this renderer's business and all of them
   Windows'. They are platform.c now, where the other renderer can
   share them and where the window is SDL's. */

void D3D_SaveScreenshot(void)
{
    IDirect3DSurface9* target;
    IDirect3DSurface9* copy;
    D3DSURFACE_DESC desc;
    D3DLOCKED_RECT locked;
    char path[512];

    if (!D3DRenderParams.d3dDev)
        return;
    if (D3DRenderParams.d3dNeedToCallEndScene)
    {
        D3DRenderParams.d3dDev->lpVtbl->EndScene(D3DRenderParams.d3dDev);
        D3DRenderParams.d3dNeedToCallEndScene = 0;
    }
    target = NULL;
    if (D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &target) < 0 || !target)
        return;
    target->lpVtbl->GetDesc(target, &desc);
    if (desc.Format != D3DFMT_A8R8G8B8 && desc.Format != D3DFMT_X8R8G8B8)
    {
        GDI_OutputDebugString("screenshot: back buffer format %d is not 32 bit\n", (int)desc.Format);
        target->lpVtbl->Release(target);
        return;
    }
    copy = NULL;
    if (D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(D3DRenderParams.d3dDev,
            desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &copy, 0) < 0 || !copy)
    {
        target->lpVtbl->Release(target);
        return;
    }
    if (D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev, target, copy) < 0
        || copy->lpVtbl->LockRect(copy, &locked, NULL, D3DLOCK_READONLY) < 0)
    {
        GDI_OutputDebugString("screenshot: could not read the back buffer\n");
        copy->lpVtbl->Release(copy);
        target->lpVtbl->Release(target);
        return;
    }
    /* Where it goes is the path module's rule, not this renderer's:
       the other renderer takes screenshots too and the player wants
       them in one place. */
    if (!DPT_b_ScreenshotPath(path, sizeof(path)))
    {
        GDI_OutputDebugString("screenshot: nowhere to put it\n");
        copy->lpVtbl->UnlockRect(copy);
        copy->lpVtbl->Release(copy);
        target->lpVtbl->Release(target);
        return;
    }
    if (PNG_i_WriteBgra(path, (const unsigned char*)locked.pBits,
                        locked.Pitch, (int)desc.Width, (int)desc.Height, 0))
        GDI_OutputDebugString("screenshot: %s\n", path);
    else
        GDI_OutputDebugString("screenshot: could not write %s\n", path);
    copy->lpVtbl->UnlockRect(copy);
    copy->lpVtbl->Release(copy);
    target->lpVtbl->Release(target);
}

void PC_EngineCheatFinal()
{
    tableElemStructPF* currWorld;
    profilerRasterStructElem* currRaster;

    if (GetAsyncKeyState(VK_F7) < 0)
    {
        /* Through the interface, so the renderer in use answers: this
           function is in this renderer's file but the engine calls it
           whichever renderer is running, and calling this renderer's
           screenshot directly is why the key did nothing under the
           other one. */
        if (gpst_DX_stDD && gpst_DX_stDD->Request)
            gpst_DX_stDD->Request(26, NULL);
        while (GetAsyncKeyState(VK_F7) < 0);
    }
    if (GetAsyncKeyState(VK_F6) < 0)
    {
        SOFT_ZList_Clear();
        AI_ReinitUniverse();

        for (currWorld = WOR_gst_Universe.univWorldTablePF.firstElemPtr; currWorld <= ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1; currWorld++)
        {
            if (!TAB_TableElemIsInvalid(currWorld->objectPtr)) {
                ENG_ReinitOneWorld((tWorld*)currWorld->objectPtr, 15);
            }
        }

        while (GetAsyncKeyState(VK_F6) < 0); 
    }
    if (GetAsyncKeyState(VK_F10) < 0 && GetAsyncKeyState(VK_SHIFT) < 0 && GetAsyncKeyState(VK_CONTROL) < 0)
    {
        AI_CompiledAIEnabled = AI_CompiledAIEnabled == 0;
        
        while (GetAsyncKeyState(VK_F10) < 0);
    }
    if (GetAsyncKeyState(VK_F1) < 0)
    {
        // on, to the detail page, and off again
        OVL_RastersCycleView();
        while (GetAsyncKeyState(VK_F1) < 0);
    }

    if (GetAsyncKeyState(VK_F8) < 0)
    {
        // The free camera takes the view's follow parameter and the
        // input, and gives both back on the second press.
        ECAM_Toggle();
        while (GetAsyncKeyState(VK_F8) < 0);
    }

    if (GetAsyncKeyState(VK_F9) < 0)
    {
        // Puts the free camera where the game's camera is looking,
        // once per press.
        ECAM_SyncToGameCamera();
        while (GetAsyncKeyState(VK_F9) < 0);
    }
    if (GetAsyncKeyState(VK_F2) < 0)
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff & 0x800) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff &= ~0x800;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff |= 0x800;
            }
        }
        while (GetAsyncKeyState(VK_F2) < 0);
    }
    if (GetAsyncKeyState(VK_F3) < 0)
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff & 0x10) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff &= ~0x10;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff |= 0x10;
            }
        }
        while (GetAsyncKeyState(VK_F3) < 0)
            ;
    }
    if (GetAsyncKeyState(VK_F5) < 0)
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags & 0x20000000) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags &= ~0x20000000;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags |= 0x20000000;
            }
        }
        while (GetAsyncKeyState(VK_F5) < 0);
    }
    if (GetAsyncKeyState(VK_F4) < 0)
    {
        if (PRO_rasterDetailPage)
        {
            currRaster = PRO_CurrRaster;
            if (PRO_CurrRaster)
            {
                while (1)
                {
                    currRaster = currRaster->prev;
                    if (!currRaster)
                        currRaster = DBG_profiler_first_raster;
                    if (currRaster == PRO_CurrRaster || strcasecmp(currRaster->module, PRO_CurrRaster->module))
                        break;
                }
            }
            else
            {
                currRaster = DBG_profiler_first_raster;
            }
            PRO_CurrRaster = currRaster;
        }
        while (GetAsyncKeyState(VK_F4) < 0);
    }
}