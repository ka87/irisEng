#pragma once
#include "../photo.h"
#include "../world.h"
#include "../gdi.h"
#include "../tables.h"
#include "../modifiers.h"
#include "../modifiers/photomodifier.h"

/* The engine around the photo analyser: the photo modifier lookup,
   the bounding-volume helpers, the ray cast and the keyboard, plus a
   record of every call a side made and of every line it logged.
   IMG_Log4Debugging writes through the C library, so both copies are
   compiled with fopen/fwrite/fclose redirected here. */

#define PHMOCK_LOG_MAX 20000
#define PHMOCK_TEXT 512

typedef struct sPhMockCall
{
    const char* fn;
    int ni;
    int nf;
    int ia[4];
    float fa[4];
    char text[PHMOCK_TEXT];
} tPhMockCall;

extern tPhMockCall PHMOCK_log[PHMOCK_LOG_MAX];
extern int PHMOCK_logLen;
extern int PHMOCK_logOverflow;

/* The scene. Objects 0..PHMOCK_OBJECTS-1 sit in the table the two
   analysers walk; each is driven from its index so every branch is
   reachable. */
#define PHMOCK_OBJECTS 24

extern tWorld PHMOCK_world;
extern tGdi_DisplayData PHMOCK_dd;
extern tGameObjectMain PHMOCK_objects[PHMOCK_OBJECTS];
extern tGameObjectMain PHMOCK_animObjects[PHMOCK_OBJECTS];
extern tGameObjectMain PHMOCK_snapObjects[PHMOCK_OBJECTS];
extern tModifier PHMOCK_mdfs[PHMOCK_OBJECTS];
extern tPhotoMdfStruct PHMOCK_photos[PHMOCK_OBJECTS];
extern tableElemStructPF PHMOCK_table[PHMOCK_OBJECTS];

/* Per-case knobs. */
extern int PHMOCK_shiftDown;       /* what GetAsyncKeyState(VK_SHIFT) reports */
extern int PHMOCK_rayHit;          /* -1 nothing, -2 the object itself, else an index */
extern float PHMOCK_bvRatio;
extern float PHMOCK_screenRatio;
extern int PHMOCK_cullPattern;     /* picks the ratios from the call ordinal */
extern int PHMOCK_isAABBoxMask;    /* bit per object: is its volume a box */
extern int PHMOCK_viewMtxIsCamera;
extern int PHMOCK_designOverride;
extern Mtx4x4 PHMOCK_viewMtx;

void PHMOCK_Init(void);
void PHMOCK_Reset(void);
void PHMOCK_BuildScene(unsigned int seed, int pattern);

FILE* PHMOCK_fopen(const char* path, const char* mode);
size_t PHMOCK_fwrite(const void* buf, size_t size, size_t n, FILE* fp);
int PHMOCK_fclose(FILE* fp);

void PHMOCK_SetCase(const char* name);
void PHMOCK_Summary(const char* title);
int PHMOCK_SameInt(int base, int New, const char* what, const char* where);
int PHMOCK_SameFloat(float base, float New, const char* what, const char* where);
int PHMOCK_CompareLog(const tPhMockCall* a, int alen, const tPhMockCall* b, int blen,
                      const char* where);
void PHMOCK_SaveLog(tPhMockCall* dest, int* destLen);

extern int PHMOCK_cases;
extern int PHMOCK_failures;
extern long PHMOCK_compares;
