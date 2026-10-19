#include "../memory.h"
#include "../loading.h"
#include "../textures.h"
#include "../modifiers/shadowmodifier.h"
#include "geomaterial.h"

tGEOMAT MAT_gst_DefaultSingleMaterial = {
    .intf = NULL,
    .usageCount = 0,
    .multiMaterialNumIfType4ThenAmbiantColor = 0xffffffff,
    .multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT**)0xffffffff,
    .solidVertexColor = 0xffffffff,
    .unkFloat1 = 0.0f,
    .unkFloat2 = 1.0f,
    .negativeZeroOrOneUnknown = 0,
    .idxOrPtr = 0,
    .validityFlag = 0
};

void MAT_FreeMultiMaterial(struct sGEOMAT* a1)
{
    int v1;
    tGEOMAT** v2;
    tGEOMAT** i;

    if (a1->intf->id == 4)
    {
        v1 = a1->multiMaterialNumIfType4ThenAmbiantColor;
        if (v1)
        {
            v2 = a1->multiMaterialPtrIfType4ThenDiffuseColor;
            for (i = &v2[v1]; v2 < i; ++v2)
            {
                if (*v2)
                {
                    (*v2)->intf->AddRef((tGroGeneric*)*v2, -1);
                    (*v2)->intf->Destroy((tGroGeneric*)*v2);
                }
            }
            MEM_Free(a1->multiMaterialPtrIfType4ThenDiffuseColor);
            a1->multiMaterialNumIfType4ThenAmbiantColor = 0;
        }
    }
}

void MAT_FreeMultiTexture(struct sGEOMAT* a1)
{
    tMultiTextureElemStruct* v1;
    tMultiTextureElemStruct* v2;

    if (a1->intf->id == 5)
    {
        v1 = (tMultiTextureElemStruct*)a1->idxOrPtr;
        if (v1)
        {
            do
            {
                v2 = v1->nextTextureKey;
                MEM_Free(v1);
                v1 = v2;
            } while (v2);
        }
    }
}

void MAT_DestroyMaterial(struct sGroGeneric* a1)
{
    if (a1)
    {
        if (a1->usageCount <= 0)
        {
            MAT_FreeMultiMaterial((tGEOMAT*)a1);
            MAT_FreeMultiTexture((tGEOMAT*)a1);
            //nullsub_32();
            LOA_DeleteAddress((int)a1);
            MEM_Free(a1);
        }
    }
}

void MAT_Validate_Single(struct sGEOMAT* a1)
{
    double v1;
    int v2;

    v1 = a1->unkFloat2;
    a1->validityFlag = 0x11;
    if (v1 < 0.98000002
        || (v2 = a1->idxOrPtr, v2 != -1) && (TEX_gst_GlobalList.texElems[v2].texElemFlag & 8) != 0)
    {
        a1->validityFlag = 0xD1;
    }
}

tGEOMAT* MAT_allocGRO_Single_sub_4704E0()
{
    tGEOMAT* v0;

    v0 = (tGEOMAT*)MEM_p_Alloc(sizeof(tGEOMAT));
    memset(v0, 0, sizeof(tGEOMAT));
    GRO_Struct_Init((tGroGeneric*)v0, 3);
    MAT_Validate_Single(v0);
    return v0;
}

struct sGroGeneric* MAT_pst_CreateSingle()
{
    return (tGroGeneric*)MAT_allocGRO_Single_sub_4704E0();
}

struct sGroGeneric* MAT_pst_CreateSingleFromBuffer(struct sGroGeneric *a1, char **a2, tWorld *world)
{
  tGEOMAT *v3;
  tEngTexListElemStruct *v4;

