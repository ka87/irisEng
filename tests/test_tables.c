/* The differential test for tables.c.
 *
 * The frozen pre-rewrite copy (tests/baseline_tables.c) and the
 * rewrite go into one binary with their symbols prefixed BASE_ and
 * NEW_ -- the thirty-one functions and both module globals, so the
 * two sides share nothing but the mock allocator.  Each side runs the
 * same script over its own state and is compared on what it leaves in
 * the table descriptor, on every element of the element array, on the
 * manager list, on the values the query functions return, and on the
 * ordered sequence of allocator calls.
 *
 * One limitation, stated rather than papered over.  Both Resize
 * functions compute the number of elements to copy from a byte count
 * divided by the element size written as a literal shift -- >>2 for
 * the P table, >>3 for the PF table, which are the sizes of those
 * structs in a 32-bit build.  On this 64-bit host the structs are 8
 * and 16 bytes, so the frozen copy's count comes out at 2n-1 where
 * the rewrite's is n.  min() with the old element count makes the two
 * agree whenever the table grows, which is what every call site in
 * the engine does and what this test drives; a resize that shrinks a
 * table by 256 elements or more is not comparable on this host and is
 * left out, named here rather than hidden. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables_mocks.h"

#define DECL(p) \
    BOOL p##TAB_TableElemIsInvalid(void*); \
    void p##TAB_DLlist_CreateElem(tabDlistElemStruct**); \
    void p##TAB_DLlist_CreateAndAddElem(tabDlistStruct*, tableDescStruct*); \
    void p##TAB_DLlist_Create(tabDlistStruct*); \
    void p##TAB_DLlist_Remove(tabDlistStruct*); \
    void p##TAB_DLlist_RemoveElem(tabDlistStruct*, tabDlistElemStruct*); \
    void p##TAB_TablesManager_Init(void); \
    void p##TAB_TablesManager_RegisterTable(tableDescStruct*); \
    void p##TAB_TablesManager_UnregisterTable(tableDescStruct*); \
    void p##TAB_TablesManager_Close(void); \
    void p##TAB_TablesManager_Call(void); \
    void p##TAB_TablesManager_RemoveHolesFromTablesList(tabDlistStruct*); \
    void p##TAB_Ptable_Init(tableDescStruct*, unsigned int, float); \
    void p##TAB_Ptable_Resize(tableDescStruct*, unsigned int); \
    void p##TAB_Ptable_Close(tableDescStruct*); \
    void p##TAB_Ptable_RemoveHoles(tableDescStruct*); \
    void p##TAB_PTable_AddElemToTable(tableDescStruct*, void*); \
    int  p##TAB_ul_Ptable_GetElemIndexWithPointer(tableDescStruct*, void*); \
    void p##TAB_PFtable_Init(tableDescStruct*, unsigned int, float); \
    void p##TAB_PFtable_Clear(tableDescStruct*); \
    void p##TAB_PFtable_Resize(tableDescStruct*, unsigned int); \
    void p##TAB_PFtable_Close(tableDescStruct*); \
    void p##TAB_PFtable_RemoveHoles(tableDescStruct*); \
    void p##TAB_PFtable_RemoveElem(tableDescStruct*, tableElemStructPF*); \
    int  p##TAB_ul_PFtable_GetElemIndexWithPointer(tableDescStruct*, void*); \
    tableElemStructPF* p##TAB_pst_PFtable_GetElemWithPointer(tableDescStruct*, void*); \
    void p##TAB_AddElemToTablePF(tableDescStruct*, void*, int); \
    void p##TAB_PFTable_AddElemToTable(tableDescStruct*, void*, int); \
    void p##TAB_PFtable_RemoveElemWithPointer(tableDescStruct*, void*); \
    void p##TAB_InitModule(void); \
    void p##TAB_CloseModule(void); \
    extern tabDlistStruct p##gst_TablesManagerList; \
    extern BOOL p##gb_TablesManagerListInitialized;

DECL(BASE_)
DECL(NEW_)

typedef struct sApi
{
    BOOL (*IsInvalid)(void*);
    void (*ElemCreate)(tabDlistElemStruct**);
    void (*ListAdd)(tabDlistStruct*, tableDescStruct*);
    void (*ListCreate)(tabDlistStruct*);
    void (*ListRemove)(tabDlistStruct*);
    void (*ListRemoveElem)(tabDlistStruct*, tabDlistElemStruct*);
    void (*MgrInit)(void);
    void (*MgrRegister)(tableDescStruct*);
    void (*MgrUnregister)(tableDescStruct*);
    void (*MgrClose)(void);
    void (*MgrCall)(void);
    void (*MgrHolesList)(tabDlistStruct*);
    void (*PInit)(tableDescStruct*, unsigned int, float);
    void (*PResize)(tableDescStruct*, unsigned int);
    void (*PClose)(tableDescStruct*);
    void (*PHoles)(tableDescStruct*);
    void (*PAdd)(tableDescStruct*, void*);
    int  (*PIndex)(tableDescStruct*, void*);
    void (*FInit)(tableDescStruct*, unsigned int, float);
    void (*FClear)(tableDescStruct*);
    void (*FResize)(tableDescStruct*, unsigned int);
    void (*FClose)(tableDescStruct*);
    void (*FHoles)(tableDescStruct*);
    void (*FRemoveElem)(tableDescStruct*, tableElemStructPF*);
    int  (*FIndex)(tableDescStruct*, void*);
    tableElemStructPF* (*FGet)(tableDescStruct*, void*);
    void (*FAddRaw)(tableDescStruct*, void*, int);
    void (*FAdd)(tableDescStruct*, void*, int);
    void (*FRemovePtr)(tableDescStruct*, void*);
    void (*ModInit)(void);
    void (*ModClose)(void);
    tabDlistStruct* list;
    BOOL* inited;
} tApi;

#define API(p) { \
    p##TAB_TableElemIsInvalid, p##TAB_DLlist_CreateElem, \
    p##TAB_DLlist_CreateAndAddElem, p##TAB_DLlist_Create, \
    p##TAB_DLlist_Remove, p##TAB_DLlist_RemoveElem, \
    p##TAB_TablesManager_Init, p##TAB_TablesManager_RegisterTable, \
    p##TAB_TablesManager_UnregisterTable, p##TAB_TablesManager_Close, \
    p##TAB_TablesManager_Call, p##TAB_TablesManager_RemoveHolesFromTablesList, \
    p##TAB_Ptable_Init, p##TAB_Ptable_Resize, p##TAB_Ptable_Close, \
    p##TAB_Ptable_RemoveHoles, p##TAB_PTable_AddElemToTable, \
    p##TAB_ul_Ptable_GetElemIndexWithPointer, \
    p##TAB_PFtable_Init, p##TAB_PFtable_Clear, p##TAB_PFtable_Resize, \
    p##TAB_PFtable_Close, p##TAB_PFtable_RemoveHoles, \
    p##TAB_PFtable_RemoveElem, p##TAB_ul_PFtable_GetElemIndexWithPointer, \
    p##TAB_pst_PFtable_GetElemWithPointer, p##TAB_AddElemToTablePF, \
    p##TAB_PFTable_AddElemToTable, p##TAB_PFtable_RemoveElemWithPointer, \
    p##TAB_InitModule, p##TAB_CloseModule, \
    &p##gst_TablesManagerList, &p##gb_TablesManagerListInitialized }

static const tApi apiBase = API(BASE_);
static const tApi apiNew = API(NEW_);

/* ------------------------------------------------------------------ */

