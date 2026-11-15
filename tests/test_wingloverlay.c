/* A unit test for the OpenGL renderer's 2D pass.
 *
 * New code with no earlier copy, so a direct check.
 *
 * Every GL call in win/wingl.c goes through a function pointer in
 * glcore.h, so this drives the whole pass with no driver: the
 * pointers are aimed at mocks that record what was asked for, and a
 * case can then say which program was used, what was uploaded, how
 * the vertex layouts were described, and what state the pass left
 * behind.
 *
 * The shaders are the real ones from data/shaders. The loader reads
 * them through datapath.c, whose idea of where the executable is gets
 * pointed at the repository -- so a shader file with a typo in its
 * attribute names would be a failing test rather than a black overlay
 * on somebody's machine.
 *
 * win/wingl.c is included rather than linked, because the pass's state
 * is static to it and what matters here is what that state does.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* The GL types and the interface, before the mocks that use them.
   win/wingl.c brings these in too, and is included further down. */
#include "../win/glcore.h"
#include "../win/glshader.h"
#include "../gdi.h"

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

/* ------------------------------------------------------- the log, mocked */

static char wgLog[64][512];
static int  wgLogLen;

void DBG_OutputDebugString(char* format, ...)
{
    va_list ap;

    if (wgLogLen >= 64)
        return;
    va_start(ap, format);
    vsnprintf(wgLog[wgLogLen], sizeof(wgLog[0]), format, ap);
    va_end(ap);
    wgLogLen++;
}

/* ------------------------------------------------- Windows, mocked

   None of these are called by the cases below -- the pass does no
   windowing -- but wingl.c refers to them, so they have to exist. */

static char wg_exePath[512];

DWORD GetModuleFileNameA(HMODULE mod, LPSTR out, DWORD n)
{
    size_t len = strlen(wg_exePath);

    (void)mod;
    if (!wg_exePath[0] || len + 1 > n)
        return 0;
    memcpy(out, wg_exePath, len + 1);
    return (DWORD)len;
}

HDC      GetDC(HWND h) { (void)h; return NULL; }
int      ReleaseDC(HWND h, HDC d) { (void)h; (void)d; return 1; }
BOOL     DestroyWindow(HWND h) { (void)h; return 1; }
HMODULE  GetModuleHandleA(LPCSTR n) { (void)n; return NULL; }
HMODULE  LoadLibraryA(LPCSTR n) { (void)n; return NULL; }
FARPROC  GetProcAddress(HMODULE m, LPCSTR n) { (void)m; (void)n; return NULL; }
BOOL     UnregisterClassA(LPCSTR n, HINSTANCE i) { (void)n; (void)i; return 1; }
WORD     RegisterClassA(const WNDCLASSA* c) { (void)c; return 0; }
HWND     CreateWindowExA(DWORD a, LPCSTR b, LPCSTR c, DWORD d, int e, int f,
                         int g, int h, HWND i, HMENU j, HINSTANCE k, LPVOID l)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    (void)g; (void)h; (void)i; (void)j; (void)k; (void)l;
    return NULL;
}
LRESULT  DefWindowProcA(HWND a, UINT b, WPARAM c, LPARAM d)
{
    (void)a; (void)b; (void)c; (void)d;
    return 0;
}
BOOL     GetClientRect(HWND h, RECT* r) { (void)h; (void)r; return 0; }
int      ChoosePixelFormat(HDC d, const PIXELFORMATDESCRIPTOR* p)
{
    (void)d; (void)p; return 0;
}
BOOL     SetPixelFormat(HDC d, int f, const PIXELFORMATDESCRIPTOR* p)
{
    (void)d; (void)f; (void)p; return 0;
}
BOOL     SwapBuffers(HDC d) { (void)d; return 1; }
HGLRC    wglCreateContext(HDC d) { (void)d; return NULL; }
BOOL     wglMakeCurrent(HDC d, HGLRC c) { (void)d; (void)c; return 1; }
BOOL     wglDeleteContext(HGLRC c) { (void)c; return 1; }
void*    wglGetProcAddress(LPCSTR n) { (void)n; return NULL; }

