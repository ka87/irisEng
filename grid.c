#include "math.h"
#include "gameobject.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "grid.h"

int GRID_gi_TestWall = 0x20;

/* The neighbour scan table. The first sixteen entries are the eight
   neighbours as (dx, dy) pairs, in the order a search walks them:
   the four orthogonals first, then the four diagonals. The entries
   from 16 on repeat the first fourteen, which is what lets the
   pathfinder read ai_Scan[i + 8 .. i + 11] past the end of a diagonal
   pair to find the two orthogonals that diagonal cuts between. */
int ai_Scan[43] = { 0, -1, 1, 0, 0, 1, -1, 0, -1, -1, 1, -1, 1, 1, -1, 1, -1, 0, 0, -1, 1, 0, 0, 1, -1, 0, -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0, 1 };
//int ai_Scan_511[17] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0, 1 };

// A row of the live window is this many cells wide.
static int GRI_l_Stride(tWorldGridStruct* grid)
{
    return grid->s3Size7LimitMaxX * grid->gridElemSize;
}

// The capability byte of one cell of the live window.
static char* GRI_l_Cell(tWorldGridStruct* grid, int x, int y)
{
    return &grid->capasPtr[y * GRI_l_Stride(grid) + x];
}

// The centre of one cell in world coordinates. The window's origin is
// its lower limit in cells times the cell size, and the half is the
// step from a cell's corner to its centre.
static void GRI_l_CellToWorld(tWorldGridStruct* grid, int x, int y, Vec3* pos)
{
    pos->x = (float)((double)(grid->gridElemSize * grid->s5Limit1MinX)
                     + ((double)x + grid->startX + 0.5));
    pos->y = (float)((double)(grid->gridElemSize * grid->s5Limit1MinY)
                     + ((double)y + grid->startY + 0.5));
    pos->z = 0.0f;
}

// Adds the capability to every cell of the list, and takes it back off
// the ones that took it if any cell refuses, so a tag is all or
// nothing. Returns whether the whole list was tagged.
static int GRI_l_TagCells(tWorldGridStruct* grid, Vec3* cells, int num, char capability)
{
    int taken;
    int i;

    for (taken = 0; taken < num; ++taken)
    {
        if (!GRI_AddCapa(grid, &cells[taken], capability))
            break;
    }
    if (taken == num)
        return 1;

    for (i = 0; i < taken; ++i)
        GRI_AddCapa(grid, &cells[i], -capability);
    return 0;
}

// A world position to a cell of the whole grid, clamped to it. Returns
// whether the position was inside without clamping.
int GRI_3Dto2D(tWorldGridStruct* grid, Vec3* pos, int* retPosX, int* retPosY)
{
    int inside;

    inside = 1;
    if (pos->x < (double)grid->startX)
    {
        *retPosX = 0;
        inside = 0;
    }
    else if ((double)(grid->gridCountX * grid->gridElemSize) + grid->startX < pos->x)
    {
        *retPosX = grid->gridCountX * grid->gridElemSize - 1;
        inside = 0;
    }
    else
    {
        *retPosX = (int)(pos->x - grid->startX);
    }

    if (pos->y < (double)grid->startY)
    {
        *retPosY = 0;
        inside = 0;
    }
    else if ((double)(grid->gridCountY * grid->gridElemSize) + grid->startY < pos->y)
    {
        *retPosY = grid->gridCountY * grid->gridElemSize - 1;
        inside = 0;
    }
    else
    {
        *retPosY = (int)(pos->y - grid->startY);
    }
    return inside;
}

// A cell of the whole grid to a cell of the live window, clamped to it.
// Returns whether it was inside the window without clamping.
int GRI_ConvertTo2DReal(tWorldGridStruct* grid, int32_t* posX, int32_t* posY)
{
    int inside;
    int limit;

    inside = 1;

    *posX -= grid->gridElemSize * grid->s5Limit1MinX;
    if (*posX < 0)
    {
        *posX = 0;
        inside = 0;
    }
    *posY -= grid->gridElemSize * grid->s5Limit1MinY;
    if (*posY < 0)
    {
        *posY = 0;
        inside = 0;
    }

    limit = grid->gridElemSize * grid->s3Size7LimitMaxX;
    if (*posX >= limit)
    {
        *posX = limit - 1;
        inside = 0;
    }
    limit = grid->gridElemSize * grid->s4Size7LimitMaxY;
    if (*posY >= limit)
    {
        *posY = limit - 1;
        inside = 0;
    }
    return inside;
}

// Replaces a cell's capability, unless the cell is a wall.
void GRI_SetCapa(tWorldGridStruct* grid, Vec3* pos, char capability)
{
    char* cell;
    int cellX;
    int cellY;

    if (!GRI_3Dto2D(grid, pos, &cellX, &cellY) || !GRI_ConvertTo2DReal(grid, &cellX, &cellY))
        return;

    cell = GRI_l_Cell(grid, cellX, cellY);
    if ((GRID_gi_TestWall & *cell) == 0)
        *cell = capability;
}

// Adds to a cell's capability. Reports failure only where the cell
// already carries both of the 0xC0 bits and the caller is setting one
// of them, which is how a tag finds out that a cell is taken.
int GRI_AddCapa(tWorldGridStruct* grid, Vec3* pos, char capability)
{
    char* cell;
    int cellX;
    int cellY;

    if (!GRI_3Dto2D(grid, pos, &cellX, &cellY))
        return 1;
    if (!GRI_ConvertTo2DReal(grid, &cellX, &cellY))
        return 1;

    if ((GRID_gi_Mode & 1) == 0 && (*GRI_l_Cell(grid, cellX, cellY) & GRID_gi_TestWall) != 0)
        return 1;

    cell = GRI_l_Cell(grid, cellX, cellY);
    if (capability > 0 && (capability & 0xC0) != 0 && (*cell & 0xC0) == 0xC0)
        return 0;

    *cell += capability;
    return 1;
}

int GRI_i_GetCapa(tWorldGridStruct* grid, Vec3* pos)
{
    int cellX;
    int cellY;

    if (!GRI_3Dto2D(grid, pos, &cellX, &cellY) || !GRI_ConvertTo2DReal(grid, &cellX, &cellY))
        return 0;
    return *GRI_l_Cell(grid, cellX, cellY);
}

// Moves a position to the centre of its cell, stepping to a free
// neighbour first when the cell itself is a wall or is taken. Reports
// failure only when every neighbour is blocked too.
int GRI_i_ComputeDest(struct sWorldGridStruct* grid, Vec3* pos)
{
    int neighbours[16] = { -1, 0, -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1 };
    int blocked;
    int cellX;
    int cellY;
    int i;

    if (!GRI_3Dto2D(grid, pos, &cellX, &cellY) || !GRI_ConvertTo2DReal(grid, &cellX, &cellY))
        return 0;

    // the original adds rather than ors, which differs only if the
    // wall bit the script set overlaps 0xC0
    blocked = GRID_gi_TestWall + 0xC0;

    if ((*GRI_l_Cell(grid, cellX, cellY) & blocked) != 0)
    {
        for (i = 0; ; i += 2)
        {
            if (i >= 16)
                return 0;

            if ((*GRI_l_Cell(grid, cellX + neighbours[i], cellY + neighbours[i + 1]) & blocked) == 0)
            {
                cellX += neighbours[i];
                cellY += neighbours[i + 1];
                break;
            }
        }
    }

    GRI_l_CellToWorld(grid, cellX, cellY, pos);
    return 1;
}

