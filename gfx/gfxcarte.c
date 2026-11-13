#include "../time.h"
#include "../memory.h"
#include "../textures.h"
#include "gfxcarte.h"

int i_LockMat_351 = 0; // TODO FIXME, game doesn't set it explicitly, check

void GFX_Carte_UpdatePalette(tGfxCarteData* carte, int16_t texelemIdx)
{
    tEngTexListElemStruct* tex;
    tEngPaletteElem* palette;
    uint8_t* dst;
    uint8_t* src;
    int* dstEntry;
    int* srcEntry;
    unsigned int status;
    float blend;
    int uncoverId;
    int i;

    tex = &TEX_gst_GlobalList.texElems[texelemIdx];
    if ((tex->texElemFlag & 0x40) == 0)
        return;

    palette = &TEX_gst_GlobalList.palettes[tex->palettedData.paletteID];
    if (!palette->paletteData)
        return;
    if ((palette->flags & 8) == 0 || (palette->flags & 1) != 0)
        return;

    // one 32-byte palette bank per piece, picked by that piece's status bit
    status = carte->pieceStatus;
    carte->currentPieceStatus = status;
    dst = palette->paletteData + 32;
    for (i = 0; i < 31; i++)
    {
        if ((carte->flags & 0x20) != 0)
            src = &carte->paletteData[64 * (status & 1)];
        else
            src = &carte->paletteData[32 * (status & 1) + 32];
        memcpy(dst, src, 32u);
        status >>= 1;
        dst += 32;
    }

    uncoverId = (int8_t)carte->uncoverPiecePaletteID;
    if (uncoverId != -1)
    {
        if (uncoverId >= 0 && uncoverId < 32)
        {
            carte->remainingUncoverTime = (float)(carte->remainingUncoverTime - TIM_gf_dt);
            if (0.0 <= carte->remainingUncoverTime)
            {
                blend = (float)fabs(MATH_GetSin((float)(carte->remainingUncoverTime * 6.2831855)));
                dstEntry = (int*)&palette->paletteData[32 * uncoverId + 32];
                srcEntry = (int*)&carte->paletteData[32 * ((carte->flags & 0x20) == 0)];
                for (i = 0; i < 8; i++)
                    dstEntry[i] = LIGHT_ul_Interpol2Colors(dstEntry[i], srcEntry[i], blend);
                gpst_DX_stDD->Request(22, (void*)(tex->palettedData.paletteID));
                return;
            }
            carte->remainingUncoverTime = 1.5;
        }
        carte->uncoverPiecePaletteID = -1;
    }

    gpst_DX_stDD->Request(22, (void*)(tex->palettedData.paletteID));
}

// the map's sixteen piece textures live on the texture key chain behind the
// material; the render takes them out and Destroy puts them back
static void GFX_l_CarteCaptureTextures(tGfxCarteData* carte, tMultiTextureElemStruct* elem)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        elem = elem->nextTextureKey;
        if (!elem)
            return;
        carte->texElemIndices[i] = elem->texElemIdx;
    }
}

static void GFX_l_CarteRestoreTextures(tGfxCarteData* carte, tMultiTextureElemStruct* elem)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        elem = elem->nextTextureKey;
        if (!elem)
            return;
        elem->texElemIdx = carte->texElemIndices[i] & 0xffff;
    }
}

static void GFX_l_CarteCorner(Vec3* out, float x, float y, Vec3* centre, Vec3* dir, float z)
{
    float dx;
    float dy;

    dx = x - centre->x;
    dy = y - centre->y;
    out->x = (float)(dir->x * dy + dx * dir->y + centre->x);
    out->y = (float)(dy * dir->y - dx * dir->x + centre->y);
    out->z = z;
}

static float GFX_l_CarteCursorPos(float worldPos, float offset, float size, double invScale)
{
    return (float)((1.0 - (worldPos - offset) / size) * invScale);
}

int GFX_i_Carte_Render(struct sGfxCarteData* carte, struct sGEOMAT* mat,
                       struct sGameObjectMain* gao)
{
    tMultiTextureElemStruct* elem;
    tMultiTextureElemStruct* mapElem;
    tMultiTextureElemStruct* cursor;
    tMultiTextureElemStruct* chain;
    tEngTexListElemStruct* tex;
    tViewElem* view;
    Vec3* verts;
    Vec3 dir;
    Vec3 centre;
    Vec3 offset;
    uint8_t* paletteData;
    double invScale;
    float left;
    float right;
    float top;
    float bottom;
    float scale;
    float grow;
    float cursorX;
    float cursorY;
    int hideRest;
    int k;

