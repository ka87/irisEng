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

#include "winstubs.h"

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

#define WGM0(fn)                 static void GL_APIENTRY MK_##fn(void) { WG_Record(#fn, 0); }
#define WGM1(fn, T1)             static void GL_APIENTRY MK_##fn(T1 a) { WG_Record(#fn, 1, (long long)a); }
#define WGM2(fn, T1, T2)         static void GL_APIENTRY MK_##fn(T1 a, T2 b) { WG_Record(#fn, 2, (long long)a, (long long)b); }
#define WGM3(fn, T1, T2, T3)     static void GL_APIENTRY MK_##fn(T1 a, T2 b, T3 c) { WG_Record(#fn, 3, (long long)a, (long long)b, (long long)c); }

WGM1(glCullFace, GLenum)
WGM1(glFrontFace, GLenum)
WGM1(glEnable, GLenum)
WGM1(glDisable, GLenum)
WGM2(glBlendFunc, GLenum, GLenum)
WGM1(glUseProgram, GLuint)
WGM1(glBindVertexArray, GLuint)
WGM2(glBindBuffer, GLenum, GLuint)
WGM1(glEnableVertexAttribArray, GLuint)
WGM3(glDrawArrays, GLenum, GLint, GLsizei)
/* By hand rather than through the macros above: four arguments, and
   the last is a byte offset into the bound index buffer, which goes on
   the record as the number it is. */
static void GL_APIENTRY MK_glDrawElements(GLenum mode, GLsizei count,
                                          GLenum type, const void* offset)
{
    WG_Record("glDrawElements", 4, (long long)mode, (long long)count,
              (long long)type, (long long)(size_t)offset);
}
WGM2(glBindTexture, GLenum, GLuint)
WGM1(glActiveTexture, GLenum)
WGM2(glPixelStorei, GLenum, GLint)
WGM3(glTexParameteri, GLenum, GLenum, GLint)
WGM2(glUniform1i, GLint, GLint)
WGM1(glViewport_unused, GLenum)

/* The rest of glcore.h's list: the frame's edges, the version query
   and the debug callback. Nothing in these cases looks at them, but
   every pointer has to go somewhere -- an entry point left aimed at
   nothing is a call through a null pointer, and that lands as a
   segmentation fault in whichever case reaches it first. An earlier
   version of WG_CheckThePointers kept a list of the ones deliberately
   left out, which went stale the moment the shadow targets started
   clearing: the list was a promise about what the cases do not reach,
   and it was wrong. So there is no list now, and no pointer is left
   null. */
static long WG_l_Scale(float v)
{
    return (long)(v * 1000000.0f + (v < 0.0f ? -0.5f : 0.5f));
}

WGM1(glClear, GLbitfield)
WGM1(glClearDepth, GLclampd)
WGM1(glClearStencil, GLint)
WGM0(glFinish)
WGM0(glFlush)

/* Scaled by a million on the way to the record, the way the uniform
   mocks are. It used to truncate to an integer, and every component of
   a clear colour is under one, so every colour but white recorded as
   nought -- "cleared to black" passed for a clear to any colour at
   all, and the water's clear to the colour the engine asked for could
   not be checked. */
static void GL_APIENTRY MK_glClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    WG_Record("glClearColor", 4, (long long)WG_l_Scale(r),
              (long long)WG_l_Scale(g), (long long)WG_l_Scale(b),
              (long long)WG_l_Scale(a));
}

static void GL_APIENTRY MK_glScissor(GLint x, GLint y, GLsizei w, GLsizei h)
{
    WG_Record("glScissor", 4, (long long)x, (long long)y, (long long)w,
              (long long)h);
}

static GLenum GL_APIENTRY MK_glGetError(void) { WG_Record("glGetError", 0); return GL_NO_ERROR; }

static const GLubyte* GL_APIENTRY MK_glGetString(GLenum name)
{
    WG_Record("glGetString", 1, (long long)name);
    return (const GLubyte*)"mock";
}

static void GL_APIENTRY MK_glDebugMessageCallback(GL_pfn_DebugProc proc, const void* user)
{
    (void)proc;
    (void)user;
    WG_Record("glDebugMessageCallback", 0);
}

static void GL_APIENTRY MK_glDebugMessageControl(GLenum source, GLenum type,
                                     GLenum severity, GLsizei count,
                                     const GLuint* ids, GLboolean enabled)
{
    (void)ids;
    WG_Record("glDebugMessageControl", 5, (long long)source, (long long)type,
              (long long)severity, (long long)count, (long long)enabled);
}

static void GL_APIENTRY MK_glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void* offset)
{
    WG_Record("glVertexAttribPointer", 6, (long long)index, (long long)size,
              (long long)type, (long long)normalized, (long long)stride,
              (long long)(size_t)offset);
}

/* A copy of what was uploaded, as well as the pointer.

   The pointer alone is not safe to read later: the video present
   builds its six vertices on the stack, so by the time a case looks
   at them the frame is gone. ASan said so the moment that case was
   written. One case checks the pointer's identity -- that the flat
   overlay batch is handed over without being copied -- so both are
   kept, and anything reading contents reads the copy. */
#define WG_BUFCOPY (64 * 1024)
static unsigned char wg_bufferCopy[WG_BUFCOPY];
static long          wg_bufferCopyLen;

static void GL_APIENTRY MK_glBufferData(GLenum target, GLsizei size, const void* data,
                            GLenum usage)
{
    wg_lastBufferData = (void*)data;
    wg_bufferCopyLen = 0;
    if (data && size > 0 && size <= (GLsizei)sizeof(wg_bufferCopy))
    {
        memcpy(wg_bufferCopy, data, (size_t)size);
        wg_bufferCopyLen = (long)size;
    }
    wg_lastBufferSize = (long)size;
    /* The first four floats go on the record as well, scaled, so that
       a case can read the quad each pass uploaded rather than only
       the last one. For an after-effect's quad those are the first
       vertex's x and y and the second vertex's x and y, which is
       where the strip it covers starts and ends. */
    {
        long long f[4] = { 0, 0, 0, 0 };
        int       i;

        if (data && size >= (GLsizei)(4 * sizeof(GLfloat)))
            for (i = 0; i < 4; i++)
            {
                GLfloat v = ((const GLfloat*)data)[i];

                f[i] = (long long)(v * 1000000.0f
                                   + (v < 0.0f ? -0.5f : 0.5f));
            }
        WG_Record("glBufferData", 7, (long long)target, (long long)size,
                  (long long)usage, f[0], f[1], f[2], f[3]);
    }
}

static const GLfloat* wg_lastMatrix;
static int            wg_lastTranspose;
/* Each matrix uploaded, kept in order and not just the last: a draw
   sets two of them now -- the combined one for the position and the
   view on its own for the fog -- and keeping only the last would read
   the second where the first was meant, which is how this case first
   failed. */
#define WG_MATRICES 8
static const GLfloat* wg_matrix[WG_MATRICES];
static GLfloat        wg_matrixData[WG_MATRICES][16];
static int            wg_matrixNum;

static void GL_APIENTRY MK_glUniformMatrix4fv(GLint loc, GLsizei count, GLboolean transpose,
                                  const GLfloat* value)
{
    wg_lastMatrix = value;
    wg_lastTranspose = transpose;
    if (wg_matrixNum < WG_MATRICES)
    {
        /* The sixteen floats, not only the pointer. A renderer is free
           to upload a matrix it built on its own stack -- the water's
           projection remap is one -- and the pointer is dangling by
           the time a case looks at it. */
        if (value)
            memcpy(wg_matrixData[wg_matrixNum], value,
                   16 * sizeof(GLfloat));
        else
            memset(wg_matrixData[wg_matrixNum], 0,
                   16 * sizeof(GLfloat));
        wg_matrix[wg_matrixNum++] = value;
    }
    WG_Record("glUniformMatrix4fv", 3, (long long)loc, (long long)count,
              (long long)transpose);
}

static void GL_APIENTRY MK_glClipControl(GLenum origin, GLenum depth)
{
    WG_Record("glClipControl", 2, (long long)origin, (long long)depth);
}

static void GL_APIENTRY MK_glDepthFunc(GLenum f) { WG_Record("glDepthFunc", 1, (long long)f); }

/* Declared with the framebuffer mocks, which answer glGetIntegerv out
   of what this last set. */
static GLint wg_viewport[4];

static void GL_APIENTRY MK_glViewport(GLint x, GLint y, GLsizei w, GLsizei h)
{
    wg_viewport[0] = x;
    wg_viewport[1] = y;
    wg_viewport[2] = (GLint)w;
    wg_viewport[3] = (GLint)h;
    WG_Record("glViewport", 4, (long long)x, (long long)y, (long long)w,
              (long long)h);
}

static void GL_APIENTRY MK_glUniform4f(GLint loc, GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
    WG_Record("glUniform4f", 5, (long long)loc, (long long)a, (long long)b,
              (long long)c, (long long)d);
    /* Again, scaled. The record above truncates to whole numbers,
       which suits the viewport it was written for and loses every
       digit of a texture-space offset -- one pixel of a 640 wide
       picture is 0.0015625, and that records as nought. Both are kept
       rather than one changed, because the cases that read the
       viewport read it as whole numbers. */
    WG_Record("glUniform4f.scaled", 5, (long long)loc,
              (long long)(a * 1000000.0f + (a < 0.0f ? -0.5f : 0.5f)),
              (long long)(b * 1000000.0f + (b < 0.0f ? -0.5f : 0.5f)),
              (long long)(c * 1000000.0f + (c < 0.0f ? -0.5f : 0.5f)),
              (long long)(d * 1000000.0f + (d < 0.0f ? -0.5f : 0.5f)));
}

static void GL_APIENTRY MK_glUniform1f(GLint loc, GLfloat v)
{
    /* The reference is a byte scaled into 0..1; recorded multiplied
       back up so a case can compare whole numbers. */
    WG_Record("glUniform1f", 2, (long long)loc,
              (long long)(v * 255.0f + 0.5f));
    /* And again at a million, for the values that are not a byte --
       a gain and a bias are arbitrary numbers and the record above
       would round them to the nearest two hundred and fifty-fifth. */
    WG_Record("glUniform1f.scaled", 2, (long long)loc,
              (long long)(v * 1000000.0f + (v < 0.0f ? -0.5f : 0.5f)));
}

static void GL_APIENTRY MK_glPolygonMode(GLenum face, GLenum mode)
{
    WG_Record("glPolygonMode", 2, (long long)face, (long long)mode);
}

static void GL_APIENTRY MK_glDepthMask(GLboolean on)
{
    WG_Record("glDepthMask", 1, (long long)on);
}

static void GL_APIENTRY MK_glColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
    WG_Record("glColorMask", 4, (long long)r, (long long)g, (long long)b,
              (long long)a);
}

static void GL_APIENTRY MK_glGenerateMipmap(GLenum target)
{
    WG_Record("glGenerateMipmap", 1, (long long)target);
}

/* The last pixels any upload was handed, so a case can check that the
   rows arrived in the right order rather than only that a call
   happened. */
static unsigned char wg_pixels[64 * 64 * 4];
static int           wg_pixelBytes;

static void WG_KeepPixels(const void* pixels, int bytes)
{
    wg_pixelBytes = 0;
    if (!pixels || bytes <= 0 || bytes > (int)sizeof(wg_pixels))
        return;
    memcpy(wg_pixels, pixels, (size_t)bytes);
    wg_pixelBytes = bytes;
}

/* How wide a pixel is in the format the renderer handed over. The
   glyph atlas uploads one byte a pixel and a texture four, so taking
   four either way walks off the end of the atlas's coverage array --
   which is how this was first written. */
static int WG_i_PixelBytes(GLenum format)
{
    return format == GL_RED ? 1 : 4;
}

static void GL_APIENTRY MK_glTexImage2D(GLenum target, GLint level, GLint internalFormat,
                            GLsizei w, GLsizei h, GLint border, GLenum format,
                            GLenum type, const void* pixels)
{
    WG_KeepPixels(pixels, WG_i_PixelBytes(format) * w * h);
    WG_Record("glTexImage2D", 8, (long long)target, (long long)level,
              (long long)internalFormat, (long long)w, (long long)h,
              (long long)border, (long long)format, (long long)type);
}

static void GL_APIENTRY MK_glTexSubImage2D(GLenum target, GLint level, GLint x, GLint y,
                               GLsizei w, GLsizei h, GLenum format,
                               GLenum type, const void* pixels)
{
    WG_KeepPixels(pixels, WG_i_PixelBytes(format) * w * h);
    WG_Record("glTexSubImage2D", 8, (long long)target, (long long)level,
              (long long)x, (long long)y, (long long)w, (long long)h,
              (long long)format, (long long)type);
}

static void GL_APIENTRY MK_glTexParameterf(GLenum target, GLenum pname, GLfloat v)
{
    WG_Record("glTexParameterf", 3, (long long)target, (long long)pname,
              (long long)(v * 1000.0f));
}

/* ----------------------------------------- framebuffers, mocked

   Names come from the same counter as everything else, so a case can
   say which framebuffer was bound and not merely that one was. */

static GLuint wg_boundFbo;
static GLuint wg_boundRbo;
/* Which framebuffer each colour attachment went to, so a case can
   check that a target samples the texture it was drawn into. */
static GLuint wg_fboColor[64];
static GLuint wg_fboDepth[64];
/* Made incomplete on demand, to drive the arm that gives up. */
static int    wg_fboIncomplete;

static void GL_APIENTRY MK_glGenFramebuffers(GLsizei n, GLuint* out)
{
    GLsizei i;

    for (i = 0; i < n; i++)
        out[i] = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenFramebuffers", 1, (long long)n);
}

static void GL_APIENTRY MK_glDeleteFramebuffers(GLsizei n, const GLuint* in)
{
    WG_Record("glDeleteFramebuffers", 2, (long long)n, (long long)in[0]);
}

static void GL_APIENTRY MK_glBindFramebuffer(GLenum target, GLuint fb)
{
    wg_boundFbo = fb;
    WG_Record("glBindFramebuffer", 2, (long long)target, (long long)fb);
}

static void GL_APIENTRY MK_glFramebufferTexture2D(GLenum target, GLenum attachment,
                                      GLenum texTarget, GLuint texture,
                                      GLint level)
{
    if (wg_boundFbo < 64)
        wg_fboColor[wg_boundFbo] = texture;
    WG_Record("glFramebufferTexture2D", 5, (long long)target,
              (long long)attachment, (long long)texTarget,
              (long long)texture, (long long)level);
}

static GLenum GL_APIENTRY MK_glCheckFramebufferStatus(GLenum target)
{
    WG_Record("glCheckFramebufferStatus", 1, (long long)target);
    return wg_fboIncomplete ? 0 : GL_FRAMEBUFFER_COMPLETE;
}

static void GL_APIENTRY MK_glGenRenderbuffers(GLsizei n, GLuint* out)
{
    GLsizei i;

    for (i = 0; i < n; i++)
        out[i] = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenRenderbuffers", 1, (long long)n);
}

static void GL_APIENTRY MK_glDeleteRenderbuffers(GLsizei n, const GLuint* in)
{
    WG_Record("glDeleteRenderbuffers", 2, (long long)n, (long long)in[0]);
}

static void GL_APIENTRY MK_glBindRenderbuffer(GLenum target, GLuint rb)
{
    wg_boundRbo = rb;
    WG_Record("glBindRenderbuffer", 2, (long long)target, (long long)rb);
}

static void GL_APIENTRY MK_glRenderbufferStorage(GLenum target, GLenum format,
                                     GLsizei w, GLsizei h)
{
    WG_Record("glRenderbufferStorage", 4, (long long)target,
              (long long)format, (long long)w, (long long)h);
}

static void GL_APIENTRY MK_glFramebufferRenderbuffer(GLenum target, GLenum attachment,
                                         GLenum rbTarget, GLuint rb)
{
    if (wg_boundFbo < 64)
        wg_fboDepth[wg_boundFbo] = rb;
    WG_Record("glFramebufferRenderbuffer", 4, (long long)target,
              (long long)attachment, (long long)rbTarget, (long long)rb);
}

/* The viewport the renderer asks for back. Answered from whatever the
   last glViewport set, so a save and restore is a real round trip
   rather than a constant the test also writes. */
static void GL_APIENTRY MK_glGetIntegerv(GLenum pname, GLint* data)
{
    int i;

    WG_Record("glGetIntegerv", 1, (long long)pname);
    if (pname != GL_VIEWPORT || !data)
        return;
    for (i = 0; i < 4; i++)
        data[i] = wg_viewport[i];
}

static void GL_APIENTRY MK_glGenBuffers(GLsizei n, GLuint* out)
{
    (void)n;
    *out = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenBuffers", 1, (long long)*out);
}

static void GL_APIENTRY MK_glGenVertexArrays(GLsizei n, GLuint* out)
{
    (void)n;
    *out = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenVertexArrays", 1, (long long)*out);
}

static void GL_APIENTRY MK_glGenTextures(GLsizei n, GLuint* out)
{
    (void)n;
    *out = wg_genFails ? 0 : (GLuint)wg_nextName++;
    WG_Record("glGenTextures", 1, (long long)*out);
}

static void GL_APIENTRY MK_glDeleteBuffers(GLsizei n, const GLuint* in)
{
    (void)n;
    WG_Record("glDeleteBuffers", 1, (long long)*in);
}

static void GL_APIENTRY MK_glDeleteVertexArrays(GLsizei n, const GLuint* in)
{
    (void)n;
    WG_Record("glDeleteVertexArrays", 1, (long long)*in);
}

static void GL_APIENTRY MK_glDeleteTextures(GLsizei n, const GLuint* in)
{
    (void)n;
    WG_Record("glDeleteTextures", 1, (long long)*in);
}

/* the shader half, enough for the loader to succeed */
/* The sun's readback. Not driven here -- the sun has its own cases in
   the render harness, where a real driver can actually finish a read --
   but every entry in glcore.h's list has to be aimed at something, and
   a null pointer is a crash rather than a missing feature. */
static void GL_APIENTRY MK_glReadPixels(GLint x, GLint y, GLsizei w, GLsizei h,
                                        GLenum format, GLenum type, void* data)
{
    (void)format;
    (void)type;
    (void)data;
    WG_Record("glReadPixels", 4, (long long)x, (long long)y,
              (long long)w, (long long)h);
}

static GLsync GL_APIENTRY MK_glFenceSync(GLenum condition, GLbitfield flags)
{
    WG_Record("glFenceSync", 2, (long long)condition, (long long)flags);
    return (GLsync)(size_t)0xF0;
}

static GLenum GL_APIENTRY MK_glClientWaitSync(GLsync sync, GLbitfield flags,
                                              GLuint64 timeout)
{
    (void)sync;
    (void)timeout;
    WG_Record("glClientWaitSync", 1, (long long)flags);
    return GL_TIMEOUT_EXPIRED;
}

static void GL_APIENTRY MK_glDeleteSync(GLsync sync)
{
    (void)sync;
    WG_Record("glDeleteSync", 0);
}

static void* GL_APIENTRY MK_glMapBufferRange(GLenum target, GLintptr offset,
                                             GLsizeiptr length,
                                             GLbitfield access)
{
    (void)offset;
    (void)length;
    WG_Record("glMapBufferRange", 2, (long long)target, (long long)access);
    return NULL;
}

static GLboolean GL_APIENTRY MK_glUnmapBuffer(GLenum target)
{
    WG_Record("glUnmapBuffer", 1, (long long)target);
    return GL_TRUE;
}

static void GL_APIENTRY MK_glUniform2f(GLint loc, GLfloat a, GLfloat b)
{
    WG_Record("glUniform2f", 3, (long long)loc,
              (long long)(a * 255.0f + 0.5f),
              (long long)(b * 255.0f + 0.5f));
    /* And at a million, for the zoom's crop step, which is a small
       fraction of the texture and would record as nought as a byte. */
    WG_Record("glUniform2f.scaled", 3, (long long)loc,
              (long long)(a * 1000000.0f + (a < 0.0f ? -0.5f : 0.5f)),
              (long long)(b * 1000000.0f + (b < 0.0f ? -0.5f : 0.5f)));
}

static void GL_APIENTRY MK_glUniform3f(GLint loc, GLfloat a, GLfloat b,
                                      GLfloat c)
{
    WG_Record("glUniform3f", 4, (long long)loc, (long long)(a * 255.0f + 0.5f),
              (long long)(b * 255.0f + 0.5f), (long long)(c * 255.0f + 0.5f));
    /* And at a million, for values that are not a byte: the
       grayscale's weights are signed and go to five places. */
    WG_Record("glUniform3f.scaled", 4, (long long)loc,
              (long long)(a * 1000000.0f + (a < 0.0f ? -0.5f : 0.5f)),
              (long long)(b * 1000000.0f + (b < 0.0f ? -0.5f : 0.5f)),
              (long long)(c * 1000000.0f + (c < 0.0f ? -0.5f : 0.5f)));
}

static void GL_APIENTRY MK_glDisableVertexAttribArray(GLuint index)
{
    WG_Record("glDisableVertexAttribArray", 1, (long long)index);
}

/* The copy that puts the scene on the screen. Its eight rectangle
   arguments are the whole of the letterbox, so they are all
   recorded. */
static void GL_APIENTRY MK_glBlitFramebuffer(GLint sx0, GLint sy0, GLint sx1,
                                             GLint sy1, GLint dx0, GLint dy0,
                                             GLint dx1, GLint dy1,
                                             GLbitfield mask, GLenum filter)
{
    (void)mask;
    (void)filter;
    WG_Record("glBlitFramebuffer", 8, (long long)sx0, (long long)sy0,
              (long long)sx1, (long long)sy1, (long long)dx0, (long long)dy0,
              (long long)dx1, (long long)dy1);
}

