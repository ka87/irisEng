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

void WAY_FreeStruct(struct sWay* way);
void WAY_ReinitLinks(struct sWay* way);
void WAY_RemoveAllNetworks(tWayAllNetworks* allNetworks);
tWayAllNetworks* WAY_ul_AllNetWorksCallback(uint32_t offset);
void WAY_InitModule();
void WAY_CloseModule();
struct sWayElem* WAY_pst_SearchLinkList(struct sGameObjectMain* wp, struct sWayNetwork* network);
struct sWayElemInnerElem* WAY_pst_SearchOneLinkInList(struct sWayElem* linkList, struct sGameObjectMain* wp);
void WAY_ComputePoundNetworks(struct sWorld* world);
struct sWay* WAY_ul_AllLinkListsCallback(uint32_t offset);
