#pragma once
#include "common.h"

// A különbözõ EOT tábláknál vigyázni, mert ha túlcsordulnak (500 elemre vannak inicializálva), akkor nincs ellenõrzés arra, 
// hogy megteltek és túlírják a memóriát, ez a gyakorlatban azt jelenti, hogy egy következõ tábla elemeit írják felül 
// (pl inventory manager init, mikor még nem volt a special init func megírva alatta, és az o_case alá folyamatosan duplikálta az objektumokat 16osával
// és beleírt az animációs objektumok táblába. mivel az ANI_mainCall és társaiban már nincs objektumflag ellenõrzés a 2-es flagre, ezért azthitte az objektumra, hogy animáéciós. 
// Mivel ennek nem volt már animációs struktúrája, mikor ráhivatkozott elcrashelt)

typedef struct tableElemStructP
{
    void *objectPtr;
} tableElemStructP;

typedef struct tableElemStructPF
{
    void* objectPtr;
    int objectFlags; // gao.flags másolata
} tableElemStructPF;



typedef struct tableDescStruct
{
    void* lastElemPtr;
    void* nextElemPtr;
    unsigned int holeNum;
    unsigned int actElemNum;
    void* firstElemPtr;
    byte tableType;
    byte _pad[3];
    float defragRatio;
} tableDescStruct;

typedef struct tabDlistElemStruct
{
    struct tabDlistElemStruct* nextElem;
    struct tabDlistElemStruct* prevElem;
    tableDescStruct* tableElemPtr;
} tabDlistElemStruct;

typedef struct tabDlistStruct
{
    tabDlistElemStruct* firstElem;
    tabDlistElemStruct* lastElem;
    int elemNum;
} tabDlistStruct;

tabDlistStruct gst_TablesManagerList;
BOOL gb_TablesManagerListInitialized;

void TAB_PFTable_AddElemToTable(tableDescStruct* a1, void* object, int flags);
void TAB_PTable_AddElemToTable(tableDescStruct* a1, void* a2);
void TAB_AddElemToTablePF(tableDescStruct* a1, void* a2, int a3);
void TAB_PFtable_RemoveElemWithPointer(tableDescStruct* a1, void* a2);
void TAB_PFtable_Clear(tableDescStruct* a1);
int TAB_ul_PFtable_GetElemIndexWithPointer(tableDescStruct* a1, void* a2);
int TAB_ul_Ptable_GetElemIndexWithPointer(tableDescStruct* a1, void* a2);
void TAB_PFtable_Init(tableDescStruct* a1, unsigned int num, float defragRatio);
void TAB_Ptable_Init(tableDescStruct* a1, unsigned int num, float defragRatio);
void TAB_PFtable_RemoveElem(tableDescStruct* a1, tableElemStructPF* a2);
void TAB_PFtable_Close(tableDescStruct* a1);
void TAB_PFtable_RemoveHoles(tableDescStruct* a1);
BOOL TAB_TableElemIsInvalid(void* a1);
void TAB_Ptable_Resize(tableDescStruct* a1, unsigned int num);
void TAB_Ptable_Close(tableDescStruct* a1);
void TAB_Ptable_RemoveHoles(tableDescStruct* a1);
void TAB_PFtable_Resize(tableDescStruct* a1, unsigned int size);
void TAB_TablesManager_Call();
void TAB_InitModule();
void TAB_CloseModule();
