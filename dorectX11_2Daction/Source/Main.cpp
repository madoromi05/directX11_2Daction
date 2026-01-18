#include <windows.h>
#include "Graphics.h" 

Graphics* g_pApp = NULL;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    // Windowクラスではなく、Graphicsクラスの実体を作る
    // これにより、Windowの機能とGraphicsの機能の両方が使えるようになります
    g_pApp = new Graphics();

    if (g_pApp != NULL)
    {
        // ウィンドウ作成
        if (FAILED(g_pApp->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
        {
            MessageBox(NULL, L"ウィンドウ作成に失敗しました", L"エラー", MB_OK);
            delete g_pApp;
            return 0;
        }

        // DirectX初期化
        if (FAILED(g_pApp->InitD3D()))
        {
            MessageBox(NULL, L"DirectXの初期化(InitD3D)に失敗しました", L"エラー", MB_OK);
            delete g_pApp;
            return 0;
        }

        // パイプライン（シェーダー）初期化
        if (FAILED(g_pApp->InitPipeline()))
        {
            MessageBox(NULL, L"シェーダー作成(InitPipeline)に失敗しました。\nhlslファイルが見つからない可能性があります。", L"エラー", MB_OK);
            delete g_pApp;
            return 0;
        }

        // ポリゴン初期化
        if (FAILED(g_pApp->InitPolygon()))
        {
            MessageBox(NULL, L"ポリゴン作成(InitPolygon)に失敗しました", L"エラー", MB_OK);
            delete g_pApp;
            return 0;
        }

        // ゲームループ開始 (Window::Update)
        g_pApp->Update();


        g_pApp->DestroyD3D();
        delete g_pApp;
    }

    return 0;
}