// Walks a path back from its far end and reports how many of its points
// are reachable: the first point, counting from the end, whose cell or
// any of the seven cells around it is a wall or is taken, stops the
// walk. A path through its own grid ignores the 0xC bits, which is how
// a cell blocked by the caller itself does not block the caller.
int GRI_SmoothWay(struct sWorldGridStruct* ownGrid, struct sWorldGridStruct* grid, Vec3* path, int num)
{
    Vec3* point;
    char capability;
    int cellX;
    int cellY;
    int neighbourX;
    int neighbourY;
    int at;
    int i;

    at = num - 1;
    point = &path[at];

    while (at >= 0)
    {
        if (!GRI_3Dto2D(grid, point, &cellX, &cellY))
            break;

        // ai_Scan[26..41] is the eight-neighbour run this walk uses
        for (i = 26; i <= 40; i += 2)
        {
            neighbourX = cellX + ai_Scan[i];
            neighbourY = cellY + ai_Scan[i + 1];
            if (!GRI_ConvertTo2DReal(grid, &neighbourX, &neighbourY))
                return at + 1;

            capability = *GRI_l_Cell(grid, neighbourX, neighbourY);
            if ((capability & GRID_gi_TestWall) != 0 && (grid == ownGrid || (capability & 0xC) == 0))
                return at + 1;
            if ((capability & 0xC0) != 0 && (grid == ownGrid || (capability & 0xC) == 0))
                return at + 1;
        }

        --at;
        --point;
    }
    return at + 1;
}

int GRID_3Dto2D(tWorldGridStruct* grid, Vec3* pos, int32_t* cellX, int32_t* cellY)
{
    if (!GRI_3Dto2D(grid, pos, cellX, cellY))
        return 0;
    return GRI_ConvertTo2DReal(grid, cellX, cellY) != 0;
}

/* Whether the pathfinder may step onto a cell.

   ownGrid is the grid the path is built in and colGrid the one the
   walkability is read from. They are usually the same; where they are
   not, the 0xC bits mark a cell blocked by the caller itself and are
   ignored, so an object does not block its own path.

   The first test allows the step when the collision grid's cell is
   BOTH a wall and taken, which reads oddly but is what the original
   has, and the same pair of conditions guards the target nudge above. */
static BOOL GRI_l_StepAllowed(tWorldGridStruct* ownGrid, tWorldGridStruct* colGrid, int x, int y, int fromStartCells, int nearLimit)
{
    char colCapability;
    char ownCapability;

    colCapability = *GRI_l_Cell(colGrid, x, y);
    if ((colCapability & GRID_gi_TestWall) != 0 && (colCapability & 0xC0) != 0)
        return TRUE;

    ownCapability = *GRI_l_Cell(ownGrid, x, y);
    if ((ownCapability & GRID_gi_TestWall) != 0
        && (ownGrid == colGrid || (ownCapability & 0xC) == 0))
    {
        return FALSE;
    }

    // a cell another object has taken still gives way to one that is
    // close enough to its goal
    if ((ownCapability & 0xC0) != 0
        && (ownGrid == colGrid || (ownCapability & 0xC) == 0)
        && fromStartCells <= nearLimit)
    {
        return FALSE;
    }
    return TRUE;
}

/* A* over the grid, from start to target, writing the path into
   pathOut as a walk back from the target. Returns how many points it
   wrote, or zero when there is no path.

   maxDist caps how far along the start-to-target direction the target
   is allowed to be, in world units, and maxCells caps how far from the
   start the search may wander, in cells. nearLimit is the cell distance
   below which a cell another object has taken is still walkable, so an
   object close to its goal may push through. maxPoints is pathOut's
   capacity: a longer path has its leading points dropped, which keeps
   the end nearest the target.

   The seventh parameter is unused. */
