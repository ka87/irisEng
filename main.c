#include "common.h"
#include "memory.h"
#include "math.h"
#include "time.h"
#include "sound.h"
#include "engine.h"
#include "bigfiles.h"
#include "win/wingdi.h"
#include "world.h"
#include "sideload.h"
#include "datafiles.h"
#include "font.h"
#include "loading.h"
#include "dbg.h"
#include "platform.h"
#include "profiler.h"
#include "gdi.h"


/* The window, the display and the renderer's device.
 *
 * The window's own procedure used to do the second and third of these
 * on WM_CREATE, which meant the startup order was decided by when
 * Windows chose to send a message. The window is SDL's now and there
 * is no such message, so it is said here -- in the order it always
 * happened in, and in the place that already sequences the startup. */
static int MAI_l_b_DisplayOpen(void)
{
    void* window;
    int   width = 0;
    int   height = 0;
    int   windowed = 0;

    PLT_WindowSettings(&width, &height, &windowed);
    /* The settings' windowed value is the engine's flag and not a
       property of the window: it is what the renderer reads when it
       decides whether to take an exclusive display mode. The window
       creation used to set it here itself, and it only ever turns it
       on, which is what the command line does too. */
    if (windowed)
        ENG_WindowedMode = 1;
    if (!PLT_b_Open("IRISEngine", width, height))
        return 0;

    window = PLT_p_NativeWindow();
    MAI_gh_MainWindow = (HWND)window;
    MAI_gst_MainHandles.hwnd = (HWND)window;
    MAI_gst_MainHandles.hwndSetAfterCreateWindow = (HWND)window;

    fbss = GDI_fnpst_CreateDisplayData();
    MAI_gst_MainHandles.displayData = fbss;
    GDI_fnl_InitInterface(fbss);
    fbss->rendererState = fbss->CreateDevice();
    PRO_InitRasters(fbss->rasters, "Display Data");
    GDI_AttachDisplayAndReadaptDisplay(fbss, (HWND)window);
    return 1;
}

/* And what it did on the way out, which was WM_CLOSE and WM_DESTROY. */
static void MAI_l_DisplayClose(void)
{
    if (MAI_gst_MainHandles.displayData)
    {
        GDI_DetachDisplay(MAI_gst_MainHandles.displayData);
        MAI_gst_MainHandles.displayData->DestroyDevice(
            MAI_gst_MainHandles.displayData->rendererState);
        GDI_DestroyDisplayData(MAI_gst_MainHandles.displayData);
        MAI_gst_MainHandles.displayData = NULL;
    }
    PLT_Close();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	CreateMutexA(NULL, TRUE, "IrisEngGame");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		OutputDebugStringA("Another instance of the game is already running.");
	}
	else
	{
		/* First thing into the log, so a run can be told from the one
		   before it and the version is never a question. */
		DBG_LogBanner();

		ENG_InitApplication();

		// datafiles.ini, if there is one, then the command line over
		// the top of it: ENG_ParseCommandLine only ever sets the
		// flags it finds and only writes the big file name when the
		// line actually carries one, so whatever is given there wins
		// and whatever is not falls back to the ini.
		DAT_InitModule();
		if (DAT_b_Binarized())
			LOA_gb_SpeedMode = 1;
		if (DAT_pc_BigFile())
			strcpy(MAI_gst_BIGFileName, DAT_pc_BigFile());

		strcpy(cmdLine, lpCmdLine);
		ENG_ParseCommandLine(cmdLine);
		BIG_Open(MAI_gst_BIGFileName);
		SIDE_InitModule();
		FNT_InitModule();
		ENG_bgeModuleHandle = hInstance;
		if (!MAI_l_b_DisplayOpen())
		{
			DBG_OutputDebugString("the window would not open; stopping\n");
			DBG_LogExit();
			return 0;
		}
		ENG_InitEngine();
		ENG_Loop();
		WOR_Universe_Close();
		// before the engine closes, while the renderer's device is
		// still there to hand the glyph atlases back to
		FNT_CloseModule();
		ENG_CloseEngine();
		MAI_l_DisplayClose();
		BIG_Close();
		SIDE_CloseModule();
		ENG_CloseApplication();

		/* And the last. A log without this line stopped somewhere
		   else. */
		DBG_LogExit();
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
