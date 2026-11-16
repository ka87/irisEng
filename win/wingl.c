#include "wingl.h"
#include "glload.h"
#include "glshader.h"
#include "wingdimath.h"
#include "overlay.h"
#include "../projection.h"
#include "../geovertex.h"
#include "../geosprite.h"
#include "../rasterstate.h"
#include "../depthblur.h"
#include "../aecolor.h"
#include "../aeframe.h"
#include "../watermesh.h"
#include "../waterreflect.h"
#include "../watersmooth.h"
#include "../aefade.h"
#include "../aehue.h"
#include "../aemotion.h"
#include "../aezoom2.h"
#include "../water.h"
#include "../fog.h"
#include "../textures.h"
#include "../cinebands.h"
#include "../cameraengine.h"
#include "../editorcamera.h"
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

typedef HGLRC (GL_APIENTRY *GL_pfn_CreateContextAttribs)(HDC, HGLRC, const int*);

static GL_pfn_CreateContextAttribs gl_createContextAttribs;

/* The module opengl32.dll, kept for the second half of a name
   lookup: wglGetProcAddress answers for what a context added past
   OpenGL 1.1 and is entitled to refuse the 1.1 entry points, which
   the module exports itself. */
static HMODULE gl_module;

static tGlRenderState gl_state;

typedef struct sGlTexture
{
    GLuint name;
    int    hasLevels;   /* a mip chain was generated for it */
} tGlTexture;

static tGlTexture* gl_textures;
static int         gl_textureNum;

/* Whether this renderer generates mip chains at all. The other
   renderer reads a device capability for this; here it is simply on,
   kept as a variable so that the one place that decides is visible. */
static int gl_mipmapping = 1;

/* The alpha the vertex colours are combined with, as the last
   material asked for it. */
static int gl_colorMaskAlpha;

typedef struct sGlMaterial
{
    int   textured;
    int   alphaMode;    /* 0 none, 1 keep above the reference, 2 below */
    float alphaRef;     /* 0..1 */
    /* Which fog curve this material's pass draws with, FOG_OFF for
       none. The variant it names is in gl_fogSecondColor, because it
       stands until another material names a different one. */
    int   fogCurve;
    /* The depth-blur pass draws the same vertices through a different
       program, so which program the next draw uses is part of what the
       material says. */
    int   b_depthBlur;
} tGlMaterial;

static tGlMaterial gl_material;

/* The sprite quads' vertices on their way to the card. Grown rather
   than allocated per batch, like the world's. */
static tGdi_TexturedVertex* gl_sprVerts;
static int                  gl_sprVertMax;
static int                  gl_sprVertNum;

#define GL_SHADOW_TARGETS 8
#define GL_SHADOW_SIZE    128

/* The border the engine leaves untouched around a shadow target. The
   texture is sampled with clamped addressing, so the edge pixels are
   what spreads outwards past the shadow -- they have to stay as the
   clear left them, which means the viewport is inset by one. */
#define GL_SHADOW_BORDER  1

typedef struct sGlTarget
{
    GLuint fbo;
    GLuint color;      /* the texture a later draw samples */
    int    width;
    int    height;
} tGlTarget;

static tGlTarget gl_shadowTargets[GL_SHADOW_TARGETS];
static GLuint    gl_shadowDepth;
static int       gl_shadowTried;
static int       gl_shadowReady;

/* What was in force before a target was bound, so it can be put back.
   The projection is saved by GL_SetViewMatrixSDW and the viewport
   here, which is where the other renderer saves each of them -- the
   engine calls the two in that order. */
static int    gl_targetInForce;
static GLint  gl_savedViewport[4];
static Mtx4x4 gl_savedProjMtx;



/* Defined with the rest of the 2D pass, at the bottom; GL_Close has
   to call it while the context is still current. */
static void GL_l_ForgetPass(void);
static void GL_l_ForgetGeometry(void);
static void GL_l_ForgetScene(void);
static void GL_l_BindScene(void);
static void GL_l_CopyToScreen(GLuint fromFbo);
static void GL_l_ReadBlurState(tDpb_State* out);
static int  GL_l_b_BlurReady(void);
static void GL_l_ForgetBlur(void);
static void GL_l_ForgetTargets(void);
static void GL_l_ForgetVideo(void);
static GLuint GL_l_i_RunAfterEffects(void);
static void GL_l_ForgetAe(void);
static void GL_l_ForgetWater(void);
static void GL_l_ForgetWaterSmooth(void);
static void GL_l_SmoothWater(void);
static int  GL_l_b_WantsReflection(void);
static void GL_l_ReflectStaged(int count);
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
    /* The doubled modulate, which this renderer really does have: the
       world shader multiplies by two and that is the whole of it.
     *
       Answering no here is not a harmless understatement, which is why
       it is set rather than left at the memset's nought. The material
       path's own fallback for a renderer that cannot double is to
       **draw the geometry a second time additively** -- and the shader
       would double each of those passes, so the lit, shadowed geometry
       came out at four times the brightness it should be. The actors
       are where that shows, because theirs is the material kind that
       asks for the doubling. */
    gl_state.canDiffuse2x = 1;
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
    /* Kept current deliberately. This line was left saying the depth
       blur and the culling were missing for four changes after both
       went in, and it sent a search for why the world came out dark
       looking in the wrong place for an afternoon. A log that lies
       about what is running is worse than no log. */
    DBG_OutputDebugString("GL: this renderer draws the overlay, its text, "
                          "the textured world with fog and culling, its "
                          "shadows, its sprites, fullscreen video, the "
                          "depth blur and the sun's occlusion test, and "
                          "the after-effects for the depth blur, the "
                          "brightness, the contrast and the grayscale, "
                          "the remanence, the motion blur and the zoom "
                          "smooth, and the water with its reflection; "
                          "the water's clip plane is honoured and its "
                          "surface smoothing is not\n");
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

    /* The scene target, made and bound before anything draws. The
       copy at the end of a frame reads what the frame drew, so a
       target that only came into being at the end of the first frame
       would have the first frame copy nothing -- which it did, until
       this. */
    GL_l_BindScene();

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
        GL_l_ForgetBlur();
        /* Before the scene target, which the spares are sized from. */
        GL_l_ForgetAe();
        GL_l_ForgetScene();
        GL_l_ForgetTargets();
        GL_l_ForgetWater();
        GL_l_ForgetWaterSmooth();
        GL_l_ForgetVideo();
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

/* The colour write mask, through a setter for the same reason the
 * depth state goes through one: something else has to be able to put
 * it back.
 *
 * That something is the clear below. **OpenGL's clear is masked by the
 * colour write mask and the other renderer's is not**, and the engine
 * leans on the difference without knowing it: before a shadow target
 * is cleared it sets a material whose flags turn the colour write off
 * entirely -- 0x2260, which asks for sampler state and no drawing --
 * so a clear made straight through OpenGL writes nothing at all. The
 * target then keeps whatever was in it, which on a fresh texture is
 * undefined memory and afterwards is the last caster's silhouette. A
 * shadow is sampled as `dst * (1 - src)`, so a target full of bright
 * rubbish darkens everything it is sampled onto: a black rectangle
 * where a shadow should be.
 *
 * Three of the clears in this file already forced the mask on first
 * and four did not, which is the shape of a rule that has to live in
 * one place rather than be remembered at each site.
 */
static void GL_l_SetColorMask(int r, int g, int b, int a)
{
    gl_state.colorWriteOff[0] = r ? 0 : 1;
    gl_state.colorWriteOff[1] = g ? 0 : 1;
    gl_state.colorWriteOff[2] = b ? 0 : 1;
    gl_state.colorWriteOff[3] = a ? 0 : 1;
    GLFN_glColorMask(r ? GL_TRUE : GL_FALSE, g ? GL_TRUE : GL_FALSE,
                     b ? GL_TRUE : GL_FALSE, a ? GL_TRUE : GL_FALSE);
}

/* Clears the colour buffer whatever the mask says, and leaves the mask
   as it found it. Every colour clear in this file goes through here. */
static void GL_l_ClearColour(float r, float g, float b, float a, int more)
{
    int masked = gl_state.colorWriteOff[0] || gl_state.colorWriteOff[1]
                 || gl_state.colorWriteOff[2] || gl_state.colorWriteOff[3];

    if (masked)
        GLFN_glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    GLFN_glClearColor((GLfloat)r, (GLfloat)g, (GLfloat)b, (GLfloat)a);
    GLFN_glClear((GLbitfield)(GL_COLOR_BUFFER_BIT | more));
    if (masked)
        GLFN_glColorMask(gl_state.colorWriteOff[0] ? GL_FALSE : GL_TRUE,
                         gl_state.colorWriteOff[1] ? GL_FALSE : GL_TRUE,
                         gl_state.colorWriteOff[2] ? GL_FALSE : GL_TRUE,
                         gl_state.colorWriteOff[3] ? GL_FALSE : GL_TRUE);
}

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

    if (mask & GL_DEPTH_BUFFER_BIT)
    {
        /* Depth is only written by a clear if writing is on, which is
           a piece of GL state the engine sets for drawing. */
        GLFN_glDepthMask(GL_TRUE);
        GLFN_glClearDepth(1.0);
    }
    if (mask & GL_STENCIL_BUFFER_BIT)
        GLFN_glClearStencil(0);

    if (mask & GL_COLOR_BUFFER_BIT)
        GL_l_ClearColour((GLfloat)((color >> 16) & 0xFF) / 255.0f,
                         (GLfloat)((color >> 8) & 0xFF) / 255.0f,
                         (GLfloat)(color & 0xFF) / 255.0f,
                         (GLfloat)((color >> 24) & 0xFF) / 255.0f,
                         (int)(mask & ~(GLbitfield)GL_COLOR_BUFFER_BIT));
    else
        GLFN_glClear(mask);

    if ((mask & GL_DEPTH_BUFFER_BIT) && !gl_state.zWrite)
        GLFN_glDepthMask(GL_FALSE);
}

void GL_Flip(void)
{
    /* The after-effects first, over the frame in the scene target.
       They run as a chain of passes between spare targets, so what
       comes back is whichever target the last one wrote -- the scene
       target itself when none of them ran. */
    GLuint shown = GL_l_i_RunAfterEffects();

    /* Then that on the screen, letterboxed if a cutscene asked for
       it. */
    GL_l_CopyToScreen(shown);

    /* Then the debug overlay, on the screen rather than in the scene
       target: it goes on top of everything, outside the letterbox,
       which is where the other renderer puts it too -- after its own
       copy to the screen.
     *
       This is engine work rather than a renderer's, and the other
       renderer does it at the same point for the same reason: the one
       neutral caller of Flip cannot do it instead, because the other
       renderer updates the rasters inside its own Flip and the overlay
       has to be drawn after that or it shows the frame before's
       numbers. Without this call the overlay was never drawn at all
       under this renderer. */
    OVL_Draw();

    if (gl_state.dc)
        SwapBuffers(gl_state.dc);

    /* And back to the scene target for the next frame, so that
       nothing has to remember to switch it on. This is also where a
       target of the wrong size is rebuilt: at the end of a frame,
       so the next one draws into the right one -- rebuilding inside
       the copy would throw away the frame it was about to copy. */
    GL_l_BindScene();
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
/* Whether the engine should ask for the depth-blur pass at all. The
   material path calls this before every one, so answering no is how
   the pass stays out of the frame; it is the honest answer whenever
   there is no blur in force or no program to draw it with. */
int GL_b_DepthBlurOn(void)
{
    tDpb_State want;

    if (!gl_state.context)
        return 0;
    GL_l_ReadBlurState(&want);
    if (want.kind == DPB_OFF)
        return 0;
    return GL_l_b_BlurReady();
}

void GL_SetCanDiffuse2x(int on) { gl_state.canDiffuse2x = on ? 1 : 0; }
void GL_SetWantsNoFlare(int on) { gl_state.wantsNoFlare = on ? 1 : 0; }
void GL_SetNoZonAlpha(int on)   { gl_state.noZonAlpha = on ? 1 : 0; }

/* ------------------------------------------------- geometry caching

   Nothing is cached. Answering no is not a gap: it sends the engine
   down the immediate path, which this renderer draws, and it means
   the engine never asks for a cache to be dropped -- so dropping one
   is nothing to do rather than something to get wrong. */

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
/* The winding in force and whether culling is on at all, kept because
   the water's reflection has to draw the same geometry with the facing
   the other way round and put it back. Reading it out of the driver
   would be a round trip per object, and the renderer is the only thing
   that sets it. */
static GLenum gl_frontFace = GL_CCW;
static int    gl_cullOn;

static tGlProgram gl_geoProgram;
static GLint      gl_geoViewProj = -1;
static GLint      gl_geoTex = -1;
static GLint      gl_geoTextured = -1;
static GLint      gl_geoAlphaMode = -1;
static GLint      gl_geoAlphaRef = -1;
static GLint      gl_geoDiffuse2x = -1;
static GLint      gl_geoView = -1;
static GLint      gl_geoFogCurve = -1;
static GLint      gl_geoFogStart = -1;
static GLint      gl_geoFogEnd = -1;
static GLint      gl_geoFogDensity = -1;
static GLint      gl_geoFogColor = -1;
static GLint      gl_geoUVMode = -1;
static GLint      gl_geoUVMtx = -1;
/* The water's reflection reads these three and nothing else does. */
static GLint      gl_geoClipping = -1;
static GLint      gl_geoClipPlane = -1;
static GLint      gl_geoWorld = -1;

/* The material's texture-coordinate projection. The engine computes a
   transform into d3dTransformationMatrix and says which source it
   applies to in GDI_gi_TextureUVMode; both are the engine's and are
   written whichever renderer is in force. Mode 2 wants the
   camera-space normal, which this renderer's vertex does not carry --
   the engine keeps normals in a stream of their own that this renderer
   does not upload -- so that mode is reported once and drawn as mode
   nought. */
static GLuint gl_geoNrmBuffer;
static Vec3*  gl_geoNrms;
static int    gl_geoNrmMax;
/* Whether the normal attribute's array is enabled, so it is switched
   only when it changes: with the array disabled the shader reads the
   generic value, which the other modes never look at. */
static int    gl_geoNrmOn;
/* Whether this draw has normals to upload, decided before the draw and
   read inside it. */
static int    gl_geoNrmWanted;

/* ------------------------------------------------------------- fog
 *
 * The engine hands over a curve, a range, a density and a colour once,
 * and then each material says whether fog applies to it and which of
 * two colours it uses. fog.c reads that; what is here is the uniforms
 * and the one piece of state the material flags cannot carry.
 *
 * That piece is the variant. Three of the four the flags can name pick
 * a colour and the fourth means keep the one already in force, so the
 * current variant has to be remembered between materials -- exactly as
 * the other renderer remembers its own. */
static tFog_State gl_fog;
static int        gl_fogSecondColor;   /* the variant last named */
static int        gl_fogSaidNoVertex;  /* the one-off complaint below */

/* ------------------------------------------------- the depth blur
 *
 * The same vertices, a second time, through a 256 by 1 ramp of
 * blur-amount-per-depth, writing the frame's alpha and nothing else.
 * The ramp's coordinate comes from the camera-space position, so the
 * coordinate is the depth; alpha 255 means no blur and 0 means full,
 * which is why the ramp is solid when no blur is in force.
 *
 * depthblur.c decides which blur and builds the table. What is here is
 * the texture it becomes, the coordinate the shader is told to make,
 * and the narrowed write.
 *
 * Nothing in this renderer reads the alpha yet -- the after-effects
 * are the other renderer's -- so this writes the channel the sun's
 * occlusion test will read when that follows. b_DepthBlurOn is what
 * asks the engine for the pass at all, so until the write is right it
 * is better for that to say no than for the pass to run and put
 * something arbitrary in the frame's alpha.
 */
static tGlProgram gl_blurProgram;
static GLint      gl_blurViewProj = -1;
static GLint      gl_blurView = -1;
static GLint      gl_blurRamp = -1;
static GLint      gl_blurProjected = -1;
static GLint      gl_blurTranslateU = -1;
static GLint      gl_blurScaleW = -1;
static GLint      gl_blurScaleUfromZ = -1;
static GLuint     gl_blurTex;
static int        gl_blurTried;
static int        gl_blurReady;

/* The decision the ramp was last built for, so the table is rebuilt
   when it changes and not once a frame. */
static tDpb_State gl_blurState;
static int        gl_blurHaveState;

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
    gl_geoTex = GL_i_ProgramUniform(&gl_geoProgram, "uTex");
    gl_geoTextured = GL_i_ProgramUniform(&gl_geoProgram, "uTextured");
    gl_geoAlphaMode = GL_i_ProgramUniform(&gl_geoProgram, "uAlphaMode");
    gl_geoAlphaRef = GL_i_ProgramUniform(&gl_geoProgram, "uAlphaRef");
    gl_geoDiffuse2x = GL_i_ProgramUniform(&gl_geoProgram, "uDiffuse2x");
    gl_geoView = GL_i_ProgramUniform(&gl_geoProgram, "uView");
    gl_geoFogCurve = GL_i_ProgramUniform(&gl_geoProgram, "uFogCurve");
    gl_geoFogStart = GL_i_ProgramUniform(&gl_geoProgram, "uFogStart");
    gl_geoFogEnd = GL_i_ProgramUniform(&gl_geoProgram, "uFogEnd");
    gl_geoFogDensity = GL_i_ProgramUniform(&gl_geoProgram, "uFogDensity");
    gl_geoFogColor = GL_i_ProgramUniform(&gl_geoProgram, "uFogColor");
    gl_geoUVMode = GL_i_ProgramUniform(&gl_geoProgram, "uUVMode");
    gl_geoUVMtx = GL_i_ProgramUniform(&gl_geoProgram, "uUVMtx");
    gl_geoClipping = GL_i_ProgramUniform(&gl_geoProgram, "uClipping");
    gl_geoClipPlane = GL_i_ProgramUniform(&gl_geoProgram, "uClipPlane");
    gl_geoWorld = GL_i_ProgramUniform(&gl_geoProgram, "uWorld");

    GLFN_glGenBuffers(1, &gl_geoBuffer);
    GLFN_glGenBuffers(1, &gl_geoNrmBuffer);
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

    /* The normals, in a buffer of their own because the engine keeps
       them in an array of their own. Described here and left
       disabled: only the projection that reads a normal turns it on,
       and the sprites share this vertex array and have none. */
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_geoNrmBuffer);
    GLFN_glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(Vec3), (const void*)(size_t)0);
    GLFN_glDisableVertexAttribArray(3);
    gl_geoNrmOn = 0;
    GLFN_glBindVertexArray(0);

    gl_geoReady = 1;
    return 1;
}

/* What the engine's state says the blur should be, right now. The
   four values it is built from are declared in gdi.h: they are the
   after-effects state the AI scripts move, kept whichever renderer is
   in force, so reading them here is reading live state and not the
   other renderer's leftovers. */
static void GL_l_ReadBlurState(tDpb_State* out)
{
    tDpb_Source src;

    memset(&src, 0, sizeof(src));
    /* noZonAlpha is this renderer's name for the tweak that turns the
       blur off; the engine sets it through the interface. */
    src.b_zonAlphaTweak = gl_state.noZonAlpha;
    src.aeMaster = (unsigned int)PC_AE_MASTER;
    src.b_sunEffectOn = gl_state.sunEffectOn;
    if (gpst_DX_stDD)
    {
        src.nearPlane = gpst_DX_stDD->ct.cam.nearPlane;
        src.farPlane = gpst_DX_stDD->ct.cam.farPlane;
    }
    src.depthBlurNear1 = depthBlurNear1;
    src.depthBlurType0 = depthBlurType0;
    src.depthBlurType1 = depthBlurType1;

    DPB_Decode(&src, out);
}

/* The ramp as a texture: 256 by 1, one byte each, which is all the
   table carries -- the engine builds it as words with the amount in
   the top byte and nothing in the other three. Linear and clamped,
   which is what the other renderer's sampler asks for. */
static void GL_l_UploadRamp(const tDpb_State* st)
{
    int           table[256];
    unsigned char bytes[256];
    int           i;

    DPB_BuildRamp(st, table);
    for (i = 0; i < 256; i++)
        bytes[i] = (unsigned char)(((unsigned int)table[i] >> 24) & 0xFF);

    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_blurTex);
    GLFN_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 1, 0, GL_RED,
                      GL_UNSIGNED_BYTE, bytes);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static int GL_l_b_BlurReady(void)
{
    if (gl_blurTried)
        return gl_blurReady;
    gl_blurTried = 1;
    if (!gl_state.context)
        return 0;

    if (!GL_b_ProgramLoad(&gl_blurProgram, "depthblur.vert", "depthblur.frag"))
        return 0;
    gl_blurViewProj = GL_i_ProgramUniform(&gl_blurProgram, "uViewProj");
    gl_blurView = GL_i_ProgramUniform(&gl_blurProgram, "uView");
    gl_blurRamp = GL_i_ProgramUniform(&gl_blurProgram, "uRamp");
    gl_blurProjected = GL_i_ProgramUniform(&gl_blurProgram, "uProjected");
    gl_blurTranslateU = GL_i_ProgramUniform(&gl_blurProgram, "uTranslateU");
    gl_blurScaleW = GL_i_ProgramUniform(&gl_blurProgram, "uScaleW");
    gl_blurScaleUfromZ = GL_i_ProgramUniform(&gl_blurProgram, "uScaleUfromZ");

    GLFN_glGenTextures(1, &gl_blurTex);
    if (!gl_blurTex)
    {
        DBG_OutputDebugString("GL: no texture for the blur ramp\n");
        GL_ProgramFree(&gl_blurProgram);
        return 0;
    }

    gl_blurReady = 1;
    gl_blurHaveState = 0;
    return 1;
}

static void GL_l_ForgetBlur(void)
{
    if (gl_blurReady && gl_state.context)
    {
        GL_ProgramFree(&gl_blurProgram);
        if (gl_blurTex)
            GLFN_glDeleteTextures(1, &gl_blurTex);
    }
    gl_blurTex = 0;
    gl_blurViewProj = -1;
    gl_blurView = -1;
    gl_blurRamp = -1;
    gl_blurProjected = -1;
    gl_blurTranslateU = -1;
    gl_blurScaleW = -1;
    gl_blurScaleUfromZ = -1;
    gl_blurTried = 0;
    gl_blurReady = 0;
    gl_blurHaveState = 0;
}

