#pragma once
#include "gro_common.h"
#include "../common.h"
#include "../gameobject.h"

typedef struct sLIGHT
{
    struct sGroIntf* intf;
    int unk1;
    int flags;
    int color;
    float fnear;
    float ffar;
    float density;
    float i8;
    struct sGameObjectMain* lightGao;
} tLIGHT;

void LIGHT_Init();