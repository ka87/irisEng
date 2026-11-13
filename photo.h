#pragma once
#include "common.h"
#include "gameobject.h"

int IMG_Log;
int IMG_PhotoSummary;
struct sGameObjectMain* IMG_Gao;

int IMG_Analyser(tWorld* world, unsigned int mission);
unsigned int IMG_Analyser_Bis(tWorld* world, int mission);
int IMG_ObjectAnalyser(struct sGameObjectMain* gao, struct sWorld* world, struct sGdi_DisplayData* dd, int missionMask, int checkVisibility, int rawInfo, int ignoreMission, int* code);