static GLuint GL_APIENTRY MK_glCreateShader(GLenum type)
{
    WG_Record("glCreateShader", 1, (long long)type);
    return (GLuint)wg_nextName++;
}
static void GL_APIENTRY MK_glShaderSource(GLuint s, GLsizei n, const GLchar* const* v,
                              const GLint* l)
{
    (void)v; (void)l;
    WG_Record("glShaderSource", 2, (long long)s, (long long)n);
}
static void GL_APIENTRY MK_glCompileShader(GLuint s) { WG_Record("glCompileShader", 1, (long long)s); }
static void GL_APIENTRY MK_glGetShaderiv(GLuint s, GLenum p, GLint* out)
{
    (void)s; (void)p;
    *out = wg_compileFails ? GL_FALSE : GL_TRUE;
}
static void GL_APIENTRY MK_glGetShaderInfoLog(GLuint s, GLsizei m, GLsizei* l, GLchar* log)
{
    (void)s;
    (void)m;
    log[0] = 0;
    if (l)
        *l = 0;
}
static void GL_APIENTRY MK_glDeleteShader(GLuint s) { WG_Record("glDeleteShader", 1, (long long)s); }
static GLuint GL_APIENTRY MK_glCreateProgram(void)
{
    GLuint p = (GLuint)wg_nextName++;

    WG_Record("glCreateProgram", 1, (long long)p);
    return p;
}
static void GL_APIENTRY MK_glAttachShader(GLuint p, GLuint s)
{
    WG_Record("glAttachShader", 2, (long long)p, (long long)s);
}
static void GL_APIENTRY MK_glDetachShader(GLuint p, GLuint s)
{
    WG_Record("glDetachShader", 2, (long long)p, (long long)s);
}
static void GL_APIENTRY MK_glLinkProgram(GLuint p) { WG_Record("glLinkProgram", 1, (long long)p); }
static void GL_APIENTRY MK_glGetProgramiv(GLuint p, GLenum pn, GLint* out)
{
    (void)p; (void)pn;
    *out = GL_TRUE;
}
static void GL_APIENTRY MK_glGetProgramInfoLog(GLuint p, GLsizei m, GLsizei* l, GLchar* log)
{
    (void)p;
    (void)m;
    log[0] = 0;
    if (l)
        *l = 0;
}
static void GL_APIENTRY MK_glDeleteProgram(GLuint p) { WG_Record("glDeleteProgram", 1, (long long)p); }

/* Uniform locations, by name, so a case can tell them apart and so a
   missing one can be arranged. */
static int wg_noUniform;
static GLint GL_APIENTRY MK_glGetUniformLocation(GLuint p, const GLchar* name)
{
    (void)p;
    WG_Record("glGetUniformLocation", 0);
    if (wg_noUniform)
        return -1;
    if (strcmp(name, "uViewport") == 0)
        return 1;
    if (strcmp(name, "uAtlas") == 0)
        return 2;
    if (strcmp(name, "uViewProj") == 0)
        return 3;
    if (strcmp(name, "uTex") == 0)
        return 4;
    if (strcmp(name, "uTextured") == 0)
        return 5;
    if (strcmp(name, "uAlphaMode") == 0)
        return 6;
    if (strcmp(name, "uAlphaRef") == 0)
        return 7;
    if (strcmp(name, "uDiffuse2x") == 0)
        return 8;
    return 9;
}

/* A location for each name the renderer asks for, so that a case can
   say which uniform was set and not merely that one was. */
#define WG_U_VIEWPROJ  3
#define WG_U_TEX       4
#define WG_U_TEXTURED  5
#define WG_U_ALPHAMODE 6
#define WG_U_ALPHAREF  7
#define WG_U_DIFFUSE2X 8

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
    GLFN_glDrawElements = MK_glDrawElements;
    GLFN_glBufferData = MK_glBufferData;
    GLFN_glGenBuffers = MK_glGenBuffers;
    GLFN_glBlitFramebuffer = MK_glBlitFramebuffer;
    GLFN_glCullFace = MK_glCullFace;
    GLFN_glFrontFace = MK_glFrontFace;
    GLFN_glDisableVertexAttribArray = MK_glDisableVertexAttribArray;
    GLFN_glUniform2f = MK_glUniform2f;
    GLFN_glUniform3f = MK_glUniform3f;
    GLFN_glReadPixels = MK_glReadPixels;
    GLFN_glFenceSync = MK_glFenceSync;
    GLFN_glClientWaitSync = MK_glClientWaitSync;
    GLFN_glDeleteSync = MK_glDeleteSync;
    GLFN_glMapBufferRange = MK_glMapBufferRange;
    GLFN_glUnmapBuffer = MK_glUnmapBuffer;
    GLFN_glGenVertexArrays = MK_glGenVertexArrays;
    GLFN_glDeleteBuffers = MK_glDeleteBuffers;
    GLFN_glDeleteVertexArrays = MK_glDeleteVertexArrays;
    GLFN_glGenTextures = MK_glGenTextures;
    GLFN_glDeleteTextures = MK_glDeleteTextures;
    GLFN_glBindTexture = MK_glBindTexture;
    GLFN_glActiveTexture = MK_glActiveTexture;
    GLFN_glPixelStorei = MK_glPixelStorei;
    GLFN_glTexImage2D = MK_glTexImage2D;
    GLFN_glTexSubImage2D = MK_glTexSubImage2D;
    GLFN_glTexParameteri = MK_glTexParameteri;
    GLFN_glTexParameterf = MK_glTexParameterf;
    GLFN_glGenerateMipmap = MK_glGenerateMipmap;
    GLFN_glUniform1i = MK_glUniform1i;
    GLFN_glUniform1f = MK_glUniform1f;
    GLFN_glUniform4f = MK_glUniform4f;
    GLFN_glPolygonMode = MK_glPolygonMode;
    GLFN_glDepthMask = MK_glDepthMask;
    GLFN_glColorMask = MK_glColorMask;

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
    GLFN_glUniformMatrix4fv = MK_glUniformMatrix4fv;
    GLFN_glClipControl = MK_glClipControl;
    GLFN_glDepthFunc = MK_glDepthFunc;
    GLFN_glViewport = MK_glViewport;
    GLFN_glGetIntegerv = MK_glGetIntegerv;

    GLFN_glGenFramebuffers = MK_glGenFramebuffers;
    GLFN_glDeleteFramebuffers = MK_glDeleteFramebuffers;
    GLFN_glBindFramebuffer = MK_glBindFramebuffer;
    GLFN_glFramebufferTexture2D = MK_glFramebufferTexture2D;
    GLFN_glCheckFramebufferStatus = MK_glCheckFramebufferStatus;
    GLFN_glGenRenderbuffers = MK_glGenRenderbuffers;
    GLFN_glDeleteRenderbuffers = MK_glDeleteRenderbuffers;
    GLFN_glBindRenderbuffer = MK_glBindRenderbuffer;
    GLFN_glRenderbufferStorage = MK_glRenderbufferStorage;
    GLFN_glFramebufferRenderbuffer = MK_glFramebufferRenderbuffer;

    GLFN_glClear = MK_glClear;
    GLFN_glClearColor = MK_glClearColor;
    GLFN_glClearDepth = MK_glClearDepth;
    GLFN_glClearStencil = MK_glClearStencil;
    GLFN_glScissor = MK_glScissor;
    GLFN_glFinish = MK_glFinish;
    GLFN_glFlush = MK_glFlush;
    GLFN_glGetError = MK_glGetError;
    GLFN_glGetString = MK_glGetString;
    GLFN_glDebugMessageCallback = MK_glDebugMessageCallback;
    GLFN_glDebugMessageControl = MK_glDebugMessageControl;
}

/* Every pointer the renderer could reach has to be aimed somewhere.
   An entry point added to glcore.h's list, used in win/wingl.c and
   missed above is a call through a null pointer, which lands as a
   segmentation fault in whichever case happened to reach it first and
   says nothing about why. Walking the same list that generated the
   pointers turns that into a named failure.

   There is deliberately no list of exemptions: one existed, and it
   went stale as soon as a case reached something on it. Every entry
   gets a mock, even the ones nothing here looks at. */
static void WG_CheckThePointers(void)
{
#define WG_CHECK(ret, name, params, args)                                     \
    if (!GLFN_##name)                                                         \
    {                                                                         \
        printf("  FAIL %s is in glcore.h's list and aimed at nothing\n",      \
               #name);                                                        \
        fails++;                                                              \
    }                                                                         \
    checks++;
    GL_FUNCTION_LIST(WG_CHECK)
#undef WG_CHECK
}

/* ------------------------------------------------------------- the rig */

/* The engine's display data, which the renderer reads its settings and
   the big buffer out of. */
static tGdi_DisplayData gl_dd;

/* The two display settings the projection reads. They live in gdi.c,
   which would pull the whole engine in here, so the driver defines
   them -- the values are what gdi.c has. */
int   GDI_gi_Widescreen = 1;
float GDI_gf_RefYoverX = 0.75f;
float GDI_gaf_ScreenRatio[4];

/* The mip-map level-of-detail bias, a debug tweak the renderer puts
   on every texture it binds. It lives in dbg.c, which would pull the
   whole engine in here, so the driver defines it -- the value is the
   one dbg.c has. */
float tweak_mipmaplodbias = -0.60000002f;

/* The engine's texture list, which the renderer indexes to find a
   texture's size. textures.c would bring the engine with it, so the
   driver owns it here and a case fills in the entries it needs. */
tEngTexListStruct TEX_gst_GlobalList;

/* The top of the engine's matrix stack, which is what makes a sprite
   face the camera. */
static Mtx4x4 wg_sprMtx;

/* The clock the sprite wobble reads. time.c would bring the engine
   with it, so the driver owns it. */
float TIM_gul_TicksCount;

/* math.c is linked for MATH_MakeOGLMatrix and brings its aligned
   allocators with it. Nothing here allocates a matrix array, so these
   exist to satisfy the linker. */
void* MEM_p_AllocAligned(int size, int alignment)
{
    (void)alignment;
    return malloc((size_t)size);
}

void* MEM_p_ReallocAligned(void* p, int size, int alignment)
{
    (void)alignment;
    return realloc(p, (size_t)size);
}

void MEM_FreeAlign(void* p) { free(p); }

/* The sun's sampler is one plain allocation. */
/* The engine's debug overlay. Recorded rather than run: what this
   harness can say is whether the renderer draws it at all, which is
   the thing that was missing -- it was drawn from the other
   renderer's Flip and from nowhere else, so under this one the
   rasters panel and the command field never appeared. What it draws
   once called is the engine's own and goes through the same interface
   entries this file already checks. */
static int wg_ovlDrawn;

/* The cutscene letterbox's engine state, which cinebands.c reads.
   CIN_Bands is the engine's switch; the game starts it on. */
int   CIN_Bands = 1;
int   tweak_ae_nobands = 0;
float D3D_gf_CutsceneBand = 0.10416666f;

/* The two switches that turn the whole after-effects chain off.
   Both start off, as the game does. */
int   tweak_ae_no = 0;
/* The culling override, at the nought that leaves the renderer's own
   mapping in force -- which is what every case here is written
   against. */
int   tweak_gl_cull = 0;

/* ---- the water's engine side ----
 *
 * The simulation lives in water.c, which would bring the engine with
 * it, so the driver owns the few things the renderer reads from it. A
 * case fills the grid in; the blur is counted rather than done,
 * because the renderer's contract with it is only that it runs over
 * the wave array before anything reads that array. */
/* pst_GlobalsWaterParams is not defined here: water.h already leaves
   a tentative definition of it, which is how the engine shares it
   between the simulation and the renderers. */
int   tweak_water_blur_normal = 0;
static int   wg_waterBlurCalls;
static void* wg_waterBlurArg;

void WTR_WaterBlurNormal(Vec3* normals, int rowLength, int rowCount)
{
    wg_waterBlurCalls++;
    wg_waterBlurArg = normals;
    (void)rowLength;
    (void)rowCount;
}

/* The water's own flip override, at the nought that leaves the
   renderer's choice in force. */
int   tweak_gl_water_flip = 0;

int   ECAM_gi_NoAfterEffects = 0;

/* The two colour parameters, at the neutral the game starts them at.
   Neutral is a half and not nought: left at the nought a tentative
   definition gives, every frame would come back flattened to mid
   grey and half darkened. */
float unkAEFunc2Param = 0.5f;
float colorBalanceParam1 = 0.5f;

void OVL_Draw(void)
{
    wg_ovlDrawn++;
    WG_Record("OVL_Draw", 0);
}

void* MEM_p_Alloc(unsigned int size) { return malloc((size_t)size); }
void  MEM_Free(void* p) { free(p); }

/* A context the pass will accept. There is no driver; what the pass
   checks is that it has one. */
static void reset(void)
{
    GL_l_ForgetPass();
    GL_l_ForgetGeometry();
    GL_ReleaseTextures(NULL);
    GL_l_ForgetTargets();
    GL_l_ForgetVideo();
    /* The after-effects too, or a case that makes the driver refuse
       something finds the chain already built from the case before and
       never reaches the refusal. The scene target is deliberately left
       alone: it is sized from gl_state, which reset puts back to the
       same size every time, and rebuilding it would move the name and
       call counts every case above is written against. */
    GL_l_ForgetAe();
    GL_l_ForgetWater();
    pst_GlobalsWaterParams = NULL;
    tweak_gl_water_flip = 0;
    /* And the two colour parameters back to neutral, which is a half
       for both of them. */
    unkAEFunc2Param = 0.5f;
    colorBalanceParam1 = 0.5f;
    memset(&gl_material, 0, sizeof(gl_material));
    gl_colorMaskAlpha = 0;
    gl_mipmapping = 1;
    memset(&gl_dd, 0, sizeof(gl_dd));
    gpst_DX_stDD = &gl_dd;
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
    wg_bufferCopyLen = 0;
    wg_lastBufferSize = 0;
    wg_lastMatrix = NULL;
    memset(wg_matrix, 0, sizeof(wg_matrix));
    memset(wg_matrixData, 0, sizeof(wg_matrixData));
    wg_matrixNum = 0;
    wg_lastTranspose = -1;
    wg_boundFbo = 0;
    wg_boundRbo = 0;
    wg_fboIncomplete = 0;
    memset(wg_fboColor, 0, sizeof(wg_fboColor));
    memset(wg_fboDepth, 0, sizeof(wg_fboDepth));
    wg_viewport[0] = 0;
    wg_viewport[1] = 0;
    wg_viewport[2] = 800;
    wg_viewport[3] = 600;
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

/* ------------------------------------------------- world geometry */

/* A chunk of two triangles over four vertices, with every index
   different so a mix-up shows. */
static Vec3                   wg_pos[4];
static tGeoUVElem             wg_uv[4];
static int                    wg_colors[4];
static tGeoTriangleChunkElem  wg_tris[2];
static tGeoTriangleChunks     wg_chunk;
static tBigBufferStruct       wg_bb;
static tDDCamStruct           wg_cam;

static void WG_BuildChunk(void)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        wg_pos[i].x = (float)(i + 1);
        wg_pos[i].y = (float)(i + 10);
        wg_pos[i].z = (float)(i + 100);
        wg_uv[i].u = (float)i * 0.25f;
        wg_uv[i].v = 1.0f - (float)i * 0.25f;
        wg_colors[i] = 0x11223300 + i;
    }
    wg_tris[0].v1 = 0; wg_tris[0].v2 = 1; wg_tris[0].v3 = 2;
    wg_tris[0].uv1 = 0; wg_tris[0].uv2 = 1; wg_tris[0].uv3 = 2;
    wg_tris[1].v1 = 1; wg_tris[1].v2 = 2; wg_tris[1].v3 = 3;
    wg_tris[1].uv1 = 1; wg_tris[1].uv2 = 2; wg_tris[1].uv3 = 3;
    memset(&wg_chunk, 0, sizeof(wg_chunk));
    wg_chunk.triangleNum = 2;
    wg_chunk.triangleChunkElems = wg_tris;

    memset(&wg_bb, 0, sizeof(wg_bb));
    wg_bb.colorMask = 0;
    wg_bb.solidVertexColor = 0x00804020;
    gl_dd.bigBuffer = &wg_bb;
}

/* The normal attribute is switched off again, and the normals are
 * uploaded only for the projection that reads them.
 *
 * Both are call-sequence properties rather than picture ones, which is
 * why they are here and not in the render harness: a sprite drawn with
 * the attribute still enabled reads a buffer that belongs to the last
 * piece of world geometry and may be shorter than the sprite's own
 * vertices, and nothing about the resulting picture is defined. The
 * render harness watched the renderer's own bookkeeping instead, which
 * a mutation that skipped the call alone slipped straight past.
 */
static tGEOGRO wg_gro;
static Vec3    wg_normals[4];

/* WG_l_Arg, further down this file, is the safe way to read a call's
   argument: it answers -1 when there is no such call. The idiom used
   around it dereferences what WG_pc_Call returns straight away, which
   holds only while the count assertion above it passes -- under a
   mutation that changes the count that is a crash instead of a
   failure, which is how this case first reported two of them. */
static long WG_l_Arg(const char* name, int nth, int arg);

static void caseGeometryNormals(void)
{
    int i;

    printf("-- the normal attribute is switched, not left on\n");

    reset();
    WG_BuildChunk();

    /* One draw first, to get the program and its vertex array made:
       that setup enables the three attributes the vertex has and
       disables the normal's, and counting those as the draw's own was
       how the first draft of this case failed. */
    GDI_gi_TextureUVMode = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);

    /* No chrome projection, so no normals: the upload is for the one
       mode that reads them and not for every draw. */
    wg_callLen = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    okEq(WG_i_Count("glDisableVertexAttribArray"), 0,
         "nothing to switch off when it was already off");
    okEq(WG_i_Count("glBufferData"), 1,
         "and only the vertices are uploaded");

    /* And now with them. */
    memset(&wg_gro, 0, sizeof(wg_gro));
    for (i = 0; i < 4; i++)
    {
        wg_normals[i].x = (float)i;
        wg_normals[i].y = 0.0f;
        wg_normals[i].z = 1.0f;
    }
    wg_gro.normals = wg_normals;
    wg_gro.vertexNum = 4;
    gl_dd.GROGEO = &wg_gro;

    gl_dd.GROGEO = &wg_gro;
    GDI_gi_TextureUVMode = 2;
    wg_callLen = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    okEq(WG_i_Count("glEnableVertexAttribArray"), 1,
         "the normal attribute is turned on for the chrome projection");
    okEq(WG_l_Arg("glEnableVertexAttribArray", 0, 0), 3,
         "and it is the normal's one");
    /* Two uploads: the normals and the vertices. */
    okEq(WG_i_Count("glBufferData"), 2,
         "the normals are uploaded as well as the vertices");

    /* A second draw of the same kind does not switch it again. */
    wg_callLen = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    okEq(WG_i_Count("glEnableVertexAttribArray"), 0,
         "a second such draw does not turn it on again");

    /* And a draw without them turns it off -- the call, not just the
       bookkeeping. */
    wg_callLen = 0;
    GDI_gi_TextureUVMode = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    okEq(WG_i_Count("glDisableVertexAttribArray"), 1,
         "a draw without normals turns the attribute off");
    okEq(WG_l_Arg("glDisableVertexAttribArray", 0, 0), 3,
         "and it is the normal's one");
    okEq(WG_i_Count("glBufferData"), 1,
         "with only the vertices uploaded");

    gl_dd.GROGEO = NULL;
}

/* Which winding the renderer tells OpenGL is front, for each of the
 * three things the engine can ask for.
 *
 * The table below is **measured**, on a real card in a real scene. It
 * used to be a reading of the two APIs' conventions instead, and the
 * reading was wrong: the engine's winding name does *not* carry over
 * unchanged, it inverts. The scene came out almost dark with objects
 * missing, because the front of every surface was being culled.
 *
 * This case asserted the wrong table and passed, which is the part
 * worth remembering. It was written from the same reading as the
 * renderer, so the two agreed with each other and neither was
 * checked against anything. A table copied from the code's own
 * premise does not test the premise; it only makes it harder to
 * change.
 *
 * So: if this ever has to be swapped again, it takes a card and a
 * scene to say so. The reasoning that gives the other answer is in
 * win/wingl.c, written out as the wrong answer it is.
 */
static void caseGeometryCulling(void)
{
    /* The draw mask's two culling bits, and what each should produce.
       0x800 off is no culling at all; with it on, 0x2000 chooses. */
    static const struct
    {
        int         mask;
        int         b_enabled;
        long        front;
        const char* what;
    } cases[3] = {
        { 0x10,                 0, 0,      "no culling bit, no culling" },
        { 0x10 | 0x800,         1, GL_CW,  "the engine's counter-clockwise" },
        { 0x10 | 0x800 | 0x2000, 1, GL_CCW, "the engine's clockwise" }
    };
    int i;

    printf("-- the winding the renderer calls front\n");

    for (i = 0; i < 3; i++)
    {
        reset();
        wg_callLen = 0;
        gl_dd.drawMaskAllowedStuffCurrent = cases[i].mask;
        GL_TextureInit(-1, 0, 0);

        /* Whether culling was switched on or off, by argument and not
           by call count: both names take many arguments in a draw.
           Checked as well as the winding, because a renderer that set
           the winding and never enabled culling passed the winding
           half of this table on its own. */
        {
            int j;
            int on = 0;
            int off = 0;

            for (j = 0; j < wg_callLen; j++)
            {
                if (strcmp(wg_calls[j].name, "glEnable") == 0
                    && wg_calls[j].arg[0] == GL_CULL_FACE)
                    on = 1;
                if (strcmp(wg_calls[j].name, "glDisable") == 0
                    && wg_calls[j].arg[0] == GL_CULL_FACE)
                    off = 1;
            }

            if (!cases[i].b_enabled)
            {
                okEq(WG_i_Count("glFrontFace"), 0, cases[i].what);
                okEq(WG_i_Count("glCullFace"), 0, "and no face is chosen");
                ok(off && !on, "and culling is turned off");
            }
            else
            {
                okEq(WG_i_Count("glFrontFace"), 1, cases[i].what);
                okEq(WG_l_Arg("glFrontFace", 0, 0), cases[i].front,
                     "is the winding OpenGL is told is front");
                okEq(WG_l_Arg("glCullFace", 0, 0), GL_BACK,
                     "and the back is what goes");
                ok(on && !off, "with culling turned on");
            }
        }
    }

    /* The two settings really are different, or the table above would
       hold for a renderer that ignored the bit. */
    ok(GL_CW != GL_CCW, "the two windings are two");
}

