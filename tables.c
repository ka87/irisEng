#include "common.h"
#include "memory.h"
#include "tables.h"

BOOL TAB_TableElemIsInvalid(void* elem)
{
    return !elem || ((uint8_t)elem & 3) == 1;
}

void TAB_DLlist_CreateElem(tabDlistElemStruct** tab)
{
    *tab = (tabDlistElemStruct*)MEM_p_Alloc(sizeof(tabDlistElemStruct));
    (*tab)->nextElem = NULL;
    (*tab)->prevElem = NULL;
    (*tab)->tableElemPtr = NULL;
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
        elem->prevElem = tableList->lastElem;
    }
    else
    {
        tableList->firstElem = elem;
    }
    tableList->lastElem = elem;
    ++tableList->elemNum;
}

void TAB_DLlist_Create(tabDlistStruct* a1)
{
    a1->elemNum = 0;
    a1->lastElem = 0;
    a1->firstElem = 0;
}

void TAB_DLlist_Remove(tabDlistStruct* a1)
{
    tabDlistElemStruct* currElem;
    tabDlistElemStruct* nextElem;

    currElem = a1->firstElem;
    if (a1->firstElem)
    {
        do
        {
            nextElem = currElem->nextElem;
            MEM_Free(currElem);
            currElem = nextElem;
        } while (nextElem);
    }
}