int GRID_ShortestPath(struct sGameObjectMain* gao, struct sWorldGridStruct* ownGrid, struct sWorldGridStruct* colGrid, Vec3* start, Vec3* target, Vec3* pathOut, int unused, int nearLimit, float maxDist, int maxCells, int maxPoints)
{
    Vec3 sight;
    Vec3 capped;
    Vec3 toTarget;
    Vec3 fromStart;
    tGridPtrElem* cell;
    char capability;
    float best;
    float cost;
    float reach;
    float length;
    int startX;
    int startY;
    int targetX;
    int targetY;
    int atX;
    int atY;
    int tryX;
    int tryY;
    int baseX;
    int baseY;
    int bestIdx;
    int openNum;
    int closedNum;
    int found;
    int abandoned;
    int scan;
    int scanned;
    int stride;
    int hops;
    int written;
    int at;
    int i;

    atX = 0;
    atY = 0;
    found = 0;
    abandoned = 0;

    if (!GRID_3Dto2D(ownGrid, start, &startX, &startY))
        return 0;

    /* The sight conversion's result is overwritten by the target's on
       the next line and never read. Preserved as written. */
    OBJ_SightGet(gao, &sight);
    sight.x = sight.x + start->x;
    sight.y = sight.y + start->y;
    sight.z = sight.z + start->z;
    GRID_3Dto2D(ownGrid, &sight, &targetX, &targetY);
    GRID_3Dto2D(ownGrid, target, &targetX, &targetY);

    // the target is pulled back to the nearer of maxCells and maxDist
    reach = (float)maxCells;
    if (reach >= maxDist)
        reach = maxDist;

    toTarget.x = target->x - start->x;
    toTarget.y = target->y - start->y;
    toTarget.z = target->z - start->z;
    if (sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z) > reach)
    {
        length = (float)sqrt(toTarget.z * toTarget.z
                             + toTarget.x * toTarget.x
                             + toTarget.y * toTarget.y);
        toTarget.y = 1.0f / length * toTarget.y;
        toTarget.z = toTarget.z * (1.0f / length);
        toTarget.x = toTarget.x * (1.0f / length) * reach;
        toTarget.y = toTarget.y * reach;
        toTarget.z = toTarget.z * reach;
        capped.x = toTarget.x + start->x;
        capped.y = toTarget.y + start->y;
        capped.z = toTarget.z + start->z;
        GRID_3Dto2D(ownGrid, &capped, &targetX, &targetY);
    }

    tryX = targetX;
    tryY = targetY;

    if (targetX == startX && targetY == startY)
    {
        GRI_l_CellToWorld(ownGrid, startX, startY, pathOut);
        return 1;
    }

    // a target cell that is a wall, or taken and not close enough to
    // push through, is nudged to a free neighbour
    if (ownGrid == colGrid
        && (abs(targetY - startY) + abs(targetX - startX) < nearLimit
            || (*GRI_l_Cell(colGrid, targetX, targetY) & GRID_gi_TestWall) != 0))
    {
        capability = *GRI_l_Cell(colGrid, targetX, targetY);
        if (((capability & 0xC0) != 0
                 && abs(targetY - startY) + abs(targetX - startX) < nearLimit
             || (capability & GRID_gi_TestWall) != 0)
            && ((capability & GRID_gi_TestWall) == 0 || (capability & 0xC0) == 0))
        {
            // every candidate is an offset from the cell the target
            // landed in, not from the one tried last
            baseX = targetX;
            baseY = targetY;
            for (scan = 0; scan < 16; scan += 2)
            {
                tryX = baseX + ai_Scan[scan];
                tryY = baseY + ai_Scan[scan + 1];
                capability = *GRI_l_Cell(colGrid, tryX, tryY);
                targetX = tryX;
                targetY = tryY;
                if ((GRID_gi_TestWall & capability) == 0 && (capability & 0xC0) == 0)
                    break;
            }
        }

        capability = *GRI_l_Cell(colGrid, tryX, tryY);
        if (capability
            && ((GRID_gi_TestWall & capability) == 0 || (capability & 0xC0) == 0)
            && ((GRID_gi_TestWall & capability) != 0 || (capability & 0xC0) != 0))
        {
            return 0;
        }
    }

    stride = GRI_l_Stride(ownGrid);
    cell = &ownGrid->ptrs[startX + startY * stride];
    cell->flags = 2;
    cell->fEval = 0.0;
    cell->wayX = -1;
    cell->wayY = -1;

    closedNum = 0;
    openNum = 1;
    GRID_gast_Best[0].ptrElems = cell;
    GRID_gast_Best[0].gridShorts.i1 = startX;
    GRID_gast_Best[0].gridShorts.i2 = startY;

    while (openNum && !found && !abandoned)
    {
        // the cheapest open cell. The original unrolls this four at a
        // time with a remainder loop; it is one loop here.
        best = FLT_MAX;
        bestIdx = -1;
        for (i = 0; i < openNum; ++i)
        {
            if (best > GRID_gast_Best[i].ptrElems->fEval)
            {
                best = GRID_gast_Best[i].ptrElems->fEval;
                atX = GRID_gast_Best[i].gridShorts.i1;
                atY = GRID_gast_Best[i].gridShorts.i2;
                bestIdx = i;
            }
        }

        ownGrid->ptrs[atX + atY * stride].flags = 1;

        /* The closed list is written before its bound is checked, and
           the bound is 2000 where GRID_gast_Explored holds 1000. Both
           are preserved as written. */
        GRID_gast_Explored[closedNum].i1 = atX;
        GRID_gast_Explored[closedNum].i2 = atY;
        if (closedNum >= 2000)
            break;

        // the open list's last entry fills the hole
        --openNum;
        GRID_gast_Best[bestIdx].ptrElems = GRID_gast_Best[openNum].ptrElems;
        GRID_gast_Best[bestIdx].gridShorts = GRID_gast_Best[openNum].gridShorts;
        ++closedNum;

        if (atX == targetX && atY == targetY)
            found = 1;

        /* The neighbour walk starts from the quadrant the object faces,
           so the first cells tried are the ones ahead of it. */
        if (gao->matrices->v2.x < -0.5)
            scan = 12;
        else if (gao->matrices->v2.x > 0.5)
            scan = 4;
        else if (gao->matrices->v2.y >= -0.5)
            scan = 8;
        else
            scan = 0;

        for (scanned = 0; scanned < 8 && !abandoned; ++scanned, scan = (scan + 2) % 16)
        {
            /* Note the asymmetry: a neighbour below the window is
               skipped, but one past its far edge abandons the whole
               search. Preserved as written. */
            tryX = atX + ai_Scan[scan];
            if (tryX < 0)
                continue;
            if (tryX >= ownGrid->gridElemSize * ownGrid->s3Size7LimitMaxX)
            {
                abandoned = 1;
                break;
            }
            if (abs(tryX - startX) > maxCells)
                continue;

            tryY = atY + ai_Scan[scan + 1];
            if (tryY < 0)
                continue;
            if (tryY >= ownGrid->gridElemSize * ownGrid->s4Size7LimitMaxY)
            {
                abandoned = 1;
                break;
            }
            if (abs(tryY - startY) > maxCells)
                continue;

            /* A diagonal may not cut between two blocked orthogonals.
               ai_Scan[scan + 8 .. scan + 11] are those two neighbours,
               which is what the table's repeated tail is for. */
            if (scan >= 8)
            {
                if ((*GRI_l_Cell(colGrid, atX + ai_Scan[scan + 8], atY + ai_Scan[scan + 9])
                     & (GRID_gi_TestWall | 0xC0)) != 0)
                {
                    continue;
                }
                if ((*GRI_l_Cell(colGrid, atX + ai_Scan[scan + 10], atY + ai_Scan[scan + 11])
                     & (GRID_gi_TestWall | 0xC0)) != 0)
                {
                    continue;
                }
            }

            // the step's cost: how far the neighbour still is from the
            // target, plus a penalty for turning away from the sight
            // and another for leaving the straight line to the target
            cost = (float)(int)(abs(targetX - tryX) + abs(targetY - tryY));
            if ((tryX != startX || tryY != startY) && (targetX != startX || targetY != startY))
            {
                OBJ_SightGet(gao, &sight);
                sight.z = 0.0;

                fromStart.x = (float)tryX - (float)startX;
                fromStart.y = (float)tryY - (float)startY;
                fromStart.z = 0.0;
                MATH_NormalizeVector(&fromStart, &fromStart);
                cost = (float)fabs(1.0f - MATH_DotProduct(&sight, &fromStart)) * 20.0f + cost;

                toTarget.x = (float)tryX - (float)startX;
                toTarget.y = (float)tryY - (float)startY;
                toTarget.z = 0.0;
                MATH_NormalizeVector(&toTarget, &toTarget);
                fromStart.x = (float)targetX - (float)startX;
                fromStart.y = (float)targetY - (float)startY;
                fromStart.z = 0.0;
                MATH_NormalizeVector(&fromStart, &fromStart);
                cost = (float)fabs(1.0 - MATH_DotProduct(&toTarget, &fromStart)) * 30.0f + cost;
            }

            if (!GRI_l_StepAllowed(ownGrid, colGrid, tryX, tryY,
                                   abs(tryX - startX) + abs(tryY - startY), nearLimit))
            {
                continue;
            }

            cell = &ownGrid->ptrs[tryX + tryY * stride];
            if (cell->flags == 1)
                continue;

            if (cell->flags == 2)
            {
                // already open, so keep the cheaper way in
                if (cost + ownGrid->ptrs[atX + atY * stride].fEval < cell->fEval)
                {
                    cell->fEval = cost + ownGrid->ptrs[atX + atY * stride].fEval;
                    cell->wayX = atX;
                    cell->wayY = atY;
                }
                continue;
            }

            cell->flags = 2;
            cell->fEval = cost + ownGrid->ptrs[atX + atY * stride].fEval;
            cell->wayX = atX;
            cell->wayY = atY;

            /* The open list is written before its bound is checked too,
               so entry 1000 of a 1000-entry array is written on the
               step that stops the search. Preserved as written. */
            GRID_gast_Best[openNum].ptrElems = cell;
            GRID_gast_Best[openNum].gridShorts.i1 = tryX;
            GRID_gast_Best[openNum].gridShorts.i2 = tryY;
            if (openNum >= 1000)
            {
                abandoned = 1;
                break;
            }
            ++openNum;
        }
    }

    for (i = 0; i < closedNum; ++i)
    {
        ownGrid->ptrs[GRID_gast_Explored[i].i1
                      + stride * GRID_gast_Explored[i].i2].flags = 0;
    }
    for (i = 0; i < openNum; ++i)
    {
        ownGrid->ptrs[GRID_gast_Best[i].gridShorts.i1
                      + stride * GRID_gast_Best[i].gridShorts.i2].flags = 0;
    }

    if (!found)
        return 0;

    // how long the way back from the target is
    hops = 0;
    atX = targetX;
    atY = targetY;
    while (atX != -1)
    {
        at = atX + stride * atY;
        atX = ownGrid->ptrs[at].wayX;
        atY = ownGrid->ptrs[at].wayY;
        ++hops;
    }

    // then it again, writing the cells out and dropping the leading
    // ones where the path is longer than pathOut can hold
    written = 0;
    atX = targetX;
    atY = targetY;
    while (atX != -1)
    {
        if (hops > maxPoints)
        {
            --hops;
        }
        else
        {
            GRI_l_CellToWorld(ownGrid, atX, atY, &pathOut[written]);
            ++written;
        }
        at = atX + atY * stride;
        atX = ownGrid->ptrs[at].wayX;
        atY = ownGrid->ptrs[at].wayY;
    }
    return written;
}

