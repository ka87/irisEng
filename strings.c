#include "memory.h"
#include "win/wingdi.h"
#include "loading.h"
#include "bigfiles.h"
#include "ai.h"
#include "gameobject.h"
#include "textures.h"
#include "inputoutput.h"
#include "win/wininputoutput.h"
#include "win/wingdi.h"
#include "strings.h"

int pcINO_ControlsReadFormatBufferSize;
char* pcINO_ControlsReadFormatBuffer;
int STRDataPtrStart;
int STR_sgst_Format;
int STR_si_Id = 0; // game doesn't set it explicitly
float STR_f_DefaultLetterDepth = 0.0099999998f;

char STRU1Offset = 0;
char STRV1Offset = 1;
char STRU2Offset = 0;
char STRV2Offset = 0;
char STRU3Offset = 1;
char STRV3Offset = 1;
char STRU4Offset = 1;
char STRV4Offset = 0;

static tFontData* STR_l_NewFont()
{
    tFontData* fonts;
    size_t size;
    int num;

    fonts = STR_sgpst_Font;
    num = STR_sgl_NumberOfFont;
    if (num >= STR_sgl_MaxNumberOfFont)
    {
        size = sizeof(tFontData) * (STR_sgl_MaxNumberOfFont + 20);
        STR_sgl_MaxNumberOfFont += 20;
        if (STR_sgpst_Font)
            fonts = (tFontData*)MEM_p_Realloc(STR_sgpst_Font, size);
        else
            fonts = (tFontData*)MEM_p_Alloc(size);
        num = STR_sgl_NumberOfFont;
        STR_sgpst_Font = fonts;
    }
    STR_sgl_NumberOfFont = num + 1;
    return &fonts[num];
}

static tFontData* STR_l_FontDescForPage(int page)
{
    return STR_pst_GetFontDesc((tGEOMAT*)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), (int16_t)page);
}

static void STR_l_UseGaoMaterial(tGEOSTRStruct* str, tGameObjectMain* gao)
{
    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
    str->fontMaterialID = -1;
    str->fontDesc = NULL;
}

static int STR_l_GlyphIndex(uint32_t flagsAndChar)
{
    unsigned int ch;

    ch = (uint8_t)flagsAndChar;
    if (ch < 32)
        return 0;
    return ch - 32;
}

static int STR_l_LetterRange(tGEOSTRStruct* str, tGEOSTRStringData* sd, int firstChar, int charNum,
                             tGEOSTRLetter** begin, tGEOSTRLetter** end)
{
    int first;

    first = STR_l_GetValidIndex(str, sd, firstChar);
    *begin = &str->letters[first + sd->index];
    *end = *begin + STR_l_GetValidNumber(str, sd, first, charNum);
    return first;
}

void STR_3DStringList_Reinit()
{
    int i;

    for (i = 0; i < STR_3DString_Number; i++)
    {
        if (STR_3DString_Obj[i]->extended && STR_3DString_Obj[i]->extended->AIInstance)
        {
                AI_ReinitInstance(STR_3DString_Obj[i]->extended->AIInstance);
        }
    }
}

void STR_3DStringList_Clear()
{
    STR_3DString_Number = 0;
};

void STR_3DStringList_Add(struct sGameObjectMain* gao)
{
    int i;

    if (STR_3DString_Number == 64)
        return;

    for (i = 0; i < STR_3DString_Number; i++)
    {
        if (STR_3DString_Obj[i] == gao)
            return;
    }

    STR_3DString_Obj[STR_3DString_Number] = gao;
    STR_3DString_Number++;
}

tFontData* STR_pst_GetFont(int key)
{
    int i;

    for (i = 0; i < STR_sgl_NumberOfFont; i++)
    {
        if (STR_sgpst_Font[i].texKey == key)
            return &STR_sgpst_Font[i];
    }
    return 0;
}

void STR_FontResolveAlternativeKey()
{
    tFontData* font;
    int i;

    for (i = 0; i < STR_FontAltKeyNumber; ++i)
    {
        font = STR_pst_GetFont(STR_FontAltKey[i].key);
        if (font)
            font->texKeyAlternative = STR_FontAltKey[i].key2;
    }
    STR_FontAltKeyNumber = 0;
}

void STR_FontAddAlternativeKey(int key, int altKey)
{
    if (STR_FontAltKeyNumber < 16)
    {
        STR_FontAltKey[STR_FontAltKeyNumber].key = key;
        STR_FontAltKey[STR_FontAltKeyNumber].key2 = altKey;
        STR_FontAltKeyNumber++;
    }
}

BOOL STR_b_LoadFontDescriptor(struct sFontData* font, int key)
{
    unsigned int ch;
    tFontGlypthElem* fge;
    char* data;
    char* offset;
    char* end;
    char* tail;
    int pos;
    int size;
    char magic[8];

    pos = BIG_ul_SearchKeyToPos(key);
    if (pos == -1)
        return 0;
    data = BIG_pc_ReadFileTmp(pos, &size);
    offset = data;
    if ((unsigned int)size < 12)
        return 0;
    LOA_ReadCharArray(&offset, magic, 8u, 1);
    if (strncmp(magic, "FONTDESC", 8u))
        return 0;
    end = &data[size - 4];
    tail = end;
    if (LOA_ReadLong(&tail, 0, 4) != -1)
        return 0;
    LOA_ReadLong(&offset, 0, 1);
    while (offset < end)
    {
        ch = LOA_ReadLong(&offset, 0, 1);
        if (ch < 32 || ch > 255)
            return 0;
        fge = &(font->fge[ch - 32]);
        fge->x = LOA_ReadFloat(&offset, 0, 1);
        fge->w = LOA_ReadFloat(&offset, 0, 1);
        fge->y = LOA_ReadFloat(&offset, 0, 1);
        fge->h = LOA_ReadFloat(&offset, 0, 1);
    }
    return LOA_ReadLong(&offset, 0, 1) == -1;
}

void STR_AddFont(int key, struct sEngTexIntermediateStruct* tex)
{
    tFontData* font;

    if (!tex || !tex->imgDataPtr)
        return;

    font = STR_pst_GetFont(key);
    if (!font)
    {
        font = STR_l_NewFont();
        font->texKey = key;
    }

    font->width = tex->realWidth;
    font->height = tex->realHeight;
    memset(font->fge, 0, sizeof(font->fge));
    if (tex->tp.magicParams.fontDescriptorKey)
        STR_b_LoadFontDescriptor(font, tex->tp.magicParams.fontDescriptorKey);
}

void STR_SetDisplay()
{
    int drawMaskAllowedStuffBackup;
    tDDCamStruct cams;

    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = &MATH_gst_IdentityMatrix;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    drawMaskAllowedStuffBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    
    sul_SaveDisplayInfo = gpst_DX_stDD->flags;
    gpst_DX_stDD->flags = sul_SaveDisplayInfo | 0x8;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x8010900;    
    memcpy(&cams, &gpst_DX_stDD->ct, sizeof(cams));
    sul_SaveDrawMask = drawMaskAllowedStuffBackup;
    cams.flags = 0x401;
    
    gpst_DX_stDD->SetProjectionMatrix(&cams);
}

void STR_Close()
{
    if (STR_sgpst_GO)
    {
        ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = NULL;
        ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = NULL;
        OBJ_GameObject_Remove(STR_sgpst_GO, 1);
        if (STR_sgpst_Font)
            MEM_Free(STR_sgpst_Font);
    }
}

struct sFontData* STR_pst_GetFontDesc(struct sGEOMAT* mat, int matNum)
{
    tMultiTextureElemStruct* multiTex;
    tFontData* font;
    tFontGlypthElem* fge;
    int texIdx;
    int texKey;
    int sub;
    int i;

    if (!mat)
        return 0;

    if (mat->intf->id == 4)
    {
        sub = matNum;
        if (matNum >= mat->multiMaterialNumIfType4ThenAmbiantColor)
            sub = 0;
        mat = mat->multiMaterialPtrIfType4ThenDiffuseColor[sub];
    }

    texIdx = -1;
    if (mat->intf->id == 3)
    {
        texIdx = mat->idxOrPtr;
    }
    else if (mat->intf->id == 5)
    {
        multiTex = (tMultiTextureElemStruct*)mat->idxOrPtr;
        if (multiTex)
            texIdx = multiTex->texElemIdx;
    }

    if (texIdx != -1)
    {
        if ((TEX_gst_GlobalList.texElems[texIdx].texElemFlag & 0x40) != 0)
            texIdx = TEX_gst_GlobalList.texElems[texIdx].palettedData.textureID;
        texKey = TEX_gst_GlobalList.texElems[texIdx].texKey;
        for (i = 0; i < STR_sgl_NumberOfFont; i++)
        {
            if (STR_sgpst_Font[i].texKey == texKey || STR_sgpst_Font[i].texKeyAlternative == texKey)
                return &STR_sgpst_Font[i];
        }
    }

