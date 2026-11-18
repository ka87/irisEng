/* A unit test for platform/glshader.c.
 *
 * New code with no earlier copy, so a direct check.
 *
 * Every GL call in the shader loader goes through a function pointer
 * in glcore.h, which is what makes this possible with no driver
 * anywhere: the pointers are aimed at mocks here, and a case can say
 * "this stage will not compile" or "the link fails" and see exactly
 * what the loader does about it -- including what it cleans up, which
 * is the part that is invisible on a real machine until something runs
 * out of objects.
 *
 * The files are real. The loader reads them through datapath.c, whose
 * idea of the executable is pointed at a directory this test makes, so
 * the bytes the driver would be handed are the bytes on disk.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

#include "../platform/glshader.h"

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

static void okStr(const char* got, const char* want, const char* what)
{
    checks++;
    if (strcmp(got, want) != 0)
    {
        printf("  FAIL %s:\n    got  \"%s\"\n    want \"%s\"\n", what, got, want);
        fails++;
    }
}

/* ------------------------------------------------------- the log, mocked */

static char gsLog[64][512];
static int  gsLogLen;

void DBG_OutputDebugString(char* format, ...)
{
    va_list ap;

    if (gsLogLen >= 64)
        return;
    va_start(ap, format);
    vsnprintf(gsLog[gsLogLen], sizeof(gsLog[0]), format, ap);
    va_end(ap);
    gsLogLen++;
}

static int GS_i_Logged(const char* needle)
{
    int i, n = 0;

    for (i = 0; i < gsLogLen; i++)
        if (strstr(gsLog[i], needle))
            n++;
    return n;
}

/* ---------------------------------------------------- the exe, mocked */

#include "sdlpath.h"

/* ------------------------------------------------------- GL, mocked */

#define GS_MAXOBJ 64

static int    gs_nextName = 100;
static int    gs_createShaderFailsFor;   /* a GLenum, or 0 */
static int    gs_createProgramFails;
static int    gs_compileFailsFor;        /* a shader name, or 0 */
static int    gs_compileFailsForType;    /* a GLenum, or 0 */
static int    gs_linkFails;
static GLint  gs_uniformLocation = 7;
static int    gs_uniformCalls;

static GLuint gs_shadersAlive[GS_MAXOBJ];
static int    gs_shadersAliveLen;
static GLuint gs_programsAlive[GS_MAXOBJ];
static int    gs_programsAliveLen;

/* what source each shader was handed, so a case can check the bytes */
static char   gs_source[GS_MAXOBJ][512];
static GLuint gs_sourceOf[GS_MAXOBJ];
static int    gs_sourceLen;

static GLenum gs_typeOf[GS_MAXOBJ];
static GLuint gs_typeName[GS_MAXOBJ];
static int    gs_typeLen;

static int    gs_attachCount;
static int    gs_detachCount;
static int    gs_linkCount;

static void gs_remember(GLuint* list, int* len, GLuint name)
{
    if (*len < GS_MAXOBJ)
        list[(*len)++] = name;
}

static void gs_forget(GLuint* list, int* len, GLuint name)
{
    int i;

    for (i = 0; i < *len; i++)
        if (list[i] == name)
        {
            list[i] = list[--(*len)];
            return;
        }
}

static GLenum gs_typeFor(GLuint shader)
{
    int i;

    for (i = 0; i < gs_typeLen; i++)
        if (gs_typeName[i] == shader)
            return gs_typeOf[i];
    return 0;
}

static GLuint GL_APIENTRY MockCreateShader(GLenum type)
{
    GLuint name;

    if (gs_createShaderFailsFor && (GLenum)gs_createShaderFailsFor == type)
        return 0;
    name = (GLuint)gs_nextName++;
    gs_remember(gs_shadersAlive, &gs_shadersAliveLen, name);
    if (gs_typeLen < GS_MAXOBJ)
    {
        gs_typeName[gs_typeLen] = name;
        gs_typeOf[gs_typeLen] = type;
        gs_typeLen++;
    }
    return name;
}

