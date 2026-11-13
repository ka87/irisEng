#include "common.h"
#include "memory.h"
#include "tables.h"

BOOL TAB_TableElemIsInvalid(void* elem)
{
    return !elem || ((uint8_t)elem & 3) == 1;
}

void TAB_DLlist_CreateElem(tabDlistElemStruct** elem)
{
    *elem = (tabDlistElemStruct*)MEM_p_Alloc(sizeof(tabDlistElemStruct));
    (*elem)->nextElem = NULL;
    (*elem)->prevElem = NULL;
    (*elem)->tableElemPtr = NULL;
}

void TAB_DLlist_CreateAndAddElem(tabDlistStruct* tableList, tableDescStruct* table)
{
    tabDlistElemStruct* lastElem;
    tabDlistElemStruct* elem = NULL;

    TAB_DLlist_CreateElem(&elem);

    elem->tableElemPtr = table;
    lastElem = tableList->lastElem;

    if (lastElem)
    {
        lastElem->nextElem = elem;
        elem->prevElem = lastElem;
    }
    else
    {
        tableList->firstElem = elem;
    }
    tableList->lastElem = elem;
    ++tableList->elemNum;
}

void TAB_DLlist_Create(tabDlistStruct* list)
{
    list->elemNum = 0;
    list->lastElem = 0;
    list->firstElem = 0;
}

void TAB_DLlist_Remove(tabDlistStruct* list)
{
    tabDlistElemStruct* elem;
    tabDlistElemStruct* nextElem;

    for (elem = list->firstElem; elem; elem = nextElem)
    {
        nextElem = elem->nextElem;
        MEM_Free(elem);
    }
}

void TAB_DLlist_RemoveElem(tabDlistStruct* list, tabDlistElemStruct* elem)
{
    tabDlistElemStruct* prevElem;
    tabDlistElemStruct* nextElem;

    prevElem = elem->prevElem;
    nextElem = elem->nextElem;
    if (prevElem)
        prevElem->nextElem = nextElem;
    else
        list->firstElem = nextElem;
    if (nextElem)
        nextElem->prevElem = prevElem;
    else
        list->lastElem = prevElem;
    --list->elemNum;
    MEM_Free(elem);
}

void TAB_TablesManager_Init()
{
    TAB_DLlist_Create(&gst_TablesManagerList);
    gb_TablesManagerListInitialized = 1;
}

void TAB_TablesManager_RegisterTable(tableDescStruct* table)
{
    if (table->defragRatio != 1.0)
        TAB_DLlist_CreateAndAddElem(&gst_TablesManagerList, table);
}

void TAB_TablesManager_UnregisterTable(tableDescStruct* table)
{
    tabDlistElemStruct* elem;

    for (elem = gst_TablesManagerList.firstElem; elem; elem = elem->nextElem)
    {
        if (elem->tableElemPtr == table)
        {
            TAB_DLlist_RemoveElem(&gst_TablesManagerList, elem);
            return;
        }
    }
}

void TAB_TablesManager_Close()
{
    if (gb_TablesManagerListInitialized)
        TAB_DLlist_Remove(&gst_TablesManagerList);
    gb_TablesManagerListInitialized = 0;
}

void TAB_Ptable_Init(tableDescStruct* table, unsigned int num, float defragRatio)
{
    tableElemStructP* newPtr;

    if (num < 2)
        num = 2;
    newPtr = (tableElemStructP*)MEM_p_Alloc(sizeof(tableElemStructP) * num);
    table->firstElemPtr = newPtr;
    table->nextElemPtr = newPtr;
    table->holeNum = 0;
    table->actElemNum = 0;
    table->lastElemPtr = &newPtr[num - 1];
    table->tableType = 20;
    table->defragRatio = defragRatio;
    TAB_TablesManager_RegisterTable(table);
}

void TAB_Ptable_Resize(tableDescStruct* table, unsigned int num)
{
    tableElemStructP* oldPtr;
    tableElemStructP* newPtr;
    unsigned int oldElemNum;
    unsigned int newElemNum;
    unsigned int usedElemNum;
    unsigned int elemNumToCopy;
    unsigned int i;

    oldPtr = (tableElemStructP*)table->firstElemPtr;
    oldElemNum = (tableElemStructP*)table->lastElemPtr - oldPtr + 1;
    newElemNum = num;
    if (abs((int)(num - oldElemNum)) < 256)
        newElemNum = oldElemNum + 256;
    usedElemNum = (tableElemStructP*)table->nextElemPtr - oldPtr;
    if (newElemNum < 2)
        newElemNum = 2;
    newPtr = (tableElemStructP*)MEM_p_Alloc(sizeof(tableElemStructP) * newElemNum);
    table->lastElemPtr = &newPtr[newElemNum - 1];
    table->firstElemPtr = newPtr;
    elemNumToCopy = newElemNum;
    if (elemNumToCopy >= oldElemNum)
        elemNumToCopy = oldElemNum;
    for (i = 0; i < elemNumToCopy; ++i)
        newPtr[i] = oldPtr[i];
    if (usedElemNum >= newElemNum)
        usedElemNum = newElemNum - 1;
    table->nextElemPtr = &newPtr[usedElemNum];
    MEM_Free(oldPtr);
}

