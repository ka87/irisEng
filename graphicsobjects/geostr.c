#include "../memory.h"
#include "../win/wingdi.h"
#include "../loading.h"
#include "../strings.h"
#include "../time.h"
#include "geostr.h"

unsigned int ul_SubMatOrder[] = { 8, 9, 0x0D, 0x10, 0x0A, 0x0B, 0x0C, 0x0E, 0x0F, 0, 1, 2, 3, 4, 5, 6, 7, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };

tGEOSTRStruct* STR_pst_Create()
{
    tGEOSTRStruct* str;

    str = (tGEOSTRStruct*)MEM_p_Alloc(sizeof(tGEOSTRStruct));
    memset(str, 0, sizeof(tGEOSTRStruct));
    GRO_Struct_Init((tGroGeneric*)str, 10);

    str->stringNum = 32;
    str->stringData = (tGEOSTRStringData*)MEM_p_Alloc(32 * sizeof(tGEOSTRStringData));
    memset(str->stringData, 0, 32 * sizeof(tGEOSTRStringData));

    str->effectsNum = 0;
    str->maxEffectsNum = 32;
    memset(str->effect, 0, 40 * str->stringNum);

    str->width = 640;
    str->height = 480;
    str->fontMaterialID = -1;
    return str;
}

struct sGroGeneric* STR_p_CreateFromBuffer(struct sGroGeneric* gro, char** readptr,
                                           struct sWorld* world)
{
    tGEOSTRStruct* str;
    int letterNum;

    str = STR_pst_Create();
    letterNum = LOA_ReadLong(readptr, 0, 1);
    str->unkNum = letterNum;

    if (letterNum)
        str->letters = (tGEOSTRLetter*)MEM_p_Alloc(sizeof(tGEOSTRLetter) * letterNum);
    else
        str->letters = 0;

    if (!str->letters)
    {
        str->unkNum = 0;
        return (tGroGeneric*)str;
    }

    if (str->unkNum > 0)
        memset(str->letters, 0, sizeof(tGEOSTRLetter) * str->unkNum);
    return (tGroGeneric*)str;
}

void STR_Free(struct sGroGeneric* gro)
{
    tGEOSTRStruct* str;

    if (!gro || gro->intf->id != 10 || gro->usageCount > 0)
        return;

    str = (tGEOSTRStruct*)gro;
    if (str->unkNum)
        MEM_Free(str->letters);
    if (str->stringNum)
        MEM_Free(str->stringData);
    LOA_DeleteAddress((int)str);
    MEM_Free(str);
}

int STR_l_HasSomethingToRender(struct sGameObjectVisualObject* gaovo, struct sGroGeneric** gro)
{
    *gro = NULL;
    return 1;
}

void STR_Reinit(struct sGroGeneric* gro)
{
    tGEOSTRStruct* str;

    str = (tGEOSTRStruct*)gro;
    memset(str->stringData, 0, sizeof(tGEOSTRStringData) * str->stringNum);
    str->effectsNum = 0;
}

static int STR_l_Effect_LetterSpan(tStrEffectRun* run)
{
    return (int)(run->lettersEnd - run->letters);
}

// all four corners of every letter in the run
static void STR_l_Effect_SetLetterColors(tStrEffectRun* run, unsigned int color)
{
    unsigned int* colors;
    int i;
    int span;

    span = STR_l_Effect_LetterSpan(run);
    colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[run->firstVertex + 1];
    for (i = 0; i < span; i++)
    {
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;
        colors += 4;
    }
}

static void STR_l_Effect_SetFrameColors(tStrEffectRun* run, unsigned int color)
{
    int i;

    for (i = 0; i < run->frameVertices; i++)
        GFX_gpst_Geo->vertexColors[run->frameFirst + 1 + i] = color;
}

// the box around every vertex of the run; min and max start from the
// first vertex, which is read even when the range is empty
static void STR_l_Effect_Bounds(Vec3* from, Vec3* to, Vec3* min, Vec3* max)
{
    Vec3* v;

    min->x = from->x;
    max->x = from->x;
    min->y = from->y;
    max->y = from->y;
    for (v = from; v < to; v++)
    {
        if (v->x < (double)min->x)
            min->x = v->x;
        if (v->y < (double)min->y)
            min->y = v->y;
        if (v->x > (double)max->x)
            max->x = v->x;
        if (max->y < v->y)
            max->y = v->y;
    }
}

static void STR_l_Effect_ScaleAbout(Vec3* from, Vec3* to, float cx, float cy, float scale)
{
    Vec3* v;

    for (v = from; v < to; v++)
    {
        v->x = (float)((v->x - cx) * scale + cx);
        v->y = (v->y - cy) * scale + cy;
    }
}

static void STR_l_Effect_RotateAbout(Vec3* from, Vec3* to, float cx, float cy,
                                     float sin, float cos)
{
    Vec3* v;
    double dx;
    double dy;
    float rotated;

    for (v = from; v < to; v++)
    {
        dx = v->x - cx;
        dy = v->y - cy;
        rotated = (float)(cos * dy + dx * sin);
        v->x = (float)(dx * cos - dy * sin + cx);
        v->y = rotated + cy;
    }
}

// An effect returns 1 when it is finished and the dispatcher should
// drop it, 0 to keep it for the next frame. Setting 0x400 on the
// string hides the string for good.
static int STR_l_Effect_Done(tGEOSTRStringData* sd, int hideString)
{
    if (hideString)
        sd->flags |= 0x400u;
    return 1;
}

// 1: fades the letters and the frame toward black over f1, after an f3
// delay; 0x10000 runs it the other way round
static int STR_l_Effect_FadeToBlack(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    unsigned int* colors;
    unsigned int color;
    float blend;
    double elapsed;
    int span;
    int i;

    if (effect->timeElapsed >= (double)effect->f3)
        blend = (effect->timeElapsed - effect->f3) / effect->f1;
    else
        blend = 0.0f;
    if ((effect->flags & 0x10000) != 0)
        blend = (float)(1.0 - blend);

    span = STR_l_Effect_LetterSpan(run);
    colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[run->firstVertex + 1];
    for (i = 0; i < span; i++)
    {
        color = LIGHT_ul_Interpol2Colors(colors[0], 0, blend);
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;
        colors += 4;
    }

    colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[run->frameFirst + 1];
    for (i = 0; i < run->frameVertices; i++)
    {
        colors[i] = LIGHT_ul_Interpol2Colors(colors[i], 0, blend);
    }

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed < effect->f1 + effect->f3)
        return 0;
    return STR_l_Effect_Done(run->sd, (effect->flags & 0x10000) == 0);
}

// 2: holds one of the two colours, or blends between them over f1, and
// then swaps which one it holds and starts again
static int STR_l_Effect_Blink(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    unsigned int color;
    float blend;
    double elapsed;
    int flags;

    flags = effect->flags;
    if ((flags & 0x10000) != 0)
    {
        blend = effect->timeElapsed / effect->f1;
        if ((flags & 0x1000000) != 0)
            blend = (float)(1.0 - blend);
        color = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(effect->color1),
                                         *(uint32_t*)&(effect->color2), blend);
    }
    else if ((flags & 0x1000000) != 0)
    {
        color = *(uint32_t*)&(effect->color1);
    }
    else
    {
        color = *(uint32_t*)&(effect->color2);
    }

    STR_l_Effect_SetLetterColors(run, color);

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed < effect->f1)
    {
        effect->flags &= ~0x2000000u;
        return 0;
    }

    effect->timeElapsed = 0.0f;
    if ((effect->flags & 0x1000000) == 0)
        effect->flags |= 0x1000000;
    else
        effect->flags &= ~0x1000000u;
    effect->flags |= 0x2000000;
    return 0;
}

