#include <windows.h>
#include "Game/Game.h"
#include "Window.h"

game::Game* g_pGame = NULL;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    engine::Window window;
    if (FAILED(window.InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
    {
        MessageBox(NULL, L"ウィンドウの初期化に失敗しました", L"エラー", MB_OK);
        return 0;
    }
    
    g_pGame = new game::Game();

    if (g_pGame != NULL)
    {
        if (FAILED(g_pGame->Init(window.GetHWND(), WINDOW_WIDTH, WINDOW_HEIGHT)))
        {
            MessageBox(NULL, L"ゲームの初期化に失敗しました", L"エラー", MB_OK);
            delete g_pGame;
            return 0;
        }

        // ゲームループ開始
        while (window.ProcessMessage())
        {
            g_pGame->Update();
            g_pGame->Render();
        }

        // 終了処理
        delete g_pGame;
    }

    return 0;
}