void TAB_Ptable_Close(tableDescStruct* table)
{
    MEM_Free(table->firstElemPtr);
    table->firstElemPtr = 0;
    table->actElemNum = 0;
    table->holeNum = 0;
    table->nextElemPtr = 0;
    table->lastElemPtr = 0;
    TAB_TablesManager_UnregisterTable(table);
}

void TAB_Ptable_RemoveHoles(tableDescStruct* table)
{
    tableElemStructP* dest;
    tableElemStructP* end;
    tableElemStructP* scan;

    dest = (tableElemStructP*)table->firstElemPtr;
    end = (tableElemStructP*)table->nextElemPtr;
    scan = dest;
    table->nextElemPtr = &end[-(int)table->holeNum];
    if (dest < end)
    {
        for (;;)
        {
            while (!TAB_TableElemIsInvalid(dest->objectPtr))
            {
                if (++dest >= end)
                {
                    table->holeNum = 0;
                    return;
                }
            }
            if (scan < dest)
                scan = dest;
            while (++scan != end)
            {
                if (!TAB_TableElemIsInvalid(scan->objectPtr))
                    break;
            }
            if (scan == end)
                break;
            dest->objectPtr = scan->objectPtr;
            scan->objectPtr = (void*)1;
            if (++dest >= end)
            {
                table->holeNum = 0;
                return;
            }
        }
        if (!table->holeNum)
            table->nextElemPtr = (tableElemStructP*)table->nextElemPtr - 1;
    }
    table->holeNum = 0;
}

void TAB_PTable_AddElemToTable(tableDescStruct* table, void* object)
{
    if (table->nextElemPtr > table->lastElemPtr)
        TAB_Ptable_Resize(table, 2 * (table->holeNum + table->actElemNum));
    ((tableElemStructP*)(table->nextElemPtr))->objectPtr = object;
    ++table->actElemNum;
    table->nextElemPtr = ((tableElemStructP*)(table->nextElemPtr)) + 1;
}

int TAB_ul_Ptable_GetElemIndexWithPointer(tableDescStruct* table, void* object)
{
    tableElemStructP* elem;
    tableElemStructP* end;
    int index;

    elem = (tableElemStructP*)table->firstElemPtr;
    end = (tableElemStructP*)table->nextElemPtr;
    for (index = 0; elem < end; ++index, ++elem)
    {
        if (object == elem->objectPtr)
            return index;
    }
    return -1;
}

void TAB_PFtable_Init(tableDescStruct* table, unsigned int num, float defragRatio)
{
    tableElemStructPF* newPtr;

    if (num < 2)
        num = 2;
    newPtr = (tableElemStructPF*)MEM_p_Alloc(sizeof(tableElemStructPF) * num);
    table->firstElemPtr = newPtr;
    table->nextElemPtr = newPtr;
    table->holeNum = 0;
    table->actElemNum = 0;
    table->lastElemPtr = &newPtr[num - 1];
    table->defragRatio = defragRatio;
    table->tableType = 30;
    TAB_TablesManager_RegisterTable(table);
}

void TAB_PFtable_Clear(tableDescStruct* table)
{
    table->nextElemPtr = table->firstElemPtr;
    table->holeNum = 0;
    table->actElemNum = 0;
}

void TAB_PFtable_Resize(tableDescStruct* table, unsigned int size)
{
    tableElemStructPF* oldPtr;
    tableElemStructPF* newPtr;
    unsigned int oldElemNum;
    unsigned int newElemNum;
    unsigned int usedElemNum;
    unsigned int elemNumToCopy;
    unsigned int i;

    oldPtr = (tableElemStructPF*)table->firstElemPtr;
    oldElemNum = ((tableElemStructPF*)(table->lastElemPtr)) - oldPtr + 1;
    newElemNum = size;
    if (abs((int)(oldElemNum - size)) < 256)
        newElemNum = oldElemNum + 256;
    usedElemNum = ((tableElemStructPF*)(table->nextElemPtr)) - oldPtr;
    if (newElemNum < 2)
        newElemNum = 2;
    newPtr = (tableElemStructPF*)MEM_p_Alloc(sizeof(tableElemStructPF) * newElemNum);
    table->lastElemPtr = &newPtr[newElemNum - 1];
    table->firstElemPtr = newPtr;
    elemNumToCopy = newElemNum;
    if (elemNumToCopy >= oldElemNum)
        elemNumToCopy = oldElemNum;
    for (i = 0; i < elemNumToCopy; ++i)
        newPtr[i] = oldPtr[i];
    if (usedElemNum >= newElemNum)
        usedElemNum = newElemNum - 1;
    table->nextElemPtr = &newPtr[usedElemNum];
    MEM_Free(oldPtr);
}

void TAB_PFtable_Close(tableDescStruct* table)
{
    if (table->firstElemPtr)
        MEM_Free(table->firstElemPtr);
    table->firstElemPtr = 0;
    table->actElemNum = 0;
    table->holeNum = 0;
    table->nextElemPtr = 0;
    table->lastElemPtr = 0;
    TAB_TablesManager_UnregisterTable(table);
}

