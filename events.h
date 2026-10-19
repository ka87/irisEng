#pragma once
#include "gameobject.h"
#include "common.h"

typedef union uEventElemElemPtrData {
    struct s {
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
    byte* dataPtr;
    int size;
    unsigned __int16 elemNum;
    __int16 flags;
    unsigned __int16 boneID;
    __int16 s3;
} tGameObjectEventsListElem;

typedef struct sGameObjectEventsList
{
    struct sGameObjectEventsListElem* listElems;
    unsigned __int16 num;
    __int16 flags;
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

void EVE_DeleteListTracks(tGameObjectEventsList* a1);
void EVE_FreeMainStruct(struct sGameObjectMain* a1);
int EVE_b_RunAllTracks(tEventData* a1, float* a2);
struct sEventData* EVE_AllocateMainStruct(struct sGameObjectMain* a1);
void EVE_InitData(tEventData* a1, tGameObjectEventsList* a2);
void EVE_GetTracksTime(struct sEventData* a1, int a2, float* currTime, float* fullTime);
void EVE_SetTracksTime(struct sEventData* a1, float time);
int EVE_b_RunAnimation(tEventData* a1, float* a4);
uint16_t EVE_w_Event_InterpolationKey_GetType(struct sGameObjectEventElem* a1);
Vec3* EVE_pst_Event_InterpolationKey_GetPos(tGameObjectEventElem* a1);
void EVE_ResolveRefs(struct sGameObjectMain* a1, int a2);
void EVE_ResolveListTrackRef(tGameObjectEventsList* a1);
void EVE_Duplicate(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3);
struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t Offset);
void EVE_ReinitData(struct sEventData* a1);
void EVE_SetRunningTrack(struct sAnimTracks* a1, int a2);
void EVE_SetRunningTracks(struct sEventData* a1, int a2);
void EVE_SetPauseTrack(struct sAnimTracks* a1, int a2);
void EVE_SetPauseTracks(struct sEventData* a1, int a2);
void EVE_ReinitTrack(struct sEventData* a1, int trackID);
void EVE_SetAllTimesTo(float a1);
int EVE_i_SearchLabel(struct sGameObjectEventsListElem* a1, int a2, float* a3);
void EVE_MainCall(struct sWorld* a1);