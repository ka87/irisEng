#pragma once
#include "../particlegenerator.h"
#include "../gdi.h"
#include "../gfx.h"
#include "../time.h"
#include "../engine.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/gro_common.h"
#include "../modifiers.h"
#include "../modifiers/stpmodifier.h"

/* The engine around a particle generator: the two object lookups, the
   shared geometry buffer, the display data and the pseudo-random
   sequence, plus a record of every call a side made. The real math.c
   is linked rather than mocked, so the trig tables, the vector
   helpers and MATH_RandFloat are the engine's own. */

#define PMOCK_LOG_MAX 400000
#define PMOCK_ARGS 4

typedef struct sPMockCall
{
    const char* fn;
    int ni;
    int nf;
    int ia[PMOCK_ARGS];
    float fa[PMOCK_ARGS];
} tPMockCall;

#define PMOCK_VERTS 8192
#define PMOCK_UVS 64
#define PMOCK_TRIS 8192
#define PMOCK_SPRITES 4096

extern tPMockCall PMOCK_log[PMOCK_LOG_MAX];
extern int PMOCK_logLen;
extern int PMOCK_logOverflow;

/* The geometry the last Render saw, taken once per side. */
typedef struct sPMockCapture
{
    int taken;
    unsigned int vertexNum;
    unsigned int uvNum;
    unsigned int spritesNum;
    unsigned int drawMask;
    int materialNum;
    unsigned int triangleNum;
    Vec3 verts[PMOCK_VERTS];
    tGeoUVElem uvs[PMOCK_UVS];
    tGeoTriangleChunkElem tris[PMOCK_TRIS];
    int colors[PMOCK_VERTS + 1];
    tGeoSpriteElem sprites[PMOCK_SPRITES];
} tPMockCapture;

extern tPMockCapture PMOCK_capture;

/* The object tree the two generators walk. */
extern tGameObjectMain PMOCK_gao;
extern tGameObjectMain PMOCK_genGao;
extern tGameObjectMain PMOCK_shapeGao;
extern Mtx4x4 PMOCK_gaoMtx;
extern Mtx4x4 PMOCK_genMtx;
extern Mtx4x4 PMOCK_shapeMtx;
extern tGEOGRO PMOCK_gro;

/* What OBJ_p_GetGro and MDF_pst_GetByType hand back; set per case. */
extern tGEOGRO* PMOCK_groForGao1;
extern int PMOCK_groId;
extern int PMOCK_haveStp;
extern int PMOCK_stpHasStp;

void PMOCK_Init(void);
void PMOCK_Reset(void);
void PMOCK_Seed(unsigned int seed);

/* Comparison helpers. Each returns 1 when the two values differ past
   the tolerance and records the worst gap seen so far. */
int PMOCK_SameInt(int base, int New, const char* what, const char* where);
int PMOCK_CloseFloat(float base, float New, const char* what, const char* where);
int PMOCK_CompareLog(const tPMockCall* a, int alen, const tPMockCall* b, int blen,
                     const char* where);

void PMOCK_SaveLog(tPMockCall* dest, int* destLen);
void PMOCK_SaveCapture(tPMockCapture* dest);

void PMOCK_SetCase(const char* name);
void PMOCK_Summary(const char* title);
extern int PMOCK_cases;
extern int PMOCK_failures;
extern long PMOCK_compares;
extern long PMOCK_floatsDiffering;
extern double PMOCK_worstAbs;
extern double PMOCK_worstRel;
extern double PMOCK_worstRelAbs;
extern float PMOCK_tolAbs;
extern float PMOCK_tolRel;
