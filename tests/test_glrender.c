/* What the shipped shaders actually compute.
 *
 * Every other test of the OpenGL renderer aims its GL function
 * pointers at mocks. That is what makes them fast and what lets them
 * say precisely which call was made with which argument -- but a mock
 * cannot say what came out the other end. The shader files are loaded
 * and their uniform names checked against the renderer's, and then the
 * arithmetic inside them is nobody's business: three deliberate
 * breakages of geometry.frag -- replacing the modulate with an
 * assignment, inverting the alpha test, letting the doubling hit the
 * alpha -- went through that harness untouched.
 *
 * So this one asks a real driver. tests/eglheadless.c brings up an
 * OpenGL 4.5 core context with no window, the renderer's own pointers
 * are aimed at it, and the drawing goes through win/wingl.c's own
 * entry points -- the same ones the engine calls. The result is read
 * back a pixel at a time.
 *
 * That makes this the only test here that covers the renderer and the
 * shaders together, against a compiler that will refuse what is not
 * GLSL and a rasteriser that will put the triangle where the matrices
 * say. It is also the slowest and the one most likely to be
 * unavailable, which is why it is a test of its own rather than more
 * cases in tests/test_wingloverlay.c.
 *
 * Unavailable is allowed: a machine with no libEGL, no software
 * renderer or no 4.5 core profile is an ordinary machine, and this
 * says so and passes.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "eglheadless.h"
#include "../win/glcore.h"
#include "../win/glload.h"
#include "../win/glshader.h"
#include "../gdi.h"
#include "../textures.h"

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

/* A channel is compared with a little room: the rasteriser
   interpolates, the texture is filtered, and a byte that came back one
   off the arithmetic is the hardware agreeing rather than disagreeing.
   Anything this test turns on changes a channel by far more. */
static void okNear(int got, int want, const char* what)
{
    checks++;
    if (got < want - 2 || got > want + 2)
    {
        printf("  FAIL %s: got %d want %d\n", what, got, want);
        fails++;
    }
}

/* ------------------------------------------------------------ the log */

static char glrLog[64][512];
static int  glrLogLen;

void DBG_OutputDebugString(char* format, ...)
{
    va_list ap;

    if (glrLogLen >= 64)
        return;
    va_start(ap, format);
    vsnprintf(glrLog[glrLogLen], sizeof(glrLog[0]), format, ap);
    va_end(ap);
    glrLogLen++;
}

/* --------------------------------------------- Windows, stood in for

   The renderer refers to these; nothing here calls the parts that
   would use them, because the context comes from EGL rather than from
   a window. */

static char glr_exePath[512];

DWORD GetModuleFileNameA(HMODULE mod, LPSTR out, DWORD size)
{
    (void)mod;
    strncpy(out, glr_exePath, size - 1);
    out[size - 1] = 0;
    return (DWORD)strlen(out);
}

#include "winstubs.h"

/* The engine's globals the renderer reads. Values as gdi.c has them;
   gdi.c itself would bring the whole engine. */
int   GDI_gi_Widescreen = 1;
float GDI_gf_RefYoverX = 0.75f;
float GDI_gaf_ScreenRatio[4];
float tweak_mipmaplodbias = -0.60000002f;
tEngTexListStruct TEX_gst_GlobalList;

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

/* The module under test, included because its state is static to it
   and because this has to drive the same entry points the engine
   does. */
#include "../win/wingl.c"

/* ------------------------------------------- the test's own GL calls

   glReadPixels and the framebuffer this draws into are the test's
   tools, not the renderer's, so they are resolved here rather than
   added to glcore.h's list -- that list is what the renderer uses,
   and a name in it that nothing in the renderer calls makes the list
   a lie. */

/* Deliberately not square, and not a power of two in either axis. A
   square surface cannot see the overlay shader's x and y taking each
   other's viewport size, which is a swap of two lines that scores
   perfectly on one. */
#define GLR_W 80
#define GLR_H 48

/* The read-back format. GL_RGBA8 is how a texture is stored and is in
   glcore.h because the renderer asks for it; GL_RGBA is how pixels are
   handed over, which only this test does. */
#define GL_RGBA 0x1908

static void (*glr_ReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum,
                              void*);

static GLuint glr_fbo;
static GLuint glr_colour;
static GLuint glr_depth;
static unsigned char glr_pixels[GLR_W * GLR_H * 4];

/* The pixel at x, y measured from the top left, which is how every
   other part of this engine measures. OpenGL reads from the bottom,
   so the row is turned over here once rather than in every case. */
static const unsigned char* glr_pc_At(int x, int y)
{
    static const unsigned char black[4] = { 0, 0, 0, 0 };

    if (x < 0 || y < 0 || x >= GLR_W || y >= GLR_H)
        return black;
    return &glr_pixels[((GLR_H - 1 - y) * GLR_W + x) * 4];
}

static int glr_R(int x, int y) { return glr_pc_At(x, y)[0]; }
static int glr_G(int x, int y) { return glr_pc_At(x, y)[1]; }
static int glr_B(int x, int y) { return glr_pc_At(x, y)[2]; }
static int glr_A(int x, int y) { return glr_pc_At(x, y)[3]; }

static void glr_Read(void)
{
    memset(glr_pixels, 0, sizeof(glr_pixels));
    glr_ReadPixels(0, 0, GLR_W, GLR_H, GL_RGBA, GL_UNSIGNED_BYTE, glr_pixels);
}

/* A surface to draw into, cleared to something no shader here can
   produce, so a pixel nothing drew is told apart from a pixel drawn
   black. */
#define GLR_BG 64

static int glr_b_MakeSurface(void)
{
    GLFN_glGenTextures(1, &glr_colour);
    GLFN_glBindTexture(GL_TEXTURE_2D, glr_colour);
    GLFN_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GLR_W, GLR_H, 0,
                      GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    GLFN_glGenRenderbuffers(1, &glr_depth);
    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, glr_depth);
    GLFN_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                               GLR_W, GLR_H);
    GLFN_glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLFN_glGenFramebuffers(1, &glr_fbo);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, glr_colour, 0);
    GLFN_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_RENDERBUFFER, glr_depth);
    return GLFN_glCheckFramebufferStatus(GL_FRAMEBUFFER)
           == GL_FRAMEBUFFER_COMPLETE;
}

