#include "geosprite.h"

#include "sprites.h"
#include "math.h"
#include "time.h"

/* The index the explode generator puts in the first element to say
   that this list is its own: positions and sizes together rather than
   positions alone, and an offset in the second element. See
   GFX_SetNumberOfSprites in gfxexplode.c. */
#define GSP_C0DE 0xC0DE

int GSP_i_QuadCount(const struct sGeoSprites* sprites)
{
    return sprites ? (int)sprites->num : 0;
}

void GSP_Walk(const struct sGeoSprites* sprites, const tGsp_Source* src,
              GSP_pfn_Emit emit, void* user)
{
    tGdi_SpriteQuad        quad;
    tGdi_SpriteQuad* const out = &quad;
    const tGeoSpriteElem*  elem;
    const tGeoSpriteElem*  elemEnd;
    const int*             colors;
    const int*             second;
    int                    color;
    float rightX, rightY, rightZ;
    float upX, upY, upZ;
    float c1x, c1y, c1z;
    float c2x, c2y, c2z;
    float c3x, c3y, c3z;
    float c4x, c4y, c4z;

    if (!sprites || !src || !src->matrix || !emit)
        return;

    colors = src->colors;
    second = src->second;
    color = src->mask ^ (src->alpha | (int)src->solidColor);

    /* The two axes a corner is built along, out of the matrix's first
       two columns -- so a sprite faces whatever the matrix faces. The
       right-hand axis is negated and carries the batch's aspect
       ratio; the up axis carries only the size. Both are as the other
       renderer computes them. */
    rightX = -(src->matrix->v1.x * sprites->globalSize * sprites->globalRatio);
    rightY = -(src->matrix->v2.x * sprites->globalSize * sprites->globalRatio);
    rightZ = -(src->matrix->v3.x * sprites->globalSize * sprites->globalRatio);
    upX = src->matrix->v1.y * sprites->globalSize;
    upY = src->matrix->v2.y * sprites->globalSize;
    upZ = src->matrix->v3.y * sprites->globalSize;

    c1x = -rightX - upX;
    c1y = -rightY - upY;
    c1z = -rightZ - upZ;
    c2x = rightX - upX;
    c2y = rightY - upY;
    c2z = rightZ - upZ;
    c3x = upX + rightX;
    c3y = upY + rightY;
    c3z = upZ + rightZ;
    c4x = upX - rightX;
    c4y = upY - rightY;
    c4z = upZ - rightZ;

    elem = sprites->spriteElems;
    elemEnd = &elem[sprites->num];

    if (elem->idx == GSP_C0DE)
    {
        /* The explode generator's layout: the second element holds an
           offset into the positions and the colours, and each position
           carries its own size beside it. */
        int                     offset = elem[1].idx;
        const tSpriteC0DEData*  c0de;
        const tSpriteC0DEData*  c0deEnd;

        c0de = &((const tSpriteC0DEData*)src->positions)[offset];
        c0deEnd = &c0de[sprites->num];
        if (colors)
            colors = &colors[offset];
        if (second)
            second += offset;

        for (; c0de < c0deEnd; c0de++)
        {
            double size;

            if (colors)
            {
                color = src->mask ^ (src->alpha | *colors);
                /* Preserved: elem still points at the sentinel, so
                   this reads the second colour set at index 0xC0DE
                   rather than at this sprite's own. */
                if (second)
                    color = second[elem->idx] | color & 0xFFFFFF;
                colors++;
            }
            else if (second)
            {
                color = second[elem->idx] | color & 0xFFFFFF;
            }
            out->color = color;

            /* Preserved: a double, so each corner is worked out at
               double precision and rounded once on the way into the
               float. */
            size = c0de->f1;
            out->v1.x = (float)(c1x * size + c0de->v1.x);
            out->v1.y = (float)(c1y * size + c0de->v1.y);
            out->v1.z = (float)(c1z * size + c0de->v1.z);
            out->v2.x = (float)(c2x * size + c0de->v1.x);
            out->v2.y = (float)(c2y * size + c0de->v1.y);
            out->v2.z = (float)(c2z * size + c0de->v1.z);
            out->v3.x = (float)(c3x * size + c0de->v1.x);
            out->v3.y = (float)(c3y * size + c0de->v1.y);
            out->v3.z = (float)(c3z * size + c0de->v1.z);
            out->v4.x = (float)(c4x * size + c0de->v1.x);
            out->v4.y = (float)(c4y * size + c0de->v1.y);
            out->v4.z = (float)(c4z * size + c0de->v1.z);
            emit(user, out);
        }
        return;
    }

    {
        /* Counts the sprites for the wobble's phase, and starts at one
           rather than nought because it is stepped before it is
           used. */
        float wave = 0.0f;

        for (; elem < elemEnd; ++elem)
        {
            double      size;
            const Vec3* base;

            wave = wave + 1.0f;
            if (colors)
            {
                int idx = elem->idx;

                color = src->mask ^ (src->alpha | colors[idx]);
                if (second)
                    color = second[idx] | color & 0xFFFFFF;
            }
            else if (second)
            {
                color = second[elem->idx] | color & 0xFFFFFF;
            }
            out->color = color;

            size = elem->size;
            base = &(((const Vec3*)src->positions)[elem->idx]);
            out->v1.x = (float)(c1x * size + base->x);
            out->v1.y = (float)(c1y * size + base->y);
            out->v1.z = (float)(c1z * size + base->z);
            out->v2.x = (float)(c2x * size + base->x);
            out->v2.y = (float)(c2y * size + base->y);
            out->v2.z = (float)(c2z * size + base->z);
            out->v3.x = (float)(c3x * size + base->x);
            out->v3.y = (float)(c3y * size + base->y);
            out->v3.z = (float)(c3z * size + base->z);
            out->v4.x = (float)(c4x * size + base->x);
            out->v4.y = (float)(c4y * size + base->y);
            out->v4.z = (float)(c4z * size + base->z);

            if (src->waveSprite)
            {
                /* Two of the four corners only, which is what makes
                   this a shear rather than a shift: the quad leans
                   instead of moving. */
                double sine = MATH_GetSin(wave);
                double wobble = sine
                                * MATH_GetSin((float)(sine * TIM_gul_TicksCount
                                                      * 2.5))
                                * 0.2;

                out->v1.x = (float)(out->v1.x + wobble);
                out->v2.x = (float)(wobble + out->v2.x);
            }
            emit(user, out);
        }
    }
}

/* ------------------------------------------------ the array form */

static void GSP_l_Append(void* user, const tGdi_SpriteQuad* quad)
{
    tGdi_SpriteQuad** next = (tGdi_SpriteQuad**)user;

    **next = *quad;
    (*next)++;
}

void GSP_BuildQuads(tGdi_SpriteQuad* out, const struct sGeoSprites* sprites,
                    const tGsp_Source* src)
{
    tGdi_SpriteQuad* next = out;

    if (!out)
        return;
    GSP_Walk(sprites, src, GSP_l_Append, &next);
}
