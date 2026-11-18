/* Which of the grid's functions decide a position's height, and which
 * must leave it alone.
 *
 * Three places in grid.c turn a cell back into a world position, and
 * they do not all want the same thing of z:
 *
 * - **GRID_ShortestPath** writes fresh path points. A path point is on
 *   the grid's plane and its height is nought, which both of that
 *   function's two writers say outright.
 * - **GRI_i_ComputeDest** adjusts a position it was *given* -- the
 *   caller's destination, nudged to the centre of a cell that is not a
 *   wall. That position already has a height and keeping it is the
 *   whole point: nought is the bottom of the world rather than the
 *   ground the object stands on, and a destination down there is one
 *   nothing can reach.
 *
 * The rewrite of grid.c factored all three through one helper that set
 * z to nought, which is right for two of them and wrong for the third.
 * That is what left the companions standing still, and nothing in the
 * suite could see it: grid.c had no test at all.
 *
 * So this is small and specific on purpose. It does not try to compare
 * the pathfinder against anything -- the copy it was rewritten from
 * cannot be run, because the decompiler's output reads locals that are
 * only assigned later in the file. It pins the one property the
 * rewrite got wrong, and the two it got right, so the three cannot be
 * folded together again.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../math.h"
#include "../gameobject.h"
#include "../grid.h"

/* ---- what grid.c reaches for ------------------------------------- */

static int gh_unreachable;
static void gh_Unreachable(const char* what)
{
    printf("FAIL: %s was called; no case here should reach it\n", what);
    gh_unreachable++;
}

char* BIG_pc_ReadFileTmp(uint32_t k, int a, int b)
{ (void)k; (void)a; (void)b; gh_Unreachable("BIG_pc_ReadFileTmp"); return NULL; }
void LOA_DeleteAddress(void* a) { (void)a; gh_Unreachable("LOA_DeleteAddress"); }
void LOA_MakeFileRef(void* a, void* b) { (void)a; (void)b; gh_Unreachable("LOA_MakeFileRef"); }
int  LOA_ReadChar(char** p, char* o, char m) { (void)p; (void)o; (void)m; gh_Unreachable("LOA_ReadChar"); return 0; }
int  LOA_ReadFloat(char** p, float* o, char m) { (void)p; (void)o; (void)m; gh_Unreachable("LOA_ReadFloat"); return 0; }
int  LOA_ReadLong(char** p, uint32_t* o, char m) { (void)p; (void)o; (void)m; gh_Unreachable("LOA_ReadLong"); return 0; }
int  LOA_ReadShort(char** p, uint16_t* o, char m) { (void)p; (void)o; (void)m; gh_Unreachable("LOA_ReadShort"); return 0; }
void* MEM_p_Alloc(unsigned int n) { return malloc(n ? n : 1); }
void  MEM_Free(void* p) { free(p); }