static void glr_Begin(void)
{
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GLFN_glClearColor((GLclampf)GLR_BG / 255.0f, (GLclampf)GLR_BG / 255.0f,
                      (GLclampf)GLR_BG / 255.0f, 1.0f);
    GLFN_glClearDepth(1.0);
    GLFN_glDepthMask(GL_TRUE);
    GLFN_glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    GLFN_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLFN_glDisable(GL_BLEND);
    GLFN_glDisable(GL_DEPTH_TEST);
    GLFN_glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/* --------------------------------------------------------- the rig */

static tGdi_DisplayData  glr_dd;
static tBigBufferStruct  glr_bb;

static void glr_Reset(void)
{
    GL_l_ForgetPass();
    GL_l_ForgetGeometry();
    GL_l_ForgetBlur();
    GL_ReleaseTextures(NULL);
    GL_l_ForgetTargets();

    /* The after-effects state the blur's decision is read from. It is
       the engine's, set by the AI scripts, so a case that wants a
       particular blur sets it and every other case starts with none. */
    PC_AE_MASTER = 0;
    depthBlurNear1 = 0.0f;
    depthBlurType0 = 0.0f;
    depthBlurType1 = 0.0f;

    memset(&glr_dd, 0, sizeof(glr_dd));
    memset(&glr_bb, 0, sizeof(glr_bb));
    glr_dd.bigBuffer = &glr_bb;
    gpst_DX_stDD = &glr_dd;
    memset(&gl_material, 0, sizeof(gl_material));
    gl_colorMaskAlpha = 0;
    gl_mipmapping = 1;
    GDI_gi_Diffuse2x = 0;

    /* The renderer is told it has a context. It has one -- EGL's --
       and the only thing it would use the window for is making one,
       which this does not ask it to do. */
    memset(&gl_state, 0, sizeof(gl_state));
    gl_state.context = (HGLRC)(size_t)0xE61;
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
    gl_state.zEnable = 1;
    gl_state.zWrite = 1;

    glrLogLen = 0;
    glr_Begin();
}

/* ------------------------------------------- geometry for the cases

   A square covering the middle of the surface, in world coordinates
   that the identity matrices below leave in clip space -- so the
   square lands where its numbers say and a matrix uploaded
   transposed, or a y left unflipped, moves it. */

static Vec3                       glr_pos[4];
static tGeoUVElem                 glr_uv[4];
static tGeoTriangleChunkElem      glr_tris[2];
static tGeoTriangleChunks         glr_chunk;

static void glr_BuildQuad(void)
{
    /* -0.5 .. 0.5 in x and y, which with an identity projection is
       the middle half of the surface in each axis. */
    glr_pos[0].x = -0.5f; glr_pos[0].y = -0.5f; glr_pos[0].z = 0.0f;
    glr_pos[1].x =  0.5f; glr_pos[1].y = -0.5f; glr_pos[1].z = 0.0f;
    glr_pos[2].x =  0.5f; glr_pos[2].y =  0.5f; glr_pos[2].z = 0.0f;
    glr_pos[3].x = -0.5f; glr_pos[3].y =  0.5f; glr_pos[3].z = 0.0f;

    /* The whole of the texture, with v matching the y above so that
       the picture is not turned over relative to the square. */
    glr_uv[0].u = 0.0f; glr_uv[0].v = 1.0f;
    glr_uv[1].u = 1.0f; glr_uv[1].v = 1.0f;
    glr_uv[2].u = 1.0f; glr_uv[2].v = 0.0f;
    glr_uv[3].u = 0.0f; glr_uv[3].v = 0.0f;

    memset(glr_tris, 0, sizeof(glr_tris));
    glr_tris[0].v1 = 0; glr_tris[0].v2 = 1; glr_tris[0].v3 = 2;
    glr_tris[0].uv1 = 0; glr_tris[0].uv2 = 1; glr_tris[0].uv3 = 2;
    glr_tris[1].v1 = 0; glr_tris[1].v2 = 2; glr_tris[1].v3 = 3;
    glr_tris[1].uv1 = 0; glr_tris[1].uv2 = 2; glr_tris[1].uv3 = 3;

    memset(&glr_chunk, 0, sizeof(glr_chunk));
    glr_chunk.triangleNum = 2;
    glr_chunk.triangleChunkElems = glr_tris;
}

/* Identity view and projection, so world coordinates are clip
   coordinates and a square at -0.5..0.5 is the middle half. */
static void glr_IdentityMatrices(void)
{
    Mtx4x4 id;

    MATH_SetIdentityMatrix(&id);
    GL_SetViewMatrix(&id);
    MATH_SetIdentityMatrix(&gl_projMtx);
    GL_l_CombineMatrices();
}

/* A solid colour throughout, as the engine asks for when a chunk has
   no colour array.
 *
 * The engine's own word is alpha, blue, green, red -- the red in the
 * lowest byte. geovertex.c's packing is what says so: it moves the
 * lowest byte to bits 16..23 and that byte back down, leaving a word
 * the other renderer declares to Direct3D as D3DCOLOR, which is
 * A8R8G8B8. So the swap is a conversion out of the engine's order and
 * into the device's, and the fragment shader's own bgra swizzle is the
 * second half of the same journey: those bytes read back as four
 * normalised bytes arrive blue, green, red, alpha.
 *
 * Writing the four channels out separately rather than as one hex
 * literal, because a literal in this order reads like a mistake. The
 * first draft of these cases passed 0xAARRGGBB words and every
 * expectation for red and blue came out swapped -- which is what this
 * test is for. */
static void glr_SolidColour(int r, int g, int b, int a)
{
    glr_bb.vertexColorArray = NULL;
    glr_bb.vertexColorArraySecond = NULL;
    glr_bb.colorMask = 0;
    glr_bb.solidVertexColor = (int)(((unsigned int)a << 24)
                                    | ((unsigned int)b << 16)
                                    | ((unsigned int)g << 8)
                                    | (unsigned int)r);
}

/* A one-pixel texture of a known colour, in the engine's own byte
   order, loaded the way the engine loads one. */
static unsigned char       glr_texel[4];
static tEngTexListElemStruct glr_texElems[4];
static tEngTexIntermediateStruct glr_inter;

/* Four distinct corners, so a u and v swapped, or a corner read from
   the wrong place, moves a colour this test is looking at. A single
   texel cannot see any of that -- and did not, until it was this.

   The rows are given bottom-up relative to the v the quad uses: v = 0
   is the quad's top, and a texture's first row in memory is the one
   OpenGL calls v = 0, so this array's first two pixels are the top
   pair.

   Each is an A8R8G8B8 word, as the engine hands its pixels over. */
static unsigned char glr_quadTex[2 * 2 * 4];

static void glr_FourCorners(void)
{
    /* top left red, top right green, bottom left blue, bottom right
       white -- four values no pair of which is confusable */
    static const int px[4][4] = {   /* r, g, b, a */
        { 0xFF, 0x00, 0x00, 0xFF },
        { 0x00, 0xFF, 0x00, 0xFF },
        { 0x00, 0x00, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0xFF, 0xFF },
    };
    int i;

    for (i = 0; i < 4; i++)
    {
        glr_quadTex[i * 4 + 0] = (unsigned char)px[i][2];   /* blue */
        glr_quadTex[i * 4 + 1] = (unsigned char)px[i][1];   /* green */
        glr_quadTex[i * 4 + 2] = (unsigned char)px[i][0];   /* red */
        glr_quadTex[i * 4 + 3] = (unsigned char)px[i][3];   /* alpha */
    }

    memset(glr_texElems, 0, sizeof(glr_texElems));
    /* the engine stores these the other way round; both are 2 here,
       which the one-texel fixture below cannot say anything about
       either -- that is what tests/test_wingloverlay.c checks */
    glr_texElems[1].normalData.width = 2;
    glr_texElems[1].normalData.height = 2;
    TEX_gst_GlobalList.texElems = glr_texElems;
    TEX_gst_GlobalList.maxTexNum = 4;

    memset(&glr_inter, 0, sizeof(glr_inter));
    glr_inter.colorDepth2 = 32;
    glr_inter.imgDataPtr = (char*)glr_quadTex;
    glr_inter.tp.flagsUnkAndProceduralUnkb1 = 0x40;    /* no mip chain */

    GL_TextureStore(&glr_dd, 3);
    GL_PaletteLoad(&glr_dd, &glr_texElems[1], &glr_inter, 1);
}

/* A texture is not a vertex colour: the engine hands its pixels over
   as A8R8G8B8 words, which on this machine is blue, green, red, alpha
   in memory, and the renderer tells OpenGL exactly that. No swap
   anywhere. The asymmetry with glr_SolidColour above is real and is
   the reason both are spelled out channel by channel here. */
static void glr_OneTexel(int r, int g, int b, int a)
{
    glr_texel[0] = (unsigned char)b;
    glr_texel[1] = (unsigned char)g;
    glr_texel[2] = (unsigned char)r;
    glr_texel[3] = (unsigned char)a;

    memset(glr_texElems, 0, sizeof(glr_texElems));
    glr_texElems[1].normalData.width = 1;
    glr_texElems[1].normalData.height = 1;
    TEX_gst_GlobalList.texElems = glr_texElems;
    TEX_gst_GlobalList.maxTexNum = 4;

    memset(&glr_inter, 0, sizeof(glr_inter));
    glr_inter.colorDepth2 = 32;
    glr_inter.imgDataPtr = (char*)glr_texel;
    /* no mip chain, so the sampler the renderer picks is a simple one */
    glr_inter.tp.flagsUnkAndProceduralUnkb1 = 0x40;

    GL_TextureStore(&glr_dd, 3);
    GL_PaletteLoad(&glr_dd, &glr_texElems[1], &glr_inter, 1);
}

/* ------------------------------------------------------- the cases */

static void caseContext(void)
{
    printf("-- the driver answering\n");
    printf("   %s\n   %s\n   GLSL %s\n", EGLH_version, EGLH_renderer,
           EGLH_glsl);
    ok(GL_b_VersionAtLeast(EGLH_version, 4, 5),
       "the context is at least 4.5, which is what the renderer asks for");
    ok(glr_ReadPixels != NULL, "and the test can read its pixels back");
}

static void caseGeometryModulates(void)
{
    printf("-- the world's shader modulates the texture by the colour\n");
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();

    /* Half grey texture, half grey colour: modulated, that is a
       quarter. An assignment instead of a modulate gives a half, and
       a modulate the other way round gives the same quarter -- which
       is why the two operands are different below. */
    /* A half-grey texture, and a colour whose three channels are all
       different so that a pair swapped anywhere shows. */
    glr_OneTexel(0x80, 0x80, 0x80, 0x80);
    glr_SolidColour(0xFF, 0x40, 0x80, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* Each channel is the texture's times the colour's: 0x80 by 0xFF
       is 0x80, by 0x40 is 0x20, by 0x80 is 0x40. An assignment rather
       than a modulate gives 0x80 in all three. */
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x80,
           "red is the texture times the colour");
    okNear(glr_G(GLR_W / 2, GLR_H / 2), 0x20,
           "green likewise, and not red's answer");
    okNear(glr_B(GLR_W / 2, GLR_H / 2), 0x40,
           "blue likewise, and not green's");
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 0x80,
           "and the alpha is modulated too");

    /* The square is where its coordinates say, which is the matrices
       and the viewport agreeing. */
    okNear(glr_R(2, 2), GLR_BG, "outside the square nothing was drawn");
    okNear(glr_R(GLR_W / 2, 2), GLR_BG, "above it either");
}

