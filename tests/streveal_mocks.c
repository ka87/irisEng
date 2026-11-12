/* Everything geostr.c refers to that the reveal effect does not need.
   The test calls only the reveal, so these exist to let the file link.
   Each one aborts if it is ever reached, which is the point: if the
   reveal grows a dependency, the test says so rather than passing on a
   stand-in. */

#include <stdio.h>
#include <stdlib.h>

#include "../graphicsobjects/geostr.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/gro_common.h"

static void MOCK_l_Unreachable(const char* who)
{
    printf("the reveal test reached %s, which it never should\n", who);
    abort();
}

#define UNREACHABLE(name) MOCK_l_Unreachable(name)

tGroIntf GRO_gast_Interface[12];

void GRO_Struct_Init(tGroGeneric* gro, int type) { UNREACHABLE("GRO_Struct_Init"); }
void LOA_DeleteAddress(int addr) { UNREACHABLE("LOA_DeleteAddress"); }
int LOA_ReadLong(char** at, unsigned int* out, char mode) { UNREACHABLE("LOA_ReadLong"); return 0; }
void* MEM_p_Alloc(unsigned int size) { UNREACHABLE("MEM_p_Alloc"); return 0; }
void MEM_Free(void* ptr) { UNREACHABLE("MEM_Free"); }

int D3D_FlatLayerOffsetX(int w, int h) { UNREACHABLE("D3D_FlatLayerOffsetX"); return 0; }
int D3D_FlatLayerWidth(int w, int h) { UNREACHABLE("D3D_FlatLayerWidth"); return 0; }
void GFX_NeedGeomEx(unsigned int a, unsigned int b, unsigned int c,
                    unsigned int d, int e) { UNREACHABLE("GFX_NeedGeomEx"); }
void GFX_NeedGeomExElem(unsigned int a, unsigned int b) { UNREACHABLE("GFX_NeedGeomExElem"); }
unsigned int LIGHT_ul_Interpol2Colors(int a, int b, float t) { UNREACHABLE("LIGHT_ul_Interpol2Colors"); return 0; }
float MATH_GetSin(float f) { UNREACHABLE("MATH_GetSin"); return 0.0f; }
float MATH_GetCos(float f) { UNREACHABLE("MATH_GetCos"); return 0.0f; }
float RND_RandFloatRange(float a, float b) { UNREACHABLE("RND_RandFloatRange"); return 0.0f; }

struct sGameObjectMain* OBJ_GameObject_Create(int key) { UNREACHABLE("OBJ_GameObject_Create"); return 0; }
void AI_ExecCallback(struct sGameObjectMain* gao, int which) { UNREACHABLE("AI_ExecCallback"); }
void STR_SetDisplay(void) { UNREACHABLE("STR_SetDisplay"); }
void STR_GetRectShort(tGEOSTRStruct* str, int idx, int a, int b, Vec3* min, Vec3* max) { UNREACHABLE("STR_GetRectShort"); }
int STR_l_GetValidIndex(tGEOSTRStruct* str, tGEOSTRStringData* sd, int idx) { UNREACHABLE("STR_l_GetValidIndex"); return 0; }
int STR_l_GetValidNumber(tGEOSTRStruct* str, tGEOSTRStringData* sd, int idx, int num) { UNREACHABLE("STR_l_GetValidNumber"); return 0; }
struct sGEOSTRStringData* STR_pst_GetString(tGEOSTRStruct* str, int idx) { UNREACHABLE("STR_pst_GetString"); return 0; }
struct sFontData* STR_pst_GetFontDesc(struct sGEOMAT* mat, int16_t id) { UNREACHABLE("STR_pst_GetFontDesc"); return 0; }
