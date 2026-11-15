#include "eglheadless.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------- EGL, declared here

   No headers on this machine, so the types and the constants the
   context needs are written out. EGL's handles are all opaque
   pointers and its attribute lists are all arrays of int terminated
   by EGL_NONE, which is what makes this short enough to be worth
   doing rather than installing a package the build cannot reach. */

typedef void*        EGLDisplay;
typedef void*        EGLConfig;
typedef void*        EGLContext;
typedef void*        EGLSurface;
typedef int          EGLint;
typedef unsigned int EGLenum;
typedef unsigned int EGLBoolean;

#define EGL_DEFAULT_DISPLAY                  ((void*)0)
#define EGL_NO_CONTEXT                       ((EGLContext)0)
#define EGL_NO_SURFACE                       ((EGLSurface)0)
#define EGL_NO_DISPLAY                       ((EGLDisplay)0)
#define EGL_NONE                             0x3038
#define EGL_SURFACE_TYPE                     0x3033
#define EGL_PBUFFER_BIT                      0x0001
#define EGL_RENDERABLE_TYPE                  0x3040
#define EGL_OPENGL_BIT                       0x0008
#define EGL_RED_SIZE                         0x3024
#define EGL_GREEN_SIZE                       0x3023
#define EGL_BLUE_SIZE                        0x3022
#define EGL_ALPHA_SIZE                       0x3021
#define EGL_DEPTH_SIZE                       0x3025
#define EGL_WIDTH                            0x3057
#define EGL_HEIGHT                           0x3056
#define EGL_OPENGL_API                       0x30A2
#define EGL_CONTEXT_MAJOR_VERSION            0x3098
#define EGL_CONTEXT_MINOR_VERSION            0x30FB
#define EGL_CONTEXT_OPENGL_PROFILE_MASK      0x30FD
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT  0x00000001

/* Mesa's own platform for "no window system at all", which is what
   makes this work on a machine with no X and no Wayland. */
#define EGL_PLATFORM_SURFACELESS_MESA        0x31DD

/* GL_VERSION, GL_RENDERER, GL_SHADING_LANGUAGE_VERSION, spelled out
   rather than included: this file is below glcore.h, not above it. */
#define EGLH_GL_VERSION                      0x1F02
#define EGLH_GL_RENDERER                     0x1F01
#define EGLH_GL_GLSL                         0x8B8C

const char* EGLH_why = "";
const char* EGLH_version;
const char* EGLH_renderer;
const char* EGLH_glsl;

static void*      eglh_lib;
static EGLDisplay eglh_display;
static EGLContext eglh_context;
static EGLSurface eglh_surface;
static int        eglh_open;

static EGLDisplay (*eglh_GetDisplay)(void*);
static EGLDisplay (*eglh_GetPlatformDisplay)(EGLenum, void*, const EGLint*);
static EGLBoolean (*eglh_Initialize)(EGLDisplay, EGLint*, EGLint*);
static EGLBoolean (*eglh_Terminate)(EGLDisplay);
static EGLBoolean (*eglh_BindAPI)(EGLenum);
static EGLBoolean (*eglh_ChooseConfig)(EGLDisplay, const EGLint*, EGLConfig*,
                                       EGLint, EGLint*);
static EGLContext (*eglh_CreateContext)(EGLDisplay, EGLConfig, EGLContext,
                                        const EGLint*);
static EGLBoolean (*eglh_DestroyContext)(EGLDisplay, EGLContext);
static EGLSurface (*eglh_CreatePbufferSurface)(EGLDisplay, EGLConfig,
                                               const EGLint*);
static EGLBoolean (*eglh_DestroySurface)(EGLDisplay, EGLSurface);
static EGLBoolean (*eglh_MakeCurrent)(EGLDisplay, EGLSurface, EGLSurface,
                                      EGLContext);
static void*      (*eglh_GetProcAddress)(const char*);

static const char* (*eglh_glGetString)(unsigned int);

static int EGLH_l_b_Load(void)
{
    struct { void* slot; const char* name; } want[] = {
        { &eglh_GetDisplay,           "eglGetDisplay" },
        { &eglh_Initialize,           "eglInitialize" },
        { &eglh_Terminate,            "eglTerminate" },
        { &eglh_BindAPI,              "eglBindAPI" },
        { &eglh_ChooseConfig,         "eglChooseConfig" },
        { &eglh_CreateContext,        "eglCreateContext" },
        { &eglh_DestroyContext,       "eglDestroyContext" },
        { &eglh_CreatePbufferSurface, "eglCreatePbufferSurface" },
        { &eglh_DestroySurface,       "eglDestroySurface" },
        { &eglh_MakeCurrent,          "eglMakeCurrent" },
        { &eglh_GetProcAddress,       "eglGetProcAddress" },
    };
    unsigned int i;

    eglh_lib = dlopen("libEGL.so.1", RTLD_NOW);
    if (!eglh_lib)
    {
        EGLH_why = "no libEGL.so.1 on this machine";
        return 0;
    }
    for (i = 0; i < sizeof(want) / sizeof(want[0]); i++)
    {
        void* p = dlsym(eglh_lib, want[i].name);

        if (!p)
        {
            EGLH_why = "libEGL.so.1 is missing an entry point";
            return 0;
        }
        memcpy(want[i].slot, &p, sizeof(p));
    }
    /* This one is an extension, so it is looked up through EGL itself
       and is allowed to be absent -- the default display is tried when
       it is. */
    {
        void* p = eglh_GetProcAddress("eglGetPlatformDisplayEXT");

        memcpy(&eglh_GetPlatformDisplay, &p, sizeof(p));
    }
    return 1;
}

