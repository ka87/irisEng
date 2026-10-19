#pragma once
#include "common.h"
#include "basic_array.h"

typedef struct BIGFatTableElem
{
    int fatNum;
    int i2;
    int posFat;
    int nextPosFat;
    int i5;
    int i6;
} BIGFatTableElem;

typedef struct BIGFileTableElem
{
    int key;
    int pos;
} BIGFileTableElem;

typedef struct BIGHeaderStruct {
    char ac_def[4];
    int unk2;
    int keyNum;
    int unk4;
    int unk5;
    int unk6;
    int unk7;
    int unk8;
    int unk9;
    int fatTableNum;
    int startKey;
} BIGHeaderStruct;

typedef struct BIGGSTStruct
{
    BIGHeaderStruct hdr;
    char filePath[260];
    FILE* CLibFileHandle;
    BASStruct bfBigKeyToPos;
    BIGFileTableElem* bfFileTable;
    BIGFatTableElem* bfFatTable;
} BIGGSTStruct;

typedef struct BIGCompressedBlockStruct
{
    char* src;
    char* dest;
    int totalSize;
    int i4;
} BIGCompressedBlockStruct;


BIGGSTStruct BIG_gst;
int BIG_gi_ReadMode;
int BIG_gb_CanCache;

char* BIG_gp_ReadBuffer;
int BIG_gi_ReadSeek;
int BIG_si_DecompressedSize;
int BIG_gi_ReadSize;
char* BIG_sp_CompressedNextBlock;
int BIG_si_CompressedSize;
char* BIG_gp_CompressedBuffer;
int BIG_gi_RealSeek;
int BIG_si_BinFileSize;
char* BIG_gp_CompressedBufferPtrBackup;
int BIG_si_BinFileReadSize;
int BIG_gul_ReadPos;
int BIG_ul_CachePos;
size_t gai_CurSize[24];
char* gap_FirstPointer[24];
int gai_CurBinFileSize[20];
unsigned int gaui_CurBinFilePos[20];

void* BIG_gp_GlobalBuffer;
int BIG_gi_GlobalBufferSize;
void* BIG_gp_GlobalSpecialBuffer;
int BIG_gi_GlobalSpecialBufferSize;

extern BOOL LOA_gb_DoReadAllFats;
extern int BIGcomp_M_CompressedSize;
extern int BIGcomp_M_BlockSize;
extern int gi_TestBin;
extern int gi_CurBin;
extern int BIG_gi_SpecialRead;

void BIG_InitModule(void);
void BIG_CloseModule(void);
int BIG_ul_SearchKeyToPos(uint32_t key);
void BIG_FreeGlobalBuffer();
int BIG_fseek(FILE* Stream, int Offset, int Origin);
int BIG_SpeedMode_fread(char** currOffset, int size, FILE* Stream);
char* BIG_pc_ReadFileTmp(int Offset, int* size);
void BIG_ReadNoSeek(int pos, void* dest, unsigned int len);
unsigned int BIG_ul_GetLengthFile(int Offset);
void BINK_SeekAndReadFromBF(int Offset, void* DstBuf, unsigned int MaxCharCount);
void BIG_Open(char* FileName);
void BIG_Close();
void PreLoadMoveBlocks(int a1);