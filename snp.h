#pragma once
#include "gameobject.h"
#include "common.h"

typedef struct sSnpObjectsToRemoveQueue
{
    uint16_t destPos;
    uint16_t num;
    struct sGameObjectMain* gaos[1024];
    uint16_t queueNum;
    uint16_t elemIdxes[1025];
} tSnpObjectsToRemoveQueue;

typedef struct sSnpAxisElem
{
    struct sGameObjectMain* gao;
    int16_t gaoSnPIndex;
    uint8_t flags;
    uint8_t gap;
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
    uint8_t _pad[2];
    struct sSnpObjectsToRemoveQueue* snpObjectsRemoveQueue;
    struct sSnpAxis* axisPtr[3];
    struct sGameObjectMain* gaosPtr[1024];
} tSnp;

void INT_SnP_Alloc(struct sWorld* world);
void INT_SnP_AddObject(struct sGameObjectMain* gao, struct sSnp* snp);
void INT_SnP_InsertionSort(struct sSnp* snp, int axisMode);
void INT_SnP_DetachObject(struct sGameObjectMain* gao, struct sWorld* world);
void INT_SnP_AttachObject(struct sGameObjectMain* gao, struct sWorld* world);
void INT_SnP_DesAlloc(struct sWorld* world);
void INT_SnP_Refresh(struct sSnp* snp, tableDescStruct* table);
void INT_SnP_Reinit(struct sWorld* world);
int INT_SnP_FullGetFlag(int* flagArr, int snpIndexA, int snpIndexB, int count, int* bitOut, int* wordOut);
int INT_SnP_GetFlag(int* flagArr, char bit, int word);