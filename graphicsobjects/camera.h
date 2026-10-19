#pragma once
#include "gro_common.h"
#include "../common.h"

typedef struct sGEOCAM
{
    struct sGroIntf* intf;
    int usageCount;
    int flags;
    float nearPlane;
    float farPlane;
    float fov;
} tGEOCAM;

void CAM_Init();

