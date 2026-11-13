#pragma once
#include "../gfx.h"

typedef struct sGfxGaoData
{
    struct sGameObjectMain* gao;
    float focale;
} tGfxGaoData;

struct sGfxGaoData* GFX_Gao_Create(struct sGameObjectMain* gao);
void GFX_Gao_Render(struct sGfxGaoData* gfx);
void GFX_Gao_Destroy(struct sGfxGaoData* gfx);