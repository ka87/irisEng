#pragma once
#include "common.h"
#include "bink.h"

typedef struct sJpegLibIntf
{
    void* indata;
    int width;
    int height;
    void* outdata;
    int arith_coding;
    int CCIR601_sampling;
    int smoothing_factor;
    int quality;
    int i9;
    int error;
    int bytesNumPerPixel;
} tJpegLibIntf;

typedef struct sProceduralIntf
{
    int id;
    void * (*Load)(char*, int, int, int);
    void (*Free)(struct sTexProceduralStruct* a1);
    void (*Disrupt)(struct sTexProceduralStruct* a1);
    BOOL (*Update)(struct sTexProceduralStruct* a1);
} tProceduralIntf;

typedef struct sTexProceduralStruct
{
    int16_t elemIdx;
    int16_t s2;
    uint32_t* proceduralPicData;
    int16_t width;
    int16_t height;
    tProceduralIntf* proceduralIntf;
    void* proceduralTexPtr; // fIXME
}  tTexProceduralStruct;

typedef struct sTexAnimatedInnerElem
{
    int key;
    int16_t texTime;
    int16_t texIdx;
} tTexAnimatedInnerElem;

typedef struct sTexAnimatedElem
{
    int16_t id;
    int16_t flags;
    uint8_t texNum;
    uint8_t currentTex;
    int16_t remainingTime;
    uint8_t b3;
    uint8_t animationFlags;
    uint8_t gap[2];
    tTexAnimatedInnerElem* texPtr;
} tTexAnimatedElem;

typedef struct sEngPaletteElem
{
    int key;
    uint8_t* paletteData;
    uint8_t flags;
    uint8_t b1;
    int16_t paletteID;
} tEngPaletteElem;

typedef struct sEngTexListNormalElemData {
    int16_t width;
    int16_t height;
} tEngTexListNormalElemData;

typedef struct sEngTexListPalettedElemData {
    int16_t textureID;
    int16_t paletteID;
} tEngTexListPalettedElemData;

typedef struct sEngTexListProceduralElemData {
    int16_t textureID;
    int16_t paletteID;
} tEngTexListProceduralElemData;

typedef struct sEngTexListAnimatedElemData {
    int16_t dummy;
    int16_t textureID;
} tEngTexListAnimatedElemData;

typedef struct sEngTexListImgLinkElemData {
    void* ptr;
} tEngTexListImgLinkElemData;


typedef struct sEngTexListElemStruct
{
    int texKey;
    union {
        tEngTexListPalettedElemData palettedData;
        tEngTexListProceduralElemData proceduralData;
        tEngTexListAnimatedElemData animatedData;
        tEngTexListNormalElemData normalData;
        tEngTexListImgLinkElemData imgLink;
    };    
    int16_t texElemFlag;
    int16_t textElemIdx;
} tEngTexListElemStruct;

typedef struct sEngTexListStruct
{
    int maxTexNum;
    int actTexElemNum;
    struct sEngTexListElemStruct* texElems;
    int unk1;
    int paletteNum;
    struct sEngPaletteElem* palettes;
} tEngTexListStruct;

typedef struct sBumpMapElem
{
    int16_t s1;
    uint8_t b1;
    uint8_t b2;
    int i2;
    char* ptr;
    int i4;
} tBumpMapElem;

typedef struct sBumpMapList
{
    int num;
    int i2;
    tBumpMapElem** ptr;
} tBumpMapList;

typedef struct sMatSpriteElem
{
    int i1;
    int i2;
    int texKey;
    int16_t s1;
    int16_t texIdx;
    float f1;
    float f2;
    int flags;
    float f3;
    float f4;
    float f5;
    float f6;
    int bumpMapID;
    tBumpMapElem* bumpMapPtr;
    float f7;
    int i7;
    int i8;
} tMatSpriteElem;

typedef struct sMatSpriteList
{
    int idx;
    tMatSpriteElem* elem;
    struct sMatSpriteList* prev;
} tMatSpriteList;


typedef struct sEngTexIntermediateElemStruct
{
    int palettedTextureID;
    int paletteID;
    int textureID;
} tEngTexIntermediateElemStruct;

/*
typedef struct sEngTexIntermediateStructTexAnimatedParams
{
    int extraDataByType;
    void* proceduralOrAnimedTexPtr;
    int unkOlyanminthaFlagsLenne;
    int unkkkkkkk;
} tEngTexIntermediateStructTexAnimatedParams;
*/


typedef struct sEngTexIntermediateStructTexMatSpriteParams
{
    tMatSpriteElem* matSprite;
    int dummy1;
    int dummy2;
    int dummy3;
} tEngTexIntermediateStructTexMatSpriteParams;


typedef struct sEngTexIntermediateStructTexAnimatedParams
{
    int num;
    tTexAnimatedInnerElem* aiePtr;
    int flags;
    int dummy;
} tEngTexIntermediateStructTexAnimatedParams;