static void GL_l_ForgetGeometry(void)
{
    if (gl_geoReady && gl_state.context)
    {
        GL_ProgramFree(&gl_geoProgram);
        if (gl_geoBuffer)
            GLFN_glDeleteBuffers(1, &gl_geoBuffer);
        if (gl_geoNrmBuffer)
            GLFN_glDeleteBuffers(1, &gl_geoNrmBuffer);
        if (gl_geoVao)
            GLFN_glDeleteVertexArrays(1, &gl_geoVao);
    }
    gl_geoBuffer = 0;
    gl_geoNrmBuffer = 0;
    gl_geoNrmOn = 0;
    free(gl_geoNrms);
    gl_geoNrms = NULL;
    gl_geoNrmMax = 0;
    gl_geoVao = 0;
    gl_geoViewProj = -1;
    gl_geoTex = -1;
    gl_geoTextured = -1;
    gl_geoAlphaMode = -1;
    gl_geoAlphaRef = -1;
    gl_geoDiffuse2x = -1;
    gl_geoView = -1;
    gl_geoFogCurve = -1;
    gl_geoFogStart = -1;
    gl_geoFogEnd = -1;
    gl_geoFogDensity = -1;
    gl_geoFogColor = -1;
    gl_geoUVMode = -1;
    gl_geoUVMtx = -1;
    gl_geoClipping = -1;
    gl_geoClipPlane = -1;
    gl_geoWorld = -1;
    gl_geoTried = 0;
    gl_geoReady = 0;
    free(gl_geoVerts);
    gl_geoVerts = NULL;
    gl_geoVertMax = 0;
    free(gl_sprVerts);
    gl_sprVerts = NULL;
    gl_sprVertMax = 0;
    gl_sprVertNum = 0;
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

/* The one place vertices reach the card: the world's triangles and
   the sprites' quads are the same vertex and the same program, so
   they are the same draw with a different staging buffer behind
   them. */
static void GL_l_DrawStaged(const tGdi_TexturedVertex* verts, int count,
                            int b_depthBlur, int b_normals)
{
    if (!verts || count <= 0)
        return;

    /* The depth-blur pass is the same vertices through a different
       program, so this is where the two part company. Everything below
       belongs to the world's program and would be set on the wrong one
       otherwise.

       Which pass this is comes in as an argument rather than being read
       off the material, because the sprites share this draw and the
       blur never applies to them. Read from the material it would have
       depended on the engine happening to call TextureInit between a
       blur pass and the sprites -- and a sprite drawn through the blur
       program is a sprite that writes alpha and no colour, which is a
       sprite that is not there. */
    if (b_depthBlur)
    {
        GLFN_glUseProgram(gl_blurProgram.program);
        if (gl_blurViewProj >= 0)
            GLFN_glUniformMatrix4fv(gl_blurViewProj, 1, GL_FALSE,
                                    (const GLfloat*)&gl_viewProjMtx);
        if (gl_blurView >= 0)
            GLFN_glUniformMatrix4fv(gl_blurView, 1, GL_FALSE,
                                    (const GLfloat*)&gl_viewMtx);
        if (gl_blurRamp >= 0)
            GLFN_glUniform1i(gl_blurRamp, 0);
        if (gl_blurProjected >= 0)
            GLFN_glUniform1i(gl_blurProjected, gl_blurState.b_projected);
        if (gl_blurTranslateU >= 0)
            GLFN_glUniform1f(gl_blurTranslateU, gl_blurState.translateU);
        if (gl_blurScaleW >= 0)
            GLFN_glUniform1f(gl_blurScaleW, gl_blurState.scaleW);
        if (gl_blurScaleUfromZ >= 0)
            GLFN_glUniform1f(gl_blurScaleUfromZ, gl_blurState.scaleUfromZ);

        GLFN_glBindVertexArray(gl_geoVao);
        GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_geoBuffer);
        GLFN_glBufferData(GL_ARRAY_BUFFER,
                          (GLsizei)(sizeof(tGdi_TexturedVertex)
                                    * (size_t)count),
                          verts, GL_STREAM_DRAW);
        GLFN_glDrawArrays(GL_TRIANGLES, 0, (GLsizei)count);
        GLFN_glBindVertexArray(0);
        return;
    }

    GLFN_glUseProgram(gl_geoProgram.program);
    if (gl_geoViewProj >= 0)
        GLFN_glUniformMatrix4fv(gl_geoViewProj, 1, GL_FALSE,
                                (const GLfloat*)&gl_viewProjMtx);
    /* The material, as the last GL_TextureInit left it. The texture
       itself is already bound to unit zero there; what the shader
       cannot be told through the pipeline goes here. */
    if (gl_geoTextured >= 0)
        GLFN_glUniform1i(gl_geoTextured, gl_material.textured);
    if (gl_geoAlphaMode >= 0)
        GLFN_glUniform1i(gl_geoAlphaMode, gl_material.alphaMode);
    if (gl_geoAlphaRef >= 0)
        GLFN_glUniform1f(gl_geoAlphaRef, gl_material.alphaRef);
    if (gl_geoTex >= 0)
        GLFN_glUniform1i(gl_geoTex, 0);
    /* A display setting, read at the draw rather than kept with the
       material, because the engine can change it between the two.
     *
       **Both switches, as the other renderer's stage needs both.** It
       asks for the doubled modulate only when the engine wants the
       doubling *and* the renderer says it can, and takes the plain
       modulate otherwise -- because when it cannot, the material path
       draws the geometry a second time additively to double it, and a
       shader that doubled as well would double that too. The
       capability is true here, so this is normally just the engine's
       setting; the tweak console can turn it off, and then this
       renderer has to behave like a card without the op. */
    if (gl_geoDiffuse2x >= 0)
        GLFN_glUniform1i(gl_geoDiffuse2x,
                         (GDI_gi_Diffuse2x && gl_state.canDiffuse2x)
                         ? 1 : 0);

    /* The material's texture-coordinate projection, read at the draw
       rather than kept with the material: the engine computes it after
       TextureInit and before the draw, which is the one window in
       which it is right. */
    if (gl_geoUVMode >= 0)
    {
        int mode = GDI_gi_TextureUVMode;

        /* Modes 4 and 5 belong to the depth-blur pass, which has its
           own program and never reaches here. Mode 2 wants a normal
           there is none of. */
        /* Mode 2 reads the normal, which only a draw that has one
           can. Without it the stored coordinate is drawn, which is
           visibly unprojected rather than a normal invented from
           nothing.

           Asked of this draw's own answer and not of the attribute's
           state: that state is switched further down this function, so
           reading it here answered for the draw before -- which meant
           the first chrome-mapped material of a frame was always drawn
           unprojected. */
        if (mode == 2 && !b_normals)
            mode = 0;
        else if (mode != 1 && mode != 2 && mode != 3)
            mode = 0;
        GLFN_glUniform1i(gl_geoUVMode, mode);
        if (mode != 0 && gl_geoUVMtx >= 0)
            GLFN_glUniformMatrix4fv(gl_geoUVMtx, 1, GL_FALSE,
                                    (const GLfloat*)&d3dTransformationMatrix[0]);
    }

    /* Fog. The view matrix goes with it, because the depth the curve
       is a function of is the one thing the combined matrix has thrown
       away. */
    if (gl_geoView >= 0)
        GLFN_glUniformMatrix4fv(gl_geoView, 1, GL_FALSE,
                                (const GLfloat*)&gl_viewMtx);
    if (gl_geoFogCurve >= 0)
        GLFN_glUniform1i(gl_geoFogCurve, gl_material.fogCurve);
    if (gl_material.fogCurve != FOG_OFF)
    {
        unsigned int c = FOG_i_Color(&gl_fog, gl_fogSecondColor);

        if (gl_geoFogStart >= 0)
            GLFN_glUniform1f(gl_geoFogStart, gl_fog.start);
        if (gl_geoFogEnd >= 0)
            GLFN_glUniform1f(gl_geoFogEnd, gl_fog.end);
        if (gl_geoFogDensity >= 0)
            GLFN_glUniform1f(gl_geoFogDensity, gl_fog.density);
        /* A8R8G8B8, so red is the second byte down. */
        if (gl_geoFogColor >= 0)
            GLFN_glUniform3f(gl_geoFogColor,
                             (GLfloat)((c >> 16) & 0xFF) / 255.0f,
                             (GLfloat)((c >> 8) & 0xFF) / 255.0f,
                             (GLfloat)(c & 0xFF) / 255.0f);
    }
    GLFN_glBindVertexArray(gl_geoVao);

    /* The normals, switched rather than set every draw: the sprites
       share this vertex array and have none, so leaving the attribute
       enabled would have them read a buffer belonging to the last
       piece of world geometry. */
    if (b_normals != gl_geoNrmOn)
    {
        if (b_normals)
            GLFN_glEnableVertexAttribArray(3);
        else
            GLFN_glDisableVertexAttribArray(3);
        gl_geoNrmOn = b_normals;
    }
    if (b_normals)
    {
        GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_geoNrmBuffer);
        GLFN_glBufferData(GL_ARRAY_BUFFER,
                          (GLsizei)(sizeof(Vec3) * (size_t)count),
                          gl_geoNrms, GL_STREAM_DRAW);
    }

    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_geoBuffer);
    GLFN_glBufferData(GL_ARRAY_BUFFER,
                      (GLsizei)(sizeof(tGdi_TexturedVertex) * (size_t)count),
                      verts, GL_STREAM_DRAW);
    GLFN_glDrawArrays(GL_TRIANGLES, 0, (GLsizei)count);
    GLFN_glBindVertexArray(0);
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

    /* The normals, when the material's projection is the one that
       reads them. The engine keeps them on the geometry rather than
       handing them to the draw, which is where the other renderer
       finds them too. */
    gl_geoNrmWanted = 0;
    if (GDI_gi_TextureUVMode == 2 && gpst_DX_stDD->GROGEO
        && gpst_DX_stDD->GROGEO->normals)
    {
        if (needed > gl_geoNrmMax)
        {
            Vec3* grownNrm;

            grownNrm = (Vec3*)realloc(gl_geoNrms,
                                      sizeof(*grownNrm) * (size_t)needed);
            if (grownNrm)
            {
                gl_geoNrms = grownNrm;
                gl_geoNrmMax = needed;
            }
        }
        if (needed <= gl_geoNrmMax)
        {
            GVX_BuildNormals(gl_geoNrms, chunks,
                             gpst_DX_stDD->GROGEO->normals);
            gl_geoNrmWanted = 1;
        }
    }

    bb = gpst_DX_stDD->bigBuffer;
    src.vertices = vertices;
    src.uvs = uvs;
    src.colors = bb ? bb->vertexColorArray : NULL;
    src.second = bb ? bb->vertexColorArraySecond : NULL;
    src.mask = bb ? bb->colorMask : 0;

    /* The alpha the material asked for, which the other renderer also
       keeps in its own state and reads here. */
    src.alpha = gl_colorMaskAlpha;
    src.solidColor = src.colors
                         ? 0
                         : GVX_ui_SolidColor(src.mask, src.alpha,
                                             bb ? bb->solidVertexColor : 0);
    src.useVertexUv = (gpst_DX_stDD->flags & 2) != 0;

    GVX_BuildTriangles(gl_geoVerts, chunks, &src);

    GL_l_DrawStaged(gl_geoVerts, needed, gl_material.b_depthBlur,
                    gl_geoNrmWanted);

    /* And again into the water's reflection, mirrored, when this
       object is worth reflecting. The depth-blur pass is excluded
       because it is a different program writing a blur amount rather
       than a picture -- there is nothing in it for the water to
       mirror, and the uniforms the reflection sets belong to the
       world's program. */
    if (!gl_material.b_depthBlur && GL_l_b_WantsReflection())
        GL_l_ReflectStaged(needed);
    return 0;
}

/* ------------------------------------------------------- textures
 *
 * The engine refers to a texture by its index into TEX_gst_GlobalList,
 * and by nothing else: that index is what arrives in an upload
 * request and what arrives with every material. So the table below is
 * the whole mapping, one OpenGL name per entry.
 *
 * Two indices are not indices. -1 means "no texture, use the vertex
 * colour alone", and -2 means the depth-blur ramp -- a 256 by 1 table
 * the other renderer looks a blur amount up in, which waits on the
 * depth-blur pass. A negative index with the top bit set is a shadow
 * target, which is a texture this renderer does have.
 */

/* Standing in for a shadow target when one is sampled as a texture.
   Filled on demand because the target's name is not known until the
   targets are built. */
static tGlTexture gl_shadowAsTexture;

static tGlTexture* GL_l_p_Texture(int textureID)
{
    /* A negative index is a shadow target with the top bit set, which
       is what the other renderer masks off before indexing its own
       array of them. It does not then check the range, so -1 and -2
       -- which mean "no texture" and "the renderer's own copy" --
       reach it as indices a billion entries past the end. Checked
       here, so those two land as no texture at all. */
    if (textureID < 0)
    {
        int idx = (int)((unsigned int)textureID & ~0x80000000u);

        if (idx >= GL_SHADOW_TARGETS || !gl_shadowReady)
            return NULL;
        if (!gl_shadowTargets[idx].color)
            return NULL;
        gl_shadowAsTexture.name = gl_shadowTargets[idx].color;
        gl_shadowAsTexture.hasLevels = 0;
        return &gl_shadowAsTexture;
    }
    if (!gl_textures || textureID < 0 || textureID >= gl_textureNum)
        return NULL;
    if (!gl_textures[textureID].name)
        return NULL;
    return &gl_textures[textureID];
}

int GL_TextureStore(struct sGdi_DisplayData* dd, int textureNum)
{
    int count;

    (void)dd;

    /* One more entry than the engine asked for, and at least two: the
       other renderer sizes its array the same way, and the engine
       does index the extra one. */
    count = textureNum ? textureNum : 1;
    GL_ReleaseTextures(dd);
    gl_textures = (tGlTexture*)calloc((size_t)count + 1, sizeof(tGlTexture));
    if (!gl_textures)
        return 0;
    gl_textureNum = count + 1;
    return 1;
}

void GL_ReleaseTextures(struct sGdi_DisplayData* dd)
{
    int i;

    (void)dd;

    if (!gl_textures)
        return;
    if (gl_state.context)
        for (i = 0; i < gl_textureNum; i++)
            if (gl_textures[i].name)
                GLFN_glDeleteTextures(1, &gl_textures[i].name);
    free(gl_textures);
    gl_textures = NULL;
    gl_textureNum = 0;
}

/* How many mip levels a texture of this size has, counting the full
   one. OpenGL wants the number; the other renderer passes zero and
   lets the device work it out. */
static int GL_l_i_LevelCount(int width, int height)
{
    int longest = width > height ? width : height;
    int levels = 1;

    while (longest > 1)
    {
        longest >>= 1;
        levels++;
    }
    return levels;
}

/* The one place a texture's pixels reach the card. The engine has
   already converted whatever was in the file into 32-bit words in the
   order the renderer wants, so there is nothing to unpack here: the
   rows are tightly packed and four bytes to a pixel. */
static int GL_l_b_Upload(tGlTexture* slot, int width, int height,
                         const void* pixels, int wantLevels)
{
    GLuint name = 0;

    if (!gl_state.context || width <= 0 || height <= 0 || !pixels)
        return 0;

    GLFN_glGenTextures(1, &name);
    if (!name)
        return 0;
    GLFN_glBindTexture(GL_TEXTURE_2D, name);
    GLFN_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    GLFN_glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                      GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    slot->hasLevels = 0;
    if (wantLevels)
    {
        /* The levels have to exist before a sampler is allowed to ask
           for them: a mip-filtered sampler on a texture with only the
           full level is incomplete, and an incomplete texture samples
           as black rather than failing. That is why the slot
           remembers, and why GL_l_TextureSampler reads it. */
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,
                             GL_l_i_LevelCount(width, height) - 1);
        GLFN_glGenerateMipmap(GL_TEXTURE_2D);
        slot->hasLevels = 1;
    }
    else
    {
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    }

    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    slot->name = name;
    return 1;
}

void GL_PaletteLoad(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle,
                    struct sEngTexIntermediateStruct* inter, int idx)
{
    int     depth;
    int     width;
    int     height;
    int16_t texFlags;
    int     wantLevels;

    if (!dd || !tle || !inter)
        return;

    depth = inter->colorDepth2;

    /* Preserved from the other renderer, and the other way round than
       it reads: the element's width field holds the height of the
       picture and its height field holds the width. textures.c fills
       them that way and both renderers have to undo it the same way
       or every texture arrives transposed. */
    width = tle->normalData.height;
    height = tle->normalData.width;

    /* Whether this texture gets a mip chain, and the engine's own
       record of it: tle->texElemFlag is read outside the renderer, so
       it is set here exactly as the other renderer sets it. */
    texFlags = inter->tp.flagsUnkAndProceduralUnkb1;
    if (!gl_mipmapping || (texFlags & 0x40) != 0 || (texFlags & 0x200) != 0
        || inter->tp.type == 5)
    {
        inter->bmpUnkb1 &= ~0x10u;
        wantLevels = 0;
    }
    else
    {
        inter->bmpUnkb1 |= 0x10u;
        wantLevels = 1;
    }
    tle->texElemFlag = inter->bmpUnkb1;

    if (idx < 0 || idx >= gl_textureNum)
        return;
    if (depth != 32 && depth != 24)
    {
        /* The engine converts both of those to 32-bit words before
           handing them over; anything else it does not convert, and
           the other renderer leaves the slot empty too. */
        gl_textures[idx].name = 0;
        gl_textures[idx].hasLevels = 0;
        return;
    }

    if (GL_l_b_Upload(&gl_textures[idx], width, height, inter->imgDataPtr,
                      wantLevels))
        dd->texManager.paletteUsedMemory += (depth == 32 ? 4 : 3)
                                            * width * height;
}

/* The engine's procedural textures hand their pixels over this way,
   once a frame each. */
/* The engine's fog parameters, which arrive once and then stand until
   they are replaced. */
static void GL_l_FogRequest(tFogParams* fog)
{
    tFog_Source src;

    if (!fog)
        return;

    src.b_enabled = (fog->flags & 1) != 0;
    src.engineCurve = fog->fogVertexMode;
    src.start = fog->fogStart;
    src.end = fog->fogEnd;
    src.density = fog->density;
    src.engineColor = (unsigned int)fog->fogColor;

    FOG_Decode(&src, &gl_fog);
    /* New parameters, so the variant starts from the first colour --
       which is where the other renderer starts it too. */
    gl_fogSecondColor = 0;

    /* The engine's fourth curve takes its factor off the vertices
       rather than off the distance, and this renderer has no
       per-vertex fog to take it from: the engine's vertex carries a
       position, a colour and a texture coordinate and nothing else.
       So that case draws no fog, and says so once rather than being
       silently wrong. */
    if (gl_fog.b_on && gl_fog.curve == FOG_OFF && !gl_fogSaidNoVertex)
    {
        gl_fogSaidNoVertex = 1;
        DBG_OutputDebugString("GL: the world asked for per-vertex fog,"
                              " which this renderer has no source for;"
                              " drawing none\n");
    }
}

static void GL_l_TextureStoreRequest(tTexUploadRequest* req)
{
    tEngTexListElemStruct* tle;
    tGlTexture*            slot;
    int                    width;
    int                    height;

    if (!req || !req->dataPtr || !TEX_gst_GlobalList.texElems)
        return;
    if ((int)req->idx < 0 || (int)req->idx >= gl_textureNum)
        return;

    tle = &TEX_gst_GlobalList.texElems[req->idx];
    width = tle->normalData.height;   /* as in GL_PaletteLoad */
    height = tle->normalData.width;
    slot = &gl_textures[req->idx];

    /* An existing name is replaced in place rather than recreated: a
       procedural texture is re-uploaded every frame it changes, and
       the sampler state set on it would be lost with it. */
    if (slot->name && gl_state.context)
    {
        GLFN_glBindTexture(GL_TEXTURE_2D, slot->name);
        GLFN_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        GLFN_glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                             GL_BGRA, GL_UNSIGNED_BYTE, req->dataPtr);
        if (slot->hasLevels)
            GLFN_glGenerateMipmap(GL_TEXTURE_2D);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }
    GL_l_b_Upload(slot, width, height, req->dataPtr, 0);
}

int GL_Request(int type, void* param)
{
    switch (type)
    {
    case 1:
        GL_l_FogRequest((tFogParams*)param);
        break;
    case 12:
        GL_l_TextureStoreRequest((tTexUploadRequest*)param);
        break;
    default:
        /* Sprites, snapshots and the rest still wait on the passes
           that use them. */
        break;
    }
    return 0;
}

/* --------------------------------------------- the per-draw state */

/* What the fragment shader needs that the pipeline cannot hold: core
   OpenGL has no alpha test, so the comparison is done there. */
/* The min filter is the two filters together, and which combinations
   are allowed depends on the texture: see the comment in
   GL_l_b_Upload. */
static GLenum GL_l_e_MinFilter(const tRst_State* st, int hasLevels)
{
    if (!hasLevels)
        return st->filter == RST_LINEAR ? GL_LINEAR : GL_NEAREST;
    if (st->filter == RST_LINEAR)
        return st->mipFilter == RST_LINEAR ? GL_LINEAR_MIPMAP_LINEAR
                                           : GL_LINEAR_MIPMAP_NEAREST;
    return st->mipFilter == RST_LINEAR ? GL_NEAREST_MIPMAP_LINEAR
                                       : GL_NEAREST_MIPMAP_NEAREST;
}

static void GL_l_Blend(const tRst_State* st)
{
    GLenum src;
    GLenum dst;

    switch (st->blend)
    {
    case RST_BLEND_KEEP:
        /* The material names no equation, so the last one stands.
           The other renderer does the same. */
        return;
    case RST_BLEND_OFF:
        GLFN_glDisable(GL_BLEND);
        GLFN_glBlendFunc(GL_ONE, GL_ZERO);
        return;
    case RST_BLEND_SRC_ALPHA:
        src = st->blendInverted ? GL_ONE_MINUS_SRC_ALPHA : GL_SRC_ALPHA;
        dst = st->blendInverted ? GL_SRC_ALPHA : GL_ONE_MINUS_SRC_ALPHA;
        break;
    case RST_BLEND_PREMULTIPLIED:
        src = GL_ONE;
        dst = st->blendInverted ? GL_SRC_ALPHA : GL_ONE_MINUS_SRC_ALPHA;
        break;
    case RST_BLEND_DEST_ALPHA:
        src = st->blendInverted ? GL_ONE_MINUS_DST_ALPHA : GL_DST_ALPHA;
        dst = st->blendInverted ? GL_DST_ALPHA : GL_ONE_MINUS_DST_ALPHA;
        break;
    case RST_BLEND_DEST_ALPHA_ADD:
        src = st->blendInverted ? GL_ONE_MINUS_DST_ALPHA : GL_DST_ALPHA;
        dst = GL_ONE;
        break;
    case RST_BLEND_ADD:
        src = GL_ONE;
        dst = GL_ONE;
        break;
    case RST_BLEND_INV_SRC_COLOR:
        src = GL_ZERO;
        dst = GL_ONE_MINUS_SRC_COLOR;
        break;
    default:
        return;
    }
    GLFN_glEnable(GL_BLEND);
    GLFN_glBlendFunc(src, dst);
}

