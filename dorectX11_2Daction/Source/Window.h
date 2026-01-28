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
		virtual ~Window() {}

		HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
		LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

		void Update();
		virtual void App() = 0;
		virtual void Render() {};

	protected:
		HWND m_hWnd = NULL;
	};
}