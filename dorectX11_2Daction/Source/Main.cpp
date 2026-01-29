#include <windows.h>
#include "Game/Game.h"

game::Game* g_pGame = NULL;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    // Windowクラスではなく、Graphicsクラスの実体を作る
    // これにより、Windowの機能とGraphicsの機能の両方が使えるようになります
    g_pGame = new game::Game();

    if (g_pGame != NULL)
    {
        // Game::Init 内で Window作成、DirectX初期化、オブジェクト生成を一括で行う
        // ※Game.h の Init 定義に合わせて引数を渡す
        if (FAILED(g_pGame->Init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
        {
            MessageBox(NULL, L"ゲームの初期化に失敗しました", L"エラー", MB_OK);
            delete g_pGame;
            return 0;
        }

        // ゲームループ開始
        g_pGame-> Run();

        // 終了処理
        delete g_pGame;
    }

    return 0;
}