/* Run-length encodes one tile of the capability grid into dest, as a
   length-prefixed stream of (count, value) runs. Returns the byte
   length written, or zero when the tile is a single run of value zero
   -- which is how an empty tile is dropped rather than stored.

   mask is applied to every cell before comparing, so a caller can
   encode a subset of the capability bits. */
static uint8_t* GRI_l_EmitRun(uint8_t* at, int count, int value)
{
    if (count == 1)
    {
        /* A single cell carrying one of the 0xC0 bits gets a count-of-
           one tag. One without gets NO tag, just its value byte, which
           is the decoder's plain-byte form. */
        if ((value & 0xC0) != 0)
        {
            *at++ = 0x80;
            *at++ = 1;
        }
    }
    else if (count > 63 || (value & 0xFFFFFFC0) != 0)
    {
        if (count > 0xFFFF)
        {
            // unreachable, and it would drop the run: a tile is at most
            // gridElemSize squared and gridElemSize is a char
            return at;
        }
        if (count > 255)
        {
            *at++ = 64;
            *(int16_t*)at = count;
            at += 2;
        }
        else
        {
            *at++ = 0x80;
            *at++ = count;
        }
    }
    else
    {
        // a short run of a value that fits in six bits packs its count
        // into the tag
        *at++ = count | 0xC0;
    }
    *at++ = value;
    return at;
}

int GRI_Compress(tWorldGridStruct* grid, char* capas, int tileX, int tileY, char* dest, int mask)
{
    uint8_t* at;
    char* cell;
    int runValue;
    int endX;
    int endY;
    int rowGap;
    int runLength;
    int runs;
    int length;
    int x;
    int y;

    endX = grid->gridElemSize + tileX;
    endY = grid->gridElemSize + tileY;

    if (capas == grid->capasPtr)
    {
        cell = &capas[tileY * grid->gridElemSize * grid->s3Size7LimitMaxX + tileX];
        rowGap = grid->gridElemSize * (grid->s3Size7LimitMaxX - 1);
    }
    else
    {
        cell = &grid->notUsedSecondaryCapasPtr[tileY * grid->gridElemSize * grid->gridCountX + tileX];
        rowGap = grid->gridElemSize * (grid->gridCountX - 1);
    }

    // the first cell opens the first run; the length word goes in front
    runValue = mask & *cell;
    ++cell;
    runLength = 1;
    runs = 0;
    at = (uint8_t*)dest + 4;

    y = tileY;
    x = tileX + 1;
    while (y != endY)
    {
        if (x == endX)
        {
            x = tileX;
            ++y;
            cell += rowGap;
            continue;
        }

        if ((mask & *cell) == runValue)
        {
            ++runLength;
        }
        else
        {
            at = GRI_l_EmitRun(at, runLength, runValue);
            ++runs;
            runValue = mask & *cell;
            runLength = 1;
        }
        ++cell;
        ++x;
    }
    at = GRI_l_EmitRun(at, runLength, runValue);

    length = (char*)at - dest;
    *(int32_t*)dest = length;

    // a tile that is one run of nothing is not worth storing
    if (!runValue && !runs)
        return 0;
    return length;
}

// Encodes every tile of the live window that the new window at
// (windowX, windowY) will not cover, so the cells about to be dropped
// are kept.
void GRI_CompressReal(tWorldGridStruct* grid, int windowX, int windowY)
{
    char* tile;
    int* stored;
    unsigned int length;
    int id;
    int tileX;
    int tileY;
    int gridX;
    int gridY;

    tile = (char*)MEM_p_Alloc(grid->gridElemSize * grid->gridElemSize + 4);

    for (tileX = 0; tileX < grid->s3Size7LimitMaxX; ++tileX)
    {
        if (tileX + grid->s5Limit1MinX >= grid->gridCountX)
            continue;

        for (tileY = 0; tileY < grid->s4Size7LimitMaxY; ++tileY)
        {
            gridY = tileY + grid->s5Limit1MinY;
            if (gridY >= grid->gridCountY)
                break;

            gridX = grid->s5Limit1MinX + tileX;
            if (gridX < windowX || gridY < windowY
                || gridX >= windowX + grid->s3Size7LimitMaxX
                || gridY >= windowY + grid->s4Size7LimitMaxY)
            {
                length = 0;
            }
            else
            {
                length = GRI_Compress(grid, grid->capasPtr,
                                      tileX * grid->gridElemSize,
                                      tileY * grid->gridElemSize,
                                      tile, 255);
            }

            id = grid->gridCountX * gridY + gridX;
            if (!length)
            {
                if (grid->ptr[id])
                {
                    MEM_Free(grid->ptr[id]);
                    grid->ptr[id] = 0;
                }
                continue;
            }

            // the stored tile is reused when it is already the right
            // size, since its first word is its length
            stored = (int*)grid->ptr[id];
            if (stored && length == *stored)
            {
                memcpy(grid->ptr[id], tile, length);
                continue;
            }
            if (stored)
                MEM_Free(grid->ptr[id]);
            grid->ptr[id] = (char*)MEM_p_Alloc(length);
            memcpy(grid->ptr[id], tile, length);
        }
    }
    MEM_Free(tile);
}