static void caseGeometryUntextured(void)
{
    printf("-- a material with no texture is the vertex colour alone\n");
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_OneTexel(0, 0, 0, 0xFF);     /* a black texture, to be ignored */
    glr_SolidColour(0x80, 0x40, 0xC0, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    GL_TextureInit(-1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* Not multiplied by the black texture, which is what it would be
       if the shader sampled whatever was bound regardless. */
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x80, "red is the colour's own");
    okNear(glr_G(GLR_W / 2, GLR_H / 2), 0x40, "green too");
    okNear(glr_B(GLR_W / 2, GLR_H / 2), 0xC0, "and blue");
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 0xFF, "with the alpha untouched by the texture");
}

static void caseGeometrySwizzle(void)
{
    printf("-- the colour arrives in the right order\n");
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_SolidColour(0xFF, 0x40, 0x00, 0xFF);   /* red, some green, no blue */
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    GL_TextureInit(-1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* A packed A8R8G8B8 word read as four normalised bytes arrives
       blue, green, red, alpha, and the shader puts them back. Without
       that swizzle this comes out blue rather than red. */
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF, "the red channel has the red");
    okNear(glr_G(GLR_W / 2, GLR_H / 2), 0x40, "the green has the green");
    okNear(glr_B(GLR_W / 2, GLR_H / 2), 0x00, "and the blue has none");
}

