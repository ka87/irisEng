#include "../time.h"
#include "../memory.h"
#include "../textures.h"
#include "gfxcarte.h"

int i_LockMat_351 = 0; // TODO FIXME, game doesn't set it explicitly, check

void GFX_Carte_UpdatePalette(tGfxCarteData* carte, int16_t texelemIdx)
{
    tEngTexListElemStruct* v2;
    byte* v3;
    tEngPaletteElem* v4;
    byte v5;
    unsigned int v6;
    char v7;
    byte* v8;
    int v9;
    byte* v10;
    byte* v11;
    byte* v12;
    byte* v13;
    signed __int8 v14;
    double v15;
    double v19;
    int* v20;
    int* v21;
    int v22;
    float v23;
    float v24;
    tEngTexListElemStruct* v25;
    tEngPaletteElem* engPaletteElem;

    v2 = &TEX_gst_GlobalList.texElems[texelemIdx];
    v25 = v2;
    if ((v2->texElemFlag & 0x40) != 0)
    {
        v3 = TEX_gst_GlobalList.palettes[v2->palettedData.paletteID].paletteData;
        v4 = &TEX_gst_GlobalList.palettes[v2->palettedData.paletteID];
        engPaletteElem = v4;
        if (v3)
        {
            v5 = v4->flags;
            if ((v5 & 8) != 0 && (v5 & 1) == 0)
            {
                v6 = carte->pieceStatus;
                v7 = carte->flags;
                carte->currentPieceStatus = v6;
                v8 = v4->paletteData + 32;
                v9 = 31;
                if ((v7 & 0x20) != 0)
                {
                    do
                    {
                        v10 = &carte->paletteData[64 * (v6 & 1)];
                        v11 = v8;
                        v6 >>= 1;
                        v8 += 32;
                        --v9;
                        memcpy(v11, v10, 32u);
                    } while (v9);
                }
                else
                {
                    do
                    {
                        v12 = &carte->paletteData[32 * (v6 & 1) + 32];
                        v13 = v8;
                        v6 >>= 1;
                        v8 += 32;
                        --v9;
                        memcpy(v13, v12, 32u);
                    } while (v9);
                }
                v14 = carte->uncoverPiecePaletteID;
                if (v14 == -1)
                    goto LABEL_15;
                if (v14 >= 0 && v14 < 32)
                {
                    v15 = carte->remainingUncoverTime - TIM_gf_dt;
                    carte->remainingUncoverTime = (float)v15;
                    if (0.0 <= v15)
                    {
                        v23 = (float)(v15 * 6.2831855);
                        v19 = MATH_GetSin(v23);
                        v20 = (int*)&engPaletteElem->paletteData[32 * v14 + 32];
                        v24 = (float)fabs(v19);
                        v21 = (int*)&carte->paletteData[32 * ((carte->flags & 0x20) == 0)];
                        v22 = 8;
                        do
                        {
                            *v20 = LIGHT_ul_Interpol2Colors(*v20, *v21, v24);
                            ++v20;
                            ++v21;
                            --v22;
                        } while (v22);
                        goto LABEL_15;
                    }
                    carte->remainingUncoverTime = 1.5;
                }
                carte->uncoverPiecePaletteID = -1;
            LABEL_15:
                gpst_DX_stDD->Request(22, (void *)(v25->palettedData.paletteID));
                return;
            }
        }
    }
}

