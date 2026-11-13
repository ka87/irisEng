#pragma once
#include "common.h"
#include "graphicsobjects/geo.h"
#include "graphicsobjects/geomaterial.h"

extern tGEOMAT MAT_gst_DefaultSingleMaterial;

void GAO_XMN_Render_TransparentTriangles(struct sGEOGRO* geogro, struct sGEOMAT* geomat);
void MAT_DrawIndexedTriangle(struct sGEOGRO* geogro, struct sGEOMAT* geomat, struct sGeoTriangleChunks* triangleChunks);
void GEO_Render_TransparentTriangles(struct sGdi_DisplayData* dxdd, struct sGEOGRO* geogro, struct sGameObjectVisualObject* visual);
BOOL MAT_IsMaterialTransparent(struct sGEOMAT* geomat, int idx, int renderingFlags, void* unk1);
void MAT_DrawIndexedSprites(struct sGEOGRO* geogro, struct sGEOMAT* geomat, struct sGeoSprites* spr);
unsigned int MAT_GetValidityMASK(struct sGEOMAT* geomat, int idx, struct sGEOGRO* geogro);
BOOL MAT_VUISIdentity(struct sMultiTextureElemStruct* texElem);
void MAT_UV_COMPUTE_GetMAtrix(struct sGdi_DisplayData* dxdd, struct sGameObjectMain* gao, unsigned int uvMode, Mtx4x4* mtx);
void MAT_SetScale(int* transform, float scale);
void MAT_SetPos(int* transform, float pos);
void MAT_SetRotation(int* transformU, int* transformV, float angle);
float MAT_GetSpeed(int transform);
float MAT_GetScale(int transform);
float MAT_GetRotation(int transformU, unsigned int transformV);