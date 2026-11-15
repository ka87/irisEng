#include "wingl.h"
#include "glload.h"
#include "glshader.h"
#include "wingdimath.h"
#include "../projection.h"
#include "../geovertex.h"
#include "../cameraengine.h"
#include "../soft.h"
#include "../dbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------- what WGL adds */

/* Two extension entry points, which is the awkwardness at the heart
   of getting a modern context on Windows: the function that creates
   one can only be found through a context that already exists. So a
   throwaway window gets a throwaway 1.1 context, that context answers
   for these two, and then the real context is made with them. */

#define WGL_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB       0x2092
#define WGL_CONTEXT_FLAGS_ARB               0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB        0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB    0x00000001
#define WGL_CONTEXT_DEBUG_BIT_ARB           0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002

typedef HGLRC (*GL_pfn_CreateContextAttribs)(HDC, HGLRC, const int*);

static GL_pfn_CreateContextAttribs gl_createContextAttribs;

/* The module opengl32.dll, kept for the second half of a name
   lookup: wglGetProcAddress answers for what a context added past
   OpenGL 1.1 and is entitled to refuse the 1.1 entry points, which
   the module exports itself. */
static HMODULE gl_module;

static tGlRenderState gl_state;

/* Defined with the rest of the 2D pass, at the bottom; GL_Close has
   to call it while the context is still current. */
static void GL_l_ForgetPass(void);
static void GL_l_ForgetGeometry(void);
static Mtx4x4 gl_viewProjMtx;
static int            gl_available = -1;   /* -1 until asked */
static int            gl_saidOnce;

/* -------------------------------------------------- name resolution */

static void* GL_l_p_FromContext(const char* name)
{
    return (void*)wglGetProcAddress(name);
}

static void* GL_l_p_FromModule(const char* name)
{
    return gl_module ? (void*)GetProcAddress(gl_module, name) : NULL;
}

/* ------------------------------------------------- the pixel format */

static int GL_l_i_SetPixelFormat(HDC dc)
{
    PIXELFORMATDESCRIPTOR want;
    int                   chosen;

    memset(&want, 0, sizeof(want));
    want.nSize = sizeof(want);
    want.nVersion = 1;
    want.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    want.iPixelType = PFD_TYPE_RGBA;
    want.cColorBits = 32;
    want.cDepthBits = 24;
    want.cStencilBits = 8;
    want.iLayerType = PFD_MAIN_PLANE;

    chosen = ChoosePixelFormat(dc, &want);
    if (!chosen)
        return 0;
    if (!SetPixelFormat(dc, chosen, &want))
        return 0;
    return chosen;
}

/* ------------------------------------------------ the first context

   A 1.1 context on a window of its own, for the sole purpose of
   finding wglCreateContextAttribsARB. It is current only for as long
   as that takes.

   It has to be a separate window, not the game's: a window's pixel
   format can be set once and only once, and the format that suits a
   1.1 context is not necessarily the one the real context wants. */

static int GL_l_b_FindContextCreator(void)
{
    WNDCLASSA cls;
    HWND      wnd;
    HDC       dc;
    HGLRC     ctx;
    int       ok = 0;

    if (gl_createContextAttribs)
        return 1;

    memset(&cls, 0, sizeof(cls));
    cls.lpfnWndProc = DefWindowProcA;
    cls.hInstance = GetModuleHandleA(NULL);
    cls.lpszClassName = "irisEngGLProbe";
    if (!RegisterClassA(&cls))
        return 0;

    wnd = CreateWindowExA(0, cls.lpszClassName, "", 0, 0, 0, 1, 1,
                          NULL, NULL, cls.hInstance, NULL);
    if (!wnd)
    {
        UnregisterClassA(cls.lpszClassName, cls.hInstance);
        return 0;
    }

    dc = GetDC(wnd);
    if (dc && GL_l_i_SetPixelFormat(dc))
    {
        ctx = wglCreateContext(dc);
        if (ctx && wglMakeCurrent(dc, ctx))
        {
            gl_createContextAttribs = (GL_pfn_CreateContextAttribs)
                GL_p_Resolve("wglCreateContextAttribsARB",
                             GL_l_p_FromContext, GL_l_p_FromModule);
            ok = gl_createContextAttribs != NULL;
            wglMakeCurrent(dc, NULL);
        }
        if (ctx)
            wglDeleteContext(ctx);
    }
    if (dc)
        ReleaseDC(wnd, dc);
    DestroyWindow(wnd);
    UnregisterClassA(cls.lpszClassName, cls.hInstance);

    if (!ok)
        DBG_OutputDebugString("GL: no wglCreateContextAttribsARB; "
                              "this driver cannot make a modern context\n");
    return ok;
}

