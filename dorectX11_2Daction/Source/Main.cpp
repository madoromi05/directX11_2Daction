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
        if (SUCCEEDED(g_pApp->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
        {
            // DirectXを初期化する
            if (SUCCEEDED(g_pApp->InitD3D()))
            {
                g_pApp->Update();
            }
        }

        g_pApp->DestroyD3D();
        delete g_pApp;
    }

    return 0;
}