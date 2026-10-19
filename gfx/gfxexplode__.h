#pragma once
#include "../gfx.h"

typedef struct sGfxExplodeData
{
    tSpriteC0DEData wind;
    tSpriteC0DEData friction;
    int num;
    tSpriteC0DEData* vecPtr;
    tSpriteC0DEData* normalPtr;
    int* vertrexColors;
    float* particlesTimeElapsed;
    Vec3 initialPos;
    Vec3 mainPosSpeed;
    Vec3 mainPosFriction;
    Vec3 initialPosVolumeX;
    Vec3 initialPosVolumeY;
    Vec3 initialPosVolumeZ;
    float gravity;
    float ground;
    float growSpeedMin;
    float growSpeedMax;
    float normSpeedMin;
    float normSpeedMax;
    Vec3 speedMin;
    Vec3 speedMax;
    float initialSizeMin;
    float initialSizeMax;
    float phase1Time;
    float phase2Time;
    float ooPhase1Time;
    float ooPhase2Time;
    float timeRandomFactor;
    int material1ID;
    int material2ID;
    int material3ID;
    int material4ID;
    int phase1Color;
    int phase2Color;
    int phase3Color;
    int piecesToGenerate;
    int triangleNum;
    float generationSpeed;
    float fdf;
    float timeElapsed;
    int GFXDirectly;
    unsigned int interPolatedColors[512];
    tGEOGRO geo;
    tGameObjectMain* gao;
    tGeoSprites spr[4];
    tGeoSpriteElem geoSub[8];
} tGfxExplodeData;

float fTimeRandomFactors[256];

struct sGfxExplodeData* GFX_Explode_Create();
void GFX_Explode_Destroy(struct sGfxExplodeData* a1);
BOOL GFX_i_Explode_Render(struct sGfxExplodeData* a1, struct sGEOMAT* a2);
void GFX_SetRenderStateForExplode();
int GFX_i_Explode_Geti(struct sGfxExplodeData* a1, int a2);
void GFX_Explode_Seti(struct sGfxExplodeData* a1, int a2, int a3);
void GFX_Explode_Setf(struct sGfxExplodeData* a1, int a2, float a3);
void GFX_Explode_Setv(struct sGfxExplodeData* a1, int a2, Vec3* a3);