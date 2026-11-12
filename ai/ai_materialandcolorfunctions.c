#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
#include "../materialanddraw.h"
#include "ai_materialandcolorfunctions.h"

// The sub-material a script names. A non-zero top byte makes the id a
// key into the big files; otherwise it indexes the object's own
// material, wrapped when that material is a multi-material.
tGEOMAT* MAT_ValidateSUBMAT_C(tGameObjectMain* gao, unsigned int subMatID)
{
    tGEOMAT* material;
    unsigned int pos;

    if ((subMatID & 0xFF000000) != 0)
    {
        pos = BIG_ul_SearchKeyToPos(subMatID);
        if (pos == -1)
            return NULL;

        material = (tGEOMAT*)LOA_ul_SearchAddress(pos);
        if (material == (tGEOMAT*)-1)
            return NULL;
        return material;
    }

    if ((gao->flags & 0x4000) == 0
        || !gao->basic->actorObjectOrVisualObject
        || !((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT)
    {
        return NULL;
    }

    material = (tGEOMAT*)((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
    if (material->intf->id != 4)
        return material;
    return material->multiMaterialPtrIfType4ThenDiffuseColor[
        subMatID % material->multiMaterialNumIfType4ThenAmbiantColor];
}

// Where one of a material's three colours lives, so the caller can
// read or write it. Only the single and multi-texture materials have
// them.
void MAT_GetColorPtr(tGEOMAT* material, void** colorPtr, int multiMatId, int which)
{
    tGEOMAT* sub;
    int clamped;
    int id;

    sub = material;
    *colorPtr = 0;
    if (material->intf->id == 4)
    {
        clamped = material->multiMaterialNumIfType4ThenAmbiantColor - 1;
        if (multiMatId <= clamped)
            clamped = multiMatId;
        sub = material->multiMaterialPtrIfType4ThenDiffuseColor[clamped];
    }

    id = sub->intf->id;
    if (id == 4 || (id != 3 && id != 5))
        return;

    if (which == 1)                                     // constant
        *colorPtr = &sub->solidVertexColor;
    else if (which == 2)                                // ambiant
        *colorPtr = &sub->multiMaterialNumIfType4ThenAmbiantColor;
    else if (which == 0)                                // diffuse
        *colorPtr = &sub->multiMaterialPtrIfType4ThenDiffuseColor;
}

// The multi-texture level at multiMatElemId, walking the chain from the
// material's first. Note that the clamp here has no -1 where
// MAT_GetColorPtr's has one, so a multimatId equal to the count indexes
// one past the end; preserved as written.
tMultiTextureElemStruct** MAT_GetMtxLvlPtr(tGEOMAT* material, tMultiTextureElemStruct** level, int multimatId, int multiMatElemId)
{
    tMultiTextureElemStruct* next;
    tGEOMAT* sub;
    int clamped;

    sub = material;
    *level = 0;
    if (material->intf->id == 4)
    {
        clamped = material->multiMaterialNumIfType4ThenAmbiantColor;
        if (multimatId <= clamped)
            clamped = multimatId;
        sub = material->multiMaterialPtrIfType4ThenDiffuseColor[clamped];
    }

    if (sub->intf->id != 5)
        return level;

    *level = (tMultiTextureElemStruct*)sub->idxOrPtr;
    if (!*level)
        return level;

    while (multiMatElemId)
    {
        next = (*level)->nextTextureKey;
        --multiMatElemId;
        *level = next;
        if (!next)
            break;
    }
    return level;
}

void AI_EvalFunc_MATSetConstant_C(struct sGameObjectMain* gao, int color, int subMatID)
{
    tGEOMAT* material;
    int* constant;

    constant = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetColorPtr(material, (void**)&constant, 0, 1);
    if (constant)
        *constant = color;
}

tAINode* AI_EvalFunc_MATSetConstant(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int color;
    int subMatID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    subMatID = AI_GetIntFromStack();
    color = AI_GetIntFromStack();
    AI_EvalFunc_MATSetConstant_C(gaoUltra, color, subMatID);
    return node + 1;
}

int AI_EvalFunc_MATGetConstant_C(struct sGameObjectMain* gao, unsigned int subMatID)
{
    tGEOMAT* material;
    int* constant;

    constant = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetColorPtr(material, (void**)&constant, 0, 1);
    if (!constant)
        return 0;
    return *constant;
}

tAINode* AI_EvalFunc_MATGetConstant(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    subMatID = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATGetConstant_C(gaoUltra, subMatID));
    return node + 1;
}

// The diffuse colour shares its field with the multi-material pointer,
// which is why it is handled as a pointer-wide value.
void AI_EvalFunc_MATSetDiffuse_C(struct sGameObjectMain* gao, int* diffuse, unsigned int subMatID)
{
    tGEOMAT* material;
    int** slot;

    slot = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetColorPtr(material, (void**)&slot, 0, 0);
    if (slot)
        *slot = diffuse;
}

tAINode* AI_EvalFunc_MATSetDiffuse(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int* diffuse;
    int subMatID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    subMatID = AI_GetIntFromStack();
    diffuse = (int*)AI_GetIntFromStack();
    AI_EvalFunc_MATSetDiffuse_C(gaoUltra, diffuse, subMatID);
    return node + 1;
}

int AI_EvalFunc_MATGetDiffuse_C(struct sGameObjectMain* gao, unsigned int subMatID)
{
    tGEOMAT* material;
    int* slot;

    slot = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetColorPtr(material, (void**)&slot, 0, 0);
    if (!slot)
        return 0;
    return *slot;
}

tAINode* AI_EvalFunc_MATGetDiffuse(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    subMatID = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATGetDiffuse_C(gaoUltra, subMatID));
    return node + 1;
}

tAINode* AI_EvalFunc_MATSetAmbient(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGEOMAT* material;
    int** slot;
    int* ambient;
    int subMatID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    subMatID = AI_GetIntFromStack();
    material = MAT_ValidateSUBMAT_C(gaoUltra, subMatID);
    ambient = (int*)AI_GetIntFromStack();

    slot = 0;
    if (material)
        MAT_GetColorPtr(material, (void**)&slot, 0, 2);
    if (slot)
        *slot = ambient;
    return node + 1;
}

tAINode* AI_EvalFunc_MATGetAmbient(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGEOMAT* material;
    int* slot;
    int ambient;
    int subMatID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    subMatID = AI_GetIntFromStack();
    material = MAT_ValidateSUBMAT_C(gaoUltra, subMatID);

    ambient = 0;
    slot = 0;
    if (material)
        MAT_GetColorPtr(material, (void**)&slot, 0, 2);
    if (slot)
        ambient = *slot;
    AI_PutIntToStack(ambient);
    return node + 1;
}

// Bit 11 of a level's flags is its DISABLED bit, so enabling clears it
// and disabling sets it. The original wrote that as four maskings
// through a 0xFFF window, which came to the same two lines.
void AI_EvalFunc_MATEnableMltxtLvl_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId, int enable)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (!level)
        return;

    if (enable)
        level->subTextureFlags &= ~0x800u;
    else
        level->subTextureFlags |= 0x800u;
}

