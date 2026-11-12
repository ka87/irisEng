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

void MAT_FreeMultiMaterial(struct sGEOMAT* mat)
{
    tGEOMAT** children;
    int num;
    int i;

    if (mat->intf->id != 4)
        return;

    num = mat->multiMaterialNumIfType4ThenAmbiantColor;
    if (!num)
        return;

    children = mat->multiMaterialPtrIfType4ThenDiffuseColor;
    for (i = 0; i < num; i++)
    {
        if (children[i])
        {
            children[i]->intf->AddRef((tGroGeneric*)children[i], -1);
            children[i]->intf->Destroy((tGroGeneric*)children[i]);
        }
    }
    MEM_Free(mat->multiMaterialPtrIfType4ThenDiffuseColor);
    mat->multiMaterialNumIfType4ThenAmbiantColor = 0;
}

void MAT_FreeMultiTexture(struct sGEOMAT* mat)
{
    tMultiTextureElemStruct* elem;
    tMultiTextureElemStruct* next;

    if (mat->intf->id != 5)
        return;

    for (elem = (tMultiTextureElemStruct*)mat->idxOrPtr; elem; elem = next)
    {
        next = elem->nextTextureKey;
        MEM_Free(elem);
    }
}

void MAT_DestroyMaterial(struct sGroGeneric* gro)
{
    if (!gro || gro->usageCount > 0)
        return;

    MAT_FreeMultiMaterial((tGEOMAT*)gro);
    MAT_FreeMultiTexture((tGEOMAT*)gro);
    LOA_DeleteAddress((int)gro);
    MEM_Free(gro);
}

void MAT_Validate_Single(struct sGEOMAT* mat)
{
    int texIdx;

    texIdx = mat->idxOrPtr;
    mat->validityFlag = 0x11;
    if (mat->unkFloat2 < 0.98000002
        || (texIdx != -1 && (TEX_gst_GlobalList.texElems[texIdx].texElemFlag & 8) != 0))
    {
        mat->validityFlag = 0xD1;
    }
}

static tGEOMAT* MAT_l_AllocSingle(void)
{
    tGEOMAT* mat;

    mat = (tGEOMAT*)MEM_p_Alloc(sizeof(tGEOMAT));
    memset(mat, 0, sizeof(tGEOMAT));
    GRO_Struct_Init((tGroGeneric*)mat, 3);
    MAT_Validate_Single(mat);
    return mat;
}

struct sGroGeneric* MAT_pst_CreateSingle()
{
    return (tGroGeneric*)MAT_l_AllocSingle();
}

struct sGroGeneric* MAT_pst_CreateSingleFromBuffer(struct sGroGeneric* gro, char** readptr,
                                                   tWorld* world)
{
    tGEOMAT* mat;
    tEngTexListElemStruct* texElem;

    mat = (tGEOMAT*)MEM_p_Alloc(sizeof(tGEOMAT));
    memset(mat, 0, sizeof(tGEOMAT));
    GRO_Struct_Init((tGroGeneric*)mat, gro->intf->id);
    mat->multiMaterialNumIfType4ThenAmbiantColor = LOA_ReadLong(readptr, 0, 1);
    mat->multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT**)LOA_ReadLong(readptr, 0, 1);
    mat->solidVertexColor = LOA_ReadLong(readptr, 0, 1);
    mat->unkFloat1 = LOA_ReadFloat(readptr, 0, 1);
    mat->unkFloat2 = LOA_ReadFloat(readptr, 0, 1);
    mat->negativeZeroOrOneUnknown = LOA_ReadLong(readptr, 0, 1);
    mat->idxOrPtr = LOA_ReadLong(readptr, 0, 1); // ha nem idx akkor key
    mat->validityFlag = LOA_ReadLong(readptr, 0, 1);

    if (mat->idxOrPtr != -1)
    {
        texElem = TEX_pst_List_FindTexture(&TEX_gst_GlobalList, mat->idxOrPtr);
        if (texElem)
            mat->idxOrPtr = texElem->textElemIdx;
        else
            mat->idxOrPtr = TEX_w_List_AddTexture(&TEX_gst_GlobalList, mat->idxOrPtr);
    }
    if (mat->negativeZeroOrOneUnknown > 1u)
        mat->negativeZeroOrOneUnknown = 0;

    MAT_Validate_Single(mat);
    return (tGroGeneric*)mat;
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