/* ------------------------------------------------- the real context */

/* Makes a 4.5 core context current on dc and loads the entry points.
   Leaves nothing current and nothing allocated if it cannot. */
static HGLRC GL_l_p_MakeContext(HDC dc)
{
    static const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    HGLRC ctx;
    char  missing[64];
    int   absent;

    if (!gl_createContextAttribs)
        return NULL;

    ctx = gl_createContextAttribs(dc, NULL, attribs);
    if (!ctx)
    {
        DBG_OutputDebugString("GL: no 4.5 core context here\n");
        return NULL;
    }
    if (!wglMakeCurrent(dc, ctx))
    {
        wglDeleteContext(ctx);
        DBG_OutputDebugString("GL: the 4.5 context would not become current\n");
        return NULL;
    }

    absent = GL_i_LoadFunctions(GL_l_p_FromContext, GL_l_p_FromModule,
                                missing, sizeof(missing));
    if (absent)
    {
        DBG_OutputDebugString("GL: %d entry point(s) missing, first is %s\n",
                              absent, missing);
        GL_ForgetFunctions();
        wglMakeCurrent(dc, NULL);
        wglDeleteContext(ctx);
        return NULL;
    }
    return ctx;
}

/* ------------------------------------------------------- is it here */

int GL_b_Available(void)
{
    WNDCLASSA cls;
    HWND      wnd;
    HDC       dc;
    HGLRC     ctx;

    if (gl_available >= 0)
        return gl_available;
    gl_available = 0;

    if (!gl_module)
        gl_module = LoadLibraryA("opengl32.dll");
    if (!gl_module)
    {
        DBG_OutputDebugString("GL: no opengl32.dll\n");
        return 0;
    }
    if (!GL_l_b_FindContextCreator())
        return 0;

    /* A context of its own on a window of its own, made and thrown
       away. Asking the driver is the only way to know, and asking it
       on the game's window would spend that window's one chance to
       have a pixel format set. */
    memset(&cls, 0, sizeof(cls));
    cls.lpfnWndProc = DefWindowProcA;
    cls.hInstance = GetModuleHandleA(NULL);
    cls.lpszClassName = "irisEngGLTry";
    if (!RegisterClassA(&cls))
        return 0;
    wnd = CreateWindowExA(0, cls.lpszClassName, "", 0, 0, 0, 1, 1,
                          NULL, NULL, cls.hInstance, NULL);
    if (!wnd)
    {
        UnregisterClassA(cls.lpszClassName, cls.hInstance);
        return 0;
    }

    dc = GetDC(wnd);
    if (dc && GL_l_i_SetPixelFormat(dc))
    {
        ctx = GL_l_p_MakeContext(dc);
        if (ctx)
        {
            const GLubyte* version = GLFN_glGetString(GL_VERSION);

            gl_available = GL_b_VersionAtLeast((const char*)version, 4, 5);
            if (!gl_available)
                DBG_OutputDebugString("GL: context reports \"%s\", "
                                      "which is not 4.5\n",
                                      version ? (const char*)version : "?");
            wglMakeCurrent(dc, NULL);
            wglDeleteContext(ctx);
            GL_ForgetFunctions();
        }
    }
    if (dc)
        ReleaseDC(wnd, dc);
    DestroyWindow(wnd);
    UnregisterClassA(cls.lpszClassName, cls.hInstance);
    return gl_available;
}

/* ------------------------------------------------------ the device */

void* GL_p_CreateDevice(void)
{
    memset(&gl_state, 0, sizeof(gl_state));
    gl_state.zEnable = 1;
    gl_state.zWrite = 1;
    return &gl_state;
}

void GL_DestroyDevice(void* rendererState)
{
    (void)rendererState;
    GL_Close();
}

/* What the driver says it is. Logged once, because the first question
   about a renderer that draws nothing is which driver answered. */
static void GL_l_SayWhatThisIs(void)
{
    if (gl_saidOnce)
        return;
    gl_saidOnce = 1;
    DBG_OutputDebugString("GL: %s, %s, GLSL %s\n",
        (const char*)GLFN_glGetString(GL_VENDOR),
        (const char*)GLFN_glGetString(GL_RENDERER),
        (const char*)GLFN_glGetString(GL_SHADING_LANGUAGE_VERSION));
    DBG_OutputDebugString("GL: this renderer clears and presents; "
                          "geometry, textures, video and text are not "
                          "drawn through it yet\n");
}

static void GL_l_SizeFromWindow(HWND hwnd)
{
    RECT r;

    gl_state.width = 0;
    gl_state.height = 0;
    if (hwnd && GetClientRect(hwnd, &r))
    {
        gl_state.width = r.right - r.left;
        gl_state.height = r.bottom - r.top;
    }
}

