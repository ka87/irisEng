#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine_mocks.h"

tMockCall MOCK_log[MOCK_LOG_MAX];
int MOCK_logLen;
int MOCK_overflow;

static tGameObjectMain** s_gaos;
static int s_gaoNum;
static tGameObjectMain** s_byRank;
static int s_byRankNum;
static void* s_curGroup;
static const unsigned char* s_stream;
static int s_streamLen;

void MOCK_Reset(void)
{
    MOCK_logLen = 0;
    MOCK_overflow = 0;
    memset(MOCK_log, 0, sizeof(MOCK_log));
}

void MOCK_RegisterGaos(tGameObjectMain** gaos, int num)
{
    s_gaos = gaos;
    s_gaoNum = num;
}

void MOCK_SetGroupRanks(tGameObjectMain** byRank, int num)
{
    s_byRank = byRank;
    s_byRankNum = num;
}

void MOCK_SetCurGroup(void* grp)
{
    s_curGroup = grp;
}

void MOCK_SetLoadStream(const unsigned char* bytes, int len)
{
    s_stream = bytes;
    s_streamLen = len;
}

int MOCK_GaoIndex(const void* gao)
{
    int i;

    if (!gao)
        return -1;
    for (i = 0; i < s_gaoNum; i++)
    {
        if (s_gaos[i] == gao)
            return i;
    }
    return -2;
}

void MOCK_Record(const char* fn, const void* gao, int nargs, ...)
{
    va_list ap;
    tMockCall* c;
    int i;

    if (MOCK_logLen >= MOCK_LOG_MAX)
    {
        MOCK_overflow = 1;
        return;
    }
    c = &MOCK_log[MOCK_logLen++];
    c->fn = fn;
    c->gao = MOCK_GaoIndex(gao);
    c->nargs = nargs;
    va_start(ap, nargs);
    for (i = 0; i < nargs && i < MOCK_CALL_ARGS; i++)
        c->arg[i] = (float)va_arg(ap, double);
    va_end(ap);
}

/* ------------------------------------------------------------------ */
/* Faithful copies of math.c, because the pre-refactor code spells     */
/* these out inline and the rewritten code calls them. If they are not */
/* identical the comparison is meaningless.                            */
/* ------------------------------------------------------------------ */

