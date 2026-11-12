/* The three allocator calls math.c makes. Nothing is freed: the test
   needs no reuse and a leak keeps the two copies' state separate. */
#include <stdlib.h>
#include <stddef.h>

void* MEM_p_AllocAligned(int size, int align)
{
    void* p = NULL;
    if (align < (int)sizeof(void*)) align = sizeof(void*);
    if (posix_memalign(&p, (size_t)align, size ? size : 1) != 0) return NULL;
    return p;
}

void* MEM_p_ReallocAligned(void* old, unsigned int size, int align)
{
    void* p = MEM_p_AllocAligned(size, align);
    (void)old;
    return p;
}

void MEM_FreeAlign(void* p) { (void)p; }
