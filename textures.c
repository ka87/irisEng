#include "common.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "textures.h"
#include "gdi.h"
#include "time.h"
#include "profiler.h"
#include "jpeg-6b/jpeglib.h"
#include "engine.h"
#include "strings.h"
#include <setjmp.h>


int TEXPRO_sul_UpdateCounter = -1;
int gi_LastES = -1;

int al_QualityCompressionOrder[] = { 0, 1, 0, 2, 0, 1, 3, 0, 1, 2, 0, 1, 0, 2, 0, 1, 3, 0, 1, 2, 4 };
int al_QualityCompressionLimit[] = { 0x10, 0x10, 0x20, 0x40, 0x400, 0 };

typedef struct sTexScaleTap
{
    unsigned int first;
    unsigned int firstWeight;
    unsigned int last;
    unsigned int lastWeight;
} tTexScaleTap;

static char* TEX_l_AllocImgData(unsigned int size)
{
    char* data;

    data = (char*)MEM_p_AllocAligned(size, 64);
    MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetAligned(data);

    return data;
}

static void TEX_l_FreeImgData(void* data)
{
    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(data);
    MEM_FreeAlign(data);
}

static char* TEX_l_FreeScaleSrc(char* src)
{
    if (src)
        TEX_l_FreeImgData(src);

    return NULL;
}

static void* TEX_l_GrowAlignedList(void* elems, int num, int bytes)
{
    void* grown;

    if (num == 256)
    {
        grown = MEM_p_AllocAligned(bytes, 64);
    }
    else
    {
        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAligned(elems);
        grown = MEM_p_ReallocAligned(elems, bytes, 64);
    }
    MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetAligned(grown);

    return grown;
}

static int TEX_l_SwapRB(int color)
{
    return color & 0xFF00FF00 | ((uint8_t)color << 16) | (uint8_t)(color >> 16);
}

static unsigned int TEX_l_ShiftChannel(unsigned int pixel, unsigned int mask, int shift)
{
    if (shift < 0)
        return (mask & pixel) << -(char)shift;

    return (mask & pixel) >> shift;
}

static int TEX_l_BuildScaleTaps(tTexScaleTap* taps, int srcSize, int destSize)
{
    unsigned int acc;
    unsigned int first;
    int step;
    int i;

    acc = 0;
    first = 0;
    step = (srcSize << 8) / destSize;
    for (i = 0; i < destSize; i++)
    {
        taps[i].first = first;
        taps[i].firstWeight = 256 - (uint8_t)acc;
        acc += step;
        taps[i].last = acc >> 8;
        if (acc >> 8 == taps[i].first)
            taps[i].firstWeight += (uint8_t)acc - 256;
        else
            taps[i].lastWeight = (uint8_t)acc;
        first = acc >> 8;
    }
    taps[destSize - 1].last = srcSize - 1;
    taps[destSize - 1].lastWeight = 255;

    return step;
}

static unsigned int TEX_l_TapWeight(tTexScaleTap* tap, unsigned int i)
{
    if (i == tap->first)
        return tap->firstWeight;
    if (i == tap->last)
        return tap->lastWeight;

    return 256;
}

void PreLoadTex_Finished(int *ret)
{
    *ret = 1;
}

void PreLoadTexReset()
{
    gp_LastPointer = MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer;
    memset(gai_CurSize, 0, 80);
    gi_CurBin = 0;
    gap_FirstPointer[0] = (char*)(MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer);
}

struct jpeg_error_st {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

void my_error_exit(j_common_ptr cinfo)
{
    struct jpeg_error_st* myerr = (struct jpeg_error_st*)cinfo->err;

    longjmp(myerr->setjmp_buffer, 1);
}

void JpegInfo(tJpegLibIntf* jpg)
{
    struct jpeg_decompress_struct jds;
    struct jpeg_error_st jerr;

    jds.err = (void*)jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&jds);
        jpg->error = 1;
    }
    else
    {
        jpeg_create_decompress(&jds);

        jpeg_stdio_src_jade(&jds);
        jpeg_read_header(&jds, 1);
        jpeg_start_decompress(&jds);

        jpg->width = jds.output_width;
        jpg->height = jds.output_height;
        jpg->bytesNumPerPixel = jds.num_components;

        jpeg_destroy_decompress(&jds);

        jpg->error = 0;
    }
}

void JpegRead(tJpegLibIntf* jpg)
{
    char** scanLineArray;
    char* currData;
    struct jpeg_decompress_struct jds;
    struct jpeg_error_st jerr;

    jds.err = (void*)jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&jds);
        jpg->error = 1;
    }
    else
    {
        jpeg_create_decompress(&jds);
        jpeg_stdio_src_jade(&jds);
        jpeg_read_header(&jds, 1);
        jpeg_start_decompress(&jds);
        jpg->bytesNumPerPixel = jds.num_components;

        scanLineArray = jds.mem->alloc_sarray(((j_common_ptr)&jds), 1, (jds.output_width * jds.output_components), 1);

        currData = (char*)jpg->indata;
        if (jds.output_scanline < (unsigned int)jds.output_height)
        {
            while (1)
            {
                jpeg_read_scanlines(&jds, scanLineArray, 1);
                memcpy(currData, *scanLineArray, (jds.output_width * jds.output_components));
                currData += (jds.output_width * jds.output_components);
                if (jds.output_scanline >= (unsigned int)jds.output_height)
                    break;
            }
        }
        jpeg_finish_decompress(&jds);
        jpeg_destroy_decompress(&jds);
        jpg->error = 0;
    }
}

void JpegWrite(tJpegLibIntf* jpg)
{
    char* row;

    struct jpeg_compress_struct jcs;
    struct jpeg_error_st jerr;

    jpgHightPointer = 0;

    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_compress(&jcs);
        jpg->error = 1;
    }
    else
    {

        memset(&jcs, 0, sizeof(jcs));
        memset(&jerr, 0, sizeof(jerr));
        jcs.err = (void*)jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;

        jpeg_create_compress(&jcs);
        jpeg_stdio_dest_jade(&jcs, jpg->outdata);

        jcs.in_color_space = JCS_RGB; // todo fixme: ez nincs az eredetiben
        jcs.jpeg_color_space = JCS_RGB;
        jcs.image_width = jpg->width;
        jcs.image_height = jpg->height;
        jcs.num_components = 3;
        jcs.input_components = 3; // todo fixme: ez nincs az eredetiben

        jpeg_set_defaults(&jcs);

        jcs.smoothing_factor = jpg->smoothing_factor;
        jcs.data_precision = 8;
        jcs.arith_code = jpg->arith_coding;
        jcs.optimize_coding = TRUE;
        jcs.CCIR601_sampling = jpg->CCIR601_sampling;

        if (jpg->width > 320)
            jcs.dct_method = JDCT_IFAST;
        if (jpg->width > 2000)
            jcs.dct_method = JDCT_ISLOW;

        jpeg_set_quality(&jcs, jpg->quality, 1);
        jpeg_default_colorspace(&jcs);
        jpeg_start_compress(&jcs, 1);

        while (jcs.next_scanline < jcs.image_height)
        {
            row = (char*)(jpg->indata) + 3 * jpg->width * jcs.next_scanline;
            jpeg_write_scanlines(&jcs, &row, 1);
        }
        jpeg_finish_compress(&jcs);
        jpeg_destroy_compress(&jcs);
        jpg->error = 0;
    }
}

void TEX_Compress24_Xo2(char* src, char* dest, int w, int h)
{
    unsigned int i;

    for (i = (unsigned int)(h * w) >> 1; i; --i)
    {
        *dest = ((*src >> 1) & 0x7E) + ((src[3] >> 1) & 0x7E);
        dest[1] = ((src[1] >> 1) & 0x7E) + ((src[4] >> 1) & 0x7E);
        dest[2] = ((src[2] >> 1) & 0x7E) + ((src[5] >> 1) & 0x7E);
        src += 6;
        dest += 3;
    }
}

void TEX_Compress24_Yo2(char* src, char* dest, int w, unsigned int h)
{
    unsigned int y;
    int x;
    int stride;

    stride = 3 * w;
    for (y = h >> 1; y; --y)
    {
        for (x = stride; x; --x)
        {
            *dest++ = ((*src >> 1) & 0x7E) + ((src[stride] >> 1) & 0x7E);
            ++src;
        }
        src += stride;
    }
}

void TEX_Procedural_Reinit(struct sGdi_DisplayData* dd)
{
    tTexProceduralStruct* pro;
    int i;

    if (TEX_gi_NbProcedural)
    {
        pro = TEX_gdst_Procedural;
        for (i = 0; i < TEX_gi_NbProcedural; ++i, ++pro)
        {
            pro->proceduralIntf->Free(pro);
            if ((dd->texManager.flags & 0x200000) == 0 || (pro->s2 & 1) == 0)
            {
                if (pro->proceduralPicData)
                    MEM_FreeAlign(pro->proceduralPicData);
            }
        }
        TEX_gi_NbProcedural = 0;
        MEM_Free(TEX_gdst_Procedural);
        TEX_gdst_Procedural = 0;
    }
}

void TEX_Anim_Reinit()
{
    int i;

    if (TEX_gi_NbAnimated)
    {
        for (i = 0; i < TEX_gi_NbAnimated; ++i)
        {
            if (TEX_gdst_Animated[i].texNum)
                MEM_Free(TEX_gdst_Animated[i].texPtr);
        }
        TEX_gi_NbAnimatedMax = 0;
        TEX_gi_NbAnimated = 0;
        MEM_Free(TEX_gdst_Animated);
        TEX_gdst_Animated = 0;
    }
}

struct sEngTexListElemStruct* TEX_pst_List_FindTexture(struct sEngTexListStruct* list, int key)
{
    int i;

    for (i = 0; i < list->actTexElemNum; i++) {
        if (list->texElems[i].texKey == key) return &list->texElems[i];
    }

    return NULL;
}

struct sEngPaletteElem* TEX_pst_List_FindPalette(tEngTexListStruct* list, int key)
{
    int i;

    for (i = 0; i < list->paletteNum; i++) {
        if (list->palettes[i].key == key) return &list->palettes[i];
    }

    return NULL;
}

struct sTexAnimatedElem* TEX_pst_Anim_Get(int16_t id)
{
    int i;

    for (i = 0; i < TEX_gi_NbAnimated; i++) {
        if (TEX_gdst_Animated[i].id == id) return &TEX_gdst_Animated[i];
    }

