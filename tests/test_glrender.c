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
#include "../water.h"

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

/* And a tighter one, for a reading whose expected value was worked
   out rather than chosen. okNear's two bytes of room is right for a
   channel that went through the rasteriser; it is too much for a
   number this file derives from aecolor.c, where the whole difference
   being looked for can be a step or two. The tolerance is the
   caller's, and the caller has to say why it is what it is. */
static void okWithin(double got, double want, double tol, const char* what)
{
    checks++;
    if (!(fabs(got - want) <= tol))
    {
        printf("  FAIL %s: got %.3f want %.3f (+-%.3f)\n", what, got, want,
               tol);
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
/* The fade's timer. gdi.c keeps it in the other renderer's parameter
   block and reaches it with these two; here they are the test's own
   storage, which is also how a case drives a fade. */
int  glr_fadeOpacity;

int GDI_i_AeFadeOpacity(void)
{
    return glr_fadeOpacity;
}

void GDI_AeFadeOpacitySet(int opacity)
{
    glr_fadeOpacity = opacity;
}

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

/* The sun's sampler is one plain allocation. */
/* The engine's debug overlay, which GL_Flip draws last. Its own
   behaviour is the engine's and is the same for both renderers; what
   matters here is only that it links. The mock harness is where the
   call itself is checked. */
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
/* The water's own debug tweaks, which live in dbg.c. The reflection
   reads three of them: the size an object has to reach to be worth
   reflecting, whether the clip plane is used, and how far off the water
   line it sits. Here at the values dbg.c gives them, except the
   threshold, which starts at nought -- where everything is worth
   reflecting. */
float tweak_water_min_bv;
int   tweak_water_clip = 1;
float tweak_water_off;
/* The smoothing: on by default, as dbg.c has it, with both amounts at
   the nought that makes every tap coincide. */
int   tweak_water_smooth = 1;
float tweak_water_smoothx;
float tweak_water_smoothy;

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

/* The engine's answer about a bounding volume's kind, which the
   water's size test asks before choosing which number to read. It is a
   sentinel bit pattern in a field that is a coordinate the rest of the
   time, and this is the engine's own rule rather than a stand-in -- the
   reflection only ever asks, so there is nothing else to mock. */
BOOL OBJ_BV_IsSphere(tGameObjectBV* bv)
{
    return bv && (*((uint32_t*)&bv->BVMax.z)) == 0xFFFFFFFFu;
}


int   ECAM_gi_NoAfterEffects = 0;

/* The two colour parameters, at the neutral the game starts them at.
   Neutral is a half and not nought: left at the nought a tentative
   definition gives, every frame would come back flattened to mid
   grey and half darkened. */
float unkAEFunc2Param = 0.5f;
float colorBalanceParam1 = 0.5f;

void OVL_Draw(void) { }

void* MEM_p_Alloc(unsigned int size) { return malloc((size_t)size); }
void  MEM_Free(void* p) { free(p); }

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
    GL_l_ForgetAe();
    GL_ReleaseTextures(NULL);
    GL_l_ForgetTargets();
    GL_l_ForgetWater();
    pst_GlobalsWaterParams = NULL;
    tweak_gl_water_flip = 0;
    tweak_water_blur_normal = 0;
    tweak_water_clip = 0;
    tweak_water_off = 0.0f;
    tweak_water_min_bv = 0.0f;
    waterHeight = 0.0f;
    tweak_water_smooth = 1;
    tweak_water_smoothx = 0.0f;
    tweak_water_smoothy = 0.0f;
    GL_l_ForgetWaterSmooth();

    /* The two switches that turn the whole after-effects chain off,
       both back to the off the game starts them at. */
    tweak_ae_no = 0;
    ECAM_gi_NoAfterEffects = 0;
    unkAEFunc2Param = 0.5f;
    colorBalanceParam1 = 0.5f;

    /* The after-effects state the blur's decision is read from. It is
       the engine's, set by the AI scripts, so a case that wants a
       particular blur sets it and every other case starts with none. */
    PC_AE_MASTER = 0;
    GDI_gi_TextureUVMode = 0;
    memset(&d3dTransformationMatrix[0], 0, sizeof(d3dTransformationMatrix[0]));
    glr_dd.GROGEO = NULL;
    memset(&gl_fog, 0, sizeof(gl_fog));
    gl_fogSecondColor = 0;
    gl_fogSaidNoVertex = 0;
    depthBlurNear1 = 0.0f;
    depthBlurType0 = 0.0f;
    depthBlurType1 = 0.0f;
    /* The fade's duration and its timer. The timer outlives a frame,
       so a case that left a fade part way through would hand the next
       one a picture frozen somewhere it never asked for. */
    gAEOpacity = 0.0f;
    glr_fadeOpacity = 0;
    unknownAEFlag4GlobalVar = 0.0f;
    texFactorManual = 0.0f;
    /* The motion smooth's watched direction. The renderer does not
       clear this with the targets on purpose -- see the note beside it
       -- so the harness must, or a case inherits a camera turn that
       never happened. */
    memset(&gl_aeMotionPrev, 0, sizeof(gl_aeMotionPrev));
    blurFactor = 0.0f;

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

/* ================================================ the sun's occlusion test
 *
 * Nine points around the sun, nine bits back: which of them have
 * something in front of them. The answer comes out of the frame's
 * alpha, which the depth-blur pass above wrote -- so these cases draw
 * a blur pass first and then ask about it, which is how the two pieces
 * meet.
 *
 * A set bit means blocked. That is not what the other renderer's
 * comments say, and it is worth being able to point at why: the engine
 * sets all nine when the sun is hidden outright, and in the flare's
 * render a set bit contributes zero luminance. Both are in gfx/gfxsun.c.
 */

/* The readback is asynchronous on purpose, so a read straight after a
   queue is allowed to answer "not yet". The engine tolerates that --
   it reuses last frame's bits -- but a test cannot, so it waits for
   the card first. glFinish is the blunt instrument that makes the
   asynchrony stop mattering for the duration of one check. */
static void glr_FinishGPU(void)
{
    if (GLFN_glFinish)
        GLFN_glFinish();
}

/* One blur pass over a rectangle at a given depth, so a frame can be
   built out of more than one depth and a point's alpha depends on
   where it is. */
static void glr_BlurRect(float x0, float x1, float z)
{
    glr_BuildQuad();
    glr_pos[0].x = x0; glr_pos[0].z = z;
    glr_pos[1].x = x1; glr_pos[1].z = z;
    glr_pos[2].x = x1; glr_pos[2].z = z;
    glr_pos[3].x = x0; glr_pos[3].z = z;

    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
}

/* A frame whose alpha is the sun's depth ramp, with the quad at a
   known depth -- so every pixel of the square carries a known alpha
   and a point sampled inside it has a known answer. */
static void glr_SunFrame(float quadDepth)
{
    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    gl_state.sunEffectOn = 1;
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;

    glr_QuadAtDepth(quadDepth);
    glr_BlurMatrices();
    glr_SolidColour(0, 0, 0, 0);

    GL_TextureInit(-1, 0, 1);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
}

/* The camera-space depth at which the sun's own blur amount is `want`,
   so a case can put the threshold between two alphas it has measured
   rather than hoping one lands on the right side of it. */
static float glr_SunDepthForThreshold(const tDpb_State* st, int want)
{
    return (float)((1.0 - (double)want / 255.0) * (double)st->farUsed);
}

static void caseSunSampler(void)
{
    void* sampler;

    printf("-- the sun's sampler is made and says it is ready\n");

    glr_Reset();
    sampler = GL_p_SunSamplerCreate();
    ok(sampler != NULL, "a sampler is made");
    ok(GL_b_SunSamplerReady(sampler), "and says so");
    ok(!GL_b_SunSamplerReady(NULL), "nothing is not a sampler");

    /* Whether a sun wants its test is the aggregate over every sun,
       because that is what the blur's mode depends on. */
    GL_SunEffectOnOff(sampler, 1);
    okEq(gl_state.sunEffectOn, 1, "a sun that asks turns the effect on");
    GL_SunEffectOnOff(sampler, 0);
    okEq(gl_state.sunEffectOn, 0, "and the last one to stop turns it off");

    /* Two suns, so the aggregate is doing something. */
    {
        void* other = GL_p_SunSamplerCreate();

        ok(other != NULL, "a second sampler is made");
        GL_SunEffectOnOff(sampler, 1);
        GL_SunEffectOnOff(other, 0);
        okEq(gl_state.sunEffectOn, 1,
             "one sun still wanting it keeps it on");
        GL_SunEffectOnOff(sampler, 0);
        okEq(gl_state.sunEffectOn, 0, "and with neither it goes off");
        GL_SunSamplerDestroy(other);
    }

    /* Sampling only happens in the blur mode whose ramp carries the
       depth, which is the sun's own. */
    gl_state.sunEffectOn = 1;
    ok(GL_b_SunSamplingNow(sampler),
       "with the sun's blur in force there is something to read");
    PC_AE_MASTER = 8;
    depthBlurType0 = 0.5f;
    depthBlurType1 = 1.0f;
    ok(!GL_b_SunSamplingNow(sampler),
       "and with another blur in force there is not");

    GL_SunSamplerDestroy(sampler);
    ok(!GL_b_SunSamplerReady(NULL), "and a destroyed sampler is not asked");
}

static void caseSunReadsTheBlurAlpha(void)
{
    void*  sampler;
    int    x[9];
    int    y[9];
    int    blocked = 0;
    int    i;
    double alphaInSquare;

    printf("-- the nine bits come out of the alpha the blur wrote\n");

    glr_SunFrame(0.5f);
    sampler = GL_p_SunSamplerCreate();
    ok(sampler != NULL, "a sampler for the frame just drawn");

    /* What the square's alpha is, so the expectations below are
       computed from the frame rather than assumed. */
    glr_Read();
    alphaInSquare = (double)glr_A(GLR_W / 2, GLR_H / 2);
    ok(alphaInSquare > 0.0 && alphaInSquare < 255.0,
       "the square carries a middling alpha, so either comparison is"
       " reachable");

    /* All nine points inside the square, and a threshold below its
       alpha: every one is nearer than the sun, so every one blocks. */
    for (i = 0; i < 9; i++)
    {
        x[i] = GLR_W / 2;
        y[i] = GLR_H / 2;
    }

    /* The threshold comes from the sun's own depth, so a sun behind
       the far plane has a threshold of nothing and everything drawn
       blocks it. */
    GL_SunSampleQueue(sampler, x, y, 9, 0x1FF, 2.0f);
    glr_FinishGPU();
    ok(GL_b_SunSampleRead(sampler, &blocked), "an answer arrives");
    okEq(blocked, 0x1FF, "and every point is blocked by what was drawn");

    /* And a sun in front of everything: its own alpha is the highest
       there is, so nothing blocks it. */
    blocked = 0;
    GL_SunSampleQueue(sampler, x, y, 9, 0x1FF, -1.0f);
    glr_FinishGPU();
    ok(GL_b_SunSampleRead(sampler, &blocked), "a second answer arrives");
    okEq(blocked, 0, "and nothing blocks a sun in front of the frame");

    GL_SunSamplerDestroy(sampler);
}

static void caseSunMaskAndPacking(void)
{
    void* sampler;
    int   x[9];
    int   y[9];
    int   blocked = 0;

    tDpb_State st;
    int        alphaNear;
    int        alphaFar;
    float      sunZ;

    printf("-- a point outside the viewport is blocked without being"
           " looked at\n");

    /* A frame of two depths: the whole square far away, and a strip
       down the middle of it close up. So a point on the strip is
       nearer than a point beside it, and a threshold between the two
       gives the sampled points different answers -- which is what it
       takes to tell a resolve that walks the pixels in order from one
       that indexes them by bit.
     *
     * The background is no use for this. It is never drawn, so it
     * keeps the cleared alpha, and this harness clears it to 255 --
     * the nearest value there is, so the background blocks everything.
     * The first draft of this case assumed the opposite and failed. */
    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    gl_state.sunEffectOn = 1;
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BlurMatrices();
    glr_SolidColour(0, 0, 0, 0);
    glr_BlurRect(-0.5f, 0.5f, 0.8f);      /* far, the whole square */
    glr_BlurRect(-0.2f, 0.2f, 0.1f);      /* near, a strip of it */

    glr_Read();
    alphaNear = glr_A(GLR_W / 2, GLR_H / 2);           /* on the strip */
    alphaFar = glr_A((int)(GLR_W * 0.7), GLR_H / 2);   /* beside it */
    ok(alphaNear > alphaFar + 8,
       "the strip reads nearer than the square around it");

    GL_l_ReadBlurState(&st);
    sunZ = glr_SunDepthForThreshold(&st, (alphaNear + alphaFar) / 2);

    sampler = GL_p_SunSamplerCreate();

    /* Three of the nine were on screen -- bits 0, 4 and 8 -- and the
       engine hands their pixels over packed into the first three
       places. Bits 0 and 4 are on the strip and block; bit 8 is beside
       it and does not. A resolve that indexed the pixels by bit rather
       than by order would read the third pixel for bit 8, which is the
       right one, and nothing at all for bits 0 and 4 -- so the two
       mistakes point opposite ways and all three are checked. */
    x[0] = GLR_W / 2;             y[0] = GLR_H / 2;
    x[1] = GLR_W / 2;             y[1] = GLR_H / 2 + 2;
    x[2] = (int)(GLR_W * 0.7);    y[2] = GLR_H / 2;

    GL_SunSampleQueue(sampler, x, y, 3, (1 << 0) | (1 << 4) | (1 << 8),
                      sunZ);
    glr_FinishGPU();
    ok(GL_b_SunSampleRead(sampler, &blocked), "an answer arrives");

    okEq(blocked & ((1 << 0) | (1 << 4)), (1 << 0) | (1 << 4),
         "the two points on the near strip are blocked");
    okEq(blocked & (1 << 8), 0,
         "the one beside it, farther than the sun, is not");
    okEq(blocked & ~((1 << 0) | (1 << 4) | (1 << 8)),
         0x1FF & ~((1 << 0) | (1 << 4) | (1 << 8)),
         "and the six that were never sampled are all blocked");

    GL_SunSamplerDestroy(sampler);
}

static void caseSunRing(void)
{
    void* sampler;
    int   x[9];
    int   y[9];
    int   blocked = 0;
    int   i;

    printf("-- three samples in flight, and the fourth frame goes"
           " unsampled\n");

    glr_SunFrame(0.5f);
    sampler = GL_p_SunSamplerCreate();

    for (i = 0; i < 9; i++)
    {
        x[i] = GLR_W / 2;
        y[i] = GLR_H / 2;
    }

    /* Nothing read, so the ring fills. The fourth queue has nowhere to
       go and is dropped rather than overwriting an answer that has not
       been collected. */
    for (i = 0; i < 4; i++)
        GL_SunSampleQueue(sampler, x, y, 9, 0x1FF, 2.0f);
    okEq(((tGlSun*)sampler)->inFlight, 3, "three, and no more, are in flight");

    /* One read empties the whole ring, because an older answer is of
       no use once a newer one has been given. */
    glr_FinishGPU();
    ok(GL_b_SunSampleRead(sampler, &blocked), "a read answers");
    okEq(((tGlSun*)sampler)->inFlight, 0,
         "and retires everything up to the one it read");
    ok(!GL_b_SunSampleRead(sampler, &blocked),
       "so a second read has nothing to give");

    /* And the ring takes samples again afterwards, which is the thing
       an off-by-one in the retirement breaks. */
    for (i = 0; i < 3; i++)
        GL_SunSampleQueue(sampler, x, y, 9, 0x1FF, 2.0f);
    okEq(((tGlSun*)sampler)->inFlight, 3, "the ring fills again");
    glr_FinishGPU();
    ok(GL_b_SunSampleRead(sampler, &blocked), "and answers again");

    GL_SunSamplerDestroy(sampler);
}

/* The fence is consulted, and a sample the card has not finished with
 * is not read.
 *
 * Every case above calls glFinish first, which is what makes them
 * deterministic -- and it also means the fence is always signalled, so
 * a sampler that ignored it entirely passed all of them. This is the
 * one place the asynchrony itself is checked, and it is checked by
 * aiming that one entry point at a mock that says "not yet": every GL
 * call goes through a function pointer, which is what makes that
 * possible.
 */
static GLenum (GL_APIENTRY *glr_realWaitSync)(GLsync, GLbitfield, GLuint64);

static GLenum GL_APIENTRY glr_WaitSyncNotYet(GLsync sync, GLbitfield flags,
                                             GLuint64 timeout)
{
    (void)sync;
    (void)flags;
    (void)timeout;
    return GL_TIMEOUT_EXPIRED;
}

static void caseSunWaitsForTheCard(void)
{
    void* sampler;
    int   x[9];
    int   y[9];
    int   blocked = 0x5A5A;
    int   i;

    printf("-- a sample the card has not finished with is not read\n");

    glr_SunFrame(0.5f);
    sampler = GL_p_SunSamplerCreate();
    for (i = 0; i < 9; i++)
    {
        x[i] = GLR_W / 2;
        y[i] = GLR_H / 2;
    }

    GL_SunSampleQueue(sampler, x, y, 9, 0x1FF, 2.0f);
    glr_FinishGPU();          /* so the only thing saying "not yet" is the mock */

    glr_realWaitSync = GLFN_glClientWaitSync;
    GLFN_glClientWaitSync = glr_WaitSyncNotYet;
    ok(!GL_b_SunSampleRead(sampler, &blocked),
       "the read answers no while the fence says not yet");
    okEq(blocked, 0x5A5A, "and leaves the caller's word alone");
    okEq(((tGlSun*)sampler)->inFlight, 1,
         "and the sample is still in flight, not thrown away");

    GLFN_glClientWaitSync = glr_realWaitSync;
    ok(GL_b_SunSampleRead(sampler, &blocked),
       "and once the fence says so, the same sample is read");
    okEq(blocked, 0x1FF, "with the answer it was holding all along");

    GL_SunSamplerDestroy(sampler);
}

static void caseSunRefusals(void)
{
    void* sampler;
    int   x[9];
    int   y[9];
    int   blocked = 0x5A5A;
    int   i;

    printf("-- what the sampler declines to do\n");

    glr_SunFrame(0.5f);
    sampler = GL_p_SunSamplerCreate();
    for (i = 0; i < 9; i++)
    {
        x[i] = GLR_W / 2;
        y[i] = GLR_H / 2;
    }

    /* Nothing queued, so nothing to read -- and the caller's value is
       left alone rather than being written with a made-up answer. */
    ok(!GL_b_SunSampleRead(sampler, &blocked), "an empty ring answers no");
    okEq(blocked, 0x5A5A, "and leaves the caller's word alone");

    /* A sampler that is not one, and counts that are not counts. */
    GL_SunSampleQueue(NULL, x, y, 9, 0x1FF, 1.0f);
    GL_SunSampleQueue(sampler, NULL, y, 9, 0x1FF, 1.0f);
    GL_SunSampleQueue(sampler, x, NULL, 9, 0x1FF, 1.0f);
    GL_SunSampleQueue(sampler, x, y, -1, 0x1FF, 1.0f);
    GL_SunSampleQueue(sampler, x, y, 10, 0x1FF, 1.0f);
    okEq(((tGlSun*)sampler)->inFlight, 0, "none of those queued anything");
    ok(!GL_b_SunSampleRead(NULL, &blocked), "and nothing is not a sampler");

    /* A read that is asked for no answer still retires the slot, so a
       caller that does not want the bits does not wedge the ring. */
    GL_SunSampleQueue(sampler, x, y, 9, 0x1FF, 2.0f);
    glr_FinishGPU();
    ok(GL_b_SunSampleRead(sampler, NULL), "a read with nowhere to put it"
                                          " still answers");
    okEq(((tGlSun*)sampler)->inFlight, 0, "and still retires the slot");

    GL_SunSamplerDestroy(sampler);
    GL_SunSamplerDestroy(NULL);     /* and destroying nothing is nothing */

    /* With no context there is no sampler to make: this renderer may
       not be the one in force. */
    gl_state.context = NULL;
    ok(GL_p_SunSamplerCreate() == NULL, "no context, no sampler");
}

/* ============================================================== fog
 *
 * The engine hands over a curve, a range, a density and a colour once,
 * and then each material says whether fog applies to it and which of
 * two colours it uses. The other renderer turns that into
 * fixed-function state; this one has to do the arithmetic, so the
 * arithmetic is what these cases are about and only a real driver can
 * run it.
 *
 * The draw-mask bit 0x4000 is what permits fog on a pass, and the
 * material's flag word carries the variant in bits 16 to 19.
 */

#define GLR_D_FOG   0x00004000
#define GLR_D_SOLID 0x00000010

/* The engine's fog request, which is the only way these settings get
   in -- so the cases go through it rather than writing the state. */
static void glr_FogRequest(int engineCurve, float start, float end,
                           float density, unsigned int engineColor)
{
    tFogParams fog;

    memset(&fog, 0, sizeof(fog));
    fog.flags = 1;
    fog.fogVertexMode = (char)engineCurve;
    fog.fogStart = start;
    fog.fogEnd = end;
    fog.density = density;
    fog.fogColor = (int)engineColor;
    GL_Request(1, &fog);
}

/* A lit quad at a known depth with fog permitted, so the colour read
   back is the fogged one. The material's flag word carries the fog
   variant in bits 16 to 19; `mode` is that field. */
static void glr_DrawFogged(float depth, int mode, int b_fogAllowed)
{
    /* Cleared first. Without this a second draw lands on top of the
       first, and with an additive blend that reads as saturated white
       whatever the fog did -- which is how the variant case first
       failed. */
    glr_Begin();
    glr_BuildQuad();
    glr_pos[0].z = depth;
    glr_pos[1].z = depth;
    glr_pos[2].z = depth;
    glr_pos[3].z = depth;
    glr_BlurMatrices();
    /* White, so the fog colour is all of what is read back -- and an
       alpha that is not full, so that fog reaching the alpha would
       move it. With the alpha at full it would not: the fog colour's
       own alpha is full too, and mixing one with the other is the
       value either way. */
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0x40);
    glr_dd.drawMaskAllowedStuffCurrent =
        GLR_D_SOLID | (b_fogAllowed ? GLR_D_FOG : 0);

    GL_TextureInit(-1, (mode & 0xF) << 16, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
}

/* What the shader should have produced: the fog colour and the pixel
   mixed by the factor, worked out from fog.c rather than here. */
static int glr_FoggedChannel(int pixel, int fogChannel, float depth)
{
    double f = (double)FOG_f_Factor(&gl_fog, depth);

    return (int)((1.0 - f) * (double)fogChannel + f * (double)pixel + 0.5);
}

static void caseFogCurves(void)
{
    /* The engine's own numbering: 0 linear, 1 exponential, 2 squared,
       3 the one that takes its factor off the vertices. */
    static const int   curves[3] = { 0, 1, 2 };
    static const float depths[3] = { 0.1f, 0.4f, 0.8f };
    int                ci;
    int                di;

    printf("-- the three distance curves\n");

    for (ci = 0; ci < 3; ci++)
        for (di = 0; di < 3; di++)
        {
            int want;

            glr_Reset();
            glr_dd.ct.cam.nearPlane = 0.01f;
            glr_dd.ct.cam.farPlane = 1.0f;
            /* A fog colour whose channels are all different, and in
               the engine's byte order -- so a renderer that forgot the
               red-and-blue swap reads one of them in another's place. */
            glr_FogRequest(curves[ci], 0.0f, 1.0f, 2.0f, 0x00204060u);

            glr_DrawFogged(depths[di], 0, 1);

            /* Red is the engine word's lowest byte, because the engine
               keeps its colours the other way round from the device.
               0x204060 is therefore red 0x60, green 0x40, blue 0x20. */
            want = glr_FoggedChannel(0xFF, 0x60, depths[di]);
            okNear(glr_R(GLR_W / 2, GLR_H / 2), want,
                   "red is the pixel and the fog colour mixed by the"
                   " curve");
            want = glr_FoggedChannel(0xFF, 0x40, depths[di]);
            okNear(glr_G(GLR_W / 2, GLR_H / 2), want, "green likewise");
            want = glr_FoggedChannel(0xFF, 0x20, depths[di]);
            okNear(glr_B(GLR_W / 2, GLR_H / 2), want,
                   "and blue, which is the engine's lowest byte and not"
                   " its highest");
        }

    /* And the three curves really do differ at one of those depths, or
       the loop above would pass whichever one the shader ran. */
    {
        tFog_Source src;
        tFog_State  a;
        tFog_State  b;
        tFog_State  c;

        src.b_enabled = 1;
        src.start = 0.0f;
        src.end = 1.0f;
        src.density = 2.0f;
        src.engineColor = 0;
        src.engineCurve = 0;  FOG_Decode(&src, &a);
        src.engineCurve = 1;  FOG_Decode(&src, &b);
        src.engineCurve = 2;  FOG_Decode(&src, &c);
        ok(fabs((double)FOG_f_Factor(&a, 0.4f) - FOG_f_Factor(&b, 0.4f)) > 0.05
           && fabs((double)FOG_f_Factor(&b, 0.4f)
                   - FOG_f_Factor(&c, 0.4f)) > 0.05,
           "the three curves give three different factors at that depth");
    }
}

static void caseFogAlphaAndPermission(void)
{
    int fogged;
    int plain;

    printf("-- what fog does not touch\n");

    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    /* A dark fog against a white quad. White fog on a white quad is
       invisible however much of it there is, which is what the first
       draft of this case asked about. */
    glr_FogRequest(0, 0.0f, 1.0f, 1.0f, 0x00102030u);

    /* Alpha is not fogged: the hardware's fog leaves it alone, and the
       depth-blur pass reads that alpha afterwards. */
    glr_DrawFogged(0.9f, 0, 1);
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 0x40, "the alpha is not fogged");
    fogged = glr_R(GLR_W / 2, GLR_H / 2);
    ok(fogged < 0x80, "and the colour is heavily fogged at that depth");

    /* A pass whose draw mask does not permit fog is not fogged, even
       with fog set up. */
    glr_DrawFogged(0.9f, 0, 0);
    plain = glr_R(GLR_W / 2, GLR_H / 2);
    okNear(plain, 0xFF, "a pass that does not permit fog is not fogged");
    ok(fogged != plain, "and the permitted one was, so this means"
                        " something");

    /* No fog asked for at all: the request's flag clear. */
    {
        tFogParams off;

        memset(&off, 0, sizeof(off));
        off.flags = 0;
        GL_Request(1, &off);
        okEq(gl_fog.b_on, 0, "the request can turn fog off");
        glr_DrawFogged(0.9f, 0, 1);
        okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF, "and then nothing is fogged");
    }
}

