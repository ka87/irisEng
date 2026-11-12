#include "ai_animationfunctions.h"

// An animation builtin works on the actor behind the game object.
static tGameObjectActorObject* AI_l_Actor(struct sGameObjectMain* gao)
{
    return (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
}

static tGameObjectActorObject* AI_l_ActorOrNull(struct sGameObjectMain* gao)
{
    if (!gao || !gao->basic || !gao->basic->actorObjectOrVisualObject)
        return NULL;
    return AI_l_Actor(gao);
}

// The same, for the builtins that also want the object to carry the
// actor flag.
static tGameObjectActorObject* AI_l_AnimatedActor(struct sGameObjectMain* gao)
{
    if (!gao || (gao->flags & 2) == 0)
        return NULL;
    return AI_l_ActorOrNull(gao);
}

int AI_EvalFunc_ANIFrequencyGet_C(struct sGameObjectMain* gao, int animIdx)
{
    return AI_l_Actor(gao)->animPtr[animIdx]->frequency;
}

tAINode* AI_EvalFunc_ANIFrequencyGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    animIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANIFrequencyGet_C(gaoUltra, animIdx));
    return node + 1;
}

// The script gives a frequency in frames; the actor keeps it in the
// animation's own 64ths.
int AI_EvalFunc_ANIFrequencySet_C(struct sGameObjectMain* gao, int animIdx, int freq)
{
    unsigned int scaled;

    scaled = (unsigned int)((float)freq * 60.0f / 64.0f + 0.5f);
    AI_l_Actor(gao)->animPtr[animIdx]->frequency = scaled;
    return scaled;
}

tAINode* AI_EvalFunc_ANIFrequencySet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;
    int freq;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    freq = AI_GetIntFromStack();
    animIdx = AI_GetIntFromStack();
    AI_EvalFunc_ANIFrequencySet_C(gaoUltra, animIdx, freq);
    return node + 1;
}

unsigned int AI_EvalFunc_ANICurrentFrameGet_C(struct sGameObjectMain* gao, int animIdx)
{
    float currTime;
    float fullTime;

    EVE_GetTracksTime(AI_l_Actor(gao)->animPtr[animIdx]->ptr, 0, &currTime, &fullTime);
    return (unsigned int)(currTime * 60.0f);
}

tAINode* AI_EvalFunc_ANICurrentFrameGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    animIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANICurrentFrameGet_C(gaoUltra, animIdx));
    return node + 1;
}

void AI_EvalFunc_ANICurrentFrameSet_C(struct sGameObjectMain* gao, int animIdx, int frame)
{
    float frames;
    float time;

    frames = (float)frame;
    time = (float)(frames * 0.016666668);
    EVE_SetTracksTime(AI_l_Actor(gao)->animPtr[animIdx]->ptr, time);
}

tAINode* AI_EvalFunc_ANICurrentFrameSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;
    int frame;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    frame = AI_GetIntFromStack();
    animIdx = AI_GetIntFromStack();
    AI_EvalFunc_ANICurrentFrameSet_C(gaoUltra, animIdx, frame);
    return node + 1;
}

unsigned int AI_EvalFunc_ANINbFrameGet_C(struct sGameObjectMain* gao, int animIdx)
{
    float currTime;
    float fullTime;

    EVE_GetTracksTime(AI_l_Actor(gao)->animPtr[animIdx]->ptr, 0, &currTime, &fullTime);
    return (unsigned int)(fullTime * 60.0);
}

tAINode* AI_EvalFunc_ANINbFrameGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    animIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANINbFrameGet_C(gaoUltra, animIdx));
    return node + 1;
}

// A stub: the builtin takes its argument and always reports that there
// is no animation.
tAINode* AI_EvalFunc_ANIAnimGet(tAINode* node)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetIntFromStack();
    AI_PutIntToStack(-1);
    return node + 1;
}

int AI_EvalFunc_ANIFlagGet_C(tGameObjectMain* gao, int animIdx)
{
    return AI_l_Actor(gao)->animPtr[animIdx]->flags;
}

tAINode* AI_EvalFunc_ANIFlagGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    animIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANIFlagGet_C(gaoUltra, animIdx));
    return node + 1;
}