tAINode* AI_EvalFunc_MATEnableMltxtLvl(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int enable;

    enable = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MATEnableMltxtLvl_C(gaoUltra, subMatID, multiMatElemId, enable);
    return node + 1;
}

// The same, with the multi-material index the script chooses rather
// than the first.
void AI_EvalFunc_MATEnableMltxtLvlMT_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId, int multimatId, int enable)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, multimatId, multiMatElemId);
    if (!level)
        return;

    if (enable)
        level->subTextureFlags &= ~0x800u;
    else
        level->subTextureFlags |= 0x800u;
}

tAINode* AI_EvalFunc_MATEnableMltxtLvlMT(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int multimatId;
    int enable;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    enable = AI_GetIntFromStack();
    multimatId = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    AI_EvalFunc_MATEnableMltxtLvlMT_C(gaoUltra, subMatID, multiMatElemId, multimatId, enable);
    return node + 1;
}

// The level's alpha is five bits at the top of its parameter word, so
// the script sees it over 8.
int AI_EvalFunc_GetLocalAlpha_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;
    int alpha;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);

    if (level)
        alpha = (level->phongOrPlanarParam & 0xF800) >> 11;
    else
        alpha = 0;
    return 8 * alpha;
}

tAINode* AI_EvalFunc_GetLocalAlpha(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_GetLocalAlpha_C(gaoUltra, subMatID, multiMatElemId));
    return node + 1;
}