  v3 = (tGEOMAT *)MEM_p_Alloc(sizeof(tGEOMAT));
  memset(v3, 0, sizeof(tGEOMAT));
  GRO_Struct_Init((tGroGeneric*)v3, a1->intf->id);
  v3->multiMaterialNumIfType4ThenAmbiantColor = LOA_ReadLong(a2, 0, 1);
  v3->multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT **)LOA_ReadLong(a2, 0, 1);
  v3->solidVertexColor = LOA_ReadLong(a2, 0, 1);
  v3->unkFloat1 = LOA_ReadFloat(a2, 0, 1);
  v3->unkFloat2 = LOA_ReadFloat(a2, 0, 1);
  v3->negativeZeroOrOneUnknown = LOA_ReadLong(a2, 0, 1);
  v3->idxOrPtr = LOA_ReadLong(a2, 0, 1); // ha nem idx akkor key
  v3->validityFlag = LOA_ReadLong(a2, 0, 1);
  if ( v3->idxOrPtr != -1 )
  {
    v4 = TEX_pst_List_FindTexture(&TEX_gst_GlobalList, v3->idxOrPtr);
    if ( v4 )
      v3->idxOrPtr = v4->textElemIdx;
    else
      v3->idxOrPtr = TEX_w_List_AddTexture(&TEX_gst_GlobalList, v3->idxOrPtr);
  }
  if ( v3->negativeZeroOrOneUnknown > 1u )
    v3->negativeZeroOrOneUnknown = 0;
  MAT_Validate_Single(v3);
  return (tGroGeneric*)v3;
}

int MAT_GetValidatedMask_MTT(tGEOMAT* geomat, int idx_ununsed)
{
    return geomat->validityFlag;
}

int MAT_GetValidatedMask_MSM(tGEOMAT* geomat, int idx)
{
    if (idx == -1)
        return geomat->solidVertexColor;

    if (idx > geomat->multiMaterialNumIfType4ThenAmbiantColor - 1)
        idx = geomat->multiMaterialNumIfType4ThenAmbiantColor - 1;

    return geomat->multiMaterialPtrIfType4ThenDiffuseColor[idx]->intf->GetValidatedMask(geomat->multiMaterialPtrIfType4ThenDiffuseColor[idx], 0);
}

void MAT_AllocMultiMaterial(tGEOMAT* a1, int num)
{
    int v2;

    if (a1->intf->id == 4)
    {
        v2 = num;
        if (num > 255)
            v2 = 0;
        a1->multiMaterialNumIfType4ThenAmbiantColor = v2;
        if (v2)
            a1->multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT**)MEM_p_Alloc(4 * v2);
        else
            a1->multiMaterialPtrIfType4ThenDiffuseColor = NULL;
    }
}

struct sGroGeneric* MAT_pst_CreateMultiFromBuffer(struct sGroGeneric* a1, char** a2, tWorld* a3)
{
    tGEOMAT* v3;
    int v4;
    int i;
    int v6;

    WOR_gpst_WorldToLoadIn = a3;
    v3 = (tGEOMAT*)MEM_p_Alloc(20); // TODO FIXME ez nem a rendes tGEOMAT struktúra, csinálni egy sajátot neki
    v3->intf = 0;
    v3->usageCount = 0;
    v3->multiMaterialNumIfType4ThenAmbiantColor = 0;
    v3->multiMaterialPtrIfType4ThenDiffuseColor = 0;
    v3->solidVertexColor = 0;
    GRO_Struct_Init((tGroGeneric*)v3, a1->intf->id);
    v4 = LOA_ReadLong(a2, 0, 1);
    MAT_AllocMultiMaterial(v3, v4);
    for (i = 0; i < v3->multiMaterialNumIfType4ThenAmbiantColor; ++i)
    {
        v6 = LOA_ReadLong(a2, 0, 1);
        if (v6 == -1)
            v3->multiMaterialPtrIfType4ThenDiffuseColor[i] = NULL;
        else
            LOA_MakeFileRef(v6, &v3->multiMaterialPtrIfType4ThenDiffuseColor[i], GEO_ul_Load_ObjectCallback, 1);
    }
    return (tGroGeneric*)v3;
}