    for (i = 0; i < STR_sgl_NumberOfFont; i++)
    {
        if (STR_sgpst_Font[i].mat == mat)
            return &STR_sgpst_Font[i];
    }

    font = STR_l_NewFont();
    font->mat = mat;
    fge = font->fge;
    for (i = 0; i < 224; i++)
    {
        fge[i].x = 0.0;
        fge[i].y = 0.0;
        fge[i].w = 0.0;
        fge[i].h = 1.0;
    }
    return font;
}

struct sGEOSTRStruct* STR_pst_Get(struct sGameObjectMain* gao)
{
    tGEOSTRStruct* result;

    if ((gao->flags & 0x4000) == 0
        || (result = (tGEOSTRStruct*)((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) == 0
        || result->intf->id != 10)
    {
        result = NULL;
    }
    return result;
}

int STR_l_GetLength(struct sGEOSTRStruct* str, unsigned int strIdx)
{
    if (strIdx < str->stringNum)
        return str->stringData[strIdx].number;
    return -1;
}

struct sGEOSTRStringData* STR_pst_GetString(struct sGEOSTRStruct* str, int strIdx)
{
    if (str->letters && strIdx >= 0 && strIdx < str->stringNum)
        return &str->stringData[strIdx];
    return 0;
}

int STR_l_GetMarkIndex(struct sGEOSTRStruct* str, struct sGEOSTRStringData* sd, int mark)
{
    int i;

    for (i = 0; i < sd->number; i++)
    {
        if (((str->letters[sd->index + i].flagsAndChar >> 20) & 0xF) == mark)
            break;
    }
    return i;
}

int STR_l_GetValidIndex(struct sGEOSTRStruct* str, struct sGEOSTRStringData* sd, int firstChar)
{
    int first;

    first = firstChar;
    if (firstChar <= -50)
        first = STR_l_GetMarkIndex(str, sd, (-2 - (uint8_t)firstChar) & 0xF);
    if (first <= -10)
        first = STR_l_GetMarkIndex(str, sd, (6 - (uint8_t)first) & 0xF) + 1;
    if (first == -2)
        first = sd->index2;
    if (first < 0 || first >= sd->number)
        first = sd->number;
    return first;
}

int STR_l_GetValidNumber(struct sGEOSTRStruct* str, struct sGEOSTRStringData* sd, int firstChar, int charNum)
{
    int num;
    int markIdx;

    if (charNum <= -10)
    {
        markIdx = STR_l_GetMarkIndex(str, sd, (6 - (uint8_t)charNum) & 0xF);
        if (markIdx >= firstChar)
            return markIdx - firstChar;
        return sd->number - firstChar;
    }

    num = charNum;
    if (charNum == -2)
        num = sd->index2 + sd->number2 - firstChar;
    if (num <= 0 || firstChar + num > sd->number)
        return sd->number - firstChar;
    return num;
}

void STR_GetRectShort(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, Vec3* min, Vec3* max)
{
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int16_t minX;
    int16_t minY;
    int16_t maxX;
    int16_t maxY;
    int16_t x;
    int16_t y;

    minX = 10000;
    minY = 10000;
    maxX = -10000;
    maxY = -10000;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
    {
        STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
        for (; letter < end; ++letter)
        {
            x = letter->x;
            if (x < minX)
                minX = x;
            y = letter->y;
            if (y < minY)
                minY = y;
            if ((int16_t)(x + letter->width) > maxX)
                maxX = x + letter->width;
            if ((int16_t)(y + letter->height) > maxY)
                maxY = y + letter->height;
        }
    }

    if (min)
    {
        min->z = 0.0;
        if (minX == 10000)
        {
            min->x = 0.0;
            min->y = 0.0;
        }
        else
        {
            min->x = (float)minX;
            min->y = (float)minY;
        }
    }
    if (max)
    {
        max->z = 0.0;
        if (minX == 10000)
        {
            max->x = 0.0;
            max->y = 0.0;
        }
        else
        {
            max->x = (float)maxX;
            max->y = (float)maxY;
        }
    }
}

void STR_GlobalSet(int globalIdx, char* src)
{
    int i;

    // preserved: the bound test truncates to a byte but the index does not
    if ((uint8_t)globalIdx < 24u)
    {
        for (i = 0; i < 15 && src[i]; i++)
            STR_sgaac_Global[globalIdx].str[i] = src[i];
        STR_sgaac_Global[globalIdx].str[i] = 0;
    }
}

void STR_GlobalSetChar(int globalIdx, int charIdx, char character)
{
    if ((uint8_t)globalIdx < 24u)
    {
        STR_sgaac_Global[globalIdx].str[15] = 0;
        if ((uint8_t)charIdx < 15u)
            STR_sgaac_Global[globalIdx].str[charIdx] = character;
    }
}

tStrGlobalStruct* STR_sz_GlobalGet(int globalIdx)
{
    if ((uint8_t)globalIdx < 24u)
        return &STR_sgaac_Global[globalIdx];
    return 0;
}

void STR_Reset(tGameObjectMain* gao)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_Reinit((tGroGeneric*)str);
}

static char* STR_l_TwoKeyNames(int mapIdx1, int mapIdx2)
{
    int key1;
    int key2;

    key1 = pcINO_keyMaps[mapIdx1];
    key2 = pcINO_keyMaps[mapIdx2];
    if (key1 == -1)
        return "\\p16\\2";
    if (key1 < 0 || key2 < 0)
        return prefixOneSpace;
    strcpy(pcINO_KeyNamesToRet, buttonNames[key1].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[key2].str);
    return pcINO_KeyNamesToRet;
}

char* STR_GetKeyName(int ch, int combo)
{
    int key;

    if (!pcINO_keyMaps)
        return "";

    if (ch >= '0' && ch <= '=')
    {
        key = pcINO_keyMaps[ch - '0'];
        if (key >= 0)
            return buttonNames[key].str;
        return prefixOneSpace;
    }
    if (ch == '?')
    {
        key = pcINO_keyMaps[3];
        if (key >= 0)
            return buttonNames[key].str;
        return prefixOneSpace;
    }
    if (ch < 'A' || ch > 'S')
    {
        pcINO_KeyNamesToRet[0] = ch;
        pcINO_KeyNamesToRet[1] = 0;
        return pcINO_KeyNamesToRet;
    }
    if (ch == 'A')
    {
        switch (combo)
        {
        case 1214:
            return STR_l_TwoKeyNames(14, 16);
        case 1315:
            return STR_l_TwoKeyNames(15, 17);
        case 2345:
            return pcINO_GetFourKeyNamesCattedTogether(14, 15, 17, 16);
        }
        return "";
    }
    if (ch == 'F')
    {
        switch (combo)
        {
        case 1214:
            return STR_l_TwoKeyNames(18, 20);
        case 1315:
            return STR_l_TwoKeyNames(19, 21);
        case 2345:
            return pcINO_GetFourKeyNamesCattedTogether(18, 19, 21, 20);
        }
        return "";
    }
    if (ch != 'O')
        return "";
    if (combo == 24)
        return STR_l_TwoKeyNames(8, 10);
    return STR_l_TwoKeyNames(9, 11);
}

static char* STR_l_CopyUpTo(char* dst, char* src, char* last)
{
    char* p;

    for (p = src; p != last + 1; ++p)
        *dst++ = *p;
    return dst;
}

char* STR_sz_ControlsReadFormat_pcINO(char* str)
{
    char* src;
    char* dst;
    char* end;
    char* name;
    char ch;
    unsigned int value;
    unsigned int curPage;
    unsigned int page;
    unsigned int pageSet;
    unsigned int restorePage;
    unsigned int back;
    int combo;
    int modeChanged;
    int savedMode;
    int savedSubmode;

    curPage = 0;
    page = 0;
    pageSet = 0;
    restorePage = 0;
    combo = 0;
    modeChanged = 0;
    savedSubmode = pcINO_SelectedControlSubmode;
    savedMode = pcINO_SelectedControlMode;

    if (!pcINO_ControlsReadFormatBuffer)
    {
        pcINO_ControlsReadFormatBufferSize = 512;
        pcINO_ControlsReadFormatBuffer = (char*)MEM_p_Alloc(512u);
    }

    src = str;
    dst = pcINO_ControlsReadFormatBuffer;

    for (ch = *src; ch; ch = *++src)
    {
        if (ch == '\\')
        {
            switch (src[1])
            {
            case '-':
            case 'n':
                dst[0] = '\\';
                dst[1] = src[1];
                dst += 2;
                ++src;
                break;
            case 'd':
                dst[0] = '\\';
                dst[1] = src[1];
                dst[2] = src[2];
                dst += 3;
                src += 2;
                break;
            case 'O':
                pcINO_SetControlMode(strtoul(src + 2, &end, 10));
                if (end)
                {
                    modeChanged = 1;
                    src = end;
                }
                break;
            case 'o':
                pcINO_SetControlSubmode(strtoul(src + 2, &end, 10));
                if (end)
                {
                    modeChanged = 1;
                    src = end;
                }
                break;
            case 'b':
                combo = strtoul(src + 2, &end, 10);
                src = end;
                break;
            case 'P':
                if (page != 16)
                    restorePage = page;
                value = strtoul(src + 2, &end, 10);
                curPage = value;
                pageSet = value;
                if (end)
                {
                    if (value != 16)
                        dst = STR_l_CopyUpTo(dst, src, end);
                    src = end;
                }
                page = pageSet;
                break;
            case 'p':
                value = strtoul(src + 2, &end, 10);
                curPage = value;
                if (end)
                {
                    if (value != 16)
                        dst = STR_l_CopyUpTo(dst, src, end);
                    src = end;
                }
                page = pageSet;
                break;
            default:
                end = strchr(src + 2, '\\');
                if (end)
                {
                    dst = STR_l_CopyUpTo(dst, src, end);
                    src = end;
                }
                page = pageSet;
                break;
            }
        }
        else if (curPage == 16)
        {
            name = STR_GetKeyName(ch, combo);
            if (name && *name)
            {
                back = page;
                if (page == 16)
                    back = restorePage;
                dst += sprintf(dst, "%s\\P%d\\", name, back);
            }
            curPage = page;
            combo = 0;
        }
        else
        {
            *dst++ = ch;
            curPage = page;
            combo = 0;
        }
    }

    *dst = 0;
    if (modeChanged)
    {
        pcINO_SetControlMode(savedMode);
        pcINO_SetControlSubmode(savedSubmode);
    }
    return pcINO_ControlsReadFormatBuffer;
}

int STR_l_ReadFormat_Int(char* str)
{
    char* p;
    char ch;
    int value;
    int len;
    int done;

    p = str;
    value = 0;
    len = 0;
    done = 0;
    STRDataPtrStart = 0;
    ch = *str;
    while (ch != '\\' && ch)
    {
        if (!done)
        {
            if (isdigit(ch))
            {
                value = ch + 10 * value - '0';
                STRDataPtrStart = value;
            }
            else
            {
                done = 1;
            }
        }
        ch = *++p;
        ++len;
    }
    return len;
}

int STR_l_ReadFormat_Hexa(char* str)
{
    char* p;
    char ch;
    int value;
    int len;
    int done;

    p = str;
    value = 0;
    len = 0;
    done = 0;
    STRDataPtrStart = 0;
    ch = *str;
    while (ch != '\\' && ch)
    {
        if (!done)
        {
            if (isdigit(ch))
            {
                value = 16 * value + ch - '0';
                STRDataPtrStart = value;
            }
            else if (ch >= 'a' && ch <= 'f')
            {
                value = 16 * value + ch - 87;
                STRDataPtrStart = value;
            }
            else if (ch >= 'A' && ch <= 'F')
            {
                value = 16 * value + ch - 55;
                STRDataPtrStart = value;
            }
            else
            {
                done = 1;
            }
        }
        ch = *++p;
        ++len;
    }
    return len;
}

int STR_l_ReadFormat_Float(char* str)
{
    char* p;
    char ch;
    float scale;
    int len;
    int done;

    p = str;
    scale = 0.0;
    len = 0;
    done = 0;
    STRDataPtrStart = 0;
    ch = *str;
    while (ch != '\\' && ch)
    {
        if (!done)
        {
            if (isdigit(ch))
            {
                if (scale == 0.0)
                {
                    *(float*)&STRDataPtrStart = (float)((double)(ch - 48) + *(float*)&STRDataPtrStart * 10.0);
                }
                else
                {
                    *(float*)&STRDataPtrStart = (float)((double)(ch - 48) * scale + *(float*)&STRDataPtrStart);
                    scale = scale * 0.1f;
                }
            }
            else if (ch == '.')
            {
                scale = 0.1f;
            }
            else
            {
                done = 1;
            }
        }
        ch = *++p;
        ++len;
    }
    return len;
}

int STR_l_ReadFormat(char* format)
{
    int ret = 1;

    if (format[0] != '\\')
        return 0;
    
    STR_sgst_Format = 0;

    switch (format[1])
    {
    case '-':
        ret = 2;
        STR_sgst_Format = 11;
        break;
    case 'D':
        STR_sgst_Format = 12;
        ret = STR_l_ReadFormat_Int(format + 2) + 3;
        break;
    case 'P':
        STR_sgst_Format = 9;
        ret = STR_l_ReadFormat_Int(format + 2) + 3;
        break;
    case 'a':
        STR_sgst_Format = 10;
        ret = STR_l_ReadFormat_Int(format + 2) + 3;
        break;
    case 'b':
        STR_sgst_Format = 13;
        ret = STR_l_ReadFormat_Int(format + 2) + 3;
        break;
    case 'c':
        STR_sgst_Format = 3;
        ret = STR_l_ReadFormat_Hexa(format + 2) + 3;
        break;
    case 'd':
        STR_sgst_Format = 12;
        STRDataPtrStart = format[2] - '0';
        ret = 3;
        break;
    case 'h':
        STR_sgst_Format = 7;
        ret = STR_l_ReadFormat_Float(format + 2) + 3;
        break;
    case 'm':
        STR_sgst_Format = 2;
        ret = STR_l_ReadFormat_Int(format + 2) + 3;
        break;
    case 'n':
        ret = 2;
        STR_sgst_Format = 1;
        break;
    case 'p':
        STR_sgst_Format = 8;
        ret = STR_l_ReadFormat_Int(format + 2) + 3;
        break;
    case 'w':
        STR_sgst_Format = 6;
        ret = STR_l_ReadFormat_Float(format + 2) + 3;
        break;
    case 'x':
        STR_sgst_Format = 4;
        ret = STR_l_ReadFormat_Float(format + 2) + 3;
        break;
    case 'y':
        STR_sgst_Format = 5;
        ret = STR_l_ReadFormat_Float(format + 2) + 3;
        break;
    default:
        return ret;
    }
    return ret;
}

void STR_SetCurEffect(tGEOSTRStruct* str, int effectID)
{
    int i;

    if (!STR_spst_CurEffect || STR_spst_CurEffect->effectID != effectID)
    {
        for (i = 0; i < str->effectsNum; i++) {
            if (str->effect[i].effectID == effectID) {
                STR_spst_CurEffect = &str->effect[i];
                return;
            }
        }

        STR_spst_CurEffect = NULL;
    }   
}

//if effectType has 0x80000000, then first parameter is a game object
int STR_i_EffectAdd(void* geoStrOrGao, uint32_t effectType, int stringID, int startIndex, int len)
{
    tGEOSTRStruct* str;
    int flags;
    tGEOSTREffectStruct* effect;

    str = (tGEOSTRStruct*)geoStrOrGao;
    flags = effectType;

    if (effectType >= 0)
        str = STR_pst_Get((tGameObjectMain*)geoStrOrGao);
    else
        flags = effectType + 0x80000000;

    if (!str)
        return -1;
    
    if (str->effectsNum == str->maxEffectsNum)
        return -1;
    
    effect = &str->effect[str->effectsNum];
    memset(effect, 0, sizeof(tGEOSTREffectStruct));
    
    str->effectsNum++;

    effect->stringid = stringID;
    effect->number = len;
    effect->effectID = STR_si_Id;
    effect->flags = flags;
    effect->index = startIndex;

    STR_spst_CurEffect = effect;
    STR_si_Id++;

    switch (flags)
    {
    case 6:
        effect->f1 = 0.0049999999f;
        effect->timeElapsed = 0.0049999999f;
        effect->color1 = 1.0f;
        return effect->effectID;
    case 7:
    case 8:
    case 9:
    case 10:
        effect->timeElapsed = 0.1f;
        effect->color2 = 0.1f;
        effect->f1 = 1.0;
        return effect->effectID;
    case 11:
    case 12:
        effect->color1 = 0.0f;
        effect->color2 = 2.0f;
        effect->f1 = 1.0f;
        return effect->effectID;
    case 13:
    case 14:
        effect->color1 = 0.0f;
        effect->f1 = 1.0f;
        return effect->effectID;
    case 16:
        effect->f1 = 1.0f;
        return effect->effectID;
    case 17:
        effect->timeElapsed = 1.0f;
        return effect->effectID;
    case 18:
        effect->f1 = 0.1f;
        return effect->effectID;
    case 20:
        effect->timeElapsed = 0.0f;
        effect->color1 = 0.2f;
        effect->f1 = 1.0f;
        return effect->effectID;
    default:
        return effect->effectID;
        break;
    }
}

void STR_ParseString(struct sGEOSTRStruct* geostr, int strIdx, struct sGEOSTRLetter* dest, char* src, int len, float width, float height, int color, int firstChar)
{
    tGEOSTRLetter* prev;
    tGEOSTRLetter* letter;
    tStrGlobalStruct* global;
    tGEOSTREffectStructType20* es19;
    char* currPtr;
    char ch;
    uint8_t rawChar;
    char rawFlag;
    unsigned int flags;
    int page;
    int pageSticky;
    int effectId;
    int n;
    int rest;
    float x;
    float y;

    prev = 0;
    letter = dest;
    rawChar = 0;
    flags = 0;
    x = 0.0;
    y = 0.0;
    page = 0;
    pageSticky = 0;
    rawFlag = 0;
    global = 0;
    currPtr = STR_sz_ControlsReadFormat_pcINO(src);

    while (1)
    {
        ch = *currPtr;
        if (!ch)
            break;

        if (ch == '\r')
        {
            ++currPtr;
            continue;
        }
        if (ch == '\n')
        {
            ++currPtr;
            if (prev)
                prev->flagsAndChar |= 0x20000000;
            continue;
        }

        if (ch == '\\')
        {
            currPtr += STR_l_ReadFormat(currPtr);
            switch (STR_sgst_Format)
            {
            case 1:
                if (prev)
                    prev->flagsAndChar |= 0x20000000;
                break;
            case 2:
                flags = (flags & 0xFF0FFFFF) + (STRDataPtrStart << 20);
                break;
            case 3:
                color = STRDataPtrStart;
                break;
            case 4:
                x = *(float*)&STRDataPtrStart;
                break;
            case 5:
                y = *(float*)&STRDataPtrStart;
                break;
            case 6:
                width = *(float*)&STRDataPtrStart;
                break;
            case 7:
                height = *(float*)&STRDataPtrStart;
                break;
            case 8:
                page = STRDataPtrStart;
                break;
            case 9:
                page = STRDataPtrStart;
                pageSticky = STRDataPtrStart;
                break;
            case 10:
                rawChar = STRDataPtrStart;
                rawFlag = 1;
                break;
            case 11:
                flags |= 0x10000000u;
                break;
            case 12:
                global = STR_sz_GlobalGet(STRDataPtrStart);
                break;
            case 13:
                effectId = STR_i_EffectAdd(geostr, 0x80000013, strIdx, firstChar + (letter - dest), 1); // 19-es effect (0x13)
                if (effectId != -1)
                {
                    STR_SetCurEffect(geostr, effectId);
                    if (STR_spst_CurEffect)
                    {
                        es19 = (tGEOSTREffectStructType20*)STR_spst_CurEffect;
                        es19->flagsAndChar = 0;
                        es19->unkb1 = 0;
                        n = STRDataPtrStart;
                        es19->unki1 = 0;
                        if (n)
                        {
                            do
                            {
                                es19->unki1 = (n % 10) | (16 * es19->unki1);
                                // preserved: (n - n % 10) % 10 is always 0, so only the lowest digit is packed
                                rest = (n - n % 10) % 10;
                                n = (n - n % 10) / 10;
                            } while ((rest >> 31) + rest);
                        }
                        es19->color2 = 0.0;
                        es19->color1 = 0.40000001f;
                    }                   
                }
                break;
            default:
                break;
            }

            if (!rawFlag && !global)
                continue;
        }

        if (!len)
            return;

        while (1)
        {
            if (rawFlag)
            {
                letter->flagsAndChar = flags | rawChar;
            }
            else if (!global)
            {
                letter->flagsAndChar = flags | STR_sgac_CharTable[(uint8_t)*currPtr];
            }
            else
            {
                // preserved: a global's characters index the table as signed chars
                letter->flagsAndChar = flags | STR_sgac_CharTable[global->str[0]];
                global = (tStrGlobalStruct*)((char*)global + 1);
            }

            letter->flagsAndChar |= page << 12;
            letter->x = (int)((double)(uint16_t)geostr->width * x)
                      + D3D_FlatLayerOffsetX((int)gpst_DX_stDD->ct.cam.width,
                                             (int)gpst_DX_stDD->ct.cam.height);
            letter->y = (int)((double)(uint16_t)geostr->height * y);
            letter->width = (unsigned int)((double)(uint16_t)geostr->width * width);
            letter->height = (unsigned int)((double)(uint16_t)geostr->height * height);
            letter->textColor = color;

            if (!global || !global->str[0])
                break;
            ++letter;
            --len;
        }

        if (!rawFlag && !global)
            ++currPtr;
        prev = letter++;
        --len;
        flags = 0;
        page = pageSticky;
        rawFlag = 0;
        global = 0;
    }
}

void STR_ChangeStringLength(tGEOSTRStruct* str, int strIdx, int lenToSet)
{
    tGEOSTRStringData* sd;
    tGEOSTRStringData* next;
    tGEOSTRStringData* end;
    tGEOSTRLetter* newEnd;
    tGEOSTRLetter* oldNext;
    int first;
    int capacity;
    int moved;
    int total;
    uint16_t at;
    uint16_t left;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;
    if (lenToSet == sd->number)
        return;

    first = sd->index;
    capacity = str->unkNum;
    if (lenToSet + first > capacity)
        lenToSet = capacity - first;
    if (lenToSet == sd->number)
        return;

    next = sd + 1;
    end = &str->stringData[str->stringNum];
    if (next >= end)
    {
        sd->number = lenToSet;
        sd->number2 = lenToSet;
        return;
    }

    newEnd = &str->letters[lenToSet + first];
    oldNext = &str->letters[next->index];
    moved = end[-1].number + end[-1].index - next->index;
    if (lenToSet + moved + first > capacity)
        moved = capacity - sd->index - lenToSet;
    if (moved > 0)
        memcpy(newEnd, oldNext, 16 * moved);
    if (newEnd > oldNext)
        memset(oldNext, 0, (char*)newEnd - (char*)oldNext);
    sd->number = lenToSet;
    sd->number2 = lenToSet;

    while (1)
    {
        at = next[-1].number + next[-1].index;
        total = at + next->number;
        next->index = at;
        if (total > (str->unkNum & 0xffff))
            break;
        if (++next >= end)
            return;
    }

    left = (str->unkNum & 0xffff) - at;
    next->number = left;
    // preserved: the clamped length goes into the edited string's number2, not next's
    sd->number2 = left;
    for (++next; next < end; ++next)
    {
        next->index = str->unkNum;
        next->number = 0;
        next->number2 = 0;
    }
}

void STR_RecomputePos(tGEOSTRStruct* str, tGEOSTRStringData* sd, int firstChar, int charNum)
{
    tGEOSTRLetter* letters;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    tGEOSTRLetter* lead;
    unsigned int pageVal;
    int16_t pageId;
    int16_t lineX;
    int16_t penX;
    int16_t topY;
    int16_t leadX;
    int bottom;
    int lineY;
    int h;
    int leadH;
    int first;
    int skipped;

    letters = str->letters;
    first = sd->index;
    letter = &letters[first + firstChar];
    if (charNum > 0)
        end = &letter[charNum];
    else
        end = &letters[first + sd->number];

    skipped = 0;
    if (!firstChar && (letter->flagsAndChar & 0x10000000) != 0 && &letter[1] < end)
    {
        skipped = 1;
        ++letter;
    }

    bottom = (uint16_t)letter->y;
    lineX = letter->x;
    topY = bottom;
    lineY = (uint16_t)bottom;
    penX = lineX;

    if ((sd->flags & 0x10) != 0)
    {
        for (; letter < end; ++letter)
        {
            pageVal = (letter->flagsAndChar >> 12) & 0x3F;
            if ((int16_t)pageVal != str->fontMaterialID)
            {
                str->fontMaterialID = pageVal;
                str->fontDesc = STR_l_FontDescForPage(pageVal);
            }
            if (str->fontDesc)
            {
                letter->x = penX;
                penX += letter->width;
                h = letter->height;
                letter->y = lineY;
                if ((int16_t)lineY + (int16_t)h > (int16_t)bottom)
                    bottom = h + lineY;
                if ((letter->flagsAndChar & 0x20000000) != 0)
                {
                    pageId = str->fontMaterialID;
                    penX = lineX;
                    lineY = bottom;
                    if (pageId >= 0 && pageId < 24)
                        lineY = bottom + str->shorts[pageId];
                }
            }
        }
    }
    else
    {
        for (; letter < end; ++letter)
        {
            letter->x = penX;
            penX += letter->width;
            h = letter->height;
            letter->y = lineY;
            if ((int16_t)lineY + (int16_t)h > (int16_t)bottom)
                bottom = h + lineY;
            if ((letter->flagsAndChar & 0x20000000) != 0)
            {
                penX = lineX;
                lineY = bottom;
            }
        }
    }

    if (skipped)
    {
        lead = &str->letters[firstChar + sd->index];
        leadX = lineX - lead->width;
        leadH = (int16_t)lead->height;
        lead->y = topY;
        lead->x = leadX - 4;
        if (topY + leadH > (int16_t)bottom)
            lead->y = topY - ((leadH + topY - (int16_t)bottom) >> 1);
    }
}

unsigned int STR_i_GlobalLength(int globalIdx)
{
    if ((uint8_t)globalIdx < 24u)
        return strlen(STR_sgaac_Global[globalIdx].str);
    return 0;
}

int STR_l_GetStringLength(char* string)
{
    char* p;
    char ch;
    int len;

    len = 0;
    p = STR_sz_ControlsReadFormat_pcINO(string);
    while (*p)
    {
        ch = *p;
        if (ch == '\\')
        {
            p += STR_l_ReadFormat(p);
            if (STR_sgst_Format == 10)
            {
                ++len;
            }
            else if (STR_sgst_Format == 12)
            {
                len += STR_i_GlobalLength(STRDataPtrStart);
            }
        }
        else if (ch == '\n' || ch == '\r')
        {
            ++p;
        }
        else
        {
            ++p;
            ++len;
        }
    }
    return len;
}

void STR_EffectDelAllForAString(tGEOSTRStruct* str, int strIdx)
{
    uint8_t last;
    int i;

    i = 0;
    while (i < str->effectsNum)
    {
        if (str->effect[i].stringid == strIdx)
        {
            last = str->effectsNum - 1;
            str->effectsNum = last;
            if (i < last)
                memcpy(&str->effect[i], &str->effect[last], sizeof(tGEOSTREffectStruct));
        }
        else
        {
            ++i;
        }
    }
    STR_spst_CurEffect = 0;
}

void STR_SetString(struct sGameObjectMain* gao, int strIdx, char* stringToSet)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;

