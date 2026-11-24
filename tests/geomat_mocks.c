/* Just enough of the engine for geomaterial.c to load a material.
 *
 * The allocator is the C library's, deliberately: an allocation this
 * file asks for too small is the whole point of the --asan mode beside
 * this, and the engine's own arena would absorb it.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../loading.h"
#include "../textures.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geomaterial.h"

#include "geomat_mocks.h"

/* ------------------------------------------------------------ the heap */

int GMMOCK_liveBlocks;

void* MEM_p_Alloc(unsigned int size)
{
    ++GMMOCK_liveBlocks;
    return malloc(size ? size : 1);
}

void MEM_Free(void* ptr)
{
    if (ptr)
        --GMMOCK_liveBlocks;
    free(ptr);
}

/* ------------------------------------------------------- the read head */

/* The driver lays out a buffer and the loader walks it. Each read
   records its width, so the driver can assert that the loader consumed
   exactly what it wrote and in the order it wrote it. */

char* GMMOCK_readHead;
int GMMOCK_reads[GMMOCK_MAXREADS];
int GMMOCK_readNum;

static void note(int width)
{
    if (GMMOCK_readNum < GMMOCK_MAXREADS)
        GMMOCK_reads[GMMOCK_readNum] = width;
    ++GMMOCK_readNum;
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int32_t v;

    (void)retVal;
    (void)readMode;
    memcpy(&v, *offset, 4);
    *offset += 4;
    note(4);
    return v;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    uint16_t v;

    (void)retVal;
    (void)readMode;
    memcpy(&v, *offset, 2);
    *offset += 2;
    note(2);
    return v;
}

float LOA_ReadFloat(char** offset, float* out, char readMode)
{
    float v;

    (void)out;
    (void)readMode;
    memcpy(&v, *offset, 4);
    *offset += 4;
    note(4);
    return v;
}

/* --------------------------------------------------- the texture list */

/* Every key the loader looks up, in order, so the driver can say which
   field it read the key out of. The loader's two fields are crossed --
   the short holds the follow-on marker and the long holds the key --
   and nothing but this would notice them being read the other way
   round, since both are small numbers. */

int GMMOCK_asked[GMMOCK_MAXREADS];
int GMMOCK_askedNum;
int GMMOCK_addedNum;

static void asked(int key)
{
    if (GMMOCK_askedNum < GMMOCK_MAXREADS)
        GMMOCK_asked[GMMOCK_askedNum] = key;
    ++GMMOCK_askedNum;
}

/* Keys below GMMOCK_KNOWN are already in the list, at the index the key
   itself names; anything else is added. */
struct sEngTexListElemStruct* TEX_pst_List_FindTexture(
    struct sEngTexListStruct* list, int key)
{
    asked(key);
    if (key < 0 || key >= GMMOCK_KNOWN)
        return NULL;
    return &list->texElems[key];
}

int16_t TEX_w_List_AddTexture(tEngTexListStruct* list, int key)
{
    (void)list;
    (void)key;
    return (int16_t)(GMMOCK_KNOWN + GMMOCK_addedNum++);
}

/* ------------------------------------------------- the rest, unused here */

tGroIntf GRO_gast_Interface[12];

void GRO_Struct_Init(tGroGeneric* gro, int type)
{
    gro->intf = &GRO_gast_Interface[type];
    gro->usageCount = 0;
}

struct sGroGeneric* GEO_ul_Load_ObjectCallback(uint32_t Offset)
{
    (void)Offset;
    return NULL;
}

int GMMOCK_fileRefNum;
int GMMOCK_fileRefKeys[GMMOCK_MAXREADS];

void LOA_MakeFileRef(int key, void* retVal, void* (*func)(uint32_t), char flags)
{
    (void)func;
    (void)flags;
    if (GMMOCK_fileRefNum < GMMOCK_MAXREADS)
        GMMOCK_fileRefKeys[GMMOCK_fileRefNum] = key;
    ++GMMOCK_fileRefNum;
    /* The real one resolves later; here the slot keeps the key, which
       is what the caller wrote into it before this was called. */
    *(void**)retVal = NULL;
}

void LOA_DeleteAddress(int pos)
{
    (void)pos;
}

uint32_t HDL_ul_Handle(void* p)
{
    return (uint32_t)(uintptr_t)p;
}

void GAO_ModifierShadow_DetectUsedTextures(void* gao, char* used)
{
    (void)gao;
    (void)used;
}

void GMMOCK_Reset(void)
{
    GMMOCK_readNum = 0;
    GMMOCK_askedNum = 0;
    GMMOCK_addedNum = 0;
    GMMOCK_fileRefNum = 0;
    GMMOCK_liveBlocks = 0;
}