int GFX_i_Carte_Render(struct sGfxCarteData* carte, struct sGEOMAT* mat, struct sGameObjectMain* a3)
{
    tMultiTextureElemStruct* v4;
    tMultiTextureElemStruct* v5;
    tEngTexListElemStruct* v6; 
    byte* v7;
    int v8; 
    int* v9;
    tMultiTextureElemStruct* v10;
    tMultiTextureElemStruct* v11;
    tMultiTextureElemStruct* v12;
    tMultiTextureElemStruct* v13;
    tMultiTextureElemStruct* v14;
    tMultiTextureElemStruct* v15;
    tMultiTextureElemStruct* v16;
    Vec3* v17;
    double v18;
    double v19;
    double v20;
    BOOL v21;
    unsigned int v22;
    double v23;
    Vec3* v24;
    double v25; 
    int v26; 
    Vec3* v27;
    double v28;
    Vec3* v29; 
    double v30;
    double v31;
    Vec3* v32; 
    tViewElem* v33;
    tGdi_DisplayData* v34;
    float v35;
    double v36;
    double v37;
    double v38;
    double v39;
    double v40;
    double v41;
    double v42;
    double v43;
    double v44;
    double v45;
    double v46;
    double v47;
    double v48;
    int v49;
    double v50;
    Vec3* v51;
    int v52; 
    int v53; 
    double v54;
    int v55;
    int v56;
    tMultiTextureElemStruct* v57;
    tMultiTextureElemStruct* v58;
    tMultiTextureElemStruct* v60;
    Mtx4x4* v61;
    double v62;
    double v63;
    Vec3* v64;
    __int16 v65;
    int v66;
    tGfxCarteData* v67;
    double v68;
    double v69;
    tMultiTextureElemStruct* i;
    int v71;
    float v72;
    float v73;
    float v74;
    float v75;
    float v76;
    float v77;
    float v78;
    float v79;
    float v80;
    float v81;
    Vec3 a1a; 
    Vec3 a3a; 
    Vec3 dest;
    float v85;
    float v86;
    float cartea;
    float a2b;
    float a2c;
    float a2; 
    float a2e;
    float a2a;

    if (!mat || mat->intf->id != 5 || !a3 || !carte->objectCount || !carte->gao[0])
        return 1;
    if ((carte->flags & 1) == 0)
    {
        v4 = (tMultiTextureElemStruct*)mat->idxOrPtr;
        if (!v4)
            return 1;
        v5 = v4->nextTextureKey;
        if (!v5)
            return 1;
        v5->subTextureFlags = v5->subTextureFlags & ~0xB000u | 0x4000;
        v6 = &TEX_gst_GlobalList.texElems[v5->texElemIdx];
        if ((v6->texElemFlag & 0x40) != 0)
        {
            v7 = TEX_gst_GlobalList.palettes[v6->palettedData.paletteID].paletteData;
            if (v7)
                memcpy(carte->paletteData, v7, sizeof(carte->paletteData));
            else
                carte->paletteCanBeUpdated = 0;
        }
        v8 = 0;
        v9 = &carte->texElemIndices[1];
        do
        {
            v10 = v5->nextTextureKey;
            if (!v10)
                break;
            *(v9 - 1) = v10->texElemIdx;
            v11 = v10->nextTextureKey;
            if (!v11)
                break;
            *v9 = v11->texElemIdx;
            v12 = v11->nextTextureKey;
            if (!v12)
                break;
            v9[1] = v12->texElemIdx;
            v13 = v12->nextTextureKey;
            if (!v13)
                break;
            v9[2] = v13->texElemIdx;
            v14 = v13->nextTextureKey;
            if (!v14)
                break;
            v9[3] = v14->texElemIdx;
            v15 = v14->nextTextureKey;
            if (!v15)
                break;
            v9[4] = v15->texElemIdx;
            v16 = v15->nextTextureKey;
            if (!v16)
                break;
            v9[5] = v16->texElemIdx;
            v5 = v16->nextTextureKey;
            if (!v5)
                break;
            v9[6] = v5->texElemIdx;
            v8 += 8;
            v9 += 8;
        } while (v8 < 16);
        carte->flags |= 1u;
    }
    if ((carte->flags & 4) == 0)
    {
        GFX_NeedGeom(4u, 4u, 2u, 0);
        GFX_gpst_Geo->UVs->u = 1.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 0.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 0.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 1.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;
        v17 = GFX_gpst_Geo->vertices;
        if ((carte->flags & 0x40) != 0)
        {
            a2b = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
            v18 = carte->anotherPos[0].x * gpst_DX_stDD->ct.cam.width + a2b;
            v19 = carte->anotherPos[1].x * gpst_DX_stDD->ct.cam.width + a2b;
            a2c = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
            v20 = carte->anotherPos[0].y * gpst_DX_stDD->ct.cam.height + a2c;
            v22 = carte->PositionToRotationVertexIdx & 0x80000003;
            v21 = carte->PositionToRotationVertexIdx < 0;
            a2 = carte->anotherPos[1].y * gpst_DX_stDD->ct.cam.height + a2c;
            a3a.x = (float)((v19 + v18) * 0.5);
            a3a.y = (float)((a2 + v20) * 0.5);
            v23 = carte->pos2D[0].z;
            if (v21)
                v22 = (((byte)v22 - 1) | ~3u) + 1;
            v24 = &v17[v22];
            v24->x = (float)v18;
            v24->y = (float)v20;
            v24->z = (float)v23;
            v25 = carte->pos2D[0].z;
            v26 = (carte->PositionToRotationVertexIdx + 1) % 4;
            v17[v26].x = (float)v19;
            v27 = &v17[v26];
            v27->y = (float)v20;
            v27->z = (float)v25;
            v28 = carte->pos2D[0].z;
            v29 = &v17[(carte->PositionToRotationVertexIdx + 2) % 4];
            v29->x = (float)v19;
            v29->y = a2;
            v29->z = (float)v28;
            v30 = v18;
            v31 = carte->pos2D[0].z;
            v32 = &v17[(carte->PositionToRotationVertexIdx + 3) % 4];
            v32->x = (float)v30;
            v32->y = a2;
            v32->z = (float)v31;
        }
        else
        {
            if (WOR_gpst_CurrentWorld && (v33 = WOR_gpst_CurrentWorld->viewPtr) != 0)
            {
                MATH_VecInvert(&a1a, &v33->cameraMtx.v2);
                v34 = gpst_DX_stDD;
            }
            else
            {
                v34 = gpst_DX_stDD;
                v35 = gpst_DX_stDD->ct.cam.objectMtx.v3.y;
                a1a.x = gpst_DX_stDD->ct.cam.objectMtx.v3.x;
                a1a.y = v35;
            }
            a1a.z = 0.0;
            if (a1a.x == 0.0 && a1a.y == 0.0)
                return 1;
            MATH_NormalizeEqualVector(&a1a);
            v36 = (double)v34->ct.cam.viewPortRealPosX;
            cartea = (float)(v34->ct.cam.width * carte->pos2D[0].x + v36);
            v81 = (float)(carte->pos2D[1].x * v34->ct.cam.width + v36);
            v37 = (double)v34->ct.cam.viewPortRealPosY;
            v80 = (float)(carte->pos2D[0].y * v34->ct.cam.height + v37);
            a2e = (float)(carte->pos2D[1].y * v34->ct.cam.height + v37);
            a3a.x = (float)((v81 + cartea) * 0.5);
            a3a.y = (float)((a2e + v80) * 0.5);
            v38 = cartea - a3a.x;
            dest.x = (float)v38;
            v39 = v80 - a3a.y;
            v85 = (float)(a1a.x * v39 + (float)(dest.x * a1a.y));
            v40 = v39 * a1a.y - dest.x * a1a.x;
            v41 = carte->pos2D[0].z;
            v17->x = v85 + a3a.x;
            v17->y = (float)(v40 + a3a.y);
            v17->z = (float)v41;
            v42 = v81 - a3a.x;
            v85 = (float)(a1a.x * v39 + v42 * a1a.y);
            v43 = v39 * a1a.y - v42 * a1a.x;
            v44 = carte->pos2D[0].z;
            v17[1].x = v85 + a3a.x;
            v17[1].y = (float)(v43 + a3a.y);
            dest.z = 0.0;
            v17[1].z = (float)v44;
            dest.x = (float)v42;
            v45 = a2e - a3a.y;
            v85 = (float)(a1a.x * v45 + dest.x * a1a.y);
            v46 = v45 * a1a.y - dest.x * a1a.x;
            v47 = carte->pos2D[0].z;
            v17[2].x = v85 + a3a.x;
            v17[2].y = (float)(v46 + a3a.y);
            v17[2].z = (float)v47;
            dest.x = (float)v38;
            dest.y = (float)v45;
            v86 = dest.y * a1a.y - dest.x * a1a.x;
            v48 = carte->pos2D[0].z;
            v17[3].x = dest.y * a1a.x + dest.x * a1a.y + a3a.x;
            v17[3].y = v86 + a3a.y;
            v17[3].z = (float)v48;
        }
        v49 = carte->flags;
        if ((v49 & 0x18) != 0)
        {
            v50 = carte->remainingEffectTime - TIM_gf_dt;
            carte->remainingEffectTime = (float)v50;
            a2a = (float)(v50 / carte->effectTime);
            if ((v49 & 0x10) != 0)
            {
                if (a2a < 0.0)
                {
                    carte->flags = v49 & ~0x10u | 4;
                    a2a = 0.0;
                }
            }
            else if (carte->remainingEffectTime >= 0.0)
            {
                a2a = (float)(1.0 - a2a);
            }
            else
            {
                carte->flags = v49 & ~8u;
                a2a = 1.0;
            }
            a3a.z = carte->pos2D[0].z;
            v51 = v17;
            v52 = 4;
            do
            {
                MATH_SubstractVec(&dest, v51, &a3a);
                MATH_VecMultScalarWithoutBuffer(&dest, a2a);
                MATH_VecAddVec(v51++, &a3a, &dest);
                --v52;
            } while (v52);
        }
        v53 = 4;
        do
        {
            CAM_2Dto3D(&gpst_DX_stDD->ct.cam, v17, v17);
            ++v17;
            --v53;
        } while (v53);
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v1 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v2 = 1;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v3 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv1 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv2 = 1;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv3 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v1 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v2 = 3;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v3 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv1 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv2 = 3;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv3 = 0;
        if (i_LockMat_351)
            goto LABEL_81;
        v54 = carte->remainingTime - TIM_gf_dt;
        carte->remainingTime = (float)v54;
        if (v54 < 0.0)
        {
            carte->remainingTime = carte->blinkTime;
            v55 = carte->flags;
            if ((v55 & 2) != 0)
                v56 = v55 & ~2u;
            else
                v56 = v55 | 2;
            carte->flags = v56;
        }
        v57 = (tMultiTextureElemStruct*)mat->idxOrPtr;
        if (!v57)
            return 1;
        v58 = v57->nextTextureKey;
        if (!v58)
            return 1;
        if (carte->pieceStatus == carte->currentPieceStatus && carte->uncoverPiecePaletteID == 0xFF)
        {
            carte->remainingUncoverTime = 1.5;
        }
        else
        {
            if (carte->paletteCanBeUpdated)
                GFX_Carte_UpdatePalette(carte, v58->texElemIdx);
            carte->currentPieceStatus = carte->pieceStatus;
        }
        if ((carte->flags & 0x40) != 0)
        {
            v58->subTextureFlags = v58->subTextureFlags & ~0xA0000u | 0x50000;
            MAT_SetScale(&v58->transform1, 1.0);
            v72 = 1.0;
        }
        else
        {
            v58->subTextureFlags = v58->subTextureFlags & ~0xC0000u | 0x30000;
            MAT_SetScale(&v58->transform1, carte->scale);
            v72 = carte->scale;
        }
        MAT_SetScale(&v58->transform2, v72);
        if (!carte->objectCount)
            goto LABEL_81;
        if ((carte->flags & 0x40) != 0)
        {
            MAT_SetPos(&v58->transform1, 0.0);
            MAT_SetPos(&v58->transform2, 0.0);
            v60 = v58->nextTextureKey;
            if (v60)
            {
                v60->texElemIdx = carte->texElemIndices[carte->textElemIndexId[0] & 0xF];
                v61 = carte->gao[0]->matrices;
                v62 = -(1.0 - (v61->Pos.x - carte->offset.x) / carte->size.x);
                v86 = (float)(-(1.0 - (v61->Pos.y - carte->offset.y) / carte->size.y));
                v73 = (float)(v62 + carte->cursorSize.x);
                MAT_SetPos(&v60->transform1, v73);
                v74 = v86 + carte->cursorSize.y;
                MAT_SetPos(&v60->transform2, v74);
                v75 = (float)(fabs(sin(TIM_gf_MainClock * 4.0)));
                mat->solidVertexColor = LIGHT_ul_Interpol2Colors(0xFF00007F, 0xFF00FFFF, v75);
                v60->subTextureFlags = v60->subTextureFlags & 0xFFF627FF | 0x62000;
            LABEL_79:
                for (i = v60->nextTextureKey; i; i = i->nextTextureKey)
                    i->subTextureFlags |= 0x800u;
                goto LABEL_81;
            }
        }
        else
        {
            v63 = 1.0 / carte->scale;
            v64 = &carte->gao[0]->matrices->Pos;
            v85 = (float)((1.0 - (v64->x - carte->offset.x) / carte->size.x) * v63 - 0.5);
            v86 = (float)((1.0 - (v64->y - carte->offset.y) / carte->size.y) * v63 - 0.5);
            MAT_SetPos(&v58->transform1, v85);
            MAT_SetPos(&v58->transform2, v86);
            v60 = v58->nextTextureKey;
            if (v60)
            {
                v76 = (float)(carte->cursorSize.x - 0.5);
                MAT_SetPos(&v60->transform1, v76);
                v77 = (float)(carte->cursorSize.y - 0.5);
                MAT_SetPos(&v60->transform2, v77);
                v65 = carte->texElemIndices[carte->textElemIndexId[0] & 0xF];
                v60->subTextureFlags = v60->subTextureFlags & ~0xBB000u | 0x44000;
                v60->texElemIdx = v65;
                mat->solidVertexColor = 0x66000000;
                v60->subTextureFlags &= ~0x800u;
                v66 = 1;
                if (carte->objectCount > 1)
                {
                    v67 = carte;
                    while (v67->gao[v66])
                    {
                        v60 = v60->nextTextureKey;
                        if (!v60)
                            break;
                        if ((carte->flags & 2) != 0 || (v67->textElemIndexId[v66] & 0x10) == 0)
                        {
                            v60->subTextureFlags &= ~0x800u;
                            v68 = 1.0 / carte->scale;
                            v69 = (1.0 - (v67->gao[v66]->matrices->Pos.x - carte->offset.x) / carte->size.x) * v68;
                            dest.y = (float)((1.0 - (v67->gao[v66]->matrices->Pos.y - carte->offset.y) / carte->size.y) * v68);
                            v78 = (float)(v85 - v69 + carte->cursorSize.x);
                            MAT_SetPos(&v60->transform1, v78);
                            v79 = v86 - dest.y + carte->cursorSize.y;
                            MAT_SetPos(&v60->transform2, v79);
                            v60->texElemIdx = carte->texElemIndices[v67->textElemIndexId[v66] & 0xF];
                        }
                        else
                        {
                            v60->subTextureFlags |= 0x800u;
                        }
                        ++v66;
                        if (v66 >= carte->objectCount)
                            goto LABEL_79;
                    }
                    goto LABEL_81;
                }
                goto LABEL_79;
            }
        }
    LABEL_81:
        v71 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v71 & ~0x8000900u;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v71;
    }
    return 1;
}

