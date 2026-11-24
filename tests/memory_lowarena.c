/* Puts the allocator's arena where a 32-bit build would have it.
 *
 * The arena is one block taken from the C library, and its address is
 * kept in a 32-bit field -- so on a 64-bit host the allocator loses
 * the top half of its own base pointer before it does anything else.
 * Linked into the driver with -Wl,--wrap=malloc, this hands the big
 * request back a mapping below 4 GB, which is what a 32-bit build
 * would get and what makes the existing logic exercisable here.
 *
 * Only the arena-sized request is diverted. Everything smaller goes
 * to the real allocator, so the driver's own bookkeeping and the C
 * library are untouched; and -Wl,--wrap only reaches calls in the
 * objects being linked, never the ones inside libc.
 *
 * MAP_32BIT is Linux on x86-64. Where it is absent the mapping is
 * refused and the wrapper says so rather than quietly handing back a
 * high address and letting the driver report an allocator bug that is
 * really a missing flag.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

void* __real_malloc(size_t size);

/* Anything this big is the arena; the driver never asks for so much. */
#define LOWARENA_MIN 0x100000u

int LOWARENA_diverted;

void* __wrap_malloc(size_t size)
{
    void* p;

    if (size < LOWARENA_MIN)
        return __real_malloc(size);

#if defined(__linux__) && defined(MAP_32BIT) && defined(__x86_64__)
    p = mmap(NULL, size, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    if (p == MAP_FAILED || (uintptr_t)p > 0xFFFFFFFFu)
    {
        if (p != MAP_FAILED)
            munmap(p, size);
        printf("  SKIP: no mapping below 4 GB was available, so the arena\n");
        printf("        cannot be put where a 32-bit build would have it.\n");
        exit(0);
    }
    ++LOWARENA_diverted;
    return p;
#else
    (void)p;
    printf("  SKIP: this host has no way to ask for memory below 4 GB.\n");
    exit(0);
#endif
}
