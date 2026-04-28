#ifdef _DEBUG
#include "TextRenderer.h"
#include "Debug.h"
#include <d3dcompiler.h>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

namespace engine
{
    HRESULT TextRenderer::Init(ID3D11Device* pDevice, int screenW, int screenH)
    {
        HRESULT hr;

        auto compile = [](const wchar_t* path, const char* entry, const char* target,
                          ID3DBlob** ppBlob) -> HRESULT
        {
            Microsoft::WRL::ComPtr<ID3DBlob> errBlob;
            HRESULT hr = D3DCompileFromFile(path, nullptr,
                D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, target,
                D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, ppBlob, &errBlob);
            if (FAILED(hr) && errBlob)
                OutputDebugStringA(static_cast<char*>(errBlob->GetBufferPointer()));
            return hr;
        };

        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob;
        hr = compile(L"Source/assets/Hlsl/TextVertexShader.hlsl", "main", "vs_5_0", &vsBlob);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("TextRenderer: VS compile failed"); return hr; }

        hr = compile(L"Source/assets/Hlsl/TextPixelShader.hlsl", "main", "ps_5_0", &psBlob);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("TextRenderer: PS compile failed"); return hr; }

        hr = pDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_pVS);
        if (FAILED(hr)) return hr;
        hr = pDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pPS);
        if (FAILED(hr)) return hr;

        D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        hr = pDevice->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc),
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_pLayout);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("TextRenderer: InputLayout failed"); return hr; }

        D3D11_BUFFER_DESC vbDesc{};
        vbDesc.ByteWidth      = sizeof(Vertex) * kMaxChars * 6;
        vbDesc.Usage          = D3D11_USAGE_DYNAMIC;
        vbDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        hr = pDevice->CreateBuffer(&vbDesc, nullptr, &m_pVB);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("TextRenderer: VB failed"); return hr; }

        D3D11_BUFFER_DESC cbDesc{};
        cbDesc.ByteWidth      = sizeof(TextCB);
        cbDesc.Usage          = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        hr = pDevice->CreateBuffer(&cbDesc, nullptr, &m_pCB);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("TextRenderer: CB failed"); return hr; }

        D3D11_BLEND_DESC blendDesc{};
        blendDesc.RenderTarget[0].BlendEnable           = TRUE;
        blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = pDevice->CreateBlendState(&blendDesc, &m_pBlend);
        if (FAILED(hr)) return hr;

        D3D11_DEPTH_STENCIL_DESC dsDesc{};
        dsDesc.DepthEnable    = FALSE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = pDevice->CreateDepthStencilState(&dsDesc, &m_pDS);
        if (FAILED(hr)) return hr;

        D3D11_SAMPLER_DESC sampDesc{};
        sampDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        hr = pDevice->CreateSamplerState(&sampDesc, &m_pSampler);
        if (FAILED(hr)) return hr;

        D3D11_RASTERIZER_DESC rastDesc{};
        rastDesc.FillMode = D3D11_FILL_SOLID;
        rastDesc.CullMode = D3D11_CULL_NONE;
        hr = pDevice->CreateRasterizerState(&rastDesc, &m_pRaster);
        if (FAILED(hr)) return hr;

        XMMATRIX ortho = XMMatrixOrthographicOffCenterLH(
            0.0f, static_cast<float>(screenW),
            static_cast<float>(screenH), 0.0f,
            0.0f, 1.0f);
        XMStoreFloat4x4(&m_ortho, XMMatrixTranspose(ortho));

        return S_OK;
    }

    void TextRenderer::DrawString(ID3D11DeviceContext* pContext,
                                  FontAtlas&           atlas,
                                  const char*          text,
                                  float x, float y,
                                  XMFLOAT4             color)
    {
        std::vector<Vertex> verts;
        verts.reserve(128 * 6);

        const int lineH = atlas.GetLineHeight();
        float cx = x;
        float cy = y + lineH;  // cy = baseline (Y increases downward)

        for (const char* p = text; *p; ++p)
        {
            if (*p == '\n')
            {
                cx  = x;
                cy += lineH * 1.2f;
                continue;
            }

            if (static_cast<unsigned char>(*p) < 32 ||
                static_cast<unsigned char>(*p) > 126) continue;

            const GlyphInfo& g = atlas.GetGlyph(*p);

            if (g.bmpW > 0 && g.bmpH > 0)
            {
                float gx = cx + g.bearingX;
                float gy = cy - g.bearingY;
                float x1 = gx + g.bmpW;
                float y1 = gy + g.bmpH;

                verts.push_back({ gx, gy, g.u0, g.v0 });
                verts.push_back({ x1, gy, g.u1, g.v0 });
                verts.push_back({ gx, y1, g.u0, g.v1 });
                verts.push_back({ x1, gy, g.u1, g.v0 });
                verts.push_back({ x1, y1, g.u1, g.v1 });
                verts.push_back({ gx, y1, g.u0, g.v1 });
            }

            cx += g.advance;
            if (static_cast<int>(verts.size()) / 6 >= kMaxChars) break;
        }

        if (verts.empty()) return;

        D3D11_MAPPED_SUBRESOURCE mapped{};
        if (FAILED(pContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) return;
        memcpy(mapped.pData, verts.data(), verts.size() * sizeof(Vertex));
        pContext->Unmap(m_pVB.Get(), 0);

        D3D11_MAPPED_SUBRESOURCE cbMapped{};
        if (FAILED(pContext->Map(m_pCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &cbMapped))) return;
        TextCB* pCB   = static_cast<TextCB*>(cbMapped.pData);
        pCB->ortho    = m_ortho;
        pCB->color    = color;
        pContext->Unmap(m_pCB.Get(), 0);

        pContext->VSSetShader(m_pVS.Get(), nullptr, 0);
        pContext->PSSetShader(m_pPS.Get(), nullptr, 0);
        pContext->IASetInputLayout(m_pLayout.Get());
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        UINT stride = sizeof(Vertex), offset = 0;
        pContext->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &stride, &offset);
        pContext->VSSetConstantBuffers(0, 1, m_pCB.GetAddressOf());
        pContext->PSSetConstantBuffers(0, 1, m_pCB.GetAddressOf());

        ID3D11ShaderResourceView* srv = atlas.GetSRV();
        pContext->PSSetShaderResources(0, 1, &srv);
        pContext->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());

        float blendFactor[4] = {};
        pContext->OMSetBlendState(m_pBlend.Get(), blendFactor, 0xFFFFFFFF);
        pContext->OMSetDepthStencilState(m_pDS.Get(), 0);
        pContext->RSSetState(m_pRaster.Get());

        pContext->Draw(static_cast<UINT>(verts.size()), 0);

        // フォントアトラスをtextだけに制限するためのコード
        ID3D11ShaderResourceView* nullSRV = nullptr;
        pContext->PSSetShaderResources(0, 1, &nullSRV);
        pContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        pContext->OMSetDepthStencilState(nullptr, 0);
    }
}
#endif // _DEBUG