// 3: colours every vertex by where it sits in the run's own box,
// blending between four colours packed into the effect's float fields
static int STR_l_Effect_ColorByPosition(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    unsigned int* colors;
    unsigned int acrossColor;
    unsigned int downColor;
    float originX;
    float originY;
    float width;
    float height;
    float invWidth;
    float invHeight;
    float across;
    float down;

    verts = GFX_gpst_Geo->vertices;
    to = &verts[run->endVertex];
    from = &verts[run->firstVertex];
    originX = from->x;
    originY = from->y;
    colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[run->firstVertex + 1];

    width = to[-2].x - from->x;
    height = to[-2].y - originY;
    if (width == 0.0 || height == 0.0)
        return 0;

    invWidth = (float)(1.0 / width);
    invHeight = (float)(1.0 / height);

    for (; from < to; ++from, ++colors)
    {
        across = (from->x - originX) * invWidth;
        acrossColor = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(effect->f1),
                                               *(uint32_t*)&(effect->timeElapsed), across);
        downColor = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(effect->color2),
                                             *(uint32_t*)&(effect->color1), across);
        down = (from->y - originY) * invHeight;
        *colors = LIGHT_ul_Interpol2Colors(acrossColor, downColor, down);
    }
    return 0;
}

// 4: fades the letters and the frame from color1 to color2 over f1,
// after an f3 delay
static int STR_l_Effect_FadeBetweenColors(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    unsigned int color;
    float blend;
    double elapsed;

    if (effect->timeElapsed >= (double)effect->f3)
        blend = (effect->timeElapsed - effect->f3) / effect->f1;
    else
        blend = 0.0f;

    color = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(effect->color1),
                                     *(uint32_t*)&(effect->color2), blend);
    STR_l_Effect_SetLetterColors(run, color);
    STR_l_Effect_SetFrameColors(run, color);

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed < effect->f1 + effect->f3)
        return 0;
    return STR_l_Effect_Done(run->sd, (*(uint32_t*)&(effect->color2) & 0xFFFFFF) == 0);
}

// 5: a sine wave down the run, each letter lifted by the sine of its
// own centre; color2 is the clock and f3 the elapsed time
static int STR_l_Effect_Wave(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    double lift;
    double remaining;
    double elapsed;
    float amplitude;
    float phase;
    float at;

    verts = GFX_gpst_Geo->vertices;
    remaining = effect->color2 - effect->f3;
    to = &verts[run->endVertex];
    from = &verts[run->firstVertex];
    amplitude = (float)((double)(uint16_t)run->str->height * effect->f1
                        * remaining * remaining);
    phase = (float)(remaining * effect->timeElapsed);

    for (; from < to; from += 4)
    {
        at = (float)((from[2].x + from->x) * 0.5 * effect->color1 + phase);
        lift = fabs(MATH_GetSin(at)) * amplitude;
        from[0].y = (float)(from[0].y - lift);
        from[1].y = (float)(from[1].y - lift);
        from[2].y = (float)(from[2].y - lift);
        from[3].y = (float)(from[3].y - lift);
    }

    elapsed = TIM_gf_dt + effect->f3;
    effect->f3 = (float)elapsed;
    if (elapsed < effect->color2)
        return 0;
    return 1;
}

// 6: jitters every letter by a random offset that dies away as color2
// approaches color1, with an optional white flash over the first half
static int STR_l_Effect_Shake(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    tGEOSTRStruct* str = run->str;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    unsigned int color;
    double dy;
    double flashAt;
    double elapsed;
    float decay;
    float dx;
    float flash;

    decay = (float)((1.0 - effect->color2 / effect->color1)
                    * (1.0 - effect->color2 / effect->color1));
    verts = GFX_gpst_Geo->vertices;
    to = &verts[run->endVertex];
    from = &verts[run->firstVertex];

    for (; from < to; from += 4)
    {
        dx = (float)(RND_RandFloatRange(-effect->f1, effect->f1)
                     * (double)(uint16_t)str->width * decay);
        dy = RND_RandFloatRange(-effect->timeElapsed, effect->timeElapsed)
             * (double)(uint16_t)str->height * decay;

        from[0].x = dx + from[0].x;
        from[0].y = (float)(from[0].y - dy);
        from[1].x = dx + from[1].x;
        from[1].y = (float)(from[1].y - dy);
        from[2].x = dx + from[2].x;
        from[2].y = (float)(from[2].y - dy);
        from[3].x = dx + from[3].x;
        from[3].y = (float)(from[3].y - dy);
    }

    if ((effect->flags & 0x30000) != 0)
    {
        flashAt = (effect->color2 / effect->color1) + (effect->color2 / effect->color1);
        flash = (float)flashAt;
        if ((effect->flags & 0x20000) != 0)
        {
            flashAt = 2.0 - flashAt;
            flash = (float)flashAt;
        }
        if (flashAt > 1.0)
            flash = 1.0f;
        color = LIGHT_ul_Interpol2Colors(0, 0xFFFFFFFF, flash);
        STR_l_Effect_SetLetterColors(run, color);
    }

    elapsed = TIM_gf_dt + effect->color2;
    effect->color2 = (float)elapsed;
    if (elapsed < effect->color1)
        return 0;
    return 1;
}

// 7: opens each letter vertically from its own middle, the letters
// starting one after another; color1 is the clock and color2 the delay
static int STR_l_Effect_OpenVertically(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    tGEOSTRLetter* letter;
    Vec3* quad;
    double head;
    double open;
    double middle;
    double half;
    float stagger;
    double elapsed;
    int index;

    if (effect->color1 < (double)effect->color2)
    {
        effect->color1 = TIM_gf_dt + effect->color1;
        return 0;
    }

    stagger = (float)((effect->f1 - 0.050000001 - effect->timeElapsed) / (double)run->count);
    index = 0;
    quad = &GFX_gpst_Geo->vertices[run->firstVertex];
    for (letter = run->letters; letter < run->lettersEnd; ++letter)
    {
        head = effect->color1 - effect->color2 - (double)index * stagger;
        if (head <= 0.0)
            break;

        open = head / effect->timeElapsed;
        if (open >= 1.0)
            open = 1.0;

        middle = (quad[1].y + quad[0].y) * 0.5;
        half = (1.0 - open) * ((quad[1].y - quad[0].y) * 0.5);
        quad[0].y = (float)(middle - half);
        quad[3].y = (float)(middle - half);
        quad[1].y = (float)(middle + half);
        quad[2].y = (float)(middle + half);

        quad += 4;
        index++;
    }

    elapsed = TIM_gf_dt + effect->color1;
    effect->color1 = (float)elapsed;
    if (elapsed >= effect->color2 + effect->f1)
        return STR_l_Effect_Done(run->sd, 1);
    return 0;
}

// 8: the same shape closing rather than opening, and without the early
// exit, so every letter is visited
static int STR_l_Effect_CloseVertically(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* quad;
    double closed;
    double middle;
    double half;
    float amount;
    float stagger;
    double elapsed;
    int span;
    int i;

    stagger = (float)((effect->f1 - 0.050000001 - effect->timeElapsed) / (double)run->count);
    span = STR_l_Effect_LetterSpan(run);
    quad = &GFX_gpst_Geo->vertices[run->firstVertex];

    for (i = 0; i < span; i++)
    {
        closed = (effect->color1 - effect->color2 - (double)i * stagger) / effect->timeElapsed;
        amount = (float)closed;
        if (closed < 1.0)
        {
            if (amount < 0.0)
                amount = 0.0f;
            middle = (quad[1].y + quad[0].y) * 0.5;
            half = (quad[1].y - quad[0].y) * 0.5 * amount;
            quad[0].y = (float)(middle - half);
            quad[3].y = (float)(middle - half);
            quad[1].y = (float)(middle + half);
            quad[2].y = (float)(middle + half);
        }
        quad += 4;
    }

    elapsed = TIM_gf_dt + effect->color1;
    effect->color1 = (float)elapsed;
    if (elapsed < effect->color2 + effect->f1)
        return 0;
    return 1;
}

// 9 and 10: scales the whole run about the centre of its own box; kind
// 9 runs the scale the other way round. color1 is the clock, color2 the
// delay.
static int STR_l_Effect_ScaleRun(tStrEffectRun* run, int kind)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    Vec3 min;
    Vec3 max;
    double progress;
    double elapsed;
    float scale;

    verts = GFX_gpst_Geo->vertices;
    from = &verts[run->firstVertex];
    to = &verts[run->endVertex];
    STR_l_Effect_Bounds(from, to, &min, &max);

    progress = (effect->color1 - effect->color2) / effect->f1;
    if (progress < 0.0)
        progress = 0.0;
    scale = (float)(progress * progress);
    if (kind == 9)
        scale = (float)(1.0 - scale);

    STR_l_Effect_ScaleAbout(from, to, (float)((max.x + min.x) * 0.5),
                            (float)((max.y + min.y) * 0.5), scale);

    elapsed = TIM_gf_dt + effect->color1;
    effect->color1 = (float)elapsed;
    if (elapsed < effect->color2 + effect->f1)
        return 0;
    return STR_l_Effect_Done(run->sd, kind == 9);
}

