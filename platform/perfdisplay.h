#pragma once

// The performance display `tweak perf` puts over the frame: a red band
// when the frame rate is up, a frame-rate row, and one bar per
// profiler raster with the longest at the top.
//
// Drawn through the same three interface entries the overlay uses and
// the same debug font, so there is one copy of it rather than one per
// renderer. What it decides is in perfbars.h; what it reads is the
// profiler's own per-frame sums.
//
// Called once a frame from a renderer's flip, before the overlay, so
// the overlay's panel sits on top of it -- which is the order the
// Direct3D flip has always had.
void PFD_Draw(void);
