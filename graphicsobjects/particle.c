#include "particle.h"
#include "../memory.h"
#include "../loading.h"
#include "../particlegenerator.h"
#include "../modifiers/particlemodifier.h"

int PAG_sgi_GenId = 2;

struct sPAGStruct* PAG_pst_Create()
{
    tPAGStruct* pag;

    pag = (tPAGStruct*)MEM_p_Alloc(sizeof(tPAGStruct));
    memset(pag, 0, sizeof(tPAGStruct));
    GRO_Struct_Init((tGroGeneric*)pag, 11);
    pag->num = 1;
    pag->generators[0].generatorID = 1;
    pag->generators[0].gao = 0;
    return pag;
}

struct sGroGeneric* PAG_p_Duplicate(struct sGroGeneric* gro)
{
    tPAGStruct* copy;
    tPAGElemStruct* particles;
    int num;

    copy = PAG_pst_Create();
    memcpy(copy, gro, sizeof(tPAGStruct));

    num = copy->triangleNum;
    if (num)
    {
        particles = (tPAGElemStruct*)MEM_p_Alloc(sizeof(tPAGElemStruct) * num);
        copy->pagElems = particles;
        memcpy(particles, ((tPAGStruct*)gro)->pagElems,
               sizeof(tPAGElemStruct) * num);
    }
    return (tGroGeneric*)copy;
}

void PAG_Free(struct sGroGeneric* gro)
{
    tPAGStruct* pag;

    pag = (tPAGStruct*)gro;
    if (!gro || gro->intf->id != 11 || pag->usageCnt > 0)
        return;

    if (pag->triangleNum)
        MEM_Free(pag->pagElems);
    LOA_DeleteAddress((int)gro);
    MEM_Free(gro);
}

static void PAG_l_ReadMotionFloats(tPAGStruct* pag, char** readptr)
{
    pag->ffb5 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb6 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb7 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb8 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb9 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb10 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb11 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb12 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb13 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffb14 = LOA_ReadFloat(readptr, 0, 1);
}

static void PAG_l_SkipSixLongs(char** readptr)
{
    int i;

    for (i = 0; i < 6; i++)
        LOA_ReadLong(readptr, 0, 2);
}

static void PAG_l_ReadLateTail(tPAGStruct* pag, char** readptr)
{
    LOA_ReadVector(readptr, &pag->v1, 1);
    pag->f1 = LOA_ReadFloat(readptr, 0, 1);
    pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
    pag->color = LOA_ReadLong(readptr, 0, 1);
    pag->i3 = LOA_ReadFloat(readptr, 0, 1);
    pag->ff = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa1 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa2 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa3 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa4 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa5 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa6 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa7 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa8 = LOA_ReadFloat(readptr, 0, 1);
    pag->ffa9 = LOA_ReadFloat(readptr, 0, 1);
}

struct sGroGeneric* PAG_p_CreateFromBuffer(struct sGroGeneric* gro, char** readptr,
                                           struct sWorld* world)
{
    tPAGStruct* pag;
    float paired;
    int version;
    int triangleNum;

