#pragma once
#include "common.h"

typedef struct BASKeyValStruct
{
    int key;
    int val;
} BASKeyValStruct;

typedef struct BASStruct
{
    BASKeyValStruct* keyValPtr;
    int actElemNum;
    int maxElemNum;
    int elemNumToAddWhenFull;
} BASStruct;

BASKeyValStruct* sgpst_Prev;
extern int BAS_bsortmode;

void BAS_binit(BASStruct* a1, int elemNum);
void BAS_bfree(BASStruct* a1);
void BAS_binsert(unsigned int key, int val, BASStruct* bas);
int BAS_bsearch(int key, BASStruct* bas);
void BAS_bdeletegetval(int key, BASStruct* bas, int* val);
void BAS_bdelete(int key, BASStruct* bas);
void BAS_bsort(BASStruct* a1);
void BAS_InitModule();
void BAS_CloseModule();