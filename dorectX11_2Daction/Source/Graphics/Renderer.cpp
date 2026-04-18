#include "Renderer.h"
#pragma comment(lib, "d3d11.lib")

namespace engine {
	Renderer::Renderer()
        : m_pDevice( nullptr )
        , m_pSwapChain( nullptr )
        , m_pContext( nullptr )
        , m_pRenderTargetView( nullptr )
        , m_pDepthStencilView( nullptr )
        , m_pDepthStencilBuffer( nullptr )
	{}

    Renderer::~Renderer()
    {
        if (m_pDepthStencilView)   m_pDepthStencilView->Release();
        if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
        if (m_pRenderTargetView)   m_pRenderTargetView->Release();
        if (m_pSwapChain)          m_pSwapChain->Release();
        if (m_pContext)            m_pContext->Release();
        if (m_pDevice)             m_pDevice->Release();
    }

    HRESULT Renderer::Init( HWND hwnd, int width, int height ) {
        // スワップチェーンの設定
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory( &sd, sizeof( sd ) );
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        // デバイスとスワップチェーンを同時に作成する
        D3D_FEATURE_LEVEL featureLevel;
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &m_pSwapChain,
            &m_pDevice,
            &featureLevel,
            &m_pContext
        );
        if (FAILED( hr )) return hr;

        // バックバッファからレンダーターゲットビューを作成する
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( void** ) &pBackBuffer );
        if (FAILED( hr )) return hr;

        hr = m_pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &m_pRenderTargetView );
        pBackBuffer->Release();
        if (FAILED( hr )) return hr;

        // 深度ステンシルバッファの設定
        D3D11_TEXTURE2D_DESC depthDesc;
        ZeroMemory( &depthDesc, sizeof( depthDesc ) );
        depthDesc.Width = width;
        depthDesc.Height = height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.SampleDesc.Quality = 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = m_pDevice->CreateTexture2D( &depthDesc, nullptr, &m_pDepthStencilBuffer );
        if (FAILED( hr )) return hr;

        hr = m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView );
        if (FAILED( hr )) return hr;

        // レンダーターゲットと深度ステンシルをパイプラインにセットする
        m_pContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

        // ビューポートの設定
        D3D11_VIEWPORT vp;
        vp.Width = static_cast< float >( width );
        vp.Height = static_cast< float >( height );
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        m_pContext->RSSetViewports( 1, &vp );

        return S_OK;
    }
    void Renderer::BeginFrame( float r, float g, float b, float a )
    {
        float clearColor[4] = {r, g, b, a};
        m_pContext->ClearRenderTargetView( m_pRenderTargetView, clearColor );
        m_pContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    }

    void Renderer::EndFrame()
    {
        m_pSwapChain->Present( 1, 0 );
    }

    ID3D11Device* Renderer::GetDevice() const
    {
        return m_pDevice;
    }

    ID3D11DeviceContext* Renderer::GetContext() const
    {
        return m_pContext;
    }
}