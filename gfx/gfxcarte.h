#pragma once
#include "../gameobject.h"
#include "../materialanddraw.h"
#include "../gfx.h"

typedef struct sGfxCarteData
{
    Vec3 pos2D[2];
    Vec3 offset;
    Vec3 size;
    Vec3 cursorSize;
    Vec3 anotherPos[2];
    float scale;
    float blinkTime;
    float effectTime;
    float remainingEffectTime;
    uint8_t uncoverPiecePaletteID;
    uint8_t paletteCanBeUpdated;
    uint8_t _pad[1];
    char PositionToRotationVertexIdx;
    int pieceStatus;
    int currentPieceStatus;
    tGameObjectMain* gao[16];
    int textElemIndexId[16];
    int texElemIndices[16];
    int objectCount;
    uint8_t paletteData[96];
    float remainingTime;
    float remainingUncoverTime;
    int flags;
} tGfxCarteData;

struct sGfxCarteData* GFX_Carte_Create(void);
void GFX_Carte_Destroy(struct sGfxCarteData* carte, struct sGEOMAT* mat);
int GFX_i_Carte_Render(struct sGfxCarteData* carte, struct sGEOMAT* mat, struct sGameObjectMain* gao);
void GFX_Carte_Seti(struct sGfxCarteData* carte, unsigned int id, unsigned int value);
void GFX_Carte_Object(tGfxElem* gfxList, int16_t id, tGameObjectMain* gao, int mode, int textElemIndexId);