/* The overlay is drawn at the end of a frame.
 *
 * It is engine work rather than a renderer's, and it happens inside
 * each renderer's Flip because the other renderer updates the rasters
 * in its own and the overlay has to come after that. This renderer did
 * not do it, so the rasters panel and the command field were never
 * drawn at all -- which is a thing no amount of testing the overlay's
 * own entry points could notice, because they were all correct and
 * nothing called them.
 */
/* The scene target, and the copy that puts it on the screen.
 *
 * The world is drawn into a target of its own and copied at the end of
 * the frame, which is what makes the cutscene letterbox possible: the
 * bands are not drawn, they are the part of the screen the copy does
 * not reach. So what matters is the rectangle of the copy and what is
 * on the screen outside it, and both are call-sequence facts.
 */
static long wg_l_BlitArg(int idx)
{
    return WG_l_Arg("glBlitFramebuffer", 0, idx);
}

static int wg_b_ClearedToBlack(void)
{
    int i;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, "glClear") == 0)
            return 1;
    return 0;
}

/* The scene target is made by GL_Init, which cannot run here -- it
   wants a window -- so the case makes it the way Init does. Without
   it the copy has nothing to read and does nothing, which is what the
   first frame did before Init made it. */
static void wg_SceneReady(void)
{
    GL_l_BindScene();
}

static void caseSceneCopy(void)
{
    printf("-- the scene is copied to the screen, letterboxed or not\n");

    /* No letterbox: the whole height is copied and the screen is not
       cleared first, because it is about to be covered entirely. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    wg_callLen = 0;
    GL_Flip();

    okEq(WG_i_Count("glBlitFramebuffer"), 1, "one copy");
    okEq(wg_l_BlitArg(1), 0, "from the bottom of the scene");
    okEq(wg_l_BlitArg(3), gl_state.height, "to the top of it");
    okEq(wg_l_BlitArg(5), 0, "onto the bottom of the screen");
    okEq(wg_l_BlitArg(7), gl_state.height, "and the top");
    okEq(wg_l_BlitArg(2), gl_state.width, "the full width");
    ok(!wg_b_ClearedToBlack(), "and the screen is not cleared first");

    /* The letterbox: a strip, with the screen cleared so the bands are
       black rather than the frame before's picture. */
    reset();
    wg_SceneReady();
    CIN_Bands = 1;
    tweak_ae_nobands = 0;
    D3D_gf_CutsceneBand = 0.25f;
    wg_callLen = 0;
    GL_Flip();

    okEq(WG_i_Count("glBlitFramebuffer"), 1, "one copy again");
    ok(wg_b_ClearedToBlack(), "the screen is cleared, so the bands are"
                              " black");
    /* A quarter of the screen each way, measured from the bottom
       because that is where OpenGL's window origin is. */
    okEq(wg_l_BlitArg(1), gl_state.height / 4, "the strip starts a quarter up");
    okEq(wg_l_BlitArg(3), gl_state.height - gl_state.height / 4,
         "and ends a quarter down");
    okEq(wg_l_BlitArg(5), wg_l_BlitArg(1), "onto the same place on the"
                                           " screen");
    okEq(wg_l_BlitArg(7), wg_l_BlitArg(3), "at the same height, so it is"
                                           " not stretched");

    /* The tweak takes the letterbox away again. */
    reset();
    wg_SceneReady();
    CIN_Bands = 1;
    tweak_ae_nobands = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(wg_l_BlitArg(3), gl_state.height,
         "the tweak gives the whole height back");
    tweak_ae_nobands = 0;
    CIN_Bands = 0;
    D3D_gf_CutsceneBand = 0.10416666f;

    /* The overlay goes on the screen, after the copy -- not into the
       scene target, where the letterbox would cut it off. */
    reset();
    wg_SceneReady();
    wg_callLen = 0;
    GL_Flip();
    {
        int blit = WG_i_At("glBlitFramebuffer");
        int ovl = WG_i_At("OVL_Draw");

        ok(blit >= 0 && ovl > blit,
           "the overlay is drawn after the copy, so it lands on the"
           " screen");
    }

    /* And the scene target is bound again at the end, so the next
       frame draws into it without anything having to switch it on. */
    {
        int i;
        long last = -1;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glBindFramebuffer") == 0)
                last = (long)wg_calls[i].arg[1];
        ok(last > 0, "the frame ends with the scene target bound, not the"
                     " screen");
    }
}

static void caseFlipDrawsOverlay(void)
{
    printf("-- the frame ends by drawing the overlay\n");

    reset();
    wg_ovlDrawn = 0;
    wg_callLen = 0;
    GL_Flip();
    okEq(wg_ovlDrawn, 1, "Flip draws the overlay");

    /* And it goes before the buffers are swapped, or it would land on
       the frame after. SwapBuffers is the stub's own, so the order is
       read off the log. */
    {
        int ovl = WG_i_At("OVL_Draw");

        ok(ovl >= 0, "the call is in the log");
    }

    /* With no device context there is nothing to swap, and the
       overlay is still drawn: it is the engine's work and does not
       depend on this renderer having a window. */
    reset();
    gl_state.dc = NULL;
    wg_ovlDrawn = 0;
    GL_Flip();
    okEq(wg_ovlDrawn, 1, "and it is drawn whether or not there is a"
                         " surface to present");
}

static void caseGeometry(void)
{
    const tWgCall* c;

    printf("-- world geometry\n");
    reset();
    WG_BuildChunk();

    okEq(GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4), 0,
         "the draw reports what the other renderer reports");
    okEq(WG_i_Count("glCreateShader"), 2,
         "the geometry program is one pair of stages");
    okEq(WG_i_Count("glDrawArrays"), 1, "one draw");
    c = WG_pc_Call("glDrawArrays", 0);
    okEq(c->arg[0], GL_TRIANGLES, "of triangles");
    okEq(c->arg[2], 6, "two triangles is six vertices");
    okEq(wg_lastBufferSize, (long)(6 * sizeof(tGdi_TexturedVertex)),
         "uploaded as six of the engine's geometry vertices");

    /* the matrix reaches the shader untransposed, which is what makes
       GLSL read the engine's row-major storage as the transpose and
       so apply the engine's own row-vector arithmetic */
    /* Two matrices now: the combined one for the position and the
       view on its own, because the fog is a function of the distance
       from the camera and that is what the combined one has thrown
       away. The combined one goes first, which is what the check below
       reads. */
    okEq(WG_i_Count("glUniformMatrix4fv"), 2,
         "the combined matrix and the view are both set");
    c = WG_pc_Call("glUniformMatrix4fv", 0);
    okEq(c->arg[1], 1, "one matrix at a time");
    okEq(c->arg[2], GL_FALSE, "not transposed on the way");
    c = WG_pc_Call("glUniformMatrix4fv", 1);
    okEq(c->arg[2], GL_FALSE, "and the second is not transposed either");
    c = WG_pc_Call("glUniformMatrix4fv", 0);

    /* And it is view times projection, in that order. Checked against
       a product worked out here rather than by the helper the
       renderer uses, because the order is the whole question and two
       matrices multiplied the other way round is a picture that looks
       plausible and is wrong. */
    {
        Mtx4x4 view;
        Mtx4x4 proj;
        float  want[16];
        int    i, j, k;
        int    bad = 0;

        memset(&view, 0, sizeof(view));
        memset(&proj, 0, sizeof(proj));
        for (i = 0; i < 16; i++)
        {
            ((float*)&view)[i] = (float)(i + 1);
            ((float*)&proj)[i] = (float)(32 - i);
        }
        /* a view matrix with no separate scale factors, so the
           conversion leaves it alone */
        view.mtxflag = 0;

        reset();
        WG_BuildChunk();
        GL_SetViewMatrix(&view);
        memcpy(&gl_projMtx, &proj, sizeof(Mtx4x4WithoutAdditionalData));
        GL_l_CombineMatrices();
        wg_callLen = 0;
        GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);

        /* The left operand is the converted view matrix, not the one
           handed in: MATH_MakeOGLMatrix sets w to 1, which changes
           the four products of that row. What this case is about is
           the order of the multiply, so the conversion is taken as
           given rather than re-derived here. */
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
            {
                float sum = 0.0f;

                for (k = 0; k < 4; k++)
                    sum += ((const float*)&gl_viewMtx)[i * 4 + k]
                           * ((const float*)&proj)[k * 4 + j];
                want[i * 4 + j] = sum;
            }
        okEq(wg_matrixNum, 2, "two matrices were uploaded");
        ok(wg_matrix[0] != NULL, "the first is there");
        if (wg_matrix[0])
            for (i = 0; i < 16; i++)
                if (wg_matrix[0][i] != want[i])
                    bad++;
        okEq(bad, 0, "and it is the view times the projection, in that order");

        /* And the second is the view on its own, which is what the fog
           needs: handing it the product instead would compute the fog
           from the clip-space depth, and that is a mistake no picture
           of a lit room would obviously show. */
        bad = 0;
        if (wg_matrix[1])
        {
            for (i = 0; i < 16; i++)
                if (wg_matrix[1][i] != ((const float*)&gl_viewMtx)[i])
                    bad++;
        }
        else
        {
            bad = 1;
        }
        okEq(bad, 0, "and the second is the view on its own");
    }

    /* the vertices themselves: positions picked by index, and the
       solid colour used because there is no colour array */
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy : NULL);
        unsigned int want = GVX_ui_SolidColor(0, 0, 0x00804020);

        ok(v != NULL, "the upload came from the staging buffer");
        if (v)
        {
            okEq((long)v[0].pos.x, 1, "the first corner is vertex 0");
            okEq((long)v[1].pos.x, 2, "the second is vertex 1");
            okEq((long)v[2].pos.x, 3, "the third is vertex 2");
            okEq((long)v[3].pos.x, 2, "the next triangle starts at vertex 1");
            okEq((long)v[5].pos.x, 4, "and ends at vertex 3");
            okEq((long)v[0].color, (long)want,
                 "with the solid colour, packed as the engine packs it");
        }
    }

    /* nothing to draw */
    wg_callLen = 0;
    okEq(GL_DrawElementIndexedTriangles(NULL, wg_pos, wg_uv, 4), 0, "no chunk");
    okEq(GL_DrawElementIndexedTriangles(&wg_chunk, NULL, wg_uv, 4), 0, "no vertices");
    wg_chunk.triangleNum = 0;
    okEq(GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4), 0,
         "no triangles");
    okEq(WG_i_Count("glDrawArrays"), 0, "and none of them draws");
    wg_chunk.triangleNum = 2;

    /* the program is built once, not per chunk */
    wg_callLen = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    okEq(WG_i_Count("glCreateShader"), 0, "a second chunk compiles nothing");
    okEq(WG_i_Count("glDrawArrays"), 1, "and still draws");
}

static void caseViewport(void)
{
    const tWgCall* c;

    printf("-- the viewport's y is measured from the other end\n");
    reset();
    gl_state.width = 800;
    gl_state.height = 600;

    memset(&wg_cam, 0, sizeof(wg_cam));
    wg_cam.flags = 1;                 /* perspective */
    wg_cam.focale = 1.0f;
    wg_cam.nearPlane = 0.1f;
    wg_cam.farPlane = 100.0f;
    wg_cam.YoverX = 0.75f;

    /* Off-centre on purpose. A centred viewport -- which is what
       letterboxing alone produces -- has the same y measured from
       either end: the rectangle sits (height - fit) / 2 from the top
       and the same from the bottom. So a case built on letterboxing
       cannot see the flip at all, and a renderer that forgot it would
       look right until somebody used a camera with an inset or a
       split-screen viewport. The first version of this case was
       exactly that vacuous. */
    wg_cam.viewPortPosX = 0.0f;
    wg_cam.viewPortPosY = 0.25f;
    wg_cam.viewPortSizeWidth = 1.0f;
    wg_cam.viewPortSizeHeight = 0.5f;
    gl_dd.ddcs2.pixelYoverX = 1.0f;
    gl_dd.ddcs2.manualScreenRatio = 0.75f;
    gl_dd.ddcs2.selectedScreenRatio = 0;
    gl_dd.ddcs2.flags2 = 0;

    wg_callLen = 0;
    GL_SetProjectionMatrix(&wg_cam);

    okEq(WG_i_Count("glViewport"), 1, "the viewport is set");
    c = WG_pc_Call("glViewport", 0);
    /* The engine's rectangle has its origin at the top left and
       OpenGL's at the bottom left, so the y is the surface height
       less the rectangle's bottom edge. A full-height viewport gives
       zero either way, which is why this case is letterboxed. */
    okEq(c->arg[1], 600 - (wg_cam.viewPortRealPosY + c->arg[3]),
         "the y is measured from the bottom");
    ok(c->arg[1] != wg_cam.viewPortRealPosY,
       "which this camera makes different from the engine's y");
    okEq(c->arg[0], wg_cam.viewPortRealPosX, "x is unchanged");
    /* The surface is 800x600 and the camera asks for half the fitted
       height, so the viewport is half of whatever the fit came to --
       and the fit is bounded by the surface. */
    ok(c->arg[3] > 0 && c->arg[3] <= 300,
       "and the height is the camera's half of the fit");
}

static void caseNoViewport(void)
{
    printf("-- the projection that leaves the viewport alone\n");
    reset();

    memset(&wg_cam, 0, sizeof(wg_cam));
    wg_cam.flags = 0x400;             /* the 2D projection */
    wg_cam.YoverX = 0.75f;
    wg_cam.viewPortSizeWidth = 1.0f;
    wg_cam.viewPortSizeHeight = 1.0f;
    gl_dd.ddcs2.pixelYoverX = 1.0f;
    gl_dd.ddcs2.manualScreenRatio = 0.75f;

    wg_callLen = 0;
    GL_SetProjectionMatrix(&wg_cam);

    /* This camera draws into a target whose viewport was already
       chosen, and the computation says so. Without that check the
       renderer would set a viewport here, which the perspective case
       next door cannot see because its flag is the other way. */
    okEq(WG_i_Count("glViewport"), 0, "no viewport is set");
    ok(1, "and the projection is still computed");
}

static void caseClipControl(void)
{
    printf("-- the depth range\n");
    reset();
    wg_callLen = 0;

    /* GL_Init cannot run here -- it wants a window -- so the call is
       checked where it is, as the one thing in this file that makes
       the engine's own projections usable unchanged. */
    GLFN_glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    okEq(WG_pc_Call("glClipControl", 0)->arg[0], GL_LOWER_LEFT,
         "the origin stays OpenGL's own");
    okEq(WG_pc_Call("glClipControl", 0)->arg[1], GL_ZERO_TO_ONE,
         "and the depth range becomes the engine's");
}

/* ------------------------------------------------------- textures */

/* One texture's worth of fixture: the engine's list entry, the
   intermediate description the loader hands over, and the pixels. */
static tEngTexListElemStruct  wg_texElems[4];
static tEngTexIntermediateStruct wg_inter;
static unsigned char          wg_src[8 * 4 * 4];   /* 8 wide, 4 tall */

#define WG_TEX_W 8
#define WG_TEX_H 4

/* A picture whose every pixel is different, so a row order, a stride
   or a transpose that went wrong is visible rather than plausible. */
static void WG_BuildTexture(int depth, int texFlags, int type)
{
    int i;

    for (i = 0; i < WG_TEX_W * WG_TEX_H * 4; i++)
        wg_src[i] = (unsigned char)(i * 7u + 1u);

    memset(wg_texElems, 0, sizeof(wg_texElems));
    /* The engine stores these the other way round -- the element's
       width field holds the picture's height. Written here the way
       textures.c writes it, so a renderer that reads it straight
       fails. */
    wg_texElems[1].normalData.width = (int16_t)WG_TEX_H;
    wg_texElems[1].normalData.height = (int16_t)WG_TEX_W;
    TEX_gst_GlobalList.texElems = wg_texElems;
    TEX_gst_GlobalList.maxTexNum = 4;

    memset(&wg_inter, 0, sizeof(wg_inter));
    wg_inter.colorDepth2 = (uint8_t)depth;
    wg_inter.imgDataPtr = (char*)wg_src;
    wg_inter.tp.flagsUnkAndProceduralUnkb1 = (int16_t)texFlags;
    wg_inter.tp.type = (uint8_t)type;
}

static void caseTextureUpload(void)
{
    const tWgCall* c;

    printf("-- a texture reaches the card\n");
    reset();
    WG_BuildTexture(32, 0, 0);
    okEq(GL_TextureStore(&gl_dd, 3), 1, "the table is made");
    wg_callLen = 0;

    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);

    okEq(WG_i_Count("glTexImage2D"), 1, "one upload");
    c = WG_pc_Call("glTexImage2D", 0);
    okEq(c->arg[0], GL_TEXTURE_2D, "of a 2D texture");
    okEq(c->arg[1], 0, "at the full level");
    okEq(c->arg[2], GL_RGBA8, "stored as eight bits a channel");
    okEq(c->arg[3], WG_TEX_W, "as wide as the picture");
    okEq(c->arg[4], WG_TEX_H, "and as tall");
    okEq(c->arg[6], GL_BGRA,
         "read in the order the engine's words are in memory");
    okEq(c->arg[7], GL_UNSIGNED_BYTE, "a byte a channel");
    okEq(wg_pixelBytes, WG_TEX_W * WG_TEX_H * 4, "the whole picture");
    okEq(memcmp(wg_pixels, wg_src, (size_t)wg_pixelBytes), 0,
         "byte for byte as the engine built it");

    /* Rows are packed and four bytes to a pixel, which is not what a
       fresh context assumes for either setting. */
    okEq(WG_i_Count("glPixelStorei"), 2, "both pack settings are stated");

    okEq(gl_dd.texManager.paletteUsedMemory, 4 * WG_TEX_W * WG_TEX_H,
         "and the engine is told what it cost");

    /* A 24-bit source has already been widened to 32-bit words by the
       engine, so it uploads the same way and only the accounting
       differs. */
    reset();
    WG_BuildTexture(24, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    wg_callLen = 0;
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    okEq(WG_i_Count("glTexImage2D"), 1, "a 24-bit source uploads too");
    okEq(WG_pc_Call("glTexImage2D", 0)->arg[6], GL_BGRA, "the same way");
    okEq(gl_dd.texManager.paletteUsedMemory, 3 * WG_TEX_W * WG_TEX_H,
         "and is accounted at three bytes a pixel");

    /* Anything else the engine has not converted, and the other
       renderer leaves the slot empty rather than uploading rubbish. */
    reset();
    WG_BuildTexture(16, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    wg_callLen = 0;
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    okEq(WG_i_Count("glTexImage2D"), 0,
         "a depth the engine does not convert is not uploaded");
    ok(GL_l_p_Texture(1) == NULL, "and leaves no texture behind");
}

static void caseTextureLevels(void)
{
    printf("-- mip levels, and the engine's record of them\n");
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    wg_callLen = 0;
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);

    okEq(WG_i_Count("glGenerateMipmap"), 1, "a chain is generated");
    /* 8 by 4 is four levels: 8x4, 4x2, 2x1, 1x1. */
    okEq(WG_pc_Call("glTexParameteri", 0)->arg[1], GL_TEXTURE_MAX_LEVEL,
         "the top level is stated first");
    okEq(WG_pc_Call("glTexParameteri", 0)->arg[2], 3,
         "eight by four is four levels");
    ok(GL_l_p_Texture(1)->hasLevels != 0, "and the slot remembers");
    okEq(wg_inter.bmpUnkb1 & 0x10, 0x10, "the engine is told it has them");
    okEq(wg_texElems[1].texElemFlag & 0x10, 0x10, "through the list entry too");

    /* Three separate reasons a texture gets no chain, and each one has
       to reach the engine's own record as well as the card. */
    {
        static const struct { const char* why; int flags; int type; } no[] = {
            { "the texture asks for none",          0x40, 0 },
            { "it carries its own palette",        0x200, 0 },
            { "it is the kind that is never built",    0, 5 },
        };
        unsigned int i;

        for (i = 0; i < sizeof(no) / sizeof(no[0]); i++)
        {
            reset();
            WG_BuildTexture(32, no[i].flags, no[i].type);
            GL_TextureStore(&gl_dd, 3);
            wg_callLen = 0;
            GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
            okEq(WG_i_Count("glGenerateMipmap"), 0, no[i].why);
            okEq(WG_pc_Call("glTexParameteri", 0)->arg[2], 0,
                 "and the top level is the only one");
            okEq(wg_inter.bmpUnkb1 & 0x10, 0, "the engine is told so");
            okEq(wg_texElems[1].texElemFlag & 0x10, 0, "through the entry");
        }
    }

    /* And the renderer's own switch, which is the fourth reason. */
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    gl_mipmapping = 0;
    wg_callLen = 0;
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    okEq(WG_i_Count("glGenerateMipmap"), 0,
         "the renderer can refuse chains outright");
}

