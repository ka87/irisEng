#include "../time.h"
#include "ai_timefunctions.h"

void TIME_Wait_C(float amount)
{
    tAI* aiUltraBak = AI_gpst_CurrentInstanceUltra;

    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    aiUltraBak->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = amount;
}

tAINode* AI_EvalFunc_TIMEWait(tAINode* aiNode)
{
    float delay = AI_GetFloatFromStack();

    TIME_Wait_C(delay);

    *AI_gppst_CurrentJumpNode = aiNode + 1;

    return (tAINode*)0;
}

float AI_EvalFunc_TIMEGet_C()
{
    return TIM_gf_MainClock - TIM_gf_LockTime;
}

tAINode* AI_EvalFunc_TIMEGet(tAINode* aiNode)
{
    float time = AI_EvalFunc_TIMEGet_C();

    AI_PutFloatToStack(time);

    return aiNode + 1;
}

BOOL AI_EvalFunc_TIMEElapsed_C(float time, float amountInSec)
{
    return TIM_gf_MainClock - TIM_gf_LockTime - time >= amountInSec;
}

tAINode* AI_EvalFunc_TIMEElapsed(tAINode* aiNode)
{
    float time, amountInSec;
    int ret;

    amountInSec = AI_GetFloatFromStack();
    time = AI_GetFloatFromStack();
    ret = AI_EvalFunc_TIMEElapsed_C(time, amountInSec);
    AI_PutIntToStack(ret);

    return aiNode + 1;
}

tAINode* AI_EvalFunc_TIMEGetDt(tAINode* aiNode)
{
    AI_PutFloatToStack(TIM_gf_dt);
    return aiNode + 1;
}

void AI_EvalFunc_TIMEFactorSet_C(float factor)
{
    TIM_gf_SpeedFactor = factor;
}

tAINode* AI_EvalFunc_TIMEFactorSet(tAINode* aiNode)
{
    float factor = AI_GetFloatFromStack();

    AI_EvalFunc_TIMEFactorSet_C(factor);

    return aiNode + 1;
}

void AI_EvalFunc_TIMELock_C()
{
    TIM_gf_LockBegTime = TIM_gf_MainClock;
}

tAINode* AI_EvalFunc_TIMELock(tAINode* aiNode)
{
    AI_EvalFunc_TIMELock_C();
    return aiNode + 1;
}

void AI_EvalFunc_TIMEUnlock_C()
{
    TIM_gf_LockTime = TIM_gf_MainClock - TIM_gf_LockBegTime + TIM_gf_LockTime;
}

tAINode* AI_EvalFunc_TIMEUnlock(tAINode* aiNode)
{
    AI_EvalFunc_TIMEUnlock_C();
    return aiNode + 1;
}