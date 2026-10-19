#include "../memory.h"
#include "../time.h"
#include "../graphicsobjects/geostr.h"
#include "../strings.h"
#include "gfxstr.h"

struct sGfxStrData* GFX_Str_Create(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGameObjectVisualObject* v2;
    tGEOSTRStruct* v3;
    tGfxStrData* result;

    if (!a1)
        return 0;
    v1 = a1->basic;
    if (!v1)
        return 0;
    v2 = (tGameObjectVisualObject*)(v1->actorObjectOrVisualObject);
    if (!v2)
        return 0;
    v3 = (tGEOSTRStruct*)(v2->GROGEOOrSomethingElse);
    if (!v3 || v3->intf->id != 10)
        return 0;
    result = (tGfxStrData*)MEM_p_Alloc(sizeof(tGfxStrData));
    result->gao = a1;
    result->num = 0;
    result->color1 = 0xFFFFFFFF;
    result->color2 = 0xFFFFFF00;
    result->ptr = 0;
    result->UVs = 0;
    result->timeElapsed = 5.0f;
    return result;
}

int GFX_i_Str_Render(struct sGfxStrData* a1)
{
    unsigned int v3;
    int v4;
    int v5;
    Vec3* v6;
    tGeoTriangleChunkElem* v7;
    int v8;
    int v9;
    unsigned __int16 v10;
    unsigned __int16 v11;
    tGeoTriangleChunkElem* v12;
    unsigned __int16 v13;
    int v14;
    Vec3 a3;
    float v16;
    float v17;
    int v18;

    if (a1->num)
    {
        v16 = a1->timeElapsed - TIM_gf_dt;
        a1->timeElapsed = v16;
        if (v16 < 0.0)
            return 0;
        if (v16 <= 4.8000002)
            v17 = 1.0;
        else
            v17 = (float)((5.0 - v16) * 5.0);
        v3 = LIGHT_ul_Interpol2Colors(0, a1->color1, v17);
        v18 = LIGHT_ul_Interpol2Colors(0, a1->color2, v17);
        GFX_NeedGeom(4 * a1->num, 4 * a1->num, 2 * a1->num, 1);
        memcpy(GFX_gpst_Geo->UVs, a1->UVs, 4 * (sizeof(tGeoUVElem)) * a1->num);
        v4 = 0;
        if (4 * a1->num > 0)
        {
            v5 = 0;
            do
            {
                v6 = a1->ptr;
                a3.x = (float)(gpst_DX_stDD->ct.cam.width * v6[v5].x + (double)gpst_DX_stDD->ct.cam.viewPortRealPosX);
                a3.y = (float)(v6[v5].y * gpst_DX_stDD->ct.cam.height + (double)gpst_DX_stDD->ct.cam.viewPortRealPosY);
                a3.z = v6[v5].z;
                CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &GFX_gpst_Geo->vertices[v5], &a3);
                ++v4;
                ++v5;
            } while (v4 < 4 * a1->num);
        }
        v7 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        v8 = 0;
        if (a1->num > 0)
        {
            v9 = 0;
            do
            {
                v7->v1 = 4 * v8;
                v7->uv1 = 4 * v8;
                v10 = 4 * v8 + 1;
                v7->v2 = v10;
                v7->uv2 = v10;
                v11 = 4 * v8 + 2;
                v7->v3 = v11;
                v7->uv3 = v11;
                v7[1].v1 = v11;
                v7[1].uv1 = v11;
                v7[1].v3 = 4 * v8;
                v7[1].uv3 = 4 * v8;
                v12 = v7 + 1;
                v13 = 4 * v8 + 3;
                v12->v2 = v13;
                v12->uv2 = v13;
                GFX_gpst_Geo->vertexColors[v9 + 1] = v3;
                GFX_gpst_Geo->vertexColors[v9 + 2] = v18;
                GFX_gpst_Geo->vertexColors[v9 + 3] = v18;
                v7 = v12 + 1;
                GFX_gpst_Geo->vertexColors[v9 + 4] = v3;
                ++v8;
                v9 += 4;
            } while (v8 < a1->num);
        }
        v14 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v14 & ~0x8000900u;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v14;
    }
    return 1;
}

void GFX_Str_Destroy(struct sGfxStrData* a1)
{
    tGeoUVElem* v1;

    if (a1->num)
    {
        MEM_Free(a1->ptr);
        v1 = a1->UVs;
        a1->ptr = NULL;
        MEM_Free(v1);
        a1->UVs = NULL;
        a1->num = 0;
    }
}

