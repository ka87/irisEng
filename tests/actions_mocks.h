#pragma once
#include <stddef.h>
#include "../common.h"
#include "../math.h"
#include "../basic_array.h"
#include "../events.h"
#include "../gameobject.h"

/* The engine around actions.c: an arena allocator whose blocks land at
   the same addresses for both sides, the loader's byte readers over a
   synthetic file, the animation calls, and a record of every call a
   side made.

   Nothing is ever handed back -- MEM_Free only logs -- so the first
   side's state stays readable while the second runs, and the two sides
   are separated by rewinding the arena. Every block sits 64 bytes into
   a larger one and is followed by 64 more, because the engine sizes an
   action by hand as 20 bytes per element, which is the 32-bit size of
   tActionElem and not the size this build has. */

#define ACTMOCK_LOG_MAX   400000
#define ACTMOCK_ARENA     (4 * 1024 * 1024)
#define ACTMOCK_ACTIONS   10
#define ACTMOCK_ELEMS     4
#define ACTMOCK_TRANS     6
#define ACTMOCK_FILE      8192

typedef struct sActMockCall
{
    const char* fn;
    int ni;
    int nf;
    int ia[4];
    float fa[2];
} tActMockCall;

extern tActMockCall ACTMOCK_log[ACTMOCK_LOG_MAX];
extern int ACTMOCK_logLen;
extern int ACTMOCK_logOverflow;

extern unsigned char ACTMOCK_arena[ACTMOCK_ARENA];
extern size_t ACTMOCK_arenaUsed;

/* One action kit laid out the way the engine lays it out: the header
   followed by the action pointer table, and each action header followed
   by its element table. */
/* The pointer table runs to 0x10000 entries although the kit only
   declares ACTMOCK_ACTIONS of them, so that index 0xFFFF -- the "no
   current action" sentinel ACT_pst_FindTransition guards against -- is
   memory the harness owns and can put a real action in. Without that
   the guard cannot be tested: both copies would read the same
   out-of-bounds bytes and agree. */
#define ACTMOCK_FAR 0x10000

typedef struct sActKitBuf
{
    tActionKit kit;
    tAction* actions[ACTMOCK_FAR];
} tActKitBuf;

/* The engine puts an action's element table directly after the 4-byte
   header, which is what sizeof(tAction) is here too -- but tActionElem
   wants 8-byte alignment in this build and only 4 in the engine's, so a
   struct with a tActionElem array in it would put the table at 8. The
   bytes are declared raw and ACTMOCK_Elems does the engine's own
   arithmetic over them. */
typedef struct sActBuf
{
    tAction action;
    unsigned char elemBytes[ACTMOCK_ELEMS * 40];
} tActBuf;

#define ACTMOCK_Elems(act) ((tActionElem*)(((char*)(act)) + sizeof(tAction)))

typedef struct sActScene
{
    tGameObjectMain gao;
    tGameObjectMain srcGao;
    tGameObjectBasic basic;
    tGameObjectBasic srcBasic;
    tDyn dyn;
    tDyn srcDyn;
    tGameObjectActorObject actor;
    tGameObjectActorObject srcActor;
    tGameObjectExtended ext;
    Mtx4x4 flashMtx;

    tActKitBuf kit;
    tActKitBuf srcKit;
    tActBuf actions[ACTMOCK_ACTIONS];
    tActBuf srcActions[ACTMOCK_ACTIONS];
    tAction* savedActions[ACTMOCK_ACTIONS];

    tGameObjectEventsList eventLists[ACTMOCK_ACTIONS];
    tGameObjectEventsListElem listElems[ACTMOCK_ACTIONS];
    tGameObjectEventElem trackElems[ACTMOCK_ACTIONS][4];
    tShape shapes[ACTMOCK_ACTIONS];

    BASStruct transitions[ACTMOCK_ACTIONS][ACTMOCK_ELEMS];
    BASKeyValStruct keyVals[ACTMOCK_ACTIONS][ACTMOCK_ELEMS][ACTMOCK_TRANS];
    tTransitionElem transElems[ACTMOCK_ACTIONS][ACTMOCK_ELEMS][ACTMOCK_TRANS];

    tAnim anims[4];
    tEventData animData[4];
    tAnimTracks animTracks[4];

    unsigned char file[ACTMOCK_FILE];
    int fileSize;
    unsigned char kitFile[ACTMOCK_FILE];
    int kitFileSize;
} tActScene;

extern tActScene ACTMOCK_scene;

/* What the driver keeps so the two sides can be compared on more than
   the scene: the return values of the loader callbacks and of
   ACT_pst_FindTransition, reduced to an integer. */
typedef struct sActScratch
{
    int ret[256];
    int retLen;
    unsigned long long hash;   /* every pushed value, not just the first 256 */
} tActScratch;

extern tActScratch ACTMOCK_scratch;

void ACTMOCK_Init(void);
void ACTMOCK_Reset(void);
void ACTMOCK_BuildScene(unsigned int seed, int pattern);
void ACTMOCK_BuildFiles(unsigned int seed, int pattern);
void ACTMOCK_Push(int value);
int ACTMOCK_Tag(const void* p);

/* The loader callbacks the two copies hand to LOA_MakeFileRef. The
   pointer value differs per side, so the driver names them and the mock
   logs the name's id rather than the pointer. */
void ACTMOCK_RegisterCallback(void* fn, int id);

void ACTMOCK_Save(void);
int ACTMOCK_CompareWithSaved(const char* where);

void ACTMOCK_SetCase(const char* name);
void ACTMOCK_Summary(const char* title);
int ACTMOCK_SameInt(int base, int New, const char* what, const char* where);
int ACTMOCK_SameLong(long long base, long long New, const char* what, const char* where);
int ACTMOCK_SameFloat(float base, float New, const char* what, const char* where);
int ACTMOCK_SameBytes(const void* base, const void* New, size_t n,
                      const char* what, const char* where);

extern int ACTMOCK_cases;
extern int ACTMOCK_failures;
extern long ACTMOCK_compares;