void GL_TextureInit(int textureID, int flags, char mode)
{
    tRst_State  st;
    tGlTexture* slot;

    if (!gl_state.context || !gpst_DX_stDD)
        return;

    if ((mode & 1) != 0)
    {
        /* The depth-blur pass, and it is not what its shape suggests.
           The texture it samples is not a picture of anything: it is a
           256 by 1 ramp the after-effects fill with a blur amount per
           depth. The world's geometry is drawn through it with texture
           coordinates taken from the camera-space position and divided
           through, so the coordinate is the depth, and the colour write
           is narrowed to the alpha channel -- so what the pass leaves
           behind is a per-pixel blur amount in the frame's alpha, for
           the after-effects to read.

           The engine asks for this pass only when b_DepthBlurOn says
           yes, so reaching here means there is a blur and a program
           for it. The ramp is rebuilt when the decision changes rather
           than once a frame.

           The texture-UV mode the other renderer sets here is read
           only by that renderer, so it is not set from here. */
        tDpb_State want;

        if (!GL_l_b_BlurReady())
            return;

        GL_l_ReadBlurState(&want);
        if (!gl_blurHaveState
            || memcmp(&want, &gl_blurState, sizeof(want)) != 0)
        {
            gl_blurState = want;
            gl_blurHaveState = 1;
            GL_l_UploadRamp(&gl_blurState);
        }
        else
        {
            GLFN_glActiveTexture(GL_TEXTURE0);
            GLFN_glBindTexture(GL_TEXTURE_2D, gl_blurTex);
        }

        /* Alpha and nothing else, which is the whole point of the
           pass. Depth testing stays on so the nearest surface is the
           one whose blur amount lands, and depth writing goes off so
           the pass does not disturb the buffer the first pass left. */
        GL_l_SetColorMask(0, 0, 0, 1);
        GL_SetZWriteEnable(0);
        GLFN_glDisable(GL_BLEND);
        gl_material.alphaMode = 0;
        gl_material.b_depthBlur = 1;
        return;
    }
    gl_material.b_depthBlur = 0;
    if ((mode & 2) != 0)
        return;

    RST_Decode((unsigned int)flags,
               gpst_DX_stDD->drawMaskAllowedStuffCurrent, &st);

    GLFN_glPolygonMode(GL_FRONT_AND_BACK, st.solidFill ? GL_FILL : GL_LINE);

    /* Through the setters, not straight at OpenGL. A material decides
       the depth state and so does the engine, through SetZEnable and
       SetZWriteEnable, and the engine reads it back with i_ZEnable --
       the shadow pass saves it, turns it off and puts it back. Setting
       the state here without recording it leaves that read answering
       with whatever the last SetZEnable said, so the restore puts back
       a value that was never in force. */
    GL_SetZEnable(st.depthTest);
    GLFN_glDepthFunc(st.depthCompare == RST_DEPTH_EQUAL ? GL_EQUAL : GL_LEQUAL);
    GL_SetZWriteEnable(st.depthWrite);

    /* Culling.
     *
     * The engine names the winding to *cull*, in the terms the other
     * renderer's device uses. OpenGL names the winding that is
     * *front*. The mapping between them is the line below, and it is
     * **measured, not derived** -- on an RTX 2060, by trying all three
     * answers in a real scene.
     *
     * The derivation is written out here because it gives the opposite
     * answer and is therefore wrong, and because it is persuasive
     * enough that someone will reach it again. It goes: the device
     * decides facing from a window whose y runs downward and OpenGL
     * from one whose y runs upward; clip y = +1 is the top of the
     * screen in both, so the two window spaces differ by a y flip,
     * which reverses a triangle's orientation; and the two APIs name
     * opposite things, which reverses it back. Two reversals cancel,
     * so the winding OpenGL is told is front would carry the same
     * name the engine gave for culling.
     *
     * That is what shipped, and the world came out almost dark with
     * objects missing -- the front of every surface culled, leaving
     * unlit interiors. The right answer is the other one.
     *
     * What the measurement also settles, and the derivation got wrong,
     * is that the two renderers' window-space windings **agree**:
     * there is no net y flip between them. Which step supplies that is
     * not established, so nothing else should be derived from it --
     * see the note in retail/notes/engine-gaps.md for what else was
     * reasoned from the flip and is now suspect. */
    /* The gl_cull tweak overrides all of this: 1 turns culling off
       altogether and 2 reverses the facing. It is what found the
       mapping above, and it is kept because the mapping is measured on
       one card rather than derived -- so a card that disagrees wants
       it to hand rather than a rebuild. */
    if (st.cull == RST_CULL_NONE || tweak_gl_cull == 1)
    {
        GLFN_glDisable(GL_CULL_FACE);
        gl_cullOn = 0;
    }
    else
    {
        GLFN_glEnable(GL_CULL_FACE);
        GLFN_glCullFace(GL_BACK);
        if (tweak_gl_cull == 2)
            gl_frontFace = st.cull == RST_CULL_CW ? GL_CW : GL_CCW;
        else
            gl_frontFace = st.cull == RST_CULL_CW ? GL_CCW : GL_CW;
        GLFN_glFrontFace(gl_frontFace);
        gl_cullOn = 1;
    }

    GL_l_SetColorMask(st.writeRGB, st.writeRGB, st.writeRGB,
                      st.writeAlpha);

    GL_l_Blend(&st);

    /* Core OpenGL dropped the alpha test, so the shader does it. The
       reference is a byte on the engine's side and a normalised float
       in the shader. */
    gl_material.alphaMode = st.alphaTest
                                ? (st.alphaCompare == RST_ALPHA_LESS ? 2 : 1)
                                : 0;
    gl_material.alphaRef = (float)st.alphaRef / 255.0f;

    /* Fog. The variant a material names stands until another names a
       different one, and the fourth thing the flags can say is
       exactly that -- keep the one in force -- so only a named
       variant moves it. */
    if (st.fogVariant != RST_FOG_KEEP)
        gl_fogSecondColor = st.fogUsesSecondColor;
    gl_material.fogCurve = FOG_b_Applies(&gl_fog, st.fogAllowed)
                               ? gl_fog.curve : FOG_OFF;

    slot = GL_l_p_Texture(textureID);
    gl_material.textured = slot != NULL;
    if (slot)
    {
        GLFN_glActiveTexture(GL_TEXTURE0);
        GLFN_glBindTexture(GL_TEXTURE_2D, slot->name);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                             st.addressU == RST_WRAP ? GL_REPEAT
                                                     : GL_CLAMP_TO_EDGE);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                             st.addressV == RST_WRAP ? GL_REPEAT
                                                     : GL_CLAMP_TO_EDGE);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                             (GLint)GL_l_e_MinFilter(&st, slot->hasLevels));
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                             st.filter == RST_LINEAR ? GL_LINEAR : GL_NEAREST);
        GLFN_glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS,
                             tweak_mipmaplodbias);
    }

    /* The alpha the vertex colours are combined with. The other
       renderer keeps it in its own parameters too; the vertex builder
       is handed it at draw time. */
    gl_colorMaskAlpha = (int)st.colorMaskAlpha;
}

/* ---------------------------------------------------------- sprites
 *
 * A sprite is a position, a size and an index into the colour arrays;
 * what has to be drawn is four corners facing the camera. None of
 * that working out is this renderer's -- geosprite.c walks the batch
 * and hands over one quad at a time -- and what is here is turning
 * each quad into the two triangles the world's own program draws.
 *
 * The same program, because a sprite's vertex is the same vertex: a
 * position, a packed colour and one texture coordinate. The material
 * was set by the GL_TextureInit that came before, exactly as it was
 * for the world's triangles.
 */

/* Six vertices a quad, in the order and with the corner coordinates
   the other renderer writes: two triangles sharing the first and
   third corners. */
static void GL_l_EmitSprite(void* user, const tGdi_SpriteQuad* quad)
{
    tGdi_TexturedVertex* v;
    unsigned int         color;
    int                  i;

    (void)user;

    if (gl_sprVertNum + 6 > gl_sprVertMax)
        return;
    v = &gl_sprVerts[gl_sprVertNum];
    gl_sprVertNum += 6;

    /* The engine's colour word turned round for the device, which is
       what the other renderer does at this same point. */
    color = GVX_ui_DeviceColor((unsigned int)quad->color);

    v[0].pos = quad->v1; v[0].u = 1.0f; v[0].v = 1.0f;
    v[1].pos = quad->v2; v[1].u = 0.0f; v[1].v = 1.0f;
    v[2].pos = quad->v3; v[2].u = 0.0f; v[2].v = 0.0f;
    v[3].pos = quad->v1; v[3].u = 1.0f; v[3].v = 1.0f;
    v[4].pos = quad->v3; v[4].u = 0.0f; v[4].v = 0.0f;
    v[5].pos = quad->v4; v[5].u = 1.0f; v[5].v = 0.0f;
    for (i = 0; i < 6; i++)
        v[i].color = color;
}

int GL_DrawSprites(tGeoSprites* sprites, Vec3* positions, int num)
{
    tBigBufferStruct* bb;
    tGsp_Source       src;
    int               count;
    int               needed;

    (void)num;

    if (!sprites || !positions || !gpst_DX_stDD)
        return 0;
    count = GSP_i_QuadCount(sprites);
    if (count <= 0)
        return 0;
    if (!GL_l_b_GeometryReady())
        return 0;

    needed = count * 6;
    if (needed > gl_sprVertMax)
    {
        tGdi_TexturedVertex* grown;

        grown = (tGdi_TexturedVertex*)realloc(gl_sprVerts,
                                              sizeof(*grown) * (size_t)needed);
        if (!grown)
            return 0;
        gl_sprVerts = grown;
        gl_sprVertMax = needed;
    }
    gl_sprVertNum = 0;

    bb = gpst_DX_stDD->bigBuffer;
    src.positions = positions;
    src.colors = bb ? bb->vertexColorArray : NULL;
    src.second = bb ? bb->vertexColorArraySecond : NULL;
    src.mask = bb ? bb->colorMask : 0;
    src.alpha = gl_colorMaskAlpha;
    src.solidColor = bb ? (unsigned int)bb->solidVertexColor : 0u;
    src.matrix = gpst_DX_stDD->matrixStack.actMatrixElem;
    src.waveSprite = GDI_gb_WaveSprite;

    GSP_Walk(sprites, &src, GL_l_EmitSprite, NULL);

    /* Never the blur pass, and never normals: both are the world's
       geometry only. */
    GL_l_DrawStaged(gl_sprVerts, gl_sprVertNum, 0, 0);
    return 0;
}

/* The engine offers a renderer the chance to upload a texture with
   its palette left on the card, and only asks when a display setting
   is on that is off by default. Answering no sends it down
   GL_PaletteLoad, which is where this renderer does the work. */
int GL_SetTexturePalette(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return 0;
}

/* Dropping one palette, which this renderer does not keep separately
   -- a texture's colours are in the texture. The other renderer's
   entry is empty too. */
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

/* ------------------------------------------------- render targets
 *
 * Drawing into a texture rather than into the window. The engine does
 * this for the shadow each caster projects: eight small targets, one
 * per shadow in force, drawn into once and then sampled by every
 * object the shadow falls on.
 *
 * In Direct3D a render target is a surface handed to the device, and
 * the depth buffer the window already has stays attached -- it only
 * has to be at least as big, which a back-buffer-sized one is. OpenGL
 * has no such arrangement: a framebuffer object owns its attachments,
 * so these need a depth buffer of their own. One is enough, shared
 * between them, because only one target is ever bound at a time.
 *
 * Nesting is refused rather than stacked, as the other renderer
 * refuses it: a second request while one is in force does nothing,
 * and so does a release with none in force.
 */

/* ================================================ the scene target
 *
 * The world is drawn into a target of its own and copied to the screen
 * at the end of the frame, rather than drawn straight to the back
 * buffer.
 *
 * That is what the cutscene letterbox needs, and the letterbox is why
 * this exists now: the bands are not drawn anywhere. The other
 * renderer's after-effects copy their finished frame back to the
 * screen and, when the letterbox is on, copy only the strip down the
 * middle -- so the bands are whatever the screen already held, which
 * is black. Drawing straight to the back buffer leaves nothing
 * uncopied and so can have no bands at all.
 *
 * It is also where the after-effects will go. None of them are here
 * yet; what is here is the target, and the copy that is the last step
 * of that pass in the other renderer.
 *
 * The target is the default binding rather than something switched on
 * at the start of a frame. A frame that did not switch it on would
 * draw to the back buffer and then have the copy land on top of it,
 * and there is no one place every frame passes through to be sure of
 * it -- so instead everything that unbinds puts this back, and the
 * screen itself is bound only for the copy.
 */
static GLuint gl_sceneFbo;
static GLuint gl_sceneColour;
static GLuint gl_sceneDepth;
static int    gl_sceneW;
static int    gl_sceneH;
static int    gl_sceneTried;
static int    gl_sceneReady;

static void GL_l_ForgetScene(void)
{
    if (gl_state.context)
    {
        if (gl_sceneFbo)
            GLFN_glDeleteFramebuffers(1, &gl_sceneFbo);
        if (gl_sceneColour)
            GLFN_glDeleteTextures(1, &gl_sceneColour);
        if (gl_sceneDepth)
            GLFN_glDeleteRenderbuffers(1, &gl_sceneDepth);
    }
    gl_sceneFbo = 0;
    gl_sceneColour = 0;
    gl_sceneDepth = 0;
    gl_sceneW = 0;
    gl_sceneH = 0;
    gl_sceneTried = 0;
    gl_sceneReady = 0;
}

static int GL_l_b_SceneReady(void)
{
    /* Rebuilt when the window changes size, because a target of the
       old size would be copied to the new screen stretched. */
    if (gl_sceneTried
        && (gl_sceneW != gl_state.width || gl_sceneH != gl_state.height))
        GL_l_ForgetScene();

    if (gl_sceneTried)
        return gl_sceneReady;
    gl_sceneTried = 1;
    if (!gl_state.context || gl_state.width <= 0 || gl_state.height <= 0)
        return 0;

    gl_sceneW = gl_state.width;
    gl_sceneH = gl_state.height;

    GLFN_glGenFramebuffers(1, &gl_sceneFbo);
    GLFN_glGenTextures(1, &gl_sceneColour);
    GLFN_glGenRenderbuffers(1, &gl_sceneDepth);
    if (!gl_sceneFbo || !gl_sceneColour || !gl_sceneDepth)
    {
        DBG_OutputDebugString("GL: no scene target; drawing to the screen\n");
        GL_l_ForgetScene();
        gl_sceneTried = 1;
        return 0;
    }

    GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gl_sceneW, gl_sceneH, 0,
                      GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* Clamped, which a fresh texture is not: the after-effects sample
       this one with offsets, and with no letterbox the strip they
       cover is the whole picture -- so a tap on the top row steps past
       the end of it. Left repeating, that tap lands on the bottom of
       the picture and the sky picks up the ground. */
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, gl_sceneDepth);
    GLFN_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                               gl_sceneW, gl_sceneH);
    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_sceneFbo);
    GLFN_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, gl_sceneColour, 0);
    GLFN_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_RENDERBUFFER, gl_sceneDepth);
    if (GLFN_glCheckFramebufferStatus(GL_FRAMEBUFFER)
        != GL_FRAMEBUFFER_COMPLETE)
    {
        DBG_OutputDebugString("GL: the scene target is not complete;"
                              " drawing to the screen\n");
        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GL_l_ForgetScene();
        gl_sceneTried = 1;
        return 0;
    }

    gl_sceneReady = 1;
    return 1;
}

/* The framebuffer the world draws into: the scene target when there is
   one, and the screen when there is not -- a driver that cannot give
   us a target should still show the game, without the letterbox. */
static GLuint GL_l_i_SceneFbo(void)
{
    return GL_l_b_SceneReady() ? gl_sceneFbo : 0;
}

/* Puts the scene target back as the current framebuffer, which is what
   every path that bound something else has to end with. */
static void GL_l_BindScene(void)
{
    if (!gl_state.context)
        return;
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_SceneFbo());
}

/* The finished frame on the screen, letterboxed if a cutscene asked
 * for it.
 *
 * A stretch blit of one strip, which is what the other renderer's copy
 * is too. The screen is cleared first and only the strip is copied, so
 * the bands are the cleared black rather than anything drawn.
 *
 * Which framebuffer the frame is in is an argument rather than always
 * the scene target, because the after-effects leave it in one of their
 * own: an effect cannot read and write one target in the same pass, so
 * each one writes somewhere else and the last one to run decides where
 * the frame ended up. */
static void GL_l_CopyToScreen(GLuint fromFbo)
{
    tCbd_Bands bands;
    int        top;
    int        bottom;

    if (!gl_sceneReady || !gl_state.context)
        return;

    CBD_Decide(&bands);

    /* Measured from the bottom, because that is where OpenGL's window
       origin is and the fractions are from the top. */
    top = (int)((1.0f - bands.bottom) * (float)gl_sceneH);
    bottom = (int)((1.0f - bands.top) * (float)gl_sceneH);
    if (top < 0)
        top = 0;
    if (bottom > gl_sceneH)
        bottom = gl_sceneH;
    if (bottom <= top)
        return;

    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (bands.b_on)
    {
        /* Only when there are bands: clearing a screen that is about
           to be covered entirely is work for nothing. */
        GL_l_SetColorMask(1, 1, 1, 1);
        GL_l_ClearColour(0.0f, 0.0f, 0.0f, 1.0f, 0);
    }

    GLFN_glBindFramebuffer(GL_READ_FRAMEBUFFER, fromFbo);
    GLFN_glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    GLFN_glBlitFramebuffer(0, top, gl_sceneW, bottom,
                           0, top, gl_sceneW, bottom,
                           GL_COLOR_BUFFER_BIT, GL_NEAREST);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void GL_l_ForgetTargets(void)
{
    int i;

    if (gl_state.context)
    {
        for (i = 0; i < GL_SHADOW_TARGETS; i++)
        {
            if (gl_shadowTargets[i].fbo)
                GLFN_glDeleteFramebuffers(1, &gl_shadowTargets[i].fbo);
            if (gl_shadowTargets[i].color)
                GLFN_glDeleteTextures(1, &gl_shadowTargets[i].color);
        }
        if (gl_shadowDepth)
            GLFN_glDeleteRenderbuffers(1, &gl_shadowDepth);
    }
    memset(gl_shadowTargets, 0, sizeof(gl_shadowTargets));
    gl_shadowDepth = 0;
    gl_shadowTried = 0;
    gl_shadowReady = 0;
    gl_targetInForce = 0;
}

/* Builds one target: a colour texture to sample later, and the shared
   depth buffer. Returns whether the framebuffer came out usable --
   an incomplete one draws nothing and reports nothing, so it is
   checked here rather than discovered as an empty shadow. */
static int GL_l_b_MakeTarget(tGlTarget* t, int width, int height)
{
    t->width = width;
    t->height = height;

    GLFN_glGenTextures(1, &t->color);
    if (!t->color)
        return 0;
    GLFN_glBindTexture(GL_TEXTURE_2D, t->color);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                      GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    /* No chain, and a sampler that wants none: a material can ask for
       mip filtering and these have one level, which would make the
       texture incomplete. */
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    GLFN_glGenFramebuffers(1, &t->fbo);
    if (!t->fbo)
        return 0;
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);
    GLFN_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, t->color, 0);
    GLFN_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_RENDERBUFFER, gl_shadowDepth);
    if (GLFN_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return 0;
    }

    /* White, as the other renderer fills them: a shadow target is
       sampled as how much light reaches a point, so untouched means
       unshadowed. Left at the zero a fresh texture has, every object
       would be in shadow until something drew into it. */
    GL_l_ClearColour(1.0f, 1.0f, 1.0f, 1.0f, 0);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return 1;
}

static int GL_l_b_TargetsReady(void)
{
    int i;

    if (gl_shadowTried)
        return gl_shadowReady;
    gl_shadowTried = 1;
    if (!gl_state.context)
        return 0;

    GLFN_glGenRenderbuffers(1, &gl_shadowDepth);
    if (!gl_shadowDepth)
    {
        DBG_OutputDebugString("GL: no depth buffer for the shadow targets\n");
        return 0;
    }
    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, gl_shadowDepth);
    GLFN_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                               GL_SHADOW_SIZE, GL_SHADOW_SIZE);
    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, 0);

    for (i = 0; i < GL_SHADOW_TARGETS; i++)
        if (!GL_l_b_MakeTarget(&gl_shadowTargets[i], GL_SHADOW_SIZE,
                               GL_SHADOW_SIZE))
        {
            DBG_OutputDebugString("GL: shadow target %d would not build\n", i);
            GL_l_ForgetTargets();
            gl_shadowTried = 1;   /* asked and answered; do not ask again */
            return 0;
        }

    gl_shadowReady = 1;
    return 1;
}

void GL_SetRenderTargetAndBackupPreviousSDW(int textureID, int clear)
{
    tGlTarget* t;

    if (!gl_state.context)
        return;

    if (textureID == -1)
    {
        if (!gl_targetInForce)
            return;
        /* Back to the scene target, which is what the world draws
           into -- not to the screen, which is only bound for the copy
           at the end of the frame. */
        GL_l_BindScene();
        GLFN_glViewport(gl_savedViewport[0], gl_savedViewport[1],
                        gl_savedViewport[2], gl_savedViewport[3]);
        /* Only the projection, because the caller puts the view back
           itself -- which is what the other renderer relies on too. */
        gl_projMtx = gl_savedProjMtx;
        GL_l_CombineMatrices();
        gl_targetInForce = 0;
        return;
    }

    if (gl_targetInForce)
        return;
    if (textureID < 0 || textureID >= GL_SHADOW_TARGETS)
        return;
    if (!GL_l_b_TargetsReady())
        return;

    t = &gl_shadowTargets[textureID];
    GLFN_glGetIntegerv(GL_VIEWPORT, gl_savedViewport);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);
    gl_targetInForce = 1;

    if (clear)
    {
        /* Colour only, and to black: the shadow is drawn in white and
           what it does not cover stays unshadowed through the border.
           The other renderer clears the same one buffer. */
        GL_l_ClearColour(0.0f, 0.0f, 0.0f, 0.0f, 0);
    }

    /* The inset border, measured from the bottom because that is where
       OpenGL's viewport origin is. It comes out symmetric on a square
       target with an even border, but writing it as the subtraction is
       what keeps it right if either stops being true. */
    GLFN_glViewport(GL_SHADOW_BORDER,
                    t->height - (GL_SHADOW_BORDER
                                 + (t->height - 2 * GL_SHADOW_BORDER)),
                    t->width - 2 * GL_SHADOW_BORDER,
                    t->height - 2 * GL_SHADOW_BORDER);
}

