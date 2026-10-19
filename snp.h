#pragma once
#include "gameobject.h"
#include "common.h"

typedef struct sSnpObjectsToRemoveQueue
{
    unsigned __int16 destPos;
    unsigned __int16 num;
    struct sGameObjectMain* gaos[1024];
    unsigned __int16 queueNum;
    unsigned __int16 elemIdxes[1025];
} tSnpObjectsToRemoveQueue;

typedef struct sSnpAxisElem
{
    struct sGameObjectMain* gao;
    __int16 gaoSnPIndex;
    byte flags;
    byte gap;
    float* ptrToBVAxisFloat;
    float posPlusBVSetIn40DB10;
} tSnpAxisElem;

typedef struct sSnpAxis
{
    struct sSnpAxisElem* elems;
    int flagsArray[16368];
} tSnpAxis;

typedef struct sSnp
{
    uint16_t currElemIdx;
    byte _pad[2];
    struct sSnpObjectsToRemoveQueue* snpObjectsRemoveQueue;
    struct sSnpAxis* axisPtr[3];
    struct sGameObjectMain* gaosPtr[1024];
} tSnp;

void INT_SnP_Alloc(struct sWorld* a1);
void INT_SnP_AddObject(struct sGameObjectMain* a1, struct sSnp* a2);
void INT_SnP_InsertionSort(struct sSnp* a1, int a2);
void INT_SnP_DetachObject(struct sGameObjectMain* a1, struct sWorld* a2);
void INT_SnP_AttachObject(struct sGameObjectMain* a1, struct sWorld* a2);
void INT_SnP_DesAlloc(struct sWorld* a1);
void INT_SnP_Refresh(struct sSnp* a1, tableDescStruct* a2);
void INT_SnP_Reinit(struct sWorld* a1);
int INT_SnP_FullGetFlag(int* a1, int a2, int a3, int a4, int* a5, int* a6);
int INT_SnP_GetFlag(int* a1, char offset, int idx);