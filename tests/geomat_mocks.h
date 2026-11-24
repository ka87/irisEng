#pragma once

#define GMMOCK_MAXREADS 256
#define GMMOCK_KNOWN    8       /* texture keys 0..7 are already listed */

extern int GMMOCK_liveBlocks;

extern int GMMOCK_reads[GMMOCK_MAXREADS];
extern int GMMOCK_readNum;

extern int GMMOCK_asked[GMMOCK_MAXREADS];
extern int GMMOCK_askedNum;
extern int GMMOCK_addedNum;

extern int GMMOCK_fileRefKeys[GMMOCK_MAXREADS];
extern int GMMOCK_fileRefNum;

void GMMOCK_Reset(void);
