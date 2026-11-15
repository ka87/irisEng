#pragma once
#include "glcore.h"

/* GLSL, loaded from data\shaders at run time rather than compiled in.

   Loaded rather than embedded because a shader is the part of a
   renderer most likely to need changing on somebody else's machine,
   and a file that can be edited and the game restarted is worth more
   than one that needs a rebuild. A missing or broken shader is
   reported with the driver's own compile log and the renderer declines
   to draw with it, which is the same shape as a missing font: logged
   once, and then nothing.

   Everything here goes through the function pointers in glcore.h, so
   a test can drive the whole of it with no driver present. */

typedef struct sGlProgram
{
    GLuint program;
    GLuint vertex;
    GLuint fragment;
} tGlProgram;

/* Compiles data\shaders\<vertName> and <fragName> and links them.

   Returns 0 and leaves the program empty if either file is missing,
   either stage will not compile, or the two will not link together.
   The driver's log is what gets printed -- it is the only thing that
   says which line. */
int GL_b_ProgramLoad(tGlProgram* out, const char* vertName, const char* fragName);

/* Gives the program and both stages back. Safe on an empty one and
   safe twice. */
void GL_ProgramFree(tGlProgram* p);

/* The location of a uniform, or -1. Logged when it is missing, because
   a uniform that is not there is usually a name that does not match
   rather than a uniform the shader chose not to declare -- and the
   draw that follows would silently do nothing useful. */
GLint GL_i_ProgramUniform(const tGlProgram* p, const char* name);

/* How much of a shader file this will read. A GLSL source past this is
   a shader that has outgrown what the renderer is doing with it. */
#define GL_SHADERMAX 16384
