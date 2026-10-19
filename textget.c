#include "world.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "textures.h"
#include "strings.h"
#include "textget.h"

int txUnknownConst = 1;

void TEXT_Free(tTextGroupStruct* txg)
{
    if (txg == NULL) return;

    txg->usageCount--;
    if (txg->usageCount == 0)
    {
        if (txg->ids)
        {
            MEM_Free(txg->ids);
            LOA_DeleteAddress((int)txg->ids);
        }
        if (txg->texts)
        {
            MEM_Free(txg->texts);
            LOA_DeleteAddress((int)txg->texts);
        }
        MEM_Free(txg);
        LOA_DeleteAddress((int)txg);
    }
}

void TEXT_FreeAll()
{
    int i;

    if (TEXT_gst_Global.worldTexts2)
    {
        for (i = 0; i < TEXT_gst_Global.num2; i++)
        {
            if (TEXT_gst_Global.worldTexts2[i].ptr)
            {
                TEXT_Free(TEXT_gst_Global.worldTexts2[i].ptr);
            }
        }
        LOA_DeleteAddress((int)TEXT_gst_Global.worldTexts2);
        MEM_Free(TEXT_gst_Global.worldTexts2);
    }

    if (TEXT_gst_Global.worldTexts1)
    {
        for (i = 0; i < TEXT_gst_Global.num; i++)
        {
            if (TEXT_gst_Global.worldTexts1[i].ptr)
            {
                TEXT_Free(TEXT_gst_Global.worldTexts1[i].ptr);
            }
        }
        LOA_DeleteAddress((int)TEXT_gst_Global.worldTexts1);
        MEM_Free(TEXT_gst_Global.worldTexts1);
    }
    TEXT_gst_Global.num2 = 0;
    TEXT_gst_Global.num = 0;
    TEXT_gst_Global.worldTexts2 = NULL;
    TEXT_gst_Global.worldTexts1 = NULL;
}

int TEXT_i_GetLang()
{
    return TEXT_gst_Global.language;
}

int* TEXT_ul_CallbackLoadIds(uint32_t Offset)
{
    int32_t* currIDPtr;
    int32_t* ret;
    int size;
    int num;
    char* currBufferPtr = BIG_pc_ReadFileTmp(Offset, &size);

    if (!currBufferPtr)
        return NULL;

    currIDPtr = MEM_p_Alloc(size + 4);
    num = size / sizeof(uint32_t);
    
    ret = currIDPtr;
    
    if ((size & ~3) != 0)
    {
        while (num > 0)
        {
            *currIDPtr = LOA_ReadLong(&currBufferPtr, 0, 1);
            LOA_ReadString(&currBufferPtr, 0, 64, 2); // FIXME: this won't work in non binarized mode, it's the same in the original exe
            num--;
            ++currIDPtr;
        }
    }
    return ret;
}

char* TEXT_ul_CallbackLoadText(uint32_t Offset)
{
    char* ret;
    int size;
    char* currBufferPtr;

    currBufferPtr = BIG_pc_ReadFileTmp(Offset, &size);
    if (!currBufferPtr || size)
    {
        ret = (char*)MEM_p_Alloc(size);
        LOA_ReadString(&currBufferPtr, ret, size, 1);
    }
    else
    {
        ret = (char*)MEM_p_Alloc(1);
        ret[0] = '\0';
    }
    return ret;
}

tTextGroupStruct* TEXT_ul_CallbackLoadLang(uint32_t Offset)
{
    tTextGroupStruct* tgs;
    char* buffer;
    char* currBufferPtr;
    int idsKeyRead;
    int textsKeyRead;
    int idsKey;
    int textsKey;
    int pos;
    int idsKeyLang;
    int lngID;
    int idsKeyFirst;
    int textsKeyFirst;
    int textsKeyLang;
    int size;

    tgs = (tTextGroupStruct*)MEM_p_Alloc(sizeof(tTextGroupStruct));
    tgs->key = 0;
    tgs->num = 0;
    tgs->ids = NULL;
    tgs->texts = NULL;
    tgs->usageCount = 0;
    buffer = BIG_pc_ReadFileTmp(Offset, &size);
    
    textsKeyFirst = 0;
    idsKeyFirst = 0;
    textsKeyLang = 0;
    idsKeyLang = 0;
    lngID = 0;
    
    if (!size) {
        textsKey = 0;
        idsKeyLang = 0;
        idsKey = 0;
    }
    else 
    {
        currBufferPtr = buffer;
        while (size > 0)
        {
            idsKeyRead = LOA_ReadLong(&currBufferPtr, 0, 1);
            LOA_ReadLong(&currBufferPtr, 0, 1);
            textsKeyRead = LOA_ReadLong(&currBufferPtr, 0, 1);
            LOA_ReadLong(&currBufferPtr, 0, 1);
            size -= (4 * sizeof(int32_t));

            if (idsKeyRead && idsKeyRead != -1)
            {
                if (!idsKeyFirst)
                {
                    idsKeyFirst = idsKeyRead;
                    textsKeyFirst = textsKeyRead;
                }
                if (lngID == TEXT_gst_Global.language)
                {
                    idsKeyLang = idsKeyRead;
                    textsKeyLang = textsKeyRead;
                }
                ++lngID;
            }
        }

        idsKey = idsKeyLang;

        if (idsKeyLang)
        {
            textsKey = textsKeyLang;
        }
        else
        {        
            textsKey = textsKeyFirst;
            idsKeyLang = idsKeyFirst;
            idsKey = idsKeyFirst;
        }
    }
    pos = BIG_ul_SearchKeyToPos(idsKey);
    tgs->num = BIG_ul_GetLengthFile(pos) / 68;
    tgs->key = LOA_ul_GetCurrentKey();
    LOA_MakeFileRef(idsKey, &tgs->ids, TEXT_ul_CallbackLoadIds, 1);
    LOA_MakeFileRef(textsKey, &tgs->texts, TEXT_ul_CallbackLoadText, 1);
    
    return tgs;
}