void AI_EvalFunc_SetLocalAlpha_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId, int alpha)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (level)
    {
        level->phongOrPlanarParam =
            ((uint16_t)(alpha >> 3) << 11) | (level->phongOrPlanarParam & 0x7FF);
    }
}

tAINode* AI_EvalFunc_SetLocalAlpha(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int alpha;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    alpha = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    AI_EvalFunc_SetLocalAlpha_C(gaoUltra, subMatID, multiMatElemId, alpha);
    return node + 1;
}

// The alpha threshold is six bits of the flag word, so the script sees
// it over 4.
unsigned int AI_EvalFunc_GetALPHATRESH_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);

    if (!level)
        return 0;
    return 4 * ((level->subTextureFlags & 0x3F000000) >> 24);
}

tAINode* AI_EvalFunc_GetALPHATRESH(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;

    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_GetALPHATRESH_C(gaoUltra, subMatID, multiMatElemId));
    return node + 1;
}

void AI_EvalFunc_SetALPHATRESH_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId, int threshold)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (level)
    {
        level->subTextureFlags =
            (((threshold >> 2) << 24) & 0x3F000000) | (level->subTextureFlags & 0xC0FFFFFF);
    }
}

tAINode* AI_EvalFunc_SetALPHATRESH(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int threshold;

    threshold = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SetALPHATRESH_C(gaoUltra, subMatID, multiMatElemId, threshold);
    return node + 1;
}

// Which word the flags go into depends on the element id: -1 is the
// material's own, below 1000 a level's texture flags, and 1000 and up
// the same level's parameter word.
void AI_EvalFunc_SetFlag_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId, int toSet, int16_t toClear)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (!material)
        return;

    if (multiMatElemId == -1)
    {
        material->negativeZeroOrOneUnknown |= toSet;
        material->negativeZeroOrOneUnknown &= ~toClear;
    }
    else if (multiMatElemId < 1000)
    {
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
        if (level)
        {
            level->subTextureFlags |= toSet;
            level->subTextureFlags &= ~toClear;
        }
    }
    else
    {
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId - 1000);
        if (level)
        {
            level->phongOrPlanarParam |= toSet;
            level->phongOrPlanarParam &= ~toClear;
        }
    }
}

tAINode* AI_EvalFunc_SetFlag(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int toSet;
    int16_t toClear;

    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SetFlag_C(gaoUltra, subMatID, multiMatElemId, toSet, toClear);
    return node + 1;
}

// The read side of SetFlag. Note that an id of 1000 or more whose
// level is not found falls through and looks the raw id up again,
// which walks that many links into the chain; preserved as written.
int AI_EvalFunc_GetFlag_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemID)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (!material)
        return 0;

    if (multiMatElemID == -1)
        return material->negativeZeroOrOneUnknown;

    if (multiMatElemID >= 1000)
    {
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemID - 1000);
        if (level)
            return level->phongOrPlanarParam;
    }

    MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemID);
    if (level)
        return level->subTextureFlags;
    return 0;
}

tAINode* AI_EvalFunc_GetFlag(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemID;

    multiMatElemID = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_GetFlag_C(gaoUltra, subMatID, multiMatElemID));
    return node + 1;
}