void GL_SetViewMatrixSDW(Mtx4x4* mtxptr, float* unused)
{
    (void)unused;

    if (!mtxptr)
        return;

    /* Saved here rather than where the target is bound, because that
       is where the other renderer saves it and the engine calls this
       one first. */
    gl_savedProjMtx = gl_projMtx;

    /* The shadow is drawn with the projection out of the way: the view
       matrix the engine builds for it already maps the caster into the
       target's own square. The other renderer also resets its world
       transform here, which this renderer does not have -- vertices
       arrive in world space and only the view and the projection are
       ever multiplied. */
    MATH_SetIdentityMatrix(&gl_projMtx);
    GL_SetViewMatrix(mtxptr);
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

/* ================================================= the sun's occlusion test
 *
 * The sun's flare is lit octant by octant, and each octant is dark if
 * the sample that belongs to it was blocked. So what the engine wants
 * from a renderer is nine bits: which of nine points around the sun
 * have something in front of them.
 *
 * The answer is read out of the frame's alpha, which the depth-blur
 * pass wrote -- that is the whole reason the two are one piece of work.
 * Alpha there is 255 at the camera and falls to 0 at the far plane, so
 * a pixel's alpha says how near whatever was drawn there is, and a
 * pixel whose alpha is greater than the sun's own is nearer than the
 * sun and blocks it.
 *
 * **A set bit means blocked, not visible.** The other renderer calls
 * the word "passed" and comments that an unsampled point is "never
 * occluded", and both of those read the wrong way round: the engine
 * sets all nine when the sun is hidden outright
 * (GFX_l_SunHidden), and in the render a set bit contributes zero
 * luminance (`self = (flags & (1 << k)) ? 0.0 : 2.0`). So a point
 * outside the viewport, whose bit is set without being sampled, counts
 * as blocked -- which is what it should be, since an off-screen point
 * cannot light anything.
 *
 * Reading pixels back is a stall if it is done the obvious way, and
 * this happens every frame. So the reads go into a buffer, a fence
 * records where the card had got to, and the answer is collected a
 * frame or more later: three buffers in a ring, the newest finished
 * one is the one read, and a frame that finds the ring full goes
 * unsampled. The other renderer does the same with three
 * system-memory surfaces and a lock that refuses to wait; this is that
 * design in OpenGL's terms, and the engine already tolerates "no
 * answer yet" because that renderer can say it too.
 */

/* The engine's allocator, declared rather than included: memory.h
   collides with the mocks in more than one harness, and these two are
   all the sun needs. The signatures are memory.h's own. */
void* MEM_p_Alloc(unsigned int size);
void  MEM_Free(void* ptr);

#define GL_SUN_POINTS 9
#define GL_SUN_SLOTS  3
#define GL_SUN_SUNS   16

typedef struct sGlSunSlot
{
    GLuint pbo;
    GLsync fence;
    int    b_inFlight;
    int    mask;        /* which of the nine were sampled */
    int    sampled;     /* how many, which is how many pixels were read */
    double threshold;   /* the blur amount at the sun's own depth */
} tGlSunSlot;

typedef struct sGlSun
{
    tGlSunSlot slots[GL_SUN_SLOTS];
    int        next;     /* where the next sample goes */
    int        oldest;   /* the oldest unread */
    int        inFlight;
    int        b_ready;
} tGlSun;

/* Which suns exist and which of them want the test, as parallel tables
   walked by index -- the same shape the other renderer keeps, because
   the blur's mode depends on whether *any* sun wants it and not on
   this one. */
static tGlSun* gl_suns[GL_SUN_SUNS];
static int     gl_sunWants[GL_SUN_SUNS];

static void GL_l_SunRegister(tGlSun* sun)
{
    int i;

    for (i = 0; i < GL_SUN_SUNS; i++)
    {
        if (!gl_suns[i])
        {
            gl_suns[i] = sun;
            gl_sunWants[i] = 0;
            return;
        }
    }
}

static void GL_l_SunForget(tGlSun* sun)
{
    int i;

    for (i = 0; i < GL_SUN_SUNS; i++)
    {
        if (gl_suns[i] == sun)
        {
            gl_suns[i] = NULL;
            gl_sunWants[i] = 0;
            return;
        }
    }
}

void* GL_p_SunSamplerCreate(void)
{
    tGlSun* sun;
    int     i;

    if (!gl_state.context)
        return NULL;

    sun = (tGlSun*)MEM_p_Alloc(sizeof(*sun));
    if (!sun)
        return NULL;
    memset(sun, 0, sizeof(*sun));

    for (i = 0; i < GL_SUN_SLOTS; i++)
    {
        GLFN_glGenBuffers(1, &sun->slots[i].pbo);
        if (!sun->slots[i].pbo)
            break;
        /* Nine pixels of four bytes, which is what one sample is.
           STREAM_READ says what it is for: written by the card once
           and read by us once. */
        GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, sun->slots[i].pbo);
        GLFN_glBufferData(GL_PIXEL_PACK_BUFFER,
                          (GLsizei)(GL_SUN_POINTS * 4), NULL, GL_STREAM_READ);
    }
    GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    if (i != GL_SUN_SLOTS)
    {
        DBG_OutputDebugString("GL: no buffers for the sun's readback\n");
        for (i = 0; i < GL_SUN_SLOTS; i++)
            if (sun->slots[i].pbo)
                GLFN_glDeleteBuffers(1, &sun->slots[i].pbo);
        MEM_Free(sun);
        return NULL;
    }

    sun->b_ready = 1;
    GL_l_SunRegister(sun);
    return sun;
}

/* A slot's reads are abandoned: the fence goes and the slot is free
   again. The buffer stays, because it is reused. */
static void GL_l_SunSlotRetire(tGlSunSlot* slot)
{
    if (slot->fence)
    {
        GLFN_glDeleteSync(slot->fence);
        slot->fence = NULL;
    }
    slot->b_inFlight = 0;
    slot->mask = 0;
    slot->sampled = 0;
}

void GL_SunSamplerDestroy(void* sampler)
{
    tGlSun* sun = (tGlSun*)sampler;
    int     i;

    if (!sun)
        return;

    GL_SunEffectOnOff(sun, 0);
    GL_l_SunForget(sun);
    for (i = 0; i < GL_SUN_SLOTS; i++)
    {
        GL_l_SunSlotRetire(&sun->slots[i]);
        if (sun->slots[i].pbo && gl_state.context)
            GLFN_glDeleteBuffers(1, &sun->slots[i].pbo);
        sun->slots[i].pbo = 0;
    }
    MEM_Free(sun);
}

int GL_b_SunSamplerReady(void* sampler)
{
    tGlSun* sun = (tGlSun*)sampler;

    return sun && sun->b_ready && gl_state.context != NULL;
}

int GL_b_SunSamplingNow(void* sampler)
{
    tDpb_State st;

    (void)sampler;
    /* The depth blur writes the alpha this test reads, so there is
       only one of its modes in which there is anything to read. */
    if (!gl_state.context)
        return 0;
    GL_l_ReadBlurState(&st);
    return st.kind == DPB_SUN;
}

void GL_SunSampleQueue(void* sampler, const int* x, const int* y, int num,
                       int pointMask, float cameraZ)
{
    tGlSun*     sun = (tGlSun*)sampler;
    tGlSunSlot* slot;
    tDpb_State  st;
    int         i;

    if (!GL_b_SunSamplerReady(sampler) || !x || !y)
        return;
    if (num < 0 || num > GL_SUN_POINTS)
        return;
    /* The ring has caught up with its own oldest unread sample, so
       this frame goes unsampled rather than throwing an answer away. */
    if (sun->inFlight >= GL_SUN_SLOTS)
        return;

    slot = &sun->slots[sun->next];
    slot->mask = pointMask;
    slot->sampled = num;
    GL_l_ReadBlurState(&st);
    /* What counts as blocked: the blur amount at the sun's own depth,
       taken through the same byte the frame's alpha is. */
    slot->threshold = (double)(unsigned char)DPB_i_BlurAtDepth(&st, cameraZ);

    GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, slot->pbo);
    GLFN_glPixelStorei(GL_PACK_ALIGNMENT, 1);
    for (i = 0; i < num; i++)
    {
        /* One pixel each, packed into the buffer in the order they
           were given -- so the nth pixel read is the nth bit set in
           the mask, which is how the resolve walks them. The offset is
           passed where a pointer would be, which is how a read into a
           buffer says where in it to write. */
        GLFN_glReadPixels(x[i], y[i], 1, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                          (void*)(size_t)(i * 4));
    }
    GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    if (slot->fence)
        GLFN_glDeleteSync(slot->fence);
    slot->fence = GLFN_glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    slot->b_inFlight = 1;
    sun->next = (sun->next + 1) % GL_SUN_SLOTS;
    sun->inFlight++;
}

/* Has the card finished with this slot? A wait of no time at all, so
   this asks rather than blocks -- which is the whole point of there
   being three of them.
 *
 * With a timeout of zero the only answer that means yes is
 * ALREADY_SIGNALED: CONDITION_SATISFIED is what comes back when a wait
 * blocked and the fence was signalled while it waited, and a wait of
 * no time cannot block. An earlier version accepted both, which read
 * as thorough and was a branch nothing could ever reach. */
static int GL_l_b_SunSlotDone(const tGlSunSlot* slot)
{
    if (!slot->b_inFlight || !slot->fence)
        return 0;
    return GLFN_glClientWaitSync(slot->fence, 0, 0) == GL_ALREADY_SIGNALED;
}

/* The nine bits, from the nine bytes of alpha.
 *
 * A point the engine did not sample -- one outside the viewport -- is
 * blocked without being looked at, because an off-screen point cannot
 * light an octant. The ones that were sampled are packed, so the nth
 * pixel belongs to the nth set bit and not to bit n. */
static int GL_l_i_SunResolve(const tGlSunSlot* slot,
                             const unsigned char* pixels)
{
    int blocked = 0;
    int taken = 0;
    int i;

    for (i = 0; i < GL_SUN_POINTS; i++)
    {
        if ((slot->mask & (1 << i)) == 0)
        {
            blocked |= 1 << i;
            continue;
        }
        if (taken < slot->sampled
            && (double)pixels[taken * 4 + 3] > slot->threshold)
            blocked |= 1 << i;
        taken++;
    }
    return blocked;
}

int GL_b_SunSampleRead(void* sampler, int* outBlocked)
{
    tGlSun*              sun = (tGlSun*)sampler;
    const unsigned char* pixels;
    int                  found = -1;
    int                  i;
    int                  at;

    if (!GL_b_SunSamplerReady(sampler) || sun->inFlight <= 0)
        return 0;

    /* The newest the card has finished with, walking back from the one
       queued last towards the oldest unread: a newer answer is a
       better answer, and the ones behind it are dropped rather than
       being read out of order. */
    for (i = 0; i < sun->inFlight; i++)
    {
        at = (sun->next - 1 - i + 2 * GL_SUN_SLOTS) % GL_SUN_SLOTS;
        if (GL_l_b_SunSlotDone(&sun->slots[at]))
        {
            found = at;
            break;
        }
    }
    if (found < 0)
        return 0;

    GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, sun->slots[found].pbo);
    pixels = (const unsigned char*)GLFN_glMapBufferRange(
        GL_PIXEL_PACK_BUFFER, 0, (GLsizeiptr)(GL_SUN_POINTS * 4),
        GL_MAP_READ_BIT);
    if (!pixels)
    {
        GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        return 0;
    }
    if (outBlocked)
        *outBlocked = GL_l_i_SunResolve(&sun->slots[found], pixels);
    GLFN_glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    GLFN_glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    /* Everything up to and including the one just read is finished
       with, whether or not the card had got to it: an older answer is
       of no use once a newer one has been given. */
    while (sun->inFlight > 0)
    {
        int oldest = sun->oldest;

        GL_l_SunSlotRetire(&sun->slots[oldest]);
        sun->oldest = (oldest + 1) % GL_SUN_SLOTS;
        sun->inFlight--;
        if (oldest == found)
            break;
    }
    return 1;
}

/* Whether a sun wants its test is what puts the depth blur into the
   mode whose ramp that test reads, so the aggregate over every sun is
   what the blur asks about -- not this one's own answer. */
void GL_SunEffectOnOff(void* sampler, int on)
{
    tGlSun* sun = (tGlSun*)sampler;
    int     anyOn = 0;
    int     i;

    for (i = 0; i < GL_SUN_SUNS; i++)
    {
        if (gl_suns[i] == sun && sun)
            gl_sunWants[i] = on ? 1 : 0;
        if (gl_suns[i])
            anyOn |= gl_sunWants[i];
    }
    gl_state.sunEffectOn = anyOn;
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

/* ------------------------------------------------- the after-effects
 *
 * Everything from here to GL_l_i_RunAfterEffects is the chain that
 * runs over the finished frame, between the world's last draw and the
 * copy to the screen.
 *
 * The shape is a ping-pong. An effect reads one target and writes
 * another, because no pass may read the target it is writing -- so
 * there are two spares beside the scene target and each pass names
 * which it reads and which it writes. What the chain hands back is
 * whichever target the last pass wrote, and the copy to the screen
 * takes that rather than always the scene.
 *
 * One effect lives here so far: the depth blur, which is the one whose
 * input the world already writes. The frame's alpha holds a blur
 * amount per pixel, three widening passes blur the colour, and a last
 * pass mixes sharp and blurred by that alpha.
 */
/* Three spares beside the frame.
 *
 * Two is not enough, and the reason is worth stating. The other
 * renderer's chain has three targets and uses the frame itself as one
 * of them -- it writes the finished effect back over the frame and
 * carries on. This renderer cannot: its depth-blur mix *reads* the
 * frame while it writes, and the frame's alpha is that effect's whole
 * input, so the frame has to stay as the world drew it.
 *
 * That costs a target. With two spares, an effect that needs a blurred
 * copy of its input and a place to put the result has nowhere left to
 * put it -- the remanence wants exactly that, and with two spares its
 * blurs would scribble over the target the motion blur after it keeps
 * its history in. Three leaves one free whatever the chain is holding.
 */
#define GL_AE_SPARES 3

/* The spares plus the fade's frozen copy, which is a target but never
   a spare -- see GL_AE_SLOT_FREEZE. */
#define GL_AE_TARGETS (GL_AE_SPARES + 1)

/* The three spares, by the roles the renderer this was read from gives
 * its own three textures.
 *
 * That renderer's chain holds two of them at a time -- a *current* and
 * a *spare* -- and exchanges them after every effect, while a third is
 * scratch for the blur passes. Its current starts as the frame itself,
 * which is why it only needs three textures in all.
 *
 * Here the frame has to stay as the world drew it for as long as the
 * depth blur's mix needs its alpha, so the mix cannot write it in
 * place the way that renderer does. It writes GL_AE_SLOT_MIXTMP and
 * the result is copied back into the frame, which leaves every target
 * from that point on holding what it holds there -- and that matters,
 * because the motion blur reads whatever is in the target it writes.
 *
 * So the first two slots line up with that renderer's two non-frame
 * textures and the third has no counterpart.
 */
#define GL_AE_SLOT_SCRATCH  (DPB_SLOT_A)
#define GL_AE_SLOT_SPARE    (DPB_SLOT_A + 1)
#define GL_AE_SLOT_MIXTMP   (DPB_SLOT_A + 2)

/* And one more that is not a spare at all: where the fade keeps the
 * picture it froze.
 *
 * It is outside the rotation above for the reason the fade exists --
 * what is in it has to survive from the frame the fade started to the
 * frame it ends, which is up to 255 frames of every other effect
 * writing spares. Handed out as scratch it would be painted over on
 * the first one, and the fade would dissolve out of whatever happened
 * to be there. The other renderer keeps the same thing in a texture of
 * its own, for the same reason and with no comment.
 *
 * The spare walk counts to GL_AE_SPARES rather than GL_AE_TARGETS, and
 * that bound is **defensive and not load-bearing today**: the walk
 * takes the first slot not among the two it is asked to avoid, and
 * with three spares and two avoided it stops at the third whatever the
 * bound says. So no test can tell the two bounds apart, and the sweep
 * says so rather than this comment claiming a clobber it prevents.
 * What it does do is make the fourth slot not-a-spare by construction,
 * so a third thing to avoid -- or a fourth spare -- cannot reach it by
 * accident. */
#define GL_AE_SLOT_FREEZE   (DPB_SLOT_A + GL_AE_SPARES)

static GLuint     gl_aeFbo[GL_AE_TARGETS];
static GLuint     gl_aeColour[GL_AE_TARGETS];
static int        gl_aeW;
static int        gl_aeH;
static int        gl_aeTried;
static int        gl_aeReady;
static GLuint     gl_aeVao;
static tGlProgram gl_aeBlurProgram;
static tGlProgram gl_aeMixProgram;
static GLint      gl_aeBlurSrc = -1;
static GLint      gl_aeBlurOff01 = -1;
static GLint      gl_aeBlurOff23 = -1;
static GLint      gl_aeBlurWeights = -1;
static GLint      gl_aeMixSharp = -1;
static GLint      gl_aeMixBlurred = -1;
static tGlProgram gl_aeRemProgram;
static GLint      gl_aeRemSharp = -1;
static GLint      gl_aeRemBlurred = -1;
static GLint      gl_aeRemWeight = -1;
static tGlProgram gl_aeBlendProgram;
static GLint      gl_aeBlendSrc = -1;
static GLint      gl_aeBlendAlpha = -1;
static tGlProgram gl_aeFadeProgram;
static GLint      gl_aeFadeLive = -1;
static GLint      gl_aeFadeFrozen = -1;
static GLint      gl_aeFadeShare = -1;
static tGlProgram gl_aeHueProgram;
static GLint      gl_aeHueSrc = -1;
static GLint      gl_aeHueSubtract = -1;
static tGlProgram gl_aeZoom2Program;
static GLint      gl_aeZoom2Src = -1;
static GLint      gl_aeZoom2TapsA = -1;
static GLint      gl_aeZoom2TapsB = -1;
static GLint      gl_aeZoom2Count = -1;
static GLint      gl_aeZoom2Weight = -1;
static tGlProgram gl_aeMotionProgram;
static GLint      gl_aeMotionSrc = -1;
static GLint      gl_aeMotionTapU = -1;
static GLint      gl_aeMotionTapV = -1;
static GLint      gl_aeMotionWeight = -1;

/* Last frame's watched direction, which is the motion smooth's whole
 * input.
 *
 * Deliberately **not** cleared when the after-effect targets are
 * forgotten. Rebuilding the targets happens when the window changes
 * size, and the camera has not jumped just because the window has --
 * zeroing this there would put the watched direction at the origin and
 * smear the next frame by the whole distance back to it. The other
 * renderer keeps its own copy of the same thing and does not clear it
 * either.
 *
 * One copy per renderer rather than one shared, which is the opposite
 * of the fade's timer and for a good reason: nothing outside this
 * effect ever writes it, so a renderer that is not running simply has
 * a stale point, and only the renderer in force reads its own. */
static Vec3       gl_aeMotionPrev;
static tGlProgram gl_aeZoomProgram;
static GLint      gl_aeZoomSrc = -1;
static GLint      gl_aeZoomBandLo = -1;
static GLint      gl_aeZoomBandHi = -1;
static GLint      gl_aeZoomInset = -1;
static GLint      gl_aeZoomTaps = -1;
static GLint      gl_aeZoomWeight = -1;
static tGlProgram gl_aeColorProgram;
static GLint      gl_aeColorSrc = -1;
static GLint      gl_aeColorGain = -1;
static GLint      gl_aeColorBias = -1;
static GLint      gl_aeColorGrayscale = -1;
static GLint      gl_aeColorWeights = -1;

static void GL_l_ForgetAe(void)
{
    int i;

    /* On the context alone, and not on the ready flag as well: this is
       also the failure path out of the middle of building, where the
       flag is still clear and some of the names below already exist.
       Every one of them is checked, and freeing a program that was
       never loaded does nothing. */
    if (gl_state.context)
    {
        GL_ProgramFree(&gl_aeBlurProgram);
        GL_ProgramFree(&gl_aeMixProgram);
        GL_ProgramFree(&gl_aeColorProgram);
        GL_ProgramFree(&gl_aeRemProgram);
        GL_ProgramFree(&gl_aeBlendProgram);
        GL_ProgramFree(&gl_aeFadeProgram);
        GL_ProgramFree(&gl_aeHueProgram);
        GL_ProgramFree(&gl_aeMotionProgram);
        GL_ProgramFree(&gl_aeZoom2Program);
        GL_ProgramFree(&gl_aeZoomProgram);
        for (i = 0; i < GL_AE_TARGETS; i++)
        {
            if (gl_aeFbo[i])
                GLFN_glDeleteFramebuffers(1, &gl_aeFbo[i]);
            if (gl_aeColour[i])
                GLFN_glDeleteTextures(1, &gl_aeColour[i]);
        }
        if (gl_aeVao)
            GLFN_glDeleteVertexArrays(1, &gl_aeVao);
    }
    memset(gl_aeFbo, 0, sizeof(gl_aeFbo));
    memset(gl_aeColour, 0, sizeof(gl_aeColour));
    gl_aeVao = 0;
    gl_aeW = 0;
    gl_aeH = 0;
    gl_aeBlurSrc = -1;
    gl_aeBlurOff01 = -1;
    gl_aeBlurOff23 = -1;
    gl_aeBlurWeights = -1;
    gl_aeMixSharp = -1;
    gl_aeMixBlurred = -1;
    gl_aeColorSrc = -1;
    gl_aeColorGain = -1;
    gl_aeColorBias = -1;
    gl_aeColorGrayscale = -1;
    gl_aeColorWeights = -1;
    gl_aeRemSharp = -1;
    gl_aeRemBlurred = -1;
    gl_aeRemWeight = -1;
    gl_aeBlendSrc = -1;
    gl_aeBlendAlpha = -1;
    gl_aeFadeLive = -1;
    gl_aeFadeFrozen = -1;
    gl_aeFadeShare = -1;
    gl_aeHueSrc = -1;
    gl_aeHueSubtract = -1;
    gl_aeMotionSrc = -1;
    gl_aeMotionTapU = -1;
    gl_aeMotionTapV = -1;
    gl_aeMotionWeight = -1;
    gl_aeZoom2Src = -1;
    gl_aeZoom2TapsA = -1;
    gl_aeZoom2TapsB = -1;
    gl_aeZoom2Count = -1;
    gl_aeZoom2Weight = -1;
    gl_aeZoomSrc = -1;
    gl_aeZoomBandLo = -1;
    gl_aeZoomBandHi = -1;
    gl_aeZoomInset = -1;
    gl_aeZoomTaps = -1;
    gl_aeZoomWeight = -1;
    gl_aeTried = 0;
    gl_aeReady = 0;
}

/* The spare targets, the two programs and the quad's vertex array.
 *
 * Colour only: an effect never tests or writes depth, and the frame it
 * reads has its depth behind it in the scene target where the world
 * left it. */
static int GL_l_b_AeReady(void)
{
    int i;

    /* Rebuilt with the scene target, for the same reason: a spare of
       the old size would hold the frame stretched. */
    if (gl_aeTried && (gl_aeW != gl_sceneW || gl_aeH != gl_sceneH))
        GL_l_ForgetAe();

    if (gl_aeTried)
        return gl_aeReady;
    gl_aeTried = 1;
    if (!gl_state.context || gl_sceneW <= 0 || gl_sceneH <= 0)
        return 0;

    /* Recorded before anything is built, not after: the size is half
       of what says this has already been asked and answered, and a
       failure that left it at zero would look like a size change on
       the next frame and start the whole build again -- recompiling
       the shaders every frame for as long as the game ran. */
    gl_aeW = gl_sceneW;
    gl_aeH = gl_sceneH;

    /* The quad goes through the 2D pass's buffer, so that has to be
       built -- the overlay normally gets there first, but a frame in
       which nothing drew an overlay would not have. */
    if (!GL_l_b_PassReady())
        return 0;

    if (!GL_b_ProgramLoad(&gl_aeBlurProgram, "aepass.vert", "aeblur.frag"))
        return 0;
    if (!GL_b_ProgramLoad(&gl_aeMixProgram, "aepass.vert", "aedepthmix.frag"))
    {
        GL_ProgramFree(&gl_aeBlurProgram);
        return 0;
    }
    if (!GL_b_ProgramLoad(&gl_aeColorProgram, "aepass.vert", "aecolor.frag"))
    {
        GL_ProgramFree(&gl_aeBlurProgram);
        GL_ProgramFree(&gl_aeMixProgram);
        return 0;
    }
    if (!GL_b_ProgramLoad(&gl_aeRemProgram, "aepass.vert", "aeremanence.frag")
        || !GL_b_ProgramLoad(&gl_aeBlendProgram, "aepass.vert", "aeblend.frag")
        || !GL_b_ProgramLoad(&gl_aeFadeProgram, "aepass.vert", "aefade.frag")
        || !GL_b_ProgramLoad(&gl_aeHueProgram, "aepass.vert", "aehue.frag")
        || !GL_b_ProgramLoad(&gl_aeMotionProgram, "aepass.vert",
                             "aemotion.frag")
        || !GL_b_ProgramLoad(&gl_aeZoom2Program, "aepass.vert",
                             "aezoom2.frag")
        || !GL_b_ProgramLoad(&gl_aeZoomProgram, "aepass.vert", "aezoom.frag"))
    {
        /* Whichever of the seven got as far as loading is freed with
           the rest; freeing one that did not does nothing. */
        GL_ProgramFree(&gl_aeBlurProgram);
        GL_ProgramFree(&gl_aeMixProgram);
        GL_ProgramFree(&gl_aeColorProgram);
        GL_ProgramFree(&gl_aeRemProgram);
        GL_ProgramFree(&gl_aeBlendProgram);
        GL_ProgramFree(&gl_aeFadeProgram);
        GL_ProgramFree(&gl_aeHueProgram);
        GL_ProgramFree(&gl_aeMotionProgram);
        GL_ProgramFree(&gl_aeZoom2Program);
        GL_ProgramFree(&gl_aeZoomProgram);
        return 0;
    }
    gl_aeBlurSrc = GL_i_ProgramUniform(&gl_aeBlurProgram, "uSrc");
    gl_aeBlurOff01 = GL_i_ProgramUniform(&gl_aeBlurProgram, "uOffsets01");
    gl_aeBlurOff23 = GL_i_ProgramUniform(&gl_aeBlurProgram, "uOffsets23");
    gl_aeBlurWeights = GL_i_ProgramUniform(&gl_aeBlurProgram, "uWeights");
    gl_aeMixSharp = GL_i_ProgramUniform(&gl_aeMixProgram, "uSharp");
    gl_aeMixBlurred = GL_i_ProgramUniform(&gl_aeMixProgram, "uBlurred");
    gl_aeColorSrc = GL_i_ProgramUniform(&gl_aeColorProgram, "uSrc");
    gl_aeColorGain = GL_i_ProgramUniform(&gl_aeColorProgram, "uGain");
    gl_aeColorBias = GL_i_ProgramUniform(&gl_aeColorProgram, "uBias");
    gl_aeColorGrayscale = GL_i_ProgramUniform(&gl_aeColorProgram,
                                              "uGrayscale");
    gl_aeColorWeights = GL_i_ProgramUniform(&gl_aeColorProgram, "uWeights");
    gl_aeRemSharp = GL_i_ProgramUniform(&gl_aeRemProgram, "uSharp");
    gl_aeRemBlurred = GL_i_ProgramUniform(&gl_aeRemProgram, "uBlurred");
    gl_aeRemWeight = GL_i_ProgramUniform(&gl_aeRemProgram, "uWeight");
    gl_aeBlendSrc = GL_i_ProgramUniform(&gl_aeBlendProgram, "uSrc");
    gl_aeBlendAlpha = GL_i_ProgramUniform(&gl_aeBlendProgram, "uAlpha");
    gl_aeFadeLive = GL_i_ProgramUniform(&gl_aeFadeProgram, "uLive");
    gl_aeFadeFrozen = GL_i_ProgramUniform(&gl_aeFadeProgram, "uFrozen");
    gl_aeFadeShare = GL_i_ProgramUniform(&gl_aeFadeProgram, "uFrozenShare");
    gl_aeHueSrc = GL_i_ProgramUniform(&gl_aeHueProgram, "uSrc");
    gl_aeHueSubtract = GL_i_ProgramUniform(&gl_aeHueProgram, "uSubtract");
    gl_aeMotionSrc = GL_i_ProgramUniform(&gl_aeMotionProgram, "uSrc");
    gl_aeMotionTapU = GL_i_ProgramUniform(&gl_aeMotionProgram, "uTapU");
    gl_aeMotionTapV = GL_i_ProgramUniform(&gl_aeMotionProgram, "uTapV");
    gl_aeMotionWeight = GL_i_ProgramUniform(&gl_aeMotionProgram, "uWeight");
    gl_aeZoom2Src = GL_i_ProgramUniform(&gl_aeZoom2Program, "uSrc");
    gl_aeZoom2TapsA = GL_i_ProgramUniform(&gl_aeZoom2Program, "uTapsA");
    gl_aeZoom2TapsB = GL_i_ProgramUniform(&gl_aeZoom2Program, "uTapsB");
    gl_aeZoom2Count = GL_i_ProgramUniform(&gl_aeZoom2Program, "uTapCount");
    gl_aeZoom2Weight = GL_i_ProgramUniform(&gl_aeZoom2Program, "uWeight");
    gl_aeZoomSrc = GL_i_ProgramUniform(&gl_aeZoomProgram, "uSrc");
    gl_aeZoomBandLo = GL_i_ProgramUniform(&gl_aeZoomProgram, "uBandLo");
    gl_aeZoomBandHi = GL_i_ProgramUniform(&gl_aeZoomProgram, "uBandHi");
    gl_aeZoomInset = GL_i_ProgramUniform(&gl_aeZoomProgram, "uInset");
    gl_aeZoomTaps = GL_i_ProgramUniform(&gl_aeZoomProgram, "uTaps");
    gl_aeZoomWeight = GL_i_ProgramUniform(&gl_aeZoomProgram, "uWeight");

    for (i = 0; i < GL_AE_TARGETS; i++)
    {
        GLFN_glGenTextures(1, &gl_aeColour[i]);
        GLFN_glGenFramebuffers(1, &gl_aeFbo[i]);
        if (!gl_aeColour[i] || !gl_aeFbo[i])
        {
            DBG_OutputDebugString("GL: no spare target %d for the"
                                  " after-effects\n", i);
            GL_l_ForgetAe();
            gl_aeTried = 1;
            return 0;
        }
        GLFN_glBindTexture(GL_TEXTURE_2D, gl_aeColour[i]);
        GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gl_aeW, gl_aeH, 0,
                          GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /* Clamped, because every pass steps outside the quad it is
           drawing: a tap at the edge has to take the edge pixel
           rather than wrapping round to the other side of the
           picture. */
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                             GL_CLAMP_TO_EDGE);
        GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                             GL_CLAMP_TO_EDGE);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);

        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_aeFbo[i]);
        GLFN_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                    GL_TEXTURE_2D, gl_aeColour[i], 0);
        if (GLFN_glCheckFramebufferStatus(GL_FRAMEBUFFER)
            != GL_FRAMEBUFFER_COMPLETE)
        {
            DBG_OutputDebugString("GL: spare target %d is not complete;"
                                  " no after-effects\n", i);
            GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
            GL_l_ForgetAe();
            gl_aeTried = 1;
            return 0;
        }

        /* Cleared, because a fresh texture's contents are undefined
           and every pass steps outside the strip it draws: a tap over
           the edge has to land on something known. Black, so that a
           pass reading past what the one before wrote darkens rather
           than inventing colour. */
        GL_l_SetColorMask(1, 1, 1, 1);
        GL_l_ClearColour(0.0f, 0.0f, 0.0f, 0.0f, 0);
    }
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLFN_glGenVertexArrays(1, &gl_aeVao);
    if (!gl_aeVao)
    {
        DBG_OutputDebugString("GL: no vertex array for the after-effects\n");
        GL_l_ForgetAe();
        gl_aeTried = 1;
        return 0;
    }
    GLFN_glBindVertexArray(gl_aeVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_quadBuffer);
    GLFN_glEnableVertexAttribArray(0);
    GLFN_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                               (GLsizei)(2 * sizeof(GLfloat)),
                               (const void*)(size_t)0);
    GLFN_glBindVertexArray(0);

    gl_aeReady = 1;
    return 1;
}

