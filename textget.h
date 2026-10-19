#pragma once
#include "common.h"

typedef struct sTextGet
{
    int key;
    unsigned int idx;
} tTextGet;

typedef struct sTextGroupStruct
{
    int key;
    unsigned int num;
    int* ids;
    char* texts;
    int usageCount;
} tTextGroupStruct;


typedef struct sWorldTextsStruct
{
    tTextGroupStruct* ptr;
} tWorldTextsStruct;

typedef struct sTextGlobal
{
    int language;
    int num2;
    tWorldTextsStruct* worldTexts2;
    int num;
    tWorldTextsStruct* worldTexts1;
} tTextGlobal;


tTextGlobal TEXT_gst_Global;
int TEXT_ChangeLangUnkCounter;
int TEXT_ChangeLangUnkVar;
int* gpi_NumLoaded;

int TEXT_i_GetLang();
void TEXT_ChangeLang(int a1);
void TEXT_FreeAll();
void TEXT_InitModule();
void TEXT_CloseModule();
void TEXT_LoadWorldText(int key);