/* ------------------------------------------------------- GL, recorded */

#define WG_MAXCALLS 512
#define WG_MAXARGS  8

typedef struct sWgCall
{
    const char* name;
    long long   arg[WG_MAXARGS];
    int         args;
} tWgCall;

static tWgCall wg_calls[WG_MAXCALLS];
static int     wg_callLen;

static void WG_Record(const char* name, int n, ...)
{
    va_list ap;
    int     i;

    if (wg_callLen >= WG_MAXCALLS)
        return;
    wg_calls[wg_callLen].name = name;
    wg_calls[wg_callLen].args = n;
    va_start(ap, n);
    for (i = 0; i < n && i < WG_MAXARGS; i++)
        wg_calls[wg_callLen].arg[i] = va_arg(ap, long long);
    va_end(ap);
    wg_callLen++;
}

static int WG_i_Count(const char* name)
{
    int i, n = 0;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, name) == 0)
            n++;
    return n;
}

/* The nth call of a name, or NULL. */
static const tWgCall* WG_pc_Call(const char* name, int nth)
{
    int i, n = 0;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, name) == 0 && n++ == nth)
            return &wg_calls[i];
    return NULL;
}

/* Where a name first appears, for ordering checks. -1 if never. */
static int WG_i_At(const char* name)
{
    int i;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, name) == 0)
            return i;
    return -1;
}

static int   wg_nextName = 10;
static int   wg_compileFails;
static int   wg_genFails;
static void* wg_lastBufferData;
static long  wg_lastBufferSize;

#define WGM0(fn)                 static void MK_##fn(void) { WG_Record(#fn, 0); }
#define WGM1(fn, T1)             static void MK_##fn(T1 a) { WG_Record(#fn, 1, (long long)a); }
#define WGM2(fn, T1, T2)         static void MK_##fn(T1 a, T2 b) { WG_Record(#fn, 2, (long long)a, (long long)b); }
#define WGM3(fn, T1, T2, T3)     static void MK_##fn(T1 a, T2 b, T3 c) { WG_Record(#fn, 3, (long long)a, (long long)b, (long long)c); }

WGM1(glEnable, GLenum)
WGM1(glDisable, GLenum)
WGM2(glBlendFunc, GLenum, GLenum)
WGM1(glUseProgram, GLuint)
WGM1(glBindVertexArray, GLuint)
WGM2(glBindBuffer, GLenum, GLuint)
WGM1(glEnableVertexAttribArray, GLuint)
WGM3(glDrawArrays, GLenum, GLint, GLsizei)
WGM2(glBindTexture, GLenum, GLuint)
WGM1(glActiveTexture, GLenum)
WGM2(glPixelStorei, GLenum, GLint)
WGM3(glTexParameteri, GLenum, GLenum, GLint)
WGM2(glUniform1i, GLint, GLint)
WGM1(glViewport_unused, GLenum)

static void MK_glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void* offset)
{
    WG_Record("glVertexAttribPointer", 6, (long long)index, (long long)size,
              (long long)type, (long long)normalized, (long long)stride,
              (long long)(size_t)offset);
}

static void MK_glBufferData(GLenum target, GLsizei size, const void* data,
                            GLenum usage)
{
    wg_lastBufferData = (void*)data;
    wg_lastBufferSize = (long)size;
    WG_Record("glBufferData", 3, (long long)target, (long long)size,
              (long long)usage);
}

static void MK_glUniform4f(GLint loc, GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
    WG_Record("glUniform4f", 5, (long long)loc, (long long)a, (long long)b,
              (long long)c, (long long)d);
}