#define RES_MAX 4000000
static unsigned long long res[2][RES_MAX];
static int resLen[2];
static int resOverflow;
static int side;
static int caseId;
static int caseStart[2][20000];
static int caseCount;

#define NT 4
static tableDescStruct TBL[NT];

static void Rec(unsigned long long v)
{
    if (resLen[side] >= RES_MAX)
    {
        resOverflow = 1;
        return;
    }
    res[side][resLen[side]++] = v;
}

static unsigned long long Fbits(float f)
{
    unsigned int b;
    memcpy(&b, &f, 4);
    return b;
}

/* Everything about a table descriptor that is comparable across the
   two sides: the counters, the type, the ratio, which allocation the
   array is in, and where next/last sit relative to it in elements. */
static void RecDesc(const tableDescStruct* t, int elemSize)
{
    Rec(t->holeNum);
    Rec(t->actElemNum);
    Rec(t->tableType);
    Rec(Fbits(t->defragRatio));
    Rec(TBM_Loc(t->firstElemPtr));
    if (t->firstElemPtr)
    {
        Rec((unsigned long long)(long long)
            (((char*)t->nextElemPtr - (char*)t->firstElemPtr) / elemSize));
        Rec((unsigned long long)(long long)
            (((char*)t->lastElemPtr - (char*)t->firstElemPtr) / elemSize));
    }
    else
    {
        Rec(TBM_Loc(t->nextElemPtr));
        Rec(TBM_Loc(t->lastElemPtr));
    }
}

