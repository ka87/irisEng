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

/* --- lazymodifier: verbatim copies of the engine helpers ---------- */

void MATH_SubstractVec(Vec3* dest, Vec3* v1, Vec3* v2)
{
    dest->x = v1->x - v2->x;
    dest->y = v1->y - v2->y;
    dest->z = v1->z - v2->z;
}

void MATH_VecAddVec(Vec3* dest, Vec3* src1, Vec3* src2)
{
    dest->x = src1->x + src2->x;
    dest->y = src1->y + src2->y;
    dest->z = src1->z + src2->z;
}

void MATH_VecAddVecWithoutBuffer(Vec3* a1, Vec3* a2)
{
    a1->x = a1->x + a2->x;
    a1->y = a2->y + a1->y;
    a1->z = a2->z + a1->z;
}

float MATH_DotProduct(Vec3* v1, Vec3* v2)
{
    return v1->z * v2->z + v1->y * v2->y + v1->x * v2->x;
}

void MATH_CopyMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    if (dest != src)
        memcpy(dest, src, sizeof(Mtx4x4));
}

void MATH_Copy33Matrix(Mtx4x4* dest, Mtx4x4* src)
{
    dest->v1 = src->v1;
    dest->v2 = src->v2;
    dest->v3 = src->v3;
    if ((src->mtxflag & 0x4) != 0)
        dest->mtxflag |= 0x4;
}

void MATH_Orthonormalize(Mtx4x4* mtx)
{    
    double v3;
    double v8; 
    double v13;

    v3 = sqrt(mtx->v1.x * mtx->v1.x + mtx->v1.y * mtx->v1.y + mtx->v1.z * mtx->v1.z);
    mtx->v1.x = (float)(1.0 / v3 * mtx->v1.x);
    mtx->v1.y = (float)(1.0 / v3 * mtx->v1.y);
    mtx->v1.z = (float)(1.0 / v3 * mtx->v1.z);
    mtx->v2.x = mtx->v1.z * mtx->v3.y - mtx->v1.y * mtx->v3.z;
    mtx->v2.y = mtx->v1.x * mtx->v3.z - mtx->v1.z * mtx->v3.x;
    mtx->v2.z = mtx->v3.x * mtx->v1.y - mtx->v3.y * mtx->v1.x;
    v8 = sqrt(mtx->v2.z * mtx->v2.z + mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y);
    mtx->v2.x = (float)(1.0 / v8 * mtx->v2.x);
    mtx->v2.y = (float)(1.0 / v8 * mtx->v2.y);
    mtx->v2.z = (float)(1.0 / v8 * mtx->v2.z);
    mtx->v3.x = mtx->v2.z * mtx->v1.y - mtx->v2.y * mtx->v1.z;
    mtx->v3.y = mtx->v1.z * mtx->v2.x - mtx->v2.z * mtx->v1.x;
    mtx->v3.z = mtx->v1.x * mtx->v2.y - mtx->v2.x * mtx->v1.y;
    v13 = sqrt(mtx->v3.z * mtx->v3.z + mtx->v3.x * mtx->v3.x + mtx->v3.y * mtx->v3.y);
    mtx->v3.x = (float)(1.0 / v13 * mtx->v3.x);
    mtx->v3.y = (float)(1.0 / v13 * mtx->v3.y);
    mtx->v3.z = (float)(1.0 / v13 * mtx->v3.z);
}