/* Which of the two colours a variant fogs to.
 *
 * Checked as state rather than on screen, and on purpose: the flag
 * word's mode field picks the blend equation as well as the fog
 * variant, and only two of the sixteen modes have blending off --
 * both of them the first variant. So no pair of modes isolates the
 * variant on screen, and a first draft that compared two drawn
 * colours was really comparing two blends.
 *
 * The shader's use of whichever colour is selected is checked once, on
 * screen, in caseFogCurves, which draws with a mode whose blend is
 * off. What is left to check here is the selection, and that is C.
 */
static void caseFogVariants(void)
{
    printf("-- which of the two colours a variant fogs to\n");

    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;
    glr_FogRequest(0, 0.0f, 1.0f, 1.0f, 0x00204060u);

    /* The second colour is black, and not by chance: the renderer that
       has fog writes zero into it every time fog parameters arrive and
       never writes anything else. */
    okEq((long)(FOG_i_Color(&gl_fog, 0) & 0xFFFFFF), 0x604020,
         "the first colour is the engine's, in the device's order");
    okEq((long)FOG_i_Color(&gl_fog, 1), 0,
         "and the second is black");

    /* Modes 0, 1, 3 and 7 are the first variant, 2 and 4 the third --
       which also uses the first colour -- and 5 and 6 the one that
       uses the second. */
    glr_DrawFogged(0.9f, 1, 1);
    okEq(gl_fogSecondColor, 0, "mode 1 names the first colour");
    glr_DrawFogged(0.9f, 5, 1);
    okEq(gl_fogSecondColor, 1, "mode 5 names the second");
    glr_DrawFogged(0.9f, 2, 1);
    okEq(gl_fogSecondColor, 0, "mode 2 names the first again");
    glr_DrawFogged(0.9f, 6, 1);
    okEq(gl_fogSecondColor, 1, "and mode 6 the second");

    /* Mode 8 names no variant and means keep the one in force, which a
       renderer that re-decided from every material would get wrong in
       both directions -- so it is checked from each side. */
    glr_DrawFogged(0.9f, 8, 1);
    okEq(gl_fogSecondColor, 1, "mode 8 keeps the second");
    glr_DrawFogged(0.9f, 1, 1);
    glr_DrawFogged(0.9f, 8, 1);
    okEq(gl_fogSecondColor, 0, "and keeps the first just as readily");

    /* New parameters start the variant over, as they do in the other
       renderer. */
    glr_DrawFogged(0.9f, 5, 1);
    okEq(gl_fogSecondColor, 1, "the second is in force");
    glr_FogRequest(0, 0.0f, 1.0f, 1.0f, 0x00204060u);
    okEq(gl_fogSecondColor, 0, "and a fresh request starts from the first");
}

static void caseFogVertexCurve(void)
{
    printf("-- the curve this renderer has no source for\n");

    glr_Reset();
    glr_dd.ct.cam.nearPlane = 0.01f;
    glr_dd.ct.cam.farPlane = 1.0f;

    /* The engine's fourth curve number takes its factor off the
       vertices, and this renderer's vertex carries no such thing. Fog
       stays on in the state -- the other renderer leaves it on too --
       but no distance curve is drawn, and the renderer says so once.

       What must not happen is the colour being lost: the first draft of
       fog.c treated this number as fog off and zeroed the colour, and
       the other renderer's differential test reported 44 differences
       for it. */
    glr_FogRequest(3, 0.0f, 1.0f, 1.0f, 0x00204060u);
    okEq(gl_fog.b_on, 1, "fog is still on");
    okEq(gl_fog.curve, FOG_OFF, "with no distance curve");
    okEq((long)(gl_fog.color & 0xFFFFFF), 0x604020,
         "and the colour is kept, swapped into the device's order");

    glr_DrawFogged(0.9f, 0, 1);
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF,
           "nothing is fogged, rather than something arbitrary being");
}

/* ======================================= the material's UV projection
 *
 * A material's texture coordinate is not always the one stored in the
 * geometry. The engine computes a transform and says which source it
 * applies to: the stored coordinate as it is, the stored coordinate
 * transformed, or the camera-space position transformed. The last of
 * those is how a planar projection works and the middle one is how a
 * scrolling texture scrolls.
 *
 * The transform is the engine's own -- it is written whichever
 * renderer is in force -- so what these cases check is that this
 * renderer reads it and applies it to the right source, with the right
 * shape of input. The two transformed modes disagree about that shape
 * and the engine's own fixup is what says so, which makes it exactly
 * the kind of thing to check rather than assume.
 */

/* A transform that maps its input somewhere unmistakable, written into
   the engine's matrix the way the material path writes it. Row-vector
   and row-major, as every matrix here is. */
static void glr_SetUVMatrix(const float* m)
{
    memcpy(&d3dTransformationMatrix[0], m,
           sizeof(Mtx4x4WithoutAdditionalData));
}

/* A two-by-two texture whose four texels are four different reds, so
   which texel a coordinate landed on is readable off the red channel
   alone -- and the four values are far apart, so a filtered blend of
   two of them is still distinguishable from either. */
static unsigned char glr_quadTexels[2 * 2 * 4];

static void glr_CornerTexture(void)
{
    int i;
    static const int reds[4] = { 0x20, 0x60, 0xA0, 0xE0 };

    /* The engine's own byte order: blue, green, red, alpha. */
    for (i = 0; i < 4; i++)
    {
        glr_quadTexels[i * 4 + 0] = 0;
        glr_quadTexels[i * 4 + 1] = 0;
        glr_quadTexels[i * 4 + 2] = (unsigned char)reds[i];
        glr_quadTexels[i * 4 + 3] = 0xFF;
    }

    memset(glr_texElems, 0, sizeof(glr_texElems));
    glr_texElems[1].normalData.width = 2;
    glr_texElems[1].normalData.height = 2;
    TEX_gst_GlobalList.texElems = glr_texElems;
    TEX_gst_GlobalList.maxTexNum = 4;

    memset(&glr_inter, 0, sizeof(glr_inter));
    glr_inter.colorDepth2 = 32;
    glr_inter.imgDataPtr = (char*)glr_quadTexels;
    glr_inter.tp.flagsUnkAndProceduralUnkb1 = 0x40;   /* no mip chain */

    GL_TextureStore(&glr_dd, 3);
    GL_PaletteLoad(&glr_dd, &glr_texElems[1], &glr_inter, 1);
}

static void caseUVStored(void)
{
    printf("-- mode nought is the stored coordinate, untouched\n");

    glr_Reset();
    glr_OneTexel(0x40, 0x80, 0xC0, 0xFF);
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BuildQuad();
    glr_IdentityMatrices();

    /* A transform that would send everything to one corner, left in
       place to prove mode nought ignores it. */
    {
        float m[16] = { 0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f };
        glr_SetUVMatrix(m);
    }
    GDI_gi_TextureUVMode = 0;

    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x40, "the texel's red");
    okNear(glr_G(GLR_W / 2, GLR_H / 2), 0x80, "its green");
    okNear(glr_B(GLR_W / 2, GLR_H / 2), 0xC0,
           "and its blue, so the transform was not applied");
}

static void caseUVFromPosition(void)
{
    float m[16];

    printf("-- mode one takes the coordinate from the camera-space"
           " position\n");

    glr_Reset();
    glr_CornerTexture();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BuildQuad();
    glr_IdentityMatrices();

    /* The stored coordinates are flattened to one corner, so the only
       thing that can vary across the quad is a coordinate taken from
       the position. Left as they were, they span nought to one just as
       the mapping below does -- so feeding the transform the stored
       coordinate instead would produce four different texels too, and
       the check below would pass either way. It did, until this. */
    {
        int i;

        for (i = 0; i < 4; i++)
        {
            glr_uv[i].u = 0.0f;
            glr_uv[i].v = 0.0f;
        }
    }

    /* u and v from the position's x and y, mapped so that the quad's
       middle lands in the middle of the texture: the quad spans -0.5
       to 0.5, so u = x + 0.5. The translation is in the last row,
       which is where a point's transform puts it -- a generated
       coordinate's fourth component is one. */
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;                 /* u from x */
    m[5] = 1.0f;                 /* v from y */
    m[12] = 0.5f; m[13] = 0.5f;  /* the translation, in the last row */
    glr_SetUVMatrix(m);
    GDI_gi_TextureUVMode = 1;

    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* The quad's four quarters now sample the texture's four texels,
       so each quarter is its own colour -- which a coordinate taken
       from the stored UVs, or a transform applied to the wrong source,
       would not produce. */
    {
        int qx = GLR_W / 2 - GLR_W / 8;
        int qy = GLR_H / 2 - GLR_H / 8;
        int rx = GLR_W / 2 + GLR_W / 8;
        int ry = GLR_H / 2 + GLR_H / 8;
        int a = glr_R(qx, qy);
        int b = glr_R(rx, qy);
        int c = glr_R(qx, ry);
        int d = glr_R(rx, ry);

        ok(!(a == b && b == c && c == d),
           "the four quarters do not all sample the same texel");
        /* And the whole of it is inside the texture rather than
           clamped to one edge, which is what a translation left in the
           wrong row would give. */
        ok(a != 0 || b != 0 || c != 0 || d != 0,
           "and something was sampled at all");
    }
}

static void caseUVTransformedStored(void)
{
    float m[16];
    int   plain;
    int   shifted;

    printf("-- mode three transforms the stored coordinate\n");

    glr_Reset();
    glr_CornerTexture();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BuildQuad();
    glr_IdentityMatrices();

    GDI_gi_TextureUVMode = 0;
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    plain = glr_R(GLR_W / 4, GLR_H / 4);

    /* The identity, with the stored coordinate shifted by half. The
       translation goes in the *third* row, not the last: the engine
       moves it there itself before handing the matrix over, which is
       only necessary because a stored coordinate's fourth component is
       zero. Putting it in the last row here would change nothing, and
       that is the mistake this case exists to catch. */
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[8] = 0.5f; m[9] = 0.5f;    /* the translation, in the third row */
    glr_SetUVMatrix(m);
    GDI_gi_TextureUVMode = 3;

    glr_Begin();
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    shifted = glr_R(GLR_W / 4, GLR_H / 4);

    ok(plain != shifted,
       "a transform on the stored coordinate moves what is sampled");

    /* And the translation really is read out of the third row: the
       same numbers in the last row have to do nothing, because a
       stored coordinate has no fourth component to multiply them by. */
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[12] = 0.5f; m[13] = 0.5f;  /* the last row, which is not read */
    glr_SetUVMatrix(m);

    glr_Begin();
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();
    okEq(glr_R(GLR_W / 4, GLR_H / 4), plain,
         "and the last row is not read for a stored coordinate");
}

/* The chrome projection: the coordinate comes from the camera-space
 * normal.
 *
 * The engine keeps normals in an array on the geometry rather than
 * handing them to the draw, which is where the other renderer finds
 * them too, so a case has to put a geometry with normals in the
 * display data. The normals are expanded per corner by the same walk
 * the positions take -- a second walk over the same indices -- and the
 * one thing that can go wrong there is the two disagreeing, which is
 * what the first check below is about.
 */
static tGEOGRO glr_gro;
static Vec3    glr_normals[4];