static void RecPElems(const tableDescStruct* t, int count)
{
    int i;
    const tableElemStructP* e = (const tableElemStructP*)t->firstElemPtr;
    for (i = 0; i < count; ++i)
        Rec(e ? (unsigned long long)(size_t)e[i].objectPtr : 0);
}

static void RecFElems(const tableDescStruct* t, int count)
{
    int i;
    const tableElemStructPF* e = (const tableElemStructPF*)t->firstElemPtr;
    for (i = 0; i < count; ++i)
    {
        Rec(e ? (unsigned long long)(size_t)e[i].objectPtr : 0);
        Rec(e ? (unsigned long long)(unsigned int)e[i].objectFlags : 0);
    }
}

static void RecList(const tApi* a)
{
    const tabDlistElemStruct* e;
    int n = 0;
    Rec((unsigned long long)(unsigned int)a->list->elemNum);
    Rec(TBM_Loc(a->list->firstElem));
    Rec(TBM_Loc(a->list->lastElem));
    Rec((unsigned long long)(unsigned int)*a->inited);
    for (e = a->list->firstElem; e && n < 64; e = e->nextElem, ++n)
    {
        int idx = -1;
        int i;
        for (i = 0; i < NT; ++i)
            if (e->tableElemPtr == &TBL[i])
                idx = i;
        Rec((unsigned long long)(unsigned int)idx);
        Rec(TBM_Loc(e));
        Rec(TBM_Loc(e->prevElem));
        Rec(TBM_Loc(e->nextElem));
    }
    Rec((unsigned long long)n);
}

static int caseIdx[2];

static void NewCase(void)
{
    if (caseIdx[side] < 20000)
        caseStart[side][caseIdx[side]] = resLen[side];
    ++caseIdx[side];
    if (side == 0)
        caseCount = caseIdx[0];
}

/* sentinel object pointers: low two bits 01 is what the table reads as
   a hole, everything else is a live object */
static void* Obj(int i)
{
    return (void*)(size_t)(0x100 + 4 * i);
}

/* ------------------------------------------------------------------ */

static void ScenInvalid(const tApi* a)
{
    int v;
    NewCase();
    for (v = 0; v < 600; ++v)
        Rec((unsigned long long)(unsigned int)a->IsInvalid((void*)(size_t)v));
    for (v = 0; v < 64; ++v)
        Rec((unsigned long long)(unsigned int)a->IsInvalid((void*)(size_t)(0x10000 + 7 * v)));
}

