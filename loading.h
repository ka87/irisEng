#pragma once
#include "common.h"
#include "math.h"
#include "modifiers/spritegenmodifier.h"
#include "ai.h"
#include "basic_array.h"
#include "handle.h"

/* What the address store answers for a position it has nothing
   against. It is also what several callers compare the resolved
   pointer against, so it has to sign-extend the way it always did. */
#define LOA_ADDRESS_NOT_FOUND (-1)

/* What the store holds, as a pointer. The store's slots are four bytes
   wide, so on a build with wider pointers they hold a handle and this
   is what turns one back into an address; where pointers already fit
   it is the cast the engine always wrote.

   Not-found is passed through as it is, because the callers test the
   result against (T*)-1 rather than asking the store first. */
static __inline void* LOA_p_FromStored(int stored)
{
    if (stored == LOA_ADDRESS_NOT_FOUND)
        return (void*)(intptr_t)stored;
    return HDL_p_Pointer((uint32_t)stored);
}


typedef struct loaStackElem
{
    int key;
    void** retValPtr;
    void* (* funcPtr)(uint32_t address);
    uint8_t flags;
    uint8_t _pad[3];
} loaStackElem;

unsigned int LOA_ul_BinKey;
int LOA_gb_SpeedMode;
int LOA_gi_LastRef;
int LOA_gi_CurRef;
loaStackElem* LOA_gpst_CurRef;
loaStackElem LOA_gast_Stack[5000];
loaStackElem* LOA_lastElem;
BASStruct LOA_gst_Array[256];
BASStruct LOA_gst_InvArray;
BASStruct LOA_gst_SpecialArray;
int dword_98F20C;
int dword_98F210;

int LOA_sb_ActivateSoundLoadAfterBin;

extern int LOA_gb_CompressBin;
extern int ENG_PlatformID;

unsigned long LOA_ul_FileTypeSize_20; // geometry vertexnum;
unsigned long LOA_ul_FileTypeSize_21; // geometry normalnum;
unsigned long LOA_ul_FileTypeSize_22; // geometry vertexcolornum;
unsigned long LOA_ul_FileTypeSize_23; // geometry uvnum;
unsigned long LOA_ul_FileTypeSize_24; // gerometry triangles
unsigned long LOA_ul_FileTypeSize_25; // geometry mrm
unsigned long LOA_ul_FileTypeSize_26; // gerometry skn
unsigned long LOA_ul_FileTypeSize_27; // vertexcolors
unsigned long LOA_ul_FileTypeSize_28; // geometry trianglechunknum
unsigned long LOA_ul_FileTypeSize_29;
unsigned long LOA_ul_FileTypeSize_30; // events (track metadata)
unsigned long LOA_ul_FileTypeSize_31; // events (track data)
unsigned long LOA_ul_FileTypeSize_32; // events
unsigned long LOA_ul_FileTypeSize_33; // events
unsigned long LOA_ul_FileTypeSize_34; // gao
unsigned long LOA_ul_FileTypeSize_35; // AI vars
unsigned long LOA_ul_FileTypeSize_36; // AI
unsigned long LOA_ul_FileTypeSize_37;
unsigned long LOA_ul_FileTypeSize_38;
unsigned long LOA_ul_FileTypeSize_39;

void LOA_InitModule(void);
void LOA_CloseModule(void);
char LOA_ReadChar(char** offset, char* retVal, char readMode);
uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode);
int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode);
float LOA_ReadFloat(char** offset, float* out, char readMode);
char** LOA_ReadCharArray(char** offset, char* retVal, unsigned int len, char readMode);
void LOA_ReadString(char** offset, char* retVal, int len, char readMode);
void LOA_ReadVector(char** offset, Vec3* retVal, int readMode);
void LOA_ReadMatrix(char** offset, Mtx4x4* retVal, int readMode);
void LOA_ReadSpriteMapper(char** offset, tSpgSpriteMapperStruct* retVal, char readMode);
char* LOA_FetchBuffer(int size);
char* LOA_FetchFile(int* sizeRet);
int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode);
void LOA_DeleteAddress(int pos);
/* retVal is where to put the loaded thing: a pointer to the caller's
   own pointer, of whatever type that is. It is void* and not void**
   because C converts any object pointer to void* and does not convert
   a T** to a void**, so every caller of this would otherwise be
   passing an incompatible type. */
void LOA_MakeFileRef(int key, void* retVal, void* (*func)(uint32_t), char flags);

/* A file key parked in a pointer field until the resolve pass fills it
   in.
 
   Most of the loader reads a key into a local and passes that to
   LOA_MakeFileRef, so the field only ever holds the resolved pointer.
   A few places cannot: the key is read in one pass and resolved in
   another, or in another function, and the field is the only storage
   that travels with the thing it belongs to. There the field holds a
   key for a while, and these two say which of the two it is holding
   rather than leaving it to a cast that also happens to narrow it. */
static __inline uintptr_t LOA_ref_FromKey(int key)
{
    /* Sign extended, which is what the casts that used to do this did
       and what the frozen copies in tests/ still do. It matters for one
       value: a key of -1 is "no reference", and several places test the
       field for it by comparing against a pointer of every bit set.
       Zero extending would stop those matching on a build with wider
       pointers while leaving a 32-bit build alone -- and the actions
       differential says so, 72 of its 864 cases. */
    return (uintptr_t)(intptr_t)key;
}

static __inline int LOA_i_KeyFromRef(const void* ref)
{
    return (int)(uintptr_t)ref;
}
int LOA_ul_SearchAddress(int pos);
void LOA_ReadAINode(char** offset, tAINode* retVal, int readMode);
BOOL LOA_IsSpecial(unsigned int key);
int LOA_ul_GetCurrentKey();
void LOA_Resolve();
void LOA_BeginSpeedMode(int key);
void LOA_EndSpeedMode();
BOOL isSpeedModeAndBinFile();
void LOA_AddPosAddress(unsigned int pos, unsigned int address);
void LOA_ReadAILocal(char** offset, tAILocal* retVal, int readMode);
void LOA_LoadSpecialArray();
void LOA_KillSpecialArray();
void LOA_ReadCompressedQuaternion(char** offset, CompressedQuat* retVal, char readMode);
void LOA_ReadUltraCompressedQuaternion(char** offset, UltraCompressedQuat* retVal, char readMode);
void LOA_ReadQuaternion(char** offset, Quat4* retVal, int readMode);