/* Binds texture 1 after a material with these flags, and leaves the
   call log holding what that did. */
static void WG_Material(unsigned int flags, int drawMask, int textureID)
{
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    gl_dd.drawMaskAllowedStuffCurrent = drawMask;
    wg_callLen = 0;
    GL_TextureInit(textureID, (int)flags, 0);
}

/* What a glUniform1i aimed at this location last set, or -1. */
static long WG_l_Uniform1i(int location)
{
    int i;
    long v = -1;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, "glUniform1i") == 0
            && wg_calls[i].arg[0] == (long long)location)
            v = (long)wg_calls[i].arg[1];
    return v;
}

/* Whether a capability was enabled (1), disabled (0), or never
   mentioned (-1) as of the nth call of `before` -- or of the whole
   log when `before` is NULL. The cutoff matters: a pass that turns
   something off to draw and puts it back afterwards reads as "on" if
   only the last mention counts, and what is being asked is what was
   in force at the draw. */
static int WG_i_b_CapabilityAt(GLenum cap, const char* before)
{
    int limit = wg_callLen;
    int i;
    int state = -1;

    if (before)
    {
        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, before) == 0)
            {
                limit = i;
                break;
            }
    }
    for (i = 0; i < limit; i++)
    {
        if (wg_calls[i].arg[0] != (long long)cap)
            continue;
        if (strcmp(wg_calls[i].name, "glEnable") == 0)
            state = 1;
        else if (strcmp(wg_calls[i].name, "glDisable") == 0)
            state = 0;
    }
    return state;
}

static int WG_i_b_Capability(GLenum cap)
{
    int i;
    int state = -1;

    for (i = 0; i < wg_callLen; i++)
    {
        if (wg_calls[i].arg[0] != (long long)cap)
            continue;
        if (strcmp(wg_calls[i].name, "glEnable") == 0)
            state = 1;
        else if (strcmp(wg_calls[i].name, "glDisable") == 0)
            state = 0;
    }
    return state;
}

/* The value a glTexParameteri of this name was given, or -1. */
static long WG_l_TexParam(GLenum pname)
{
    int i;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, "glTexParameteri") == 0
            && wg_calls[i].arg[1] == (long long)pname)
            return (long)wg_calls[i].arg[2];
    return -1;
}

static void caseSampler(void)
{
    printf("-- the sampler a material asks for\n");

    /* Both addressing bits clear is clamp on both axes, and each bit
       belongs to one axis -- which is the pair most easily swapped. */
    WG_Material(0, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE, "u clamps");
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE, "v clamps");
    WG_Material(1, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_S), GL_REPEAT, "the first bit is u");
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE, "and not v");
    WG_Material(2, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE, "the second is v");
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_T), GL_REPEAT, "and not u");

    /* The filter and the mip filter are two bits and four min filters,
       and the mag filter only ever sees the first of them. */
    WG_Material(0, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_NEAREST_MIPMAP_NEAREST,
         "neither bit is nearest throughout");
    okEq(WG_l_TexParam(GL_TEXTURE_MAG_FILTER), GL_NEAREST, "magnified too");
    WG_Material(4, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_LINEAR_MIPMAP_NEAREST,
         "the first bit is the filter within a level");
    okEq(WG_l_TexParam(GL_TEXTURE_MAG_FILTER), GL_LINEAR, "which magnifies too");
    WG_Material(8, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_NEAREST_MIPMAP_LINEAR,
         "the second is the filter between levels");
    okEq(WG_l_TexParam(GL_TEXTURE_MAG_FILTER), GL_NEAREST,
         "which magnification cannot see");
    WG_Material(12, 0, 1);
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_LINEAR_MIPMAP_LINEAR,
         "both bits is linear throughout");

    /* The trap: a mip-filtered sampler on a texture with only the full
       level makes it incomplete, and an incomplete texture samples as
       black rather than failing. So the mip part is dropped when the
       chain was never built. */
    reset();
    WG_BuildTexture(32, 0x40, 0);     /* a texture that gets no chain */
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    wg_callLen = 0;
    GL_TextureInit(1, 12, 0);         /* both filter bits set */
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_LINEAR,
         "a texture with no chain gets a sampler that wants none");

    /* The debug tweak goes on every texture the renderer binds. */
    WG_Material(0, 0, 1);
    okEq(WG_i_Count("glTexParameterf"), 1, "and the level-of-detail bias");
    okEq(WG_pc_Call("glTexParameterf", 0)->arg[1], GL_TEXTURE_LOD_BIAS,
         "is what it sets");
    okEq(WG_pc_Call("glTexParameterf", 0)->arg[2], -600,
         "at the value the debug module holds");
}

static void caseMaterialState(void)
{
    printf("-- the pipeline state a material asks for\n");

    /* Depth: the comparison, the write, and whether it tests at all --
       the last of which comes from the draw mask and not the flags. */
    WG_Material(0, 0, 1);
    okEq(WG_pc_Call("glDepthFunc", 0)->arg[0], GL_LEQUAL, "nearer wins");
    okEq(WG_pc_Call("glDepthMask", 0)->arg[0], GL_TRUE, "and depth is written");
    okEq(WG_i_b_Capability(GL_DEPTH_TEST), 0,
         "a pass that does not allow depth does not test it");
    WG_Material(0, 0x1000000, 1);
    okEq(WG_i_b_Capability(GL_DEPTH_TEST), 1, "and one that does, does");
    WG_Material(0x100, 0, 1);
    okEq(WG_pc_Call("glDepthFunc", 0)->arg[0], GL_EQUAL,
         "or only what is already at that depth");
    WG_Material(0x200, 0, 1);
    okEq(WG_pc_Call("glDepthMask", 0)->arg[0], GL_FALSE,
         "and a material can draw without writing depth");

    /* Colour write, which is two bits and both of them inverted. */
    WG_Material(0, 0, 1);
    okEq(WG_pc_Call("glColorMask", 0)->arg[0], GL_TRUE, "colour is written");
    okEq(WG_pc_Call("glColorMask", 0)->arg[3], GL_TRUE, "and alpha with it");
    WG_Material(0x20, 0, 1);
    okEq(WG_pc_Call("glColorMask", 0)->arg[0], GL_TRUE, "one bit stops alpha");
    okEq(WG_pc_Call("glColorMask", 0)->arg[3], GL_FALSE, "and only alpha");
    WG_Material(0x40, 0, 1);
    okEq(WG_pc_Call("glColorMask", 0)->arg[0], GL_FALSE, "the other stops colour");
    okEq(WG_pc_Call("glColorMask", 0)->arg[3], GL_TRUE, "and only colour");

    /* The fill mode, which the pass allows rather than the material. */
    WG_Material(0, 0, 1);
    okEq(WG_pc_Call("glPolygonMode", 0)->arg[1], GL_LINE,
         "a pass that does not allow solid fill draws edges");
    WG_Material(0, 0x10, 1);
    okEq(WG_pc_Call("glPolygonMode", 0)->arg[1], GL_FILL, "and otherwise faces");

    /* The blend equations the mode selects, and the one bit that
       reverses four of them. */
    WG_Material(0u << 16, 0, 1);
    okEq(WG_i_Count("glBlendFunc"), 1, "mode zero states a blend");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[0], GL_ONE, "of source only");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[1], GL_ZERO, "over nothing");
    WG_Material(1u << 16, 0, 1);
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[0], GL_SRC_ALPHA,
         "mode one blends on the source's alpha");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[1], GL_ONE_MINUS_SRC_ALPHA,
         "against its complement");
    WG_Material((1u << 16) | 0x80, 0, 1);
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[0], GL_ONE_MINUS_SRC_ALPHA,
         "and the one reversing bit turns it around");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[1], GL_SRC_ALPHA, "both ways");
    WG_Material(5u << 16, 0, 1);
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[0], GL_ONE, "mode five adds");
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[1], GL_ONE, "both unchanged");
    WG_Material(6u << 16, 0, 1);
    okEq(WG_pc_Call("glBlendFunc", 0)->arg[1], GL_ONE_MINUS_SRC_COLOR,
         "mode six multiplies by the complement of the source");
    WG_Material(9u << 16, 0, 1);
    okEq(WG_i_Count("glBlendFunc"), 0,
         "and a mode that names no equation leaves the last one standing");

    /* The alpha the vertex colours are combined with. */
    WG_Material(0, 0, 1);
    okEq(gl_colorMaskAlpha, 0, "no masked alpha by default");
    WG_Material(0x400, 0, 1);
    okEq((long)(unsigned int)gl_colorMaskAlpha, 0xFF000000L,
         "and one bit makes it opaque");

    /* And it has to reach the vertices, which is a step further than
       holding it: the builder combines it into every colour it packs,
       and a renderer that keeps it and then hands the builder a zero
       looks right in every state check. */
    {
        const tGdi_TexturedVertex* v;

        WG_BuildChunk();
        wg_callLen = 0;
        GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
        v = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                          : NULL);
        ok(v != NULL, "a draw after that material uploaded vertices");
        if (v)
        {
            okEq((long)v[0].color,
                 (long)GVX_ui_SolidColor(0, 0xFF000000, 0x00804020),
                 "whose colour carries the material's alpha");
            /* and plainly not the colour it would have without it */
            ok(v[0].color != GVX_ui_SolidColor(0, 0, 0x00804020),
               "which is not the colour it would have had otherwise");
        }
    }
}

static void caseAlphaTest(void)
{
    const tWgCall* c;

    printf("-- the alpha test, which the shader does\n");

    /* Off, and then each of the two senses, with a reference value
       that is not a round number. 0x2A in bits 24..29 of the word. */
    WG_Material(0, 0, 1);
    okEq(gl_material.alphaMode, 0, "no test by default");
    WG_Material(0x10 | (0x2Au << 24), 0, 1);
    okEq(gl_material.alphaMode, 1, "one bit turns it on, keeping what is above");
    okEq((long)(gl_material.alphaRef * 255.0f + 0.5f), 0xA8,
         "at the reference the top bits hold, less its lowest two");
    WG_Material(0x10 | 0x80 | (0x2Au << 24), 0, 1);
    okEq(gl_material.alphaMode, 2, "and the reversing bit keeps what is below");

    /* And it has to reach the shader, which only happens at the draw. */
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    WG_BuildChunk();
    gl_dd.drawMaskAllowedStuffCurrent = 0x10;
    GL_TextureInit(1, 0x10 | (0x2Au << 24), 0);
    wg_callLen = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);

    c = WG_pc_Call("glUniform1f", 0);
    ok(c != NULL, "the reference is uploaded");
    if (c)
    {
        okEq(c->arg[0], WG_U_ALPHAREF, "to the uniform that holds it");
        okEq(c->arg[1], 0xA8, "normalised from the engine's byte");
    }
    okEq(WG_l_Uniform1i(WG_U_ALPHAMODE), 1, "and the sense with it");
    okEq(WG_l_Uniform1i(WG_U_TEXTURED), 1, "and that there is a texture");
    okEq(WG_l_Uniform1i(WG_U_TEX), 0, "bound to unit zero");
    okEq(WG_l_Uniform1i(WG_U_DIFFUSE2X), 0, "and that the colour is not doubled");

    /* The doubling is a display setting the engine can change between
       two draws, so it is read at the draw and not with the material. */
    GDI_gi_Diffuse2x = 1;
    wg_callLen = 0;
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    okEq(WG_l_Uniform1i(WG_U_DIFFUSE2X), 1,
         "and the setting reaches the next draw with no new material");
    GDI_gi_Diffuse2x = 0;
}

static void caseUntextured(void)
{
    printf("-- a material with no texture\n");

    /* -1 is what the engine passes when the colour is the vertex's
       alone. The two other out-of-range indices are the renderer's own
       render target and a shadow map, neither of which this renderer
       has yet -- so all three have to leave it untextured rather than
       index the table. */
    WG_Material(0, 0, -1);
    okEq(WG_i_Count("glBindTexture"), 0, "nothing is bound");
    okEq(gl_material.textured, 0, "and the shader is told so");
    WG_Material(0, 0, -2);
    okEq(gl_material.textured, 0, "nor for the renderer's own copy");
    WG_Material(0, 0, (int)0x80000001u);
    okEq(gl_material.textured, 0, "nor for a shadow map");
    /* An index inside the table but never loaded is the same case, and
       the one most likely to be let through. */
    WG_Material(0, 0, 2);
    okEq(gl_material.textured, 0, "nor an index with no texture in it");
    WG_Material(0, 0, 99);
    okEq(gl_material.textured, 0, "nor one past the end of the table");
}

static void caseProceduralUpload(void)
{
    tTexUploadRequest req;

    printf("-- a procedural texture re-uploaded\n");
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);

    req.idx = 1;
    req.dataPtr = (uint32_t*)wg_src;
    wg_callLen = 0;
    okEq(GL_Request(12, &req), 0, "the request is answered");

    okEq(WG_i_Count("glTexImage2D"), 0,
         "an existing texture is not made again");
    okEq(WG_i_Count("glTexSubImage2D"), 1, "its pixels are replaced");
    okEq(WG_pc_Call("glTexSubImage2D", 0)->arg[4], WG_TEX_W, "at full width");
    okEq(WG_pc_Call("glTexSubImage2D", 0)->arg[5], WG_TEX_H, "and height");
    okEq(WG_pc_Call("glTexSubImage2D", 0)->arg[6], GL_BGRA, "in the same order");
    okEq(memcmp(wg_pixels, wg_src, WG_TEX_W * WG_TEX_H * 4), 0,
         "byte for byte");
    okEq(WG_i_Count("glGenerateMipmap"), 1,
         "and the chain is rebuilt, because the pixels changed under it");

    /* A slot the engine never loaded gets a texture of its own rather
       than a replacement into nothing. */
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    req.idx = 1;
    req.dataPtr = (uint32_t*)wg_src;
    wg_callLen = 0;
    GL_Request(12, &req);
    okEq(WG_i_Count("glTexImage2D"), 1, "an empty slot is filled");
    okEq(WG_i_Count("glTexSubImage2D"), 0, "not replaced");

    /* An index the table does not have must not be written through. */
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    req.idx = 99;
    wg_callLen = 0;
    GL_Request(12, &req);
    okEq(WG_i_Count("glTexImage2D") + WG_i_Count("glTexSubImage2D"), 0,
         "an index past the table is refused");

    /* And a request type this renderer does not handle is harmless. */
    wg_callLen = 0;
    okEq(GL_Request(25, (void*)(size_t)1), 0, "an unhandled request answers");
    okEq(wg_callLen, 0, "and does nothing");
}

static void caseTextureTeardown(void)
{
    printf("-- giving the textures back\n");
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    wg_callLen = 0;

    GL_ReleaseTextures(&gl_dd);
    okEq(WG_i_Count("glDeleteTextures"), 1, "the one texture is deleted");
    ok(gl_textures == NULL, "and the table with it");

    /* Twice in a row, which happens because the display teardown and a
       world change both do it. */
    wg_callLen = 0;
    GL_ReleaseTextures(&gl_dd);
    okEq(WG_i_Count("glDeleteTextures"), 0, "a second release deletes nothing");

    /* And making the table again releases what was there, so a world
       change does not leak the previous world's textures. */
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    wg_callLen = 0;
    GL_TextureStore(&gl_dd, 3);
    okEq(WG_i_Count("glDeleteTextures"), 1,
         "a new table gives the old one's textures back");
}

/* -------------------------------------------------- render targets */

/* The value a named call's nth argument had, or -1. */
static long WG_l_Arg(const char* name, int nth, int arg)
{
    const tWgCall* c = WG_pc_Call(name, nth);

    return c ? (long)c->arg[arg] : -1;
}

static void caseTargetsBuild(void)
{
    printf("-- the shadow targets are built once\n");
    reset();
    wg_callLen = 0;

    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);

    okEq(WG_i_Count("glGenRenderbuffers"), 1,
         "one depth buffer for all of them, because one is bound at a time");
    okEq(WG_l_Arg("glRenderbufferStorage", 0, 1), GL_DEPTH_COMPONENT24,
         "with depth in it");
    okEq(WG_l_Arg("glRenderbufferStorage", 0, 2), 128, "a hundred and twenty");
    okEq(WG_l_Arg("glRenderbufferStorage", 0, 3), 128, "eight square");

    okEq(WG_i_Count("glGenFramebuffers"), 8, "eight targets");
    okEq(WG_i_Count("glFramebufferTexture2D"), 8, "each with a colour texture");
    okEq(WG_i_Count("glFramebufferRenderbuffer"), 8,
         "and each sharing the one depth buffer");
    okEq(WG_l_Arg("glFramebufferTexture2D", 0, 1), GL_COLOR_ATTACHMENT0,
         "the texture is the colour attachment");
    okEq(WG_l_Arg("glFramebufferRenderbuffer", 0, 1), GL_DEPTH_ATTACHMENT,
         "and the buffer the depth one");
    okEq(WG_i_Count("glCheckFramebufferStatus"), 8,
         "each is checked, because an incomplete one draws nothing quietly");

    /* The colour textures: the size the other renderer gives them, and
       a sampler that does not ask for levels they do not have. */
    okEq(WG_l_Arg("glTexImage2D", 0, 3), 128, "the colour texture is square");
    okEq(WG_l_Arg("glTexImage2D", 0, 4), 128, "at a hundred and twenty eight");
    okEq(WG_l_Arg("glTexImage2D", 0, 2), GL_RGBA8, "eight bits a channel");
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_LINEAR,
         "and a sampler that wants no mip chain");
    okEq(WG_l_TexParam(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE,
         "clamped, which is what makes the border matter");

    /* White, not black: a shadow target says how much light reaches a
       point, so an untouched one has to mean unshadowed. */
    okEq(WG_i_Count("glClearColor"), 8,
         "one white fill each, and no clear because none was asked for");
    okEq(WG_l_Arg("glClearColor", 0, 0), 1000000, "the fill is white");
    okEq(WG_l_Arg("glClearColor", 0, 3), 1000000, "opaque");

    /* And not again on the next bind. */
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(1, 0);
    okEq(WG_i_Count("glGenFramebuffers"), 0, "a second bind builds nothing");
    okEq(WG_i_Count("glGenRenderbuffers"), 0, "and no second depth buffer");
}

static void caseTargetsBind(void)
{
    printf("-- binding one, and putting back what was there\n");
    reset();
    /* a viewport that is not the whole window and not square, so a
       restore that rebuilds it from the window size rather than
       remembering it comes out wrong */
    GLFN_glViewport(37, 11, 500, 250);
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);   /* build them */
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    GLFN_glViewport(37, 11, 500, 250);
    wg_callLen = 0;

    GL_SetRenderTargetAndBackupPreviousSDW(3, 0);
    okEq(WG_i_Count("glBindFramebuffer"), 1, "one framebuffer is bound");
    ok(WG_l_Arg("glBindFramebuffer", 0, 1) == (long)gl_shadowTargets[3].fbo,
       "the one the engine asked for");
    okEq(WG_i_Count("glGetIntegerv"), 1, "the viewport is read back");
    okEq(WG_l_Arg("glGetIntegerv", 0, 0), GL_VIEWPORT, "and it is the viewport");
    okEq(WG_i_Count("glClear"), 0, "nothing is cleared without being asked");

    /* The border: inset by one on every side, so the edge pixels keep
       what the fill put there and the clamped sampler spreads that
       outwards instead of spreading the shadow. */
    okEq(WG_l_Arg("glViewport", 0, 0), 1, "the viewport is inset in x");
    okEq(WG_l_Arg("glViewport", 0, 1), 1, "and in y");
    okEq(WG_l_Arg("glViewport", 0, 2), 126, "leaving a one-pixel border");
    okEq(WG_l_Arg("glViewport", 0, 3), 126, "on all four sides");

    /* And back -- to the scene target, which is what the world draws
       into, and not to the screen. The screen is bound only for the
       copy at the end of a frame, so restoring to it would have the
       rest of the frame drawn where the copy is about to overwrite
       it. */
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    ok(WG_l_Arg("glBindFramebuffer", 0, 1) > 0,
       "the scene target is bound again, not the screen");
    okEq(WG_l_Arg("glViewport", 0, 0), 37, "and the viewport comes back");
    okEq(WG_l_Arg("glViewport", 0, 1), 11, "exactly");
    okEq(WG_l_Arg("glViewport", 0, 2), 500, "as it was");
    okEq(WG_l_Arg("glViewport", 0, 3), 250, "in all four numbers");

    /* Asked to clear. */
    reset();
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(2, 1);
    okEq(WG_i_Count("glClear"), 1, "a clear is asked for");
    okEq(WG_l_Arg("glClear", 0, 0), GL_COLOR_BUFFER_BIT,
         "of the colour only, as the other renderer clears");
    okEq(WG_l_Arg("glClearColor", 0, 0), 0, "to black");
}

static void caseTargetsRefusals(void)
{
    printf("-- the binds that do nothing\n");

    /* Nesting. The other renderer refuses a second request while one
       is in force, and so does this -- otherwise the saved viewport
       and projection would be the shadow pass's own. */
    reset();
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(1, 1);
    okEq(wg_callLen, 0, "a second target while one is in force does nothing");

    /* Releasing with none in force. */
    reset();
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    okEq(wg_callLen, 0, "and so does a second release");

    /* An index past the eight. */
    reset();
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(8, 0);
    okEq(WG_i_Count("glBindFramebuffer"), 0, "an index past the eight binds none");
    GL_SetRenderTargetAndBackupPreviousSDW(-2, 0);
    okEq(WG_i_Count("glBindFramebuffer"), 0, "and nor does a negative one");

    /* A framebuffer the driver will not complete. Giving up has to
       leave nothing bound and nothing to put back, and it must not be
       retried on every shadow of every frame. */
    reset();
    wg_fboIncomplete = 1;
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    okEq(WG_l_Arg("glBindFramebuffer", WG_i_Count("glBindFramebuffer") - 1, 1), 0,
         "an incomplete framebuffer leaves the window bound");
    okEq(WG_i_Count("glViewport"), 0, "and no viewport is set");
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    okEq(wg_callLen, 0, "and it is not tried again");
    /* and the release that the engine makes anyway is harmless */
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    okEq(wg_callLen, 0, "nor is the release that follows it");
}