static void MK_glTexImage2D(GLenum target, GLint level, GLint internalFormat,
                            GLsizei w, GLsizei h, GLint border, GLenum format,
                            GLenum type, const void* pixels)
{
    (void)pixels;
    WG_Record("glTexImage2D", 8, (long long)target, (long long)level,
              (long long)internalFormat, (long long)w, (long long)h,
              (long long)border, (long long)format, (long long)type);
}

static void MK_glGenBuffers(GLsizei n, GLuint* out)
{
    (void)n;
    *out = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenBuffers", 1, (long long)*out);
}

static void MK_glGenVertexArrays(GLsizei n, GLuint* out)
{
    (void)n;
    *out = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenVertexArrays", 1, (long long)*out);
}

static void MK_glGenTextures(GLsizei n, GLuint* out)
{
    (void)n;
    *out = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenTextures", 1, (long long)*out);
}

static void MK_glDeleteBuffers(GLsizei n, const GLuint* in)
{
    (void)n;
    WG_Record("glDeleteBuffers", 1, (long long)*in);
}

static void MK_glDeleteVertexArrays(GLsizei n, const GLuint* in)
{
    (void)n;
    WG_Record("glDeleteVertexArrays", 1, (long long)*in);
}

static void MK_glDeleteTextures(GLsizei n, const GLuint* in)
{
    (void)n;
    WG_Record("glDeleteTextures", 1, (long long)*in);
}

/* the shader half, enough for the loader to succeed */
static GLuint MK_glCreateShader(GLenum type)
{
    WG_Record("glCreateShader", 1, (long long)type);
    return (GLuint)wg_nextName++;
}
static void MK_glShaderSource(GLuint s, GLsizei n, const GLchar* const* v,
                              const GLint* l)
{
    (void)v; (void)l;
    WG_Record("glShaderSource", 2, (long long)s, (long long)n);
}
static void MK_glCompileShader(GLuint s) { WG_Record("glCompileShader", 1, (long long)s); }
static void MK_glGetShaderiv(GLuint s, GLenum p, GLint* out)
{
    (void)s; (void)p;
    *out = wg_compileFails ? GL_FALSE : GL_TRUE;
}
static void MK_glGetShaderInfoLog(GLuint s, GLsizei m, GLsizei* l, GLchar* log)
{
    (void)s;
    (void)m;
    log[0] = 0;
    if (l)
        *l = 0;
}
static void MK_glDeleteShader(GLuint s) { WG_Record("glDeleteShader", 1, (long long)s); }
static GLuint MK_glCreateProgram(void)
{
    GLuint p = (GLuint)wg_nextName++;

    WG_Record("glCreateProgram", 1, (long long)p);
    return p;
}
static void MK_glAttachShader(GLuint p, GLuint s)
{
    WG_Record("glAttachShader", 2, (long long)p, (long long)s);
}
static void MK_glDetachShader(GLuint p, GLuint s)
{
    WG_Record("glDetachShader", 2, (long long)p, (long long)s);
}
static void MK_glLinkProgram(GLuint p) { WG_Record("glLinkProgram", 1, (long long)p); }
static void MK_glGetProgramiv(GLuint p, GLenum pn, GLint* out)
{
    (void)p; (void)pn;
    *out = GL_TRUE;
}
static void MK_glGetProgramInfoLog(GLuint p, GLsizei m, GLsizei* l, GLchar* log)
{
    (void)p;
    (void)m;
    log[0] = 0;
    if (l)
        *l = 0;
}
static void MK_glDeleteProgram(GLuint p) { WG_Record("glDeleteProgram", 1, (long long)p); }

/* Uniform locations, by name, so a case can tell them apart and so a
   missing one can be arranged. */
static int wg_noUniform;
static GLint MK_glGetUniformLocation(GLuint p, const GLchar* name)
{
    (void)p;
    WG_Record("glGetUniformLocation", 0);
    if (wg_noUniform)
        return -1;
    if (strcmp(name, "uViewport") == 0)
        return 1;
    if (strcmp(name, "uAtlas") == 0)
        return 2;
    return 3;
}