void MATH_ConvertQuaternionToMatrix(Mtx4x4* mtx, Quat4* q)
{
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    double v16;
    double v17;
    float v18; 
    float v19; 
    float a2a; 

    v3 = q->q1 * q->q1;
    v4 = 0.5 - (v3 + v3);
    v5 = q->q2 * q->q1;
    v6 = v5 + v5;
    v7 = q->q3 * q->q1;
    v8 = v7 + v7;
    v9 = q->q4 * q->q1;
    v18 = (float)(v9 + v9);
    v10 = q->q2 * q->q2;
    v19 = (float)(0.5 - (v10 + v10));
    v11 = q->q3 * q->q2;
    a2a = (float)(v11 + v11);
    v12 = q->q4 * q->q2;
    v13 = v12 + v12;
    v14 = q->q3 * q->q3;
    v15 = 0.5 - (v14 + v14);
    v16 = q->q4 * q->q3;
    mtx->_m14 = 0.0;
    mtx->_m24 = 0.0;
    v17 = v16 + v16;
    mtx->_m34 = 0.0;
    mtx->mtxflag = mtx->mtxflag & ~8u | 4;
    mtx->v1.x = (float)(v15 + v19);
    mtx->v2.x = (float)(v6 - v17);
    mtx->v3.x = (float)(v13 + v8);
    mtx->v1.y = (float)(v17 + v6);
    mtx->v2.y = (float)(v15 + v4);
    mtx->v3.y = a2a - v18;
    mtx->v1.z = (float)(v8 - v13);
    mtx->v2.z = a2a + v18;
    mtx->v3.z = (float)(v19 + v4);
}

void MATH_Mul33MatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2, char flagSet)
{
    if ((src1->mtxflag & 0x3c) != 0)
    {
        if ((src2->mtxflag & 0x3c) != 0)
        {
            dest->v1.x = src1->v1.x * src2->v1.x + src1->v1.y * src2->v2.x + src2->v3.x * src1->v1.z;
            dest->v1.y = src2->v3.y * src1->v1.z + src2->v1.y * src1->v1.x + src2->v2.y * src1->v1.y;
            dest->v1.z = src1->v1.x * src2->v1.z + src1->v1.y * src2->v2.z + src1->v1.z * src2->v3.z;
            dest->v2.x = src1->v2.z * src2->v3.x + src1->v2.y * src2->v2.x + src1->v2.x * src2->v1.x;
            dest->v2.y = src1->v2.y * src2->v2.y + src1->v2.x * src2->v1.y + src1->v2.z * src2->v3.y;
            dest->v2.z = src1->v2.y * src2->v2.z + src1->v2.x * src2->v1.z + src1->v2.z * src2->v3.z;
            dest->v3.x = src2->v2.x * src1->v3.y + src2->v1.x * src1->v3.x + src2->v3.x * src1->v3.z;
            dest->v3.y = src2->v2.y * src1->v3.y + src2->v1.y * src1->v3.x + src2->v3.y * src1->v3.z;
            dest->v3.z = src1->v3.z * src2->v3.z + src1->v3.y * src2->v2.z + src1->v3.x * src2->v1.z;
            if (flagSet)
            {
                if ((src1->mtxflag & 0x4) != 0 || (src2->mtxflag & 0x4) != 0)
                    dest->mtxflag |= 0x4;
            }
        }
        else
        {
            MATH_Copy33Matrix(dest, src1);
        }
    }
    else
    {
        MATH_Copy33Matrix(dest, src2);
    }
}

void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* gao)
{
    MOCK_Record("OBJ_ComputeGlobalWhenHie", gao, 0);
}

void CAM_Inverse(tDDContainerStruct* ct)
{
    MOCK_Record("CAM_Inverse", ct, 0);
}

/* --- xmenmodifier ------------------------------------------------ */

tXmenCapture MOCK_xmenCaptures[MOCK_XMEN_CAPTURES];
int MOCK_xmenCaptureCount;

void GRO_Struct_Init(tGroGeneric* gro, int kind)
{
    MOCK_Record("GRO_Struct_Init", gro, 1, (double)kind);
}

struct sGroGeneric* GEO_ul_Load_ObjectCallback(uint32_t offset)
{
    MOCK_Record("GEO_ul_Load_ObjectCallback", 0, 1, (double)offset);
    return 0;
}

void DBG_OutputDebugString(char* format, ...)
{
    MOCK_Record("DBG_OutputDebugString", 0, 0);
}

void SOFT_MatrixStack_Reset(tMatrixStackStruct* stack, Mtx4x4* mtx)
{
    MOCK_Record("SOFT_MatrixStack_Reset", mtx, 0);
    stack->actMatrixElem = mtx;
}