int GL_Init(HWND hwnd, struct sGdi_DisplayData* dd)
{
    (void)dd;

    if (!GL_b_Available())
        return 0;

    gl_state.hwnd = hwnd;
    gl_state.dc = GetDC(hwnd);
    if (!gl_state.dc)
    {
        DBG_OutputDebugString("GL: the window has no device context\n");
        return 0;
    }
    if (!GL_l_i_SetPixelFormat(gl_state.dc))
    {
        DBG_OutputDebugString("GL: no pixel format on this window\n");
        return 0;
    }
    gl_state.context = GL_l_p_MakeContext(gl_state.dc);
    if (!gl_state.context)
        return 0;

    GL_l_SayWhatThisIs();
    GL_l_SizeFromWindow(hwnd);
    GLFN_glViewport(0, 0, gl_state.width, gl_state.height);

    /* The engine's projections put z in 0..1, which is Direct3D's
       range and not OpenGL's -1..1. Core 4.5 can be told to use it,
       which is a large part of why this renderer asks for 4.5: the
       engine's matrices then need no adjusting, and projection.c can
       be shared between the two renderers rather than having a
       variant for each.

       The origin stays at the lower left, OpenGL's own. Both
       conventions put clip y = +1 at the top of the screen -- one
       measures the window from the bottom and negates on the way, the
       other does not -- so nothing is upside down, and the overlay's
       pixel mapping is unaffected. */
    GLFN_glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    GLFN_glEnable(GL_DEPTH_TEST);
    GLFN_glDepthFunc(GL_LEQUAL);
    GLFN_glDepthMask(GL_TRUE);

    /* Culling off. Which winding the engine's geometry has is a
       question to answer by looking at the result rather than by
       reasoning here, and drawing both faces is the reading that
       cannot hide anything. */
    GLFN_glDisable(GL_CULL_FACE);
    return 1;
}

int GL_Close(void)
{
    if (gl_state.context)
    {
        GL_l_ForgetPass();
        GL_l_ForgetGeometry();
        wglMakeCurrent(gl_state.dc, NULL);
        wglDeleteContext(gl_state.context);
        gl_state.context = NULL;
        GL_ForgetFunctions();
    }
    if (gl_state.dc)
    {
        ReleaseDC(gl_state.hwnd, gl_state.dc);
        gl_state.dc = NULL;
    }
    gl_state.hwnd = NULL;
    return 1;
}

int GL_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* dd)
{
    (void)dd;

    /* The context survives a resize; the viewport does not. */
    GL_l_SizeFromWindow(hwnd ? hwnd : gl_state.hwnd);
    if (gl_state.context)
        GLFN_glViewport(0, 0, gl_state.width, gl_state.height);
    return 1;
}

/* --------------------------------------------------- clear and flip */

/* The engine asks for its clear in the same terms Direct3D took: a
   mask of what to clear and a packed colour. */
void GL_Clear(int which, int color)
{
    GLbitfield mask = 0;

    if (!gl_state.context)
        return;

    if (which & 1)
        mask |= GL_COLOR_BUFFER_BIT;
    if (which & 2)
        mask |= GL_DEPTH_BUFFER_BIT;
    if (which & 4)
        mask |= GL_STENCIL_BUFFER_BIT;
    if (!mask)
        return;

    if (mask & GL_COLOR_BUFFER_BIT)
        GLFN_glClearColor((GLclampf)((color >> 16) & 0xFF) / 255.0f,
                          (GLclampf)((color >> 8) & 0xFF) / 255.0f,
                          (GLclampf)(color & 0xFF) / 255.0f,
                          (GLclampf)((color >> 24) & 0xFF) / 255.0f);
    if (mask & GL_DEPTH_BUFFER_BIT)
    {
        /* Depth is only written by a clear if writing is on, which is
           a piece of GL state the engine sets for drawing. */
        GLFN_glDepthMask(GL_TRUE);
        GLFN_glClearDepth(1.0);
    }
    if (mask & GL_STENCIL_BUFFER_BIT)
        GLFN_glClearStencil(0);

    GLFN_glClear(mask);

    if ((mask & GL_DEPTH_BUFFER_BIT) && !gl_state.zWrite)
        GLFN_glDepthMask(GL_FALSE);
}

void GL_Flip(void)
{
    if (gl_state.dc)
        SwapBuffers(gl_state.dc);
}

void GL_GetDrawingSize(int* width, int* height)
{
    if (width)
        *width = gl_state.width;
    if (height)
        *height = gl_state.height;
}

/* ------------------------------------------------------- depth state */

