#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace engine
{
    // 任意の構造体TをGPUの定数バッファに乗せる汎用テンプレートクラス。
    // Tは16バイトアライメントを満たす必要がある。
    // 使用例:
    //   ConstantBuffer<SimpleShaderConstantBuffer> cb;
    //   cb.Init(pDevice);
    //   cb.Update(pContext, data);
    //   cb.Bind(pContext, 0);
    template<typename T>
    class ConstantBuffer
    {
    public:
        ConstantBuffer() = default;
        ~ConstantBuffer() = default;

        HRESULT Init( ID3D11Device* pDevice )
        {
            // 16バイトアライメントチェック
            static_assert( sizeof( T ) % 16 == 0,
                "ConstantBuffer: sizeof(T) must be a multiple of 16 bytes." );

            D3D11_BUFFER_DESC bd;
            ZeroMemory( &bd, sizeof( bd ) );
            bd.Usage = D3D11_USAGE_DYNAMIC;
            bd.ByteWidth = sizeof( T );
            bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            return pDevice->CreateBuffer( &bd, nullptr, m_pBuffer.GetAddressOf() );
        }

        // データをGPUバッファに転送する。毎フレーム呼ぶ。
        void Update( ID3D11DeviceContext* pContext, const T& data )
        {
            D3D11_MAPPED_SUBRESOURCE mapped;
            if (SUCCEEDED( pContext->Map( m_pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
            {
                memcpy( mapped.pData, &data, sizeof( T ) );
                pContext->Unmap( m_pBuffer.Get(), 0 );
            }
        }

        // VS・PS両方の指定スロットにバインドする。
        // slot : シェーダー側の register(bN) のNに対応する
        void Bind( ID3D11DeviceContext* pContext, UINT slot )
        {
            ID3D11Buffer* buf = m_pBuffer.Get();
            pContext->VSSetConstantBuffers( slot, 1, &buf );
            pContext->PSSetConstantBuffers( slot, 1, &buf );
        }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
    };

}