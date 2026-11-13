/* Everything strings.c refers to outside itself. The font-descriptor
   loader is given a real byte stream so the FONTDESC path runs; the
   rest record what they were called with, which is what the test
   compares between the two copies. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "strings_mocks.h"

int STRMOCK_logLen;
char STRMOCK_log[STRMOCK_LOGMAX][96];

int pcINO_SelectedControlMode;
int pcINO_SelectedControlSubmode;
char prefixOneSpace[8] = " ";

struct sGroGeneric* STRMOCK_gro;

void STRMOCK_Reset(void)
{
    STRMOCK_logLen = 0;
}

static void STRMOCK_Record(const char* fmt, ...)
{
    va_list ap;

    if (STRMOCK_logLen >= STRMOCK_LOGMAX)
        return;
    va_start(ap, fmt);
    vsnprintf(STRMOCK_log[STRMOCK_logLen], sizeof(STRMOCK_log[0]), fmt, ap);
    va_end(ap);
    STRMOCK_logLen++;
}

/* --- memory ------------------------------------------------------- */

void* MEM_p_Alloc(unsigned int size)
{
    STRMOCK_Record("MEM_p_Alloc %u", size);
    return calloc(1, size);
}

void* MEM_p_Realloc(void* ptr, size_t size)
{
    STRMOCK_Record("MEM_p_Realloc %u", (unsigned int)size);
    return realloc(ptr, size);
}

void MEM_Free(void* ptr)
{
    STRMOCK_Record("MEM_Free");
    free(ptr);
}

/* --- the display -------------------------------------------------- */

int D3D_FlatLayerOffsetX(int w, int h)
{
    STRMOCK_Record("D3D_FlatLayerOffsetX %d %d", w, h);
    return (w - h * 4 / 3) / 2;
}

int D3D_FlatLayerWidth(int w, int h)
{
    STRMOCK_Record("D3D_FlatLayerWidth %d %d", w, h);
    return h * 4 / 3;
}

void PC_FreeABackCache(struct sGEOGRO* gro)
{
    STRMOCK_Record("PC_FreeABackCache");
}

/* --- the game object tree ----------------------------------------- */

struct sGroGeneric* OBJ_p_GetGro(struct sGameObjectMain* gao)
{
    STRMOCK_Record("OBJ_p_GetGro");
    return STRMOCK_gro;
}

void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown)
{
    STRMOCK_Record("OBJ_GameObject_Remove %d", (int)refCountDown);
}

void AI_ReinitInstance(tAI* ai)
{
    STRMOCK_Record("AI_ReinitInstance");
}

void STR_Reinit(struct sGroGeneric* gro)
{
    STRMOCK_Record("STR_Reinit");
}

/* --- the input mapping -------------------------------------------- */

char* pcINO_GetFourKeyNamesCattedTogether(int a, int b, int c, int d)
{
    STRMOCK_Record("pcINO_GetFourKeyNamesCattedTogether %d %d %d %d", a, b, c, d);
    strcpy(pcINO_KeyNamesToRet, "<four>");
    return pcINO_KeyNamesToRet;
}

void INO_SetControlMode_inner_sub_48ACB0(int mode)
{
    STRMOCK_Record("INO_SetControlMode %d", mode);
    pcINO_SelectedControlMode = mode;
}

void INO_SetControlSubmode_inner_sub_48A650(int mode)
{
    STRMOCK_Record("INO_SetControlSubmode %d", mode);
    pcINO_SelectedControlSubmode = mode;
}

/* --- the big file and the loader ---------------------------------- */

/* One FONTDESC record, built to the shape STR_b_LoadFontDescriptor
   reads: the tag, a long, then (code, x, w, y, h) per glyph, then -1. */
static char STRMOCK_font[8 + 4 + 3 * 20 + 4];
int STRMOCK_fontSize;
int STRMOCK_fontKey = 0x11223344;

static void STRMOCK_PutLong(int at, int v)
{
    memcpy(&STRMOCK_font[at], &v, 4);
}

static void STRMOCK_PutFloat(int at, float v)
{
    memcpy(&STRMOCK_font[at], &v, 4);
}

void STRMOCK_BuildFont(int glyphs)
{
    int at;
    int i;

    memcpy(STRMOCK_font, "FONTDESC", 8);
    STRMOCK_PutLong(8, 0);
    at = 12;
    for (i = 0; i < glyphs; i++)
    {
        STRMOCK_PutLong(at, i == 0 ? 32 : 'A' + i);
        STRMOCK_PutFloat(at + 4, 0.1f * (float)(i + 1));
        STRMOCK_PutFloat(at + 8, 0.2f * (float)(i + 1));
        STRMOCK_PutFloat(at + 12, 0.3f * (float)(i + 1));
        STRMOCK_PutFloat(at + 16, 0.4f * (float)(i + 1));
        at += 20;
    }
    STRMOCK_PutLong(at, -1);
    STRMOCK_fontSize = at + 4;
}

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    STRMOCK_Record("BIG_ul_SearchKeyToPos %08x", key);
    if (key == (uint32_t)STRMOCK_fontKey)
        return 1;
    return -1;
}

char* BIG_pc_ReadFileTmp(int pos, int* size)
{
    STRMOCK_Record("BIG_pc_ReadFileTmp %d", pos);
    *size = STRMOCK_fontSize;
    return STRMOCK_font;
}

/* readMode selects the on-disk format, it is not a count, so one value
   is read whatever it says. */
int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int v = 0;

    memcpy(&v, *offset, 4);
    *offset += 4;
    STRMOCK_Record("LOA_ReadLong %d %d", (int)readMode, v);
    return v;
}

float LOA_ReadFloat(char** offset, float* out, char readMode)
{
    float v = 0.0f;

    memcpy(&v, *offset, 4);
    *offset += 4;
    STRMOCK_Record("LOA_ReadFloat %d %.9g", (int)readMode, (double)v);
    return v;
}

char** LOA_ReadCharArray(char** offset, char* dest, unsigned int len, char readMode)
{
    memcpy(dest, *offset, (size_t)len);
    *offset += len;
    STRMOCK_Record("LOA_ReadCharArray %u %d", len, (int)readMode);
    return offset;
}