    if (!stringToSet)
        return;

    str = STR_pst_Get(gao);

    if (!str)
        return;

    STR_l_UseGaoMaterial(str, gao);
    sd = STR_pst_GetString(str, strIdx);

    if (!sd)
        return;

    STR_EffectDelAllForAString(str, strIdx);
    STR_ChangeStringLength(str, strIdx, STR_l_GetStringLength(stringToSet));
    STR_ParseString(str, strIdx, &str->letters[sd->index], stringToSet, sd->number, 0.050000001f, 0.050000001f, -1, 0);
    
    if ((sd->flags & 0x200) == 0)
        STR_RecomputePos(str, sd, 0, -1);
    
    sd->index2 = 0;
    sd->number2 = sd->number;
}

void STR_SetSize(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* size)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    tFontData* font;
    unsigned int pageVal;
    unsigned int h;
    int glyph;
    int first;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    STR_l_UseGaoMaterial(str, gao);
    first = STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);

    if (size->x < 0.0 || size->y <= 0.0)
    {
        for (; letter < end; ++letter)
        {
            pageVal = (letter->flagsAndChar >> 12) & 0x3F;
            if ((int16_t)pageVal != str->fontMaterialID)
            {
                str->fontMaterialID = pageVal;
                str->fontDesc = STR_l_FontDescForPage(pageVal);
            }
            font = str->fontDesc;
            if (font && size->x < 0.0)
            {
                glyph = STR_l_GlyphIndex(letter->flagsAndChar);
                // across, scale by the width the text was laid
                // out across rather than the whole window, so
                // the glyphs keep their shape
                letter->width = (unsigned int)(fabs(font->fge[glyph].y - font->fge[glyph].x)
                    * (double)(uint16_t)font->width
                    * D3D_FlatLayerWidth((int)gpst_DX_stDD->ct.cam.width,
                                         (int)gpst_DX_stDD->ct.cam.height)
                    * 0.0015625);
                h = (unsigned int)(fabs(font->fge[glyph].h - font->fge[glyph].w)
                    * (double)(uint16_t)str->fontDesc->height
                    * gpst_DX_stDD->ct.cam.height
                    * 0.0020833334);
            }
            else
            {
                h = (unsigned int)((double)(uint16_t)str->width * size->x);
                letter->width = h;
                if (font)
                {
                    glyph = STR_l_GlyphIndex(letter->flagsAndChar);
                    h = (unsigned int)(fabs(font->fge[glyph].h - font->fge[glyph].w)
                        / fabs(font->fge[glyph].y - font->fge[glyph].x)
                        * size->x
                        * (double)(uint16_t)str->height);
                }
            }
            letter->height = h;
        }
    }
    else
    {
        for (; letter < end; ++letter)
        {
            letter->width = (unsigned int)((double)(uint16_t)str->width * size->x);
            letter->height = (unsigned int)((double)(uint16_t)str->height * size->y);
        }
    }

    if ((sd->flags & 0x200) == 0)
        STR_RecomputePos(str, sd, first, -1);
}

