#include "memory.h"
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

void STR_3DStringList_Add(struct sGameObjectMain* a1)
{
    int v1;
    int v2;

    v1 = STR_3DString_Number;
    if (STR_3DString_Number != 64)
    {
        v2 = 0;
        if (STR_3DString_Number <= 0)
        {
        LABEL_5:
            STR_3DString_Obj[STR_3DString_Number] = a1;
            STR_3DString_Number = v1 + 1;
        }
        else
        {
            while (STR_3DString_Obj[v2] != a1)
            {
                if (++v2 >= STR_3DString_Number)
                    goto LABEL_5;
            }
        }
    }
}

tFontData* STR_pst_GetFont(int a1)
{
    int v1;
    tFontData* i; 

    v1 = 0;
    if (STR_sgl_NumberOfFont <= 0)
        return 0;
    for (i = STR_sgpst_Font; i->texKey != a1; ++i)
    {
        if (++v1 >= STR_sgl_NumberOfFont)
            return 0;
    }
    return &STR_sgpst_Font[v1];
}

void STR_FontResolveAlternativeKey()
{
    int v0;
    int i;
    tFontData* v2;

    v0 = STR_FontAltKeyNumber;
    for (i = 0; i < v0; ++i)
    {
        v2 = STR_pst_GetFont(STR_FontAltKey[i].key);
        if (v2)
            v2->texKeyAlternative = STR_FontAltKey[i].key2;
    }
    STR_FontAltKeyNumber = 0;
}

void STR_FontAddAlternativeKey(int a1, int a2)
{
    int v2;

    v2 = STR_FontAltKeyNumber;
    if (STR_FontAltKeyNumber < 16)
    {
        STR_FontAltKey[STR_FontAltKeyNumber].key = a1;
        STR_FontAltKey[v2].key2 = a2;
        STR_FontAltKeyNumber = v2 + 1;
    }
}

BOOL STR_b_LoadFontDescriptor(struct sFontData* a1, int key)
{
    int v2;
    char* v4;
    unsigned int v5;
    tFontGlypthElem* fge;
    char* offset;
    int size;
    char* a1a;
    char Str1[8];

    v2 = BIG_ul_SearchKeyToPos(key);
    if (v2 == -1)
        return 0;
    v4 = BIG_pc_ReadFileTmp(v2, &size);
    offset = v4;
    if ((unsigned int)size < 12)
        return 0;
    LOA_ReadCharArray(&offset, Str1, 8u, 1);
    if (strncmp(Str1, "FONTDESC", 8u))
        return 0;
    a1a = &v4[size - 4];
    if (LOA_ReadLong(&a1a, 0, 4) != -1)
        return 0;
    LOA_ReadLong(&offset, 0, 1);
    if (offset < &v4[size - 4])
    {
        while (1)
        {
            v5 = LOA_ReadLong(&offset, 0, 1);
            if (v5 < 32 || v5 > 255)
                break;
            fge = &(a1->fge[v5 - 32]);
            fge->x = LOA_ReadFloat(&offset, 0, 1);
            fge->w = LOA_ReadFloat(&offset, 0, 1);
            fge->y = LOA_ReadFloat(&offset, 0, 1);
            fge->h = LOA_ReadFloat(&offset, 0, 1);
            if (offset >= &v4[size - 4])
                return LOA_ReadLong(&offset, 0, 1) == -1;
        }
        return 0;
    }
    return LOA_ReadLong(&offset, 0, 1) == -1;
}