int SOFT_l_MatrixStack_Push(tMatrixStackStruct* stack, Mtx4x4* mtx)
{
    MOCK_Record("SOFT_l_MatrixStack_Push", mtx, 0);
    stack->actMatrixElem = mtx;
    return 0;
}

void MATH_SetIdentityMatrix(Mtx4x4* mtx)
{
    memset(mtx, 0, sizeof(Mtx4x4));
    mtx->v1.x = 1.0f;
    mtx->v2.y = 1.0f;
    mtx->v3.z = 1.0f;
    mtx->w = 1.0f;
}

void MATH_TransformVector(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    Vec3 v;

    v.x = mtx->v1.x * src->x + mtx->v2.x * src->y + mtx->v3.x * src->z;
    v.y = mtx->v1.y * src->x + mtx->v2.y * src->y + mtx->v3.y * src->z;
    v.z = mtx->v1.z * src->x + mtx->v2.z * src->y + mtx->v3.z * src->z;
    *dest = v;
}

void MATH_BlendVector(Vec3* dest, Vec3* v1, Vec3* v2, float amount)
{
    if (amount < 0.0f)
        amount = 0.0f;
    if (amount > 1.0f)
        amount = 1.0f;
    dest->x = (1.0f - amount) * v1->x + amount * v2->x;
    dest->y = (1.0f - amount) * v1->y + amount * v2->y;
    dest->z = (1.0f - amount) * v1->z + amount * v2->z;
}

void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags)
{
    MOCK_Record("LOA_MakeFileRef", retVal, 2, (double)key, (double)flags);
}

static void MOCK_l_XmenCapture(struct sGEOGRO* geogro, struct sGeoTriangleChunks* chunks,
                               int transparent)
{
    tXmenCapture* cap;
    int n;
    int i;

    MOCK_Record(transparent ? "GAO_XMN_Render_TransparentTriangles"
                            : "MAT_DrawIndexedTriangle",
                0, 2, (double)geogro->vertexNum,
                (double)(chunks ? chunks->triangleNum : 0));

    if (MOCK_xmenCaptureCount >= MOCK_XMEN_CAPTURES)
        return;
    cap = &MOCK_xmenCaptures[MOCK_xmenCaptureCount++];
    memset(cap, 0, sizeof(*cap));
    cap->transparent = transparent;
    cap->vertexNum = geogro->vertexNum;
    cap->uvNum = geogro->uvNum;
    cap->triangleNum = chunks ? chunks->triangleNum : XMEN_ELEMENT[0].triangleNum;
    cap->materialNum = XMEN_ELEMENT[0].materialNum;

    memcpy(cap->tris, XMEN_OBJECT_TRIS, sizeof(cap->tris));
    memcpy(cap->uvs, XMEN_UVs, sizeof(cap->uvs));
    n = geogro->vertexNum;
    if (n > 64)
        n = 64;
    for (i = 0; i < n; i++)
        cap->verts[i] = geogro->vertices[i];
}

void MAT_DrawIndexedTriangle(struct sGEOGRO* geogro, struct sGEOMAT* geomat,
                             struct sGeoTriangleChunks* chunks)
{
    MOCK_l_XmenCapture(geogro, chunks, 0);
}

void GAO_XMN_Render_TransparentTriangles(struct sGEOGRO* geogro, struct sGEOMAT* geomat)
{
    MOCK_l_XmenCapture(geogro, 0, 1);
}

BOOL MATH_b_NulVector(Vec3* v)
{
    if (fabs(v->x) != 0.0f || fabs(v->y) != 0.0f || fabs(v->z) != 0.0f) return FALSE;
    return TRUE;
}

void MATH_NormalizeVectorWithNullVectorCheck(Vec3* dest, Vec3* src)
{    
    if (!MATH_b_NulVector(src))
    {
        MATH_NormalizeVector(dest, src);
    }
    else
    {
        dest->x = 0.0f;
        dest->y = 0.0f;
        dest->z = 0.0f;
    }
}