    return NULL;
}

struct sTexAnimatedElem* TEX_pst_Anim_Add(int16_t id, char texNum, int16_t flags, struct sTexAnimatedInnerElem* data)
{
    tTexAnimatedElem* anim;

    if (TEX_gi_NbAnimated == TEX_gi_NbAnimatedMax)
    {
        TEX_gi_NbAnimatedMax += 16;
        if (TEX_gi_NbAnimatedMax == 16)
            TEX_gdst_Animated = (tTexAnimatedElem*)MEM_p_Alloc(sizeof(tTexAnimatedElem) * TEX_gi_NbAnimatedMax);
        else
            TEX_gdst_Animated = (tTexAnimatedElem*)MEM_p_Realloc(TEX_gdst_Animated, sizeof(tTexAnimatedElem) * TEX_gi_NbAnimatedMax);
    }
    anim = &TEX_gdst_Animated[TEX_gi_NbAnimated++];
    anim->flags = flags;
    anim->texNum = texNum;
    anim->id = id;
    anim->b3 = 0;
    anim->animationFlags = 0;
    anim->texPtr = data;
    if (data)
    {
        anim->currentTex = 0;
        anim->remainingTime = data->texTime;
        TEX_gst_GlobalList.texElems[id].animatedData.textureID = data->texIdx;
    }
    else
    {
        anim->remainingTime = 0;
        anim->currentTex = -1;
        TEX_gst_GlobalList.texElems[id].animatedData.textureID = -1;
    }
    return anim;
}

void TEX_Anim_Update()
{
    double time;
    tTexAnimatedElem* anim;
    int frames;
    int i;
    uint8_t texNum;
    uint8_t animationFlags;
    uint8_t forced;
    int16_t next;
    tTexAnimatedInnerElem* tex;

    time = TIM_gf_dt + sf_TimeLeft_218;
    anim = TEX_gdst_Animated;
    frames = (unsigned int)(60.0f * time);
    sf_TimeLeft_218 = (float)(time - (double)frames * 0.016666668);

    for (i = 0; i < TEX_gi_NbAnimated; ++i, ++anim)
    {
        texNum = anim->texNum;
        if (!texNum)
            continue;

        animationFlags = anim->animationFlags;
        forced = anim->b3;
        if ((animationFlags & 2) == 0 || forced == anim->currentTex)
        {
            if ((animationFlags & 1) != 0)
                continue;
            anim->remainingTime -= frames;
            if (anim->remainingTime >= 0)
                continue;
            next = (uint8_t)(anim->currentTex + 1) % (int16_t)texNum;
        }
        else
        {
            next = (forced % texNum) & 0xff;
            anim->b3 = (uint8_t)next;
        }
        anim->currentTex = (uint8_t)next;
        tex = &anim->texPtr[(uint8_t)next];
        anim->remainingTime = tex->texTime;
        TEX_gst_GlobalList.texElems[anim->id].animatedData.textureID = tex->texIdx;
    }
}

void TEX_Procedural_AfterLoad(struct sGdi_DisplayData* dd)
{
    tTexProceduralStruct* pro;
    tEngTexListElemStruct* tle;
    int i;

    pro = TEX_gdst_Procedural;
    for (i = 0; i < TEX_gi_NbProcedural; ++i, ++pro)
    {
        tle = &TEX_gst_GlobalList.texElems[pro->elemIdx];
        pro->height = tle->normalData.height;
        pro->width = tle->normalData.width;
        if ((dd->texManager.flags & 0x200000) != 0 && (pro->s2 & 1) != 0)
            pro->proceduralPicData = (uint32_t *)(gpst_DX_stDD->Request(34, (void *)(tle->texKey)));
        if (!pro->proceduralPicData)
        {
            pro->proceduralPicData = (uint32_t*)MEM_p_AllocAligned(4 * tle->normalData.height * tle->normalData.width, 16);
            memset(pro->proceduralPicData, 0, 4 * tle->normalData.height * tle->normalData.width);
        }
    }
}

struct sTexProceduralStruct* TEX_pst_Procedural_Get(int16_t elemIdx)
{
    int i;

    for (i = 0; i < TEX_gi_NbProcedural; ++i)
    {
        if (TEX_gdst_Procedural[i].elemIdx == elemIdx)
            return &TEX_gdst_Procedural[i];
    }

    return 0;
}

void TEX_Procedural_Update(struct sGdi_DisplayData* dd)
{
    int i;
    tTexProceduralStruct* pro;
    tTexUploadRequest tur;

    gpst_DX_stDD = dd;
    if (TEXPRO_sul_UpdateCounter != dd->rasters[4].unk1)
    {
        TEXPRO_sul_UpdateCounter = dd->rasters[4].unk1;
        pro = TEX_gdst_Procedural;
        for (i = 0; i < TEX_gi_NbProcedural; ++i, ++pro)
        {
            pro->proceduralIntf->Disrupt(pro);
            if (pro->proceduralIntf->Update(pro) || (pro->s2 & 0x100) == 0)
            {
                pro->s2 |= 0x100u;
                if ((dd->texManager.flags & 0x200000) == 0 || (pro->s2 & 1) == 0)
                {
                    tur.idx = pro->elemIdx;
                    tur.dataPtr = pro->proceduralPicData;
                    dd->Request(12, (void *)&tur);
                }
            }
        }
    }
}

void TEX_Compress_Xo2(char* src, char* dest, int w, int h)
{
    unsigned int i;

    for (i = (unsigned int)(h * w) >> 1; i; --i)
    {
        *(uint32_t*)dest = ((*(uint32_t*)src >> 1) & 0x7E7E7E7E) + ((*((uint32_t*)src + 1) >> 1) & 0x7E7E7E7E);
        src += 8;
        dest += 4;
    }
}

void TEX_Compress_Yo2(char* src, char* dest, int w, unsigned int h)
{
    unsigned int y;
    int x;
    int stride;

    stride = 4 * w;
    for (y = h >> 1; y; --y)
    {
        for (x = w; x; --x)
        {
            *(uint32_t*)dest = ((*(uint32_t*)src >> 1) & 0x7E7E7E7E) + ((*(uint32_t*)&src[stride] >> 1) & 0x7E7E7E7E);
            src += 4;
            dest += 4;
        }
        src += stride;
    }
}

void TEX_Procedural_UpdateWithAnother(struct sTexProceduralStruct* dest, struct sTexProceduralStruct* src)
{
    uint16_t w;
    uint16_t h;

    w = src->width;
    h = src->height;
    for (; w > dest->width; w >>= 1)
        TEX_Compress_Xo2((char*)src->proceduralPicData, (char*)src->proceduralPicData, w, h);
    for (; h > dest->height; h >>= 1)
        TEX_Compress_Yo2((char*)src->proceduralPicData, (char*)src->proceduralPicData, w, h);
    memcpy(dest->proceduralPicData, src->proceduralPicData, 4 * w * (unsigned int)h);
    dest->s2 &= ~100u; //HIBYTE(s2) &= ~0x1
}

unsigned int TEX_ul_GetPower2Up(unsigned int n)
{
    unsigned int result;
    unsigned int i;

    result = 1;
    for (i = 0; i < 30; ++i)
    {
        if (n <= result)
            break;
        result *= 2;
    }
    return result;
}

void TEX_Expand8_XBy2(char* src, char* dest, int w, int h)
{
    char* in;
    char* out;

    out = &dest[2 * h * w];
    // preserved: the walk stops above src[0], so the first pixel is not expanded
    for (in = &src[h * w - 1]; in > src; --in)
    {
        out[-1] = *in;
        out[-2] = *in;
        out -= 2;
    }
}

void TEX_Expand_YBy2(char* src, char* dest, int w, int h)
{
    char* in;
    char* out;
    int stride;
    int x;
    int y;

    out = &dest[8 * h * w];
    in = &src[4 * h * w];
    stride = 4 * w;
    for (y = h; y; --y)
    {
        for (x = w; x; --x)
        {
            in -= 4;
            out -= 4;
            *(uint32_t*)out = *(uint32_t*)in;
            *(uint32_t*)&out[-stride] = *(uint32_t*)in;
        }
        out -= stride;
    }
}

int __cdecl TEX_l_File_LoadAnimated(char* offset, tEngTexIntermediateStruct* tex, int size)
{
    uint8_t num;
    uint16_t flags;
    int i;
    tTexAnimatedInnerElem* elems;

    if (size)
    {
        LOA_ReadLong(&offset, 0, 1);
        flags = LOA_ReadShort(&offset, 0, 1);
        num = LOA_ReadChar(&offset, 0, 1);
        LOA_ReadChar(&offset, 0, 2);
    }
    else
    {
        flags = 0;
        num = 0;
    }
    tex->flags |= 0x20u;
    elems = 0;
    if (num)
    {
        elems = (tTexAnimatedInnerElem*)MEM_p_Alloc(sizeof(tTexAnimatedInnerElem) * num);
        for (i = 0; i < num; i++)
        {
            elems[i].key = LOA_ReadLong(&offset, 0, 1);
            elems[i].texTime = LOA_ReadShort(&offset, 0, 1);
            elems[i].texIdx = -1;
            LOA_ReadShort(&offset, 0, 2);
        }
    }
    tex->tp.animatedParams.aiePtr = elems;
    tex->tp.animatedParams.num = num;
    tex->tp.animatedParams.flags = flags;
    return 1;
}

int TEX_l_File_LoadRawPalette(char* offset, tEngTexIntermediateStruct* tex, unsigned int size)
{
    int* elems;
    unsigned int i;

    elems = (int*)tex->elems;
    memset(tex->elems, 0xFFu, sizeof(tex->elems));
    if (size > 80 || (size & 3) != 0)
        return 0;
    for (i = (size - 32) >> 2; i; --i)
        *elems++ = LOA_ReadLong(&offset, 0, 1);

    return 1;
}

void TEX_File_VSwap(char* dest, char* src, int w, int h, int bpp)
{
    unsigned int stride;
    char* in;
    int y;

    stride = bpp * w;
    in = &src[stride * (h - 1)];
    for (y = h; y; --y)
    {
        memcpy(dest, in, stride);
        dest += stride;
        in -= stride;
    }
}