/* The scene target and the two spares, by the slot numbers the
   schedule names them with. */
static GLuint GL_l_i_AeSlotFbo(int slot)
{
    if (slot == DPB_SLOT_FRAME)
        return gl_sceneFbo;
    if (slot >= DPB_SLOT_A && slot < DPB_SLOT_A + GL_AE_TARGETS)
        return gl_aeFbo[slot - DPB_SLOT_A];
    return 0;
}

static GLuint GL_l_i_AeSlotTexture(int slot)
{
    if (slot == DPB_SLOT_FRAME)
        return gl_sceneColour;
    if (slot >= DPB_SLOT_A && slot < DPB_SLOT_A + GL_AE_TARGETS)
        return gl_aeColour[slot - DPB_SLOT_A];
    return 0;
}

/* Somewhere to write that is none of the slots named.
 *
 * The frame is never the answer: its alpha is the depth blur's input
 * and nothing in the chain may overwrite it.
 *
 * The answer has to be the *same* answer next frame for the same
 * chain, because the motion blur reads what it wrote last time round.
 * That is why this walks the spares in order and takes the first that
 * will do, rather than anything that happens to be free -- given the
 * same set of effects it returns the same slot every frame. Turn
 * another effect on and the slots shift along, and the motion blur
 * loses one frame of history; the other renderer does the same. */
static int GL_l_i_AeSpareExcept(int avoid1, int avoid2)
{
    int i;

    for (i = 0; i < GL_AE_SPARES; i++)
    {
        int slot = DPB_SLOT_A + i;

        if (slot != avoid1 && slot != avoid2)
            return slot;
    }
    /* Unreachable with three spares and at most two to avoid; the
       first spare is a defined answer rather than a wrong one. */
    return DPB_SLOT_A;
}

/* Somewhere to write that is not what is being read, for the effects
   that read one thing. */
static int GL_l_i_AeOtherSlot(int slot)
{
    return GL_l_i_AeSpareExcept(slot, slot);
}

/* One pass's quad, covering the strip between two fractions measured
   from the top of the picture.
 *
 * Clip coordinates, so plus one is the top -- the renderer's depth
 * range is set up to keep OpenGL's own origin, and the world is drawn
 * with the top of the screen at plus one, so the frame in the target
 * has its first row at the bottom. That is also why the vertex stage
 * can work the texture coordinate out from the position. */
static void GL_l_AeDrawBand(float topFraction, float bottomFraction)
{
    GLfloat v[12];
    GLfloat yTop = 1.0f - 2.0f * topFraction;
    GLfloat yBottom = 1.0f - 2.0f * bottomFraction;

    v[0] = -1.0f; v[1] = yBottom;
    v[2] = -1.0f; v[3] = yTop;
    v[4] =  1.0f; v[5] = yBottom;

    v[6] =  1.0f; v[7] = yBottom;
    v[8] = -1.0f; v[9] = yTop;
    v[10] =  1.0f; v[11] = yTop;

    GL_l_DrawBatch(gl_aeVao, v, 2, 2 * sizeof(GLfloat));
}

/* The engine's state the three frame-sum effects are decided from.
   Read in one place because all three want the band's bottom edge as
   well as their own parameter, and only one of them uses it. */
static void GL_l_AeReadSource(tAef_Source* out, const tCbd_Bands* bands)
{
    out->aeMaster = (unsigned int)PC_AE_MASTER;
    out->motionBlur = motionBlurFactor;
    out->remanence = remenanceFactor;
    out->zoomSmooth = zoomSmoothFactor;
    out->screenBlur = blurFactor;
    out->bandBottom = bands ? bands->bottom : 1.0f;
}

/* One target's colour over another's, leaving the destination's alpha
 * alone.
 *
 * The depth blur's mix needs this: the renderer this follows writes
 * its mix into the frame with the write narrowed to colour, so the
 * frame keeps the alpha the world gave it. A pass here cannot read and
 * write one texture, so the mix lands elsewhere and this puts it back.
 *
 * The blend program is a plain passthrough of colour with an alpha of
 * its own, which is exactly what is wanted once the alpha write is
 * masked off -- so no seventh shader.
 */
static void GL_l_AeCopyColour(const tCbd_Bands* bands, int from, int to)
{
    GLFN_glUseProgram(gl_aeBlendProgram.program);
    if (gl_aeBlendSrc >= 0)
        GLFN_glUniform1i(gl_aeBlendSrc, 0);
    if (gl_aeBlendAlpha >= 0)
        GLFN_glUniform1f(gl_aeBlendAlpha, 1.0f);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(to));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));

    GL_l_SetColorMask(1, 1, 1, 0);
    GL_l_AeDrawBand(bands->top, bands->bottom);
    GL_l_SetColorMask(1, 1, 1, 1);
}

/* The exchange the chain does after an effect that wrote the spare:
   what it wrote becomes the current frame and what was current becomes
   the spare. Written out because the renderer this follows writes it
   out, and because which target an effect lands on is decided by how
   many of these have happened. */
static void GL_l_AeExchange(int* cur, int* spr)
{
    int was = *spr;

    *spr = *cur;
    *cur = was;
}

/* A run of blur passes over the frame, each reading what the one
 * before wrote.
 *
 * Shared by the two effects that want one: the depth blur's three
 * passes at one, two and three pixels, and the remanence's four at
 * one, three, five and seven. What differs between them is the reach
 * and the count, which arrive as arguments; what does not is the
 * ping-pong, the taps and the strip covered.
 *
 * Starts from `from`, works alternately in `first` and `second`, and
 * returns the slot the last pass wrote. The caller's own input is left
 * alone -- both callers need it afterwards, one as the sharp frame for
 * a mix and one for a sum.
 *
 * Drawn over the widened reach rather than the band, because whatever
 * samples the result will sample it right up to the band's edge and a
 * tap there reaches past it.
 */
static int GL_l_i_AeBlurChain(const tCbd_Bands* bands, const float* uSteps,
                              const float* vSteps, int count, int from,
                              int first, int second)
{
    int to = first;
    int i;

    GLFN_glUseProgram(gl_aeBlurProgram.program);
    if (gl_aeBlurSrc >= 0)
        GLFN_glUniform1i(gl_aeBlurSrc, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);

    for (i = 0; i < count; i++)
    {
        tDpb_BlurPass pass;
        float         offsets[2 * DPB_BLUR_TAPS];
        float         weights[DPB_BLUR_TAPS];

        pass.fromSlot = from;
        pass.toSlot = to;
        pass.uStep = uSteps[i];
        pass.vStep = vSteps[i];
        DPB_BlurTaps(&pass, offsets, weights);

        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(to));
        GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));
        /* The schedule's vertical offsets are measured down the
           screen, which is the direction the other renderer's texture
           coordinates run. This renderer's run the other way, so they
           are negated here -- and it matters, because the four taps
           are not weighted alike: the heaviest of them is the one
           below the pixel, and a missing negation puts the smear
           above every edge instead of below it. */
        if (gl_aeBlurOff01 >= 0)
            GLFN_glUniform4f(gl_aeBlurOff01, offsets[0], -offsets[1],
                             offsets[2], -offsets[3]);
        if (gl_aeBlurOff23 >= 0)
            GLFN_glUniform4f(gl_aeBlurOff23, offsets[4], -offsets[5],
                             offsets[6], -offsets[7]);
        if (gl_aeBlurWeights >= 0)
            GLFN_glUniform4f(gl_aeBlurWeights, weights[0], weights[1],
                             weights[2], weights[3]);

        GL_l_AeDrawBand(bands->reachTop, bands->reachBottom);

        /* Back and forth between the two the caller named, leaving its
           own input alone throughout. Which of the two comes first is
           the caller's, because the renderer this follows starts the
           depth blur's run on its scratch texture and the remanence's
           on the chain's spare -- and both runs have to end where
           that one ends them, since what reads the result reads it by
           name. */
        from = to;
        to = (to == first) ? second : first;
    }
    return from;
}

/* The depth blur, over the frame. Returns the slot the finished frame
   ended up in.
 *
   It reads the frame itself and not whatever the chain is currently
   holding, because the alpha it mixes by belongs to the frame as the
   world drew it -- so this is the first effect in the chain or it is
   mixing by an alpha something else has been through. */
static int GL_l_i_AeDepthBlur(const tCbd_Bands* bands, int spare)
{
    tDpb_BlurPass passes[DPB_BLUR_PASSES];
    float         uSteps[DPB_BLUR_PASSES];
    float         vSteps[DPB_BLUR_PASSES];
    int           n;
    int           i;
    int           blurred;
    int           mixInto;

    n = DPB_i_BlurSchedule(passes, DPB_BLUR_PASSES);
    if (n <= 0)
        return DPB_SLOT_FRAME;
    for (i = 0; i < n; i++)
    {
        uSteps[i] = passes[i].uStep;
        vSteps[i] = passes[i].vStep;
    }

    /* Starting on the scratch, which is where the renderer this
       follows starts it -- so the three passes land scratch, spare,
       scratch and leave the spare holding the second pass. What is
       left in the spare is not waste: the motion blur reads whatever
       is in the target it writes, and with the depth blur in front of
       it that is this. */
    blurred = GL_l_i_AeBlurChain(bands, uSteps, vSteps, n, DPB_SLOT_FRAME,
                                 GL_AE_SLOT_SCRATCH, spare);

    /* And the mix, over the band itself. It reads the frame and the
       blurred copy, so it writes the third spare -- and is then copied
       back over the frame, because that is where the renderer this
       follows leaves it and everything after this one is placed
       relative to that. */
    mixInto = GL_AE_SLOT_MIXTMP;

    GLFN_glUseProgram(gl_aeMixProgram.program);
    if (gl_aeMixSharp >= 0)
        GLFN_glUniform1i(gl_aeMixSharp, 0);
    if (gl_aeMixBlurred >= 0)
        GLFN_glUniform1i(gl_aeMixBlurred, 1);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(mixInto));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(DPB_SLOT_FRAME));
    GLFN_glActiveTexture(GL_TEXTURE1);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(blurred));

    GL_l_AeDrawBand(bands->top, bands->bottom);

    GLFN_glActiveTexture(GL_TEXTURE1);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);

    /* Back over the frame, colour only. The renderer this follows
       writes its mix straight into the frame and narrows the write to
       colour, so the frame keeps the alpha the world gave it; this is
       the same thing in two steps because a pass here cannot read and
       write one texture. */
    GL_l_AeCopyColour(bands, mixInto, DPB_SLOT_FRAME);
    return DPB_SLOT_FRAME;
}

/* The remanence: a wide blur of the frame added back on top of it.
 *
 * A bloom rather than a trail, whatever the name says -- nothing here
 * reads the frame before. Returns the slot the frame ended in, which
 * is the one it came in on when the effect is not asked for.
 */
static int GL_l_b_AeRemanence(const tCbd_Bands* bands, int from, int spare)
{
    tAef_Source src;
    float       uSteps[AEF_REMANENCE_PASSES];
    float       vSteps[AEF_REMANENCE_PASSES];
    int         n;
    int         blurred;
    int         into;

    GL_l_AeReadSource(&src, bands);
    if (!AEF_b_Remanence(&src))
        return 0;

    n = AEF_i_RemanenceReaches(uSteps, vSteps, AEF_REMANENCE_PASSES);
    if (n <= 0)
        return 0;

    /* Starting on the chain's spare rather than the scratch, which is
       the other way round from the depth blur -- and is how the
       renderer this follows does it. Four passes alternating from
       there end on the scratch, which is where the sum below looks
       for them. */
    blurred = GL_l_i_AeBlurChain(bands, uSteps, vSteps, n, from,
                                 spare, GL_AE_SLOT_SCRATCH);

    /* The sum goes to the chain's spare, which the exchange after it
       then makes the current frame. */
    into = spare;

    GLFN_glUseProgram(gl_aeRemProgram.program);
    if (gl_aeRemSharp >= 0)
        GLFN_glUniform1i(gl_aeRemSharp, 0);
    if (gl_aeRemBlurred >= 0)
        GLFN_glUniform1i(gl_aeRemBlurred, 1);
    if (gl_aeRemWeight >= 0)
        GLFN_glUniform1f(gl_aeRemWeight, AEF_f_RemanenceWeight(&src));
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(into));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));
    GLFN_glActiveTexture(GL_TEXTURE1);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(blurred));

    GL_l_AeDrawBand(bands->top, bands->bottom);

    GLFN_glActiveTexture(GL_TEXTURE1);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);
    return 1;
}

/* The hue sweep: a colour taken off the picture, cycling.
 *
 * All the arithmetic is aehue.c's and the differential against the
 * other renderer compares it packed word for packed word. What is
 * here is the renderer's share, and two parts of it are worth saying.
 *
 * **The three amounts come out of a packed colour and not out of three
 * numbers.** That looks like a detour and is the opposite: at a depth
 * above one the channels overflow their bytes, and they do not all
 * overflow the same way -- red's extra is lost and green's and blue's
 * carry into the channel above. Asking the shared code for the word
 * and unpacking it reproduces that; asking it for three clamped
 * fractions would be right about the arithmetic and wrong about the
 * picture.
 *
 * **The alpha write is masked off.** The other renderer narrows its
 * colour write to the three colour channels for this effect, so the
 * target keeps whatever alpha it already had. Nothing downstream reads
 * it -- the one effect whose input is the frame's alpha has already
 * run by here -- but the write is the write.
 */
static int GL_l_b_AeHue(const tCbd_Bands* bands, int from, int spare)
{
    float        subtract[AHU_CHANNELS];
    unsigned int packed;

    if (!AHU_b_On((unsigned int)PC_AE_MASTER))
        return 0;

    packed = AHU_u_Subtract(unknownAEFlag4GlobalVar, texFactorManual);
    AHU_Channels(subtract, packed);

    GLFN_glUseProgram(gl_aeHueProgram.program);
    if (gl_aeHueSrc >= 0)
        GLFN_glUniform1i(gl_aeHueSrc, 0);
    if (gl_aeHueSubtract >= 0)
        GLFN_glUniform3f(gl_aeHueSubtract, subtract[0], subtract[1],
                         subtract[2]);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(spare));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));

    GL_l_SetColorMask(1, 1, 1, 0);
    GL_l_AeDrawBand(bands->top, bands->bottom);
    GL_l_SetColorMask(1, 1, 1, 1);
    return 1;
}

/* The fade: a frozen copy of the picture dissolving into the live one.
 *
 * Four things set this effect apart from its neighbours in the chain,
 * and three of them are visible right here.
 *
 * **It keeps a frame from the past, and not the last one.** The motion
 * blur reads the frame before, which it gets for free by not clearing
 * the target it writes. This one reads the frame the fade *started*
 * on, which may be 255 frames back, so it needs a target nothing else
 * may touch -- GL_AE_SLOT_FREEZE, and the note there says why it is
 * outside the spare rotation.
 *
 * **It covers the whole picture and not the letterbox band.** Every
 * other effect here is handed the band and draws between its edges;
 * the other renderer's quad for this one spans the whole target, and
 * so does this. Nothing is visible either way -- the bars are painted
 * over afterwards -- but the draw is the draw.
 *
 * **It carries a timer and switches itself off**, which is the one
 * place in this renderer where an effect writes the after-effects
 * word. The arithmetic of the timer is aefade.h's; what is here is
 * when it is stepped, which is *after* the draw. So the first frame of
 * a fade is shown at the opacity the fade started with, which is why a
 * fade begins invisibly: the frozen copy is still identical to the
 * live frame it was taken from.
 */