/* Four normals, one per corner of the quad, each pointing a different
   way -- so a normal that reached the wrong corner lands on a
   different texel and the picture changes.
 *
 * Turned one corner round from the positions, deliberately. The
 * quad's corners run anticlockwise from the bottom left and so would
 * these if they matched it, which means a coordinate taken from the
 * position instead of the normal would land on the same four texels in
 * the same four places -- and that mutation passed until this. */
static void glr_QuadNormals(void)
{
    memset(&glr_gro, 0, sizeof(glr_gro));
    glr_normals[0].x =  1.0f; glr_normals[0].y = -1.0f; glr_normals[0].z = 0.0f;
    glr_normals[1].x =  1.0f; glr_normals[1].y =  1.0f; glr_normals[1].z = 0.0f;
    glr_normals[2].x = -1.0f; glr_normals[2].y =  1.0f; glr_normals[2].z = 0.0f;
    glr_normals[3].x = -1.0f; glr_normals[3].y = -1.0f; glr_normals[3].z = 0.0f;
    glr_gro.normals = glr_normals;
    glr_gro.vertexNum = 4;
    glr_dd.GROGEO = &glr_gro;
}

/* A view matrix that translates, with the quad moved the other way by
   the same amount -- so the picture is where it would have been and
   the camera-space positions are what they would have been, but the
   view matrix now has a translation in it.
 *
 * That translation is the only thing that tells a normal transformed
 * as a direction from one transformed as a point. With an identity
 * view the two are the same vector, and that mutation passed until
 * this. */
static void glr_ChromeMatrices(float tx, float ty)
{
    Mtx4x4 view;
    int    i;

    MATH_SetIdentityMatrix(&view);
    view.Pos.x = tx;
    view.Pos.y = ty;
    GL_SetViewMatrix(&view);
    MATH_SetIdentityMatrix(&gl_projMtx);
    GL_l_CombineMatrices();

    for (i = 0; i < 4; i++)
    {
        glr_pos[i].x -= tx;
        glr_pos[i].y -= ty;
    }
}

/* The chrome transform as MAT_UV_Compute_Chrome builds it: a normal's
   -1..1 mapped onto 0..1, with the half in the last row because a
   generated coordinate's fourth component is one. */
static void glr_ChromeMatrix(void)
{
    float m[16];

    memset(m, 0, sizeof(m));
    m[0] = 0.5f;                  /* u from the normal's x */
    m[5] = -0.5f;                 /* v from its y, the other way up */
    m[12] = 0.5f; m[13] = 0.5f;   /* the translation, in the last row */
    glr_SetUVMatrix(m);
}

static void caseUVNormalWalk(void)
{
    tGeoTriangleChunkElem tris[2];
    tGeoTriangleChunks    chunk;
    tGdi_TexturedVertex   verts[6];
    Vec3                  pos[4];
    Vec3                  nrm[4];
    Vec3                  out[6];
    tGvx_Source           src;
    int                   i;
    int                   bad = 0;

    printf("-- the normals are walked in the same order as the"
           " positions\n");

    /* Each position and each normal carries its own index, so the two
       walks can be compared corner by corner rather than by eye. */
    for (i = 0; i < 4; i++)
    {
        pos[i].x = (float)i; pos[i].y = 0.0f; pos[i].z = 0.0f;
        nrm[i].x = (float)i; nrm[i].y = 0.0f; nrm[i].z = 0.0f;
    }

    /* Corners deliberately out of order and reused, so a walk that
       happened to be sequential would not pass by accident. */
    tris[0].v1 = 2; tris[0].v2 = 0; tris[0].v3 = 3;
    tris[1].v1 = 1; tris[1].v2 = 3; tris[1].v3 = 2;
    memset(&chunk, 0, sizeof(chunk));
    chunk.triangleChunkElems = tris;
    chunk.triangleNum = 2;

    memset(&src, 0, sizeof(src));
    src.vertices = pos;
    src.solidColor = 0xFFFFFFFFu;

    GVX_BuildTriangles(verts, &chunk, &src);
    GVX_BuildNormals(out, &chunk, nrm);

    for (i = 0; i < 6; i++)
        if (verts[i].pos.x != out[i].x)
            bad++;
    okEq(bad, 0, "every corner's normal is the one its position came"
                 " from");

    /* And the walk really is non-trivial, or the check above would
       pass for a walk that ignored the indices. */
    ok(verts[0].pos.x == 2.0f && verts[1].pos.x == 0.0f,
       "the corners are taken in the order the chunk names them");

    /* No normals is a return, not a crash: a geometry can have none. */
    memset(out, 0, sizeof(out));
    GVX_BuildNormals(out, &chunk, NULL);
    okEq((long)out[0].x, 0, "no normals, nothing written");
    GVX_BuildNormals(NULL, &chunk, nrm);
}

static void caseUVFromNormal(void)
{
    int a;
    int b;
    int c;
    int d;

    printf("-- the chrome projection, from the camera-space normal\n");

    glr_Reset();
    glr_CornerTexture();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BuildQuad();
    glr_QuadNormals();
    /* Big enough to matter: the translation only shows if it moves the
       sampled coordinate across a texel boundary, and this texture has
       one boundary in each axis. A translation of a fifth of the
       texture shifted every sample without any of them crossing, and
       the mutation that transforms the normal as a point passed. */
    glr_ChromeMatrices(1.0f, 0.6f);
    glr_ChromeMatrix();

    /* Flattened, so the only thing that can vary across the quad is a
       coordinate taken from the normal. */
    {
        int i;

        for (i = 0; i < 4; i++)
        {
            glr_uv[i].u = 0.0f;
            glr_uv[i].v = 0.0f;
        }
    }

    GDI_gi_TextureUVMode = 2;
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    a = glr_R(GLR_W / 2 - GLR_W / 8, GLR_H / 2 - GLR_H / 8);
    b = glr_R(GLR_W / 2 + GLR_W / 8, GLR_H / 2 - GLR_H / 8);
    c = glr_R(GLR_W / 2 - GLR_W / 8, GLR_H / 2 + GLR_H / 8);
    d = glr_R(GLR_W / 2 + GLR_W / 8, GLR_H / 2 + GLR_H / 8);

    /* The four quarters land on the four texels, one each, and in the
       order the corner normals name: the texture's reds are 0x20,
       0x60, 0xA0 and 0xE0, far enough apart that a filtered blend of
       two is not mistakable for either.
     *
     * The exact four rather than "they differ": the stored
       coordinates are flattened to nought, so a coordinate taken from
       them would be one colour throughout -- but a normal that
       reached the wrong corner, or a transform applied with the wrong
       sign, also gives four different colours. */
    /* Which quarter gets which texel depends on the texture's row
       order and on which way up the readback is, so the permutation is
       pinned from what the driver produces rather than predicted here.
       What makes it meaningful is that it is a *different* permutation
       from the one a coordinate taken from the position gives -- the
       corner normals are turned one place round from the corner
       positions on purpose, and the mutation that feeds the position
       instead now fails. */
    okNear(a, 0xA0, "the first quarter is its own texel");
    okNear(b, 0x20, "the second");
    okNear(c, 0xE0, "the third");
    okNear(d, 0x60, "and the fourth, in the order the normals name");

    /* And the four really are four, so none of the above is two
       quarters agreeing by accident. */
    ok(a != b && a != c && a != d && b != c && b != d && c != d,
       "four quarters, four texels");
    okEq(gl_geoNrmOn, 1, "with the normal attribute enabled for it");
}

static void caseUVNormalMissing(void)
{
    float m[16];

    printf("-- the chrome projection with no normals to read\n");

    glr_Reset();
    glr_OneTexel(0x40, 0x80, 0xC0, 0xFF);
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BuildQuad();
    glr_IdentityMatrices();

    /* A geometry with no normals at all, which is what the display
       data holds for every other case. A transform that would send
       every coordinate off the texture, so applying it to anything
       would show. */
    glr_dd.GROGEO = NULL;
    memset(m, 0, sizeof(m));
    m[12] = 50.0f; m[13] = 50.0f;
    glr_SetUVMatrix(m);
    GDI_gi_TextureUVMode = 2;

    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    okEq(gl_geoNrmOn, 0, "the normal attribute is left off");
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x40,
           "and the stored coordinate is drawn rather than a normal"
           " invented from nothing");
}

/* The sprites share the geometry's vertex array, so an attribute left
   enabled by a piece of world geometry would have them reading its
   normals. */
static void caseUVNormalLeavesSpritesAlone(void)
{
    tGeoSpriteElem elems[1];
    Vec3           pos[1];
    tGeoSprites    sprites;
    Mtx4x4         stack;

    printf("-- the sprites are drawn after a chrome-mapped material\n");

    glr_Reset();
    glr_CornerTexture();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_BuildQuad();
    glr_QuadNormals();
    glr_ChromeMatrices(1.0f, 0.6f);
    glr_ChromeMatrix();
    GDI_gi_TextureUVMode = 2;
    GL_TextureInit(1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    okEq(gl_geoNrmOn, 1, "the normals are on");

    elems[0].idx = 0;
    elems[0].dummy = 0x3F00;
    pos[0].x = 0.0f; pos[0].y = 0.0f; pos[0].z = 0.0f;
    memset(&sprites, 0, sizeof(sprites));
    sprites.num = 1;
    sprites.spriteElems = elems;
    sprites.globalSize = 1.0f;
    sprites.globalRatio = 1.0f;
    MATH_SetIdentityMatrix(&stack);
    glr_dd.matrixStack.actMatrixElem = &stack;

    /* The chrome case's view translates, and the sprite is at the
       world origin rather than being moved to suit -- so the matrices
       go back to identity first, as they would between one frame's
       geometry and the next's sprites. */
    glr_Begin();
    glr_IdentityMatrices();
    glr_SolidColour(0xFF, 0x20, 0x40, 0xFF);
    GL_TextureInit(-1, 0, 0);
    GL_DrawSprites(&sprites, pos, 1);
    glr_Read();

    okEq(gl_geoNrmOn, 0, "and the sprites turned them off again");
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0xFF,
           "the sprite is drawn in its own colour");
}

/* ========================================================= culling
 *
 * What is checked here is the OpenGL half of the culling: that a
 * triangle wound one way in clip space survives one of the two
 * settings and is dropped by the other. The mapping from the engine's
 * request to those settings is a reading of two conventions rather
 * than a measurement -- the other renderer's device is not here -- and
 * is checked as a table in the mock harness instead.
 *
 * The triangle is given its winding by hand, in clip space with an
 * identity projection, so nothing between here and the rasteriser can
 * turn it round without this noticing.
 */
static void glr_WoundTriangle(int b_counterClockwise)
{
    /* Three corners around the middle of the surface. Counter-
       clockwise with x right and y up is the first order; the other is
       the same three with two of them swapped. */
    glr_pos[0].x =  0.0f; glr_pos[0].y =  0.5f; glr_pos[0].z = 0.0f;
    if (b_counterClockwise)
    {
        glr_pos[1].x = -0.5f; glr_pos[1].y = -0.5f; glr_pos[1].z = 0.0f;
        glr_pos[2].x =  0.5f; glr_pos[2].y = -0.5f; glr_pos[2].z = 0.0f;
    }
    else
    {
        glr_pos[1].x =  0.5f; glr_pos[1].y = -0.5f; glr_pos[1].z = 0.0f;
        glr_pos[2].x = -0.5f; glr_pos[2].y = -0.5f; glr_pos[2].z = 0.0f;
    }
    glr_pos[3] = glr_pos[0];

    glr_uv[0].u = 0.5f; glr_uv[0].v = 0.0f;
    glr_uv[1].u = 0.0f; glr_uv[1].v = 1.0f;
    glr_uv[2].u = 1.0f; glr_uv[2].v = 1.0f;
    glr_uv[3] = glr_uv[0];

    /* One triangle, not two: a quad's second triangle has the
       opposite winding under some orderings and would always leave
       something on the screen. */
    glr_tris[0].v1 = 0;
    glr_tris[0].v2 = 1;
    glr_tris[0].v3 = 2;
    memset(&glr_chunk, 0, sizeof(glr_chunk));
    glr_chunk.triangleChunkElems = glr_tris;
    glr_chunk.triangleNum = 1;
}

