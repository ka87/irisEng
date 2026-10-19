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
    byte gap1[8];
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
    byte AIPriority;
    byte aa;
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
    byte if_this_not0_wont_render_gao;
    byte u3;
    unsigned __int16 snpIndex;
    Mtx4x4* matrices;
    struct sGameObjectBV* boundingVolume;
    struct sGameObjectBasic* basic;
    struct sGameObjectExtended* extended;
    byte distance;
    byte LODDist;
    byte designFlags;
    char process;
} tGameObjectMain;

typedef struct sGameObjectVisualObject
{
    struct sGroGeneric* GROGEOOrSomethingElse;
    struct sGroGeneric* GROMAT;
    int objDrawMask;
    int* RLIVertexColors;
    byte renderOnlyWithGFXRender;
    byte culledFlagFromCullingObject;
    byte vmilyenDistanceLightAttenuationFactor;
    signed char boneDisplayOrder_zListBufferNum;
} tGameObjectVisualObject;

typedef struct sShape
{
    int16_t usageCount;
    uint16_t num;
    byte shapesOriginal[64];
    byte shapes[64];
} tShape;

typedef struct sAnim
{
    int16_t flags;
    byte frequency;
    byte _pad;
    struct sEventData* ptr;
    byte gapb[8];
    Mtx4x4 flashMtx;
    uint16_t actionForTransition;
    byte actionElemForTransition;
    byte transitionFlags;
    byte flags2;
    byte actionLength;
    uint16_t startTime;
    byte gap1[8];
} tAnim;

typedef struct sActionElem
{
    struct sGameObjectEventsList* eventLists;
    BASStruct* transition;
    struct sShape* shape;
    byte changeActionCallAmountToChange;
    byte b2;
    byte actionFrequency;
    byte customBits;
    int16_t designFlags;
    byte b5;
    byte _pad;
} tActionElem;

typedef struct sAction
{
    byte actionElemNum;
    byte actionElemIDForTransition;
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
    byte flags;
    byte transitionLength;
} tTransitionElem;