/* The last three put two or more live elements in front of the first
   hole, which is the only shape where the compaction's scan cursor is
   left behind the write cursor.  Nothing else here reaches that
   branch: every other pattern either has its first hole at index 0 or
   at index 1, or has no hole at all. */
static const unsigned int holePat[11] = {
    0x00000000u, 0xFFFFFFFFu, 0xAAAAAAAAu, 0x55555555u,
    0x00000001u, 0x80000000u, 0x0F0F0F0Fu, 0xC3000081u,
    0x00000004u, 0x000000C0u, 0x00000124u
};

static void ScenPtable(const tApi* a)
{
    static const unsigned int caps[6] = { 0, 1, 2, 3, 8, 33 };
    int ci, ni, pi, hi;

    for (ci = 0; ci < 6; ++ci)
    for (ni = 0; ni < 5; ++ni)
    for (pi = 0; pi < 11; ++pi)
    for (hi = 0; hi < 4; ++hi)
    {
        static const int ns[5] = { 0, 1, 2, 7, 20 };
        int n = ns[ni];
        int i, holes = 0, cap;
        tableElemStructP* e;

        NewCase();
        a->PInit(&TBL[0], caps[ci], 0.5f);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));

        for (i = 0; i < n; ++i)
            a->PAdd(&TBL[0], Obj(i));
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));

        e = (tableElemStructP*)TBL[0].firstElemPtr;
        for (i = 0; i < n; ++i)
        {
            if ((holePat[pi] >> (i & 31)) & 1)
            {
                e[i].objectPtr = (void*)1;
                ++holes;
            }
        }
        if (n)
        {
            TBL[0].actElemNum = (unsigned int)(n - holes);
            switch (hi)
            {
            case 0: TBL[0].holeNum = 0; break;
            case 1: TBL[0].holeNum = (unsigned int)holes; break;
            case 2: TBL[0].holeNum = (unsigned int)holes + 1; break;
            default: TBL[0].holeNum = 1; break;
            }
        }
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructP)) + 1;

        a->PHoles(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
        RecPElems(&TBL[0], cap);

        for (i = -1; i < n + 1; ++i)
            Rec((unsigned long long)(unsigned int)a->PIndex(&TBL[0], Obj(i)));
        Rec((unsigned long long)(unsigned int)a->PIndex(&TBL[0], (void*)1));
        Rec((unsigned long long)(unsigned int)a->PIndex(&TBL[0], NULL));

        a->PClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
    }
}