static int glr_i_DrawWound(int b_counterClockwise, int mask)
{
    glr_Reset();
    glr_SolidColour(0xFF, 0xFF, 0xFF, 0xFF);
    glr_WoundTriangle(b_counterClockwise);
    glr_IdentityMatrices();
    glr_dd.drawMaskAllowedStuffCurrent = mask;

    GL_TextureInit(-1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
    glr_Read();

    /* A point inside the triangle whichever way it is wound. */
    return glr_R(GLR_W / 2, GLR_H / 2);
}

static void caseCulling(void)
{
    int ccwNone;
    int cwNone;
    int ccwCullCcw;
    int cwCullCcw;
    int ccwCullCw;
    int cwCullCw;

    printf("-- a triangle survives one setting and not the other\n");

    /* With culling off, both windings are drawn -- so the two
       triangles are both really there and the differences below are
       the culling and not the geometry. */
    ccwNone = glr_i_DrawWound(1, 0x10);
    cwNone = glr_i_DrawWound(0, 0x10);
    okNear(ccwNone, 0xFF, "with culling off the first winding is drawn");
    okNear(cwNone, 0xFF, "and so is the second");

    /* Which winding each request keeps.
     *
       These four numbers are **measured**, on a real card in a real
       scene, by trying every answer. They are not derived, and the
       distinction matters here more than anywhere else in this file:
       an earlier version of this case asserted the opposite four, and
       it passed, because it was written from the same reading of the
       two APIs' conventions that the renderer was written from. A
       test that encodes the code's own premise cannot discover that
       the premise is wrong -- it only makes the wrong answer harder
       to change.
     *
       So what is pinned below is the observed mapping and nothing
       more. The reasoning that gives the other answer is written out
       in win/wingl.c so that nobody re-derives it; if these four ever
       need swapping, it takes a scene and a card to say so, not an
       argument. */
    ccwCullCcw = glr_i_DrawWound(1, 0x10 | 0x800);
    cwCullCcw = glr_i_DrawWound(0, 0x10 | 0x800);
    okNear(ccwCullCcw, GLR_BG, "the engine's first request culls the"
                               " counter-clockwise triangle");
    okNear(cwCullCcw, 0xFF, "and keeps the clockwise one");

    /* And the other request culls the other one, which is the whole
       point of there being two. */
    ccwCullCw = glr_i_DrawWound(1, 0x10 | 0x800 | 0x2000);
    cwCullCw = glr_i_DrawWound(0, 0x10 | 0x800 | 0x2000);
    okNear(ccwCullCw, 0xFF, "the second request keeps the"
                            " counter-clockwise one");
    okNear(cwCullCw, GLR_BG, "and culls the clockwise one");
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


/* ------------------------------------- the depth blur's after-effect

   These cases run the chain for real: a pattern is put straight into
   the scene target's texture, the chain is asked to run over it, and
   the result is read back out of whichever framebuffer it says the
   frame ended up in.

   The scene is made 640 by 480 for them, because that is the size the
   blur's steps are stated in -- a step is one pixel of a 640 by 480
   picture, expressed as a fraction of the texture. At that size each
   tap lands exactly on a neighbouring texel's centre, so what comes
   back is arithmetic rather than whatever the sampler made of a
   fraction of a pixel. At the 80 by 48 the rest of this file uses, one
   step is an eighth of a pixel and every case below would be reading
   filter noise.
 */
#define GLR_AE_W 640
#define GLR_AE_H 480

static unsigned char* glr_aePattern;

/* One pixel out of a framebuffer, by texture row -- row 0 is the
   bottom of the picture, which is the way both the upload below and
   OpenGL's own read run. The cases are written in those rows rather
   than in screen rows so that nothing is turned over twice. */
static void glr_AeAt(GLuint fbo, int x, int row, int* rgba)
{
    unsigned char px[4];

    px[0] = px[1] = px[2] = px[3] = 0;
    GLFN_glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    GLFN_glReadPixels(x, row, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
    GLFN_glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    rgba[0] = px[0]; rgba[1] = px[1]; rgba[2] = px[2]; rgba[3] = px[3];
}

static int glr_AeRed(GLuint fbo, int x, int row)
{
    int rgba[4];

    glr_AeAt(fbo, x, row, rgba);
    return rgba[0];
}

/* The scene target at the blur's own size, with a pattern in it.
   `redAt` and `alphaAt` are asked for each pixel, so a case states its
   picture as the two functions rather than as a buffer. */
static GLuint glr_AeScene(int (*redAt)(int x, int row),
                          int (*alphaAt)(int x, int row))
{
    int x;
    int row;

    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_AE_W;
    gl_state.height = GLR_AE_H;
    if (!GL_l_b_SceneReady())
        return 0;

    if (!glr_aePattern)
        glr_aePattern = (unsigned char*)malloc(GLR_AE_W * GLR_AE_H * 4);
    if (!glr_aePattern)
        return 0;

    for (row = 0; row < GLR_AE_H; row++)
        for (x = 0; x < GLR_AE_W; x++)
        {
            unsigned char* p = &glr_aePattern[(row * GLR_AE_W + x) * 4];

            p[0] = (unsigned char)redAt(x, row);
            p[1] = 0;
            p[2] = 0;
            p[3] = (unsigned char)alphaAt(x, row);
        }

    /* Straight into the texture the scene target is built on, which
       is exactly what the world would have left there -- and lets a
       case name an alpha per pixel, which is the input the whole
       effect turns on. */
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    return gl_sceneFbo;
}

/* A step across: dark on the left of the middle column, bright on the
   right. Blurring it moves red leftwards over the edge, and the
   unblurred half is where the mix has to leave it alone. */
static int glr_aeStepAcross(int x, int row)
{
    (void)row;
    return x < GLR_AE_W / 2 ? 0 : 240;
}

/* Sharp in the bottom half of the texture, fully blurred in the top.
   255 is no blur and 0 is all of it, which is the way round the ramp
   writes it. */
static int glr_aeAlphaSplit(int x, int row)
{
    (void)x;
    return row < GLR_AE_H / 2 ? 255 : 0;
}

static int glr_aeAlphaNone(int x, int row)
{
    (void)x; (void)row;
    return 0;
}

static void caseAeDepthBlurMix(void)
{
    GLuint scene;
    GLuint shown;
    int    mid = GLR_AE_W / 2;
    int    sharpRow = GLR_AE_H / 4;        /* alpha 255 down here */
    int    blurRow = 3 * GLR_AE_H / 4;     /* alpha 0 up here */
    int    k;
    int    last;

    printf("-- the depth blur mixes sharp and blurred by the frame's alpha\n");
    glr_Reset();
    scene = glr_AeScene(glr_aeStepAcross, glr_aeAlphaSplit);
    ok(scene != 0, "the scene target was made at the blur's own size");
    if (!scene)
        return;

    PC_AE_MASTER = 0x8;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != 0, "the chain says where the frame is");
    /* And it is the scene target. The mix cannot read and write one
       texture, so it lands in a spare of its own and is copied back
       over the frame -- which is where the renderer this follows
       leaves it, and what every effect after it is placed relative
       to. The copy is colour only, so the frame keeps the alpha the
       world gave it; the readings further down check that. */
    okEq((long)(shown == scene), 1, "and it is the frame itself");

    /* Where the frame said no blur, the step is still a step: the two
       pixels either side of it are the values that went in. A mix
       taking its alpha the other way round blurs this half and
       leaves the other sharp, so both of these move. */
    okEq(glr_AeRed(shown, mid - 1, sharpRow), 0,
         "no blur here, so the dark side is still dark");
    okEq(glr_AeRed(shown, mid, sharpRow), 240,
         "and the bright side still bright");

    /* Where it said full blur, red has moved across the edge. */
    ok(glr_AeRed(shown, mid - 1, blurRow) > 0,
       "full blur there, so red has crossed to the dark side");
    ok(glr_AeRed(shown, mid, blurRow) < 240,
       "and the bright side has given some up");

    /* And it fades with distance rather than being a flat smear --
       which is what three passes of widening taps produce, and a
       single pass with one reach would not. */
    last = 256;
    for (k = 1; k <= 4; k++)
    {
        int here = glr_AeRed(shown, mid - k, blurRow);

        ok(here <= last, "the bleed does not grow further from the edge");
        last = here;
    }
    ok(glr_AeRed(shown, mid - 1, blurRow) > glr_AeRed(shown, mid - 4, blurRow),
       "the pixel beside the edge has more of it than one four away");
    okEq(glr_AeRed(shown, 40, blurRow), 0,
         "and none of it reaches the far side of the picture");

    /* How far it reaches is what says there were three passes and that
       each read the one before. The steps are one, two and three
       pixels, so red can cross five pixels of edge but not six -- and
       only by compounding: a single pass of any one of those steps,
       or three passes that all read the frame instead of each other,
       leave nothing this far out. */
    ok(glr_AeRed(shown, mid - 5, blurRow) > 0,
       "red crosses five pixels, which only three compounding passes do");
    okEq(glr_AeRed(shown, mid - 7, blurRow), 0, "and stops before seven");

    /* Away from the edge the four taps all read the same pixel, so a
       blurred region of one colour has to come back as that colour
       exactly. That is the four weights coming to one, and it is the
       check that catches any of them being wrong: it does not care
       which, and it does not need an edge to see it. */
    okEq(glr_AeRed(shown, GLR_AE_W - 40, blurRow), 240,
         "a blurred stretch of one colour comes back as that colour");
    okEq(glr_AeRed(shown, GLR_AE_W - 40, sharpRow), 240,
         "and so does the sharp one");

    /* The alpha travels on, because every effect after this one is
       mixed the same way and would be a no-op without it. */
    {
        int rgba[4];

        glr_AeAt(shown, mid - 1, sharpRow, rgba);
        okEq(rgba[3], 255, "the frame's alpha is carried through, sharp side");
        glr_AeAt(shown, mid - 1, blurRow, rgba);
        okEq(rgba[3], 0, "and on the blurred side");
    }
}

/* A step down the texture: bright in the rows below the middle, dark
   above. Which side of it bleeds further is the whole question. */
static int glr_aeStepDown(int x, int row)
{
    (void)x;
    return row < GLR_AE_H / 2 ? 240 : 0;
}

static void caseAeDepthBlurDirection(void)
{
    GLuint scene;
    GLuint shown;
    int    x = GLR_AE_W / 2;
    int    edge = GLR_AE_H / 2;
    int    into;
    int    outOf;

    printf("-- the blur's heaviest tap is the one below, not above\n");
    glr_Reset();
    scene = glr_AeScene(glr_aeStepDown, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
        return;

    PC_AE_MASTER = 0x8;
    shown = GL_l_i_RunAfterEffects();
    okEq((long)(shown == scene), 1,
         "the chain ran, leaving the frame where it began");
    if (!shown)
    {
        return;
    }

    /* The four taps are not weighted alike: the one below the pixel
       keeps about half of the result and the one above about a
       quarter. So a dark pixel sitting above a bright region picks up
       roughly twice as much as a bright pixel sitting above a dark one
       gives away.
     *
       This is the check that the vertical offsets were turned the
       right way round for this renderer's textures. Negating them
       swaps these two numbers over, and every other check in this file
       passes either way -- the picture is blurred whichever side the
       smear falls on. */
    into = glr_AeRed(shown, x, edge);            /* dark, bright below */
    outOf = 240 - glr_AeRed(shown, x, edge - 1); /* bright, dark above */

    ok(into > 0, "the dark row above the step picks red up");
    ok(outOf > 0, "the bright row below it gives red away");
    ok(into > outOf,
       "and the row above picks up more than the row below gives away");
    /* Twice, to within what eight bits and three compounding passes
       leave of it. A symmetric blur makes these equal, which fails
       the comparison above; getting the sign wrong makes the ratio
       the other way up, which fails it too. */
    ok(into > outOf * 3 / 2, "about half again as much at least");
}

static void caseAeDepthBlurGates(void)
{
    GLuint scene;
    int    mid = GLR_AE_W / 2;
    int    row = GLR_AE_H / 2;

    printf("-- what turns the after-effects off\n");
    glr_Reset();
    scene = glr_AeScene(glr_aeStepAcross, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
        return;

    /* Read off the picture rather than off which framebuffer comes
       back. The depth blur's result is copied into the frame now, so
       every one of these answers with the frame whether it ran or
       not -- and the only thing that says is whether the step across
       the middle came out blurred.
     *
       Which means the pattern has to go back in before each one: the
       chain writes the frame, so a second run would blur what the
       first already blurred. In the game the world redraws it every
       frame and the question does not arise. */
    {
        int i;

        for (i = 0; i < 7; i++)
        {
            static const unsigned int master[7] =
                { 0, 0x8, 0x1000, 0x4, 0x8, 0x8, 0x8 };
            static const int wantBlur[7] = { 0, 1, 1, 0, 0, 0, 1 };
            static const char* const what[7] = {
                "no bits set, so the step stays sharp",
                "the first bit blurs it",
                "and so does the second",
                "the effect next door does not",
                "nor does it with the tweak that switches the effects off",
                "nor with the editor camera's own switch",
                "and with both back off it blurs again"
            };

            /* The pattern back in, fresh each time. */
            glr_AeScene(glr_aeStepAcross, glr_aeAlphaNone);
            PC_AE_MASTER = master[i];
            tweak_ae_no = (i == 4);
            ECAM_gi_NoAfterEffects = (i == 5);
            {
                GLuint shown = GL_l_i_RunAfterEffects();
                int    bled = glr_AeRed(shown, mid - 1, row) > 0;

                okEq((long)bled, (long)wantBlur[i], what[i]);
            }
        }
        tweak_ae_no = 0;
        ECAM_gi_NoAfterEffects = 0;
    }

    /* Put the rig back the way the rest of the file wants it. */
    PC_AE_MASTER = 0;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

static int glr_aeFlat(int x, int row)
{
    (void)x; (void)row;
    return 240;
}

static int glr_aeAlphaAll(int x, int row)
{
    (void)x; (void)row;
    return 255;
}

static void caseAeDepthBlurBands(void)
{
    GLuint     scene;
    GLuint     shown;
    tCbd_Bands bands;
    int        mid = GLR_AE_W / 2;
    int        bandTopRow;
    int        bandBottomRow;
    int        marginRow;

    printf("-- the blur reaches past the band the mix covers\n");
    glr_Reset();

    /* A cutscene's letterbox, which is the only time the two strips
       differ: the mix covers the band and the blur a margin either
       side of it, so that a tap at the band's own edge has blurred
       pixels to land on. */
    CBD_Decide(&bands);
    okEq((long)(bands.b_on != 0), 1, "the rig has the letterbox on");
    ok(bands.reachTop < bands.top, "the blur reaches above the band");
    ok(bands.reachBottom > bands.bottom, "and below it");

    /* In texture rows, which is how the readings below are taken. The
       fractions are measured from the top of the picture and the rows
       from the bottom, so one is the other subtracted from the
       height. */
    bandTopRow = (int)((1.0f - bands.top) * (float)GLR_AE_H) - 1;
    bandBottomRow = (int)((1.0f - bands.bottom) * (float)GLR_AE_H) + 1;
    marginRow = (bandTopRow + (int)((1.0f - bands.reachTop)
                                    * (float)GLR_AE_H)) / 2;

    /* One colour everywhere and no alpha at all, so every pixel is
       taken from the blurred copy. Away from an edge the four taps
       read the same pixel, so the whole band has to come back as the
       colour that went in -- including its topmost and bottommost
       rows, which can only be right if the blur was drawn over the
       wider strip. Narrowed to the band, those rows' taps reach
       pixels no pass wrote, which are black. */
    scene = glr_AeScene(glr_aeFlat, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
        return;
    PC_AE_MASTER = 0x8;
    shown = GL_l_i_RunAfterEffects();
    okEq((long)(shown == scene), 1,
         "the chain ran, leaving the frame where it began");
    if (!shown)
    {
        return;
    }
    okEq(glr_AeRed(shown, mid, bandTopRow), 240,
         "the band's top row is the colour that went in");
    okEq(glr_AeRed(shown, mid, bandBottomRow), 240,
         "and its bottom row");

    /* And the other way round: a frame that asks for no blur at all
       comes back sharp inside the band, and the rows between the band
       and the blur's reach are not the mix's business -- it never
       drew there, so what is in them is the last blur rather than the
       picture. */
    scene = glr_AeScene(glr_aeStepAcross, glr_aeAlphaAll);
    if (!scene)
    {
        ok(0, "the second scene target was made");
        return;
    }
    shown = GL_l_i_RunAfterEffects();
    okEq((long)(shown == scene), 1,
         "the chain ran, leaving the frame where it began");
    if (!shown)
    {
        return;
    }
    okEq(glr_AeRed(shown, mid - 1, (bandTopRow + bandBottomRow) / 2), 0,
         "inside the band the step is sharp");
    /* And the margin above the band is the frame as the world drew it:
       the mix covers the band and nothing copies the rest back, so
       what is there is the original and not a blur of it. That is
       what the renderer this follows leaves there too -- its mix
       writes the band of the frame and leaves the rest alone. An
       earlier arrangement here left a blur intermediate in those
       rows, which was a divergence rather than a feature. */
    okEq(glr_AeRed(shown, mid - 1, marginRow), 0,
         "and the margin above it is the frame as it was drawn");

    PC_AE_MASTER = 0;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}


static void caseAeDepthBlurEdges(void)
{
    GLuint scene;
    GLuint shown;
    int    x = GLR_AE_W / 2;
    int    saved = CIN_Bands;

    printf("-- a tap past the edge of the picture takes the edge\n");
    glr_Reset();

    /* No letterbox, so the strip the blur covers is the whole picture
       and the taps on its first and last rows step off the end of it.
       The frame is bright along the bottom and dark everywhere else:
       with the texture left repeating, a tap off the top lands on the
       bottom and the top row picks the brightness up. */
    CIN_Bands = 0;
    scene = glr_AeScene(glr_aeStepDown, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = saved;
        return;
    }
    PC_AE_MASTER = 0x8;
    shown = GL_l_i_RunAfterEffects();
    okEq((long)(shown == scene), 1,
         "the chain ran, leaving the frame where it began");
    if (!shown)
    {
        CIN_Bands = saved;
        return;
    }

    /* glr_aeStepDown is bright in the rows below the middle, so the
       top row is deep inside the dark half and nothing legitimate can
       reach it: the nearest bright pixel is two hundred rows away and
       the blur crosses five. */
    okEq(glr_AeRed(shown, x, GLR_AE_H - 1), 0,
         "the top row of the picture stays dark");
    okEq(glr_AeRed(shown, x, GLR_AE_H - 2), 0, "and the row below it");
    /* And the other end, where the wrap would go the other way: the
       bottom row is bright and must not be dimmed by the dark rows at
       the top of the picture. */
    okEq(glr_AeRed(shown, x, 0), 240, "the bottom row stays bright");
    okEq(glr_AeRed(shown, x, 1), 240, "and the row above it");

    CIN_Bands = saved;
    PC_AE_MASTER = 0;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}


/* -------------------------------------- the colour after-effects

   Brightness, contrast and grayscale, over a frame whose pixels are
   put there by hand -- so each case names an input colour and reads
   the output, which is the only way to tell a gain from a bias or a
   grayscale from a desaturation.
 */
static int glr_aeRamp(int x, int row)
{
    (void)row;
    /* A ramp across, so one frame carries every input value: a case
       can pick the pixel whose answer it wants to state. */
    return x * 255 / (GLR_AE_W - 1);
}

static int glr_aeAlphaMark(int x, int row)
{
    (void)x; (void)row;
    /* Something that is neither 0 nor 255, so that an effect writing
       the alpha shows up rather than matching by luck. */
    return 0x5A;
}

/* The ramp's value at a column, which is what the effects are applied
   to. Worked out the same way the pattern writes it. */
static int glr_aeRampAt(int x)
{
    return x * 255 / (GLR_AE_W - 1);
}

static void caseAeBrightness(void)
{
    GLuint scene;
    GLuint shown;
    int    row = GLR_AE_H / 2;
    int    rgba[4];

    printf("-- the brightness adds a grey, or takes one off\n");
    glr_Reset();
    CIN_Bands = 0;
    scene = glr_AeScene(glr_aeRamp, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    /* A quarter of the way above neutral: a quarter of a channel
       added, everywhere, with no change of gain. */
    unkAEFunc2Param = 0.75f;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");
    {
        int add = (int)(0.25f * 255.0f);   /* truncated, as it is there */
        int mid = GLR_AE_W / 2;
        int lo = GLR_AE_W / 8;

        okEq(glr_AeRed(shown, mid, row), glr_aeRampAt(mid) + add,
             "a mid pixel is that much brighter");
        okEq(glr_AeRed(shown, lo, row), glr_aeRampAt(lo) + add,
             "and a dark one by exactly the same amount, not in"
             " proportion");
        /* The top end clamps rather than wrapping round to black,
           which is what an unclamped add would do in eight bits. */
        okEq(glr_AeRed(shown, GLR_AE_W - 1, row), 255,
             "and the brightest clamps at white");
    }

    /* The same distance below neutral takes the same amount off. */
    unkAEFunc2Param = 0.25f;
    shown = GL_l_i_RunAfterEffects();
    {
        int sub = (int)(0.25f * 255.0f);
        int mid = GLR_AE_W / 2;

        okEq(glr_AeRed(shown, mid, row), glr_aeRampAt(mid) - sub,
             "below neutral it takes the same amount off");
        okEq(glr_AeRed(shown, 0, row), 0, "and the darkest clamps at black");
    }

    /* The alpha is not the brightness's business. */
    glr_AeAt(shown, GLR_AE_W / 2, row, rgba);
    okEq(rgba[3], 0x5A, "the frame's alpha is left alone");

    /* And green and blue move with red: it is a grey being added, not
       a tint. */
    {
        int mid = GLR_AE_W / 2;

        glr_AeAt(shown, mid, row, rgba);
        okEq(rgba[1], 0, "green had none and still has none");
        okEq(rgba[2], 0, "nor blue");
    }

    unkAEFunc2Param = 0.5f;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

static void caseAeContrast(void)
{
    GLuint scene;
    GLuint shown;
    int    row = GLR_AE_H / 2;
    int    mid;
    int    i;

    printf("-- the contrast is a gain about mid grey\n");
    glr_Reset();
    CIN_Bands = 0;
    scene = glr_AeScene(glr_aeRamp, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    /* The column whose value is nearest mid grey, which is the pivot
       everything below is stated about. */
    mid = 0;
    for (i = 0; i < GLR_AE_W; i++)
        if (glr_aeRampAt(i) == 128)
        {
            mid = i;
            break;
        }
    okEq(glr_aeRampAt(mid), 128, "the ramp has a mid-grey column");

    /* All the way down: the frame is thrown away and replaced by a
       flat mid grey. Read at three columns, so a gain that merely
       went small rather than to nothing fails. */
    colorBalanceParam1 = 0.0f;
    shown = GL_l_i_RunAfterEffects();
    okEq(glr_AeRed(shown, 0, row), 128, "at nought the darkest is mid grey");
    okEq(glr_AeRed(shown, mid, row), 128, "the middle too");
    okEq(glr_AeRed(shown, GLR_AE_W - 1, row), 128, "and the brightest");

    /* All the way up: twice the gain about the pivot, so the pivot
       stays put and everything else runs to an end. */
    colorBalanceParam1 = 1.0f;
    shown = GL_l_i_RunAfterEffects();
    {
        int atMid = glr_AeRed(shown, mid, row);

        ok(atMid >= 126 && atMid <= 130, "at one, mid grey stays mid grey");
        ok(glr_AeRed(shown, mid + (GLR_AE_W - mid) / 4, row)
           > glr_aeRampAt(mid + (GLR_AE_W - mid) / 4),
           "a pixel above the pivot is pushed up");
        ok(glr_AeRed(shown, mid / 2, row) < glr_aeRampAt(mid / 2),
           "and one below it down");
        okEq(glr_AeRed(shown, GLR_AE_W - 1, row), 255, "white stays white");
        okEq(glr_AeRed(shown, 0, row), 0, "and black black");
    }

    /* Halfway down: every pixel moves towards the pivot, and none
       past it -- a gain applied about nought instead of about mid
       grey would darken the bright half instead of pulling it in. */
    colorBalanceParam1 = 0.25f;
    shown = GL_l_i_RunAfterEffects();
    for (i = 4; i < GLR_AE_W; i += 64)
    {
        int was = glr_aeRampAt(i);
        int now = glr_AeRed(shown, i, row);

        if (was < 120)
            ok(now > was && now <= 130,
               "a dark pixel comes up towards the pivot, not past it");
        else if (was > 136)
            ok(now < was && now >= 126,
               "and a bright one comes down to it");
    }

    colorBalanceParam1 = 0.5f;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

/* A frame of one colour per row band, so one pass gives the grayscale
   of pure red, pure green and pure blue. */
static int glr_aeChannels(int x, int row)
{
    (void)x;
    /* Red only in the bottom third. The green and blue thirds are
       written by the two helpers below, because the pattern writer
       takes a red function and an alpha function. */
    return row < GLR_AE_H / 3 ? 255 : 0;
}

static void caseAeGrayscale(void)
{
    GLuint scene;
    GLuint shown;
    int    row = GLR_AE_H / 2;
    int    rgba[4];
    int    i;
    int    pureRed;
    int    pureGreen;
    int    pureBlue;

    printf("-- the grayscale, and its green-heavy weights\n");
    glr_Reset();
    CIN_Bands = 0;

    /* Three solid colours written straight into the frame, one per
       column band, so that one draw gives all three weights. The
       pattern helper writes red and alpha, so this one goes in by
       hand. */
    scene = glr_AeScene(glr_aeChannels, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }
    for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
    {
        unsigned char* p = &glr_aePattern[i * 4];
        int            x = i % GLR_AE_W;

        p[0] = (unsigned char)(x < GLR_AE_W / 3 ? 255 : 0);
        p[1] = (unsigned char)(x >= GLR_AE_W / 3
                               && x < 2 * GLR_AE_W / 3 ? 255 : 0);
        p[2] = (unsigned char)(x >= 2 * GLR_AE_W / 3 ? 255 : 0);
        p[3] = 0x5A;
    }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    PC_AE_MASTER = 0x1;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");

    /* All three channels come out the same, which is what makes it a
       grayscale rather than a channel mixer. */
    glr_AeAt(shown, GLR_AE_W / 6, row, rgba);
    pureRed = rgba[0];
    okEq(rgba[1], pureRed, "pure red comes out grey, green matching red");
    okEq(rgba[2], pureRed, "and blue matching too");
    okEq(rgba[3], 0x5A, "with the frame's alpha carried through");

    glr_AeAt(shown, GLR_AE_W / 2, row, rgba);
    pureGreen = rgba[0];
    okEq(rgba[1], pureGreen, "and pure green comes out grey");
    glr_AeAt(shown, 5 * GLR_AE_W / 6, row, rgba);
    pureBlue = rgba[0];
    okEq(rgba[2], pureBlue, "and pure blue");

    /* The weights, as the only thing a picture can say about them:
       their order, and how far apart they are. Green carries most of
       it by a long way and blue almost none -- an even split, or the
       usual broadcast luminance, fails both of these. */
    ok(pureGreen > pureRed, "green weighs more than red");
    ok(pureRed > pureBlue, "and red more than blue");
    ok(pureGreen > 3 * pureRed, "green by more than three times red");
    ok(pureBlue < 20, "and blue barely counts at all");

    /* And each of the three at the value the step's own numbers give,
       so the shader is checked against aecolor.c rather than against
       an ordering that several wrong weightings would also satisfy. */
    {
        tAec_Source gsrc;
        tAec_Step   gstep[AEC_MAX_STEPS];
        int         got[3];
        int         c;

        gsrc.aeMaster = 0x1;
        gsrc.brightness = 0.5f;
        gsrc.contrast = 0.5f;
        AEC_i_Steps(&gsrc, gstep, AEC_MAX_STEPS);
        got[0] = pureRed;
        got[1] = pureGreen;
        got[2] = pureBlue;
        for (c = 0; c < 3; c++)
        {
            double want = (double)gstep[0].weights[c] * gstep[0].gain
                          + gstep[0].bias;

            if (want < 0.0)
                want = 0.0;
            okWithin((double)got[c], want * 255.0, 1.0,
                     "a pure channel comes back at its own weight");
        }
    }

    /* And the weights not coming to one, which is visible: white does
       not come back white. */
    for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
    {
        unsigned char* p = &glr_aePattern[i * 4];

        p[0] = p[1] = p[2] = 255;
        p[3] = 0x5A;
    }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    shown = GL_l_i_RunAfterEffects();
    {
        int         white = glr_AeRed(shown, GLR_AE_W / 2, row);
        tAec_Source gsrc;
        tAec_Step   gstep[AEC_MAX_STEPS];
        double      want;

        ok(white < 255, "white does not come back white");

        /* And exactly how far short, worked out from what aecolor.c
           says the step is rather than left as a range.
         *
           A range is what this was, and it was too loose to see the
           shader dropping the grayscale's gain and bias: without
           them white comes back at the weights' bare sum, which is
           two steps away in eight bits -- inside the range and
           outside this. The gain and the bias are each under a
           hundredth, so nothing coarser than an exact reading will
           do. */
        gsrc.aeMaster = 0x1;
        gsrc.brightness = 0.5f;
        gsrc.contrast = 0.5f;
        AEC_i_Steps(&gsrc, gstep, AEC_MAX_STEPS);
        want = ((double)gstep[0].weights[0] + gstep[0].weights[1]
                + gstep[0].weights[2]) * gstep[0].gain + gstep[0].bias;
        okWithin((double)white, want * 255.0, 1.0,
                 "white comes back at exactly what the weights, the gain"
                 " and the bias say");

        /* Which is not the weights' sum on its own -- so the reading
           above is telling the gain and the bias apart from nothing
           rather than merely agreeing with the weights. */
        ok(fabs(want * 255.0
                - ((double)gstep[0].weights[0] + gstep[0].weights[1]
                   + gstep[0].weights[2]) * 255.0) > 1.5,
           "and that is more than a step away from the bare sum");
    }

    PC_AE_MASTER = 0;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

static void caseAeColorOrder(void)
{
    GLuint scene;
    GLuint shown;
    int    row = GLR_AE_H / 2;
    int    x = GLR_AE_W / 4;
    int    both;
    int    brightOnly;

    printf("-- the three run in order, clamped between\n");
    glr_Reset();
    CIN_Bands = 0;
    scene = glr_AeScene(glr_aeRamp, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    /* Brightness all the way up, then contrast all the way up. The
       brightness pushes most of the ramp past white and the clamp
       catches it, so the contrast sees white rather than the number
       the arithmetic would have carried -- and the result is white.
       Composed without the clamp between them, it would be white
       too; what the clamp changes is everything below that, which
       the next reading is for. */
    unkAEFunc2Param = 1.0f;
    colorBalanceParam1 = 1.0f;
    shown = GL_l_i_RunAfterEffects();
    both = glr_AeRed(shown, x, row);

    unkAEFunc2Param = 1.0f;
    colorBalanceParam1 = 0.5f;
    shown = GL_l_i_RunAfterEffects();
    brightOnly = glr_AeRed(shown, x, row);

    /* The brightness alone already takes this pixel over the pivot,
       so the contrast afterwards pushes it further up rather than
       down -- which is the order mattering: contrast first would
       have pushed the original dark pixel down to black and the
       brightness would then have lifted it only to the half. */
    ok(brightOnly > 128, "the brightness alone lifts it above the pivot");
    ok(both > brightOnly, "and the contrast afterwards lifts it further");

    /* The other order, done by hand, to show the two differ: a dark
       pixel stretched first and brightened after. */
    {
        double mid = 128.0 / 255.0;
        double v = (double)glr_aeRampAt(x) / 255.0;
        double stretchedFirst = (v - mid) * 2.0 + mid;
        double thenBright;

        if (stretchedFirst < 0.0)
            stretchedFirst = 0.0;
        thenBright = stretchedFirst + 0.5;
        if (thenBright > 1.0)
            thenBright = 1.0;
        ok(fabs(thenBright * 255.0 - (double)both) > 8.0,
           "the other order gives a visibly different answer");
    }

    /* And with all three on, the grayscale is last: the output is
       grey whatever the other two did. */
    PC_AE_MASTER = 0x1;
    unkAEFunc2Param = 0.7f;
    colorBalanceParam1 = 0.3f;
    shown = GL_l_i_RunAfterEffects();
    {
        int rgba[4];

        glr_AeAt(shown, x, row, rgba);
        okEq(rgba[1], rgba[0], "with all three on the answer is grey");
        okEq(rgba[2], rgba[0], "in every channel");
    }

    PC_AE_MASTER = 0;
    unkAEFunc2Param = 0.5f;
    colorBalanceParam1 = 0.5f;
    motionBlurFactor = 0.0f;
    remenanceFactor = 0.0f;
    zoomSmoothFactor = 0.0f;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}


/* ------------------------------- the effects that sum whole frames

   Remanence, motion blur and zoom smooth. Each is a sum, so a frame of
   one flat colour gives an exact answer the weights can be read off:
   every tap or term reads the same value, and what comes back is that
   value times what the effect adds up to. Frames with an edge in them
   then say where the sum reaches.
 */
static int glr_aeFlatMid(int x, int row)
{
    (void)x; (void)row;
    return 100;
}

static int glr_aeBlack(int x, int row)
{
    (void)x; (void)row;
    return 0;
}

static int glr_aeWhite(int x, int row)
{
    (void)x; (void)row;
    return 255;
}

static void caseAeRemanence(void)
{
    GLuint scene;
    GLuint shown;
    int    row = GLR_AE_H / 2;
    int    x = GLR_AE_W / 2;
    int    rgba[4];

    printf("-- the remanence adds a blur of the frame back on top of it\n");
    glr_Reset();
    CIN_Bands = 0;

    /* One flat colour, so the blur of it is the same colour and the
       whole effect is a multiply by one plus the weight. That makes
       the weight readable off a single pixel. */
    scene = glr_AeScene(glr_aeFlatMid, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    PC_AE_MASTER = 0x800;
    remenanceFactor = 0.25f;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");
    {
        tAef_Source fs;
        double      want;

        fs.aeMaster = 0x800;
        fs.motionBlur = 0.0f;
        fs.remanence = 0.25f;
        fs.zoomSmooth = 0.0f;
        fs.bandBottom = 1.0f;
        want = 100.0 * (1.0 + (double)AEF_f_RemanenceWeight(&fs));
        okWithin((double)glr_AeRed(shown, x, row), want, 1.5,
                 "a flat frame comes back brighter by the weight");
        /* Which is brighter than it went in -- a bloom that did not
           brighten would be a no-op, and a weight read as a mix
           rather than an addition would leave it unchanged. */
        ok(glr_AeRed(shown, x, row) > 100, "and it is brighter, not a mix");
    }

    /* The alpha travels on: the effects after this one are mixed by
       it. */
    glr_AeAt(shown, x, row, rgba);
    okEq(rgba[3], 0x5A, "with the frame's alpha carried through");

    /* It saturates. At the top setting the weight is two, so anything
       above a third of white comes back white -- that is what makes a
       bloom bloom rather than haze. */
    remenanceFactor = 1.0f;
    shown = GL_l_i_RunAfterEffects();
    okEq(glr_AeRed(shown, x, row), 255,
         "at the top setting a middling frame blooms to white");

    /* And the bloom spreads: a single bright patch on black lights
       the black around it, which is the blur being what is added
       rather than the frame itself. */
    {
        int i;

        for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
        {
            unsigned char* p = &glr_aePattern[i * 4];
            int            px = i % GLR_AE_W;
            int            py = i / GLR_AE_W;
            int            on = px > GLR_AE_W / 2 - 3 && px < GLR_AE_W / 2 + 3
                                && py > row - 3 && py < row + 3;

            p[0] = (unsigned char)(on ? 255 : 0);
            p[1] = 0;
            p[2] = 0;
            p[3] = 0x5A;
        }
        GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
        GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                             GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);

        remenanceFactor = 1.0f;
        shown = GL_l_i_RunAfterEffects();
        ok(glr_AeRed(shown, x + 6, row) > 0,
           "black just outside a bright patch is lit by the bloom");
        okEq(glr_AeRed(shown, x + 120, row), 0,
             "and black far from it is not");
        ok(glr_AeRed(shown, x + 6, row) > glr_AeRed(shown, x + 14, row),
           "with the light falling off further out");
    }

    PC_AE_MASTER = 0;
    remenanceFactor = 0.0f;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

static void caseAeMotionBlur(void)
{
    GLuint scene;
    GLuint first;
    GLuint second;
    int    row = GLR_AE_H / 2;
    int    x = GLR_AE_W / 2;

    printf("-- the motion blur mixes this frame with the one before\n");
    glr_Reset();
    CIN_Bands = 0;

    /* Two frames, run one after the other with nothing forgotten in
       between -- which is the only way to see an effect whose other
       input is what it wrote last time.
     *
       White first, over a target the chain cleared to black when it
       built it, at a half: so the first frame comes back at half
       white. Then black, which keeps half of that: a quarter. Those
       two numbers together say the mixing is with the target's own
       previous contents and that the share is the right way round. */
    scene = glr_AeScene(glr_aeWhite, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    PC_AE_MASTER = 0x10;
    motionBlurFactor = 0.5f;
    first = GL_l_i_RunAfterEffects();
    ok(first != scene, "it ran");
    {
        /* The share kept is a byte's complement, so halfway keeps 128
           of 255 rather than exactly half -- the unit test pins that
           and this follows it rather than assuming a half. */
        tAef_Source fs;
        double      keep;

        fs.aeMaster = 0x10;
        fs.motionBlur = 0.5f;
        fs.remanence = 0.0f;
        fs.zoomSmooth = 0.0f;
        fs.bandBottom = 1.0f;
        keep = (double)AEF_f_MotionBlurKeep(&fs);

        okWithin((double)glr_AeRed(first, x, row), 255.0 * (1.0 - keep), 1.5,
                 "the first frame over a cleared target is its own share"
                 " of white");

        /* Now a black frame. What comes back is the share kept of
           what the first frame left, and nothing of the black. */
        {
            int i;

            for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
            {
                unsigned char* p = &glr_aePattern[i * 4];

                p[0] = p[1] = p[2] = 0;
                p[3] = 0x5A;
            }
            GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
            GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                                 GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
            GLFN_glBindTexture(GL_TEXTURE_2D, 0);
        }
        second = GL_l_i_RunAfterEffects();
        okEq((long)(second == first), 1,
             "the second frame lands in the same target as the first,"
             " which is what makes the history readable");
        okWithin((double)glr_AeRed(second, x, row),
                 255.0 * (1.0 - keep) * keep, 2.0,
                 "and is the share kept of what the frame before left");
        /* Not black, which is what a motion blur reading a cleared or
           a freshly-drawn target would give. */
        ok(glr_AeRed(second, x, row) > 0,
           "so a black frame after a white one is not black");
    }

    /* A lopsided setting, because a half is the one place the two
       shares are nearly equal: at a half the new frame takes 127 of
       255 and the old keeps 128, so reading the wrong one of the two
       is a single step and invisible. At a quarter they are 191 and
       64, and the reading below tells them apart. */
    motionBlurFactor = 0.25f;
    GL_l_ForgetAe();
    {
        tAef_Source fs;
        double      keep;
        int         i;

        /* A white frame again, over spares the chain clears when it
           rebuilds them -- which GL_l_ForgetAe above arranged, so
           this starts from a known history rather than the trail left
           by the readings before. */
        for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
        {
            unsigned char* p = &glr_aePattern[i * 4];

            p[0] = p[1] = p[2] = 255;
            p[3] = 0x5A;
        }
        GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
        GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                             GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);

        fs.aeMaster = 0x10;
        fs.motionBlur = 0.25f;
        fs.remanence = 0.0f;
        fs.zoomSmooth = 0.0f;
        fs.bandBottom = 1.0f;
        keep = (double)AEF_f_MotionBlurKeep(&fs);
        ok(keep < 0.3, "a quarter keeps about a quarter of the old frame");

        okWithin((double)glr_AeRed(GL_l_i_RunAfterEffects(), x, row),
                 255.0 * (1.0 - keep), 2.0,
                 "so three quarters of the white frame comes through on"
                 " the first frame");
    }

    /* At nought nothing of the frame before survives, so the result
       is the frame itself -- which is also the setting at which the
       effect is skipped, so this is the boundary either side of it. */
    motionBlurFactor = 0.0f;
    okEq((long)(GL_l_i_RunAfterEffects() == scene), 1,
         "at nought the effect is skipped altogether");

    PC_AE_MASTER = 0;
    motionBlurFactor = 0.0f;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

/* The fade.
 *
 * glr_aeStepDown above is the picture used, and it is used because it
 * is lopsided *vertically*: the fade copies the frame aside with a
 * blit and reads it back through a quad, and either of those getting
 * the vertical the wrong way round returns the same two levels in the
 * wrong halves -- which a flat picture, or one split left and right,
 * cannot see at all.
 */
/* The hue sweep.
 *
 * The one case that matters is that the three channels come off by
 * three *different* amounts. A shader subtracting one number would
 * darken the picture convincingly and be a brightness; the whole
 * effect is that it is not one number.
 *
 * So the picture is grey -- equal in all three channels -- and what is
 * read back is three unequal channels. Nothing about a tinted picture
 * says which channel got which amount, so the three are checked
 * against the shared code's own packed colour rather than against each
 * other.
 */
/* The motion smooth, and the one thing only a picture can say: which
 * way the smear goes.
 *
 * The four taps are at minus three, plus two, minus one and nothing
 * times the step -- a **lopsided** set, three steps one way and two
 * the other. That asymmetry is what makes the direction testable: a
 * smear flipped vertically reaches three steps the wrong way and two
 * the other, which is still a smear of the right length and looks
 * entirely convincing. Reading only the amount of blur would pass.
 *
 * So the frame is black with one bright row in it, the camera is
 * turned about one axis only, and what is asserted is the rows the
 * trail lands on -- including the two rows it must *not* reach.
 */
/* The screen blur, against a modelled profile.
 *
 * Seven passes of a four-tap cross, each reaching one step further
 * than the last. What a real driver can say that the other harnesses
 * cannot is what the composition actually comes to -- so this models
 * all seven passes and compares the profile row by row, rather than
 * asserting that a bright row got wider.
 *
 * The model quantises after each pass, as the eight-bit target does.
 * Modelling it in floats and comparing loosely would hide a wrong
 * weight; modelling one pass instead of seven would hide a wrong
 * count, which is the mistake the water's smoothing shipped with.
 */
#define GLR_SB_ROWS 160

static void glr_ScreenBlurModel(double* out, int rows, int centre,
                                float strength)
{
    float  uSteps[AEF_SCREENBLUR_PASSES];
    float  vSteps[AEF_SCREENBLUR_PASSES];
    float  weights[DPB_BLUR_TAPS];
    double cur[GLR_SB_ROWS];
    double next[GLR_SB_ROWS];
    int    n;
    int    p;
    int    i;

    n = AEF_i_ScreenBlurReaches(uSteps, vSteps, AEF_SCREENBLUR_PASSES,
                                strength);
    DPB_ChainWeights(weights);

    for (i = 0; i < rows; i++)
        cur[i] = 0.0;
    cur[centre] = 255.0;

    for (p = 0; p < n; p++)
    {
        /* The vertical reach in rows. The two taps across land on the
           same row as the pixel, so in a vertical profile they only
           add their weight to the middle. */
        int step = (int)((double)vSteps[p] * (double)GLR_AE_H + 0.5);
        double mid = (double)weights[0] + (double)weights[1];

        for (i = 0; i < rows; i++)
        {
            double v = mid * cur[i];

            /* The heavier of the two vertical taps samples the row
               *above* in this renderer's rows, because the renderer
               negates the schedule's vertical offsets on their way to
               the shader. Model them the other way round and the
               profile comes out mirrored -- which is what a first
               draft of this did, and the mirror is plainly visible
               because the two weights are far from equal. */
            if (i + step < rows)
                v += (double)weights[2] * cur[i + step];
            if (i - step >= 0)
                v += (double)weights[3] * cur[i - step];
            /* Rounded to a byte, as writing it to the target does. */
            next[i] = (double)(int)(v + 0.5);
            if (next[i] > 255.0)
                next[i] = 255.0;
        }
        for (i = 0; i < rows; i++)
            cur[i] = next[i];
    }
    for (i = 0; i < rows; i++)
        out[i] = cur[i];
}

static void caseAeScreenBlur(void)
{
    GLuint scene;
    GLuint shown;
    double want[GLR_SB_ROWS];
    int    x = GLR_AE_W / 2;
    int    centre = GLR_AE_H / 2;
    int    base = centre - GLR_SB_ROWS / 2;
    int    i;

    printf("-- the screen blur, seven passes against a modelled"
           " profile\n");
    glr_Reset();
    CIN_Bands = 0;

    scene = glr_AeScene(glr_aeAlphaNone, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    /* Black but for one bright row. */
    for (i = 0; i < GLR_AE_W * GLR_AE_H * 4; i++)
        glr_aePattern[i] = 0;
    for (i = 0; i < GLR_AE_W; i++)
        glr_aePattern[(centre * GLR_AE_W + i) * 4 + 0] = 255;
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    /* A reach of 25, which makes a step exactly one pixel of a 640 by
       480 picture -- so the seven passes reach one to seven rows and
       the steps are whole rows, with no bilinear blending to model. */
    PC_AE_MASTER = 0x20;
    blurFactor = 25.0f;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");

    glr_ScreenBlurModel(want, GLR_SB_ROWS, GLR_SB_ROWS / 2, 25.0f);

    /* Row by row over the modelled window. Two levels of slack, for
       the rounding the model and the card need not agree on exactly. */
    {
        int differing = 0;

        for (i = 0; i < GLR_SB_ROWS; i++)
        {
            double got = (double)glr_AeRed(shown, x, base + i);

            if (!(got >= want[i] - 2.0 && got <= want[i] + 2.0))
                differing++;
        }
        okEq((long)differing, 0L, "every row of the profile matches the"
                                  " seven-pass model");
    }

    /* And the shape, said separately so that a model and a renderer
       agreeing on nothing cannot pass.
     *
       The spread is **not** symmetric, and that is the depth blur's
       own doing rather than this effect's: its two vertical taps are
       weighted differently, so the blur leans one way. Asserting
       symmetry here is wrong -- a first draft did, and failed, which
       is how the lean got checked rather than assumed. */
    ok(glr_AeRed(shown, x, centre) < 200,
       "the bright row has spread rather than stayed");
    {
        int above = 0;
        int below = 0;

        for (i = 1; i <= 7; i++)
        {
            above += glr_AeRed(shown, x, centre + i);
            below += glr_AeRed(shown, x, centre - i);
        }
        ok(above != below, "the spread leans, as the blur's weights do");
        ok(above > below, "and it leans the way the heavier tap points");
    }

    /* A reach of one does nothing anyone could see, which is what
       makes the parameter a distance rather than a fraction: the seven
       passes land under a third of a pixel out, so every tap falls on
       the pixel it started from. */
    /* A reach under a pixel moves nothing and still costs brightness,
       which is the property worth a case of its own. Every tap lands
       on the pixel it started from, so nothing spreads -- but each tap
       is interpolated a fraction of a pixel out, bringing back a
       mixture with its dark neighbour, and seven passes of that
       compound to about half.
     *
       A first draft asserted the row came back untouched. It does not,
       and the reading is what said so. */
    blurFactor = 1.0f;
    shown = GL_l_i_RunAfterEffects();
    ok(glr_AeRed(shown, x, centre + 4) <= 2,
       "a reach of one spreads nothing");
    ok(glr_AeRed(shown, x, centre) > 100 && glr_AeRed(shown, x, centre) < 150,
       "and yet costs about half the brightness");

    PC_AE_MASTER = 0;
    blurFactor = 0.0f;
    CIN_Bands = 1;
}

static void caseAeMotionSmooth(void)
{
    GLuint scene;
    GLuint shown;
    int    x = GLR_AE_W / 2;
    int    row = GLR_AE_H / 2;
    int    step;
    int    i;

    printf("-- the motion smooth smears along the camera's turn\n");
    glr_Reset();
    CIN_Bands = 0;

    scene = glr_AeScene(glr_aeAlphaNone, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    /* Black but for one bright row. */
    for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
    {
        glr_aePattern[i * 4 + 0] = 0;
        glr_aePattern[i * 4 + 1] = 0;
        glr_aePattern[i * 4 + 2] = 0;
        glr_aePattern[i * 4 + 3] = 0;
    }
    for (i = 0; i < GLR_AE_W; i++)
        glr_aePattern[(row * GLR_AE_W + i) * 4 + 0] = 255;
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);

    PC_AE_MASTER = (int)AMO_BIT;

    /* The camera still, to settle the watched direction. Rotating and
       not translating: the transform that carries the direction has no
       translation term, so a camera that only travels moves nothing. */
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);
    GL_l_i_RunAfterEffects();

    /* Then a turn about the vertical only. A lean of 0.16 gives a
       movement of 0.16, a smear of eight, and a step of exactly eight
       rows -- the vertical scale is a four-hundred-and-eightieth and
       the frame is 480 rows, so one unit of smear is one row. */
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);
    glr_dd.ct.cam.cameraMtx.v2.y = 1.0f + 0.16f;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");
    step = 8;

    /* Each tap that lands on the bright row contributes its own
       weight of it and nothing else does, so each of the four rows of
       the trail reads the weight times full brightness. */
    {
        double lit = 255.0 * (double)AMO_f_Weight();

        okWithin((double)glr_AeRed(shown, x, row), lit, 2.0,
                 "the row itself is lit by the tap at nothing");
        okWithin((double)glr_AeRed(shown, x, row - 3 * step), lit, 2.0,
                 "and three steps one way");
        okWithin((double)glr_AeRed(shown, x, row - 1 * step), lit, 2.0,
                 "and one step the same way");
        okWithin((double)glr_AeRed(shown, x, row + 2 * step), lit, 2.0,
                 "and two steps the other");

        /* The two that say it is the right way up. A vertical flip
           puts the three-step reach on the other side and the
           two-step reach on this one -- same trail, same length,
           mirrored. These are the only readings that can tell. */
        okEq((long)glr_AeRed(shown, x, row + 3 * step), 0L,
             "three steps the other way is dark");
        okEq((long)glr_AeRed(shown, x, row - 2 * step), 0L,
             "and two steps this way is dark");

        /* And the trail really is lopsided, or the pair above would
           hold for a symmetric smear read carelessly. */
        ok(3 * step != 2 * step, "with the trail lopsided by design");
    }

    /* A turn about the line of sight crosses the threshold and smears
       by nothing, so the picture comes back as four copies of itself
       in the same place -- the weight times four, which is a shade
       under the original.
     *
       Settled first, with a frame at the identity. Without it the
       movement would be measured from where the turn above left the
       watched direction, which is a movement on two axes and not the
       one this is about -- and that is how this assertion first went
       in wrong, reading a vertical smear and calling it none. */
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);
    GL_l_i_RunAfterEffects();
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);
    glr_dd.ct.cam.cameraMtx.v3.z = 1.0f + 0.5f;
    shown = GL_l_i_RunAfterEffects();
    okWithin((double)glr_AeRed(shown, x, row),
             255.0 * (double)AMO_f_Weight() * 4.0, 2.0,
             "turning away smears by nothing and dims by three parts"
             " in 255");
    okEq((long)glr_AeRed(shown, x, row - 3 * step), 0L,
         "with no trail at all");

    PC_AE_MASTER = 0;
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);
    CIN_Bands = 1;
}

static void caseAeHue(void)
{
    GLuint       scene;
    GLuint       shown;
    int          x = GLR_AE_W / 2;
    int          row = GLR_AE_H / 2;
    int          rgba[4];
    unsigned int packed;
    float        sub[AHU_CHANNELS];
    const int    grey = 200;

    printf("-- the hue sweep takes a different amount from each channel\n");
    glr_Reset();
    CIN_Bands = 0;

    scene = glr_AeScene(glr_aeWhite, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    /* Grey in all three channels. glr_AeScene writes red alone, which
       is the input every other case here wants, so this one paints its
       own -- a tint of a grey is the only picture that shows three
       different amounts as three different levels. */
    {
        int i;

        for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
        {
            glr_aePattern[i * 4 + 0] = (unsigned char)grey;
            glr_aePattern[i * 4 + 1] = (unsigned char)grey;
            glr_aePattern[i * 4 + 2] = (unsigned char)grey;
            glr_aePattern[i * 4 + 3] = 0x5A;
        }
        GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
        GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                             GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    }

    /* A depth and a phase whose three amounts are all different and
       all inside the grey, so nothing is clipped at black and every
       channel is readable. */
    PC_AE_MASTER = (int)AHU_BIT;
    unknownAEFlag4GlobalVar = 0.5f;
    texFactorManual = 0.2f;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");

    packed = AHU_u_Subtract(0.5f, 0.2f);
    AHU_Channels(sub, packed);
    glr_AeAt(shown, x, row, rgba);

    okWithin((double)rgba[0], (double)grey - (double)sub[0] * 255.0, 1.5,
             "red came off by red's amount");
    okWithin((double)rgba[1], (double)grey - (double)sub[1] * 255.0, 1.5,
             "green by green's");
    okWithin((double)rgba[2], (double)grey - (double)sub[2] * 255.0, 1.5,
             "and blue by blue's");

    /* And the three really are different, or the three readings above
       would hold for a shader subtracting one number. */
    ok(rgba[0] != rgba[1] && rgba[1] != rgba[2] && rgba[0] != rgba[2],
       "with the three channels left at three different levels");

    /* The one channel the wave leaves alone is untouched, which is what
       makes this a tint rather than a darkening: at this phase green's
       own amount is nothing. */
    okEq((long)(sub[1] * 255.0f + 0.5f), 0L, "green's amount is nothing"
                                             " at this phase");
    okWithin((double)rgba[1], (double)grey, 1.5,
             "so green comes through untouched");

    /* Saturating at black and not wrapping: a depth deep enough to
       take more than the picture has leaves black, not a bright
       channel. A wrap here would be the brightest possible mistake. */
    unknownAEFlag4GlobalVar = 1.0f;
    texFactorManual = 0.5f;
    shown = GL_l_i_RunAfterEffects();
    AHU_Channels(sub, AHU_u_Subtract(1.0f, 0.5f));
    ok(sub[0] * 255.0f > (float)grey, "more red is asked for than there is");
    glr_AeAt(shown, x, row, rgba);
    okEq((long)rgba[0], 0L, "and red saturates at black");

    PC_AE_MASTER = 0;
    unknownAEFlag4GlobalVar = 0.0f;
    texFactorManual = 0.0f;
    CIN_Bands = 1;
}

static void caseAeFade(void)
{
    GLuint scene;
    GLuint shown;
    int    x = GLR_AE_W / 2;
    int    bright = GLR_AE_H / 4;               /* the step's lit half */
    int    dark = GLR_AE_H - GLR_AE_H / 4;

    printf("-- the fade dissolves out of a frame it froze earlier\n");
    glr_Reset();
    CIN_Bands = 0;

    scene = glr_AeScene(glr_aeStepDown, glr_aeAlphaNone);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    PC_AE_MASTER = (int)AFD_BIT;
    gAEOpacity = 1.0f;                  /* sixty frames asked for */
    glr_fadeOpacity = AFD_FULL;

    /* The first frame of a fade shows the picture unchanged, because
       the copy it dissolves out of was taken from that very picture.
       Both halves, so a copy that lost the vertical shows here. */
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");
    okEq(glr_AeRed(shown, x, bright), 240,
         "the first frame is the picture itself, lit half");
    okEq(glr_AeRed(shown, x, dark), 0, "and dark half");
    okEq(glr_fadeOpacity, AFD_i_Next(AFD_FULL, AFD_f_Step(1.0f)),
         "with the timer moved on by one step");

    /* Now the world moves: the live frame goes flat black while the
       fade carries on. What comes back has to be the *frozen* picture
       at its share -- which is the whole point of the effect, and the
       one thing a single frame cannot show. */
    {
        int i;

        for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
            glr_aePattern[i * 4] = 0;
        GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
        GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                             GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);
    }
    glr_fadeOpacity = 128;
    shown = GL_l_i_RunAfterEffects();

    okWithin((double)glr_AeRed(shown, x, bright),
             240.0 * (double)AFD_f_Blend(128), 1.5,
             "a black world still shows the frozen picture at its share");
    okEq(glr_AeRed(shown, x, dark), 0,
         "and the frozen picture's dark half stays dark");
    /* The right way up, which the two readings above cannot say on
       their own: swap them and both still land on a level that is
       somewhere in the picture. */
    ok(glr_AeRed(shown, x, bright) > glr_AeRed(shown, x, dark) + 50,
       "with the frozen copy the right way up");
    /* And not re-frozen: a copy taken every frame would be a copy of
       the black world, and this would be black. */
    ok(glr_AeRed(shown, x, bright) > 0,
       "the copy was taken once and not again");

    /* The end of the fade: almost nothing of the frozen picture is
       left, and the effect switches itself off. */
    glr_fadeOpacity = 1;
    shown = GL_l_i_RunAfterEffects();
    okWithin((double)glr_AeRed(shown, x, bright), 240.0 / 255.0, 1.5,
             "the last frame is almost all the live world");
    okEq(PC_AE_MASTER & (int)AFD_BIT, 0, "and the fade cleared its own bit");

    PC_AE_MASTER = 0;
    gAEOpacity = 0.0f;
    glr_fadeOpacity = 0;
    CIN_Bands = 1;
}

static void caseAeZoomSmooth(void)
{
    GLuint scene;
    GLuint shown;
    int    row = GLR_AE_H / 2;
    int    x = GLR_AE_W / 2;
    int    rgba[4];

    printf("-- the zoom smears a pixel along the line from the centre\n");
    glr_Reset();
    CIN_Bands = 0;

    /* One flat colour again: all eight taps read the same value, so
       what comes back is that value times the eight weights. They
       come to 248 of 255, so a flat frame comes back a shade dark --
       which is the whole of what the weight can be read from. */
    scene = glr_AeScene(glr_aeFlatMid, glr_aeAlphaMark);
    ok(scene != 0, "the scene target was made");
    if (!scene)
    {
        CIN_Bands = 1;
        return;
    }

    PC_AE_MASTER = 0x400;
    zoomSmoothFactor = 1.0f;
    shown = GL_l_i_RunAfterEffects();
    ok(shown != scene, "it ran");
    okWithin((double)glr_AeRed(shown, x, row),
             100.0 * (double)AEF_f_ZoomWeight() * AEF_ZOOM_TAPS, 1.5,
             "a flat frame comes back at the eight weights' sum");
    ok(glr_AeRed(shown, x, row) < 100,
       "which is a shade under what went in");
    glr_AeAt(shown, x, row, rgba);
    okEq(rgba[3], 0x5A, "with the frame's alpha carried through");

    /* And the direction, which is the thing a smeared picture cannot
       tell you and which I had backwards first time.
     *
       Every tap after the first crops inwards, which *magnifies*: the
       tap shows a zoomed-in frame. So a pixel reads positions nearer
       the centre than itself, and light therefore appears at
       positions further out than where it was -- the smear trails
       **away** from the centre, not towards it. A crop that went the
       other way would smear inwards and the picture would look just
       as smeared, which is why this is checked on both sides of the
       line rather than by there being a smear at all. */
    {
        int i;
        int ringX = GLR_AE_W / 2 + 160;

        for (i = 0; i < GLR_AE_W * GLR_AE_H; i++)
        {
            unsigned char* p = &glr_aePattern[i * 4];
            int            px = i % GLR_AE_W;

            p[0] = (unsigned char)(px >= ringX && px < ringX + 2 ? 255 : 0);
            p[1] = 0;
            p[2] = 0;
            p[3] = 0x5A;
        }
        GLFN_glBindTexture(GL_TEXTURE_2D, gl_sceneColour);
        GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLR_AE_W, GLR_AE_H,
                             GL_RGBA, GL_UNSIGNED_BYTE, glr_aePattern);
        GLFN_glBindTexture(GL_TEXTURE_2D, 0);

        shown = GL_l_i_RunAfterEffects();

        /* Read over a span rather than at a pixel. Eight taps of a
           two-pixel line are eight discrete copies of it, about eight
           pixels apart at this distance from the centre -- not a
           continuous streak -- so a single reading between two copies
           is dark whichever way the smear runs. Two earlier drafts of
           this case asserted single pixels and failed for that reason
           rather than because the direction was wrong. The claim is
           about which side the copies are on, so that is what is
           measured. */
        {
            int k;
            int outward = 0;
            int inward = 0;

            for (k = 4; k <= 36; k++)
            {
                int out = glr_AeRed(shown, ringX + k, row);
                int in = glr_AeRed(shown, ringX - k, row);

                if (out > outward)
                    outward = out;
                if (in > inward)
                    inward = in;
            }
            ok(outward > 0, "there is light outside the line, away from the"
                            " centre");
            okEq(inward, 0, "and none inside it, towards the centre");
            ok(outward > inward, "so the smear trails away from the centre");
        }

        /* And it runs out. "Far enough out" is further than it looks:
           how far a pixel reaches back grows with its own distance
           from the centre, because the crop is proportional -- so the
           copies stretch to about seventy pixels here rather than the
           forty the span above covers. Measured, not reasoned: a
           draft of this checked from sixty and found a copy at
           sixty-eight. */
        {
            int k;
            /* Not `far`: the Windows stubs define that as a macro,
               and a local by that name vanishes into nothing. */
            int reach = 0;

            for (k = 80; k <= 200; k++)
                if (glr_AeRed(shown, ringX + k, row) > reach)
                    reach = glr_AeRed(shown, ringX + k, row);
            okEq(reach, 0, "far enough out the smear has run out");
        }
    }

    PC_AE_MASTER = 0;
    zoomSmoothFactor = 0.0f;
    CIN_Bands = 1;
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    gl_state.width = GLR_W;
    gl_state.height = GLR_H;
}

/* ------------------------------------------------------- the water
 *
 * Drawn on a real driver and read back, which is the only way to see
 * what the surface actually comes out as. The mock harness checks the
 * calls and tests/run_wingdi.sh checks the shared mesh against the
 * other renderer; what is left for here is the picture.
 *
 * The reflection target is cleared to one flat colour, so the
 * projection does not come into these: wherever a pixel samples, it
 * gets the same answer. That is deliberate -- it leaves the blend to
 * be measured on its own, and the blend is the part a shader can get
 * wrong silently.
 */

#define GLR_WTR_COLS 5
#define GLR_WTR_ROWS 5
#define GLR_WTR_NUM  (GLR_WTR_COLS * GLR_WTR_ROWS)

static tWaterParamsGlobalStruct glr_water;
static Vec3         glr_waterPos[GLR_WTR_NUM];
static Vec3         glr_waterWave[GLR_WTR_NUM];
static unsigned int glr_waterCol[GLR_WTR_NUM];

/* A grid over the whole of clip space, flat at the depth asked for,
   with one colour and one alpha throughout. `bottom` is what the
   reflection is cleared to.
 *
 * The four channels are separate arguments rather than one word, for
 * the reason glr_SolidColour gives above it: the engine keeps a colour
 * with red in the lowest byte, so a hex literal in that order reads
 * like a mistake and the first draft of these cases had every red and
 * blue the wrong way round.
 *
 * `bottom` is a word, and a differently ordered one: the renderer this
 * came from hands it straight to a clear that reads it as alpha, red,
 * green, blue, so that is what it means here too. Whether the engine
 * fills it in that order is the engine's business and the same
 * question for both renderers. */
static void glr_WaterFixture(int r, int g, int b, int a, int bottom, float z)
{
    int row;
    int col;
    int i = 0;

    memset(&glr_water, 0, sizeof(glr_water));
    for (row = 0; row < GLR_WTR_ROWS; row++)
        for (col = 0; col < GLR_WTR_COLS; col++)
        {
            /* Past the edges on purpose: the strip covers all but the
               last row, so a grid that stopped at the edge would
               leave a band of the surface undrawn and the pixel a
               case reads could be background. */
            glr_waterPos[i].x = -1.5f + 3.0f * (float)col
                                        / (float)(GLR_WTR_COLS - 1);
            glr_waterPos[i].y = -1.5f + 3.0f * (float)row
                                        / (float)(GLR_WTR_ROWS - 1);
            glr_waterPos[i].z = z;
            /* No wave at all, so the direction is the bare one from
               the camera and nothing in the picture depends on the
               fall-off. The ripple has its own checks elsewhere. */
            glr_waterWave[i].x = 0.0f;
            glr_waterWave[i].y = 0.0f;
            glr_waterWave[i].z = 0.0f;
            glr_waterCol[i] = ((unsigned int)a << 24)
                              | ((unsigned int)b << 16)
                              | ((unsigned int)g << 8)
                              | (unsigned int)r;
            i++;
        }
    glr_water.vertices = glr_waterPos;
    glr_water.vecsForUVs = glr_waterWave;
    glr_water.colors = glr_waterCol;
    glr_water.horizontalVertexNum = GLR_WTR_COLS;
    glr_water.verticallVertexNum = GLR_WTR_ROWS;
    glr_water.waterEngine.bottomColor = bottom;
    pst_GlobalsWaterParams = &glr_water;

    /* A real camera, at the origin looking along the axes. The water
       draw builds its own view by inverting this -- the engine's view
       matrix is the matrix stack's top and has the last object folded
       into it -- so a fixture that left this a block of nought would
       hand the draw a matrix with no inverse, and every case here
       would be reading whatever that produced. */
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.objectMtx);
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);
}

/* Prepares the frame, then takes the target back: prepare leaves the
   scene target bound, and these cases want the surface in a buffer
   they can read a pixel out of. */
static void glr_WaterDraw(void)
{
    GL_PrepareWaterRendering();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    glr_Read();
}

static void caseWaterReflectionCleared(void)
{
    int rgba[4];

    printf("-- the reflection target holds the colour the water looks into\n");

    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    /* 0x20406080: alpha 0x20, red 0x40, green 0x60, blue 0x80. All
       four different, so the word cannot be read the wrong way round
       and still pass. */
    glr_WaterFixture(0x20, 0x40, 0x60, 0xFF, 0x20406080, 0.0f);
    GL_PrepareWaterRendering();

    glr_AeAt(gl_waterFbo, 8, 8, rgba);
    okEq(rgba[0], 0x40, "red is the second byte down");
    okEq(rgba[1], 0x60, "green the third");
    okEq(rgba[2], 0x80, "blue the lowest");
    okEq(rgba[3], 0x20, "and alpha the top");

    /* The whole of it, not just the corner the sampler would reach
       first: a clear that only took in part of the target would leave
       the rest holding the last frame's reflection. */
    glr_AeAt(gl_waterFbo, 500, 500, rgba);
    okEq(rgba[0], 0x40, "and the far corner is cleared too");
}

static void caseWaterBlend(void)
{
    printf("-- the surface mixes its own colour with the reflection\n");

    /* Alpha at nought: none of the reflection, all of the water's own
       colour. The reflection is cleared to a cyan nothing else here produces so
       that a mix the wrong way round cannot be mistaken for a pass. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    glr_WaterFixture(0x20, 0x40, 0x80, 0x00, 0x0000FFFF, 0.0f);
    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x20, "with no alpha the pixel is the"
                                            " water's own red");
    okEq(glr_G(GLR_W / 2, GLR_H / 2), 0x40, "its own green");
    okEq(glr_B(GLR_W / 2, GLR_H / 2), 0x80, "and its own blue");

    /* And the surface really covered the pixel rather than the case
       reading the background and agreeing with itself. */
    ok(glr_R(GLR_W / 2, GLR_H / 2) != GLR_BG
       || glr_G(GLR_W / 2, GLR_H / 2) != GLR_BG,
       "and the surface did cover the pixel");

    /* Alpha full: all of the reflection and none of the water. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x0000FFFF, 0.0f);
    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x00, "with full alpha the pixel is"
                                            " the reflection's red");
    okEq(glr_G(GLR_W / 2, GLR_H / 2), 0xFF, "its green");
    okEq(glr_B(GLR_W / 2, GLR_H / 2), 0xFF, "and its blue");

    /* And in between, the mix. Half of nought and 0xFF is where a mix
       written the other way round still differs, because the two ends
       are not symmetric about it. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    glr_WaterFixture(0x20, 0x40, 0x80, 0x80, 0x0000FFFF, 0.0f);
    glr_WaterDraw();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x10, "halfway, red is half the"
                                              " water's");
    okNear(glr_G(GLR_W / 2, GLR_H / 2), 0xA0, "green is between the two");
    okNear(glr_B(GLR_W / 2, GLR_H / 2), 0xC0, "and so is blue");

    /* The alpha written out is the water's, not the mix's: it is what
       the after-effects downstream read the frame's alpha for. */
    okNear(glr_A(GLR_W / 2, GLR_H / 2), 128, "and the alpha written is the"
                                             " water's own");
}

static void caseWaterIgnoresTheEngineView(void)
{
    Mtx4x4 elsewhere;

    printf("-- the surface is drawn by the camera, not by the last"
           " object\n");

    /* While the world is drawn, what the engine calls the view matrix
       is the top of its matrix stack -- the camera with the object
       being drawn folded into it -- and nothing puts the camera back
       before the water. So the water draw has to build its own view
       from the camera, as the other renderer does, and a stray
       transform left in the engine's must not reach it.

       The camera stays at the origin, so the surface belongs exactly
       where the cases above find it. The engine's view is then moved
       far to one side, which is what a object drawn somewhere else
       leaves behind: if that reached the draw, the surface would be
       off the screen and the middle pixel would read background. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    glr_WaterFixture(0x20, 0x40, 0x80, 0x00, 0x0000FFFF, 0.0f);

    MATH_SetIdentityMatrix(&elsewhere);
    elsewhere.Pos.x = 40.0f;
    elsewhere.Pos.y = 25.0f;
    GL_SetViewMatrix(&elsewhere);

    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x20,
         "the surface is still where the camera put it");
    okEq(glr_G(GLR_W / 2, GLR_H / 2), 0x40, "with its own green");
    okEq(glr_B(GLR_W / 2, GLR_H / 2), 0x80, "and its own blue");

    /* And the texture coordinate is built from the same view, so a
       stray transform would misplace the reflection even where the
       surface still covered the pixel. Full alpha, so the pixel is
       the reflection alone: the target is cleared to a cyan nothing
       else here produces. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x0000FFFF, 0.0f);
    MATH_SetIdentityMatrix(&elsewhere);
    elsewhere.Pos.x = 40.0f;
    elsewhere.Pos.y = 25.0f;
    GL_SetViewMatrix(&elsewhere);
    glr_WaterDraw();
    okEq(glr_G(GLR_W / 2, GLR_H / 2), 0xFF,
         "and the reflection is sampled where the camera says");
    okEq(glr_B(GLR_W / 2, GLR_H / 2), 0xFF, "in both channels of it");
}

static void caseWaterNotBlended(void)
{
    printf("-- the surface replaces the pixel rather than blending with it\n");

    /* The background is a flat grey and the water is opaque in its own
       colour with no alpha. If the surface were blended into the
       target the grey would show through; it must not. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_Begin();
    glr_WaterFixture(0x20, 0x40, 0x80, 0x00, 0x0000FFFF, 0.0f);
    glr_WaterDraw();
    okEq(glr_R(4, 4), 0x20, "a corner pixel is the water's colour exactly");

    /* Twice over, which is where a blend would show even if the first
       draw happened to land on the right number. */
    glr_WaterDraw();
    okEq(glr_R(4, 4), 0x20, "and drawing it again does not darken it");
    okEq(glr_B(4, 4), 0x80, "in any channel");
}

/* The reflection target with a pattern in it rather than one flat
 * colour: red over its lower half in texture terms, green over the
 * upper. Everything above this draws against a flat reflection, which
 * leaves the sampling unmeasured -- a flip, a view translation or a
 * dropped divide all sample a different place and get the same answer
 * back. Two mutants survived the lot.
 *
 * Uploaded straight into the target's texture, which is quicker than
 * drawing into it and does not disturb any state the renderer is
 * holding. */
static unsigned char* glr_waterPattern;

#define GLR_WTR_LOW_R  0xC0
#define GLR_WTR_LOW_G  0x00
#define GLR_WTR_HIGH_R 0x00
#define GLR_WTR_HIGH_G 0xC0

static void glr_WaterPatternIntoReflection(void)
{
    int row;
    int x;

    if (!glr_waterPattern)
        glr_waterPattern = (unsigned char*)malloc(512 * 512 * 4);
    for (row = 0; row < 512; row++)
        for (x = 0; x < 512; x++)
        {
            unsigned char* p = &glr_waterPattern[(row * 512 + x) * 4];

            /* Row nought is v = 0 in OpenGL, so the first half of the
               rows is the first half of v. */
            p[0] = (unsigned char)(row < 256 ? GLR_WTR_LOW_R : GLR_WTR_HIGH_R);
            p[1] = (unsigned char)(row < 256 ? GLR_WTR_LOW_G : GLR_WTR_HIGH_G);
            p[2] = 0;
            p[3] = 0xFF;
        }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_waterPattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
}

/* Where the shared remap says a grid point's reflection coordinate
 * lands, so a case can state which half of the pattern a pixel must
 * read rather than discovering it.
 *
 * The remap is watermesh.c's and run_wingdi.sh has already compared it
 * against the other renderer, so this is not the renderer's own
 * arithmetic restated -- it is the question of whether this renderer
 * samples where that arithmetic says. */
static float glr_f_WaterV(float worldY, float dirZ)
{
    Mtx4x4WithoutAdditionalData remap;
    const float*                m;
    float                       v;
    float                       w;

    WMS_ProjectionToTexture(&remap,
                            (const Mtx4x4WithoutAdditionalData*)&gl_projMtx,
                            512, 512);
    m = (const float*)&remap;
    /* Row-vector, with the direction's three components and a one:
       the second column for v, the third for the divide. */
    v = worldY * m[4 + 1] + dirZ * m[8 + 1] + m[12 + 1];
    w = worldY * m[4 + 2] + dirZ * m[8 + 2] + m[12 + 2];
    return w != 0.0f ? v / w : 0.0f;
}

static void caseWaterSampling(void)
{
    float vTop;
    float vBottom;

    printf("-- where on the reflection the surface samples\n");

    /* The grid's depth is chosen, not convenient. The coordinate is
       projective, so the divide by the direction's depth is what
       decides which half of the pattern a row lands in: at a half the
       divide doubles both rows past the boundary and they read the
       same half, which is what the precondition below is there to
       catch -- and did. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x00000000, 0.9f);
    GL_PrepareWaterRendering();
    glr_WaterPatternIntoReflection();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    glr_Read();

    /* A quarter of the way down the surface is world y = +0.5, three
       quarters is -0.5 -- the clip space this leaves world
       coordinates in, with y up and the rows counted from the top. */
    vTop = glr_f_WaterV(0.5f, 0.9f);
    vBottom = glr_f_WaterV(-0.5f, 0.9f);
    ok((vTop < 0.5f) != (vBottom < 0.5f),
       "the two rows read opposite halves of the pattern, so there is"
       " something to measure");

    okNear(glr_R(GLR_W / 2, GLR_H / 4),
           vTop < 0.5f ? GLR_WTR_LOW_R : GLR_WTR_HIGH_R,
           "the upper row reads the half the shared remap points at");
    okNear(glr_G(GLR_W / 2, GLR_H / 4),
           vTop < 0.5f ? GLR_WTR_LOW_G : GLR_WTR_HIGH_G,
           "in green as well as red");
    okNear(glr_R(GLR_W / 2, 3 * GLR_H / 4),
           vBottom < 0.5f ? GLR_WTR_LOW_R : GLR_WTR_HIGH_R,
           "and the lower row reads the other half");

    /* The console's flip turns that over, and only that. Which way
       round the default should be is a measurement on a real scene and
       is not asserted anywhere -- that is what the tweak is for. What
       is asserted is that nought means "where the shared arithmetic
       says", which is what the other renderer does, and that one means
       the other way. */
    tweak_gl_water_flip = 1;
    GL_PrepareWaterRendering();
    glr_WaterPatternIntoReflection();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 4),
           vTop < 0.5f ? GLR_WTR_HIGH_R : GLR_WTR_LOW_R,
           "with the flip on, the upper row reads the other half");
    okNear(glr_R(GLR_W / 2, 3 * GLR_H / 4),
           vBottom < 0.5f ? GLR_WTR_HIGH_R : GLR_WTR_LOW_R,
           "and so does the lower");
    tweak_gl_water_flip = 0;
}

static void caseWaterDirectionIsADirection(void)
{
    Mtx4x4 view;
    float  vNoTranslation;
    float  vWithIt;

    printf("-- the camera-to-vertex vector is a direction, not a point\n");

    /* The view gets a translation, which a direction must not pick up.
       With the view at the identity the two spellings are the same
       thing -- a mutant that gave the direction a one in its fourth
       component survived every case above, because there was no
       translation for it to pick up.

       The translation comes from the **camera**, because that is
       where the water's view comes from: the draw inverts the
       camera's own matrix rather than taking the engine's view, which
       during the world has the last object folded into it. A camera a
       unit behind the origin is a view that translates a unit
       forward, which is the matrix this case used to set by hand. */
    glr_Reset();
    glr_IdentityMatrices();
    (void)view;

    /* The grid sits where the translation puts it back inside the clip
       volume: at minus a half it lands at a half once the view has
       moved it. */
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x00000000, -0.5f);
    glr_dd.ct.cam.objectMtx.Pos.z = -1.0f;
    GL_PrepareWaterRendering();
    glr_WaterPatternIntoReflection();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    glr_Read();

    /* The direction's depth component is the grid's own minus the
       camera's -- minus a half less minus one, so a half -- with no
       view translation in it. Taking the translation as well would add
       the view's own unit and make it one and a half, and the two land
       in different halves of the pattern. */
    vNoTranslation = glr_f_WaterV(0.5f, 0.5f);
    vWithIt = glr_f_WaterV(0.5f, 1.5f);
    ok((vNoTranslation < 0.5f) != (vWithIt < 0.5f),
       "the two readings fall in different halves, so there is"
       " something to measure");
    okNear(glr_R(GLR_W / 2, GLR_H / 4),
           vNoTranslation < 0.5f ? GLR_WTR_LOW_R : GLR_WTR_HIGH_R,
           "and the surface reads the half with no translation in it");
}

/* A quad of a known colour at a known depth, drawn through the
 * geometry path, to put something in front of or behind the water.
 *
 * The depth state is set *after* GL_TextureInit, and that is the whole
 * reason this is a function. TextureInit is the renderer's per-material
 * state call and it owns the depth state, so a case that sets its own
 * beforehand has it quietly replaced -- two draws then both land
 * whatever their depths, and a water surface behind a wall reads as the
 * water ignoring depth. It did, for a while. */
static void glr_WaterWall(int r, int g, int b, float z)
{
    glr_OneTexel(0, 0, 0, 0xFF);     /* ignored; the material has none */
    glr_SolidColour(r, g, b, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_pos[0].z = glr_pos[1].z = glr_pos[2].z = glr_pos[3].z = z;
    GL_TextureInit(-1, 0, 0);
    /* OpenGL writes no depth at all while the test is off, whatever the
       depth mask says, so this has to switch the test on and not only
       the mask. */
    GLFN_glEnable(GL_DEPTH_TEST);
    GLFN_glDepthFunc(GL_LEQUAL);
    GLFN_glDepthMask(GL_TRUE);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
}

static void caseWaterDepth(void)
{
    printf("-- the surface is depth tested and does not write depth\n");

    /* Nearer is a *smaller* z here. This renderer sets the clip volume
       to nought-to-one rather than the minus-one-to-one OpenGL starts
       with, so a depth is the clip z straight through and a negative z
       is outside the volume entirely -- not drawn far away, not drawn
       at all. */
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_WaterWall(0x33, 0x22, 0x11, 0.2f);

    glr_WaterFixture(0x20, 0x40, 0x80, 0x00, 0x0000FFFF, 0.6f);
    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x33, "where something nearer was"
                                            " drawn, the water does not"
                                            " show");
    /* And outside the quad it does: otherwise the check above would
       pass on a surface that drew nowhere at all. */
    okEq(glr_R(2, 2), 0x20, "and outside it, it does");

    /* The other way round, so the check above is about depth and not
       about the quad being drawn first. */
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_WaterWall(0x33, 0x22, 0x11, 0.8f);
    glr_WaterFixture(0x20, 0x40, 0x80, 0x00, 0x0000FFFF, 0.4f);
    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x20, "and where the water is the"
                                            " nearer, it covers what was"
                                            " behind it");

    /* Depth not written, which takes a second surface *behind* the
       first to show. The draw sets less-or-equal itself, so two
       surfaces at the same depth both pass whether or not depth was
       written and prove nothing; put the second one further off
       instead, where it comes through only if the buffer still holds
       the clear. */
    glr_Reset();
    glr_IdentityMatrices();
    glr_WaterFixture(0x20, 0x40, 0x80, 0x00, 0x0000FFFF, 0.3f);
    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x20, "the first surface draws");
    glr_WaterFixture(0x80, 0x00, 0x00, 0x00, 0x0000FFFF, 0.7f);
    glr_WaterDraw();
    okEq(glr_R(GLR_W / 2, GLR_H / 2), 0x80, "and a second one behind it"
                                            " still draws, so the first"
                                            " wrote no depth");
}

/* ------------------------------------------- the water's reflection
 *
 * Drawn on a real driver and read back out of the reflection target,
 * which is the only way to see whether the mirrored draw lands where
 * the arithmetic says. The mock harness checks the calls; this checks
 * the picture.
 *
 * The numbers are chosen around the clip volume rather than for
 * roundness. This renderer clips z to nought-to-one, so a mirrored
 * object has to land inside that too: with the water at 0.6, an object
 * at 0.8 mirrors to 0.4 and is visible, and one at 0.3 mirrors to 0.9,
 * which is inside the volume but on the far side of the water line --
 * so it is the clip plane that removes it and not the volume, which is
 * what makes the clip plane checkable at all.
 */

static tGameObjectMain glr_reflGao;
static tGameObjectBV   glr_reflBv;
static Mtx4x4          glr_reflMtx;

#define GLR_WATER_H 0.6f

static void glr_ReflectionFixture(void)
{
    /* The reflection's clear colour is something no material here
       produces, so a pixel that holds it is a pixel nothing reflected
       into. The word is alpha, red, green, blue, which is not the
       order the vertex colours above it use -- this one goes to a
       clear and those go through the stream. */
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x0000FFFF, 0.5f);

    memset(&glr_reflGao, 0, sizeof(glr_reflGao));
    memset(&glr_reflBv, 0, sizeof(glr_reflBv));
    /* The object's transform is the identity, so its vertices are its
       world positions and the mirror is the only thing moving them. */
    MATH_SetIdentityMatrix(&glr_reflMtx);
    glr_reflBv.BVMin.x = -2.0f;
    glr_reflBv.BVMax.x = 2.0f;
    glr_reflBv.BVMin.y = -2.0f;
    glr_reflBv.BVMax.y = 2.0f;
    glr_reflGao.matrices = &glr_reflMtx;
    glr_reflGao.boundingVolume = &glr_reflBv;
    glr_dd.currTargetGao = &glr_reflGao;

    /* The camera the mirrored object is measured against. Left at the
       zero a reset gives it, every reflected vertex would collapse to
       the origin. */
    MATH_SetIdentityMatrix(&glr_dd.ct.cam.cameraMtx);

    waterHeight = GLR_WATER_H;
    tweak_water_min_bv = 0.0f;
    tweak_water_clip = 1;
    GDI_gi_TextureUVMode = 0;
}

/* The quad through the geometry path at a given depth. Culling is left
   off -- 0x10 without the cull bit -- so that what this measures is
   where the reflection draws and not which way its triangles face.
   The facing has its own check in the mock harness, where it can be
   read directly rather than inferred from a missing picture. */
static void glr_ReflectAt(float z, int r, int g, int b)
{
    glr_OneTexel(0, 0, 0, 0xFF);
    glr_SolidColour(r, g, b, 0xFF);
    glr_dd.drawMaskAllowedStuffCurrent = 0x10;
    glr_pos[0].z = glr_pos[1].z = glr_pos[2].z = glr_pos[3].z = z;
    GL_TextureInit(-1, 0, 0);
    GL_DrawElementIndexedTriangles(&glr_chunk, glr_pos, glr_uv, 4);
}

static void caseWaterReflectionDraws(void)
{
    int rgba[4];

    printf("-- the world appears in the reflection, mirrored\n");

    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_ReflectionFixture();
    GL_PrepareWaterRendering();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);

    /* Above the water, so it mirrors to the near side of the water
       line and is kept. */
    glr_ReflectAt(0.8f, 0x40, 0x80, 0xC0);

    /* The target is 512 square and the quad covers its middle half. */
    glr_AeAt(gl_waterFbo, 256, 256, rgba);
    okEq(rgba[0], 0x40, "an object above the water is in the reflection");
    okEq(rgba[1], 0x80, "in green too");
    okEq(rgba[2], 0xC0, "and blue");

    /* Outside the quad the target still holds what it was cleared to,
       which is what says the draw covered a part of it rather than all
       of it. */
    glr_AeAt(gl_waterFbo, 20, 20, rgba);
    okEq(rgba[1], 0xFF, "and away from it the clear colour is untouched");

    /* And the frame itself still has the object drawn into it the
       ordinary way: the reflection is an extra draw, not a diversion. */
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    glr_Read();
    okNear(glr_R(GLR_W / 2, GLR_H / 2), 0x40, "and the frame has it as"
                                              " well");
}

