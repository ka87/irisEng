/* What basic_array.c asks of the engine, which is only the allocator.
 *
 * The C library's is used here so that this check does not depend on
 * the engine's own: what is under test is the array's bookkeeping, not
 * where its storage comes from. A realloc that moves the block is the
 * interesting case for the array and malloc is free to do it, which is
 * one more thing the engine's pool would hide.
 */

#include <stdlib.h>

void* MEM_p_Alloc(unsigned int size) { return malloc(size ? size : 1u); }
void* MEM_p_Realloc(void* src, size_t size)
{
    return realloc(src, size ? size : 1u);
}
void MEM_Free(void* ptr) { free(ptr); }
