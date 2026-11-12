#include "../sectors.h"
#include "../collision.h"
#include "../engine.h"
#include "ai_debugfunctions.h"
#include "ai_sectorfunctions.h"

// A sector's objects live in a fixed hundred-wide slice of one flat
// array.
#define AI_SECTOR_CAPACITY 100

// A table entry is in use when it has a pointer and its low two bits
// do not carry the free tag 01.
static BOOL AI_l_ObjectInUse(void* objectPtr)
{
    return objectPtr && ((uintptr_t)objectPtr & 3) != 1;
}

// Takes the object out of the sector it is in: the list's last entry
// moves into the slot it leaves behind.
static void AI_l_RemoveFromSector(tGameObjectMain* gao, uint16_t sectorID)
{
    int base;

    base = AI_SECTOR_CAPACITY * sectorID;
    gapst_AllObjSectors[base + gai_NumObjSectors[sectorID] - 1]->extended->sectorElemIdx =
        gao->extended->sectorElemIdx;
    gapst_AllObjSectors[base + gao->extended->sectorElemIdx] =
        gapst_AllObjSectors[base + (gai_NumObjSectors[sectorID]--) - 1];
}

static void AI_l_AddToSector(tGameObjectMain* gao, int sectorID)
{
    gapst_AllObjSectors[AI_SECTOR_CAPACITY * sectorID + gai_NumObjSectors[sectorID]] = gao;
    gao->extended->sectorID = sectorID;
    gao->extended->sectorElemIdx = gai_NumObjSectors[sectorID];
    ++gai_NumObjSectors[sectorID];
}

// Moves the object into whichever registered sector its pivot is in,
// and out of every sector when it is in none. The object inherits its
// sector's invisibility count either way.
void AI_EvalFunc_GRPSectUpdateMe_C(tGameObjectMain* gao)
{
    uint16_t sectorID;
    int i;

    if ((gao->flags2 & 0x10000000) == 0)
    {
        for (i = 0; i < gi_NumSectors; ++i)
        {
            if (!COL_BV_PivotCollide(gapst_AllSectors[i], gao, FALSE))
                continue;

            sectorID = gao->extended->sectorID;
            if (i != sectorID)
            {
                // the unset sector id is 0xFFFF, and the field is a
                // uint16_t, so the original's second test for -1 could
                // never fire
                if (sectorID != 0xFFFF)
                    AI_l_RemoveFromSector(gao, sectorID);
                AI_l_AddToSector(gao, i);
            }
            gao->if_this_not0_wont_render_gao = gapst_AllSectors[i]->if_this_not0_wont_render_gao;
            return;
        }
    }

    sectorID = gao->extended->sectorID;
    if (sectorID != 0xFFFF)
    {
        AI_l_RemoveFromSector(gao, sectorID);
        gao->extended->sectorID = 0xffff; // -1
        gao->extended->sectorElemIdx = 0xffff; // -1
        gao->if_this_not0_wont_render_gao = 0;
    }
}

int AI_EvalFunc_GRPSectRegister_C(struct sGameObjectMain* gao)
{
    gapst_AllSectors[gi_NumSectors] = gao;
    gai_NumObjSectors[gi_NumSectors++] = 0;
    return gi_NumSectors - 1;
}

tAINode* AI_EvalFunc_GRPSectRegister(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int enable;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    enable = *(int32_t*)AI_PopStackSlot();
    if (enable)
        AI_PutIntToStack(AI_EvalFunc_GRPSectRegister_C(gaoUltra));
    else
        AI_PutIntToStack(-1);
    return node + 1;
}

// An object is invisible while its count is above zero, so the script
// raises and lowers a count rather than setting a flag; -1 clears it
// outright.
static void AI_l_ApplyInvisible(tGameObjectMain* gao, int invisible)
{
    if (invisible == -1)
        gao->if_this_not0_wont_render_gao = 0;
    else if (invisible)
        ++gao->if_this_not0_wont_render_gao;
    else if (gao->if_this_not0_wont_render_gao)
        --gao->if_this_not0_wont_render_gao;
}

