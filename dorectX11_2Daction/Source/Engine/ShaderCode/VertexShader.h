#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace engine
{
    // 頂点シェーダーとInputLayoutの生成・管理を担う。
    // Init後にBind()を呼ぶことでパイプラインにセットされる。
    class VertexShader
    {
    public:
        VertexShader() = default;
        ~VertexShader() = default;

        HRESULT Init( ID3D11Device* pDevice, const wchar_t* hlslPath );
        void Bind( ID3D11DeviceContext* pContext );

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_pInputLayout;
    };
}