void AI_EvalFunc_ANIFlagSet_C(tGameObjectMain* gao, int animIdx, int16_t toSet, int16_t toClear)
{
    tAnim* anim;

    anim = AI_l_Actor(gao)->animPtr[animIdx];
    anim->flags |= toSet;
    anim->flags &= ~toClear;
}

tAINode* AI_EvalFunc_ANIFlagSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;
    int16_t toSet;
    int16_t toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    animIdx = AI_GetIntFromStack();
    AI_EvalFunc_ANIFlagSet_C(gaoUltra, animIdx, toSet, toClear);
    return node + 1;
}

int AI_EvalFunc_ANIFlagTest_C(tGameObjectMain* gao, int animIdx, uint16_t mask)
{
    return mask & AI_l_Actor(gao)->animPtr[animIdx]->flags;
}

tAINode* AI_EvalFunc_ANIFlagTest(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;
    uint16_t mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    animIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANIFlagTest_C(gaoUltra, animIdx, mask));
    return node + 1;
}

// The play mode is the three bits above the flag word's lowest.
int AI_EvalFunc_ANIModeGet_C(tGameObjectMain* gao, int animIdx)
{
    return (AI_l_Actor(gao)->animPtr[animIdx]->flags & 0xF) >> 1;
}

tAINode* AI_EvalFunc_ANIModeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    animIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANIModeGet_C(gaoUltra, animIdx));
    return node + 1;
}

void AI_EvalFunc_ANIModeSet_C(tGameObjectMain* gao, int animIdx, int16_t mode)
{
    tAnim* anim;

    anim = AI_l_Actor(gao)->animPtr[animIdx];
    anim->flags &= 0xFFF0u;
    anim->flags |= 2 * mode;
}

tAINode* AI_EvalFunc_ANIModeSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int animIdx;
    int16_t mode;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mode = AI_GetIntFromStack();
    animIdx = AI_GetIntFromStack();
    AI_EvalFunc_ANIModeSet_C(gaoUltra, animIdx, mode);
    return node + 1;
}

void AI_EvalFunc_ANIShapeSelect_C(struct sGameObjectMain* gao, uint8_t aiCanal, uint8_t shapeIdx)
{
    ANI_ChangeBoneShape(gao, aiCanal, shapeIdx, 1);
}

tAINode* AI_EvalFunc_ANIShapeSelect(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t aiCanal;
    uint8_t shapeIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    shapeIdx = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    AI_EvalFunc_ANIShapeSelect_C(gaoUltra, aiCanal, shapeIdx);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C(struct sGameObjectMain* gao, uint8_t aiCanal)
{
    return ANI_pst_GetObjectByAICanal(gao, aiCanal);
}

struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C_CURRENT(uint8_t aiCanal)
{
    return ANI_pst_GetObjectByAICanal(AI_gpst_CurrentGameObject, aiCanal);
}

tAINode* AI_EvalFunc_ANICanalGaoGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* bone;
    uint8_t aiCanal;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    aiCanal = AI_GetIntFromStack();
    bone = AI_EvalFunc_ANICanalGaoGet_C(gaoUltra, aiCanal);
    AI_PutObjectToStack(&bone, AI_gaw_EnumLink[40]);
    return node + 1;
}

BOOL AI_EvalFunc_ANIIsInBlend_C(tGameObjectMain* gao)
{
    return (AI_l_Actor(gao)->flagsEgBlendIw2 & 2) != 0;
}

BOOL AI_EvalFunc_ANIIsInBlend_C_CURRENT()
{
    return AI_EvalFunc_ANIIsInBlend_C(AI_gpst_CurrentGameObject);
}

tAINode* AI_EvalFunc_ANIIsInBlend(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ANIIsInBlend_C(gaoUltra));
    return node + 1;
}

