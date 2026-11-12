#pragma once
#include "../gfx.h"

typedef struct sGfxExplodeData
{
    tSpriteC0DEData wind;
    tSpriteC0DEData friction;
    int num;
    tSpriteC0DEData* positionVecs;
    tSpriteC0DEData* speedVecs;
    int* vertexColors;
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
    float lastGenerationTimeStamp;
    float timeElapsed;
    int GFXDirectly;
    unsigned int interPolatedColors[512];
    tGEOGRO geo;
    tGameObjectMain* gao;
    tGeoSprites spr[4];
    tGeoSpriteElem spriteElems[8];
} tGfxExplodeData;

float fTimeRandomFactors[256];

struct sGfxExplodeData* GFX_Explode_Create(void);
void GFX_Explode_Destroy(struct sGfxExplodeData* expl);
BOOL GFX_i_Explode_Render(struct sGfxExplodeData* expl, struct sGEOMAT* material);
void GFX_SetRenderStateForExplode(void);
int GFX_i_Explode_Geti(struct sGfxExplodeData* expl, int type);
void GFX_Explode_Seti(struct sGfxExplodeData* expl, int type, int val);
void GFX_Explode_Setf(struct sGfxExplodeData* expl, int type, float val);
void GFX_Explode_Setv(struct sGfxExplodeData* expl, int type, Vec3* val);