static void caseTargetsMatrices(void)
{
    Mtx4x4 view;
    int    i;

    printf("-- the matrices the shadow pass puts in and takes out\n");
    reset();

    /* A projection that is plainly not an identity, so the save and
       the restore are both visible. */
    memset(&gl_projMtx, 0, sizeof(gl_projMtx));
    for (i = 0; i < 4; i++)
        ((float*)&gl_projMtx)[i * 4 + i] = (float)(i + 2);

    memset(&view, 0, sizeof(view));
    view.v1.x = 1.0f;
    view.v2.y = 1.0f;
    view.v3.z = 1.0f;
    view.w = 1.0f;
    view.Pos.x = 5.0f;

    GL_SetViewMatrixSDW(&view, NULL);
    okEq((long)((float*)&gl_projMtx)[0], 1,
         "the projection becomes an identity for the shadow");
    okEq((long)((float*)&gl_projMtx)[5], 1, "on the diagonal");
    okEq((long)((float*)&gl_projMtx)[1], 0, "and nothing off it");
    okEq((long)((float*)&gl_viewProjMtx)[12], 5,
         "so the combined matrix is the view alone");

    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    okEq((long)((float*)&gl_projMtx)[0], 2, "and the projection comes back");
    okEq((long)((float*)&gl_projMtx)[5], 3, "every term of it");
    okEq((long)((float*)&gl_projMtx)[10], 4, "including the depth row");

    /* The view is deliberately not restored: the engine sets it again
       itself once the target is released, and the other renderer
       relies on that too. */
    okEq((long)((float*)&gl_viewProjMtx)[12], 10,
         "the view is left as the shadow pass set it, times the"
         " projection that came back");

    GL_SetViewMatrixSDW(NULL, NULL);
    ok(1, "a null matrix is refused rather than crashing");
}

static void caseShadowAsTexture(void)
{
    printf("-- a shadow target sampled as a texture\n");
    reset();
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    wg_callLen = 0;

    /* This is how the engine asks for one: the index with the top bit
       set, which is what the other renderer masks off. */
    GL_TextureInit((int)(0x80000000u | 3u), 0, 0);
    okEq(gl_material.textured, 1, "it is a texture");
    ok(WG_l_Arg("glBindTexture", 0, 1) == (long)gl_shadowTargets[3].color,
       "and it is the one that target was drawn into");

    /* The two negative indices that are not shadow targets. The other
       renderer masks the top bit off these too and indexes a billion
       entries past its array; this one checks the range, so they come
       out as no texture. */
    wg_callLen = 0;
    GL_TextureInit(-1, 0, 0);
    okEq(gl_material.textured, 0, "no texture is still no texture");
    GL_TextureInit(-2, 0, 0);
    okEq(gl_material.textured, 0, "and so is the renderer's own copy");
    GL_TextureInit((int)(0x80000000u | 8u), 0, 0);
    okEq(gl_material.textured, 0, "and a shadow index past the eight");

    /* A target has one level, so a material asking for mip filtering
       must not get it: a mip-filtered sampler on a single-level
       texture makes it incomplete, and an incomplete texture samples
       black -- which on a shadow target means everything it falls on
       goes dark. */
    wg_callLen = 0;
    GL_TextureInit((int)(0x80000000u | 3u), 12, 0);   /* both filter bits */
    okEq(WG_l_TexParam(GL_TEXTURE_MIN_FILTER), GL_LINEAR,
         "a shadow target gets a sampler that asks for no levels");
}

static void caseDepthReadBack(void)
{
    printf("-- what the engine reads back about depth\n");
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);

    /* The shadow pass saves the depth state, turns it off and puts it
       back. A material sets that state too, so if the material's
       setting does not go through the same place the engine reads, the
       restore puts back a value that was never in force. */
    gl_dd.drawMaskAllowedStuffCurrent = 0x1000000;
    GL_TextureInit(1, 0, 0);
    okEq(GL_i_ZEnable(), 1, "a pass that allows depth leaves it on");
    gl_dd.drawMaskAllowedStuffCurrent = 0;
    GL_TextureInit(1, 0, 0);
    okEq(GL_i_ZEnable(), 0, "and one that does not, off");

    /* the round trip the shadow pass makes */
    gl_dd.drawMaskAllowedStuffCurrent = 0x1000000;
    GL_TextureInit(1, 0, 0);
    {
        int saved = GL_i_ZEnable();

        GL_SetZEnable(0);
        okEq(GL_i_ZEnable(), 0, "turned off for the shadow");
        GL_SetZEnable(saved);
        okEq(GL_i_ZEnable(), 1, "and put back as it was");
    }

    /* The depth write is not read back through the interface, but the
       renderer reads it itself: clearing the depth buffer has to turn
       the mask on to do it and then put it back, and "back" is
       whatever was last asked for. A material that set the mask
       straight at OpenGL would leave that putting back the wrong
       value, so depth would start being written again after the next
       clear. */
    GL_TextureInit(1, 0x200, 0);        /* a material that writes no depth */
    wg_callLen = 0;
    GL_Clear(1 | 2, 0);     /* the engine's own colour and depth bits */
    okEq(WG_l_Arg("glDepthMask", WG_i_Count("glDepthMask") - 1, 0), GL_FALSE,
         "after a depth clear the mask goes back to what the material"
         " asked for");
}

static void caseTargetsTeardown(void)
{
    printf("-- giving the targets back\n");
    reset();
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    wg_callLen = 0;

    GL_l_ForgetTargets();
    okEq(WG_i_Count("glDeleteFramebuffers"), 8, "all eight are deleted");
    okEq(WG_i_Count("glDeleteRenderbuffers"), 1, "and the depth buffer once");
    okEq(WG_i_Count("glDeleteTextures"), 8, "with their colour textures");

    wg_callLen = 0;
    GL_l_ForgetTargets();
    okEq(wg_callLen, 0, "a second teardown deletes nothing");

    /* And they can be built again, which is what a display change
       does. */
    wg_callLen = 0;
    GL_SetRenderTargetAndBackupPreviousSDW(0, 0);
    okEq(WG_i_Count("glGenFramebuffers"), 8, "and built again after it");
}

static void caseShadowSequence(void)
{
    Mtx4x4 view;

    printf("-- the sequence the shadow pass actually makes\n");
    reset();
    WG_BuildChunk();
    GLFN_glViewport(0, 0, 800, 600);

    memset(&gl_projMtx, 0, sizeof(gl_projMtx));
    ((float*)&gl_projMtx)[0] = 7.0f;
    memset(&view, 0, sizeof(view));
    view.v1.x = 1.0f;
    view.v2.y = 1.0f;
    view.v3.z = 1.0f;
    view.w = 1.0f;
    wg_callLen = 0;

    /* What graphicsobjects/shadow.c does, in its order. */
    GL_SetViewMatrixSDW(&view, NULL);
    GL_TextureInit(-1, 0x2260, 0);
    GL_SetRenderTargetAndBackupPreviousSDW(5, 1);
    GL_TextureInit(-1, 0, 0);
    GL_SetZEnable(0);
    GL_SetZWriteEnable(0);
    GL_DrawElementIndexedTriangles(&wg_chunk, wg_pos, wg_uv, 4);
    GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);

    okEq(WG_i_Count("glDrawArrays"), 1, "the caster is drawn");
    ok(WG_i_At("glDrawArrays") > WG_i_At("glBindFramebuffer"),
       "after a target was bound");
    okEq(gl_material.textured, 0, "untextured, as the pass asks");

    /* And the scene target is back, with its viewport, before anything
       else is drawn. Not the screen: see the restore case above. */
    ok(WG_l_Arg("glBindFramebuffer", WG_i_Count("glBindFramebuffer") - 1, 1) > 0,
       "and the scene target is bound again at the end");
    okEq((long)((float*)&gl_projMtx)[0], 7, "with the projection restored");
    ok(wg_viewport[2] == 800 && wg_viewport[3] == 600,
       "and the viewport back to the window's");
}

/* ------------------------------------------------------- sprites */

static tGeoSpriteElem wg_sprElems[4];
static Vec3           wg_sprPos[4];
static tGeoSprites    wg_sprites;

static void WG_BuildSprites(int num)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        /* The element is a union: its low half is the index into the
           positions and the whole of it, read as a float, is the
           sprite's size. So the two are chosen together -- a high
           half of 0x3F00 makes the float about a half.

           Leaving the high half at nought, which the first draft did,
           makes the float nought: every corner then collapses onto
           the sprite's position, the quad has no area, and any two of
           the six vertices can be swapped without a test noticing. */
        wg_sprElems[i].idx = (uint16_t)i;
        wg_sprElems[i].dummy = 0x3F00;
        wg_sprPos[i].x = (float)(i + 1);
        wg_sprPos[i].y = (float)(2 * i);
        wg_sprPos[i].z = (float)(-i);
    }
    memset(&wg_sprites, 0, sizeof(wg_sprites));
    wg_sprites.num = (uint32_t)num;
    wg_sprites.spriteElems = wg_sprElems;
    wg_sprites.globalSize = 1.0f;
    wg_sprites.globalRatio = 1.0f;

    memset(&wg_bb, 0, sizeof(wg_bb));
    wg_bb.colorMask = 0;
    wg_bb.solidVertexColor = 0x00402010;
    gl_dd.bigBuffer = &wg_bb;
    MATH_SetIdentityMatrix(&wg_sprMtx);
    gl_dd.matrixStack.actMatrixElem = &wg_sprMtx;
}

static void caseSprites(void)
{
    printf("-- sprites, as two triangles each\n");
    reset();
    WG_BuildSprites(3);
    wg_callLen = 0;

    okEq(GL_DrawSprites(&wg_sprites, wg_sprPos, 4), 0,
         "the draw reports what the other renderer reports");
    okEq(WG_i_Count("glDrawArrays"), 1, "one draw for the batch");
    okEq(WG_l_Arg("glDrawArrays", 0, 2), 3 * 6,
         "six vertices a sprite, two triangles sharing two corners");
    okEq(wg_lastBufferSize, (long)(3 * 6 * sizeof(tGdi_TexturedVertex)),
         "uploaded as the same vertex the world's triangles use");

    /* The corner coordinates, which are what tells the texture which
       way up it is on the quad. */
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);

        ok(v != NULL, "the upload came from the staging buffer");
        if (v)
        {
            okEq((long)(v[0].u * 2), 2, "the first corner is at u = 1");
            okEq((long)(v[0].v * 2), 2, "and v = 1");
            okEq((long)(v[1].u * 2), 0, "the second at u = 0");
            okEq((long)(v[2].v * 2), 0, "the third at v = 0");
            /* And which corner each vertex actually holds, against
               the quad the builder produced for the same batch. The
               texture coordinates alone cannot see two positions
               swapped while their coordinates stay, which is a swap
               of two lines. */
            {
                tGdi_SpriteQuad q[4];
                tGsp_Source     src;
                int             bad = 0;
                int             k;
                static const int want[6] = { 1, 2, 3, 1, 3, 4 };

                src.positions = wg_sprPos;
                src.colors = NULL;
                src.second = NULL;
                src.mask = 0;
                src.alpha = gl_colorMaskAlpha;
                src.solidColor = (unsigned int)wg_bb.solidVertexColor;
                src.matrix = &wg_sprMtx;
                src.waveSprite = GDI_gb_WaveSprite;
                GSP_BuildQuads(q, &wg_sprites, &src);

                for (k = 0; k < 6; k++)
                {
                    const Vec3* c = (want[k] == 1) ? &q[0].v1
                                  : (want[k] == 2) ? &q[0].v2
                                  : (want[k] == 3) ? &q[0].v3
                                                   : &q[0].v4;

                    if (v[k].pos.x != c->x || v[k].pos.y != c->y
                        || v[k].pos.z != c->z)
                        bad++;
                }
                okEq(bad, 0,
                     "each vertex holds the corner it should: one, two,"
                     " three, then one, three, four");
            }

            /* The colour is turned round for the device exactly as the
               world's vertices are, so that one shader puts both
               back. */
            okEq((long)v[0].color,
                 (long)GVX_ui_DeviceColor(0x00402010u),
                 "with the colour in the device's order");
            okEq((long)v[5].color, (long)v[0].color,
                 "the same on every corner");
        }
    }

    /* Nothing to draw. */
    wg_callLen = 0;
    okEq(GL_DrawSprites(NULL, wg_sprPos, 4), 0, "no batch");
    okEq(GL_DrawSprites(&wg_sprites, NULL, 4), 0, "no positions");
    wg_sprites.num = 0;
    okEq(GL_DrawSprites(&wg_sprites, wg_sprPos, 4), 0, "no sprites");
    okEq(WG_i_Count("glDrawArrays"), 0, "and none of them draws");

    /* The alpha a material asked for has to reach the sprites too,
       through the same builder the world's vertices go through. */
    reset();
    WG_BuildTexture(32, 0, 0);
    GL_TextureStore(&gl_dd, 3);
    GL_PaletteLoad(&gl_dd, &wg_texElems[1], &wg_inter, 1);
    WG_BuildSprites(1);
    gl_dd.drawMaskAllowedStuffCurrent = 0;
    GL_TextureInit(1, 0x400, 0);         /* the bit that makes it opaque */
    wg_callLen = 0;
    GL_DrawSprites(&wg_sprites, wg_sprPos, 4);
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);

        ok(v != NULL, "a sprite was staged");
        if (v)
            okEq((long)v[0].color,
                 (long)GVX_ui_DeviceColor(0xFF000000u | 0x00402010u),
                 "whose colour carries the material's alpha");
    }

    /* And so does the wobble, which is a display setting rather than
       a material's. */
    reset();
    WG_BuildSprites(1);
    GDI_gb_WaveSprite = 0;
    GL_DrawSprites(&wg_sprites, wg_sprPos, 4);
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);
        float still = v ? v[0].pos.x : 0.0f;

        TIM_gul_TicksCount = 12.0f;
        GDI_gb_WaveSprite = 1;
        GL_DrawSprites(&wg_sprites, wg_sprPos, 4);
        v = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                          : NULL);
        ok(v != NULL && v[0].pos.x != still,
           "the wobble setting reaches the sprites");
        GDI_gb_WaveSprite = 0;
    }

    /* A second batch reuses the staging buffer rather than growing
       it again. */
    reset();
    WG_BuildSprites(2);
    GL_DrawSprites(&wg_sprites, wg_sprPos, 4);
    wg_callLen = 0;
    GL_DrawSprites(&wg_sprites, wg_sprPos, 4);
    okEq(WG_i_Count("glDrawArrays"), 1, "a second batch still draws");
    okEq(WG_i_Count("glCreateShader"), 0, "and compiles nothing");
}

/* The same colour conversion appears in two places -- geovertex.c
   fuses it into the packing of a vertex colour, and the sprite path
   has a colour already made and only needs it turned round. They have
   to agree, and nothing else would notice if they stopped. */
static void caseDeviceColour(void)
{
    unsigned int seed;
    int          bad = 0;

    printf("-- the two spellings of the colour conversion agree\n");
    for (seed = 0; seed < 20000u; seed++)
    {
        unsigned int c = seed * 2654435761u;
        int          one = (int)c;

        if (GVX_ui_DeviceColor(c) != GVX_ui_Color(&one, NULL, 0, 0, 0, 0))
            bad++;
    }
    okEq(bad, 0, "over twenty thousand colours");
}

/* -------------------------------------------------- the video display */

static void caseVideo(void)
{
    void* bits = NULL;
    int   pitch = 0;
    int   height = 0;

    printf("-- the video display\n");
    reset();
    wg_callLen = 0;

    okEq(GL_b_VideoDisplayCreate(32, 16), 1, "a display is made");
    okEq(WG_i_Count("glTexImage2D"), 1, "one texture for the picture");
    okEq(WG_l_Arg("glTexImage2D", 0, 3), 32, "the movie's width");
    okEq(WG_l_Arg("glTexImage2D", 0, 4), 16, "and its height");
    okEq(WG_l_Arg("glTexImage2D", 0, 6), GL_BGRA,
         "read in the order the engine's words are in memory");
    okEq(WG_l_TexParam(GL_TEXTURE_MAX_LEVEL), 0,
         "with no levels, so a filter cannot ask for one");

    /* The picture starts black, so a frame that does not fill it --
       or a movie whose first frame has not arrived -- shows black
       rather than whatever the memory held. Checked on what was
       handed to OpenGL rather than on what the allocator returned,
       which is the only way to check it without depending on the
       allocator. */
    {
        int i;
        int nonzero = 0;

        okEq(wg_pixelBytes, 32 * 16 * 4, "the whole picture was uploaded");
        for (i = 0; i < wg_pixelBytes && i < (int)sizeof(wg_pixels); i++)
            if (wg_pixels[i])
                nonzero++;
        okEq(nonzero, 0, "and every byte of it was black");
    }

    /* Locking hands out the picture, not the screen. */
    okEq(GL_b_VideoFrameLock(&bits, &pitch, &height), 1, "it locks");
    ok(bits != NULL, "and gives somewhere to write");
    okEq(pitch, 32 * 4, "four bytes a pixel, packed");
    okEq(height, 16,
         "the picture's own height -- the screen's is how the bottom of"
         " a frame goes missing");

    /* Unlocking is where the frame reaches the card. */
    wg_callLen = 0;
    GL_VideoFrameUnlock();
    okEq(WG_i_Count("glTexSubImage2D"), 1, "unlocking uploads the frame");
    okEq(WG_l_Arg("glTexSubImage2D", 0, 4), 32, "all of it");
    okEq(WG_l_Arg("glTexSubImage2D", 0, 5), 16, "in both axes");

    /* Nothing works before a display exists, and nothing crashes. */
    reset();
    okEq(GL_b_VideoFrameLock(&bits, &pitch, &height), 0,
         "no display means no lock");
    wg_callLen = 0;
    GL_VideoFrameUnlock();
    GL_VideoFramePresent(320, 200, GDI_VIDEOFIT_SMOOTH);
    okEq(wg_callLen, 0, "and nothing to unlock or present");
    okEq(GL_b_VideoDisplayCreate(0, 200), 0, "nor a display of no width");
    okEq(GL_b_VideoDisplayCreate(320, -1), 0, "nor of a negative height");
}

static void caseVideoFit(void)
{
    printf("-- how a frame is fitted to the screen\n");

    /* The surface is 800 by 600 in this harness, so a 320 by 200
       movie scaled to the width is 800 by 500, leaving fifty pixels
       of black above and below. */
    reset();
    GL_b_VideoDisplayCreate(320, 200);
    wg_callLen = 0;
    GL_VideoFramePresent(320, 200, GDI_VIDEOFIT_SMOOTH);

    okEq(WG_i_Count("glClear"), 1, "the screen is cleared first");
    okEq(WG_l_Arg("glClearColor", 0, 0), 0, "to black");
    okEq(WG_i_b_CapabilityAt(GL_BLEND, "glDrawArrays"), 0,
         "and the frame is not blended");
    okEq(WG_i_b_CapabilityAt(GL_DEPTH_TEST, "glDrawArrays"), 0,
         "nor depth tested -- the frame goes over whatever the last"
         " scene left in the depth buffer");
    okEq(WG_i_b_CapabilityAt(GL_CULL_FACE, "glDrawArrays"), 0, "nor culled");
    okEq(WG_l_TexParam(GL_TEXTURE_MAG_FILTER), GL_LINEAR, "smoothed");
    okEq(WG_i_Count("glDrawArrays"), 1, "and drawn once");
    okEq(WG_l_Arg("glDrawArrays", 0, 2), 6, "as two triangles");

    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);

        ok(v != NULL, "from the staging buffer");
        if (v)
        {
            okEq((long)v[0].pos.x, 0, "the left edge is at the screen's");
            okEq((long)v[0].pos.y, 50, "and the top is inset by the bars");
            okEq((long)v[1].pos.x, 800, "the right edge is the screen's");
            okEq((long)v[2].pos.y, 550, "and the bottom is inset to match");
            /* The frame fills the picture here, so the coordinates run
               the whole way. */
            okEq((long)(v[1].u * 16), 16, "the texture is read to its edge");
            okEq((long)(v[2].v * 16), 16, "in both axes");
        }
    }

    /* Unsmoothed takes the same rectangle and a different filter --
       the one thing that separates the two. */
    reset();
    GL_b_VideoDisplayCreate(320, 200);
    wg_callLen = 0;
    GL_VideoFramePresent(320, 200, GDI_VIDEOFIT_SHARP);
    okEq(WG_l_TexParam(GL_TEXTURE_MAG_FILTER), GL_NEAREST, "unsmoothed");
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);

        if (v)
            okEq((long)v[1].pos.x, 800, "and the same rectangle");
    }

    /* A pixel for a pixel, centred: 320 by 200 in the middle of 800
       by 600. */
    reset();
    GL_b_VideoDisplayCreate(320, 200);
    wg_callLen = 0;
    GL_VideoFramePresent(320, 200, GDI_VIDEOFIT_ACTUALSIZE);
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);

        ok(v != NULL, "a frame at its own size is drawn");
        if (v)
        {
            okEq((long)v[0].pos.x, (800 - 320) / 2, "centred across");
            okEq((long)v[0].pos.y, (600 - 200) / 2, "and down");
            okEq((long)v[1].pos.x, (800 - 320) / 2 + 320,
                 "at the movie's own width");
        }
    }

    /* A frame smaller than the picture it was written into: the other
       renderer blits a sub-rectangle, so the coordinates have to stop
       short rather than stretch the whole picture over it.

       The two fractions are deliberately different -- half the width
       and all of the height -- because a frame that is the same
       fraction of both cannot see the two coordinates swapped. */
    reset();
    GL_b_VideoDisplayCreate(320, 200);
    wg_callLen = 0;
    GL_VideoFramePresent(160, 200, GDI_VIDEOFIT_ACTUALSIZE);
    {
        const tGdi_TexturedVertex* v
            = (const tGdi_TexturedVertex*)(wg_bufferCopyLen ? wg_bufferCopy
                                                            : NULL);

        ok(v != NULL, "a part-filled picture is drawn");
        if (v)
        {
            okEq((long)(v[1].u * 16), 8,
                 "the texture is read to half its width");
            okEq((long)(v[2].v * 16), 16, "and all of its height");
            okEq((long)v[1].pos.x, (800 - 160) / 2 + 160,
                 "over the frame's own size on the screen");
        }
    }
}

