/* Everything textures.c reaches outside itself, for the differential
   test. The two copies of the file share these, so each side sees the
   same allocator and the same byte streams.

   Three things matter:

   - nothing is ever handed back. The first side's buffers have to stay
     readable while the second side runs, and a copy that writes a
     little way past its block must not make the process die where the
     real engine runs on, so every block sits 64 bytes inside a larger
     one and MEM_Free only records the call;
   - the block size is stored in the word in front of the pointer,
     because TEX_File_LoadPalette reads it there;
   - everything else aborts if it is reached, so a new dependency fails
     the test rather than passing on a stand-in. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
#include "../time.h"
#include "textures_mocks.h"

tMockTexCall MOCKT_log[MOCKT_LOG_MAX];
int MOCKT_logLen;
int MOCKT_overflow;

int gi_CurBin;
int ENG_PlatformID;

static const unsigned char* MOCKT_stream;
static int MOCKT_streamLen;

#define GUARD_BEFORE 64
#define GUARD_AFTER 8192

void MOCKT_Reset(void)
{
    MOCKT_logLen = 0;
    MOCKT_overflow = 0;
    MEM_gst_MemoryInfo.usedMemoryByTextures = 0;
}

void MOCKT_Record(const char* fn, int a, int b)
{
    if (MOCKT_logLen >= MOCKT_LOG_MAX)
    {
        MOCKT_overflow = 1;
        return;
    }
    MOCKT_log[MOCKT_logLen].fn = fn;
    MOCKT_log[MOCKT_logLen].a = a;
    MOCKT_log[MOCKT_logLen].b = b;
    MOCKT_logLen++;
}

void MOCKT_SetStream(const unsigned char* bytes, int len)
{
    MOCKT_stream = bytes;
    MOCKT_streamLen = len;
}

static void* MOCKT_Block(unsigned int size, const char* who)
{
    unsigned char* raw;
    unsigned char* block;

    raw = (unsigned char*)calloc(1, size + GUARD_BEFORE + GUARD_AFTER);
    if (!raw)
    {
        printf("the textures test ran out of memory in %s\n", who);
        abort();
    }
    block = raw + GUARD_BEFORE;
    *((uint32_t*)block - 1) = size;
    return block;
}

/* How far past a block the test may compare: the block itself plus the
   trailing guard, so a stray write shows up as a difference instead of
   killing the process. */
int MOCKT_BlockSpan(void* ptr)
{
    return ptr ? (int)*((uint32_t*)ptr - 1) + GUARD_AFTER : 0;
}

void* MEM_p_Alloc(unsigned int size)
{
    MOCKT_Record("MEM_p_Alloc", (int)size, 0);
    return MOCKT_Block(size, "MEM_p_Alloc");
}

void* MEM_p_AllocTmp(int size)
{
    MOCKT_Record("MEM_p_AllocTmp", size, 0);
    return MOCKT_Block(size, "MEM_p_AllocTmp");
}

void* MEM_p_AllocAligned(int size, int alignment)
{
    MOCKT_Record("MEM_p_AllocAligned", size, alignment);
    return MOCKT_Block(size, "MEM_p_AllocAligned");
}

void* MEM_p_Realloc(void* ptr, size_t size)
{
    void* block;
    unsigned int was;

    MOCKT_Record("MEM_p_Realloc", (int)size, 0);
    block = MOCKT_Block(size, "MEM_p_Realloc");
    if (ptr)
    {
        was = *((uint32_t*)ptr - 1);
        memcpy(block, ptr, was < size ? was : size);
    }
    return block;
}

void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment)
{
    void* block;
    unsigned int was;

    MOCKT_Record("MEM_p_ReallocAligned", (int)size, alignment);
    block = MOCKT_Block(size, "MEM_p_ReallocAligned");
    if (ptr)
    {
        was = *((uint32_t*)ptr - 1);
        memcpy(block, ptr, was < size ? was : size);
    }
    return block;
}

void MEM_Free(void* ptr)
{
    MOCKT_Record("MEM_Free", ptr ? (int)*((uint32_t*)ptr - 1) : -1, 0);
}

