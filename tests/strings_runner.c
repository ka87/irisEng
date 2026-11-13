/* One case body, compiled twice: once against the frozen copy of
   strings.c and once against the rewrite. The -D flags that rename the
   file's symbols are passed to this file too, so every call below binds
   to whichever copy it was built for. */

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

extern int STR_si_Id;
extern int STR_sgst_Format;
extern int STRDataPtrStart;

/* strings.c's own entry points that strings.h does not declare */
tFontData* STR_pst_GetFont(int key);
BOOL STR_b_LoadFontDescriptor(struct sFontData* font, int key);
int STR_l_GetMarkIndex(struct sGEOSTRStruct* str, struct sGEOSTRStringData* sd, int mark);
char* STR_GetKeyName(int ch, int combo);
char* STR_sz_ControlsReadFormat_pcINO(char* str);
int STR_l_ReadFormat(char* format);
void STR_SetCurEffect(tGEOSTRStruct* str, int effectID);
void STR_ChangeStringLength(tGEOSTRStruct* str, int strIdx, int lenToSet);
void STR_RecomputePos(tGEOSTRStruct* str, tGEOSTRStringData* sd, int firstChar, int charNum);
unsigned int STR_i_GlobalLength(int globalIdx);
void STR_EffectDelAllForAString(tGEOSTRStruct* str, int strIdx);
void STR_EffectDelAll(tGEOSTRStruct* str);
void STR_EffectCheck(tGEOSTRStruct* str, int strIdx, int firstChar, int charNum);
int STR_i_EffectGetId(struct sGEOSTRStruct* str, int strIdx);

