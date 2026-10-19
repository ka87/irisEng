#pragma once
#include "common.h"
#include "gameobject.h"

int IMG_Log;
int IMG_PhotoSummary;
struct sGameObjectMain* IMG_Gao;

int IMG_Analyser(tWorld* a1, unsigned int a2);
unsigned int IMG_Analyser_Bis(tWorld* a1, int a2);
int IMG_ObjectAnalyser(struct sGameObjectMain* a1, struct sWorld* a2, struct sGdi_DisplayData* a3, int a4, int a51, int a52, int a6, int* a7);