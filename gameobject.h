#pragma once
#include "math.h"
#include "ai.h"
#include "dynamics.h"
#include "collision.h"
#include "groups.h"
#include "ways.h"
#include "modifiers.h"
#include "events.h"
#include "sound.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/light.h"
#include "basic_array.h"
#include "common.h"

typedef struct sGaoMatrix
{
    Mtx4x4 mtx;
    struct sGameObjectMain* refGao;
    struct sGameObjectMain* refGao2;
    uint8_t gap1[8];
} tGaoMatrix;


typedef struct sGameObjectMultiPartData
{
    int elemNum;
    void* multipartGaoObjectPartsOrMatrices;
} tGameObjectMultiPartData;

typedef struct sGameObjectBasic
{
    void* actorObjectOrVisualObject;
    struct sDyn* dyn;
    struct sGaoMatrix* mtx;
    struct sGameObjectMultiPartData* multiPartData;
    struct sGEOMAT* extraMaterial;
} tGameObjectBasic;

typedef struct sGameObjectBV
{
    Vec3 BVMin;
    Vec3 BVMax;
    Vec3 BVMinOriginal;
    Vec3 BVMaxOriginal;
    Vec3 BVMinLocal;
    Vec3 BVMaxLocal;
} tGameObjectBV;

typedef struct sGameObjectCollDetectionList
{
    struct sGameObjectMain** detectionListGaos;
    int overlapNum;
} tGameObjectCollDetectionList;

typedef struct sGameObjectCollMain
{
    struct sGameObjectCollDetectionList* detectionList;
    struct sColInstance* colInstance;
    struct sColMap* colMap;
} tGameObjectCollMain;

typedef struct sGameObjectDesignFields
{
    int designFlags;
    int designInt1;
    int designInt2;
    float designFloat1;
    float designFloat2;
    Vec3 designVec1;
    Vec3 designVec2;
    int designGO1;
    int designGO2;
    int designNet1;
    int designNet2;
    int designText1;
    int u9;
    int designText2;
    int u11;
    int designDesFlags;
    int u13;
    int designInt3;
} tGameObjectDesignFields;

typedef struct sMessage
{
    struct sGameObjectMain* gao;
    struct sGameObjectMain* gao2;
    Vec3 v1;
    Vec3 v2;
    float f2;
    float f1;
    int Int1;
    int Int2;
    int msgID;
} tMessage;

typedef struct sGameObjectMessages
{
    struct sMessage msgs[5];
    int count;
} tGameObjectMessages;

typedef struct sGameObjectExtended
{
    struct sGameObjectCollMain* coll;
    struct sAI* AIInstance;
    struct sGameObjectDesignFields* fields;
    struct sGroup* grp;
    struct sGroGeneric* light;
    struct sGameObjectMessages* messages;
    struct sWay* way;
    struct sModifier* mdf;
    struct sEventData* eve;
    struct sSndBank* snd;
    uint16_t capa;
    uint16_t capaBackup;
    uint8_t AIPriority;
    uint8_t aa;
    int16_t extraFlag;
    float aiLod;
    float renderMinReqDistSq;
    uint16_t sectorID;
    uint16_t sectorElemIdx;
} tGameObjectExtended;

typedef struct sMultiPartGameObjectLinks
{
    Mtx4x4* mtxPtr;
    struct sGameObjectMain* gao;
    int mtxIdx;
} tMultiPartGameObjectLinks;

typedef struct sGameObjectMain
{
    int flags; // 0x4000000 = pause
    int flags2; // a BVMinSet állít rá 0x2000000-t, 1 = active
    uint8_t if_this_not0_wont_render_gao;
    uint8_t u3;
    uint16_t snpIndex;
    Mtx4x4* matrices;
    struct sGameObjectBV* boundingVolume;
    struct sGameObjectBasic* basic;
    struct sGameObjectExtended* extended;
    uint8_t distance;
    uint8_t LODDist;
    uint8_t designFlags;
    char process;
} tGameObjectMain;

typedef struct sGameObjectVisualObject
{
    struct sGroGeneric* GROGEOOrSomethingElse;
    struct sGroGeneric* GROMAT;
    int objDrawMask;
    int* RLIVertexColors;
    uint8_t renderOnlyWithGFXRender;
    uint8_t culledFlagFromCullingObject;
    uint8_t vmilyenDistanceLightAttenuationFactor;
    signed char boneDisplayOrder_zListBufferNum;
} tGameObjectVisualObject;

typedef struct sShape
{
    int16_t usageCount;
    uint16_t num;
    uint8_t shapesOriginal[64];
    uint8_t shapes[64];
} tShape;

