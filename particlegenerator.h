#pragma once
#include "common.h"
#include "gameobject.h"

typedef struct sPAGGeneratorElemStruct
{
    int generatorID;
    struct sGameObjectMain* gao;
    float generatorFreq;
    float fff21;
    float fff22;
} tPAGGeneratorElemStruct;

typedef struct sPAGElemStruct
{
    int particleState;
    Vec3 particlePos;
    Vec3 v1;
    float ff;
    float ff2;
    float fff;
    float pagPosAndVertexIndexFloatBase;
    float ffff2;
    unsigned int f5;
} tPAGElemStruct;


typedef struct sPAGStruct
{
    tGroIntf* intf;
    int usageCnt;
    int renderCnt;
    float timeElapsed;
    int flags;
    byte mode;
    byte subMode;
    __int16 num;
    struct sGameObjectMain* gao1;
    struct sGameObjectMain* gaoForMatrixRef;
    struct sPAGGeneratorElemStruct generators[16];
    float ffb1;
    float ffb2;
    float ffb3;
    float ffb4;
    float ffb5;
    float ffb6;
    float ffb7;
    float ffb8;
    float ffb9;
    float ffb10;
    float ffb11;
    float ffb12;
    float ffb13;
    float ffb14;
    float ffb15;
    float ffb16;
    float ffb17;
    float fd21;
    float fd22;
    Vec3 v1;
    float f1;
    float i3;
    float ff;
    float ffa1;
    float ffa2;
    float ffa3;
    float ffa4;
    float ffa5;
    float ffa6;
    float ffa7;
    float ffa8;
    float ffa9;
    int color;
    int triangleNum;
    int triangleNum2;
    struct sPAGElemStruct* pagElems;
    byte gap[116];
} tPAGStruct;

void PAG_RenderCommon(struct sGameObjectMain* a1, struct sPAGStruct* a2);
void PAG_Update(struct sGameObjectMain* a1, struct sPAGStruct* a2, float dt);