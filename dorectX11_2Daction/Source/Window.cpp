#include "Window.h"

namespace engine
{
	//グローバル変数 (Windowクラスのインスタンス参照用)
	Window* g_pWindow = NULL;

	// OSから見たウィンドウプロシージャー
	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (g_pWindow) return g_pWindow->MsgProc(hWnd, uMsg, wParam, lParam);
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	//ウィンドウ作成
	HRESULT Window::InitWindow(HINSTANCE hInstance,
		INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR windowName)
	{
		g_pWindow = this;
		// ウィンドウの定義
		WNDCLASSEX  wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		wc.lpszClassName = windowName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassEx(&wc);

		//ウィンドウの作成
		m_hWnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW,
			0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
		if (!m_hWnd)
		{
			return E_FAIL;
		}
		//ウインドウの表示
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

		return S_OK;
	}

	//ウィンドウプロシージャー
	LRESULT Window::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_DESTROY || (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE))
		{
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	//メッセージループの入り口
	bool Window::ProcessMessage()
	{
		MSG msg = { 0 };
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return false; // 終了
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true; // 継続
	}
}