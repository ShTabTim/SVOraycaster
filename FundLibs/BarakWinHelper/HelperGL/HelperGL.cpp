#include "HelperGL.h"
#include <glad/glad.h>
#include <Windows.h>

void EnableOpenGL(hWindow w, HGLRC* hRC) {
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(w.getHDC(), &pfd);
	SetPixelFormat(w.getHDC(), iFormat, &pfd);
	*hRC = wglCreateContext(w.getHDC());
	wglMakeCurrent(w.getHDC(), *hRC);
}

void DisableOpenGL(hWindow w, HGLRC hRC) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(w.getWin(), w.getHDC());
}