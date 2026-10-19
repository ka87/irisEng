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
    byte uncoverPiecePaletteID;
    byte paletteCanBeUpdated;
    byte _pad[1];
    char PositionToRotationVertexIdx;
    int pieceStatus;
    int currentPieceStatus;
    tGameObjectMain* gao[16];
    int textElemIndexId[16];
    int texElemIndices[16];
    int objectCount;
    byte paletteData[96];
    float remainingTime;
    float remainingUncoverTime;
    int flags;
} tGfxCarteData;

struct sGfxCarteData* GFX_Carte_Create();
void GFX_Carte_Destroy(struct sGfxCarteData* a1, struct sGEOMAT* a2);
int GFX_i_Carte_Render(struct sGfxCarteData* carte, struct sGEOMAT* mat, struct sGameObjectMain* a3);
void GFX_Carte_Seti(struct sGfxCarteData* a1, unsigned int a2, unsigned int a3);
void GFX_Carte_Object(tGfxElem* a1, __int16 id, tGameObjectMain* a3, int a4, int textElemIndexId);