#pragma once
#include <stddef.h>
#include "../common.h"
#include "../math.h"
#include "../gameobject.h"
#include "../tables.h"
#include "../world.h"
#include "../snp.h"

/* The engine around the sweep-and-prune: an arena allocator whose
   blocks land at the same addresses for both sides, the two game-object
   helpers snp.c calls, the table validity test, and a record of every
   call a side made.

   The arena is never handed anything back -- MEM_Free only logs -- so
   the first side's state stays readable while the second runs, and the
   two sides are separated by rewinding the arena rather than by
   freeing. Every block sits 64 bytes into a larger one and is followed
   by 64 more, because the engine's own element arithmetic assumes a
   32-bit build (see run_snp.sh) and a short copy must not land in
   another block. */

#define SNPMOCK_LOG_MAX  600000
#define SNPMOCK_ARENA    (6 * 1024 * 1024)
#define SNPMOCK_OBJECTS  16
#define SNPMOCK_DLIST    200

typedef struct sSnpMockCall
{
    const char* fn;
    int ni;
    int nf;
    int ia[4];
    float fa[4];
} tSnpMockCall;

extern tSnpMockCall SNPMOCK_log[SNPMOCK_LOG_MAX];
extern int SNPMOCK_logLen;
extern int SNPMOCK_logOverflow;

extern unsigned char SNPMOCK_arena[SNPMOCK_ARENA];
extern size_t SNPMOCK_arenaUsed;

/* The scene both sides run over. One copy, rebuilt from the same seed
   before each side runs, so every pointer either side stores can be
   compared directly. */
typedef struct sSnpScene
{
    tWorld world;
    tGameObjectMain gaos[SNPMOCK_OBJECTS];
    tGameObjectBV bvs[SNPMOCK_OBJECTS];
    Mtx4x4 mtx[SNPMOCK_OBJECTS];
    tGameObjectExtended ext[SNPMOCK_OBJECTS];
    tGameObjectCollMain coll[SNPMOCK_OBJECTS];
    tGameObjectCollDetectionList dlist[SNPMOCK_OBJECTS];
    tGameObjectMain* dgaos[SNPMOCK_OBJECTS][SNPMOCK_DLIST];
    tableElemStructPF table[SNPMOCK_OBJECTS];
    tableDescStruct tableDesc;
    int isBox[SNPMOCK_OBJECTS];
    int invalid[SNPMOCK_OBJECTS];
    Vec3 basePos[SNPMOCK_OBJECTS];
} tSnpScene;

extern tSnpScene SNPMOCK_scene;

/* Scratch the driver uses for the small functions that take raw
   arrays rather than a world. */
typedef struct sSnpScratch
{
    int flags[4096];
    tSnp snp;
    tSnpAxis axis[3];
    tSnpAxisElem elems[3][2048];
    tSnpObjectsToRemoveQueue queue;
    int outBit;
    int outWord;
    int ret;
    int16_t ret16;
} tSnpScratch;

extern tSnpScratch SNPMOCK_scratch;

void SNPMOCK_Init(void);
void SNPMOCK_Reset(void);                       /* arena + log, not the scene */
void SNPMOCK_BuildScene(unsigned int seed, int pattern);
void SNPMOCK_FillAxisTails(tSnp* snp);
void SNPMOCK_MoveObjects(unsigned int seed, float amount);

/* Replace the scene with boxes at the origin whose axis keys form a
   chain, each one the float nearest the next plus `step`. That puts the
   sort's epsilon and the comparison inside its sink loop on their exact
   boundaries, which randomly placed objects never reach. */
void SNPMOCK_ChainKeys(double step, float extent, int roundUp);
void SNPMOCK_TouchingKeys(float base, float extent);

/* snp.c's two game-object calls and its one math call, wrapped so the
   sequence can be compared. The real math.c is linked, so the wrapper
   delegates rather than reimplementing. */
Mtx4x4* SNPMOCK_GetAbsoluteMatrix(tGameObjectMain* gao);
BOOL SNPMOCK_BV_IsAABBox(tGameObjectBV* bv);
BOOL SNPMOCK_TableElemIsInvalid(void* elem);
void SNPMOCK_TransformVertexNoScale(Vec3* dest, Mtx4x4* mtx, Vec3* src);

int SNPMOCK_ObjectIndex(const tGameObjectMain* gao);

/* The snapshot one side leaves behind. */
void SNPMOCK_Save(void);
int SNPMOCK_CompareWithSaved(const char* where);

void SNPMOCK_SetCase(const char* name);
void SNPMOCK_Summary(const char* title);
int SNPMOCK_SameInt(int base, int New, const char* what, const char* where);
int SNPMOCK_SameLong(long long base, long long New, const char* what, const char* where);
int SNPMOCK_SameFloat(float base, float New, const char* what, const char* where);
int SNPMOCK_SameBytes(const void* base, const void* New, size_t n,
                      const char* what, const char* where);

extern int SNPMOCK_cases;
extern int SNPMOCK_failures;
extern long SNPMOCK_compares;