void MEM_FreeAlign(void* ptr)
{
    MOCKT_Record("MEM_FreeAlign", ptr ? (int)*((uint32_t*)ptr - 1) : -1, 0);
}

int MEM_MemBlockSizeGetAligned(void* ptr)
{
    return ptr ? (int)*((uint32_t*)ptr - 1) : 0;
}

int MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(void* ptr)
{
    return ptr ? (int)*((uint32_t*)ptr - 1) : 0;
}

int MEM_MemBlockSizeGetTmpAddressSpace(void* ptr)
{
    return ptr ? (int)*((uint32_t*)ptr - 1) : 0;
}

/* The real bodies out of loading.c, which cannot be compiled here. */

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

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    int size;
    int16_t result;

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

float LOA_ReadFloat(char** offset, float* retVal, char readMode)
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
    if (retVal)
        *retVal = result;
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

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    MOCKT_Record("BIG_ul_SearchKeyToPos", (int)key, 0);
    return MOCKT_stream ? 1 : -1;
}

char* BIG_pc_ReadFileTmp(int offset, int* size)
{
    MOCKT_Record("BIG_pc_ReadFileTmp", offset, MOCKT_streamLen);
    *size = MOCKT_streamLen;
    return (char*)MOCKT_stream;
}

static void MOCKT_Unreachable(const char* who)
{
    printf("the textures test reached %s, which it never should\n", who);
    abort();
}

float TIM_f_Clock_TrueRead() { MOCKT_Unreachable("TIM_f_Clock_TrueRead"); return 0.0f; }
void STR_AddFont(int key, struct sEngTexIntermediateStruct* tex) { MOCKT_Unreachable("STR_AddFont"); }
int TEX_l_File_LoadProcedural(char* offset, tEngTexIntermediateStruct* tex, int readPtr) { MOCKT_Unreachable("TEX_l_File_LoadProcedural"); return 0; }

struct jpeg_error_mgr* jpeg_std_error(struct jpeg_error_mgr* err) { MOCKT_Unreachable("jpeg_std_error"); return 0; }
void jpeg_CreateDecompress(void* a, int b, size_t c) { MOCKT_Unreachable("jpeg_CreateDecompress"); }
void jpeg_CreateCompress(void* a, int b, size_t c) { MOCKT_Unreachable("jpeg_CreateCompress"); }
void jpeg_destroy_decompress(void* a) { MOCKT_Unreachable("jpeg_destroy_decompress"); }
void jpeg_destroy_compress(void* a) { MOCKT_Unreachable("jpeg_destroy_compress"); }
void jpeg_finish_decompress(void* a) { MOCKT_Unreachable("jpeg_finish_decompress"); }
void jpeg_finish_compress(void* a) { MOCKT_Unreachable("jpeg_finish_compress"); }
void jpeg_read_header(void* a, int b) { MOCKT_Unreachable("jpeg_read_header"); }
void jpeg_read_scanlines(void* a, void* b, int c) { MOCKT_Unreachable("jpeg_read_scanlines"); }
void jpeg_write_scanlines(void* a, void* b, int c) { MOCKT_Unreachable("jpeg_write_scanlines"); }
void jpeg_set_defaults(void* a) { MOCKT_Unreachable("jpeg_set_defaults"); }
void jpeg_set_quality(void* a, int b, int c) { MOCKT_Unreachable("jpeg_set_quality"); }
void jpeg_start_compress(void* a, int b) { MOCKT_Unreachable("jpeg_start_compress"); }
void jpeg_start_decompress(void* a) { MOCKT_Unreachable("jpeg_start_decompress"); }
void jpeg_default_colorspace(void* a) { MOCKT_Unreachable("jpeg_default_colorspace"); }
void jpeg_stdio_src_jade(void* a) { MOCKT_Unreachable("jpeg_stdio_src_jade"); }
void jpeg_stdio_dest_jade(void* a, void* b) { MOCKT_Unreachable("jpeg_stdio_dest_jade"); }
