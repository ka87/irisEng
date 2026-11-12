#pragma once
#include "../gfx.h"

typedef struct sGfxGaoData
{
    struct sGameObjectMain* gao;
    float focale;
} tGfxGaoData;

struct sGfxGaoData* GFX_Gao_Create(struct sGameObjectMain* gao);
void draw_game_object_in_GFX_sub_45DBB0(struct sGfxGaoData* gfx);
void GFX_Gao_Destroy(struct sGfxGaoData* gfx);