static void ScenPtableResize(const tApi* a)
{
    static const unsigned int caps[4] = { 2, 5, 40, 300 };
    static const int grow[6] = { 0, 1, 2, 60, 600, 1200 };
    int ci, gi, i, cap, n;

    for (ci = 0; ci < 4; ++ci)
    for (gi = 0; gi < 6; ++gi)
    {
        NewCase();
        a->PInit(&TBL[0], caps[ci], 0.25f);
        n = (int)caps[ci] - 1;
        for (i = 0; i < n; ++i)
            a->PAdd(&TBL[0], Obj(i));
        /* growth only: see the note at the top of this file */
        a->PResize(&TBL[0], caps[ci] + (unsigned int)grow[gi]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructP)) + 1;
        RecPElems(&TBL[0], cap < 2000 ? cap : 2000);
        a->PClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
    }

    /* A resize that shrinks the table, which is the only way to reach
       the clamp on the used-element count.  On a 64-bit host the
       frozen copy's element count comes out at 2n-1 against the
       rewrite's n (see the note at the top of this file), so it
       copies past the end of the new array; the allocator's slack
       absorbs that, and the two still write the same first n
       elements, which is what is compared. */
    for (ci = 0; ci < 2; ++ci)
    for (gi = 0; gi < 5; ++gi)
    {
        static const unsigned int bigCaps[2] = { 600, 1400 };
        static const unsigned int small[5] = { 0, 2, 3, 50, 300 };
        NewCase();
        a->PInit(&TBL[0], bigCaps[ci], 0.25f);
        n = (int)bigCaps[ci] - 1;
        for (i = 0; i < n; ++i)
            a->PAdd(&TBL[0], Obj(i));
        a->PResize(&TBL[0], small[gi]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructP)) + 1;
        RecPElems(&TBL[0], cap);
        a->PClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
    }


    /* A partly filled table resized to exactly the number of elements
       in use, which is the one way the clamp on the used count is
       reached at equality rather than above it. */
    for (ci = 0; ci < 2; ++ci)
    for (gi = 0; gi < 3; ++gi)
    {
        static const unsigned int partCap[2] = { 600, 1400 };
        static const int partUse[2] = { 300, 500 };
        NewCase();
        a->PInit(&TBL[0], partCap[ci], 0.25f);
        for (i = 0; i < partUse[ci]; ++i)
            a->PAdd(&TBL[0], Obj(i));
        a->PResize(&TBL[0], (unsigned int)(partUse[ci] + gi - 1));
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructP)) + 1;
        RecPElems(&TBL[0], cap);
        a->PClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
    }

    /* the automatic resize on a full table */
    NewCase();
    a->PInit(&TBL[0], 4, 0.25f);
    for (i = 0; i < 700; ++i)
    {
        a->PAdd(&TBL[0], Obj(i));
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
    }
    cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                (int)sizeof(tableElemStructP)) + 1;
    RecPElems(&TBL[0], cap);
    a->PClose(&TBL[0]);
}

static void ScenPFtable(const tApi* a)
{
    static const unsigned int caps[6] = { 0, 1, 2, 3, 8, 33 };
    static const int ns[5] = { 0, 1, 2, 7, 20 };
    int ci, ni, pi, hi;

    for (ci = 0; ci < 6; ++ci)
    for (ni = 0; ni < 5; ++ni)
    for (pi = 0; pi < 11; ++pi)
    for (hi = 0; hi < 4; ++hi)
    {
        int n = ns[ni];
        int i, holes = 0, cap;
        tableElemStructPF* e;

        NewCase();
        a->FInit(&TBL[0], caps[ci], 0.5f);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));

        for (i = 0; i < n; ++i)
            a->FAdd(&TBL[0], Obj(i), 0x1000 + i);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));

        e = (tableElemStructPF*)TBL[0].firstElemPtr;
        for (i = 0; i < n; ++i)
        {
            if ((holePat[pi] >> (i & 31)) & 1)
            {
                e[i].objectPtr = (void*)1;
                ++holes;
            }
        }
        if (n)
        {
            TBL[0].actElemNum = (unsigned int)(n - holes);
            switch (hi)
            {
            case 0: TBL[0].holeNum = 0; break;
            case 1: TBL[0].holeNum = (unsigned int)holes; break;
            case 2: TBL[0].holeNum = (unsigned int)holes + 1; break;
            default: TBL[0].holeNum = 1; break;
            }
        }
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructPF)) + 1;

        a->FHoles(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
        RecFElems(&TBL[0], cap);

        for (i = -1; i < n + 1; ++i)
        {
            Rec((unsigned long long)(unsigned int)a->FIndex(&TBL[0], Obj(i)));
            Rec(TBM_Loc(a->FGet(&TBL[0], Obj(i))));
        }
        Rec((unsigned long long)(unsigned int)a->FIndex(&TBL[0], (void*)1));
        Rec(TBM_Loc(a->FGet(&TBL[0], (void*)1)));

        a->FClear(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
        a->FClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
    }
}

