#include "common.h"
#include "memory.h"
#include "math.h"
#include "time.h"
#include "sound.h"
#include "engine.h"
#include "bigfiles.h"
#include "win/wingdi.h"
#include "world.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	CreateMutexA(NULL, TRUE, "BeyondGoodAndEvilGame");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		OutputDebugStringA("Another instance of the game is already running");
	}
	else
	{
		ENG_InitApplication();
		strcpy(cmdLine, lpCmdLine);
		strcat(cmdLine, " /B sally_clean.bf");
		//strcat(cmdLine, " /SINGLESTREAM /NOSOUND /B sally_clean.bf");
		ENG_ParseCommandLine(cmdLine);
		BIG_Open(MAI_gst_BIGFileName);
		ENG_bgeModuleHandle = hInstance;
		ENG_WinGameWindowInit(hInstance);
		ENG_InitEngine();
		ENG_Loop();
		WOR_Universe_Close();
		ENG_CloseEngine();
		DestroyWindow(MAI_gh_MainWindow);
		BIG_Close();
		ENG_CloseApplication();
	}
	return 0;
};

/*
int main()
{
	int holesNum;
	int sizeOfHoles;

	int i;

	MEM_InitModule();
	MATH_InitModule();
	TIM_InitModule();
	TIM_MeasureTicksPerSecond();

	printf("elapsedtimeTR=%f\n", TIM_f_Clock_TrueRead());
	printf("elapsedtime=%f\n", TIM_f_Clock_ElapsedTime());

	void* v = MEM_p_Alloc(100);
	void* v2 = MEM_p_Alloc(4096);
	void* v4 = MEM_p_Alloc(100);
	printf("v=%p\n", v);
	printf("v2=%p\n", v2);
	printf("v4=%p\n", v4);
	MEM_Free(v);
	MEM_Free(v2);
	MEM_GetHolesStat(&holesNum, &sizeOfHoles);
	printf("hn: %u, soh: %u\n", holesNum, sizeOfHoles);
	void* v3 = MEM_p_Alloc(100);
	printf("v3=%p\n", v3);
	MEM_GetHolesStat(&holesNum, &sizeOfHoles);
	printf("hn: %u, soh: %u\n", holesNum, sizeOfHoles);
	void* v5 = MEM_p_Alloc(100);
	void* v5a = MEM_p_Alloc(100);
	void* v6 = MEM_p_Alloc(4096);
	void* v7 = MEM_p_Alloc(4096);
	void* v8 = MEM_p_Alloc(100);
	printf("v5=%p\n", v5);
	printf("v5a=%p\n", v5a);
	printf("v6=%p\n", v6);
	printf("v7=%p\n", v7);
	printf("v8=%p\n", v8);
	MEM_GetHolesStat(&holesNum, &sizeOfHoles);
	printf("hn: %u, soh: %u\n", holesNum, sizeOfHoles);
	MEM_Free(v6);
	MEM_Free(v5);
	MEM_Free(v5a);
	MEM_Free(v7);
	MEM_Free(v8);
	MEM_GetHolesStat(&holesNum, &sizeOfHoles);
	printf("before defrag hn: %u, soh: %u\n", holesNum, sizeOfHoles);
	MEM_Defrag(0);
	MEM_GetHolesStat(&holesNum, &sizeOfHoles);
	printf("after defrag hn: %u, soh: %u\n", holesNum, sizeOfHoles);
	void* vend = MEM_p_AllocFromEnd(100);
	void* vend2 = MEM_p_AllocFromEnd(100);
	printf("vend=%p\n", vend);
	printf("vend2=%p\n", vend2);
	MEM_FreeFromEnd(vend2);
	void* vend3 = MEM_p_AllocFromEnd(100);
	printf("vend3=%p\n", vend3);
	void* valigned1 = MEM_p_AllocAligned(100, 16);
	MEM_FreeAlign(valigned1);
	void* valigned2 = MEM_p_AllocAligned(100, 16);
	//MEM_FreeAlign(valigned2);
	void* valigned3 = MEM_p_AllocAligned(100, 16);
	//MEM_FreeAlign(valigned3);
	printf("valigned1=%p\n", valigned1);
	printf("valigned2=%p\n", valigned2);
	printf("valigned3=%p\n", valigned3);

	printf("elapsedtimeTR=%f\n", TIM_f_Clock_TrueRead());
	Sleep(200);
	printf("elapsedtimeTR+200us=%f\n", TIM_f_Clock_TrueRead());

	printf("elapsedtime=%f\n", TIM_f_Clock_ElapsedTime());
	printf("JadeRand %d\n", JADErand());
	printf("JadeRand %d\n", JADErand());
	Sleep(200);
	printf("JadeRand %d\n", JADErand());

	char a[4] = { 0x11,0x22,0x33,0x44 };
	int b = *((int*)a);
	printf("b=%x\n", b);

	SND_l_InitVolumeTable();
	for (i = 0; i < 256; i++) {
		printf("%0.2f ", SND_af_VolTable[i]);
		if (i % 16 == 0 && i != 0) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("%d ", SND_al_AttTable[i]);
		if (i % 16 == 0 && i != 0) printf("\n");
	}
	printf("D3DOK_NOAUTOGEN 0x%x\n", D3DOK_NOAUTOGEN);
	printf("D3DERR_DEVICEREMOVED 0x%x\n", D3DERR_DEVICEREMOVED);
	printf("D3DERR_DEVICELOST 0x%x\n", D3DERR_DEVICELOST);
	printf("D3DERR_DEVICENOTRESET 0x%x\n", D3DERR_DEVICENOTRESET);	
	printf("stride 0x204: %d\n", D3D_CalculateStride(0x204));
	printf("QS %x\n", MY_QS_ALLINPUT_COMPATIBLE);

	uint32_t test = 5;
	uint8_t test2 = *(uint8_t *)&test;
	printf("test2 %d\n", test2);
	test = 0xaabbccdd;
	test2 = *(uint8_t*)&test;
	printf("test2 %x\n", test2);
	printf("log(2): %f\n", log(2));
	printf("log2(2): %f\n", log2(2));
	printf("log10(2): %f\n", log10(2));
	printf("\n");

}
*/
