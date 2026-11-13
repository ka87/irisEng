#pragma once
#include "gameobject.h"
#include "common.h"

typedef union uEventElemElemPtrData {
    struct {
        uint16_t size;
        uint16_t type;
    };
    void* data;
} uEventElemElemPtrData;

typedef struct sGameObjectEventElem
{
    uint16_t duration;
    int16_t flags2;
    //void* elemPtr;
    uEventElemElemPtrData eptrdu;
} tGameObjectEventElem;

typedef struct sGameObjectEventsListElem
{
    struct sGameObjectMain* gao;
    struct sGameObjectEventElem* trackElems;
    uint8_t* dataPtr;
    int size;
    uint16_t elemNum;
    int16_t flags;
    uint16_t boneID;
    int16_t s3;
} tGameObjectEventsListElem;

typedef struct sGameObjectEventsList
{
    struct sGameObjectEventsListElem* listElems;
    uint16_t num;
    int16_t flags;
    int usageCount;
} tGameObjectEventsList;

typedef struct sAnimTracks
{
    float currTime;
    int32_t privateData; // privatedata? ebben sokminden van, pl -1 is kerül meg floatok is, lasd ANI_BeforeRender
    uint16_t currElemKeyFrame;
    int16_t flag;
} tAnimTracks;

typedef struct sEventData
{
    struct sGameObjectEventsList* eventListStruct;
    struct sAnimTracks* tracks;
} tEventData;

typedef struct sEventTimeElem
{
    float start;
    float multiplicator;
} tEventTimeElem;



struct sGameObjectMain* EVE_gpst_OwnerGAO;
struct sGameObjectMain* EVE_gpst_CurrentGAO;
tGameObjectEventsList* EVE_gpst_CurrentListTracks;
tGameObjectEventsListElem* EVE_gpst_CurrentTrack;
tGameObjectEventElem* EVE_gpst_CurrentEvent;
Mtx4x4* EVE_gpst_CurrentRefMatrix;
Mtx4x4* EVE_gpst_SavedMatrix;
tEventData* EVE_gpst_CurrentData;
tAnimTracks* EVE_gpst_CurrentParam;
float EVE_gf_DT;
float EVE_gf_ElapsedDT;
int EVE_gb_NeedToReparse;
int EVE_gb_PlayOnlyAnimTracks;
int EVE_gb_ForcePlayerOnAllTracks;
int AnimSizeOnDisk;
int NumTracks;

int ENG_gb_EVERunning;
tGameObjectEventsList* EVE_gpst_CurrentReolve;

extern int EVE_gb_CanFlash;

void EVE_DeleteListTracks(tGameObjectEventsList* list);
void EVE_FreeMainStruct(struct sGameObjectMain* gao);
int EVE_b_RunAllTracks(tEventData* eve, float* dtLeft);
struct sEventData* EVE_AllocateMainStruct(struct sGameObjectMain* gao);
void EVE_InitData(tEventData* eve, tGameObjectEventsList* list);
void EVE_GetTracksTime(struct sEventData* eve, int trackIdx, float* currTime, float* fullTime);
void EVE_SetTracksTime(struct sEventData* eve, float time);
int EVE_b_RunAnimation(tEventData* eve, float* dtLeft);
uint16_t EVE_w_Event_InterpolationKey_GetType(struct sGameObjectEventElem* key);
Vec3* EVE_pst_Event_InterpolationKey_GetPos(tGameObjectEventElem* key);
void EVE_ResolveRefs(struct sGameObjectMain* gao, int incrementUsageCount);
void EVE_ResolveListTrackRef(tGameObjectEventsList* list);
void EVE_Duplicate(struct sGameObjectMain* srcGao, struct sGameObjectMain* destGao, int wholeEVEWithTrackData);
struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t Offset);
void EVE_ReinitData(struct sEventData* eve);
void EVE_SetRunningTrack(struct sAnimTracks* state, int running);
void EVE_SetRunningTracks(struct sEventData* eve, int running);
void EVE_SetPauseTrack(struct sAnimTracks* state, int paused);
void EVE_SetPauseTracks(struct sEventData* eve, int paused);
void EVE_ReinitTrack(struct sEventData* eve, int trackID);
void EVE_SetAllTimesTo(float time);
int EVE_i_SearchLabel(struct sGameObjectEventsListElem* track, int label, float* labelTime);
void EVE_MainCall(struct sWorld* world);