static void caseGeometryDiffuse2x(void)
{
    printf("-- the doubled modulate, colour only\n");
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_OneTexel(0x80, 0x80, 0x80, 0x80);
    glr_SolidColour(0x40, 0x40, 0x40, 0x80);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    GL_TextureInit(1, 0, 0);

    /* 0x80 by 0x40 is 0x20 in colour, 0x80 by 0x80 is 0x40 in alpha. */
    GDI_gi_Diffuse2x = 0;
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x20, "undoubled, a quarter times a half");
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 0x40, "and the alpha a half times a half");

    glr_Begin();
    GDI_gi_Diffuse2x = 1;
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x40, "doubled, the colour is twice that");
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 0x40,
           "and the alpha is left alone -- the other renderer's doubled"
           " modulate is a colour operation only");
}

static void caseGeometryAlphaTest(void)
{
    printf("-- the alpha test, both senses\n");
    glr_BuildQuad();

    /* An alpha of 0x80 against a reference of 0x40: above it. The
       reference the engine passes is six bits at 24..29, so 0x40 is
       (0x10 << 24) with the material's own test bit set. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_OneTexel(0xFF, 0xFF, 0xFF, 0xFF);
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0x80);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    GL_TextureInit(1, 0x10 | (0x10u << 24), 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF,
           "keeping what is above the reference draws an alpha of 0x80"
           " against 0x40");

    /* The same pixel with the sense reversed must be gone. */
    glr_Begin();
    GL_TextureInit(1, 0x10 | 0x80 | (0x10u << 24), 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), GLR_BG,
           "and reversed, the same pixel is discarded");

    /* And below the reference, the other way round. */
    glr_Begin();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0x20);
    GL_TextureInit(1, 0x10 | (0x10u << 24), 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), GLR_BG, "an alpha of 0x20 against 0x40 is dropped");

    glr_Begin();
    GL_TextureInit(1, 0x10 | 0x80 | (0x10u << 24), 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF, "and reversed, it is kept");

    /* With no test at all, both are drawn. */
    glr_Begin();
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF, "and with no test, everything is drawn");
}

static void caseGeometryTexCoords(void)
{
    printf("-- each corner samples the corner of the texture it names\n");
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_FourCorners();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);    /* white, so the texture shows */
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* The quad covers the middle half of the surface. A quarter of
       the way into it in each axis is inside one of the four texels,
       clear of the boundary, so nothing here rests on how the sampler
       rounds at a seam. */
    {
        int left   = GLR_W / 4 + GLR_W / 16;
        int right  = GLR_W / 2 + GLR_W / 8 + GLR_W / 16;
        int top    = GLR_H / 4 + GLR_H / 16;
        int bottom = GLR_H / 2 + GLR_H / 8 + GLR_H / 16;

        okNear(glr_R(left, top), 0xFF, "the top left corner is the red texel");
        okNear(glr_G(left, top), 0x00, "and only red");

        okNear(glr_G(right, top), 0xFF, "the top right is the green one");
        okNear(glr_R(right, top), 0x00, "and only green");

        okNear(glr_B(left, bottom), 0xFF, "the bottom left is the blue one");
        okNear(glr_R(left, bottom), 0x00, "and only blue");

        okNear(glr_R(right, bottom), 0xFF, "and the bottom right is white");
        okNear(glr_G(right, bottom), 0xFF, "in every channel");
        okNear(glr_B(right, bottom), 0xFF, "of the three");
    }
}