Vec3* STR_pst_GetSize(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* size)
{
    tGEOSTRStruct* str;
    double dx;
    float dy;
    Vec3 max;
    Vec3 min;

    if (!size)
        return 0;

    str = STR_pst_Get(gao);
    if (str)
    {
        STR_l_UseGaoMaterial(str, gao);
        STR_GetRectShort(str, strIdx, firstChar, charNum, &min, &max);
        dx = max.x - min.x;
        size->x = (float)dx;
        dy = max.y - min.y;
        size->y = dy;
        size->z = max.z - min.z;
        if (dx < 0.0)
            size->x = (float)(dx * -1.0);
        if (dy < 0.0)
            size->y = (float)(dy * -1.0);
        size->x = (float)(size->x / (double)(uint16_t)str->width);
        size->y = (float)(size->y / (double)(uint16_t)str->height);
    }
    return size;
}

void STR_Align(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int align)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    tGEOSTRLetter* lineStart;
    int16_t minX;
    int16_t minY;
    int16_t maxX;
    int16_t maxY;
    int16_t x;
    int16_t y;
    int16_t dy;
    int dx;
    int first;
    int num;
    Vec3 size;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    first = STR_l_GetValidIndex(str, sd, firstChar);
    num = STR_l_GetValidNumber(str, sd, first, charNum);
    if (!align)
    {
        STR_pst_GetSize(gao, strIdx, first, num, &size);
        size.x = (float)((double)(uint16_t)str->width * size.x);
    }

    letter = &str->letters[first + sd->index];
    end = &letter[num];
    if ((letter->flagsAndChar & 0x10000000) != 0)
        ++letter;
    lineStart = letter;

    while (letter < end)
    {
        maxY = -10000;
        minY = 10000;
        minX = 10000;
        maxX = -10000;

        while (letter < end)
        {
            x = letter->x;
            if (x < minX)
                minX = x;
            y = letter->y;
            if (y < minY)
                minY = y;
            if ((int16_t)(x + letter->width) > maxX)
                maxX = x + letter->width;
            if ((int16_t)(y + letter->height) > maxY)
                maxY = y + letter->height;
            ++letter;
            if ((letter[-1].flagsAndChar & 0x20000000) != 0)
                break;
        }

        if (align)
        {
            if (align == 1)
            {
                dy = maxY - minY;
                for (; lineStart < letter; ++lineStart)
                    lineStart->y += (dy - (int16_t)lineStart->height) / 2;
            }
            else if (align == 2)
            {
                for (; lineStart < letter; ++lineStart)
                    lineStart->y = maxY - lineStart->height;
            }
        }
        else
        {
            dx = (minX + (int16_t)(unsigned int)size.x - maxX) / 2;
            for (; lineStart < letter; ++lineStart)
                lineStart->x += dx;
        }
    }
}