/* The module under test. */
#include "../win/wingl.c"

static void WG_AimThePointers(void)
{
    GLFN_glEnable = MK_glEnable;
    GLFN_glDisable = MK_glDisable;
    GLFN_glBlendFunc = MK_glBlendFunc;
    GLFN_glUseProgram = MK_glUseProgram;
    GLFN_glBindVertexArray = MK_glBindVertexArray;
    GLFN_glBindBuffer = MK_glBindBuffer;
    GLFN_glEnableVertexAttribArray = MK_glEnableVertexAttribArray;
    GLFN_glVertexAttribPointer = MK_glVertexAttribPointer;
    GLFN_glDrawArrays = MK_glDrawArrays;
    GLFN_glBufferData = MK_glBufferData;
    GLFN_glGenBuffers = MK_glGenBuffers;
    GLFN_glGenVertexArrays = MK_glGenVertexArrays;
    GLFN_glDeleteBuffers = MK_glDeleteBuffers;
    GLFN_glDeleteVertexArrays = MK_glDeleteVertexArrays;
    GLFN_glGenTextures = MK_glGenTextures;
    GLFN_glDeleteTextures = MK_glDeleteTextures;
    GLFN_glBindTexture = MK_glBindTexture;
    GLFN_glActiveTexture = MK_glActiveTexture;
    GLFN_glPixelStorei = MK_glPixelStorei;
    GLFN_glTexImage2D = MK_glTexImage2D;
    GLFN_glTexParameteri = MK_glTexParameteri;
    GLFN_glUniform1i = MK_glUniform1i;
    GLFN_glUniform4f = MK_glUniform4f;

    GLFN_glCreateShader = MK_glCreateShader;
    GLFN_glShaderSource = MK_glShaderSource;
    GLFN_glCompileShader = MK_glCompileShader;
    GLFN_glGetShaderiv = MK_glGetShaderiv;
    GLFN_glGetShaderInfoLog = MK_glGetShaderInfoLog;
    GLFN_glDeleteShader = MK_glDeleteShader;
    GLFN_glCreateProgram = MK_glCreateProgram;
    GLFN_glAttachShader = MK_glAttachShader;
    GLFN_glDetachShader = MK_glDetachShader;
    GLFN_glLinkProgram = MK_glLinkProgram;
    GLFN_glGetProgramiv = MK_glGetProgramiv;
    GLFN_glGetProgramInfoLog = MK_glGetProgramInfoLog;
    GLFN_glDeleteProgram = MK_glDeleteProgram;
    GLFN_glGetUniformLocation = MK_glGetUniformLocation;
}

/* ------------------------------------------------------------- the rig */

/* A context the pass will accept. There is no driver; what the pass
   checks is that it has one. */
static void reset(void)
{
    GL_l_ForgetPass();
    memset(&gl_state, 0, sizeof(gl_state));
    gl_state.context = (HGLRC)(size_t)0xC047;
    gl_state.width = 800;
    gl_state.height = 600;
    gl_state.zEnable = 1;
    gl_state.zWrite = 1;

    wgLogLen = 0;
    wg_callLen = 0;
    wg_nextName = 10;
    wg_compileFails = 0;
    wg_genFails = 0;
    wg_noUniform = 0;
    wg_lastBufferData = NULL;
    wg_lastBufferSize = 0;
    /* the repository root, so data/shaders resolves to the real files */
    snprintf(wg_exePath, sizeof(wg_exePath), "./iriseng.exe");
}

static tGdi_Vertex        wg_flat[6];
static tGdi_TexturedVertex wg_text[6];

/* ---------------------------------------------------------- the cases */