static void caseGeometryMatrix(void)
{
    Mtx4x4 id;
    int    shift;

    printf("-- the matrix is applied the way the engine stores it\n");
    glr_Reset();
    glr_BuildQuad();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    /* An identity view, and a projection that does nothing but move
       the picture sideways. The engine's matrices are row-vector and
       row-major, so the translation sits in the fourth row -- which
       is exactly the term a transpose moves into the fourth column,
       where it would scale instead of shifting. Two identities cannot
       see that, and that is what the other cases here use. */
    MATH_SetIdentityMatrix(&id);
    GL_SetViewMatrix(&id);
    MATH_SetIdentityMatrix(&gl_projMtx);
    gl_projMtx.Pos.x = 0.5f;
    GL_l_CombineMatrices();

    GL_TextureInit(-1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* Half of clip space is a quarter of the surface. So the square's
       left edge, which sat a quarter of the way across, is now at the
       middle, and its right edge has gone off the surface. */
    shift = GLR_W / 4;
    okNear(glr_R(GLR_W / 4 - 4, GLR_H / 2), GLR_BG,
           "where the square was is now clear");
    okNear(glr_R(GLR_W / 4 + shift + 4, GLR_H / 2), 0xFF,
           "and it has moved by a quarter of the surface");
    okNear(glr_R(GLR_W / 2, 2), GLR_BG, "without moving in y");
}

static void caseOverlayQuads(void)
{
    tGdi_Vertex v[6];
    int         i;

    printf("-- the overlay's quads land in the pixels they name\n");
    glr_Reset();

    /* A rectangle from (8, 4) to (24, 20) in the engine's own
       coordinates: pixels, origin at the top left. Two triangles,
       because that is what the renderer is handed. */
    memset(v, 0, sizeof(v));
    /* Red and blue deliberately different: green is the middle
       channel and survives an r/b swap untouched, so a green
       rectangle cannot see the overlay shader's own colour swizzle go
       missing. The overlay's word is a plain A8R8G8B8 -- unlike the
       world's vertex colour, which geovertex.c converts into one. */
    for (i = 0; i < 6; i++)
        v[i].color = (int)0xFFFF2040u;   /* opaque, red, a little green */
    v[0].pos.x = 8;  v[0].pos.y = 4;
    v[1].pos.x = 24; v[1].pos.y = 4;
    v[2].pos.x = 24; v[2].pos.y = 20;
    v[3].pos.x = 8;  v[3].pos.y = 4;
    v[4].pos.x = 24; v[4].pos.y = 20;
    v[5].pos.x = 8;  v[5].pos.y = 20;

    GL_OverlayBegin();
    GL_OverlayDrawQuads(v, 2);
    GL_OverlayEnd();
    glr_Read();

    /* Inside. The y is the test's own top-left measure, so this also
       says the shader's pixel-to-clip mapping turns the y over. */
    okNear(glr_R(16, 12), 0xFF, "the middle of the rectangle is drawn");
    okNear(glr_G(16, 12), 0x20, "in the colour asked for");
    okNear(glr_B(16, 12), 0x40,
           "with red and blue the way round the engine packs them");

    /* Outside, on all four sides, one pixel clear of the edge so that
       nothing rests on how the rasteriser rounds. */
    okNear(glr_R(16, 2), GLR_BG, "above it, nothing");
    okNear(glr_R(16, 22), GLR_BG, "below it, nothing");
    okNear(glr_R(6, 12), GLR_BG, "left of it, nothing");
    okNear(glr_R(26, 12), GLR_BG, "right of it, nothing");
}

static void caseOverlayText(void)
{
    tGdi_TexturedVertex v[6];
    unsigned char       coverage[4];
    void*               atlas;
    int                 i;

    printf("-- a glyph's coverage becomes its alpha\n");
    glr_Reset();

    /* A two-by-two atlas: opaque on the left, clear on the right, so
       a u that sampled the wrong way round or a coverage read as a
       colour both show. */
    coverage[0] = 0xFF; coverage[1] = 0x00;
    coverage[2] = 0xFF; coverage[3] = 0x00;
    atlas = GL_p_CreateAlphaTexture(2, 2, coverage);
    ok(atlas != NULL, "the atlas is made");

    memset(v, 0, sizeof(v));
    for (i = 0; i < 6; i++)
        v[i].color = (int)0xFFFF0000u;   /* opaque red */
    /* the left half of the atlas over pixels 8..24, the right half
       over 24..40 */
    v[0].pos.x = 8;  v[0].pos.y = 8;  v[0].u = 0.0f;  v[0].v = 0.0f;
    v[1].pos.x = 40; v[1].pos.y = 8;  v[1].u = 1.0f;  v[1].v = 0.0f;
    v[2].pos.x = 40; v[2].pos.y = 24; v[2].u = 1.0f;  v[2].v = 1.0f;
    v[3].pos.x = 8;  v[3].pos.y = 8;  v[3].u = 0.0f;  v[3].v = 0.0f;
    v[4].pos.x = 40; v[4].pos.y = 24; v[4].u = 1.0f;  v[4].v = 1.0f;
    v[5].pos.x = 8;  v[5].pos.y = 24; v[5].u = 0.0f;  v[5].v = 1.0f;

    GL_OverlayBegin();
    GL_OverlayDrawTexturedQuads(atlas, v, 2);
    GL_OverlayEnd();
    glr_Read();

    /* The pass blends, so a covered pixel is the glyph's colour over
       the background and an uncovered one is the background. */
    okNear(glr_R(12, 16), 0xFF, "where the atlas is opaque, the glyph is drawn");
    okNear(glr_R(36, 16), GLR_BG,
           "and where it is clear, the background shows through");
    okNear(glr_G(12, 16), 0x00, "in the glyph's own colour");

    GL_DestroyAlphaTexture(atlas);
}

static void caseSprites(void)
{
    tGeoSpriteElem elems[2];
    Vec3           pos[2];
    tGeoSprites    sprites;
    Mtx4x4         stack;

    printf("-- a sprite lands where the matrix points it\n");
    glr_Reset();
    glr_IdentityMatrices();
    glr_SolidColour(0xFF, 0x20, 0x40, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    /* One sprite at the origin. The matrix stack's first two columns
       are the axes the corners are built along, so an identity gives a
       square in the x/y plane.

       The element is a union: the low half is the index into the
       positions and the whole of it, read as a float, is the sprite's
       size. So the two have to be chosen together -- index nought and
       a high half of 0x3F00 is the float 0.5, which with an identity
       matrix puts the corners at plus and minus a half. Leaving the
       high half at nought makes the float nought too, and the quad
       collapses to a point; which is what the first draft of this
       case did, and it drew nothing. */
    elems[0].idx = 0;
    elems[0].dummy = 0x3F00;
    pos[0].x = 0.0f;
    pos[0].y = 0.0f;
    pos[0].z = 0.0f;
    memset(&sprites, 0, sizeof(sprites));
    sprites.num = 1;
    sprites.spriteElems = elems;
    sprites.globalSize = 1.0f;
    sprites.globalRatio = 1.0f;

    MATH_SetIdentityMatrix(&stack);
    glr_dd.matrixStack.actMatrixElem = &stack;

    GL_TextureInit(-1, 0, 0);
    GL_DrawSprites(&sprites, pos, 1);
    glr_Read();

    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF,
           "the sprite is drawn in the middle");
    okNear(glr_R(2, 2), GLR_BG, "and the corners are where the size says");
    okNear(glr_G(GLR_W / 2, GLR_H / 2), 0x20,
           "in the colour the engine asked for");
    okNear(glr_B(GLR_W / 2, GLR_H / 2), 0x40,
           "with red and blue the way round the engine packs them");

    /* The batch's own size scales every corner, so taking it away
       takes the sprite away. */
    glr_Begin();
    sprites.globalSize = 0.0f;
    GL_DrawSprites(&sprites, pos, 1);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), GLR_BG,
           "and a batch with no size draws nothing at all");
}

static void caseVideo(void)
{
    void*          bits = NULL;
    int            pitch = 0;
    int            height = 0;
    unsigned char* px;

    printf("-- a movie frame fitted to the screen\n");
    glr_Reset();

    /* A four-by-two picture, written the way the engine's decoder
       writes one: thirty-two-bit words, blue first. Each pixel a
       different colour, so a frame turned over or read with the
       channels swapped is visible. */
    ok(GL_b_VideoDisplayCreate(4, 2) == 1, "a display is made");
    ok(GL_b_VideoFrameLock(&bits, &pitch, &height) == 1, "and locks");
    okEq(pitch, 16, "four pixels of four bytes");
    okEq(height, 2, "two rows");

    px = (unsigned char*)bits;
    if (px)
    {
        int i;

        for (i = 0; i < 8; i++)
        {
            /* top row red, bottom row blue, so which way up it lands
               is the thing this says */
            int top = (i < 4);

            px[i * 4 + 0] = (unsigned char)(top ? 0x00 : 0xFF);  /* blue */
            px[i * 4 + 1] = 0x00;                                /* green */
            px[i * 4 + 2] = (unsigned char)(top ? 0xFF : 0x00);  /* red */
            /* Nothing in the fourth byte. The engine's decoder writes
               into a surface whose fourth channel is padding, so a
               frame drawn with the texture's own alpha comes out
               invisible -- which is what the shader forcing it to one
               is for. */
            px[i * 4 + 3] = 0x00;
        }
    }
    GL_VideoFrameUnlock();

    /* A pixel for a pixel, centred, so the picture is four by two in
       the middle of the surface. */
    GL_VideoFramePresent(4, 2, GDI_VIDEOFIT_ACTUALSIZE);
    glr_Read();

    {
        int left = (GLR_W - 4) / 2;
        int top = (GLR_H - 2) / 2;

        okNear(glr_R(left + 1, top), 0xFF,
               "the top row of the frame is at the top of the picture");
        okNear(glr_B(left + 1, top), 0x00, "and is red, not blue");
        okNear(glr_B(left + 1, top + 1), 0xFF,
               "the bottom row is below it");
        okNear(glr_R(left + 1, top + 1), 0x00, "and is blue");
        okNear(glr_A(left + 1, top), 0xFF,
               "drawn opaque whatever the frame's fourth channel says");

        /* And black everywhere else, because the present clears. */
        okNear(glr_R(2, 2), 0, "the rest of the screen is black");
        okNear(glr_R(left + 1, top - 3), 0,
               "including just above the frame");
    }

    /* Scaled to the width: four pixels across becomes the whole
       surface, and the two rows become half its height centred. */
    glr_Begin();
    GL_VideoFramePresent(4, 2, GDI_VIDEOFIT_SMOOTH);
    glr_Read();
    /* Four pixels across becomes the whole surface, so the two rows
       become twenty pixels each, centred: the frame spans y 4 to 44.
       Sampled at ten, which is inside the top row and clear of the
       seam at twenty-four -- the smoothing blends across that seam,
       and a sample near it reads a red-blue mixture rather than
       either. Which is what the first draft of this did. */
    okNear(glr_R(2, 10), 0xFF,
           "scaled up, the frame reaches the left edge");
    okNear(glr_R(GLR_W - 3, 10), 0xFF, "and the right");
    okNear(glr_B(GLR_W / 2, 38), 0xFF, "with the bottom row below the seam");
    okNear(glr_R(GLR_W / 2, 1), 0,
           "and black bars above, because the shapes do not match");

    GL_VideoDisplayClose();
}