// Decodes one tile back into the capability grid. A tile with neither a
// live copy nor a stored one fills with zero.
void GRI_UnCompress(tWorldGridStruct* grid, char* capas, int tileX, int tileY)
{
    uint8_t* at;
    char* cell;
    char runValue;
    int rowGap;
    int rowWidth;
    int id;
    int offset;
    int runLength;
    int firstX;
    int endX;
    int y;
    int endY;
    int x;
    int tag;

    if (capas == grid->capasPtr)
    {
        if (grid->s5Limit1MinX + tileX >= grid->gridCountX)
            return;
        if (tileY + grid->s5Limit1MinY >= grid->gridCountY)
            return;

        rowWidth = grid->s3Size7LimitMaxX;
        id = tileX + grid->gridCountX * (tileY + grid->s5Limit1MinY) + grid->s5Limit1MinX;
        cell = &capas[tileY * grid->gridElemSize * grid->gridElemSize * rowWidth];
    }
    else
    {
        rowWidth = grid->gridCountX;
        id = tileX + tileY * rowWidth;
        cell = &grid->notUsedSecondaryCapasPtr[tileY * grid->gridElemSize * rowWidth * grid->gridElemSize];
    }

    rowGap = grid->gridElemSize * (rowWidth - 1);
    cell += tileX * grid->gridElemSize;

    // the live copy if there is one, otherwise the stored one
    at = (uint8_t*)grid->ptr[id];
    if (!at)
    {
        offset = grid->ptrInts[id];
        if (offset == -1)
            at = 0;
        else
            at = &grid->compressedGrid[offset];
    }
    if (at)
        at += 4;                                // past the length word

    firstX = tileX * grid->gridElemSize * grid->gridElemSize;
    endX = grid->gridElemSize + firstX;
    y = tileY * grid->gridElemSize * grid->gridElemSize;
    endY = y + grid->gridElemSize;

    runLength = 0;
    runValue = 0;
    x = firstX;
    while (y != endY)
    {
        if (!runLength)
        {
            if (!at)
            {
                // nothing stored, so the whole tile is zero
                runLength = grid->gridElemSize * grid->gridElemSize;
                runValue = 0;
            }
            else
            {
                tag = *at & 0xC0;
                if (tag == 0xC0)
                {
                    runLength = *at & 0x3F;
                    runValue = at[1];
                    at += 2;
                }
                else if (tag == 0x80)
                {
                    runLength = at[1];
                    runValue = at[2];
                    at += 3;
                }
                else if (tag == 0x40)
                {
                    runLength = *(uint16_t*)(at + 1);
                    runValue = at[3];
                    at += 4;
                }
                else
                {
                    runLength = 1;
                    runValue = *at++;
                }
            }
        }

        while (runLength && x != endX)
        {
            --runLength;
            ++x;
            *cell++ = runValue;
        }
        if (x == endX)
        {
            x = firstX;
            cell += rowGap;
            ++y;
        }
    }
}

void GRI_UnCompressReal(tWorldGridStruct* grid)
{
    int tileX;
    int tileY;

    for (tileX = 0; tileX < grid->s3Size7LimitMaxX; ++tileX)
    {
        for (tileY = 0; tileY < grid->s4Size7LimitMaxY; ++tileY)
            GRI_UnCompress(grid, grid->capasPtr, tileX, tileY);
    }
}

// Slides the live window so that it is centred on a world cell, storing
// the tiles it leaves and decoding the ones it arrives at. A window
// that already covers the whole grid in an axis does not move in it.
void GRID_SetRealCenter(tWorldGridStruct* grid, int worldX, int worldY)
{
    int windowX;
    int windowY;
    int half;

    windowX = grid->iInSetCenter1;
    if (grid->iInSetCenter2 - grid->iInSetCenter1 + 1 > 7)
    {
        half = (uint32_t)((double)grid->s3Size7LimitMaxX * 0.5 + 0.5);
        windowX = worldX / grid->gridElemSize - half;
        if (windowX < grid->iInSetCenter1)
            windowX = grid->iInSetCenter1;
        else if (worldX / grid->gridElemSize >= grid->iInSetCenter2)
            windowX = grid->iInSetCenter2 - half;
    }

    windowY = grid->iInSetCenter3;
    if (grid->iInSetCenter4 - grid->iInSetCenter3 + 1 > 7)
    {
        half = (uint32_t)((double)grid->s4Size7LimitMaxY * 0.5 + 0.5);
        windowY = worldY / grid->gridElemSize - half;
        if (windowY < grid->iInSetCenter3)
            windowY = grid->iInSetCenter3;
        else if (worldY / grid->gridElemSize >= grid->iInSetCenter4)
            windowY = grid->iInSetCenter4 - half;
    }

    if (windowX == grid->s5Limit1MinX && windowY == grid->s5Limit1MinY)
        return;

    GRI_CompressReal(grid, windowX, windowY);
    grid->s5Limit1MinX = windowX;
    grid->s5Limit1MinY = windowY;
    GRI_UnCompressReal(grid);
    grid->centerX = (float)((double)(grid->gridElemSize * grid->s5Limit1MinX) + grid->startX);
    grid->centerY = (float)((double)(grid->gridElemSize * grid->s5Limit1MinY) + grid->startY);
}

/* Allocates the per-cell search state and decodes the window into the
   capability grid, on the first call and whenever the world reloads.

   The first pass finds the bounding box of the tiles the file actually
   stores, which is what the window is limited to. A box wider than
   seven tiles in an axis is capped at seven and its limits are opened
   to the whole grid, so the window can slide across it. */
void GRI_Reinit(struct sWorldGridStruct* grid)
{
    int* stored;
    unsigned int size;
    int cells;
    int tileX;
    int tileY;
    int i;

    if (!grid->ptrs)
    {
        grid->iInSetCenter1 = 1000;
        grid->iInSetCenter3 = 1000;
        grid->iInSetCenter2 = -1;
        grid->iInSetCenter4 = -1;

        for (tileX = 0; tileX < grid->gridCountX; ++tileX)
        {
            stored = &grid->ptrInts[tileX];
            for (tileY = 0; tileY < grid->gridCountY; ++tileY)
            {
                if (*stored != -1)
                {
                    if (tileX < grid->iInSetCenter1)
                        grid->iInSetCenter1 = tileX;
                    if (tileY < grid->iInSetCenter3)
                        grid->iInSetCenter3 = tileY;
                    if (tileX > grid->iInSetCenter2)
                        grid->iInSetCenter2 = tileX;
                    if (tileY > grid->iInSetCenter4)
                        grid->iInSetCenter4 = tileY;
                }
                stored += grid->gridCountX;
            }
        }

        if (grid->iInSetCenter2 == -1 || grid->iInSetCenter4 == -1)
        {
            grid->s3Size7LimitMaxX = 0;
            grid->s4Size7LimitMaxY = 0;
        }
        else
        {
            grid->s3Size7LimitMaxX = (int16_t)(grid->iInSetCenter2 - grid->iInSetCenter1 + 1);
            grid->s4Size7LimitMaxY = (int16_t)(grid->iInSetCenter4 - grid->iInSetCenter3 + 1);

            if (grid->s3Size7LimitMaxX > 7)
            {
                grid->s3Size7LimitMaxX = 7;
                grid->iInSetCenter1 = 0;
                grid->iInSetCenter2 = grid->gridCountX;
            }
            if (grid->s4Size7LimitMaxY > 7)
            {
                grid->s4Size7LimitMaxY = 7;
                grid->iInSetCenter3 = 0;
                grid->iInSetCenter4 = grid->gridCountY;
            }
        }

        cells = grid->gridElemSize * grid->gridElemSize
                * grid->s3Size7LimitMaxX * grid->s4Size7LimitMaxY;
        size = sizeof(tGridPtrElem) * cells;
        if (size)
            grid->ptrs = (tGridPtrElem*)MEM_p_Alloc(size);
        LOA_ul_FileTypeSize_39 += size;
        memset(grid->ptrs, 0, size);
    }

    for (i = 0; i < grid->gridCountX * grid->gridCountY; ++i)
    {
        if (grid->ptr[i])
        {
            MEM_Free(grid->ptr[i]);
            grid->ptr[i] = 0;
        }
    }

    grid->s5Limit1MinX = grid->iInSetCenter1;
    grid->s5Limit1MinY = grid->iInSetCenter3;
    GRI_UnCompressReal(grid);
}

