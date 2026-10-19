#pragma once
#include "../math.h"
#include "../gfx.h"

typedef struct sGfxTableData
{
    byte gap1[2];
    char num1;
    char num2;
    int colors[10];
    float ff1;
    float ff2;
    float ff3;
    float f[10];
    float f2[10];
    char vertexColorIndexes[100];
} tGfxTableData;

struct sGfxTableData* GFX_Table_Create();
int GFX_i_Table_Render(struct sGfxTableData* a1);
void GFX_Table_Setv(struct sGfxTableData* a1, int a2, Vec3* a3);