void MAT_AllocMultiMaterial(tGEOMAT* mat, int num)
{
    int count;

    if (mat->intf->id != 4)
        return;

    count = (num > 255) ? 0 : num;
    mat->multiMaterialNumIfType4ThenAmbiantColor = count;
    if (count)
        mat->multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT**)MEM_p_Alloc(4 * count);
    else
        mat->multiMaterialPtrIfType4ThenDiffuseColor = NULL;
}

struct sGroGeneric* MAT_pst_CreateMultiFromBuffer(struct sGroGeneric* gro, char** readptr,
                                                  tWorld* world)
{
    tGEOMAT* mat;
    int wanted;
    int childKey;
    int i;

    WOR_gpst_WorldToLoadIn = world;
    mat = (tGEOMAT*)MEM_p_Alloc(20); // TODO FIXME ez nem a rendes tGEOMAT struktúra, csinálni egy sajátot neki
    mat->intf = 0;
    mat->usageCount = 0;
    mat->multiMaterialNumIfType4ThenAmbiantColor = 0;
    mat->multiMaterialPtrIfType4ThenDiffuseColor = 0;
    mat->solidVertexColor = 0;
    GRO_Struct_Init((tGroGeneric*)mat, gro->intf->id);

    wanted = LOA_ReadLong(readptr, 0, 1);
    MAT_AllocMultiMaterial(mat, wanted);
    for (i = 0; i < mat->multiMaterialNumIfType4ThenAmbiantColor; i++)
    {
        childKey = LOA_ReadLong(readptr, 0, 1);
        if (childKey == -1)
            mat->multiMaterialPtrIfType4ThenDiffuseColor[i] = NULL;
        else
            LOA_MakeFileRef(childKey, &mat->multiMaterialPtrIfType4ThenDiffuseColor[i],
                            GEO_ul_Load_ObjectCallback, 1);
    }
    return (tGroGeneric*)mat;
}

void MAT_Validate_Multitexture(tGEOMAT* mat)
{
    tMultiTextureElemStruct* elem;
    unsigned int flags;
    unsigned int nextFlags;
    unsigned int blend;

    elem = (tMultiTextureElemStruct*)mat->idxOrPtr;
    mat->validityFlag = 0;
    if (!elem)
        return;

    flags = elem->subTextureFlags;
    if (((flags & 0xF0000) != 0 && ((flags >> 16) & 0xF) != 7) || (flags & 0x40) != 0)
        mat->validityFlag = 0xC0;

    for (; elem; elem = elem->nextTextureKey)
    {
        flags = elem->subTextureFlags;

        if ((elem->transform1 == 0x3F800000 && elem->transform2 == 0x3F800000)
            || !elem->transform1 || !elem->transform2)
        {
            elem->transform1 = 0;
        }

        // the mapping mode, HIBYTE(LOWORD(subTextureFlags)) >> 4
        switch ((flags >> 12) & 0xF)
        {
        case 0:
        case 5:
        case 6:
            mat->validityFlag |= 1;
            break;
        case 1:
            mat->validityFlag |= 0x22;
            break;
        case 2:
            mat->validityFlag |= 4;
            break;
        case 3:
            mat->validityFlag |= 0x10;
            break;
        case 4:
            mat->validityFlag |= 8;
            break;
        default:
            break;
        }

        blend = (flags >> 16) & 0xF;
        if (blend == 0 || blend == 7)
        {
            if ((flags & 0x400) == 0 && elem->nextTextureKey)
            {
                nextFlags = (elem->nextTextureKey->subTextureFlags >> 16) & 0xF;
                if (nextFlags == 3 || nextFlags == 4)
                    mat->validityFlag |= 0x10u;
            }
        }
        else if (blend == 1 || blend == 2)
        {
            if ((flags & 0x400) == 0)
                mat->validityFlag |= 0x10u;
        }

        if ((flags & 0x10) != 0 && (flags & 0x400) == 0)
            mat->validityFlag |= 0x10u;
        if ((flags & 0xF00000) == 0x300000 || (flags & 0xF00000) == 0x400000)
            mat->validityFlag |= 0x20u;
    }
}