typedef struct sAnim
{
    int16_t flags;
    uint8_t frequency;
    uint8_t _pad;
    struct sEventData* ptr;
    uint8_t gapb[8];
    Mtx4x4 flashMtx;
    uint16_t actionForTransition;
    uint8_t actionElemForTransition;
    uint8_t transitionFlags;
    uint8_t flags2;
    uint8_t actionLength;
    uint16_t startTime;
    uint8_t gap1[8];
} tAnim;

typedef struct sActionElem
{
    struct sGameObjectEventsList* eventLists;
    BASStruct* transition;
    struct sShape* shape;
    uint8_t changeActionCallAmountToChange;
    uint8_t b2;
    uint8_t actionFrequency;
    uint8_t customBits;
    int16_t designFlags;
    uint8_t b5;
    uint8_t _pad;
} tActionElem;

typedef struct sAction
{
    uint8_t actionElemNum;
    uint8_t actionElemIDForTransition;
    int16_t usageCount;
} tAction;

typedef struct sActionKit
{
    uint16_t elemNum;
    int16_t usageCount;
    int32_t flags;
    struct sAction** savedActions;
} tActionKit;

typedef struct sTransitionElem // volt sTransitionElemInner
{
    uint16_t actionID;
    uint8_t flags;
    uint8_t transitionLength;
} tTransitionElem;

typedef struct sGameObjectActorObject
{
    uint8_t initedInANIMainCall;
    uint8_t flagsEgBlendIw2; // 1 = synchronizetracks? (EVE_SynchronizeTracks, ANI_MainCall)
    int16_t currTime;
    struct sGroup* group;
    struct sGroup* skeletonGroup;
    struct sGroup* skeletonGroupToRestoreOnReinit;
    struct sShape* shape2;
    uint8_t shapes[64];
    struct sShape* shape;
    struct sAnim* animPtr[4];
    struct sAction* actionPtr;
    struct sActionKit* actionKit;
    struct sTransitionElem* transitionElem;
    uint16_t currentAction;
    uint16_t currentActionTransition;
    uint8_t actionItem;
    uint8_t callCountForChangeAction;
    uint8_t eventRunningFrequency;
    uint8_t unkb;
    uint8_t culledFlagFromCullingObject;
    uint8_t ANILOD;
    uint8_t lod;
    uint8_t aniBitFieldLOD;
    float aniPelvisOffset;
    float elapsedDT;
} tGameObjectActorObject;


int OBJ_gb_DuplicateAI;
struct sGameObjectMain* OBJ_gpst_ObjectToLoadIn;
struct sGameObjectMain* MSG_gpo_Owner;

struct sGameObjectMain* AI_MainActorArr[2];
extern struct sGameObjectMain** AI_MainActor;

float OBJ_gf_EYEFOLLOW_HeadZMax;
float OBJ_gf_EYEFOLLOW_HeadZMin;
float OBJ_gf_EYEFOLLOW_HeadXMax;
float OBJ_gf_EYEFOLLOW_HeadXMin;
float OBJ_gf_EYEFOLLOW_TorsoZMax;
float OBJ_gf_EYEFOLLOW_TorsoZMin;
float OBJ_gf_EYEFOLLOW_HeadXOffset;