static void caseVideoTeardown(void)
{
    printf("-- giving the video display back\n");
    reset();
    GL_b_VideoDisplayCreate(320, 200);
    wg_callLen = 0;

    GL_VideoDisplayClose();
    okEq(WG_i_Count("glDeleteTextures"), 1, "the picture's texture is deleted");
    wg_callLen = 0;
    GL_VideoDisplayClose();
    okEq(WG_i_Count("glDeleteTextures"), 0, "a second close deletes nothing");

    /* And a second display replaces the first rather than leaking
       it, which is what happens between two movies. */
    reset();
    GL_b_VideoDisplayCreate(320, 200);
    wg_callLen = 0;
    okEq(GL_b_VideoDisplayCreate(640, 480), 1, "a second display is made");
    okEq(WG_i_Count("glDeleteTextures"), 1, "and gives the first one back");
    okEq(WG_l_Arg("glTexImage2D", 0, 3), 640, "at the new size");
}


/* ------------------------------- the depth blur's after-effect chain

   The real-driver harness reads the picture the chain produces; this
   one reads the calls it makes. They catch different things: a wrong
   ping-pong still produces a blurred picture, and a texture-space
   offset of a fraction of a pixel is arithmetic the picture cannot
   show at the size that harness draws at.
 */

/* The framebuffer bound to a target at the point a call was made --
   the state the pass was actually in, rather than the last bind in the
   whole frame. */
static long wg_l_FboBefore(long target, int before)
{
    long fb = -1;
    int  i;

    for (i = 0; i < before && i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, "glBindFramebuffer") == 0
            && (wg_calls[i].arg[0] == target
                /* A bind to the whole framebuffer sets both of its
                   halves, so it counts as a bind to either. */
                || wg_calls[i].arg[0] == GL_FRAMEBUFFER))
            fb = (long)wg_calls[i].arg[1];
    return fb;
}

/* The texture bound to a unit at the point a call was made. */
static long wg_l_TextureBefore(long unit, int before)
{
    long tex = -1;
    long active = GL_TEXTURE0;
    int  i;

    for (i = 0; i < before && i < wg_callLen; i++)
    {
        if (strcmp(wg_calls[i].name, "glActiveTexture") == 0)
            active = (long)wg_calls[i].arg[0];
        else if (strcmp(wg_calls[i].name, "glBindTexture") == 0
                 && active == unit)
            tex = (long)wg_calls[i].arg[1];
    }
    return tex;
}

/* Scaled and rounded exactly as the two mocks above record a float,
   so that a case's expected value and the recorded one round the same
   way rather than one truncating. */
/* The same scaling the mocks record with, under the name the cases
   below it were written against. One implementation, so a case and a
   mock cannot round differently. */
static long wg_l_Scaled(float v)
{
    return WG_l_Scale(v);
}

/* Where the nth call of a name sits in the log. */
static int wg_i_CallAt(const char* name, int nth)
{
    int i, n = 0;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, name) == 0 && n++ == nth)
            return i;
    return -1;
}

static void caseGeometryCullingTweak(void)
{
    printf("-- the culling override, which is what found the mapping\n");

    /* 1 turns culling off whatever the engine asked for. */
    reset();
    wg_callLen = 0;
    gl_dd.drawMaskAllowedStuffCurrent = 0x10 | 0x800;
    tweak_gl_cull = 1;
    GL_TextureInit(-1, 0, 0);
    okEq(WG_i_Count("glFrontFace"), 0, "with the override at one, no"
                                       " winding is chosen");
    {
        int i;
        int off = 0;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glDisable") == 0
                && wg_calls[i].arg[0] == GL_CULL_FACE)
                off = 1;
        ok(off, "and culling is switched off");
    }

    /* 2 keeps culling on with the facing the other way round -- which
       is the wrong answer now, and was the right one before the
       mapping was measured. Both settings are checked so that the
       override cannot quietly stop overriding. */
    reset();
    wg_callLen = 0;
    gl_dd.drawMaskAllowedStuffCurrent = 0x10 | 0x800;
    tweak_gl_cull = 2;
    GL_TextureInit(-1, 0, 0);
    okEq(WG_l_Arg("glFrontFace", 0, 0), GL_CCW,
         "with the override at two, the engine's counter-clockwise"
         " reverses");
    reset();
    wg_callLen = 0;
    gl_dd.drawMaskAllowedStuffCurrent = 0x10 | 0x800 | 0x2000;
    tweak_gl_cull = 2;
    GL_TextureInit(-1, 0, 0);
    okEq(WG_l_Arg("glFrontFace", 0, 0), GL_CW,
         "and its clockwise reverses the other way");

    /* And nought leaves the measured mapping alone, which every other
       case in this file depends on. */
    reset();
    wg_callLen = 0;
    gl_dd.drawMaskAllowedStuffCurrent = 0x10 | 0x800;
    tweak_gl_cull = 0;
    GL_TextureInit(-1, 0, 0);
    okEq(WG_l_Arg("glFrontFace", 0, 0), GL_CW,
         "and nought leaves the measured mapping in force");

    tweak_gl_cull = 0;
}

static void caseAeDepthBlurChain(void)
{
    tDpb_BlurPass passes[DPB_BLUR_PASSES];
    int           n;
    int           i;
    int           mixAt;
    int           blitAt;
    long          mixFbo;

    printf("-- the depth blur's passes, in order, and what each reads\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x8;
    wg_callLen = 0;
    GL_Flip();

    n = DPB_i_BlurSchedule(passes, DPB_BLUR_PASSES);
    okEq(WG_i_Count("glDrawArrays"), n + 2,
         "one draw per blur pass, one for the mix, and one to copy the"
         " mix back over the frame");

    /* The state a pass needs, switched off before the first of them.
     *
       None of these three can be seen in the picture as the chain
       stands: the spares carry no depth buffer, so a depth test that
       was left on would pass everything, and nothing before the chain
       leaves culling or blending on. They are checked here because
       that is an accident of how the spares are built rather than
       something the passes may rely on -- the day one of them wants a
       depth buffer, a depth test left on would throw the frame away
       and no case above would say why. */
    {
        int first = wg_i_CallAt("glDrawArrays", 0);
        int i;
        int depth = 0;
        int cull = 0;
        int blend = 0;

        for (i = 0; i < first && i >= 0 && i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glDisable") == 0)
            {
                if (wg_calls[i].arg[0] == GL_DEPTH_TEST) depth = 1;
                if (wg_calls[i].arg[0] == GL_CULL_FACE) cull = 1;
                if (wg_calls[i].arg[0] == GL_BLEND) blend = 1;
            }
        ok(depth, "the depth test is off before the first pass draws");
        ok(cull, "and culling");
        ok(blend, "and blending");
    }

    /* Each blur pass writes the target the schedule names and reads
       the one before it. Read off the state each draw was made in,
       which is the only way a wrong ping-pong shows: all four draws
       look alike otherwise. */
    for (i = 0; i < n; i++)
    {
        int at = wg_i_CallAt("glDrawArrays", i);

        if (at < 0)
        {
            ok(0, "the blur pass was drawn");
            continue;
        }
        okEq(wg_l_FboBefore(GL_FRAMEBUFFER, at),
             (long)GL_l_i_AeSlotFbo(passes[i].toSlot),
             "the pass writes the target the schedule names");
        okEq(wg_l_TextureBefore(GL_TEXTURE0, at),
             (long)GL_l_i_AeSlotTexture(passes[i].fromSlot),
             "and reads the one it names");
    }

    /* And the mix: the frame on the first unit, the blurred copy on
       the second, and written to neither of them. */
    mixAt = wg_i_CallAt("glDrawArrays", n);
    ok(mixAt >= 0, "the mix was drawn");
    if (mixAt < 0)
        return;
    mixFbo = wg_l_FboBefore(GL_FRAMEBUFFER, mixAt);
    okEq(wg_l_TextureBefore(GL_TEXTURE0, mixAt), (long)gl_sceneColour,
         "the mix reads the frame on the first unit");
    okEq(wg_l_TextureBefore(GL_TEXTURE1, mixAt),
         (long)GL_l_i_AeSlotTexture(DPB_i_BlurResultSlot()),
         "and the blurred copy on the second");
    ok(mixFbo != (long)gl_sceneFbo, "it does not write the frame it read");
    ok(mixFbo != (long)GL_l_i_AeSlotFbo(DPB_i_BlurResultSlot()),
       "nor the blurred copy it read");

    /* And then the mix is copied back over the frame, colour only --
       so the frame keeps the alpha the world gave it, which is what
       the renderer this follows achieves by narrowing its write. */
    {
        int copyAt = wg_i_CallAt("glDrawArrays", n + 1);
        int i;
        int masked = 0;

        ok(copyAt >= 0, "the copy back was drawn");
        okEq(wg_l_FboBefore(GL_FRAMEBUFFER, copyAt), (long)gl_sceneFbo,
             "into the frame");
        okEq(wg_l_TextureBefore(GL_TEXTURE0, copyAt), 
             (long)GL_l_i_AeSlotTexture(GL_AE_SLOT_MIXTMP),
             "from where the mix wrote");
        for (i = 0; i < copyAt; i++)
            if (strcmp(wg_calls[i].name, "glColorMask") == 0
                && wg_calls[i].arg[3] == GL_FALSE)
                masked = 1;
        ok(masked, "with the alpha write masked off");
    }

    /* The copy to the screen then reads the frame, which is where the
       chain left it. */
    blitAt = wg_i_CallAt("glBlitFramebuffer", 0);
    ok(blitAt >= 0, "the frame was copied to the screen");
    if (blitAt >= 0)
        okEq(wg_l_FboBefore(GL_READ_FRAMEBUFFER, blitAt), (long)gl_sceneFbo,
             "from the frame, where the chain left it");
    (void)mixFbo;

    PC_AE_MASTER = 0;
}

static void caseAeDepthBlurRefusals(void)
{
    printf("-- the chain when it cannot be built\n");

    /* A shader that will not compile. The frame still has to reach the
       screen: the effect is missing, which is a worse picture, and the
       alternative is no picture at all. */
    reset();
    wg_SceneReady();
    /* The 2D pass first, so that the refusal below is the chain's own
       shaders being refused and not the buffer it borrows from that
       pass. */
    ok(GL_l_b_PassReady(), "the 2D pass builds");
    CIN_Bands = 0;
    PC_AE_MASTER = 0x8;
    wg_compileFails = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "nothing is drawn");
    okEq(WG_i_Count("glBlitFramebuffer"), 1, "but the frame is still copied");
    {
        int blit = wg_i_CallAt("glBlitFramebuffer", 0);

        okEq(wg_l_FboBefore(GL_READ_FRAMEBUFFER, blit), (long)gl_sceneFbo,
             "from the scene target, where the world left it");
    }
    /* And it is not retried every frame: the ask is remembered. */
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glCreateShader"), 0, "a second frame does not try again");
    wg_compileFails = 0;

    /* A driver that will not give out names. Same answer. */
    reset();
    wg_SceneReady();
    ok(GL_l_b_PassReady(), "the 2D pass builds again");
    CIN_Bands = 0;
    PC_AE_MASTER = 0x8;
    wg_genFails = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "nothing is drawn for that either");
    okEq(WG_i_Count("glBlitFramebuffer"), 1, "and the frame still gets out");
    wg_genFails = 0;

    PC_AE_MASTER = 0;
}

static void caseAeDepthBlurUniforms(void)
{
    tDpb_BlurPass passes[DPB_BLUR_PASSES];
    float         offsets[2 * DPB_BLUR_TAPS];
    float         weights[DPB_BLUR_TAPS];
    int           n;
    int           i;

    printf("-- the taps each pass asks for, and which way up\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x1000;
    wg_callLen = 0;
    GL_Flip();

    n = DPB_i_BlurSchedule(passes, DPB_BLUR_PASSES);
    for (i = 0; i < n; i++)
    {
        DPB_BlurTaps(&passes[i], offsets, weights);

        /* Three uniforms per pass, in the order the renderer sets
           them: the two taps across, the two down, then the weights.
           Scaled by a million, because a step is a fraction of a
           texture and whole numbers would all be nought. */
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3, 1),
             wg_l_Scaled(offsets[0]),
             "the first tap steps left by the pass's own reach");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3, 3),
             wg_l_Scaled(offsets[2]),
             "and the second right by it");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3, 2), 0,
             "neither of them moves vertically");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3, 4), 0, "nor the other");

        /* The two down, turned over for this renderer's textures: the
           schedule measures them down the screen and a texture here
           runs up it. The heaviest tap is the fourth, so this is the
           sign that decides which side of an edge the smear lands
           on. */
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 1, 2),
             wg_l_Scaled(-offsets[5]),
             "the third tap is the schedule's, turned over");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 1, 4),
             wg_l_Scaled(-offsets[7]),
             "and so is the fourth");
        ok(WG_l_Arg("glUniform4f.scaled", i * 3 + 1, 4) < 0,
           "which puts the heaviest tap below the pixel in a texture"
           " that runs upwards");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 1, 1), 0,
             "and neither of them moves across");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 1, 3), 0,
             "nor the other");

        /* The weights, which are not equal and are not the round
           fractions they sit near. */
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 2, 1),
             wg_l_Scaled(weights[0]), "the first weight");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 2, 2),
             wg_l_Scaled(weights[1]), "the second");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 2, 3),
             wg_l_Scaled(weights[2]), "the third");
        okEq(WG_l_Arg("glUniform4f.scaled", i * 3 + 2, 4),
             wg_l_Scaled(weights[3]), "and the fourth");
    }

    /* A pass's reach grows, so the second pass asks for twice the
       first and the third three times it. Stated here as well as in
       the unit test, because this is the side that could set them all
       from one pass. */
    ok(WG_l_Arg("glUniform4f.scaled", 3, 3)
       > WG_l_Arg("glUniform4f.scaled", 0, 3),
       "the second pass reaches further than the first");
    ok(WG_l_Arg("glUniform4f.scaled", 6, 3)
       > WG_l_Arg("glUniform4f.scaled", 3, 3),
       "and the third further than the second");

    PC_AE_MASTER = 0;
}

