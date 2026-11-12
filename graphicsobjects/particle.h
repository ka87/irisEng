#pragma once
#include "gro_common.h"

struct sPAGStruct* PAG_pst_Create();
struct sGroGeneric* PAG_p_CreateFromBuffer(struct sGroGeneric* gro, char** readptr, struct sWorld* world);
struct sGroGeneric* PAG_p_Duplicate(struct sGroGeneric* gro);
void PAG_Free(struct sGroGeneric* gro);
void PAG_Reinit(struct sGroGeneric* gro);
void PAG_Init();
struct sPAGStruct* PAG_pst_GetGameObjectParticleGenerator(struct sGameObjectMain* gao);
int PAG_i_AddGenerator(struct sPAGStruct* pag, struct sGameObjectMain* gao, float frequency);
void PAG_DelGenerator(struct sPAGStruct* pag, int generatorID);
void PAG_SetGeneratorFrequency(struct sPAGStruct* pag, int generatorID, float frequency);