// 11 and 12: waits out color1, then scales the run and the frame about
// the run's centre while fading both to black. Kind 12 runs the scale
// the other way round and parks everything off screen while it waits.
static int STR_l_Effect_ScaleAndFade(tStrEffectRun* run, int kind)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    Vec3* frameFrom;
    Vec3* frameTo;
    Vec3* v;
    Vec3 min;
    Vec3 max;
    unsigned int* colors;
    unsigned int color;
    double progress;
    double elapsed;
    float fade;
    float scale;
    float centreX;
    float centreY;

    verts = GFX_gpst_Geo->vertices;
    from = &verts[run->firstVertex];
    to = &verts[run->endVertex];
    STR_l_Effect_Bounds(from, to, &min, &max);

    if (effect->timeElapsed < (double)effect->color1)
    {
        if (kind == 12)
        {
            for (v = from; v < to; ++v)
            {
                v->x = -1.0;
                v->y = -1.0;
            }
            verts = GFX_gpst_Geo->vertices;
            frameFrom = &verts[run->frameFirst];
            frameTo = &verts[run->frameVertices + run->frameFirst];
            for (v = frameFrom; v < frameTo; ++v)
            {
                v->x = -1.0;
                v->y = -1.0;
            }
        }
        effect->timeElapsed = TIM_gf_dt + effect->timeElapsed;
        return 0;
    }

    progress = (effect->timeElapsed - effect->color1) / effect->f1;
    fade = (float)(progress * progress);
    if (kind == 12)
        fade = (float)(1.0 - fade);
    scale = (float)((effect->color2 - 1.0) * fade + 1.0);
    centreX = (float)((max.x + min.x) * 0.5);
    centreY = (float)((max.y + min.y) * 0.5);

    color = LIGHT_ul_Interpol2Colors(GFX_gpst_Geo->vertexColors[run->firstVertex + 1],
                                     0, fade);
    colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[run->firstVertex + 1];
    for (v = from; v < to; ++v, ++colors)
    {
        v->x = (float)((v->x - centreX) * scale + centreX);
        v->y = (v->y - centreY) * scale + centreY;
        *colors = color;
    }

    if (run->frameVertices)
    {
        verts = GFX_gpst_Geo->vertices;
        frameFrom = &verts[run->frameFirst];
        frameTo = &verts[run->frameFirst + run->frameVertices];
        color = LIGHT_ul_Interpol2Colors(
            GFX_gpst_Geo->vertexColors[run->frameFirst + 1], 0, fade);
        colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[run->frameFirst + 1];
        for (v = frameFrom; v < frameTo; ++v, ++colors)
        {
            v->x = (float)((v->x - centreX) * scale + centreX);
            v->y = (v->y - centreY) * scale + centreY;
            *colors = color;
        }
    }

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed < effect->f1 + effect->color1)
        return 0;
    return STR_l_Effect_Done(run->sd, kind == 11);
}

// 13 and 14: folds the run about the horizontal line color1, pulling y
// toward it by a sine and lifting z by a cosine; kind 14 runs it the
// other way round
static int STR_l_Effect_FoldY(tStrEffectRun* run, int kind)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    double progress;
    double elapsed;
    double folded;
    float angle;
    float cos;
    float sin;

    verts = GFX_gpst_Geo->vertices;
    progress = effect->timeElapsed / effect->f1;
    to = &verts[run->endVertex];
    from = &verts[run->firstVertex];
    if (kind == 14)
        progress = 1.0 - progress;

    angle = (float)(progress * 1.5707964);
    cos = MATH_GetCos(angle);
    sin = MATH_GetSin(angle);

    for (; from < to; from++)
    {
        folded = (from->y - effect->color1) * sin + effect->color1;
        from->y = (float)folded;
        from->z = (float)(fabs(folded - effect->color1) * cos + from->z);
    }

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed < effect->f1)
        return 0;
    return STR_l_Effect_Done(run->sd, kind == 14);
}

// 15: spins the run and the frame about the run's centre, f1 carrying
// the angle and timeElapsed the rate
static int STR_l_Effect_Rotate(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* from;
    Vec3* to;
    Vec3* frameFrom;
    Vec3* frameTo;
    Vec3 min;
    Vec3 max;
    float centreX;
    float centreY;
    float sin;
    float cos;

    verts = GFX_gpst_Geo->vertices;
    from = &verts[run->firstVertex];
    to = &verts[run->endVertex];
    STR_l_Effect_Bounds(from, to, &min, &max);

    centreX = (float)((max.x + min.x) * 0.5);
    centreY = (float)((max.y + min.y) * 0.5);
    sin = MATH_GetSin(effect->f1);
    cos = MATH_GetCos(effect->f1);
    effect->f1 = TIM_gf_dt * effect->timeElapsed + effect->f1;

    STR_l_Effect_RotateAbout(&GFX_gpst_Geo->vertices[run->firstVertex], to,
                             centreX, centreY, sin, cos);

    verts = GFX_gpst_Geo->vertices;
    frameFrom = &verts[run->frameFirst];
    frameTo = &verts[run->frameVertices + run->frameFirst];
    if (frameFrom >= frameTo)
        return 0;

    STR_l_Effect_RotateAbout(frameFrom, frameTo, centreX, centreY, sin, cos);
    return 0;
}

// 17: clips the run to the horizontal band between f1 and timeElapsed,
// moving the band's edges onto the letters and the UVs with them, and
// parking a letter that falls outside it off screen
static int STR_l_Effect_ClipBand(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    tGEOGRO* geo;
    Vec3* quad;
    Vec3* to;
    tGeoUVElem* uvs;
    double top;
    double bottom;
    double height;
    double topV;
    float bottomV;
    float vSpan;
    float bandTop;
    float bandBottom;
    double screenHeight;

    geo = GFX_gpst_Geo;
    to = &geo->vertices[run->endVertex];
    quad = &geo->vertices[run->firstVertex];
    screenHeight = (double)(uint16_t)run->str->height;
    bandTop = (float)((1.0 - effect->timeElapsed) * screenHeight);
    bandBottom = (float)((1.0 - effect->f1) * screenHeight);

    if (quad >= to)
        return 0;

    while (1)
    {
        if (quad[1].y > (double)bandBottom || quad->y < (double)bandTop)
        {
            quad[0].x = -100.0f;
            quad[1].x = -100.0f;
            quad[2].x = -100.0f;
            quad[3].x = -100.0f;
            quad[0].y = -100.0f;
            quad[1].y = -100.0f;
            quad[2].y = -100.0f;
            quad[3].y = -100.0f;
        }
        else if (quad->y > (double)bandBottom || quad[1].y < (double)bandTop)
        {
            if (quad->y <= (double)bandBottom)
                top = quad->y;
            else
                top = bandBottom;
            if (quad[1].y >= (double)bandTop)
                bottom = quad[1].y;
            else
                bottom = bandTop;

            height = quad[1].y - quad->y;
            uvs = &geo->UVs[quad - geo->vertices];
            vSpan = uvs[1].v - uvs->v;
            topV = (top - quad->y) / height * vSpan + uvs->v;
            bottomV = (float)((bottom - quad->y) / height * vSpan + uvs->v);

            quad[0].y = (float)top;
            quad[3].y = (float)top;
            uvs[0].v = (float)topV;
            uvs[3].v = (float)topV;
            quad[1].y = (float)bottom;
            quad[2].y = (float)bottom;
            uvs[1].v = bottomV;
            uvs[2].v = bottomV;
        }

        quad += 4;
        if (quad >= to)
            break;
        geo = GFX_gpst_Geo;
    }
    return 0;
}