static void GL_APIENTRY MockShaderSource(GLuint shader, GLsizei count,
                             const GLchar* const* strings, const GLint* lengths)
{
    (void)lengths;
    if (count == 1 && gs_sourceLen < GS_MAXOBJ)
    {
        gs_sourceOf[gs_sourceLen] = shader;
        snprintf(gs_source[gs_sourceLen], sizeof(gs_source[0]), "%s", strings[0]);
        gs_sourceLen++;
    }
}

static void GL_APIENTRY MockCompileShader(GLuint shader) { (void)shader; }

static void GL_APIENTRY MockGetShaderiv(GLuint shader, GLenum pname, GLint* out)
{
    if (pname != GL_COMPILE_STATUS)
    {
        *out = 0;
        return;
    }
    if (gs_compileFailsFor && (GLuint)gs_compileFailsFor == shader)
        *out = GL_FALSE;
    else if (gs_compileFailsForType
             && (GLenum)gs_compileFailsForType == gs_typeFor(shader))
        *out = GL_FALSE;
    else
        *out = GL_TRUE;
}

static void GL_APIENTRY MockGetShaderInfoLog(GLuint shader, GLsizei max, GLsizei* len,
                                 GLchar* log)
{
    const char* text = "0(3) : error C0000: syntax error";
    GLsizei     n = (GLsizei)strlen(text);

    (void)shader;
    if (n > max - 1)
        n = max - 1;
    memcpy(log, text, (size_t)n);
    log[n] = 0;
    if (len)
        *len = n;
}

static void GL_APIENTRY MockDeleteShader(GLuint shader)
{
    gs_forget(gs_shadersAlive, &gs_shadersAliveLen, shader);
}

static GLuint GL_APIENTRY MockCreateProgram(void)
{
    GLuint name;

    if (gs_createProgramFails)
        return 0;
    name = (GLuint)gs_nextName++;
    gs_remember(gs_programsAlive, &gs_programsAliveLen, name);
    return name;
}

static void GL_APIENTRY MockAttachShader(GLuint p, GLuint s) { (void)p; (void)s; gs_attachCount++; }
static void GL_APIENTRY MockDetachShader(GLuint p, GLuint s) { (void)p; (void)s; gs_detachCount++; }
static void GL_APIENTRY MockLinkProgram(GLuint p) { (void)p; gs_linkCount++; }

static void GL_APIENTRY MockGetProgramiv(GLuint p, GLenum pname, GLint* out)
{
    (void)p;
    *out = (pname == GL_LINK_STATUS && !gs_linkFails) ? GL_TRUE : GL_FALSE;
}

static void GL_APIENTRY MockGetProgramInfoLog(GLuint p, GLsizei max, GLsizei* len, GLchar* log)
{
    const char* text = "Link error: no fragment output";
    GLsizei     n = (GLsizei)strlen(text);

    (void)p;
    if (n > max - 1)
        n = max - 1;
    memcpy(log, text, (size_t)n);
    log[n] = 0;
    if (len)
        *len = n;
}

static void GL_APIENTRY MockDeleteProgram(GLuint p)
{
    gs_forget(gs_programsAlive, &gs_programsAliveLen, p);
}

static GLint GL_APIENTRY MockGetUniformLocation(GLuint p, const GLchar* name)
{
    (void)p;
    (void)name;
    gs_uniformCalls++;
    return gs_uniformLocation;
}

static void GS_AimThePointers(void)
{
    GLFN_glCreateShader = MockCreateShader;
    GLFN_glShaderSource = MockShaderSource;
    GLFN_glCompileShader = MockCompileShader;
    GLFN_glGetShaderiv = MockGetShaderiv;
    GLFN_glGetShaderInfoLog = MockGetShaderInfoLog;
    GLFN_glDeleteShader = MockDeleteShader;
    GLFN_glCreateProgram = MockCreateProgram;
    GLFN_glAttachShader = MockAttachShader;
    GLFN_glDetachShader = MockDetachShader;
    GLFN_glLinkProgram = MockLinkProgram;
    GLFN_glGetProgramiv = MockGetProgramiv;
    GLFN_glGetProgramInfoLog = MockGetProgramInfoLog;
    GLFN_glDeleteProgram = MockDeleteProgram;
    GLFN_glGetUniformLocation = MockGetUniformLocation;
}