void TEX_File_FreeDescription(struct sEngTexIntermediateStruct* tex)
{
    if (tex->descriptionPtr && tex->tgaUnkS1)
    {
        MEM_Free((void*)tex->descriptionPtr);
        tex->descriptionPtr = 0;
    }
    if (tex->imgDataPtr)
    {
        TEX_l_FreeImgData(tex->imgDataPtr);
        tex->imgDataPtr = 0;
    }
}

int TEX_l_File_LoadJpeg(char* data, struct sEngTexIntermediateStruct* tex, size_t size)
{
    uint8_t colorDepth;
    char texFlags;
    int pixels;
    int bytes;
    char* temp;
    char* gray;
    char* rgb;
    char* p;
    char c;
    tJpegLibIntf jpg;

    memset(&jpg, 0, sizeof(jpg));
    if ((tex->flags & 1) != 0)
    {
        lJADE_JPEG_INPOUT_FILE_LENGTH = size;
        ucJADE_JPEG_INPOUT_FILE = data;
        JpegInfo(&jpg);
        tex->realHeight = jpg.height;
        tex->realWidth = jpg.width;
        tex->colorDepth2 = 8 * (jpg.bytesNumPerPixel & 0xff);
        tex->colorDepth1 = 8 * (jpg.bytesNumPerPixel & 0xff);
        texFlags = (char)(tex->tp.flagsUnkAndProceduralUnkb1);
        tex->tgaUnk1b = 0;
        tex->tgaUnkS1 = 0;
        tex->redMask = 0xFF;
        tex->greenMask = 0xFF00;
        tex->blueMask = 0xFF0000;
        tex->alphaMask = 0;
        if ((texFlags & 8) != 0)
            tex->bmpUnkb1 |= 0x10;
    }
    colorDepth = tex->colorDepth1;
    if (colorDepth != 8 && colorDepth != 24)
        return 0;
    if ((tex->flags & 2) != 0)
    {
        pixels = tex->realWidth * tex->realHeight;
        bytes = 3 * pixels;
        if (!bytes)
            return 0;
        tex->imgDataPtr = TEX_l_AllocImgData(bytes);
        temp = (char*)MEM_p_Alloc(bytes);
        jpg.indata = temp;
        lJADE_JPEG_INPOUT_FILE_LENGTH = size;
        ucJADE_JPEG_INPOUT_FILE = data;
        JpegRead(&jpg);
        if (tex->colorDepth1 == 8)
        {
            TEX_File_VSwap(tex->imgDataPtr, temp, tex->realWidth, tex->realHeight, 1);
            rgb = &tex->imgDataPtr[bytes - 3];
            for (gray = &tex->imgDataPtr[tex->realWidth * tex->realHeight - 1]; rgb >= gray; --gray)
            {
                rgb[0] = *gray;
                rgb[1] = *gray;
                rgb[2] = *gray;
                rgb -= 3;
            }
        }
        else if (tex->colorDepth1 == 24)
        {
            for (p = temp; p < &temp[bytes]; p += 3)
            {
                c = p[0];
                p[0] = p[2];
                p[2] = c;
            }
            TEX_File_VSwap(tex->imgDataPtr, temp, tex->realWidth, tex->realHeight, 3);
        }
        MEM_Free(temp);

        /*
        FILE* fp = fopen("dbg_readback.ppm", "wb");
        if (fp) {
            fprintf(fp, "P6\n%d %d\n255\n", tex->realWidth, tex->realHeight);
            fwrite(tex->imgDataPtr, 1, tex->realWidth * tex->realHeight * 3, fp);
            fclose(fp);
        }
        */

    }
    return 1;
}

void TEX_l_File_TgaUncompress(char colorDepth, char* dest, char* src, int size)
{
    int left;
    char packet;
    uint8_t count;
    int pixel;
    int i;

    left = size;
    while (left > 0)
    {
        packet = *src;
        count = (*src++ & 0x7F) + 1;
        if (packet >= 0)
        {
            for (i = count; i; --i)
            {
                if (colorDepth == 24)
                {
                    dest[0] = src[0];
                    dest[1] = src[1];
                    dest[2] = src[2];
                    dest += 3;
                    src += 3;
                    left -= 3;
                }
                else
                {
                    *(uint32_t*)dest = *(uint32_t*)src;
                    src += 4;
                    dest += 4;
                    left -= 4;
                }
            }
        }
        else
        {
            pixel = *(uint32_t*)src;
            if (colorDepth == 24)
                src += 3;
            else
                src += 4;
            for (i = count; i; --i)
            {
                if (colorDepth == 24)
                {
                    dest[0] = pixel;
                    dest[1] = (pixel >> 8) & 0xff;
                    dest[2] = (pixel >> 16) & 0xff;
                    dest += 3;
                    left -= 3;
                }
                else
                {
                    // preserved: a run of 32 bit pixels is written to the same address every time
                    *(uint32_t*)dest = pixel;
                    left -= 4;
                }
            }
        }
    }
}

int TEX_l_File_LoadTga(char* data, tEngTexIntermediateStruct* tex)
{
    uint16_t mapLength;
    uint16_t w;
    int16_t h;
    uint8_t imageType;
    uint8_t mapEntryBits;
    char colorDepth;
    int16_t flags;
    char* afterHeader;
    int pixels;
    char* imgData;
    unsigned int bytes;

    LOA_ReadChar(&data, 0, 1);
    LOA_ReadChar(&data, 0, 1);
    imageType = LOA_ReadChar(&data, 0, 1);
    LOA_ReadShort(&data, 0, 1);
    mapLength = LOA_ReadShort(&data, 0, 1);
    mapEntryBits = LOA_ReadChar(&data, 0, 1);
    LOA_ReadShort(&data, 0, 1);
    LOA_ReadShort(&data, 0, 1);
    w = LOA_ReadShort(&data, 0, 1);
    h = LOA_ReadShort(&data, 0, 1);
    colorDepth = LOA_ReadChar(&data, 0, 1);
    LOA_ReadChar(&data, 0, 1);
    flags = tex->flags;
    if ((tex->flags & 1) != 0)
    {
        afterHeader = data;
        tex->realWidth = w;
        tex->realHeight = h;
        tex->tgaUnkS1 = mapLength;
        tex->colorDepth2 = colorDepth;
        tex->colorDepth1 = colorDepth;
        tex->tgaUnk1b = mapEntryBits;
        tex->tgaDataEndPtr = (int)afterHeader;
        tex->tgaDataStartPtr = 0;
        if ((tex->tp.flagsUnkAndProceduralUnkb1 & 8) != 0)
            tex->bmpUnkb1 |= 0x10u;
        switch (colorDepth)
        {
        case 32:
            tex->alphaMask = 0xFF000000;
            tex->redMask = 0xFF0000;
            tex->greenMask = 0xFF00;
            tex->blueMask = 0xFF;
            break;
        case 24:
            tex->redMask = 0xFF0000;
            tex->greenMask = 0xFF00;
            tex->blueMask = 0xFF;
            tex->alphaMask = 0;
            break;
        case 16:
            // preserved: the 16 bit case takes the 24 bit masks, not 5-5-5 as the bmp loader does
            tex->alphaMask = 0;
            tex->redMask = 0xFF0000;
            tex->greenMask = 0xFF00;
            tex->blueMask = 0xFF;
            break;
        case 8:
            tex->tgaDataStartPtr = (int)afterHeader;
            tex->tgaDataEndPtr = (int)&afterHeader[mapLength * (mapEntryBits >> 3)];
            break;
        default:
            return 0;
        }
    }
    if ((flags & 2) != 0)
    {
        pixels = tex->realWidth * tex->realHeight;
        if (!pixels)
            return 0;
        LOA_ReadCharArray(&data, 0, mapLength * (tex->tgaUnk1b >> 3), 2);
        imgData = (char*)MEM_p_AllocTmp(pixels * (tex->colorDepth2 >> 3));
        tex->imgDataPtr = imgData;
        MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetTmpAddressSpace(imgData);
        bytes = pixels * (tex->colorDepth1 >> 3);
        if (imageType >= 9u)
        {
            TEX_l_File_TgaUncompress(colorDepth, tex->imgDataPtr, data, bytes);
            LOA_ReadCharArray(&data, 0, bytes, 1);
            return 1;
        }
        LOA_ReadCharArray(&data, tex->imgDataPtr, bytes, 1);
    }
    return 1;
}

int TEX_l_File_LoadBmp(tTexWeirdBMPHeader* hdr, tEngTexIntermediateStruct* tex)
{
    int16_t flags;
    uint8_t colorDepth;
    int pixels;

    flags = tex->flags;
    if ((tex->flags & 1) != 0)
    {
        tex->realWidth = hdr->width;
        tex->realHeight = hdr->height;
        tex->colorDepth2 = hdr->colorDepth;
        tex->colorDepth1 = hdr->colorDepth;
        if ((tex->tp.flagsUnkAndProceduralUnkb1 & 8) != 0)
            tex->bmpUnkb1 |= 0x10u;
        switch (tex->colorDepth1)
        {
        case 32u:
            tex->alphaMask = 0xFF000000;
            tex->redMask = 0xFF0000;
            tex->greenMask = 0xFF00;
            tex->blueMask = 0xFF;
            break;
        case 24u:
            tex->redMask = 0xFF0000;
            tex->greenMask = 0xFF00;
            tex->blueMask = 0xFF;
            tex->alphaMask = 0;
            break;
        case 16u:
            tex->redMask = 0x7C00;
            tex->greenMask = 0x3E0;
            tex->blueMask = 0x1F;
            tex->alphaMask = 32768;
            break;
        case 8u:
            break;
        default:
            return 0;
        }
    }
    if ((flags & 2) == 0)
        return 1;

    pixels = tex->realWidth * tex->realHeight;
    if (!pixels)
        return 0;

    colorDepth = tex->colorDepth1;
    if (colorDepth != 32 && colorDepth != 24 && colorDepth != 16 && colorDepth != 8)
        return 0;

    tex->imgDataPtr = TEX_l_AllocImgData(pixels * (tex->colorDepth2 >> 3));
    // preserved: hdr is stepped by whole headers, so the pixels are read sizeof^2 bytes in
    memcpy(tex->imgDataPtr, hdr + sizeof(tTexWeirdBMPHeader), pixels * (tex->colorDepth1 >> 3));

    return 1;
}

