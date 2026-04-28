#pragma once
#ifdef _DEBUG

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "FontAtlas.h"

namespace engine
{
    class TextRenderer
    {
    public:
        static constexpr int kMaxChars = 512;

        HRESULT Init(ID3D11Device* pDevice, int screenW, int screenH);

        void DrawString(ID3D11DeviceContext* pContext,
                        FontAtlas&           atlas,
                        const char*          text,
                        float x, float y,
                        DirectX::XMFLOAT4    color);
    private:
        struct Vertex { float x, y, u, v; };

        struct alignas(16) TextCB
        {
            DirectX::XMFLOAT4X4 ortho;  // 64 bytes
            DirectX::XMFLOAT4   color;  // 16 bytes
        };

        Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_pVS;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pPS;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pVB;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pCB;
        Microsoft::WRL::ComPtr<ID3D11BlendState>        m_pBlend;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDS;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>      m_pSampler;
        Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_pRaster;

        DirectX::XMFLOAT4X4 m_ortho{};
    };
}

#endif // _DEBUG
