#include "common.h"
#include "memory.h"
#include "bigfiles.h"
#include "basic_array.h"
#include "sound.h"
#include "dbg.h"
#include "textget.h"
#include "loading.h"

int LOA_gb_CompressBin = 1;

char LOA_ReadChar(char** offset, char* retVal, char readMode)
{
    char result;
    int size;

    result = 0;
    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
    {
        size = 0;
    }
    else
    {
        result = **offset;
        size = 1;
    }
    *offset += size;
    if (retVal)
        *retVal = result;
    return result;
}

void LOA_ReadSpriteMapper(char** offset, tSpgSpriteMapperStruct* retVal, char readMode)
{
    if (retVal)
    {
        LOA_ReadChar(offset, &retVal->b1, readMode);
        LOA_ReadChar(offset, &retVal->b2, readMode);
        LOA_ReadChar(offset, &retVal->b3, readMode);
        LOA_ReadChar(offset, &retVal->b4, readMode);
        LOA_ReadChar(offset, &retVal->b5, readMode);
        LOA_ReadChar(offset, &retVal->b6, readMode);
        LOA_ReadChar(offset, &retVal->b7, readMode);
        LOA_ReadChar(offset, &retVal->b8, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += 8;
    }
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int result;
    int size;

    result = 0;
    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
    {
        size = 0;
    }
    else
    {
        result = *(uint32_t*)*offset;
        size = 4;
    }
    *offset += size;
    if (retVal)
        *retVal = result;
    return result;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    int size;
    __int16 result;

    size = 2;
    result = 0;
    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        size = 0;
    else
        result = *(uint16_t*)*offset;
    *offset += size;
    if (retVal)
        *retVal = result;
    return result;
}

float LOA_ReadFloat(char** offset, float* a2, char readMode)
{
    float result;
    int size;

    result = 0.0;
    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
    {
        size = 0;
    }
    else
    {
        result = *(float*)*offset;
        size = 4;
    }
    *offset += size;
    if (a2)
        *a2 = result;
    return result;
}

char** LOA_ReadCharArray(char** offset, char* retVal, unsigned int len, char readMode)
{
    char** result;

    result = offset;
    if (retVal)
        memcpy(retVal, *offset, len);
    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        *offset = *offset;
    else
        *offset += len;
    return result;
}

void LOA_ReadString(char** offset, char* retVal, int len, char readMode)
{
    if (retVal)
    {
        *retVal = 0;
        LOA_ReadCharArray(offset, retVal, len, readMode);
        retVal[len - 1] = 0;
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += len;
    }
}

int LOA_ReadBinFileHeader(char** offset, uint32_t* retVal, char readMode)
{
    return LOA_ReadLong(offset, retVal, readMode);
}

void LOA_ReadVector(char** offset, Vec3* retVal, int readMode)
{
    if (retVal)
    {
        LOA_ReadFloat(offset, &retVal->x, readMode);
        LOA_ReadFloat(offset, &retVal->y, readMode);
        LOA_ReadFloat(offset, &retVal->z, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(Vec3);
    }
}

void LOA_ReadMatrix(char** offset, Mtx4x4* retVal, int readMode)
{
    if (retVal)
    {
        LOA_ReadFloat(offset, &retVal->v1.x, readMode);
        LOA_ReadFloat(offset, &retVal->v1.y, readMode);
        LOA_ReadFloat(offset, &retVal->v1.z, readMode);
        LOA_ReadFloat(offset, &retVal->_m14, readMode);
        LOA_ReadFloat(offset, &retVal->v2.x, readMode);
        LOA_ReadFloat(offset, &retVal->v2.y, readMode);
        LOA_ReadFloat(offset, &retVal->v2.z, readMode);
        LOA_ReadFloat(offset, &retVal->_m24, readMode);
        LOA_ReadFloat(offset, &retVal->v3.x, readMode);
        LOA_ReadFloat(offset, &retVal->v3.y, readMode);
        LOA_ReadFloat(offset, &retVal->v3.z, readMode);
        LOA_ReadFloat(offset, &retVal->_m34, readMode);
        LOA_ReadVector(offset, &retVal->Pos, readMode);
        LOA_ReadFloat(offset, &retVal->w, readMode);
        LOA_ReadLong(offset, &retVal->mtxflag, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(Mtx4x4);
    }
}

void LOA_ReadQuaternion(char** offset, Quat4* retVal, int readMode)
{
    if (retVal)
    {
        LOA_ReadFloat(offset, &retVal->q1, readMode);
        LOA_ReadFloat(offset, &retVal->q2, readMode);
        LOA_ReadFloat(offset, &retVal->q3, readMode);
        LOA_ReadFloat(offset, &retVal->q4, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(Quat4);
    }
}

void LOA_ReadCompressedQuaternion(char** offset, CompressedQuat* retVal, char readMode)
{
    if (retVal)
    {
        LOA_ReadShort(offset, &retVal->s1, readMode);
        LOA_ReadShort(offset, &retVal->s2, readMode);
        LOA_ReadShort(offset, &retVal->s3, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(CompressedQuat);
    }
}

void LOA_ReadUltraCompressedQuaternion(char** offset, UltraCompressedQuat* retVal, char readMode)
{
    if (retVal)
    {
        LOA_ReadShort(offset, &retVal->s1, readMode);
        LOA_ReadChar(offset, &retVal->b2, readMode);
        LOA_ReadChar(offset, &retVal->b3, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(UltraCompressedQuat);
    }
}

void LOA_ReadAINode(char** offset, tAINode* retVal, int readMode)
{
    if (retVal)
    {
        LOA_ReadLong(offset, &retVal->val, readMode);
        LOA_ReadShort(offset, &retVal->val2, readMode);
        LOA_ReadChar(offset, &retVal->flags, readMode);
        LOA_ReadChar(offset, &retVal->category, readMode);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(tAINode);
    }
}

void LOA_ReadAILocal(char** offset, tAILocal* retVal, int readMode)
{
    if (retVal)
    {
        LOA_ReadLong(offset, &retVal->offset, readMode);
        LOA_ReadLong(offset, &retVal->type, readMode);
        LOA_ReadString(offset, retVal->name, 30, readMode);
        LOA_ReadShort(offset, 0, 2);
    }
    else if ((readMode & 2) == 0 || !LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        *offset += sizeof(tAILocal);
    }
}

void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags)
{
    int lastRef;
    loaStackElem* stackElem;

    if (!key || key == -1)
    {
        if (retVal)
            *retVal = 0;
    }
    else
    {
        lastRef = LOA_gi_LastRef;
        stackElem = &LOA_gast_Stack[LOA_gi_LastRef];
        stackElem->key = key;
        stackElem->retValPtr = retVal;
        stackElem->funcPtr = func;
        LOA_lastElem = stackElem;
        stackElem->flags = flags;
        LOA_gi_LastRef = (lastRef + 1) % 5000;
    }
}

uint8_t LOA_HashPos(uint32_t a1)
{
    return (a1 >> 8) + a1;
}

int LOA_ul_SearchAddress(int a1)
{
    uint8_t hash = LOA_HashPos(a1);
    return BAS_bsearch(a1, &LOA_gst_Array[hash]);
}

void LOA_DeleteAddress(int a1)
{
    uint8_t hash;
    uint32_t val = 0;

    BAS_bdeletegetval(a1, &LOA_gst_InvArray, &val);
    if (val)
    {
        hash = LOA_HashPos(val);
        BAS_bdeletegetval(val, &LOA_gst_Array[hash], &val);
    }
}

void LOA_AddPosAddress(unsigned int pos, unsigned int address)
{
    uint8_t hash;

    hash = LOA_HashPos(pos);
    BAS_binsert(pos, address, &LOA_gst_Array[hash]);
    BAS_binsert(address, pos, &LOA_gst_InvArray);
}

int LOA_ul_GetCurrentKey()
{
    return LOA_gast_Stack[LOA_gi_CurRef].key;
}

BOOL LOA_IsSpecial(unsigned int a1)
{
    if (BIG_gi_ReadMode)
        return BAS_bsearch(a1, &LOA_gst_SpecialArray) != -1;
    else
        return FALSE;
}

void LOA_AddKeyToSpecialArray(int a1)
{
    BAS_binsert(a1, 1, &LOA_gst_SpecialArray);
}

void LOA_KillSpecialArray()
{
    BAS_bfree(&LOA_gst_SpecialArray);
}

void LOA_Resolve()
{
    loaStackElem* currElem;
    int position;
    void* addr;
    void** retValPtr;
    char tmpStr[100];

    BIG_gb_CanCache = 1;
    for (; LOA_gi_CurRef % 5000 != LOA_gi_LastRef; LOA_gi_CurRef = (LOA_gi_CurRef + 1) % 5000)
    {
        currElem = &LOA_gast_Stack[LOA_gi_CurRef];
        LOA_gpst_CurRef = currElem;
        position = BIG_ul_SearchKeyToPos(currElem->key);
        if (position == -1)
        {
            if ((currElem->flags & 1) != 0)
            {
                sprintf(tmpStr, "Key does not exist (%x)", currElem->key);
                DBG_assert(0, __FILE__, __LINE__, "", tmpStr, NULL, 1);
            }
        }
        else
        {
            addr = (void*)LOA_ul_SearchAddress(position);
            if (addr == (void*)-1)
            {
                addr = (void*)(currElem->funcPtr)(position);
                if ((currElem->flags & 4) == 0)
                {
                    if (position)
                        LOA_AddPosAddress(position, (unsigned int)addr);
                }
            }
            retValPtr = currElem->retValPtr;
            if (retValPtr)
                *retValPtr = addr;
        }
    }
    BIG_FreeGlobalBuffer();
    BIG_gb_CanCache = 0;
}

fpos_t call_fgetpos(FILE* Stream)
{
    fpos_t pos;

    fgetpos(Stream, &pos);
    return pos;
}

void LOA_LoadSpecialArray()
{
    FILE* fp;
    unsigned int len;
    unsigned int num;
    int key;

    BAS_binit(&LOA_gst_SpecialArray, 4000);
    fp = fopen("jade.spe", "rb");
    if (fp)
    {
        BIG_fseek(fp, 0, 2);
        len = (unsigned int)call_fgetpos(fp);
        BIG_fseek(fp, 0, 0);
        if (len > 0)
        {
            num = ((len - 1) >> 2) + 1;
            do
            {
                fread(&key, 4u, 1u, fp);
                LOA_AddKeyToSpecialArray(key);
                --num;
            } while (num);
        }
        fclose(fp);
    }
}

void LOA_BeforeLoading()
{
    BASStruct* currArrElem;

    currArrElem = LOA_gst_Array;
    do
        BAS_binit(currArrElem++, 80);
    while (currArrElem <= &LOA_gst_Array[255]);

    BAS_binit(&LOA_gst_InvArray, 10000);

    LOA_gi_LastRef = 0;
    LOA_gi_CurRef = 0;
}

void LOA_InitModule(void)
{
    LOA_BeforeLoading();
}

void LOA_AfterLoading()
{
    BASStruct* currArrElem;

    currArrElem = LOA_gst_Array;
    do
        BAS_bfree(currArrElem++);
    while (currArrElem <= &LOA_gst_Array[255]);
    
    BAS_bfree(&LOA_gst_InvArray);

    LOA_gi_LastRef = 0;
    LOA_gi_CurRef = 0;
}

void LOA_BeginSpeedMode(int key)
{
    int binKey;
    unsigned int v2;

    if (ENG_PlatformID == 2)
    {
        BIGcomp_M_BlockSize = 0x3E800;
        BIGcomp_M_CompressedSize = 0x80000;
    }
    binKey = key;
    gi_TestBin = -1;
    if (key == 0xFF800000 || key == 0xFF400000 || (key & 0xFF000000) == 0xFD000000 || (key & 0xFF000000) == 0xFE000000)
    {
        LOA_sb_ActivateSoundLoadAfterBin = 0;
    }
    else
    {
        LOA_sb_ActivateSoundLoadAfterBin = 1;
        SND_gst_Params.sndMatrixRef = 0;
        dword_98F20C = 1;
    }
    if (!LOA_gb_SpeedMode)
    {
        SND_StreamStopAll();
        if (key != 0xFF400000 && key != 0xFF800000 && (key & 0xFF000000) != 0xFE000000 && (key & 0xFF000000) != 0xFD000000)
            SND_NotifyNewLoading();
        return;
    }
    if (!LOA_ul_BinKey && (key == 0xFF800000 || key == 0xFF400000 || (key & 0xFF000000) == 0xFE000000))
        return;
    if (key == 0xFF400000)
    {
        gi_TestBin = 4;
        binKey = LOA_ul_BinKey & 0xFFFFF | 0xFF400000;
        LOA_gb_CompressBin = 0;
    }
    else if (key == 0xFF800000)
    {
        binKey = LOA_ul_BinKey & 0xFFFFF | 0xFF800000;
        gi_TestBin = 1;
        LOA_gb_CompressBin = 1;
        ++dword_98F210;
    }
    else if ((key & 0xFF000000) == 0xFE000000)
    {
        gi_TestBin = 3;
        LOA_gb_CompressBin = 1;
    }
    else
    {
        if ((key & 0xFF000000) != 0xFD000000)     // ez a world bin
        {
            SND_NotifyNewLoading();
            if (!SND_gc_NoSound)
                SND_StreamStopAll();

            gi_TestBin = 0;
            binKey = key & 0xFFFFF | 0xFF000000;
            LOA_gb_CompressBin = 1;
            LOA_sb_ActivateSoundLoadAfterBin = 1;
            goto LABEL_31;
        }
        gi_TestBin = 2;
        binKey = (key ^ LOA_ul_BinKey) & 0xFFFFF ^ key;
        LOA_gb_CompressBin = 1;
    }
    LOA_sb_ActivateSoundLoadAfterBin = 0;
LABEL_31:
    LOA_ul_BinKey = binKey;
    BIG_gi_ReadSeek = 0;
    BIG_gp_ReadBuffer = 0;
    BIG_gp_CompressedBuffer = 0;
    BIG_gi_ReadSize = 0;
    BIG_gi_ReadMode = 0;
    v2 = BIG_ul_SearchKeyToPos(binKey);           // speedmodeban (ami itt mindig lesz) a key-el tér vissza
                                                  // 
    if (v2 != -1)                               // szóval ez itt mindig teljesül
    {
        BIG_gul_ReadPos = v2;
        BIG_gi_ReadMode = 2;
        BIG_gi_ReadSize = 0x200000;
        BIG_gi_ReadSeek = 0;
        BIG_gi_RealSeek = 0;
    }
}

char* LOA_FetchBuffer(int size)
{
    char* currOffset;

    currOffset = NULL;
    if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
        return 0;
    currOffset = 0;
    BIG_SpeedMode_fread(&currOffset, size, BIG_gst.CLibFileHandle);
    return currOffset;
}

char* LOA_FetchFile(int* sizeRet)
{
    char* currOffset;
    int size;

    if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
        return 0;
    currOffset = 0;
    BIG_SpeedMode_fread(&currOffset, 4, BIG_gst.CLibFileHandle);
    LOA_ReadBinFileHeader(&currOffset, &size, 8);
    *sizeRet = size;
    return LOA_FetchBuffer(size);
}

void LOA_CloseModule(void)
{
    LOA_AfterLoading();
}

void LOA_EndSpeedMode()
{
    int v0;

    if (LOA_gb_SpeedMode)
    {
        if (BIG_gp_ReadBuffer && BIG_gi_ReadMode == 2)
        {
            MEM_FreeFromEnd(BIG_gp_ReadBuffer);
            if (BIG_gp_CompressedBufferPtrBackup)
                MEM_Free(BIG_gp_CompressedBufferPtrBackup);
            BIG_gp_ReadBuffer = 0;
            BIG_gp_CompressedBuffer = 0;
        }
        BIG_gi_ReadMode = 0;
    }
    else
    {
        SND_gb_AllSoundsLoaded = 0;
    }
    if (LOA_sb_ActivateSoundLoadAfterBin)
    {
        SND_gb_AllSoundsLoaded = 0;                 // SND_ActivateAfterBin inline
        if (SND_gb_UnloadedFile && SND_gst_Params.soundEnabled)
        {
            SND_gst_Params.flags &= ~0x3000u;
            SND_gb_UnloadedFile = 0;
            SND_LoadAllSounds();
            if (LOA_gb_SpeedMode)
            {
                v0 = LOA_ul_BinKey & 0xFFFFF | ((((TEXT_gst_Global.language & 0xff) + 1) | 0xFFFFFFE0) << 20);
                LOA_BeginSpeedMode(v0);
                SND_HeaderLoadArray(&SND_gst_HeaderArray, v0);
                LOA_EndSpeedMode();
            }
            else
            {
                SND_HeaderMakeArray(&SND_gst_HeaderArray);
            }
            SND_ResolveStaticPrefetchForSpecialMdF();
            SND_StreamResolveSyncPrefetch();
        }
    }
}

BOOL isSpeedModeAndBinFile()
{
    return LOA_gb_SpeedMode && BIG_gi_ReadMode == 2;
}