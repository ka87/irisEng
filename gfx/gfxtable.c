#include "../memory.h"
#include "../time.h"
#include "gfxtable.h"

int GFX_i_Table_Render(struct sGfxTableData* table)
{
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3 screenPos;
    int* colors;
    double x;
    double y;
    double xLeft;
    float camWidth;
    float camHeight;
    float yTop;
    int color;
    int baseVertex;
    int cells;
    int row;
    int col;
    int i;

    cells = table->num2 * table->num1;
    if (!cells)
        return 1;

    GFX_NeedGeom(4 * cells, 4u, 2 * cells, 1);
    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;

    camWidth = gpst_DX_stDD->ct.cam.width;
    camHeight = gpst_DX_stDD->ct.cam.height;
    verts = GFX_gpst_Geo->vertices;
    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    colors = GFX_gpst_Geo->vertexColors + 1;
    baseVertex = 0;

    y = table->ff2 * -camHeight + camHeight;
    for (row = 0; row < table->num1; row++)
    {
        yTop = (float)y;
        y = y + -camHeight * table->f[row];
        x = camWidth * table->ff1;
        for (col = 0; col < table->num2; col++)
        {
            xLeft = x;
            color = table->colors[table->vertexColorIndexes[10 * row + col]];
            x = x + camWidth * table->f2[col];

            tris[0].v1 = baseVertex;
            tris[0].v2 = baseVertex + 1;
            tris[0].v3 = baseVertex + 2;
            tris[0].uv1 = 0;
            tris[0].uv2 = 1;
            tris[0].uv3 = 2;
            tris[1].v1 = baseVertex + 2;
            tris[1].v2 = baseVertex + 3;
            tris[1].v3 = baseVertex;
            tris[1].uv1 = 2;
            tris[1].uv2 = 3;
            tris[1].uv3 = 0;

            verts[0].x = (float)xLeft;
            verts[0].y = yTop;
            verts[0].z = table->ff3;
            verts[1].x = (float)x;
            verts[1].y = yTop;
            verts[1].z = table->ff3;
            verts[2].x = (float)x;
            verts[2].y = (float)y;
            verts[2].z = table->ff3;
            verts[3].x = (float)xLeft;
            verts[3].y = (float)y;
            verts[3].z = table->ff3;

            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;

            tris += 2;
            verts += 4;
            colors += 4;
            baseVertex += 4;
        }
    }

    verts = GFX_gpst_Geo->vertices;
    for (i = 0; i < 4 * cells; i++)
    {
        CAM_2Dto3DCamera2(&gpst_DX_stDD->ct.cam, &screenPos, &verts[i]);
        MATH_TransformVertex(&verts[i], &gpst_DX_stDD->ct.cam.objectMtx, &screenPos);
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}

void GFX_Table_Setv(struct sGfxTableData* table, int id, Vec3* value)
{
    unsigned int index;
    unsigned int rowIdx;
    unsigned int colIdx;
    unsigned int colorIdx;
    char* slot;
    int i;

    switch (id)
    {
    case 7201:
        index = (unsigned int)value->x;
        if ((int)index == -1)
        {
            for (i = 0; i < 10; i++)
                table->f[i] = value->y;
        }
        // the bound is the array length, not its last index, so an index of
        // ten writes f2[0]; it stays inside the struct
        else if ((index & 0x80000000) == 0 && (int)index <= 10)
        {
            table->f[index] = value->y;
        }
        break;

    case 7202:
        index = (unsigned int)value->x;
        if ((int)index == -1)
        {
            for (i = 0; i < 10; i++)
                table->f2[i] = value->y;
            *(float*)table->vertexColorIndexes = value->y; // TODO FIXME: ez jó így??
        }
        else if ((index & 0x80000000) == 0 && (int)index <= 10)
        {
            table->f2[index] = value->y;
        }
        break;

    case 7203:
        rowIdx = (unsigned int)value->x;
        if ((rowIdx & 0x80000000) != 0 || (int)rowIdx > 10)
            break;
        colIdx = (unsigned int)value->y;
        if ((colIdx & 0x80000000) != 0 || (int)colIdx > 10)
            break;
        colorIdx = (unsigned int)value->z;
        slot = &table->vertexColorIndexes[10 * rowIdx + colIdx];
        *slot = colorIdx;
        if ((colorIdx & 0x80u) != 0)
            *slot = 0;
        if (*slot >= 10)
            *slot = 9;
        break;

    default:
        break;
    }
}

struct sGfxTableData* GFX_Table_Create(void)
{
    tGfxTableData* table;
    int i;

    table = (tGfxTableData*)MEM_p_Alloc(sizeof(tGfxTableData));
    memset(table, 0, sizeof(tGfxTableData));
    table->num1 = 10;
    table->num2 = 10;

    // only the f cursor advances in the original, so colors and f2 are
    // written at index zero ten times over and their other nine entries
    // keep the zero the memset left
    table->colors[0] = 0xffffffff;
    table->f2[0] = 0.1f;
    for (i = 0; i < 10; i++)
        table->f[i] = 0.1f;
    return table;
}