    if (!mat || mat->intf->id != 5 || !gao || !carte->objectCount || !carte->gao[0])
        return 1;

    if ((carte->flags & 1) == 0)
    {
        elem = (tMultiTextureElemStruct*)mat->idxOrPtr;
        if (!elem)
            return 1;
        mapElem = elem->nextTextureKey;
        if (!mapElem)
            return 1;

        mapElem->subTextureFlags = mapElem->subTextureFlags & ~0xB000u | 0x4000;
        tex = &TEX_gst_GlobalList.texElems[mapElem->texElemIdx];
        if ((tex->texElemFlag & 0x40) != 0)
        {
            paletteData = TEX_gst_GlobalList.palettes[tex->palettedData.paletteID].paletteData;
            if (paletteData)
                memcpy(carte->paletteData, paletteData, sizeof(carte->paletteData));
            else
                carte->paletteCanBeUpdated = 0;
        }
        GFX_l_CarteCaptureTextures(carte, mapElem);
        carte->flags |= 1u;
    }

    if ((carte->flags & 4) != 0)
        return 1;

    GFX_NeedGeom(4u, 4u, 2u, 0);
    GFX_gpst_Geo->UVs[0].u = 1.0;
    GFX_gpst_Geo->UVs[0].v = 0.0;
    GFX_gpst_Geo->UVs[1].u = 0.0;
    GFX_gpst_Geo->UVs[1].v = 0.0;
    GFX_gpst_Geo->UVs[2].u = 0.0;
    GFX_gpst_Geo->UVs[2].v = 1.0;
    GFX_gpst_Geo->UVs[3].u = 1.0;
    GFX_gpst_Geo->UVs[3].v = 1.0;
    verts = GFX_gpst_Geo->vertices;

    if ((carte->flags & 0x40) != 0)
    {
        left = (float)(carte->anotherPos[0].x * gpst_DX_stDD->ct.cam.width
                       + (float)gpst_DX_stDD->ct.cam.viewPortRealPosX);
        right = (float)(carte->anotherPos[1].x * gpst_DX_stDD->ct.cam.width
                        + (float)gpst_DX_stDD->ct.cam.viewPortRealPosX);
        top = (float)(carte->anotherPos[0].y * gpst_DX_stDD->ct.cam.height
                      + (float)gpst_DX_stDD->ct.cam.viewPortRealPosY);
        bottom = (float)(carte->anotherPos[1].y * gpst_DX_stDD->ct.cam.height
                         + (float)gpst_DX_stDD->ct.cam.viewPortRealPosY);
        centre.x = (float)((right + left) * 0.5);
        centre.y = (float)((bottom + top) * 0.5);

        k = carte->PositionToRotationVertexIdx;
        verts[k % 4].x = left;
        verts[k % 4].y = top;
        verts[k % 4].z = carte->pos2D[0].z;
        verts[(k + 1) % 4].x = right;
        verts[(k + 1) % 4].y = top;
        verts[(k + 1) % 4].z = carte->pos2D[0].z;
        verts[(k + 2) % 4].x = right;
        verts[(k + 2) % 4].y = bottom;
        verts[(k + 2) % 4].z = carte->pos2D[0].z;
        verts[(k + 3) % 4].x = left;
        verts[(k + 3) % 4].y = bottom;
        verts[(k + 3) % 4].z = carte->pos2D[0].z;
    }
    else
    {
        view = WOR_gpst_CurrentWorld ? WOR_gpst_CurrentWorld->viewPtr : 0;
        if (view)
        {
            MATH_VecInvert(&dir, &view->cameraMtx.v2);
        }
        else
        {
            dir.x = gpst_DX_stDD->ct.cam.objectMtx.v3.x;
            dir.y = gpst_DX_stDD->ct.cam.objectMtx.v3.y;
        }
        dir.z = 0.0;
        if (dir.x == 0.0 && dir.y == 0.0)
            return 1;
        MATH_NormalizeEqualVector(&dir);

        left = (float)(gpst_DX_stDD->ct.cam.width * carte->pos2D[0].x
                       + (double)gpst_DX_stDD->ct.cam.viewPortRealPosX);
        right = (float)(carte->pos2D[1].x * gpst_DX_stDD->ct.cam.width
                        + (double)gpst_DX_stDD->ct.cam.viewPortRealPosX);
        top = (float)(carte->pos2D[0].y * gpst_DX_stDD->ct.cam.height
                      + (double)gpst_DX_stDD->ct.cam.viewPortRealPosY);
        bottom = (float)(carte->pos2D[1].y * gpst_DX_stDD->ct.cam.height
                         + (double)gpst_DX_stDD->ct.cam.viewPortRealPosY);
        centre.x = (float)((right + left) * 0.5);
        centre.y = (float)((bottom + top) * 0.5);

        GFX_l_CarteCorner(&verts[0], left, top, &centre, &dir, carte->pos2D[0].z);
        GFX_l_CarteCorner(&verts[1], right, top, &centre, &dir, carte->pos2D[0].z);
        GFX_l_CarteCorner(&verts[2], right, bottom, &centre, &dir, carte->pos2D[0].z);
        GFX_l_CarteCorner(&verts[3], left, bottom, &centre, &dir, carte->pos2D[0].z);
    }