void MAT_Validate_Multitexture(tGEOMAT* a1)
{
    tMultiTextureElemStruct* v1;
    unsigned int v2;
    int v3;
    int v4;
    unsigned int v5;
    BOOL v6;
    tMultiTextureElemStruct* v7;
    unsigned int v8;

    v1 = (tMultiTextureElemStruct*)a1->idxOrPtr;
    a1->validityFlag = 0;
    if (v1)
    {
        v2 = v1->subTextureFlags;
        if ((v2 & 0xF0000) != 0 && (((v1->subTextureFlags >> 16) & 0xffff) & 0xF) != 7 || (v2 & 0x40) != 0)
            a1->validityFlag = 0xC0;
        do
        {
            v3 = v1->transform1;
            if (v3 == 0x3F800000 && v1->transform2 == 0x3F800000 || !v3 || !v1->transform2)
                v1->transform1 = 0;
//            switch ((uint8_t)((((v1->subTextureFlags >> 16) & 0xffff) << 8) & 0xff) >> 4) // ezt azért meg kellene nézni egyszer, !most már ok
            switch ((uint8_t)(((v1->subTextureFlags) & 0xffff) >> 8) >> 4) // elõször 16-tal letolja majd 8-cal vissza (HIBYTE(LOWORD(v1->subTextureFlags)))
            {
            case 0:
            case 5:
            case 6:
                v4 = a1->validityFlag | 1;
                goto LABEL_17;
            case 1:
                v4 = a1->validityFlag | 0x22;
                goto LABEL_17;
            case 2:
                v4 = a1->validityFlag | 4;
                goto LABEL_17;
            case 3:
                v4 = a1->validityFlag | 0x10;
                goto LABEL_17;
            case 4:
                v4 = a1->validityFlag | 8;
            LABEL_17:
                a1->validityFlag = v4;
                break;
            default:
                break;
            }
            v5 = v1->subTextureFlags;
            switch (((v5 >> 16) & 0xffff) & 0xF)
            {
            case 0:
            case 7:
                if ((v5 & 0x400) == 0)
                {
                    v7 = v1->nextTextureKey;
                    if (v7)
                    {
                        if ((((v7->subTextureFlags >> 16) & 0xff) & 0xF) == 3)
                            a1->validityFlag |= 0x10u;
                        v6 = (((v1->nextTextureKey->subTextureFlags >> 16) & 0xff) & 0xF) == 4;
                        goto LABEL_25;
                    }
                }
                break;
            case 1:
            case 2:
                v6 = (v5 & 0x400) == 0;
            LABEL_25:
                if (v6)
                    a1->validityFlag |= 0x10u;
                break;
            default:
                break;
            }
            v8 = v1->subTextureFlags;
            if ((v8 & 0x10) != 0 && (v8 & 0x400) == 0)
                a1->validityFlag |= 0x10u;
            if ((v1->subTextureFlags & 0xF00000) == 0x300000)
                a1->validityFlag |= 0x20u;
            if ((v1->subTextureFlags & 0xF00000) == 0x400000)
                a1->validityFlag |= 0x20u;
            v1 = v1->nextTextureKey;
        } while (v1);
    }
}

struct sGroGeneric* MAT_pst_CreateMultiTextureFromBuffer(tGroGeneric* a2, char** a3, tWorld* world)
{
    tGEOMAT* v3;
    tMultiTextureElemStruct** v4;
    int v5;
    tEngTexListElemStruct* v6;