struct sGroGeneric* MAT_pst_CreateMultiTextureFromBuffer(tGroGeneric* gro, char** readptr,
                                                         tWorld* world)
{
    tGEOMAT* mat;
    tMultiTextureElemStruct** link;
    tEngTexListElemStruct* texElem;
    int nextMarker;

    mat = (tGEOMAT*)MEM_p_Alloc(sizeof(tGEOMAT));
    memset(mat, 0, sizeof(tGEOMAT));
    GRO_Struct_Init((tGroGeneric*)mat, gro->intf->id);
    mat->multiMaterialNumIfType4ThenAmbiantColor = LOA_ReadLong(readptr, 0, 1);
    mat->multiMaterialPtrIfType4ThenDiffuseColor = (tGEOMAT**)LOA_ReadLong(readptr, 0, 1);
    mat->solidVertexColor = LOA_ReadLong(readptr, 0, 1);
    mat->unkFloat1 = LOA_ReadFloat(readptr, 0, 1);
    mat->unkFloat2 = LOA_ReadFloat(readptr, 0, 1);
    mat->negativeZeroOrOneUnknown = LOA_ReadLong(readptr, 0, 1);
    mat->idxOrPtr = LOA_ReadLong(readptr, 0, 1);
    mat->validityFlag = LOA_ReadLong(readptr, 0, 1);

    if (!mat->idxOrPtr)
    {
        MAT_Validate_Multitexture(mat);
        return (tGroGeneric*)mat;
    }

    link = (tMultiTextureElemStruct**)&mat->idxOrPtr;
    do
    {
        *link = (tMultiTextureElemStruct*)MEM_p_Alloc(sizeof(tMultiTextureElemStruct));
        (*link)->texElemIdx = LOA_ReadShort(readptr, 0, 1);
        (*link)->phongOrPlanarParam = LOA_ReadShort(readptr, 0, 1);
        (*link)->subTextureFlags = LOA_ReadLong(readptr, 0, 1);
        (*link)->transform1 = LOA_ReadLong(readptr, 0, 1);
        (*link)->transform2 = LOA_ReadLong(readptr, 0, 1);
        (*link)->nextTextureKey = (tMultiTextureElemStruct*)LOA_ReadLong(readptr, 0, 1);

        // the short holds whether another element follows and the long
        // holds this element's texture key, which is the other way round
        // from what the two fields are named
        nextMarker = (*link)->texElemIdx;
        texElem = TEX_pst_List_FindTexture(&TEX_gst_GlobalList,
                                           (int)(*link)->nextTextureKey);
        if (texElem)
            (*link)->texElemIdx = texElem->textElemIdx;
        else
            (*link)->texElemIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList,
                                                        (int)(*link)->nextTextureKey);

        link = &(*link)->nextTextureKey;
        // a pointert beállítja az id-ra ami nyilván invalid, de ha az id
        // nulla az akkor az gyakorlatilag a NULL, egyébként meg
        // felülíródik a következõ ciklusban
        *link = (tMultiTextureElemStruct*)nextMarker;
    } while (nextMarker);

    MAT_Validate_Multitexture(mat);
    return (tGroGeneric*)mat;
}