// 18: shifts one corner of each side of every letter in x by f1, which
// shears the letters
static int STR_l_Effect_ShearLetters(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* quad;
    Vec3* to;

    verts = GFX_gpst_Geo->vertices;
    to = &verts[run->endVertex];
    quad = &verts[run->firstVertex];
    if (quad >= to)
        return 0;

    do
    {
        quad[0].x = (float)(quad[0].x + effect->f1);
        quad[3].x = quad[3].x + effect->f1;
        quad += 4;
    } while (quad < to);
    return 0;
}

// 19: steps the first letter of the run through a sequence of digits
// packed four bits at a time into unki1, holding each for color1
static int STR_l_Effect_Scramble(tStrEffectRun* run)
{
    tGEOSTREffectStructType20* effect = (tGEOSTREffectStructType20*)run->effect;
    double remaining;
    unsigned int step;
    int digit;

    remaining = effect->color2 - TIM_gf_dt;
    effect->color2 = (float)remaining;
    if (remaining >= 0.0)
        return 0;

    step = effect->unkb1;
    effect->color2 = run->effect->color1;
    digit = (effect->unki1 >> (4 * step)) & 0xF;
    if (digit)
    {
        effect->unkb1 = step + 1;
    }
    else
    {
        effect->unkb1 = 1;
        digit = effect->unki1 & 0xF;
    }

    if (!effect->flagsAndChar)
        effect->flagsAndChar = run->letters->flagsAndChar;
    if (digit == 1)
        effect->color2 = (float)(effect->color2 * 0.5);

    run->letters->flagsAndChar &= 0xffffff00;
    run->letters->flagsAndChar |= (char)effect->flagsAndChar + digit - 1;
    return 0;
}

// 20: cuts every letter into five vertical slices and pushes them
// apart, the two outer edges travelling twice as far and the two
// middle ones on the square of the clock, with each pair of slices
// tinted by its own factor
static int STR_l_Effect_Slice(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    tGEOSTRStruct* str = run->str;
    Vec3* quad;
    unsigned int* colors;
    tGeoUVElem* uvs;
    double middle;
    double uStep;
    double elapsed;
    float xStep;
    float reach;
    float t;
    float tt;
    float outerT;
    float edge[6];
    double slide;
    float slideF;
    unsigned int outerColor;
    unsigned int midColor;
    unsigned int innerColor;
    int quadIdx;
    int i;

    t = effect->timeElapsed / effect->f1;
    reach = (float)((double)(uint16_t)str->width * effect->color1);
    outerT = t + t;

    for (i = 0; i < run->count; i++)
    {
        quadIdx = 4 * str->specularColorsPtr[run->startIdx + i + run->sd->index];
        quad = &GFX_gpst_Geo->vertices[quadIdx];

        // the six slice edges, at a fifth of the letter's width apart
        xStep = (float)((quad[2].x - quad->x) * 0.2);
        middle = (quad->x + quad[2].x) * 0.5;

        // the first slice's own edge, pushed twice as far
        edge[0] = (float)((quad->x - middle) * reach * t * 2.0 + quad->x);

        // the second edge is the only one whose base is narrowed to a
        // float before it is used, which is where the chain restarts
        slide = quad->x + xStep;
        slideF = (float)slide;
        edge[1] = (float)((slide - middle) * reach * t + slideF);

        slide = slideF + xStep;
        edge[2] = (float)((slide - middle) * reach * t * t + slide);
        slide = slide + xStep;
        edge[3] = (float)((slide - middle) * reach * t * t + slide);
        slide = slide + xStep;
        edge[4] = (float)((slide - middle) * reach * t + slide);
        slide = slide + xStep;
        edge[5] = (float)((slide - middle) * reach * t * 2.0 + slide);

        quad[0].x = edge[0];
        quad[1].x = edge[0];
        quad[2].x = edge[1];
        quad[3].x = edge[1];
        quad[4].x = edge[1];
        quad[5].x = edge[1];
        quad[6].x = edge[2];
        quad[7].x = edge[2];
        quad[8].x = edge[2];
        quad[9].x = edge[2];
        quad[10].x = edge[3];
        quad[11].x = edge[3];
        quad[12].x = edge[3];
        quad[13].x = edge[3];
        quad[14].x = edge[4];
        quad[15].x = edge[4];
        quad[16].x = edge[4];
        quad[17].x = edge[4];
        quad[18].x = edge[5];
        quad[19].x = edge[5];

        colors = (unsigned int*)&GFX_gpst_Geo->vertexColors[quadIdx + 1];
        tt = t * t;
        innerColor = LIGHT_ul_Interpol2Colors(*colors, 0, tt);
        midColor = LIGHT_ul_Interpol2Colors(*colors, 0, t);
        if (outerT < 1.0)
            outerColor = LIGHT_ul_Interpol2Colors(*colors, 0, outerT);
        else
            outerColor = 0;

        colors[0] = outerColor;
        colors[1] = outerColor;
        colors[18] = outerColor;
        colors[19] = outerColor;
        colors[2] = midColor;
        colors[3] = midColor;
        colors[4] = midColor;
        colors[5] = midColor;
        colors[14] = midColor;
        colors[15] = midColor;
        colors[16] = midColor;
        colors[17] = midColor;
        colors[6] = innerColor;
        colors[7] = innerColor;
        colors[8] = innerColor;
        colors[9] = innerColor;
        colors[10] = innerColor;
        colors[11] = innerColor;
        colors[12] = innerColor;
        colors[13] = innerColor;

        uvs = &GFX_gpst_Geo->UVs[quadIdx];
        uStep = (uvs[2].u - uvs->u) * 0.2;
        uvs[2].u = (float)(uvs->u + uStep);
        uvs[3].u = (float)(uvs->u + uStep);
        uvs[4].u = (float)(uvs->u + uStep);
        uvs[5].u = (float)(uvs->u + uStep);
        uvs[6].u = (float)(uStep + uStep + uvs->u);
        uvs[7].u = (float)(uStep + uStep + uvs->u);
        uvs[8].u = (float)(uStep + uStep + uvs->u);
        uvs[9].u = (float)(uStep + uStep + uvs->u);
        uvs[10].u = (float)(3.0 * uStep + uvs->u);
        uvs[11].u = (float)(3.0 * uStep + uvs->u);
        uvs[12].u = (float)(3.0 * uStep + uvs->u);
        uvs[13].u = (float)(3.0 * uStep + uvs->u);
        uvs[14].u = (float)(uStep * 4.0 + uvs->u);
        uvs[15].u = (float)(uStep * 4.0 + uvs->u);
        uvs[16].u = (float)(uStep * 4.0 + uvs->u);
        uvs[17].u = (float)(uStep * 4.0 + uvs->u);
    }

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed >= effect->f1)
        return STR_l_Effect_Done(run->sd, 1);
    return 0;
}

// 16: reveals the string behind a sweep line. Every letter of a run
// with this effect owns two quads -- the letter itself in corners 0 to
// 3 and a trailing copy in 4 to 7 -- which is what the effect's own
// pre-pass asks the renderer for. color2's bits, read as an int, pick
// the direction: 0 and 1 sweep vertically, 2 and 3 horizontally.

static void STR_l_Effect_ParkLetter(Vec3* quad)
{
    quad[0].x = -100.0f;
    quad[1].x = -100.0f;
    quad[2].x = -100.0f;
    quad[3].x = -100.0f;
    quad[0].y = -100.0f;
    quad[1].y = -100.0f;
    quad[2].y = -100.0f;
    quad[3].y = -100.0f;
}

static void STR_l_Effect_ParkTrail(Vec3* quad)
{
    quad[4].x = -100.0f;
    quad[5].x = -100.0f;
    quad[6].x = -100.0f;
    quad[7].x = -100.0f;
    quad[4].y = -100.0f;
    quad[5].y = -100.0f;
    quad[6].y = -100.0f;
    quad[7].y = -100.0f;
}