void STR_SetStringExt(struct sGameObjectMain* gao, int strIdx, int flags, char* src, Vec3* pivot)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    uint16_t num;
    Vec3 size;

    if (!src)
        return;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    STR_l_UseGaoMaterial(str, gao);
    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    if ((flags & 0x10000) != 0)
    {
        STR_ChangeStringLength(str, strIdx, STR_l_GetStringLength(src));
        STR_ParseString(str, strIdx, &str->letters[sd->index], src, sd->number, 0.050000001f, 0.050000001f, -1, 0);
        num = sd->number;
        sd->index2 = 0;
        sd->number2 = num;
    }
    if ((flags & 0x100000) != 0)
        sd->flags = flags;
    if ((flags & 0x40000) != 0)
        sd->pivotPoint = *pivot;
    if ((flags & 0x20000) != 0)
    {
        size.x = -1.0;
        size.y = 0.0;
        size.z = 0.0;
        STR_SetSize(gao, strIdx, 0, -1, &size);
    }
    if ((flags & 0x80000) != 0)
        STR_Align(gao, strIdx, 0, -1, ((int)(flags >> 16) >> 8) & 0xF);
}

void STR_InsertString(struct sGameObjectMain* gao, int strIdx, int firstChar, char* src)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    int len;
    int first;
    int oldNum;
    int color;

    if (!src)
        return;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    STR_l_UseGaoMaterial(str, gao);
    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    len = STR_l_GetStringLength(src);
    if (!len)
        return;

    first = STR_l_GetValidIndex(str, sd, firstChar);
    oldNum = sd->number;
    STR_ChangeStringLength(str, strIdx, len + oldNum);
    letter = &str->letters[first + sd->index];

    if (first != oldNum && sd->number > len)
    {
        color = letter->textColor;
        memcpy(&letter[len], letter, 16 * (sd->number - len - first));
        STR_ParseString(str, strIdx, letter, src, len, 0.0, 0.0, color, first);
    }
    else if (first)
    {
        STR_ParseString(str, strIdx, letter, src, len, 0.0, 0.0, letter[-1].textColor, first);
    }
    else
    {
        STR_ParseString(str, strIdx, letter, src, len, 0.050000001f, 0.050000001f, -1, 0);
    }

    if ((sd->flags & 0x200) == 0)
        STR_RecomputePos(str, sd, 0, -1);
}