/* Reads the compressed grid out of the big file, record by record.
   Each record is a length word followed by a run of tagged runs, and
   the reader copies them through rather than decoding them, so the
   result is what GRI_UnCompress expects.

   NOTE: the 0xC0 branch below can never be taken. The tag is held in a
   char, so `tag & 0xC0` is a positive int, and it is compared against
   0xFFFFFFC0, which is unsigned -- the decompiler's rendering of a
   comparison the compiled code made at byte width. The effect is that a
   short-run tag's value byte is not read and the stream desynchronises
   from there. GRI_UnCompress had the same artefact and carries a fix
   with the note "for routing fix, it was char"; the one-line equivalent
   here is `((uint8_t)tag & 0xC0) == 0xC0`. Left as written pending a
   decision, since it changes what the loader reads. */
char* GRID_ul_CallbackCompressedGrid(uint32_t Offset)
{
    char* src;
    char* end;
    char* recordEnd;
    char* dest;
    char* at;
    char* result;
    char tag;
    int length;
    int size;

    src = BIG_pc_ReadFileTmp(Offset, &size);
    if (!size)
    {
        result = (char*)MEM_p_Alloc(4u);
        *(int32_t*)result = 0;
        return result;
    }

    dest = (char*)MEM_p_Alloc(size);
    LOA_ul_FileTypeSize_39 += size;
    end = &src[size];
    at = dest;

    while (src < end)
    {
        length = LOA_ReadLong(&src, (uint32_t*)at, 1);
        recordEnd = &src[length - 4];
        at += 4;

        while (src < recordEnd)
        {
            tag = LOA_ReadChar(&src, 0, 1);
            *at++ = tag;

            if ((tag & 0xC0) == 0xFFFFFFC0)
            {
                // see the note above: never taken
                *at++ = LOA_ReadChar(&src, 0, 1);
            }
            else if (tag == (char)0x80)
            {
                *at++ = LOA_ReadChar(&src, 0, 1);
                *at++ = LOA_ReadChar(&src, 0, 1);
            }
            else if (tag == 64)
            {
                LOA_ReadShort(&src, (uint16_t*)at, 1);
                at += 2;
                *at++ = LOA_ReadChar(&src, 0, 1);
            }
        }
    }
    return dest;
}

tWorldGridStruct* GRID_ul_CallbackWorldGrid(uint32_t Offset)
{
    tWorldGridStruct* grid;
    int* stored;
    unsigned int size;
    int tiles;
    int capasSize;
    char* src;
    int fileSize;
    int i;

    GRID_gi_Current = 0;
    grid = (tWorldGridStruct*)MEM_p_Alloc(sizeof(tWorldGridStruct));
    LOA_ul_FileTypeSize_39 += sizeof(tWorldGridStruct);
    memset(grid, 0, sizeof(tWorldGridStruct));

    src = BIG_pc_ReadFileTmp(Offset, &fileSize);
    LOA_ReadLong(&src, 0, 2);
    LOA_ReadLong(&src, 0, 2);
    grid->compressedGrid = (char*)LOA_ReadLong(&src, 0, 1);
    LOA_ReadLong(&src, 0, 2);
    LOA_ReadLong(&src, 0, 2);
    LOA_ReadLong(&src, 0, 2);
    grid->startX = LOA_ReadFloat(&src, 0, 1);
    grid->startY = LOA_ReadFloat(&src, 0, 1);
    grid->centerX = LOA_ReadFloat(&src, 0, 1);
    grid->centerY = LOA_ReadFloat(&src, 0, 1);
    grid->gridCountX = LOA_ReadShort(&src, 0, 1);
    grid->gridCountY = LOA_ReadShort(&src, 0, 1);
    LOA_ReadShort(&src, 0, 2);
    LOA_ReadShort(&src, 0, 2);
    grid->s5Limit1MinX = LOA_ReadShort(&src, 0, 1);
    grid->s5Limit1MinY = LOA_ReadShort(&src, 0, 1);
    grid->gridElemSize = LOA_ReadChar(&src, 0, 1);
    LOA_ReadLong(&src, 0, 2);
    LOA_ReadChar(&src, 0, 2);
    LOA_ReadChar(&src, 0, 2);
    LOA_ReadChar(&src, 0, 2);

    // the window starts at the largest it can be; GRI_Reinit shrinks it
    // to the tiles the file actually stores
    grid->s3Size7LimitMaxX = 7;
    grid->s4Size7LimitMaxY = 7;
    grid->i2 = 0;

    tiles = grid->gridCountY * grid->gridCountX;
    size = 4 * tiles;
    grid->ptrInts = (int*)MEM_p_Alloc(size);
    LOA_ul_FileTypeSize_39 += size;
    grid->ptr = (char**)MEM_p_Alloc(size);
    LOA_ul_FileTypeSize_39 += size;
    memset(grid->ptr, 0, size);

    stored = grid->ptrInts;
    for (i = 0; i < grid->gridCountY * grid->gridCountX; ++i)
        *stored++ = LOA_ReadLong(&src, 0, 1);

    LOA_MakeFileRef((int)grid->compressedGrid, &grid->compressedGrid,
                    GRID_ul_CallbackCompressedGrid, 1);

    capasSize = grid->s3Size7LimitMaxX * grid->gridElemSize;
    capasSize *= grid->s4Size7LimitMaxY * grid->gridElemSize;
    grid->capasPtr = (char*)MEM_p_Alloc(capasSize);
    LOA_ul_FileTypeSize_39 += capasSize;
    grid->ptrs = 0;
    return grid;
}

