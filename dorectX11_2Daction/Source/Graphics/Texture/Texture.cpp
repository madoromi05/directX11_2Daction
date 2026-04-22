#include "Texture.h"
#include <WICTextureLoader.h>

namespace engine
{
    HRESULT Texture::Init( ID3D11Device* pDevice,
        ID3D11DeviceContext* pContext,
        const wchar_t* filePath )
    {
        // WICTextureLoaderで画像を読み込みSRVを生成する
        HRESULT hr = DirectX::CreateWICTextureFromFile(
            pDevice,
            pContext,
            filePath,
            nullptr,
            m_pSRV.GetAddressOf()
        );
        if (FAILED( hr )) return hr;

        return CreateSamplers( pDevice );
    }

    void Texture::Bind( ID3D11DeviceContext* pContext, UINT slot, SamplerType type )
    {
        // SRVをピクセルシェーダーにバインドする
        ID3D11ShaderResourceView* pSRV = m_pSRV.Get();
        pContext->PSSetShaderResources( slot, 1, &pSRV );

        // 用途に応じたサンプラーをバインドする
        ID3D11SamplerState* pSampler = ( type == SamplerType::Wrap )
            ? m_pSamplerWrap.Get()
            : m_pSamplerClamp.Get();
        pContext->PSSetSamplers( slot, 1, &pSampler );
    }

    HRESULT Texture::CreateSamplers( ID3D11Device* pDevice )
    {
        D3D11_SAMPLER_DESC sd;
        ZeroMemory( &sd, sizeof( sd ) );
        sd.Filter = D3D11_FILTER_ANISOTROPIC;
        sd.MaxAnisotropy = kMaxAnisotropy;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sd.MinLOD = 0.0f;
        sd.MaxLOD = D3D11_FLOAT32_MAX;

        // Wrapサンプラー：地形など繰り返しテクスチャ用
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        HRESULT hr = pDevice->CreateSamplerState( &sd, m_pSamplerWrap.GetAddressOf() );
        if (FAILED( hr )) return hr;

        // Clampサンプラー：UIなど短時間表示オブジェクト用
        sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        return pDevice->CreateSamplerState( &sd, m_pSamplerClamp.GetAddressOf() );
    }

}