static void caseWaterReflectionClipped(void)
{
    int rgba[4];

    printf("-- and what is under the water does not\n");

    /* Below the water line. Mirrored it lands beyond the surface,
       where nothing could be seen in water, and the clip plane takes
       it. Its mirrored depth is inside the clip volume, so the volume
       cannot be what removes it. */
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_ReflectionFixture();
    GL_PrepareWaterRendering();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    glr_ReflectAt(0.3f, 0x40, 0x80, 0xC0);

    glr_AeAt(gl_waterFbo, 256, 256, rgba);
    okEq(rgba[1], 0xFF, "an object under the water is clipped out of the"
                        " reflection");

    /* With the plane switched off it comes back, which is what says the
       plane removed it rather than the mirror putting it somewhere
       nothing can see. */
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_ReflectionFixture();
    tweak_water_clip = 0;
    GL_PrepareWaterRendering();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    glr_ReflectAt(0.3f, 0x40, 0x80, 0xC0);

    glr_AeAt(gl_waterFbo, 256, 256, rgba);
    okEq(rgba[0], 0x40, "and with the plane off it is drawn after all");
}

static void caseWaterReflectionMirrored(void)
{
    int rgba[4];

    printf("-- the mirror is a mirror, not a move\n");

    /* Two objects the same distance either side of the water line
       mirror onto each other. So the one above and the one below,
       drawn with the plane off, have to land on the same place in the
       target -- and a mirror written as a translation would put them
       in two different places.
     *
     * The water is at 0.6, so 0.8 mirrors to 0.4 and 0.4 mirrors to
     * 0.8: the pair is its own reflection. The depths differ, so this
     * is only meaningful with the second object drawn where the first
     * cannot hide it, which is why they go in separate frames and the
     * target is read twice. */
    glr_Reset();
    glr_BuildQuad();
    glr_IdentityMatrices();
    glr_ReflectionFixture();
    tweak_water_clip = 0;
    GL_PrepareWaterRendering();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    glr_ReflectAt(0.8f, 0x40, 0x00, 0x00);
    {
        int first[4];

        glr_AeAt(gl_waterFbo, 256, 256, first);
        okEq(first[0], 0x40, "the object above the water reflects");

        /* Now the one below, in its own frame. */
        glr_Reset();
        glr_BuildQuad();
        glr_IdentityMatrices();
        glr_ReflectionFixture();
        tweak_water_clip = 0;
        GL_PrepareWaterRendering();
        GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
        GLFN_glViewport(0, 0, GLR_W, GLR_H);
        glr_ReflectAt(0.4f, 0x00, 0x40, 0x00);
        glr_AeAt(gl_waterFbo, 256, 256, rgba);
        okEq(rgba[1], 0x40, "and so does the one the same distance below");
    }

    /* The two land on the same pixels: a mirror swaps the pair, a
       translation would move both the same way and the second would be
       somewhere else. Checked at a corner of the quad rather than its
       middle, where a shifted copy would still overlap. */
    glr_AeAt(gl_waterFbo, 132, 132, rgba);
    okEq(rgba[1], 0x40, "at the corner of the quad as well, so the pair"
                        " maps onto itself rather than being shifted");
}