void GRI_Destroy(tWorldGridStruct* grid)
{
    int i;

    if (!grid)
        return;

    for (i = 0; i < grid->gridCountX * grid->gridCountY; ++i)
    {
        if (grid->ptr[i])
            MEM_Free(grid->ptr[i]);
    }
    if (grid->compressedGrid)
    {
        LOA_DeleteAddress((int)grid->compressedGrid);
        MEM_Free(grid->compressedGrid);
    }
    if (grid->ptrInts)
        MEM_Free(grid->ptrInts);
    if (grid->ptr)
        MEM_Free(grid->ptr);
    if (grid->capasPtr)
        MEM_Free(grid->capasPtr);
    if (grid->ptrs)
        MEM_Free(grid->ptrs);
    LOA_DeleteAddress((int)grid);
    MEM_Free(grid);
}

// Whether the cell one step from pos, in the given direction, is a wall
// or is taken.
static BOOL GRI_l_Blocked(tWorldGridStruct* grid, Vec3* pos, float dx, float dy, int blocked)
{
    Vec3 at;

    at.x = (float)(pos->x + dx);
    at.y = (float)(pos->y + dy);
    at.z = pos->z;
    return (GRI_i_GetCapa(grid, &at) & blocked) != 0;
}

/* Whether the object is hemmed in: blocked on both sides of its line of
   sight. The two cells beside it are tested first, and then, where the
   sight points along the dominant axis, the two cells diagonally that
   way as well.

   The two halves are the same test with the axes exchanged -- note that
   side A is the cell above when the sight runs along x and the cell to
   the left when it runs along y. The original reached the second half's
   tail through a goto into the first's. */
BOOL GRI_Surround(tWorldGridStruct* grid, Vec3* pos, Vec3* sight)
{
    int blocked;
    int sideA;
    int sideB;

    blocked = GRID_gi_TestWall | 0xC0;
    sideA = 0;
    sideB = 0;

    if (fabs(sight->x) >= fabs(sight->y))
    {
        if (GRI_l_Blocked(grid, pos, 0.0f, 1.0f, blocked))
            sideA = 1;
        if (GRI_l_Blocked(grid, pos, 0.0f, -1.0f, blocked))
            sideB = 1;
        if (sideA && sideB)
            return 1;

        if (sight->x > 0.0)
        {
            if (GRI_l_Blocked(grid, pos, 1.0f, 1.0f, blocked))
                ++sideA;
            if (sideA && sideB)
                return 1;
            if (GRI_l_Blocked(grid, pos, 1.0f, -1.0f, blocked))
                ++sideB;
            return sideA && sideB;
        }
        if (sight->x < 0.0)
        {
            if (GRI_l_Blocked(grid, pos, -1.0f, 1.0f, blocked))
                ++sideA;
            if (sideA && sideB)
                return 1;
            if (GRI_l_Blocked(grid, pos, -1.0f, -1.0f, blocked))
                ++sideB;
            return sideA && sideB;
        }
        return 0;
    }

    if (GRI_l_Blocked(grid, pos, -1.0f, 0.0f, blocked))
        sideA = 1;
    if (GRI_l_Blocked(grid, pos, 1.0f, 0.0f, blocked))
        sideB = 1;
    if (sideA && sideB)
        return 1;

    if (sight->y > 0.0)
    {
        if (GRI_l_Blocked(grid, pos, -1.0f, 1.0f, blocked))
            ++sideA;
        if (sideA && sideB)
            return 1;
        if (GRI_l_Blocked(grid, pos, 1.0f, 1.0f, blocked))
            ++sideB;
        return sideA && sideB;
    }
    if (sight->y < 0.0)
    {
        if (GRI_l_Blocked(grid, pos, -1.0f, -1.0f, blocked))
            ++sideA;
        if (sideA && sideB)
            return 1;
        if (GRI_l_Blocked(grid, pos, 1.0f, -1.0f, blocked))
            ++sideB;
        return sideA && sideB;
    }
    return 0;
}

/* Fills cells with the positions a tag covers, and returns how many.

   shape 0 is a four-cell block: the cell itself, the horizontal
   neighbour the position leans towards, the vertical one it leans
   towards, and the diagonal between them. shape 2 is the cell and all
   eight around it, built as a spiral outwards. Any other shape is the
   cell alone. */
int GRI_GetTag(tWorldGridStruct* grid, Vec3* pos, int shape, int capability, Vec3* cells)
{
    float x;
    float y;
    float z;
    float leanX;
    float leanY;
    float backX;
    int cellX;
    int cellY;

    GRI_3Dto2D(grid, pos, &cellX, &cellY);
    x = pos->x;
    y = pos->y;
    z = pos->z;
    cells[0] = *pos;

    if (shape == 2)
    {
        cells[1].x = (float)(x - 1.0);
        cells[1].y = y;
        cells[1].z = z;
        cells[2].x = (float)(cells[1].x + 2.0);
        cells[2].y = y;
        cells[2].z = z;
        cells[3].x = cells[2].x;
        cells[3].y = (float)(y - 1.0);
        cells[3].z = z;
        cells[4].x = (float)(cells[3].x - 1.0);
        cells[4].y = cells[3].y;
        cells[4].z = z;
        cells[5].x = (float)(cells[4].x - 1.0);
        cells[5].y = cells[3].y;
        cells[5].z = z;
        cells[6].x = cells[5].x;
        cells[6].y = (float)(cells[3].y + 2.0);
        cells[6].z = z;
        cells[7].x = (float)(cells[6].x + 1.0);
        cells[7].y = cells[6].y;
        cells[7].z = z;
        cells[8].x = (float)(cells[7].x + 1.0);
        cells[8].y = cells[6].y;
        cells[8].z = z;
        return 9;
    }

    if (shape != 0)
        return 1;

    /* The position's offset within its cell decides which way the block
       leans: more than half a cell past the cell's own coordinate and
       it takes the next cell rather than the previous one. */
    cells[1].y = y;
    cells[1].z = z;
    if ((float)fabs(((float)cellX + grid->startX) - pos->x) > 0.5)
    {
        leanX = (float)(x - 1.0);
        backX = (float)(leanX + 1.0);
    }
    else
    {
        leanX = (float)(x + 1.0);
        backX = (float)(leanX - 1.0);
    }
    cells[1].x = leanX;

    if ((float)fabs(((float)cellY + grid->startY) - pos->y) <= 0.5)
        leanY = (float)(y - 1.0);
    else
        leanY = (float)(y + 1.0);

    cells[2].x = leanX;
    cells[2].y = leanY;
    cells[2].z = z;
    cells[3].x = backX;
    cells[3].y = leanY;
    cells[3].z = z;
    return 4;
}

int GRI_Tag(tWorldGridStruct* grid, Vec3* pos, int shape, int capability)
{
    Vec3 cells[20];
    int num;

    num = GRI_GetTag(grid, pos, shape, capability, cells);
    return GRI_l_TagCells(grid, cells, num, capability);
}

/* Tags every cell of the rectangle between two world positions. Returns
   failure when a cell refuses, and also when the rectangle is larger
   than the two hundred cells the list holds, in which case nothing is
   tagged at all. */