static void GL_l_AeFreeze(int from)
{
    /* A copy of the whole target, which is a blit rather than a pass:
       nothing is being computed, and the other renderer copies
       surface to surface here as well. The read and draw bindings are
       left as the frame bindings afterwards, which every caller sets
       for itself before it draws. */
    GLFN_glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_l_i_AeSlotFbo(from));
    GLFN_glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                           GL_l_i_AeSlotFbo(GL_AE_SLOT_FREEZE));
    GLFN_glBlitFramebuffer(0, 0, gl_aeW, gl_aeH, 0, 0, gl_aeW, gl_aeH,
                           GL_COLOR_BUFFER_BIT, GL_NEAREST);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static int GL_l_b_AeFade(int from, int spare)
{
    int   opacity;
    float step;

    if (!AFD_b_On((unsigned int)PC_AE_MASTER))
        return 0;

    opacity = GDI_i_AeFadeOpacity();
    step = AFD_f_Step(gAEOpacity);

    /* The frame the fade starts on: the picture is put aside, and from
       here to the end of the fade it is that copy being dissolved out
       of. */
    if (AFD_b_Freeze(opacity))
        GL_l_AeFreeze(from);

    GLFN_glUseProgram(gl_aeFadeProgram.program);
    if (gl_aeFadeLive >= 0)
        GLFN_glUniform1i(gl_aeFadeLive, 0);
    if (gl_aeFadeFrozen >= 0)
        GLFN_glUniform1i(gl_aeFadeFrozen, 1);
    if (gl_aeFadeShare >= 0)
        GLFN_glUniform1f(gl_aeFadeShare, AFD_f_Blend(opacity));
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(spare));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));
    GLFN_glActiveTexture(GL_TEXTURE1);
    GLFN_glBindTexture(GL_TEXTURE_2D,
                       GL_l_i_AeSlotTexture(GL_AE_SLOT_FREEZE));

    /* The whole picture, not the band -- see the note above. */
    GL_l_AeDrawBand(0.0f, 1.0f);

    GLFN_glActiveTexture(GL_TEXTURE1);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);

    /* And then the timer, after the draw. A fade that has run out
       clears its own bit, which is what the scripts watch to know the
       fade is over. */
    if (AFD_b_Advance(&opacity, step))
        PC_AE_MASTER = (int)AFD_u_Cleared((unsigned int)PC_AE_MASTER);
    GDI_AeFadeOpacitySet(opacity);
    return 1;
}

/* The motion blur: this frame over the frame before.
 *
 * The one effect that reads across frames, and it does it without a
 * history buffer: the target it writes is never cleared, so what is
 * already in it is what this effect left there last frame. The mixing
 * is the framebuffer's blend, because the thing being mixed with is
 * the destination.
 *
 * That makes the *choice* of target the substance of this effect
 * rather than an implementation detail. It has to come out the same
 * every frame, or the history read is some other effect's leftovers;
 * GL_l_i_AeSpareExcept is written to walk the spares in order for that
 * reason, and this effect is why.
 */
static int GL_l_b_AeMotionBlur(const tCbd_Bands* bands, int from, int spare)
{
    tAef_Source src;
    int         into;

    GL_l_AeReadSource(&src, bands);
    if (!AEF_b_MotionBlur(&src))
        return 0;

    /* The chain's spare, and that is the whole of the choice. Where
       the spare happens to be is decided by how many effects have run
       before this one, and the renderer this follows is the same --
       which is why running it on its own gives a trail that works and
       running it behind the remanence does not. See the comment above
       the chain. */
    into = spare;

    GLFN_glUseProgram(gl_aeBlendProgram.program);
    if (gl_aeBlendSrc >= 0)
        GLFN_glUniform1i(gl_aeBlendSrc, 0);
    /* The new frame's share. The blend below takes one less it for
       what is already there, which is the frame before. */
    if (gl_aeBlendAlpha >= 0)
        GLFN_glUniform1f(gl_aeBlendAlpha,
                         1.0f - AEF_f_MotionBlurKeep(&src));
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(into));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));

    /* The only effect in the chain that blends, so it turns it on and
       off around itself rather than leaving it to the chain. */
    GLFN_glEnable(GL_BLEND);
    GLFN_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_l_AeDrawBand(bands->top, bands->bottom);
    GLFN_glDisable(GL_BLEND);
    return 1;
}

/* The zoom smooth: the frame added to itself at eight tightening crops
 * about the centre of the band, so a pixel smears along the line from
 * the centre through it.
 */
/* The motion smooth: a smear along the way the camera just moved.
 *
 * The arithmetic -- what is watched, how far it has to move, where the
 * four samples sit and what they weigh -- is aemotion.c's, and the
 * differential against the other renderer drives the threshold from
 * both sides. What is here is the renderer's share, and three parts of
 * it are worth saying.
 *
 * **Last frame's point is replaced whether or not the smear ran**, and
 * that is the line most easily left out. Leave it inside the branch
 * and a slow camera accumulates its way over the threshold and then
 * smears by everything it has drifted since it started -- which looks
 * like a bug in the camera rather than in here.
 *
 * **The chain has to run for that to happen at all.** So the gate asks
 * for this effect's bit even though the bit alone may not smear, for
 * the same reason the fade is asked: a frame where the chain was
 * skipped is a frame where the point was not updated.
 *
 * **The vertical offsets are negated on the way to the shader.** The
 * other renderer measures its texture's vertical from the top of the
 * picture and this one measures it from the bottom, so an offset that
 * reaches up there reaches down here. Nothing in the arithmetic knows
 * that; it is this renderer's own business, and there is a case that
 * puts a bright row in the frame and reads which way it smeared rather
 * than taking it on trust.
 */
static int GL_l_b_AeMotionSmooth(const tCbd_Bands* bands, int from, int spare)
{
    Vec3  now;
    float u[AMO_TAPS];
    float v[AMO_TAPS];
    float dx = 0.0f;
    float dy = 0.0f;
    int   smear;

    if (!AMO_b_On((unsigned int)PC_AE_MASTER))
        return 0;

    AMO_Probe(&now.x);
    MATH_TransformVector(&now, &gpst_DX_stDD->ct.cam.cameraMtx, &now);
    smear = AMO_b_Decide(&dx, &dy, &now.x, &gl_aeMotionPrev.x);

    /* Before the early return, not after it -- see the note above. */
    gl_aeMotionPrev = now;
    if (!smear)
        return 0;

    AMO_Taps(u, v, dx, dy);

    GLFN_glUseProgram(gl_aeMotionProgram.program);
    if (gl_aeMotionSrc >= 0)
        GLFN_glUniform1i(gl_aeMotionSrc, 0);
    if (gl_aeMotionTapU >= 0)
        GLFN_glUniform4f(gl_aeMotionTapU, u[0], u[1], u[2], u[3]);
    /* Negated, because this renderer's vertical runs the other way. */
    if (gl_aeMotionTapV >= 0)
        GLFN_glUniform4f(gl_aeMotionTapV, -v[0], -v[1], -v[2], -v[3]);
    if (gl_aeMotionWeight >= 0)
        GLFN_glUniform1f(gl_aeMotionWeight, AMO_f_Weight());
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(spare));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));

    GL_l_SetColorMask(1, 1, 1, 0);
    GL_l_AeDrawBand(bands->top, bands->bottom);
    GL_l_SetColorMask(1, 1, 1, 1);
    return 1;
}

static int GL_l_b_AeZoomSmooth(const tCbd_Bands* bands, int from, int spare)
{
    tAef_Source  src;
    tAef_ZoomTap taps[AEF_ZOOM_TAPS];
    int          n;
    int          into;
    float        loV;
    float        hiV;

    GL_l_AeReadSource(&src, bands);
    if (!AEF_b_ZoomSmooth(&src))
        return 0;

    n = AEF_i_ZoomTaps(&src, taps, AEF_ZOOM_TAPS);
    if (n <= 0)
        return 0;

    into = spare;

    /* The band's corners in texture space. The fractions are measured
       from the top of the picture and this renderer's textures run up
       it, so the band's top is the larger coordinate.
     *
       No test can tell this from the un-flipped spelling, and that is
       not a gap: the band is symmetric by construction -- its bottom
       is one less its top -- so one less the bottom *is* the top and
       the two forms agree at every setting. The derivation is written
       the right way round so that an asymmetric band, if one ever
       arrives, does not quietly flip the zoom's centre. */
    loV = 1.0f - bands->bottom;
    hiV = 1.0f - bands->top;

    GLFN_glUseProgram(gl_aeZoomProgram.program);
    if (gl_aeZoomSrc >= 0)
        GLFN_glUniform1i(gl_aeZoomSrc, 0);
    if (gl_aeZoomBandLo >= 0)
        GLFN_glUniform2f(gl_aeZoomBandLo, 0.0f, loV);
    if (gl_aeZoomBandHi >= 0)
        GLFN_glUniform2f(gl_aeZoomBandHi, 1.0f, hiV);
    /* One tap's worth of crop. The taps are evenly spaced, so the
       step is the whole of what the shader needs to place all eight --
       and the first tap, at no crop at all, is the frame itself. */
    if (gl_aeZoomInset >= 0)
        GLFN_glUniform2f(gl_aeZoomInset, taps[1].insetU, taps[1].insetV);
    if (gl_aeZoomTaps >= 0)
        GLFN_glUniform1i(gl_aeZoomTaps, n);
    if (gl_aeZoomWeight >= 0)
        GLFN_glUniform1f(gl_aeZoomWeight, AEF_f_ZoomWeight());
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(into));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));

    GL_l_AeDrawBand(bands->top, bands->bottom);
    return 1;
}

/* The three colour effects, over the band. Returns the slot the frame
   ended up in, which is the one it started in when none of them
   apply.
 *
   A draw each rather than one pass doing all three: that is how the
   renderer with these runs them, and each draw lands in an eight-bit
   target, so a channel one step pushed past white is white before the
   next step sees it. Folded into one pass they would need a clamp
   between them to come out the same, and would still differ by the
   rounding -- so the drawing is left as it is and the arithmetic is
   the only thing shared. */
/* The screen blur: the frame blurred seven times over, each pass
 * reaching one step further than the last.
 *
 * The least work of any effect in this chain, and the reason is worth
 * stating: every pass is the **same four-tap blur the depth blur is
 * built from**, so there is no shader here and no arithmetic -- only a
 * schedule, and the schedule is aeframe.c's. What is left is handing
 * seven reaches to the chain driver the remanence already uses.
 *
 * Seven passes alternating from the chain's spare end back **on** the
 * spare, which is where the exchange after this call looks for them.
 * An even count would end on the scratch and the blur would be thrown
 * away, so the odd count is load-bearing rather than incidental.
 */
static int GL_l_b_AeScreenBlur(const tCbd_Bands* bands, int from, int spare)
{
    tAef_Source src;
    float       uSteps[AEF_SCREENBLUR_PASSES];
    float       vSteps[AEF_SCREENBLUR_PASSES];
    int         n;

    GL_l_AeReadSource(&src, bands);
    if (!AEF_b_ScreenBlur(&src))
        return 0;

    n = AEF_i_ScreenBlurReaches(uSteps, vSteps, AEF_SCREENBLUR_PASSES,
                                src.screenBlur);
    if (n <= 0)
        return 0;

    GL_l_i_AeBlurChain(bands, uSteps, vSteps, n, from, spare,
                       GL_AE_SLOT_SCRATCH);
    return 1;
}

/* The second zoom smooth: eight samples of the frame, each turned
 * about the middle of the picture, summed.
 *
 * All of it is aezoom2.c's, and this is the thinnest renderer-side
 * piece of any effect here -- eight transforms, a count and a weight,
 * handed to one draw. Three notes.
 *
 * **The eight transforms go over as matrices, not angles.** Rebuilding
 * them in the shader from an angle would put the effect's one real
 * finding -- that the transform is written as a rotation and is not
 * one -- in a place no differential can reach. They are computed where
 * the other renderer's copy is compared against them.
 *
 * **Two four-by-four uniforms hold them.** Eight taps of four entries
 * is thirty-two floats, which is exactly two of those, one tap per
 * column. This renderer has no vector-array uniform entry point and
 * needs none for it.
 *
 * **The other renderer takes all eight in one draw only when the card
 * has eight stages, which no card has.** It splits them across stages
 * and draws instead, and the split changes the angles' last bits, so
 * the split is asked for rather than assumed -- see AZ2_i_Stages. Four
 * is what this renderer reports, which is what a modern card reports
 * there too.
 */
static int GL_l_b_AeZoomSmooth2(const tCbd_Bands* bands, int from,
                                int spare)
{
    float angles[AZ2_TAPS];
    float taps[2][16];
    int   stages;
    int   samples;
    int   n;
    int   i;

    if (!AZ2_b_On((unsigned int)PC_AE_MASTER, zoomSmoothFactor2))
        return 0;

    stages = AZ2_i_Stages(4, ENG_ifSingleStreamThenEq2Otherwise8);
    samples = AZ2_i_Samples(stages);
    n = AZ2_i_SampledAngles(angles, AZ2_TAPS, zoomSmoothFactor2, stages);
    if (n <= 0)
        return 0;

    /* One tap to a column, four to a matrix, and the columns past the
       count left at nothing -- the shader stops at the count, so what
       is in them does not reach the picture. */
    for (i = 0; i < 32; i++)
        taps[i / 16][i % 16] = 0.0f;
    for (i = 0; i < n && i < AZ2_TAPS; i++)
        AZ2_Tap(&taps[i / 4][(i % 4) * 4], angles[i]);

    GLFN_glUseProgram(gl_aeZoom2Program.program);
    if (gl_aeZoom2Src >= 0)
        GLFN_glUniform1i(gl_aeZoom2Src, 0);
    if (gl_aeZoom2TapsA >= 0)
        GLFN_glUniformMatrix4fv(gl_aeZoom2TapsA, 1, GL_FALSE, taps[0]);
    if (gl_aeZoom2TapsB >= 0)
        GLFN_glUniformMatrix4fv(gl_aeZoom2TapsB, 1, GL_FALSE, taps[1]);
    if (gl_aeZoom2Count >= 0)
        GLFN_glUniform1i(gl_aeZoom2Count, n);
    if (gl_aeZoom2Weight >= 0)
        GLFN_glUniform1f(gl_aeZoom2Weight, AZ2_f_Weight(samples));
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(spare));
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));

    GLFN_glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    GL_l_AeDrawBand(bands->top, bands->bottom);
    GLFN_glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    return 1;
}

static int GL_l_i_AeColor(const tCbd_Bands* bands, int from)
{
    tAec_Source   src;
    tAec_Step     steps[AEC_MAX_STEPS];
    int           n;
    int           i;

    src.aeMaster = (unsigned int)PC_AE_MASTER;
    src.brightness = unkAEFunc2Param;
    src.contrast = colorBalanceParam1;
    n = AEC_i_Steps(&src, steps, AEC_MAX_STEPS);
    if (n <= 0)
        return from;

    GLFN_glUseProgram(gl_aeColorProgram.program);
    if (gl_aeColorSrc >= 0)
        GLFN_glUniform1i(gl_aeColorSrc, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);

    for (i = 0; i < n; i++)
    {
        int to = GL_l_i_AeOtherSlot(from);

        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, GL_l_i_AeSlotFbo(to));
        GLFN_glBindTexture(GL_TEXTURE_2D, GL_l_i_AeSlotTexture(from));
        if (gl_aeColorGain >= 0)
            GLFN_glUniform1f(gl_aeColorGain, steps[i].gain);
        if (gl_aeColorBias >= 0)
            GLFN_glUniform1f(gl_aeColorBias, steps[i].bias);
        /* The weights are only a grayscale's, and the flag is what
           says to use them. Both are set every step so that a step
           cannot inherit the one before's. */
        if (gl_aeColorGrayscale >= 0)
            GLFN_glUniform1f(gl_aeColorGrayscale,
                             steps[i].kind == AEC_GRAYSCALE ? 1.0f : 0.0f);
        if (gl_aeColorWeights >= 0)
            GLFN_glUniform3f(gl_aeColorWeights, steps[i].weights[0],
                             steps[i].weights[1], steps[i].weights[2]);

        GL_l_AeDrawBand(bands->top, bands->bottom);
        from = to;
    }
    return from;
}

/* Whether any effect in the chain has something to do this frame. */
static int GL_l_b_AeWanted(void)
{
    tAec_Source src;
    tAec_Step   steps[AEC_MAX_STEPS];

    if (DPB_b_AfterEffect((unsigned int)PC_AE_MASTER))
        return 1;
    /* The fade is asked before the rest because it is the one effect
       that has to run in order to *stop*: it clears its own bit when
       its timer runs out, and a frame where the chain was skipped is a
       frame where that cannot happen. */
    if (AFD_b_On((unsigned int)PC_AE_MASTER))
        return 1;
    /* And the hue sweep, for the same reason the fade is asked here:
       its bit is the whole of its gate, so a depth of nought is still
       a draw. */
    if (AHU_b_On((unsigned int)PC_AE_MASTER))
        return 1;
    /* And the motion smooth, which has to run even on a frame it will
       not smear on: that is the frame it records where the camera is,
       and without it the movement accumulates. */
    if (AMO_b_On((unsigned int)PC_AE_MASTER))
        return 1;
    if (AZ2_b_On((unsigned int)PC_AE_MASTER, zoomSmoothFactor2))
        return 1;

    src.aeMaster = (unsigned int)PC_AE_MASTER;
    src.brightness = unkAEFunc2Param;
    src.contrast = colorBalanceParam1;
    if (AEC_i_Steps(&src, steps, AEC_MAX_STEPS) > 0)
        return 1;

    {
        tAef_Source fsrc;

        /* The band is not read by any of the three gates, so a
           default will do here -- the question is only whether
           anything wants to run. */
        GL_l_AeReadSource(&fsrc, NULL);
        if (AEF_b_Remanence(&fsrc) || AEF_b_MotionBlur(&fsrc)
            || AEF_b_ZoomSmooth(&fsrc) || AEF_b_ScreenBlur(&fsrc))
            return 1;
    }
    return 0;
}

