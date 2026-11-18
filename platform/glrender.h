#pragma once

#include <SDL3/SDL_video.h>

struct sWaterParamsGlobalStruct;
struct sWorld;
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
   first row that can run is what the game gets when engine.ini
   asks for nothing, and that is Direct3D; this one is reached by
   asking for it with

       [render]
       backend = gl

   What it does so far: comes up, clears and presents; draws the
   debug overlay and its text; draws the world's geometry with its
   textures, its sampler and blend state, and its alpha test; and
   draws each caster's shadow into a target of its own and samples
   it back; draws the engine's sprites; and plays fullscreen video.
   What it does not do yet is the sun's occlusion test and the
   depth-blur pass -- and those two are one piece of work, because
   the alpha the occlusion test reads is what the depth-blur pass
   writes. The geometry cache is answered rather than implemented:
   the engine is told there is none, so it always takes the immediate
   path, which this renderer draws. Those interface entries are present and
   inert, because an entry left NULL is a crash rather than a
   missing feature; each carries a comment saying what it waits on,
   and the log says it once at startup.

   Culling is decoded from the draw mask and deliberately not applied,
   so everything is drawn: which winding the engine calls front
   depends on the projection as well as on the geometry, and a wrong
   guess takes holes out of the world that look like a texture or a
   projection fault.

   The context is 4.5 core. There is no fallback to a lower version
   and no compatibility profile: the point of the rewrite is the
   modern API, and a renderer that quietly came up on 3.3 would fail
   later and further away. A machine that cannot give 4.5 core makes
   this row unavailable, and the picker falls back to Direct3D. */

/* Whether a 4.5 core context can be had here. Answered by making one
   on a hidden window of its own and throwing both away, which is the
   only answer that is not a guess -- and on a window of its own
   because the game's may not exist yet when this is asked, and
   because a context made to ask a question should not be the one the
   game then draws through. Remembered, so the cost is paid once. */
int GL_b_Available(void);

/* What the renderer keeps. Handed out through CreateDevice and opaque
   to everything outside this file. */
typedef struct sGlRenderState
{
    /* The window, as the engine passes it and as the library that
       made it knows it. The first is what the engine's interface
       carries; the second is what the context is made on. */
    HWND         hwnd;
    SDL_Window*  window;
    SDL_GLContext context;
    int   width;
    int   height;
    int   zEnable;
    int   zWrite;
    /* Which colour channels the renderer last turned *off*, one entry
       a channel. Kept because a clear has to put the mask out of the
       way and then put it back -- see the setter.
     *
       Written as what is off rather than what is on so that nothing
       has to initialise it: a device starts as a block of nought, and
       nought here says every channel is written, which is OpenGL's
       own default. */
    int   colorWriteOff[4];
    int   canDiffuse2x;
    int   wantsNoFlare;
    int   noZonAlpha;
    /* Whether any sun wants its occlusion test, which is the one thing
       that turns the depth blur on when nothing else asked for it.
       There is no sampler in this renderer yet, so the engine has
       nothing to switch it on with and it stays off -- but the blur
       reads it, so it is recorded rather than dropped. */
    int   sunEffectOn;
} tGlRenderState;

void* GL_p_CreateDevice(void);
void  GL_DestroyDevice(void* rendererState);
int   GL_Init(HWND hwnd, struct sGdi_DisplayData* dd);
int   GL_Close(void);
int   GL_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* dd);
void  GL_Clear(int which, int color);
void  GL_Flip(void);
void  GL_PC_AE(struct sWorld* world);
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
const char* GL_sz_Name(void);
int   GL_b_GeometryHasCache(struct sGEOGRO* gro);
void  GL_DiscardGeometryCache(struct sGEOGRO* gro);

/* Nothing: this renderer builds a piece of geometry's buffers the
   first time it draws it, so a world that has just loaded needs no
   pass of its own. Answered rather than left out, so the engine's
   call through the interface needs no guard for one renderer. */
void  GL_BuildGeometryCaches(struct sWorld* world);

/* Told that a depth-blur setting moved. Nothing to do: the blur pass
   compares the settings against the ones it last uploaded a ramp for
   and uploads again when they differ, so it finds out by itself.
   Answered rather than left out, as above. */
void  GL_RebuildDepthBlurRamp(void);

/* ---- present but inert, each for the reason in glrender.c ---- */
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
void GL_SetTexCoordIndex(int stage, int source);
void GL_SetTexCoordMatrix(int stage, const Mtx4x4* matrix);
void GL_SetTextureTransformFlags(int stage, int transform);
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
int GL_b_SunSampleRead(void* sampler, int* outBlocked);
void GL_SunEffectOnOff(void* sampler, int on);
void* GL_p_CreateAlphaTexture(int width, int height, const uint8_t* coverage);
void GL_DestroyAlphaTexture(void* texture);
void GL_OverlayDrawTexturedQuads(void* texture, const void* vertices, int triangleCount);

/* The water: the reflection target cleared and kept, before the world
   is drawn, and the surface drawn after it. See the comment at the
   water section of platform/glrender.c -- the reflection itself is not drawn
   yet, so the surface mirrors a flat colour. */
void GL_PrepareWaterRendering(void);
void GL_DrawWater(struct sWaterParamsGlobalStruct* water);