    v3 = (tGEOMAT*)MEM_p_Alloc(sizeof(tGEOMAT));
    memset(v3, 0, sizeof(tGEOMAT));
    GRO_Struct_Init((tGroGeneric*)v3, a2->intf->id);
    v3->multiMaterialNumIfType4ThenAmbiantColor = LOA_ReadLong(a3, 0, 1);
    v3->multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT**)LOA_ReadLong(a3, 0, 1);
    v3->solidVertexColor = LOA_ReadLong(a3, 0, 1);
    v3->unkFloat1 = LOA_ReadFloat(a3, 0, 1);
    v3->unkFloat2 = LOA_ReadFloat(a3, 0, 1);
    v3->negativeZeroOrOneUnknown = LOA_ReadLong(a3, 0, 1);
    v4 = (tMultiTextureElemStruct**)&v3->idxOrPtr;
    v3->idxOrPtr = LOA_ReadLong(a3, 0, 1);
    v3->validityFlag = LOA_ReadLong(a3, 0, 1);
    if (v3->idxOrPtr)
    {
        do
        {
            *v4 = (tMultiTextureElemStruct*)MEM_p_Alloc(sizeof(tMultiTextureElemStruct));
            (*v4)->texElemIdx = LOA_ReadShort(a3, 0, 1);
            (*v4)->phongOrPlanarParam = LOA_ReadShort(a3, 0, 1);
            (*v4)->subTextureFlags = LOA_ReadLong(a3, 0, 1);
            (*v4)->transform1 = LOA_ReadLong(a3, 0, 1);
            (*v4)->transform2 = LOA_ReadLong(a3, 0, 1);
            (*v4)->nextTextureKey = (tMultiTextureElemStruct*)LOA_ReadLong(a3, 0, 1);
            v5 = (*v4)->texElemIdx;
            v6 = TEX_pst_List_FindTexture(&TEX_gst_GlobalList, (int)(*v4)->nextTextureKey);
            if (v6)
                (*v4)->texElemIdx = v6->textElemIdx;
            else
                (*v4)->texElemIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, (int)(*v4)->nextTextureKey);
            v4 = &(*v4)->nextTextureKey;
            *v4 = (tMultiTextureElemStruct*)v5; // a pointert beállítja az id-ra ami nyilván invalid, de ha az id nulla az akkor az gyakorlatilag a NULL, egyébként meg felülíródik a következõ ciklusban
        } while (v5);
    }
    MAT_Validate_Multitexture(v3);
    return (tGroGeneric*)v3;
}

tGEOMAT* MAT_allocGRO_sub_470350()
{
    tGEOMAT* v0;

    v0 = (tGEOMAT*)MEM_p_Alloc(20u);
    v0->intf = 0;
    v0->usageCount = 0;
    v0->multiMaterialNumIfType4ThenAmbiantColor = 0;
    v0->multiMaterialPtrIfType4ThenDiffuseColor = 0;
    v0->solidVertexColor = 0;
    GRO_Struct_Init((tGroGeneric*)v0, 4);
    return v0;
}

struct sGroGeneric* MAT_pst_CreateMulti()
{
    tGEOMAT* v0;

    v0 = MAT_allocGRO_sub_470350();
    MAT_AllocMultiMaterial(v0, 1);
    *v0->multiMaterialPtrIfType4ThenDiffuseColor = 0;
    return (tGroGeneric*)v0;
}

struct sGroGeneric* MAT_Duplicate(struct sGroGeneric* a1) {
    return NULL;
}

void MAT_Init()
{
    MAT_Validate_Multitexture(&MAT_gst_DefaultSingleMaterial);
    GRO_gast_Interface[3].Create = MAT_pst_CreateSingle;
    GRO_gast_Interface[3].CreateFromBuffer = MAT_pst_CreateSingleFromBuffer;
    GRO_gast_Interface[3].Duplicate = MAT_Duplicate;
    GRO_gast_Interface[3].Destroy = MAT_DestroyMaterial;
    GRO_gast_Interface[3].GetValidatedMask = MAT_GetValidatedMask_MTT;
    GRO_gast_Interface[4].Create = MAT_pst_CreateMulti;
    GRO_gast_Interface[4].CreateFromBuffer = MAT_pst_CreateMultiFromBuffer;
    GRO_gast_Interface[4].Destroy = MAT_DestroyMaterial;
    GRO_gast_Interface[4].GetValidatedMask = MAT_GetValidatedMask_MSM;
    GRO_gast_Interface[5].CreateFromBuffer = MAT_pst_CreateMultiTextureFromBuffer;
    GRO_gast_Interface[5].Duplicate = MAT_Duplicate;
    GRO_gast_Interface[5].Destroy = MAT_DestroyMaterial;
    GRO_gast_Interface[5].GetValidatedMask = MAT_GetValidatedMask_MTT;
    MAT_gst_DefaultSingleMaterial.intf = &GRO_gast_Interface[5];
}

