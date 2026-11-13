/* strings.c, frozen copy against rewrite.
 *
 * Both copies are in one binary with their own symbols. The world --
 * the string object, its letters, its string records, the global string
 * slots, the font table, the 3D-string list -- is this file's, shared
 * by both, and rebuilt from scratch before each side runs so the two
 * see identical state. After each run the whole world is captured as
 * bytes along with whatever the call returned and the ordered list of
 * engine calls it made, and the two captures are compared.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <sys/mman.h>

#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../ai.h"
#include "../gameobject.h"
#include "../textures.h"
#include "../inputoutput.h"
#include "../win/wininputoutput.h"
#include "../win/wingdi.h"
#include "../strings.h"
#include "../graphicsobjects/geo.h"
#include "strings_mocks.h"

void STRCASE_RunBase(int op, int a, int b, int c, int d, int e, float f, const char* s);
void STRCASE_RunNew(int op, int a, int b, int c, int d, int e, float f, const char* s);

extern int BASE_STR_si_Id;
extern int NEW_STR_si_Id;
extern char* BASE_pcINO_ControlsReadFormatBuffer;
extern char* NEW_pcINO_ControlsReadFormatBuffer;
extern int BASE_pcINO_ControlsReadFormatBufferSize;
extern int NEW_pcINO_ControlsReadFormatBufferSize;

#define LETTERS 256
#define STRINGS 4
#define FONTS   8
#define QUADS   24

int STRCASE_ri[8];
float STRCASE_rf[8];
char STRCASE_rs[1024];

struct sGEOSTRStruct* STRW_str;
struct sGameObjectMain* STRW_gao;
struct sGEOMAT* STRW_mat;
struct sEngTexIntermediateStruct* STRW_tex;
struct sFontData STRW_fontBuf;

static tGroIntf STRW_strIntf;
static tGroIntf STRW_groIntf;
static tGroIntf STRW_matIntf;

static tGEOSTRStruct STRW_strBuf;
static tGEOSTRLetter STRW_letters[LETTERS];
static tGEOSTRStringData STRW_sd[STRINGS];
static tFontData STRW_fonts[FONTS];

static tGameObjectMain STRW_gaoBuf;
static tGameObjectBasic STRW_gaoBasic;
static tGameObjectVisualObject STRW_gaoVis;
static tGameObjectMain STRW_goBuf;
static tGameObjectBasic STRW_goBasic;
static tGameObjectVisualObject STRW_goVis;

#define MATS 9
static tGEOMAT STRW_mats[MATS];
static tGEOMAT* STRW_multiList[2];
static tMultiTextureElemStruct* STRW_multiTex;
static tGroIntf STRW_matIntf4;
static tGroIntf STRW_matIntf5;
int g_matVariant;
static tEngTexIntermediateStruct STRW_texBuf;
static tEngTexListElemStruct STRW_texElems[4];

static tGEOGRO STRW_groBuf;
static tGeoUVElem STRW_uvs[QUADS * 4];
static tGeoTriangleChunks STRW_chunks;

static tGdi_DisplayData STRW_dd;

/* ---- the capture ------------------------------------------------- */

typedef struct sCapture
{
    tGEOSTRStruct str;
    tGEOSTRLetter letters[LETTERS];
    tGEOSTRStringData sd[STRINGS];
    tFontData fonts[FONTS];
    tStrGlobalStruct globals[24];
    tStrFontAltKey altKeys[16];
    tGeoUVElem uvs[QUADS * 4];
    tGameObjectVisualObject goVis;
    tGameObjectVisualObject gaoVis;
    tFontData fontBuf;
    char keyNames[256];
    char rs[1024];
    int ri[8];
    float rf[8];
    int chunkMaterial;
    int altKeyNum;
    int fontNum;
    int fontMax;
    int fontOffset;
    int curEffect;
    int listNum;
    int listIdx[64];
    int ctlMode;
    int ctlSubmode;
    int logLen;
    char log[STRMOCK_LOGMAX][96];
} tCapture;

static tCapture g_base;
static tCapture g_new;

static int g_fails;
static int g_cases;
static char g_case[192];

static int STRMOCK_SetIdentity(Mtx4x4* m) { return 0; }
static void STRMOCK_SetProj(tDDCamStruct* c) { }

