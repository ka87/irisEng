#include "ai_animationfunctions.h"

int AI_EvalFunc_ANIFrequencyGet_C(struct sGameObjectMain* gao, int animIdx)
{
    return ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[animIdx]->frequency;
}

tAINode* AI_EvalFunc_ANIFrequencyGet(tAINode* aiNode)
{
    int animIdx;
    int freq;
    tGameObjectMain* currUltraBackup = AI_gpst_CurrentUltra;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    animIdx = AI_GetIntFromStack();
    freq = AI_EvalFunc_ANIFrequencyGet_C(currUltraBackup, animIdx);
    AI_PutIntToStack(freq);
    return aiNode + 1;
}

int AI_EvalFunc_ANIFrequencySet_C(struct sGameObjectMain* gao, int animIdx, int freq)
{
    unsigned int freqCalcd;

    freqCalcd = (unsigned int)((float)freq * 60.0f / 64.0f + 0.5f);
    ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[animIdx]->frequency = freqCalcd;

    return freqCalcd;
}

tAINode* AI_EvalFunc_ANIFrequencySet(tAINode* aiNode)
{
    int animIdx, freq;
    tGameObjectMain* currUltraBackup;

    currUltraBackup = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    freq = AI_GetIntFromStack();
    animIdx = AI_GetIntFromStack();

    AI_EvalFunc_ANIFrequencySet_C(currUltraBackup, animIdx, freq);

    return aiNode + 1;
}

unsigned int AI_EvalFunc_ANICurrentFrameGet_C(struct sGameObjectMain* gao, int animIdx)
{
    float currTime, fullTime;

    EVE_GetTracksTime(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[animIdx]->ptr, 0, &currTime, &fullTime);

    return (unsigned int)(currTime * 60.0f);
}

tAINode* AI_EvalFunc_ANICurrentFrameGet(tAINode* node)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_ANICurrentFrameGet_C(v4, v1);
    AI_PutIntToStack(v2);
    return node + 1;
}

void AI_EvalFunc_ANICurrentFrameSet_C(struct sGameObjectMain* gao, int a2, int a3)
{
    float a2a;
    float v4; 

    v4 = (float)a3;
    a2a = (float)(v4 * 0.016666668);
    EVE_SetTracksTime(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2]->ptr, a2a);
}

tAINode* AI_EvalFunc_ANICurrentFrameSet(tAINode* node)
{
    int v1;
    int v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_ANICurrentFrameSet_C(v4, v1, v3);
    return node + 1;
}

unsigned int AI_EvalFunc_ANINbFrameGet_C(struct sGameObjectMain* gao, int a2)
{
    float a4;
    float a3;

    EVE_GetTracksTime(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2]->ptr, 0, &a3, &a4);
    return (unsigned int)(a4 * 60.0);
}

tAINode* AI_EvalFunc_ANINbFrameGet(tAINode* node)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_ANINbFrameGet_C(v4, v1);
    AI_PutIntToStack(v2);
    return node + 1;
}

int ret_negative_1a() {
    return -1;
}

tAINode* AI_EvalFunc_ANIAnimGet(tAINode* node)
{
    int v1;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetIntFromStack();
    v1 = ret_negative_1a();
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_ANIFlagGet_C(tGameObjectMain* gao, int a2)
{
    return ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2]->flags;
}

tAINode* AI_EvalFunc_ANIFlagGet(tAINode* node)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_ANIFlagGet_C(v4, v1);
    AI_PutIntToStack(v2);
    return node + 1;
}

void AI_EvalFunc_ANIFlagSet_C(tGameObjectMain* gao, int a2, int16_t a3, int16_t a4)
{
    tAnim* v4;

    v4 = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2];
    v4->flags |= a3;
    v4->flags &= ~a4;
}

tAINode* AI_EvalFunc_ANIFlagSet(tAINode* node)
{
    int v1;
    int16_t v3;
    tGameObjectMain* v4;
    int16_t a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_ANIFlagSet_C(v4, v1, v3, a4);
    return node + 1;
}

int AI_EvalFunc_ANIFlagTest_C(tGameObjectMain* gao, int a2, uint16_t a3)
{
    return a3 & ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2]->flags;
}

tAINode* AI_EvalFunc_ANIFlagTest(tAINode* node)
{
    int v1;
    int v2;
    uint16_t v4;
    tGameObjectMain* v5;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_ANIFlagTest_C(v5, v1, v4);
    AI_PutIntToStack(v2);
    return node + 1;
}

int AI_EvalFunc_ANIModeGet_C(tGameObjectMain* gao, int a2)
{
    return (((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2]->flags & 0xF) >> 1;
}

tAINode* AI_EvalFunc_ANIModeGet(tAINode* node)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_ANIModeGet_C(v4, v1);
    AI_PutIntToStack(v2);
    return node + 1;
}

