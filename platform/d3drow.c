/* The Direct3D renderer's row in the table -- see d3drow.h for why it
   is a file of its own rather than part of gdi.c.
 *
 * Only the row is here. Everything it points at is the renderer's, in
 * the files beside this one; this is the list that turns those into
 * the interface the engine draws through, and the one place where the
 * two vocabularies meet. */

#include "../common.h"
#include "../gdi.h"
#include "d3drow.h"
#include "d3drender.h"
#include "d3dvideo.h"
#include "d3dsun.h"

int D3D_b_Available(void)
{
    return 1;      /* the renderer the game shipped on */
}

void D3D_FillRenderer(struct sGdi_DisplayData* dd)
{
    /* The interface holds the renderer's state opaquely, so the one
       function that hands it out is cast here rather than every
       caller learning the real type. */
    dd->CreateDevice = (void*(*)(void))D3D_pst_CreateDevice;
    dd->DestroyDevice = (void(*)(void*))D3D_DestroyDevice;
    dd->Init = D3D_l_Init;
    dd->Close = D3D_l_Close;
    dd->ReadaptDisplay = D3D_l_ReadaptDisplay;
    dd->Clear = D3D_Clear;
    dd->Flip = D3D_Flip;
    dd->SetViewMatrix = D3D_SetViewMatrix;
    dd->SetProjectionMatrix = D3D_SetProjectionMatrix;
    dd->DrawElementIndexedTriangles = D3D_l_DrawElementIndexedTriangles;
    dd->DrawSprites = D3D_DrawSprites;
    dd->Request = D3D_l_Request;
    dd->TextureInit = D3D_l_Texture_Init;
    dd->TextureStore = D3D_l_Texture_Store;
    dd->PaletteLoad = D3D_Palette_Load;
    dd->SetTexturePalette = D3D_Set_Texture_Palette;
    dd->TextureUnload = D3D_Texture_Unload;
    dd->SetTextureTarget = D3D_SetTextureTarget;
    dd->ReleaseTextures = D3D_ReleaseTextures;
    dd->SetRenderTargetAndBackupPreviousSDW = D3D_SetRenderTargetAndBackupPreviousSDW;
    dd->SetViewMatrixSDW = D3D_SetViewMatrix_SDW;
    dd->SetZEnable = D3D_SetZEnable;
    dd->i_ZEnable = D3D_i_ZEnable;
    dd->SetZWriteEnable = D3D_SetZWriteEnable;
    dd->SetTexCoordIndex = D3D_SetTexCoordIndex;
    dd->SetTextureTransformFlags = D3D_SetTextureTransformFlags;
    dd->SetTexCoordMatrix = D3D_SetTexCoordMatrix;
    dd->b_VideoDisplayCreate = D3D_b_VideoDisplayCreate;
    dd->VideoDisplayClose = D3D_VideoDisplayClose;
    dd->b_VideoFrameLock = D3D_b_VideoFrameLock;
    dd->VideoFrameUnlock = D3D_VideoFrameUnlock;
    dd->VideoFramePresent = D3D_VideoFramePresent;
    dd->p_SunSamplerCreate = D3D_p_SunSamplerCreate;
    dd->SunSamplerDestroy = D3D_SunSamplerDestroy;
    dd->b_SunSamplerReady = D3D_b_SunSamplerReady;
    dd->b_SunSamplingNow = D3D_b_SunSamplingNow;
    dd->SunSampleQueue = D3D_SunSampleQueue;
    dd->b_SunSampleRead = D3D_b_SunSampleRead;
    dd->SunEffectOnOff = D3D_SunEffectOnOff;
    dd->b_CanDiffuse2x = D3D_b_CanDiffuse2x;
    dd->b_WantsNoFlare = D3D_b_WantsNoFlare;
    dd->b_NoZonAlpha = D3D_b_NoZonAlpha;
    dd->b_DepthBlurOn = D3D_b_DepthBlurOn;
    dd->GetDrawingSize = D3D_GetDrawingSize;
    dd->SetCanDiffuse2x = D3D_SetCanDiffuse2x;
    dd->SetWantsNoFlare = D3D_SetWantsNoFlare;
    dd->SetNoZonAlpha = D3D_SetNoZonAlpha;
    dd->b_Ready = D3D_b_Ready;
    dd->sz_Name = D3D_sz_Name;
    dd->AfterEffects = D3D_PC_AE;
    dd->OverlayBegin = D3D_OverlayBegin;
    dd->OverlayDrawQuads = D3D_OverlayDrawQuads;
    dd->OverlayEnd = D3D_OverlayEnd;
    dd->p_CreateAlphaTexture = D3D_p_CreateAlphaTexture;
    dd->DestroyAlphaTexture = D3D_DestroyAlphaTexture;
    dd->OverlayDrawTexturedQuads = D3D_OverlayDrawTexturedQuads;
    dd->b_GeometryHasCache = D3D_b_GeometryHasCache;
    dd->DiscardGeometryCache = D3D_DiscardGeometryCache;
    dd->BuildGeometryCaches = D3D_CreateGameObjectsD3DStuffForMultiStream;
    dd->RebuildDepthBlurRamp = D3D_RebuildDepthBlurRamp;
    dd->BeginScene = D3D_BeginSceneCall;
    dd->PrepareWaterRendering = D3D_PrepareWaterRendering;
    dd->DrawWater = D3D_DrawWater_DrawVertexTable;
}