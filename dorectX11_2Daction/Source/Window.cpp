#include "Window.h"

namespace engin
{
	//グローバル変数 (Windowクラスのインスタンス参照用)
	Window* g_pWindow = NULL;

	// OSから見たウィンドウプロシージャー
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (g_pWindow) {
			return g_pWindow->MsgProc(hWnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	//ウィンドウ作成
	HRESULT Window::InitWindow(HINSTANCE hInstance,
		INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
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
		wc.lpszClassName = WindowName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassEx(&wc);

		//ウィンドウの作成
		m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
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
	LRESULT Window::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (iMsg)
		{
		case WM_KEYDOWN:
			switch ((char)wParam)
			{
			case VK_ESCAPE://ESCキーで修了
				PostQuitMessage(0);
				break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

	//メッセージループとアプリケーション処理の入り口
	void Window::Update()
	{
		// メッセージループ
		MSG msg = { 0 };
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//アプリケーションの処理はここから
				App();
			}
		}
		//アプリケーションの終了
	}
}