float MATH_DotProduct(Vec3* a, Vec3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void MATH_NormalizeVector(Vec3* dest, Vec3* src)
{
    double len = src->x * (double)src->x + src->y * (double)src->y
                 + src->z * (double)src->z;

    if (len > 0.0)
    {
        int i;
        float f;

        /* A square root by halving, so this file needs no libm and no
           table module: the cases below do not depend on the value. */
        double r = len;
        for (i = 0; i < 40; i++)
            r = (r + len / r) * 0.5;
        f = (float)r;
        dest->x = src->x / f;
        dest->y = src->y / f;
        dest->z = src->z / f;
    }
    else
    {
        dest->x = dest->y = dest->z = 0.0f;
    }
}

static Vec3 gh_sight;
void OBJ_SightGet(tGameObjectMain* gao, Vec3* sight) { (void)gao; *sight = gh_sight; }

/* ---- a grid to ask about ----------------------------------------- */

#define GH_ELEM   4
#define GH_WINDOW 7
#define GH_SPAN   (GH_ELEM * GH_WINDOW)
#define GH_ALLOC  64

static tWorldGridStruct gh_grid;
static char             gh_capas[GH_ALLOC * GH_ALLOC];
static tGridPtrElem     gh_cells[GH_ALLOC * GH_ALLOC];

static void gh_Build(void)
{
    memset(&gh_grid, 0, sizeof(gh_grid));
    memset(gh_capas, 0, sizeof(gh_capas));
    memset(gh_cells, 0, sizeof(gh_cells));
    gh_grid.capasPtr = gh_capas;
    gh_grid.ptrs = gh_cells;
    gh_grid.gridElemSize = GH_ELEM;
    gh_grid.gridCountX = GH_WINDOW;
    gh_grid.gridCountY = GH_WINDOW;
    gh_grid.s3Size7LimitMaxX = GH_WINDOW;
    gh_grid.s4Size7LimitMaxY = GH_WINDOW;
    gh_grid.iInSetCenter2 = GH_WINDOW;
    gh_grid.iInSetCenter4 = GH_WINDOW;
    GRID_gi_TestWall = 0x20;
    GRID_gi_Mode = 0;
}

static int gh_checks;
static int gh_failures;

static void gh_Float(float got, float want, const char* what)
{
    gh_checks++;
    if (got != want)
    {
        printf("FAIL: %s: got %g, wanted %g\n", what, (double)got, (double)want);
        gh_failures++;
    }
}

static void gh_Int(int got, int want, const char* what)
{
    gh_checks++;
    if (got != want)
    {
        printf("FAIL: %s: got %d, wanted %d\n", what, got, want);
        gh_failures++;
    }
}

int main(void)
{
    Vec3 pos;
    Vec3 path[8];
    Vec3 start, target;
    Mtx4x4 matrix;
    tGameObjectMain gao;
    int ret;

    printf("=== grid heights ===\n");

    memset(&gao, 0, sizeof(gao));
    memset(&matrix, 0, sizeof(matrix));
    gao.matrices = &matrix;
    gh_sight.x = 1.0f;

    /* ---- the destination keeps the height it came with ------------
       Both arms of it: the cell the position lands in is free, and the
       cell is a wall so a neighbour is taken instead. The height must
       survive either way -- it is the ground the object is standing
       on, and the function has no business having an opinion about
       it. */
    printf("-- GRI_i_ComputeDest leaves the height alone\n");

    gh_Build();
    pos.x = 10.4f; pos.y = 6.7f; pos.z = 123.5f;
    ret = GRI_i_ComputeDest(&gh_grid, &pos);
    gh_Int(ret, 1, "it answers that it found a cell");
    gh_Float(pos.z, 123.5f, "the height is untouched when the cell is free");
    gh_Int(pos.x == 10.4f, 0, "and x has moved to the cell's centre");

    gh_Build();
    gh_capas[6 * GH_SPAN + 10] = (char)0x20;      /* the cell it lands in */
    pos.x = 10.4f; pos.y = 6.7f; pos.z = -45.25f;
    ret = GRI_i_ComputeDest(&gh_grid, &pos);
    gh_Int(ret, 1, "it answers that it found a neighbour");
    gh_Float(pos.z, -45.25f, "the height is untouched when a neighbour is taken");

    /* A negative height, because nought is the value the defect left
       behind and a case that used nought could not tell the two
       apart. */
    gh_Build();
    pos.x = 2.5f; pos.y = 2.5f; pos.z = -1.0f;
    GRI_i_ComputeDest(&gh_grid, &pos);
    gh_Float(pos.z, -1.0f, "and a height below the water is kept too");

    /* ---- a path point's height is nought --------------------------
       The other side of the same coin: these two do decide the
       height, and a case that only checked the one above would pass
       with all three sites leaving z alone. */
    printf("-- a path point's height is nought\n");

    gh_Build();
    start.x = 4.5f; start.y = 4.5f; start.z = 77.0f;
    target = start;
    path[0].z = 999.0f;
    ret = GRID_ShortestPath(&gao, &gh_grid, &gh_grid, &start, &target,
                            path, 0, 4, 64.0f, GH_SPAN, 8);
    gh_Int(ret, 1, "a target in the start cell is one point");
    gh_Float(path[0].z, 0.0f, "whose height is nought, not the start's");

    gh_Build();
    start.x = 2.5f; start.y = 2.5f; start.z = 77.0f;
    target.x = 9.5f; target.y = 2.5f; target.z = 88.0f;
    path[0].z = 999.0f;
    path[1].z = 999.0f;
    ret = GRID_ShortestPath(&gao, &gh_grid, &gh_grid, &start, &target,
                            path, 0, 4, 64.0f, GH_SPAN, 8);
    gh_checks++;
    if (ret < 1)
    {
        printf("FAIL: a clear run across an empty grid found no path (%d)\n", ret);
        gh_failures++;
    }
    else
    {
        int i;

        for (i = 0; i < ret && i < 8; i++)
            gh_Float(path[i].z, 0.0f, "every point of a walked path is at nought");
    }

    printf("  %d checks, %d failure(s)\n", gh_checks, gh_failures);
    if (gh_failures || gh_unreachable)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