int EGLH_b_Open(void)
{
    static const EGLint cfgAttr[] = {
        EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE,   8, EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE,  8, EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };
    /* 4.5 core and nothing less, which is what the renderer asks a
       real driver for. A context that came up on 3.3 would compile
       the shaders and then differ somewhere further away. */
    static const EGLint ctxAttr[] = {
        EGL_CONTEXT_MAJOR_VERSION, 4,
        EGL_CONTEXT_MINOR_VERSION, 5,
        EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
        EGL_NONE
    };
    /* The surface is incidental: everything is drawn into a
       framebuffer object. It exists because a context has to be made
       current against something on drivers without the surfaceless
       extension. */
    static const EGLint pbAttr[] = { EGL_WIDTH, 64, EGL_HEIGHT, 64, EGL_NONE };

    EGLConfig cfg;
    EGLint    major, minor, found;
    void*     p;

    if (eglh_open)
        return 1;
    if (!EGLH_l_b_Load())
        return 0;

    /* The surfaceless platform first, because there is no display
       server here; the ordinary default display is the fallback for a
       machine where there is one. */
    if (eglh_GetPlatformDisplay)
    {
        eglh_display = eglh_GetPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA,
                                               EGL_DEFAULT_DISPLAY, NULL);
        if (eglh_display != EGL_NO_DISPLAY
            && !eglh_Initialize(eglh_display, &major, &minor))
            eglh_display = EGL_NO_DISPLAY;
    }
    if (eglh_display == EGL_NO_DISPLAY)
    {
        eglh_display = eglh_GetDisplay(EGL_DEFAULT_DISPLAY);
        if (eglh_display == EGL_NO_DISPLAY
            || !eglh_Initialize(eglh_display, &major, &minor))
        {
            EGLH_why = "no EGL display, with or without a window system";
            eglh_display = EGL_NO_DISPLAY;
            return 0;
        }
    }

    if (!eglh_BindAPI(EGL_OPENGL_API))
    {
        EGLH_why = "this EGL does not offer desktop OpenGL";
        EGLH_Close();
        return 0;
    }
    if (!eglh_ChooseConfig(eglh_display, cfgAttr, &cfg, 1, &found) || found < 1)
    {
        EGLH_why = "no EGL config with colour, alpha and depth";
        EGLH_Close();
        return 0;
    }
    eglh_context = eglh_CreateContext(eglh_display, cfg, EGL_NO_CONTEXT,
                                      ctxAttr);
    if (eglh_context == EGL_NO_CONTEXT)
    {
        EGLH_why = "no OpenGL 4.5 core context here";
        EGLH_Close();
        return 0;
    }
    /* A surface is nice to have and not required: with the
       surfaceless extension the context goes current against none. */
    eglh_surface = eglh_CreatePbufferSurface(eglh_display, cfg, pbAttr);
    if (!eglh_MakeCurrent(eglh_display, eglh_surface, eglh_surface,
                          eglh_context))
    {
        EGLH_why = "the 4.5 context would not become current";
        EGLH_Close();
        return 0;
    }

    p = eglh_GetProcAddress("glGetString");
    memcpy(&eglh_glGetString, &p, sizeof(p));
    if (!eglh_glGetString)
    {
        EGLH_why = "the context has no glGetString";
        EGLH_Close();
        return 0;
    }
    EGLH_version = eglh_glGetString(EGLH_GL_VERSION);
    EGLH_renderer = eglh_glGetString(EGLH_GL_RENDERER);
    EGLH_glsl = eglh_glGetString(EGLH_GL_GLSL);

    eglh_open = 1;
    return 1;
}

void EGLH_Close(void)
{
    if (eglh_display != EGL_NO_DISPLAY)
    {
        if (eglh_MakeCurrent)
            eglh_MakeCurrent(eglh_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                             EGL_NO_CONTEXT);
        if (eglh_surface != EGL_NO_SURFACE && eglh_DestroySurface)
            eglh_DestroySurface(eglh_display, eglh_surface);
        if (eglh_context != EGL_NO_CONTEXT && eglh_DestroyContext)
            eglh_DestroyContext(eglh_display, eglh_context);
        if (eglh_Terminate)
            eglh_Terminate(eglh_display);
    }
    eglh_surface = EGL_NO_SURFACE;
    eglh_context = EGL_NO_CONTEXT;
    eglh_display = EGL_NO_DISPLAY;
    eglh_open = 0;
    EGLH_version = NULL;
    EGLH_renderer = NULL;
    EGLH_glsl = NULL;
}

void* EGLH_p_Proc(const char* name)
{
    if (!eglh_open || !eglh_GetProcAddress)
        return NULL;
    return eglh_GetProcAddress(name);
}