// 0 and 1: the line runs across the string and the letters above or
// below it are hidden, which way round depending on the direction
static void STR_l_Effect_RevealOneVertical(tStrEffectRun* run, int quadIdx,
                                           int direction, float band)
{
    Vec3* quad;
    tGeoUVElem* uvs;
    double at;
    double trailEdge;
    float trailV;
    int movingA;
    int movingB;
    int fixedA;
    int fixedB;

    // the two corners of the edge that travels with the line, and the
    // two of the edge that stays put
    movingA = direction;
    movingB = 3 - (direction != 0);
    fixedA = 1 - direction;
    fixedB = (direction != 0) + 2;

    quad = &GFX_gpst_Geo->vertices[4 * quadIdx];

    if (quad[1].y > (double)band)
    {
        if (fixedA)
            STR_l_Effect_ParkLetter(quad);
        STR_l_Effect_ParkTrail(quad);
        return;
    }
    if (quad->y < (double)band)
    {
        if (direction != 0)
            STR_l_Effect_ParkLetter(quad);
        STR_l_Effect_ParkTrail(quad);
        return;
    }

    uvs = &GFX_gpst_Geo->UVs[4 * quadIdx];
    at = (band - quad[fixedA].y)
         / (quad[movingA].y - quad[fixedA].y)
         * (uvs[movingA].v - uvs[fixedA].v)
         + uvs[fixedA].v;
    uvs[movingB].v = (float)at;
    uvs[movingA].v = (float)at;
    quad[movingB].y = band;
    quad[movingA].y = band;

    trailV = uvs[movingA].v;
    uvs[7].v = uvs[movingA].v;
    uvs[6].v = trailV;
    uvs[5].v = trailV;
    uvs[4].v = trailV;

    quad[movingB + 4].y = band;
    quad[movingA + 4].y = band;

    // the trail reaches the edge of the screen the line came from
    if (direction == 0)
        trailEdge = (double)(uint16_t)run->str->height;
    else
        trailEdge = 0.0;
    quad[fixedB + 4].y = (float)trailEdge;
    quad[fixedA + 4].y = (float)trailEdge;
    quad[fixedA + 4].x = quad[fixedA + 4].x + run->effect->color1;
    quad[fixedB + 4].x = run->effect->color1 + quad[fixedB + 4].x;
}

// 2: the line runs down the string from its left edge
static void STR_l_Effect_RevealOneRightward(tStrEffectRun* run, int quadIdx, float line)
{
    Vec3* quad;
    tGeoUVElem* uvs;
    double at;
    double right;
    float trailU;

    quad = &GFX_gpst_Geo->vertices[4 * quadIdx];

    if (quad[3].x < (double)line)
    {
        STR_l_Effect_ParkTrail(quad);
        return;
    }
    if (quad->x > (double)line)
    {
        STR_l_Effect_ParkLetter(quad);
        STR_l_Effect_ParkTrail(quad);
        return;
    }

    uvs = &GFX_gpst_Geo->UVs[4 * quadIdx];
    at = (line - quad->x) / (quad[3].x - quad->x) * (uvs[3].u - uvs->u) + uvs->u;
    uvs[3].u = (float)at;
    uvs[2].u = (float)at;
    quad[3].x = line;
    quad[2].x = line;

    trailU = uvs[2].u;
    uvs[7].u = uvs[2].u;
    uvs[6].u = trailU;
    uvs[5].u = trailU;
    uvs[4].u = trailU;

    quad[5].x = line;
    quad[4].x = line;
    right = (double)(uint16_t)run->str->width;
    quad[7].x = (float)right;
    quad[6].x = (float)right;
    quad[6].y = quad[6].y + run->effect->color1;
    quad[7].y = run->effect->color1 + quad[7].y;
}

// 3: the same the other way round, from the right edge
static void STR_l_Effect_RevealOneLeftward(tStrEffectRun* run, int quadIdx, float line)
{
    Vec3* quad;
    tGeoUVElem* uvs;
    double at;
    float trailU;
    float trailY;

    quad = &GFX_gpst_Geo->vertices[4 * quadIdx];

    if (quad[3].x < (double)line)
    {
        STR_l_Effect_ParkLetter(quad);
        STR_l_Effect_ParkTrail(quad);
        return;
    }
    if (quad->x > (double)line)
    {
        STR_l_Effect_ParkTrail(quad);
        return;
    }

    uvs = &GFX_gpst_Geo->UVs[4 * quadIdx];
    at = (line - quad[3].x) / (quad->x - quad[3].x) * (uvs->u - uvs[3].u) + uvs[3].u;
    uvs[1].u = (float)at;
    uvs[0].u = (float)at;
    quad[1].x = line;
    quad[0].x = line;

    trailU = uvs->u;
    uvs[7].u = uvs->u;
    uvs[6].u = trailU;
    uvs[5].u = trailU;
    uvs[4].u = trailU;

    trailY = quad[4].y;
    quad[5].x = 0.0;
    quad[4].x = 0.0;
    quad[7].x = line;
    quad[6].x = line;
    quad[4].y = (float)(trailY + run->effect->color1);
    quad[5].y = run->effect->color1 + quad[5].y;
}

int STR_l_Effect_Reveal(tStrEffectRun* run)
{
    tGEOSTREffectStruct* effect = run->effect;
    Vec3* verts;
    Vec3* quad;
    float boxTop;
    float boxBottom;
    float boxLeft;
    float boxHeight;
    float boxWidth;
    double boxRight;
    double progress;
    float line;
    double elapsed;
    int direction;
    int quadIdx;
    int i;

    // the box around the letters' own quads: the top and bottom of
    // each letter and its left and right edge
    verts = GFX_gpst_Geo->vertices;
    quad = &verts[4 * run->spec[run->startIdx + run->sd->index]];
    boxTop = quad->y;
    boxBottom = quad->y;
    boxLeft = quad->x;
    boxRight = quad->x;
    for (i = 0; i < run->count; i++)
    {
        quad = &verts[4 * run->spec[run->startIdx + i + run->sd->index]];
        if (quad->y > (double)boxTop)
            boxTop = quad->y;
        if (quad[1].y < (double)boxBottom)
            boxBottom = quad[1].y;
        if (boxRight < quad[2].x)
            boxRight = quad[2].x;
        if (quad->x < (double)boxLeft)
            boxLeft = quad->x;
    }

    direction = *(int32_t*)&(effect->color2);
    boxHeight = boxTop - boxBottom;
    boxWidth = (float)(boxRight - boxLeft);
    progress = effect->timeElapsed / effect->f1;

    if (direction == 0 || direction == 1)
    {
        if (direction != 0)
            progress = 1.0 - progress;
        line = (float)(progress * boxHeight + boxBottom);
        for (i = 0; i < run->count; i++)
        {
            quadIdx = run->spec[run->startIdx + i + run->sd->index];
            STR_l_Effect_RevealOneVertical(run, quadIdx, direction, line);
        }
    }
    else if (direction == 2)
    {
        line = (float)(progress * boxWidth + boxLeft);
        for (i = 0; i < run->count; i++)
        {
            quadIdx = run->spec[run->startIdx + i + run->sd->index];
            STR_l_Effect_RevealOneRightward(run, quadIdx, line);
        }
    }
    else if (direction == 3)
    {
        line = (float)((1.0 - progress) * boxWidth + boxLeft);
        for (i = 0; i < run->count; i++)
        {
            quadIdx = run->spec[run->startIdx + i + run->sd->index];
            STR_l_Effect_RevealOneLeftward(run, quadIdx, line);
        }
    }

    elapsed = TIM_gf_dt + effect->timeElapsed;
    effect->timeElapsed = (float)elapsed;
    if (elapsed < effect->f1)
        return 0;
    return 1;
}

