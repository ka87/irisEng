#include "particle.h"
#include "../memory.h"
#include "../loading.h"
#include "../particlegenerator.h"
#include "../modifiers/particlemodifier.h"

int PAG_sgi_GenId = 2;

struct sPAGStruct* PAG_pst_Create()
{
    tPAGStruct* v0;

    v0 = (tPAGStruct*)MEM_p_Alloc(sizeof(tPAGStruct));
    memset(v0, 0, sizeof(tPAGStruct));
    GRO_Struct_Init((tGroGeneric*)v0, 11);
    v0->num = 1;
    v0->generators[0].generatorID = 1;
    v0->generators[0].gao = 0;
    return v0;
}

struct sGroGeneric* PAG_p_Duplicate(struct sGroGeneric* a1)
{
    tPAGStruct* v1;
    int v2;
    tPAGElemStruct* v3;
    unsigned int v4;

    v1 = PAG_pst_Create();
    memcpy(v1, a1, sizeof(tPAGStruct));
    v2 = v1->triangleNum;
    if (v2)
    {
        v3 = (tPAGElemStruct*)MEM_p_Alloc(sizeof(tPAGElemStruct) * v2);
        v4 = sizeof(tPAGElemStruct) * v1->triangleNum;
        v1->pagElems = v3;
        memcpy(v3, ((tPAGStruct*)(a1))->pagElems, v4);
    }
    return (tGroGeneric*)v1;
}

void PAG_Free(struct sGroGeneric* a1)
{
    if (a1 && a1->intf->id == 11 && ((tPAGStruct*)(a1))->usageCnt <= 0)
    {
        //nullsub_32();
        if (((tPAGStruct*)(a1))->triangleNum)
            MEM_Free(((tPAGStruct*)(a1))->pagElems);
        LOA_DeleteAddress((int)a1);
        MEM_Free(a1);
    }
}

struct sGroGeneric* PAG_p_CreateFromBuffer(struct sGroGeneric* a1, char** a2, struct sWorld* world)
{
    tPAGStruct* v2;
    int v3;
    double v4; 
    double v5; 
    double v6; 
    double v7; 
    double v8; 
    double v9; 
    double v10;
    int v11; 
    Vec3* v13; 
    Vec3* v14; 

