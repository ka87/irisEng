#pragma once
#include "../gameobject.h"
#include "../graphicsobjects/geo.h"
#include "../gfx.h"

typedef struct sGfxStrData
{
    int num;
    int color1;
    int color2;
    float timeElapsed;
    struct sGameObjectMain* gao;
    Vec3* ptr;
    struct sGeoUVElem* UVs;
} tGfxStrData;

struct sGfxStrData* GFX_Str_Create(struct sGameObjectMain* gao);
int GFX_i_Str_Render(struct sGfxStrData* gfx);
void GFX_Str_Destroy(struct sGfxStrData* gfx);
void GFX_Str_Seti(struct sGfxStrData* gfx, int id, unsigned int idx);