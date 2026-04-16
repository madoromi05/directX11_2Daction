#pragma once
#include <d3d11.h>

namespace engine
{
    // ピクセルシェーダーの生成・管理を担う。
    // Init後にBind()を呼ぶことでパイプラインにセットされる。
    class PixelShader
    {
    public:
        PixelShader();
        ~PixelShader();

        HRESULT Init(ID3D11Device* pDevice, const wchar_t* hlslPath);

        void Bind(ID3D11DeviceContext* pContext);

    private:
        ID3D11PixelShader* m_pPixelShader;
    };

}