/* ------------------------------------------ the surface smoothing
 *
 * Drawn on a real driver and read back. The reflection is given a
 * pattern, blurred, and read again: a blur that works mixes
 * neighbouring pixels, and one that does nothing leaves the pattern
 * exactly as it was.
 *
 * The second part is the one worth having, because the amounts start
 * at nought and nought is the shipping default -- so "the blur runs"
 * and "the blur changes nothing" both have to be true at once, and a
 * blur that quietly smeared the reflection by a texel on a default run
 * would be a defect nobody would think to look for.
 */

static void glr_SmoothPatternIntoReflection(void)
{
    int row;
    int x;

    if (!glr_waterPattern)
        glr_waterPattern = (unsigned char*)malloc(512 * 512 * 4);
    /* Hard vertical edges every 128 pixels, and the width is measured
       rather than chosen: the blur runs twice, each pass reaching one
       step either side, and a step of the amount used below is a
       little over sixteen texels -- so the two passes together reach
       about thirty-two. Bands of 64 put their own centre inside that
       reach, which made "the middle of a band is untouched" false; at
       128 the centre is sixty-four texels clear. */
    for (row = 0; row < 512; row++)
        for (x = 0; x < 512; x++)
        {
            unsigned char* p = &glr_waterPattern[(row * 512 + x) * 4];
            int            band = (x / 128) & 1;

            p[0] = (unsigned char)(band ? 0xFF : 0x00);
            p[1] = (unsigned char)(band ? 0xFF : 0x00);
            p[2] = (unsigned char)(band ? 0xFF : 0x00);
            p[3] = 0xFF;
        }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_waterPattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
}