struct sGfxCarteData* GFX_Carte_Create()
{
    tGfxCarteData* v0;

    v0 = (tGfxCarteData*)MEM_p_Alloc(sizeof(tGfxCarteData));
    memset(v0, 0, sizeof(tGfxCarteData));
    v0->uncoverPiecePaletteID = -1;
    v0->pieceStatus = -1;
    v0->currentPieceStatus = -1;
    v0->effectTime = 0.2f;
    v0->remainingUncoverTime = 1.5f;
    v0->paletteCanBeUpdated = 1;
    v0->flags = 0x24;
    return v0;
}

void GFX_Carte_Seti(struct sGfxCarteData* a1, unsigned int a2, unsigned int a3)
{
    int v3;
    int v4;
    int v5;
    double v6;
    int v7;
    double v8;
    int v9;
    int v10;

    switch (a2)
    {
    case 16102u:
        if (a3 == 1)
        {
            v3 = a1->flags;
            if ((v3 & 0x40) != 0)
                a1->flags = v3 & ~0x40u | 4;
            return;
        }
        if (!a3)
        {
            v4 = a1->flags;
            if ((v4 & 0x40) == 0)
            {
                a1->flags = v4 | 0x40;
                goto LABEL_10;
            }
        }
        break;
    case 16100u:
        if (a3 == 1)
        {
        LABEL_10:
            v5 = a1->flags;
            if ((v5 & 4) != 0)
            {
                a1->remainingEffectTime = a1->effectTime;
                a1->flags = v5 & ~4u | 8;
            }
            else if ((v5 & 0x10) != 0)
            {
                v6 = a1->effectTime - a1->remainingEffectTime;
                a1->flags = v5 & ~0x10u | 8;
                a1->remainingEffectTime = (float)v6;
            }
            return;
        }
        if (!a3)
        {
            v7 = a1->flags;
            if ((v7 & 4) == 0)
            {
                if ((v7 & 8) != 0)
                {
                    v8 = a1->effectTime - a1->remainingEffectTime;
                    a1->flags = v7 & ~8u | 0x10;
                    a1->remainingEffectTime = (float)v8;
                }
                else if ((v7 & 0x10) == 0)
                {
                    a1->remainingEffectTime = a1->effectTime;
                    a1->flags = v7 | 0x10;
                }
            }
        }
        break;
    case 16101u:
        if (a3 == 1)
        {
            v9 = a1->flags;
            if ((v9 & 0x20) != 0)
            {
                a1->currentPieceStatus = -1;
                a1->flags = v9 & ~0x20u;
            }
        }
        else if (!a3)
        {
            v10 = a1->flags;
            if ((v10 & 0x20) == 0)
            {
                a1->currentPieceStatus = -1;
                a1->flags = v10 | 0x20;
            }
        }
        break;
    }
}