static GLuint GL_l_i_RunAfterEffects(void)
{
    tCbd_Bands bands;
    GLuint     shown;
    int        cur;
    int        spr;

    /* Where the world drew. Without a scene target it drew straight to
       the screen, and there is nothing here that can run over it. */
    shown = GL_l_i_SceneFbo();
    if (!gl_sceneReady || !gl_state.context)
        return shown;

    /* The two switches that turn the whole chain off. The other
       renderer spells this as zeroing the master word for the length
       of its own pass and putting it back after; nothing else here
       reads the word, so the gate is the same thing said once. */
    if (tweak_ae_no || ECAM_gi_NoAfterEffects)
        return shown;
    /* Is there anything to do at all? Asked of every effect in the
       chain rather than of one of them, so that nothing is built or
       bound for a frame that wants none of it. */
    if (!GL_l_b_AeWanted())
        return shown;
    if (!GL_l_b_AeReady())
        return shown;

    CBD_Decide(&bands);

    /* No depth, no culling, no blending and the whole colour written:
       a pass is one quad over the frame and every one of those would
       change what it leaves behind. */
    GLFN_glDisable(GL_DEPTH_TEST);
    GLFN_glDisable(GL_CULL_FACE);
    GLFN_glDisable(GL_BLEND);
    GL_l_SetColorMask(1, 1, 1, 1);
    GLFN_glViewport(0, 0, gl_aeW, gl_aeH);

    /* The chain, in the order the other renderer runs it. Each effect
       is handed the slot the frame is in and says where it left it,
       so adding one is adding a line here -- and an effect that has
       nothing to do hands the slot straight back.
     *
       The order is the other renderer's and gaps in it are deliberate:
       between the ones below sit five more not written yet, and when
       they arrive they go in their places rather than on the end. The
       order matters most to the motion blur, which reads the target it
       wrote last frame -- so what runs before it decides which target
       that is. */
    /* A current and a spare, exchanged after every effect that writes
       the spare -- which is how the renderer this follows carries the
       frame through its chain, and the reason each effect lands where
       it does.
     *
       The depth blur is the exception at both ends: it writes the
       frame rather than the spare, and so there is no exchange after
       it. That is its own doing there as well.
     *
       One consequence is worth knowing rather than discovering. The
       motion blur reads whatever is in the target it writes, which is
       the spare -- so where the spare has got to decides what it reads
       as the frame before. Run on its own, the spare has not moved and
       holds its own output from last frame, and the trail works. Run
       behind the remanence, one exchange has happened and the spare is
       the frame, whose contents are the world as it was drawn this
       frame -- so the trail silently stops being a trail. That is what
       the renderer this follows does, and matching it is deliberate. */
    cur = DPB_SLOT_FRAME;
    spr = GL_AE_SLOT_SPARE;

    if (DPB_b_AfterEffect((unsigned int)PC_AE_MASTER))
        cur = GL_l_i_AeDepthBlur(&bands, spr);
    if (GL_l_b_AeHue(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    if (GL_l_b_AeRemanence(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    if (GL_l_b_AeFade(cur, spr))
        GL_l_AeExchange(&cur, &spr);
    if (GL_l_b_AeMotionBlur(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    if (GL_l_b_AeMotionSmooth(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    if (GL_l_b_AeZoomSmooth(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    if (GL_l_b_AeScreenBlur(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    /* The last of the chain before the colour pass. The other renderer
       takes the spare as the frame here without putting the old frame
       back in the spare -- it is the last effect, so nothing looks --
       and exchanging does the same thing and leaves the pair tidy. */
    if (GL_l_b_AeZoomSmooth2(&bands, cur, spr))
        GL_l_AeExchange(&cur, &spr);
    /* The colour effects are last and nothing reads a target after
       them, so they keep their own simpler arrangement: each writes
       somewhere that is not what it is reading. The renderer this
       follows writes a temporary and copies back, which is the same
       picture and one draw more. */
    cur = GL_l_i_AeColor(&bands, cur);
    shown = GL_l_i_AeSlotFbo(cur);

    GLFN_glUseProgram(0);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /* Back to what the engine asked for, as the overlay and video
       passes do. */
    if (gl_state.zEnable)
        GLFN_glEnable(GL_DEPTH_TEST);
    return shown;
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

/* ======================================================= the water
 *
 * Two passes in one place, and only the second of them is here yet.
 *
 * The first is a reflection: the world drawn a second time into an
 * off-screen target with each object mirrored in the water plane. That
 * is a change inside the per-object draw path and is not done here, so
 * the target this clears stays the colour it was cleared to and the
 * surface below reflects a flat colour rather than the world. The
 * surface is in the right place, the right shape, the right tint and
 * rippling correctly; what it mirrors is missing.
 *
 * The second is the surface: a grid the simulation has already built,
 * drawn once after the world with the reflection projected onto it
 * through the rippled direction from the camera to each grid point.
 * The grid, that direction and the projection are in watermesh.c,
 * shared with the other renderer and compared against it byte for byte
 * -- so what is left here is the drawing, which is the part that
 * differs between the two.
 *
 * Until lately the engine called straight into the other renderer for
 * both, so a machine running this one got no water at all. That is
 * what the two interface entries at the bottom are for.
 */

/* The reflection target, square and the size the other renderer makes
   it. The size is not arbitrary: the projection remap in watermesh.c
   is written against a nominal size, and a target of a different size
   is scaled by the ratio rather than being wrong -- but matching it
   keeps the ratio at one, which is what the original computes. */
#define GL_WATER_SIZE   512

static GLuint     gl_waterFbo;
static GLuint     gl_waterColour;
static GLuint     gl_waterDepth;
static int        gl_waterTried;
static int        gl_waterReady;

/* The camera as it stood when the frame was prepared, which is what
   every mirrored object this frame is measured against. Declared with
   the rest of the water's state rather than beside the reflection
   below, because the prepare that fills it comes first in the file. */
static Mtx4x4     gl_waterCamMtx;

/* Whether there is water this frame. The engine's own answer is
   whether it has water parameters at all, asked once before the world
   is drawn; everything after reads this rather than asking again, so a
   frame cannot half have water. */
static int        gl_waterOn;

static tGlProgram gl_waterProgram;
static GLint      gl_waterViewProj = -1;
static GLint      gl_waterView = -1;
static GLint      gl_waterTexMtx = -1;
static GLint      gl_waterSampler = -1;
static GLint      gl_waterFlipV = -1;
static GLint      gl_waterFogCurve = -1;
static GLint      gl_waterFogStart = -1;
static GLint      gl_waterFogEnd = -1;
static GLint      gl_waterFogDensity = -1;
static GLint      gl_waterFogColor = -1;
static GLuint     gl_waterVao;
static GLuint     gl_waterVbo;
static GLuint     gl_waterIbo;

/* The grid the index strip was last built for. The strip depends on
   nothing but the two counts, and those change when the world does
   rather than when the water moves, so it is rebuilt when they move
   and left alone otherwise. */
static unsigned int gl_waterCols;
static int          gl_waterRows;
static int          gl_waterIndices;

/* The vertex scratch, grown rather than reallocated per frame. */
static tWms_Vertex* gl_waterVerts;
static int          gl_waterVertMax;

static void GL_l_ForgetWater(void)
{
    if (gl_state.context)
    {
        if (gl_waterFbo)
            GLFN_glDeleteFramebuffers(1, &gl_waterFbo);
        if (gl_waterColour)
            GLFN_glDeleteTextures(1, &gl_waterColour);
        if (gl_waterDepth)
            GLFN_glDeleteRenderbuffers(1, &gl_waterDepth);
        if (gl_waterVbo)
            GLFN_glDeleteBuffers(1, &gl_waterVbo);
        if (gl_waterIbo)
            GLFN_glDeleteBuffers(1, &gl_waterIbo);
        if (gl_waterVao)
            GLFN_glDeleteVertexArrays(1, &gl_waterVao);
        GL_ProgramFree(&gl_waterProgram);
    }
    gl_waterFbo = 0;
    gl_waterColour = 0;
    gl_waterDepth = 0;
    gl_waterVbo = 0;
    gl_waterIbo = 0;
    gl_waterVao = 0;
    gl_waterViewProj = -1;
    gl_waterView = -1;
    gl_waterTexMtx = -1;
    gl_waterSampler = -1;
    gl_waterFlipV = -1;
    gl_waterFogCurve = -1;
    gl_waterFogStart = -1;
    gl_waterFogEnd = -1;
    gl_waterFogDensity = -1;
    gl_waterFogColor = -1;
    gl_waterTried = 0;
    gl_waterReady = 0;
    gl_waterOn = 0;
    memset(&gl_waterCamMtx, 0, sizeof(gl_waterCamMtx));
    gl_waterCols = 0;
    gl_waterRows = 0;
    gl_waterIndices = 0;
    free(gl_waterVerts);
    gl_waterVerts = NULL;
    gl_waterVertMax = 0;
}

/* Builds the reflection target, the program and the buffers, once.
   Asked and answered: a machine that cannot give one of them is not
   asked again, and gets no water rather than a failed draw a frame. */
static int GL_l_b_WaterReady(void)
{
    if (gl_waterTried)
        return gl_waterReady;
    gl_waterTried = 1;
    if (!gl_state.context)
        return 0;

    GLFN_glGenTextures(1, &gl_waterColour);
    GLFN_glGenRenderbuffers(1, &gl_waterDepth);
    GLFN_glGenFramebuffers(1, &gl_waterFbo);
    if (!gl_waterColour || !gl_waterDepth || !gl_waterFbo)
    {
        DBG_OutputDebugString("GL: no reflection target for the water\n");
        GL_l_ForgetWater();
        gl_waterTried = 1;
        return 0;
    }

    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GL_WATER_SIZE,
                      GL_WATER_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    /* One level, and a sampler that wants no more: the other renderer
       makes this with a single level too, and asking for mip filtering
       on it would make the texture incomplete. Linear and clamped on
       both axes is what that renderer sets by hand for this draw. */
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, gl_waterDepth);
    GLFN_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                               GL_WATER_SIZE, GL_WATER_SIZE);
    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_waterFbo);
    GLFN_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, gl_waterColour, 0);
    GLFN_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_RENDERBUFFER, gl_waterDepth);
    if (GLFN_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
        DBG_OutputDebugString("GL: the water's reflection target would not build\n");
        GL_l_ForgetWater();
        gl_waterTried = 1;
        return 0;
    }
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!GL_b_ProgramLoad(&gl_waterProgram, "water.vert", "water.frag"))
    {
        GL_l_ForgetWater();
        gl_waterTried = 1;
        return 0;
    }
    gl_waterViewProj = GL_i_ProgramUniform(&gl_waterProgram, "uViewProj");
    gl_waterView = GL_i_ProgramUniform(&gl_waterProgram, "uView");
    gl_waterTexMtx = GL_i_ProgramUniform(&gl_waterProgram, "uTexMtx");
    gl_waterSampler = GL_i_ProgramUniform(&gl_waterProgram, "uReflection");
    gl_waterFlipV = GL_i_ProgramUniform(&gl_waterProgram, "uFlipV");
    gl_waterFogCurve = GL_i_ProgramUniform(&gl_waterProgram, "uFogCurve");
    gl_waterFogStart = GL_i_ProgramUniform(&gl_waterProgram, "uFogStart");
    gl_waterFogEnd = GL_i_ProgramUniform(&gl_waterProgram, "uFogEnd");
    gl_waterFogDensity = GL_i_ProgramUniform(&gl_waterProgram, "uFogDensity");
    gl_waterFogColor = GL_i_ProgramUniform(&gl_waterProgram, "uFogColor");

    GLFN_glGenVertexArrays(1, &gl_waterVao);
    GLFN_glGenBuffers(1, &gl_waterVbo);
    GLFN_glGenBuffers(1, &gl_waterIbo);
    if (!gl_waterVao || !gl_waterVbo || !gl_waterIbo)
    {
        DBG_OutputDebugString("GL: no buffers for the water surface\n");
        GL_l_ForgetWater();
        gl_waterTried = 1;
        return 0;
    }

    GLFN_glBindVertexArray(gl_waterVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_waterVbo);
    GLFN_glEnableVertexAttribArray(0);
    GLFN_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tWms_Vertex),
                               (const void*)(size_t)0);
    GLFN_glEnableVertexAttribArray(1);
    GLFN_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                               (GLsizei)sizeof(tWms_Vertex),
                               (const void*)(size_t)12);
    GLFN_glEnableVertexAttribArray(2);
    GLFN_glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                               (GLsizei)sizeof(tWms_Vertex),
                               (const void*)(size_t)24);
    /* The index buffer is part of the vertex array's state, so it is
       bound here once rather than per draw. */
    GLFN_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_waterIbo);
    GLFN_glBindVertexArray(0);

    gl_waterReady = 1;
    return 1;
}

/* The index strip, rebuilt when the grid changes size and otherwise
   left where it is. Returns how many indices it holds. */
static int GL_l_i_WaterStrip(unsigned int columns, int rows)
{
    unsigned short* strip;
    int             want;

    if (columns == gl_waterCols && rows == gl_waterRows)
        return gl_waterIndices;

    gl_waterCols = columns;
    gl_waterRows = rows;
    gl_waterIndices = 0;

    want = WMS_i_IndexCount(columns, rows);
    if (want <= 0)
        return 0;

    strip = (unsigned short*)malloc((size_t)want * sizeof(*strip));
    if (!strip)
        return 0;
    if (WMS_i_StripIndices(columns, rows, strip, want) != want)
    {
        /* Below two columns the strip writes fewer indices than the
           count claims -- watermesh.h says so, and the simulation
           builds no grid that small. Drawing the difference would be
           drawing whatever the buffer held. */
        free(strip);
        return 0;
    }

    GLFN_glBindVertexArray(gl_waterVao);
    GLFN_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_waterIbo);
    GLFN_glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      (GLsizei)((size_t)want * sizeof(*strip)), strip,
                      GL_STATIC_DRAW);
    GLFN_glBindVertexArray(0);
    free(strip);

    gl_waterIndices = want;
    return want;
}

void GL_PrepareWaterRendering(void)
{
    unsigned int bottom;

    /* Asked once a frame, and the answer is what the rest of the
       frame goes by. Cleared first so a frame that gets no further
       than here has no water rather than the last frame's. */
    gl_waterOn = 0;
    if (!gl_state.context || !pst_GlobalsWaterParams)
        return;
    if (!GL_l_b_WaterReady())
        return;

    bottom = (unsigned int)pst_GlobalsWaterParams->waterEngine.bottomColor;

    /* The colour the water is looking down into. The reflection is
       drawn over this where there is anything to reflect, and until
       that pass exists this is the whole of what the surface samples.
       A8R8G8B8, so red is the second byte down. */
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_waterFbo);
    GLFN_glViewport(0, 0, GL_WATER_SIZE, GL_WATER_SIZE);
    GL_l_SetColorMask(1, 1, 1, 1);
    GLFN_glDepthMask(GL_TRUE);
    GLFN_glClearDepth(1.0);
    GL_l_ClearColour((GLfloat)((bottom >> 16) & 0xFF) / 255.0f,
                     (GLfloat)((bottom >> 8) & 0xFF) / 255.0f,
                     (GLfloat)(bottom & 0xFF) / 255.0f,
                     (GLfloat)((bottom >> 24) & 0xFF) / 255.0f,
                     GL_DEPTH_BUFFER_BIT);

    /* The camera as it stands now, which every mirrored object this
       frame is measured against. Kept here rather than read per
       object: the engine moves the camera while the world is drawn,
       and a reflection built against a moving camera smears. */
    MATH_MakeOGLMatrix(&gl_waterCamMtx, &gpst_DX_stDD->ct.cam.cameraMtx);

    /* Back where the world is drawn. The scene target is bound rather
       than the window, because that is what the frame is being built
       in -- the after-effects read it afterwards. */
    GL_l_BindScene();
    gl_waterOn = 1;
}

void GL_DrawWater(struct sWaterParamsGlobalStruct* water)
{
    Mtx4x4WithoutAdditionalData texMtx;
    Mtx4x4                      camInv;
    Mtx4x4                      camView;
    Mtx4x4                      camViewProj;
    tWms_Source                 src;
    int                         indices;
    int                         verts;
    int                         want;

    if (!gl_state.context || !water || !gl_waterOn)
        return;
    if (!gl_waterReady)
        return;

    want = (int)water->horizontalVertexNum * water->verticallVertexNum;
    if (want <= 0)
        return;

    indices = GL_l_i_WaterStrip(water->horizontalVertexNum,
                                water->verticallVertexNum);
    if (indices <= 0)
        return;

    if (want > gl_waterVertMax)
    {
        tWms_Vertex* grown = (tWms_Vertex*)realloc(gl_waterVerts,
                                                   (size_t)want * sizeof(*grown));

        if (!grown)
            return;
        gl_waterVerts = grown;
        gl_waterVertMax = want;
    }

    /* The blur over the wave array runs before anything reads it, the
       way the other renderer runs it: it rewrites the array in place,
       so it cannot be folded into the shared vertex build, which takes
       the array as it finds it. */
    if (tweak_water_blur_normal)
        WTR_WaterBlurNormal(water->vecsForUVs, water->horizontalVertexNum,
                            water->verticallVertexNum);

    src.positions = water->vertices;
    src.waves = water->vecsForUVs;
    src.colors = water->colors;
    src.columns = water->horizontalVertexNum;
    src.rows = water->verticallVertexNum;
    src.camera = gpst_DX_stDD->ct.cam.objectMtx.Pos;
    verts = WMS_i_Vertices(&src, gl_waterVerts, gl_waterVertMax);
    if (verts <= 0)
        return;

    WMS_ProjectionToTexture(&texMtx,
                            (const Mtx4x4WithoutAdditionalData*)&gl_projMtx,
                            GL_WATER_SIZE, GL_WATER_SIZE);

    /* The camera's own view, built here rather than taken from the
     * engine -- and this is the whole of why the water used to be
     * somewhere else.
     *
     * While the world is drawn, what the engine calls the view matrix
     * is the top of its matrix stack: the camera with the object being
     * drawn folded into it. It sets that per object and nothing puts
     * the camera back before the water, so taking the engine's view
     * here draws the surface through whatever object happened to be
     * drawn last -- usually off the screen entirely, and when it does
     * land, turned.
     *
     * The other renderer sets both transforms itself for this draw:
     * the view from the inverse of the camera's own matrix, the world
     * to identity. This is that, and the three things the shader
     * builds from these matrices -- the position, the reflection's
     * texture coordinate and the fog depth -- are all wrong together
     * when it is not done.
     *
     * The inverse is taken here rather than read from the camera's
     * kept one, because that is what the other renderer does; the two
     * agree, and taking it is cheap once a frame. */
    camInv = gpst_DX_stDD->ct.cam.objectMtx;
    MATH_InvertMatrix(&camInv, &gpst_DX_stDD->ct.cam.objectMtx);
    /* The scale flags, cleared as that renderer clears them before
       handing the matrix over. */
    camInv._m14 = 0.0f;
    camInv._m24 = 0.0f;
    camInv._m34 = 0.0f;
    camInv.w = 1.0f;
    MATH_MakeOGLMatrix(&camView, &camInv);
    D3D_MatrixMultiply((D3DMATRIX*)&camViewProj,
                       (const D3DMATRIX*)&camView,
                       (const D3DMATRIX*)&gl_projMtx);

    /* The reflection is blurred before it is sampled, which is where
       the other renderer does it too -- in the function that hands the
       reflection over to this draw. It writes back into the reflection
       itself, so there is nothing to pass along.
     *
     * Before this draw's program is made current, and that order is
     * forced: the blur binds a program of its own, and a uniform set
     * while one program is current belongs to that program. Called
     * after the uniforms below, it replaced the program they had just
     * been set on and the surface drew with the blur's -- which is
     * nothing at all. */
    GL_l_SmoothWater();

    GLFN_glUseProgram(gl_waterProgram.program);
    if (gl_waterViewProj >= 0)
        GLFN_glUniformMatrix4fv(gl_waterViewProj, 1, GL_FALSE,
                                (const GLfloat*)&camViewProj);
    if (gl_waterView >= 0)
        GLFN_glUniformMatrix4fv(gl_waterView, 1, GL_FALSE,
                                (const GLfloat*)&camView);
    if (gl_waterTexMtx >= 0)
        GLFN_glUniformMatrix4fv(gl_waterTexMtx, 1, GL_FALSE,
                                (const GLfloat*)&texMtx);
    if (gl_waterSampler >= 0)
        GLFN_glUniform1i(gl_waterSampler, 0);
    if (gl_waterFlipV >= 0)
        GLFN_glUniform1i(gl_waterFlipV, tweak_gl_water_flip ? 1 : 0);

    /* Fogged, as that renderer turns fog on for this draw by hand. */
    if (gl_waterFogCurve >= 0)
        GLFN_glUniform1i(gl_waterFogCurve, gl_fog.curve);
    if (gl_fog.curve != FOG_OFF)
    {
        unsigned int c = FOG_i_Color(&gl_fog, gl_fogSecondColor);

        if (gl_waterFogStart >= 0)
            GLFN_glUniform1f(gl_waterFogStart, gl_fog.start);
        if (gl_waterFogEnd >= 0)
            GLFN_glUniform1f(gl_waterFogEnd, gl_fog.end);
        if (gl_waterFogDensity >= 0)
            GLFN_glUniform1f(gl_waterFogDensity, gl_fog.density);
        if (gl_waterFogColor >= 0)
            GLFN_glUniform3f(gl_waterFogColor,
                             (GLfloat)((c >> 16) & 0xFF) / 255.0f,
                             (GLfloat)((c >> 8) & 0xFF) / 255.0f,
                             (GLfloat)(c & 0xFF) / 255.0f);
    }

    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);

    /* Tested against the world's depth and not written into it, which
       is what lets the surface be hidden by what is in front of it
       without hiding the rest of itself. */
    GLFN_glEnable(GL_DEPTH_TEST);
    GLFN_glDepthFunc(GL_LEQUAL);
    GLFN_glDepthMask(GL_FALSE);

    /* Not blended. The mixing is the shader's, and the result replaces
       the pixel -- see water.frag. */
    GLFN_glDisable(GL_BLEND);
    GL_l_SetColorMask(1, 1, 1, 1);

    /* Both sides.
     *
     * The other renderer sets no culling for this draw at all: it
     * inherits whatever the last object drawn left in force, which is
     * not a specification of anything. Drawing the surface from both
     * sides is the one reading that comes out the same whatever was
     * inherited, and it is the safe way round -- the wrong guess at a
     * facing is an invisible surface, which is the defect this
     * renderer has already shipped once. */
    GLFN_glDisable(GL_CULL_FACE);

    GLFN_glBindVertexArray(gl_waterVao);
    GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_waterVbo);
    GLFN_glBufferData(GL_ARRAY_BUFFER,
                      (GLsizei)((size_t)verts * sizeof(tWms_Vertex)),
                      gl_waterVerts, GL_STREAM_DRAW);
    GLFN_glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)indices,
                        GL_UNSIGNED_SHORT, (const void*)(size_t)0);
    GLFN_glBindVertexArray(0);

    /* Depth writing back on: everything after this draw expects it,
       and the other renderer puts it back here too. */
    GLFN_glDepthMask(GL_TRUE);
}

/* ------------------------------------------------- the reflection
 *
 * The water's first pass, and the one that makes the surface mirror
 * the world rather than a flat colour.
 *
 * It is not a pass of its own in the usual sense: there is no loop over
 * the world anywhere here. Each object, as it is drawn, is drawn a
 * **second time** into the reflection target with its transform
 * mirrored in the water plane -- so the pass is a tail on the ordinary
 * draw, which is where the other renderer puts it too.
 *
 * Three things have to change for that second draw and change back
 * afterwards:
 *
 *   the target     the reflection's, at its own size
 *   the matrices   the mirrored object transform against the camera
 *                  alone, rather than the combined matrix the engine
 *                  hands over for the ordinary draw
 *   the winding    reversed, because mirroring turns every triangle
 *                  over
 *
 * and a clip plane keeps the result under the water line.
 *
 * The mirrored transform, the size threshold and the plane are in
 * waterreflect.c, shared with the other renderer and compared against
 * it; what is here is the four GL calls that aim the draw.
 */

/* What the second draw goes back to.
 *
 * The first two are **worked out** rather than read back: the target
 * the world is being drawn into is the scene's and its size is the
 * screen's, and there is no moment during the world's drawing when
 * either is anything else -- the one case that binds something else,
 * a shadow, is refused above. Asking the driver instead would be a
 * round trip per object for an answer the renderer already knows. The
 * names say "back to" and not "was" for that reason.
 *
 * The winding is genuinely the one that was in force, because the
 * renderer tracks it. */
static GLuint gl_waterBackToFbo;
static GLint  gl_waterBackToViewport[4];
static int    gl_waterWasCulling;
static GLenum gl_waterWasFrontFace;

/* Whether the second draw is wanted for the object being drawn now.
 *
 * The gates are the other renderer's, in its order. Two of them sit at
 * its call site rather than in the function, and they are the two most
 * easily lost: a material whose texture coordinates are generated two
 * particular ways is skipped, and so is anything already being drawn
 * into a target of its own -- a shadow, say, which has no business
 * putting itself in the water. */
static int GL_l_b_WantsReflection(void)
{
    const tGameObjectBV* bv;

    if (!gl_waterOn || !gl_waterReady)
        return 0;
    if (GDI_gi_TextureUVMode == 4 || GDI_gi_TextureUVMode == 5)
        return 0;
    if (gl_targetInForce)
        return 0;
    if (!gpst_DX_stDD || !gpst_DX_stDD->currTargetGao
        || !gpst_DX_stDD->currTargetGao->matrices)
        return 0;

    bv = gpst_DX_stDD->currTargetGao->boundingVolume;
    if (!bv)
        return 0;
    return WRF_b_WorthReflecting(bv, OBJ_BV_IsSphere((tGameObjectBV*)bv),
                                 tweak_water_min_bv);
}

/* Aims the draw at the reflection, leaving the program bound and the
   vertex data where it is -- the whole point is to draw the same
   triangles again. */
static void GL_l_AimAtReflection(void)
{
    Mtx4x4 mirrored;
    Mtx4x4 view;
    Mtx4x4 viewProj;

    gl_waterBackToFbo = (GLuint)GL_l_i_SceneFbo();
    gl_waterBackToViewport[0] = 0;
    gl_waterBackToViewport[1] = 0;
    gl_waterBackToViewport[2] = gl_state.width;
    gl_waterBackToViewport[3] = gl_state.height;
    gl_waterWasCulling = gl_cullOn;
    gl_waterWasFrontFace = gl_frontFace;

    WRF_Mirror(&mirrored, gpst_DX_stDD->currTargetGao->matrices,
               waterHeight);
    /* The camera alone, not the view the engine set. That matrix has
       the object folded into it already, so using it would apply the
       object's transform twice -- once unmirrored. */
    D3D_MatrixMultiply((D3DMATRIX*)&view, (const D3DMATRIX*)&mirrored,
                       (const D3DMATRIX*)&gl_waterCamMtx);
    D3D_MatrixMultiply((D3DMATRIX*)&viewProj, (const D3DMATRIX*)&view,
                       (const D3DMATRIX*)&gl_projMtx);

    /* The target is square and the screen is not, and the projection
       is the screen's -- so the reflection is drawn with the wrong
       aspect ratio and the surface samples it that way. That is the
       other renderer's behaviour rather than an oversight here: it
       sets the render target without setting a viewport, and in that
       interface setting a render target resets the viewport to the
       whole of it. Reproduced, because the surface's own projection
       was built against it. */
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_waterFbo);
    GLFN_glViewport(0, 0, GL_WATER_SIZE, GL_WATER_SIZE);

    if (gl_geoViewProj >= 0)
        GLFN_glUniformMatrix4fv(gl_geoViewProj, 1, GL_FALSE,
                                (const GLfloat*)&viewProj);
    /* The fog goes with it: its depth is measured from the camera, and
       for this draw that is the mirrored object against the camera. */
    if (gl_geoView >= 0)
        GLFN_glUniformMatrix4fv(gl_geoView, 1, GL_FALSE,
                                (const GLfloat*)&view);

    if (tweak_water_clip)
    {
        float plane[4];

        WRF_ClipPlane(plane, waterHeight, tweak_water_off);
        if (gl_geoWorld >= 0)
            GLFN_glUniformMatrix4fv(gl_geoWorld, 1, GL_FALSE,
                                    (const GLfloat*)&mirrored);
        if (gl_geoClipPlane >= 0)
            GLFN_glUniform4f(gl_geoClipPlane, plane[0], plane[1],
                             plane[2], plane[3]);
        if (gl_geoClipping >= 0)
            GLFN_glUniform1i(gl_geoClipping, 1);
        GLFN_glEnable(GL_CLIP_DISTANCE0);
    }

    /* Mirroring turns every triangle over, so the facing has to turn
       with it or the reflection shows the insides of things. Reversed
       rather than switched off: the other renderer swaps its two cull
       modes here, and leaving culling off instead would draw back faces
       the original never draws. */
    if (gl_waterWasCulling)
        GLFN_glFrontFace(gl_frontFace == GL_CCW ? GL_CW : GL_CCW);
}

