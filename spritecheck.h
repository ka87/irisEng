#pragma once

/* A check on the vertices the sprite generator produces.
 *
 * Set SPGCHK_ENABLED to 1 and every batch is scanned before it is
 * drawn. Anything that cannot be a position on screen is reported
 * once per batch, with the inputs that produced it, so a bad vertex
 * can be traced back to the noise vectors and the random table
 * entries it was built from rather than guessed at.
 *
 * Reports go through the ordinary debug output. Each line starts
 * SPGCHK so they can be filtered out of a capture.
 *
 * SPGCHK_LIMIT caps how many batches report, so a map that is wrong
 * in every batch does not produce a gigabyte of log. Set it to 0 for
 * no cap. */

#define SPGCHK_ENABLED  1
#define SPGCHK_LIMIT    40

/* Anything beyond this many world units from the origin is treated as
   impossible. The visible water quad is laid out at 18000, so this is
   well clear of anything legitimate. */
#define SPGCHK_SANE_RANGE  1.0e6f

struct sVec3;

#if SPGCHK_ENABLED
void SPGCHK_CheckBatch(const char* where, int num);
void SPGCHK_ReportInputs(const char* where);
#else
#define SPGCHK_CheckBatch(where, num)  ((void)0)
#define SPGCHK_ReportInputs(where)     ((void)0)
#endif