void STR_SetFloat(struct sGameObjectMain* gao, int strIdx, float value, char precision)
{
    char format[8];
    char buf[32];

    strcpy(format, "%.0f");
    format[2] = precision + '0';
    sprintf(buf, format, value);
    STR_SetString(gao, strIdx, buf);
}

void STR_InsertFloat(tGameObjectMain* gao, int strIdx, int firstChar, float value, char precision)
{
    char format[8];
    char buf[32];

    strcpy(format, "%.0f");
    format[2] = precision + '0';
    sprintf(buf, format, value);
    STR_InsertString(gao, strIdx, firstChar, buf);
}

void STR_SetPivot(struct sGEOSTRStruct* str, int strIdx, Vec3* pivot)
{
    tGEOSTRStringData* sd;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
        sd->pivotPoint = *pivot;
}

Vec3* STR_pst_GetPivot(struct sGEOSTRStruct* str, int strIdx)
{
    tGEOSTRStringData* sd;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
        return &sd->pivotPoint;
    return NULL;
}

void STR_SetFlag(struct sGEOSTRStruct* str, int strIdx, int16_t flags)
{
    tGEOSTRStringData* sd;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
        sd->flags = flags;
}

int STR_ul_GetFlag(struct sGEOSTRStruct* str, int strIdx)
{
    tGEOSTRStringData* sd;
    int16_t flags = 0;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
        flags = sd->flags;
    return flags;
}

void STR_SetColor(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int color)
{
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
    {
        STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
        for (; letter < end; ++letter)
            letter->textColor = color;
    }
}

void STR_SetAlpha(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int alpha)
{
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
    {
        STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
        for (; letter < end; ++letter)
        {
            letter->textColor &= ~0xff000000;
            letter->textColor |= alpha << 24;
        }
    }
}

void STR_SetColorExt(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int color, int mask)
{
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int masked;
    int keep;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
    {
        STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
        masked = mask & color;
        keep = ~mask;
        for (; letter < end; ++letter)
            letter->textColor = masked | letter->textColor & keep;
    }
}

unsigned int STR_GetColor(struct sGEOSTRStruct* str, int strIdx, int charIdx)
{
    tGEOSTRStringData* sd;
    unsigned int color = 0;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
        color = str->letters[sd->index + STR_l_GetValidIndex(str, sd, charIdx)].textColor;
    return color;
}

void STR_ChangePage(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int page, int matchPage)
{
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int bits;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
    bits = page << 12;

    if (matchPage < 0)
    {
        for (; letter < end; ++letter)
            letter->flagsAndChar = bits | letter->flagsAndChar & 0xFFFC0FFF;
    }
    else if (matchPage < 256)
    {
        for (; letter < end; ++letter)
        {
            if ((letter->flagsAndChar & 0x3F000) == matchPage)
                letter->flagsAndChar = bits | letter->flagsAndChar & 0xFFFC0FFF;
        }
    }
    else if (matchPage < 512)
    {
        for (; letter < end; ++letter)
        {
            if ((letter->flagsAndChar & 0x3F000) != matchPage - 256)
                letter->flagsAndChar = bits | letter->flagsAndChar & 0xFFFC0FFF;
        }
    }
}

void STR_SetPos(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* pos)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    int first;
    int num;
    int at;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    first = STR_l_GetValidIndex(str, sd, firstChar);
    num = STR_l_GetValidNumber(str, sd, first, charNum);
    if (!num)
        return;

    if (pos->z == 0.0f)
        pos->z = STR_f_DefaultLetterDepth;

    at = first + sd->index;
    if (at < str->unkNum)
    {
        letter = &str->letters[at];
        letter->x = (int)((double)(int16_t)str->width * pos->x)
                  + D3D_FlatLayerOffsetX((int)gpst_DX_stDD->ct.cam.width,
                                         (int)gpst_DX_stDD->ct.cam.height);
        letter->y = (int)((double)(int16_t)str->height * pos->y);
    }
    if ((sd->flags & 0x200) == 0)
    {
        STR_l_UseGaoMaterial(str, gao);
        STR_RecomputePos(str, sd, first, num);
    }
}

void STR_EffectCheck(tGEOSTRStruct* str, int strIdx, int firstChar, int charNum)
{
    tGEOSTREffectStruct* effect;
    int i;
    int at;
    int left;

    for (i = 0; i < str->effectsNum; i++)
    {
        if (str->effect[i].stringid == strIdx)
            break;
    }
    if (i >= str->effectsNum)
        return;

    effect = &str->effect[i];
    at = effect->index;
    if (firstChar >= at)
        return;

    if (firstChar + charNum < at)
    {
        effect->index = at - charNum;
        return;
    }

    left = effect->number;
    effect->index = firstChar;
    if (left > 0)
    {
        effect->number = left - charNum;
        if (left - charNum < 0)
            effect->flags |= 1u;
    }
}

void STR_DelChar(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    int first;
    int num;
    int oldNum;
    int mark;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    mark = charNum > -10 ? -1 : -10 - charNum;
    first = STR_l_GetValidIndex(str, sd, firstChar);
    if (first >= sd->number)
        return;

    num = STR_l_GetValidNumber(str, sd, first, charNum);
    STR_EffectCheck(str, strIdx, first, num);
    oldNum = sd->number;
    if (num + first != oldNum)
    {
        letter = &str->letters[first + sd->index];
        memcpy(letter, &letter[num], 16 * (oldNum - first - num));
        if (mark != -1)
            letter->flagsAndChar &= ~((mark & 0xF) << 20);
    }
    STR_ChangeStringLength(str, strIdx, sd->number - num);
    STR_l_UseGaoMaterial(str, gao);
    if ((sd->flags & 0x200) == 0)
        STR_RecomputePos(str, sd, 0, -1);
}

void STR_SetChar(struct sGEOSTRStruct* str, int strIdx, int charIdx, int character)
{
    tGEOSTRStringData* sd;
    int at;

    sd = STR_pst_GetString(str, strIdx);
    if (sd)
    {
        at = STR_l_GetValidIndex(str, sd, charIdx);
        if (at < sd->number)
            str->letters[at + sd->index].flagsAndChar ^= (character ^ str->letters[at + sd->index].flagsAndChar) & 0x3FFFF;
    }
}

unsigned int STR_i_GetChar(tGEOSTRStruct* str, int strIdx, int charIdx)
{
    tGEOSTRStringData* sd;
    int at;

    sd = STR_pst_GetString(str, strIdx);
    if (sd && (at = STR_l_GetValidIndex(str, sd, charIdx), at < sd->number))
        return str->letters[at + sd->index].flagsAndChar & 0x3FFFF;
    return 0;
}