/* And puts all four back. */
static void GL_l_LeaveReflection(void)
{
    if (tweak_water_clip)
    {
        GLFN_glDisable(GL_CLIP_DISTANCE0);
        if (gl_geoClipping >= 0)
            GLFN_glUniform1i(gl_geoClipping, 0);
    }
    if (gl_waterWasCulling)
        GLFN_glFrontFace(gl_waterWasFrontFace);

    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_waterBackToFbo);
    GLFN_glViewport(gl_waterBackToViewport[0], gl_waterBackToViewport[1],
                    gl_waterBackToViewport[2], gl_waterBackToViewport[3]);

    /* The matrices the ordinary draw was using. They are the
       renderer's own, so they are put back from it rather than saved
       and restored. */
    if (gl_geoViewProj >= 0)
        GLFN_glUniformMatrix4fv(gl_geoViewProj, 1, GL_FALSE,
                                (const GLfloat*)&gl_viewProjMtx);
    if (gl_geoView >= 0)
        GLFN_glUniformMatrix4fv(gl_geoView, 1, GL_FALSE,
                                (const GLfloat*)&gl_viewMtx);
}

/* Draws the triangles already in the buffer a second time, into the
   reflection. The vertex data is left where it is -- that is the whole
   point -- so this binds the vertex array again and draws, with
   nothing uploaded. */
static void GL_l_ReflectStaged(int count)
{
    if (count <= 0)
        return;
    GL_l_AimAtReflection();
    GLFN_glBindVertexArray(gl_geoVao);
    GLFN_glDrawArrays(GL_TRIANGLES, 0, (GLsizei)count);
    GLFN_glBindVertexArray(0);
    GL_l_LeaveReflection();
}

/* ------------------------------------------ the surface smoothing
 *
 * The third and last of the water's pieces: before the surface samples
 * the reflection, the reflection is blurred -- out into a scratch
 * target and back again, so the result ends up where it started and
 * the surface needs to know nothing about it.
 *
 * Three things about it, all read out of the other renderer rather
 * than chosen here, and all in watersmooth.h at more length:
 *
 *   - it is **off** unless somebody turns it on, because the two
 *     amounts are debug tweaks that start at nought and then every tap
 *     lands in the same place;
 *   - the other renderer's two-texture-unit arm **cannot run**, since
 *     the only thing that calls the blur requires four units first, so
 *     only the four-tap arithmetic is here;
 *   - the vertical tap offsets are nought at the top of the rectangle
 *     and three steps at the bottom, interpolated between, so the blur
 *     opens out towards the near edge of the water. That is why the
 *     taps are computed per vertex -- see watersmooth.vert.
 */

static GLuint     gl_wsmFbo;
static GLuint     gl_wsmColour;
static int        gl_wsmTried;
static int        gl_wsmReady;

static tGlProgram gl_wsmProgram;
static GLint      gl_wsmSrcU = -1;
static GLint      gl_wsmSrcV = -1;
static GLint      gl_wsmUStep = -1;
static GLint      gl_wsmVStep = -1;
static GLint      gl_wsmSampler = -1;
static GLint      gl_wsmWeights = -1;
static GLuint     gl_wsmVao;
static GLuint     gl_wsmVbo;

static void GL_l_ForgetWaterSmooth(void)
{
    if (gl_state.context)
    {
        if (gl_wsmFbo)
            GLFN_glDeleteFramebuffers(1, &gl_wsmFbo);
        if (gl_wsmColour)
            GLFN_glDeleteTextures(1, &gl_wsmColour);
        if (gl_wsmVbo)
            GLFN_glDeleteBuffers(1, &gl_wsmVbo);
        if (gl_wsmVao)
            GLFN_glDeleteVertexArrays(1, &gl_wsmVao);
        GL_ProgramFree(&gl_wsmProgram);
    }
    gl_wsmFbo = 0;
    gl_wsmColour = 0;
    gl_wsmVbo = 0;
    gl_wsmVao = 0;
    gl_wsmSrcU = -1;
    gl_wsmSrcV = -1;
    gl_wsmUStep = -1;
    gl_wsmVStep = -1;
    gl_wsmSampler = -1;
    gl_wsmWeights = -1;
    gl_wsmTried = 0;
    gl_wsmReady = 0;
}

static int GL_l_b_WaterSmoothReady(void)
{
    if (gl_wsmTried)
        return gl_wsmReady;
    gl_wsmTried = 1;
    if (!gl_state.context)
        return 0;

    /* The scratch is the reflection's size rather than the screen's.
       The other renderer borrows a screen-sized target it already has
       and so writes into a 512-square window centred in it; the region
       sampled is the same either way, and the shared code works out
       the rectangle for whichever size it is given. Square is simply
       the smaller allocation. */
    GLFN_glGenTextures(1, &gl_wsmColour);
    GLFN_glGenFramebuffers(1, &gl_wsmFbo);
    if (!gl_wsmColour || !gl_wsmFbo)
    {
        DBG_OutputDebugString("GL: no scratch target for the water smoothing\n");
        GL_l_ForgetWaterSmooth();
        gl_wsmTried = 1;
        return 0;
    }

    GLFN_glBindTexture(GL_TEXTURE_2D, gl_wsmColour);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GL_WATER_SIZE,
                      GL_WATER_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* Mirrored, which is what the other renderer sets for the blur's
       own samplers -- a tap that steps off the edge comes back inside
       rather than smearing the edge pixel. The surface draw wants
       clamping instead, so the reflection's own wrap mode is moved
       back and forth around the blur below. */
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, gl_wsmFbo);
    GLFN_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, gl_wsmColour, 0);
    if (GLFN_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);
        DBG_OutputDebugString("GL: the water's scratch target would not build\n");
        GL_l_ForgetWaterSmooth();
        gl_wsmTried = 1;
        return 0;
    }
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!GL_b_ProgramLoad(&gl_wsmProgram, "watersmooth.vert",
                          "watersmooth.frag"))
    {
        GL_l_ForgetWaterSmooth();
        gl_wsmTried = 1;
        return 0;
    }
    gl_wsmSrcU = GL_i_ProgramUniform(&gl_wsmProgram, "uSrcU");
    gl_wsmSrcV = GL_i_ProgramUniform(&gl_wsmProgram, "uSrcV");
    gl_wsmUStep = GL_i_ProgramUniform(&gl_wsmProgram, "uStep");
    gl_wsmVStep = GL_i_ProgramUniform(&gl_wsmProgram, "vStep");
    gl_wsmSampler = GL_i_ProgramUniform(&gl_wsmProgram, "uSrc");
    gl_wsmWeights = GL_i_ProgramUniform(&gl_wsmProgram, "uWeights");

    GLFN_glGenVertexArrays(1, &gl_wsmVao);
    GLFN_glGenBuffers(1, &gl_wsmVbo);
    if (!gl_wsmVao || !gl_wsmVbo)
    {
        DBG_OutputDebugString("GL: no buffers for the water smoothing\n");
        GL_l_ForgetWaterSmooth();
        gl_wsmTried = 1;
        return 0;
    }

    /* The quad, once: a clip-space rectangle with each corner saying
       where it is in the rectangle -- x nought at the left and one at
       the right, y nought at the top and one at the bottom. The corner
       is what the vertical ramp is a function of, and it never
       changes, so the whole thing is uploaded here and not per pass. */
    {
        static const GLfloat quad[] = {
            /*  x      y     cornerX cornerY */
            -1.0f, -1.0f,   0.0f, 1.0f,   /* bottom left  */
             1.0f, -1.0f,   1.0f, 1.0f,   /* bottom right */
            -1.0f,  1.0f,   0.0f, 0.0f,   /* top left     */
             1.0f,  1.0f,   1.0f, 0.0f    /* top right    */
        };

        GLFN_glBindVertexArray(gl_wsmVao);
        GLFN_glBindBuffer(GL_ARRAY_BUFFER, gl_wsmVbo);
        GLFN_glBufferData(GL_ARRAY_BUFFER, (GLsizei)sizeof(quad), quad,
                          GL_STATIC_DRAW);
        GLFN_glEnableVertexAttribArray(0);
        GLFN_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                                   (GLsizei)(4 * sizeof(GLfloat)),
                                   (const void*)(size_t)0);
        GLFN_glEnableVertexAttribArray(1);
        GLFN_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                                   (GLsizei)(4 * sizeof(GLfloat)),
                                   (const void*)(size_t)(2 * sizeof(GLfloat)));
        GLFN_glBindVertexArray(0);
    }

    gl_wsmReady = 1;
    return 1;
}

/* One pass: everything in `from` read, everything in `to` written. */
static void GL_l_WaterSmoothPass(const tWsm_Rect* from, GLuint srcTex,
                                 GLuint dstFbo, const tWsm_Rect* to,
                                 const tWsm_Tap* taps, float uStep,
                                 float vStep)
{
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, dstFbo);
    GLFN_glViewport(to->firstColumn, to->firstRow,
                    to->lastColumn - to->firstColumn + 1,
                    to->lastRow - to->firstRow + 1);

    GLFN_glUseProgram(gl_wsmProgram.program);
    if (gl_wsmSrcU >= 0)
        GLFN_glUniform2f(gl_wsmSrcU,
                         (GLfloat)from->firstColumn / (GLfloat)(from->width - 1),
                         (GLfloat)from->lastColumn / (GLfloat)(from->width - 1));
    /* The quad's top edge first, then its bottom -- and in this
       interface a target's first row is the bottom of the picture, so
       the quad's bottom is the rectangle's **first** row. Passed the
       other way round, each pass wrote a vertically mirrored copy;
       two passes cancelled it, so the finished picture looked right
       and only the vertical ramp was at the wrong end. */
    if (gl_wsmSrcV >= 0)
        GLFN_glUniform2f(gl_wsmSrcV,
                         (GLfloat)from->lastRow / (GLfloat)(from->height - 1),
                         (GLfloat)from->firstRow / (GLfloat)(from->height - 1));
    if (gl_wsmUStep >= 0)
        GLFN_glUniform1f(gl_wsmUStep, uStep);
    if (gl_wsmVStep >= 0)
        GLFN_glUniform1f(gl_wsmVStep, vStep);
    if (gl_wsmSampler >= 0)
        GLFN_glUniform1i(gl_wsmSampler, 0);
    if (gl_wsmWeights >= 0)
        GLFN_glUniform4f(gl_wsmWeights, taps[0].weight, taps[1].weight,
                         taps[2].weight, taps[3].weight);

    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, srcTex);

    GLFN_glDisable(GL_DEPTH_TEST);
    GLFN_glDepthMask(GL_FALSE);
    GLFN_glDisable(GL_BLEND);
    GLFN_glDisable(GL_CULL_FACE);
    GL_l_SetColorMask(1, 1, 1, 1);

    GLFN_glBindVertexArray(gl_wsmVao);
    GLFN_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GLFN_glBindVertexArray(0);
}

/* Blurs the reflection in place, by going out to the scratch and back.
 *
 * Called before the surface samples the reflection, which is where the
 * other renderer does it too -- from the same function that hands the
 * reflection over. */
static void GL_l_SmoothWater(void)
{
    tWsm_Pass passes[2];
    tWsm_Tap  taps[WSM_TAPS];
    float     uStep;
    float     vStep;
    GLint     wasFbo = 0;
    GLint     wasViewport[4];

    /* The reflection check is belt and braces: the one caller has
       already returned if the reflection is not there, so breaking
       this half of the test changes nothing and a sweep cannot kill
       it. Kept because the function should not assume its caller, and
       noted so that the surviving mutant is not mistaken for a gap. */
    if (!tweak_water_smooth || !gl_waterReady)
        return;

    /* Asked for rather than worked out, unlike the reflection's.
     *
     * The reflection runs inside the per-object draw, where the
     * renderer knows what is bound because it bound it; this runs
     * where the engine calls in to draw the surface, and the only
     * honest answer to "what was bound" is the driver's. It is one
     * query a frame rather than one an object, which is what makes
     * that affordable -- and the alternative was wrong: the first
     * version put the frame back by assuming, and the surface drew
     * itself into the reflection at the reflection's size. */
    GLFN_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &wasFbo);
    wasViewport[0] = 0;
    wasViewport[1] = 0;
    wasViewport[2] = gl_state.width;
    wasViewport[3] = gl_state.height;
    GLFN_glGetIntegerv(GL_VIEWPORT, wasViewport);

    /* **Inside** the saving, not before it. The targets and the
       program are built on first use, and that build binds a
       framebuffer of its own and leaves the default one bound when it
       is done -- so asking what was bound afterwards gets nought, and
       the frame is "restored" to the window. The surface then drew
       into the window instead of the frame and vanished, on the first
       frame only, which is the kind of thing that looks like a driver
       problem. */
    if (!GL_l_b_WaterSmoothReady())
    {
        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)wasFbo);
        GLFN_glViewport(wasViewport[0], wasViewport[1], wasViewport[2],
                        wasViewport[3]);
        return;
    }

    WSM_Passes(passes, GL_WATER_SIZE, GL_WATER_SIZE, GL_WATER_SIZE);

    /* A step each way, from the amounts the console holds. Both start
       at nought, and then every tap lands in the same place and the
       two passes below are two copies -- which is the default, and is
       why turning the smoothing on changes nothing until the amounts
       are set as well. */
    uStep = WSM_f_Step(tweak_water_smoothx, passes[0].from.width - 1);
    vStep = WSM_f_Step(tweak_water_smoothy, passes[0].from.height - 1);
    WSM_Taps(taps, uStep, vStep);

    /* The reflection is sampled mirrored for the blur and clamped for
       the surface, so its wrap mode moves either side of the two
       passes. */
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    GL_l_WaterSmoothPass(&passes[0].from, gl_waterColour, gl_wsmFbo,
                         &passes[0].to, taps, uStep, vStep);
    GL_l_WaterSmoothPass(&passes[1].from, gl_wsmColour, gl_waterFbo,
                         &passes[1].to, taps, uStep, vStep);

    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    /* And the frame back, exactly as it was found. */
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)wasFbo);
    GLFN_glViewport(wasViewport[0], wasViewport[1], wasViewport[2],
                    wasViewport[3]);
}

/* ---------------------------------------------- the video display
 *
 * A movie-sized picture the engine writes decoded frames into, and
 * the renderer fits it to the screen. The engine knows about files,
 * decoding and the clock; it does not know what a surface is.
 *
 * The other renderer keeps its picture in system memory so the
 * decoder can lock it, copies that into a render-target texture and
 * blits the texture to the back buffer with a stretch. Here the
 * picture is plain memory, the copy is an upload, and the stretch is
 * a textured quad -- which is the same three steps with the parts
 * OpenGL names differently.
 */

static unsigned char* gl_videoPixels;
static GLuint         gl_videoTex;
static int            gl_videoWidth;
static int            gl_videoHeight;

/* The screen as it was when the display was made, which is what the
   fit is computed against. The other renderer takes it from the back
   buffer at the same moment and for the same reason: a frame is
   fitted to the screen the movie started on. */
static int            gl_videoScreenW;
static int            gl_videoScreenH;

static tGlProgram     gl_videoProgram;
static GLint          gl_videoViewport = -1;
static GLint          gl_videoSampler = -1;
static int            gl_videoTried;
static int            gl_videoReady;

void GL_VideoDisplayClose(void)
{
    if (gl_videoTex && gl_state.context)
        GLFN_glDeleteTextures(1, &gl_videoTex);
    gl_videoTex = 0;
    free(gl_videoPixels);
    gl_videoPixels = NULL;
    gl_videoWidth = 0;
    gl_videoHeight = 0;
}

static void GL_l_ForgetVideo(void)
{
    GL_VideoDisplayClose();
    if (gl_videoReady && gl_state.context)
        GL_ProgramFree(&gl_videoProgram);
    gl_videoViewport = -1;
    gl_videoSampler = -1;
    gl_videoTried = 0;
    gl_videoReady = 0;
}

static int GL_l_b_VideoProgramReady(void)
{
    if (gl_videoTried)
        return gl_videoReady;
    gl_videoTried = 1;
    if (!gl_state.context)
        return 0;

    /* The 2D pass first, because what is borrowed from it is not only
       the vertex stage: the vertex array that describes this layout
       and the buffer the vertices are staged in are the pass's, and
       without them the draw is a draw of nothing. Asking for them
       here rather than assuming them is the difference between a
       movie and a black screen. */
    if (!GL_l_b_PassReady())
        return 0;

    /* The glyphs' vertex stage, which already maps pixel coordinates
       to clip space and carries a texture coordinate. Only the
       fragment stage differs. */
    if (!GL_b_ProgramLoad(&gl_videoProgram, "overlaytext.vert", "video.frag"))
        return 0;
    gl_videoViewport = GL_i_ProgramUniform(&gl_videoProgram, "uViewport");
    gl_videoSampler = GL_i_ProgramUniform(&gl_videoProgram, "uAtlas");
    gl_videoReady = 1;
    return 1;
}

int GL_b_VideoDisplayCreate(int width, int height)
{
    size_t bytes;

    GL_VideoDisplayClose();

    if (!gl_state.context || width <= 0 || height <= 0)
        return 0;
    if (!GL_l_b_VideoProgramReady())
        return 0;

    bytes = (size_t)width * (size_t)height * 4u;
    gl_videoPixels = (unsigned char*)calloc(1, bytes);
    if (!gl_videoPixels)
        return 0;

    GLFN_glGenTextures(1, &gl_videoTex);
    if (!gl_videoTex)
    {
        GL_VideoDisplayClose();
        return 0;
    }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_videoTex);
    GLFN_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    GLFN_glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    /* The engine's decoder writes the same thirty-two-bit words a
       texture is made of, so the upload reads them the same way. */
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                      GL_BGRA, GL_UNSIGNED_BYTE, gl_videoPixels);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    gl_videoWidth = width;
    gl_videoHeight = height;
    gl_videoScreenW = gl_state.width;
    gl_videoScreenH = gl_state.height;
    return 1;
}

int GL_b_VideoFrameLock(void** bits, int* pitch, int* height)
{
    if (!gl_videoPixels)
        return 0;

    if (bits)
        *bits = gl_videoPixels;
    if (pitch)
        *pitch = gl_videoWidth * 4;
    /* The picture's own height, not the screen's. Whatever fills it
       clamps to what it is told, so telling it anything else is how
       the bottom of a frame goes missing. */
    if (height)
        *height = gl_videoHeight;
    return 1;
}

void GL_VideoFrameUnlock(void)
{
    /* Where the other renderer's unlock gives the surface back to the
       driver, this is where the frame reaches the card. */
    if (!gl_videoPixels || !gl_videoTex || !gl_state.context)
        return;
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_videoTex);
    GLFN_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    GLFN_glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gl_videoWidth,
                         gl_videoHeight, GL_BGRA, GL_UNSIGNED_BYTE,
                         gl_videoPixels);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
}

void GL_VideoFramePresent(int width, int height, int fit)
{
    tGdi_TexturedVertex v[6];
    float               scale;
    GLint               filter;
    int                 scaledW, scaledH;
    int                 left, top;
    float               u, vmax;
    int                 i;

    if (!gl_videoPixels || !gl_videoTex || !gl_state.context)
        return;
    if (width <= 0 || height <= 0)
        return;
    if (!GL_l_b_VideoProgramReady() || !gl_quadBuffer)
        return;

    switch (fit)
    {
    case GDI_VIDEOFIT_SMOOTH:
        scale = (float)gl_videoScreenW / (float)width;
        filter = GL_LINEAR;
        break;
    case GDI_VIDEOFIT_SHARP:
        scale = (float)gl_videoScreenW / (float)width;
        filter = GL_NEAREST;
        break;
    default:
        /* A pixel for a pixel, centred. */
        scale = 1.0f;
        filter = GL_NEAREST;
        break;
    }

    /* The same truncation the other renderer does, and the same
       integer division for the centring -- so the picture lands on
       the same pixel. */
    scaledW = (int)((float)width * scale);
    scaledH = (int)((float)height * scale);
    left = (gl_videoScreenW - scaledW) / 2;
    top = (gl_videoScreenH - scaledH) / 2;

    /* The frame may be smaller than the picture it was written into,
       and the other renderer takes a sub-rectangle for exactly that.
       Here that is the texture coordinate's upper bound rather than
       the whole of it. */
    u = (float)width / (float)gl_videoWidth;
    vmax = (float)height / (float)gl_videoHeight;

    memset(v, 0, sizeof(v));
    for (i = 0; i < 6; i++)
        v[i].color = 0xFFFFFFFFu;
    v[0].pos.x = (float)left;            v[0].pos.y = (float)top;
    v[0].u = 0.0f;                       v[0].v = 0.0f;
    v[1].pos.x = (float)(left + scaledW); v[1].pos.y = (float)top;
    v[1].u = u;                          v[1].v = 0.0f;
    v[2].pos.x = (float)(left + scaledW); v[2].pos.y = (float)(top + scaledH);
    v[2].u = u;                          v[2].v = vmax;
    v[3].pos.x = (float)left;            v[3].pos.y = (float)top;
    v[3].u = 0.0f;                       v[3].v = 0.0f;
    v[4].pos.x = (float)(left + scaledW); v[4].pos.y = (float)(top + scaledH);
    v[4].u = u;                          v[4].v = vmax;
    v[5].pos.x = (float)left;            v[5].pos.y = (float)(top + scaledH);
    v[5].u = 0.0f;                       v[5].v = vmax;

    /* Black first, because a frame that does not fill the screen
       leaves the rest of it showing, and what is left showing has to
       be black rather than the last thing drawn. */
    GLFN_glDisable(GL_DEPTH_TEST);
    GLFN_glDisable(GL_CULL_FACE);
    GLFN_glDisable(GL_BLEND);
    GL_l_SetColorMask(1, 1, 1, 1);
    GL_l_ClearColour(0.0f, 0.0f, 0.0f, 0.0f, 0);

    GLFN_glUseProgram(gl_videoProgram.program);
    if (gl_videoViewport >= 0)
        GLFN_glUniform4f(gl_videoViewport, (GLfloat)gl_state.width,
                         (GLfloat)gl_state.height, 0.0f, 0.0f);
    if (gl_videoSampler >= 0)
        GLFN_glUniform1i(gl_videoSampler, 0);
    GLFN_glActiveTexture(GL_TEXTURE0);
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_videoTex);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    GLFN_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    GL_l_DrawBatch(gl_textVao, v, 2, sizeof(tGdi_TexturedVertex));

    GLFN_glUseProgram(0);
    /* Back to what the engine asked for, as the overlay pass does. */
    if (gl_state.zEnable)
        GLFN_glEnable(GL_DEPTH_TEST);
}