static tGroup* AI_l_GroupOf(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x800000) == 0 || !gao->extended || !gao->extended->grp)
        return NULL;
    return gao->extended->grp;
}

void AI_EvalFunc_GRPSectInvisibleSet_C(struct sGameObjectMain* gao, int sectorID, int invisible)
{
    tGroup* group;
    tableElemStructPF* entry;
    tableElemStructPF* end;
    tGameObjectMain* sectorObj;
    int i;

    AI_l_ApplyInvisible(gao, invisible);

    group = AI_l_GroupOf(gao);
    if (group)
    {
        end = (tableElemStructPF*)(group->tablePF->nextElemPtr);
        for (entry = group->tablePF->firstElemPtr; entry < end; ++entry)
        {
            if (AI_l_ObjectInUse(entry->objectPtr))
                AI_l_ApplyInvisible((tGameObjectMain*)entry->objectPtr, invisible);
        }
    }

    if (sectorID == 0xFFFF || sectorID == -1)
        return;

    for (i = 0; sectorID < gi_NumSectors && i < gai_NumObjSectors[sectorID]; ++i)
    {
        sectorObj = gapst_AllObjSectors[AI_SECTOR_CAPACITY * sectorID + i];
        if (sectorObj)
            sectorObj->if_this_not0_wont_render_gao = gao->if_this_not0_wont_render_gao;
    }
}

tAINode* AI_EvalFunc_GRPSectInvisibleSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int sectorID;
    int invisible;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    invisible = AI_GetIntFromStack();
    sectorID = AI_GetIntFromStack();
    AI_EvalFunc_GRPSectInvisibleSet_C(gaoUltra, sectorID, invisible);
    return node + 1;
}

// A stub: the two arguments are taken and traced.
tAINode* AI_EvalFunc_GRPSectInactiveSet(tAINode* node)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetIntFromStack();
    AI_GetIntFromStack();
    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
}

void AI_EvalFunc_GRPSectSetIsIn_C(struct sGameObjectMain* gao)
{
    tGroup* group;
    tableElemStructPF* entry;
    tableElemStructPF* end;

    group = AI_l_GroupOf(gao);
    if (!group)
        return;

    end = (tableElemStructPF*)(group->tablePF->nextElemPtr);
    for (entry = group->tablePF->firstElemPtr; entry < end; ++entry)
    {
        if (AI_l_ObjectInUse(entry->objectPtr))
            ((tGameObjectMain*)entry->objectPtr)->flags2 |= 0x10000u;
    }
}

tAINode* AI_EvalFunc_GRPSectSetIsIn(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_GRPSectSetIsIn_C(gaoUltra);
    return node + 1;
}

// The eight corners of an object's bounding box in world space.
static void AI_l_BoundingBoxCorners(tGameObjectMain* gao, Vec3 corners[8])
{
    int i;

    corners[0] = gao->boundingVolume->BVMax;
    corners[1] = gao->boundingVolume->BVMin;

    corners[2].x = corners[0].x;
    corners[2].y = corners[1].y;
    corners[2].z = corners[0].z;
    corners[3].x = corners[1].x;
    corners[3].y = corners[1].y;
    corners[3].z = corners[0].z;
    corners[4].x = corners[1].x;
    corners[4].y = corners[0].y;
    corners[4].z = corners[0].z;
    corners[5].x = corners[0].x;
    corners[5].y = corners[0].y;
    corners[5].z = corners[1].z;
    corners[6].x = corners[0].x;
    corners[6].y = corners[1].y;
    corners[6].z = corners[1].z;
    corners[7].x = corners[1].x;
    corners[7].y = corners[0].y;
    corners[7].z = corners[1].z;

    for (i = 0; i < 8; ++i)
        MATH_VecAddVecWithoutBuffer(&corners[i], &gao->matrices->Pos);
}

