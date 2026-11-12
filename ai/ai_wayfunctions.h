#pragma once
#include "../common.h"
#include "../gameobject.h"

int okok;
tGameObjectMain* AI_WAYgpstBest;
float AI_WAYgfBest;

struct sGameObjectMain* AI_EvalFunc_WAYNearestWPOfObj_C(struct sGameObjectMain* gao, struct sWayNetwork* network, int flagMustHave, int flagMustNothave, int whatToTest);
struct sWayNetwork* AI_EvalFunc_WAYNetOfObj_C(struct sGameObjectMain* gao);
struct sGameObjectMain* AI_EvalFunc_WAYNetNextWP_C(struct sGameObjectMain* gao, struct sWayNetwork* network, struct sGameObjectMain* wp, int type, int elemID);
int AI_EvalFunc_WAYNetFill_C(struct sWayNetwork* network, struct sGameObjectMain* gao, struct sGameObjectMain** gaoList);
unsigned int AI_EvalFunc_WAYShortWay_C(struct sGameObjectMain* gao, struct sWayNetwork* network, struct sGameObjectMain** path, struct sGameObjectMain* from, struct sGameObjectMain* to);
struct sGameObjectMain* AI_EvalFunc_WAYRootGet_C(struct sWayNetwork* network);

tAINode* AI_EvalFunc_WAYNetNextWP(tAINode* node);
tAINode* AI_EvalFunc_WAYNetFill(tAINode* node);
tAINode* AI_EvalFunc_WAYNearestWPOfObj(tAINode* node);
tAINode* AI_EvalFunc_WAYShortWay(tAINode* node);
tAINode* AI_EvalFunc_WAYCapaSetLink(tAINode* node);
tAINode* AI_EvalFunc_WAYCapaGetLink(tAINode* node);
tAINode* AI_EvalFunc_WAYNearestWPOfObjExcl(tAINode* node);
tAINode* AI_EvalFunc_WAYNetOfObj(tAINode* node);
tAINode* AI_EvalFunc_WAYDesignSetLink(tAINode* node);
tAINode* AI_EvalFunc_WAYDesignGetLink(tAINode* node);
tAINode* AI_EvalFunc_WAYRootGet(tAINode* node);
tAINode* AI_EvalFunc_WAYGetNumLinks(tAINode* node);
tAINode* AI_EvalFunc_WAYLinkSetWeight(tAINode* node);
tAINode* AI_EvalFunc_WAYLinkGetWeight(tAINode* node);
tAINode* AI_EvalFunc_WAYGet(tAINode* node);
tAINode* AI_EvalFunc_WAYShortParams(tAINode* node);