#pragma once
#include "tables.h"
#include "common.h"

typedef struct sGroup
{
    tableDescStruct* tablePF;
    int unk2;
    int groupElemNum;
} tGroup;

void GRP_ObjDetachGroup(struct sGameObjectMain* a1);
void GRP_DuplicateGroupObjects(struct sWorld* a1, struct sGroup* a2, struct sGroup* a3);
struct sGroup* GRP_pst_CreateNewGroup();
struct sGroup* GRP_ul_LoadGrpCallback(uint32_t Offset);