int STR_l_Effect_Treat(tGEOSTRStruct* str, tGEOSTREffectStruct* effect)
{
    tStrEffectRun run;
    tGEOSTRStringData* sd;
    int letterBase;
    int kind;

    sd = STR_pst_GetString(str, effect->stringid);
    if (!sd)
        return 1;
    if ((sd->flags & 0x400) != 0)
        return 0;

    run.str = str;
    run.effect = effect;
    run.sd = sd;
    run.spec = str->specularColorsPtr;
    run.startIdx = STR_l_GetValidIndex(str, sd, effect->index);
    run.count = STR_l_GetValidNumber(str, sd, run.startIdx, effect->number);

    letterBase = run.startIdx + sd->index;
    run.letters = &str->letters[letterBase];
    run.lettersEnd = &run.letters[run.count];
    run.firstVertex = 4 * run.spec[letterBase];
    run.endVertex = 4 * run.spec[letterBase + run.count - 1] + 4;

    // a run whose last letter sits on quad zero has no geometry
    if (run.endVertex == 4)
        return 1;

    if ((effect->flags & 0x100) != 0 && (sd->flags & 0x100) != 0)
    {
        // the frame moves with the letters; 0x200 means the frame only
        if ((effect->flags & 0x200) != 0)
        {
            run.lettersEnd = &str->letters[letterBase];
            run.endVertex = run.firstVertex;
        }
        run.frameVertices = 36;
        run.frameFirst = 4 * sd->i2;
    }
    else
    {
        run.frameVertices = 0;
        run.frameFirst = 0;
        if (!run.count)
            return 1;
    }

    kind = effect->flags & 0xff;
    switch (kind)
    {
    case 1:
        return STR_l_Effect_FadeToBlack(&run);
    case 2:
        return STR_l_Effect_Blink(&run);
    case 3:
        return STR_l_Effect_ColorByPosition(&run);
    case 4:
        return STR_l_Effect_FadeBetweenColors(&run);
    case 5:
        return STR_l_Effect_Wave(&run);
    case 6:
        return STR_l_Effect_Shake(&run);
    case 7:
        return STR_l_Effect_OpenVertically(&run);
    case 8:
        return STR_l_Effect_CloseVertically(&run);
    case 9:
    case 10:
        return STR_l_Effect_ScaleRun(&run, kind);
    case 11:
    case 12:
        return STR_l_Effect_ScaleAndFade(&run, kind);
    case 13:
    case 14:
        return STR_l_Effect_FoldY(&run, kind);
    case 15:
        return STR_l_Effect_Rotate(&run);
    case 16:
        return STR_l_Effect_Reveal(&run);
    case 17:
        return STR_l_Effect_ClipBand(&run);
    case 18:
        return STR_l_Effect_ShearLetters(&run);
    case 19:
        return STR_l_Effect_Scramble(&run);
    case 20:
        return STR_l_Effect_Slice(&run);
    default:
        return 0;
    }
}

void STR_Effect_TreatAll(tGEOSTRStruct* str)
{
    int i;

    for (i = 0; i < str->effectsNum; ) {
        if ((STR_l_Effect_Treat(str, &str->effect[i]) & 0x1) != 0)
        {
            if (i < (str->effectsNum - 1))
            {
                memcpy(&str->effect[i], &str->effect[str->effectsNum - 1], sizeof(tGEOSTREffectStruct));
                str->effectsNum--;
                continue;
            }
            str->effectsNum--;
        }
        else
        {
            i++;
        }
    }
}

int STR_l_Effect_PreTreat(tGEOSTRStruct* str, tGEOSTREffectStruct* effect)
{
    tGEOSTRStringData* sd;
    int currIdx, num;

    switch (effect->flags & 0xff) {
    case 16:
        sd = STR_pst_GetString(str, effect->stringid);
        if (!sd || (sd->flags & 0x400) != 0)
            return 0;
        
        currIdx = STR_l_GetValidIndex(str, sd, effect->index);
        num = STR_l_GetValidNumber(str, sd, currIdx, effect->number);
        for (; currIdx < num; currIdx++)
        {
            str->specularColorsPtr[currIdx + sd->index] = 1;
        }
        return num;
        break;
    case 20:
        sd = STR_pst_GetString(str, effect->stringid);
        if (!sd || (sd->flags & 0x400) != 0)
            return 0;
             
        currIdx = STR_l_GetValidIndex(str, sd, effect->index);
        num = STR_l_GetValidNumber(str, sd, currIdx, effect->number);                
        for (; currIdx < num; currIdx++)
        {
            str->specularColorsPtr[currIdx + sd->index] = 4;
        }
        return 4 * num;
        break;
    default:
        return 0;
    }   
}

int STR_l_Effect_PreTreatAll(tGEOSTRStruct* str)
{
    int total;
    int i;

    memset(str->specularColorsPtr, 0, 4 * str->unkNum);
    total = 0;
    for (i = 0; i < str->effectsNum; i++)
        total += STR_l_Effect_PreTreat(str, &str->effect[i]);
    return total;
}

// A nine-patch: three columns by three rows of quads over four x edges
// and four y edges. The middle column and the middle row sample one line
// of texels each, at the midpoint between the glyph's two edges, which is
// what lets them stretch.
void STR_AddFrame(int baseQuad, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, int16_t x3, int16_t y3,
                  float z, int color, tFontGlypthElem* glyph, int chunkIdx)
{
    tGeoTriangleChunks* chunk;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    float xEdge[4];
    float yEdge[4];
    float uEdge[4];
    float vEdge[4];
    float uMid;
    float vMid;
    int baseVertex;
    int quadBase;
    int quad;
    int col;
    int row;
    int i;

    baseVertex = 4 * baseQuad;
    verts = &GFX_gpst_Geo->vertices[baseVertex];
    uvs = &GFX_gpst_Geo->UVs[baseVertex];

    xEdge[0] = x0;
    xEdge[1] = x1;
    xEdge[2] = x2;
    xEdge[3] = x3;
    yEdge[0] = y0;
    yEdge[1] = y1;
    yEdge[2] = y2;
    yEdge[3] = y3;

    uMid = (float)((glyph->y + glyph->x) * 0.5);
    vMid = (float)((glyph->h + glyph->w) * 0.5);
    uEdge[0] = glyph->x;
    uEdge[1] = uMid;
    uEdge[2] = uMid;
    uEdge[3] = glyph->y;
    vEdge[0] = glyph->h;
    vEdge[1] = vMid;
    vEdge[2] = vMid;
    vEdge[3] = glyph->w;

    for (col = 0; col < 3; col++)
    {
        for (row = 0; row < 3; row++)
        {
            verts[0].x = xEdge[col];
            verts[0].y = yEdge[row];
            verts[0].z = z;
            verts[1].x = xEdge[col];
            verts[1].y = yEdge[row + 1];
            verts[1].z = z;
            verts[2].x = xEdge[col + 1];
            verts[2].y = yEdge[row + 1];
            verts[2].z = z;
            verts[3].x = xEdge[col + 1];
            verts[3].y = yEdge[row];
            verts[3].z = z;

            uvs[0].u = uEdge[col];
            uvs[0].v = vEdge[row];
            uvs[1].u = uEdge[col];
            uvs[1].v = vEdge[row + 1];
            uvs[2].u = uEdge[col + 1];
            uvs[2].v = vEdge[row + 1];
            uvs[3].u = uEdge[col + 1];
            uvs[3].v = vEdge[row];

            verts += 4;
            uvs += 4;
        }
    }

    for (i = 0; i < 36; i++)
        GFX_gpst_Geo->vertexColors[baseVertex + 1 + i] = color;

    chunk = &GFX_gpst_Geo->triangleChunks[chunkIdx];
    tris = &chunk->triangleChunkElems[chunk->triangleNum];
    chunk->triangleNum += 18;
    for (quad = 0; quad < 9; quad++)
    {
        quadBase = baseVertex + 4 * quad;

        tris[0].v1 = quadBase;
        tris[0].uv1 = quadBase;
        tris[0].v2 = quadBase + 1;
        tris[0].uv2 = quadBase + 1;
        tris[0].v3 = quadBase + 2;
        tris[0].uv3 = quadBase + 2;
        tris[1].v1 = quadBase;
        tris[1].uv1 = quadBase;
        tris[1].v2 = quadBase + 2;
        tris[1].uv2 = quadBase + 2;
        tris[1].v3 = quadBase + 3;
        tris[1].uv3 = quadBase + 3;
        tris += 2;
    }
}