int GRI_TagBox(struct sWorldGridStruct* grid, Vec3* pos, Vec3* to, char capability)
{
    Vec3 cells[200];
    float minX;
    float minY;
    float maxX;
    float maxY;
    float swap;
    double x;
    double y;
    int num;
    int cellX;
    int cellY;

    GRI_3Dto2D(grid, pos, &cellX, &cellY);
    minX = (float)((double)cellX + grid->startX);
    minY = (float)((double)cellY + grid->startY);

    // note that the second conversion's two outputs are taken the other
    // way round, so cellY carries the x and cellX the y
    GRI_3Dto2D(grid, to, &cellY, &cellX);
    maxX = (float)((double)cellY + grid->startX);
    maxY = (float)((double)cellX + grid->startY);

    if (minX > (double)maxX)
    {
        swap = minX;
        minX = maxX;
        maxX = swap;
    }
    if (minY > (double)maxY)
    {
        swap = minY;
        minY = maxY;
        maxY = swap;
    }

    num = 0;
    for (x = minX; x <= (double)maxX; x = x + 1.0)
    {
        for (y = minY; y <= (double)maxY; y = y + 1.0)
        {
            cells[num].x = (float)x;
            cells[num].y = (float)y;
            ++num;
            if (num == 200)
                return 0;
        }
    }
    return GRI_l_TagCells(grid, cells, num, capability);
}

/* Fills cells with the two positions a vector tag covers: the cell
   itself and its neighbour along whichever axis the vector is weaker
   in, which is the cell the object would be pushed into. A non-zero
   shape covers the cell alone. */
int GRI_GetTagVec(tWorldGridStruct* grid, Vec3* pos, int shape, int capability, Vec3* dir, Vec3* cells)
{
    float x;
    float y;
    float z;
    float dirX;
    float lean;
    int cellX;
    int cellY;

    GRI_3Dto2D(grid, pos, &cellX, &cellY);
    x = pos->x;
    y = pos->y;
    z = pos->z;
    cells[0] = *pos;

    dirX = dir->x;
    if (fabs(dir->y) > fabs(dir->x))
        dirX = 0.0f;

    if (shape)
        return 1;

    if (dirX == 0.0f)
    {
        if ((float)fabs(((float)cellY + grid->startY) - pos->y) <= 0.5f)
            lean = (float)(y - 1.0);
        else
            lean = (float)(y + 1.0);
        cells[1].x = x;
        cells[1].y = lean;
        cells[1].z = z;
        return 2;
    }

    if ((float)fabs(((float)cellX + grid->startX) - pos->x) <= 0.5f)
        lean = (float)(x - 1.0);
    else
        lean = (float)(x + 1.0);
    cells[1].x = lean;
    cells[1].y = y;
    cells[1].z = z;
    return 2;
}

int GRI_TagVec(struct sWorldGridStruct* grid, Vec3* pos, int shape, int capability, Vec3* dir)
{
    Vec3 cells[20];
    int num;

    num = GRI_GetTagVec(grid, pos, shape, capability, dir, cells);
    return GRI_l_TagCells(grid, cells, num, capability);
}

int GRI_GetTagDyn(struct sWorldGridStruct* grid, Vec3* pos, float distLimit, Vec3* gridPosRet)

{
    float gridRealStartPosX;
    float gridRealStartPosY;
    float distX;
    float distY;
    int retValX, retValY;

/*
// x 0 3 6 9
// y 1 4 7 10
// z 2 5 8 11
*/

    GRI_3Dto2D(grid, pos, &retValX, &retValY);
    
    gridRealStartPosX = (float)retValX + grid->startX + 0.5f;
    gridRealStartPosY = (float)retValY + grid->startY + 0.5f;
    distX = pos->x - gridRealStartPosX;
    distY = pos->y - gridRealStartPosY;
    
    gridPosRet->x = gridRealStartPosX;
    gridPosRet->y = gridRealStartPosY;
    gridPosRet->z = 0.0f;

    distLimit = 0.5f - distLimit;
    if (distX > distLimit) {
        gridPosRet[1].z = 0.0f; 
        gridPosRet[1].y = gridRealStartPosY;
        gridPosRet[1].x = gridRealStartPosX;

        if (distY > distLimit) {
            gridPosRet[1].x = gridRealStartPosX + 1.0f;
            gridPosRet[2].x = gridRealStartPosX;
            gridPosRet[2].y = gridRealStartPosY + 1.0f;
            gridPosRet[2].z = 0.0f;
            gridPosRet[3].x = gridRealStartPosX + 1.0f;
            gridPosRet[3].y = gridRealStartPosY + 1.0f;
            gridPosRet[3].z = 0.0f;
            return 4;
        }

        gridPosRet[1].x = gridPosRet[1].x + 1.0f;
        if (-distLimit > distY) {
            gridPosRet[2].x = gridRealStartPosX;
            gridPosRet[2].y = gridRealStartPosY - 1.0f;
            gridPosRet[2].z = 0.0f;
            gridPosRet[3].x = gridRealStartPosX + 1.0f;
            gridPosRet[3].y = gridRealStartPosY - 1.0f;
            gridPosRet[3].z = 0.0f;
            return 4;
        }
    }
    else {
        if (-distLimit <= distX) {
            if (distY <= distLimit) {
                if (distY < -distLimit) {
                    gridPosRet[1].x = gridRealStartPosX;
                    gridPosRet[1].y = gridRealStartPosY - 1.0f;
                    gridPosRet[1].z = 0.0f;
                    return 2;
                }
                return 1;
            }
            gridPosRet[1].x = gridRealStartPosX;
            gridPosRet[1].y = gridRealStartPosY + 1.0f;
            gridPosRet[1].z = 0.0f;
        }
        else {
            gridPosRet[1].x = gridRealStartPosX;
            gridPosRet[1].y = gridRealStartPosY;
            gridPosRet[1].z = 0.0f;

            if (distY > distLimit) {
                gridPosRet[1].x = gridRealStartPosX - 1.0f;
                gridPosRet[2].x = gridRealStartPosX;
                gridPosRet[2].y = gridRealStartPosY + 1.0f;
                gridPosRet[2].z = 0.0f;
                gridPosRet[3].x = gridRealStartPosX - 1.0f;
                gridPosRet[3].y = gridRealStartPosY + 1.0f;
                gridPosRet[3].z = 0.0f;
                return 4;
            }
            gridPosRet[1].x = gridRealStartPosX - 1.0f;
            if (distY < -distLimit) {
                gridPosRet[2].x = gridRealStartPosX;
                gridPosRet[2].y = gridRealStartPosY - 1.0f;
                gridPosRet[2].z = 0.0f;
                gridPosRet[3].x = gridRealStartPosX - 1.0f;
                gridPosRet[3].y = gridRealStartPosY - 1.0f;
                gridPosRet[3].z = 0.0f;
                return 4;
            }
        }
    }
    return 2;
}

int GRI_TagDyn(struct sWorldGridStruct* grid, Vec3* pos, float distLimit, Vec3* cells, char capability)
{
    int num;

    num = GRI_GetTagDyn(grid, pos, distLimit, cells);
    return GRI_l_TagCells(grid, cells, num, capability);
}
