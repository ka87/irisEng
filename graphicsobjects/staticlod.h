#pragma once
#include "gro_common.h"
#include "geo.h"
#include "../common.h"

typedef struct sGEOSTATICLOD
{
    struct sGroIntf* intf;
    int usageCount;
    unsigned __int8 lodNum;
    unsigned __int8 currDistance;
    byte distance[6];
    struct sGEOGRO* geo[6];
} tGEOSTATICLOD;

struct sGEOGRO* GEO_pst_StaticLOD_GetLOD(struct sGEOSTATICLOD* a1, uint8_t a2);
void GEO_StaticLOD_Init();