int TEX_l_File_LoadRaw(char* offset, tEngTexIntermediateStruct* tex)
{
    int pixels;
    unsigned int bytes;

    if ((tex->flags & 2) != 0)
    {
        pixels = tex->realWidth * tex->realHeight;
        if (!pixels)
            return 0;
        bytes = (pixels * (unsigned int)tex->colorDepth2) >> 3;
        tex->imgDataPtr = TEX_l_AllocImgData(bytes);
        LOA_ReadCharArray(&offset, tex->imgDataPtr, bytes, 1);
    }
    return 1;
}

void TEX_File_DevelopParams(tEngTexIntermediateStruct* tex)
{
    tex->realWidth = tex->tp.imageDataWidth;
    tex->realHeight = tex->tp.imageDataHeight;
    switch (tex->tp.imageDataColorDepth - 16)
    {
    case 0:
        tex->colorDepth1 = 32;
        tex->alphaMask = 0xFF000000;
        tex->redMask = 0xFF0000;
        tex->greenMask = 0xFF00;
        tex->blueMask = 0xFF;
        tex->colorDepth2 = tex->colorDepth1;
        return;
    case 16:
        tex->colorDepth1 = 24;
        tex->alphaMask = 0;
        tex->redMask = 0xFF0000;
        tex->greenMask = 0xFF00;
        tex->blueMask = 0xFF;
        tex->colorDepth2 = tex->colorDepth1;
        return;
    case 32:
        tex->colorDepth1 = 16;
        tex->alphaMask = 0x8000;
        tex->redMask = 0x7C00;
        tex->greenMask = 0x3E0;
        tex->blueMask = 0x1F;
        tex->colorDepth2 = tex->colorDepth1;
        return;
    case 33:
        tex->colorDepth1 = 16;
        tex->alphaMask = 0xF000;
        tex->redMask = 0xF00;
        tex->greenMask = 0xF0;
        tex->blueMask = 0xF;
        tex->colorDepth2 = tex->colorDepth1;
        return;
    case 48:
        tex->colorDepth1 = 8;
        tex->colorDepth2 = tex->colorDepth1;
        return;
    case 64:
        tex->colorDepth1 = 4;
        tex->colorDepth2 = tex->colorDepth1;
        return;
    default:
        tex->colorDepth2 = tex->colorDepth1;
        return;
    }
}

void TEX_GetMaskParams(uint32_t mask, uint8_t* bits, uint8_t* shift)
{
    uint32_t rest;
    uint8_t n;

    rest = mask;
    *bits = 0;
    *shift = 0;
    if (mask)
    {
        if ((mask & 1) == 0)
        {
            n = 0;
            do
            {
                rest >>= 1;
                ++n;
            } while ((rest & 1) == 0);
            *shift = n;
        }
        n = *bits;
        do
        {
            rest >>= 1;
            ++n;
        } while ((rest & 1) != 0);
        *bits = n;
    }
}

void TEX_GetColorConvertParams(int srcMask, unsigned int destMask, int* shift, unsigned int* mask)
{
    int n;
    uint8_t srcBits;
    uint8_t srcShift;
    uint8_t destBits;
    uint8_t destShift;

    if (srcMask && destMask != 0)
    {
        TEX_GetMaskParams(srcMask, &srcBits, &srcShift);
        TEX_GetMaskParams(destMask, &destBits, &destShift);
        // preserved: a source channel narrower than the destination one leaves both outputs unwritten
        if (srcBits >= destBits)
        {
            n = srcBits - destBits - destShift + srcShift;
            *shift = n;
            if (n <= 0)
                *mask = destMask >> -(char)n;
            else
                *mask = destMask << n;
        }
    }
    else
    {
        *shift = 0;
        *mask = 0;
    }
}

int TEX_l_ConvertPF(struct sEngTexIntermediateStruct* src, struct sEngTexIntermediateStruct* dest)
{
    int redShift;
    int greenShift;
    int blueShift;
    int alphaShift;
    unsigned int redMask;
    unsigned int greenMask;
    unsigned int blueMask;
    unsigned int alphaMask;
    char* in;
    char* out;
    int srcBpp;
    int destBpp;
    unsigned int keep;
    unsigned int drop;
    unsigned int pixel;
    int x;
    int y;

    TEX_GetColorConvertParams(src->redMask, dest->redMask, &redShift, &redMask);
    TEX_GetColorConvertParams(src->greenMask, dest->greenMask, &greenShift, &greenMask);
    TEX_GetColorConvertParams(src->blueMask, dest->blueMask, &blueShift, &blueMask);
    TEX_GetColorConvertParams(src->alphaMask, dest->alphaMask, &alphaShift, &alphaMask);
    in = src->imgDataPtr;
    out = dest->imgDataPtr;
    srcBpp = src->colorDepth1 >> 3;
    destBpp = dest->colorDepth1 >> 3;
    if (dest->colorDepth1 == 16)
        keep = 0xFFFF;
    else
        keep = dest->colorDepth1 != 24 ? -1 : 0xFFFFFF;
    drop = ~keep;

    for (y = 0; y < src->realHeight; y++)
    {
        for (x = 0; x < src->realWidth; x++)
        {
            pixel = *(uint32_t*)in;
            *(uint32_t*)out = (keep & (TEX_l_ShiftChannel(pixel, redMask, redShift)
                | TEX_l_ShiftChannel(pixel, greenMask, greenShift)
                | TEX_l_ShiftChannel(pixel, blueMask, blueShift)
                | TEX_l_ShiftChannel(pixel, alphaMask, alphaShift)))
                | (drop & *(uint32_t*)out);
            in += srcBpp;
            out += destBpp;
        }
    }
    return 1;
}

char TEX_uc_GetMaskShift(unsigned int mask)
{
    unsigned int rest;
    char result;

    rest = mask;
    result = 0;
    if (mask && (mask & 1) == 0)
    {
        do
        {
            rest >>= 1;
            ++result;
        } while ((rest & 1) == 0);
    }
    return result;
}

void TEX_Manager_Init(struct sTexManagerStruct* mgr)
{
    mgr->keyLoadOrderIDs[0] = 3;
    mgr->keyLoadOrderIDs[1] = 0;
    mgr->keyLoadOrderIDs[2] = -1;
    mgr->keyLoadOrderIDs[3] = -1;
    mgr->i2 = 512;
    mgr->i3 = 8;
    mgr->texMemoryNeededCompressedCase2Backup = 0x1000000;
    mgr->texMemoryNeededCompressedCase1Backup = 0x80000;
    mgr->flags = 0x10d;
}

void TEX_Manager_Reinit(struct sTexManagerStruct* mgr)
{
    mgr->texMemoryNeededCompressedCase2 = mgr->texMemoryNeededCompressedCase2Backup;
    mgr->texMemoryNeededUncompressedCase2 = 0;
    mgr->texMemoryNeededCompressedCase1 = mgr->texMemoryNeededCompressedCase1Backup;
    mgr->texMemoryNeededUncompressedCase1 = 0;
    mgr->paletteUsedMemory = 0;
    mgr->timsIdx = 0;
    mgr->elemsFlagsWithoutFlag2 = 0;
    mgr->elemsFlagsWithFlag2 = 0;
}

void TEX_List_Free(tEngTexListStruct* list)
{
    int i;

    if (list->maxTexNum)
        TEX_l_FreeImgData(list->texElems);

    if (list->unk1)
    {
        for (i = 0; i < list->paletteNum; ++i)
        {
            if (list->palettes[i].paletteData)
            {
                TEX_l_FreeImgData(list->palettes[i].paletteData);
                list->palettes[i].paletteData = 0;
            }
        }
        TEX_l_FreeImgData(list->palettes);
    }
    memset(list, 0, sizeof(tEngTexListStruct));
}

void TEX_List_Init(struct sEngTexListStruct* list)
{
    memset(list, 0, sizeof(tEngTexListStruct));
}

// a pc-s verzióban ennek semmi értelme
void ResetPreloadTexAll()
{
    gi_NumES = 0;
    gi_LastES = -1;
    memset(gap_ES, 0, sizeof(gap_ES));
    memset(gau_ES, 0, sizeof(gau_ES));
    PreLoadTexReset();
    MEM_gp_MaxBlock = MEM_gst_MemoryInfo.nextFreeMemoryPointer;
    memset(gai_CurBinFileSize, 0, sizeof(gai_CurBinFileSize));
    memset(gaui_CurBinFilePos, 0, sizeof(gaui_CurBinFilePos));
}

int16_t TEX_w_List_AddTexture(tEngTexListStruct* list, int key)
{
    int correctedKey;
    int16_t result;
    int num;
    tEngTexListElemStruct* tle;

    correctedKey = key;
    if (key == -1)
        return -1;
    if (ENG_PlatformID == 2)
    {
        if ((key & 0xFFFFFF0F) == 0 && (key & 0xF0) != 0)
            correctedKey = key + 1;
        switch (correctedKey)
        {
        case 0x1F0016B6:
            correctedKey = 0x8F005893;
            break;
        case 0x62000872:
            correctedKey = 0x8F005895;
            break;
        case 0x720006F4:
            correctedKey = 0x8F005897;
            break;
        case 0x62000F6D:
            correctedKey = 0x8F005899;
            break;
        }
    }
    else if (ENG_PlatformID == 3)
    {
        if ((key & 0xFFFFFF0F) == 0 && (key & 0xF0) != 0)
            correctedKey = key + 2;
        switch (correctedKey)
        {
        case 0x1F0016B6:
            correctedKey = 0x8F005894;
            break;
        case 0x62000872:
            correctedKey = 0x8F005896;
            break;
        case 0x720006F4:
            correctedKey = 0x8F005898;
            break;
        case 0x62000F6D:
            correctedKey = 0x8F00589A;
            break;
        }
    }
    num = list->actTexElemNum;
    if (num)
    {
        tle = TEX_pst_List_FindTexture(list, correctedKey);
        if (tle)
            return tle->textElemIdx;
    }
    if (num == list->maxTexNum)
    {
        list->maxTexNum += 256;
        list->texElems = (tEngTexListElemStruct*)TEX_l_GrowAlignedList(list->texElems, list->maxTexNum,
            sizeof(tEngTexListElemStruct) * list->maxTexNum);
    }
    tle = &list->texElems[list->actTexElemNum++];
    tle->texKey = 0;
    tle->normalData.width = 0;
    tle->normalData.height = 0;
    tle->texElemFlag = 0;
    tle->texKey = correctedKey;
    result = (short)((list->actTexElemNum & 0xffff) - 1);
    tle->textElemIdx = result;
    return result;
}