struct sTexProceduralStruct* AI_EvalFunc_MATGetPro_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemId)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (!material)
        return NULL;

    MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (!level)
        return NULL;
    return TEX_pst_Procedural_Get(level->texElemIdx);
}

tAINode* AI_EvalFunc_MATGetPro(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;

    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack((int)AI_EvalFunc_MATGetPro_C(gaoUltra, subMatID, multiMatElemId));
    return node + 1;
}

// Sets a field of a procedural texture. What the field index means
// depends on the kind: a photo takes a frame count, a flag, a mapping
// object or a lifetime in milliseconds, and an MPEG takes its play and
// loop flags, a byte of its flag word, or another texture to follow.
void AI_EvalFunc_MATProSeti_C(struct sTexProceduralStruct* procedural, int field, int param1)
{
    tTexProcPhoto* photo;
    tTexProcMpeg* mpeg;
    float ms;

    if (!procedural)
        return;

    if (procedural->proceduralIntf->id == 4)
    {
        photo = (tTexProcPhoto*)(procedural->proceduralTexPtr);
        if (field == 0)
        {
            photo->b2 = param1;
        }
        else if (field == 1)
        {
            photo->b3 = 1;
        }
        else if (field == 2)
        {
            TEXPRO_Photo_SetMapGao(procedural, (tGameObjectMain*)param1);
        }
        else if (field == 3 && photo->x == 0x10000000)
        {
            photo->x = 128;
            ms = (float)param1;
            photo->f1 = (float)(ms / 1000.0);
            photo->remainingTime = photo->f1;
        }
        return;
    }

    if (procedural->proceduralIntf->id != 3)
        return;

    mpeg = (tTexProcMpeg*)(procedural->proceduralTexPtr);
    switch (field)
    {
    case 0:
        if (param1)
            mpeg->flags |= 1u;
        else
            mpeg->flags &= ~1u;
        break;
    case 1:
        if (param1)
            mpeg->flags |= 2u;
        else
            mpeg->flags &= ~2u;
        break;
    case 10:
        mpeg->flags = (mpeg->flags & ~0x00ff0000) | (param1 << 16);
        break;
    case 11:
        TEX_Procedural_UpdateWithAnother(procedural, (tTexProceduralStruct*)param1);
        break;
    }
}

tAINode* AI_EvalFunc_MATProSeti(tAINode* node)
{
    tTexProceduralStruct* procedural;
    int field;
    int param1;

    param1 = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    procedural = (tTexProceduralStruct*)AI_GetIntFromStack();
    AI_EvalFunc_MATProSeti_C(procedural, field, param1);
    return node + 1;
}

// A level's UV animation: speed, scale or rotation of either of its
// two transforms.
float AI_EvalFunc_GetUV_C(tGameObjectMain* gao, unsigned int subMatID, int multiMatElemId, int which)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;
    double value;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (!level)
        return 0.0;

    switch (which)
    {
    case 0:
        value = MAT_GetSpeed(level->transform1);
        break;
    case 1:
        value = MAT_GetSpeed(level->transform2);
        break;
    case 2:
        value = MAT_GetScale(level->transform1);
        break;
    case 3:
        value = MAT_GetScale(level->transform2);
        break;
    case 4:
        value = MAT_GetRotation(level->transform1, level->transform2);
        break;
    default:
        return 0.0;
    }
    return (float)value;
}

tAINode* AI_EvalFunc_GetUV(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int which;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    which = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    AI_PutFloatToStack(AI_EvalFunc_GetUV_C(gaoUltra, subMatID, multiMatElemId, which));
    return node + 1;
}

void AI_EvalFunc_SetUV_C(struct sGameObjectMain* gao, int subMatID, int multiMatElemId, int which, float value)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    level = 0;
    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (material)
        MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (!level)
        return;

    switch (which)
    {
    case 0:
        MAT_SetPos(&level->transform1, value);
        break;
    case 1:
        MAT_SetPos(&level->transform2, value);
        break;
    case 2:
        MAT_SetScale(&level->transform1, value);
        break;
    case 3:
        MAT_SetScale(&level->transform2, value);
        break;
    case 4:
        MAT_SetRotation(&level->transform1, &level->transform2, value);
        break;
    }
}