int STR_l_GetSubLength(struct sGEOSTRStruct* str, int strIdx, int lineIdx)
{
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int left;
    int run;
    int longest;
    int len;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return 0;

    letter = &str->letters[sd->index];
    end = &letter[sd->number];

    if (lineIdx < 0)
    {
        run = 0;
        longest = 0;
        for (; letter < end; ++letter)
        {
            ++run;
            if ((letter->flagsAndChar & 0x20000000) != 0)
            {
                if (run > longest)
                    longest = run;
                run = 0;
            }
        }
        if (run > longest)
            longest = run;
        return longest;
    }

    if (lineIdx)
    {
        left = lineIdx;
        do
        {
            if ((letter->flagsAndChar & 0x20000000) != 0)
                --left;
            if (++letter >= end)
                return 0;
        } while (left);
    }

    for (len = 0; letter < end; ++letter)
    {
        ++len;
        if ((letter->flagsAndChar & 0x20000000) != 0)
            break;
    }
    return len;
}

Vec3* STR_pst_GetMin(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* min)
{
    tGEOSTRStruct* str;

    if (!min)
        return 0;

    str = STR_pst_Get(gao);
    if (str)
    {
        STR_l_UseGaoMaterial(str, gao);
        STR_GetRectShort(str, strIdx, firstChar, charNum, min, 0);
        min->x = (float)(min->x / (double)(uint16_t)str->width);
        min->y = (float)(min->y / (double)(uint16_t)str->height);
    }
    return min;
}

Vec3* STR_pst_GetMax(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* max)
{
    tGEOSTRStruct* str;

    if (!max)
        return 0;

    str = STR_pst_Get(gao);
    if (str)
    {
        STR_l_UseGaoMaterial(str, gao);
        STR_GetRectShort(str, strIdx, firstChar, charNum, 0, max);
        max->x = (float)(max->x / (double)(uint16_t)str->width);
        max->y = (float)(max->y / (double)(uint16_t)str->height);
    }
    return max;
}

void STR_SetFrame(struct sGameObjectMain* gao, int strIdx, char frame, int16_t flags)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;

    str = STR_pst_Get(gao);
    if (str)
    {
        sd = STR_pst_GetString(str, strIdx);
        if (sd)
            sd->frame = flags & 0xFFF | ((frame & 0x3F) << 12);
    }
}

void STR_EffectDelAll(tGEOSTRStruct* str)
{
    str->effectsNum = 0;
    STR_spst_CurEffect = NULL;
}

void STR_EffectDel(struct sGameObjectMain* gao, int effectId)
{
    tGEOSTRStruct* str;
    uint8_t last;
    int at;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    if (effectId < 0)
    {
        STR_EffectDelAll(str);
        return;
    }

    STR_SetCurEffect(str, effectId);
    if (STR_spst_CurEffect)
    {
        at = ((char*)STR_spst_CurEffect - (char*)&str->effect[0]) / sizeof(tGEOSTREffectStruct);
        last = str->effectsNum - 1;
        str->effectsNum = last;
        if (at < last)
            memcpy(STR_spst_CurEffect, &str->effect[last], sizeof(tGEOSTREffectStruct));
        STR_spst_CurEffect = NULL;
    }
}

void STR_SetFramePos(struct sGameObjectMain* gao, int strIdx, Vec3* pos, Vec3* size)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;

    str = STR_pst_Get(gao);
    if (str)
    {
        sd = STR_pst_GetString(str, strIdx);
        if (sd)
        {
            sd->framePosX1 = (int)((double)(uint16_t)str->width * pos->x);
            sd->framePosY1 = (int)((double)(uint16_t)str->height * pos->y);
            sd->framePosX2 = (int)((double)(uint16_t)str->width * size->x);
            sd->framePosY2 = (int)((double)(uint16_t)str->height * size->y);
        }
    }
}

void STR_SetFrameBorderSize(struct sGameObjectMain* gao, int strIdx, float border)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;

    str = STR_pst_Get(gao);
    if (str)
    {
        sd = STR_pst_GetString(str, strIdx);
        if (sd)
        {
            sd->frameBorderSize1 = (int)((double)(uint16_t)str->width * border);
            sd->frameBorderSize2 = (int)((double)(uint16_t)str->height * border);
        }
    }
}

void STR_SetFrameColor(struct sGameObjectMain* gao, int strIdx, int color)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;

    str = STR_pst_Get(gao);
    if (str)
    {
        sd = STR_pst_GetString(str, strIdx);
        if (sd)
            sd->frameColor = color;
    }
}

void STR_SetSubStringWithLine(struct sGameObjectMain* gao, int strIdx, int firstLine, int lineNum)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    int left;
    int at;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    letter = &str->letters[sd->index];
    at = 0;

    if (firstLine > 0)
    {
        left = firstLine;
        while (1)
        {
            if ((letter->flagsAndChar & 0x20000000) != 0)
                --left;
            ++letter;
            if (++at >= sd->number)
                return;
            if (left <= 0)
                break;
        }
    }

    sd->index2 = at;
    if (lineNum > 0)
    {
        left = lineNum;
        do
        {
            if ((letter->flagsAndChar & 0x20000000) != 0)
                --left;
            ++letter;
            ++at;
        } while (at < sd->number && left > 0);
    }
    sd->number2 = at - sd->index2;
}

void STR_SetSubStringWithMark(struct sGameObjectMain* gao, int strIdx, int firstMark, int markNum)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    uint16_t num;
    int left;
    int at;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    letter = &str->letters[sd->index];
    at = 0;

    if (firstMark > 0)
    {
        left = firstMark;
        while (1)
        {
            if ((letter->flagsAndChar & 0xF00000) == 0xF00000)
                --left;
            if (!left)
                sd->index2 = at;
            ++letter;
            if (++at >= sd->number)
                return;
            if (left <= 0)
                break;
        }
    }

    if (markNum > 0)
    {
        left = markNum;
        do
        {
            if ((letter->flagsAndChar & 0xF00000) == 0xF00000)
                --left;
            ++letter;
            ++at;
        } while (at < sd->number && left > 0);
    }

    num = at - sd->index2;
    sd->number2 = num;
    if (at < sd->number && num)
        sd->number2 = num - 1;
}

void STR_SetSubString(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    uint16_t num;
    int first;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    first = STR_l_GetValidIndex(str, sd, firstChar);
    if (first < sd->number)
    {
        num = STR_l_GetValidNumber(str, sd, first, charNum);
        sd->index2 = first;
        sd->number2 = num;
    }
}

int STR_i_CountLine(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int lines;

    str = STR_pst_Get(gao);
    if (!str)
        return 0;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return 0;

    STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
    lines = 0;
    for (; letter < end; ++letter)
    {
        if ((letter->flagsAndChar & 0x20000000) != 0)
            ++lines;
    }
    return lines + 1;
}

int STR_i_CountMark(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int mark)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int marks;

    str = STR_pst_Get(gao);
    if (!str)
        return 0;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return 0;

    STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
    marks = 0;
    for (; letter < end; ++letter)
    {
        if (((letter->flagsAndChar >> 20) & 0xF) == mark)
            ++marks;
    }
    return marks + 1;
}

void STR_ToString(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, unsigned char* dest)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int page;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
    for (; letter < end; ++letter)
    {
        if ((letter->flagsAndChar & 0x3F000) != 0)
        {
            page = (letter->flagsAndChar >> 12) & 0x3F;
            *dest++ = '\\';
            *dest++ = 'p';
            if (page >= 10)
            {
                *dest++ = page / 10 + '0';
                page = page % 10;
            }
            *dest++ = page + '0';
            *dest++ = '\\';
        }
        *dest = letter->flagsAndChar;
        ++dest;
    }
    *dest = 0;
}

void STR_SetLetterFlag(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int flags)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
    for (; letter < end; ++letter)
    {
        letter->flagsAndChar &= ~0xff000000;
        letter->flagsAndChar |= flags << 24;
    }
}

int STR_GetLetterFlag(struct sGameObjectMain* gao, int strIdx, int charIdx)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    int at;

    str = STR_pst_Get(gao);
    // preserved: the string is not checked for NULL before it is used
    sd = STR_pst_GetString(str, strIdx);
    if (sd && (at = STR_l_GetValidIndex(str, sd, charIdx), at < sd->number))
        return str->letters[at + sd->index].flagsAndChar >> 24;
    return 0;
}

void STR_ToUpper(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    unsigned int ch;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    STR_l_UseGaoMaterial(str, gao);
    STR_l_LetterRange(str, sd, firstChar, charNum, &letter, &end);
    for (; letter < end; ++letter)
    {
        ch = (uint8_t)letter->flagsAndChar;
        if (ch >= 'a' && (uint8_t)letter->flagsAndChar <= 'z')
        {
            letter->flagsAndChar &= ~0xff;
            letter->flagsAndChar |= ch - ' ';
        }
    }
}