typedef struct sEngTexIntermediateStructTexMagicParams
{
    int fontDescriptorKey;
    int magic1;
    int magic2;
    int magic3;
} tEngTexIntermediateStructTexMagicParams;

typedef struct sEngTexIntermediateStructTexProceduralParams
{
    int interfaceIdx;
    void* interfacePtr;
    int flags;
    int dummy;
} tEngTexIntermediateStructTexProceduralParams;

typedef struct sEngTexIntermediateStructTexParams
{
    int uuuu;
    int16_t flagsUnkAndProceduralUnkb1;
    uint8_t type;
    uint8_t imageDataColorDepth;
    uint16_t imageDataWidth;
    uint16_t imageDataHeight;
    int unk;
    union {
        tEngTexIntermediateStructTexAnimatedParams animatedParams;
        tEngTexIntermediateStructTexMagicParams magicParams;
        tEngTexIntermediateStructTexProceduralParams proceduralParams;
        tEngTexIntermediateStructTexMatSpriteParams matSpriteParams;
    };
} tEngTexIntermediateStructTexParams;


// TODO ezen meg van mit dolgozni
typedef struct sEngTexIntermediateStruct
{
    int16_t flags;
    int16_t bmpUnkb1;
    uint16_t realWidth;
    uint16_t realHeight;
    uint8_t gapa[4];
    uint16_t realWidthResizeToThis;
    uint16_t realHeightResizeToThis;
    uint8_t colorDepth1;
    uint8_t colorDepth2;
    uint8_t tgaUnk1b;
    uint8_t __Pad;
    int16_t tgaUnkS1;
    uint8_t gape[1];
    uint8_t unkFlagForTextureTargets;
    int redMask;
    int greenMask;
    int blueMask;
    int alphaMask;
    int descriptionPtr;
    char* imgDataPtr;
    int tgaDataStartPtr;
    int tgaDataEndPtr;
    struct sEngTexIntermediateElemStruct elems[4];
    int16_t textureID;
    int16_t paletteID;
    int16_t texIdx;
    int16_t probablyType;
    tEngTexIntermediateStructTexParams tp;
} tEngTexIntermediateStruct;

typedef struct sTexWeirdBMPHeader
{
    uint8_t gap[18];
    int16_t width;
    uint8_t gap2[2];
    int16_t height;
    uint8_t gap3[4];
    uint8_t colorDepth;
    uint8_t gap1[25];
} tTexWeirdBMPHeader;

typedef struct sTexProcWater
{
    int16_t* ptr1;
    int16_t* ptr2;
    unsigned int values[256];
    uint8_t gap[3072];
} tTexProcWater;

typedef struct sTexProcPlasma
{
    uint8_t* ptr1;
    uint8_t* ptr2;
    unsigned int values[256];
    uint8_t gap[3076];
    float f1;
    float f2;
} tTexProcPlasma;

typedef struct sTexProcPhoto
{
    char b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
    int x;
    float f1;
    float remainingTime;
    int16_t sa[8];
} tTexProcPhoto;


// Bink
typedef struct sBinkUnkStruct
{
    char* blockDataStartPtr;
    int stateMachine2MeansBusy;
    struct sBinkUnkStruct* nextDataBlock;
    int actSlotID;
} tBinkUnkStruct;

typedef struct sBinkIFPtr
{
    HBINK binkHandle;
    int binkVideoKey;
    struct sTexProceduralStruct* proceduralStructPtr;
    int filePosInBF;
    int size;
    int dataBlockStartCurrOffset;
    int videofilesize;
    struct sBinkUnkStruct* linkedListStart;
    struct sBinkUnkStruct* actualElemInLinkedList;
    char* dataAndLinkedList;
} tBinkIFPtr;

typedef struct stTexProcMpegSub
{
    int bufferSize;
    int width;
    int height;
    float f3;
    int i4;
    int i5;
    int (*ReadNext)(void*);
    void (*Start)();
    uint8_t gap2[4];
    int ii1;
    int ii2;
    int ii3;
    int binkKey;
    tBinkIFPtr* binkIFPtr;
    int ii5;
} tTexProcMpegSub;

typedef struct sTexProcMpeg
{
    int key;
    int pos;
    int startOffset;
    int endOffset;
    int flags;
    int i6;
    int i7;
    int i8;
    int i9;
    int i10;
    int i11;
    tTexProcMpegSub sub;
} tTexProcMpeg;

typedef struct sTexProFire
{
    uint8_t* ptr;
    uint8_t* ptr2;
    int arr[256];    
    uint8_t gap[3072];
    int unknum;
} tTexProFire;


int jpgHightPointer;
int TEX_gi_NbProcedural;
tTexProceduralStruct* TEX_gdst_Procedural;
int TEX_gi_NbAnimated;
int TEX_gi_NbAnimatedMax;
tTexAnimatedElem* TEX_gdst_Animated;
tEngTexListStruct TEX_gst_GlobalList;

