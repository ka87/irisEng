#pragma once

/* Diagnostics for the dynamic allocator.
 *
 * Every allocation reports which of the four paths it took, and the
 * hole chain is validated before it is followed, because a hole whose
 * link has been overwritten crashes inside MEM_p_Alloc at the moment
 * it is dereferenced -- which looks like a fault in the allocator
 * rather than in whatever corrupted the block.
 *
 * MEMCHK_VERBOSE 1 reports every allocation, which is a great many
 * lines; 0 reports only the faults and a periodic summary. Start at 0.
 *
 * Lines begin MEMCHK so they can be filtered out of a capture. */

#define MEMCHK_ENABLED  1
#define MEMCHK_VERBOSE  0
#define MEMCHK_SUMMARY_EVERY 2000

#if MEMCHK_ENABLED
void MEMCHK_Init(unsigned int base, int size);
/* describe an address relative to the heap; for reading a crash address */
void MEMCHK_Where(const char* what, unsigned int addr);
/* returns 1 when the hole is safe to follow, 0 when it is not */
int  MEMCHK_HoleIsSound(unsigned int elemID, int* holeDesc);
void MEMCHK_Note(const char* path, unsigned int size, unsigned int elemID, int blockSize);
void MEMCHK_Exhausted(unsigned int size, unsigned int elemID, int blockSize);
#else
#define MEMCHK_Init(b, s)             ((void)0)
#define MEMCHK_Where(w, a)            ((void)0)
#define MEMCHK_HoleIsSound(e, h)      (1)
#define MEMCHK_Note(p, s, e, b)       ((void)0)
#define MEMCHK_Exhausted(s, e, b)     ((void)0)
#endif