// One quad, the degenerate case of the nine-patch above: two x edges,
// two y edges, and the glyph's own four texture edges.
void STR_AddSprite(int baseQuad, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                   float z, int color, tFontGlypthElem* glyph, int chunkIdx)
{
    tGeoTriangleChunks* chunk;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    int* colors;
    int baseVertex;

    baseVertex = 4 * baseQuad;

    verts = &GFX_gpst_Geo->vertices[baseVertex];
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z;
    verts[1].x = x0;
    verts[1].y = y1;
    verts[1].z = z;
    verts[2].x = x1;
    verts[2].y = y1;
    verts[2].z = z;
    verts[3].x = x1;
    verts[3].y = y0;
    verts[3].z = z;

    colors = &GFX_gpst_Geo->vertexColors[baseVertex + 1];
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;

    uvs = &GFX_gpst_Geo->UVs[baseVertex];
    uvs[0].u = glyph->x;
    uvs[0].v = glyph->h;
    uvs[1].u = glyph->x;
    uvs[1].v = glyph->w;
    uvs[2].u = glyph->y;
    uvs[2].v = glyph->w;
    uvs[3].u = glyph->y;
    uvs[3].v = glyph->h;

    chunk = &GFX_gpst_Geo->triangleChunks[chunkIdx];
    tris = &chunk->triangleChunkElems[chunk->triangleNum];
    chunk->triangleNum += 2;

    tris[0].v1 = baseVertex;
    tris[0].uv1 = baseVertex;
    tris[0].v2 = baseVertex + 1;
    tris[0].uv2 = baseVertex + 1;
    tris[0].v3 = baseVertex + 2;
    tris[0].uv3 = baseVertex + 2;
    tris[1].v1 = baseVertex;
    tris[1].uv1 = baseVertex;
    tris[1].v2 = baseVertex + 2;
    tris[1].uv2 = baseVertex + 2;
    tris[1].v3 = baseVertex + 3;
    tris[1].uv3 = baseVertex + 3;
}

void STR_ComputeFrame(tGEOSTRStruct* str, int idx, tGEOSTRStringData* stringData)
{
    Vec3 min, max;     

    *(int32_t *)&(min.x) = -1; // ez azért kell, hogy lásd ezt: ***** lejjebb

    if ((stringData->flags & 0x20) != 0)
    {
        STR_GetRectShort(str, idx, 0, -1, &min, &max);
        stringData->framePosX1 = (int)min.x;
        stringData->framePosY1 = (int)min.y;
        stringData->framePosX2 = (int)max.x;
        stringData->framePosY2 = (int)max.y;
    }

    if ((stringData->flags & 0x100) != 0)
    {
        if ((stringData->flags & 0x3) == 0x1)
        {
            STR_w_XFramePivot += D3D_FlatLayerOffsetX((int)gpst_DX_stDD->ct.cam.width,
                                                      (int)gpst_DX_stDD->ct.cam.height)
                               + (stringData->framePosX1 + stringData->framePosX2) / -2;
        }
        else if ((stringData->flags & 0x3) == 0x2)
        {
            STR_w_XFramePivot += stringData->framePosX1 - stringData->framePosX2;
        }
 
        if ((stringData->flags & 0xc) == 0x4)
        {
            STR_w_YFramePivot += (stringData->framePosY1 + stringData->framePosY2) / -2;
        }
        else if ((stringData->flags & 0xc) == 0x8)
        {
            STR_w_YFramePivot += stringData->framePosY1 - stringData->framePosY2;
        }
    }

    if ((stringData->flags & 0xf) != 0)
    {
        if ((*(int32_t *)&(min.x)) == -1) // itt ellenõriz rá *****
            STR_GetRectShort(str, idx, 0, -1, &min, &max);

        if ((stringData->flags & 0x3) == 0x1)
        {
            STR_w_XPivot += D3D_FlatLayerOffsetX((int)gpst_DX_stDD->ct.cam.width,
                                                 (int)gpst_DX_stDD->ct.cam.height)
                          + (int16_t)(unsigned int)(max.x + min.x) / -2;
        }
        else if ((stringData->flags & 0x3) == 0x2)
        {
            STR_w_XPivot -= (int16_t)(max.x - min.x);
        }

        if ((stringData->flags & 0xc) == 0x4)
        {
            STR_w_YPivot += (int16_t)(unsigned int)(max.y + min.y) / -2;
        }
        else if ((stringData->flags & 0xc) == 0x8)
        {
            STR_w_YPivot -= (int16_t)(max.y - min.y);
        }
    }
}

