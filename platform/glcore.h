#pragma once
#include <stddef.h>

/* OpenGL core, declared here because Windows ships headers for
   1.1 and nothing else: everything past that is reached through a
   function pointer the driver hands over at run time. So this file is
   the part of the GL headers the renderer actually uses, written out,
   and glload.c is what fills the pointers in.

   The list grows as the renderer does. It holds what is used and
   nothing else -- an entry point that is declared but never called is
   a name that has to resolve for no reason, and on a driver that
   happens not to export it that is a renderer that refuses to start
   over a function it would never have called. */

/* Every GL and WGL entry point on Windows is __stdcall, and so is a
   callback the driver invokes. A pointer declared without it is
   called as though the caller cleans up a stack the callee has
   already cleaned, which corrupts the stack on every single call.
   MSVC's run-time checks catch the first one -- "the value of ESP was
   not properly saved across a function call" -- and nothing catches
   the rest. Elsewhere the platform default is the right convention
   and this is empty. */
#if defined(_WIN32)
#define GL_APIENTRY __stdcall
#else
#define GL_APIENTRY
#endif

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
/* Pointer-width and 64-bit types, for the buffer mapping and the fences
   the sun's readback uses. GLsync is an opaque handle, not an id. */
typedef ptrdiff_t      GLsizeiptr;
typedef ptrdiff_t      GLintptr;
typedef unsigned long long GLuint64;
typedef struct __GLsync* GLsync;

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

/* The sun's occlusion test reads nine pixels back out of the frame,
   and does it without stalling: the reads go into a buffer and a fence
   says when the card has finished with them. */
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_STREAM_READ                    0x88E1
#define GL_MAP_READ_BIT                   0x0001
#define GL_SYNC_GPU_COMMANDS_COMPLETE     0x9117
#define GL_ALREADY_SIGNALED               0x911A
#define GL_TIMEOUT_EXPIRED                0x911B
#define GL_CONDITION_SATISFIED            0x911C
#define GL_WAIT_FAILED                    0x911D

#define GL_ARRAY_BUFFER                   0x8892
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_FLOAT                          0x1406
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_UNSIGNED_SHORT                 0x1403

#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84

#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_R8                             0x8229
#define GL_RED                            0x1903
#define GL_RGBA                           0x1908
#define GL_RGBA8                          0x8058
/* The engine's textures are A8R8G8B8 words, which on a little-endian
   machine is blue, green, red, alpha in memory. */
#define GL_BGRA                           0x80E1
#define GL_LINEAR                         0x2601
#define GL_NEAREST                        0x2600
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_REPEAT                         0x2901
#define GL_MIRRORED_REPEAT                0x8370
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_UNPACK_ROW_LENGTH              0x0CF2

/* Framebuffer objects: a renderer draws into one of these instead of
   the window when it needs the result as a texture. */
#define GL_FRAMEBUFFER                    0x8D40
/* The two halves of a framebuffer binding, for the copy that puts the
   scene on the screen. */
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_RENDERBUFFER                   0x8D41
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_VIEWPORT                       0x0BA2

#define GL_FILL                           0x1B02
#define GL_LINE                           0x1B01
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408
#define GL_CW                             0x0900
#define GL_CCW                            0x0901

#define GL_CLIP_ORIGIN                    0x935C
#define GL_CLIP_DEPTH_MODE                0x935D
#define GL_NEGATIVE_ONE_TO_ONE            0x935E
#define GL_ZERO_TO_ONE                    0x935F
#define GL_CLIP_DISTANCE0                 0x3000
#define GL_LOWER_LEFT                     0x8CA1
#define GL_UPPER_LEFT                     0x8CA2

#define GL_LEQUAL                         0x0203
#define GL_EQUAL                          0x0202

#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_ONE                            1
#define GL_ZERO                           0

#define GL_NUM_EXTENSIONS                 0x821D
#define GL_EXTENSIONS                     0x1F03
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_DONT_CARE                      0x1100

