#include "VertexShader.h"
#include "SimpleVertex.h"
#include "Debug.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace engine
{
    HRESULT VertexShader::Init(ID3D11Device* pDevice, const wchar_t* hlslPath)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> pErrBlob;

        HRESULT hr = D3DCompileFromFile(
            hlslPath,
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "VS", "vs_5_0",
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

        hr = pDevice->CreateVertexShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &m_pVertexShader
        );
        
        if (FAILED( hr )) return hr;

        // SimpleVertexの構成に合わせた入力レイアウト定義
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SimpleVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SimpleVertex, normal),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(SimpleVertex, texCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        hr = pDevice->CreateInputLayout(
            layout,
            ARRAYSIZE(layout),
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            &m_pInputLayout
        );
        
        if (FAILED( hr ))
        {
            DEBUG_LOG_ERROR( "InputLayout の生成に失敗しました" );
            return hr;
        }
        return hr;
    }

    void VertexShader::Bind(ID3D11DeviceContext* pContext)
    {
        pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
        pContext->IASetInputLayout(m_pInputLayout.Get() );
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}