tAINode* AI_EvalFunc_SetUV(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;
    int which;
    float value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    value = AI_GetFloatFromStack();
    which = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    AI_EvalFunc_SetUV_C(gaoUltra, subMatID, multiMatElemId, which, value);
    return node + 1;
}

// For a photo, whether it has finished; for an MPEG, whether it is
// playing. Only field 0 answers anything.
BOOL AI_EvalFunc_MATProGeti_C(struct sTexProceduralStruct* procedural, int field)
{
    tTexProcPhoto* photo;
    tTexProcMpeg* mpeg;

    if (!procedural)
        return FALSE;

    if (procedural->proceduralIntf->id == 4)
    {
        if (field)
            return FALSE;
        photo = (tTexProcPhoto*)(procedural->proceduralTexPtr);
        return photo->b1 == (char)photo->b2 && !photo->b3;
    }

    if (procedural->proceduralIntf->id == 3 && !field)
    {
        mpeg = (tTexProcMpeg*)(procedural->proceduralTexPtr);
        return (mpeg->flags & 1) != 0;
    }
    return FALSE;
}

tAINode* AI_EvalFunc_MATProGeti(tAINode* node)
{
    tTexProceduralStruct* procedural;
    int field;

    field = AI_GetIntFromStack();
    procedural = (tTexProceduralStruct*)AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATProGeti_C(procedural, field));
    return node + 1;
}

tTexAnimatedElem* AI_EvalFunc_MATAntGet_C(tGameObjectMain* gao, unsigned int subMatID, int multiMatElemId)
{
    tMultiTextureElemStruct* level;
    tGEOMAT* material;

    material = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (!material)
        return NULL;

    MAT_GetMtxLvlPtr(material, &level, 0, multiMatElemId);
    if (!level)
        return NULL;
    return TEX_pst_Anim_Get(level->texElemIdx);
}

tAINode* AI_EvalFunc_MATAntGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int subMatID;
    int multiMatElemId;

    multiMatElemId = AI_GetIntFromStack();
    subMatID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack((int)AI_EvalFunc_MATAntGet_C(gaoUltra, subMatID, multiMatElemId));
    return node + 1;
}

// Field 1 starts and stops an animated texture; field 2 pins it to one
// frame, or -1 lets it run again.
void AI_EvalFunc_MATAntSeti_C(tTexAnimatedElem* anim, int field, int value)
{
    if (!anim)
        return;

    if (field == 1)
    {
        if (value)
            anim->animationFlags |= 1u;
        else
            anim->animationFlags &= ~1u;
    }
    else if (field == 2)
    {
        if (value == -1)
        {
            anim->animationFlags &= ~2u;
        }
        else
        {
            anim->animationFlags |= 2u;
            anim->b3 = value;
        }
    }
}

tAINode* AI_EvalFunc_MATAntSeti(tAINode* node)
{
    tTexAnimatedElem* anim;
    int field;
    int value;

    value = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    anim = (tTexAnimatedElem*)AI_GetIntFromStack();
    AI_EvalFunc_MATAntSeti_C(anim, field, value);
    return node + 1;
}

int AI_EvalFunc_COLORSetRGBA_C(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    return red | (green << 8) | (blue << 16) | (alpha << 24);
}

tAINode* AI_EvalFunc_COLORSetRGBA(tAINode* node)
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    alpha = AI_GetIntFromStack();
    blue = AI_GetIntFromStack();
    green = AI_GetIntFromStack();
    red = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_COLORSetRGBA_C(red, green, blue, alpha));
    return node + 1;
}

int AI_EvalFunc_COLORGetRed_C(uint8_t color)
{
    return color;
}

tAINode* AI_EvalFunc_COLORGetRed(tAINode* node)
{
    uint8_t color;

    color = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_COLORGetRed_C(color));
    return node + 1;
}

