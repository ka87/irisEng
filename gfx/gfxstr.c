#include "../memory.h"
#include "../time.h"
#include "../graphicsobjects/geostr.h"
#include "../strings.h"
#include "gfxstr.h"

struct sGfxStrData* GFX_Str_Create(struct sGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;
    tGEOSTRStruct* str;
    tGfxStrData* gfx;

    if (!gao || !gao->basic)
        return 0;
    visual = (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
    if (!visual)
        return 0;
    str = (tGEOSTRStruct*)(visual->GROGEOOrSomethingElse);
    if (!str || str->intf->id != 10)
        return 0;

    gfx = (tGfxStrData*)MEM_p_Alloc(sizeof(tGfxStrData));
    gfx->gao = gao;
    gfx->num = 0;
    gfx->color1 = 0xFFFFFFFF;
    gfx->color2 = 0xFFFFFF00;
    gfx->ptr = 0;
    gfx->UVs = 0;
    gfx->timeElapsed = 5.0f;
    return gfx;
}

int GFX_i_Str_Render(struct sGfxStrData* gfx)
{
    tGeoTriangleChunkElem* tris;
    Vec3 screenPos;
    float remaining;
    float fade;
    unsigned int topColor;
    unsigned int bottomColor;
    int baseVertex;
    int letter;
    int i;

    if (!gfx->num)
        return 1;

    remaining = gfx->timeElapsed - TIM_gf_dt;
    gfx->timeElapsed = remaining;
    if (remaining < 0.0)
        return 0;

    if (remaining <= 4.8000002)
        fade = 1.0;
    else
        fade = (float)((5.0 - remaining) * 5.0);
    topColor = LIGHT_ul_Interpol2Colors(0, gfx->color1, fade);
    bottomColor = LIGHT_ul_Interpol2Colors(0, gfx->color2, fade);

    GFX_NeedGeom(4 * gfx->num, 4 * gfx->num, 2 * gfx->num, 1);
    memcpy(GFX_gpst_Geo->UVs, gfx->UVs, 4 * sizeof(tGeoUVElem) * gfx->num);

    for (i = 0; i < 4 * gfx->num; i++)
    {
        screenPos.x = (float)(gpst_DX_stDD->ct.cam.width * gfx->ptr[i].x
                              + (double)gpst_DX_stDD->ct.cam.viewPortRealPosX);
        screenPos.y = (float)(gfx->ptr[i].y * gpst_DX_stDD->ct.cam.height
                              + (double)gpst_DX_stDD->ct.cam.viewPortRealPosY);
        screenPos.z = gfx->ptr[i].z;
        CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &GFX_gpst_Geo->vertices[i], &screenPos);
    }

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    for (letter = 0; letter < gfx->num; letter++)
    {
        baseVertex = 4 * letter;

        tris[0].v1 = baseVertex;
        tris[0].uv1 = baseVertex;
        tris[0].v2 = baseVertex + 1;
        tris[0].uv2 = baseVertex + 1;
        tris[0].v3 = baseVertex + 2;
        tris[0].uv3 = baseVertex + 2;
        tris[1].v1 = baseVertex + 2;
        tris[1].uv1 = baseVertex + 2;
        tris[1].v2 = baseVertex + 3;
        tris[1].uv2 = baseVertex + 3;
        tris[1].v3 = baseVertex;
        tris[1].uv3 = baseVertex;

        GFX_gpst_Geo->vertexColors[baseVertex + 1] = topColor;
        GFX_gpst_Geo->vertexColors[baseVertex + 2] = bottomColor;
        GFX_gpst_Geo->vertexColors[baseVertex + 3] = bottomColor;
        GFX_gpst_Geo->vertexColors[baseVertex + 4] = topColor;

        tris += 2;
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}

void GFX_Str_Destroy(struct sGfxStrData* gfx)
{
    if (!gfx->num)
        return;

    MEM_Free(gfx->ptr);
    gfx->ptr = NULL;
    MEM_Free(gfx->UVs);
    gfx->UVs = NULL;
    gfx->num = 0;
}

void GFX_Str_Seti(struct sGfxStrData* gfx, int id, unsigned int idx)
{
    tGEOSTRStruct* str;
    tGEOSTRStringData* string;
    tGEOSTRLetter* letter;
    tFontGlypthElem* glyph;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3 min;
    Vec3 max;
    double top;
    double bottom;
    double right;
    float pivotX;
    float pivotY;
    float texWidth;
    float texHeight;
    float left;
    float advance;
    unsigned int materialId;
    unsigned int ch;
    unsigned int glyphIdx;
    int count;
    int i;

    if (id != 19100)
        return;

    str = STR_pst_Get(gfx->gao);
    if (!str)
        return;

    GFX_Str_Destroy(gfx);
    count = STR_l_GetLength(str, idx);
    if (!count || !STR_pst_GetString(str, idx))
        return;

    gfx->ptr = (Vec3*)MEM_p_Alloc(4 * sizeof(Vec3) * count);
    gfx->UVs = (tGeoUVElem*)MEM_p_Alloc(4 * sizeof(tGeoUVElem) * count);
    gfx->num = count;

    string = STR_pst_GetString(str, idx);
    verts = gfx->ptr;
    uvs = gfx->UVs;
    letter = &str->letters[string->index];
    pivotX = string->pivotPoint.x;
    pivotY = string->pivotPoint.y;

    if ((string->flags & 0xF) != 0)
    {
        STR_GetRectShort(str, idx, 0, -1, &min, &max);
        min.x = (float)(min.x / (double)(uint16_t)str->width);
        min.y = (float)(min.y / (double)(uint16_t)str->height);
        max.x = (float)(max.x / (double)(uint16_t)str->width);
        max.y = (float)(max.y / (double)(uint16_t)str->height);

        if ((string->flags & 3) == 1)
            pivotX = (float)(pivotX - (max.x + min.x) * 0.5);
        else if ((string->flags & 3) == 2)
            pivotX = (float)(pivotX - (max.x - min.x));

        if ((string->flags & 0xC) == 4)
            pivotY = (float)(pivotY - (max.y + min.y) * 0.5);
        else if ((string->flags & 0xC) == 8)
            pivotY = (float)(pivotY - (max.y - min.y));
    }

    for (i = 0; i < count; i++)
    {
        materialId = (letter->flagsAndChar >> 12) & 0x3F;
        if ((int16_t)materialId != str->fontMaterialID)
        {
            str->fontMaterialID = materialId;
            str->fontDesc = STR_pst_GetFontDesc(
                (tGEOMAT*)(((tGameObjectVisualObject*)
                            (STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT),
                (int16_t)materialId);
        }

        ch = (uint8_t)letter->flagsAndChar;
        glyphIdx = (ch < 32) ? 0 : ch - 32;
        glyph = &str->fontDesc->fge[glyphIdx];

        texWidth = (float)(uint16_t)str->width;
        texHeight = (float)(uint16_t)str->height;
        left = (float)((double)letter->x / texWidth + pivotX);
        top = (double)letter->y / texHeight + pivotY;
        advance = (float)((double)letter->width / texWidth);
        bottom = (double)(int16_t)letter->height / texHeight + top;
        right = advance + left;

        verts[0].x = left;
        verts[0].y = (float)top;
        verts[0].z = 0.059999999f;
        verts[1].x = left;
        verts[1].y = (float)bottom;
        verts[1].z = 0.059999999f;
        verts[2].x = (float)right;
        verts[2].y = (float)bottom;
        verts[2].z = 0.059999999f;
        verts[3].x = (float)right;
        verts[3].y = (float)top;
        verts[3].z = 0.059999999f;

        uvs[0].u = glyph->x;
        uvs[0].v = glyph->h;
        uvs[1].u = glyph->x;
        uvs[1].v = glyph->w;
        uvs[2].u = glyph->y;
        uvs[2].v = glyph->w;
        uvs[3].u = glyph->y;
        uvs[3].v = glyph->h;

        verts += 4;
        uvs += 4;
        letter++;
    }
}
