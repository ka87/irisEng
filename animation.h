#pragma once
#include "common.h"

extern uint8_t ANI_c_DefaultBitFieldLOD;

Vec3 ANI_gst_PositionBeforeAnimPlayer;

struct sGameObjectMain* pst_RenderingStack[256];
int PushedStack;

tGameObjectActorObject* ANI_pst_CreateGameObjectAnim();
struct sAnim* ANI_AllocAnim();
void ANI_Free(tGameObjectActorObject* gaoao);
void ANI_UpdateFlash(struct sGameObjectMain* gao, char which);
void ANI_ChangeAction(struct sGameObjectMain* gao, struct sGameObjectActorObject* actor);
void ANI_OneCall(struct sGameObjectMain* gao);
void ANI_SetTrackList(struct sAnim* anim, struct sGameObjectEventsList* list);
void ANI_ChangeShape(struct sGameObjectMain* gao, struct sShape* shape);
void ANI_pst_DuplicateGameObjectAnim(struct sWorld* world, struct sGameObjectMain* dstGao, struct sGameObjectMain* srcGao);
void ANI_PlayGameObjectAnimation(tGameObjectMain* gao, tGameObjectActorObject* actor, float dt, Vec3* speed);
void ANI_ResolveRefs(struct sGameObjectMain* gao);
struct sShape* ANI_ul_ShapeCallback(uint32_t Offset);
void ANI_Reinit(struct sGameObjectMain* gao);
void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* gao);
int ANI_GaoAnimationIsTransitioning(struct sGameObjectMain* gao);
struct sGameObjectMain* ANI_pst_GetReference(struct sGameObjectMain* gao);
struct sGameObjectMain* ANI_pst_GetObjectByAICanal(struct sGameObjectMain* gao, uint8_t aiCanal);
void ANI_ApplyHierarchyOnFlash(struct sGameObjectMain* gao);
void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* gao, Vec3* offset, char which);
void ANI_MainCall(struct sWorld* world);
void ANI_BeforeRendering(struct sWorld* world);
struct sGameObjectMain* ANI_pst_GetReferenceInit(struct sGameObjectMain* gao);
void ANI_Render(struct sGameObjectMain* mainGao);
void ANI_ChangeBoneShape(struct sGameObjectMain* gao, uint8_t canal, uint8_t shapeIdx, int byAICanal);
void ANI_ChangeDefaultSkeleton(struct sGameObjectMain* gao, struct sGameObjectMain* srcGao);
void ANI_RestoreDefaultSkeleton(struct sGameObjectMain* gao, struct sGameObjectMain* srcGao);
void ANI_CloneSet(struct sGameObjectMain* gao, struct sGameObjectMain* srcGao, int actionId);
uint8_t ANI_uc_GetEngineCanalByAICanal(struct sGameObjectMain* gao, char aiCanal);