void GL_SetZEnable(int on)
{
    gl_state.zEnable = on ? 1 : 0;
    if (!gl_state.context)
        return;
    if (gl_state.zEnable)
        GLFN_glEnable(GL_DEPTH_TEST);
    else
        GLFN_glDisable(GL_DEPTH_TEST);
}

int GL_i_ZEnable(void)
{
    return gl_state.zEnable;
}

void GL_SetZWriteEnable(int on)
{
    gl_state.zWrite = on ? 1 : 0;
    if (gl_state.context)
        GLFN_glDepthMask(gl_state.zWrite ? GL_TRUE : GL_FALSE);
}

/* ------------------------------------------------------ capabilities

   Direct3D answers these from what the card reported. This renderer
   requires 4.5 core, where all three are a given: two texture stages
   with a doubled modulate, a colour buffer with alpha, and the
   precision the depth blur wants are not optional features of a 4.5
   implementation. The depth blur is off because nothing is drawn
   yet. */

int GL_b_CanDiffuse2x(void) { return gl_state.canDiffuse2x; }
int GL_b_WantsNoFlare(void) { return gl_state.wantsNoFlare; }
int GL_b_NoZonAlpha(void)   { return gl_state.noZonAlpha; }
int GL_b_DepthBlurOn(void)  { return 0; }

void GL_SetCanDiffuse2x(int on) { gl_state.canDiffuse2x = on ? 1 : 0; }
void GL_SetWantsNoFlare(int on) { gl_state.wantsNoFlare = on ? 1 : 0; }
void GL_SetNoZonAlpha(int on)   { gl_state.noZonAlpha = on ? 1 : 0; }

/* ------------------------------------------------- geometry caching

   Nothing is cached because nothing is drawn. Answering no means the
   engine never asks for a cache to be dropped, and dropping one is
   then nothing to do rather than something to get wrong. */

/* Whether there is a context to draw through. Real, because the
   overlay asks before it starts a pass and the answer is knowable
   whether or not anything can yet be drawn. */
int GL_b_Ready(void)
{
    return gl_state.context != NULL;
}

int GL_b_GeometryHasCache(struct sGEOGRO* gro)
{
    (void)gro;
    return 0;
}

void GL_DiscardGeometryCache(struct sGEOGRO* gro)
{
    (void)gro;
}

/* ================================================================
   Present and inert.

   Every one of these is an interface entry the engine will call, and
   an entry left NULL is a crash rather than a missing feature. Each
   says what it is waiting for. The values they answer with are the
   ones that make the engine carry on rather than wait: nothing ready,
   nothing created, nothing to read.
   ================================================================ */

/* ================================================== world geometry

   The engine hands over a view matrix, a camera to build a projection
   from, and then chunks of triangles with its positions, colours and
   texture coordinates in separate arrays. None of the working out is
   this renderer's: projection.c fits the camera to the surface and
   geovertex.c builds the interleaved vertices. What is here is the
   matrix the shader gets, the buffer the vertices go in, and the
   draw.

   Untextured so far. The vertices carry their texture coordinate and
   the shader carries it through, so the layout is the one textures
   will need; nothing samples it yet. */

static Mtx4x4     gl_viewMtx;
static Mtx4x4     gl_projMtx;
static tGlProgram gl_geoProgram;
static GLint      gl_geoViewProj = -1;
static GLuint     gl_geoVao;
static GLuint     gl_geoBuffer;
static int        gl_geoTried;
static int        gl_geoReady;

/* The staging buffer the vertices are built in before being uploaded.
   It grows to the largest chunk seen and is never shrunk: the engine
   draws the same geometry every frame, so this settles within the
   first one. */
static tGdi_TexturedVertex* gl_geoVerts;
static int                  gl_geoVertMax;

static int GL_l_b_GeometryReady(void)
{
    if (gl_geoTried)
        return gl_geoReady;
    gl_geoTried = 1;
    if (!gl_state.context)
        return 0;

    if (!GL_b_ProgramLoad(&gl_geoProgram, "geometry.vert", "geometry.frag"))
        return 0;
    gl_geoViewProj = GL_i_ProgramUniform(&gl_geoProgram, "uViewProj");

    GLFN_glGenBuffers(1, &gl_geoBuffer);
    GLFN_glGenVertexArrays(1, &gl_geoVao);
    if (!gl_geoBuffer || !gl_geoVao)
    {
        DBG_OutputDebugString("GL: no buffer or vertex array for geometry\n");
        GL_ProgramFree(&gl_geoProgram);
        return 0;
    }

    /* The same layout the glyph quads use, because the engine's
       geometry vertex and the interface's textured one are the same
       shape -- position, packed colour, one texture coordinate. */
    GLFN_glBindVertexArray(gl_geoVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_geoBuffer);
    GLFN_glEnableVertexAttribArray(0);
    GLFN_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tGdi_TexturedVertex),
                               (const void*)(size_t)0);
    GLFN_glEnableVertexAttribArray(1);
    GLFN_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                               (GLsizei)sizeof(tGdi_TexturedVertex),
                               (const void*)(size_t)12);
    GLFN_glEnableVertexAttribArray(2);
    GLFN_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tGdi_TexturedVertex),
                               (const void*)(size_t)16);
    GLFN_glBindVertexArray(0);

    gl_geoReady = 1;
    return 1;
}