int AI_EvalFunc_COLORGetGreen_C(int16_t color)
{
    return (color & 0xFF00) >> 8;
}

tAINode* AI_EvalFunc_COLORGetGreen(tAINode* node)
{
    int16_t color;

    color = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_COLORGetGreen_C(color));
    return node + 1;
}

int AI_EvalFunc_COLORGetBlue_C(int color)
{
    return (color & 0xFF0000) >> 16;
}

tAINode* AI_EvalFunc_COLORGetBlue(tAINode* node)
{
    int color;

    color = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_COLORGetBlue_C(color));
    return node + 1;
}

unsigned int AI_EvalFunc_COLORGetAlpha_C(int color)
{
    return (color & 0xFF000000) >> 24;
}

tAINode* AI_EvalFunc_COLORGetAlpha(tAINode* node)
{
    int color;

    color = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_COLORGetAlpha_C(color));
    return node + 1;
}

/* Blends two packed colours without unpacking them, one binary place
   at a time.

   Adding 49152 (32768 + 16384) to a blend factor in (0.02, 0.98) gives
   a float whose exponent is fixed at 2^15, so its last mantissa bit is
   worth 2^-8: the low byte of the float's bit pattern is the factor as
   eighths-of-a-256th. Bit 0x80 of that byte therefore means "add a
   half", 0x40 "add a quarter", and so on down to 0x02 for a 128th; the
   1/256 bit is not used.

   For each of those places the term is the colour shifted right by
   that many bits, with the low bits of every channel masked off so a
   channel cannot borrow from the one below it -- taken from the second
   colour where the factor's bit is set and from the first where it is
   not. The seven terms sum to the blend. */
unsigned int ul_Interpol2PackedColor(int color1, int color2, float factor)
{
    unsigned int term[7];
    float biased;
    int places;
    int i;

    if (factor >= 0.98000002)
        return color2;
    if (factor <= 0.02)
        return color1;

    biased = (float)(factor + 32768.0 + 16384.0);
    places = *(uint32_t*)&biased;

    term[0] = ((places & 0x80) != 0 ? color2 : color1) & 0xFEFEFEFE;
    term[1] = ((places & 0x40) != 0 ? color2 : color1) & 0xFCFCFCFC;
    term[2] = ((places & 0x20) != 0 ? color2 : color1) & 0xF8F8F8F8;
    term[3] = ((places & 0x10) != 0 ? color2 : color1) & 0xF0F0F0F0;
    term[4] = ((places & 0x08) != 0 ? color2 : color1) & 0xE0E0E0E0;
    term[5] = ((places & 0x04) != 0 ? color2 : color1) & 0xC0C0C0C0;
    term[6] = ((places & 0x02) != 0 ? color2 : color1) & 0x80808080;

    for (i = 0; i < 7; ++i)
        term[i] >>= i + 1;

    return term[6] + term[5] + term[4] + term[3] + term[2] + term[1] + term[0];
}

int AI_EvalFunc_COLORBlend_C(int color1, int color2, float factor)
{
    if (factor < 0.0)
        factor = 0.0;
    if (factor > 1.0)
        factor = 1.0;
    return ul_Interpol2PackedColor(color1, color2, factor);
}

tAINode* AI_EvalFunc_COLORBlend(tAINode* node)
{
    int color1;
    int color2;
    float factor;

    factor = AI_GetFloatFromStack();
    color2 = AI_GetIntFromStack();
    color1 = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_COLORBlend_C(color1, color2, factor));
    return node + 1;
}

unsigned int AI_EvalFunc_COLORBlack_C()
{
    return 0xFF000000;
}

tAINode* AI_EvalFunc_COLORWhite(tAINode* node)
{
    AI_PutIntToStack(0xFFFFFFFF);
    return node + 1;
}

tAINode* AI_EvalFunc_COLORBlack(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_COLORBlack_C());
    return node + 1;
}