static tGEOMAT* MAT_l_AllocMulti(void)
{
    tGEOMAT* mat;

    mat = (tGEOMAT*)MEM_p_Alloc(20u);
    mat->intf = 0;
    mat->usageCount = 0;
    mat->multiMaterialNumIfType4ThenAmbiantColor = 0;
    mat->multiMaterialPtrIfType4ThenDiffuseColor = 0;
    mat->solidVertexColor = 0;
    GRO_Struct_Init((tGroGeneric*)mat, 4);
    return mat;
}

struct sGroGeneric* MAT_pst_CreateMulti()
{
    tGEOMAT* mat;

    mat = MAT_l_AllocMulti();
    MAT_AllocMultiMaterial(mat, 1);
    *mat->multiMaterialPtrIfType4ThenDiffuseColor = 0;
    return (tGroGeneric*)mat;
}

struct sGroGeneric* MAT_Duplicate(struct sGroGeneric* gro)
{
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

void MAT_TestUsedForMat(tGEOMAT* mat, char* used)
{
    tGEOMAT** children;
    tMultiTextureElemStruct* elem;
    int texIdx;
    int num;
    int i;

    switch (mat->intf->id)
    {
    case 3:
        texIdx = mat->idxOrPtr;
        if (texIdx != -1)
            used[texIdx] = 1;
        break;

    case 4:
        children = mat->multiMaterialPtrIfType4ThenDiffuseColor;
        num = mat->multiMaterialNumIfType4ThenAmbiantColor;
        for (i = 0; i < num; i++)
        {
            if (children[i] && children[i]->intf->id != 4)
                MAT_TestUsedForMat(children[i], used);
        }
        break;

    case 5:
        for (elem = (tMultiTextureElemStruct*)mat->idxOrPtr; elem; elem = elem->nextTextureKey)
        {
            if (elem->texElemIdx != -1)
                used[elem->texElemIdx] = 1;
        }
        break;

    default:
        break;
    }
}

void MAT_Validate_Multi(tGEOMAT* mat)
{
    tGEOMAT** children;
    unsigned int childMask;
    uint8_t sharedMask;
    int num;
    int i;

    num = mat->multiMaterialNumIfType4ThenAmbiantColor;
    children = mat->multiMaterialPtrIfType4ThenDiffuseColor;
    sharedMask = 0xFF;
    mat->solidVertexColor = 0;

    for (i = 0; i < num; i++)
    {
        if (!children[i])
            continue;

        if (children[i]->intf->id == 5)
            MAT_Validate_Multitexture(children[i]);
        if (children[i]->intf->id == 3)
            MAT_Validate_Single(children[i]);

        childMask = children[i]->intf->GetValidatedMask(children[i], 0);
        sharedMask = sharedMask & (uint8_t)childMask;
        mat->solidVertexColor |= childMask;
    }

    // bit 0x40 is taken from the children only when every one of them
    // has it, where the rest of the mask is the union
    if ((sharedMask & 0x40) != 0)
        mat->solidVertexColor |= 0x40;
    else
        mat->solidVertexColor &= ~0x40;
}

char* MAT_pc_GetAllUsedTextureIndex(struct sGdi_DisplayData* dd, unsigned int texCount)
{
    char* used;
    tWorld* world;
    tableElemStructP* elem;
    tableElemStructP* lastElem;
    tGEOMAT* mat;

    used = (char*)MEM_p_Alloc(texCount);
    memset(used, 0, texCount);

    world = dd->CurrentWorld;
    lastElem = ((tableElemStructP*)(world->worldMaterialTable.nextElemPtr)) - 1;
    for (elem = world->worldMaterialTable.firstElemPtr; elem <= lastElem; ++elem)
    {
        mat = (tGEOMAT*)elem->objectPtr;
        if (!mat)
            continue;

        // an entry whose low two bits are 01 is still an unresolved key
        // rather than a material
        if (((uintptr_t)mat & 3) == 1)
            continue;

        MAT_TestUsedForMat(mat, used);
        if (mat->intf->id == 4)
            MAT_Validate_Multi(mat);
    }

    GAO_ModifierShadow_DetectUsedTextures((uint8_t*)used);
    return used;
}