typedef void (GL_APIENTRY *GL_pfn_DebugProc)(GLenum source, GLenum type, GLuint id,
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
    X(void,  glCullFace,             (GLenum face), (face))                   \
    X(void,  glFrontFace,            (GLenum winding), (winding))             \
    X(void,  glColorMask,            (GLboolean r, GLboolean g, GLboolean b,  \
                                      GLboolean a), (r, g, b, a))             \
    X(void,  glFinish,               (void), ())                              \
    X(void,  glFlush,                (void), ())                              \
    X(void,  glBlendFunc,            (GLenum src, GLenum dst), (src, dst))    \
    X(void,  glPixelStorei,          (GLenum pname, GLint param), (pname, param)) \
    X(void,  glPolygonMode,          (GLenum face, GLenum mode), (face, mode)) \
                                                                              \
    X(GLuint, glCreateShader,        (GLenum type), (type))                   \
    X(void,  glShaderSource,         (GLuint shader, GLsizei count,           \
                                      const GLchar* const* strings,           \
                                      const GLint* lengths),                  \
                                     (shader, count, strings, lengths))       \
    X(void,  glCompileShader,        (GLuint shader), (shader))               \
    X(void,  glGetShaderiv,          (GLuint shader, GLenum pname, GLint* out), \
                                     (shader, pname, out))                    \
    X(void,  glGetShaderInfoLog,     (GLuint shader, GLsizei max,             \
                                      GLsizei* len, GLchar* log),             \
                                     (shader, max, len, log))                 \
    X(void,  glDeleteShader,         (GLuint shader), (shader))               \
    X(GLuint, glCreateProgram,       (void), ())                              \
    X(void,  glAttachShader,         (GLuint program, GLuint shader),          \
                                     (program, shader))                       \
    X(void,  glDetachShader,         (GLuint program, GLuint shader),          \
                                     (program, shader))                       \
    X(void,  glLinkProgram,          (GLuint program), (program))             \
    X(void,  glGetProgramiv,         (GLuint program, GLenum pname, GLint* out), \
                                     (program, pname, out))                   \
    X(void,  glGetProgramInfoLog,    (GLuint program, GLsizei max,            \
                                      GLsizei* len, GLchar* log),             \
                                     (program, max, len, log))                \
    X(void,  glDeleteProgram,        (GLuint program), (program))             \
    X(void,  glUseProgram,           (GLuint program), (program))             \
    X(GLint, glGetUniformLocation,   (GLuint program, const GLchar* name),     \
                                     (program, name))                         \
    X(void,  glUniform1i,            (GLint loc, GLint v), (loc, v))          \
    X(void,  glUniform1f,            (GLint loc, GLfloat v), (loc, v))        \
    X(void,  glUniform2f,            (GLint loc, GLfloat a, GLfloat b),       \
                                     (loc, a, b))                             \
    X(void,  glUniform3f,            (GLint loc, GLfloat a, GLfloat b,        \
                                      GLfloat c), (loc, a, b, c))             \
    X(void,  glUniform4f,            (GLint loc, GLfloat a, GLfloat b,         \
                                      GLfloat c, GLfloat d), (loc, a, b, c, d)) \
    X(void,  glUniformMatrix4fv,     (GLint loc, GLsizei count,                \
                                      GLboolean transpose,                      \
                                      const GLfloat* value),                    \
                                     (loc, count, transpose, value))           \
    X(void,  glClipControl,          (GLenum origin, GLenum depth),             \
                                     (origin, depth))                          \
    X(void,  glDepthFunc,            (GLenum func), (func))                    \
                                                                              \
    X(void,  glGenBuffers,           (GLsizei n, GLuint* out), (n, out))      \
    X(void,  glDeleteBuffers,        (GLsizei n, const GLuint* in), (n, in))  \
    X(void,  glBindBuffer,           (GLenum target, GLuint buffer),           \
                                     (target, buffer))                        \
    X(void,  glBufferData,           (GLenum target, GLsizei size,            \
                                      const void* data, GLenum usage),         \
                                     (target, size, data, usage))             \
    X(void*, glMapBufferRange,       (GLenum target, GLintptr offset,         \
                                      GLsizeiptr length, GLbitfield access),  \
                                     (target, offset, length, access))        \
    X(GLboolean, glUnmapBuffer,      (GLenum target), (target))               \
    X(void,  glReadPixels,           (GLint x, GLint y, GLsizei w, GLsizei h,  \
                                      GLenum format, GLenum type,             \
                                      void* data),                            \
                                     (x, y, w, h, format, type, data))        \
    X(GLsync, glFenceSync,           (GLenum condition, GLbitfield flags),     \
                                     (condition, flags))                      \
    X(GLenum, glClientWaitSync,      (GLsync sync, GLbitfield flags,           \
                                      GLuint64 timeout),                      \
                                     (sync, flags, timeout))                  \
    X(void,  glDeleteSync,           (GLsync sync), (sync))                   \
    X(void,  glGenVertexArrays,      (GLsizei n, GLuint* out), (n, out))      \
    X(void,  glDeleteVertexArrays,   (GLsizei n, const GLuint* in), (n, in))  \
    X(void,  glBindVertexArray,      (GLuint array), (array))                 \
    X(void,  glEnableVertexAttribArray, (GLuint index), (index))              \
    X(void,  glDisableVertexAttribArray, (GLuint index), (index))             \
    X(void,  glVertexAttribPointer,  (GLuint index, GLint size, GLenum type,   \
                                      GLboolean normalized, GLsizei stride,    \
                                      const void* offset),                     \
                                     (index, size, type, normalized, stride, offset)) \
    X(void,  glDrawArrays,           (GLenum mode, GLint first, GLsizei count), \
                                     (mode, first, count))                    \
    X(void,  glDrawElements,         (GLenum mode, GLsizei count, GLenum type, \
                                      const void* offset),                     \
                                     (mode, count, type, offset))             \
                                                                              \
    X(void,  glGenTextures,          (GLsizei n, GLuint* out), (n, out))      \
    X(void,  glDeleteTextures,       (GLsizei n, const GLuint* in), (n, in))  \
    X(void,  glBindTexture,          (GLenum target, GLuint texture),          \
                                     (target, texture))                       \
    X(void,  glActiveTexture,        (GLenum unit), (unit))                   \
    X(void,  glTexImage2D,           (GLenum target, GLint level,             \
                                      GLint internalFormat, GLsizei w,         \
                                      GLsizei h, GLint border, GLenum format,  \
                                      GLenum type, const void* pixels),        \
                                     (target, level, internalFormat, w, h,     \
                                      border, format, type, pixels))          \
    X(void,  glTexParameteri,        (GLenum target, GLenum pname, GLint v),   \
                                     (target, pname, v))                      \
    X(void,  glTexParameterf,        (GLenum target, GLenum pname, GLfloat v), \
                                     (target, pname, v))                      \
    X(void,  glTexSubImage2D,        (GLenum target, GLint level, GLint x,     \
                                      GLint y, GLsizei w, GLsizei h,          \
                                      GLenum format, GLenum type,             \
                                      const void* pixels),                    \
                                     (target, level, x, y, w, h, format,       \
                                      type, pixels))                          \
    X(void,  glGenerateMipmap,       (GLenum target), (target))               \
                                                                              \
    X(void,  glGenFramebuffers,      (GLsizei n, GLuint* out), (n, out))      \
    X(void,  glDeleteFramebuffers,   (GLsizei n, const GLuint* in), (n, in))  \
    X(void,  glBindFramebuffer,      (GLenum target, GLuint fb),               \
                                     (target, fb))                           \
    X(void,  glFramebufferTexture2D, (GLenum target, GLenum attachment,        \
                                      GLenum texTarget, GLuint texture,        \
                                      GLint level),                            \
                                     (target, attachment, texTarget, texture,  \
                                      level))                                 \
    X(GLenum, glCheckFramebufferStatus, (GLenum target), (target))            \
    X(void,  glBlitFramebuffer,      (GLint sx0, GLint sy0, GLint sx1,         \
                                      GLint sy1, GLint dx0, GLint dy0,        \
                                      GLint dx1, GLint dy1,                   \
                                      GLbitfield mask, GLenum filter),        \
                                     (sx0, sy0, sx1, sy1, dx0, dy0, dx1, dy1, \
                                      mask, filter))                          \
    X(void,  glGenRenderbuffers,     (GLsizei n, GLuint* out), (n, out))      \
    X(void,  glDeleteRenderbuffers,  (GLsizei n, const GLuint* in), (n, in))  \
    X(void,  glBindRenderbuffer,     (GLenum target, GLuint rb),               \
                                     (target, rb))                           \
    X(void,  glRenderbufferStorage,  (GLenum target, GLenum format,            \
                                      GLsizei w, GLsizei h),                   \
                                     (target, format, w, h))                  \
    X(void,  glFramebufferRenderbuffer, (GLenum target, GLenum attachment,     \
                                      GLenum rbTarget, GLuint rb),             \
                                     (target, attachment, rbTarget, rb))      \
                                                                              \
    X(const GLubyte*, glGetStringi,  (GLenum name, GLuint index),             \
                                     (name, index))

/* The ones a context is allowed not to have.
   Same shape, loaded the same way, but a missing one is not a reason
   to refuse the context: the loader counts these separately and the
   caller decides.

   Both of these are core from 4.3 and come from KHR_debug before
   that. Nothing in the engine calls either -- they are here for a
   debugger's sake -- so a 4.1 context without them draws exactly the
   same frame. glClipControl is the opposite case and is in the
   required list above: it is core from 4.5 and comes from
   ARB_clip_control before that, and the renderer cannot draw a
   correct frame without one or the other, because the engine's
   projections put z in Direct3D's range. */
#define GL_FUNCTION_LIST_OPTIONAL(X)                                          \
    X(void,  glDebugMessageCallback, (GL_pfn_DebugProc proc, const void* user), \
                                     (proc, user))                            \
    X(void,  glDebugMessageControl,  (GLenum source, GLenum type,             \
                                      GLenum severity, GLsizei count,         \
                                      const GLuint* ids, GLboolean enabled),  \
                                     (source, type, severity, count, ids, enabled))

/* One function-pointer variable per entry point, named after it with
   a prefix, plus a macro so call sites can write the GL name. */
#define GL_DECLARE(ret, name, params, args) \
    extern ret (GL_APIENTRY *GLFN_##name) params;
GL_FUNCTION_LIST(GL_DECLARE)
GL_FUNCTION_LIST_OPTIONAL(GL_DECLARE)
#undef GL_DECLARE

#define GL_REDIRECT(ret, name, params, args)
GL_FUNCTION_LIST(GL_REDIRECT)
GL_FUNCTION_LIST_OPTIONAL(GL_REDIRECT)
#undef GL_REDIRECT
