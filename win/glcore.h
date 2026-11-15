#pragma once
#include <stddef.h>

/* OpenGL 4.5 core, declared here because Windows ships headers for
   1.1 and nothing else: everything past that is reached through a
   function pointer the driver hands over at run time. So this file is
   the part of the GL headers the renderer actually uses, written out,
   and glload.c is what fills the pointers in.

   The list grows as the renderer does. It holds what is used and
   nothing else -- an entry point that is declared but never called is
   a name that has to resolve for no reason, and on a driver that
   happens not to export it that is a renderer that refuses to start
   over a function it would never have called. */

typedef unsigned int   GLenum;
typedef unsigned int   GLbitfield;
typedef unsigned int   GLuint;
typedef int            GLint;
typedef int            GLsizei;
typedef unsigned char  GLboolean;
typedef signed char    GLbyte;
typedef short          GLshort;
typedef unsigned char  GLubyte;
typedef unsigned short GLushort;
typedef unsigned long  GLulong;
typedef float          GLfloat;
typedef float          GLclampf;
typedef double         GLdouble;
typedef double         GLclampd;
typedef void           GLvoid;
typedef char           GLchar;

#define GL_FALSE                          0
#define GL_TRUE                           1

#define GL_NO_ERROR                       0
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

#define GL_SCISSOR_TEST                   0x0C11
#define GL_DEPTH_TEST                     0x0B71
#define GL_CULL_FACE                      0x0B44
#define GL_BLEND                          0x0BE2

#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                         0x1F02
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_CONTEXT_PROFILE_MASK           0x9126
#define GL_CONTEXT_CORE_PROFILE_BIT       0x00000001

#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_DONT_CARE                      0x1100

typedef void (*GL_pfn_DebugProc)(GLenum source, GLenum type, GLuint id,
                                 GLenum severity, GLsizei length,
                                 const GLchar* message, const void* user);

/* Every entry point the renderer uses, once, in one place.
   X(returntype, name, parameterlist, argumentlist)

   The list is what the loader walks, what the declarations below are
   made from, and what the test checks -- so a function cannot be
   added to one of those three and missed in the others. */
#define GL_FUNCTION_LIST(X)                                                   \
    X(const GLubyte*, glGetString,   (GLenum name), (name))                   \
    X(void,  glGetIntegerv,          (GLenum pname, GLint* data), (pname, data)) \
    X(GLenum, glGetError,            (void), ())                              \
    X(void,  glClear,                (GLbitfield mask), (mask))               \
    X(void,  glClearColor,           (GLclampf r, GLclampf g, GLclampf b,     \
                                      GLclampf a), (r, g, b, a))              \
    X(void,  glClearDepth,           (GLclampd depth), (depth))               \
    X(void,  glClearStencil,         (GLint s), (s))                          \
    X(void,  glViewport,             (GLint x, GLint y, GLsizei w, GLsizei h), \
                                     (x, y, w, h))                            \
    X(void,  glScissor,              (GLint x, GLint y, GLsizei w, GLsizei h), \
                                     (x, y, w, h))                            \
    X(void,  glEnable,               (GLenum cap), (cap))                     \
    X(void,  glDisable,              (GLenum cap), (cap))                     \
    X(void,  glDepthMask,            (GLboolean on), (on))                    \
    X(void,  glColorMask,            (GLboolean r, GLboolean g, GLboolean b,  \
                                      GLboolean a), (r, g, b, a))             \
    X(void,  glFinish,               (void), ())                              \
    X(void,  glFlush,                (void), ())                              \
    X(void,  glDebugMessageCallback, (GL_pfn_DebugProc proc, const void* user), \
                                     (proc, user))                            \
    X(void,  glDebugMessageControl,  (GLenum source, GLenum type,             \
                                      GLenum severity, GLsizei count,         \
                                      const GLuint* ids, GLboolean enabled),  \
                                     (source, type, severity, count, ids, enabled))

/* One function-pointer variable per entry point, named after it with
   a prefix, plus a macro so call sites can write the GL name. */
#define GL_DECLARE(ret, name, params, args) extern ret (*GLFN_##name) params;
GL_FUNCTION_LIST(GL_DECLARE)
#undef GL_DECLARE

#define GL_REDIRECT(ret, name, params, args)
GL_FUNCTION_LIST(GL_REDIRECT)
#undef GL_REDIRECT
