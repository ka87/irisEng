#include "common.h"
#include "memory.h"
#include "basic_array.h"

int BAS_bsortmode = 1;

int ERR_InitModule()
{
    return remove("ERRasser.log");
}

void BAS_InitModule()
{
    ERR_InitModule();
}

void BAS_CloseModule()
{    
}

int BAS_bsearch(int key, BASStruct *bas)
{
  int actElemNum;
  BASKeyValStruct *actKeyValPtr;
  BASKeyValStruct *prevElem;
  int middleElemIdx;
  int isOdd;
  int finalMiddleElemIdx;
  int actKey;
  BASKeyValStruct *actKeyValElem;

  actElemNum = bas->actElemNum;
  actKeyValPtr = bas->keyValPtr;
  prevElem = &bas->keyValPtr[actElemNum - 1];
  sgpst_Prev = NULL;

  if (bas->keyValPtr > prevElem)
    return -1;

  while (1)
  {
    middleElemIdx = actElemNum / 2;
    if ( !(actElemNum / 2) )
      break;
    isOdd = actElemNum & 1;
    finalMiddleElemIdx = actElemNum / 2;
    if ( !isOdd )
      finalMiddleElemIdx = middleElemIdx - 1;
    actKey = actKeyValPtr[finalMiddleElemIdx].key;
    actKeyValElem = &actKeyValPtr[finalMiddleElemIdx];
    if ( key >= actKey )
    {
      if ( key <= actKey )
      {
        sgpst_Prev = actKeyValElem - 1;
        return actKeyValElem->val;
      }
      sgpst_Prev = actKeyValElem;
      actKeyValPtr = actKeyValElem + 1;
      actElemNum = middleElemIdx;
      if (actKeyValPtr > prevElem)
          return -1;
      continue;
    }
    prevElem = actKeyValElem - 1;
    if (isOdd) {
        actElemNum = middleElemIdx;
        if (actKeyValPtr > prevElem)
            return -1;
        continue;
    }
    actElemNum = middleElemIdx - 1;
    if ( actKeyValPtr > prevElem )
      return -1;
  }
  if ( !actElemNum )
    return -1;
  if ( key > actKeyValPtr->key )
  {
    sgpst_Prev = actKeyValPtr;
    return -1;
  }
  if ( key != actKeyValPtr->key )
    return -1;
  return actKeyValPtr->val;
}

void BAS_binsert(unsigned int key, int val, BASStruct* bas)
{
    int originalActElemNum;
    int newTotalNum;
    BASKeyValStruct* currKeyValPtr;
    int actElemNum;
    BASKeyValStruct* tmpCurrKEyValPtr;

    originalActElemNum = bas->actElemNum;
    if (bas->actElemNum == bas->maxElemNum)
    {
        newTotalNum = bas->elemNumToAddWhenFull + bas->maxElemNum;
        bas->maxElemNum = newTotalNum;
        if (bas->actElemNum)
            bas->keyValPtr = (BASKeyValStruct*)MEM_p_Realloc(bas->keyValPtr, sizeof(BASKeyValStruct) * newTotalNum);
        else
            bas->keyValPtr = (BASKeyValStruct*)MEM_p_Alloc(sizeof(BASKeyValStruct) * newTotalNum);
    }
    currKeyValPtr = bas->keyValPtr;
    if (BAS_bsortmode)
    {
        BAS_bsearch(key, bas);
        actElemNum = bas->actElemNum;
        if (actElemNum)
        {
            tmpCurrKEyValPtr = currKeyValPtr;
            if (sgpst_Prev)
                tmpCurrKEyValPtr = sgpst_Prev + 1;
            if (tmpCurrKEyValPtr != &currKeyValPtr[actElemNum] && tmpCurrKEyValPtr->key == key)
            {
                tmpCurrKEyValPtr->val = val;
                return;
            }
        }
        if (sgpst_Prev)
        {
            if (sgpst_Prev == &currKeyValPtr[originalActElemNum - 1])
            {
                currKeyValPtr += originalActElemNum;
            }
            else
            {
                memcpy(&sgpst_Prev[2], &sgpst_Prev[1], sizeof(BASKeyValStruct) * (((int)&currKeyValPtr[-1] + sizeof(BASKeyValStruct) * originalActElemNum - (int)sgpst_Prev) >> 3));
                currKeyValPtr = sgpst_Prev + 1;
            }
        }
        else
        {
            memcpy(&currKeyValPtr[1], currKeyValPtr, sizeof(BASKeyValStruct) * originalActElemNum);
        }
    }
    else
    {
        currKeyValPtr += bas->actElemNum;
    }
    currKeyValPtr->key = key;
    currKeyValPtr->val = val;
    sgpst_Prev = currKeyValPtr;
    bas->actElemNum++;
}