    if ((carte->flags & 0x18) != 0)
    {
        carte->remainingEffectTime = (float)(carte->remainingEffectTime - TIM_gf_dt);
        grow = (float)(carte->remainingEffectTime / carte->effectTime);
        if ((carte->flags & 0x10) != 0)
        {
            if (grow < 0.0)
            {
                carte->flags = carte->flags & ~0x10u | 4;
                grow = 0.0;
            }
        }
        else if (carte->remainingEffectTime >= 0.0)
        {
            grow = (float)(1.0 - grow);
        }
        else
        {
            carte->flags = carte->flags & ~8u;
            grow = 1.0;
        }

        centre.z = carte->pos2D[0].z;
        for (k = 0; k < 4; k++)
        {
            MATH_SubstractVec(&offset, &verts[k], &centre);
            MATH_VecMultScalarWithoutBuffer(&offset, grow);
            MATH_VecAddVec(&verts[k], &centre, &offset);
        }
    }

    for (k = 0; k < 4; k++)
        CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &verts[k], &verts[k]);

    GFX_SetQuadIndices();

    hideRest = 0;
    cursor = 0;
    if (!i_LockMat_351)
    {
        carte->remainingTime = (float)(carte->remainingTime - TIM_gf_dt);
        if (carte->remainingTime < 0.0)
        {
            carte->remainingTime = carte->blinkTime;
            if ((carte->flags & 2) != 0)
                carte->flags &= ~2u;
            else
                carte->flags |= 2;
        }

        elem = (tMultiTextureElemStruct*)mat->idxOrPtr;
        if (!elem)
            return 1;
        mapElem = elem->nextTextureKey;
        if (!mapElem)
            return 1;

        if (carte->pieceStatus == carte->currentPieceStatus
            && carte->uncoverPiecePaletteID == 0xFF)
        {
            carte->remainingUncoverTime = 1.5;
        }
        else
        {
            if (carte->paletteCanBeUpdated)
                GFX_Carte_UpdatePalette(carte, mapElem->texElemIdx);
            carte->currentPieceStatus = carte->pieceStatus;
        }

        if ((carte->flags & 0x40) != 0)
        {
            mapElem->subTextureFlags = mapElem->subTextureFlags & ~0xA0000u | 0x50000;
            MAT_SetScale(&mapElem->transform1, 1.0);
            scale = 1.0;
        }
        else
        {
            mapElem->subTextureFlags = mapElem->subTextureFlags & ~0xC0000u | 0x30000;
            MAT_SetScale(&mapElem->transform1, carte->scale);
            scale = carte->scale;
        }
        MAT_SetScale(&mapElem->transform2, scale);

        if (carte->objectCount && (carte->flags & 0x40) != 0)
        {
            MAT_SetPos(&mapElem->transform1, 0.0);
            MAT_SetPos(&mapElem->transform2, 0.0);
            cursor = mapElem->nextTextureKey;
            if (cursor)
            {
                cursor->texElemIdx = carte->texElemIndices[carte->textElemIndexId[0] & 0xF];
                cursorX = (float)(-(1.0 - (carte->gao[0]->matrices->Pos.x - carte->offset.x)
                                    / carte->size.x) + carte->cursorSize.x);
                cursorY = (float)(-(1.0 - (carte->gao[0]->matrices->Pos.y - carte->offset.y)
                                    / carte->size.y) + carte->cursorSize.y);
                MAT_SetPos(&cursor->transform1, cursorX);
                MAT_SetPos(&cursor->transform2, cursorY);
                mat->solidVertexColor = LIGHT_ul_Interpol2Colors(
                    0xFF00007F, 0xFF00FFFF,
                    (float)fabs(sin(TIM_gf_MainClock * 4.0)));
                cursor->subTextureFlags = cursor->subTextureFlags & 0xFFF627FF | 0x62000;
                hideRest = 1;
            }
        }
        else if (carte->objectCount)
        {
            invScale = 1.0 / carte->scale;
            cursorX = GFX_l_CarteCursorPos(carte->gao[0]->matrices->Pos.x, carte->offset.x,
                                           carte->size.x, invScale) - 0.5f;
            cursorY = GFX_l_CarteCursorPos(carte->gao[0]->matrices->Pos.y, carte->offset.y,
                                           carte->size.y, invScale) - 0.5f;
            MAT_SetPos(&mapElem->transform1, cursorX);
            MAT_SetPos(&mapElem->transform2, cursorY);

            cursor = mapElem->nextTextureKey;
            if (cursor)
            {
                MAT_SetPos(&cursor->transform1, (float)(carte->cursorSize.x - 0.5));
                MAT_SetPos(&cursor->transform2, (float)(carte->cursorSize.y - 0.5));
                cursor->subTextureFlags = cursor->subTextureFlags & ~0xBB000u | 0x44000;
                cursor->texElemIdx = carte->texElemIndices[carte->textElemIndexId[0] & 0xF];
                mat->solidVertexColor = 0x66000000;
                cursor->subTextureFlags &= ~0x800u;

                if (carte->objectCount > 1)
                {
                    k = 1;
                    while (carte->gao[k])
                    {
                        cursor = cursor->nextTextureKey;
                        if (!cursor)
                            break;

                        if ((carte->flags & 2) != 0
                            || (carte->textElemIndexId[k] & 0x10) == 0)
                        {
                            cursor->subTextureFlags &= ~0x800u;
                            invScale = 1.0 / carte->scale;
                            MAT_SetPos(&cursor->transform1,
                                       (float)(cursorX
                                               - GFX_l_CarteCursorPos(
                                                     carte->gao[k]->matrices->Pos.x,
                                                     carte->offset.x, carte->size.x, invScale)
                                               + carte->cursorSize.x));
                            MAT_SetPos(&cursor->transform2,
                                       cursorY
                                       - GFX_l_CarteCursorPos(
                                             carte->gao[k]->matrices->Pos.y,
                                             carte->offset.y, carte->size.y, invScale)
                                       + carte->cursorSize.y);
                            cursor->texElemIdx =
                                carte->texElemIndices[carte->textElemIndexId[k] & 0xF];
                        }
                        else
                        {
                            cursor->subTextureFlags |= 0x800u;
                        }

                        k++;
                        if (k >= carte->objectCount)
                        {
                            hideRest = 1;
                            break;
                        }
                    }
                }
                else
                {
                    hideRest = 1;
                }
            }
        }
    }

    if (hideRest)
    {
        for (chain = cursor->nextTextureKey; chain; chain = chain->nextTextureKey)
            chain->subTextureFlags |= 0x800u;
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}