// TODO FIXME: itt a decompile során eléggé elszáll a nagy stack miatt, megnézni ohgy jó-e így, lehet nem
// Narrows the view frustum down to what each door the script supplies
// leaves visible, and raises the invisibility count of every object of
// the group and the sector that the narrowed frustum culls.
void AI_EvalFunc_GRPCullDoor_C(tGameObjectMain* gao, tGameObjectMain** doors, int viewIdx, int sectorID)
{
    tGameObjectMain* sectorObjs[1000];
    tGameObjectMain* openDoors[50];
    Vec3 corners[8];
    Vec3 planeLeft;
    Vec3 planeUp;
    Vec3 planeRight;
    Vec3 planeDown;
    Vec3 toCorner;
    Vec3 toDoor;
    Vec3 toCam;
    tDDCamStruct narrowed;
    tDDContainerStruct* ct;
    tGdi_DisplayData* displayData;
    tGroup* group;
    tableElemStructPF* entry;
    tableElemStructPF* end;
    tGameObjectMain* member;
    tWorld* world;
    Vec3* norm;
    Vec3* plane;
    double len;
    float dot;
    float bestDot;
    int bestCorner;
    int sectorObjNum;
    int doorNum;
    int i;
    int j;
    int k;
    int l;
    int m;

    if (!AI_l_GroupOf(gao))
        return;

    world = WOR_gpst_CurrentWorld;
    displayData = world->viewPtr[viewIdx].displayData;
    if (!displayData)
        return;

    world->viewPtr[viewIdx].viewFunctionPtr(&world->viewPtr[viewIdx]);

    ct = &displayData->ct;
    CAM_SetCameraMatrix(ct, &world->viewPtr[viewIdx].cameraMtx);
    CAM_Inverse(ct);
    CAM_Engine_ComputePlans(ct);
    OBJ_UpdateCullingVars(&ct->cam);
    memcpy(&narrowed, ct, sizeof(narrowed));

    // everything the cull may hide: the group's own members, and the
    // objects registered to the sector
    sectorObjNum = 0;
    group = gao->extended->grp;
    end = (tableElemStructPF*)(group->tablePF->nextElemPtr);
    for (entry = group->tablePF->firstElemPtr; entry < end; ++entry)
    {
        member = (tGameObjectMain*)entry->objectPtr;
        if (AI_l_ObjectInUse(member))
            sectorObjs[sectorObjNum++] = member;
    }
    if (sectorID != 0xFFFF && sectorID != -1)
    {
        for (i = 0; i < gai_NumObjSectors[sectorID]; ++i)
            sectorObjs[sectorObjNum++] = gapst_AllObjSectors[AI_SECTOR_CAPACITY * sectorID + i];
    }

    // the doors worth narrowing through: on screen, and not marked out
    doorNum = 0;
    while (*doors)
    {
        if (ENG_gb_ActiveSectorization && !OBJ_CullingObject(*doors) && ((*doors)->flags2 & 0x200000) == 0)
            openDoors[doorNum++] = *doors;
        ++doors;
    }

    if (doorNum)
    {
        planeLeft = g_stGlobalNormPlaneLeft;
        planeUp = g_stGlobalNormPlaneUp;
        planeRight = g_stGlobalNormPlaneRight;
        planeDown = g_stGlobalNormPlaneDown;

        for (j = 0; j < doorNum; ++j)
        {
            AI_l_BoundingBoxCorners(openDoors[j], corners);

            // a door seen too far off its own axis narrows nothing,
            // and gives up on the rest of the doors too
            for (k = 0; k < 8; ++k)
            {
                MATH_SubstractVec(&toCorner, &corners[k], &ct->cam.objectMtx.Pos);
                MATH_NormalizeVector(&toCorner, &toCorner);
                if (MATH_DotProduct(&ct->cam.objectMtx.v3, &toCorner) < 0.30000001)
                    return;
            }

            // each of the four frustum planes is swung onto the corner
            // of the door that is nearest to it
            for (l = 0; l < 4; ++l)
            {
                switch (l)
                {
                case 0:
                    norm = &planeLeft;
                    plane = &narrowed.planeLeft;
                    break;
                case 1:
                    norm = &planeUp;
                    plane = &narrowed.planeUp;
                    break;
                case 2:
                    norm = &planeRight;
                    plane = &narrowed.planeRight;
                    break;
                default:
                    norm = &planeDown;
                    plane = &narrowed.planeDown;
                    break;
                }

                bestDot = 10.0;
                bestCorner = -1;
                for (k = 0; k < 8; ++k)
                {
                    MATH_SubstractVec(&toCorner, &corners[k], &ct->cam.objectMtx.Pos);
                    dot = MATH_DotProduct(norm, &toCorner);
                    len = MATH_VecLen(&toCorner);
                    dot = (float)(dot / len);
                    if (dot < 0.0)
                    {
                        // a corner behind the plane leaves it where it is
                        bestCorner = -1;
                        break;
                    }
                    if (dot < bestDot)
                    {
                        bestDot = dot;
                        bestCorner = k;
                    }
                }

                if (bestCorner == -1)
                {
                    *plane = *norm;
                }
                else
                {
                    MATH_SubstractVec(&toCorner, &corners[bestCorner], &ct->cam.objectMtx.Pos);
                    MATH_NormalizeEqualVector(&toCorner);
                    if (l && l != 2)
                        MATH_CrossProduct(plane, &toCorner, &ct->cam.objectMtx.v1);
                    else
                        MATH_CrossProduct(plane, &toCorner, &ct->cam.objectMtx.v2);
                    if (!l || l == 3)
                        MATH_VecInvert(plane, plane);
                }
                MATH_TransformVector(plane, &ct->cam.cameraMtx, plane);
            }

            narrowed.planeDown.x = ct->cam.planeDown.x;
            narrowed.planeUp.x = ct->cam.planeUp.x;
            narrowed.planeLeft.y = ct->cam.planeLeft.y;
            narrowed.planeRight.y = ct->cam.planeRight.y;
            OBJ_UpdateCullingVars(&narrowed);

            for (m = 0; m < sectorObjNum; ++m)
            {
                if (!sectorObjs[m])
                    continue;

                // both directions are computed and neither is read
                MATH_SubstractVec(&toDoor, &sectorObjs[m]->matrices->Pos, &openDoors[j]->matrices->Pos);
                MATH_NormalizeEqualVector(&toDoor);
                MATH_SubstractVec(&toCam, &sectorObjs[m]->matrices->Pos, &ct->cam.objectMtx.Pos);
                MATH_NormalizeEqualVector(&toCam);

                // visible through this door, so it is not this cull's
                // to hide
                if (!OBJ_CullingObject(sectorObjs[m]))
                {
                    if (sectorObjs[m]->if_this_not0_wont_render_gao)
                        --sectorObjs[m]->if_this_not0_wont_render_gao;
                    sectorObjs[m] = 0;
                }
            }
        }
    }

    for (i = 0; i < sectorObjNum; ++i)
    {
        if (sectorObjs[i])
            ++sectorObjs[i]->if_this_not0_wont_render_gao;
    }
}

tAINode* AI_EvalFunc_GRPCullDoor(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int viewIdx;
    int sectorID;

    sectorID = AI_GetIntFromStack();
    viewIdx = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_GRPCullDoor_C(gaoUltra, (tGameObjectMain**)var.ptr, viewIdx, sectorID);
    return node + 1;
}

int AI_EvalFunc_GRPFillArray_C(tGameObjectMain* gao, tGameObjectMain** members)
{
    tGroup* group;
    tableElemStructPF* entry;
    tableElemStructPF* end;
    int num;

    group = AI_l_GroupOf(gao);
    if (!group)
        return 0;

    num = 0;
    end = (tableElemStructPF*)(group->tablePF->nextElemPtr);
    for (entry = group->tablePF->firstElemPtr; entry < end; ++entry)
    {
        if (AI_l_ObjectInUse(entry->objectPtr))
            members[num++] = (tGameObjectMain*)entry->objectPtr;
    }
    return num;
}

tAINode* AI_EvalFunc_GRPFillArray(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_GRPFillArray_C(gaoUltra, (tGameObjectMain**)var.ptr));
    return node + 1;
}
