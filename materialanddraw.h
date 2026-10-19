#pragma once
#include "common.h"
#include "graphicsobjects/geo.h"
#include "graphicsobjects/geomaterial.h"

extern tGEOMAT MAT_gst_DefaultSingleMaterial;

void GAO_XMN_Render_TransparentTriangles(struct sGEOGRO* geogro, struct sGEOMAT* geomat);
void MAT_DrawIndexedTriangle(struct sGEOGRO* geogro, struct sGEOMAT* geomat, struct sGeoTriangleChunks* triangleChunks);
void GEO_Render_TransparentTriangles(struct sGdi_DisplayData* a1, struct sGEOGRO* a2, struct sGameObjectVisualObject* a3);
BOOL MAT_IsMaterialTransparent(struct sGEOMAT* geomat, int idx, int renderingFlags, void* unk1);
void MAT_DrawIndexedSprites(struct sGEOGRO* a1, struct sGEOMAT* a2, struct sGeoSprites* a4);
unsigned int MAT_GetValidityMASK(struct sGEOMAT* a1, int a2, struct sGEOGRO* a3);
BOOL MAT_VUISIdentity(struct sMultiTextureElemStruct* a1);
void MAT_UV_COMPUTE_GetMAtrix(struct sGdi_DisplayData* a1, struct sGameObjectMain* a2, unsigned int a3, Mtx4x4* a4);
void MAT_SetScale(int* a1, float a2);
void MAT_SetPos(int* a1, float a2);
void MAT_SetRotation(int* a1, int* a2, float a3);
float MAT_GetSpeed(int a1);
float MAT_GetScale(int a1);
float MAT_GetRotation(int a1, unsigned int a2);