void STR_AddFont(int key, struct sEngTexIntermediateStruct* a2)
{
    tFontData* v2;
    int v3;
    tFontData* v4;
    tFontData* v5;
    BOOL v6;
    int v7;
    size_t v8;

    if (a2 && a2->imgDataPtr)
    {
        v2 = STR_sgpst_Font;
        v3 = STR_sgl_NumberOfFont;
        if (!STR_sgl_NumberOfFont)
            goto LABEL_10;
        v4 = STR_sgpst_Font;
        v5 = &STR_sgpst_Font[STR_sgl_NumberOfFont];
        v6 = STR_sgpst_Font == v5;
        if (STR_sgpst_Font < v5)
        {
            do
            {
                if (v4->texKey == key)
                    break;
                ++v4;
            } while (v4 < v5);
            v6 = v4 == v5;
        }
        if (v6 || !v4)
        {
        LABEL_10:
            v7 = STR_sgl_MaxNumberOfFont;
            if (STR_sgl_NumberOfFont >= STR_sgl_MaxNumberOfFont)
            {
                STR_sgl_MaxNumberOfFont += 20;
                v8 = sizeof(tFontData) * (v7 + 20);
                if (STR_sgpst_Font)
                    v2 = (tFontData*)MEM_p_Realloc(STR_sgpst_Font, v8);
                else
                    v2 = (tFontData*)MEM_p_Alloc(v8);
                v3 = STR_sgl_NumberOfFont;
                STR_sgpst_Font = v2;
            }
            v4 = &v2[v3];
            STR_sgl_NumberOfFont = v3 + 1;
            v4->texKey = key;
        }
        v4->width = a2->realWidth;
        v4->height = a2->realHeight;
        memset(v4->fge, 0, sizeof(v4->fge));
        if (a2->tp.magicParams.fontDescriptorKey)
            STR_b_LoadFontDescriptor(v4, a2->tp.magicParams.fontDescriptorKey);
    }
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

struct sFontData* STR_pst_GetFontDesc(struct sGEOMAT* a1, int matNum)
{
    tGEOMAT* v2;
    unsigned int v4;
    int v5;
    tFontData* v6;
    int v7;
    int v8;
    tMultiTextureElemStruct* v9;
    int v10;
    tFontData* v11;
    tFontData* v12;
    tFontData* v13;
    tFontData* v14;
    int v15;
    tFontGlypthElem* v16;
    int v17;
    size_t v18;

    v2 = a1;
    if (!a1)
        return 0;
    if (a1->intf->id == 4)
    {
        v4 = matNum;
        if (matNum >= a1->multiMaterialNumIfType4ThenAmbiantColor)
            v4 = 0;
        v2 = a1->multiMaterialPtrIfType4ThenDiffuseColor[v4];
    }
    v5 = v2->intf->id;
    v6 = STR_sgpst_Font;
    v7 = STR_sgl_NumberOfFont;
    if (v5 == 3)
    {
        v8 = v2->idxOrPtr;
    }
    else
    {
        if (v5 != 5)
            goto LABEL_19;
        v9 = (tMultiTextureElemStruct*)v2->idxOrPtr;
        if (!v9)
            goto LABEL_19;
        v8 = v9->texElemIdx;
    }
    if (v8 != -1)
    {
        if ((TEX_gst_GlobalList.texElems[v8].texElemFlag & 0x40) != 0)
            v8 = TEX_gst_GlobalList.texElems[v8].palettedData.textureID;
        v10 = TEX_gst_GlobalList.texElems[v8].texKey;
        v11 = STR_sgpst_Font;
        v12 = &STR_sgpst_Font[STR_sgl_NumberOfFont];
        if (STR_sgpst_Font < v12)
        {
            while (v11->texKey != v10 && v11->texKeyAlternative != v10)
            {
                if (++v11 >= v12)
                    goto LABEL_19;
            }
            return v11;
        }
    }
LABEL_19:
    v13 = STR_sgpst_Font;
    v14 = &STR_sgpst_Font[STR_sgl_NumberOfFont];
    if (STR_sgpst_Font >= v14)
    {
    LABEL_22:
        v15 = STR_sgl_MaxNumberOfFont;
        if (STR_sgl_NumberOfFont >= STR_sgl_MaxNumberOfFont)
        { 
            STR_sgl_MaxNumberOfFont += 20;
            v18 = sizeof(tFontData) * (v15 + 20);
            if (STR_sgpst_Font)
                v6 = (tFontData*)MEM_p_Realloc(STR_sgpst_Font, v18);
            else
                v6 = (tFontData*)MEM_p_Alloc(v18);
            v7 = STR_sgl_NumberOfFont;
            STR_sgpst_Font = v6;
        }
        v11 = &v6[v7];
        STR_sgl_NumberOfFont = v7 + 1;
        v11->mat = v2;
        v16 = v11->fge;
        v17 = 224;
        do
        {
            v16->x = 0.0;
            v16->y = 0.0;
            v16->w = 0.0;
            v16->h = 1.0;
            ++v16;
            --v17;
        } while (v17);
        return v11;
    }
    while (v13->mat != v2)
    {
        if (++v13 >= v14)
            goto LABEL_22;
    }
    return v13;
}

struct sGEOSTRStruct* STR_pst_Get(struct sGameObjectMain* a1)
{
    tGEOSTRStruct* result;

    if ((a1->flags & 0x4000) == 0
        || (result = (tGEOSTRStruct*)((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) == 0
        || result->intf->id != 10)
    {
        result = NULL;
    }
    return result;
}

int STR_l_GetLength(struct sGEOSTRStruct* a1, unsigned int a2)
{
    int result;

    if (a2 < a1->stringNum)
        result = a1->stringData[a2].number;
    else
        result = -1;
    return result;
}

struct sGEOSTRStringData* STR_pst_GetString(struct sGEOSTRStruct* a1, int idx)
{
    tGEOSTRStringData* result;

    if (a1->letters && idx >= 0 && idx < a1->stringNum)
        result = &a1->stringData[idx];
    else
        result = 0;
    return result;
}

int STR_l_GetMarkIndex(struct sGEOSTRStruct* a1, struct sGEOSTRStringData* a2, int a3)
{
    tGEOSTRLetter* v3;
    int result;

    v3 = &a1->letters[a2->index];
    result = 0;
    if (a2->number)
    {
        do
        {
            if (((v3->flagsAndChar >> 20) & 0xF) == a3)
                break;
            ++result;
            ++v3;
        } while (result < a2->number);
    }
    return result;
}

int STR_l_GetValidIndex(struct sGEOSTRStruct* a1, struct sGEOSTRStringData* a2, int a3)
{
    int result;

    result = a3;
    if (a3 <= -50)
        result = STR_l_GetMarkIndex(a1, a2, (-2 - (byte)a3) & 0xF);
    if (result <= -10)
        result = STR_l_GetMarkIndex(a1, a2, (6 - (byte)result) & 0xF) + 1;
    if (result == -2)
        result = a2->index2;
    if (result < 0 || result >= a2->number)
        result = a2->number;
    return result;
}

int STR_l_GetValidNumber(struct sGEOSTRStruct* a1, struct sGEOSTRStringData* a2, int a3, int a4)
{
    int result;
    int v5;
    int v6;

    result = a4;
    if (a4 <= -10)
    {
        v5 = STR_l_GetMarkIndex(a1, a2, (6 - (byte)a4) & 0xF);
        v6 = a3;
        if (v5 >= a3)
            return v5 - a3;
        return a2->number - v6;
    }
    v6 = a3;
    if (a4 == -2)
        result = a2->index2 + a2->number2 - a3;
    if (result <= 0 || a3 + result > a2->number)
        return a2->number - v6;
    return result;
}

void STR_GetRectShort(struct sGEOSTRStruct* a1, int idx, int a3, int a4, Vec3* min, Vec3* max)
{
    __int16 v6;
    __int16 v7;
    tGEOSTRStringData* v8;
    tGEOSTRStringData* v9;
    int v10;
    tGEOSTRLetter* v11; 
    tGEOSTRLetter* i;
    __int16 v13;
    __int16 v14;
    __int16 v15;
    __int16 v16;
    __int16 v17;

    v6 = 10000;
    v7 = -10000;
    v16 = 10000;
    v15 = 10000;
    v17 = -10000;
    v8 = STR_pst_GetString(a1, idx);
    v9 = v8;
    if (v8)
    {
        v10 = STR_l_GetValidIndex(a1, v8, a3);
        v11 = &a1->letters[v10 + v9->index];
        for (i = &v11[STR_l_GetValidNumber(a1, v9, v10, a4)]; v11 < i; ++v11)
        {
            v13 = v11->x;
            if (v13 < v15)
                v15 = v11->x;
            v14 = v11->y;
            if (v14 < v16)
                v16 = v11->y;
            if ((__int16)(v13 + v11->width) > v7)
                v7 = v13 + v11->width;
            if ((__int16)(v14 + v11->height) > v17)
                v17 = v14 + v11->height;
        }
        v6 = v15;
    }
    if (min)
    {
        min->z = 0.0;
        if (v6 == 10000)
        {
            min->x = 0.0;
            min->y = 0.0;
        }
        else
        {
            min->x = (float)v6;
            min->y = (float)v16;
        }
    }
    if (max)
    {
        max->z = 0.0;
        if (v6 == 10000)
        {
            max->x = 0.0;
            max->y = 0.0;
        }
        else
        {
            max->x = (float)v7;
            max->y = (float)v17;
        }
    }
}

void STR_GlobalSet(int a1, char* a2)
{
    int v3;
    int v4;
    char v5;
    char v6;
    char v7;
    char v8;

    if ((uint8_t)a1 < 24u)
    {
        v3 = 0;
        while (*a2)
        {
            v4 = a1;
            STR_sgaac_Global[a1].str[v3] = *a2;
            v5 = a2[1];
            if (!v5)
            {
                STR_sgaac_Global[a1].str[v3 + 1] = 0;
                return;
            }
            STR_sgaac_Global[v4].str[v3 + 1] = v5;
            v6 = a2[2];
            if (!v6)
            {
                STR_sgaac_Global[a1].str[v3 + 2] = 0;
                return;
            }
            STR_sgaac_Global[v4].str[v3 + 2] = v6;
            v7 = a2[3];
            if (!v7)
            {
                STR_sgaac_Global[a1].str[v3 + 3] = 0;
                return;
            }
            STR_sgaac_Global[v4].str[v3 + 3] = v7;
            v8 = a2[4];
            if (!v8)
            {
                v3 += 4;
                break;
            }
            STR_sgaac_Global[v4].str[v3 + 4] = v8;
            v3 += 5;
            a2 += 5;
            if (v3 >= 15)
            {
                STR_sgaac_Global[a1].str[v3] = 0;
                return;
            }
        }
        STR_sgaac_Global[a1].str[v3] = 0;
    }
}

void STR_GlobalSetChar(int a1, int a2, char a3)
{
    if ((uint8_t)a1 < 24u)
    {
        STR_sgaac_Global[a1].str[15] = 0;
        if ((uint8_t)a2 < 15u)
            STR_sgaac_Global[a1].str[a2] = a3;
    }
}

tStrGlobalStruct* STR_sz_GlobalGet(int a1)
{
    tStrGlobalStruct* result;

    if ((uint8_t)a1 < 24u)
        result = &STR_sgaac_Global[a1];
    else
        result = 0;
    return result;
}

void STR_Reset(tGameObjectMain* a1)
{
    tGEOSTRStruct* v1;

    v1 = STR_pst_Get(a1);
    if (v1)
        STR_Reinit((tGroGeneric*)v1);
}

char* STR_GetKeyName(int a1, int a2)
{
    int v2;
    int v3;
    tInoButtonNames* v4;
    int v8;
    int v9;
    int v10;

    if (!pcINO_keyMaps)
        return "";
    if (a1 >= '0' && a1 <= '=')
    {
        v8 = pcINO_keyMaps[a1 - '0'];
        if (v8 >= 0)
            return buttonNames[v8].str;
        return prefixOneSpace;
    }
    if (a1 == '?')
    {
        v8 = pcINO_keyMaps[3];
        if (v8 >= 0)
            return buttonNames[v8].str;
        return prefixOneSpace;
    }
    if (a1 < 'A' || a1 > 'S')
    {
        pcINO_KeyNamesToRet[0] = a1;
        pcINO_KeyNamesToRet[1] = 0;
        return pcINO_KeyNamesToRet;
    }
    if (a1 == 'A')
    {
        switch (a2)
        {
        case 1214:
            v9 = 16;
            v10 = 14;
            goto LABEL_31;
        case 1315:
            v9 = 17;
            v10 = 15;
            goto LABEL_31;
        case 2345:
            return pcINO_GetFourKeyNamesCattedTogether(14, 15, 17, 16);
        }
        return "";
    }
    if (a1 == 'F')
    {
        switch (a2)
        {
        case 1214:
            v9 = 20;
            v10 = 18;
            goto LABEL_31;
        case 1315:
            v9 = 21;
            v10 = 19;
            goto LABEL_31;
        case 2345:
            return pcINO_GetFourKeyNamesCattedTogether(18, 19, 21, 20);
        }
        return "";
    }
    if (a1 != 'O')
        return "";
    if (a2 == 24)
    {
        v9 = 10;
        v10 = 8;
    }
    else
    {
        v9 = 11;
        v10 = 9;
    }
LABEL_31:
    v2 = pcINO_keyMaps[v10];
    v3 = pcINO_keyMaps[v9];
    if (v2 == -1)
        return "\\p16\\2";
    if (v2 < 0 || v3 < 0)
        return prefixOneSpace;
    v4 = &buttonNames[v2];
    strcpy(pcINO_KeyNamesToRet, v4->str);   
    strcat(pcINO_KeyNamesToRet, buttonNames[v3].str);
    return pcINO_KeyNamesToRet;
}

char* STR_sz_ControlsReadFormat_pcINO(char* str)
{
    unsigned int v1;
    char* v2;
    unsigned int v3;
    char* v4;
    char v5;
    char* v6;
    char* v7;
    unsigned int v8;
    unsigned int v9;
    unsigned int v10;
    char* v11;
    char* v12;
    char* j;
    unsigned int v14;
    char* v15;
    char* i;
    int v17;
    int v18;
    char v19;
    char* v20;
    char v21;
    char* v22;
    char* v23;
    char v24;
    char* v25;
    char* k;
    unsigned int v28;
    int a2;
    int v30;
    unsigned int v31;
    int v32;
    int a1;

    v1 = 0;
    a1 = pcINO_SelectedControlSubmode;
    v2 = pcINO_ControlsReadFormatBuffer;
    v3 = 0;
    v32 = pcINO_SelectedControlMode;
    v30 = 0;
    v28 = 0;
    v31 = 0;
    a2 = 0;
    if (!pcINO_ControlsReadFormatBuffer)
    {
        pcINO_ControlsReadFormatBufferSize = 512;
        v2 = (char*)MEM_p_Alloc(512u);
        pcINO_ControlsReadFormatBuffer = v2;
    }
    v4 = str;
    v5 = *str;
    v6 = v2;
    if (*str)
    {
        do
        {
            if (v5 == '\\')
            {
                switch (v4[1])
                {
                case '-':
                case 'n':
                    *v6 = '\\';
                    v19 = v4[1];
                    v20 = v6 + 1;
                    ++v4;
                    *v20 = v19;
                    v6 = v20 + 1;
                    goto LABEL_38;
                case 'O':
                    v17 = strtoul(v4 + 2, &str, 10);
                    INO_SetControlMode_inner_sub_48ACB0(v17);
                    goto LABEL_29;
                case 'P':
                    if (v3 != 16)
                        v31 = v3;
                    v14 = strtoul(v4 + 2, &str, 10);
                    v11 = str;
                    v1 = v14;
                    v28 = v14;
                    if (!str)
                        goto LABEL_37;
                    if (v14 != 16)
                    {
                        v15 = str + 1;
                        for (i = v4; i != v15; ++i)
                            *v6++ = *i;
                    }
                    break;
                case 'b':
                    v9 = strtoul(v4 + 2, &str, 10);
                    v4 = str;
                    a2 = v9;
                    goto LABEL_38;
                case 'd':
                    *v6 = 92;
                    v21 = v4[1];
                    v22 = v6 + 1;
                    v23 = v4 + 1;
                    *v22 = v21;
                    v24 = v23[1];
                    ++v22;
                    v4 = v23 + 1;
                    *v22 = v24;
                    v6 = v22 + 1;
                    goto LABEL_38;
                case 'o':
                    v18 = strtoul(v4 + 2, &str, 10);
                    INO_SetControlSubmode_inner_sub_48A650(v18);
                LABEL_29:
                    if (str)
                    {
                        v30 = 1;
                        v4 = str;
                    }
                    goto LABEL_38;
                case 'p':
                    v10 = strtoul(v4 + 2, &str, 10);
                    v11 = str;
                    v1 = v10;
                    if (!str)
                        goto LABEL_37;
                    if (v10 != 16)
                    {
                        v12 = str + 1;
                        for (j = v4; j != v12; ++j)
                            *v6++ = *j;
                    }
                    break;
                default:
                    v25 = strchr(v4 + 2, '\\');
                    v11 = v25;
                    str = v25;
                    if (!v25)
                        goto LABEL_37;
                    for (k = v4; k != v25 + 1; ++k)
                        *v6++ = *k;
                    break;
                }
                v4 = v11;
            LABEL_37:
                v3 = v28;
            }
            else if (v1 == 16)
            {
                v7 = STR_GetKeyName(v5, a2);
                if (v7 && *v7)
                {
                    v8 = v3;
                    if (v3 == 16)
                        v8 = v31;
                    v6 += sprintf(v6, "%s\\P%d\\", v7, v8);
                }
                v1 = v3;
                a2 = 0;
            }
            else
            {
                *v6++ = v5;
                v1 = v3;
                a2 = 0;
            }
        LABEL_38:
            v5 = *++v4;
        } while (v5);
        v2 = pcINO_ControlsReadFormatBuffer;
    }
    *v6 = 0;
    if (!v30)
        return v2;
    INO_SetControlMode_inner_sub_48ACB0(v32);
    INO_SetControlSubmode_inner_sub_48A650(a1);
    return pcINO_ControlsReadFormatBuffer;
}

int STR_l_ReadFormat_Int(char* a1)
{
    char* v1;
    int v2;
    char v3;
    int v4;
    int i;

    v1 = a1;
    v2 = 0;
    STRDataPtrStart = 0;
    v3 = *a1;
    v4 = 0;
    for (i = 0; v3 != '\\'; ++v4)
    {
        if (!v3)
            break;
        if (!i)
        {
            if (isdigit(v3))
            {
                v2 = *v1 + 10 * v2 - '0';
                STRDataPtrStart = v2;
            }
            else
            {
                i = 1;
            }
        }
        v3 = *++v1;
    }
    return v4;
}

int STR_l_ReadFormat_Hexa(char* a1)
{
    char* v1;
    int v2;
    char v3; 
    int v4;
    int i;
    char v7;

    v1 = a1;
    v2 = 0;
    STRDataPtrStart = 0;
    v3 = *a1;
    v4 = 0;
    for (i = 0; v3 != '\\'; ++v4)
    {
        if (!v3)
            break;
        if (!i)
        {            
            if (isdigit(v3))
            {
                v2 = *v1 + 16 * (v2 + 0xFFFFFFD);
                STRDataPtrStart = v2;
            }
            else
            {
                v7 = *v1;
                if (*v1 < 'a' || v7 > 'f')
                {
                    if (v7 < 'A' || v7 > 'F')
                    {
                        i = 1;
                    }
                    else
                    {
                        v2 = 16 * v2 + v7 - 55;
                        STRDataPtrStart = v2;
                    }
                }
                else
                {
                    v2 = 16 * v2 + v7 - 87;
                    STRDataPtrStart = v2;
                }
            }
        }
        v3 = *++v1;
    }
    return v4;
}

int STR_l_ReadFormat_Float(char* a1)
{
    int v1;
    char* v2;
    char v3;
    int v4;
    float i;

    v1 = 0;
    v2 = a1;
    STRDataPtrStart = 0;
    v3 = *a1;
    v4 = 0;
    for (i = 0.0; v3 != '\\'; ++v4)
    {
        if (!v3)
            break;
        if (!v1)
        {            
            if (isdigit(v3))
            {
                if (i == 0.0)
                {
                    *(float*)&STRDataPtrStart = (float)((double)((char)*v2 - 48) + *(float*)&STRDataPtrStart * 10.0);
                }
                else
                {
                    *(float*)&STRDataPtrStart = (float)((double)((char)*v2 - 48) * i + *(float*)&STRDataPtrStart);
                    i = i * 0.1f;
                }
            }
            else if (*v2 == '.')
            {
                i = 0.1f;
            }
            else
            {
                v1 = 1;
            }
        }
        v3 = *++v2;
    }
    return v4;
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

void STR_ParseString(struct sGEOSTRStruct* geostr, int idx, struct sGEOSTRLetter* dest, char* EndPtr, int len, float width, float height, int color, int a9)
{
    tGEOSTRLetter* v9;
    tGEOSTRLetter* v10;
    tStrGlobalStruct* v11;
    tGEOSTRStruct* v12;
    char v13;
    int v14;
    int v15;
    BOOL v16;
    unsigned int v17;
    unsigned __int8 v18;
    unsigned int v19;
    int v20;
    float x;
    float y;
    tGEOSTRLetter* v23;
    int v24;
    char desta; 
    char* currPtr;
    tGEOSTREffectStructType20 *es19;

    v9 = 0;
    v10 = dest;
    v18 = 0;
    v19 = 0;
    x = 0.0;
    y = 0.0;
    v20 = 0;
    v24 = 0;
    desta = 0;
    v11 = 0;
    v23 = 0;
    v12 = geostr;
    currPtr = STR_sz_ControlsReadFormat_pcINO(EndPtr);
    while (1)
    {
        v13 = *currPtr;
        if (!*currPtr)
            break;
        switch (v13)
        {
        case '\\':
            currPtr += STR_l_ReadFormat(currPtr);
            switch (STR_sgst_Format)
            {
            case 1:
                if (v9)
                    v9->flagsAndChar |= 0x20000000;
                break;
            case 2:
                v19 = (v19 & 0xFF0FFFFF) + (STRDataPtrStart << 20);
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
                v20 = STRDataPtrStart;
                break;
            case 9:
                v20 = STRDataPtrStart;
                v24 = STRDataPtrStart;
                break;
            case 10:
                v18 = STRDataPtrStart;
                desta = 1;
                goto LABEL_16;
            case 11:
                v19 |= 0x10000000u;
                break;
            case 12:
                v11 = STR_sz_GlobalGet(STRDataPtrStart);
                break;
            case 13:
                v14 = STR_i_EffectAdd(v12, 0x80000013, idx, a9 + v10 - dest, 1); // 19-es effect (0x13)
                if (v14 != -1)
                {
                    STR_SetCurEffect(v12, v14);
                    if (STR_spst_CurEffect)
                    {
                        es19 = (tGEOSTREffectStructType20*)STR_spst_CurEffect;
                        es19->flagsAndChar = 0;
                        es19->unkb1 = 0;
                        v15 = STRDataPtrStart;
                        v16 = STRDataPtrStart == 0;
                        es19->unki1 = 0;
                        if (!v16)
                        {
                            do
                            {
                                es19->unki1 = (v15 % 10) | (16 * es19->unki1);
                                v17 = (v15 - v15 % 10) % 10;
                                v15 = (v15 - v15 % 10) / 10;
                            } while ((v17 >> 31) + v17);
                            v9 = v23;
                            v12 = geostr;
                        }
                        es19->color2 = 0.0;
                        es19->color1 = 0.40000001f;
                    }                   
                }
                break;
            default:
                break;
            }
            if (v11)
            {
            LABEL_16:
                if (!len)
                    return;
                if (desta)
                {
                    v10->flagsAndChar = v19 | v18;
                    goto LABEL_37;
                }
                if (!v11)
                {
                    v10->flagsAndChar = v19 | STR_sgac_CharTable[(uint8_t)*currPtr];
                    goto LABEL_37;
                }
                while (1)
                {
                    v10->flagsAndChar = v19 | STR_sgac_CharTable[v11->str[0]];
                    v11 = (tStrGlobalStruct*)((char*)v11 + 1);
                LABEL_37:
                    v10->flagsAndChar |= v20 << 12;
                    v10->x = (int)((double)(uint16_t)v12->width * x);
                    v10->y = (int)((double)(uint16_t)v12->height * y);
                    v10->width = (unsigned int)((double)(uint16_t)v12->width * width);
                    v10->height = (unsigned int)((double)(uint16_t)v12->height * height);
                    v10->textColor = color;
                    if (!v11 || !v11->str[0])
                        break;
                    ++v10;
                    --len;
                }
                if (!desta && !v11)
                    ++currPtr;
                v9 = v10++;
                v23 = v9;
                --len;
                v19 = 0;
                v20 = v24;
                desta = 0;
                v11 = 0;
            }
            break;
        case '\r':
            ++currPtr;
            break;
        case '\n':
            ++currPtr;
            if (v9)
                v9->flagsAndChar |= 0x20000000;
            break;
        default:
            goto LABEL_16;
        }
    }
}

void STR_ChangeStringLength(tGEOSTRStruct* a1, int idx, int lenToSet)
{
    tGEOSTRStringData* v3;
    int v4;
    int v5;
    int v6;
    int v7;
    tGEOSTRStringData* v8;
    tGEOSTRStringData* v9;
    tGEOSTRLetter* v10;
    tGEOSTRLetter* v11;
    tGEOSTRLetter* v12;
    int v13;
    tGEOSTRStringData* v14;
    unsigned __int16 v15;
    int v16;
    unsigned __int16 v17;
    tGEOSTRStringData* v18;
    tGEOSTRStringData* v19;
    int v20;
    int v21;

    v3 = STR_pst_GetString(a1, idx);
    v19 = v3;
    if (v3)
    {
        v4 = lenToSet;
        v5 = v3->number;
        if (lenToSet != v5)
        {
            v6 = v3->index;
            v7 = a1->unkNum;
            v20 = v6;
            v21 = v7;
            if (lenToSet + v6 > v7)
            {
                lenToSet = v7 - v6;
                v4 = v7 - v6;
            }
            if (v4 != v5)
            {
                v8 = v3 + 1;
                v9 = &a1->stringData[a1->stringNum];
                if (&v3[1] >= v9)
                {
                    v3->number = v4;
                    v3->number2 = v4;
                }
                else
                {
                    v10 = a1->letters;
                    v11 = &v10[v4 + v6];
                    v12 = &v10[v8->index];
                    v13 = v9[-1].number + v9[-1].index - v8->index;
                    v14 = v19;
                    if (v4 + v13 + v20 > v21)
                        v13 = v21 - v19->index - lenToSet;
                    if (v13 > 0)
                    {
                        memcpy(v11, v12, 16 * v13);
                        v14 = v19;
                    }
                    if (v11 > v12)
                        memset(v12, 0, (char*)v11 - (char*)v12);
                    v14->number = lenToSet;
                    v14->number2 = lenToSet;
                    while (1)
                    {
                        v15 = v8[-1].number + v8[-1].index;
                        v16 = v15 + v8->number;
                        v8->index = v15;
                        if (v16 > (a1->unkNum & 0xffff))
                            break;
                        if (++v8 >= v9)
                            return;
                    }
                    v17 = (a1->unkNum & 0xffff) - v15;
                    v8->number = v17;
                    v18 = v8 + 1;
                    for (v14->number2 = v17; v18 < v9; ++v18)
                    {
                        v18->index = a1->unkNum;
                        v18->number = 0;
                        v18->number2 = 0;
                    }
                }
            }
        }
    }
}

void STR_RecomputePos(tGEOSTRStruct* a1, tGEOSTRStringData* a2, int a3, int a4)
{
    tGEOSTRLetter* v4;
    int v5;
    tGEOSTRLetter* v6;
    tGEOSTRLetter* v7;
    tGEOSTRLetter* v8;
    int v9;
    __int16 v10;
    int v11;
    unsigned int v12;
    int v13;
    __int16 v14;
    int v15;
    tGEOSTRLetter* v16;
    int v17;
    __int16 v18;
    int v19;
    tGEOSTRLetter* v20;
    tGEOSTRLetter* v21;
    int v22;
    __int16 v23;
    __int16 a4a;

    v4 = a1->letters;
    v5 = a2->index;
    v6 = &v4[v5 + a3];
    if (a4 > 0)
        v7 = &v6[a4];
    else
        v7 = &v4[v5 + a2->number];
    v21 = v7;
    if (a3 || (v6->flagsAndChar & 0x10000000) == 0)
    {
        v8 = v7;
    }
    else
    {
        v8 = v7;
        if (&v6[1] < v7)
        {
            v22 = 1;
            ++v6;
            goto LABEL_10;
        }
    }
    v22 = 0;
LABEL_10:
    v9 = (unsigned __int16)v6->y;
    v10 = v6->x;
    v23 = v9;
    v11 = (unsigned __int16)v9;
    a4a = v10;
    if ((a2->flags & 0x10) != 0)
    {
        if (v6 < v8)
        {
            do
            {
                v12 = (v6->flagsAndChar >> 12) & 0x3F;
                if ((int16_t)v12 != a1->fontMaterialID)
                {
                    a1->fontMaterialID = v12;
                    a1->fontDesc = STR_pst_GetFontDesc((tGEOMAT*)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), (__int16)v12);
                }
                if (a1->fontDesc)
                {
                    v6->x = a4a;
                    a4a += v6->width;
                    v13 = v6->height;
                    v6->y = v11;
                    if ((__int16)v11 + (__int16)v13 > (__int16)v9)
                        v9 = v13 + v11;
                    if ((v6->flagsAndChar & 0x20000000) != 0)
                    {
                        v14 = a1->fontMaterialID;
                        a4a = v10;
                        v11 = v9;
                        if (v14 >= 0 && v14 < 24)
                            v11 = v9 + a1->shorts[v14];
                    }
                }
                ++v6;
            } while (v6 < v21);
        }
    }
    else if (v6 < v8)
    {
        do
        {
            v6->x = a4a;
            a4a += v6->width;
            v15 = v6->height;
            v6->y = v11;
            if ((__int16)v11 + (__int16)v15 > (__int16)v9)
                v9 = v15 + v11;
            if ((v6->flagsAndChar & 0x20000000) != 0)
            {
                a4a = v10;
                v11 = v9;
            }
            ++v6;
        } while (v6 < v7);
    }
    if (v22)
    {
        v16 = a1->letters;
        v17 = a3 + a2->index;
        v18 = v10 - v16[v17].width;
        v19 = (__int16)v16[v17].height;
        v20 = &v16[v17];
        v20->y = v23;
        v20->x = v18 - 4;
        if (v23 + v19 > (__int16)v9)
            v20->y = v23 - ((v19 + v23 - (__int16)v9) >> 1);
    }
}

unsigned int STR_i_GlobalLength(int a1)
{
    unsigned int result;

    if ((uint8_t)a1 < 24u)
        result = strlen(STR_sgaac_Global[a1].str);
    else
        result = 0;
    return result;
}

int STR_l_GetStringLength(char* string)
{
    int v1;
    char* v2;
    char i;

    v1 = 0;
    v2 = STR_sz_ControlsReadFormat_pcINO(string);
    for (i = *v2; *v2; i = *v2)
    {
        if (i == '\\')
        {
            v2 += STR_l_ReadFormat(v2);
            if (STR_sgst_Format == 10)
            {
                ++v1;
            }
            else if (STR_sgst_Format == 12)
            {
                v1 += STR_i_GlobalLength(STRDataPtrStart);
            }
        }
        else if (i == '\n' || i == '\r')
        {
            ++v2;
        }
        else
        {
            ++v2;
            ++v1;
        }
    }
    return v1;
}

void STR_EffectDelAllForAString(tGEOSTRStruct* a1, int a2)
{
    int v2;
    tGEOSTREffectStruct* v3;
    byte v4;

    v2 = 0;
    if (a1->effectsNum)
    {
        v3 = a1->effect;
        do
        {
            if (v3->stringid == a2)
            {
                v4 = a1->effectsNum - 1;
                a1->effectsNum = v4;
                if (v2 < v4)
                    memcpy(v3, &a1->effect[v4], sizeof(tGEOSTREffectStruct));
            }
            else
            {
                ++v2;
                ++v3;
            }
        } while (v2 < a1->effectsNum);
        STR_spst_CurEffect = 0;
    }
    else
    {
        STR_spst_CurEffect = 0;
    }
}

void STR_SetString(struct sGameObjectMain* gao, int idx, char* stringToSet)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;

    if (!stringToSet)
        return;

    str = STR_pst_Get(gao);

    if (!str)
        return;

    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
    str->fontMaterialID = -1;
    str->fontDesc = NULL;    
    sd = STR_pst_GetString(str, idx);

    if (!sd)
        return;

    STR_EffectDelAllForAString(str, idx);
    STR_ChangeStringLength(str, idx, STR_l_GetStringLength(stringToSet));
    STR_ParseString(str, idx, &str->letters[sd->index], stringToSet, sd->number, 0.050000001f, 0.050000001f, -1, 0);
    
    if ((sd->flags & 0x200) == 0)
        STR_RecomputePos(str, sd, 0, -1);
    
    sd->index2 = 0;
    sd->number2 = sd->number;
}

void STR_SetSize(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5)
{
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    tGEOSTRLetter* v7;
    tGEOSTRLetter* v8;
    unsigned int v9;
    tFontData* v10;
    unsigned int v11;
    unsigned int v12;
    int v13;
    unsigned int v14;
    tFontData* v15;
    unsigned int v16;
    unsigned int v17;
    int v18;
    tGEOSTRLetter* v19;
    tGEOSTRStringData* a2; 
    int a3a;

    v5 = STR_pst_Get(a1);
    if (v5)
    {
        v6 = STR_pst_GetString(v5, idx);
        a2 = v6;
        if (v6)
        {
            ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            v5->fontMaterialID = -1;
            v5->fontDesc = 0;
            a3a = STR_l_GetValidIndex(v5, v6, a3);
            v7 = &v5->letters[a3a + v6->index];
            v8 = &v7[STR_l_GetValidNumber(v5, v6, a3a, a4)];
            v19 = v8;
            if (a5->x < 0.0 || a5->y <= 0.0)
            {
                for (; v7 < v8; ++v7)
                {
                    v9 = (v7->flagsAndChar >> 12) & 0x3F;
                    if ((int16_t)v9 != v5->fontMaterialID)
                    {
                        v5->fontMaterialID = v9;
                        v5->fontDesc = STR_pst_GetFontDesc((tGEOMAT*)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), (__int16)v9);
                    }
                    v10 = v5->fontDesc;
                    if (v10 && a5->x < 0.0)
                    {
                        v11 = (uint8_t)v7->flagsAndChar;
                        if (v11 < 32)
                            v12 = 0;
                        else
                            v12 = v11 - 32;
                        v13 = v12;
                        v7->width = (unsigned int)(fabs(v10->fge[v13].y - v10->fge[v13].x)
                            * (double)(uint16_t)v10->width
                            * gpst_DX_stDD->ct.cam.width
                            * 0.0015625);
                        v14 = (unsigned int)(fabs(v10->fge[v13].h - v10->fge[v13].w)
                            * (double)(uint16_t)v5->fontDesc->height
                            * gpst_DX_stDD->ct.cam.height
                            * 0.0020833334);
                        v8 = v19;
                    }
                    else
                    {
                        v14 = (unsigned int)((double)(uint16_t)v5->width * a5->x);
                        v7->width = v14;
                        v15 = v5->fontDesc;
                        if (v15)
                        {
                            v16 = (uint8_t)v7->flagsAndChar;
                            if (v16 < 0x20)
                                v17 = 0;
                            else
                                v17 = v16 - 32;
                            v18 = v17;
                            v14 = (unsigned int)(fabs(v15->fge[v18].h - v15->fge[v18].w)
                                / fabs(v15->fge[v18].y - v15->fge[v18].x)
                                * a5->x
                                * (double)(uint16_t)v5->height);
                        }
                    }
                    v7->height = v14;
                }
            }
            else
            {
                for (; v7 < v8; ++v7)
                {
                    v7->width = (unsigned int)((double)(uint16_t)v5->width * a5->x);
                    v7->height = (unsigned int)((double)(uint16_t)v5->height * a5->y);
                }
            }
            if ((a2->flags & 0x200) == 0)
                STR_RecomputePos(v5, a2, a3a, -1);
        }
    }
}

Vec3* STR_pst_GetSize(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5)
{
    tGEOSTRStruct* v6;
    double v7;
    Vec3 max;
    Vec3 min;
    float a5a;

    if (a5)
    {
        v6 = STR_pst_Get(a1);
        if (v6)
        {
            ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            v6->fontMaterialID = -1;
            v6->fontDesc = 0;
            STR_GetRectShort(v6, idx, a3, a4, &min, &max);
            v7 = max.x - min.x;
            a5->x = (float)v7;
            a5a = max.y - min.y;
            a5->y = a5a;
            a5->z = max.z - min.z;
            if (v7 < 0.0)
                a5->x = (float)(v7 * -1.0);
            if (a5a < 0.0)
                a5->y = (float)(a5a * -1.0);
            a5->x = (float)(a5->x / (double)(uint16_t)v6->width);
            a5->y = (float)(a5->y / (double)(uint16_t)v6->height);
        }
    }
    return a5;
}

void STR_Align(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5)
{
    tGEOSTRStruct* v5;
    tGEOSTRStruct* v6;
    tGEOSTRStringData* v7;
    tGEOSTRStringData* v8;
    int v9;
    int v10;
    int v11;
    tGEOSTRLetter* v12;
    int v13;
    tGEOSTRLetter* v14;
    tGEOSTRLetter* v15;
    tGEOSTRLetter* v16;
    __int16 v17;
    __int16 v18;
    __int16 v19;
    __int16 v20;
    __int16 v21;
    __int16 i;
    int j;
    __int16 v24;
    __int16 v25;
    tGEOSTRLetter* v26;
    Vec3 v27;

    v5 = STR_pst_Get(a1);
    v6 = v5;
    if (v5)
    {
        v7 = STR_pst_GetString(v5, a2);
        v8 = v7;
        if (v7)
        {
            v9 = STR_l_GetValidIndex(v6, v7, a3);
            v10 = STR_l_GetValidNumber(v6, v8, v9, a4);
            if (!a5)
            {
                STR_pst_GetSize(a1, a2, v9, v10, &v27);
                v27.x = (float)((double)(uint16_t)v6->width * v27.x);
            }
            v11 = v9 + v8->index;
            v12 = v6->letters;
            v13 = v12[v11].flagsAndChar;
            v14 = &v12[v11];
            v15 = &v14[v10];
            v26 = v15;
            if ((v13 & 0x10000000) != 0)
                ++v14;
            v16 = v14;
            if (v14 < v15)
            {
                do
                {
                    v17 = -10000;
                    v24 = 10000;
                    v25 = 10000;
                    v18 = -10000;
                    if (v14 < v15)
                    {
                        while (1)
                        {
                            v19 = v14->x;
                            if (v19 < v25)
                                v25 = v14->x;
                            v20 = v14->y;
                            if (v20 < v24)
                                v24 = v14->y;
                            if ((__int16)(v19 + v14->width) > v18)
                                v18 = v19 + v14->width;
                            if ((__int16)(v20 + v14->height) > v17)
                                v17 = v20 + v14->height;
                            if ((v14->flagsAndChar & 0x20000000) != 0)
                                break;
                            if (++v14 >= v26)
                                goto LABEL_22;
                        }
                        if (v14 < v26)
                            ++v14;
                    }
                LABEL_22:
                    if (a5)
                    {
                        if (a5 == 1)
                        {
                            for (i = v17 - v24; v16 < v14; ++v16)
                                v16->y += (i - (__int16)v16->height) / 2;
                        }
                        else if (a5 == 2 && v16 < v14)
                        {
                            do
                            {
                                v21 = v17 - v16->height;
                                ++v16;
                                v16[-1].y = v21;
                            } while (v16 < v14);
                        }
                    }
                    else
                    {
                        for (j = (v25 + (__int16)(unsigned int)v27.x - v18) / 2; v16 < v14; ++v16)
                            v16->x += j;
                    }
                    v15 = v26;
                } while (v14 < v26);
            }
        }
    }
}

void STR_SetStringExt(struct sGameObjectMain* a1, int a2, int a3, char* EndPtr, Vec3* a5)
{
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    int v7;
    unsigned __int16 v8;
    Vec3 a5a;

    if (EndPtr)
    {
        v5 = STR_pst_Get(a1);
        if (v5)
        {
            ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            v5->fontMaterialID = -1;
            v5->fontDesc = 0;
            v6 = STR_pst_GetString(v5, a2);
            if (v6)
            {
                if ((a3 & 0x10000) != 0)
                {
                    v7 = STR_l_GetStringLength(EndPtr);
                    STR_ChangeStringLength(v5, a2, v7);
                    STR_ParseString(v5, a2, &v5->letters[v6->index], EndPtr, v6->number, 0.050000001f, 0.050000001f, -1, 0);
                    v8 = v6->number;
                    v6->index2 = 0;
                    v6->number2 = v8;
                }
                if ((a3 & 0x100000) != 0)
                    v6->flags = a3;
                if ((a3 & 0x40000) != 0)
                    v6->pivotPoint = *a5;
                if ((a3 & 0x20000) != 0)
                {
                    a5a.x = -1.0;
                    a5a.y = 0.0;
                    a5a.z = 0.0;
                    STR_SetSize(a1, a2, 0, -1, &a5a);
                }
                if ((a3 & 0x80000) != 0)
                    STR_Align(a1, a2, 0, -1, ((int)(a3 >> 16) >> 8) & 0xF);
            }
        }
    }
}

void STR_InsertString(struct sGameObjectMain* a1, int idx, int a3, char* EndPtr)
{
    tGEOSTRStruct* v4;
    tGEOSTRStringData* v5;
    int v6;
    int v7;
    tGEOSTRLetter* v8;
    int v9;
    int a5;
    int a8;
    int a8a;

    if (EndPtr)
    {
        v4 = STR_pst_Get(a1);
        if (v4)
        {
            ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            v4->fontMaterialID = -1;
            v4->fontDesc = 0;
            v5 = STR_pst_GetString(v4, idx);
            if (v5)
            {
                v6 = STR_l_GetStringLength(EndPtr);
                a5 = v6;
                if (v6)
                {
                    v7 = STR_l_GetValidIndex(v4, v5, a3);
                    a8 = v5->number;
                    STR_ChangeStringLength(v4, idx, v6 + a8);
                    v8 = &v4->letters[v7 + v5->index];
                    if (v7 == a8 || (v9 = v5->number, v9 <= a5))
                    {
                        if (v7)
                            STR_ParseString(v4, idx, v8, EndPtr, a5, 0.0, 0.0, v8[-1].textColor, v7);
                        else
                            STR_ParseString(v4, idx, v8, EndPtr, a5, 0.050000001f, 0.050000001f, -1, 0);
                    }
                    else
                    {
                        a8a = v8->textColor;
                        memcpy(&v8[a5], v8, 16 * (v9 - a5 - v7));
                        STR_ParseString(v4, idx, v8, EndPtr, a5, 0.0, 0.0, a8a, v7);
                    }
                    if ((v5->flags & 0x200) == 0)
                        STR_RecomputePos(v4, v5, 0, -1);
                }
            }
        }
    }
}

void STR_SetFloat(struct sGameObjectMain* a1, int idx, float a3, char a4)
{
    char Format[8];
    char Buffer[32];

    strcpy(Format, "%.0f");
    Format[2] = a4 + '0';
    sprintf(Buffer, Format, a3);
    STR_SetString(a1, idx, Buffer);
}

void STR_InsertFloat(tGameObjectMain* a1, int idx, int a3, float a4, char precision)
{
    char Format[8];
    char Buffer[32];

    strcpy(Format, "%.0f");
    Format[2] = precision + '0';
    sprintf(Buffer, Format, a4);
    STR_InsertString(a1, idx, a3, Buffer);
}

void STR_SetPivot(struct sGEOSTRStruct* a1, int idx, Vec3* a3)
{
    tGEOSTRStringData* v3;

    v3 = STR_pst_GetString(a1, idx);
    if (v3)
        v3->pivotPoint = *a3;
}

Vec3* STR_pst_GetPivot(struct sGEOSTRStruct* a1, int idx)
{
    Vec3* result = NULL;
    tGEOSTRStringData* i2;

    i2 = STR_pst_GetString(a1, idx);
    if (i2)
        result = &(i2->pivotPoint);
    return result;
}

void STR_SetFlag(struct sGEOSTRStruct* a1, int idx, __int16 a3)
{
    tGEOSTRStringData* v3;

    v3 = STR_pst_GetString(a1, idx);
    if (v3)
        v3->flags = a3;
}

int STR_ul_GetFlag(struct sGEOSTRStruct* a1, int idx)
{
    tGEOSTRStringData* it;
    int16_t ret = 0;

    it = STR_pst_GetString(a1, idx);
    if (it)
        ret = it->flags;
    return ret;
}

void STR_SetColor(struct sGEOSTRStruct* a1, int idx, int a3, int a4, int a5)
{
    tGEOSTRStringData* v5;
    tGEOSTRStringData* v6;
    int v7;
    tGEOSTRLetter* v8;
    tGEOSTRLetter* i;

    v5 = STR_pst_GetString(a1, idx);
    v6 = v5;
    if (v5)
    {
        v7 = STR_l_GetValidIndex(a1, v5, a3);
        v8 = &a1->letters[v7 + v6->index];
        for (i = &v8[STR_l_GetValidNumber(a1, v6, v7, a4)]; v8 < i; ++v8)
            v8->textColor = a5;
    }
}

void STR_SetAlpha(struct sGEOSTRStruct* a1, int idx, int a3, int a4, int a5)
{
    tGEOSTRStringData* v5;
    tGEOSTRStringData* v6;
    int v7;
    tGEOSTRLetter* v8;
    tGEOSTRLetter* i;

    v5 = STR_pst_GetString(a1, idx);
    v6 = v5;
    if (v5)
    {
        v7 = STR_l_GetValidIndex(a1, v5, a3);
        v8 = &a1->letters[v7 + v6->index];
        for (i = &v8[STR_l_GetValidNumber(a1, v6, v7, a4)]; v8 < i; ++v8)
        {
            v8->textColor &= ~0xff000000;
            v8->textColor |= a5 << 24;
        }
    }
}

void STR_SetColorExt(struct sGEOSTRStruct* a1, int idx, int a3, int a4, int a5, int a6)
{
    tGEOSTRStringData* v6;
    tGEOSTRStringData* v7;
    int v8;
    tGEOSTRLetter* v9;
    int v10;
    tGEOSTRLetter* i;
    int a6a;

    v6 = STR_pst_GetString(a1, idx);
    v7 = v6;
    if (v6)
    {
        v8 = STR_l_GetValidIndex(a1, v6, a3);
        v9 = &a1->letters[v8 + v7->index];
        v10 = a6 & a5;
        a6a = ~a6;
        for (i = &v9[STR_l_GetValidNumber(a1, v7, v8, a4)]; v9 < i; ++v9)
            v9->textColor = v10 | v9->textColor & a6a;
    }
}

unsigned int STR_GetColor(struct sGEOSTRStruct* a1, int idx, int a3)
{
    tGEOSTRStringData* result;
    unsigned int ret = 0;

    result = STR_pst_GetString(a1, idx);
    if (result)
        ret = a1->letters[result->index + STR_l_GetValidIndex(a1, result, a3)].textColor;
    return ret;
}

void STR_ChangePage(struct sGEOSTRStruct* a1, int a2, int a3, int a4, int a5, int a6)
{
    tGEOSTRStringData* v6;
    tGEOSTRStringData* v7;
    int v8;
    tGEOSTRLetter* v9;
    tGEOSTRLetter* v10;
    int v11;

    v6 = STR_pst_GetString(a1, a2);
    v7 = v6;
    if (v6)
    {
        v8 = STR_l_GetValidIndex(a1, v6, a3);
        v9 = &a1->letters[v8 + v7->index];
        v10 = &v9[STR_l_GetValidNumber(a1, v7, v8, a4)];
        v11 = a5 << 12;
        if (a6 >= 0)
        {
            if (a6 >= 256)
            {
                if (a6 < 512 && v9 < v10)
                {
                    do
                    {
                        if ((v9->flagsAndChar & 0x3F000) != a6 - 256)
                            v9->flagsAndChar = v11 | v9->flagsAndChar & 0xFFFC0FFF;
                        ++v9;
                    } while (v9 < v10);
                }
            }
            else
            {
                for (; v9 < v10; ++v9)
                {
                    if ((v9->flagsAndChar & 0x3F000) == a6)
                        v9->flagsAndChar = v11 | v9->flagsAndChar & 0xFFFC0FFF;
                }
            }
        }
        else
        {
            for (; v9 < v10; ++v9)
                v9->flagsAndChar = v11 | v9->flagsAndChar & 0xFFFC0FFF;
        }
    }
}

void STR_SetPos(struct sGameObjectMain* a1, int a2, int a3, int a4, Vec3* a5)
{
    tGEOSTRStruct* v5;
    tGEOSTRStruct* v6;
    tGEOSTRStringData* v7;
    tGEOSTRStringData* v8;
    int v9;
    int v10;
    int v11;
    tGEOSTRLetter* v12;

    v5 = STR_pst_Get(a1);
    v6 = v5;
    if (v5)
    {
        v7 = STR_pst_GetString(v5, a2);
        v8 = v7;
        if (v7)
        {
            v9 = STR_l_GetValidIndex(v6, v7, a3);
            v10 = STR_l_GetValidNumber(v6, v8, v9, a4);
            if (v10)
            {
                if (a5->z == 0.0f)
                    a5->z = STR_f_DefaultLetterDepth;
                v11 = v9 + v8->index;
                if (v11 < v6->unkNum)
                {
                    v12 = &v6->letters[v11];
                    v12->x = (int)((double)(int16_t)v6->width * a5->x);
                    v12->y = (int)((double)(int16_t)v6->height * a5->y);
                }
                if ((v8->flags & 0x200) == 0)
                {
                    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
                    v6->fontMaterialID = -1;
                    v6->fontDesc = 0;
                    STR_RecomputePos(v6, v8, v9, v10);
                }
            }
        }
    }
}

void STR_EffectCheck(tGEOSTRStruct* a1, int a2, int a3, int a4)
{
    int v4;
    tGEOSTREffectStruct* i;
    unsigned int v6;
    int v7;
    tGEOSTREffectStruct* v8;
    int v9;
    int v10;

    v4 = 0;
    if (a1->effectsNum)
    {
        for (i = a1->effect; i->stringid != a2; i++)
        {
            if (++v4 >= a1->effectsNum)
                return;
        }
        v6 = v4;
        v7 = a1->effect[v6].index;
        v8 = &(a1->effect[v4]);
        if (a3 < v7)
        {
            if (a3 + a4 >= v7)
            {
                v9 = v8->number;
                v8->index = a3;
                if (v9 > 0)
                {
                    v10 = v9 - a4;
                    v8->number = v10;
                    if (v10 < 0)
                        v8->flags |= 1u;
                }
            }
            else
            {
                v8->index = v7 - a4;
            }
        }
    }
}

void STR_DelChar(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    int v7;
    int v8;
    int v9;
    tGEOSTRLetter* v10;
    int v11;
    int a3a;

    v4 = STR_pst_Get(a1);
    v5 = v4;
    if (v4)
    {
        v6 = STR_pst_GetString(v4, a2);
        if (v6)
        {
            v11 = a4 > -10 ? -1 : -10 - a4;
            v7 = STR_l_GetValidIndex(v5, v6, a3);
            a3a = v7;
            if (v7 < v6->number)
            {
                v8 = STR_l_GetValidNumber(v5, v6, v7, a4);
                STR_EffectCheck(v5, a2, v7, v8);
                v9 = v6->number;
                if (v8 + v7 != v9)
                {
                    v10 = &v5->letters[v7 + v6->index];
                    memcpy(&v5->letters[a3a + v6->index], &v10[v8], 16 * (v9 - a3a - v8));
                    if (v11 != -1)
                        v10->flagsAndChar &= ~((v11 & 0xF) << 20);
                }
                STR_ChangeStringLength(v5, a2, v6->number - v8);
                ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
                v5->fontMaterialID = -1;
                v5->fontDesc = 0;
                if ((v6->flags & 0x200) == 0)
                    STR_RecomputePos(v5, v6, 0, -1);
            }
        }
    }
}

void STR_SetChar(struct sGEOSTRStruct* a1, int a2, int a3, int a4)
{
    tGEOSTRStringData* v4;
    tGEOSTRStringData* v5;
    int v6;

    v4 = STR_pst_GetString(a1, a2);
    v5 = v4;
    if (v4)
    {
        v6 = STR_l_GetValidIndex(a1, v4, a3);
        if (v6 < v5->number)
            a1->letters[v6 + v5->index].flagsAndChar ^= (a4 ^ a1->letters[v6 + v5->index].flagsAndChar) & 0x3FFFF;
    }
}

unsigned int STR_i_GetChar(tGEOSTRStruct* a1, int idx, int a3)
{
    tGEOSTRStringData* v3;
    tGEOSTRStringData* v4;
    int v5;
    unsigned int result;

    v3 = STR_pst_GetString(a1, idx);
    v4 = v3;
    if (v3 && (v5 = STR_l_GetValidIndex(a1, v3, a3), v5 < v4->number))
        result = a1->letters[v5 + v4->index].flagsAndChar & 0x3FFFF;
    else
        result = 0;
    return result;
}

int STR_l_GetSubLength(struct sGEOSTRStruct* a1, int idx, int a3)
{
    tGEOSTRStringData* v3;
    tGEOSTRLetter* v4;
    int v5;
    int v6;
    tGEOSTRLetter* v7;
    int v8;
    int result;

    v3 = STR_pst_GetString(a1, idx);
    if (!v3)
        return 0;
    v4 = &a1->letters[v3->index];
    v5 = v3->number;
    v6 = a3;
    v7 = &v4[v5];
    if (a3 < 0)
    {
        v8 = 0;
        result = 0;
        if (v4 < v7)
        {
            do
            {
                ++v8;
                if ((v4->flagsAndChar & 0x20000000) != 0)
                {
                    if (v8 > result)
                        result = v8;
                    v8 = 0;
                }
                ++v4;
            } while (v4 < v7);
            if (v8 > result)
                result = v8;
        }
        return result;
    }
    if (a3)
    {
        do
        {
            if ((v4->flagsAndChar & 0x20000000) != 0)
                --v6;
            if (++v4 >= v7)
                return 0;
        } while (v6);
    }
    for (result = 0; v4 < v7; ++v4)
    {
        ++result;
        if ((v4->flagsAndChar & 0x20000000) != 0)
            break;
    }
    return result;
}

Vec3* STR_pst_GetMin(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5)
{
    tGEOSTRStruct* v6;

    if (!a5)
        return 0;
    v6 = STR_pst_Get(a1);
    if (v6)
    {
        ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
        v6->fontMaterialID = -1;
        v6->fontDesc = 0;
        STR_GetRectShort(v6, idx, a3, a4, a5, 0);
        a5->x = (float)(a5->x / (double)(uint16_t)v6->width);
        a5->y = (float)(a5->y / (double)(uint16_t)v6->height);
    }
    return a5;
}

Vec3* STR_pst_GetMax(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5)
{
    tGEOSTRStruct* v6;

    if (!a5)
        return 0;
    v6 = STR_pst_Get(a1);
    if (v6)
    {
        ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
        v6->fontMaterialID = -1;
        v6->fontDesc = 0;
        STR_GetRectShort(v6, idx, a3, a4, 0, a5);
        a5->x = (float)(a5->x / (double)(uint16_t)v6->width);
        a5->y = (float)(a5->y / (double)(uint16_t)v6->height);
    }
    return a5;
}

void STR_SetFrame(struct sGameObjectMain* a1, int a2, char a3, __int16 a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStringData* v5;

    v4 = STR_pst_Get(a1);
    if (v4)
    {
        v5 = STR_pst_GetString(v4, a2);
        if (v5)
            v5->frame = a4 & 0xFFF | ((a3 & 0x3F) << 12);
    }
}

void STR_EffectDelAll(tGEOSTRStruct* a1)
{
    a1->effectsNum = 0;
    STR_spst_CurEffect = NULL;
}

void STR_EffectDel(struct sGameObjectMain* a1, int a2)
{
    tGEOSTRStruct* v2;
    int v3;
    byte v4;

    v2 = STR_pst_Get(a1);
    if (v2)
    {
        if (a2 >= 0)
        {
            STR_SetCurEffect(v2, a2);
            if (STR_spst_CurEffect)
            {
                v3 = ((char*)STR_spst_CurEffect - (char*)&v2->effect[0]) / sizeof(tGEOSTREffectStruct);
                v4 = v2->effectsNum - 1;
                v2->effectsNum = v4;
                if (v3 < v4)
                    memcpy(STR_spst_CurEffect, &v2->effect[v4], sizeof(tGEOSTREffectStruct));
                STR_spst_CurEffect = NULL;
            }
        }
        else
        {
            STR_EffectDelAll(v2);
        }
    }
}

void STR_SetFramePos(struct sGameObjectMain* a1, int idx, Vec3* a3, Vec3* a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;

    v4 = STR_pst_Get(a1);
    v5 = v4;
    if (v4)
    {
        v6 = STR_pst_GetString(v4, idx);
        if (v6)
        {
            v6->framePosX1 = (int)((double)(uint16_t)v5->width * a3->x);
            v6->framePosY1 = (int)((double)(uint16_t)v5->height * a3->y);
            v6->framePosX2 = (int)((double)(uint16_t)v5->width * a4->x);
            v6->framePosY2 = (int)((double)(uint16_t)v5->height * a4->y);
        }
    }
}

void STR_SetFrameBorderSize(struct sGameObjectMain* a1, int a2, float a3)
{
    tGEOSTRStruct* v3;
    tGEOSTRStruct* v4;
    tGEOSTRStringData* v5;

    v3 = STR_pst_Get(a1);
    v4 = v3;
    if (v3)
    {
        v5 = STR_pst_GetString(v3, a2);
        if (v5)
        {
            v5->frameBorderSize1 = (int)((double)(uint16_t)v4->width * a3);
            v5->frameBorderSize2 = (int)((double)(uint16_t)v4->height * a3);
        }
    }
}

void STR_SetFrameColor(struct sGameObjectMain* a1, int a2, int a3)
{
    tGEOSTRStruct* v3;
    tGEOSTRStringData* v4;

    v3 = STR_pst_Get(a1);
    if (v3)
    {
        v4 = STR_pst_GetString(v3, a2);
        if (v4)
            v4->frameColor = a3;
    }
}

void STR_SetSubStringWithLine(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    tGEOSTRLetter* v7;
    int v8;
    tGEOSTRLetter* v9;
    int v10;
    int v11;

    v4 = STR_pst_Get(a1);
    v5 = v4;
    if (v4)
    {
        v6 = STR_pst_GetString(v4, a2);
        if (v6)
        {
            v7 = v5->letters;
            v8 = a3;
            v9 = &v7[v6->index];
            v10 = 0;
            if (a3 <= 0)
            {
            LABEL_8:
                v11 = a4;
                v6->index2 = v10;
                if (a4 > 0)
                {
                    do
                    {
                        if ((v9->flagsAndChar & 0x20000000) != 0)
                            --v11;
                        ++v9;
                        ++v10;
                    } while (v10 < v6->number && v11 > 0);
                }
                v6->number2 = v10 - v6->index2;
            }
            else
            {
                while (1)
                {
                    if ((v9->flagsAndChar & 0x20000000) != 0)
                        --v8;
                    ++v9;
                    if (++v10 >= v6->number)
                        break;
                    if (v8 <= 0)
                        goto LABEL_8;
                }
            }
        }
    }
}

void STR_SetSubStringWithMark(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    tGEOSTRLetter* v7;
    int v8;
    tGEOSTRLetter* v9;
    int v10;
    int v11;
    int v12;
    int v13;
    unsigned __int16 v14;

    v4 = STR_pst_Get(a1);
    v5 = v4;
    if (v4)
    {
        v6 = STR_pst_GetString(v4, a2);
        if (v6)
        {
            v7 = v5->letters;
            v8 = a3;
            v9 = &v7[v6->index];
            v10 = 0;
            if (a3 <= 0)
            {
            LABEL_11:
                v12 = a4;
                if (a4 > 0)
                {
                    do
                    {
                        if ((v9->flagsAndChar & 0xF00000) == 0xF00000)
                            --v12;
                        ++v9;
                        ++v10;
                    } while (v10 < v6->number && v12 > 0);
                }
                v13 = v6->number;
                v14 = v10 - v6->index2;
                v6->number2 = v14;
                if (v10 < v13)
                {
                    if (v14)
                        v6->number2 = v14 - 1;
                }
            }
            else
            {
                v11 = v6->number;
                while (1)
                {
                    if ((v9->flagsAndChar & 0xF00000) == 0xF00000)
                        --v8;
                    if (!v8)
                        v6->index2 = v10;
                    ++v9;
                    if (++v10 >= v11)
                        break;
                    if (v8 <= 0)
                        goto LABEL_11;
                }
            }
        }
    }
}

void STR_SetSubString(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    tGEOSTRStringData* v7;
    int v8;
    unsigned __int16 v9;

    v4 = STR_pst_Get(a1);
    v5 = v4;
    if (v4)
    {
        v6 = STR_pst_GetString(v4, a2);
        v7 = v6;
        if (v6)
        {
            v8 = STR_l_GetValidIndex(v5, v6, a3);
            if (v8 < v7->number)
            {
                v9 = STR_l_GetValidNumber(v5, v7, v8, a4);
                v7->index2 = v8;
                v7->number2 = v9;
            }
        }
    }
}

int STR_i_CountLine(struct sGameObjectMain* a1, int idx, int a3, int a4)
{
    tGEOSTRStruct* ret;
    tGEOSTRStringData* ret2;
    int result = 0;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    int v7;
    int v8;
    tGEOSTRLetter* v9;
    tGEOSTRLetter* v10;
    int i;

    ret = STR_pst_Get(a1);
    v5 = ret;
    if (ret)
    {
        ret2 = STR_pst_GetString(ret, idx);
        v6 = ret2;
        if (ret2)
        {
            v7 = STR_l_GetValidIndex(v5, ret2, a3);
            v8 = STR_l_GetValidNumber(v5, v6, v7, a4);
            v9 = &v5->letters[v7 + v6->index];
            v10 = &v9[v8];
            for (i = 0; v9 < v10; ++v9)
            {
                if ((v9->flagsAndChar & 0x20000000) != 0)
                    ++i;
            }
            result = i + 1;
        }
    }
    return result;
}

int STR_i_CountMark(struct sGameObjectMain* a1, int idx, int a3, int a4, int a5)
{
    tGEOSTRStruct* ret;
    tGEOSTRStringData *ret2;
    int result = 0;
    tGEOSTRStruct* v6;
    tGEOSTRStringData* v7;
    int v8;
    int v9;
    tGEOSTRLetter* v10;
    tGEOSTRLetter* v11;
    int i;

    ret = STR_pst_Get(a1);
    v6 = ret;
    if (ret)
    {
        ret2 = STR_pst_GetString(ret, idx);
        v7 = ret2;
        if (ret2)
        {
            v8 = STR_l_GetValidIndex(v6, ret2, a3);
            v9 = STR_l_GetValidNumber(v6, v7, v8, a4);
            v10 = &v6->letters[v8 + v7->index];
            v11 = &v10[v9];
            for (i = 0; v10 < v11; ++v10)
            {
                if (((v10->flagsAndChar >> 20) & 0xF) == a5)
                    ++i;
            }
            result = i + 1;
        }
    }
    return result;
}

void STR_ToString(struct sGameObjectMain* a1, int idx, int a3, int a4, unsigned char* a5)
{
    tGEOSTRStruct* v5;
    tGEOSTRStruct* v6;
    tGEOSTRStringData* v7;
    tGEOSTRStringData* v8;
    int v9;
    int v10;
    tGEOSTRLetter* v12;
    tGEOSTRLetter* i;
    signed int v14;
    byte* v15;
    byte* v16;
    byte* v17;

    v5 = STR_pst_Get(a1);
    v6 = v5;
    if (v5)
    {
        v7 = STR_pst_GetString(v5, idx);
        v8 = v7;
        if (v7)
        {
            v9 = STR_l_GetValidIndex(v6, v7, a3);
            v10 = STR_l_GetValidNumber(v6, v8, v9, a4);
            v12 = &v6->letters[v9 + v8->index];
            for (i = &v12[v10]; v12 < i; ++a5)
            {
                if ((v12->flagsAndChar & 0x3F000) != 0)
                {
                    v14 = (v12->flagsAndChar >> 12) & 0x3F;
                    *a5 = '\\';
                    v15 = a5 + 1;
                    *v15 = 'p';
                    v16 = v15 + 1;
                    if (v14 >= 10)
                    {
                        *v16++ = v14 / 10 + '0';
                        v14 = v14 % 10;
                    }
                    *v16 = v14 + '0';
                    v17 = v16 + 1;
                    *v17 = '\\';
                    a5 = v17 + 1;
                }
                *a5 = v12->flagsAndChar;
                ++v12;
            }
            *a5 = 0;
        }
    }
}

void STR_SetLetterFlag(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5)
{
    tGEOSTRStruct* v5;
    tGEOSTRStruct* v6;
    tGEOSTRStringData* v7;
    tGEOSTRStringData* v8;
    int v9;
    tGEOSTRLetter* v10;
    tGEOSTRLetter* i;

    v5 = STR_pst_Get(a1);
    v6 = v5;
    if (v5)
    {
        v7 = STR_pst_GetString(v5, a2);
        v8 = v7;
        if (v7)
        {
            v9 = STR_l_GetValidIndex(v6, v7, a3);
            v10 = &v6->letters[v9 + v8->index];
            for (i = &v10[STR_l_GetValidNumber(v6, v8, v9, a4)]; v10 < i; ++v10)
            {
                v10->flagsAndChar &= ~0xff000000;
                v10->flagsAndChar |= a5 << 24;
            }
        }
    }
}

int STR_GetLetterFlag(struct sGameObjectMain* a1, int idx, int a3)
{
    tGEOSTRStruct* v3;
    tGEOSTRStringData* v4;
    tGEOSTRStringData* v5;
    int v6;
    int result;

    v3 = STR_pst_Get(a1);
    v4 = STR_pst_GetString(v3, idx);
    v5 = v4;
    if (v4 && (v6 = STR_l_GetValidIndex(v3, v4, a3), v6 < v5->number))
        result = v3->letters[v6 + v5->index].flagsAndChar >> 24;
    else
        result = 0;
    return result;
}

void STR_ToUpper(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStringData* v5;
    int v6;
    tGEOSTRLetter* v7;
    tGEOSTRLetter* i;
    unsigned int v9;

    v4 = STR_pst_Get(a1);
    if (v4)
    {
        v5 = STR_pst_GetString(v4, a2);
        if (v5)
        {
            ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            v4->fontMaterialID = -1;
            v4->fontDesc = 0;
            v6 = STR_l_GetValidIndex(v4, v5, a3);
            v7 = &v4->letters[v6 + v5->index];
            for (i = &v7[STR_l_GetValidNumber(v4, v5, v6, a4)]; v7 < i; ++v7)
            {
                v9 = (uint8_t)v7->flagsAndChar;
                if (v9 >= 'a' && (uint8_t)v7->flagsAndChar <= 'z')
                {
                    v7->flagsAndChar &= ~0xff;
                    v7->flagsAndChar |= v9 - ' ';
                }
            }
        }
    }
}

void STR_EffectSeti(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;
    int v5;
    int v6;
    int i;
    unsigned int* tmp;

    v4 = STR_pst_Get(a1);
    if (!v4)
        return;
    if (a3 == -1)
    {
        if (a2)
        {
            if (a2 == 1)
                STR_EffectDelAllForAString(v4, a4);
        }
        else
        {
            STR_EffectDelAll(v4);
        }
        return;
    }
    if (a3 == -2)
    {
        if (a2 >= 0)
        {
            if (a2 < 24)
                v4->shorts[a2] = a4;
        }
        else
        {
            v5 = a4;
            // TODO FIXME: ezt megcsinálni igényesre, mert ez elég gáz így shortból int és az bele a shortos mezõbe
            v6 = v5 << 16;
            v6 = a4;
            tmp = (unsigned int*)(v4->shorts);
            for (i = 0; i < 12; i++) {
                tmp[i] = v6;
            }
        }
        return;
    }
    STR_SetCurEffect(v4, a2);
    if (!STR_spst_CurEffect)
        return;
    if (a3 <= 301)
    {
        if (a3 == 301)
        {
            *(uint32_t *)&(STR_spst_CurEffect->timeElapsed) = a4;
            return;
        }
        if (a3 <= 200)
        {
            if (a3 != 200)
            {
                if (a3)
                {
                    if (a3 == 1)
                        STR_spst_CurEffect->flags ^= ((uint16_t)STR_spst_CurEffect->flags ^ (uint16_t)((int32_t)a4 << 8)) & 0xFF00;
                }
                else
                {
                    STR_spst_CurEffect->flags ^= (STR_spst_CurEffect->flags ^ (a4 << 16)) & 0xFF0000;
                }
                return;
            }
        LABEL_21:
            *(uint32_t*)&(STR_spst_CurEffect->color1) = a4;
            return;
        }
        if (a3 != 201)
        {
            if (a3 == 300)
                *(uint32_t*)&(STR_spst_CurEffect->f1) = a4;
            return;
        }
    LABEL_30:
        *(uint32_t*)&(STR_spst_CurEffect->color2) = a4;
        return;
    }
    if (a3 > 400)
    {
        if (a3 == 401)
            goto LABEL_30;
        if (a3 == 1600)
            *(uint32_t*)&(STR_spst_CurEffect->color2) = a4;
    }
    else
    {
        switch (a3)
        {
        case 400:
            goto LABEL_21;
        case 302:
            *(uint32_t*)&(STR_spst_CurEffect->color1) = a4;
            return;
        case 303:
            goto LABEL_30;
        }
    }
}

int STR_i_EffectGetId(struct sGEOSTRStruct* a1, int a2)
{
    int v2;
    tGEOSTREffectStruct* i;
    tGEOSTREffectStruct* v5;

    v2 = 0;
    if (!a1->effectsNum)
        return -1;
    for (i = a1->effect; i->stringid != a2; i++)
    {
        if (++v2 >= a1->effectsNum)
            return -1;
    }
    v5 = &a1->effect[v2];
    STR_spst_CurEffect = v5;
    return v5->effectID;
}

int STR_EffectGeti(struct sGameObjectMain* a1, int a2, int a3)
{
    tGEOSTRStruct* v3;

    v3 = STR_pst_Get(a1);
    if (!v3)
        return -1;
    if (a2 == -1)
        return STR_i_EffectGetId(v3, a3);
    STR_SetCurEffect(v3, a2);
    if (!STR_spst_CurEffect)
        return -1;
    if (!a3)
        return (STR_spst_CurEffect->flags >> 16) & 0xff;
    if (a3 == 1)
        return (STR_spst_CurEffect->flags >> 8) & 0xff;
    if (a3 != 2)
        return -1;
    return (STR_spst_CurEffect->flags >> 24) & 0xff;
}

void STR_EffectSetf(struct sGameObjectMain* a1, int a2, int a3, float a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    int v6;
    tGEOSTRStringData* v7;
    tGEOSTRStringData* v8;
    int v9;
    int v10;
    int v11;

    v4 = STR_pst_Get(a1);
    v5 = v4;
    if (!v4)
        return;
    STR_SetCurEffect(v4, a2);
    if (!STR_spst_CurEffect)
        return;
    if (a3 > 752)
    {
        if (a3 > 1650)
        {
            if (a3 > 1850)
            {
                v10 = a3 - 2050;
                if (a3 == 2050)
                    goto LABEL_44;
            LABEL_42:
                if (v10 != 1)
                    return;
                goto LABEL_43;
            }
            if (a3 == 1850)
                goto LABEL_44;
            v6 = a3 - 1651;
            if (a3 != 1651)
                goto LABEL_38;
        }
        else
        {
            if (a3 == 1650)
                goto LABEL_44;
            if (a3 <= 1350)
            {
                if (a3 == 1350)
                    goto LABEL_44;
                if (a3 == 1150)
                    goto LABEL_12;
                v10 = a3 - 1151;
                if (a3 == 1151)
                {
                LABEL_18:
                    STR_spst_CurEffect->color2 = a4;
                    return;
                }
                goto LABEL_42;
            }
            if (a3 != 1351)
            {
                if (a3 != 1550)
                {
                    if (a3 == 1551)
                        STR_spst_CurEffect->timeElapsed = a4;
                    return;
                }
            LABEL_44:
                STR_spst_CurEffect->f1 = a4;
                return;
            }
        }
    LABEL_43:
        STR_spst_CurEffect->color1 = a4;
        return;
    }
    if (a3 == 752)
    {
        STR_spst_CurEffect->color2 = a4;
        STR_spst_CurEffect->color1 = 0.0;
        return;
    }
    if (a3 > 552)
    {
        switch (a3)
        {
        case 553:
            goto LABEL_18;
        case 650:
            goto LABEL_12;
        case 651:
        case 751:
            STR_spst_CurEffect->timeElapsed = a4;
            break;
        case 652:
            goto LABEL_43;
        case 750:
            if (a4 >= 0.0)
                goto LABEL_12;
            v7 = STR_pst_GetString(v5, STR_spst_CurEffect->stringid);
            v8 = v7;
            if (v7)
            {
                v9 = STR_l_GetValidIndex(v5, v7, STR_spst_CurEffect->index);
                STR_spst_CurEffect->f1 = (float)(-a4 * (double)STR_l_GetValidNumber(v5, v8, v9, STR_spst_CurEffect->number));
            }
            break;
        default:
            return;
        }
        return;
    }
    if (a3 == 552)
    {
        STR_spst_CurEffect->color1 = a4;
        return;
    }
    if (a3 > 450)
    {
        v6 = a3 - 451;
        if (a3 == 451)
        {
            STR_spst_CurEffect->f3 = a4;
            return;
        }
    LABEL_38:
        v11 = v6 - 99;
        if (v11)
        {
            if (v11 == 1)
                STR_spst_CurEffect->timeElapsed = a4;
            return;
        }
        goto LABEL_44;
    }
    if (a3 == 450)
        goto LABEL_44;
    if (a3 == 150)
    {
    LABEL_12:
        STR_spst_CurEffect->f1 = a4;
        return;
    }
    if (a3 != 151)
    {
        if (a3 != 250)
            return;
        goto LABEL_12;
    }
    STR_spst_CurEffect->f3 = a4;
}

void STR_ReinitAll()
{
    STR_sgl_NumberOfFont = 0;
}

void STR_Init3DStringExtExt(struct sGameObjectMain* a1, char* str, int a3)
{
    tGEOGRO* v3;
    tGEOGRO* v4;
    int v5;
    int v6;
    int v7;
    int v8;
    tGEOGRO* v9;
    tGeoUVElem* v10;
    tGeoUVElem* v11;
    tGEOSTRLetter* v12;
    uint16_t v13;
    tFontData* v14;
    unsigned int v15;
    unsigned int v16;
    tFontGlypthElem* v17;
    tGEOGRO* v19;
    tGEOSTRStruct geostr;
    tGEOSTRLetter dest[256];

    v3 = (tGEOGRO*)OBJ_p_GetGro(a1);
    v4 = v3;
    v19 = v3;
    if (v3 && v3->intf->id == 1)
    {
        v5 = STR_l_GetStringLength(str);
        v6 = v4->uvNum >> 2;
        v7 = v5;
        if (v5 >= v6)
            v7 = v4->uvNum >> 2;
        if (a3 >= 0 && v7 > a3)
            v7 = a3;
        v8 = STR_l_GetStringLength(str);
        geostr.height = (unsigned int)gpst_DX_stDD->ct.cam.width;
        geostr.width = (unsigned int)gpst_DX_stDD->ct.cam.height;
        STR_ParseString(&geostr, 0, dest, str, v8, 0.050000001f, 0.050000001f, -1, 0);
        ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
        geostr.fontDesc = 0;
        geostr.fontMaterialID = 0;
        geostr.fontDesc = STR_pst_GetFontDesc((tGEOMAT*)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), 0);
        if (geostr.fontDesc)
        {
            v9 = v19;
            v10 = v19->UVs;
            v11 = &v10[4 * v6];
            v19->triangleChunks->materialNum = (dest[0].flagsAndChar >> 12) & 0x3F;
            v12 = dest;
            geostr.fontMaterialID = -1;
            if (v7 > 0)
            {
                do
                {
                    v13 = (v12->flagsAndChar >> 12) & 0x3F;
                    if (v13 != geostr.fontMaterialID)
                    {
                        geostr.fontMaterialID = (v12->flagsAndChar >> 12) & 0x3F;
                        v14 = STR_pst_GetFontDesc((tGEOMAT*)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), v13);
                        v9 = v19;
                        geostr.fontDesc = v14;
                    }
                    v15 = (uint8_t)v12->flagsAndChar;
                    if (v15 < 32)
                        v16 = 0;
                    else
                        v16 = v15 - 32;
                    v17 = &geostr.fontDesc->fge[v16];
                    v10[0].u = *(&v17->x + STRU1Offset);
                    v10[0].v = *(&v17->w + STRV1Offset);
                    v10[1].u = *(&v17->x + STRU2Offset);
                    v10[1].v = *(&v17->w + STRV2Offset);
                    v10[2].u = *(&v17->x + STRU3Offset);
                    v10[2].v = *(&v17->w + STRV3Offset);
                    v10[3].u = *(&v17->x + STRU4Offset);
                    v10[3].v = *(&v17->w + STRV4Offset);
                    v10 += 4;
                    ++v12;
                    --v7;
                } while (v7);
            }
            for (; v10 < v11; ++v10)
            {
                v10->u = 0.0;
                v10->v = 0.0;
            }
            if (v9->d3dIndexBuf)
                PC_FreeABackCache(v9);
        }
    }
}