static void caseLoadsTheRealShaders(void)
{
    printf("-- the shipped shaders load\n");
    reset();

    GL_OverlayBegin();
    okEq(wgLogLen, 0, "data/shaders holds what the pass asks for");
    okEq(WG_i_Count("glCreateShader"), 4, "four stages compiled");
    okEq(WG_i_Count("glLinkProgram"), 2, "two programs linked");
    ok(gl_passReady, "and the pass is ready");

    /* the uniforms the shaders declare are the ones it looks for: a
       name that does not match would log, and the overlay would draw
       at the wrong scale with nothing to say why */
    okEq(WG_i_Count("glGetUniformLocation"), 3,
         "it asks for three uniforms");
    ok(gl_flatViewport >= 0, "the flat program's viewport was found");
    ok(gl_textViewport >= 0, "the text program's viewport was found");
    ok(gl_textAtlas >= 0, "and its atlas sampler");

    /* and it is done once */
    wg_callLen = 0;
    GL_OverlayEnd();
    GL_OverlayBegin();
    okEq(WG_i_Count("glCreateShader"), 0, "a second pass compiles nothing");
    GL_OverlayEnd();
}

static void caseState(void)
{
    printf("-- the state the pass sets and puts back\n");
    reset();
    GL_OverlayBegin();

    ok(WG_i_At("glBlendFunc") >= 0, "blending is set up");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[0], GL_SRC_ALPHA, "source factor");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[1], GL_ONE_MINUS_SRC_ALPHA,
         "destination factor");

    {
        int i, sawDepthOff = 0, sawCullOff = 0, sawBlendOn = 0;

        for (i = 0; i < wg_callLen; i++)
        {
            if (strcmp(wg_calls[i].name, "glDisable") == 0)
            {
                if (wg_calls[i].arg[0] == GL_DEPTH_TEST) sawDepthOff = 1;
                if (wg_calls[i].arg[0] == GL_CULL_FACE) sawCullOff = 1;
            }
            if (strcmp(wg_calls[i].name, "glEnable") == 0
                && wg_calls[i].arg[0] == GL_BLEND)
                sawBlendOn = 1;
        }
        ok(sawDepthOff, "the depth test is turned off");
        ok(sawCullOff, "and culling");
        ok(sawBlendOn, "and blending on");
    }

    /* End puts the depth test back, because the engine asked for it */
    wg_callLen = 0;
    GL_OverlayEnd();
    {
        int i, back = 0;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glEnable") == 0
                && wg_calls[i].arg[0] == GL_DEPTH_TEST)
                back = 1;
        ok(back, "the depth test comes back");
    }
    ok(WG_i_At("glDisable") >= 0, "and blending goes off");
    okEq(WG_pc_Call("glUseProgram", 0)->arg[0], 0, "no program is left in use");

    /* and it does not come back when the engine had it off */
    reset();
    gl_state.zEnable = 0;
    GL_OverlayBegin();
    wg_callLen = 0;
    GL_OverlayEnd();
    {
        int i, back = 0;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glEnable") == 0
                && wg_calls[i].arg[0] == GL_DEPTH_TEST)
                back = 1;
        ok(!back, "the depth test stays off if that is what was asked for");
    }
}