/* ---------------------------------------------------------- the files */

static const char* GS_DIR = "tests/build/shadertest";

static void GS_Write(const char* name, const char* text)
{
    char  path[512];
    FILE* f;

    snprintf(path, sizeof(path), "%s/data/shaders/%s", GS_DIR, name);
    f = fopen(path, "wb");
    if (!f)
    {
        printf("  FAIL the test cannot write %s\n", path);
        fails++;
        return;
    }
    fwrite(text, 1, strlen(text), f);
    fclose(f);
}

static void GS_Remove(const char* name)
{
    char path[512];

    snprintf(path, sizeof(path), "%s/data/shaders/%s", GS_DIR, name);
    remove(path);
}

static void reset(void)
{
    gsLogLen = 0;
    gs_nextName = 100;
    gs_createShaderFailsFor = 0;
    gs_createProgramFails = 0;
    gs_compileFailsFor = 0;
    gs_compileFailsForType = 0;
    gs_linkFails = 0;
    gs_uniformLocation = 7;
    gs_uniformCalls = 0;
    gs_shadersAliveLen = 0;
    gs_programsAliveLen = 0;
    gs_sourceLen = 0;
    gs_typeLen = 0;
    gs_attachCount = 0;
    gs_detachCount = 0;
    gs_linkCount = 0;
    snprintf(dpt_basePath, sizeof(dpt_basePath), "%s/", GS_DIR);
}

static const char* gs_sourceGiven(int which)
{
    return which < gs_sourceLen ? gs_source[which] : "";
}

/* ---------------------------------------------------------- the cases */

static void caseLoads(void)
{
    tGlProgram p;

    printf("-- a pair that compiles and links\n");
    reset();
    GS_Write("ok.vert", "#version 450 core\nvoid main(){}\n");
    GS_Write("ok.frag", "#version 450 core\nout vec4 c;\nvoid main(){c=vec4(1);}\n");

    ok(GL_b_ProgramLoad(&p, "ok.vert", "ok.frag"), "it loads");
    ok(p.program != 0, "and has a program");
    okEq(gs_linkCount, 1, "linked once");
    okEq(gs_attachCount, 2, "with both stages attached");
    okEq(gs_detachCount, 2, "and both detached afterwards");
    okEq(gsLogLen, 0, "and said nothing");

    /* the stages are marked for deletion once linked: the program
       holds what it needs and two objects kept alive for nothing is
       what this avoids */
    okEq(gs_shadersAliveLen, 0, "neither stage is left alive");
    okEq(gs_programsAliveLen, 1, "and the program is");
    okEq(p.vertex, 0, "the handles are cleared");
    okEq(p.fragment, 0, "both of them");

    /* The bytes on disk are the bytes the driver was handed, compared
       exactly rather than searched. A prefix match would pass on a
       buffer the loader forgot to terminate, which is the one way this
       can go wrong without anything crashing. */
    okStr(gs_sourceGiven(0), "#version 450 core\nvoid main(){}\n",
          "the vertex source arrived, and nothing after it");
    okStr(gs_sourceGiven(1),
          "#version 450 core\nout vec4 c;\nvoid main(){c=vec4(1);}\n",
          "and the fragment source, which is a different file");

    /* and the stages were created as the right kinds */
    okEq((long)gs_typeOf[0], GL_VERTEX_SHADER, "the first is a vertex shader");
    okEq((long)gs_typeOf[1], GL_FRAGMENT_SHADER, "the second a fragment one");

    GL_ProgramFree(&p);
    okEq(gs_programsAliveLen, 0, "freeing gives the program back");
    okEq(p.program, 0, "and clears it");
    GL_ProgramFree(&p);
    okEq(gs_programsAliveLen, 0, "freeing twice gives nothing back twice");
    GL_ProgramFree(NULL);
    ok(1, "and freeing nothing is survivable");
}

