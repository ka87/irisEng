#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../graphicsobjects/geostr.h"
#include "../graphicsobjects/geo.h"
#include "../time.h"

/* The reveal effect was the one function in graphicsobjects/ big enough
   to be worth a differential test, and isolating it first made one
   cheap: the frozen copy is a drop-in, so both sides take the same run
   struct and write into the same geometry buffer.

   Only the geometry the effect touches is mocked. Nothing else in
   geostr.c is linked. */

int BASE_STR_l_Effect_Reveal(tStrEffectRun* run);
int NEW_STR_l_Effect_Reveal(tStrEffectRun* run);

#define LETTERS 11
#define QUADS_PER_LETTER 2
#define VERTS (LETTERS * QUADS_PER_LETTER * 4)

static tGEOGRO MOCK_geo;
static Vec3 MOCK_verts[VERTS];
static tGeoUVElem MOCK_uvs[VERTS];
static int MOCK_colors[VERTS + 1];

static tGEOSTRStruct MOCK_str;
static tGEOSTRStringData MOCK_sd;
static tGEOSTRLetter MOCK_letters[LETTERS];
static int MOCK_spec[LETTERS];

static int g_fails;
static int g_diffs;
static double g_worstAbs;
static double g_worstRel;
static char g_case[128];

static unsigned s_rng;

static void rngSeed(unsigned seed)
{
    s_rng = seed ? seed : 1u;
}

static float rngFloat(float lo, float hi)
{
    s_rng = s_rng * 1664525u + 1013904223u;
    return lo + (hi - lo) * (float)((s_rng >> 8) & 0xFFFF) / 65535.0f;
}

static int closeEnough(float a, float b, const char* what, int idx)
{
    double d, r, m;

    if (memcmp(&a, &b, sizeof(float)) == 0)
        return 1;
    if ((a != a) != (b != b))
    {
        printf("    DIFF %s | %s[%d]: old %.9g new %.9g (NaN on one side)\n",
               g_case, what, idx, (double)a, (double)b);
        return 0;
    }
    if (a != a)
        return 1;

    d = fabs((double)a - (double)b);
    m = fabs((double)a) > fabs((double)b) ? fabs((double)a) : fabs((double)b);
    r = m > 0.0 ? d / m : d;
    g_diffs++;
    if (d > g_worstAbs)
        g_worstAbs = d;
    if (r > g_worstRel)
        g_worstRel = r;
    if (r > 1e-5 && d > 1e-5)
    {
        printf("    DIFF %s | %s[%d]: old %.9g new %.9g (rel %.3g)\n",
               g_case, what, idx, (double)a, (double)b, r);
        return 0;
    }
    return 1;
}

/* One case's starting state: a row of letters, each on two quads, with
   the second quad of each letter following the first. */
static void buildState(unsigned seed)
{
    int i;
    int q;

    rngSeed(seed);
    memset(MOCK_verts, 0, sizeof(MOCK_verts));
    memset(MOCK_uvs, 0, sizeof(MOCK_uvs));
    memset(MOCK_colors, 0, sizeof(MOCK_colors));

    for (i = 0; i < LETTERS; i++)
    {
        float left = 40.0f + 36.0f * i + rngFloat(-3.0f, 3.0f);
        float right = left + 30.0f + rngFloat(-4.0f, 8.0f);
        float top = 300.0f + rngFloat(-40.0f, 40.0f);
        float bottom = top - 44.0f - rngFloat(-6.0f, 6.0f);

        MOCK_spec[i] = QUADS_PER_LETTER * i;
        q = 4 * MOCK_spec[i];

        /* the letter, laid out as the renderer lays one out */
        MOCK_verts[q + 0].x = left;
        MOCK_verts[q + 0].y = top;
        MOCK_verts[q + 1].x = left;
        MOCK_verts[q + 1].y = bottom;
        MOCK_verts[q + 2].x = right;
        MOCK_verts[q + 2].y = bottom;
        MOCK_verts[q + 3].x = right;
        MOCK_verts[q + 3].y = top;

        /* the trailing copy, which the effect moves about */
        MOCK_verts[q + 4] = MOCK_verts[q + 0];
        MOCK_verts[q + 5] = MOCK_verts[q + 1];
        MOCK_verts[q + 6] = MOCK_verts[q + 2];
        MOCK_verts[q + 7] = MOCK_verts[q + 3];

        MOCK_uvs[q + 0].u = rngFloat(0.0f, 0.5f);
        MOCK_uvs[q + 0].v = rngFloat(0.0f, 0.5f);
        MOCK_uvs[q + 1].u = MOCK_uvs[q + 0].u;
        MOCK_uvs[q + 1].v = MOCK_uvs[q + 0].v + 0.12f;
        MOCK_uvs[q + 2].u = MOCK_uvs[q + 0].u + 0.09f;
        MOCK_uvs[q + 2].v = MOCK_uvs[q + 1].v;
        MOCK_uvs[q + 3].u = MOCK_uvs[q + 2].u;
        MOCK_uvs[q + 3].v = MOCK_uvs[q + 0].v;
        MOCK_uvs[q + 4] = MOCK_uvs[q + 0];
        MOCK_uvs[q + 5] = MOCK_uvs[q + 1];
        MOCK_uvs[q + 6] = MOCK_uvs[q + 2];
        MOCK_uvs[q + 7] = MOCK_uvs[q + 3];

        MOCK_letters[i].flagsAndChar = 0x41 + i;
        MOCK_letters[i].x = (int16_t)left;
        MOCK_letters[i].y = (int16_t)top;
    }
}