struct sGfxCarteData* GFX_Carte_Create(void)
{
    tGfxCarteData* carte;

    carte = (tGfxCarteData*)MEM_p_Alloc(sizeof(tGfxCarteData));
    memset(carte, 0, sizeof(tGfxCarteData));
    carte->uncoverPiecePaletteID = -1;
    carte->pieceStatus = -1;
    carte->currentPieceStatus = -1;
    carte->effectTime = 0.2f;
    carte->remainingUncoverTime = 1.5f;
    carte->paletteCanBeUpdated = 1;
    carte->flags = 0x24;
    return carte;
}

static void GFX_l_CarteFadeIn(tGfxCarteData* carte)
{
    if ((carte->flags & 4) != 0)
    {
        carte->remainingEffectTime = carte->effectTime;
        carte->flags = carte->flags & ~4u | 8;
    }
    else if ((carte->flags & 0x10) != 0)
    {
        carte->remainingEffectTime = (float)(carte->effectTime - carte->remainingEffectTime);
        carte->flags = carte->flags & ~0x10u | 8;
    }
}

void GFX_Carte_Seti(struct sGfxCarteData* carte, unsigned int id, unsigned int value)
{
    switch (id)
    {
    case 16102u:
        if (value == 1)
        {
            if ((carte->flags & 0x40) != 0)
                carte->flags = carte->flags & ~0x40u | 4;
        }
        else if (!value)
        {
            if ((carte->flags & 0x40) == 0)
            {
                carte->flags |= 0x40;
                GFX_l_CarteFadeIn(carte);
            }
        }
        break;

    case 16100u:
        if (value == 1)
        {
            GFX_l_CarteFadeIn(carte);
        }
        else if (!value && (carte->flags & 4) == 0)
        {
            if ((carte->flags & 8) != 0)
            {
                carte->remainingEffectTime =
                    (float)(carte->effectTime - carte->remainingEffectTime);
                carte->flags = carte->flags & ~8u | 0x10;
            }
            else if ((carte->flags & 0x10) == 0)
            {
                carte->remainingEffectTime = carte->effectTime;
                carte->flags |= 0x10;
            }
        }
        break;

    case 16101u:
        if (value == 1)
        {
            if ((carte->flags & 0x20) != 0)
            {
                carte->currentPieceStatus = -1;
                carte->flags &= ~0x20u;
            }
        }
        else if (!value)
        {
            if ((carte->flags & 0x20) == 0)
            {
                carte->currentPieceStatus = -1;
                carte->flags |= 0x20;
            }
        }
        break;

    default:
        break;
    }
}