    v2 = PAG_pst_Create();
    v2->triangleNum = LOA_ReadLong(a2, 0, 1);
    v3 = LOA_ReadLong(a2, 0, 1);
    switch (v3)
    {
    case 0:
        v2->flags = (uint8_t)LOA_ReadChar(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        LOA_ReadChar(a2, 0, 2);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff21 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff22 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v4 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = (float)v4;
        v2->ffb9 = (float)v4;
        v5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = (float)v5;
        v2->ffb11 = (float)v5;
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        goto LABEL_27;
    case 1:
        v2->flags = (uint8_t)LOA_ReadChar(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        LOA_ReadChar(a2, 0, 2);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff22 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        goto LABEL_27;
    case 2:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff22 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        LOA_ReadLong(a2, 0, 2);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v6 = LOA_ReadFloat(a2, 0, 1);
        v14 = &v2->v1;
    LABEL_7:
        v2->ffb15 = (float)v6;
        LOA_ReadVector(a2, v14, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        goto LABEL_27;
    case 3:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff22 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v6 = LOA_ReadFloat(a2, 0, 1);
        v14 = &v2->v1;
        goto LABEL_7;
    case 4:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadLong(a2, 0, 2);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        v2->color = LOA_ReadLong(a2, 0, 1);
        goto LABEL_27;
    case 5:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v2->fd21 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        v2->color = LOA_ReadLong(a2, 0, 1);
        goto LABEL_27;
    case 6:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v2->fd21 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        v2->color = LOA_ReadLong(a2, 0, 1);
        v2->i3 = LOA_ReadFloat(a2, 0, 1);
        v2->ff = LOA_ReadFloat(a2, 0, 1);
        v2->ffa1 = LOA_ReadFloat(a2, 0, 1);
        goto LABEL_27;
    case 7:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v2->fd21 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->f1 = LOA_ReadFloat(a2, 0, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        v2->color = LOA_ReadLong(a2, 0, 1);
        v2->i3 = LOA_ReadFloat(a2, 0, 1);
        v2->ff = LOA_ReadFloat(a2, 0, 1);
        v2->ffa1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa4 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa6 = LOA_ReadFloat(a2, 0, 1);
        goto LABEL_27;
    case 8:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].generatorFreq = LOA_ReadFloat(a2, 0, 1);
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v2->fd21 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->f1 = LOA_ReadFloat(a2, 0, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        v2->color = LOA_ReadLong(a2, 0, 1);
        v2->i3 = LOA_ReadFloat(a2, 0, 1);
        v2->ff = LOA_ReadFloat(a2, 0, 1);
        v2->ffa1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa4 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa6 = LOA_ReadFloat(a2, 0, 1);
        goto LABEL_27;
    case 9:
        v2->flags = LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v7 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff21 = (float)v7;
        v2->generators[0].generatorFreq = (float)v7;
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v2->fd21 = LOA_ReadFloat(a2, 0, 1);
        LOA_ReadVector(a2, &v2->v1, 1);
        v2->f1 = LOA_ReadFloat(a2, 0, 1);
        v2->fd22 = LOA_ReadFloat(a2, 0, 1);
        v2->color = LOA_ReadLong(a2, 0, 1);
        v2->i3 = LOA_ReadFloat(a2, 0, 1);
        v2->ff = LOA_ReadFloat(a2, 0, 1);
        v2->ffa1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa4 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffa8 = LOA_ReadFloat(a2, 0, 1);
        goto LABEL_27;
    case 10:
        v2->flags = (uint16_t)LOA_ReadShort(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v8 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff21 = (float)v8;
        v2->generators[0].generatorFreq = (float)v8;
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v9 = LOA_ReadFloat(a2, 0, 1);
        v13 = &v2->v1;
        break;
    case 11:
        v2->flags = LOA_ReadLong(a2, 0, 1);
        v2->mode = LOA_ReadChar(a2, 0, 1);
        v2->subMode = LOA_ReadChar(a2, 0, 1);
        v2->ffb1 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb2 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb3 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb4 = LOA_ReadFloat(a2, 0, 1);
        v10 = LOA_ReadFloat(a2, 0, 1);
        v2->generators[0].fff21 = (float)v10;
        v2->generators[0].generatorFreq = (float)v10;
        v2->ffb5 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb6 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb7 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb8 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb9 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb10 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb11 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb12 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb13 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb14 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb15 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb16 = LOA_ReadFloat(a2, 0, 1);
        v2->ffb17 = LOA_ReadFloat(a2, 0, 1);
        v9 = LOA_ReadFloat(a2, 0, 1);
        v13 = &v2->v1;
        break;
    default:
        goto LABEL_27;
    }
    v2->fd21 = (float)v9;
    LOA_ReadVector(a2, v13, 1);
    v2->f1 = LOA_ReadFloat(a2, 0, 1);
    v2->fd22 = LOA_ReadFloat(a2, 0, 1);
    v2->color = LOA_ReadLong(a2, 0, 1);
    v2->i3 = LOA_ReadFloat(a2, 0, 1);
    v2->ff = LOA_ReadFloat(a2, 0, 1);
    v2->ffa1 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa2 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa3 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa4 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa5 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa6 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa7 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa8 = LOA_ReadFloat(a2, 0, 1);
    v2->ffa9 = LOA_ReadFloat(a2, 0, 1);
LABEL_27:
    v11 = v2->triangleNum;
    if (v11)
        v2->pagElems = (tPAGElemStruct*)MEM_p_Alloc(sizeof(tPAGElemStruct) * v11);
    else
        v2->pagElems = 0;
    v2->num = 1;
    v2->generators[0].generatorID = 1;
    v2->generators[0].gao = 0;
    return (tGroGeneric*)v2;
}

void PAG_Reinit(struct sGroGeneric *a1) {
    float v2;
    tPAGStruct* a1a = (tPAGStruct*)a1;
    a1a->num = 1;
    a1a->generators[0].generatorID = 1;
    v2 = a1a->generators[0].fff21;
    a1a->triangleNum2 = 0;
    a1a->generators[0].gao = 0;
    a1a->generators[0].generatorFreq = v2;
    a1a->gao1 = 0;
    a1a->gaoForMatrixRef = 0;
}

int PAG_l_HasSomethingToRender(struct sGameObjectVisualObject* a1, struct sGroGeneric** a2)
{
    *a2 = NULL;
    return 1;
}

void PAG_Render(struct sGameObjectMain* a1)
{
    PAG_RenderCommon(a1, (tPAGStruct*)(((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse));
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

struct sPAGStruct* PAG_pst_GetGameObjectParticleGenerator(struct sGameObjectMain* a1)
{
    tPAGStruct* result;
    tGameObjectExtended* v2;
    tModifier* v3;

    if ((a1->flags & 0x4000) == 0
        || (result = (tPAGStruct*)((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) == NULL
        || result->intf->id != 11)
    {
        v2 = a1->extended;
        if (v2 && (v3 = v2->mdf) != 0)
        {
            while (v3->modifierIntf->mdfId != 18)   //  mpag
            {
                v3 = v3->nextElem;
                if (!v3)
                    goto LABEL_8;
            }
            result = ((tMPAGStruct*)v3->mdfPrivateDataPtr)->pag;
        }
        else
        {
        LABEL_8:
            result = NULL;
        }
    }
    return result;
}

int PAG_i_AddGenerator(struct sPAGStruct* a1, struct sGameObjectMain* a2, float a3)
{
    int16_t v3;
    int result;
    tPAGGeneratorElemStruct* v5;
    int v6;

    v3 = a1->num;
    if (v3 == 16)
        return -1;
    v5 = &a1->generators[v3];
    v6 = PAG_sgi_GenId;
    a1->num = v3 + 1;
    v5->generatorID = v6;
    PAG_sgi_GenId = v6 + 1;
    if (((v6 + 1) & 0x8000000) != 0)
        PAG_sgi_GenId = 2;
    a1->generators[v3].generatorFreq = a3;
    result = v5->generatorID;
    a1->generators[v3].gao = a2;
    a1->generators[v3].fff22 = 0.0;
    return result;
}

void PAG_DelGenerator(struct sPAGStruct* a1, int a2)
{
    int v2;
    int v3;
    tPAGGeneratorElemStruct* v4;

    v2 = a1->num;
    v3 = 0;
    v4 = a1->generators;
    if (v2 > 0)
    {
        while (v4->generatorID != a2)
        {
            ++v3;
            ++v4;
            if (v3 >= a1->num)
                return;
        }
        if (v3 < v2 - 1)
            memcpy(v4, &a1->generators[v2 - 1], sizeof(tPAGGeneratorElemStruct));
        --a1->num;
    }
}

void PAG_SetGeneratorFrequency(struct sPAGStruct* a1, int generatorID, float a3)
{
    tPAGGeneratorElemStruct* v3;
    int v4;
    int v5;

    v3 = a1->generators;
    v4 = a1->num;
    v5 = 0;
    if (v4 > 0)
    {
        while (v3->generatorID != generatorID)
        {
            ++v5;
            ++v3;
            if (v5 >= v4)
                return;
        }
        v3->generatorFreq = a3;
    }
}