void OBJ_GameObject_CreateExtended(tGameObjectMain* gao);
void OBJ_MakeAbsoluteMatrix(tGameObjectMain* gao, Mtx4x4* dest);
Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao);
BOOL OBJ_BV_IsAABBox(tGameObjectBV* bv);
void OBJ_ResetProcess(tGameObjectMain* gao, char callCount, char mask);
void OBJ_SetProcess(tGameObjectMain* gao, char callCount, char mask);
int OBJ_b_HasBeenProcess(tGameObjectMain* gao, char callCount, int mask);
void OBJ_SightGet(tGameObjectMain* gao, Vec3* sight);
void OBJ_SightSet(struct sGameObjectMain* gao, Vec3* sight);
void OBJ_SightInitGet(struct sGameObjectMain* gao, Vec3* sight);
void OBJ_SightGeneralSet(struct sGameObjectMain* gao, Vec3* sight, Vec3* banking);
void OBJ_BankingGeneralSet(struct sGameObjectMain* gao, Vec3* sight, Vec3* banking);
void OBJ_BankingSet(struct sGameObjectMain* gao, Vec3* banking);
void OBJ_HorizonGet(struct sGameObjectMain* gao, Vec3* horizon);
void OBJ_ScaleSet(struct sGameObjectMain* gao, Vec3* scale);
void OBJ_ZoomSet(struct sGameObjectMain* gao, float zoom);
BOOL OBJ_BV_IsSphere(tGameObjectBV* bv);
void OBJ_ReinitMatrixStorageByFlags(tGameObjectMain* gao, Mtx4x4* mtx);
tGaoMatrix* OBJ_pst_GetLocalMatrix(tGameObjectMain* gao);
void OBJ_FreeGroup(tGroup* grp);
void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown);
Mtx4x4* OBJ_pst_GetFlashMatrix(struct sGameObjectMain* gao);
void OBJ_ComputeLocalWhenHie(struct sGameObjectMain* gao);
BOOL OBJ_b_IsChild(struct sGameObjectMain* gao);
void OBJ_ComputeGlobalWithLocal(struct sGameObjectMain* gao, Mtx4x4* dest, int withRootAbsolute);
void OBJ_GameObject_AllocateGizmo(struct sGameObjectMain* gao, int elemNum, int asLinks);
void OBJ_ChangeIdentityFlags(tGameObjectMain* gao, unsigned int newFlags, int oldFlags);
void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* gao);
void OBJ_ComputeBV(tGameObjectMain* gao, int onlyIfEmpty);
void OBJ_ResolveAIRef(struct sGameObjectMain* gao);
struct sGameObjectMain* OBJ_GameObject_Duplicate(struct sWorld* world, struct sGameObjectMain* srcGao, int attach, int dupEvents, Vec3* pos);
void OBJ_AddInGroup(struct sGroup* grp, struct sGameObjectMain* object);
void OBJ_GameObject_CreateExtendedIfNot(struct sGameObjectMain* gao);
void OBJ_Gizmo_UpdateGizmoPtr(struct sGameObjectMain* gao);
void OBJ_Reinit(struct sGameObjectMain* gao);
struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t Offset);
void OBJ_SetInitialAbsolutePosition(struct sGameObjectMain* gao, Vec3* pos);
void OBJ_RestoreInitialPos(struct sGameObjectMain* gao);
void OBJ_SetAbsolutePosition(struct sGameObjectMain* gao, Vec3* pos);
void OBJ_SetLocalPosition(struct sGameObjectMain* gao, Vec3* pos);
void OBJ_SetAbsoluteMatrix(struct sGameObjectMain* gao, Mtx4x4* mtx);
void OBJ_EyeFollow(struct sGameObjectMain* gao, Vec3* target, float weight, int preset, int clampToSight, int unlimited, int torsoFollowsHead, int torsoFollowsOwn, int useGlobalLimits, int useThirdBone);
void OBJ_FreezeModifier_C(struct sGameObjectMain* gao, int mdfId, int setOrReset);
BOOL OBJ_ModifierType_C(struct sGameObjectMain* gao, int mdfId);
int OBJ_b_test_flag(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest);
struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* gao);
void OBJ_BV_ComputeCenter(struct sGameObjectMain* gao, Vec3* center);
int OBJ_Frame_CullingAABBox(Vec3* min, Vec3* max, struct sDDCamStruct* cam, int width, int height, float* outClipped, float* outScreen);
int OBJ_Frame_CullingSphere(Vec3* center, float radius, struct sDDCamStruct* cam, int width, int height, float* outClipped, float* outScreen);
void OBJ_UpdateCullingVars(struct sDDCamStruct* cam);
BOOL OBJ_CullingObject(struct sGameObjectMain* gao);
BOOL OBJ_CullingPointInside(Vec3* point, Vec3* planeLeft, Vec3* planeUp, Vec3* planeRight, Vec3* planeDown, char flags);
struct sGameObjectMain* OBJ_GameObject_Create(int flags);
struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* gao);
void OBJ_HierarchyMainCall(struct sWorld* world);
struct sGroGeneric* OBJ_p_GetGro(struct sGameObjectMain* gao);
struct sGameObjectMain* OBJ_pst_GroupGetByRank(struct sGroup* grp, int idx);
Vec3* OBJ_pst_GetPos(struct sGameObjectMain* gao);
void OBJMY_ComputeLocalWhenHie(struct sGameObjectMain* gao);
struct sGEOMAT* OBJ_p_GetGrm(struct sGameObjectMain* gao);
char OBJ_uc_CalcLODVis(struct sGameObjectMain* gao, struct sViewElem* view);
struct sDyn* OBJ_pst_GetDynaStruct(struct sGameObjectMain* gao);

tableDescStruct* OBJ_ul_World_LoadGolCallback(uint32_t Offset);

void ACT_FreeActionKit(tActionKit** ak);
void ACT_SetAction(struct sGameObjectMain* gao, int actionId, int forced);
void ACT_ActionRestoreInit(struct sActionKit* ak);
void ACT_SetUsageCounts(struct sActionKit* ak);
struct sActionKit* ACT_ul_ActionKitCallback(uint32_t Offset);
void ACTKitMergeNoOver(struct sGameObjectMain* gaoDest, struct sGameObjectMain* gaoSrc, int firstActionID, int lastActionID);
void ACTKitMergeOver(struct sGameObjectMain* gaoDest, struct sGameObjectMain* gaoSrc, int firstActionID, int lastActionID);