void MAT_TestUsedForMat(tGEOMAT* a1, char* a2)
{
    int v2;
    int v3;
    tGEOMAT** v4;
    tGEOMAT** i;
    tGEOMAT* v6;
    tMultiTextureElemStruct* j;

    v2 = a1->intf->id; // decryptage load 5833,  túlírja a ptable-t
    switch (v2)
    {
    case 3:
        v3 = a1->idxOrPtr;
        if (v3 != -1)
            a2[v3] = 1;
        break;
    case 4:
        v4 = a1->multiMaterialPtrIfType4ThenDiffuseColor;
        for (i = &v4[a1->multiMaterialNumIfType4ThenAmbiantColor]; v4 < i; ++v4)
        {
            v6 = *v4;
            if (*v4 && v6->intf->id != 4)
                MAT_TestUsedForMat(v6, a2);
        }
        break;
    case 5:
        for (j = (tMultiTextureElemStruct*)a1->idxOrPtr; j; j = j->nextTextureKey)
        {
            if (j->texElemIdx != -1)
                a2[j->texElemIdx] = 1;
        }
        break;
    }
}

void MAT_Validate_Multi(tGEOMAT* a1)
{
    int v1;
    char v2;
    tGEOMAT** v3;
    int v4;
    int v5;

    v1 = a1->multiMaterialNumIfType4ThenAmbiantColor;
    v2 = -1;
    v3 = a1->multiMaterialPtrIfType4ThenDiffuseColor;
    a1->solidVertexColor = 0;
    if (v1)
    {
        v4 = v1;
        do
        {
            if (*v3)
            {
                if ((*v3)->intf->id == 5)
                    MAT_Validate_Multitexture(*v3);
                if ((*v3)->intf->id == 3)
                    MAT_Validate_Single(*v3);
                v5 = (*v3)->intf->GetValidatedMask(*v3, 0);
                v2 &= v5;
                a1->solidVertexColor |= v5; // TODO FIXME ??????? ez mi
            }
            ++v3;
            --v4;
        } while (v4);
    }
    a1->solidVertexColor ^= ((uint8_t)v2 ^ (uint8_t)a1->solidVertexColor) & 0x40;  // TODO FIXME ??????? ez mi
}

char* MAT_pc_GetAllUsedTextureIndex(struct sGdi_DisplayData* a1, unsigned int a2)
{
    char* v2;
    tWorld* v3;
    tableElemStructP* v4;
    tableElemStructP* i;
    tGEOMAT* v6;

    v2 = (char*)MEM_p_Alloc(a2);
    memset(v2, 0, a2);
    v3 = a1->CurrentWorld;
    v4 = v3->worldMaterialTable.firstElemPtr;
    for (i = ((tableElemStructP*)(v3->worldMaterialTable.nextElemPtr)) - 1; v4 <= i; ++v4)
    {
        v6 = v4->objectPtr;
        if (v4->objectPtr)
        {
            if (((unsigned __int8)v6 & 3) != 1)
            {
                MAT_TestUsedForMat(v6, v2);
                if (((tGEOMAT*)(v4->objectPtr))->intf->id == 4)
                    MAT_Validate_Multi((tGEOMAT*)(v4->objectPtr));
            }
        }
    }
    GAO_ModifierShadow_DetectUsedTextures((byte*)v2);
    return v2;
}