static void caseShadowTarget(void)
{
    printf("-- a shadow drawn into a target and sampled back\n");
    glr_Reset();
    glr_BuildQuad();

    /* Draw a white square into shadow target 2, through the same
       sequence the engine's shadow pass makes. */
    {
        Mtx4x4 id;

        MATH_SetIdentityMatrix(&id);
        MATH_SetIdentityMatrix(&gl_projMtx);
        glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
        glr_dd.drawMaskAllowedStuffCurrent = 0x10;

        GL_SetViewMatrixSDW(&id, NULL);
        GL_TextureInit(-1, 0, 0);
        GL_SetRenderTargetAndBackupPreviousSDW(2, 1);
        GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
        GL_SetRenderTargetAndBackupPreviousSDW(-1, 0);
    }

    /* The window's framebuffer has to be bound again, and the test's
       own surface is what "the window" is here -- so put it back and
       check the target did not draw into it. */
    glr_Begin();
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), GLR_BG,
           "the shadow went into the target and not into the frame");

    /* And now sample it: the square covered the middle of the target,
       so a quad reading the middle of that texture gets white. */
    glr_IdentityMatrices();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    GL_TextureInit((int)(0x80000000u | 2u), 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF,
           "and sampling the target gives back what was drawn into it");

    /* A target nothing drew into is white, which is what makes an
       object with no shadow on it unshadowed rather than black. */
    glr_Begin();
    GL_TextureInit((int)(0x80000000u | 5u), 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF, "and an untouched target samples white");
}

/* ===================================================== the depth blur
 *
 * The pass draws the world a second time through a 256 by 1 ramp of
 * blur-amount-per-depth and writes the frame's alpha and nothing else.
 * Alpha runs the other way from what the name suggests: 255 is no blur
 * and 0 is full.
 *
 * What is checked here is the arithmetic only a real driver can run --
 * the coordinate the shader makes from the camera-space position, in
 * both of its forms, and the ramp entry it lands in. A mocked harness
 * sees the uniforms being set and nothing about what they produce.
 */

/* The quad, flat at one depth, so the whole of it samples the ramp at
   one coordinate and the alpha read back is that one entry. The
   identity projection clips to -1..1 in z, so the depths below sit
   inside that and the far plane is small to suit. */
static void glr_QuadAtDepth(float z)
{
    int i;

    glr_BuildQuad();
    for (i = 0; i < 4; i++)
        glr_pos[i].z = z;
}

/* What the ramp holds at a coordinate, read out of the table rather
   than worked out again -- so a wrong table and a wrong coordinate are
   separate failures. The sampler is linear, so a coordinate between
   two entries is between their values, with the texel centres at
   (i + 0.5) / 256. */
static double glr_RampAt(const tDpb_State* st, double u)
{
    int    table[256];
    double at;
    int    i0;
    int    i1;
    double f;

    DPB_BuildRamp(st, table);
    if (u < 0.0) u = 0.0;
    if (u > 1.0) u = 1.0;

    at = u * 256.0 - 0.5;
    if (at < 0.0) at = 0.0;
    if (at > 255.0) at = 255.0;
    i0 = (int)at;
    i1 = i0 + 1 > 255 ? 255 : i0 + 1;
    f = at - (double)i0;

    return (1.0 - f) * (double)(((unsigned int)table[i0] >> 24) & 0xFF)
           + f * (double)(((unsigned int)table[i1] >> 24) & 0xFF);
}

/* An identity view and a projection that is not identity, so the two
   matrices the pass is given are different ones.
 *
 * This matters more than it looks. The coordinate comes from the
 * camera-space position, which is the view matrix's output, and the
 * obvious wrong uniform to hand the shader is the combined one -- so
 * with both matrices identity the pass would read clip space, compute
 * the same answer, and the case would pass. It did: handing the shader
 * the combined matrix was the one mutation of this pass that no case
 * noticed.
 *
 * Only z is scaled, so the square still lands where the other cases
 * put it and the depth below is still the camera-space depth. */
static void glr_BlurMatrices(void)
{
    Mtx4x4 id;

    MATH_SetIdentityMatrix(&id);
    GL_SetViewMatrix(&id);
    MATH_SetIdentityMatrix(&gl_projMtx);
    gl_projMtx.v3.z = 0.5f;
    GL_l_CombineMatrices();
}

/* The blur the sun's test reads, set up and drawn. */
static void glr_DrawSunBlur(float z, int r, int g, int b, int a)
{
    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    gl_state.sunEffectOn = 1;
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    glr_QuadAtDepth(z);
    glr_BlurMatrices();
    glr_SolidColour(r, g, b, a);

    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
}

static void caseDepthBlurOff(void)
{
    printf("-- whether the engine is asked for the pass at all\n");
    glr_Reset();

    /* Nothing asked for it: no master bits, no sun. */
    ok(!GL_b_DepthBlurOn(), "no blur, so no pass");

    /* A sun on its own turns it on, and the tweak beats a sun. */
    gl_state.sunEffectOn = 1;
    ok(GL_b_DepthBlurOn(), "a sun that wants its test turns it on");
    gl_state.noZonAlpha = 1;
    ok(!GL_b_DepthBlurOn(), "and the tweak turns it off again");
    gl_state.noZonAlpha = 0;

    /* Without a context there is nothing to answer with, whatever the
       engine's state says -- this renderer may not be the one in
       force. */
    gl_state.context = NULL;
    ok(!GL_b_DepthBlurOn(), "and with no context the answer is no");
}