void STR_Render(tGameObjectMain* gao)
{
    tGameObjectVisualObject* gaovo;
    tGEOSTRStruct* str;
    tGEOSTRStringData* sd;
    tGEOSTRStringData* stringEnd;
    tGEOSTRLetter* letter;
    tGEOSTRLetter* lettersEnd;
    tGameObjectExtended* extended;
    tGEOMAT* material;
    tFontData* font;
    tFontGlypthElem* glyph;
    tUnkGeoStrStruct perMat;
    unsigned int* order;
    unsigned int matCount;
    unsigned int ch;
    unsigned int glyphIdx;
    int* repeatSlot;
    int displayFlags;
    int quadTotal;
    int chunkCount;
    int chunkIdx;
    int mat;
    int letterIdx;
    int repeats;
    int baseQuad;
    int stringIdx;
    int16_t top;
    int16_t bottom;
    int16_t left;
    int16_t right;
    int16_t dx;
    int16_t dy;
    int16_t frameLeft;
    int16_t frameRight;
    int16_t frameHeight;
    int i;

    gaovo = gao->basic->actorObjectOrVisualObject;
    str = (tGEOSTRStruct*)(gaovo->GROGEOOrSomethingElse);
    if (str->renderCnt == gpst_DX_stDD->rasters[4].unk1)
        return;

    str->renderCnt = gpst_DX_stDD->rasters[4].unk1;
    // the text is laid out across the width the scenes were laid
    // out for; where that sits in the window is added to each
    // letter as it is placed
    str->width = (unsigned int)D3D_FlatLayerWidth(
        (int)gpst_DX_stDD->ct.cam.width, (int)gpst_DX_stDD->ct.cam.height);
    str->height = (unsigned int)gpst_DX_stDD->ct.cam.height;

    displayFlags = gpst_DX_stDD->flags;
    if ((displayFlags & 0x800) == 0)
        gpst_DX_stDD->flags = displayFlags | 0x200;

    material = (tGEOMAT*)(gaovo->GROMAT);
    if (!material)
        return;

    extended = gao->extended;
    if (extended && extended->AIInstance)
    {
        AI_C_Callback = 1;
        AI_ExecCallback(gao, 0);
        AI_C_Callback = 0;
    }

    STR_SetDisplay();
    str->fontMaterialID = -1;
    str->fontDesc = 0;
    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = gaovo->GROMAT;

    if (material->intf->id == 4)
    {
        matCount = material->multiMaterialNumIfType4ThenAmbiantColor;
        if ((int)matCount > 64)
            matCount = 64;
    }
    else
    {
        matCount = 1;
    }

    // only the 24 counters the sub-material order below walks are
    // cleared, which is what the original's 96 bytes are
    memset(perMat.triangleNum, 0, 24 * sizeof(int));
    str->specularColorsPtr = (int*)gpst_DX_stDD->bigBuffer->data2SpecularColors;
    quadTotal = STR_l_Effect_PreTreatAll(str);

    // first pass: how many quads each sub-material needs
    stringEnd = &str->stringData[str->stringNum];
    for (sd = str->stringData; sd < stringEnd; ++sd)
    {
        if ((sd->flags & 0x400) != 0)
            continue;

        letter = &str->letters[sd->index + sd->index2];
        lettersEnd = &letter[sd->number2];
        if (lettersEnd <= letter)
            continue;

        if ((sd->flags & 0x100) != 0)
        {
            mat = (int)(((sd->frame >> 12) & 0x3F) % matCount);
            perMat.triangleNum[mat] += 9;
            quadTotal += 9;
        }

        quadTotal += (int)(lettersEnd - letter);
        for (; letter < lettersEnd; ++letter)
        {
            mat = (int)(((letter->flagsAndChar >> 12) & 0x3F) % matCount);
            letterIdx = (int)(letter - str->letters);
            perMat.triangleNum[mat] += str->specularColorsPtr[letterIdx] + 1;
        }
    }

    // the sub-materials are taken in a fixed order, and each one that
    // has quads and a font of its own gets a triangle chunk
    chunkCount = 0;
    for (order = ul_SubMatOrder; order <= &ul_SubMatOrder[23]; ++order)
    {
        mat = (int)*order;
        if (!perMat.triangleNum[mat])
        {
            perMat.fontData[mat] = 0;
            continue;
        }

        if ((int16_t)mat != str->fontMaterialID)
        {
            str->fontMaterialID = (int16_t)mat;
            str->fontDesc = STR_pst_GetFontDesc(
                (tGEOMAT*)(((tGameObjectVisualObject*)
                            (STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT),
                (int16_t)mat);
        }

        font = str->fontDesc;
        if (!font)
        {
            quadTotal -= perMat.triangleNum[mat];
            perMat.fontData[mat] = 0;
            continue;
        }

        perMat.triangleChunkID[mat] = chunkCount;
        perMat.materialID[chunkCount] = mat;
        perMat.fontData[mat] = font;
        chunkCount++;
    }

    if (quadTotal)
    {
        GFX_NeedGeomEx(4 * quadTotal, 4 * quadTotal, chunkCount, 1u, 1);
        for (chunkIdx = 0; chunkIdx < chunkCount; chunkIdx++)
        {
            mat = perMat.materialID[chunkIdx];
            GFX_NeedGeomExElem(chunkIdx, 2 * perMat.triangleNum[mat]);
            GFX_gpst_Geo->triangleChunks[chunkIdx].triangleNum = 0;
            GFX_gpst_Geo->triangleChunks[chunkIdx].materialNum = mat;
        }

        // second pass: place the frame and the letters of every string
        baseQuad = 0;
        stringIdx = 0;
        stringEnd = &str->stringData[str->stringNum];
        for (sd = str->stringData; sd < stringEnd; ++sd, ++stringIdx)
        {
            if ((sd->flags & 0x400) != 0)
                continue;

            STR_w_XPivot = (int)((double)(uint16_t)str->width * sd->pivotPoint.x);
            STR_w_XFramePivot = STR_w_XPivot;
            STR_w_YPivot = (int)((double)(uint16_t)str->height * sd->pivotPoint.y);
            STR_w_YFramePivot = STR_w_YPivot;

            letter = &str->letters[sd->index2 + sd->index];
            lettersEnd = &letter[sd->number2];
            if (lettersEnd <= letter)
                continue;

            STR_ComputeFrame(str, stringIdx, sd);

            if ((sd->flags & 0x100) != 0)
            {
                mat = (int)(((sd->frame >> 12) & 0x3F) % matCount);
                font = perMat.fontData[mat];
                str->fontDesc = font;
                // without a font for the frame the string draws nothing,
                // letters included
                if (!font)
                    continue;

                ch = (uint8_t)sd->frame;
                glyphIdx = (ch < 32) ? 0 : ch - 32;
                glyph = &font->fge[glyphIdx];

                frameLeft = sd->framePosX1;
                frameRight = STR_w_XFramePivot + sd->framePosX2;
                frameHeight = str->height;
                sd->i2 = baseQuad;

                STR_AddFrame(
                    baseQuad,
                    STR_w_XFramePivot + frameLeft - sd->frameBorderSize1,
                    frameHeight - sd->framePosY1 - STR_w_YFramePivot + sd->frameBorderSize2,
                    STR_w_XFramePivot + frameLeft,
                    frameHeight - sd->framePosY1 - STR_w_YFramePivot,
                    frameRight,
                    frameHeight - sd->framePosY2 - STR_w_YFramePivot,
                    frameRight + sd->frameBorderSize1,
                    frameHeight - sd->framePosY2 - STR_w_YFramePivot - sd->frameBorderSize2,
                    0.0f,
                    sd->frameColor,
                    glyph,
                    perMat.triangleChunkID[mat]);
                baseQuad += 9;
            }

            if (sd->index2)
            {
                STR_w_XPivot += str->letters[sd->index].x - letter->x;
                STR_w_YPivot += str->letters[sd->index].y - letter->y;
            }

            for (; letter < lettersEnd; ++letter)
            {
                mat = (int)(((letter->flagsAndChar >> 12) & 0x3F) % matCount);
                font = perMat.fontData[mat];
                str->fontDesc = font;
                str->fontMaterialID = (int16_t)mat;
                if (!font)
                    continue;

                ch = (uint8_t)letter->flagsAndChar;
                glyphIdx = (ch < 32) ? 0 : ch - 32;
                glyph = &font->fge[glyphIdx];

                if ((letter->flagsAndChar & 0x1000000) != 0)
                    dx = (int16_t)letter->width >> 1;
                else
                    dx = ((letter->flagsAndChar & 0x4000000) != 0) ? (uint16_t)letter->width : 0;
                STR_w_LetterDX = dx;

                if ((letter->flagsAndChar & 0x2000000) != 0)
                    dy = (int16_t)letter->height >> 1;
                else
                    dy = ((letter->flagsAndChar & 0x8000000) != 0) ? (uint16_t)letter->height : 0;
                STR_w_LetterDY = dy;

                top = str->height + dy - STR_w_YPivot - letter->y;
                bottom = top - (uint16_t)letter->height;
                left = letter->x + (uint16_t)(STR_w_XPivot - dx);
                right = letter->width + left;
                chunkIdx = perMat.triangleChunkID[mat];

                STR_AddSprite(baseQuad, letter->x + STR_w_XPivot - dx, top,
                              left + (uint16_t)letter->width, bottom,
                              0.0f, letter->textColor, glyph, chunkIdx);

                // the effect pre-pass left the number of extra copies
                // this letter wants in its own slot, and the slot then
                // takes the quad the letter landed on
                repeatSlot = &str->specularColorsPtr[letter - str->letters];
                repeats = *repeatSlot;
                *repeatSlot = baseQuad++;
                for (i = 0; i < repeats; i++)
                {
                    STR_AddSprite(baseQuad++, left, top, right, bottom,
                                  0.0f, letter->textColor, glyph, chunkIdx);
                }
            }
        }

        STR_Effect_TreatAll(str);
        ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = (tGroGeneric*)GFX_gpst_Geo;
        GFX_gpst_Geo->intf->Render(STR_sgpst_GO);
    }

    extended = gao->extended;
    if (extended && extended->AIInstance)
        AI_ExecCallback(gao, 1);

    gpst_DX_stDD->flags = sul_SaveDisplayInfo;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = sul_SaveDrawMask;
    gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
}

void STR_Init()
{
    int i;

    for (i = 0; i < 256; ++i)
        STR_sgac_CharTable[i] = i;
    STR_sgac_CharTable[161] = 168;
    STR_sgac_CharTable[224] = 133;
    STR_sgac_CharTable[225] = 160;
    STR_sgac_CharTable[226] = 131;
    STR_sgac_CharTable[228] = 132;
    STR_sgac_CharTable[231] = 135;
    STR_sgac_CharTable[232] = 138;
    STR_sgac_CharTable[233] = 130;
    STR_sgac_CharTable[234] = 136;
    STR_sgac_CharTable[235] = 137;
    STR_sgac_CharTable[236] = 141;
    STR_sgac_CharTable[237] = 161;
    STR_sgac_CharTable[238] = 140;
    STR_sgac_CharTable[239] = 139;
    STR_sgac_CharTable[241] = 164;
    STR_sgac_CharTable[242] = 149;
    STR_sgac_CharTable[243] = 162;
    STR_sgac_CharTable[244] = 147;
    STR_sgac_CharTable[245] = 167;
    STR_sgac_CharTable[246] = 148;
    STR_sgac_CharTable[249] = 151;
    STR_sgac_CharTable[250] = 163;
    STR_sgac_CharTable[251] = 150;
    STR_sgac_CharTable[252] = 129;
    GRO_gast_Interface[10].CreateFromBuffer = STR_p_CreateFromBuffer;
    GRO_gast_Interface[10].Destroy = STR_Free;
    GRO_gast_Interface[10].HasSomethingToRender = STR_l_HasSomethingToRender;
    GRO_gast_Interface[10].Render = STR_Render;
    GRO_gast_Interface[10].Reinit = STR_Reinit;
    STR_sgpst_GO = OBJ_GameObject_Create(0x5000);
    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->objDrawMask &= ~0x10800u;
    STR_sgpst_Font = 0;
    // twenty-three of the twenty-four entries, as the original has it
    for (i = 0; i < 23; i++)                      // ezt átnézni
        STR_sgaac_Global[i].str[0] = 0;
    STR_sgl_MaxNumberOfFont = 30;
    STR_sgl_NumberOfFont = 0;
    STR_sgpst_Font = (tFontData*)MEM_p_Alloc(sizeof(tFontData) * 30);
}
