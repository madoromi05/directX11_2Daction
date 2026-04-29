#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace engine
{
    // ピクセルシェーダーの生成・管理を担う。
    // Init後にBind()を呼ぶことでパイプラインにセットされる。
    class PixelShader
    {
    public:
        PixelShader() = default;
        ~PixelShader() = default;

        HRESULT Init(ID3D11Device* pDevice, const wchar_t* hlslPath);

        void Bind(ID3D11DeviceContext* pContext);

    private:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    };
}