static void caseAeDepthBlurQuads(void)
{
    tCbd_Bands bands;
    long       reachTopY;
    long       reachBottomY;
    long       bandTopY;
    long       bandBottomY;
    int        i;

    printf("-- the strip each pass covers\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 1;
    tweak_ae_nobands = 0;
    D3D_gf_CutsceneBand = 0.25f;
    PC_AE_MASTER = 0x8;
    wg_callLen = 0;
    GL_Flip();

    CBD_Decide(&bands);
    bandTopY = wg_l_Scaled(1.0f - 2.0f * bands.top);
    bandBottomY = wg_l_Scaled(1.0f - 2.0f * bands.bottom);
    reachTopY = wg_l_Scaled(1.0f - 2.0f * bands.reachTop);
    reachBottomY = wg_l_Scaled(1.0f - 2.0f * bands.reachBottom);
    ok(reachTopY > bandTopY, "the reach is above the band");
    ok(reachBottomY < bandBottomY, "and below it");

    /* The quad's first vertex is the bottom-left and the second the
       top-left, so the two y values on the record are the strip's
       edges. Each blur pass covers the reach. */
    for (i = 0; i < DPB_BLUR_PASSES; i++)
    {
        okEq(WG_l_Arg("glBufferData", i, 4), reachBottomY,
             "a blur pass starts at the bottom of the reach");
        okEq(WG_l_Arg("glBufferData", i, 6), reachTopY,
             "and ends at the top of it");
    }
    /* And the mix covers the band itself: it is what the screen is
       copied from, and a mix drawn over the reach would put picture
       into the letterbox. */
    okEq(WG_l_Arg("glBufferData", DPB_BLUR_PASSES, 4), bandBottomY,
         "the mix starts at the bottom of the band");
    okEq(WG_l_Arg("glBufferData", DPB_BLUR_PASSES, 6), bandTopY,
         "and ends at the top of it");

    PC_AE_MASTER = 0;
    CIN_Bands = 0;
    D3D_gf_CutsceneBand = 0.10416666f;
}


/* ----------------------------------- the colour after-effects' calls

   What the picture cannot say: which target each of the three steps
   reads and writes, that they are three separate draws rather than one
   pass, which uniform each sets, and that the quad is the band.
 */
static void caseAeColorChain(void)
{
    tAec_Source src;
    tAec_Step   steps[AEC_MAX_STEPS];
    int         n;
    int         i;
    int         from;
    long        lastFbo = -1;

    printf("-- the colour steps, a draw each, and what each reads\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    /* All three on, so the order and the chaining are both visible. */
    PC_AE_MASTER = 0x1;
    unkAEFunc2Param = 0.8f;
    colorBalanceParam1 = 0.2f;
    wg_callLen = 0;
    GL_Flip();

    src.aeMaster = 0x1;
    src.brightness = 0.8f;
    src.contrast = 0.2f;
    n = AEC_i_Steps(&src, steps, AEC_MAX_STEPS);
    okEq(n, 3, "three steps to do");
    okEq(WG_i_Count("glDrawArrays"), n,
         "and a draw each, not one pass doing all three");

    /* Each step reads what the one before wrote, starting from the
       frame -- and none of them writes the frame, whose alpha the
       depth blur needs and which an effect not yet run may want.
     *
       The slots are written out here rather than worked out with
       GL_l_i_AeOtherSlot, which is the function being checked: asking
       it where the step should have gone and then comparing that with
       where it went moves both sides of the comparison together, and
       a version of this did exactly that -- a mutant that always
       answered the first spare passed, because every step then both
       read and wrote it and the expectation agreed.
     *
       Three steps alternating between two spares from the frame is
       the first, the second, the first; that is the whole sequence
       and it is short enough to state. */
    {
        static const int wantSlot[3] = { DPB_SLOT_A, DPB_SLOT_B,
                                         DPB_SLOT_A };

        from = DPB_SLOT_FRAME;
        for (i = 0; i < n && i < 3; i++)
        {
            int at = wg_i_CallAt("glDrawArrays", i);

            if (at < 0)
            {
                ok(0, "the step was drawn");
                continue;
            }
            okEq(wg_l_TextureBefore(GL_TEXTURE0, at),
                 (long)GL_l_i_AeSlotTexture(from),
                 "the step reads where the one before left the frame");
            lastFbo = wg_l_FboBefore(GL_FRAMEBUFFER, at);
            okEq(lastFbo, (long)GL_l_i_AeSlotFbo(wantSlot[i]),
                 "and writes the spare the alternation calls for");
            /* And the two are not the same place, compared as slots:
               an earlier version compared a framebuffer's name with a
               texture's, which are different numbers for different
               things and never equal whatever the code does. */
            ok(wantSlot[i] != from, "which is not the one it is reading");
            ok(lastFbo != (long)gl_sceneFbo, "nor the frame itself");
            from = wantSlot[i];
        }
    }

    /* And the screen is copied from where the last step wrote. */
    {
        int blit = wg_i_CallAt("glBlitFramebuffer", 0);

        ok(blit >= 0, "the frame was copied");
        if (blit >= 0)
            okEq(wg_l_FboBefore(GL_READ_FRAMEBUFFER, blit), lastFbo,
                 "from where the last step wrote");
    }

    /* The gain and the bias each step asked for, in order. Compared
       against what aecolor.c says rather than against three
       constants, so the two cannot drift apart -- and scaled, because
       a gain is not a byte. */
    for (i = 0; i < n; i++)
    {
        okEq(WG_l_Arg("glUniform1f.scaled", i * 3, 1),
             wg_l_Scaled(steps[i].gain), "the step's gain");
        okEq(WG_l_Arg("glUniform1f.scaled", i * 3 + 1, 1),
             wg_l_Scaled(steps[i].bias), "and its bias");
        /* The grayscale flag, which is what picks between summing the
           channels and taking each on its own. Set every step, so a
           step cannot inherit the one before's -- which would turn
           the step after a grayscale into a second grayscale. */
        okEq(WG_l_Arg("glUniform1f.scaled", i * 3 + 2, 1),
             steps[i].kind == AEC_GRAYSCALE ? 1000000L : 0L,
             "and whether it is the grayscale");
        okEq(WG_l_Arg("glUniform3f.scaled", i, 1),
             wg_l_Scaled(steps[i].weights[0]), "the first weight");
        okEq(WG_l_Arg("glUniform3f.scaled", i, 2),
             wg_l_Scaled(steps[i].weights[1]), "the second");
        okEq(WG_l_Arg("glUniform3f.scaled", i, 3),
             wg_l_Scaled(steps[i].weights[2]), "the third");
    }
    /* Exactly one of the three is the grayscale, and it is the last
       one. A flag set for all of them, or for none, fails here as
       well as above. */
    okEq(WG_l_Arg("glUniform1f.scaled", 2, 1), 0L,
         "the first step is not the grayscale");
    okEq(WG_l_Arg("glUniform1f.scaled", 5, 1), 0L, "nor the second");
    okEq(WG_l_Arg("glUniform1f.scaled", 8, 1), 1000000L,
         "and the third is");

    PC_AE_MASTER = 0;
    unkAEFunc2Param = 0.5f;
    colorBalanceParam1 = 0.5f;
    tweak_gl_cull = 0;
    motionBlurFactor = 0.0f;
    remenanceFactor = 0.0f;
    zoomSmoothFactor = 0.0f;
}

static void caseAeColorBand(void)
{
    tCbd_Bands bands;
    long       bandTopY;
    long       bandBottomY;

    printf("-- the colour steps cover the band\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 1;
    tweak_ae_nobands = 0;
    D3D_gf_CutsceneBand = 0.25f;
    PC_AE_MASTER = 0x1;
    wg_callLen = 0;
    GL_Flip();

    CBD_Decide(&bands);
    bandTopY = wg_l_Scaled(1.0f - 2.0f * bands.top);
    bandBottomY = wg_l_Scaled(1.0f - 2.0f * bands.bottom);

    okEq(WG_i_Count("glDrawArrays"), 1, "one step, one draw");
    okEq(WG_l_Arg("glBufferData", 0, 4), bandBottomY,
         "the quad starts at the bottom of the band");
    okEq(WG_l_Arg("glBufferData", 0, 6), bandTopY, "and ends at its top");
    /* Not the blur's wider reach: these steps sample one pixel each,
       so there is nothing for a margin to be for, and a step drawn
       over the margin would put picture into the letterbox. */
    ok(bandTopY != wg_l_Scaled(1.0f - 2.0f * bands.reachTop),
       "which is not the same strip the blur covers");

    PC_AE_MASTER = 0;
    CIN_Bands = 0;
    D3D_gf_CutsceneBand = 0.10416666f;
}

static void caseAeColorGate(void)
{
    printf("-- when the colour steps do nothing\n");

    /* Both parameters neutral and the bit clear: nothing is built,
       nothing is drawn, and the frame goes out of the scene target. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "nothing drawn");
    okEq(WG_i_Count("glCreateShader"), 0,
         "and nothing even built for it");
    {
        int blit = wg_i_CallAt("glBlitFramebuffer", 0);

        okEq(wg_l_FboBefore(GL_READ_FRAMEBUFFER, blit), (long)gl_sceneFbo,
             "the frame is copied straight from the scene target");
    }

    /* A parameter a hair off neutral still counts: the engine sets
       these exactly, so there is no tolerance to apply. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    colorBalanceParam1 = 0.5001f;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 1, "a hair off neutral runs a step");
    colorBalanceParam1 = 0.5f;

    /* And the two switches that turn the chain off take these with
       them, even though neither has a bit in the master word. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x1;
    unkAEFunc2Param = 0.9f;
    tweak_ae_no = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "the tweak stops them");
    tweak_ae_no = 0;
    ECAM_gi_NoAfterEffects = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "and so does the editor camera");
    ECAM_gi_NoAfterEffects = 0;

    PC_AE_MASTER = 0;
    unkAEFunc2Param = 0.5f;
}


/* ------------------------- the effects that sum whole frames: calls

   The real-driver harness reads the pictures these produce. What it
   cannot see is which target each one writes, and for the motion blur
   that is the whole effect: it reads what it wrote last frame, so the
   target has to be the same target next time round and has to be one
   nothing else has drawn into since.
 */

/* How many draws went into a framebuffer this frame. */
static int wg_i_DrawsInto(long fbo)
{
    int i;
    int n = 0;

    for (i = 0; i < wg_callLen; i++)
        if (strcmp(wg_calls[i].name, "glDrawArrays") == 0
            && wg_l_FboBefore(GL_FRAMEBUFFER, i) == fbo)
            n++;
    return n;
}

/* The framebuffer the nth draw went into. */
static long wg_l_DrawTarget(int nth)
{
    int at = wg_i_CallAt("glDrawArrays", nth);

    return at < 0 ? -1 : wg_l_FboBefore(GL_FRAMEBUFFER, at);
}

static void caseAeMotionBlurHistory(void)
{
    long alone;

    printf("-- the motion blur writes the chain's spare, wherever it is\n");

    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x10;
    motionBlurFactor = 0.5f;

    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 1, "one draw, it being the only effect");
    alone = wg_l_DrawTarget(0);
    okEq(alone, (long)GL_l_i_AeSlotFbo(GL_AE_SLOT_SPARE),
         "and it goes to the chain's spare");
    ok(alone != (long)gl_sceneFbo, "which is not the frame it read");

    /* The same target next frame, and the frame after. That is what
       makes the history readable: what it finds there is its own
       output from last time round. A target chosen by anything that
       varied frame to frame would read a stranger's leftovers and the
       picture would still look like a motion blur. */
    wg_callLen = 0;
    GL_Flip();
    okEq(wg_l_DrawTarget(0), alone, "the second frame writes the same one");
    wg_callLen = 0;
    GL_Flip();
    okEq(wg_l_DrawTarget(0), alone, "and so does the third");
    okEq(wg_i_DrawsInto(alone), 1,
         "with nothing else drawing into it, so what it read was its own");

    /* It blends, and it is the only effect that does -- so it turns
       blending on for itself and off again rather than leaving it on
       for whatever runs next. */
    {
        int draw = wg_i_CallAt("glDrawArrays", 0);
        int i;
        int onBefore = 0;
        int offAfter = 0;

        for (i = 0; i < draw; i++)
            if (strcmp(wg_calls[i].name, "glEnable") == 0
                && wg_calls[i].arg[0] == GL_BLEND)
                onBefore = 1;
        for (i = draw + 1; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glDisable") == 0
                && wg_calls[i].arg[0] == GL_BLEND)
                offAfter = 1;
        ok(onBefore, "blending is on for its draw");
        ok(offAfter, "and off again after it");
        ok(WG_i_Count("glBlendFunc") >= 1, "with a blend function set");
    }

    /* And now the case that is deliberately broken, because the
       renderer this follows breaks it.
     *
       The remanence exchanges the current frame for the spare when it
       finishes, so by the time the motion blur runs the spare *is* the
       frame -- and what is in the frame is the world as it was drawn
       this frame, not the motion blur's output from last frame. The
       trail silently stops being a trail.
     *
       An earlier arrangement here kept a note of which targets had
       been written and picked one that still held last frame's
       picture, which worked better and differed. This matches
       instead, by request, and the assertion below is what pins it:
       the motion blur writes the frame, and the frame is something
       this frame already drew into. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x10 | 0x800;
    motionBlurFactor = 0.5f;
    remenanceFactor = 0.5f;

    wg_callLen = 0;
    GL_Flip();
    {
        int  motion = AEF_REMANENCE_PASSES + 1;
        long target;

        okEq(WG_i_Count("glDrawArrays"), motion + 1,
             "four blurs, a sum and the motion blur");
        target = wg_l_DrawTarget(motion);
        okEq(target, (long)gl_sceneFbo,
             "the motion blur writes the frame, the exchange having put"
             " the spare there");
        ok(target != alone,
           "which is a different target from when it ran by itself");
    }

    PC_AE_MASTER = 0;
    motionBlurFactor = 0.0f;
    remenanceFactor = 0.0f;
}

static void caseAeRemanenceChain(void)
{
    float u[AEF_REMANENCE_PASSES];
    float v[AEF_REMANENCE_PASSES];
    int   n;
    int   i;
    long  from;
    long  blurred;
    long  into;

    printf("-- the remanence's four blurs and where its sum goes\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x800;
    remenanceFactor = 0.5f;
    wg_callLen = 0;
    GL_Flip();

    n = AEF_i_RemanenceReaches(u, v, AEF_REMANENCE_PASSES);
    okEq(n, 4, "four reaches");
    okEq(WG_i_Count("glDrawArrays"), n + 1, "four blurs and one sum");

    /* The blurs chain: the first reads the frame, each after reads
       what the one before wrote, and none writes the frame. */
    from = (long)gl_sceneColour;
    for (i = 0; i < n; i++)
    {
        int at = wg_i_CallAt("glDrawArrays", i);

        okEq(wg_l_TextureBefore(GL_TEXTURE0, at), from,
             "the blur reads what the one before wrote");
        ok(wg_l_FboBefore(GL_FRAMEBUFFER, at) != (long)gl_sceneFbo,
           "and does not write the frame");
        /* The texture that goes with the framebuffer it wrote, so the
           next pass can be checked against it. */
        {
            long fbo = wg_l_FboBefore(GL_FRAMEBUFFER, at);
            int  slot;

            from = -1;
            for (slot = DPB_SLOT_A; slot < DPB_SLOT_A + GL_AE_TARGETS; slot++)
                if ((long)GL_l_i_AeSlotFbo(slot) == fbo)
                    from = (long)GL_l_i_AeSlotTexture(slot);
            ok(from > 0, "which is one of the spares");
        }
    }
    blurred = from;

    /* The sum reads the frame and the blurred copy and writes
       neither. With two spares there would be nowhere left; this is
       the check that says the third one is being used. */
    {
        int at = wg_i_CallAt("glDrawArrays", n);

        okEq(wg_l_TextureBefore(GL_TEXTURE0, at), (long)gl_sceneColour,
             "the sum reads the frame on the first unit");
        okEq(wg_l_TextureBefore(GL_TEXTURE1, at), blurred,
             "and the blurred copy on the second");
        into = wg_l_FboBefore(GL_FRAMEBUFFER, at);
        ok(into != (long)gl_sceneFbo, "it writes neither the frame");
        {
            int slot;

            for (slot = DPB_SLOT_A; slot < DPB_SLOT_A + GL_AE_TARGETS; slot++)
                if ((long)GL_l_i_AeSlotTexture(slot) == blurred)
                    ok(into != (long)GL_l_i_AeSlotFbo(slot),
                       "nor the blurred copy");
        }
        /* And the screen is copied from there. */
        {
            int blit = wg_i_CallAt("glBlitFramebuffer", 0);

            okEq(wg_l_FboBefore(GL_READ_FRAMEBUFFER, blit), into,
                 "and the screen is copied from where the sum wrote");
        }
    }

    /* The weight it asked for, compared against what aeframe.c says
       rather than against a constant. */
    {
        tAef_Source fs;

        fs.aeMaster = 0x800;
        fs.motionBlur = 0.0f;
        fs.remanence = 0.5f;
        fs.zoomSmooth = 0.0f;
        fs.bandBottom = 1.0f;
        /* Three uniforms per blur pass, then the sum's two samplers
           and its weight -- so the weight is the first scaled float
           after the blurs' own. */
        ok(WG_i_Count("glUniform1f.scaled") >= 1, "a weight was set");
        okEq(WG_l_Arg("glUniform1f.scaled",
                      WG_i_Count("glUniform1f.scaled") - 1, 1),
             wg_l_Scaled(AEF_f_RemanenceWeight(&fs)),
             "and it is twice the opacity");
    }

    PC_AE_MASTER = 0;
    remenanceFactor = 0.0f;
}

static void caseAeRemanenceAfterDepthBlur(void)
{
    float u[AEF_REMANENCE_PASSES];
    int   n;
    int   i;

    printf("-- the remanence behind the depth blur, and where each lands\n");
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x8 | 0x800;
    depthBlurNear1 = 10.0f;
    depthBlurType0 = 20.0f;
    depthBlurType1 = 30.0f;
    remenanceFactor = 0.5f;
    wg_callLen = 0;
    GL_Flip();

    n = AEF_i_RemanenceReaches(u, NULL, AEF_REMANENCE_PASSES);
    /* Three blurs, a mix and a copy back for the depth blur; then four
       blurs and a sum for the remanence. */
    okEq(WG_i_Count("glDrawArrays"), DPB_BLUR_PASSES + 2 + n + 1,
         "both effects' draws");

    /* The remanence reads the frame, and so it should: the depth blur
       copied its result back there, which is the arrangement the
       renderer this follows has. An earlier version of this case
       asserted the opposite -- that reading the frame would mean the
       depth blur had been skipped -- which was true of an earlier
       arrangement here and is not true of this one.
     *
       It also cannot be otherwise. The remanence is the first effect
       in the chain that exchanges the current frame for the spare, so
       whatever runs before it leaves the frame current. The
       distinction only arises for an effect with one of these in
       front of it, and there is not one yet. */
    {
        int firstRem = wg_i_CallAt("glDrawArrays", DPB_BLUR_PASSES + 2);

        ok(firstRem >= 0, "the remanence ran");
        okEq(wg_l_TextureBefore(GL_TEXTURE0, firstRem), (long)gl_sceneColour,
             "and its first blur reads the frame, where the depth blur"
             " left its result");
    }

    /* Its four blurs alternate between the chain's spare and the
       scratch, starting on the spare -- the other way round from the
       depth blur, which starts on the scratch. Both orders end on the
       scratch, which is where each one's last reader looks. */
    for (i = 0; i < n; i++)
    {
        int at = wg_i_CallAt("glDrawArrays", DPB_BLUR_PASSES + 2 + i);
        int want = (i % 2 == 0) ? GL_AE_SLOT_SPARE : GL_AE_SLOT_SCRATCH;

        okEq(wg_l_FboBefore(GL_FRAMEBUFFER, at),
             (long)GL_l_i_AeSlotFbo(want),
             "the blur writes the slot the alternation calls for");
    }

    /* And the sum reads the frame and the scratch and writes the
       spare, which the exchange after it makes current. */
    {
        int at = wg_i_CallAt("glDrawArrays", DPB_BLUR_PASSES + 2 + n);

        okEq(wg_l_TextureBefore(GL_TEXTURE0, at), (long)gl_sceneColour,
             "the sum reads the frame on the first unit");
        okEq(wg_l_TextureBefore(GL_TEXTURE1, at),
             (long)GL_l_i_AeSlotTexture(GL_AE_SLOT_SCRATCH),
             "and the blurred copy off the scratch on the second");
        okEq(wg_l_FboBefore(GL_FRAMEBUFFER, at),
             (long)GL_l_i_AeSlotFbo(GL_AE_SLOT_SPARE),
             "and writes the spare");
        {
            int blit = wg_i_CallAt("glBlitFramebuffer", 0);

            okEq(wg_l_FboBefore(GL_READ_FRAMEBUFFER, blit),
                 (long)GL_l_i_AeSlotFbo(GL_AE_SLOT_SPARE),
                 "which is what the screen is copied from");
        }
    }

    PC_AE_MASTER = 0;
    remenanceFactor = 0.0f;
    depthBlurNear1 = 0.0f;
    depthBlurType0 = 0.0f;
    depthBlurType1 = 0.0f;
}

static void caseAeZoomUniforms(void)
{
    tAef_Source  fs;
    tAef_ZoomTap taps[AEF_ZOOM_TAPS];
    tCbd_Bands   bands;

    printf("-- the zoom's band, its crop step and its weight\n");
    reset();
    wg_SceneReady();
    /* With a letterbox, so that the band it zooms about is not just
       the whole picture -- which is the case where a band read wrong
       shows. */
    CIN_Bands = 1;
    tweak_ae_nobands = 0;
    D3D_gf_CutsceneBand = 0.25f;
    PC_AE_MASTER = 0x400;
    zoomSmoothFactor = 1.0f;
    wg_callLen = 0;
    GL_Flip();

    okEq(WG_i_Count("glDrawArrays"), 1, "eight taps in one draw");

    CBD_Decide(&bands);
    fs.aeMaster = 0x400;
    fs.motionBlur = 0.0f;
    fs.remanence = 0.0f;
    fs.zoomSmooth = 1.0f;
    fs.bandBottom = bands.bottom;
    AEF_i_ZoomTaps(&fs, taps, AEF_ZOOM_TAPS);

    /* The band's corners, in texture space: the fractions run from
       the top of the picture and this renderer's textures run up it,
       so the band's top is the larger coordinate. Getting that the
       wrong way round zooms about the wrong place, and with a
       symmetric band it would still look like a zoom. */
    okEq(WG_l_Arg("glUniform2f.scaled", 0, 1), 0L,
         "the band starts at the left edge");
    okEq(WG_l_Arg("glUniform2f.scaled", 0, 2),
         wg_l_Scaled(1.0f - bands.bottom),
         "and at the band's bottom measured from below");
    okEq(WG_l_Arg("glUniform2f.scaled", 1, 1), wg_l_Scaled(1.0f),
         "and ends at the right edge");
    okEq(WG_l_Arg("glUniform2f.scaled", 1, 2),
         wg_l_Scaled(1.0f - bands.top),
         "and at the band's top");
    ok(WG_l_Arg("glUniform2f.scaled", 1, 2)
       > WG_l_Arg("glUniform2f.scaled", 0, 2),
       "with the top above the bottom, textures running upwards");

    /* One tap's worth of crop, which is what places all eight. */
    okEq(WG_l_Arg("glUniform2f.scaled", 2, 1), wg_l_Scaled(taps[1].insetU),
         "the crop step across");
    okEq(WG_l_Arg("glUniform2f.scaled", 2, 2), wg_l_Scaled(taps[1].insetV),
         "and down");
    /* And with a band they differ, because only the horizontal one
       divides by it. */
    ok(WG_l_Arg("glUniform2f.scaled", 2, 1)
       > WG_l_Arg("glUniform2f.scaled", 2, 2),
       "the letterboxed crop tightens faster across than down");

    okEq(WG_l_Arg("glUniform1i", WG_i_Count("glUniform1i") - 1, 1),
         AEF_ZOOM_TAPS, "eight taps asked for");
    okEq(WG_l_Arg("glUniform1f.scaled", 0, 1),
         wg_l_Scaled(AEF_f_ZoomWeight()), "and the weight every tap carries");

    PC_AE_MASTER = 0;
    zoomSmoothFactor = 0.0f;
    CIN_Bands = 0;
    D3D_gf_CutsceneBand = 0.10416666f;
}

static void caseAeFrameGates(void)
{
    printf("-- what each of the three needs before it runs\n");

    /* A bit with no parameter does nothing, and a parameter with no
       bit does nothing. Checked for each of the three, because one
       gate reading another's parameter would pass a test that only
       turned them all on together. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x10 | 0x800 | 0x400;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "every bit but no parameters: nothing");

    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0;
    motionBlurFactor = 1.0f;
    remenanceFactor = 1.0f;
    zoomSmoothFactor = 1.0f;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "every parameter but no bits: nothing");

    /* Each on its own, with the draw counts that tell them apart:
       the remanence takes five draws and the other two one each. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x10;
    motionBlurFactor = 1.0f;
    remenanceFactor = 1.0f;
    zoomSmoothFactor = 1.0f;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 1, "the motion blur alone is one draw");

    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x800;
    motionBlurFactor = 1.0f;
    remenanceFactor = 1.0f;
    zoomSmoothFactor = 1.0f;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), AEF_REMANENCE_PASSES + 1,
         "the remanence alone is four blurs and a sum");

    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x400;
    motionBlurFactor = 1.0f;
    remenanceFactor = 1.0f;
    zoomSmoothFactor = 1.0f;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 1, "the zoom alone is one draw");

    /* And the two switches that stop the whole chain take all three
       with them. */
    reset();
    wg_SceneReady();
    CIN_Bands = 0;
    PC_AE_MASTER = 0x10 | 0x800 | 0x400;
    motionBlurFactor = 1.0f;
    remenanceFactor = 1.0f;
    zoomSmoothFactor = 1.0f;
    tweak_ae_no = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "the tweak stops all three");
    tweak_ae_no = 0;
    ECAM_gi_NoAfterEffects = 1;
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), 0, "and so does the editor camera");
    ECAM_gi_NoAfterEffects = 0;

    /* All three together, in the engine's own order: the remanence
       first, then the motion blur, then the zoom. */
    wg_callLen = 0;
    GL_Flip();
    okEq(WG_i_Count("glDrawArrays"), AEF_REMANENCE_PASSES + 3,
         "all three together");
    {
        /* The motion blur is the only one that blends, so where
           blending is switched on says where in the order it ran. */
        int blend = WG_i_At("glEnable");
        int i;
        int before = 0;
        int after = 0;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glDrawArrays") == 0)
            {
                if (i < blend)
                    before++;
                else
                    after++;
            }
        okEq(before, AEF_REMANENCE_PASSES + 1,
             "the remanence's five draws come before the motion blur");
        okEq(after, 2, "and the zoom's one comes after it");
    }

    PC_AE_MASTER = 0;
    motionBlurFactor = 0.0f;
    remenanceFactor = 0.0f;
    zoomSmoothFactor = 0.0f;
}

/* ------------------------------------------------------- the water
 *
 * What these check is the plumbing, not the arithmetic. The grid, the
 * rippled direction and the projection remap are watermesh.c's, and
 * that file is compared against the other renderer byte for byte by
 * tests/run_wingdi.sh -- including the bytes each side writes into its
 * locked buffers. Repeating that comparison here would be measuring
 * the shared code twice and the wiring not at all.
 *
 * So these ask the questions that are this renderer's own: whether the
 * reflection target is cleared to the colour the engine asked for,
 * whether the index strip is uploaded once rather than every frame,
 * whether the right arrays and the right camera reach the vertex
 * build, and what state the surface is drawn with.
 */