static void GL_l_ForgetGeometry(void)
{
    if (gl_geoReady && gl_state.context)
    {
        GL_ProgramFree(&gl_geoProgram);
        if (gl_geoBuffer)
            GLFN_glDeleteBuffers(1, &gl_geoBuffer);
        if (gl_geoVao)
            GLFN_glDeleteVertexArrays(1, &gl_geoVao);
    }
    gl_geoBuffer = 0;
    gl_geoVao = 0;
    gl_geoViewProj = -1;
    gl_geoTried = 0;
    gl_geoReady = 0;
    free(gl_geoVerts);
    gl_geoVerts = NULL;
    gl_geoVertMax = 0;
}

/* view * projection, in the engine's own order, kept ready for the
   next draw rather than rebuilt per chunk. */
static void GL_l_CombineMatrices(void)
{
    D3D_MatrixMultiply((D3DMATRIX*)&gl_viewProjMtx, (const D3DMATRIX*)&gl_viewMtx,
                       (const D3DMATRIX*)&gl_projMtx);
}

int GL_SetViewMatrix(Mtx4x4* mtxptr)
{
    if (!mtxptr)
        return 0;

    /* The same conversion the other renderer makes: a matrix carrying
       separate scale factors is flattened into a plain affine one. */
    MATH_MakeOGLMatrix(&gl_viewMtx, mtxptr);
    GL_l_CombineMatrices();
    return 0;
}

void GL_SetProjectionMatrix(tDDCamStruct* cam)
{
    tPrj_Input  in;
    tPrj_Output out;

    if (!cam || !gpst_DX_stDD)
        return;

    in.surfaceWidth = gl_state.width;
    in.surfaceHeight = gl_state.height;
    in.selectedScreenRatio = gpst_DX_stDD->ddcs2.selectedScreenRatio;
    in.manualScreenRatio = gpst_DX_stDD->ddcs2.manualScreenRatio;
    in.pixelYoverX = gpst_DX_stDD->ddcs2.pixelYoverX;
    in.flags2 = gpst_DX_stDD->ddcs2.flags2;
    in.widescreen = GDI_gi_Widescreen;
    in.realYoverX = (gl_state.width > 0 && gl_state.height > 0)
                        ? (float)gl_state.height / (float)gl_state.width
                        : GDI_gf_RefYoverX;
    in.refYoverX = GDI_gf_RefYoverX;

    PRJ_Compute(cam, &in, &out);
    memcpy(&gl_projMtx, &out.matrix, sizeof(Mtx4x4WithoutAdditionalData));
    GL_l_CombineMatrices();

    if (out.setViewport && gl_state.context)
    {
        /* The rectangle is in the engine's window coordinates, whose
           origin is the top left. OpenGL's viewport origin is the
           bottom left, so the y has to be measured from the other
           end. Getting this wrong puts the picture in the right place
           on a full-height viewport and the wrong one on every
           letterboxed camera, which is the sort of thing that looks
           like a projection bug. */
        GLFN_glViewport(out.viewX,
                        gl_state.height - (out.viewY + out.viewHeight),
                        out.viewWidth, out.viewHeight);
    }
}