void GFX_Carte_Object(tGfxElem* gfxList, int16_t id, tGameObjectMain* gao, int mode,
                      int textElemIndexId)
{
    tGfxCarteData* carte;
    int i;

    GFX_SetCurrent(gfxList, id);
    if (!GFX_gpst_Current)
        return;

    carte = (tGfxCarteData*)GFX_gpst_Current->privateData;
    if (!mode)
    {
        if (!carte->objectCount)
            carte->objectCount = 1;
        carte->gao[0] = gao;
        carte->textElemIndexId[0] = textElemIndexId;
        return;
    }

    if (mode == 1)
    {
        if (carte->objectCount < 16)
        {
            carte->gao[carte->objectCount] = gao;
            carte->textElemIndexId[carte->objectCount++] = textElemIndexId;
        }
        return;
    }

    if (mode == -1)
    {
        for (i = 0; i < carte->objectCount; i++)
        {
            if (carte->gao[i] == gao)
                break;
        }
        if (i != carte->objectCount)
        {
            carte->objectCount--;
            carte->gao[i] = carte->gao[carte->objectCount];
            carte->textElemIndexId[i] = carte->textElemIndexId[carte->objectCount];
        }
    }
}

void GFX_Carte_Destroy(struct sGfxCarteData* carte, struct sGEOMAT* mat)
{
    tMultiTextureElemStruct* elem;
    tMultiTextureElemStruct* mapElem;
    tEngTexListElemStruct* tex;
    uint8_t* paletteData;

    if (!mat || mat->intf->id != 5 || mat->usageCount <= 0 || (carte->flags & 1) == 0)
        return;

    elem = (tMultiTextureElemStruct*)mat->idxOrPtr;
    if (!elem)
        return;
    mapElem = elem->nextTextureKey;
    if (!mapElem)
        return;

    tex = &TEX_gst_GlobalList.texElems[mapElem->texElemIdx];
    if ((tex->texElemFlag & 0x40) != 0)
    {
        paletteData = TEX_gst_GlobalList.palettes[tex->palettedData.paletteID].paletteData;
        if (paletteData)
            memcpy(paletteData, carte->paletteData, sizeof(carte->paletteData));
    }

    GFX_l_CarteRestoreTextures(carte, mapElem);
    carte->flags = 0;
}