    pag = PAG_pst_Create();
    pag->triangleNum = LOA_ReadLong(readptr, 0, 1);
    version = LOA_ReadLong(readptr, 0, 1);
    switch (version)
    {
    case 0:
        pag->flags = (uint8_t)LOA_ReadChar(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        LOA_ReadChar(readptr, 0, 2);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff21 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff22 = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_SkipSixLongs(readptr);
        pag->ffb5 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb6 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb7 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb8 = LOA_ReadFloat(readptr, 0, 1);
        paired = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb9 = paired;
        pag->ffb10 = paired;
        paired = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb11 = paired;
        pag->ffb12 = paired;
        pag->ffb13 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb14 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        break;
    case 1:
        pag->flags = (uint8_t)LOA_ReadChar(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        LOA_ReadChar(readptr, 0, 2);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff22 = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_SkipSixLongs(readptr);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 2:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff22 = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_SkipSixLongs(readptr);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 3:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff22 = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 4:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadLong(readptr, 0, 2);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        pag->color = LOA_ReadLong(readptr, 0, 1);
        break;
    case 5:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        pag->color = LOA_ReadLong(readptr, 0, 1);
        break;
    case 6:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        pag->color = LOA_ReadLong(readptr, 0, 1);
        pag->i3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ff = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa1 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 7:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->f1 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        pag->color = LOA_ReadLong(readptr, 0, 1);
        pag->i3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ff = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa4 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa5 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa6 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 8:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].generatorFreq = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->f1 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        pag->color = LOA_ReadLong(readptr, 0, 1);
        pag->i3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ff = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa4 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa5 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa6 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 9:
        pag->flags = LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        paired = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff21 = paired;
        pag->generators[0].generatorFreq = paired;
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        LOA_ReadVector(readptr, &pag->v1, 1);
        pag->f1 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd22 = LOA_ReadFloat(readptr, 0, 1);
        pag->color = LOA_ReadLong(readptr, 0, 1);
        pag->i3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ff = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa4 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa5 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa6 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa7 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffa8 = LOA_ReadFloat(readptr, 0, 1);
        break;
    case 10:
        pag->flags = (uint16_t)LOA_ReadShort(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        paired = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff21 = paired;
        pag->generators[0].generatorFreq = paired;
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadLateTail(pag, readptr);
        break;
    case 11:
        pag->flags = LOA_ReadLong(readptr, 0, 1);
        pag->mode = LOA_ReadChar(readptr, 0, 1);
        pag->subMode = LOA_ReadChar(readptr, 0, 1);
        pag->ffb1 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb2 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb3 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb4 = LOA_ReadFloat(readptr, 0, 1);
        paired = LOA_ReadFloat(readptr, 0, 1);
        pag->generators[0].fff21 = paired;
        pag->generators[0].generatorFreq = paired;
        PAG_l_ReadMotionFloats(pag, readptr);
        pag->ffb15 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb16 = LOA_ReadFloat(readptr, 0, 1);
        pag->ffb17 = LOA_ReadFloat(readptr, 0, 1);
        pag->fd21 = LOA_ReadFloat(readptr, 0, 1);
        PAG_l_ReadLateTail(pag, readptr);
        break;
    default:
        break;
    }

    triangleNum = pag->triangleNum;
    if (triangleNum)
        pag->pagElems = (tPAGElemStruct*)MEM_p_Alloc(sizeof(tPAGElemStruct) * triangleNum);
    else
        pag->pagElems = 0;
    pag->num = 1;
    pag->generators[0].generatorID = 1;
    pag->generators[0].gao = 0;
    return (tGroGeneric*)pag;
}

void PAG_Reinit(struct sGroGeneric* gro)
{
    tPAGStruct* pag;
    float loadedFreq;

    pag = (tPAGStruct*)gro;
    loadedFreq = pag->generators[0].fff21;

    pag->num = 1;
    pag->generators[0].generatorID = 1;
    pag->generators[0].gao = 0;
    pag->generators[0].generatorFreq = loadedFreq;
    pag->triangleNum2 = 0;
    pag->gao1 = 0;
    pag->gaoForMatrixRef = 0;
}

int PAG_l_HasSomethingToRender(struct sGameObjectVisualObject* gaovo, struct sGroGeneric** gro)
{
    *gro = NULL;
    return 1;
}

void PAG_Render(struct sGameObjectMain* gao)
{
    tGameObjectVisualObject* gaovo;

    gaovo = (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
    PAG_RenderCommon(gao, (tPAGStruct*)gaovo->GROGEOOrSomethingElse);
}

void PAG_Init()
{
    GRO_gast_Interface[11].CreateFromBuffer = PAG_p_CreateFromBuffer;
    GRO_gast_Interface[11].Destroy = PAG_Free;
    GRO_gast_Interface[11].HasSomethingToRender = PAG_l_HasSomethingToRender;
    GRO_gast_Interface[11].Render = PAG_Render;
    GRO_gast_Interface[11].Reinit = PAG_Reinit;
    GRO_gast_Interface[11].Duplicate = PAG_p_Duplicate;
}

struct sPAGStruct* PAG_pst_GetGameObjectParticleGenerator(struct sGameObjectMain* gao)
{
    tGameObjectVisualObject* gaovo;
    tGameObjectExtended* extended;
    tPAGStruct* pag;
    tModifier* mdf;

    if ((gao->flags & 0x4000) != 0)
    {
        gaovo = (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
        pag = (tPAGStruct*)gaovo->GROGEOOrSomethingElse;
        if (pag && pag->intf->id == 11)
            return pag;
    }

    extended = gao->extended;
    if (!extended)
        return NULL;

    for (mdf = extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == 18) // mpag
            return ((tMPAGStruct*)mdf->mdfPrivateDataPtr)->pag;
    }
    return NULL;
}

int PAG_i_AddGenerator(struct sPAGStruct* pag, struct sGameObjectMain* gao, float frequency)
{
    tPAGGeneratorElemStruct* slot;
    int16_t index;
    int id;

    index = pag->num;
    if (index == 16)
        return -1;

    slot = &pag->generators[index];
    id = PAG_sgi_GenId;
    pag->num = index + 1;
    slot->generatorID = id;

    PAG_sgi_GenId = id + 1;
    if (((id + 1) & 0x8000000) != 0)
        PAG_sgi_GenId = 2;

    slot->generatorFreq = frequency;
    slot->gao = gao;
    slot->fff22 = 0.0f;
    return slot->generatorID;
}

static int PAG_l_FindGenerator(tPAGStruct* pag, int generatorID)
{
    int i;

    for (i = 0; i < pag->num; i++)
    {
        if (pag->generators[i].generatorID == generatorID)
            return i;
    }
    return -1;
}

void PAG_DelGenerator(struct sPAGStruct* pag, int generatorID)
{
    int index;
    int last;

    index = PAG_l_FindGenerator(pag, generatorID);
    if (index < 0)
        return;

    last = pag->num - 1;
    if (index < last)
        memcpy(&pag->generators[index], &pag->generators[last],
               sizeof(tPAGGeneratorElemStruct));
    pag->num = last;
}

void PAG_SetGeneratorFrequency(struct sPAGStruct* pag, int generatorID, float frequency)
{
    int index;

    index = PAG_l_FindGenerator(pag, generatorID);
    if (index < 0)
        return;

    pag->generators[index].generatorFreq = frequency;
}