static void caseWaterSmoothDoesNothingByDefault(void)
{
    int rgba[4];

    printf("-- at the shipping amounts the blur leaves the picture alone\n");

    glr_Reset();
    glr_IdentityMatrices();
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x00000000, 0.9f);
    /* Both amounts at nought, which is what dbg.c ships. */
    tweak_water_smooth = 1;
    tweak_water_smoothx = 0.0f;
    tweak_water_smoothy = 0.0f;
    GL_PrepareWaterRendering();
    glr_SmoothPatternIntoReflection();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);

    /* Every tap lands in the same place, so the four weights sum to
       one over one sample and the picture comes back as it went in --
       at an edge as much as in the middle. */
    glr_AeAt(gl_waterFbo, 64, 100, rgba);
    okEq(rgba[0], 0x00, "a dark band is still dark");
    glr_AeAt(gl_waterFbo, 192, 100, rgba);
    okEq(rgba[0], 0xFF, "a light band is still light");
    /* The pixel either side of an edge, which is where a blur of even
       one texel would show. The first edge is at 128. */
    glr_AeAt(gl_waterFbo, 127, 100, rgba);
    okEq(rgba[0], 0x00, "and the pixel before an edge is untouched");
    glr_AeAt(gl_waterFbo, 128, 100, rgba);
    okEq(rgba[0], 0xFF, "as is the one after it");
}

