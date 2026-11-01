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

/* Walk every hole chain this often (in allocations) and report the
   first descriptor that has gone bad. The point is to bracket the
   corruption in time: the scan that finds it ran after the write, the
   scan before it ran before, so the window is however many operations
   separate them. Lower it to narrow the window; 0 turns it off.

   Set MEMCHK_SCAN_ON_FREE to also scan after every free, which is slow
   but brackets the write to a single free. */
#define MEMCHK_SCAN_EVERY    100
#define MEMCHK_SCAN_ON_FREE  0

/* Catch the writer instead of the reader. When a scan finds a bad
   descriptor, make its page read-only and log the instruction address
   of whatever writes to it next, then let the write through and carry
   on. The corrupting write strides across hundreds of bytes, so it
   comes back to the same page and gets caught on the next pass. Set to
   0 to leave pages alone. */
#define MEMCHK_PAGE_TRAP     1

#if MEMCHK_ENABLED
void MEMCHK_Init(unsigned int base, int size);
/* describe an address relative to the heap; for reading a crash address */
void MEMCHK_Where(const char* what, unsigned int addr);
/* returns the number of bad descriptors found */
int  MEMCHK_ScanChains(const char* when);
/* words either side of a descriptor, to show the shape of whatever
   wrote it rather than just the one word that is wrong */
void MEMCHK_Dump(const char* what, int* holeDesc);
/* make this address's page read-only and report what writes to it */
void MEMCHK_ArmPageTrap(void* addr);
/* returns 1 when the hole is safe to follow, 0 when it is not */
int  MEMCHK_HoleIsSound(unsigned int elemID, int* holeDesc);
void MEMCHK_Note(const char* path, unsigned int size, unsigned int elemID, int blockSize);
void MEMCHK_Exhausted(unsigned int size, unsigned int elemID, int blockSize);
#else
#define MEMCHK_Init(b, s)             ((void)0)
#define MEMCHK_Where(w, a)            ((void)0)
#define MEMCHK_ScanChains(w)          (0)
#define MEMCHK_Dump(w, h)             ((void)0)
#define MEMCHK_ArmPageTrap(a)         ((void)0)
#define MEMCHK_HoleIsSound(e, h)      (1)
#define MEMCHK_Note(p, s, e, b)       ((void)0)
#define MEMCHK_Exhausted(s, e, b)     ((void)0)
#endif
