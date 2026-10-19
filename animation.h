#pragma once
#include "common.h"

extern byte ANI_c_DefaultBitFieldLOD;

Vec3 ANI_gst_PositionBeforeAnimPlayer;

struct sGameObjectMain* pst_RenderingStack[256];
int PushedStack;

tGameObjectActorObject* ANI_pst_CreateGameObjectAnim();
struct sAnim* ANI_AllocAnim();
void ANI_Free(tGameObjectActorObject* a1);
void ANI_UpdateFlash(struct sGameObjectMain* a1, char a2);
void ANI_ChangeAction(struct sGameObjectMain* a1, struct sGameObjectActorObject* a2);
void ANI_OneCall(struct sGameObjectMain* a1);
void ANI_SetTrackList(struct sAnim* anims, struct sGameObjectEventsList* a2);
void ANI_ChangeShape(struct sGameObjectMain* a1, struct sShape* a2);
void ANI_pst_DuplicateGameObjectAnim(struct sWorld* a1, struct sGameObjectMain* a2, struct sGameObjectMain* a3);
void ANI_PlayGameObjectAnimation(tGameObjectMain* a1, tGameObjectActorObject* a2, float a3, Vec3* a4);
void ANI_ResolveRefs(struct sGameObjectMain* gao);
struct sShape* ANI_ul_ShapeCallback(uint32_t Offset);
void ANI_Reinit(struct sGameObjectMain* a1);
void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* a1);
int ANI_GaoAnimationIsTransitioning(struct sGameObjectMain* a1);
struct sGameObjectMain* ANI_pst_GetReference(struct sGameObjectMain* a1);
struct sGameObjectMain* ANI_pst_GetObjectByAICanal(struct sGameObjectMain* a1, uint8_t a2);
void ANI_ApplyHierarchyOnFlash(struct sGameObjectMain* a1);
void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* a1, Vec3* a2, char a3);
void ANI_MainCall(struct sWorld* a1);
void ANI_BeforeRendering(struct sWorld* a1);
struct sGameObjectMain* ANI_pst_GetReferenceInit(struct sGameObjectMain* a1);
void ANI_Render(struct sGameObjectMain* a1);
void ANI_ChangeBoneShape(struct sGameObjectMain* a1, unsigned __int8 a2, unsigned __int8 a3, int a4);
void ANI_ChangeDefaultSkeleton(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void ANI_RestoreDefaultSkeleton(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void ANI_CloneSet(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3);
uint8_t ANI_uc_GetEngineCanalByAICanal(struct sGameObjectMain* a1, char a2);
