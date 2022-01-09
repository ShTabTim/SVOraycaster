#include "Barak.h"
#include "HelperGL/HelperGL.h"

winClass wc;
hWindow gWin;
bool isLive = true;
hWindow getWind() { return gWin; }
HGLRC hRC;
bool quit = false;

bool threadIsLive() { return isLive; }
void SwapBuffers() { SwapBuffers(gWin.getHDC()); }
void mainThread() {
	EnableOpenGL(gWin, &hRC);
	gladLoadGL();
	quit = main();
	DisableOpenGL(gWin, hRC);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		isLive = false;
		break;
	case WM_SIZE:
		gWin.resize();
		break;
		//case WM_KEYDOWN:
		//	switch (wParam) {
		//	case VK_ESCAPE:
		//		PostQuitMessage(0);
		//		isLive = false;
		//		break;
		//	}
		//	break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow) {
	wc.init(hInst, pCmdLine, nCmdShow, L"Baramkaff", WndProc);
	prepr(&gWin);
	if (!(gWin.getX()))
		gWin.setXY(CW_USEDEFAULT, CW_USEDEFAULT);
	if (gWin.getW() == 0)
		gWin.setWH(256, 256);
	DWORD style = WS_POPUP;
	if (gWin.getName())
		gWin.init(&wc, style, nullptr);
	else
		gWin.init(&wc, L"GraphPan", style, nullptr);
	gWin.show(true);

	std::thread rTh(mainThread);

	MSG msg;
	do if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} while (msg.message != WM_QUIT && !quit);
	rTh.join();
	return msg.wParam;
}
