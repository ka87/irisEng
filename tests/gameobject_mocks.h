#ifndef GAMEOBJECT_MOCKS_H
#define GAMEOBJECT_MOCKS_H

/* Shared between the driver and the mocks of the gameobject.c
   differential test. One arena per side so the two runs allocate at the
   same addresses, and one ordered call log so a call that moved or went
   missing fails the comparison even when no value differs. */

void GMOCK_Reset(void);          /* rewind the allocator and clear the log */
void GMOCK_Log(const char* fmt, ...);
int  GMOCK_LogCount(void);
const char* GMOCK_LogLine(int i);
void GMOCK_SnapshotLog(int side); /* 0 = base, 1 = new */
int  GMOCK_CompareLogs(const char* what);
unsigned long GMOCK_HeapUsed(void);
const unsigned char* GMOCK_HeapBase(void);
void GMOCK_SnapshotHeap(int side);
int  GMOCK_CompareHeaps(const char* what);

#endif
