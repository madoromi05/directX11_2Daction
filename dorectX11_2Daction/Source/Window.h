#pragma once
#include <windows.h>

namespace engin
{
	//定数定義
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define APP_NAME L"土台（ウィンドウ作成）"

	class Window
	{
	public:
		Window() {};
		~Window() {};

		HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
		bool ProcessMessage();
		HWND GetHWND() const { return m_hWnd; }

	private:
		HWND m_hWnd = NULL;
		LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	};
}