BOOL TEX_b_File_CheckParams(tEngTexIntermediateStructTexParams* tp)
{
    BOOL result;

    if (tp->magicParams.magic3 == 0xC0DEC0DE && tp->magicParams.magic2 == 0x00FF00FF)
        result = tp->magicParams.magic1 == 0xCAD01234;
    else
        result = 0;
    return result;
}

void TEX_File_TransformParams(tEngTexIntermediateStructTexParams* tp)
{
    int16_t flags;
    int type;

    flags = tp->uuuu;
    tp->unk = *(uint32_t*)&tp->flagsUnkAndProceduralUnkb1; // TODO FIXME? ez itt átmásol 4 bájtot (flagsUnkAndProceduralUnkb1, imageDataColorDepth, type)
    type = (unsigned int)tp->uuuu >> 28;
    tp->flagsUnkAndProceduralUnkb1 = flags;
    tp->type = type;
    tp->imageDataColorDepth = 0;
}

tBumpMapElem* MAT_pst_BumpMap_Add(int id)
{
    return NULL;
}

void MAT_BumpMap_Del(tBumpMapElem* bump)
{
    int i;

    if (bump->b1-- == 1)
    {
        if ((bump->b2 & 1) == 0)
            MEM_Free(bump->ptr);
        MEM_Free(bump);
        for (i = 0; i < MAT_gst_BumpMapList.num; ++i)
        {
            if (MAT_gst_BumpMapList.ptr[i] == bump)
            {
                MAT_gst_BumpMapList.ptr[i] = MAT_gst_BumpMapList.ptr[--MAT_gst_BumpMapList.num];
                return;
            }
        }
    }
}

tMatSpriteElem* MAT_pst_SpriteGen_Load(int id, char* offset, int size)
{
    tMatSpriteList* node;
    tMatSpriteElem* sprite;
    tEngTexListElemStruct* tle;
    tBumpMapElem* bump;

    for (node = MAT_SPRT_AllSprt; node; node = node->prev)
    {
        if (node->idx == id)
            break;
    }
    if (node)
    {
        sprite = node->elem;
        tle = TEX_pst_List_FindTexture(&TEX_gst_GlobalList, sprite->texKey);
        if (tle)
            sprite->texIdx = tle->textElemIdx;
        else
            sprite->texIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, sprite->texKey);
        bump = node->elem->bumpMapPtr;
        if (bump)
        {
            node->elem->bumpMapPtr = MAT_pst_BumpMap_Add(sprite->bumpMapID);
            if (node->elem->bumpMapPtr != bump)
                MAT_BumpMap_Del(bump);
        }
        return sprite;
    }

    sprite = (tMatSpriteElem*)MEM_p_Alloc(sizeof(tMatSpriteElem));
    memset(sprite, 0, sizeof(tMatSpriteElem));
    node = (tMatSpriteList*)MEM_p_Alloc(sizeof(tMatSpriteList));
    node->prev = MAT_SPRT_AllSprt;
    node->elem = sprite;
    node->idx = id;
    MAT_SPRT_AllSprt = node;
    if (size)
    {
        sprite->i1 = LOA_ReadLong(&offset, 0, 1);
        sprite->i2 = LOA_ReadLong(&offset, 0, 1);
        sprite->texKey = LOA_ReadLong(&offset, 0, 1);
        sprite->s1 = LOA_ReadShort(&offset, 0, 1);
        sprite->texIdx = LOA_ReadShort(&offset, 0, 1);
        sprite->f1 = LOA_ReadFloat(&offset, 0, 1);
        sprite->f2 = LOA_ReadFloat(&offset, 0, 1);
        sprite->flags = LOA_ReadLong(&offset, 0, 1);
        sprite->f3 = LOA_ReadFloat(&offset, 0, 1);
        sprite->f4 = LOA_ReadFloat(&offset, 0, 1);
        sprite->f5 = LOA_ReadFloat(&offset, 0, 1);
        sprite->f6 = LOA_ReadFloat(&offset, 0, 1);
        sprite->bumpMapID = LOA_ReadLong(&offset, 0, 1);
        sprite->bumpMapPtr = (tBumpMapElem*)LOA_ReadLong(&offset, 0, 1);
        sprite->f7 = LOA_ReadFloat(&offset, 0, 1);
        sprite->i7 = LOA_ReadLong(&offset, 0, 1);
        sprite->i8 = LOA_ReadLong(&offset, 0, 1);
        if (sprite->texKey == -1)
        {
            sprite->texIdx = -1;
        }
        else
        {
            tle = TEX_pst_List_FindTexture(&TEX_gst_GlobalList, sprite->texKey);
            if (tle)
                sprite->texIdx = tle->textElemIdx;
            else
                sprite->texIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, sprite->texKey);
        }
        sprite->bumpMapPtr = MAT_pst_BumpMap_Add(sprite->bumpMapID);
    }
    else
    {
        sprite->f1 = 1.0;
        sprite->f5 = 1.0;
        sprite->f7 = 1.0;
        sprite->i1 = 0xC0DE1999;
        sprite->i2 = 0xC0DE2000;
        sprite->texKey = 0xFFFFFFFF;
        sprite->f2 = 0.0;
        sprite->texIdx = -1;
        sprite->flags = 0x80000000;
        sprite->f6 = 1.2f;
        sprite->bumpMapID = 0xFFFFFFFF;
    }
    return sprite;
}

void MAT_BumpMap_Close(char freeList)
{
    int i;
    tBumpMapElem* bump;

    if (MAT_gst_BumpMapList.i2)
    {
        for (i = 0; i < MAT_gst_BumpMapList.num; ++i)
        {
            bump = MAT_gst_BumpMapList.ptr[i];
            if ((bump->b2 & 1) == 0)
                MEM_Free(bump->ptr);
            MEM_Free(MAT_gst_BumpMapList.ptr[i]);
        }
        if (freeList)
        {
            MEM_Free(MAT_gst_BumpMapList.ptr);
            MAT_gst_BumpMapList.num = 0;
            MAT_gst_BumpMapList.i2 = 0;
            MAT_gst_BumpMapList.ptr = 0;
        }
        else
        {
            MAT_gst_BumpMapList.num = 0;
        }
    }
}

void MAT_SpriteGen_Close()
{
    tMatSpriteList* node;
    tMatSpriteList* next;
    tMatSpriteElem* sprite;

    node = MAT_SPRT_AllSprt;
    while (node)
    {
        next = node->prev;
        sprite = node->elem;
        if (sprite)
        {
            if (sprite->bumpMapPtr)
                MAT_BumpMap_Del(sprite->bumpMapPtr);
            MEM_Free(node->elem);
        }
        MEM_Free(node);
        node = next;
    }
    MAT_SPRT_AllSprt = 0;
    MAT_BumpMap_Close(1);
}

// FIXME: sztem ezeknek nem sok értelme van, fõleg a bumpmapnek, mert csak az engine-ben maradt szemét
int TEX_l_File_LoadSpriteGen(int key, char* offset, tEngTexIntermediateStruct* tex, int size)
{
    tex->tp.matSpriteParams.matSprite = MAT_pst_SpriteGen_Load(key, offset, size);
    return 1;
}

