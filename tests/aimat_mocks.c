/* Just enough of the engine for the AI material and colour builtins.
 *
 * The AI stack is the real subject here: the builtins take their
 * arguments off it and put their answers back, four bytes at a time,
 * and what goes through it is sometimes a colour and sometimes a
 * reference to something the engine owns. So this models it as a stack
 * rather than as a pair of hooks, and the driver pushes and pops the
 * way a compiled script would.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../textures.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geomaterial.h"

#include "aimat_mocks.h"

/* ------------------------------------------------------- the AI stack */

int AIMATMOCK_stack[AIMATMOCK_STACK];
int AIMATMOCK_stackNum;
int AIMATMOCK_underflows;

void AIMATMOCK_Push(int v)
{
    if (AIMATMOCK_stackNum < AIMATMOCK_STACK)
        AIMATMOCK_stack[AIMATMOCK_stackNum++] = v;
}

void AIMATMOCK_PushFloat(float f)
{
    int v;

    memcpy(&v, &f, 4);
    AIMATMOCK_Push(v);
}

int AI_GetIntFromStack(void)
{
    if (AIMATMOCK_stackNum <= 0)
    {
        ++AIMATMOCK_underflows;
        return 0;
    }
    return AIMATMOCK_stack[--AIMATMOCK_stackNum];
}

float AI_GetFloatFromStack(void)
{
    int v = AI_GetIntFromStack();
    float f;

    memcpy(&f, &v, 4);
    return f;
}

int AI_PutIntToStack(int v)
{
    AIMATMOCK_Push(v);
    return 0;
}

int AI_PutFloatToStack(float f)
{
    AIMATMOCK_PushFloat(f);
    return 0;
}

/* --------------------------------------------- the handle layer's edge */

/* The real one, so the driver exercises what the engine will: these
   two come from handle.c, which the runner links. */

/* ------------------------------------------------- the rest, recorded */

int AIMATMOCK_setMapGaoNum;
void* AIMATMOCK_setMapGao;
int AIMATMOCK_updateWithNum;
void* AIMATMOCK_updateWith;

void TEXPRO_Photo_SetMapGao(struct sTexProceduralStruct* pro,
                            struct sGameObjectMain* gao)
{
    (void)pro;
    ++AIMATMOCK_setMapGaoNum;
    AIMATMOCK_setMapGao = gao;
}

void TEX_Procedural_UpdateWithAnother(struct sTexProceduralStruct* a,
                                      struct sTexProceduralStruct* b)
{
    (void)a;
    ++AIMATMOCK_updateWithNum;
    AIMATMOCK_updateWith = b;
}

struct sTexProceduralStruct* AIMATMOCK_procedural;
struct sTexAnimatedElem* AIMATMOCK_anim;

struct sTexProceduralStruct* TEX_pst_Procedural_Get(int16_t texIdx)
{
    (void)texIdx;
    return AIMATMOCK_procedural;
}

struct sTexAnimatedElem* TEX_pst_Anim_Get(int16_t texIdx)
{
    (void)texIdx;
    return AIMATMOCK_anim;
}

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    (void)key;
    return -1;
}

int LOA_ul_SearchAddress(int pos)
{
    (void)pos;
    return 0;
}

void MAT_GetRotation(struct sGEOMAT* m, Vec3* v) { (void)m; (void)v; }
void MAT_GetScale(struct sGEOMAT* m, Vec3* v)    { (void)m; (void)v; }
void MAT_GetSpeed(struct sGEOMAT* m, Vec3* v)    { (void)m; (void)v; }
void MAT_SetPos(struct sGEOMAT* m, Vec3* v)      { (void)m; (void)v; }
void MAT_SetRotation(struct sGEOMAT* m, Vec3* v) { (void)m; (void)v; }
void MAT_SetScale(struct sGEOMAT* m, Vec3* v)    { (void)m; (void)v; }

void AIMATMOCK_Reset(void)
{
    AIMATMOCK_stackNum = 0;
    AIMATMOCK_underflows = 0;
    AIMATMOCK_setMapGaoNum = 0;
    AIMATMOCK_setMapGao = NULL;
    AIMATMOCK_updateWithNum = 0;
    AIMATMOCK_updateWith = NULL;
}