static void caseLayouts(void)
{
    const tWgCall* c;

    printf("-- the vertex layouts match the structs\n");
    reset();
    GL_OverlayBegin();

    /* Six attributes described: three for the flat layout's two, and
       three for the textured one. Checked against the structs rather
       than against the numbers in wingl.c, which is the point --
       an offset written by hand and a struct that moved would
       otherwise agree with each other and with nothing else. */
    okEq(WG_i_Count("glVertexAttribPointer"), 5,
         "five attributes are described");

    c = WG_pc_Call("glVertexAttribPointer", 0);
    okEq(c->arg[0], 0, "flat: position is attribute 0");
    okEq(c->arg[1], 3, "three floats");
    okEq(c->arg[2], GL_FLOAT, "of type float");
    okEq(c->arg[4], (long)sizeof(tGdi_Vertex), "with the struct's stride");
    okEq(c->arg[5], (long)offsetof(tGdi_Vertex, pos), "at the struct's offset");

    c = WG_pc_Call("glVertexAttribPointer", 1);
    okEq(c->arg[0], 1, "flat: colour is attribute 1");
    okEq(c->arg[1], 4, "four bytes");
    okEq(c->arg[2], GL_UNSIGNED_BYTE, "unsigned");
    okEq(c->arg[3], GL_TRUE, "normalised, so 255 means 1.0");
    okEq(c->arg[4], (long)sizeof(tGdi_Vertex), "with the struct's stride");
    okEq(c->arg[5], (long)offsetof(tGdi_Vertex, color), "at the struct's offset");

    c = WG_pc_Call("glVertexAttribPointer", 2);
    okEq(c->arg[0], 0, "text: position is attribute 0");
    okEq(c->arg[4], (long)sizeof(tGdi_TexturedVertex), "with its own stride");
    okEq(c->arg[5], (long)offsetof(tGdi_TexturedVertex, pos), "and offset");

    c = WG_pc_Call("glVertexAttribPointer", 3);
    okEq(c->arg[0], 1, "text: colour is attribute 1");
    okEq(c->arg[4], (long)sizeof(tGdi_TexturedVertex), "with its own stride");
    okEq(c->arg[5], (long)offsetof(tGdi_TexturedVertex, color), "and offset");

    c = WG_pc_Call("glVertexAttribPointer", 4);
    okEq(c->arg[0], 2, "text: the texture coordinate is attribute 2");
    okEq(c->arg[1], 2, "two floats");
    okEq(c->arg[2], GL_FLOAT, "of type float");
    okEq(c->arg[5], (long)offsetof(tGdi_TexturedVertex, u), "at the struct's offset");

    /* the two layouts are described into different vertex arrays */
    ok(gl_flatVao != gl_textVao, "the two layouts have arrays of their own");
    GL_OverlayEnd();
}

static void caseFlatBatch(void)
{
    const tWgCall* c;

    printf("-- a batch of flat quads\n");
    reset();
    GL_OverlayBegin();
    wg_callLen = 0;

    GL_OverlayDrawQuads(wg_flat, 2);

    okEq(WG_i_Count("glDrawArrays"), 1, "one draw");
    c = WG_pc_Call("glDrawArrays", 0);
    okEq(c->arg[0], GL_TRIANGLES, "of triangles");
    okEq(c->arg[1], 0, "from the start");
    okEq(c->arg[2], 6, "two triangles is six vertices");

    okEq(WG_i_Count("glBufferData"), 1, "one upload");
    okEq(wg_lastBufferSize, (long)(6 * sizeof(tGdi_Vertex)),
         "of six vertices' worth");
    ok(wg_lastBufferData == wg_flat, "from the caller's own array");

    okEq(WG_pc_Call("glUseProgram", 0)->arg[0], gl_flatProgram.program,
         "with the flat program");
    c = WG_pc_Call("glUniform4f", 0);
    okEq(c->arg[0], gl_flatViewport, "the viewport uniform is set");
    okEq(c->arg[1], 800, "to the width");
    okEq(c->arg[2], 600, "and the height");

    ok(WG_i_At("glBufferData") < WG_i_At("glDrawArrays"),
       "and the upload happens before the draw");
    okEq(WG_pc_Call("glBindVertexArray", 0)->arg[0], gl_flatVao,
         "through the flat layout's vertex array");

    /* nothing to draw is not a draw */
    wg_callLen = 0;
    GL_OverlayDrawQuads(wg_flat, 0);
    GL_OverlayDrawQuads(wg_flat, -1);
    GL_OverlayDrawQuads(NULL, 2);
    okEq(WG_i_Count("glDrawArrays"), 0, "an empty batch draws nothing");

    /* and neither is a batch outside the pass */
    GL_OverlayEnd();
    wg_callLen = 0;
    GL_OverlayDrawQuads(wg_flat, 2);
    okEq(WG_i_Count("glDrawArrays"), 0, "nor one outside the pass");
}