int TEX_l_File_Read(int key, tEngTexIntermediateStruct* tex)
{
    int16_t flags;
    int pos;
    int ok;
    char* data;
    char* params;
    int texModifyNeeded;
    uint8_t type;
    uint16_t w;
    uint16_t h;
    int size;

    flags = tex->flags;
    if ((tex->flags & 1) != 0)
    {
        memset(tex, 0, sizeof(tEngTexIntermediateStruct));
        tex->flags = flags;
    }
    pos = BIG_ul_SearchKeyToPos(key);
    if (pos == -1)
        return 0;
    data = BIG_pc_ReadFileTmp(pos, &size);
    params = data;
    if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
        params = &data[size - 32];
    tex->tp.uuuu = LOA_ReadLong(&params, 0, 1);
    tex->tp.flagsUnkAndProceduralUnkb1 = LOA_ReadShort(&params, 0, 1);
    tex->tp.type = LOA_ReadChar(&params, 0, 1);
    tex->tp.imageDataColorDepth = LOA_ReadChar(&params, 0, 1);
    tex->tp.imageDataWidth = LOA_ReadShort(&params, 0, 1);
    tex->tp.imageDataHeight = LOA_ReadShort(&params, 0, 1);
    tex->tp.unk = LOA_ReadLong(&params, 0, 1);
    tex->tp.magicParams.fontDescriptorKey = LOA_ReadLong(&params, 0, 1);
    tex->tp.magicParams.magic1 = LOA_ReadLong(&params, 0, 1);
    tex->tp.magicParams.magic2 = LOA_ReadLong(&params, 0, 1);
    tex->tp.magicParams.magic3 = LOA_ReadLong(&params, 0, 1);
    if (data && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        data = params;
    if (TEX_b_File_CheckParams(&tex->tp))
    {
        type = tex->tp.type;
        if (type == 4 || type == 5 || type == 9 || type == 7)
            tex->flags &= ~4u;
        texModifyNeeded = 0;
    }
    else
    {
        TEX_File_TransformParams(&tex->tp);
        tex->flags &= ~4u;
        texModifyNeeded = 1;
    }
    if ((tex->flags & 1) != 0)
    {
        TEX_File_DevelopParams(tex);
        if ((tex->flags & 4) != 0)
            return 1;
    }
    switch (tex->tp.type)
    {
    case 1u:
        ok = TEX_l_File_LoadTga(data, tex);
        break;
    case 2u:
        ok = TEX_l_File_LoadBmp((tTexWeirdBMPHeader*)data, tex);
        break;
    case 3u:
        ok = TEX_l_File_LoadJpeg(data, tex, size);
        break;
    case 4u:
        ok = TEX_l_File_LoadSpriteGen(key, data, tex, size - 32);
        break;
    case 5u:
        ok = TEX_l_File_LoadProcedural(data, tex, size - 32);
        break;
    case 6u:
        ok = TEX_l_File_LoadRaw(data, tex);
        break;
    case 7u:
        ok = TEX_l_File_LoadRawPalette(data, tex, size);
        break;
    case 9u:
        ok = TEX_l_File_LoadAnimated(data, tex, size - 32);
        break;
    default:
        return 0;
    }
    if (ok)
    {
        if ((tex->flags & 2) != 0 && (tex->tp.flagsUnkAndProceduralUnkb1 & 0x40) != 0)
            STR_AddFont(key, tex);
        if (texModifyNeeded)
        {
            w = tex->realWidth;
            h = tex->realHeight;
            tex->tp.imageDataColorDepth = tex->colorDepth1 != 32 ? 32 : 16;
            tex->tp.imageDataHeight = h;
            tex->tp.imageDataWidth = w;
        }
    }
    return ok;
}

int TEX_l_File_GetInfo(int key, tEngTexIntermediateStruct* tex, int paramsOnly)
{
    int16_t flags;

    flags = tex->flags & ~2 | 1;
    tex->flags = flags;
    if (paramsOnly)
        tex->flags = flags | 4;
    else
        tex->flags = flags & ~4;
    return TEX_l_File_Read(key, tex);
}

int TEX_l_File_GetContent(int key, tEngTexIntermediateStruct* tex)
{
    tex->flags = tex->flags & ~5 | 2;
    return TEX_l_File_Read(key, tex);
}

void TEX_Expand_XBy2(int* src, int* dest, int w, int h)
{
    int* in;
    int* out;

    out = &dest[2 * h * w];
    for (in = &src[h * w]; in > src; )
    {
        --in;
        out[-1] = *in;
        out[-2] = *in;
        out -= 2;
    }
}

void TEX_Compress8_Xo2(char* src, char* dest, int w, int h)
{
    unsigned int i;

    for (i = (unsigned int)(h * w) >> 1; i; --i)
    {
        *dest++ = *src;
        src += 2;
    }
}

void TEX_Compress8_Yo2(char* src, char* dest, int w, unsigned int h)
{
    unsigned int y;
    int x;

    for (y = h >> 1; y; --y)
    {
        for (x = w; x; --x)
            *dest++ = *src++;
        src += w;
    }
}

void TEX_Expand8_YBy2(char* src, char* dest, int w, int h)
{
    char* in;
    char* out;
    int x;
    int y;

    out = &dest[2 * h * w - 1];
    in = &src[h * w - 1];
    for (y = h; y; --y)
    {
        for (x = w; x; --x)
        {
            *out = *in;
            out[-w] = *in;
            --out;
            --in;
        }
        out -= w;
    }
}

void TEX_Compress4_Xo2(char* src, char* dest, int w, int h)
{
    unsigned int i;

    for (i = (unsigned int)(h * w) >> 2; i; --i)
    {
        // preserved: the second nibble is shifted signed, so a high bit sets the whole top nibble
        *dest++ = *src & 0xF0 | (src[1] >> 4);
        src += 2;
    }
}

void TEX_Compress4_Yo2(char* src, char* dest, unsigned int w, unsigned int h)
{
    unsigned int y;
    unsigned int x;

    for (y = h >> 1; y; --y)
    {
        for (x = w >> 1; x; --x)
            *dest++ = *src++;
        src += w >> 1;
    }
}

void TEX_Expand4_XBy2(char* src, char* dest, int w, int h)
{
    char* in;
    char* out;

    out = &dest[h * w];
    // preserved: the first store lands one byte past the destination, so the whole
    // walk is shifted by one and the first byte pair is never written
    for (in = &src[((unsigned int)(h * w) >> 1) - 1]; in > src; --in)
    {
        out[0] = (16 * *in) | *in & 0xF;
        out[-1] = ((uint8_t)*in >> 4) | *in & 0xF0;
        out -= 2;
    }
}

void TEX_Expand4_YBy2(char* src, char* dest, unsigned int w, int h)
{
    char* in;
    char* out;
    int stride;
    unsigned int x;
    int y;

    stride = w >> 1;
    out = &dest[h * w - 1];
    in = &src[((h * w) >> 1) - 1];
    for (y = h; y; --y)
    {
        for (x = w >> 1; x; --x)
        {
            *out = *in;
            out[-stride] = *in;
            --out;
            --in;
        }
        out -= stride;
    }
}

void TEX_Expand24_XBy2(char* src, char* dest, int w, int h)
{
    char* in;
    char* out;

    out = &dest[6 * h * w];
    for (in = &src[3 * h * w - 1]; in > src; in -= 3)
    {
        out -= 6;
        out[5] = in[0];
        out[4] = in[-1];
        out[3] = in[-2];
        out[2] = in[0];
        out[1] = in[-1];
        out[0] = in[-2];
    }
}

void TEX_Expand24_YBy2(char* src, char* dest, int w, int h)
{
    char* in;
    char* out;
    int stride;
    int x;
    int y;

    stride = 3 * w;
    out = &dest[2 * h * stride - 1];
    in = &src[h * stride - 1];
    for (y = h; y; --y)
    {
        for (x = stride; x; --x)
        {
            *out = *in;
            out[-stride] = *in;
            --out;
            --in;
        }
        out -= stride;
    }
}

int TEX_l_ConvertSizeFast(tEngTexIntermediateStruct* tex)
{
    unsigned int w;
    unsigned int h;
    unsigned int destW;
    unsigned int destH;
    unsigned int destPixels;
    char* src;

    w = tex->realWidth;
    if (TEX_ul_GetPower2Up(w) != w)
        return 0;
    destW = tex->realWidthResizeToThis;
    if (TEX_ul_GetPower2Up(destW) != destW)
        return 0;
    h = tex->realHeight;
    if (TEX_ul_GetPower2Up(h) != h)
        return 0;
    destH = tex->realHeightResizeToThis;
    if (TEX_ul_GetPower2Up(destH) != destH)
        return 0;
    destPixels = destW * destH;
    if (destPixels <= tex->realWidth * h)
    {
        src = 0;
    }
    else
    {
        src = tex->imgDataPtr;
        tex->imgDataPtr = TEX_l_AllocImgData(destPixels * tex->colorDepth1);
    }
    switch (tex->colorDepth1)
    {
    case 32u:
        for (; tex->realWidth > tex->realWidthResizeToThis; tex->realWidth >>= 1)
        {
            TEX_Compress_Xo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight > tex->realHeightResizeToThis; tex->realHeight >>= 1)
        {
            TEX_Compress_Yo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realWidth < tex->realWidthResizeToThis; tex->realWidth *= 2)
        {
            TEX_Expand_XBy2((int*)(src ? src : tex->imgDataPtr), (int*)tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight < tex->realHeightResizeToThis; tex->realHeight *= 2)
        {
            TEX_Expand_YBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        return 1;
    case 24u:
        for (; tex->realWidth > tex->realWidthResizeToThis; tex->realWidth >>= 1)
        {
            TEX_Compress24_Xo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight > tex->realHeightResizeToThis; tex->realHeight >>= 1)
        {
            TEX_Compress24_Yo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realWidth < tex->realWidthResizeToThis; tex->realWidth *= 2)
        {
            TEX_Expand24_XBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight < tex->realHeightResizeToThis; tex->realHeight *= 2)
        {
            TEX_Expand24_YBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        return 1;
    case 8u:
        for (; tex->realWidth > tex->realWidthResizeToThis; tex->realWidth >>= 1)
        {
            TEX_Compress8_Xo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight > tex->realHeightResizeToThis; tex->realHeight >>= 1)
        {
            TEX_Compress8_Yo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realWidth < tex->realWidthResizeToThis; tex->realWidth *= 2)
        {
            TEX_Expand8_XBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight < tex->realHeightResizeToThis; tex->realHeight *= 2)
        {
            TEX_Expand8_YBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        return 1;
    case 4u:
        for (; tex->realWidth > tex->realWidthResizeToThis; tex->realWidth >>= 1)
        {
            TEX_Compress4_Xo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight > tex->realHeightResizeToThis; tex->realHeight >>= 1)
        {
            TEX_Compress4_Yo2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realWidth < tex->realWidthResizeToThis; tex->realWidth *= 2)
        {
            TEX_Expand4_XBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        for (; tex->realHeight < tex->realHeightResizeToThis; tex->realHeight *= 2)
        {
            TEX_Expand4_YBy2(src ? src : tex->imgDataPtr, tex->imgDataPtr, tex->realWidth, tex->realHeight);
            src = TEX_l_FreeScaleSrc(src);
        }
        return 1;
    }
    return 0;
}

void TEX_Convert_32SwapRB(int* data, int num)
{
    int i;

    for (i = num; i; --i)
    {
        *data = *data & 0xFF00FF00 | *((uint8_t*)data + 2) | ((uint8_t)*data << 16);
        ++data;
    }
}

void TEX_Convert_24To32(char* data, int num)
{
    char* in;
    int* out;
    int i;

    in = &data[3 * num];
    out = (int*)&data[4 * num];
    for (i = num; i; --i)
    {
        in -= 3;
        --out;
        *out = (uint8_t)in[0] << 16 | (uint8_t)in[1] << 8 | (uint8_t)in[2] | 0xFF000000;
    }
}

void TEX_Convert_24To32SwapRB(char* data, int num)
{
    char* in;
    int* out;
    int i;

    in = &data[3 * num];
    out = (int*)&data[4 * num];
    for (i = num; i; --i)
    {
        in -= 3;
        --out;
        *out = (uint8_t)in[2] << 16 | (uint8_t)in[1] << 8 | (uint8_t)in[0] | 0xFF000000;
    }
}

int TEX_l_ConvertSize(tEngTexIntermediateStruct* src, tEngTexIntermediateStruct* dest)
{
    tTexScaleTap* cols;
    tTexScaleTap* rows;
    tTexScaleTap* col;
    tTexScaleTap* row;
    char* in;
    char* out;
    int srcBpp;
    int destBpp;
    int srcStride;
    int xStep;
    int yStep;
    unsigned int norm;
    unsigned int keep;
    uint8_t srcAlphaShift;
    uint8_t srcRedShift;
    uint8_t srcGreenShift;
    uint8_t srcBlueShift;
    uint8_t destAlphaShift;
    uint8_t destRedShift;
    uint8_t destGreenShift;
    uint8_t destBlueShift;
    unsigned int alpha;
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int weight;
    unsigned int rowWeight;
    unsigned int pixel;
    char* p;
    unsigned int sx;
    unsigned int sy;
    int x;
    int y;

    cols = malloc(dest->realWidth * sizeof(tTexScaleTap));
    rows = malloc(dest->realHeight * sizeof(tTexScaleTap));
    xStep = TEX_l_BuildScaleTaps(cols, src->realWidth, dest->realWidth);
    yStep = TEX_l_BuildScaleTaps(rows, src->realHeight, dest->realHeight);
    norm = xStep * yStep;

    srcAlphaShift = TEX_uc_GetMaskShift(src->alphaMask);
    srcRedShift = TEX_uc_GetMaskShift(src->redMask);
    srcGreenShift = TEX_uc_GetMaskShift(src->greenMask);
    srcBlueShift = TEX_uc_GetMaskShift(src->blueMask);
    destAlphaShift = TEX_uc_GetMaskShift(dest->alphaMask);
    destRedShift = TEX_uc_GetMaskShift(dest->redMask);
    destGreenShift = TEX_uc_GetMaskShift(dest->greenMask);
    destBlueShift = TEX_uc_GetMaskShift(dest->blueMask);
    // preserved: the bits kept from the destination pixel come from the channel
    // shifts and not from the channel masks
    keep = ~(destBlueShift | destGreenShift | destRedShift | destAlphaShift);

    srcBpp = src->colorDepth1 >> 3;
    destBpp = dest->colorDepth1 >> 3;
    srcStride = srcBpp * src->realWidth;
    in = src->imgDataPtr;
    out = dest->imgDataPtr;

    for (y = 0; y < dest->realHeight; y++)
    {
        row = &rows[y];
        for (x = 0; x < dest->realWidth; x++)
        {
            col = &cols[x];
            alpha = 0;
            red = 0;
            green = 0;
            blue = 0;
            for (sy = row->first; sy <= row->last; sy++)
            {
                rowWeight = TEX_l_TapWeight(row, sy);
                p = &in[sy * srcStride + col->first * srcBpp];
                for (sx = col->first; sx <= col->last; sx++)
                {
                    weight = rowWeight * TEX_l_TapWeight(col, sx);
                    pixel = *(uint32_t*)p;
                    alpha += weight * ((src->alphaMask & pixel) >> srcAlphaShift);
                    red += weight * ((src->redMask & pixel) >> srcRedShift);
                    green += weight * ((src->greenMask & pixel) >> srcGreenShift);
                    blue += weight * ((src->blueMask & pixel) >> srcBlueShift);
                    p += srcBpp;
                }
            }
            *(uint32_t*)out = keep & *(uint32_t*)out
                | dest->blueMask & ((blue / norm) << destBlueShift)
                | dest->greenMask & ((green / norm) << destGreenShift)
                | dest->redMask & ((red / norm) << destRedShift)
                | dest->alphaMask & ((alpha / norm) << destAlphaShift);
            out += destBpp;
        }
    }
    free(cols);
    free(rows);
    return 1;
}

void TEX_Hardware_Load(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* tex, int idx)
{
    int16_t bmpFlags;
    int bytes;
    int pixels;
    tEngTexIntermediateStruct resized;

    if ((tex->flags & 0x20) == 0)
    {
        bmpFlags = tex->bmpUnkb1;
        if ((bmpFlags & 0x20) != 0)
        {
            tle->texElemFlag = bmpFlags;
            tle->imgLink.ptr = tex->imgDataPtr;
            return;
        }
        GDI_f_Delay_AttachWorld_TextureCreate_GetContent = GDI_f_Delay_AttachWorld_TextureCreate_GetContent - TIM_f_Clock_TrueRead();
        if (tex->tp.type != 7 && !TEX_l_File_GetContent(tle->texKey, tex))
        {
            GDI_f_Delay_AttachWorld_TextureCreate_GetContent = TIM_f_Clock_TrueRead()
                + GDI_f_Delay_AttachWorld_TextureCreate_GetContent;
            return;
        }
        GDI_f_Delay_AttachWorld_TextureCreate_GetContent = TIM_f_Clock_TrueRead()
            + GDI_f_Delay_AttachWorld_TextureCreate_GetContent;
        GDI_f_Delay_AttachWorld_TextureCreate_Compress = GDI_f_Delay_AttachWorld_TextureCreate_Compress - TIM_f_Clock_TrueRead();
        if (tex->realWidth != tex->realWidthResizeToThis || tex->realHeightResizeToThis != tex->realHeight)
        {
            if ((dd->texManager.flags & 4) == 0 || !TEX_l_ConvertSizeFast(tex))
            {
                bytes = (tex->realWidthResizeToThis * tex->realHeightResizeToThis * (unsigned int)tex->colorDepth2) >> 3;
                memcpy(&resized, tex, sizeof(resized));
                resized.realWidth = resized.realWidthResizeToThis;
                resized.realHeight = resized.realHeightResizeToThis;
                resized.imgDataPtr = TEX_l_AllocImgData(bytes);
                memset(resized.imgDataPtr, 0, bytes);
                TEX_l_ConvertSize(tex, &resized);
                TEX_File_FreeDescription(tex);
                memcpy(tex, &resized, sizeof(tEngTexIntermediateStruct));
            }
        }
        GDI_f_Delay_AttachWorld_TextureCreate_Compress = TIM_f_Clock_TrueRead()
            + GDI_f_Delay_AttachWorld_TextureCreate_Compress;
        if (tex->colorDepth1 == tex->colorDepth2)
        {
            GDI_f_Delay_AttachWorld_TextureCreate_Convert32Colors = GDI_f_Delay_AttachWorld_TextureCreate_Convert32Colors - TIM_f_Clock_TrueRead();
            if ((dd->texManager.flags & 0x100000) != 0 && tex->colorDepth1 == 32)
                TEX_Convert_32SwapRB((int*)tex->imgDataPtr, tex->realWidth * tex->realHeight);
            GDI_f_Delay_AttachWorld_TextureCreate_Convert32Colors = TIM_f_Clock_TrueRead() + GDI_f_Delay_AttachWorld_TextureCreate_Convert32Colors;
        }
        else
        {
            pixels = tex->realWidth * tex->realHeight;
            GDI_f_Delay_AttachWorld_TextureCreate_Convert24To32 = GDI_f_Delay_AttachWorld_TextureCreate_Convert24To32 - TIM_f_Clock_TrueRead();
            if (tex->colorDepth1 == 24 && tex->colorDepth2 == 32)
            {
                if ((dd->texManager.flags & 0x100000) != 0)
                    TEX_Convert_24To32(tex->imgDataPtr, pixels);
                else
                    TEX_Convert_24To32SwapRB(tex->imgDataPtr, pixels);
            }
            GDI_f_Delay_AttachWorld_TextureCreate_Convert24To32 = TIM_f_Clock_TrueRead() + GDI_f_Delay_AttachWorld_TextureCreate_Convert24To32;
        }
    }
    else
    {
        tex->imgDataPtr = TEX_l_AllocImgData((tex->realWidthResizeToThis * tex->realHeightResizeToThis * (unsigned int)tex->colorDepth2) >> 3);
    }
    // preserved: the width is taken from the resized height and the height from the resized width
    tle->normalData.height = tex->realWidthResizeToThis;
    tle->normalData.width = tex->realHeightResizeToThis;
    if ((dd->texManager.flags & 8) == 0)
        tex->tp.flagsUnkAndProceduralUnkb1 &= ~0x8;
    if ((tex->tp.flagsUnkAndProceduralUnkb1 & 0x200) == 0
        || (dd->texManager.flags & 0x200000) == 0
        || !dd->SetTexturePalette(dd, tle, tex, idx))
    {
        dd->PaletteLoad(dd, tle, tex, idx);
        TEX_File_FreeDescription(tex);
    }
}

void TEX_Convert_4To32SwapRB(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;

    for (i = num; i; i -= 2)
    {
        dest[0] = TEX_l_SwapRB(paletteData[(uint8_t)*src >> 4]);
        dest[1] = TEX_l_SwapRB(paletteData[*src & 0xF]);
        dest += 2;
        ++src;
    }

}

void TEX_Convert_8To32SwapRB(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;

    for (i = num; i; --i)
    {
        *dest = TEX_l_SwapRB(paletteData[(uint8_t)*src]);
        ++src;
        ++dest;
    }
}

void TEX_Convert_4To32(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;
    for (i = num; i; i -= 2)
    {
        *dest = paletteData[*src >> 4];
        dest += 2;
        *(dest - 1) = paletteData[*src++ & 0xF];
    }
}

void TEX_Convert_8To32(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;

    for (i = num; i; --i)
        *dest++ = paletteData[*src++];

}

int TEX_l_MemoryNeeded(tEngTexIntermediateStruct* texIM)
{
    int i, tmp, ret;

    ret = (texIM->realHeightResizeToThis * texIM->realWidthResizeToThis * texIM->colorDepth2) / 8;

    if ((texIM->tp.flagsUnkAndProceduralUnkb1 & 8) != 0)
    {
        tmp = (texIM->realHeightResizeToThis * texIM->realWidthResizeToThis * texIM->colorDepth2) / 8;
        if (texIM->realWidthResizeToThis < texIM->realHeightResizeToThis)
            texIM->realHeightResizeToThis = texIM->realWidthResizeToThis;
        for (i = texIM->realHeightResizeToThis / 2; i; i /= 2)
        {
            tmp /= 4;
            ret += tmp;
        }

       while(tmp > 4) {
            tmp /= 2;
            ret += tmp;
        }
    }
    return ret;
}

BOOL TEX_l_DimensionIsGood(tEngTexIntermediateStruct* texIM, int minSize, int maxSize, int onlySquareDimensionIsAccepted, tEngTexIntermediateStruct* texIMRet)
{
    uint16_t heightPow2;
    uint16_t widthPow2;

    memcpy(texIMRet, texIM, sizeof(tEngTexIntermediateStruct));
    texIMRet->descriptionPtr = 0;
    texIMRet->imgDataPtr = NULL;

    heightPow2 = TEX_ul_GetPower2Up(texIMRet->realHeight);
    texIMRet->realHeight = heightPow2;
    if (heightPow2 < minSize)
        texIMRet->realHeight = minSize;
    if (texIMRet->realHeight > maxSize)
        texIMRet->realHeight = maxSize;
    widthPow2 = TEX_ul_GetPower2Up(texIMRet->realWidth);
    texIMRet->realWidth = widthPow2;
    if (widthPow2 < minSize)
        texIMRet->realWidth = minSize;
    if (texIMRet->realWidth > maxSize)
        texIMRet->realWidth = maxSize;
    if (!onlySquareDimensionIsAccepted)
    {
        if (texIMRet->realWidth >= texIMRet->realHeight)
            texIMRet->realHeight = texIMRet->realWidth;
        else
            texIMRet->realWidth = texIMRet->realHeight;
    }
    return texIM->realHeight == texIMRet->realHeight && texIM->realWidth == texIMRet->realWidth;
}

void TEX_Manager_ComputeCompression(struct sTexManagerStruct* mgr, struct sEngTexIntermediateStruct* tims, int timsIdx, char paletted)
{
    tEngTexIntermediateStruct* end;
    tEngTexIntermediateStruct* tim;
    int* backup;
    int* budget;
    int* uncompressed;
    int onlySquare;
    int isPaletted;
    int classIdx;
    int texClass;
    int limit;
    int mark;
    uint16_t w;
    uint16_t h;
    tEngTexIntermediateStruct wanted;

    end = &tims[timsIdx];
    if (paletted)
    {
        uncompressed = &mgr->texMemoryNeededUncompressedCase1;
        backup = &mgr->texMemoryNeededCompressedCase1Backup;
        budget = &mgr->texMemoryNeededCompressedCase1;
    }
    else
    {
        uncompressed = &mgr->texMemoryNeededUncompressedCase2;
        backup = &mgr->texMemoryNeededCompressedCase2Backup;
        budget = &mgr->texMemoryNeededCompressedCase2;
    }
    onlySquare = mgr->flags & 1;
    *uncompressed = 0;
    for (tim = tims; tim < end; tim++) // ezzel kezdeni vmit, todo
    {
        if ((tim->flags & 0x10) != 0)
            continue;
        isPaletted = (tim->tp.flagsUnkAndProceduralUnkb1 & 0x200) != 0 && (mgr->flags & 0x200000) != 0;
        if (isPaletted != paletted)
            continue;
        tim->realHeightResizeToThis = tim->realHeight;
        tim->realWidthResizeToThis = tim->realWidth;
        tim->colorDepth2 = tim->colorDepth1;
        if (!TEX_l_DimensionIsGood(tim, mgr->i3, mgr->i2, onlySquare, &wanted))
        {
            tim->realHeightResizeToThis = wanted.realHeight;
            tim->realWidthResizeToThis = wanted.realWidth;
        }
        if (tim->colorDepth1 == 24 && (mgr->flags & 0x200) == 0
            || tim->colorDepth1 == 16 && (mgr->flags & 0x400) == 0
            || (tim->colorDepth1 == 8 || tim->colorDepth1 == 4) && tim->tp.type == 7)
        {
            tim->colorDepth2 = 32;
        }
        *uncompressed += TEX_l_MemoryNeeded(tim);
    }
    *budget = *backup - *uncompressed;
    if (*budget >= 0)
        return;

    classIdx = 0;
    mark = *budget;
    while (1)
    {
        texClass = al_QualityCompressionOrder[classIdx];
        for (tim = tims; tim < end; tim++)
        {
            if ((tim->flags & 0x10) != 0 || (tim->tp.flagsUnkAndProceduralUnkb1 & 7) != texClass)
                continue;
            limit = al_QualityCompressionLimit[texClass];
            if (tim->realHeightResizeToThis <= limit && tim->realWidthResizeToThis <= limit)
                continue;

            *budget += TEX_l_MemoryNeeded(tim);
            h = tim->realHeightResizeToThis;
            w = tim->realWidthResizeToThis;
            if (h > w)
            {
                tim->realHeightResizeToThis = h >> 1;
            }
            else if (h == w)
            {
                tim->realHeightResizeToThis = h >> 1;
                tim->realWidthResizeToThis = w >> 1;
            }
            else
            {
                tim->realWidthResizeToThis = w >> 1;
            }
            *budget -= TEX_l_MemoryNeeded(tim);
            if (*budget > 0)
                break;
        }
        if (++classIdx == 21)
        {
            classIdx = 0;
            if (*budget == mark)
                return;
            mark = *budget;
        }
        if (*budget >= 0)
            return;
    }
}

int16_t TEX_w_List_AddPalette(tEngTexListStruct* list, int key)
{
    int16_t result;
    int num;
    tEngPaletteElem* pal;

    if (key == -1)
        return -1;
    num = list->paletteNum;
    if (num)
    {
        pal = TEX_pst_List_FindPalette(list, key);
        if (pal)
            return pal->paletteID;
    }
    if (num == list->unk1)
    {
        list->unk1 += 256;
        list->palettes = (tEngPaletteElem*)TEX_l_GrowAlignedList(list->palettes, list->unk1,
            sizeof(tEngPaletteElem) * list->unk1);
    }
    pal = &list->palettes[list->paletteNum++];
    pal->key = 0;
    pal->paletteData = 0;
    pal->flags = 0;
    pal->b1 = 0;
    pal->paletteID = 0;
    pal->key = key;
    result = (list->paletteNum & 0xffff) - 1;
    pal->paletteID = result;
    return result;
}

int16_t TEX_w_Manager_ChooseGoodInTex(struct sTexManagerStruct* mgr, struct sEngTexIntermediateStruct* tims)
{
    int i;
    int flagsHas0x7000;
    int slot;
    int16_t result;
    int* paletteID;

    i = 0;
    flagsHas0x7000 = mgr->flags & 0x7000;
    tims->texIdx = -1;
    tims->probablyType = 0; // TODO FIXME az biztos jó?
    tims->paletteID = -1;
    tims->textureID = -1;
    do
    {
        slot = mgr->keyLoadOrderIDs[i];
        if (slot == -1)
            return -1;
        if (tims->elems[slot].paletteID != -1)
            break;
        if (tims->elems[slot].palettedTextureID != -1)
            break;
        if (tims->elems[slot].textureID != -1)
            break;
        ++i;
    } while (i < 4);
    if (i == 4)
        return -1;

    paletteID = &tims->elems[slot].paletteID;

    if (tims->elems[slot].palettedTextureID == -1
        || *paletteID == -1
        || tims->elems[slot].textureID != -1 && flagsHas0x7000 != 0x7000)
    {
        if (tims->elems[slot].textureID != -1)
            tims->texIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, tims->elems[slot].textureID);
        result = tims->texIdx;
        tims->probablyType = 2;
    }
    else
    {
        tims->paletteID = TEX_w_List_AddPalette(&TEX_gst_GlobalList, *paletteID);
        tims->textureID = TEX_w_List_AddTexture(&TEX_gst_GlobalList, tims->elems[slot].palettedTextureID);
        tims->probablyType = (flagsHas0x7000 == 0x7000 ? 0 : 4) | 1;
        if ((tims->tp.flagsUnkAndProceduralUnkb1 & 0x100) != 0 && *paletteID != -1)
        {
            TEX_gst_GlobalList.palettes[tims->paletteID].flags |= 0x18; //LOBYTE(ADJ(v8)->unk3InCarteUpdatePalette) |= 0x18u;
        }
        result = tims->textureID;
    }
    return result;
}

void TEX_File_LoadPalette(tEngPaletteElem* pal, int swapRB)
{
    int pos;
    unsigned int allocSize;
    uint8_t* out;
    uint8_t* in;
    int left;
    int size;
    char* data;

    pos = BIG_ul_SearchKeyToPos(pal->key);
    if (pos == -1)
        return;
    data = BIG_pc_ReadFileTmp(pos, &size);
    if (size == 1024)
    {
        pal->flags |= 2;
        allocSize = 1024;
    }
    else if (size == 768)
    {
        allocSize = 1024;
    }
    else if (size == 64)
    {
        pal->flags |= 3;
        allocSize = 64;
    }
    else if (size == 48)
    {
        pal->flags |= 1;
        allocSize = 64;
    }
    else
    {
        pal->flags |= 4;
        return;
    }
    pal->paletteData = (uint8_t*)MEM_p_AllocTmp(allocSize);
    MEM_gst_MemoryInfo.usedMemoryByTextures += *((uint32_t*)pal->paletteData - 1); // TODO FIXME
    LOA_ReadCharArray(&data, (char*)pal->paletteData, size, 1);
    left = size;
    if ((pal->flags & 2) != 0)
    {
        if (swapRB)
        {
            out = &pal->paletteData[4 * (allocSize >> 2)];
            while (left)
            {
                left -= 4;
                out -= 4;
                *(uint32_t*)out = (uint32_t)out[2] | (uint32_t)out[1] << 8 | (uint32_t)out[0] << 16 | (uint32_t)out[3] << 24;
            }
        }
    }
    else
    {
        out = &pal->paletteData[4 * (allocSize >> 2)];
        in = &pal->paletteData[size];
        if (swapRB)
        {
            while (left)
            {
                left -= 3;
                in -= 3;
                out -= 4;
                *(uint32_t*)out = (uint32_t)in[2] | (uint32_t)in[1] << 8 | (uint32_t)in[0] << 16 | 0xFF000000;
            }
        }
        else
        {
            while (left)
            {
                left -= 3;
                in -= 3;
                out -= 4;
                *(uint32_t*)out = (uint32_t)in[0] | (uint32_t)in[1] << 8 | (uint32_t)in[2] << 16 | 0xFF000000;
            }
        }
    }
}

void TEX_l_ConvertSize_24To32(struct sEngTexIntermediateStruct* src, struct sEngTexIntermediateStruct* dest)
{
    int destW;
    int srcW;
    uint32_t* out;
    double xPos;
    double yPos;
    float xStep;
    float yStep;
    char* row;
    int x;
    int y;
    int offsets[1024];

    destW = dest->realWidth;
    srcW = src->realWidth;
    out = (uint32_t*)dest->imgDataPtr;
    xPos = 0.0;
    xStep = (float)((double)srcW / (double)destW);
    for (x = 0; x < destW; x++)
    {
        offsets[x] = 3 * (unsigned int)xPos;
        xPos = xPos + xStep;
    }
    yStep = (float)((double)src->realHeight / (double)dest->realHeight);
    yPos = 0.0;
    for (y = 0; y < dest->realHeight; y++)
    {
        row = &src->imgDataPtr[3 * srcW * (unsigned int)yPos];
        for (x = 0; x < destW; x++)
        {
            *out = *(uint32_t*)&row[offsets[x]] & 0xFFFFFF;
            ++out;
        }
        yPos = yPos + yStep;
    }
}
