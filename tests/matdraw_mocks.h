#pragma once
/* same order as materialanddraw.c's own includes: the engine headers
   are order-sensitive, and geo.h first does not resolve */
#include "../profiler.h"
#include "../gdi.h"
#include "../time.h"
#include "../textures.h"
#include "../win/wingdi.h"
#include "../soft.h"
#include "../sprites.h"
#include "../materialanddraw.h"

#define MDMOCK_LOG_MAX 32768
#define MDMOCK_ARGS 8

typedef struct sMdMockCall
{
    const char* fn;
    int nargs;
    long long arg[MDMOCK_ARGS];
} tMdMockCall;

extern tMdMockCall MDMOCK_log[MDMOCK_LOG_MAX];
extern int MDMOCK_logLen;
extern int MDMOCK_overflow;

/* The ZList node pool SOFT_pst_ZList_AddNode hands out of. */
#define MDMOCK_NODES 512
#define MDMOCK_NODE_MESH 4

extern tZListElem MDMOCK_nodes[MDMOCK_NODES];
extern tZListMeshElem MDMOCK_mesh[MDMOCK_NODES][MDMOCK_NODE_MESH];
extern int MDMOCK_nodeCount;
/* every Nth AddNode returns NULL, so the guarded arm is driven too */
extern int MDMOCK_nodeFailEvery;

extern unsigned int MDMOCK_ticks;
extern long long MDMOCK_qpc;

/* The material GEO_GetMaterial hands back, by chunk material number. */
extern tGEOMAT* MDMOCK_geoMaterials[8];

void MDMOCK_Reset(void);
void MDMOCK_Record(const char* fn, int nargs, ...);
int MDMOCK_Index(const void* p);
void MDMOCK_RegisterPtr(const void* p);

/* Display-device entry points, installed into the mock tGdi_DisplayData. */
void MDMOCK_TextureInit(int texIdx, int flags, char param);
int MDMOCK_DrawElementIndexedTriangles(struct sGeoTriangleChunks* chunks, Vec3* verts,
                                       struct sGeoUVElem* uvs, int vertexNum);
int MDMOCK_DrawSprites(tGeoSprites* spr, Vec3* verts, int num);
