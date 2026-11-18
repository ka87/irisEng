#include "glshader.h"
#include "../datapath.h"
#include "../dbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reads a whole shader file into buf. Returns its length, or -1. */
static int GL_l_i_ReadFile(const char* name, char* buf, int max)
{
    char  path[512];
    FILE* f;
    int   n;

    if (!DPT_b_DataPath("shaders", name, path, sizeof(path)))
    {
        DBG_OutputDebugString("GL: cannot work out where %s should be\n", name);
        return -1;
    }
    f = fopen(path, "rb");
    if (!f)
    {
        DBG_OutputDebugString("GL: no %s\n", path);
        return -1;
    }
    n = (int)fread(buf, 1, (size_t)max - 1, f);
    if (n < 0)
        n = 0;
    buf[n] = 0;

    /* A file that filled the buffer has more to it, and a shader
       truncated mid-statement produces a compile error about the
       wrong thing. */
    if (n == max - 1)
    {
        DBG_OutputDebugString("GL: %s is longer than %d bytes\n", name, max - 1);
        fclose(f);
        return -1;
    }
    fclose(f);
    if (n == 0)
    {
        DBG_OutputDebugString("GL: %s is empty\n", name);
        return -1;
    }
    return n;
}

/* Prints whatever the driver has to say about a shader or a program.
   This is the only thing that names a line number, so it is worth the
   buffer. */
static void GL_l_SayLog(const char* what, const char* name, GLuint object,
                        int isProgram)
{
    char    log[1024];
    GLsizei len = 0;

    log[0] = 0;
    if (isProgram)
        GLFN_glGetProgramInfoLog(object, (GLsizei)sizeof(log), &len, log);
    else
        GLFN_glGetShaderInfoLog(object, (GLsizei)sizeof(log), &len, log);
    if (len > (GLsizei)sizeof(log) - 1)
        len = (GLsizei)sizeof(log) - 1;
    if (len < 0)
        len = 0;
    log[len] = 0;
    DBG_OutputDebugString("GL: %s %s:\n%s\n", what, name, log);
}

static GLuint GL_l_ui_CompileStage(GLenum type, const char* name)
{
    char*              source;
    const GLchar*      strings[1];
    GLuint             shader;
    GLint              ok = GL_FALSE;
    int                len;

    source = (char*)malloc(GL_SHADERMAX);
    if (!source)
        return 0;
    len = GL_l_i_ReadFile(name, source, GL_SHADERMAX);
    if (len < 0)
    {
        free(source);
        return 0;
    }

    shader = GLFN_glCreateShader(type);
    if (!shader)
    {
        free(source);
        DBG_OutputDebugString("GL: no shader object for %s\n", name);
        return 0;
    }
    strings[0] = source;
    GLFN_glShaderSource(shader, 1, strings, NULL);
    GLFN_glCompileShader(shader);
    free(source);

    GLFN_glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        GL_l_SayLog("will not compile", name, shader, 0);
        GLFN_glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int GL_b_ProgramLoad(tGlProgram* out, const char* vertName, const char* fragName)
{
    GLint ok = GL_FALSE;

    if (!out)
        return 0;
    memset(out, 0, sizeof(*out));
    if (!vertName || !fragName)
        return 0;

    out->vertex = GL_l_ui_CompileStage(GL_VERTEX_SHADER, vertName);
    if (!out->vertex)
        return 0;
    out->fragment = GL_l_ui_CompileStage(GL_FRAGMENT_SHADER, fragName);
    if (!out->fragment)
    {
        GL_ProgramFree(out);
        return 0;
    }

    out->program = GLFN_glCreateProgram();
    if (!out->program)
    {
        DBG_OutputDebugString("GL: no program object for %s\n", vertName);
        GL_ProgramFree(out);
        return 0;
    }
    GLFN_glAttachShader(out->program, out->vertex);
    GLFN_glAttachShader(out->program, out->fragment);
    GLFN_glLinkProgram(out->program);
    GLFN_glGetProgramiv(out->program, GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        GL_l_SayLog("will not link with", fragName, out->program, 1);
        GL_ProgramFree(out);
        return 0;
    }

    /* The stages are detached once linked: the program holds what it
       needs, and leaving them attached keeps two objects alive for
       nothing. They are deleted here too, which marks them for
       deletion rather than deleting them under the program. */
    GLFN_glDetachShader(out->program, out->vertex);
    GLFN_glDetachShader(out->program, out->fragment);
    GLFN_glDeleteShader(out->vertex);
    GLFN_glDeleteShader(out->fragment);
    out->vertex = 0;
    out->fragment = 0;
    return 1;
}

void GL_ProgramFree(tGlProgram* p)
{
    if (!p)
        return;
    if (p->program)
        GLFN_glDeleteProgram(p->program);
    if (p->vertex)
        GLFN_glDeleteShader(p->vertex);
    if (p->fragment)
        GLFN_glDeleteShader(p->fragment);
    memset(p, 0, sizeof(*p));
}

GLint GL_i_ProgramUniform(const tGlProgram* p, const char* name)
{
    GLint loc;

    if (!p || !p->program || !name)
        return -1;
    loc = GLFN_glGetUniformLocation(p->program, name);
    if (loc < 0)
        DBG_OutputDebugString("GL: no uniform %s\n", name);
    return loc;
}
