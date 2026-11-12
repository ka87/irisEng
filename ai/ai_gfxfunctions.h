#pragma once
#include "../ai.h"

int AI_EvalFunc_GFXAddSorted_C(struct sGameObjectMain* aiGao, char type, int16_t idx);
void AI_EvalFunc_GFXFlagSet_C_CURRENT(int gfxId, char flag, int set);
int AI_EvalFunc_GFXDel_C_CURRENT(int gfxId);
void AI_EvalFunc_GFXMaterialSet_C_CURRENT(int gfxId, struct sGameObjectMain* source, int materialID);
void GFXSetCurGlobal(int gfxId);
void AI_EvalFunc_GFXSeti_C_CURGLOB(int paramIdx, int value);
void AI_EvalFunc_GFXSetf_C_CURGLOB(int paramIdx, float value);
void AI_EvalFunc_GFXSeti_C_CURRENT(int gfxId, int paramIdx, int value);
int AI_EvalFunc_GFXAdd_C_CURRENT(char type);
void AI_EvalFunc_GFXSetv_C_CURGLOB(int paramIdx, Vec3* value);
void AI_EvalFunc_GFXGetv_C(struct sGameObjectMain* gao, int gfxId, int paramIdx, Vec3* value);
void AI_EvalFunc_GFXSetv_C_CURRENT(int gfxId, int paramIdx, Vec3* value);
void AI_EvalFunc_GFXSetf_C_CURRENT(int gfxId, int paramIdx, float value);
void AI_EvalFunc_GFXLifeTimeSet_C(struct sGameObjectMain* gao, int gfxId, float lifeTime);
int AI_EvalFunc_GFXGeti_C(struct sGameObjectMain* gao, int gfxId, int paramIdx);

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
tAINode* AI_EvalFunc_CreateExplosion(tAINode* node);
tAINode* AI_EvalFunc_GFXCarteObject(tAINode* node);
tAINode* AI_EvalFunc_GFX_FS_VideoStart(tAINode* node);
tAINode* AI_EvalFunc_GFX_FS_VideoStop(tAINode* node);
tAINode* AI_EvalFunc_GFX_FS_VideoFinish(tAINode* node);

extern int GFX_gl_FS_VideoPlaying;
extern int GFX_gl_FS_VideoScaleMode;
void GFX_FS_VideoUpdate();
