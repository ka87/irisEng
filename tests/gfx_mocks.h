#pragma once
#include "../gfx.h"
#include "../gdi.h"
#include "../time.h"
#include "../soft.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/gro_common.h"

/* The gfx differential tests link the real math.c, so nothing in here
   stands in for a MATH_ function. What it does provide is the engine
   around a gfx effect: the shared geometry buffer, the allocator, the
   colour helpers and the display-data globals, plus a record of every
   call an effect made and a snapshot of the geometry at each render. */

#define GFXMOCK_LOG_MAX 65536
#define GFXMOCK_CALL_ARGS 8

typedef struct sGfxMockCall
{
    const char* fn;
    int nargs;
    float arg[GFXMOCK_CALL_ARGS];
} tGfxMockCall;

#define GFXMOCK_CAP_MAX 64
#define GFXMOCK_CAP_CHUNKS 8

typedef struct sGfxCapture
{
    unsigned int vertexNum;
    unsigned int uvNum;
    unsigned int chunkCount;
    unsigned int spritesNum;
    unsigned int drawMask;
    int hasColors;
    unsigned int triangleNum[GFXMOCK_CAP_CHUNKS];
    int materialNum[GFXMOCK_CAP_CHUNKS];
    unsigned int triangleTotal;
    Vec3* verts;
    tGeoUVElem* uvs;
    tGeoTriangleChunkElem* tris;
    unsigned int* colors;
} tGfxCapture;

extern tGfxMockCall GFXMOCK_log[GFXMOCK_LOG_MAX];
extern int GFXMOCK_logLen;
extern int GFXMOCK_logOverflow;

extern tGfxCapture GFXMOCK_caps[GFXMOCK_CAP_MAX];
extern int GFXMOCK_capCount;

/* Once, before any case: builds the trig and reciprocal tables, the
   geometry buffer, the display data and the gfx element the effects
   read through. */
void GFXMOCK_Init(void);

/* Between the two sides of a case: empties the log and the captures,
   frees what the allocator handed out and puts the geometry buffer
   back to its just-started state, so neither side can see the other's
   leftovers. */
void GFXMOCK_Reset(void);

void GFXMOCK_Record(const char* fn, int nargs, ...);

/* The gfx element whose aiGameObject->distance several effects test,
   and the camera the projection helpers read. */
extern tGfxElem* GFXMOCK_elem;
void GFXMOCK_SetDistance(int distance);
void GFXMOCK_SetCameraPos(float x, float y, float z);
void GFXMOCK_SetCameraBasis(float* v1, float* v2, float* v3);

/* ------------------------------------------------------------------ */
/* Comparison. A case runs the frozen copy, calls GFXMOCK_SaveSide(0), */
/* resets, runs the rewrite, calls GFXMOCK_SaveSide(1), and then       */
/* GFXMOCK_CompareSides, which reports the call sequence and every     */
/* captured geometry buffer. Anything the effect keeps in its own      */
/* state the driver compares itself, with GFXMOCK_Close.               */
/* ------------------------------------------------------------------ */

void GFXMOCK_SetCase(const char* name);
void GFXMOCK_SaveSide(int side);
int GFXMOCK_CompareSides(void);

int GFXMOCK_Close(float base, float New, const char* what, int idx);
int GFXMOCK_CloseVec(Vec3* base, Vec3* New, const char* what);
int GFXMOCK_SameInt(int base, int New, const char* what, int idx);

/* Set while comparing an effect whose frozen copy writes its vertex
   colours at the wrong indices, so the arrays cannot be compared; the
   driver checks the rewritten side against what the geometry wants
   instead, and says so. */
extern int GFXMOCK_ignoreColors;

extern int GFXMOCK_fails;
extern int GFXMOCK_cases;
extern int GFXMOCK_diffCount;
void GFXMOCK_Summary(const char* title);