static void ScenPFremove(const tApi* a)
{
    static const int ns[4] = { 1, 2, 5, 12 };
    int ni, ki, mi;

    for (ni = 0; ni < 4; ++ni)
    for (ki = 0; ki < 13; ++ki)
    for (mi = 0; mi < 3; ++mi)
    {
        int n = ns[ni];
        int i, cap;
        tableElemStructPF* e;

        if (ki > n)
            continue;

        NewCase();
        a->FInit(&TBL[0], 16, 0.5f);
        for (i = 0; i < n; ++i)
            a->FAdd(&TBL[0], Obj(i), 0x2000 + i);
        e = (tableElemStructPF*)TBL[0].firstElemPtr;
        if (mi == 1 && n > 1)
        {
            e[n / 2].objectPtr = (void*)1;         /* an existing hole */
            TBL[0].holeNum = 1;
            TBL[0].actElemNum = (unsigned int)n - 1;
        }
        if (mi == 2 && n > 2)
        {
            e[n - 1].objectPtr = NULL;             /* a null slot at the top */
            TBL[0].holeNum = 1;
            TBL[0].actElemNum = (unsigned int)n - 1;
        }
        cap = 16;

        if (ki < n)
            a->FRemoveElem(&TBL[0], &e[ki]);
        else
            a->FRemovePtr(&TBL[0], Obj(ki));
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
        RecFElems(&TBL[0], cap);

        for (i = 0; i < n; ++i)
        {
            a->FRemovePtr(&TBL[0], Obj(i));
            RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
            RecFElems(&TBL[0], cap);
        }
        a->FClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
    }

    /* the automatic compaction and resize on a full PF table */
    NewCase();
    a->FInit(&TBL[0], 4, 0.25f);
    for (ni = 0; ni < 500; ++ni)
    {
        a->FAdd(&TBL[0], Obj(ni), ni);
        if ((ni % 7) == 3)
        {
            tableElemStructPF* f = (tableElemStructPF*)TBL[0].firstElemPtr;
            a->FRemoveElem(&TBL[0], &f[ni / 3]);
        }
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
    }
    RecFElems(&TBL[0], 500);
    a->FClose(&TBL[0]);
    RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));

    /* the raw add, which skips the full test altogether */
    NewCase();
    a->FInit(&TBL[0], 8, 0.5f);
    for (ni = 0; ni < 6; ++ni)
    {
        a->FAddRaw(&TBL[0], Obj(ni), ni * 3);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
    }
    RecFElems(&TBL[0], 8);
    a->FClose(&TBL[0]);
}

static void ScenPFresize(const tApi* a)
{
    static const unsigned int caps[4] = { 2, 5, 40, 300 };
    static const int grow[6] = { 0, 1, 2, 60, 600, 1200 };
    int ci, gi, i, cap, n;

    for (ci = 0; ci < 4; ++ci)
    for (gi = 0; gi < 6; ++gi)
    {
        NewCase();
        a->FInit(&TBL[0], caps[ci], 0.25f);
        n = (int)caps[ci] - 1;
        for (i = 0; i < n; ++i)
            a->FAdd(&TBL[0], Obj(i), 0x30 + i);
        a->FResize(&TBL[0], caps[ci] + (unsigned int)grow[gi]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructPF)) + 1;
        RecFElems(&TBL[0], cap < 2000 ? cap : 2000);
        a->FClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
    }

    /* A partly filled table resized to exactly the number of elements
       in use, which is the one way the clamp on the used count is
       reached at equality rather than above it. */
    for (ci = 0; ci < 2; ++ci)
    for (gi = 0; gi < 3; ++gi)
    {
        static const unsigned int partCap[2] = { 600, 1400 };
        static const int partUse[2] = { 300, 500 };
        NewCase();
        a->FInit(&TBL[0], partCap[ci], 0.25f);
        for (i = 0; i < partUse[ci]; ++i)
            a->FAdd(&TBL[0], Obj(i), 0x40 + i);
        a->FResize(&TBL[0], (unsigned int)(partUse[ci] + gi - 1));
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
        cap = (int)(((char*)TBL[0].lastElemPtr - (char*)TBL[0].firstElemPtr) /
                    (int)sizeof(tableElemStructPF)) + 1;
        RecFElems(&TBL[0], cap);
        a->FClose(&TBL[0]);
        RecDesc(&TBL[0], (int)sizeof(tableElemStructPF));
    }
}

