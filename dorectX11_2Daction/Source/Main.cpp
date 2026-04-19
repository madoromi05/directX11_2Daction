#include <windows.h>
#include "Game/Game.h"
#include "Window.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    engine::Window window;
    if (FAILED(window.InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
    {
        MessageBox(NULL, L"ウィンドウの初期化に失敗しました", L"エラー", MB_OK);
        return 0;
    }
    
    auto pGame = std::make_unique<game::Game>();
    if (FAILED(pGame->Init(window.GetHWND(), WINDOW_WIDTH, WINDOW_HEIGHT)))
    {
        MessageBox(NULL, L"ゲームの初期化に失敗しました", L"エラー", MB_OK);
        return 0;
    }

    while (window.ProcessMessage())
    {
        pGame->Update();
        pGame->Render();
    }

    return 0;
}