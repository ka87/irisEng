#pragma once
#include <stdint.h>

/* Text rendering. The faces live in data\fonts beside the
   executable, FreeType rasterizes them, and the glyphs reach the
   screen as textured quads through the renderer interface -- so
   nothing here knows which renderer is running.

   Two faces, because the engine has always had two: a small
   proportional one for the raster readouts and a monospaced one for
   the command overlay, whose size follows the viewport. */
enum
{
    FNT_RASTERS = 0,   /* data\fonts\LiberationSans-Regular.ttf */
    FNT_OVERLAY = 1,   /* data\fonts\LiberationMono-Regular.ttf */
    FNT_COUNT   = 2
};

/* The overlay's size, which the viewport decides. Kept here rather
   than in the renderer because it is a layout question. */
#define FNT_OVERLAY_MINHEIGHT 14
#define FNT_OVERLAY_MAXHEIGHT 44

void FNT_InitModule(void);

/* Gives back the atlas textures and the FreeType objects. Safe to
   call twice and safe to call having never drawn anything. */
void FNT_CloseModule(void);

/* The line height in pixels, and what D3DX called the font height.
   Asking the overlay for it is also what resizes it after the
   viewport changed, so the command field measures itself against the
   size it is about to be drawn at. */
int FNT_i_Height(int face);

/* The width the string will occupy, measured rather than estimated:
   the caret sits at the end of the line, and a per-character guess
   drifts further from the text with every character typed. */
int FNT_i_TextWidth(int face, const char* text);

/* Draws with (x, y) at the top left of the line, which is where a
   rectangle's top used to put it. Expects to be called inside the
   renderer's overlay pass, which is what sets up the pixel-space
   projection and the alpha blending. */
void FNT_DrawText(int face, const char* text, int x, int y, uint32_t color);

/* The glyphs an atlas holds: printable ASCII. A character outside it
   is drawn and measured as FNT_FALLBACKCHAR, so a string that
   contains one still measures as wide as it draws. */
#define FNT_FIRSTCHAR    32
#define FNT_LASTCHAR     126
#define FNT_GLYPHCOUNT   (FNT_LASTCHAR - FNT_FIRSTCHAR + 1)
#define FNT_FALLBACKCHAR '?'