void AI_EvalFunc_ANIModeSet_C(tGameObjectMain* gao, int a2, int16_t a3)
{
    tAnim* v3;

    v3 = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[a2];
    v3->flags &= 0xFFF0u;
    v3->flags |= 2 * a3;
}

tAINode* AI_EvalFunc_ANIModeSet(tAINode* node)
{
    int v1;
    int16_t v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_ANIModeSet_C(v4, v1, v3);
    return node + 1;
}

void AI_EvalFunc_ANIShapeSelect_C(struct sGameObjectMain* gao, uint8_t a2, uint8_t a3)
{
    ANI_ChangeBoneShape(gao, a2, a3, 1);
}

tAINode* AI_EvalFunc_ANIShapeSelect(tAINode* node)
{
    tGameObjectMain* v2;
    uint8_t v3;
    uint8_t v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    AI_EvalFunc_ANIShapeSelect_C(v2, v3, v4);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C(struct sGameObjectMain* gao, uint8_t a2)
{
    return ANI_pst_GetObjectByAICanal(gao, a2);
}

struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C_CURRENT(uint8_t a1)
{
    return ANI_pst_GetObjectByAICanal(AI_gpst_CurrentGameObject, a1);
}

tAINode* AI_EvalFunc_ANICanalGaoGet(tAINode* node)
{
    int v2[6];
    tGameObjectMain* v3;
    uint8_t v4;
    tGameObjectMain* a1a;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v4 = AI_GetIntFromStack();
    v2[0] = (int)AI_EvalFunc_ANICanalGaoGet_C(v3, v4);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return node + 1;
}

BOOL AI_EvalFunc_ANIIsInBlend_C(tGameObjectMain* gao)
{
    return (((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->flagsEgBlendIw2 & 2) != 0;
}

BOOL AI_EvalFunc_ANIIsInBlend_C_CURRENT()
{
    return (((tGameObjectActorObject*)(AI_gpst_CurrentGameObject->basic->actorObjectOrVisualObject))->flagsEgBlendIw2 & 2) != 0;
}

tAINode* AI_EvalFunc_ANIIsInBlend(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ANIIsInBlend_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_ANISetEnd_C(tGameObjectMain* gao)
{
    float v1;
    tEventData* v2;
    int i;
    int v4;

    v2 = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[0]->ptr;
    for (i = 0; i < v2->eventListStruct->num; ++i)
    {
        if (v2->eventListStruct->listElems[i].elemNum)
        {
            v4 = v2->eventListStruct->listElems[i].elemNum - 1;
            v2->tracks[i].currElemKeyFrame = v4;
            v1 = (float)v2->eventListStruct->listElems[i].trackElems[v4].duration;
            v2->tracks[i].currTime = (float)(v1 * 1.0 / 60.0);
        }
    }
}

tAINode* AI_EvalFunc_ANISetEnd(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ANISetEnd_C(v2);
    return node + 1;
}

void AI_EvalFunc_ANISkeletonChange_C(tGameObjectMain* gao, tGameObjectMain* a2)
{
    ANI_ChangeDefaultSkeleton(gao, a2);
}

tAINode* AI_EvalFunc_ANISkeletonChange(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_ANISkeletonChange_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return node + 1;
}

void AI_EvalFunc_ANISkeletonRestore_C(tGameObjectMain* gao, tGameObjectMain* a2)
{
    ANI_RestoreDefaultSkeleton(gao, a2);
}

tAINode* AI_EvalFunc_ANISkeletonRestore(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_ANISkeletonRestore_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return node + 1;
}

void AI_EvalFunc_ANICloneSet_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2, int a3)
{
    ANI_CloneSet(gao, a2, a3);
}

tAINode* AI_EvalFunc_ANICloneSet(tAINode* node)
{
    tGameObjectMain* v2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_ANICloneSet_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3);
    return node + 1;
}

int AI_EvalFunc_ANIShapeGet_C(struct sGameObjectMain* gao, char a2)
{
    int result;
    uint8_t v3; 

    if (!gao
        || (gao->flags & 2) == 0
        || !gao->basic
        || !gao->basic->actorObjectOrVisualObject
        || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->shape)
    {
        return -1;
    }
    v3 = ANI_uc_GetEngineCanalByAICanal(gao, a2);
    if (v3 == 255)
        result = -1;
    else
        result = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->shapes[v3];
    return result;
}

tAINode* AI_EvalFunc_ANIShapeGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_ANIShapeGet_C(v3, v4);
    AI_PutIntToStack(v1);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_ANIShapeObjectGet_C(struct sGameObjectMain* gao, char a2, uint8_t a3)
{
    uint8_t v4;
    tGameObjectMain* v5;

    if (!gao
        || (gao->flags & 2) == 0
        || !gao->basic
        || !gao->basic->actorObjectOrVisualObject
        || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->shape)
    {
        return 0;
    }
    v5 = ANI_pst_GetObjectByAICanal(gao, a2);
    if (!v5)
        return 0;
    v4 = ANI_uc_GetEngineCanalByAICanal(gao, a2);
    if (v4 == 255)
        return 0;
    if (a3 == 255)
    {
        a3 = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->shapes[v4];
        if (a3 == 255)
            return 0;
    }
    if (v5->extended && v5->extended->grp && v5->extended->grp->tablePF->actElemNum > a3)
        return (tGameObjectMain*)((tableElemStructPF*)(v5->extended->grp->tablePF->firstElemPtr))[a3].objectPtr;
    return 0;
}

tAINode* AI_EvalFunc_ANIShapeObjectGet(tAINode* arg0)
{
    int v2[8];
    tGameObjectMain* v3;
    char v4;
    uint8_t v5;
    tGameObjectMain* a1;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v3;
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v2[0] = (int)AI_EvalFunc_ANIShapeObjectGet_C(v3, v4, v5);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

void AI_EvalFunc_ANIBitFieldLODSet_C(tGameObjectMain* gao, char a2)
{
    if (gao && (gao->flags & 2) != 0 && gao->basic)
    {
        if (gao->basic->actorObjectOrVisualObject)
            ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->aniBitFieldLOD = a2;
    }
}

tAINode* AI_EvalFunc_ANIBitFieldLODSet(tAINode* node)
{
    tGameObjectMain* v2;
    char v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    AI_EvalFunc_ANIBitFieldLODSet_C(v2, v3);
    return node + 1;
}

void AI_EvalFunc_ANILODSet_C(tGameObjectMain* gao, char a2)
{
    if (gao && (gao->flags & 2) != 0 && gao->basic)
    {
        if (gao->basic->actorObjectOrVisualObject)
            ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->ANILOD = a2;
    }
}

tAINode* AI_EvalFunc_ANILODSet(tAINode* node)
{
    tGameObjectMain* v2;
    char v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    AI_EvalFunc_ANILODSet_C(v2, v3);
    return node + 1;
}

int AI_EvalFunc_ANIIsBoneAnimed_C(struct sGameObjectMain* gao, uint8_t a2)
{
    tGameObjectEventsList* v3;
    int i;
    tGameObjectMain* v5;

    v5 = ANI_pst_GetObjectByAICanal(gao, a2);
    if (!v5)
        return 0;
    if (!gao
        || !gao->basic
        || !gao->basic->actorObjectOrVisualObject
        || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup
        || !((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[0])
    {
        return 0;
    }
    v3 = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[0]->ptr->eventListStruct;
    for (i = 0; i < v3->num; ++i)
    {
        if (v5 == ((tableElemStructPF*)(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->firstElemPtr))[v3->listElems[i].boneID].objectPtr)
            return 1;
    }
    return 0;
}

tAINode* AI_EvalFunc_ANIIsBoneAnimed(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    uint8_t a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_ANIIsBoneAnimed_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_ANIBlendFrameMaxSet_C(struct sGameObjectMain* gao, int a2)
{
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
    float v7;
    float v8;
    float v9;

    if (gao && gao->basic && gao->basic->actorObjectOrVisualObject && ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[1])
    {
        v4 = (float)(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[1]->actionLength + 1);
        v7 = (float)(v4 / 60.0);
        v3 = (float)((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[1]->startTime;
        v9 = (float)(v3 / 15360.0);
        v6 = v7 - v9;
        if ((uint8_t)(v6 * 60.0 + 0.5) > (uint8_t)a2)
        {
            v2 = (float)a2;
            v5 = (float)(v9 / v7 / (1.0 - v9 / v7) * (v2 * 1.0 / 60.0));
            v8 = (float)(v2 * 1.0 / 60.0 + v5);
            ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[1]->actionLength = (unsigned int)(v8 * 60.0 + 0.5);
            ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[1]->startTime = (uint16_t)(uint32_t)(v5 * 60.0 + 0.5) << 8;
        }
    }
}

tAINode* AI_EvalFunc_ANIBlendFrameMaxSet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_ANIBlendFrameMaxSet_C(v3, v1);
    return node + 1;
}

void AI_EvalFunc_ANIPelvisOffsetSet_C(struct sGameObjectMain* gao, float a2)
{
    if (gao && (gao->flags & 2) != 0 && gao->basic)
    {
        if (gao->basic->actorObjectOrVisualObject)
            ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->aniPelvisOffset = a2;
    }
}

tAINode* AI_EvalFunc_ANIPelvisOffsetSet(tAINode* node)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_ANIPelvisOffsetSet_C(v2, a2);
    return node + 1;
}