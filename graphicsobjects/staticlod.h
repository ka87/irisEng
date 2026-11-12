#pragma once
#include "gro_common.h"
#include "geo.h"
#include "../common.h"

typedef struct sGEOSTATICLOD
{
    struct sGroIntf* intf;
    int usageCount;
    uint8_t lodNum;
    uint8_t currDistance;
    uint8_t distance[6];
    struct sGEOGRO* geo[6];
} tGEOSTATICLOD;

int GEO_l_StaticLOD_GetIndex(struct sGEOSTATICLOD* lod, uint8_t distance);
struct sGEOGRO* GEO_pst_StaticLOD_GetLOD(struct sGEOSTATICLOD* lod, uint8_t distance);
void GEO_StaticLOD_Init();