static void caseAtlas(void)
{
    const tWgCall* c;
    unsigned char  coverage[4 * 2];
    void*          handle;

    printf("-- a glyph atlas\n");
    reset();
    memset(coverage, 0x80, sizeof(coverage));

    /* Deliberately not square: with a 4x4 atlas a width and a height
       swapped over in the upload is invisible, and that is exactly
       the mistake worth catching. */
    handle = GL_p_CreateAlphaTexture(4, 2, coverage);
    ok(handle != NULL, "it is created");
    okEq(WG_i_Count("glGenTextures"), 1, "one texture");

    c = WG_pc_Call("glTexImage2D", 0);
    okEq(c->arg[2], GL_R8, "single channel, because coverage is one byte");
    okEq(c->arg[3], 4, "the width given");
    okEq(c->arg[4], 2, "and the height, which is not the same number");
    okEq(c->arg[6], GL_RED, "uploaded as red");
    okEq(c->arg[7], GL_UNSIGNED_BYTE, "one byte per pixel");

    c = WG_pc_Call("glPixelStorei", 0);
    okEq(c->arg[0], GL_UNPACK_ALIGNMENT, "the row alignment is set");
    okEq(c->arg[1], 1, "to one, because the rows are packed");

    okEq(WG_i_Count("glTexParameteri"), 4, "four parameters set");
    ok(WG_i_At("glPixelStorei") < WG_i_At("glTexImage2D"),
       "and the alignment before the upload, not after");

    /* The handle is a GL name, not a pointer, and zero is both "no
       texture" to GL and NULL to everyone else -- so it travels
       offset by one. */
    okEq((long)(size_t)handle, (long)(WG_pc_Call("glGenTextures", 0)->arg[0] + 1),
         "the handle is the name, offset by one");

    wg_callLen = 0;
    GL_DestroyAlphaTexture(handle);
    okEq(WG_i_Count("glDeleteTextures"), 1, "it is given back");
    okEq(WG_pc_Call("glDeleteTextures", 0)->arg[0],
         (long)(size_t)handle - 1, "by its real name");

    wg_callLen = 0;
    GL_DestroyAlphaTexture(NULL);
    okEq(WG_i_Count("glDeleteTextures"), 0, "and NULL is not a name");

    /* refusals */
    reset();
    ok(GL_p_CreateAlphaTexture(0, 4, coverage) == NULL, "no width, no texture");
    ok(GL_p_CreateAlphaTexture(4, 0, coverage) == NULL, "no height, no texture");
    ok(GL_p_CreateAlphaTexture(4, 4, NULL) == NULL, "no pixels, no texture");
    reset();
    wg_genFails = 1;
    ok(GL_p_CreateAlphaTexture(4, 4, coverage) == NULL,
       "and a driver that will not give a name");
}