static void worldInit(void)
{
    int i;

    STRW_strIntf.id = 10;
    STRW_groIntf.id = 1;
    STRW_matIntf.id = 3;

    memset(&STRW_strBuf, 0, sizeof(STRW_strBuf));
    memset(STRW_letters, 0, sizeof(STRW_letters));
    memset(STRW_sd, 0, sizeof(STRW_sd));

    STRW_strBuf.intf = &STRW_strIntf;
    STRW_strBuf.unkNum = LETTERS;
    STRW_strBuf.stringNum = STRINGS;
    STRW_strBuf.maxEffectsNum = 32;
    STRW_strBuf.effectsNum = 4;
    for (i = 0; i < 4; i++)
    {
        STRW_strBuf.effect[i].effectID = i;
        STRW_strBuf.effect[i].stringid = i % 3;
        STRW_strBuf.effect[i].index = 2 + i;
        STRW_strBuf.effect[i].number = 3 + i;
        STRW_strBuf.effect[i].flags = 0x11223344 + i;
        STRW_strBuf.effect[i].f1 = 1.0f + (float)i;
        STRW_strBuf.effect[i].timeElapsed = 2.0f + (float)i;
        STRW_strBuf.effect[i].color1 = 3.0f + (float)i;
        STRW_strBuf.effect[i].color2 = 4.0f + (float)i;
        STRW_strBuf.effect[i].f3 = 5.0f + (float)i;
    }
    STRW_strBuf.width = 640;
    STRW_strBuf.height = 480;
    STRW_strBuf.letters = STRW_letters;
    STRW_strBuf.stringData = STRW_sd;
    STRW_strBuf.fontMaterialID = -1;
    STRW_strBuf.fontDesc = 0;
    STRW_str = &STRW_strBuf;
    for (i = 0; i < 24; i++)
        STRW_strBuf.shorts[i] = (int16_t)(3 + i);

    /* four strings: 20, 10, 6 and 0 letters, laid out back to back */
    STRW_sd[0].index = 0;  STRW_sd[0].number = 20; STRW_sd[0].number2 = 20;
    STRW_sd[1].index = 20; STRW_sd[1].number = 10; STRW_sd[1].number2 = 10;
    STRW_sd[2].index = 30; STRW_sd[2].number = 6;  STRW_sd[2].number2 = 6;
    STRW_sd[3].index = 36; STRW_sd[3].number = 0;  STRW_sd[3].number2 = 0;
    for (i = 0; i < STRINGS; i++)
    {
        STRW_sd[i].flags = (int16_t)(i == 1 ? 0x10 : 0);
        STRW_sd[i].index2 = (uint16_t)(STRW_sd[i].number > 4 ? 2 : 0);
        STRW_sd[i].number2 = (uint16_t)(STRW_sd[i].number > 4
                                        ? STRW_sd[i].number - 3 : STRW_sd[i].number);
        STRW_sd[i].pivotPoint.x = 0.0f;
        STRW_sd[i].pivotPoint.y = 0.0f;
        STRW_sd[i].pivotPoint.z = 0.0f;
    }

    /* letters: a readable pattern with pages, marks and line breaks */
    for (i = 0; i < 40; i++)
    {
        uint32_t page = (uint32_t)((i * 7 + 14) % 26);
        uint32_t mark = (uint32_t)((i % 7) == 3 ? 0xF : (i % 7));
        uint32_t ch = (uint32_t)(i % 9 == 4 ? ' ' : 'a' + (i % 26));
        int lastOfString = (i == 19 || i == 29 || i == 35);

        STRW_letters[i].flagsAndChar = ch
                                     | (page << 12)
                                     | (mark << 20)
                                     | (uint32_t)((i % 6) == 5 && !lastOfString
                                                  ? 0x20000000u : 0u)
                                     | (uint32_t)(i == 0 || i == 20 || i == 31
                                                  ? 0x10000000u : 0u);
        STRW_letters[i].x = (int16_t)(i * 7);
        STRW_letters[i].y = (int16_t)(i % 3 * 11);
        STRW_letters[i].width = (int16_t)(5 + i % 4);
        STRW_letters[i].height = (int16_t)(9 + i % 2);
        STRW_letters[i].textColor = 0x11223344u + (uint32_t)i;
    }

    /* the fonts */
    memset(STRW_fonts, 0, sizeof(STRW_fonts));
    for (i = 0; i < 2; i++)
    {
        int g;

        STRW_fonts[i].texKey = 0x1000 + i;
        STRW_fonts[i].texKeyAlternative = 0x2000 + i;
        STRW_fonts[i].mat = 0;
        STRW_fonts[i].width = (int16_t)(128 + i);
        STRW_fonts[i].height = (int16_t)(64 + i);
        for (g = 0; g < 224; g++)
        {
            STRW_fonts[i].fge[g].x = 0.01f * (float)(g % 17);
            STRW_fonts[i].fge[g].y = 0.01f * (float)(g % 17) + 0.05f;
            STRW_fonts[i].fge[g].w = 0.02f * (float)(g % 13);
            STRW_fonts[i].fge[g].h = 0.02f * (float)(g % 13) + 0.07f;
        }
    }
    STR_sgpst_Font = STRW_fonts;
    STR_sgl_NumberOfFont = 2;
    STR_sgl_MaxNumberOfFont = FONTS;
    memset(STR_FontAltKey, 0, sizeof(STR_FontAltKey));
    STR_FontAltKeyNumber = 0;
    memset(&STRW_fontBuf, 0, sizeof(STRW_fontBuf));

    /* the texture list, so STR_pst_GetFontDesc can resolve a material */
    memset(STRW_texElems, 0, sizeof(STRW_texElems));
    STRW_texElems[0].texKey = 0x1000;
    STRW_texElems[1].texKey = 0x2001;
    STRW_texElems[2].texKey = 0x9999;
    STRW_texElems[2].texElemFlag = 0x40;
    STRW_texElems[2].palettedData.textureID = 1;
    STRW_texElems[3].texKey = 0xBEEF;
    TEX_gst_GlobalList.texElems = STRW_texElems;

    STRW_matIntf4.id = 4;
    STRW_matIntf5.id = 5;
    memset(STRW_mats, 0, sizeof(STRW_mats));
    for (i = 0; i < MATS; i++)
        STRW_mats[i].intf = &STRW_matIntf;
    STRW_mats[0].idxOrPtr = 0;                 /* texKey 0x1000, font 0    */
    STRW_mats[1].idxOrPtr = 1;                 /* texKey 0x2001, font 1 alt*/
    STRW_mats[2].idxOrPtr = 2;                 /* paletted, redirected     */
    STRW_mats[3].idxOrPtr = 3;                 /* texKey nothing has       */
    STRW_mats[4].idxOrPtr = -1;                /* no texture at all        */
    /* sGEOMAT keeps this pointer in an int, which is the 32-bit engine's
       layout, so the block has to live below 4GB for the type-5 path to
       be reachable at all here. Where it cannot, the path is skipped. */
    if (!STRW_multiTex)
        STRW_multiTex = (tMultiTextureElemStruct*)mmap(
            NULL, 4096, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    if (STRW_multiTex == MAP_FAILED)
        STRW_multiTex = 0;
    if (STRW_multiTex)
        STRW_multiTex->texElemIdx = 1;
    STRW_mats[5].intf = &STRW_matIntf5;
    STRW_mats[5].idxOrPtr = (int)(intptr_t)STRW_multiTex;
    STRW_mats[6].intf = &STRW_matIntf5;
    STRW_mats[6].idxOrPtr = 0;                 /* a null multi-texture     */
    STRW_multiList[0] = &STRW_mats[0];
    STRW_multiList[1] = &STRW_mats[3];
    STRW_mats[7].intf = &STRW_matIntf4;
    STRW_mats[7].multiMaterialNumIfType4ThenAmbiantColor = 2;
    STRW_mats[7].multiMaterialPtrIfType4ThenDiffuseColor = STRW_multiList;
    STRW_mats[8].idxOrPtr = -1;                /* found by its material    */
    STRW_fonts[1].mat = &STRW_mats[8];
    STRW_mat = &STRW_mats[g_matVariant < 0 || g_matVariant >= MATS ? 0 : g_matVariant];

    memset(&STRW_texBuf, 0, sizeof(STRW_texBuf));
    STRW_texBuf.imgDataPtr = (char*)STRW_letters;
    STRW_texBuf.realWidth = 256;
    STRW_texBuf.realHeight = 128;
    STRW_texBuf.tp.magicParams.fontDescriptorKey = STRMOCK_fontKey;
    STRW_tex = &STRW_texBuf;

    /* the game objects */
    memset(&STRW_gaoVis, 0, sizeof(STRW_gaoVis));
    STRW_gaoVis.GROGEOOrSomethingElse = (struct sGroGeneric*)&STRW_strBuf;
    STRW_gaoVis.GROMAT = (struct sGroGeneric*)&STRW_mats[0];
    STRW_gaoBasic.actorObjectOrVisualObject = &STRW_gaoVis;
    memset(&STRW_gaoBuf, 0, sizeof(STRW_gaoBuf));
    STRW_gaoBuf.flags = 0x4000;
    STRW_gaoBuf.basic = &STRW_gaoBasic;
    STRW_gaoBuf.extended = 0;
    STRW_gao = &STRW_gaoBuf;

    memset(&STRW_goVis, 0, sizeof(STRW_goVis));
    STRW_goVis.GROMAT = (struct sGroGeneric*)&STRW_mats[0];
    STRW_goBasic.actorObjectOrVisualObject = &STRW_goVis;
    memset(&STRW_goBuf, 0, sizeof(STRW_goBuf));
    STRW_goBuf.basic = &STRW_goBasic;
    STR_sgpst_GO = &STRW_goBuf;

    /* the 3D-string geometry */
    for (i = 0; i < QUADS * 4; i++)
    {
        STRW_uvs[i].u = 0.5f + (float)i;
        STRW_uvs[i].v = 1.5f + (float)i;
    }
    memset(&STRW_chunks, 0, sizeof(STRW_chunks));
    memset(&STRW_groBuf, 0, sizeof(STRW_groBuf));
    STRW_groBuf.intf = &STRW_groIntf;
    STRW_groBuf.uvNum = QUADS * 4;
    STRW_groBuf.UVs = STRW_uvs;
    STRW_groBuf.triangleChunks = &STRW_chunks;
    STRW_groBuf.d3dIndexBuf = 0;
    STRMOCK_gro = (struct sGroGeneric*)&STRW_groBuf;

    /* the display */
    memset(&STRW_dd, 0, sizeof(STRW_dd));
    STRW_dd.ct.cam.width = 640.0f;
    STRW_dd.ct.cam.height = 480.0f;
    STRW_dd.SetViewMatrix = STRMOCK_SetIdentity;
    STRW_dd.SetProjectionMatrix = STRMOCK_SetProj;
    gpst_DX_stDD = &STRW_dd;

    /* the character table and the global string slots */
    for (i = 0; i < 256; i++)
        STR_sgac_CharTable[i] = i;
    memset(STR_sgaac_Global, 0, sizeof(STR_sgaac_Global));
    strcpy(STR_sgaac_Global[0].str, "NAME");
    strcpy(STR_sgaac_Global[1].str, "a longer one!!!");
    strcpy(STR_sgaac_Global[2].str, "");
    strcpy(STR_sgaac_Global[3].str, "x");

    STR_spst_CurEffect = 0;
    STR_3DString_Number = 0;
    memset(STR_3DString_Obj, 0, sizeof(STR_3DString_Obj));

    /* the key mapping */
    for (i = 0; i < 18; i++)
        snprintf(buttonNames[i].str, sizeof(buttonNames[i].str), "<btn%d>", i);
    memset(pcINO_KeyNamesToRet, 0, sizeof(pcINO_KeyNamesToRet));
    pcINO_SelectedControlMode = 3;
    pcINO_SelectedControlSubmode = 4;

    BASE_STR_si_Id = 0;
    NEW_STR_si_Id = 0;

    STRMOCK_Reset();
    memset(STRCASE_ri, 0, sizeof(STRCASE_ri));
    memset(STRCASE_rf, 0, sizeof(STRCASE_rf));
    memset(STRCASE_rs, 0, sizeof(STRCASE_rs));
}

static char STRW_keyMaps[32];

static void keyMapsSet(int variant)
{
    int i;

    if (variant < 0)
    {
        pcINO_keyMaps = 0;
        return;
    }
    for (i = 0; i < 32; i++)
    {
        if (variant == 0)
            STRW_keyMaps[i] = (char)(i % 17);
        else if (variant == 1)
            STRW_keyMaps[i] = (char)((i & 1) ? -1 : (i % 17));
        else if (variant == 3)
            STRW_keyMaps[i] = (char)((i % 3 == 0) ? -1 : (i % 17));
        else
            STRW_keyMaps[i] = (char)-2;
    }
    pcINO_keyMaps = STRW_keyMaps;
}

static void capture(tCapture* cap)
{
    int i;

    memset(cap, 0, sizeof(*cap));
    cap->str = STRW_strBuf;
    memcpy(cap->letters, STRW_letters, sizeof(cap->letters));
    memcpy(cap->sd, STRW_sd, sizeof(cap->sd));
    memcpy(cap->fonts, STRW_fonts, sizeof(cap->fonts));
    memcpy(cap->globals, STR_sgaac_Global, sizeof(cap->globals));
    memcpy(cap->altKeys, STR_FontAltKey, sizeof(cap->altKeys));
    memcpy(cap->uvs, STRW_uvs, sizeof(cap->uvs));
    cap->goVis = STRW_goVis;
    cap->gaoVis = STRW_gaoVis;
    cap->fontBuf = STRW_fontBuf;
    memcpy(cap->keyNames, pcINO_KeyNamesToRet, sizeof(cap->keyNames));
    memcpy(cap->rs, STRCASE_rs, sizeof(cap->rs));
    memcpy(cap->ri, STRCASE_ri, sizeof(cap->ri));
    memcpy(cap->rf, STRCASE_rf, sizeof(cap->rf));
    cap->chunkMaterial = STRW_chunks.materialNum;
    cap->altKeyNum = STR_FontAltKeyNumber;
    cap->fontNum = STR_sgl_NumberOfFont;
    cap->fontMax = STR_sgl_MaxNumberOfFont;
    cap->fontOffset = STR_sgpst_Font == STRW_fonts ? 0 : 1;
    cap->curEffect = STR_spst_CurEffect
                   ? (int)(STR_spst_CurEffect - STRW_strBuf.effect) : -1;
    cap->listNum = STR_3DString_Number;
    for (i = 0; i < 64; i++)
        cap->listIdx[i] = STR_3DString_Obj[i] == STRW_gao ? 1 : 0;
    cap->ctlMode = pcINO_SelectedControlMode;
    cap->ctlSubmode = pcINO_SelectedControlSubmode;
    cap->logLen = STRMOCK_logLen;
    memcpy(cap->log, STRMOCK_log, sizeof(cap->log));

    /* the string's own font pointer is only comparable as an index */
    if (STRW_strBuf.fontDesc)
        cap->str.fontDesc = (struct sFontData*)(intptr_t)
            (1 + (STRW_strBuf.fontDesc - STRW_fonts));
}

static void compareCapture(void)
{
    int i;
    int bad = 0;

#define FIELD(name, fmt, expr) \
    do { if (memcmp(&g_base.name, &g_new.name, sizeof(g_base.name)) != 0) { \
        printf("    DIFF %s | " #name "\n", g_case); bad = 1; } } while (0)

    FIELD(str, 0, 0);
    FIELD(letters, 0, 0);
    FIELD(sd, 0, 0);
    FIELD(fonts, 0, 0);
    FIELD(globals, 0, 0);
    FIELD(altKeys, 0, 0);
    FIELD(uvs, 0, 0);
    FIELD(goVis, 0, 0);
    FIELD(gaoVis, 0, 0);
    FIELD(fontBuf, 0, 0);
    FIELD(keyNames, 0, 0);
    FIELD(ri, 0, 0);
    FIELD(rf, 0, 0);
    FIELD(chunkMaterial, 0, 0);
    FIELD(altKeyNum, 0, 0);
    FIELD(fontNum, 0, 0);
    FIELD(fontMax, 0, 0);
    FIELD(fontOffset, 0, 0);
    FIELD(curEffect, 0, 0);
    FIELD(listNum, 0, 0);
    FIELD(listIdx, 0, 0);
    FIELD(ctlMode, 0, 0);
    FIELD(ctlSubmode, 0, 0);
#undef FIELD

    if (memcmp(g_base.rs, g_new.rs, sizeof(g_base.rs)) != 0)
    {
        printf("    DIFF %s | text: old \"%s\" new \"%s\"\n",
               g_case, g_base.rs, g_new.rs);
        bad = 1;
    }

    if (g_base.logLen != g_new.logLen)
    {
        printf("    DIFF %s | %d engine calls against %d\n",
               g_case, g_base.logLen, g_new.logLen);
        bad = 1;
    }
    else
    {
        for (i = 0; i < g_base.logLen; i++)
        {
            if (strcmp(g_base.log[i], g_new.log[i]) != 0)
            {
                printf("    DIFF %s | call %d: old \"%s\" new \"%s\"\n",
                       g_case, i, g_base.log[i], g_new.log[i]);
                bad = 1;
                break;
            }
        }
    }

    if (bad)
    {
        printf("  FAIL %s\n", g_case);
        g_fails++;
    }
}

static int g_keyVariant;

static void runCase(const char* name, int op, int a, int b, int c, int d, int e,
                    float f, const char* s)
{
    snprintf(g_case, sizeof(g_case), "%s", name);
    g_cases++;

    worldInit();
    keyMapsSet(g_keyVariant);
    STRCASE_RunBase(op, a, b, c, d, e, f, s);
    capture(&g_base);

    worldInit();
    keyMapsSet(g_keyVariant);
    STRCASE_RunNew(op, a, b, c, d, e, f, s);
    capture(&g_new);

    compareCapture();
}

/* The index and count arguments every text call takes, including the
   negative forms STR_l_GetValidIndex and STR_l_GetValidNumber treat as
   sentinels: -2 is the recorded substring, -10.. and -50.. are marks. */
static const int g_idx[] = { 0, 1, 3, 19, 20, 25, -1, -2, -3, -9, -10, -11, -16, -25, -49, -50, -51, -60 };
static const int g_num[] = { -1, -2, 0, 1, 5, 20, 40, -10, -12, -20 };

static const char* g_texts[] = {
    "",
    "A",
    "hello world",
    "two\nlines",
    "crlf\r\nhere",
    "\\p3\\paged\\p0\\",
    "\\P7\\sticky and \\p2\\one",
    "\\a65\\\\a66\\\\a67\\",
    "\\d1\\ and \\d0\\",
    "\\c00FF8040\\coloured",
    "\\m5\\marked",
    "\\x0.5\\\\y0.25\\placed",
    "\\w0.1\\\\h0.2\\sized",
    "\\-dash\\nnew",
    "\\b1214\\\\p16\\A",
    "\\p16\\AFO0123?",
    "\\O2\\\\o1\\mode",
    "\\q unknown \\rest",
    "\\b24\\\\p16\\O",
    "\\d99\\",
    "0123456789abcdefghij0123456789",
    "\\P16\\\\p16\\F\\P4\\",
    "\\P15\\x\\P16\\A\\P15\\",
    "\\P15\\x\\b1214\\\\P16\\A",
    "\\P15\\\\p16\\AF\\P2\\",
    "\\z",
    "\\",
    "a\\p1\\b\\p2\\c\\p3\\d",
    "exactly fifteen",
    "sixteen chars!!!",
    "\\d2\\",
    "\\d3\\x"
};
#define TEXTS ((int)(sizeof(g_texts) / sizeof(g_texts[0])))

int main(void)
{
    char name[192];
    int i, j, k;

    setvbuf(stdout, NULL, _IONBF, 0);
    STRMOCK_BuildFont(3);

    /* the global string slots: every length through the copy loop's
       bound, and indices on both sides of the 24-slot test */
    for (i = 0; i < TEXTS; i++)
    {
        snprintf(name, sizeof(name), "GlobalSet idx=5 \"%s\"", g_texts[i]);
        runCase(name, 0, 5, 0, 0, 0, 0, 0.0f, g_texts[i]);
    }
    {
        static const char* lens[] = {
            "", "1", "12", "123", "1234", "12345", "123456", "1234567",
            "12345678", "123456789", "1234567890", "12345678901",
            "123456789012", "1234567890123", "12345678901234",
            "123456789012345", "1234567890123456", "12345678901234567",
            "123456789012345678", "1234567890123456789",
            "12345678901234567890", "123456789012345678901234567890"
        };
        for (i = 0; i < (int)(sizeof(lens) / sizeof(lens[0])); i++)
        {
            snprintf(name, sizeof(name), "GlobalSet len=%d", (int)strlen(lens[i]));
            runCase(name, 0, 7, 0, 0, 0, 0, 0.0f, lens[i]);
        }
    }
    for (i = -2; i < 27; i++)
    {
        snprintf(name, sizeof(name), "GlobalSet idx=%d", i);
        runCase(name, 0, i, 0, 0, 0, 0, 0.0f, "abcdefghijklmnopqrst");
        snprintf(name, sizeof(name), "GlobalGet idx=%d", i);
        runCase(name, 2, i, 0, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "GlobalLength idx=%d", i);
        runCase(name, 3, i, 0, 0, 0, 0, 0.0f, 0);
    }
    for (i = -1; i < 17; i++)
    {
        snprintf(name, sizeof(name), "GlobalSetChar at=%d", i);
        runCase(name, 1, 2, i, 'Z', 0, 0, 0.0f, 0);
    }

    /* the format reader, the control-name preprocessor and the length */
    for (i = 0; i < TEXTS; i++)
    {
        for (k = 0; k < 3; k++)
        {
            g_keyVariant = k == 0 ? 0 : (k == 1 ? 1 : -1);
            snprintf(name, sizeof(name), "ReadFormat km=%d \"%s\"", k, g_texts[i]);
            runCase(name, 4, 0, 0, 0, 0, 0, 0.0f, g_texts[i]);
            snprintf(name, sizeof(name), "ControlsReadFormat km=%d \"%s\"", k, g_texts[i]);
            runCase(name, 5, 0, 0, 0, 0, 0, 0.0f, g_texts[i]);
            snprintf(name, sizeof(name), "GetStringLength km=%d \"%s\"", k, g_texts[i]);
            runCase(name, 6, 0, 0, 0, 0, 0, 0.0f, g_texts[i]);
        }
        g_keyVariant = 0;
    }

    /* every character STR_GetKeyName can be given, under each mapping */
    for (k = 0; k < 5; k++)
    {
        static const int combos[] = { 0, 24, 1214, 1315, 2345, 99 };

        g_keyVariant = k == 4 ? -1 : k;
        for (i = 0; i < 256; i++)
        {
            for (j = 0; j < 6; j++)
            {
                snprintf(name, sizeof(name), "GetKeyName km=%d ch=%d combo=%d",
                         k, i, combos[j]);
                runCase(name, 60, i, combos[j], 0, 0, 0, 0.0f, 0);
            }
        }
    }
    g_keyVariant = 0;

    /* setting, inserting and parsing strings */
    for (i = 0; i < TEXTS; i++)
    {
        for (j = 0; j < STRINGS + 1; j++)
        {
            snprintf(name, sizeof(name), "SetString s=%d \"%s\"", j, g_texts[i]);
            runCase(name, 7, j, 0, 0, 0, 0, 0.0f, g_texts[i]);
        }
        for (j = 0; j < (int)(sizeof(g_idx) / sizeof(g_idx[0])); j++)
        {
            snprintf(name, sizeof(name), "InsertString at=%d \"%s\"", g_idx[j], g_texts[i]);
            runCase(name, 9, 0, g_idx[j], 0, 0, 0, 0.0f, g_texts[i]);
        }
        snprintf(name, sizeof(name), "ParseString \"%s\"", g_texts[i]);
        runCase(name, 61, 0, 0, 20, 0, 0, 0.0f, g_texts[i]);
        snprintf(name, sizeof(name), "ParseString short \"%s\"", g_texts[i]);
        runCase(name, 61, 0, 2, 3, 5, 0, 0.0f, g_texts[i]);
        snprintf(name, sizeof(name), "Init3DStringExtExt \"%s\"", g_texts[i]);
        runCase(name, 63, -1, 0, 0, 0, 0, 0.0f, g_texts[i]);
        snprintf(name, sizeof(name), "Init3DStringExtExt cap=4 \"%s\"", g_texts[i]);
        runCase(name, 63, 4, 0, 0, 0, 0, 0.0f, g_texts[i]);
    }
    for (i = 0; i < TEXTS; i++)
    {
        static const int extFlags[] = {
            0, 0x10000, 0x100000, 0x40000, 0x20000, 0x80000,
            0x1F0000, 0x80000 | 0x010000, 0x380000
        };
        for (j = 0; j < (int)(sizeof(extFlags) / sizeof(extFlags[0])); j++)
        {
            snprintf(name, sizeof(name), "SetStringExt f=%x \"%s\"", extFlags[j], g_texts[i]);
            runCase(name, 8, 0, extFlags[j], 0, 0, 0, 0.5f, g_texts[i]);
        }
    }

    /* the length change, on its own */
    for (i = -4; i <= 44; i += 2)
        for (j = 0; j < STRINGS; j++)
        {
            snprintf(name, sizeof(name), "ChangeStringLength s=%d len=%d", j, i);
            runCase(name, 12, j, i, 0, 0, 0, 0.0f, 0);
        }
    /* the lengths at and around the point where the letters left over
       for the strings after this one no longer fit */
    for (i = 216; i <= 264; i++)
        for (j = 0; j < STRINGS; j++)
        {
            snprintf(name, sizeof(name), "ChangeStringLength s=%d len=%d", j, i);
            runCase(name, 12, j, i, 0, 0, 0, 0.0f, 0);
        }

    /* the index and count sentinels, through every call that takes them */
    {
        static const int ops[] = { 14, 16, 17, 20, 21, 25, 26, 27, 28, 29,
                                   30, 31, 33, 34, 35, 36, 37, 13, 15, 18 };

        for (k = 0; k < (int)(sizeof(ops) / sizeof(ops[0])); k++)
            for (i = 0; i < (int)(sizeof(g_idx) / sizeof(g_idx[0])); i++)
                for (j = 0; j < (int)(sizeof(g_num) / sizeof(g_num[0])); j++)
                {
                    snprintf(name, sizeof(name), "op=%d first=%d num=%d",
                             ops[k], g_idx[i], g_num[j]);
                    runCase(name, ops[k], 0, g_idx[i], g_num[j], 2, 0x00FF00FFu,
                            -1.0f, 0);
                    snprintf(name, sizeof(name), "op=%d first=%d num=%d s=1",
                             ops[k], g_idx[i], g_num[j]);
                    runCase(name, ops[k], 1, g_idx[i], g_num[j], 1, 0x0F0F0F0Fu,
                            0.25f, 0);
                }
    }

    /* the single-letter getters and setters */
    for (i = 0; i < (int)(sizeof(g_idx) / sizeof(g_idx[0])); i++)
    {
        static const int ops[] = { 19, 22, 23, 24, 32, 38, 39, 40, 43, 67, 68, 69, 70 };

        for (k = 0; k < (int)(sizeof(ops) / sizeof(ops[0])); k++)
        {
            snprintf(name, sizeof(name), "op=%d arg=%d", ops[k], g_idx[i]);
            runCase(name, ops[k], 0, g_idx[i], 0x1234, 0, 0, 0.75f, 0);
            snprintf(name, sizeof(name), "op=%d arg=%d b", ops[k], g_idx[i]);
            runCase(name, ops[k], g_idx[i], 3, 7, 0, 0, 0.75f, 0);
        }
    }
    for (i = -1; i <= 40; i++)
    {
        snprintf(name, sizeof(name), "GetSubLength line=%d", i);
        runCase(name, 24, 0, i, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "GetSubLength s=1 line=%d", i);
        runCase(name, 24, 1, i, 0, 0, 0, 0.0f, 0);
    }

    /* the frame, the alignment and the recomputed layout */
    for (i = -1; i <= 4; i++)
        for (j = 0; j < STRINGS; j++)
        {
            snprintf(name, sizeof(name), "Align s=%d mode=%d", j, i);
            runCase(name, 15, j, 0, -1, i, 0, 0.0f, 0);
        }
    for (i = 0; i < (int)(sizeof(g_idx) / sizeof(g_idx[0])); i++)
        for (j = 0; j < (int)(sizeof(g_num) / sizeof(g_num[0])); j++)
        {
            snprintf(name, sizeof(name), "RecomputePos first=%d num=%d",
                     g_idx[i], g_num[j]);
            runCase(name, 62, 0, g_idx[i], g_num[j], 0, 0, 0.0f, 0);
            snprintf(name, sizeof(name), "RecomputePos s=1 first=%d num=%d",
                     g_idx[i], g_num[j]);
            runCase(name, 62, 1, g_idx[i], g_num[j], 0, 0, 0.0f, 0);
        }
    for (i = 0; i < 4; i++)
    {
        snprintf(name, sizeof(name), "SetFrame s=%d", i);
        runCase(name, 40, i, 0x2A, 0x0FFF, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "SetFramePos s=%d", i);
        runCase(name, 41, i, 0, 0, 0, 0, 0.3f, 0);
        snprintf(name, sizeof(name), "SetFrameBorder s=%d", i);
        runCase(name, 42, i, 0, 0, 0, 0, 0.125f, 0);
        snprintf(name, sizeof(name), "SetFrameColor s=%d", i);
        runCase(name, 43, i, 0x44332211, 0, 0, 0, 0.0f, 0);
    }

    /* the page change, over its three windows */
    for (i = -2; i < 600; i += (i > 248 && i < 268) ? 1 : 7)
        for (j = 0; j < 3; j++)
        {
            snprintf(name, sizeof(name), "ChangePage match=%d page=%d", i, j);
            runCase(name, 20, 0, 0, -1, j, i, 0.0f, 0);
        }

    /* the effects: every field id the two dispatchers can see */
    for (i = -4; i <= 2100; i++)
    {
        snprintf(name, sizeof(name), "EffectSetf field=%d", i);
        runCase(name, 46, 0, i, 0, 0, 0, 1.5f, 0);
        snprintf(name, sizeof(name), "EffectSetf field=%d neg", i);
        runCase(name, 46, 0, i, 0, 0, 0, -2.0f, 0);
        snprintf(name, sizeof(name), "EffectSetf field=%d zero", i);
        runCase(name, 46, 0, i, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectSeti field=%d", i);
        runCase(name, 45, 0, i, 0x33445566, 0, 0, 0.0f, 0);
    }
    for (i = -4; i <= 30; i++)
    {
        snprintf(name, sizeof(name), "EffectSeti id=%d shorts", i);
        runCase(name, 45, i, -2, 0x00070008, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectSeti id=%d delete", i);
        runCase(name, 45, i, -1, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectGeti id=%d", i);
        runCase(name, 47, -1, i, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectGeti field=%d", i);
        runCase(name, 47, 0, i, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectDel id=%d", i);
        runCase(name, 48, i, 0, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectGetId s=%d", i);
        runCase(name, 51, i, 0, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "SetCurEffect id=%d", i);
        runCase(name, 53, i, 0, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectDelAllForAString s=%d", i);
        runCase(name, 50, i, 0, 0, 0, 0, 0.0f, 0);
    }
    for (i = 0; i <= 21; i++)
    {
        snprintf(name, sizeof(name), "EffectAdd type=%d", i);
        runCase(name, 44, i, 0, 2, 5, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "EffectAdd type=%d on str", i);
        runCase(name, 44, (int)(0x80000000u | (unsigned)i), 1, 0, 3, 0, 0.0f, 0);
    }
    for (i = 0; i < (int)(sizeof(g_idx) / sizeof(g_idx[0])); i++)
        for (j = 0; j < (int)(sizeof(g_num) / sizeof(g_num[0])); j++)
        {
            snprintf(name, sizeof(name), "EffectCheck first=%d num=%d",
                     g_idx[i], g_num[j]);
            runCase(name, 52, 0, g_idx[i], g_num[j], 0, 0, 0.0f, 0);
        }
    runCase("EffectDelAll", 49, 0, 0, 0, 0, 0, 0.0f, 0);

    /* the font table */
    for (i = 0; i < 6; i++)
    {
        static const int keys[] = { 0x1000, 0x1001, 0x2000, 0x4242, 0x11223344, -1 };

        snprintf(name, sizeof(name), "AddFont key=%x", keys[i]);
        runCase(name, 54, keys[i], 0, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "GetFont key=%x", keys[i]);
        runCase(name, 55, keys[i], 0, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "FontAddAlternativeKey key=%x", keys[i]);
        runCase(name, 56, keys[i], 0x7777, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "LoadFontDescriptor key=%x", keys[i]);
        runCase(name, 58, keys[i], 0, 0, 0, 0, 0.0f, 0);
    }
    runCase("FontResolveAlternativeKey", 57, 0, 0, 0, 0, 0, 0.0f, 0);
    runCase("ReinitAll", 71, 0, 0, 0, 0, 0, 0.0f, 0);
    for (k = 0; k < MATS; k++)
    {
        g_matVariant = k;
        for (i = -1; i < 6; i++)
        {
            /* a negative sub-material index is not guarded against, so
               the multi-material shape is only given indices it has */
            if (i >= 0 || k != 7)
            {
                snprintf(name, sizeof(name), "GetFontDesc mat=%d sub=%d", k, i);
                runCase(name, 59, i, 0, 0, 0, 0, 0.0f, 0);
            }
            snprintf(name, sizeof(name), "SetSize mat=%d first=%d", k, i);
            runCase(name, 14, 0, i, -1, 0, 0, -1.0f, 0);
            snprintf(name, sizeof(name), "RecomputePos mat=%d", k);
            runCase(name, 62, 1, 0, -1, 0, 0, 0.0f, 0);
        }
    }
    g_matVariant = 0;

    /* the odds and ends */
    runCase("Reset", 66, 0, 0, 0, 0, 0, 0.0f, 0);
    runCase("3DStringList_Add", 65, 0, 0, 0, 0, 0, 0.0f, 0);
    runCase("3DStringList_Reinit", 73, 0, 0, 0, 0, 0, 0.0f, 0);
    runCase("3DStringList_Clear", 73, 1, 0, 0, 0, 0, 0.0f, 0);
    for (i = 0; i < STRINGS + 1; i++)
    {
        snprintf(name, sizeof(name), "Init3DStringFromSTR s=%d", i);
        runCase(name, 64, i, -1, 0, 0, 0, 0.0f, 0);
        snprintf(name, sizeof(name), "Init3DStringFromSTR s=%d cap", i);
        runCase(name, 64, i, 3, 0, 0, 0, 0.0f, 0);
    }
    for (i = 0; i < 3; i++)
        for (j = 0; j < TEXTS; j += 5)
        {
            snprintf(name, sizeof(name), "Init3DString v=%d \"%s\"", i, g_texts[j]);
            runCase(name, 72, i, -1, 0, 0, 0, 0.0f, g_texts[j]);
        }
    for (i = 0; i < 24; i++)
    {
        char filler[40];
        int n;

        for (n = 0; n < i; n++)
            filler[n] = (char)('A' + n % 26);
        filler[i] = 0;
        for (j = 0; j < STRINGS; j++)
        {
            snprintf(name, sizeof(name), "InsertString s=%d len=%d", j, i);
            runCase(name, 9, j, 0, 0, 0, 0, 0.0f, filler);
            snprintf(name, sizeof(name), "InsertString s=%d len=%d mid", j, i);
            runCase(name, 9, j, 3, 0, 0, 0, 0.0f, filler);
        }
    }
    for (i = 0; i < 4; i++)
    {
        snprintf(name, sizeof(name), "SetFloat s=%d", i);
        runCase(name, 10, i, 3, 0, 0, 0, 3.14159f, 0);
        snprintf(name, sizeof(name), "InsertFloat s=%d", i);
        runCase(name, 11, i, 2, 4, 0, 0, -2.5f, 0);
    }

    printf("strings.c\n");
    printf("  %d cases\n", g_cases);
    if (g_fails)
        printf("  %d case(s) differ\n", g_fails);
    else
        printf("  no behavioural differences\n");
    return g_fails ? 1 : 0;
}
