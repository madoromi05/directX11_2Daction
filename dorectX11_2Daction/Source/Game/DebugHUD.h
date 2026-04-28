#pragma once
#ifdef _DEBUG

#include <d3d11.h>
#include <windows.h>
#include <memory>
#include "Graphics/Text/FontAtlas.h"
#include "Graphics/Text/TextRenderer.h"

namespace engine { class Camera; }

namespace game
{
    class DebugHUD
    {
    public:
        HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                     HWND hWnd, int screenW, int screenH);
        void Render(ID3D11DeviceContext* pContext, const engine::Camera& camera, float deltaTime);
    private:
        engine::FontAtlas    m_atlas;
        engine::TextRenderer m_textRenderer;
        bool m_initialized = false;
    };
}

#endif // _DEBUG