tWorldTextsStruct* TEXT_ul_CallbackLoadWorldText(uint32_t Offset)
{    
    tWorldTextsStruct* wts;
    int txgKey;
    int txgTag;    
    char* readBuffer;
    unsigned int num;
    unsigned int i;
    int size;

    readBuffer = BIG_pc_ReadFileTmp(Offset, &size);
    num = (size / 2 * sizeof(uint32_t));

    if (num)
    {
        wts = (tWorldTextsStruct*)MEM_p_Alloc(sizeof(tWorldTextsStruct*) * num);
        *gpi_NumLoaded = num;

        for (i = 0; i < num; i++) {
            txgKey = LOA_ReadLong(&readBuffer, 0, 1);
            txgTag = LOA_ReadLong(&readBuffer, 0, 1);

            wts[i].ptr = NULL;

            if (txgKey && txgKey != -1 && !memcmp(&txgTag, ".txg", 4))
                LOA_MakeFileRef(txgKey, &wts[i].ptr, TEXT_ul_CallbackLoadLang, 1);
        }
    }
    else
    {
        wts = (tWorldTextsStruct*)MEM_p_Alloc(sizeof(tWorldTextsStruct*));
        wts->ptr = NULL;
    }
    return wts;
}

void TEXT_LoadReal(int key)
{
    unsigned int txUnknownConstPos;
    int i;

    gpi_NumLoaded = &TEXT_gst_Global.num;
    
    LOA_MakeFileRef(key, &TEXT_gst_Global.worldTexts1, TEXT_ul_CallbackLoadWorldText, 1);
    LOA_Resolve();

    txUnknownConstPos = BIG_ul_SearchKeyToPos(txUnknownConst);
    if (LOA_gb_SpeedMode || txUnknownConstPos != -1 && txUnknownConstPos != txUnknownConst)
    {
        gpi_NumLoaded = &TEXT_gst_Global.num2;
    
        LOA_MakeFileRef(txUnknownConst, &TEXT_gst_Global.worldTexts2, TEXT_ul_CallbackLoadWorldText, 1);
        LOA_Resolve();
    }
    if (TEXT_gst_Global.worldTexts2)
    {
        for (i = 0; i < TEXT_gst_Global.num2; i++)
        {
            if (TEXT_gst_Global.worldTexts2[i].ptr)
                ++TEXT_gst_Global.worldTexts2[i].ptr->usageCount;
        }
    }
    if (TEXT_gst_Global.worldTexts1)
    {
        for (i = 0; i < TEXT_gst_Global.num; i++)
        {
            if (TEXT_gst_Global.worldTexts1[i].ptr)
                ++TEXT_gst_Global.worldTexts1[i].ptr->usageCount;
        }
    }
}

void TEXT_ChangeLang(int langID)
{
    ++TEXT_ChangeLangUnkCounter;
    ++TEXT_ChangeLangUnkVar;
    PreLoadTexReset();
    TEXT_FreeAll();
    TEXT_gst_Global.language = langID;
    LOA_BeginSpeedMode(((langID + 1) << 20) | 0xfd000000);
    if (WOR_gul_WorldText != -1)
        TEXT_LoadReal(WOR_gul_WorldText);
    LOA_EndSpeedMode();
    SND_UpdateLang();
    STR_3DStringList_Reinit();
    TEXT_ChangeLangUnkVar = 0;
}

void TEXT_LoadWorldText(int key)
{
    if (key != -1)
    {
        if (LOA_gb_SpeedMode)
        {
            LOA_BeginSpeedMode(((TEXT_gst_Global.language + 1) << 20) | 0xfd000000);
            TEXT_LoadReal(key);
            LOA_EndSpeedMode();
        }
        else
        {
            TEXT_LoadReal(key);
        }
    }
}

void TEXT_InitModule()
{
    TEXT_gst_Global.num2 = 0;
    TEXT_gst_Global.worldTexts2 = NULL;
    TEXT_gst_Global.num = 0;
    TEXT_gst_Global.worldTexts1 = NULL;
    TEXT_gst_Global.language = 0;
}

void TEXT_CloseModule()
{
    TEXT_FreeAll();
}