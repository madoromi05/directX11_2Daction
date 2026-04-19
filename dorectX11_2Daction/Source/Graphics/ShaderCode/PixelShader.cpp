#include "PixelShader.h"
#include "Debug.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace engine
{
    HRESULT PixelShader::Init(ID3D11Device* pDevice, const wchar_t* hlslPath)
    {
        ID3DBlob* pBlob = nullptr;
        ID3DBlob* pErrBlob = nullptr;

        HRESULT hr = D3DCompileFromFile(
            hlslPath,
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "PS", "ps_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            &pBlob,
            &pErrBlob
        );

        if (FAILED( hr ))
        {
            if (pErrBlob)
            {
                DEBUG_LOG_ERROR( "HLSLコンパイル失敗: "
                    + std::string( ( char* ) pErrBlob->GetBufferPointer() ) );
                pErrBlob->Release();
            }
            else
            {
                DEBUG_LOG_ERROR( "HLSLコンパイル失敗 (詳細なし)" );
            }
            return hr;
        }

        hr = pDevice->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &m_pPixelShader
        );
        if (FAILED( hr ))
        {
            DEBUG_LOG_ERROR( "Pixelhader の生成に失敗しました" );
            pBlob->Release();
            return hr;
        }

        return hr;
    }

    void PixelShader::Bind(ID3D11DeviceContext* pContext)
    {
        pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    }
}