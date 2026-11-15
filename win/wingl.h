#pragma once
#include <windows.h>

#include "../gdi.h"

/* Declared here rather than left to appear inside a parameter
   list: a struct first named in a prototype's parameters is a
   different type from the one named at file scope, and the
   definition then "conflicts" with its own declaration. */
struct sEngPaletteElem;
struct sEngTexIntermediateStruct;
struct sEngTexListElemStruct;
struct sGEOGRO;
struct sGdi_DisplayData;
struct sGeoTriangleChunks;
struct sGeoUVElem;
struct sGlRenderState;

/* The OpenGL renderer.

   It is the second row in gdi.c's table and never the fallback. The
   first row that can run is what the game gets when datafiles.ini
   asks for nothing, and that is Direct3D; this one is reached by
   asking for it with

       [render]
       backend = gl

   What it does so far is come up, clear and present. Nothing is drawn
   through it yet: the entries for geometry, textures, video and the
   sun's occlusion test are present and inert, because an interface
   entry left NULL is a crash rather than a missing feature. Each one
   carries a comment saying what it waits on, and the log says it once
   at startup.

   The context is 4.5 core. There is no fallback to a lower version
   and no compatibility profile: the point of the rewrite is the
   modern API, and a renderer that quietly came up on 3.3 would fail
   later and further away. A machine that cannot give 4.5 core makes
   this row unavailable, and the picker falls back to Direct3D. */

/* Whether a 4.5 core context can be had here. Answered by making one
   on a window of its own and throwing it away, which is the only
   answer that is not a guess. Remembered, so the cost is paid once. */
int GL_b_Available(void);

/* What the renderer keeps. Handed out through CreateDevice and opaque
   to everything outside this file. */
typedef struct sGlRenderState
{
    HWND  hwnd;
    HDC   dc;
    HGLRC context;
    int   width;
    int   height;
    int   zEnable;
    int   zWrite;
    int   canDiffuse2x;
    int   wantsNoFlare;
    int   noZonAlpha;
} tGlRenderState;

void* GL_p_CreateDevice(void);
void  GL_DestroyDevice(void* rendererState);
int   GL_Init(HWND hwnd, struct sGdi_DisplayData* dd);
int   GL_Close(void);
int   GL_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* dd);
void  GL_Clear(int which, int color);
void  GL_Flip(void);
void  GL_GetDrawingSize(int* width, int* height);

void  GL_SetZEnable(int on);
int   GL_i_ZEnable(void);
void  GL_SetZWriteEnable(int on);

int   GL_b_CanDiffuse2x(void);
int   GL_b_WantsNoFlare(void);
int   GL_b_NoZonAlpha(void);
int   GL_b_DepthBlurOn(void);
void  GL_SetCanDiffuse2x(int on);
void  GL_SetWantsNoFlare(int on);
void  GL_SetNoZonAlpha(int on);

int   GL_b_Ready(void);
int   GL_b_GeometryHasCache(struct sGEOGRO* gro);
void  GL_DiscardGeometryCache(struct sGEOGRO* gro);

/* ---- present but inert, each for the reason in wingl.c ---- */
void  GL_OverlayBegin(void);
void  GL_OverlayDrawQuads(const void* vertices, int triangleCount);
void  GL_OverlayEnd(void);
int GL_SetViewMatrix(Mtx4x4* mtxptr);
void GL_SetProjectionMatrix(tDDCamStruct* a1);
int GL_DrawElementIndexedTriangles(struct sGeoTriangleChunks* a1, Vec3* a2, struct sGeoUVElem* a3, int a4);
int GL_Request(int type, void* param);
void GL_TextureInit(int a1, int a2, char a3);
int GL_DrawSprites(tGeoSprites* a1, Vec3* a2, int num);
int GL_TextureStore(struct sGdi_DisplayData* a1, int a2);
void GL_PaletteLoad(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4);
int GL_SetTexturePalette(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4);
void GL_TextureUnload(struct sGdi_DisplayData* a1, struct sEngPaletteElem *a2, int a3);
void GL_SetTextureTarget(struct sGdi_DisplayData* a1, int a2, int a3, int a4);
void GL_ReleaseTextures(struct sGdi_DisplayData* dd);
void GL_SetRenderTargetAndBackupPreviousSDW(int textureID, int clear);
void GL_SetViewMatrixSDW(Mtx4x4* mtxptr, float* unused);
void GL_SetTexCoordIndex(int stage, int index);
void GL_SetTextureTransformFlags(int stage, int flags);
int GL_b_VideoDisplayCreate(int width, int height);
void GL_VideoDisplayClose(void);
int GL_b_VideoFrameLock(void** bits, int* pitch, int* height);
void GL_VideoFrameUnlock(void);
void GL_VideoFramePresent(int width, int height, int fit);
void* GL_p_SunSamplerCreate(void);
void GL_SunSamplerDestroy(void* sampler);
int GL_b_SunSamplerReady(void* sampler);
int GL_b_SunSamplingNow(void* sampler);
void GL_SunSampleQueue(void* sampler, const int* x, const int* y, int num, int pointMask, float cameraZ);
int GL_b_SunSampleRead(void* sampler, int* outPassed);
void GL_SunEffectOnOff(void* sampler, int on);
void* GL_p_CreateAlphaTexture(int width, int height, const uint8_t* coverage);
void GL_DestroyAlphaTexture(void* texture);
void GL_OverlayDrawTexturedQuads(void* texture, const void* vertices, int triangleCount);
