#pragma once
#include <windows.h>

//定数定義
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define APP_NAME L"土台（ウィンドウ作成）"

class Window
{
public:
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	void Run();

	HWND m_hWnd;
};