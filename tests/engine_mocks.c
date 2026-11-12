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

/* --- protexmodifier ---------------------------------------------- */

int MOCK_vuisIdentity;
tMatDecompressedStruct MOCK_uv;

void* MEM_p_AllocAligned(int size, int alignment)
{
    MOCK_Record("MEM_p_AllocAligned", 0, 2, (double)size, (double)alignment);
    return malloc(size);
}

void MEM_FreeAlign(void* ptr)
{
    MOCK_Record("MEM_FreeAlign", ptr, 0);
    free(ptr);
}

void MATH_CrossProduct(Vec3* dest, Vec3* src1, Vec3* src2)
{
    dest->x = src1->y * src2->z - src1->z * src2->y;
    dest->y = src1->z * src2->x - src1->x * src2->z;
    dest->z = src1->x * src2->y - src1->y * src2->x;
}

void MATH_VecMultScalar(Vec3* dest, Vec3* src, float scalar)
{
    dest->x = src->x * scalar;
    dest->y = src->y * scalar;
    dest->z = src->z * scalar;
}

void MATH_AddScaleVector(Vec3* dest, Vec3* src1, Vec3* src2, float scale)
{
    dest->x = src2->x * scale + src1->x;
    dest->y = src2->y * scale + src1->y;
    dest->z = src2->z * scale + src1->z;
}

void MATH_TransformVertex(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    Vec3 v;

    v.x = mtx->v1.x * src->x + mtx->v2.x * src->y + mtx->v3.x * src->z + mtx->Pos.x;
    v.y = mtx->v1.y * src->x + mtx->v2.y * src->y + mtx->v3.y * src->z + mtx->Pos.y;
    v.z = mtx->v1.z * src->x + mtx->v2.z * src->y + mtx->v3.z * src->z + mtx->Pos.z;
    *dest = v;
}

void MATH_InvertMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    Mtx4x4 m;

    m = *src;
    dest->v1.x = m.v1.x;  dest->v1.y = m.v2.x;  dest->v1.z = m.v3.x;
    dest->v2.x = m.v1.y;  dest->v2.y = m.v2.y;  dest->v2.z = m.v3.y;
    dest->v3.x = m.v1.z;  dest->v3.y = m.v2.z;  dest->v3.z = m.v3.z;
    dest->Pos.x = -m.Pos.x;
    dest->Pos.y = -m.Pos.y;
    dest->Pos.z = -m.Pos.z;
    dest->mtxflag = m.mtxflag;
    dest->w = 1.0f;
}

void MATH_MakeOGLMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    if (dest != src)
        *dest = *src;
    dest->w = 1.0f;
    MOCK_Record("MATH_MakeOGLMatrix", src, 0);
}

void MATH_MulMatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2)
{
    dest->v1.x = src1->v1.x + src2->v1.x;
    dest->v1.y = src1->v1.y - src2->v2.y;
    dest->v1.z = src1->v1.z * 0.5f;
    dest->v2.x = src1->v2.x - src2->v1.x;
    dest->v2.y = src1->v2.y + src2->v2.y;
    dest->v2.z = src1->v2.z * 0.25f;
    dest->v3.x = src1->v3.x + src2->v3.x;
    dest->v3.y = src1->v3.y - src2->v3.y;
    dest->v3.z = src1->v3.z + 1.0f;
    dest->Pos = src1->Pos;
    dest->mtxflag = src1->mtxflag | src2->mtxflag;
    dest->w = 1.0f;
    MOCK_Record("MATH_MulMatrixMatrix", src1, 0);
}

BOOL MAT_VUISIdentity(struct sMultiTextureElemStruct* elem)
{
    MOCK_Record("MAT_VUISIdentity", elem, 0);
    return MOCK_vuisIdentity;
}

void MAT_UVDecompress(struct sMultiTextureElemStruct* elem, struct sMatDecompressedStruct* out)
{
    MOCK_Record("MAT_UVDecompress", elem, 0);
    *out = MOCK_uv;
}

void MAT_UV_COMPUTE_GetMAtrix(struct sGdi_DisplayData* dd, struct sGameObjectMain* gao,
                              unsigned int param, Mtx4x4* out)
{
    float k = (float)(param & 0xff) * 0.0625f;

    MOCK_Record("MAT_UV_COMPUTE_GetMAtrix", gao, 1, (double)param);
    out->v1.x = 1.0f + k;  out->v1.y = 0.25f * k;  out->v1.z = -0.5f;
    out->v2.x = -0.125f;   out->v2.y = 2.0f - k;   out->v2.z = 0.75f * k;
    out->v3.x = 0.5f * k;  out->v3.y = -1.5f;      out->v3.z = 1.0f + 0.5f * k;
    out->Pos.x = k;        out->Pos.y = -k;        out->Pos.z = 2.0f * k;
    out->mtxflag = 2;
    out->w = 1.0f;
}

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    MOCK_Record("OBJ_pst_GetAbsoluteMatrix", gao, 0);
    return gao->matrices;
}

int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    out->QuadPart = 0;
    return 1;
}

void PRO_QPC_RecordTimeDecRef(int id)
{
    MOCK_Record("PRO_QPC_RecordTimeDecRef", 0, 1, (double)id);
}

void LOA_ReadCharArray(char** offset, char* dest, int len, char readMode)
{
    int i;

    for (i = 0; i < readMode; i++)
    {
        memcpy(dest, *offset, (size_t)len);
        *offset += len;
    }
    MOCK_Record("LOA_ReadCharArray", 0, 2, (double)len, (double)readMode);
}