void RUNNER(int op, int a, int b, int c, int d, int e, float f, const char* s)
{
    tGEOSTRStringData* sd;
    tStrGlobalStruct* glob;
    tFontData* font;
    Vec3* got;
    Vec3 v1;
    Vec3 v2;
    char* text;

    sd = STR_pst_GetString(STRW_str, a < 0 ? 0 : a);
    v1.x = f;
    v1.y = f * 0.5f;
    v1.z = f * 0.25f;
    v2.x = f * 2.0f;
    v2.y = f * 3.0f;
    v2.z = 0.0f;

    switch (op)
    {
    case 0:  STR_GlobalSet(a, (char*)s); break;
    case 1:  STR_GlobalSetChar(a, b, (char)c); break;
    case 2:
        glob = STR_sz_GlobalGet(a);
        STRCASE_ri[0] = glob != 0;
        if (glob)
            memcpy(STRCASE_rs, glob->str, sizeof(glob->str));
        break;
    case 3:  STRCASE_ri[0] = (int)STR_i_GlobalLength(a); break;
    case 4:
        STRCASE_ri[0] = STR_l_ReadFormat((char*)s);
        STRCASE_ri[1] = STR_sgst_Format;
        STRCASE_ri[2] = STRDataPtrStart;
        break;
    case 5:
        text = STR_sz_ControlsReadFormat_pcINO((char*)s);
        strncpy(STRCASE_rs, text, sizeof(STRCASE_rs) - 1);
        break;
    case 6:  STRCASE_ri[0] = STR_l_GetStringLength((char*)s); break;
    case 7:  STR_SetString(STRW_gao, a, (char*)s); break;
    case 8:  STR_SetStringExt(STRW_gao, a, b, (char*)s, &v1); break;
    case 9:  STR_InsertString(STRW_gao, a, b, (char*)s); break;
    case 10: STR_SetFloat(STRW_gao, a, f, (char)b); break;
    case 11: STR_InsertFloat(STRW_gao, a, b, f, (char)c); break;
    case 12: STR_ChangeStringLength(STRW_str, a, b); break;
    case 13: STR_SetPos(STRW_gao, a, b, c, &v1); STRCASE_rf[0] = v1.z; break;
    case 14: STR_SetSize(STRW_gao, a, b, c, &v1); break;
    case 15: STR_Align(STRW_gao, a, b, c, d); break;
    case 16: STR_SetColor(STRW_str, a, b, c, d); break;
    case 17: STR_SetAlpha(STRW_str, a, b, c, d); break;
    case 18: STR_SetColorExt(STRW_str, a, b, c, d, e); break;
    case 19: STRCASE_ri[0] = (int)STR_GetColor(STRW_str, a, b); break;
    case 20: STR_ChangePage(STRW_str, a, b, c, d, e); break;
    case 21: STR_DelChar(STRW_gao, a, b, c); break;
    case 22: STR_SetChar(STRW_str, a, b, c); break;
    case 23: STRCASE_ri[0] = (int)STR_i_GetChar(STRW_str, a, b); break;
    case 24: STRCASE_ri[0] = STR_l_GetSubLength(STRW_str, a, b); break;
    case 25: STR_SetSubString(STRW_gao, a, b, c); break;
    case 26: STR_SetSubStringWithLine(STRW_gao, a, b, c); break;
    case 27: STR_SetSubStringWithMark(STRW_gao, a, b, c); break;
    case 28: STRCASE_ri[0] = STR_i_CountLine(STRW_gao, a, b, c); break;
    case 29: STRCASE_ri[0] = STR_i_CountMark(STRW_gao, a, b, c, d); break;
    case 30: STR_ToString(STRW_gao, a, b, c, (unsigned char*)STRCASE_rs); break;
    case 31: STR_SetLetterFlag(STRW_gao, a, b, c, d); break;
    case 32: STRCASE_ri[0] = STR_GetLetterFlag(STRW_gao, a, b); break;
    case 33: STR_ToUpper(STRW_gao, a, b, c); break;
    case 34:
        STR_GetRectShort(STRW_str, a, b, c, &v1, &v2);
        STRCASE_rf[0] = v1.x; STRCASE_rf[1] = v1.y; STRCASE_rf[2] = v1.z;
        STRCASE_rf[3] = v2.x; STRCASE_rf[4] = v2.y; STRCASE_rf[5] = v2.z;
        break;
    case 35:
        STRCASE_ri[0] = STR_pst_GetMin(STRW_gao, a, b, c, &v1) == &v1;
        STRCASE_rf[0] = v1.x; STRCASE_rf[1] = v1.y; STRCASE_rf[2] = v1.z;
        break;
    case 36:
        STRCASE_ri[0] = STR_pst_GetMax(STRW_gao, a, b, c, &v1) == &v1;
        STRCASE_rf[0] = v1.x; STRCASE_rf[1] = v1.y; STRCASE_rf[2] = v1.z;
        break;
    case 37:
        STRCASE_ri[0] = STR_pst_GetSize(STRW_gao, a, b, c, &v1) == &v1;
        STRCASE_rf[0] = v1.x; STRCASE_rf[1] = v1.y; STRCASE_rf[2] = v1.z;
        break;
    case 38:
        STR_SetPivot(STRW_str, a, &v1);
        got = STR_pst_GetPivot(STRW_str, a);
        STRCASE_ri[0] = got != 0;
        if (got)
        {
            STRCASE_rf[0] = got->x; STRCASE_rf[1] = got->y; STRCASE_rf[2] = got->z;
        }
        break;
    case 39:
        STR_SetFlag(STRW_str, a, (int16_t)b);
        STRCASE_ri[0] = STR_ul_GetFlag(STRW_str, a);
        break;
    case 40: STR_SetFrame(STRW_gao, a, (char)b, (int16_t)c); break;
    case 41: STR_SetFramePos(STRW_gao, a, &v1, &v2); break;
    case 42: STR_SetFrameBorderSize(STRW_gao, a, f); break;
    case 43: STR_SetFrameColor(STRW_gao, a, b); break;
    case 44:
        STRCASE_ri[0] = STR_i_EffectAdd(STRW_gao, (uint32_t)a, b, c, d);
        STRCASE_ri[1] = STR_si_Id;
        break;
    case 45: STR_EffectSeti(STRW_gao, a, b, c); break;
    case 46: STR_EffectSetf(STRW_gao, a, b, f); break;
    case 47: STRCASE_ri[0] = STR_EffectGeti(STRW_gao, a, b); break;
    case 48: STR_EffectDel(STRW_gao, a); break;
    case 49: STR_EffectDelAll(STRW_str); break;
    case 50: STR_EffectDelAllForAString(STRW_str, a); break;
    case 51: STRCASE_ri[0] = STR_i_EffectGetId(STRW_str, a); break;
    case 52: STR_EffectCheck(STRW_str, a, b, c); break;
    case 53:
        STR_SetCurEffect(STRW_str, a);
        STRCASE_ri[0] = STR_spst_CurEffect != 0;
        STRCASE_ri[1] = STR_spst_CurEffect ? STR_spst_CurEffect->effectID : -1;
        break;
    case 54: STR_AddFont(a, STRW_tex); break;
    case 55:
        font = STR_pst_GetFont(a);
        STRCASE_ri[0] = font ? (int)(font - STR_sgpst_Font) : -1;
        break;
    case 56: STR_FontAddAlternativeKey(a, b); break;
    case 57: STR_FontResolveAlternativeKey(); break;
    case 58: STRCASE_ri[0] = STR_b_LoadFontDescriptor(&STRW_fontBuf, a); break;
    case 59:
        font = (tFontData*)STR_pst_GetFontDesc(STRW_mat, a);
        STRCASE_ri[0] = font ? (int)(font - STR_sgpst_Font) : -1;
        break;
    case 60:
        text = STR_GetKeyName(a, b);
        STRCASE_ri[0] = text != 0;
        if (text)
            strncpy(STRCASE_rs, text, sizeof(STRCASE_rs) - 1);
        break;
    case 61:
        STR_ParseString(STRW_str, a, &STRW_str->letters[b], (char*)s, c,
                        0.050000001f, 0.050000001f, -1, d);
        break;
    case 62:
        if (sd)
            STR_RecomputePos(STRW_str, sd, b, c);
        break;
    case 63: STR_Init3DStringExtExt(STRW_gao, (char*)s, a); break;
    case 64: STR_Init3DStringFromSTR(STRW_gao, STRW_gao, a, b); break;
    case 65: STR_3DStringList_Add(STRW_gao); break;
    case 66: STR_Reset(STRW_gao); break;
    case 67: STRCASE_ri[0] = STR_l_GetLength(STRW_str, (unsigned int)a); break;
    case 68: STRCASE_ri[0] = sd ? (int)(sd - STRW_str->stringData) : -1; break;
    case 69:
        if (sd)
        {
            STRCASE_ri[0] = STR_l_GetValidIndex(STRW_str, sd, b);
            STRCASE_ri[1] = STR_l_GetValidNumber(STRW_str, sd, STRCASE_ri[0], c);
        }
        break;
    case 70:
        if (sd)
            STRCASE_ri[0] = STR_l_GetMarkIndex(STRW_str, sd, b);
        break;
    case 71: STR_ReinitAll(); break;
    case 72:
        if (a == 0)
            STR_Init3DString(STRW_gao, (char*)s);
        else if (a == 1)
            STR_Init3DStringExt(STRW_gao, (char*)s, b);
        else
            STR_Init3DStringCenter(STRW_gao, (char*)s, b);
        break;
    case 73:
        STR_3DStringList_Reinit();
        if (a)
            STR_3DStringList_Clear();
        break;
    default:
        break;
    }
}
