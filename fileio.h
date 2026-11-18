#pragma once
#include <stdio.h>

/* Reading a file that does not fit in a long.
 *
 * The big file is bigger than two gigabytes and the positions inside
 * it are whole file offsets, so seeking in it cannot go through fseek,
 * whose offset is a long -- thirty-two bits where this is built.
 *
 * It used to go through fgetpos and fsetpos with arithmetic done on an
 * fpos_t, which works only because Microsoft's fpos_t happens to be a
 * sixty-four bit integer. The type is opaque by the standard and is a
 * struct under glibc, so that was the one thing in the file that could
 * not compile anywhere else -- and the warning sweep could not say so,
 * because it looks for warnings and a file that does not compile has
 * none.
 *
 * POSIX spells the wide seek fseeko and ftello and Microsoft's library
 * has the same two under names of its own, so these say the question
 * once and each platform answers it -- the same arrangement the engine
 * has for strcasecmp. They are functions rather than macros because a
 * macro named after a library call rewrites every identifier that
 * happens to share the name, and a harness cannot stand in for one.
 *
 * Off Windows the wide seek wants off_t sixty-four bits wide, which is
 * what _FILE_OFFSET_BITS asks for and what the build sets. A
 * thirty-two bit build without it would seek fine up to two gigabytes
 * and fail past it, which is the worst way to be wrong, so the header
 * refuses to compile rather than let it happen.
 */

/* Seek, with an offset wide enough for the whole file. origin is the
   C library's: SEEK_SET, SEEK_CUR or SEEK_END. Nought on success. */
int FIO_i_Seek(FILE* fp, long long offset, int origin);

/* Where in the file we are, or -1. */
long long FIO_i64_Tell(FILE* fp);

/* Whether this program can write to a named file, and making it so it
   can. The second answers whether it managed it.

   Both are about the file's permissions rather than about a handle,
   which is why they take a name: the one caller has just failed to
   open the file and is deciding whether it is worth trying again. */
int FIO_b_Writable(const char* path);
int FIO_b_MakeWritable(const char* path);

/* The mode to open a file that will be read start to end.
   Microsoft's library takes that as a hint in the mode string; it is
   a hint and nothing else, so everywhere else simply reads. */
#ifdef _WIN32
#define FIO_READ_SEQUENTIAL "rbS"
#else
#define FIO_READ_SEQUENTIAL "rb"
#endif
