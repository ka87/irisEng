#pragma once
#include "common.h"
#include "math.h"
#include "modifiers/spritegenmodifier.h"
#include "ai.h"
#include "basic_array.h"

typedef struct loaStackElem
{
    int key;
    void** retValPtr;
    void* (* funcPtr)(uint32_t address);
    byte flags;
    byte _pad[3];
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
float LOA_ReadFloat(char** offset, float* a2, char readMode);
char** LOA_ReadCharArray(char** offset, char* retVal, unsigned int len, char readMode);
void LOA_ReadString(char** offset, char* retVal, int len, char readMode);
void LOA_ReadVector(char** offset, Vec3* retVal, int readMode);
void LOA_ReadMatrix(char** offset, Mtx4x4* retVal, int readMode);
void LOA_ReadSpriteMapper(char** offset, tSpgSpriteMapperStruct* retVal, char readMode);
char* LOA_FetchBuffer(int size);
char* LOA_FetchFile(int* a1);
int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode);
void LOA_DeleteAddress(int a1);
void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags);
int LOA_ul_SearchAddress(int a1);
void LOA_ReadAINode(char** offset, tAINode* retVal, int readMode);
BOOL LOA_IsSpecial(unsigned int a1);
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