void BAS_bdelete(int key, BASStruct* bas)
{
    int actElemNum;
    BASKeyValStruct* keyValPtr;

    actElemNum = bas->actElemNum;
    if (actElemNum)
    {
        keyValPtr = bas->keyValPtr;
        if (BAS_bsearch(key, bas) != -1)
        {
            if (sgpst_Prev)
            {
                if (sgpst_Prev != &keyValPtr[actElemNum - 2])
                {
                    memcpy(&sgpst_Prev[1], &sgpst_Prev[2], sizeof(BASKeyValStruct) * (((int)&keyValPtr[-1] + sizeof(BASKeyValStruct) * actElemNum - (int)sgpst_Prev) >> 3));
                    --bas->actElemNum;
                    return;
                }
            }
            else
            {
                memcpy(keyValPtr, &keyValPtr[1], sizeof(BASKeyValStruct) * actElemNum - 8);
            }
            --bas->actElemNum;
        }
    }
}

void BAS_bdeletegetval(int key, BASStruct* bas, int* val)
{
    int actElemNum;
    BASKeyValStruct* keyValPtr;
    int retVal;

    *val = 0;
    actElemNum = bas->actElemNum;
    if (actElemNum)
    {
        keyValPtr = bas->keyValPtr;
        retVal = BAS_bsearch(key, bas);
        if (retVal != -1)
        {
            *val = retVal;
            if (sgpst_Prev)
            {
                if (sgpst_Prev != &keyValPtr[actElemNum - 2])
                {
                    memcpy(&sgpst_Prev[1], &sgpst_Prev[2], sizeof(BASKeyValStruct) * (((int)&keyValPtr[-1] + sizeof(BASKeyValStruct) * actElemNum - (int)sgpst_Prev) >> 3));
                    --bas->actElemNum;
                    return;
                }
            }
            else
            {
                memcpy(keyValPtr, &keyValPtr[1], sizeof(BASKeyValStruct) * actElemNum - sizeof(BASKeyValStruct));
            }
            --bas->actElemNum;
        }
    }
}

void BAS_binit(BASStruct* a1, int elemNum)
{
    a1->keyValPtr = 0;
    a1->actElemNum = 0;
    a1->maxElemNum = elemNum;
    a1->elemNumToAddWhenFull = elemNum;
    if (elemNum)
        a1->keyValPtr = (BASKeyValStruct*)MEM_p_Alloc(sizeof(BASKeyValStruct) * elemNum);
}

void BAS_bfree(BASStruct* a1)
{
    if (a1->keyValPtr)
        MEM_Free(a1->keyValPtr);
    a1->keyValPtr = NULL;
    a1->maxElemNum = 0;
    a1->actElemNum = 0;
}

int BAS_barray_callback(const void* a1, const void* a2)
{
    if (*(int *)a2 <= *(int *)a1)
        return *(int*)a2 < *(int*)a1;
    else
        return -1;
}

void BAS_bsort(BASStruct* a1)
{
    qsort((void *)a1->keyValPtr, a1->actElemNum, sizeof(BASKeyValStruct), BAS_barray_callback);
}

