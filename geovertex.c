#include "geovertex.h"

#include "graphicsobjects/geo.h"

/* The packing is a byte swizzle: red and blue change places on the
   way out, which is what the engine's colour words want and the one
   piece of this that is easy to get subtly wrong. */
unsigned int GVX_ui_DeviceColor(unsigned int engineColor)
{
    return engineColor & 0xFF00FF00u
           | ((engineColor >> 16) & 0xFFu)
           | ((engineColor & 0xFFu) << 16);
}

unsigned int GVX_ui_Color(const int* colors, const int* second, int mask,
                          int alpha, int idx, int secondIdx)
{
    unsigned int packed;

    packed = mask ^ (alpha | colors[idx]);
    if (second)
        packed = second[secondIdx] | packed & 0xFFFFFF;
    return packed & 0xFF00FF00 | ((uint8_t)packed << 16) | ((packed >> 16) & 0xff);
}

unsigned int GVX_ui_SolidColor(int mask, int alpha, unsigned int solidVertexColor)
{
    return (mask ^ (alpha | solidVertexColor)) & 0xFF00FF00
           | ((uint8_t)(mask ^ (alpha | (solidVertexColor & 0xff))) << 16)
           | (uint8_t)((mask ^ (alpha | solidVertexColor)) >> 16);
}

void GVX_BuildNormals(Vec3* out,
                      const struct sGeoTriangleChunks* chunks,
                      const Vec3* normals)
{
    const tGeoTriangleChunkElem* tri;
    const tGeoTriangleChunkElem* triEnd;
    int                          k;

    if (!out || !chunks || !normals)
        return;

    tri = chunks->triangleChunkElems;
    triEnd = &tri[chunks->triangleNum];
    for (; tri < triEnd; tri++)
    {
        for (k = 0; k < 3; k++)
        {
            int idx;

            if (k == 0)
                idx = tri->v1;
            else if (k == 1)
                idx = tri->v2;
            else
                idx = tri->v3;

            *out++ = normals[idx];
        }
    }
}

void GVX_BuildTriangles(tGdi_TexturedVertex* out,
                        const struct sGeoTriangleChunks* chunks,
                        const tGvx_Source* src)
{
    const tGeoTriangleChunkElem* tri;
    const tGeoTriangleChunkElem* triEnd;
    int                          k;

    if (!out || !chunks || !src)
        return;

    tri = chunks->triangleChunkElems;
    triEnd = &tri[chunks->triangleNum];
    for (; tri < triEnd; tri++)
    {
        for (k = 0; k < 3; k++)
        {
            int idx;
            int secondIdx;

            if (k == 0)
                idx = tri->v1;
            else if (k == 1)
                idx = tri->v2;
            else
                idx = tri->v3;

            out->pos = src->vertices[idx];

            if (src->colors)
            {
                secondIdx = idx;

                /* Preserved: this one of nine second-colour lookups
                   scales its index by four. */
                if (src->uvs && src->useVertexUv && k == 1)
                    secondIdx = idx * 4;
                out->color = GVX_ui_Color(src->colors, src->second, src->mask,
                                          src->alpha, idx, secondIdx);
            }
            else
            {
                out->color = src->solidColor;
            }

            if (src->uvs)
            {
                const tGeoUVElem* uv;

                if (src->useVertexUv)
                    uv = &src->uvs[idx];
                else if (k == 0)
                    uv = &src->uvs[tri->uv1];
                else if (k == 1)
                    uv = &src->uvs[tri->uv2];
                else
                    uv = &src->uvs[tri->uv3];
                out->u = uv->u;
                out->v = uv->v;
            }
            out++;
        }
    }
}