static void caseMissing(void)
{
    tGlProgram p;

    printf("-- a file that is not there\n");
    reset();
    GS_Remove("gone.vert");
    GS_Write("ok.frag", "#version 450 core\nvoid main(){}\n");

    ok(!GL_b_ProgramLoad(&p, "gone.vert", "ok.frag"), "it declines");
    okEq(p.program, 0, "with no program");
    okEq(GS_i_Logged("no "), 1, "and says so once");
    okEq(gs_shadersAliveLen, 0, "and created nothing");

    /* the fragment missing, after the vertex compiled: the vertex has
       to be given back, which is the leak a real machine would not
       show for a long time */
    reset();
    GS_Write("ok.vert", "#version 450 core\nvoid main(){}\n");
    GS_Remove("gone.frag");
    ok(!GL_b_ProgramLoad(&p, "ok.vert", "gone.frag"), "it declines");
    okEq(gs_shadersAliveLen, 0, "and the stage that did compile was freed");
    okEq(gs_programsAliveLen, 0, "with no program made");
}

static void caseEmptyAndHuge(void)
{
    tGlProgram p;
    char*      big;

    printf("-- a file with nothing in it, and one with too much\n");
    reset();
    GS_Write("empty.vert", "");
    GS_Write("ok.frag", "#version 450 core\nvoid main(){}\n");
    ok(!GL_b_ProgramLoad(&p, "empty.vert", "ok.frag"), "an empty file declines");
    okEq(GS_i_Logged("empty"), 1, "and says which");
    okEq(gs_shadersAliveLen, 0, "nothing created");

    reset();
    big = (char*)malloc(GL_SHADERMAX + 64);
    memset(big, ' ', GL_SHADERMAX + 32);
    memcpy(big, "#version 450 core\n", 18);
    big[GL_SHADERMAX + 32] = 0;
    GS_Write("huge.vert", big);
    free(big);
    ok(!GL_b_ProgramLoad(&p, "huge.vert", "ok.frag"),
       "a file past the buffer declines rather than being truncated");
    okEq(GS_i_Logged("longer than"), 1, "and says so");
}

static void caseCompileFails(void)
{
    tGlProgram p;

    printf("-- a stage the driver will not compile\n");

    /* the vertex stage */
    reset();
    GS_Write("bad.vert", "#version 450 core\nthis is not glsl\n");
    GS_Write("ok.frag", "#version 450 core\nvoid main(){}\n");
    gs_compileFailsForType = GL_VERTEX_SHADER;
    ok(!GL_b_ProgramLoad(&p, "bad.vert", "ok.frag"), "it declines");
    okEq(gs_shadersAliveLen, 0, "and gives the shader object back");
    okEq(GS_i_Logged("will not compile"), 1, "saying which file");
    ok(GS_i_Logged("syntax error") == 1,
       "and printing the driver's own log, which is the only thing with a line number");

    /* the fragment stage, after the vertex succeeded */
    reset();
    GS_Write("ok.vert", "#version 450 core\nvoid main(){}\n");
    GS_Write("bad.frag", "#version 450 core\nnope\n");
    gs_compileFailsForType = GL_FRAGMENT_SHADER;
    ok(!GL_b_ProgramLoad(&p, "ok.vert", "bad.frag"), "it declines");
    okEq(gs_shadersAliveLen, 0, "and both stages are given back");
    okEq(gs_programsAliveLen, 0, "with no program made");
    okEq(gs_linkCount, 0, "and nothing linked");
}

static void caseLinkFails(void)
{
    tGlProgram p;

    printf("-- two stages that will not link together\n");
    reset();
    GS_Write("ok.vert", "#version 450 core\nvoid main(){}\n");
    GS_Write("ok.frag", "#version 450 core\nvoid main(){}\n");
    gs_linkFails = 1;

    ok(!GL_b_ProgramLoad(&p, "ok.vert", "ok.frag"), "it declines");
    okEq(gs_linkCount, 1, "having tried");
    okEq(GS_i_Logged("will not link"), 1, "and says so");
    ok(GS_i_Logged("no fragment output") == 1, "with the driver's log");
    okEq(gs_programsAliveLen, 0, "the program is given back");
    okEq(gs_shadersAliveLen, 0, "and so are both stages");
    okEq(p.program, 0, "and nothing is left in the struct");
}

