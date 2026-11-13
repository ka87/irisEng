#pragma once
#include "common.h"
#include "gameobject.h"

int gi_NumSectors;
tGameObjectMain* gapst_AllObjSectors[5000];
tGameObjectMain* gapst_AllSectors[50];
int gai_NumObjSectors[50];

void AI_EvalFunc_GRPSectUpdateMe_C(tGameObjectMain* gao);
void GRPSectDelObj(tGameObjectMain* gao);
void* GRPSectClearAll();