void TAB_DLlist_RemoveElem(tabDlistStruct* a1, tabDlistElemStruct* a2)
{
    tabDlistElemStruct* prevElem;
    tabDlistElemStruct* nextElem;

    prevElem = a2->prevElem;
    nextElem = a2->nextElem;
    if (prevElem)
        prevElem->nextElem = nextElem;
    else
        a1->firstElem = nextElem;
    if (nextElem)
        nextElem->prevElem = prevElem;
    else
        a1->lastElem = prevElem;
    --a1->elemNum;
    MEM_Free(a2);
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

void TAB_TablesManager_UnregisterTable(tableDescStruct* a1)
{
    tabDlistElemStruct* currElem;

    currElem = gst_TablesManagerList.firstElem;
    if (gst_TablesManagerList.firstElem)
    {
        while (currElem->tableElemPtr != a1)
        {
            currElem = currElem->nextElem;
            if (!currElem)
                return;
        }
        TAB_DLlist_RemoveElem(&gst_TablesManagerList, currElem);
    }
}

void TAB_TablesManager_Close()
{
    if (gb_TablesManagerListInitialized)
        TAB_DLlist_Remove(&gst_TablesManagerList);
    gb_TablesManagerListInitialized = 0;
}

void TAB_Ptable_Init(tableDescStruct* a1, unsigned int num, float defragRatio)
{
    tableElemStructP* newPtr;

    if (num < 2)
        num = 2;
    newPtr = (tableElemStructP*)MEM_p_Alloc(sizeof(tableElemStructP) * num);
    a1->firstElemPtr = newPtr;
    a1->nextElemPtr = newPtr;
    a1->holeNum = 0;
    a1->actElemNum = 0;
    a1->lastElemPtr = &newPtr[num - 1];
    a1->tableType = 20;
    a1->defragRatio = defragRatio;
    TAB_TablesManager_RegisterTable(a1);
}

void TAB_Ptable_Resize(tableDescStruct* a1, unsigned int num)
{
    unsigned int newSize;
    tableElemStructP* firstElemPtr;
    unsigned int oldElemNum;
    tableElemStructP* newPtr;
    unsigned int elemNumToCopy;
    unsigned int i;
    tableElemStructP* oldptr;
    unsigned int usedElemNum;

    newSize = num;
    firstElemPtr = (tableElemStructP*)a1->firstElemPtr;
    oldElemNum = (tableElemStructP*)a1->lastElemPtr - firstElemPtr + 1;
    oldptr = firstElemPtr;
    if (abs(num - oldElemNum) < 256)
        newSize = (tableElemStructP*)a1->lastElemPtr - firstElemPtr + 257;
    usedElemNum = (tableElemStructP*)a1->nextElemPtr - firstElemPtr;
    if (newSize < 2)
        newSize = 2;
    newPtr = (tableElemStructP*)MEM_p_Alloc(sizeof(tableElemStructP) * newSize);
    a1->lastElemPtr = &newPtr[newSize - 1];
    elemNumToCopy = ((int)(sizeof(tableElemStructP) * newSize - sizeof(tableElemStructP)) >> 2) + 1;
    a1->firstElemPtr = newPtr;
    if (elemNumToCopy >= oldElemNum)
        elemNumToCopy = oldElemNum;
    for (i = 0; i < elemNumToCopy; ++i)
        ((tableElemStructP*)a1->firstElemPtr)[i] = oldptr[i];
    if (usedElemNum >= newSize)
        usedElemNum = newSize - 1;
    a1->nextElemPtr = &(((tableElemStructP*)a1->firstElemPtr)[usedElemNum]);
    MEM_Free(oldptr);
}

void TAB_Ptable_Close(tableDescStruct* a1)
{
    MEM_Free(a1->firstElemPtr);
    a1->firstElemPtr = 0;
    a1->actElemNum = 0;
    a1->holeNum = 0;
    a1->nextElemPtr = 0;
    a1->lastElemPtr = 0;
    TAB_TablesManager_UnregisterTable(a1);
}

void TAB_Ptable_RemoveHoles(tableDescStruct* a1)
{
    tableElemStructP* v1;
    tableElemStructP* v2;
    tableElemStructP* v3;

    v1 = a1->firstElemPtr;
    v2 = a1->nextElemPtr;
    v3 = v1;
    a1->nextElemPtr = &v2[-(int)(a1->holeNum)];
    if (v1 < v2)
    {
        while (v1->objectPtr && ((int)v1->objectPtr & 3) != 1)
        {
        LABEL_10:
            if (++v1 >= v2)
            {
                a1->holeNum = 0;
                return;
            }
        }
        if (v3 < v1)
            v3 = v1;
        while (++v3 != v2)
        {
            if (v3->objectPtr && ((int)v3->objectPtr & 3) != 1)
            {
                v1->objectPtr = v3->objectPtr;
                v3->objectPtr = (void *)1;
                goto LABEL_10; // FIXME
            }
        }
        if (!a1->holeNum)
            --((tableElemStructP*)(a1->nextElemPtr));
        a1->holeNum = 0;
    }
    a1->holeNum = 0;
}

void TAB_PTable_AddElemToTable(tableDescStruct* a1, void* a2)
{
    if (a1->nextElemPtr > a1->lastElemPtr)
        TAB_Ptable_Resize(a1, 2 * (a1->holeNum + a1->actElemNum));
    ((tableElemStructP*)(a1->nextElemPtr))->objectPtr = a2;
    ++a1->actElemNum;
    a1->nextElemPtr = ((tableElemStructP*)(a1->nextElemPtr)) + 1;
}

int TAB_ul_Ptable_GetElemIndexWithPointer(tableDescStruct* a1, void *a2)
{
    tableElemStructP* v2;
    tableElemStructP* v3;
    int result;

    v2 = a1->firstElemPtr;
    v3 = a1->nextElemPtr;
    result = 0;
    if (v2 >= v3)
        return -1;
    while (a2 != v2->objectPtr)
    {
        ++v2;
        ++result;
        if (v2 >= v3)
            return -1;
    }
    return result;
}

void TAB_PFtable_Init(tableDescStruct* a1, unsigned int num, float defragRatio)
{
    tableElemStructPF* newPtr;

    if (num < 2)
        num = 2;
    newPtr = (tableElemStructPF*)MEM_p_Alloc(sizeof(tableElemStructPF) * num);
    a1->firstElemPtr = newPtr;
    a1->nextElemPtr = newPtr;
    a1->holeNum = 0;
    a1->actElemNum = 0;
    a1->lastElemPtr = &newPtr[num - 1];
    a1->defragRatio = defragRatio;
    a1->tableType = 30;
    TAB_TablesManager_RegisterTable(a1);
}

void TAB_PFtable_Clear(tableDescStruct* a1)
{
    a1->nextElemPtr = a1->firstElemPtr;
    a1->holeNum = 0;
    a1->actElemNum = 0;
}

void TAB_PFtable_Resize(tableDescStruct* a1, unsigned int size)
{
    unsigned int newSize;
    tableElemStructPF* firstElemPtr;
    unsigned int oldElemNum;
    tableElemStructPF* newPtr;
    unsigned int elemNumToCopy;
    unsigned int i;
    unsigned int finalElemNumToCopy;
    tableElemStructPF* oldptr;
    unsigned int usedElemNum;

    newSize = size;
    firstElemPtr = a1->firstElemPtr;
    oldElemNum = ((tableElemStructPF*)(a1->lastElemPtr)) - firstElemPtr + 1;
    oldptr = firstElemPtr;
    if (abs(oldElemNum - size) < 256)
        newSize = ((tableElemStructPF *)(a1->lastElemPtr)) - firstElemPtr + 257;
    usedElemNum = ((tableElemStructPF*)(a1->nextElemPtr)) - firstElemPtr;
    if (newSize < 2)
        newSize = 2;
    newPtr = (tableElemStructPF*)MEM_p_Alloc(sizeof(tableElemStructPF) * newSize);
    a1->lastElemPtr = &newPtr[newSize - 1];
    elemNumToCopy = ((int)(sizeof(tableElemStructPF) * newSize - sizeof(tableElemStructPF)) >> 3) + 1;
    a1->firstElemPtr = newPtr;
    for (i = 0; ; ++i)
    {
        finalElemNumToCopy = elemNumToCopy;
        if (elemNumToCopy >= oldElemNum)
            finalElemNumToCopy = oldElemNum;
        if (i >= finalElemNumToCopy)
            break;
        ((tableElemStructPF*)(a1->firstElemPtr))[i] = oldptr[i];
        elemNumToCopy = ((int)(8 * newSize - 8) >> 3) + 1;
    }
    if (usedElemNum >= newSize)
        usedElemNum = newSize - 1;
    a1->nextElemPtr = &(((tableElemStructPF*)(a1->firstElemPtr))[usedElemNum]);
    MEM_Free(oldptr);
}

void TAB_PFtable_Close(tableDescStruct* a1)
{
    if (a1->firstElemPtr)
        MEM_Free(a1->firstElemPtr);
    a1->firstElemPtr = 0;
    a1->actElemNum = 0;
    a1->holeNum = 0;
    a1->nextElemPtr = 0;
    a1->lastElemPtr = 0;
    TAB_TablesManager_UnregisterTable(a1);
}

void TAB_PFtable_RemoveHoles(tableDescStruct* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* v2;
    tableElemStructPF* v3;

    v1 = a1->firstElemPtr;
    v2 = a1->nextElemPtr;
    v3 = v1;
    a1->nextElemPtr = &v2[-(int)(a1->holeNum)];
    if (v1 < v2)
    {
        while (v1->objectPtr && ((int)v1->objectPtr & 3) != 1)
        {
        LABEL_10:
            if (++v1 >= v2)
                return;
        }
        if (v3 < v1)
            v3 = v1;
        while (++v3 != v2)
        {
            if (v3->objectPtr && ((int)v3->objectPtr & 3) != 1)
            {
                v1->objectPtr = v3->objectPtr;
                v1->objectFlags = v3->objectFlags;
                v3->objectPtr = (void*)1;
                goto LABEL_10; // FIXME
            }
        }
        if (!a1->holeNum)
            --((tableElemStructPF*)(a1->nextElemPtr));
        a1->holeNum = 0;
    }
}

void TAB_PFtable_RemoveElem(tableDescStruct* a1, tableElemStructPF* a2)
{
    unsigned int v2; // edx

    if (a2 == ((tableElemStructPF*)(a1->nextElemPtr)) - 1)
    {
        if (a2 && ((uint8_t)a2 & 3) != 1)
        {
            --a1->actElemNum;
            a1->nextElemPtr = a2;
            a2->objectPtr = (void*)1;
            a2->objectFlags = 0;
        }
        else
        {
            --a1->holeNum;
            a1->nextElemPtr = a2;
            a2->objectPtr = (void*)1;
            a2->objectFlags = 0;
        }
    }
    else
    {
        if (a2->objectPtr && ((int)a2->objectPtr & 3) != 1)
        {
            a2->objectPtr = (void*)1;
            v2 = a1->actElemNum - 1;
            ++a1->holeNum;
            a1->actElemNum = v2;
        }
        a2->objectFlags = 0;
    }
}

int TAB_ul_PFtable_GetElemIndexWithPointer(tableDescStruct* a1, void* a2)
{
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    int result;

    v2 = a1->firstElemPtr;
    v3 = a1->nextElemPtr;
    result = 0;
    if (v2 >= v3)
        return -1;
    while (a2 != v2->objectPtr)
    {
        ++v2;
        ++result;
        if (v2 >= v3)
            return -1;
    }
    return result;
}

tableElemStructPF* TAB_pst_PFtable_GetElemWithPointer(tableDescStruct* a1, void* a2)
{
    tableElemStructPF* result; // eax
    tableElemStructPF* v3; // ecx

    result = a1->firstElemPtr;
    v3 = a1->nextElemPtr;
    if (result >= v3)
        return 0;
    while (a2 != result->objectPtr)
    {
        if (++result >= v3)
            return 0;
    }
    return result;
}

void TAB_AddElemToTablePF(tableDescStruct* table, void* objectPtr, int objectFlags)
{
    ((tableElemStructPF*)(table->nextElemPtr))->objectPtr = objectPtr;
    ((tableElemStructPF*)(table->nextElemPtr))->objectFlags = objectFlags;
    ++table->actElemNum;
    table->nextElemPtr = ((tableElemStructPF*)(table->nextElemPtr)) + 1;
}

void TAB_PFTable_AddElemToTable(tableDescStruct* a1, void* object, int flags)
{
    if (a1->nextElemPtr > a1->lastElemPtr)
    {
        if (a1->holeNum)
            TAB_PFtable_RemoveHoles(a1);
        if (a1->nextElemPtr > a1->lastElemPtr)
            TAB_PFtable_Resize(a1, 2 * (a1->holeNum + a1->actElemNum));
    }
    TAB_AddElemToTablePF(a1, object, flags);
}

void TAB_InitModule()
{
    TAB_TablesManager_Init();
}

void TAB_CloseModule()
{
    TAB_TablesManager_Close();
}

void TAB_TablesManager_RemoveHolesFromTablesList(tabDlistStruct* a1)
{
    tabDlistElemStruct* actElem;
    tableDescStruct* v2;
    byte v3; // al

    for (actElem = a1->firstElem; actElem; actElem = actElem->nextElem)
    {
        v2 = actElem->tableElemPtr;
        v3 = v2->tableType;
        if (v3 == 20)
        {
            if (v2->defragRatio < (double)v2->holeNum / (double)v2->actElemNum)
                TAB_Ptable_RemoveHoles(actElem->tableElemPtr);
        }
        else if (v3 == 30 && v2->defragRatio < (double)v2->holeNum / (double)v2->actElemNum)
        {
            TAB_PFtable_RemoveHoles(actElem->tableElemPtr);
        }
    }
}

void TAB_TablesManager_Call()
{
    TAB_TablesManager_RemoveHolesFromTablesList(&gst_TablesManagerList);
}

void TAB_PFtable_RemoveElemWithPointer(tableDescStruct* a1, void* a2)
{
    tableElemStructPF* v2;

    v2 = TAB_pst_PFtable_GetElemWithPointer(a1, a2);
    if (v2)
        TAB_PFtable_RemoveElem(a1, v2);
}