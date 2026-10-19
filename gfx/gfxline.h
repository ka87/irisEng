#pragma once
#include "../gfx.h"

typedef struct sGfxLineData
{
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    float f;
    int vertexColor;
} tGfxLineData;

void GFX_Line_Render(struct sGfxLineData* a1);
struct sGfxLineData* GFX_Line_Create();