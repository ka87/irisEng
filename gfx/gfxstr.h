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

struct sGfxStrData* GFX_Str_Create(struct sGameObjectMain* a1);
int GFX_i_Str_Render(struct sGfxStrData* a1);
void GFX_Str_Destroy(struct sGfxStrData* a1);
void GFX_Str_Seti(struct sGfxStrData* a1, int a2, unsigned int idx);