#pragma once

#define AIMATMOCK_STACK 64

extern int AIMATMOCK_stack[AIMATMOCK_STACK];
extern int AIMATMOCK_stackNum;
extern int AIMATMOCK_underflows;

extern int AIMATMOCK_setMapGaoNum;
extern void* AIMATMOCK_setMapGao;
extern int AIMATMOCK_updateWithNum;
extern void* AIMATMOCK_updateWith;

extern struct sTexProceduralStruct* AIMATMOCK_procedural;
extern struct sTexAnimatedElem* AIMATMOCK_anim;

void AIMATMOCK_Push(int v);
void AIMATMOCK_PushFloat(float f);
void AIMATMOCK_Reset(void);
