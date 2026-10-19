#include "sectors.h"

void* GRPSectClearAll()
{
    gi_NumSectors = 0;
    return memset(gai_NumObjSectors, 0, sizeof(gai_NumObjSectors));
}

void GRPSectDelObj(tGameObjectMain* gao)
{
    int sectorID;

    if (gao->extended)
    {
        sectorID = gao->extended->sectorID;
        if (sectorID != 0xFFFF)
        {
            if (gai_NumObjSectors[sectorID])
            {
                gapst_AllObjSectors[(100 * sectorID) + gai_NumObjSectors[sectorID] - 1]->extended->sectorElemIdx = gao->extended->sectorElemIdx;
                gapst_AllObjSectors[(100 * sectorID) + gao->extended->sectorElemIdx] = gapst_AllObjSectors[(100 * sectorID) + gai_NumObjSectors[sectorID] - 1];
                gapst_AllObjSectors[(100 * sectorID) + (gai_NumObjSectors[sectorID]--) - 1] = 0;
            }
        }
    }
}