int GL_DrawElementIndexedTriangles(struct sGeoTriangleChunks* chunks,
                                   Vec3* vertices, struct sGeoUVElem* uvs,
                                   int vertexNum)
{
    tGvx_Source       src;
    tBigBufferStruct* bb;
    int               triangleNum;
    int               needed;

    (void)vertexNum;

    if (!chunks || !vertices || !gpst_DX_stDD)
        return 0;
    triangleNum = (int)chunks->triangleNum;
    if (triangleNum <= 0)
        return 0;
    if (!GL_l_b_GeometryReady())
        return 0;

    needed = triangleNum * 3;
    if (needed > gl_geoVertMax)
    {
        tGdi_TexturedVertex* grown;

        grown = (tGdi_TexturedVertex*)realloc(gl_geoVerts,
                                              sizeof(*grown) * (size_t)needed);
        if (!grown)
            return 0;
        gl_geoVerts = grown;
        gl_geoVertMax = needed;
    }

    bb = gpst_DX_stDD->bigBuffer;
    src.vertices = vertices;
    src.uvs = uvs;
    src.colors = bb ? bb->vertexColorArray : NULL;
    src.second = bb ? bb->vertexColorArraySecond : NULL;
    src.mask = bb ? bb->colorMask : 0;

    /* The other renderer reads this out of its own state, where it is
       the alpha the colour mask is combined with. This renderer has
       no equivalent yet, so it contributes nothing. */
    src.alpha = 0;
    src.solidColor = src.colors
                         ? 0
                         : GVX_ui_SolidColor(src.mask, src.alpha,
                                             bb ? bb->solidVertexColor : 0);
    src.useVertexUv = (gpst_DX_stDD->flags & 2) != 0;

    GVX_BuildTriangles(gl_geoVerts, chunks, &src);

    GLFN_glUseProgram(gl_geoProgram.program);
    if (gl_geoViewProj >= 0)
        GLFN_glUniformMatrix4fv(gl_geoViewProj, 1, GL_FALSE,
                                (const GLfloat*)&gl_viewProjMtx);
    GLFN_glBindVertexArray(gl_geoVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_geoBuffer);
    GLFN_glBufferData(GL_ARRAY_BUFFER,
                      (GLsizei)(sizeof(tGdi_TexturedVertex) * (size_t)needed),
                      gl_geoVerts, GL_STREAM_DRAW);
    GLFN_glDrawArrays(GL_TRIANGLES, 0, (GLsizei)needed);
    GLFN_glBindVertexArray(0);
    return 0;
}

/* the upload queue waits on textures */
int GL_Request(int type, void* param)
{
    (void)type;
    (void)param;
    return 0;
}

/* textures wait on the texture stage */
void GL_TextureInit(int a1, int a2, char a3)
{
    (void)a1;
    (void)a2;
    (void)a3;
}

/* sprites are geometry too */
int GL_DrawSprites(tGeoSprites* a1, Vec3* a2, int num)
{
    (void)a1;
    (void)a2;
    (void)num;
    return 0;
}

/* textures wait on the texture stage */
int GL_TextureStore(struct sGdi_DisplayData* a1, int a2)
{
    (void)a1;
    (void)a2;
    return 0;
}

/* palettes wait on textures */
void GL_PaletteLoad(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
}

/* palettes wait on textures */
int GL_SetTexturePalette(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return 0;
}

/* nothing was loaded, so nothing is unloaded */
void GL_TextureUnload(struct sGdi_DisplayData* a1, struct sEngPaletteElem *a2, int a3)
{
    (void)a1;
    (void)a2;
    (void)a3;
}

/* render targets wait on the framebuffer stage */
void GL_SetTextureTarget(struct sGdi_DisplayData* a1, int a2, int a3, int a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
}

/* nothing was loaded */
void GL_ReleaseTextures(struct sGdi_DisplayData* dd)
{
    (void)dd;
}

/* shadow targets wait on the framebuffer stage */
void GL_SetRenderTargetAndBackupPreviousSDW(int textureID, int clear)
{
    (void)textureID;
    (void)clear;
}

/* as above */
void GL_SetViewMatrixSDW(Mtx4x4* mtxptr, float* unused)
{
    (void)mtxptr;
    (void)unused;
}

/* a fixed-function texture stage this renderer does not have */
void GL_SetTexCoordIndex(int stage, int index)
{
    (void)stage;
    (void)index;
}

/* as above */
void GL_SetTextureTransformFlags(int stage, int flags)
{
    (void)stage;
    (void)flags;
}

/* no video display. The engine logs that and plays nothing,
   which is what it already does when a file is missing */
int GL_b_VideoDisplayCreate(int width, int height)
{
    (void)width;
    (void)height;
    return 0;
}

/* nothing was created */
void GL_VideoDisplayClose(void)
{
}

/* no display to lock */
int GL_b_VideoFrameLock(void** bits, int* pitch, int* height)
{
    (void)bits;
    (void)pitch;
    (void)height;
    return 0;
}

/* nothing was locked */
void GL_VideoFrameUnlock(void)
{
}

/* nothing to present */
void GL_VideoFramePresent(int width, int height, int fit)
{
    (void)width;
    (void)height;
    (void)fit;
}

/* the occlusion test is an asynchronous readback and waits on the
   framebuffer stage. NULL is a sampler the engine does without */
void* GL_p_SunSamplerCreate(void)
{
    return NULL;
}

/* nothing was created */
void GL_SunSamplerDestroy(void* sampler)
{
    (void)sampler;
}

/* never ready, so nothing is ever queued */
int GL_b_SunSamplerReady(void* sampler)
{
    (void)sampler;
    return 0;
}

