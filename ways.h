#pragma once
#include "basic_array.h"
#include "gameobject.h"
#include "world.h"
#include "common.h"

typedef struct sWayNetwork
{
    struct sGameObjectMain* gao;
    int flags;
} tWayNetwork;

typedef struct sWayElemInnerElem
{
    int16_t capa;
    int16_t capaBackup;
    float weight;
    struct sGameObjectMain* gao;
    uint8_t design;
    uint8_t designBackup;
    int16_t s3;
} tWayElemInnerElem;

typedef struct sWayElem
{
    struct sWayNetwork* network;
    struct sWayElemInnerElem* innerElems;
    int num;
} tWayElem;

typedef struct sWay
{
    struct sWayElem* wayElems;
    unsigned int wayElemNum;
} tWay;

typedef struct sWayAllNetworks
{
    struct sWayNetwork** networks;
    int networkNum;
} tWayAllNetworks;

BASStruct WAY_gst_Seen;
struct sWorld* gpst_CurrentWorld;
int WAY_gi_ShortParams;

void WAY_FreeStruct(struct sWay* a1);
void WAY_ReinitLinks(struct sWay* a1);
void WAY_RemoveAllNetworks(tWayAllNetworks* a1);
tWayAllNetworks* WAY_ul_AllNetWorksCallback(uint32_t Offset);
void WAY_InitModule();
void WAY_CloseModule();
struct sWayElem* WAY_pst_SearchLinkList(struct sGameObjectMain* a1, struct sWayNetwork* a2);
struct sWayElemInnerElem* WAY_pst_SearchOneLinkInList(struct sWayElem* a1, struct sGameObjectMain* a2);
void WAY_ComputePoundNetworks(struct sWorld* a1);
struct sWay* WAY_ul_AllLinkListsCallback(uint32_t Offset);