static void caseDepthBlurSun(void)
{
    static const float depths[] = { -0.75f, -0.25f, 0.0f, 0.25f, 0.75f };
    int                i;

    printf("-- the sun's ramp: the coordinate is the depth over the"
           " frustum\n");

    for (i = 0; i < (int)(sizeof(depths) / sizeof(depths[0])); i++)
    {
        tDpb_State st;
        double     want;

        glr_DrawSunBlur(depths[i], 0x20, 0x40, 0x60, 0x10);
        GL_l_ReadBlurState(&st);
        want = glr_RampAt(&st, (double)depths[i] * (double)st.scaleUfromZ);

        /* Two counts of slack for the sampler's rounding and the byte
           it is read back as. The five answers are about 223, 191,
           127, 63 and 0, so a coordinate scaled wrongly, signed
           wrongly, or run through the projected form instead misses by
           far more than two. */
        okNear(glr_A(GLR_W / 2, GLR_H / 2), (int)(want + 0.5),
               "the alpha is the ramp at this depth");
    }

    /* Nearer is less blurred, which is the direction of the whole
       thing and the one a sign error reverses while every value above
       stays in range. */
    {
        int nearer;
        int farther;

        glr_DrawSunBlur(-0.9f, 0, 0, 0, 0);
        nearer = glr_A(GLR_W / 2, GLR_H / 2);
        glr_DrawSunBlur(0.9f, 0, 0, 0, 0);
        farther = glr_A(GLR_W / 2, GLR_H / 2);
        ok(nearer > farther,
           "a nearer surface is left less blurred than a farther one");
    }
}

static void caseDepthBlurProjected(void)
{
    tDpb_State st;
    double     viaReciprocal;
    double     viaLinear;

    printf("-- the far blur's ramp: the coordinate is a reciprocal\n");

    /* The far blur, whose coordinate is the blur distance over the
       depth rather than the depth over the frustum. */
    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    PC_AE_MASTER = 8;
    depthBlurType0 = 0.5f;
    depthBlurType1 = 1.0f;
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    GL_l_ReadBlurState(&st);
    okEq(st.kind, DPB_FAR, "the far blur is in force");
    okEq(st.b_projected, 1, "and its coordinate is projected");

    viaReciprocal = glr_RampAt(&st, (double)st.translateU
                                    / (0.75 * (double)st.scaleW));
    viaLinear = glr_RampAt(&st, 0.75 * (double)st.scaleUfromZ
                                + (double)st.translateU);

    /* The two forms have to disagree here, or the check below would
       pass whichever one the shader used. */
    ok(fabs(viaReciprocal - viaLinear) > 8.0,
       "the linear form lands somewhere else, so this tells them apart");

    glr_QuadAtDepth(0.75f);
    glr_IdentityMatrices();
    glr_SolidColour(0, 0, 0, 0);
    ok(GL_b_DepthBlurOn(), "the pass is asked for");
    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    okNear(glr_A(GLR_W / 2, GLR_H / 2), (int)(viaReciprocal + 0.5),
           "the alpha is the ramp at the reciprocal coordinate");
}

/* The ramp is rebuilt when the decision changes, and not once a frame.
 *
 * Every other case starts from a reset, which forgets the ramp -- so
 * the rebuild always happened on the first draw and never because
 * something changed. Leaving the ramp and the uniforms at whatever the
 * first draw set them to was the second mutation no case noticed. */
static void caseDepthBlurFollowsTheState(void)
{
    tDpb_State st;
    int        asSun;
    double     wantFar;

    printf("-- the ramp follows a decision that changes under it\n");

    glr_DrawSunBlur(0.25f, 0, 0, 0, 0);
    asSun = glr_A(GLR_W / 2, GLR_H / 2);

    /* Now the far blur instead, with no reset in between: the same
       context, the same program, a different decision. */
    gl_state.sunEffectOn = 0;
    PC_AE_MASTER = 8;
    depthBlurType0 = 0.5f;
    depthBlurType1 = 1.0f;

    GL_l_ReadBlurState(&st);
    okEq(st.kind, DPB_FAR, "the decision has changed");

    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    wantFar = glr_RampAt(&st, (double)st.translateU
                              / (0.25 * (double)st.scaleW));
    okNear(glr_A(GLR_W / 2, GLR_H / 2), (int)(wantFar + 0.5),
           "and the alpha is the new ramp, not the one already uploaded");
    ok(fabs((double)asSun - wantFar) > 8.0,
       "which is a different answer from the old ramp's, so the check"
       " above means something");
}

/* The ramp is sampled with a linear filter, as the other renderer's
   sampler asks for.
 *
 * The sun's ramp falls off smoothly, so nearest and linear differ
 * there by about one count and no tolerance can see it. The near
 * blur's ramp is a cliff -- solid to the second-to-last entry and zero
 * at the last -- so a coordinate landing between those two is halfway
 * under a linear filter and at the top under a nearest one. That is a
 * difference of about 127, which is the only way this is testable. */
static void caseDepthBlurFilter(void)
{
    tDpb_State st;
    float      depth;

    printf("-- the ramp is sampled with a linear filter\n");

    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    PC_AE_MASTER = 0x1000;
    depthBlurNear1 = 0.5f;
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    GL_l_ReadBlurState(&st);
    okEq(st.kind, DPB_NEAR, "the near blur, whose ramp has a cliff in it");

    /* Halfway between the last two entries: u = 255/256, and this
       blur's coordinate is the near distance over the depth. */
    depth = (float)((double)st.translateU / (255.0 / 256.0)
                    / (double)st.scaleW);
    glr_QuadAtDepth(depth);
    glr_BlurMatrices();
    glr_SolidColour(0, 0, 0, 0);

    ok(GL_b_DepthBlurOn(), "the pass is asked for");
    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* Blended, so about half of full. A nearest filter gives the
       entry below the cliff, which is full. */
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 128,
           "the two sides of the cliff are blended, not snapped to one");
}

/* The sprites are not the blur pass, whatever ran before them.
 *
 * They share the one draw with the world's geometry, so which program
 * that draw uses had better not be ambient state -- a sprite drawn
 * through the blur program writes alpha and no colour, which is a
 * sprite that is not there. The engine does call TextureInit between
 * the two in ordinary play; this is the case that says the sprites do
 * not depend on it. */