/* nothing is in flight */
int GL_b_SunSamplingNow(void* sampler)
{
    (void)sampler;
    return 0;
}

/* nothing to queue into */
void GL_SunSampleQueue(void* sampler, const int* x, const int* y, int num, int pointMask, float cameraZ)
{
    (void)sampler;
    (void)x;
    (void)y;
    (void)num;
    (void)pointMask;
    (void)cameraZ;
}

/* no answer ever arrives, which the engine already tolerates --
   it is the same as a sample still in flight */
int GL_b_SunSampleRead(void* sampler, int* outPassed)
{
    (void)sampler;
    (void)outPassed;
    return 0;
}

/* nothing to switch */
void GL_SunEffectOnOff(void* sampler, int on)
{
    (void)sampler;
    (void)on;
}

/* ===================================================== the 2D pass

   What the overlay draws inside, and the first thing this renderer
   draws at all. Flat quads and glyphs, in pixels, with no matrix: the
   vertex shaders take the viewport size and do the mapping in two
   lines.

   The programs and the buffers are made on first use rather than at
   startup, so a renderer that is selected and never shows an overlay
   costs nothing, and a shader that will not compile is reported when
   somebody asks for the overlay rather than during the load. */

static tGlProgram gl_flatProgram;
static tGlProgram gl_textProgram;
static GLint      gl_flatViewport = -1;
static GLint      gl_textViewport = -1;
static GLint      gl_textAtlas = -1;
static GLuint     gl_flatVao;
static GLuint     gl_textVao;
static GLuint     gl_quadBuffer;
static int        gl_passTried;     /* loaded, or tried and failed */
static int        gl_passReady;
static int        gl_inPass;

/* The two vertex layouts, described once each. The offsets are
   written out rather than taken with offsetof so that the shader's
   attribute numbers and the bytes they read sit on adjacent lines;
   the test checks them against the structs. */
static void GL_l_DescribeFlat(void)
{
    GLFN_glBindVertexArray(gl_flatVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_quadBuffer);
    GLFN_glEnableVertexAttribArray(0);
    GLFN_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tGdi_Vertex),
                               (const void*)(size_t)0);
    GLFN_glEnableVertexAttribArray(1);
    GLFN_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                               (GLsizei)sizeof(tGdi_Vertex),
                               (const void*)(size_t)12);
}

static void GL_l_DescribeText(void)
{
    GLFN_glBindVertexArray(gl_textVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_quadBuffer);
    GLFN_glEnableVertexAttribArray(0);
    GLFN_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tGdi_TexturedVertex),
                               (const void*)(size_t)0);
    GLFN_glEnableVertexAttribArray(1);
    GLFN_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                               (GLsizei)sizeof(tGdi_TexturedVertex),
                               (const void*)(size_t)12);
    GLFN_glEnableVertexAttribArray(2);
    GLFN_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tGdi_TexturedVertex),
                               (const void*)(size_t)16);
}

static int GL_l_b_PassReady(void)
{
    if (gl_passTried)
        return gl_passReady;
    gl_passTried = 1;
    if (!gl_state.context)
        return 0;

    if (!GL_b_ProgramLoad(&gl_flatProgram, "overlay.vert", "overlay.frag"))
        return 0;
    if (!GL_b_ProgramLoad(&gl_textProgram, "overlaytext.vert", "overlaytext.frag"))
    {
        GL_ProgramFree(&gl_flatProgram);
        return 0;
    }
    gl_flatViewport = GL_i_ProgramUniform(&gl_flatProgram, "uViewport");
    gl_textViewport = GL_i_ProgramUniform(&gl_textProgram, "uViewport");
    gl_textAtlas = GL_i_ProgramUniform(&gl_textProgram, "uAtlas");

    GLFN_glGenBuffers(1, &gl_quadBuffer);
    GLFN_glGenVertexArrays(1, &gl_flatVao);
    GLFN_glGenVertexArrays(1, &gl_textVao);
    if (!gl_quadBuffer || !gl_flatVao || !gl_textVao)
    {
        DBG_OutputDebugString("GL: no buffer or vertex array for the overlay\n");
        GL_ProgramFree(&gl_flatProgram);
        GL_ProgramFree(&gl_textProgram);
        return 0;
    }
    GL_l_DescribeFlat();
    GL_l_DescribeText();
    GLFN_glBindVertexArray(0);

    gl_passReady = 1;
    return 1;
}

/* Gives the pass's programs, buffers and vertex arrays back. A GL
   name outlives nothing, and the context is about to go. */