// Puts every track of the actor's first animation on its last key
// frame, the same way TRASetEnd does for one track of an event list.
void AI_EvalFunc_ANISetEnd_C(tGameObjectMain* gao)
{
    tEventData* eve;
    int lastKeyFrame;
    float frames;
    int i;

    eve = AI_l_Actor(gao)->animPtr[0]->ptr;
    for (i = 0; i < eve->eventListStruct->num; ++i)
    {
        if (!eve->eventListStruct->listElems[i].elemNum)
            continue;

        lastKeyFrame = eve->eventListStruct->listElems[i].elemNum - 1;
        eve->tracks[i].currElemKeyFrame = lastKeyFrame;
        frames = (float)eve->eventListStruct->listElems[i].trackElems[lastKeyFrame].duration;
        eve->tracks[i].currTime = (float)(frames / 60.0);
    }
}

tAINode* AI_EvalFunc_ANISetEnd(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ANISetEnd_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_ANISkeletonChange_C(tGameObjectMain* gao, tGameObjectMain* skeleton)
{
    ANI_ChangeDefaultSkeleton(gao, skeleton);
}

tAINode* AI_EvalFunc_ANISkeletonChange(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ANISkeletonChange_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

void AI_EvalFunc_ANISkeletonRestore_C(tGameObjectMain* gao, tGameObjectMain* skeleton)
{
    ANI_RestoreDefaultSkeleton(gao, skeleton);
}

tAINode* AI_EvalFunc_ANISkeletonRestore(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ANISkeletonRestore_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

void AI_EvalFunc_ANICloneSet_C(struct sGameObjectMain* gao, struct sGameObjectMain* source, int clone)
{
    ANI_CloneSet(gao, source, clone);
}

tAINode* AI_EvalFunc_ANICloneSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* source;
    int clone;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    clone = AI_GetIntFromStack();
    source = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalFunc_ANICloneSet_C(gaoUltra, source, clone);
    return node + 1;
}

int AI_EvalFunc_ANIShapeGet_C(struct sGameObjectMain* gao, char aiCanal)
{
    tGameObjectActorObject* actor;
    uint8_t engineCanal;

    actor = AI_l_AnimatedActor(gao);
    if (!actor || !actor->shape)
        return -1;

    engineCanal = ANI_uc_GetEngineCanalByAICanal(gao, aiCanal);
    if (engineCanal == 255)
        return -1;
    return actor->shapes[engineCanal];
}

tAINode* AI_EvalFunc_ANIShapeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char aiCanal;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    aiCanal = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANIShapeGet_C(gaoUltra, aiCanal));
    return node + 1;
}

// The object standing in for one shape of a bone. A shape index of 255
// means the one the bone is wearing.
struct sGameObjectMain* AI_EvalFunc_ANIShapeObjectGet_C(struct sGameObjectMain* gao, char aiCanal, uint8_t shapeIdx)
{
    tGameObjectActorObject* actor;
    tGameObjectMain* bone;
    uint8_t engineCanal;

    actor = AI_l_AnimatedActor(gao);
    if (!actor || !actor->shape)
        return NULL;

    bone = ANI_pst_GetObjectByAICanal(gao, aiCanal);
    if (!bone)
        return NULL;

    engineCanal = ANI_uc_GetEngineCanalByAICanal(gao, aiCanal);
    if (engineCanal == 255)
        return NULL;

    if (shapeIdx == 255)
    {
        shapeIdx = actor->shapes[engineCanal];
        if (shapeIdx == 255)
            return NULL;
    }

    if (bone->extended && bone->extended->grp && bone->extended->grp->tablePF->actElemNum > shapeIdx)
        return (tGameObjectMain*)((tableElemStructPF*)(bone->extended->grp->tablePF->firstElemPtr))[shapeIdx].objectPtr;
    return NULL;
}

tAINode* AI_EvalFunc_ANIShapeObjectGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* shape;
    char aiCanal;
    uint8_t shapeIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    shapeIdx = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    shape = AI_EvalFunc_ANIShapeObjectGet_C(gaoUltra, aiCanal, shapeIdx);
    AI_PutObjectToStack(&shape, AI_gaw_EnumLink[40]);
    return node + 1;
}

void AI_EvalFunc_ANIBitFieldLODSet_C(tGameObjectMain* gao, char lod)
{
    tGameObjectActorObject* actor;

    actor = AI_l_AnimatedActor(gao);
    if (actor)
        actor->aniBitFieldLOD = lod;
}