static void caseDepthBlurLeavesSpritesAlone(void)
{
    printf("-- the sprites are drawn in colour after a blur pass\n");

    glr_DrawSunBlur(0.25f, 0, 0, 0, 0);
    ok(gl_material.b_depthBlur, "the blur pass is the last thing configured");

    /* And now a sprite, with no TextureInit in between. One at the
       origin, sized as the sprite case sizes it: the element is a
       union whose whole word read as a float is the size, so index
       nought with a high half of 0x3F00 is a half. */
    {
        tGeoSpriteElem elems[1];
        Vec3           pos[1];
        tGeoSprites    sprites;
        Mtx4x4         stack;

        elems[0].idx = 0;
        elems[0].dummy = 0x3F00;
        pos[0].x = 0.0f;
        pos[0].y = 0.0f;
        pos[0].z = 0.0f;
        memset(&sprites, 0, sizeof(sprites));
        sprites.num = 1;
        sprites.spriteElems = elems;
        sprites.globalSize = 1.0f;
        sprites.globalRatio = 1.0f;
        MATH_SetIdentityMatrix(&stack);
        glr_dd.matrixStack.actMatrixElem = &stack;

        glr_Begin();
        glr_SolidColour(0xFF, 0x20, 0x40, 0xFF);
        GL_DrawSprites(&sprites, pos, 1);
        glr_Read();

        okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF,
               "the sprite is drawn in its own colour, not as alpha alone");
        okNear(glr_G(GLR_W / 2, GLR_H / 2), 0x20, "green too");
        okNear(glr_B(GLR_W / 2, GLR_H / 2), 0x40, "and blue");
    }
}

static void caseDepthBlurWritesAlphaOnly(void)
{
    int red;
    int green;
    int blue;
    int alphaBefore;

    printf("-- the pass writes the alpha and leaves the colour alone\n");

    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    gl_state.sunEffectOn = 1;
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    /* The world, drawn first: the frame the pass has to leave alone. */
    glr_QuadAtDepth(0.5f);
    glr_IdentityMatrices();
    glr_OneTexel(0xFF, 0xFF, 0xFF, 0xFF);
    glr_SolidColour(0x80, 0x40, 0xC0, 0x10);
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    red = glr_R(GLR_W / 2, GLR_H / 2);
    green = glr_G(GLR_W / 2, GLR_H / 2);
    blue = glr_B(GLR_W / 2, GLR_H / 2);
    alphaBefore = glr_A(GLR_W / 2, GLR_H / 2);
    okNear(alphaBefore, 0x10, "the first pass left its own alpha");

    /* And the blur over it, at the same depth, so only the write mask
       separates the two outcomes. */
    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    okEq(glr_R(GLR_W / 2, GLR_H / 2), red, "red is untouched");
    okEq(glr_G(GLR_W / 2, GLR_H / 2), green, "green too");
    okEq(glr_B(GLR_W / 2, GLR_H / 2), blue, "and blue");
    ok(glr_A(GLR_W / 2, GLR_H / 2) != alphaBefore,
       "and the alpha is the pass's, not the material's");

    okNear(glr_R(2, 2), GLR_BG, "nothing outside the square either");
}

static void caseShadersCompile(void)
{
    printf("-- every shipped shader compiles on a real driver\n");

    /* The mock harness checks that the files load and that the
       uniform names match. A real compiler is the only thing that
       says the GLSL is GLSL -- and it reports what is wrong with it,
       which the log carries. */
    glr_Reset();
    ok(GL_l_b_GeometryReady(), "the world's program builds");
    ok(GL_l_b_PassReady(), "and the overlay's two");
    {
        int i;

        for (i = 0; i < glrLogLen; i++)
            if (strstr(glrLog[i], "shader") || strstr(glrLog[i], "program"))
                printf("   driver said: %s", glrLog[i]);
    }
}

int main(void)
{
    void* p;
    char  missing[64];
    int   unresolved;

    printf("=== GL render test ===\n");

    /* The repository root, so data/shaders resolves to the real
       files. */
    snprintf(glr_exePath, sizeof(glr_exePath), "./iriseng.exe");

    /* The sine table, which the sprite wobble reads through
       MATH_GetSin. */
    MATH_InitModule();

    if (!EGLH_b_Open())
    {
        printf("  skipped: %s\n", EGLH_why);
        printf("  (this test needs a software OpenGL 4.5; the rest of the"
               " renderer's tests do not)\n");
        return 0;
    }

    /* The renderer's own pointers, aimed at the live driver. One
       lookup rather than two: EGL answers for everything, where WGL
       splits the list between the context and opengl32.dll. */
    unresolved = GL_i_LoadFunctions(EGLH_p_Proc, EGLH_p_Proc, missing,
                                    sizeof(missing));
    if (unresolved)
    {
        printf("  skipped: this driver is short of %d entry point(s),"
               " first is %s\n", unresolved, missing);
        EGLH_Close();
        return 0;
    }

    p = EGLH_p_Proc("glReadPixels");
    memcpy(&glr_ReadPixels, &p, sizeof(p));
    if (!glr_ReadPixels)
    {
        printf("  skipped: no glReadPixels, so nothing could be checked\n");
        EGLH_Close();
        return 0;
    }

    /* The renderer's own idea of where clip space starts, which it
       sets at init and which the engine's projections depend on. */
    GLFN_glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    if (!glr_b_MakeSurface())
    {
        printf("  skipped: no complete framebuffer to draw into\n");
        EGLH_Close();
        return 0;
    }

    caseContext();
    caseDepthBlurOff();
    caseDepthBlurSun();
    caseDepthBlurProjected();
    caseDepthBlurFollowsTheState();
    caseDepthBlurFilter();
    caseDepthBlurLeavesSpritesAlone();
    caseDepthBlurWritesAlphaOnly();
    caseShadersCompile();
    caseGeometryModulates();
    caseGeometryUntextured();
    caseGeometrySwizzle();
    caseGeometryDiffuse2x();
    caseGeometryAlphaTest();
    caseGeometryTexCoords();
    caseGeometryMatrix();
    caseOverlayQuads();
    caseOverlayText();
    caseSprites();
    caseVideo();
    caseShadowTarget();

    GL_l_ForgetPass();
    GL_l_ForgetGeometry();
    GL_l_ForgetBlur();
    GL_ReleaseTextures(NULL);
    GL_l_ForgetTargets();
    EGLH_Close();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