/* The manager: which tables are registered, and which of them the
   per-frame call decides to compact. */
static void ScenManager(const tApi* a)
{
    static const float ratios[4] = { 1.0f, 0.0f, 0.5f, 2.0f };
    int r0, r1, holes, acts;

    for (r0 = 0; r0 < 4; ++r0)
    for (r1 = 0; r1 < 4; ++r1)
    for (holes = 0; holes < 5; ++holes)
    for (acts = 0; acts < 5; ++acts)
    {
        int i;
        NewCase();
        a->ModInit();
        RecList(a);

        a->PInit(&TBL[0], 12, ratios[r0]);
        a->FInit(&TBL[1], 12, ratios[r1]);
        a->PInit(&TBL[2], 12, ratios[r0]);
        RecList(a);

        for (i = 0; i < 9; ++i)
        {
            a->PAdd(&TBL[0], Obj(i));
            a->FAdd(&TBL[1], Obj(i), i);
            a->PAdd(&TBL[2], Obj(i));
        }
        for (i = 0; i < holes; ++i)
        {
            ((tableElemStructP*)TBL[0].firstElemPtr)[2 * i].objectPtr = (void*)1;
            ((tableElemStructPF*)TBL[1].firstElemPtr)[2 * i].objectPtr = (void*)1;
            ((tableElemStructP*)TBL[2].firstElemPtr)[2 * i].objectPtr = NULL;
        }
        TBL[0].holeNum = (unsigned int)holes;
        TBL[1].holeNum = (unsigned int)holes;
        TBL[2].holeNum = (unsigned int)holes;
        TBL[0].actElemNum = (unsigned int)acts;
        TBL[1].actElemNum = (unsigned int)acts;
        TBL[2].actElemNum = (unsigned int)acts;

        a->MgrCall();
        RecDesc(&TBL[0], (int)sizeof(tableElemStructP));
        RecPElems(&TBL[0], 12);
        RecDesc(&TBL[1], (int)sizeof(tableElemStructPF));
        RecFElems(&TBL[1], 12);
        RecDesc(&TBL[2], (int)sizeof(tableElemStructP));
        RecPElems(&TBL[2], 12);
        RecList(a);

        /* unregister out of order, then close the module over whatever
           is left registered */
        a->MgrUnregister(&TBL[1]);
        RecList(a);
        a->PClose(&TBL[2]);
        RecList(a);
        a->MgrUnregister(&TBL[1]);
        RecList(a);
        a->FClose(&TBL[1]);
        RecList(a);
        a->ModClose();
        RecList(a);
        a->PClose(&TBL[0]);
        RecList(a);
    }
}

/* The doubly linked list on its own, including the element the list
   does not own. */
static void ScenList(const tApi* a)
{
    int n, k;

    for (n = 0; n <= 5; ++n)
    for (k = 0; k < 6; ++k)
    {
        tabDlistStruct list;
        tabDlistElemStruct* e;
        tabDlistElemStruct* kth = NULL;
        int i;

        if (k > n)
            continue;

        NewCase();
        a->ListCreate(&list);
        Rec((unsigned long long)(unsigned int)list.elemNum);
        for (i = 0; i < n; ++i)
            a->ListAdd(&list, &TBL[i % NT]);

        i = 0;
        for (e = list.firstElem; e; e = e->nextElem)
        {
            if (i == k)
                kth = e;
            Rec(TBM_Loc(e));
            Rec(TBM_Loc(e->prevElem));
            Rec(TBM_Loc(e->nextElem));
            ++i;
        }
        Rec((unsigned long long)(unsigned int)list.elemNum);

        if (kth)
        {
            a->ListRemoveElem(&list, kth);
            Rec((unsigned long long)(unsigned int)list.elemNum);
            Rec(TBM_Loc(list.firstElem));
            Rec(TBM_Loc(list.lastElem));
            for (e = list.firstElem; e; e = e->nextElem)
            {
                Rec(TBM_Loc(e));
                Rec(TBM_Loc(e->prevElem));
                Rec(TBM_Loc(e->nextElem));
            }
        }
        a->ListRemove(&list);
        Rec((unsigned long long)(unsigned int)list.elemNum);
        Rec(TBM_Loc(list.firstElem));

        /* an element made by hand, so CreateElem is driven on purpose */
        e = NULL;
        a->ElemCreate(&e);
        Rec(TBM_Loc(e));
        Rec(TBM_Loc(e->nextElem));
        Rec(TBM_Loc(e->prevElem));
        Rec(TBM_Loc(e->tableElemPtr));
    }
}