static void caseTextBatch(void)
{
    const tWgCall* c;
    unsigned char  coverage[4];
    void*          atlas;

    printf("-- a batch of glyphs\n");
    reset();
    memset(coverage, 0xFF, sizeof(coverage));
    GL_OverlayBegin();
    atlas = GL_p_CreateAlphaTexture(2, 2, coverage);
    wg_callLen = 0;

    GL_OverlayDrawTexturedQuads(atlas, wg_text, 2);

    okEq(WG_i_Count("glDrawArrays"), 1, "one draw");
    okEq(WG_pc_Call("glDrawArrays", 0)->arg[2], 6, "of six vertices");
    okEq(wg_lastBufferSize, (long)(6 * sizeof(tGdi_TexturedVertex)),
         "with the textured vertex's size, not the flat one's");
    okEq(WG_pc_Call("glUseProgram", 0)->arg[0], gl_textProgram.program,
         "with the text program, not the flat one");
    okEq(WG_pc_Call("glBindVertexArray", 0)->arg[0], gl_textVao,
         "and the text layout's vertex array, not the flat one -- the flat one describes a stride these vertices do not have");

    c = WG_pc_Call("glBindTexture", 0);
    okEq(c->arg[0], GL_TEXTURE_2D, "the atlas is bound");
    okEq(c->arg[1], (long)(size_t)atlas - 1, "by its real name");
    okEq(WG_pc_Call("glActiveTexture", 0)->arg[0], GL_TEXTURE0,
         "on texture unit zero");
    c = WG_pc_Call("glUniform1i", 0);
    okEq(c->arg[0], gl_textAtlas, "and the sampler is told");
    okEq(c->arg[1], 0, "unit zero");

    /* refusals */
    wg_callLen = 0;
    GL_OverlayDrawTexturedQuads(NULL, wg_text, 2);
    GL_OverlayDrawTexturedQuads(atlas, NULL, 2);
    GL_OverlayDrawTexturedQuads(atlas, wg_text, 0);
    okEq(WG_i_Count("glDrawArrays"), 0, "nothing missing draws anything");

    GL_OverlayEnd();
    wg_callLen = 0;
    GL_OverlayDrawTexturedQuads(atlas, wg_text, 2);
    okEq(WG_i_Count("glDrawArrays"), 0, "and not outside the pass");
}

static void caseRefusals(void)
{
    printf("-- when the pass cannot be had\n");

    /* no context */
    reset();
    gl_state.context = NULL;
    GL_OverlayBegin();
    okEq(WG_i_Count("glCreateShader"), 0, "no context compiles nothing");
    ok(!gl_passReady, "and the pass is not ready");
    wg_callLen = 0;
    GL_OverlayDrawQuads(wg_flat, 2);
    okEq(WG_i_Count("glDrawArrays"), 0, "and nothing draws");
    GL_OverlayEnd();

    /* a shader that will not compile: reported once, and then the
       pass stays shut rather than trying again every frame */
    reset();
    wg_compileFails = 1;
    GL_OverlayBegin();
    ok(!gl_passReady, "a shader that will not compile shuts the pass");
    ok(wgLogLen > 0, "and says so");
    {
        int said = wgLogLen;

        wg_callLen = 0;
        GL_OverlayBegin();
        GL_OverlayBegin();
        okEq(wgLogLen, said, "and does not say it again every frame");
        okEq(WG_i_Count("glCreateShader"), 0, "nor try again");
    }

    /* a driver that will not give a buffer name */
    reset();
    wg_genFails = 1;
    GL_OverlayBegin();
    ok(!gl_passReady, "no buffer shuts the pass");
    ok(WG_i_Count("glDeleteProgram") == 2,
       "and both programs are given back rather than leaked");
}

static void caseTeardown(void)
{
    printf("-- giving it all back\n");
    reset();
    GL_OverlayBegin();
    GL_OverlayEnd();
    wg_callLen = 0;

    GL_l_ForgetPass();
    okEq(WG_i_Count("glDeleteProgram"), 2, "both programs");
    okEq(WG_i_Count("glDeleteBuffers"), 1, "the buffer");
    okEq(WG_i_Count("glDeleteVertexArrays"), 2, "and both vertex arrays");
    ok(!gl_passReady, "the pass is shut");

    wg_callLen = 0;
    GL_l_ForgetPass();
    okEq(WG_i_Count("glDeleteProgram"), 0, "and doing it twice gives nothing back twice");

    /* and it can be built again afterwards */
    GL_OverlayBegin();
    ok(gl_passReady, "it comes back");
    GL_OverlayEnd();
}

int main(void)
{
    printf("=== GL overlay pass unit test ===\n");
    WG_AimThePointers();
    caseLoadsTheRealShaders();
    caseState();
    caseLayouts();
    caseFlatBatch();
    caseAtlas();
    caseTextBatch();
    caseRefusals();
    caseTeardown();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