static void fillRun(tStrEffectRun* run, tGEOSTREffectStruct* effect, int count)
{
    MOCK_geo.vertices = MOCK_verts;
    MOCK_geo.UVs = MOCK_uvs;
    MOCK_geo.vertexColors = (unsigned int*)MOCK_colors;
    MOCK_geo.vertexNum = VERTS;
    MOCK_geo.uvNum = VERTS;
    GFX_gpst_Geo = &MOCK_geo;

    MOCK_str.width = 640;
    MOCK_str.height = 480;
    MOCK_str.letters = MOCK_letters;
    MOCK_str.specularColorsPtr = MOCK_spec;
    MOCK_sd.index = 0;
    MOCK_sd.flags = 0x100;

    run->str = &MOCK_str;
    run->effect = effect;
    run->sd = &MOCK_sd;
    run->spec = MOCK_spec;
    run->startIdx = 0;
    run->count = count;
    run->letters = MOCK_letters;
    run->lettersEnd = &MOCK_letters[count];
    run->firstVertex = 4 * MOCK_spec[0];
    run->endVertex = 4 * MOCK_spec[count - 1] + 4;
    run->frameVertices = 0;
    run->frameFirst = 0;
}

static Vec3 g_vertsBase[VERTS];
static tGeoUVElem g_uvsBase[VERTS];
static int g_colorsBase[VERTS + 1];

static int compare(void)
{
    char what[48];
    int ok = 1;
    int i;

    for (i = 0; i < VERTS; i++)
    {
        snprintf(what, sizeof(what), "vert%d", i);
        ok &= closeEnough(g_vertsBase[i].x, MOCK_verts[i].x, what, 0);
        ok &= closeEnough(g_vertsBase[i].y, MOCK_verts[i].y, what, 1);
        ok &= closeEnough(g_vertsBase[i].z, MOCK_verts[i].z, what, 2);
        snprintf(what, sizeof(what), "uv%d", i);
        ok &= closeEnough(g_uvsBase[i].u, MOCK_uvs[i].u, what, 0);
        ok &= closeEnough(g_uvsBase[i].v, MOCK_uvs[i].v, what, 1);
    }
    for (i = 0; i <= VERTS; i++)
    {
        if (g_colorsBase[i] != MOCK_colors[i])
        {
            printf("    DIFF %s | color[%d]: old 0x%08X new 0x%08X\n",
                   g_case, i, (unsigned)g_colorsBase[i], (unsigned)MOCK_colors[i]);
            ok = 0;
        }
    }
    if (!ok)
    {
        printf("  FAIL %s\n", g_case);
        g_fails++;
    }
    return ok;
}

static void runCase(const char* name, int direction, float progress,
                    float lineOffset, int count, unsigned seed)
{
    tGEOSTREffectStruct effect;
    tStrEffectRun run;
    int rbase;
    int rnew;
    float dirBits;

    snprintf(g_case, sizeof(g_case), "%s", name);

    memset(&effect, 0, sizeof(effect));
    effect.flags = 16;
    effect.f1 = 1.0f;
    effect.timeElapsed = progress;
    effect.color1 = lineOffset;
    *(int32_t*)&dirBits = direction;
    effect.color2 = dirBits;
    TIM_gf_dt = 0.05f;

    buildState(seed);
    fillRun(&run, &effect, count);
    rbase = BASE_STR_l_Effect_Reveal(&run);
    memcpy(g_vertsBase, MOCK_verts, sizeof(MOCK_verts));
    memcpy(g_uvsBase, MOCK_uvs, sizeof(MOCK_uvs));
    memcpy(g_colorsBase, MOCK_colors, sizeof(MOCK_colors));

    memset(&effect, 0, sizeof(effect));
    effect.flags = 16;
    effect.f1 = 1.0f;
    effect.timeElapsed = progress;
    effect.color1 = lineOffset;
    effect.color2 = dirBits;

    buildState(seed);
    fillRun(&run, &effect, count);
    rnew = NEW_STR_l_Effect_Reveal(&run);

    if (rbase != rnew)
    {
        printf("    DIFF %s | return: old %d new %d\n", g_case, rbase, rnew);
        printf("  FAIL %s\n", g_case);
        g_fails++;
        return;
    }
    compare();
}

int main(void)
{
    char name[128];
    float progress[9] = { 0.0f, 0.01f, 0.2f, 0.45f, 0.5f, 0.7f, 0.95f, 1.0f, 1.4f };
    float offsets[3] = { 0.0f, 12.0f, -8.0f };
    int counts[4] = { 1, 2, 5, LETTERS };
    int cases = 0;
    int d, p, o, c;

    setvbuf(stdout, NULL, _IONBF, 0);

    for (d = 0; d <= 3; d++)
        for (p = 0; p < 9; p++)
            for (o = 0; o < 3; o++)
                for (c = 0; c < 4; c++)
                {
                    snprintf(name, sizeof(name),
                             "dir=%d t=%g offset=%g letters=%d",
                             d, progress[p], offsets[o], counts[c]);
                    runCase(name, d, progress[p], offsets[o], counts[c],
                            0x5EEDu + (unsigned)(p * 16 + c));
                    cases++;
                }

    /* a direction the effect does not know, which must leave the
       geometry alone and only advance the clock */
    for (d = 4; d <= 5; d++)
    {
        snprintf(name, sizeof(name), "dir=%d unknown", d);
        runCase(name, d, 0.3f, 0.0f, LETTERS, 0x1234u);
        cases++;
    }

    printf("string reveal effect\n");
    printf("  %d cases\n", cases);
    if (g_diffs)
        printf("  %d float(s) differ within tolerance; worst abs %g, worst rel %g\n",
               g_diffs, g_worstAbs, g_worstRel);
    else
        printf("  every value bit-identical\n");
    if (g_fails)
        printf("  %d case(s) differ\n", g_fails);
    else
        printf("  no behavioural differences\n");
    return g_fails ? 1 : 0;
}
