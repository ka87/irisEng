#pragma once
#include "../gameobject.h"
#include "../gfx.h"
#include "../particlegenerator.h"

typedef struct sGfxDesintegrateData
{
    Vec3 v1;
    float rand2End;
    float rand2Start;
    float rand1End;
    float rand1Start;
    float dword1C;
    float dword20;
    float f1;
    uint32_t flags;
    byte gap2C[4];
    Mtx4x4 mtx;
    tGameObjectMain* gao;
    tGEOGRO* geogro;
    tGEOMAT* geomat;
    uint32_t triangleNum;
    uint32_t pointsNum;
    int triangleChunkCount;
    byte gap98[4];
    float timeElapsed;
    tPAGStruct* pagPtr;
    byte x[12];
} tGfxDesintegrateData;

struct sGfxDesintegrateData* GFX_Desintegrate_Create(struct sGameObjectMain* a1);
void GFX_Desintegrate_Destroy(struct sGfxDesintegrateData* a1);
int GFX_i_Desintegrate_Render(struct sGfxDesintegrateData* a1);