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
    uint8_t tableType;
    uint8_t _pad[3];
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

void TAB_PFTable_AddElemToTable(tableDescStruct* table, void* object, int flags);
void TAB_PTable_AddElemToTable(tableDescStruct* table, void* object);
void TAB_AddElemToTablePF(tableDescStruct* table, void* objectPtr, int objectFlags);
void TAB_PFtable_RemoveElemWithPointer(tableDescStruct* table, void* object);
void TAB_PFtable_Clear(tableDescStruct* table);
int TAB_ul_PFtable_GetElemIndexWithPointer(tableDescStruct* table, void* object);
int TAB_ul_Ptable_GetElemIndexWithPointer(tableDescStruct* table, void* object);
void TAB_PFtable_Init(tableDescStruct* table, unsigned int num, float defragRatio);
void TAB_Ptable_Init(tableDescStruct* table, unsigned int num, float defragRatio);
void TAB_PFtable_RemoveElem(tableDescStruct* table, tableElemStructPF* elem);
void TAB_PFtable_Close(tableDescStruct* table);
void TAB_PFtable_RemoveHoles(tableDescStruct* table);
BOOL TAB_TableElemIsInvalid(void* elem);
void TAB_Ptable_Resize(tableDescStruct* table, unsigned int num);
void TAB_Ptable_Close(tableDescStruct* table);
void TAB_Ptable_RemoveHoles(tableDescStruct* table);
void TAB_PFtable_Resize(tableDescStruct* table, unsigned int size);
void TAB_TablesManager_Call();
void TAB_InitModule();
void TAB_CloseModule();