static void RunAll(const tApi* a)
{
    ScenInvalid(a);
    ScenList(a);
    ScenPtable(a);
    ScenPtableResize(a);
    ScenPFtable(a);
    ScenPFremove(a);
    ScenPFresize(a);
    ScenManager(a);
}

int main(void)
{
    int i, diff = 0, firstBad = -1, logDiff = 0;
    int n;

    for (side = 0; side < 2; ++side)
    {
        TBM_side = side;
        TBM_Reset(side);
        resLen[side] = 0;
        memset(TBL, 0, sizeof(TBL));
        caseIdx[side] = 0;
        memset(side ? apiNew.list : apiBase.list, 0, sizeof(tabDlistStruct));
        *(side ? apiNew.inited : apiBase.inited) = 0;
        RunAll(side ? &apiNew : &apiBase);
    }

    if (resOverflow || TBM_logOverflow)
    {
        printf("tables: FAIL -- log overflow\n");
        return 1;
    }
    if (resLen[0] != resLen[1])
    {
        printf("tables: FAIL -- %d values against %d\n", resLen[0], resLen[1]);
        return 1;
    }
    for (i = 0; i < resLen[0]; ++i)
    {
        if (res[0][i] != res[1][i])
        {
            ++diff;
            if (firstBad < 0)
                firstBad = i;
        }
    }
    if (TBM_logLen[0] != TBM_logLen[1])
    {
        printf("tables: FAIL -- %d allocator calls against %d\n",
               TBM_logLen[0], TBM_logLen[1]);
        return 1;
    }
    for (i = 0; i < TBM_logLen[0]; ++i)
    {
        if (strcmp(TBM_log[0][i].fn, TBM_log[1][i].fn) ||
            TBM_log[0][i].arg[0] != TBM_log[1][i].arg[0] ||
            TBM_log[0][i].arg[1] != TBM_log[1][i].arg[1] ||
            TBM_log[0][i].arg[2] != TBM_log[1][i].arg[2])
        {
            if (!logDiff)
                printf("  first call difference at %d: %s(%llu,%llu) against %s(%llu,%llu)\n",
                       i, TBM_log[0][i].fn, TBM_log[0][i].arg[0], TBM_log[0][i].arg[1],
                       TBM_log[1][i].fn, TBM_log[1][i].arg[0], TBM_log[1][i].arg[1]);
            ++logDiff;
        }
    }

    n = 0;
    if (firstBad >= 0)
    {
        for (i = 0; i < caseCount; ++i)
            if (caseStart[0][i] <= firstBad)
                n = i;
        printf("  first value difference at %d (case %d): %llu against %llu\n",
               firstBad, n, res[0][firstBad], res[1][firstBad]);
    }

    printf("tables: %d cases, %d values, %d allocator calls, "
           "%d values differing, %d calls differing\n",
           caseCount, resLen[0], TBM_logLen[0], diff, logDiff);
    return (diff || logDiff) ? 1 : 0;
}