void GFX_Str_Seti(struct sGfxStrData* a1, int a2, unsigned int idx)
{
    tGEOSTRStruct* v3;
    int v4;
    tGEOSTRStringData* v5; 
    Vec3* v6;
    tGeoUVElem* v7;
    tGEOSTRLetter* v8;
    double v9;
    __int16 v10;
    double v11;
    unsigned int v12;
    unsigned int v13;
    unsigned int v14;
    tFontGlypthElem* v15;
    double v16;
    int v17;
    int v18;
    double v19;
    double v20;
    double v21;
    float v22;
    float v23;
    int v24;
    float v25;
    float v26;
    tGEOSTRStringData* v27;
    float v28;
    int v29;
    int v30;
    Vec3 max; 
    Vec3 min; 
    float a2a;

    if (a2 != 19100)
        return;
    v3 = STR_pst_Get(a1->gao);
    if (!v3)
        return;
    GFX_Str_Destroy(a1);
    v4 = STR_l_GetLength(v3, idx);
    v24 = v4;
    if (!v4 || !STR_pst_GetString(v3, idx))
        return;
    a1->ptr = (Vec3*)MEM_p_Alloc(4 * sizeof(Vec3) * v4);       // betunkent 4 pont (Vec3 12 *4 = 48)
    a1->UVs = (tGeoUVElem*)MEM_p_Alloc(4 * sizeof(tGeoUVElem) * v4);
    a1->num = v4;
    v5 = STR_pst_GetString(v3, idx);
    v6 = a1->ptr;
    v7 = a1->UVs;
    v8 = &v3->letters[v5->index];
    a2a = v5->pivotPoint.x;
    v27 = v5;
    v22 = v5->pivotPoint.y;
    if ((v5->flags & 0xF) == 0)
        goto LABEL_16;
    STR_GetRectShort(v3, idx, 0, -1, &min, &max);
    min.x = (float)(min.x / (double)(uint16_t)v3->width);
    min.y = (float)(min.y / (double)(uint16_t)v3->height);
    max.x = (float)(max.x / (double)(uint16_t)v3->width);
    max.y = (float)(max.y / (double)(uint16_t)v3->height);
    if ((v27->flags & 3) == 1)
    {
        v9 = (max.x + min.x) * 0.5;
    LABEL_10:
        a2a = (float)(a2a - v9);
        goto LABEL_11;
    }
    if ((v27->flags & 3) == 2)
    {
        v9 = max.x - min.x;
        goto LABEL_10;
    }
LABEL_11:
    v10 = v27->flags & 0xC;
    if (v10 == 4)
    {
        v11 = (max.y + min.y) * 0.5;
    }
    else
    {
        if (v10 != 8)
            goto LABEL_16;
        v11 = max.y - min.y;
    }
    v22 = (float)(v22 - v11);
LABEL_16:
    v29 = v24;
    do
    {
        v12 = (v8->flagsAndChar >> 12) & 0x3F;
        if ((int16_t)v12 != v3->fontMaterialID)
        {
            v3->fontMaterialID = v12;
            v3->fontDesc = STR_pst_GetFontDesc((tGEOMAT*)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), (__int16)v12);
        }
        v13 = (unsigned __int8)v8->flagsAndChar;
        if (v13 < 32)
            v14 = 0;
        else
            v14 = v13 - 32;
        v15 = &v3->fontDesc->fge[v14];
        v25 = (float)(uint16_t)v3->width;
        v16 = (double)v8->x;
        v17 = v8->width;
        v30 = v8->y;
        v18 = (__int16)v8->height;
        v7 += 4;
        v6 += 4;
        ++v8;
        v23 = (float)(v16 / v25 + a2a);
        v28 = (float)(uint16_t)v3->height;
        v6[-4].x = v23;
        v6[-4].z = 0.059999999f;
        v19 = (double)v30 / v28 + v22;
        v26 = (float)((double)v17 / v25);
        v6[-4].y = (float)v19;
        v7[-4].u = v15->x;
        v20 = (double)v18 / v28 + v19;
        v7[-4].v = v15->h;
        v6[-3].y = (float)v20;
        v6[-3].x = v23;
        v6[-3].z = 0.059999999f;
        v21 = v26 + v23;
        v7[-3].u = v15->x;
        v7[-3].v = v15->w;
        v6[-2].x = (float)v21;
        v6[-2].z = 0.059999999f;
        v6[-2].y = (float)v20;
        v7[-2].u = v15->y;
        v7[-2].v = v15->w;
        v6[-1].x = (float)v21;
        v6[-1].z = 0.059999999f;
        v6[-1].y = (float)v19;
        v7[-1].u = v15->y;
        v7[-1].v = v15->h;
        --v29;
    } while (v29);
}