#include "fileio.h"

#ifdef _WIN32

#include <io.h>
#include <sys/stat.h>

int FIO_i_Seek(FILE* fp, long long offset, int origin)
{
    return _fseeki64(fp, (__int64)offset, origin);
}

long long FIO_i64_Tell(FILE* fp)
{
    return (long long)_ftelli64(fp);
}

int FIO_b_Writable(const char* path)
{
    /* 2 is the write-permission question. The constant rather than
       W_OK because this library does not define that name. */
    return _access(path, 2) == 0;
}

int FIO_b_MakeWritable(const char* path)
{
    return _chmod(path, _S_IREAD | _S_IWRITE) == 0;
}

#else /* not Windows */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* A seek that cannot reach past two gigabytes would work until the
   big file grew, which is the worst way to be wrong. */
#if !defined(_LP64) && !defined(__LP64__)
#if !defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS != 64
#error "a 32-bit build needs _FILE_OFFSET_BITS=64 for the big file"
#endif
#endif

int FIO_i_Seek(FILE* fp, long long offset, int origin)
{
    return fseeko(fp, (off_t)offset, origin);
}

long long FIO_i64_Tell(FILE* fp)
{
    return (long long)ftello(fp);
}

int FIO_b_Writable(const char* path)
{
    return access(path, W_OK) == 0;
}

int FIO_b_MakeWritable(const char* path)
{
    /* The owner's own file, readable by anyone: what the Windows call
       does, said the way this platform says it. Said in full rather
       than added to what is there, because the one caller is trying
       to open a file it could not, and reading the mode back to
       modify it is a second way to fail for no gain. */
    return chmod(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == 0;
}

#endif
