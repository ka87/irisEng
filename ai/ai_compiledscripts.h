#pragma once
#include "../ai.h"

typedef struct sStruct_funclist
{
    int key;
    int(*func)();
} tStruct_funclist;

//extern tStruct_funclist AI2C_gat_fctdefs[136];
//extern tStruct_funclist AI2C_gat_fctdefs[2530]; demo
extern tStruct_funclist AI2C_gat_fctdefs[];

unsigned int AI2C_i_SizeofArray();
