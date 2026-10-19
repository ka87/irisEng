#include "way.h"

void WAY_Free(struct sGroGeneric* a1)
{  
}

void WAY_Init()
{
    GRO_gast_Interface[7].Destroy = WAY_Free;
    WAY_gst_GroStruct = &GRO_gast_Interface[7];
}