void STR_EffectSeti(struct sGameObjectMain* gao, int effectId, int field, int value)
{
    tGEOSTRStruct* str;
    unsigned int* fill;
    int i;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    if (field == -1)
    {
        if (!effectId)
            STR_EffectDelAll(str);
        else if (effectId == 1)
            STR_EffectDelAllForAString(str, value);
        return;
    }

    if (field == -2)
    {
        if (effectId >= 0)
        {
            if (effectId < 24)
                str->shorts[effectId] = value;
        }
        else
        {
            // TODO FIXME: ezt megcsinálni igényesre, mert ez elég gáz így shortból int és az bele a shortos mezõbe
            fill = (unsigned int*)(str->shorts);
            for (i = 0; i < 12; i++) {
                fill[i] = value;
            }
        }
        return;
    }

    STR_SetCurEffect(str, effectId);
    if (!STR_spst_CurEffect)
        return;

    switch (field)
    {
    case 0:
        STR_spst_CurEffect->flags ^= (STR_spst_CurEffect->flags ^ (value << 16)) & 0xFF0000;
        break;
    case 1:
        STR_spst_CurEffect->flags ^= ((uint16_t)STR_spst_CurEffect->flags ^ (uint16_t)((int32_t)value << 8)) & 0xFF00;
        break;
    case 200:
    case 302:
    case 400:
        *(uint32_t*)&(STR_spst_CurEffect->color1) = value;
        break;
    case 201:
    case 303:
    case 401:
    case 1600:
        *(uint32_t*)&(STR_spst_CurEffect->color2) = value;
        break;
    case 300:
        *(uint32_t*)&(STR_spst_CurEffect->f1) = value;
        break;
    case 301:
        *(uint32_t*)&(STR_spst_CurEffect->timeElapsed) = value;
        break;
    default:
        break;
    }
}

int STR_i_EffectGetId(struct sGEOSTRStruct* str, int strIdx)
{
    int i;

    for (i = 0; i < str->effectsNum; i++)
    {
        if (str->effect[i].stringid == strIdx)
        {
            STR_spst_CurEffect = &str->effect[i];
            return STR_spst_CurEffect->effectID;
        }
    }
    return -1;
}

int STR_EffectGeti(struct sGameObjectMain* gao, int effectId, int field)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (!str)
        return -1;
    if (effectId == -1)
        return STR_i_EffectGetId(str, field);
    STR_SetCurEffect(str, effectId);
    if (!STR_spst_CurEffect)
        return -1;
    if (!field)
        return (STR_spst_CurEffect->flags >> 16) & 0xff;
    if (field == 1)
        return (STR_spst_CurEffect->flags >> 8) & 0xff;
    if (field != 2)
        return -1;
    return (STR_spst_CurEffect->flags >> 24) & 0xff;
}

void STR_EffectSetf(struct sGameObjectMain* gao, int effectId, int field, float value)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    int first;

    str = STR_pst_Get(gao);
    if (!str)
        return;

    STR_SetCurEffect(str, effectId);
    if (!STR_spst_CurEffect)
        return;

    switch (field)
    {
    case 150:
    case 250:
    case 450:
    case 550:
    case 650:
    case 1150:
    case 1350:
    case 1550:
    case 1650:
    case 1750:
    case 1850:
    case 2050:
        STR_spst_CurEffect->f1 = value;
        break;
    case 151:
    case 451:
        STR_spst_CurEffect->f3 = value;
        break;
    case 551:
    case 651:
    case 751:
    case 1551:
    case 1751:
        STR_spst_CurEffect->timeElapsed = value;
        break;
    case 552:
    case 652:
    case 1152:
    case 1351:
    case 1651:
    case 2051:
        STR_spst_CurEffect->color1 = value;
        break;
    case 553:
    case 1151:
        STR_spst_CurEffect->color2 = value;
        break;
    case 750:
        if (value >= 0.0)
        {
            STR_spst_CurEffect->f1 = value;
            break;
        }
        sd = STR_pst_GetString(str, STR_spst_CurEffect->stringid);
        if (sd)
        {
            first = STR_l_GetValidIndex(str, sd, STR_spst_CurEffect->index);
            STR_spst_CurEffect->f1 = (float)(-value * (double)STR_l_GetValidNumber(str, sd, first, STR_spst_CurEffect->number));
        }
        break;
    case 752:
        STR_spst_CurEffect->color2 = value;
        STR_spst_CurEffect->color1 = 0.0;
        break;
    default:
        break;
    }
}

void STR_ReinitAll()
{
    STR_sgl_NumberOfFont = 0;
}

void STR_Init3DStringExtExt(struct sGameObjectMain* gao, char* str, int maxLetters)
{
    tGEOGRO* gro;
    tGeoUVElem* uv;
    tGeoUVElem* uvEnd;
    tGEOSTRLetter* letter;
    tFontGlypthElem* fge;
    uint16_t page;
    int quads;
    int count;
    int glyph;
    int i;
    tGEOSTRStruct geostr;
    tGEOSTRLetter letters[256];

    gro = (tGEOGRO*)OBJ_p_GetGro(gao);
    if (!gro || gro->intf->id != 1)
        return;

    count = STR_l_GetStringLength(str);
    quads = gro->uvNum >> 2;
    if (count >= quads)
        count = gro->uvNum >> 2;
    if (maxLetters >= 0 && count > maxLetters)
        count = maxLetters;

    // preserved: the scratch string takes the camera width as its height and the height as its width
    geostr.height = (unsigned int)gpst_DX_stDD->ct.cam.width;
    geostr.width = (unsigned int)gpst_DX_stDD->ct.cam.height;
    STR_ParseString(&geostr, 0, letters, str, STR_l_GetStringLength(str), 0.050000001f, 0.050000001f, -1, 0);
    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
    geostr.fontDesc = 0;
    geostr.fontMaterialID = 0;
    geostr.fontDesc = STR_l_FontDescForPage(0);
    if (!geostr.fontDesc)
        return;

    uv = gro->UVs;
    uvEnd = &uv[4 * quads];
    gro->triangleChunks->materialNum = (letters[0].flagsAndChar >> 12) & 0x3F;
    letter = letters;
    geostr.fontMaterialID = -1;
    for (i = 0; i < count; i++)
    {
        page = (letter->flagsAndChar >> 12) & 0x3F;
        if (page != geostr.fontMaterialID)
        {
            geostr.fontMaterialID = (letter->flagsAndChar >> 12) & 0x3F;
            geostr.fontDesc = STR_l_FontDescForPage(page);
        }
        glyph = STR_l_GlyphIndex(letter->flagsAndChar);
        fge = &geostr.fontDesc->fge[glyph];
        uv[0].u = *(&fge->x + STRU1Offset);
        uv[0].v = *(&fge->w + STRV1Offset);
        uv[1].u = *(&fge->x + STRU2Offset);
        uv[1].v = *(&fge->w + STRV2Offset);
        uv[2].u = *(&fge->x + STRU3Offset);
        uv[2].v = *(&fge->w + STRV3Offset);
        uv[3].u = *(&fge->x + STRU4Offset);
        uv[3].v = *(&fge->w + STRV4Offset);
        uv += 4;
        ++letter;
    }
    for (; uv < uvEnd; ++uv)
    {
        uv->u = 0.0;
        uv->v = 0.0;
    }
    if (gro->d3dIndexBuf)
        PC_FreeABackCache(gro);
}

void STR_Init3DString(struct sGameObjectMain* gao, char* str)
{
    STR_Init3DStringExtExt(gao, str, -1);
}

void STR_Init3DStringExt(struct sGameObjectMain* gao, char* str, int maxLetters)
{
    STR_Init3DStringExtExt(gao, str, maxLetters);
}

void STR_Init3DStringCenter(struct sGameObjectMain* gao, char* str, int maxLetters)
{
    STR_Init3DStringExtExt(gao, str, maxLetters);
}

void STR_Init3DStringFromSTR(struct sGameObjectMain* gao, struct sGameObjectMain* source, int strIdx, int maxLetters)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* end;
    int i;
    char buf[256];

    str = STR_pst_Get(source);
    if (!str)
        return;

    sd = STR_pst_GetString(str, strIdx);
    if (!sd)
        return;

    STR_l_LetterRange(str, sd, 0, -1, &letter, &end);
    for (i = 0; letter < end; ++i)
    {
        buf[i] = letter->flagsAndChar;
        ++letter;
    }
    buf[i] = 0;
    STR_Init3DStringExtExt(gao, buf, maxLetters);
}