void GFX_Carte_Object(tGfxElem* a1, __int16 id, tGameObjectMain* a3, int a4, int textElemIndexId)
{
    tGfxCarteData* v5;
    int v6;
    int v7;
    int v8;
    tGameObjectMain** v9;

    GFX_SetCurrent(a1, id);
    if (GFX_gpst_Current)
    {
        v5 = (tGfxCarteData*)GFX_gpst_Current->privateData;
        if (a4)
        {
            if (a4 == 1)
            {
                v6 = v5->objectCount;
                if (v6 < 16)
                {
                    v5->gao[v6] = a3;
                    v5->textElemIndexId[v5->objectCount++] = textElemIndexId;
                }
            }
            else if (a4 == -1)
            {
                v7 = v5->objectCount;
                v8 = 0;
                if (v7 > 0)
                {
                    v9 = v5->gao;
                    do
                    {
                        if (*v9 == a3)
                            break;
                        ++v8;
                        ++v9;
                    } while (v8 < v5->objectCount);
                }
                if (v8 != v7)
                {
                    v5->objectCount = v7 - 1;
                    v5->gao[v8] = v5->gao[v7 - 1];
                    v5->textElemIndexId[v8] = v5->textElemIndexId[v5->objectCount];
                }
            }
        }
        else
        {
            if (!v5->objectCount)
                v5->objectCount = 1;
            v5->gao[0] = a3;
            v5->textElemIndexId[0] = textElemIndexId;
        }
    }
}