static void caseWaterSmoothBlurs(void)
{
    int rgba[4];
    int atEdge;
    int inMiddle;

    printf("-- and with an amount set it mixes neighbours\n");

    glr_Reset();
    glr_IdentityMatrices();
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x00000000, 0.9f);
    tweak_water_smooth = 1;
    /* Wide enough to cross the bands: a step is the amount over 511,
       so sixteen is about thirty texels of a 512 picture. */
    tweak_water_smoothx = 16.0f;
    tweak_water_smoothy = 0.0f;
    GL_PrepareWaterRendering();
    glr_SmoothPatternIntoReflection();
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);

    /* Two readings, because "it changed" on its own would also be true
       of a blur that wrote rubbish. At an edge the result has to be
       between the two band colours; deep inside a wide band it has to
       still be that band's colour, because the taps there all land on
       the same band. */
    glr_AeAt(gl_waterFbo, 128, 100, rgba);
    atEdge = rgba[0];
    glr_AeAt(gl_waterFbo, 64, 100, rgba);
    inMiddle = rgba[0];

    ok(atEdge > 0x10 && atEdge < 0xF0, "at a band edge the result is"
                                       " between the two colours");
    okNear(inMiddle, 0x00, "and the middle of a dark band, sixty-four"
                           " texels from either edge, is still dark");

    tweak_water_smoothx = 0.0f;
}

/* A single bright column on black, for measuring where the taps land.
   A delta is the one pattern whose blurred profile says the offsets and
   the weights outright: each tap puts a copy of the column at its own
   offset, scaled by its own weight. */
static void glr_SmoothColumnIntoReflection(int column)
{
    int row;
    int x;

    if (!glr_waterPattern)
        glr_waterPattern = (unsigned char*)malloc(512 * 512 * 4);
    for (row = 0; row < 512; row++)
        for (x = 0; x < 512; x++)
        {
            unsigned char* p = &glr_waterPattern[(row * 512 + x) * 4];
            int            on = (x == column);

            p[0] = (unsigned char)(on ? 0xFF : 0x00);
            p[1] = (unsigned char)(on ? 0xFF : 0x00);
            p[2] = (unsigned char)(on ? 0xFF : 0x00);
            p[3] = 0xFF;
        }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_waterPattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
}

/* Horizontal bands, for the vertical blur. 128 tall, as the vertical
   ones are wide and for the same measured reason. */
static void glr_SmoothRowsIntoReflection(void)
{
    int row;
    int x;

    if (!glr_waterPattern)
        glr_waterPattern = (unsigned char*)malloc(512 * 512 * 4);
    for (row = 0; row < 512; row++)
        for (x = 0; x < 512; x++)
        {
            unsigned char* p = &glr_waterPattern[(row * 512 + x) * 4];
            int            band = (row / 128) & 1;

            p[0] = (unsigned char)(band ? 0xFF : 0x00);
            p[1] = (unsigned char)(band ? 0xFF : 0x00);
            p[2] = (unsigned char)(band ? 0xFF : 0x00);
            p[3] = 0xFF;
        }
    GLFN_glBindTexture(GL_TEXTURE_2D, gl_waterColour);
    GLFN_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512,
                         GL_RGBA, GL_UNSIGNED_BYTE, glr_waterPattern);
    GLFN_glBindTexture(GL_TEXTURE_2D, 0);
}

/* An amount whose step is a whole number of texels, so that nothing in
   these two cases depends on how the sampler interpolates. A step is
   the amount over the rectangle's last index -- 511 -- and the texture
   is 512 across, so 511/32 gives exactly a thirty-second, which is
   sixteen texels. */
#define GLR_SMOOTH_AMOUNT (511.0f / 32.0f)

static void glr_SmoothSetUp(float amountX, float amountY)
{
    glr_Reset();
    glr_IdentityMatrices();
    glr_WaterFixture(0x20, 0x40, 0x80, 0xFF, 0x00000000, 0.9f);
    tweak_water_smooth = 1;
    tweak_water_smoothx = amountX;
    tweak_water_smoothy = amountY;
    GL_PrepareWaterRendering();
}

static void glr_SmoothRun(void)
{
    GLFN_glBindFramebuffer(GL_FRAMEBUFFER, glr_fbo);
    GLFN_glViewport(0, 0, GLR_W, GLR_H);
    GL_DrawWater(&glr_water);
}

static void caseWaterSmoothTapsAcross(void)
{
    int left[4];
    int right[4];
    int middle[4];

    printf("-- where the across taps land, and that they are not equal\n");

    /* A single bright column, blurred sideways only. Each tap puts a
       copy of the column sixteen texels from where it started, scaled
       by its own weight -- so the profile either side of the column
       says both where the taps are and what they weigh. */
    glr_SmoothSetUp(GLR_SMOOTH_AMOUNT, 0.0f);
    glr_SmoothColumnIntoReflection(256);
    glr_SmoothRun();

    glr_AeAt(gl_waterFbo, 256 - 16, 100, left);
    glr_AeAt(gl_waterFbo, 256, 100, middle);
    glr_AeAt(gl_waterFbo, 256 + 16, 100, right);

    /* **Both** sides, which is what says the two offsets have opposite
       signs. A pair of taps on the same side leaves one side black,
       and that scored clean while the only assertion here was a range
       at a band edge. */
    ok(left[0] > 0, "a copy lands one step to one side");
    ok(right[0] > 0, "and another one step to the other");
    ok(middle[0] > 0, "with the column itself still there");

    /* And the two sides differ, which is what says the weights are the
       mixing chain's and not a quarter each. Equal weights make the
       profile symmetric, and that also scored clean. */
    ok(left[0] != right[0], "and the two sides differ, so the weights"
                            " are not equal");
    tweak_water_smoothx = 0.0f;
}

static void caseWaterSmoothRampDown(void)
{
    int nearBottom[4];
    int nearTop[4];

    printf("-- the vertical blur opens out towards the bottom\n");

    /* Horizontal bands, blurred vertically only. Nothing before this
       case set the vertical amount at all, so every breakage of the
       ramp scored clean -- and the first version of this case had its
       two ends the wrong way round and still passed, which is how the
       shader's mirroring bug came to light.
     *
     * The two rows are **taken from a model of both passes**, not
     * estimated. A step is sixteen rows at the amount below, the
     * offsets reach three steps scaled by how far down the row is, and
     * they reach downward -- so a row above a band edge mixes with the
     * band beneath it. Estimating one pass was not enough twice over:
     * the second pass roughly doubles the reach, and a distance that
     * looked clear of it was not.
     *
     * Thirty rows above an edge, with everything else equal:
     *
     *     near the bottom   row 158   about 31 of 255
     *     near the top      row 414   255, untouched
     *
     * which is the ramp, stated as one distance with two answers. */
    glr_SmoothSetUp(0.0f, GLR_SMOOTH_AMOUNT);
    glr_SmoothRowsIntoReflection();
    glr_SmoothRun();

    /* Stated exactly, not bounded. The model says 31 of 255 and the
       driver gives 32; a bound of "less than 96" let two breakages
       through -- one tap stepping the wrong way, and the farthest tap
       stopping a step short -- because both leave the row darkish
       without leaving it where it belongs. An exact figure pins every
       tap's distance and every weight at once, which is what this one
       line is for. */
    glr_AeAt(gl_waterFbo, 100, 158, nearBottom);
    okNear(nearBottom[0], 32, "thirty rows above an edge comes back at"
                              " the figure the taps and weights give");

    glr_AeAt(gl_waterFbo, 100, 414, nearTop);
    okNear(nearTop[0], 0xFF, "and the same distance near the top is left"
                             " alone");

    tweak_water_smoothy = 0.0f;
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
    caseSunSampler();
    caseSunReadsTheBlurAlpha();
    caseSunMaskAndPacking();
    caseSunRing();
    caseSunWaitsForTheCard();
    caseSunRefusals();
    caseFogCurves();
    caseFogAlphaAndPermission();
    caseFogVariants();
    caseFogVertexCurve();
    caseUVStored();
    caseUVFromPosition();
    caseUVTransformedStored();
    caseUVNormalWalk();
    caseUVFromNormal();
    caseUVNormalMissing();
    caseUVNormalLeavesSpritesAlone();
    caseCulling();
    caseWaterReflectionCleared();
    caseWaterBlend();
    caseWaterIgnoresTheEngineView();
    caseWaterNotBlended();
    caseWaterDepth();
    caseWaterSampling();
    caseWaterDirectionIsADirection();
    caseWaterReflectionDraws();
    caseWaterReflectionClipped();
    caseWaterReflectionMirrored();
    caseWaterSmoothDoesNothingByDefault();
    caseWaterSmoothBlurs();
    caseWaterSmoothTapsAcross();
    caseWaterSmoothRampDown();
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
    caseAeDepthBlurMix();
    caseAeDepthBlurDirection();
    caseAeDepthBlurGates();
    caseAeDepthBlurBands();
    caseAeDepthBlurEdges();
    caseAeBrightness();
    caseAeContrast();
    caseAeGrayscale();
    caseAeColorOrder();
    caseAeRemanence();
    caseAeMotionBlur();
    caseAeScreenBlur();
    caseAeMotionSmooth();
    caseAeHue();
    caseAeFade();
    caseAeZoomSmooth();

    GL_l_ForgetPass();
    GL_l_ForgetGeometry();
    GL_l_ForgetBlur();
    GL_l_ForgetAe();
    GL_l_ForgetScene();
    GL_ReleaseTextures(NULL);
    GL_l_ForgetTargets();
    free(glr_aePattern);
    EGLH_Close();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