static void caseNoObjects(void)
{
    tGlProgram p;

    printf("-- a driver that will not make objects\n");

    reset();
    GS_Write("ok.vert", "#version 450 core\nvoid main(){}\n");
    GS_Write("ok.frag", "#version 450 core\nvoid main(){}\n");
    gs_createShaderFailsFor = GL_VERTEX_SHADER;
    ok(!GL_b_ProgramLoad(&p, "ok.vert", "ok.frag"), "no shader object declines");
    okEq(GS_i_Logged("no shader object"), 1, "and says so");

    reset();
    gs_createProgramFails = 1;
    ok(!GL_b_ProgramLoad(&p, "ok.vert", "ok.frag"), "no program object declines");
    okEq(GS_i_Logged("no program object"), 1, "and says so");
    okEq(gs_shadersAliveLen, 0, "with both stages given back");
}

static void caseUniforms(void)
{
    tGlProgram p;

    printf("-- uniforms\n");
    reset();
    GS_Write("ok.vert", "#version 450 core\nvoid main(){}\n");
    GS_Write("ok.frag", "#version 450 core\nvoid main(){}\n");
    GL_b_ProgramLoad(&p, "ok.vert", "ok.frag");

    gs_uniformLocation = 3;
    okEq(GL_i_ProgramUniform(&p, "uThing"), 3, "a uniform that exists");
    okEq(gsLogLen, 0, "says nothing");

    gs_uniformLocation = -1;
    okEq(GL_i_ProgramUniform(&p, "uMissing"), -1, "one that does not is -1");
    okEq(GS_i_Logged("no uniform uMissing"), 1,
         "and is named, because it is usually a typo rather than a choice");

    /* The driver would answer these, so the location it answers with
       is set to something valid first: with it left at -1 the checks
       below pass whether or not the guard is there, which is how a
       first draft of this case missed the guard being removed. */
    gs_uniformLocation = 3;
    gs_uniformCalls = 0;
    okEq(GL_i_ProgramUniform(NULL, "uThing"), -1, "no program is -1");
    okEq(GL_i_ProgramUniform(&p, NULL), -1, "no name is -1");
    {
        tGlProgram empty;

        memset(&empty, 0, sizeof(empty));
        okEq(GL_i_ProgramUniform(&empty, "uThing"), -1, "an empty program is -1");
    }
    okEq(gs_uniformCalls, 0, "and none of the three reached the driver");
    GL_ProgramFree(&p);
}

static void caseNoName(void)
{
    tGlProgram p;

    printf("-- nothing to load\n");
    reset();
    ok(!GL_b_ProgramLoad(&p, NULL, "ok.frag"), "no vertex name declines");
    ok(!GL_b_ProgramLoad(&p, "ok.vert", NULL), "no fragment name declines");
    ok(!GL_b_ProgramLoad(NULL, "ok.vert", "ok.frag"), "nowhere to put it declines");

    /* and no executable to resolve against */
    reset();
    dpt_basePath[0] = 0;
    ok(!GL_b_ProgramLoad(&p, "ok.vert", "ok.frag"), "no path declines");
    okEq(GS_i_Logged("where"), 1, "and says why");
}

int main(void)
{
    printf("=== GL shader unit test ===\n");
    GS_AimThePointers();

    /* the directory the loader will read from */
    mkdir(GS_DIR, 0777);
    {
        char sub[512];

        snprintf(sub, sizeof(sub), "%s/data", GS_DIR);
        mkdir(sub, 0777);
        snprintf(sub, sizeof(sub), "%s/data/shaders", GS_DIR);
        mkdir(sub, 0777);
    }

    caseLoads();
    caseMissing();
    caseEmptyAndHuge();
    caseCompileFails();
    caseLinkFails();
    caseNoObjects();
    caseUniforms();
    caseNoName();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
