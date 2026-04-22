#pragma once
#include "SamplerType.h"
#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace engine
{
    class Texture
    {
    public:
        Texture() = default;
        ~Texture() = default;

        HRESULT Init( ID3D11Device* pDevice,
            ID3D11DeviceContext* pContext,
            const wchar_t* filePath );

        // slot     : シェーダー側の register(tN) のNに対応する
        // type     : Wrap or Clamp
        void Bind( ID3D11DeviceContext* pContext, UINT slot, SamplerType type );

    private:
        HRESULT CreateSamplers( ID3D11Device* pDevice );

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_pSamplerWrap;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_pSamplerClamp;

        // 異方性フィルタリングの最大レベル
        static constexpr UINT kMaxAnisotropy = 4;
    };

}