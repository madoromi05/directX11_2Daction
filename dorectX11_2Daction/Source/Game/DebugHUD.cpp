#ifdef _DEBUG
#include "DebugHUD.h"
#include "Game/Player/Camera.h"
#include "Debug.h"
#include <stdio.h>

namespace game
{
    HRESULT DebugHUD::Init(ID3D11Device* pDevice, ID3D11DeviceContext* /*pContext*/,
                           HWND /*hWnd*/, int screenW, int screenH)
    {
        HRESULT hr = m_atlas.Init(pDevice, "C:/Windows/Fonts/cour.ttf", 18);
        if (FAILED(hr))
        {
            DEBUG_LOG_ERROR("DebugHUD: FontAtlas init failed");
            return hr;
        }

        hr = m_textRenderer.Init(pDevice, screenW, screenH);
        if (FAILED(hr))
        {
            DEBUG_LOG_ERROR("DebugHUD: TextRenderer init failed");
            return hr;
        }

        m_initialized = true;
        return S_OK;
    }

    void DebugHUD::Render(ID3D11DeviceContext* pContext,
                          const engine::Camera& camera, float deltaTime)
    {
        if (!m_initialized) return;

        const auto pos = camera.GetPosition();
        const float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;

        char buf[256];
        sprintf_s(buf, 256,
            "Pos  X: %+8.3f  Y: %+8.3f  Z: %+8.3f\n"
            "Yaw: %+8.3f rad  Pitch: %+8.3f rad\n"
            "FPS: %.1f",
            pos.x, pos.y, pos.z,
            camera.GetYaw(), camera.GetPitch(), fps);

        // Shadow (+1,+1 offset for readability)
        m_textRenderer.DrawString(pContext, m_atlas, buf,
            11.0f, 11.0f, { 0.0f, 0.0f, 0.0f, 1.0f });

        // Main text (white)
        m_textRenderer.DrawString(pContext, m_atlas, buf,
            10.0f, 10.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
    }
}
#endif // _DEBUG