float MATH_VecLenSquared(Vec3* vec)
{
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

float MATH_VecLen(Vec3* vec)
{
    return (float)sqrt(MATH_VecLenSquared(vec));
}

void MATH_VecInvert(Vec3* dest, Vec3* src)
{
    dest->x = -src->x;
    dest->y = -src->y;
    dest->z = -src->z;
}

void MATH_NormalizeVector(Vec3* dest, Vec3* src)
{
    float normLen;

    normLen = 1.0f / (float)sqrt(src->z * src->z + src->y * src->y + src->x * src->x);
    dest->x = normLen * src->x;
    dest->y = normLen * src->y;
    dest->z = normLen * src->z;
}

void MATH_NormalizeEqualVector(Vec3* v)
{
    long double len;

    len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x = (float)(1.0 / len * v->x);
    v->y = (float)(1.0 / len * v->y);
    v->z = (float)(1.0 / len * v->z);
}

void MATH_VecMultScalarWithoutBuffer(Vec3* vec, float scalar)
{
    vec->x = vec->x * scalar;
    vec->y = vec->y * scalar;
    vec->z = vec->z * scalar;
}

void MATH_SetNormVector(Vec3* dest, Vec3* src, float norm)
{
    MATH_NormalizeVector(dest, src);
    MATH_VecMultScalarWithoutBuffer(dest, norm);
}

float MATH_DistanceOfTwoPoints(Vec3* v1, Vec3* v2)
{
    double xDif = v1->x - v2->x;
    double yDif = v1->y - v2->y;
    double zDif = v1->z - v2->z;

    return (float)sqrt(xDif * xDif + yDif * yDif + zDif * zDif);
}

void MATH_GetScale(Vec3* scale, Mtx4x4* mtx)
{
    if ((mtx->mtxflag & 0x8) != 0)
    {
        scale->x = mtx->_m14;
        scale->y = mtx->_m24;
        scale->z = mtx->_m34;
    }
    else
    {
        scale->x = 1.0f;
        scale->y = 1.0f;
        scale->z = 1.0f;
    }
    MOCK_Record("MATH_GetScale", 0, 3,
                (double)scale->x, (double)scale->y, (double)scale->z);
}

void MATH_SetScale(Mtx4x4* mtx, Vec3* scale)
{
    mtx->_m14 = scale->x;
    mtx->_m24 = scale->y;
    mtx->_m34 = scale->z;
    mtx->mtxflag |= 0x8;
    MOCK_Record("MATH_SetScale", 0, 3,
                (double)scale->x, (double)scale->y, (double)scale->z);
}

/* ------------------------------------------------------------------ */
/* Deterministic stand-ins. Both copies call these the same way, so    */
/* they only have to be reproducible, not real.                        */
/* ------------------------------------------------------------------ */

void AI_EvalFunc_MATHVecBlendRotate_C(Vec3* from, Vec3* to, float t, Vec3* out)
{
    out->x = from->x + (to->x - from->x) * t;
    out->y = from->y + (to->y - from->y) * t;
    out->z = from->z + (to->z - from->z) * t;
    MOCK_Record("VecBlendRotate", 0, 7,
                (double)from->x, (double)from->y, (double)from->z,
                (double)to->x, (double)to->y, (double)to->z, (double)t);
}

void OBJ_ComputeGlobalWithLocal(struct sGameObjectMain* gao, Mtx4x4* mtx, int mode)
{
    MOCK_Record("OBJ_ComputeGlobalWithLocal", gao, 1, (double)mode);
}

void OBJ_SightGeneralSet(struct sGameObjectMain* gao, Vec3* a, Vec3* b)
{
    MOCK_Record("OBJ_SightGeneralSet", gao, 6,
                (double)a->x, (double)a->y, (double)a->z,
                (double)b->x, (double)b->y, (double)b->z);
}

void OBJ_BankingGeneralSet(struct sGameObjectMain* gao, Vec3* a, Vec3* b)
{
    MOCK_Record("OBJ_BankingGeneralSet", gao, 6,
                (double)a->x, (double)a->y, (double)a->z,
                (double)b->x, (double)b->y, (double)b->z);
}

void OBJMY_ComputeLocalWhenHie(struct sGameObjectMain* gao)
{
    MOCK_Record("OBJMY_ComputeLocalWhenHie", gao, 0);
}

void OBJ_RestoreInitialPos(struct sGameObjectMain* gao)
{
    MOCK_Record("OBJ_RestoreInitialPos", gao, 0);
}

struct sGameObjectMain* OBJ_pst_GroupGetByRank(struct sGroup* grp, int idx)
{
    tGameObjectMain* r;

    r = (idx >= 0 && idx < s_byRankNum) ? s_byRank[idx] : NULL;
    MOCK_Record("OBJ_pst_GroupGetByRank", r, 1, (double)idx);
    return (struct sGameObjectMain*)r;
}

struct sGroup* MDF_Modifier_GetCurGroup(struct sModifier* mdf)
{
    MOCK_Record("MDF_Modifier_GetCurGroup", 0, 0);
    return (struct sGroup*)s_curGroup;
}

void* MEM_p_Alloc(unsigned int size)
{
    return calloc(1, size);
}

void MEM_Free(void* ptr)
{
    free(ptr);
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int v = 0;
    int i;

    for (i = 0; i < readMode; i++)
    {
        v = 0;
        memcpy(&v, *offset, 4);
        *offset += 4;
    }
    MOCK_Record("LOA_ReadLong", 0, 1, (double)readMode);
    return v;
}

float LOA_ReadFloat(char** offset, float* a2, char readMode)
{
    float v = 0.0f;
    int i;

    for (i = 0; i < readMode; i++)
    {
        memcpy(&v, *offset, 4);
        *offset += 4;
    }
    MOCK_Record("LOA_ReadFloat", 0, 1, (double)readMode);
    return v;
}
