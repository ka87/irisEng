#pragma once
#include "../gfx.h"

typedef struct sGfxGaoData
{
    struct sGameObjectMain* gao;
    float focale;
} tGfxGaoData;

struct sGfxGaoData* GFX_Gao_Create(struct sGameObjectMain* a1);
void draw_game_object_in_GFX_sub_45DBB0(struct sGfxGaoData* a1);
void GFX_Gao_Destroy(struct sGfxGaoData* a1);