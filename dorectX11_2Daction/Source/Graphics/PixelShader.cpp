#include "PixelShader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace engine
{
    PixelShader::PixelShader()
        : m_pPixelShader(nullptr)
    {}

    PixelShader::~PixelShader()
    {
        if (m_pPixelShader) m_pPixelShader->Release();
    }

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

        if (FAILED(hr))
        {
            if (pErrBlob)
            {
                OutputDebugStringA((char*)pErrBlob->GetBufferPointer());
                pErrBlob->Release();
            }
            return hr;
        }

        hr = pDevice->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &m_pPixelShader
        );
        pBlob->Release();

        return hr;
    }

    void PixelShader::Bind(ID3D11DeviceContext* pContext)
    {
        pContext->PSSetShader(m_pPixelShader, nullptr, 0);
    }
}