/* A grid, with every array distinct enough that a case can tell which
   one reached which attribute. */
#define WG_WTR_COLS 4
#define WG_WTR_ROWS 3
#define WG_WTR_NUM  (WG_WTR_COLS * WG_WTR_ROWS)

static tWaterParamsGlobalStruct wg_water;
static Vec3         wg_waterPos[WG_WTR_NUM];
static Vec3         wg_waterWave[WG_WTR_NUM];
static unsigned int wg_waterCol[WG_WTR_NUM];

static void wg_WaterFixture(unsigned int columns, int rows)
{
    unsigned int i;
    unsigned int num = columns * (unsigned int)rows;

    memset(&wg_water, 0, sizeof(wg_water));
    for (i = 0; i < num && i < WG_WTR_NUM; i++)
    {
        /* Nothing symmetric and nothing repeated: a swapped pair of
           arrays, or a transposed grid, has to show. */
        wg_waterPos[i].x = 100.0f + (float)i;
        wg_waterPos[i].y = 200.0f + (float)i * 2.0f;
        wg_waterPos[i].z = 300.0f + (float)i * 3.0f;
        /* The wave is the same at every grid point but one. Flat,
           because the ripple a vertex gets is the wave times a
           fall-off that depends only on the row, and a wave that grew
           across the grid would swamp the fall-off and hide whether
           it is there at all -- which it did, and the check below
           caught it. The one odd point is what shows the array is
           read per vertex rather than once. */
        wg_waterWave[i].x = 0.125f;
        wg_waterWave[i].y = 0.0625f;
        wg_waterWave[i].z = 0.0f;
        /* A8R8G8B8 with all four bytes different, so the swap the
           stream wants is visible and the alpha is not confusable
           with a colour channel. */
        wg_waterCol[i] = 0x40000000u | ((unsigned int)(i + 1) << 16)
                         | ((unsigned int)(i + 20) << 8)
                         | (unsigned int)(i + 40);
    }
    /* The odd one out, in the middle of the first row so that neither
       end of the grid is special. */
    if (num > 1)
        wg_waterWave[1].x = 4.0f;
    wg_water.vertices = wg_waterPos;
    wg_water.vecsForUVs = wg_waterWave;
    wg_water.colors = wg_waterCol;
    wg_water.horizontalVertexNum = columns;
    wg_water.verticallVertexNum = rows;
    wg_water.waterEngine.bottomColor = 0x20406080;
    pst_GlobalsWaterParams = &wg_water;

    /* A camera somewhere nothing else is, so that a direction built
       against the wrong one cannot land on the right answer. */
    gl_dd.ct.cam.objectMtx.Pos.x = 7.0f;
    gl_dd.ct.cam.objectMtx.Pos.y = 11.0f;
    gl_dd.ct.cam.objectMtx.Pos.z = 13.0f;
}

static void caseWaterRefusals(void)
{
    printf("-- what makes a frame have no water\n");

    /* No water parameters at all is the engine saying there is no
       water this frame, and it is the only thing that says so. */
    reset();
    pst_GlobalsWaterParams = NULL;
    wg_callLen = 0;
    GL_PrepareWaterRendering();
    okEq(WG_i_Count("glClear"), 0, "with no water parameters, nothing is"
                                   " cleared");

    /* And then the surface must not draw either, however complete the
       grid it is handed. A frame that prepared no reflection and drew
       a surface anyway would sample whatever the target last held. */
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    pst_GlobalsWaterParams = NULL;
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    okEq(WG_i_Count("glDrawElements"), 0, "and the surface is not drawn");

    /* The same from the other direction: parameters present, but the
       surface asked for before anything prepared it. */
    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    okEq(WG_i_Count("glDrawElements"), 0, "a surface drawn before the frame"
                                          " was prepared draws nothing");

    /* And the frame going away between one prepare and the next, with
       the target already built. This is the only check that reaches
       the "is there water this frame" answer rather than the "is there
       a target" one: before the first prepare there is no target
       either, so a draw refused then says nothing about which guard
       refused it. A mutant that dropped the frame's own answer
       survived everything above. */
    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    GL_PrepareWaterRendering();          /* builds it, and says yes */
    pst_GlobalsWaterParams = NULL;
    GL_PrepareWaterRendering();          /* same target, and says no */
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    okEq(WG_i_Count("glDrawElements"), 0, "a frame whose water went away"
                                          " draws no surface, target or"
                                          " no target");

    /* No context is no renderer. */
    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    gl_state.context = NULL;
    wg_callLen = 0;
    GL_PrepareWaterRendering();
    GL_DrawWater(&wg_water);
    okEq(wg_callLen, 0, "and without a context neither half calls anything");
}

static void caseWaterClear(void)
{
    int at;

    printf("-- the reflection target, cleared to what the water looks into\n");

    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    wg_callLen = 0;
    GL_PrepareWaterRendering();

    ok(WG_i_Count("glClear") >= 1, "the target is cleared");

    /* 0x20406080 as A8R8G8B8: alpha 0x20, red 0x40, green 0x60,
       blue 0x80. Checked a channel at a time, because the one mistake
       this cannot afford is reading the word the other way round --
       and a palindromic colour would hide exactly that. */
    at = wg_i_CallAt("glClearColor", 0);
    ok(at >= 0, "and a clear colour is set for it");
    if (at >= 0)
    {
        okEq(wg_calls[at].arg[0], wg_l_Scaled(0x40 / 255.0f),
             "red comes from the second byte down");
        okEq(wg_calls[at].arg[1], wg_l_Scaled(0x60 / 255.0f),
             "green from the third");
        okEq(wg_calls[at].arg[2], wg_l_Scaled(0x80 / 255.0f),
             "blue from the lowest");
        okEq(wg_calls[at].arg[3], wg_l_Scaled(0x20 / 255.0f),
             "and alpha from the top");
    }

    /* Square and the size the other renderer makes it, which is also
       the size the projection remap is written against. */
    at = wg_i_CallAt("glViewport", 0);
    ok(at >= 0, "the viewport is set for the target");
    if (at >= 0)
    {
        okEq(wg_calls[at].arg[2], 512, "512 across");
        okEq(wg_calls[at].arg[3], 512, "and 512 down");
    }

    /* And the frame is put back where it was being built. Left bound
       to the reflection target, the whole of the world after this
       point would go into a 512-square texture nobody shows. */
    {
        int clearAt = wg_i_CallAt("glClear", 0);
        int i;
        long back = -1;

        for (i = clearAt + 1; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glBindFramebuffer") == 0)
            {
                back = (long)wg_calls[i].arg[1];
                break;
            }
        ok(back > 0 && back != (long)wg_fboColor[0],
           "the scene is bound again afterwards, not the reflection");
    }
}

static void caseWaterStrip(void)
{
    int first;
    int second;

    printf("-- the index strip, uploaded when the grid changes and not"
           " per frame\n");

    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    GL_PrepareWaterRendering();

    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    first = WG_i_Count("glBufferData");
    okEq(WG_i_Count("glDrawElements"), 1, "the surface is drawn once");
    okEq(first, 2, "the first frame uploads two buffers, the strip and"
                   " the vertices");

    /* A strip for a four by three grid: the count is the arithmetic
       watermesh.h sets out, and it is asked of the module rather than
       written out here, because a figure copied into a test is a
       second place for it to be wrong. */
    {
        int at = wg_i_CallAt("glDrawElements", 0);

        ok(at >= 0, "and through an index buffer");
        if (at >= 0)
        {
            okEq(wg_calls[at].arg[0], GL_TRIANGLE_STRIP,
                 "as one triangle strip");
            okEq(wg_calls[at].arg[1],
                 WMS_i_IndexCount(WG_WTR_COLS, WG_WTR_ROWS),
                 "with every index the strip holds");
            okEq(wg_calls[at].arg[2], GL_UNSIGNED_SHORT,
                 "and sixteen-bit indices");
            okEq(wg_calls[at].arg[3], 0, "from the start of the buffer");
        }
    }

    /* A second frame on the same grid: the strip depends on nothing
       but the two counts, so re-uploading it every frame would be
       work for nothing. */
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    second = WG_i_Count("glBufferData");
    okEq(second, 1, "a second frame on the same grid uploads the vertices"
                    " only");
    okEq(WG_i_Count("glDrawElements"), 1, "and still draws");

    /* A grid of a different size, and it has to come back. */
    wg_WaterFixture(3, 2);
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    okEq(WG_i_Count("glBufferData"), 2, "a grid of another size uploads the"
                                        " strip again");
    {
        int at = wg_i_CallAt("glDrawElements", 0);

        if (at >= 0)
            okEq(wg_calls[at].arg[1], WMS_i_IndexCount(3, 2),
                 "and draws the new strip's length");
    }

    /* A grid too small to strip draws nothing rather than drawing
       whatever the buffer held. */
    reset();
    wg_WaterFixture(1, 2);
    GL_PrepareWaterRendering();
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    okEq(WG_i_Count("glDrawElements"), 0, "a grid one column wide draws"
                                          " nothing");
}

static void caseWaterWiring(void)
{
    const tWms_Vertex* v;
    int                i;

    printf("-- which arrays and which camera reach the vertex build\n");

    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    GL_PrepareWaterRendering();
    wg_callLen = 0;
    GL_DrawWater(&wg_water);

    /* The vertices go up last, so the recorded copy is them. */
    okEq(wg_bufferCopyLen,
         (long)(WG_WTR_NUM * (int)sizeof(tWms_Vertex)),
         "the whole grid is uploaded");
    v = (const tWms_Vertex*)wg_bufferCopy;

    /* The position array. Checked at both ends, because a grid read
       transposed or offset by a row agrees in the middle. */
    okEq(wg_l_Scaled(v[0].pos.x), wg_l_Scaled(wg_waterPos[0].x),
         "the first position is the grid's first point");
    okEq(wg_l_Scaled(v[0].pos.z), wg_l_Scaled(wg_waterPos[0].z),
         "in all three components");
    okEq(wg_l_Scaled(v[WG_WTR_NUM - 1].pos.y),
         wg_l_Scaled(wg_waterPos[WG_WTR_NUM - 1].y),
         "and the last is the grid's last");

    /* The camera. The horizontal component of the direction is the
       one with nothing else in it -- no wave, no fall-off -- so it is
       where a camera taken from the wrong place shows plainly. */
    for (i = 0; i < WG_WTR_NUM; i++)
        if (wg_l_Scaled(v[i].dir.x)
            != wg_l_Scaled(wg_waterPos[i].x - 7.0f))
            break;
    okEq(i, WG_WTR_NUM, "every direction is measured from the engine's"
                        " camera");

    /* The wave array reaches the depth component and the colour array
       the colour. Both are checked by what would be there if the
       array had not arrived: the depth would be the bare difference,
       and the colour would be nought. */
    ok(wg_l_Scaled(v[0].dir.z) != wg_l_Scaled(wg_waterPos[0].z - 13.0f),
       "the wave array reaches the depth component");
    okEq((long)v[0].diffuse, (long)WMS_u_SwapRedBlue(wg_waterCol[0]),
         "and the colour array reaches the colour");

    /* Per vertex, not once for the grid: the one point with a bigger
       wave is pushed further than its neighbours in the same row, and
       the row is what the fall-off depends on, so within a row the
       wave is the only thing left that can differ. */
    ok(fabs((double)v[1].dir.z - (double)(wg_waterPos[1].z - 13.0f))
       > fabs((double)v[0].dir.z - (double)(wg_waterPos[0].z - 13.0f)),
       "and is read per vertex, not once for the grid");

    /* The depth component, predicted rather than compared.
     *
     * Everything above is a comparison between two vertices, and a
     * comparison survives a great deal: a wave array swapped for the
     * position array, or the grid read with its rows and columns the
     * other way round, both leave every inequality above still true.
     * Both mutants did.
     *
     * So this states what the component must be, from the position,
     * the camera and the fall-off for that vertex's row -- the
     * fall-off asked of watermesh.c, which run_wingdi.sh has already
     * compared against the other renderer, rather than written out
     * again here. Which row a vertex is in is then part of the claim,
     * and so is which array the wave came from. */
    {
        int bad = -1;

        for (i = 0; i < WG_WTR_NUM; i++)
        {
            float scale = WMS_f_RippleScale((unsigned int)(i / WG_WTR_COLS),
                                            WG_WTR_ROWS);
            double ripple = wg_waterWave[i].y + wg_waterWave[i].x;
            float want = (float)((wg_waterPos[i].z - 13.0f)
                                 - fabs(ripple * scale));

            if (wg_l_Scaled(v[i].dir.z) != wg_l_Scaled(want))
            {
                bad = i;
                break;
            }
        }
        okEq(bad, -1, "every depth component is the wave and the row's"
                      " own fall-off");
    }

    /* The grid is read row by row with the near rows first, which is
       what the fall-off is a function of. Compared down a column, and
       not across the grid: with the wave flat, the row is the only
       thing left. A grid read the other way up ripples hardest at the
       far edge and nothing else here would notice. */
    ok(fabs((double)v[0].dir.z - (double)(wg_waterPos[0].z - 13.0f))
       > fabs((double)v[(WG_WTR_ROWS - 1) * WG_WTR_COLS].dir.z
              - (double)(wg_waterPos[(WG_WTR_ROWS - 1) * WG_WTR_COLS].z
                         - 13.0f)),
       "and the first row ripples harder than the last");
}

static void caseWaterBlur(void)
{
    printf("-- the wave blur, which runs before anything reads the array\n");

    /* Off by default, and then it must not run. */
    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    tweak_water_blur_normal = 0;
    wg_waterBlurCalls = 0;
    GL_PrepareWaterRendering();
    GL_DrawWater(&wg_water);
    okEq(wg_waterBlurCalls, 0, "with the tweak off the blur does not run");

    /* On, and it runs over the engine's own wave array -- not over a
       copy, because it rewrites the array in place and the vertex
       build reads that array afterwards. */
    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    tweak_water_blur_normal = 1;
    wg_waterBlurCalls = 0;
    wg_waterBlurArg = NULL;
    GL_PrepareWaterRendering();
    GL_DrawWater(&wg_water);
    okEq(wg_waterBlurCalls, 1, "with it on the blur runs once a frame");
    ok(wg_waterBlurArg == (void*)wg_waterWave,
       "over the engine's wave array itself");
    tweak_water_blur_normal = 0;
}

static void caseWaterState(void)
{
    int i;
    int depthTest = 0;
    int blendOff = 0;
    int cullOff = 0;
    int maskOff = -1;
    int maskBackOn = -1;

    printf("-- the state the surface is drawn with\n");

    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    GL_PrepareWaterRendering();
    wg_callLen = 0;
    GL_DrawWater(&wg_water);

    for (i = 0; i < wg_callLen; i++)
    {
        const char* n = wg_calls[i].name;

        if (strcmp(n, "glEnable") == 0 && wg_calls[i].arg[0] == GL_DEPTH_TEST)
            depthTest = 1;
        if (strcmp(n, "glDisable") == 0 && wg_calls[i].arg[0] == GL_BLEND)
            blendOff = 1;
        if (strcmp(n, "glDisable") == 0 && wg_calls[i].arg[0] == GL_CULL_FACE)
            cullOff = 1;
        if (strcmp(n, "glDepthMask") == 0)
        {
            if (!wg_calls[i].arg[0] && maskOff < 0)
                maskOff = i;
            if (wg_calls[i].arg[0] && maskOff >= 0 && i > maskOff)
                maskBackOn = i;
        }
    }

    ok(depthTest, "the depth test is on, so the surface can be hidden");
    {
        int at = wg_i_CallAt("glDepthFunc", 0);

        ok(at >= 0 && wg_calls[at].arg[0] == GL_LEQUAL,
           "and compares less-or-equal, as the other renderer does");
    }

    /* Not written. The surface is one plane and the after-effects read
       the depth this frame leaves, so writing it would both fight
       itself where the strip doubles back and change what they see. */
    ok(maskOff >= 0, "depth writing is switched off for the draw");
    {
        int drawAt = wg_i_CallAt("glDrawElements", 0);

        ok(maskOff >= 0 && drawAt > maskOff, "before the draw");
        ok(maskBackOn > drawAt, "and back on after it, which everything"
                                " downstream expects");
    }

    ok(blendOff, "blending is off -- the mixing is the shader's");

    /* Both sides. The other renderer sets no culling for this draw and
       inherits whatever was last in force, which specifies nothing;
       drawing both sides is the reading that is the same whatever was
       inherited. Asserted as the decision it is, not as a measurement
       of that renderer. */
    ok(cullOff, "and culling is off, so the surface draws from either"
                " side");
}

static void caseWaterUniforms(void)
{
    printf("-- the uniforms, and the flip the console can move\n");

    reset();
    wg_WaterFixture(WG_WTR_COLS, WG_WTR_ROWS);
    GL_PrepareWaterRendering();

    /* Three matrices go up: the combined one for the position, the
       view for the fog and the direction, and the remap for the
       reflection. */
    wg_callLen = 0;
    wg_matrixNum = 0;
    GL_DrawWater(&wg_water);
    okEq(WG_i_Count("glUniformMatrix4fv"), 3, "three matrices reach the"
                                              " program");
    okEq(wg_lastTranspose, GL_FALSE, "uploaded untransposed, which is what"
                                     " makes the engine's row-vector"
                                     " arithmetic come out right");

    /* The third of them is the reflection's remap, and it is built
       from the projection alone -- not from the projection with the
       view already in it. With the view left at the identity the two
       are the same matrix and the difference is invisible, which is
       how a mutant that read the combined one survived. So the view
       is given something of its own here first.
     *
     * The remap itself is asked of watermesh.c rather than written out
     * again: run_wingdi.sh has already compared that arithmetic against
     * the other renderer, and what is in question here is only which
     * matrix was handed to it. */
    {
        Mtx4x4 view;
        Mtx4x4WithoutAdditionalData want;
        int    i;
        int    bad = -1;

        MATH_SetIdentityMatrix(&view);
        view.Pos.x = 3.0f;
        view.Pos.z = -7.0f;
        GL_SetViewMatrix(&view);
        MATH_SetIdentityMatrix(&gl_projMtx);
        gl_projMtx.v1.x = 1.75f;
        gl_projMtx.v2.y = -2.25f;
        GL_l_CombineMatrices();

        wg_callLen = 0;
        wg_matrixNum = 0;
        GL_DrawWater(&wg_water);
        okEq(wg_matrixNum, 3, "three again, with a view of its own");

        WMS_ProjectionToTexture(&want,
                                (const Mtx4x4WithoutAdditionalData*)&gl_projMtx,
                                512, 512);
        for (i = 0; i < 16; i++)
            if (wg_l_Scaled(wg_matrixData[2][i])
                != wg_l_Scaled(((const float*)&want)[i]))
            {
                bad = i;
                break;
            }
        okEq(bad, -1, "and the remap is the projection's, with no view"
                      " folded into it");
    }

    /* The flip, at nought by default and moved by the tweak. It is a
       tweak rather than a constant because the answer is a
       measurement: the one time a flip here was derived from the two
       APIs' conventions instead, it came out backwards and two
       harnesses agreed with it. */
    tweak_gl_water_flip = 1;
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    {
        int i;
        int sawOne = 0;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glUniform1i") == 0
                && wg_calls[i].arg[1] == 1)
                sawOne = 1;
        ok(sawOne, "the console's flip reaches the program");
    }
    tweak_gl_water_flip = 0;
    wg_callLen = 0;
    GL_DrawWater(&wg_water);
    {
        int i;
        int sawOne = 0;

        for (i = 0; i < wg_callLen; i++)
            if (strcmp(wg_calls[i].name, "glUniform1i") == 0
                && wg_calls[i].arg[1] == 1)
                sawOne = 1;
        ok(!sawOne, "and with it back at nought, nothing asks for a flip");
    }
}

int main(void)
{
    printf("=== GL overlay pass unit test ===\n");
    /* The sine table, which the sprite wobble reads through
       MATH_GetSin. Without it every sine is nought and the wobble is
       a no-op that nothing can see. */
    MATH_InitModule();
    WG_AimThePointers();
    WG_CheckThePointers();
    caseLoadsTheRealShaders();
    caseState();
    caseLayouts();
    caseFlatBatch();
    caseAtlas();
    caseTextBatch();
    caseRefusals();
    caseTeardown();
    caseSceneCopy();
    caseAeDepthBlurChain();
    caseAeDepthBlurRefusals();
    caseAeDepthBlurUniforms();
    caseAeDepthBlurQuads();
    caseAeColorChain();
    caseAeColorBand();
    caseAeColorGate();
    caseAeMotionBlurHistory();
    caseAeRemanenceChain();
    caseAeRemanenceAfterDepthBlur();
    caseAeZoomUniforms();
    caseAeFrameGates();
    caseFlipDrawsOverlay();
    caseGeometry();
    caseGeometryNormals();
    caseGeometryCulling();
    caseGeometryCullingTweak();
    caseWaterRefusals();
    caseWaterClear();
    caseWaterStrip();
    caseWaterWiring();
    caseWaterBlur();
    caseWaterState();
    caseWaterUniforms();
    caseViewport();
    caseNoViewport();
    caseClipControl();
    caseTextureUpload();
    caseTextureLevels();
    caseSampler();
    caseMaterialState();
    caseAlphaTest();
    caseUntextured();
    caseProceduralUpload();
    caseTextureTeardown();
    caseTargetsBuild();
    caseTargetsBind();
    caseTargetsRefusals();
    caseTargetsMatrices();
    caseShadowAsTexture();
    caseDepthReadBack();
    caseTargetsTeardown();
    caseShadowSequence();
    caseSprites();
    caseDeviceColour();
    caseVideo();
    caseVideoFit();
    caseVideoTeardown();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