tAINode* AI_EvalFunc_ANIBitFieldLODSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char lod;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    lod = AI_GetIntFromStack();
    AI_EvalFunc_ANIBitFieldLODSet_C(gaoUltra, lod);
    return node + 1;
}

void AI_EvalFunc_ANILODSet_C(tGameObjectMain* gao, char lod)
{
    tGameObjectActorObject* actor;

    actor = AI_l_AnimatedActor(gao);
    if (actor)
        actor->ANILOD = lod;
}

tAINode* AI_EvalFunc_ANILODSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char lod;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    lod = AI_GetIntFromStack();
    AI_EvalFunc_ANILODSet_C(gaoUltra, lod);
    return node + 1;
}

// Whether the bone on an AI canal is one the actor's first animation
// moves.
int AI_EvalFunc_ANIIsBoneAnimed_C(struct sGameObjectMain* gao, uint8_t aiCanal)
{
    tGameObjectActorObject* actor;
    tGameObjectEventsList* tracks;
    tGameObjectMain* bone;
    tableElemStructPF* bones;
    int i;

    bone = ANI_pst_GetObjectByAICanal(gao, aiCanal);
    if (!bone)
        return 0;

    actor = AI_l_ActorOrNull(gao);
    if (!actor || !actor->skeletonGroup || !actor->animPtr[0])
        return 0;

    tracks = actor->animPtr[0]->ptr->eventListStruct;
    bones = (tableElemStructPF*)(actor->skeletonGroup->tablePF->firstElemPtr);
    for (i = 0; i < tracks->num; ++i)
    {
        if (bone == bones[tracks->listElems[i].boneID].objectPtr)
            return 1;
    }
    return 0;
}

tAINode* AI_EvalFunc_ANIIsBoneAnimed(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t aiCanal;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    aiCanal = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ANIIsBoneAnimed_C(gaoUltra, aiCanal));
    return node + 1;
}

// Shortens the blend animation to at most frameMax frames, keeping the
// ratio between its lead-in and its length. The actor holds the length
// in frames and the lead-in in 256ths of a frame.
void AI_EvalFunc_ANIBlendFrameMaxSet_C(struct sGameObjectMain* gao, int frameMax)
{
    tGameObjectActorObject* actor;
    tAnim* blend;
    float length;
    float totalTime;
    float lead;
    float leadTime;
    float playTime;
    float frames;
    float newLeadTime;
    float newTotalTime;

    actor = AI_l_ActorOrNull(gao);
    if (!actor || !actor->animPtr[1])
        return;

    blend = actor->animPtr[1];
    length = (float)(blend->actionLength + 1);
    totalTime = (float)(length / 60.0);
    lead = (float)blend->startTime;
    leadTime = (float)(lead / 15360.0);
    playTime = totalTime - leadTime;

    if ((uint8_t)(playTime * 60.0 + 0.5) <= (uint8_t)frameMax)
        return;

    frames = (float)frameMax;
    newLeadTime = (float)(leadTime / totalTime / (1.0 - leadTime / totalTime) * (frames * 1.0 / 60.0));
    newTotalTime = (float)(frames * 1.0 / 60.0 + newLeadTime);
    blend->actionLength = (unsigned int)(newTotalTime * 60.0 + 0.5);
    blend->startTime = (uint16_t)(uint32_t)(newLeadTime * 60.0 + 0.5) << 8;
}

tAINode* AI_EvalFunc_ANIBlendFrameMaxSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int frameMax;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    frameMax = AI_GetIntFromStack();
    AI_EvalFunc_ANIBlendFrameMaxSet_C(gaoUltra, frameMax);
    return node + 1;
}

void AI_EvalFunc_ANIPelvisOffsetSet_C(struct sGameObjectMain* gao, float offset)
{
    tGameObjectActorObject* actor;

    actor = AI_l_AnimatedActor(gao);
    if (actor)
        actor->aniPelvisOffset = offset;
}

tAINode* AI_EvalFunc_ANIPelvisOffsetSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float offset;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    offset = AI_GetFloatFromStack();
    AI_EvalFunc_ANIPelvisOffsetSet_C(gaoUltra, offset);
    return node + 1;
}
