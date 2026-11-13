#pragma once
/* What the two bigfiles.c copies need from the target toolchain that
   this host does not have.  Included ahead of the file rather than
   written into it: nothing here changes the source under test.
 *
 *  - fpos_t.  On the target it is a 64-bit integer and BIG_fseek does
 *    arithmetic on one; under glibc it is an opaque struct and the
 *    file does not compile.  The real <stdio.h> is pulled in first so
 *    that its own typedef is already made, and the name is then
 *    redefined for the uses that follow.  fgetpos and fsetpos are
 *    redirected with it, because glibc's take the struct.
 *  - fread, fseek, fopen and fclose go to the harness's in-memory
 *    file, so that both copies read the same bytes and every call is
 *    logged.
 *  - _access, _chmod and _S_IWRITE are MSVC CRT names. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _access(const char* path, int mode);
int _chmod(const char* path, int pmode);
#ifndef _S_IWRITE
#define _S_IWRITE 0200
#endif

typedef long long BIGM_fpos_t;
#define fpos_t BIGM_fpos_t

int BIGM_fgetpos(FILE* fp, BIGM_fpos_t* pos);
int BIGM_fsetpos(FILE* fp, const BIGM_fpos_t* pos);
size_t BIGM_fread(void* buf, size_t size, size_t count, FILE* fp);
int BIGM_fseek(FILE* fp, long offset, int origin);
FILE* BIGM_fopen(const char* path, const char* mode);
int BIGM_fclose(FILE* fp);

#define fgetpos BIGM_fgetpos
#define fsetpos BIGM_fsetpos
#define fread BIGM_fread
#define fseek BIGM_fseek
#define fopen BIGM_fopen
#define fclose BIGM_fclose