void GFX_Carte_Destroy(struct sGfxCarteData* a1, struct sGEOMAT* a2)
{
    tMultiTextureElemStruct* v2;
    tMultiTextureElemStruct* v3;
    tEngTexListElemStruct* v4;
    byte* v5;
    int v6;
    int* v7;
    tMultiTextureElemStruct* v8;
    tMultiTextureElemStruct* v9;
    tMultiTextureElemStruct* v10;
    tMultiTextureElemStruct* v11;
    tMultiTextureElemStruct* v12;
    tMultiTextureElemStruct* v13;
    tMultiTextureElemStruct* v14;
    __int16 v15;

    if (a2)
    {
        if (a2->intf->id == 5 && a2->usageCount > 0 && (a1->flags & 1) != 0)
        {
            v2 = (tMultiTextureElemStruct*)a2->idxOrPtr;
            if (v2)
            {
                v3 = v2->nextTextureKey;
                if (v3)
                {
                    v4 = &TEX_gst_GlobalList.texElems[v3->texElemIdx];
                    if ((v4->texElemFlag & 0x40) != 0)
                    {
                        v5 = TEX_gst_GlobalList.palettes[v4->palettedData.paletteID].paletteData;
                        if (v5)
                            memcpy(v5, a1->paletteData, sizeof(a1->paletteData));
                    }
                    v6 = 0;
                    v7 = a1->texElemIndices;
                    do
                    {
                        v8 = v3->nextTextureKey;
                        if (!v8)
                            break;
                        v8->texElemIdx = v7[v6] & 0xffff;
                        v9 = v8->nextTextureKey;
                        if (!v9)
                            break;
                        v9->texElemIdx = v7[v6 + 1] & 0xffff;
                        v10 = v9->nextTextureKey;
                        if (!v10)
                            break;
                        v10->texElemIdx = v7[v6 + 2] & 0xffff;
                        v11 = v10->nextTextureKey;
                        if (!v11)
                            break;
                        v11->texElemIdx = v7[v6 + 3] & 0xffff;
                        v12 = v11->nextTextureKey;
                        if (!v12)
                            break;
                        v12->texElemIdx = v7[v6 + 4] & 0xffff;
                        v13 = v12->nextTextureKey;
                        if (!v13)
                            break;
                        v13->texElemIdx = v7[v6 + 5] & 0xffff;
                        v14 = v13->nextTextureKey;
                        if (!v14)
                            break;
                        v14->texElemIdx = v7[v6 + 6] & 0xffff;
                        v3 = v14->nextTextureKey;
                        if (!v3)
                            break;
                        v15 = v7[v6 + 7] & 0xffff;
                        v6 += 8;
                        v3->texElemIdx = v15;
                    } while (v6 < 16);
                    a1->flags = 0;
                }
            }
        }
    }
}