void TAB_PFtable_RemoveHoles(tableDescStruct* table)
{
    tableElemStructPF* dest;
    tableElemStructPF* end;
    tableElemStructPF* scan;

    dest = (tableElemStructPF*)table->firstElemPtr;
    end = (tableElemStructPF*)table->nextElemPtr;
    scan = dest;
    table->nextElemPtr = &end[-(int)table->holeNum];
    if (dest < end)
    {
        for (;;)
        {
            while (!TAB_TableElemIsInvalid(dest->objectPtr))
            {
                if (++dest >= end)
                    return;
            }
            if (scan < dest)
                scan = dest;
            while (++scan != end)
            {
                if (!TAB_TableElemIsInvalid(scan->objectPtr))
                    break;
            }
            if (scan == end)
                break;
            dest->objectPtr = scan->objectPtr;
            dest->objectFlags = scan->objectFlags;
            scan->objectPtr = (void*)1;
            if (++dest >= end)
                return;
        }
        if (!table->holeNum)
            table->nextElemPtr = (tableElemStructPF*)table->nextElemPtr - 1;
        table->holeNum = 0;
    }
}

void TAB_PFtable_RemoveElem(tableDescStruct* table, tableElemStructPF* elem)
{
    if (elem == ((tableElemStructPF*)(table->nextElemPtr)) - 1)
    {
        // preserved: the test is on the element and not on the object it holds, so the hole arm is unreachable
        if (!TAB_TableElemIsInvalid(elem))
            --table->actElemNum;
        else
            --table->holeNum;
        table->nextElemPtr = elem;
        elem->objectPtr = (void*)1;
        elem->objectFlags = 0;
    }
    else
    {
        if (!TAB_TableElemIsInvalid(elem->objectPtr))
        {
            elem->objectPtr = (void*)1;
            ++table->holeNum;
            --table->actElemNum;
        }
        elem->objectFlags = 0;
    }
}

int TAB_ul_PFtable_GetElemIndexWithPointer(tableDescStruct* table, void* object)
{
    tableElemStructPF* elem;
    tableElemStructPF* end;
    int index;

    elem = (tableElemStructPF*)table->firstElemPtr;
    end = (tableElemStructPF*)table->nextElemPtr;
    for (index = 0; elem < end; ++index, ++elem)
    {
        if (object == elem->objectPtr)
            return index;
    }
    return -1;
}

tableElemStructPF* TAB_pst_PFtable_GetElemWithPointer(tableDescStruct* table, void* object)
{
    tableElemStructPF* elem;
    tableElemStructPF* end;

    elem = (tableElemStructPF*)table->firstElemPtr;
    end = (tableElemStructPF*)table->nextElemPtr;
    for (; elem < end; ++elem)
    {
        if (object == elem->objectPtr)
            return elem;
    }
    return 0;
}

void TAB_AddElemToTablePF(tableDescStruct* table, void* objectPtr, int objectFlags)
{
    ((tableElemStructPF*)(table->nextElemPtr))->objectPtr = objectPtr;
    ((tableElemStructPF*)(table->nextElemPtr))->objectFlags = objectFlags;
    ++table->actElemNum;
    table->nextElemPtr = ((tableElemStructPF*)(table->nextElemPtr)) + 1;
}

void TAB_PFTable_AddElemToTable(tableDescStruct* table, void* object, int flags)
{
    if (table->nextElemPtr > table->lastElemPtr)
    {
        if (table->holeNum)
            TAB_PFtable_RemoveHoles(table);
        if (table->nextElemPtr > table->lastElemPtr)
            TAB_PFtable_Resize(table, 2 * (table->holeNum + table->actElemNum));
    }
    TAB_AddElemToTablePF(table, object, flags);
}

void TAB_InitModule()
{
    TAB_TablesManager_Init();
}

void TAB_CloseModule()
{
    TAB_TablesManager_Close();
}

void TAB_TablesManager_RemoveHolesFromTablesList(tabDlistStruct* list)
{
    tabDlistElemStruct* elem;
    tableDescStruct* table;
    uint8_t tableType;

    for (elem = list->firstElem; elem; elem = elem->nextElem)
    {
        table = elem->tableElemPtr;
        tableType = table->tableType;
        if (tableType == 20)
        {
            if (table->defragRatio < (double)table->holeNum / (double)table->actElemNum)
                TAB_Ptable_RemoveHoles(table);
        }
        else if (tableType == 30 && table->defragRatio < (double)table->holeNum / (double)table->actElemNum)
        {
            TAB_PFtable_RemoveHoles(table);
        }
    }
}

void TAB_TablesManager_Call()
{
    TAB_TablesManager_RemoveHolesFromTablesList(&gst_TablesManagerList);
}

void TAB_PFtable_RemoveElemWithPointer(tableDescStruct* table, void* object)
{
    tableElemStructPF* elem;

    elem = TAB_pst_PFtable_GetElemWithPointer(table, object);
    if (elem)
        TAB_PFtable_RemoveElem(table, elem);
}