tMatSpriteList* MAT_SPRT_AllSprt;
tBumpMapList MAT_gst_BumpMapList;

// Bink
tBinkIFPtr* BINK_IF_PTR_GLOBAL;
tTexProcMpeg* TEXPRO_pst_CurrentMpeg;

int TEXPRO_Cos[256];

float sf_TimeLeft_218;

int gi_NumES;
extern int gi_LastES;
struct sGameObjectMain* gap_ES[100];
int gau_ES[100];

float GDI_f_Delay_AttachWorld_TextureCreate_GetContent;
float GDI_f_Delay_AttachWorld_TextureCreate_Compress;
float GDI_f_Delay_AttachWorld_TextureCreate_Convert32Colors;
float GDI_f_Delay_AttachWorld_TextureCreate_Convert24To32;
float GDI_f_Delay_AttachWorld_TextureManager;
float GDI_f_Delay_AttachWorld_TextureCreate;
float GDI_f_Delay_AttachWorld_TextureUsed;
float GDI_f_Delay_AttachWorld_TextureInfos;
int dword_B09A28;
int dword_B09A24;

void PreLoadTex_Finished(int* a1);
void PreLoadTexReset();
void JpegWrite(tJpegLibIntf* a1);

void TEX_Compress24_Xo2(char* a1, char* a2, int a3, int a4);
void TEX_Compress24_Yo2(char* a1, char* a2, int a3, unsigned int a4);
void TEX_Procedural_Reinit(struct sGdi_DisplayData* a1);
void TEX_Procedural_Update(struct sGdi_DisplayData* a1);
void TEX_Anim_Reinit();
void TEX_Anim_Update();
void ResetPreloadTexAll();
struct sEngTexListElemStruct* TEX_pst_List_FindTexture(struct sEngTexListStruct* a1, int key);
int16_t TEX_w_List_AddTexture(tEngTexListStruct* a1, int key);
void TEX_List_Free(tEngTexListStruct* a1);
void MAT_SpriteGen_Close();
void TEX_Manager_Init(struct sTexManagerStruct* a1);
int TEX_l_File_LoadProcedural(char* a1, tEngTexIntermediateStruct* a2, int readPtr);
void TEX_Procedural_Add(int16_t elemIdx, int a2, int16_t a3, void* a4);
void TEX_Procedural_AfterLoad(struct sGdi_DisplayData* a1);
int TEX_l_File_GetContent(int a1, tEngTexIntermediateStruct* a2);
void TEX_Convert_4To32SwapRB(int* a1, unsigned char* a2, int* a3, int a4);
void TEX_Convert_8To32SwapRB(int* a1, unsigned char* a2, int* a3, int a4);
void TEX_Convert_4To32(int* a1, unsigned char* a2, int* a3, int a4);
void TEX_Convert_8To32(int* a1, unsigned char* a2, int* a3, int a4);
int16_t TEX_w_Manager_ChooseGoodInTex(struct sTexManagerStruct* a1, struct sEngTexIntermediateStruct* tims);
struct sTexAnimatedElem* TEX_pst_Anim_Add(int16_t id, char texNum, int16_t flags, struct sTexAnimatedInnerElem* data);
void TEX_Hardware_Load(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* a2, int idx);
void TEX_Manager_ComputeCompression(struct sTexManagerStruct* a1, struct sEngTexIntermediateStruct* tims, int timsIdx, char a4);
void TEX_Manager_Reinit(struct sTexManagerStruct* a1);
void TEX_File_DevelopParams(tEngTexIntermediateStruct* a1);
void TEX_File_LoadPalette(tEngPaletteElem* a1, int a2);
int TEX_l_File_GetInfo(int a1, tEngTexIntermediateStruct* a2, int a3);
void PC_VideoStreamCreate_Bink(tBinkIFPtr* a1, int a2);
void TEX_File_FreeDescription(struct sEngTexIntermediateStruct* a1);
void TEX_l_ConvertSize_24To32(struct sEngTexIntermediateStruct* a1, struct sEngTexIntermediateStruct* a2);
int TEX_l_ConvertPF(struct sEngTexIntermediateStruct* a1, struct sEngTexIntermediateStruct* a2);
int TEX_l_File_LoadJpeg(char* a1, struct sEngTexIntermediateStruct* a2, size_t a3);
void TEX_Procedural_Init();
void TEX_List_Init(struct sEngTexListStruct* a1);
struct sTexProceduralStruct* TEX_pst_Procedural_Get(int16_t a1);
void TEXPRO_Photo_SetMapGao(struct sTexProceduralStruct* a1, struct sGameObjectMain* a2);
void TEX_Procedural_UpdateWithAnother(struct sTexProceduralStruct* a1, struct sTexProceduralStruct* a2);
struct sTexAnimatedElem* TEX_pst_Anim_Get(int16_t a1);