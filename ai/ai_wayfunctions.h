#pragma once
#include "../common.h"
#include "../gameobject.h"

int okok;
tGameObjectMain* AI_WAYgpstBest;
float AI_WAYgfBest;

struct sGameObjectMain* AI_EvalFunc_WAYNearestWPOfObj_C(struct sGameObjectMain* a1, struct sWayNetwork* a2, int flagMustHave, int flagMustNothave, int whatToTest);
struct sWayNetwork* AI_EvalFunc_WAYNetOfObj_C(struct sGameObjectMain* a1);
struct sGameObjectMain* AI_EvalFunc_WAYNetNextWP_C(struct sGameObjectMain* a1, struct sWayNetwork* a2, struct sGameObjectMain* a3, int type, int elemID);
int AI_EvalFunc_WAYNetFill_C(struct sWayNetwork* network, struct sGameObjectMain* gao, struct sGameObjectMain** gaoList);
unsigned int AI_EvalFunc_WAYShortWay_C(struct sGameObjectMain* a1, struct sWayNetwork* a2, struct sGameObjectMain** a3, struct sGameObjectMain* a4, struct sGameObjectMain* a5);
struct sGameObjectMain* AI_EvalFunc_WAYRootGet_C(struct sWayNetwork* a1);

tAINode* AI_EvalFunc_WAYNetNextWP(tAINode* a1);
tAINode* AI_EvalFunc_WAYNetFill(tAINode* a1);
tAINode* AI_EvalFunc_WAYNearestWPOfObj(tAINode* a1);
tAINode* AI_EvalFunc_WAYShortWay(tAINode* a1);
tAINode* AI_EvalFunc_WAYCapaSetLink(tAINode* a1);
tAINode* AI_EvalFunc_WAYCapaGetLink(tAINode* a1);
tAINode* AI_EvalFunc_WAYNearestWPOfObjExcl(tAINode* a1);
tAINode* AI_EvalFunc_WAYNetOfObj(tAINode* a1);
tAINode* AI_EvalFunc_WAYDesignSetLink(tAINode* a1);
tAINode* AI_EvalFunc_WAYDesignGetLink(tAINode* a1);
tAINode* AI_EvalFunc_WAYRootGet(tAINode* a1);
tAINode* AI_EvalFunc_WAYGetNumLinks(tAINode* a1);
tAINode* AI_EvalFunc_WAYLinkSetWeight(tAINode* a1);
tAINode* AI_EvalFunc_WAYLinkGetWeight(tAINode* a1);
tAINode* AI_EvalFunc_WAYGet(tAINode* a1);
tAINode* AI_EvalFunc_WAYShortParams(tAINode* a1);