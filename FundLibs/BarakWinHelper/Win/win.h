#pragma once
#include <Windows.h>

class winClass {
public:
	HINSTANCE hInst;
	LPSTR lpCmdLine;
	int nCmdShow;
	LPCWSTR pClassName;
	WNDCLASSEXW wc = { 0 };
	winClass();
	void init(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow, LPCWSTR pClassName, WNDPROC fWndProc);
};

class hWin {
protected:
	HWND hWnd;
	size_t x, y;
	size_t w, h;
public:
	hWin();
	hWin(std::nullptr_t);
	HWND getWin();
	size_t getX();
	size_t getY();
	size_t getW();
	size_t getH();
	void setXY(size_t x, size_t y);
	void setWH(size_t w, size_t h);
};

class hWindow : public hWin{
private:
	LPCWSTR name;
	winClass* wic;
	HDC hdc;
public:
	HDC getHDC();
	LPCWSTR getName();
	void rename(LPCWSTR name);
	void init(winClass* wic, LPCWSTR name, DWORD dwStyle, hWin parent);
	void init(winClass* wic, DWORD dwStyle, hWin parent);
	void resize();
	void show(bool Maximized);
};
/*
case WM_COMMAND:
	if (button.down(lParam))
		TextOut(helpWin.getHDC(), 10, 110, L"ÊÍÎÏÎ×ÊÀ!!!", 11);
	break;
*/
class hButton : public hWin{
public:
	/*
	button.init(L"OK", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, helpWin);
	*/
	void init(LPCWSTR name, DWORD dwStyle, hWin parent);
	bool down(LPARAM lParam);
};

/*
case WM_HSCROLL:
	scroll.update(wParam);
	smb = scroll.getPos();
	break;
*/
class hScrollBar : public hWin{
private:
	size_t pos = 0;
	size_t min, max;
	size_t bigSpeed = 1;
public:
	void setBigSpeed(size_t speed);
	void setPos(size_t pos);
	void setScrollRange(size_t min, size_t max);
	size_t getPos();
	void update(WPARAM wParam);
	/*
	scroll.init(WS_CHILD | WS_VISIBLE | SBS_HORZ, helpWin);
	*/
	void init(DWORD dwStyle, hWin parent);
};