static void GL_l_ForgetPass(void)
{
    if (gl_passReady && gl_state.context)
    {
        GL_ProgramFree(&gl_flatProgram);
        GL_ProgramFree(&gl_textProgram);
        if (gl_quadBuffer)
            GLFN_glDeleteBuffers(1, &gl_quadBuffer);
        if (gl_flatVao)
            GLFN_glDeleteVertexArrays(1, &gl_flatVao);
        if (gl_textVao)
            GLFN_glDeleteVertexArrays(1, &gl_textVao);
    }
    gl_quadBuffer = 0;
    gl_flatVao = 0;
    gl_textVao = 0;
    gl_flatViewport = -1;
    gl_textViewport = -1;
    gl_textAtlas = -1;
    gl_passTried = 0;
    gl_passReady = 0;
    gl_inPass = 0;
}

void GL_OverlayBegin(void)
{
    if (!GL_l_b_PassReady())
        return;

    /* Flat and over the top of everything: no depth test, no culling,
       and the usual alpha blend. The depth state is this renderer's
       own to put back, which End does. */
    GLFN_glDisable(GL_DEPTH_TEST);
    GLFN_glDisable(GL_CULL_FACE);
    GLFN_glEnable(GL_BLEND);
    GLFN_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl_inPass = 1;
}

/* One upload and one draw. The overlay hands over its whole batch at
   once, which is what makes a single stream buffer enough. */
static void GL_l_DrawBatch(GLuint vao, const void* vertices, int triangleCount,
                           size_t vertexSize)
{
    GLsizei count = (GLsizei)(triangleCount * 3);

    GLFN_glBindVertexArray(vao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_quadBuffer);
    GLFN_glBufferData(GL_ARRAY_BUFFER, (GLsizei)(vertexSize * (size_t)count),
                      vertices, GL_STREAM_DRAW);
    GLFN_glDrawArrays(GL_TRIANGLES, 0, count);
    GLFN_glBindVertexArray(0);
}

void GL_OverlayDrawQuads(const void* vertices, int triangleCount)
{
    if (!gl_inPass || !vertices || triangleCount <= 0)
        return;

    GLFN_glUseProgram(gl_flatProgram.program);
    if (gl_flatViewport >= 0)
        GLFN_glUniform4f(gl_flatViewport, (GLfloat)gl_state.width,
                         (GLfloat)gl_state.height, 0.0f, 0.0f);
    GL_l_DrawBatch(gl_flatVao, vertices, triangleCount, sizeof(tGdi_Vertex));
}

void GL_OverlayEnd(void)
{
    if (!gl_inPass)
        return;
    gl_inPass = 0;

    GLFN_glUseProgram(0);
    GLFN_glDisable(GL_BLEND);

    /* Back to what the engine asked for, which this renderer tracks
       because the engine sets it through SetZEnable. */
    if (gl_state.zEnable)
        GLFN_glEnable(GL_DEPTH_TEST);
}

/* ------------------------------------------------- the glyph atlas

   One byte of coverage per pixel, so a single-channel texture and no
   conversion. The handle is a GL name, which is an integer rather
   than a pointer, so it travels as one -- offset by one, because zero
   is "no texture" to GL and NULL to everyone else. */

void* GL_p_CreateAlphaTexture(int width, int height, const uint8_t* coverage)
{
    GLuint tex = 0;

    if (!gl_state.context || width <= 0 || height <= 0 || !coverage)
        return NULL;

    GLFN_glGenTextures(1, &tex);
    if (!tex)
        return NULL;
    GLFN_glBindTexture(GL_TEXTURE_2D, tex);

    /* The rows are packed with no padding, which is not the default. */
    GLFN_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0,
                      GL_RED, GL_UNSIGNED_BYTE, coverage);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    return (void*)(size_t)(tex + 1);
}

void GL_DestroyAlphaTexture(void* texture)
{
    GLuint tex = (GLuint)((size_t)texture);

    if (!tex || !gl_state.context)
        return;
    tex--;
    GLFN_glDeleteTextures(1, &tex);
}

void GL_OverlayDrawTexturedQuads(void* texture, const void* vertices,
                                 int triangleCount)
{
    GLuint tex = (GLuint)((size_t)texture);

    if (!gl_inPass || !tex || !vertices || triangleCount <= 0)
        return;
    tex--;

    GLFN_glUseProgram(gl_textProgram.program);
    if (gl_textViewport >= 0)
        GLFN_glUniform4f(gl_textViewport, (GLfloat)gl_state.width,
                         (GLfloat)gl_state.height, 0.0f, 0.0f);
    if (gl_textAtlas >= 0)
        GLFN_glUniform1i(gl_textAtlas, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, tex);

    GL_l_DrawBatch(gl_textVao, vertices, triangleCount,
                   sizeof(tGdi_TexturedVertex));
}
