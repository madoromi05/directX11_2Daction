#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace engine
{
    // サンプラーの用途を示す。
    // Wrap  : 地形など繰り返しテクスチャに使用する。
    // Clamp : UIなど短時間表示のオブジェクトに使用する。
    enum class SamplerType
    {
        Wrap,
        Clamp,
    };

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