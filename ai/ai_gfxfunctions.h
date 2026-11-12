#pragma once
#include "../ai.h"

int AI_EvalFunc_GFXAddSorted_C(struct sGameObjectMain* a4, char a2, int16_t a3);
void AI_EvalFunc_GFXFlagSet_C_CURRENT(int a1, char a2, int a3);
int AI_EvalFunc_GFXDel_C_CURRENT(int a1);
void AI_EvalFunc_GFXMaterialSet_C_CURRENT(int a1, struct sGameObjectMain* a2, int a3);
void GFXSetCurGlobal(int a1);
void AI_EvalFunc_GFXSeti_C_CURGLOB(int a1, int a2);
void AI_EvalFunc_GFXSetf_C_CURGLOB(int a2, float a3);
void AI_EvalFunc_GFXSeti_C_CURRENT(int a1, int a2, int a3);
int AI_EvalFunc_GFXAdd_C_CURRENT(char a1);
void AI_EvalFunc_GFXSetv_C_CURGLOB(int a1, Vec3* a2);
void AI_EvalFunc_GFXGetv_C(struct sGameObjectMain* gao, int a2, int a3, Vec3* a4);
void AI_EvalFunc_GFXSetv_C_CURRENT(int a1, int a2, Vec3* a3);
void AI_EvalFunc_GFXSetf_C_CURRENT(int id, int a2, float a3);
void AI_EvalFunc_GFXLifeTimeSet_C(struct sGameObjectMain* gao, int gfxId, float a3);
int AI_EvalFunc_GFXGeti_C(struct sGameObjectMain* gao, int a2, int a3);

tAINode* AI_EvalFunc_GFXAdd(tAINode* node);
tAINode* AI_EvalFunc_GFXDel(tAINode* node);
tAINode* AI_EvalFunc_GFXAddWorld(tAINode* node);
tAINode* AI_EvalFunc_GFXAddSorted(tAINode* node);
tAINode* AI_EvalFunc_GFXFlagSet(tAINode* node);
tAINode* AI_EvalFunc_GFXMaterialSet(tAINode* node);
tAINode* AI_EvalFunc_GFXRequest(tAINode* node);
tAINode* AI_EvalFunc_GFXGetf(tAINode* node);
tAINode* AI_EvalFunc_GFXSetf(tAINode* node);
tAINode* AI_EvalFunc_GFXGeti(tAINode* node);
tAINode* AI_EvalFunc_GFXSeti(tAINode* node);
tAINode* AI_EvalFunc_GFXGetv(tAINode* node);
tAINode* AI_EvalFunc_GFXSetv(tAINode* node);
tAINode* AI_EvalFunc_GFXDepthBlur(tAINode* node);
tAINode* AI_EvalFunc_GFXLifeTimeSet(tAINode* node);
tAINode* AI_EvalFunc_CreateExplosion(tAINode* arg0);
tAINode* AI_EvalFunc_GFXCarteObject(tAINode* node);
tAINode* AI_EvalFunc_GFX_FS_VideoStart(tAINode* node);
tAINode* AI_EvalFunc_GFX_FS_VideoStop(tAINode* node);
tAINode* AI_EvalFunc_GFX_FS_VideoFinish(tAINode* node);

extern int GFX_gl_FS_VideoPlaying;
extern int GFX_gl_FS_VideoScaleMode;
void GFX_FS_VideoUpdate();