typedef struct sGameObjectActorObject
{
    byte initedInANIMainCall;
    byte flagsEgBlendIw2; // 1 = synchronizetracks? (EVE_SynchronizeTracks, ANI_MainCall) 
    __int16 currTime;
    struct sGroup* group;
    struct sGroup* skeletonGroup;
    struct sGroup* skeletonGroupToRestoreOnReinit;
    struct sShape* shape2;
    byte shapes[64];
    struct sShape* shape;
    struct sAnim* animPtr[4];
    struct sAction* actionPtr;
    struct sActionKit* actionKit;
    struct sTransitionElem* transitionElem;
    uint16_t currentAction;
    uint16_t currentActionTransition;
    byte actionItem;
    byte callCountForChangeAction;
    byte eventRunningFrequency;
    byte unkb;
    byte culledFlagFromCullingObject;
    byte ANILOD;
    byte lod;
    byte aniBitFieldLOD;
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

void OBJ_GameObject_CreateExtended(tGameObjectMain* a1);
void OBJ_MakeAbsoluteMatrix(tGameObjectMain* a1, Mtx4x4* a2);
Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* a1);
BOOL OBJ_BV_IsAABBox(tGameObjectBV* a1);
void OBJ_ResetProcess(tGameObjectMain* a1, char a2, char a3);
void OBJ_SetProcess(tGameObjectMain* a1, char a2, char a3);
int OBJ_b_HasBeenProcess(tGameObjectMain* a1, char a2, int a3);
void OBJ_SightGet(tGameObjectMain* a1, Vec3* a2);
void OBJ_SightSet(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_SightInitGet(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_SightGeneralSet(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3);
void OBJ_BankingGeneralSet(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3);
void OBJ_BankingSet(struct sGameObjectMain* a1, Vec3* a3);
void OBJ_HorizonGet(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_ScaleSet(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_ZoomSet(struct sGameObjectMain* a1, float zoom);
BOOL OBJ_BV_IsSphere(tGameObjectBV* a1);
void OBJ_ReinitMatrixStorageByFlags(tGameObjectMain* a1, Mtx4x4* a2);
tGaoMatrix* OBJ_pst_GetLocalMatrix(tGameObjectMain* a1);
void OBJ_FreeGroup(tGroup* a1);
void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown);
Mtx4x4* OBJ_pst_GetFlashMatrix(struct sGameObjectMain* a1);
void OBJ_ComputeLocalWhenHie(struct sGameObjectMain* a1);
BOOL OBJ_b_IsChild(struct sGameObjectMain* a1);
void OBJ_ComputeGlobalWithLocal(struct sGameObjectMain* a1, Mtx4x4* a2, int a3);
void OBJ_GameObject_AllocateGizmo(struct sGameObjectMain* a1, int a2, int a3);
void OBJ_ChangeIdentityFlags(tGameObjectMain* a1, unsigned int a2, int a3);
void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* a1);
void OBJ_ComputeBV(tGameObjectMain* a1, int a2);
void OBJ_ResolveAIRef(struct sGameObjectMain* a1);
struct sGameObjectMain* OBJ_GameObject_Duplicate(struct sWorld* a1, struct sGameObjectMain* srcGao, int a3, int a4, Vec3* pos);
void OBJ_AddInGroup(struct sGroup* a1, struct sGameObjectMain* object);
void OBJ_GameObject_CreateExtendedIfNot(struct sGameObjectMain* a1);
void OBJ_Gizmo_UpdateGizmoPtr(struct sGameObjectMain* a1);
void OBJ_Reinit(struct sGameObjectMain* a1);
struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t Offset);
void OBJ_SetInitialAbsolutePosition(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_RestoreInitialPos(struct sGameObjectMain* a1);
void OBJ_SetAbsolutePosition(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_SetLocalPosition(struct sGameObjectMain* a1, Vec3* a2);
void OBJ_SetAbsoluteMatrix(struct sGameObjectMain* a1, Mtx4x4* a2);
void OBJ_EyeFollow(struct sGameObjectMain* a1, Vec3* a2, float a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);
void OBJ_FreezeModifier_C(struct sGameObjectMain* a1, int a2, int setOrReset);
BOOL OBJ_ModifierType_C(struct sGameObjectMain* a1, int mdfId);
int OBJ_b_test_flag(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest);
struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* a1);
void OBJ_BV_ComputeCenter(struct sGameObjectMain* a1, Vec3* a2);
int OBJ_Frame_CullingAABBox(Vec3* a1, Vec3* a2, struct sDDCamStruct* a3, int a4, int a5, float* a6, float* a7);
int OBJ_Frame_CullingSphere(Vec3* a1, float a2, struct sDDCamStruct* a3, int a4, int a5, float* a6, float* a7);
void OBJ_UpdateCullingVars(struct sDDCamStruct* a1);
BOOL OBJ_CullingObject(struct sGameObjectMain* a1);
BOOL OBJ_CullingPointInside(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, char flags);
struct sGameObjectMain* OBJ_GameObject_Create(int flags);
struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* a1);
void OBJ_HierarchyMainCall(struct sWorld* a1);
struct sGroGeneric* OBJ_p_GetGro(struct sGameObjectMain* a1);
struct sGameObjectMain* OBJ_pst_GroupGetByRank(struct sGroup* a1, int idx);
Vec3* OBJ_pst_GetPos(struct sGameObjectMain* a1);
void OBJMY_ComputeLocalWhenHie(struct sGameObjectMain* a1);
struct sGEOMAT* OBJ_p_GetGrm(struct sGameObjectMain* a1);
char OBJ_uc_CalcLODVis(struct sGameObjectMain* a1, struct sViewElem* a2);
struct sDyn* OBJ_pst_GetDynaStruct(struct sGameObjectMain* a1);

tableDescStruct* OBJ_ul_World_LoadGolCallback(uint32_t Offset);

void ACT_FreeActionKit(tActionKit** a1);
void ACT_SetAction(struct sGameObjectMain* a1, int actionId, int forced);
void ACT_ActionRestoreInit(struct sActionKit* a1);
void ACT_SetUsageCounts(struct sActionKit* a1);
struct sActionKit* ACT_ul_ActionKitCallback(uint32_t Offset);
void ACTKitMergeNoOver(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3, int a4);
void ACTKitMergeOver(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3, int a4);