void STR_Init3DString(struct sGameObjectMain* a1, char* str)
{
    STR_Init3DStringExtExt(a1, str, -1);
}

void STR_Init3DStringExt(struct sGameObjectMain* a1, char* EndPtr, int a3)
{
    STR_Init3DStringExtExt(a1, EndPtr, a3);
}

void STR_Init3DStringCenter(struct sGameObjectMain* a1, char* EndPtr, int a3)
{
    STR_Init3DStringExtExt(a1, EndPtr, a3);
}

void STR_Init3DStringFromSTR(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int idx, int a4)
{
    tGEOSTRStruct* v4;
    tGEOSTRStruct* v5;
    tGEOSTRStringData* v6;
    tGEOSTRStringData* v7;
    int v8;
    int v9;
    tGEOSTRLetter* v10;
    tGEOSTRLetter* v11;
    int i;
    char EndPtr[256];

    v4 = STR_pst_Get(a2);
    v5 = v4;
    if (v4)
    {
        v6 = STR_pst_GetString(v4, idx);
        v7 = v6;
        if (v6)
        {
            v8 = STR_l_GetValidIndex(v5, v6, 0);
            v9 = STR_l_GetValidNumber(v5, v7, v8, -1);
            v10 = &v5->letters[v8 + v7->index];
            v11 = &v10[v9];
            for (i = 0; v10 < v11; ++i)
            {
                EndPtr[i] = v10->flagsAndChar;
                ++v10;
            }
            EndPtr[i] = 0;
            STR_Init3DStringExtExt(a1, EndPtr, a4);
        }
    }
}