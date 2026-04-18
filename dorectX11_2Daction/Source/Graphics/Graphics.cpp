#include "Graphics.h"
#include "Debug.h"

namespace engine
{
    Graphics::Graphics()
        : m_mView( XMMatrixIdentity() )
        , m_mProj( XMMatrixIdentity() )
    {
    }

    HRESULT Graphics::InitD3D( HWND hWnd, int width, int height )
    {
        return m_renderer.Init( hWnd, width, height );
    }

    HRESULT Graphics::InitPipeline()
    {
        // 実行時のカレントディレクトリを出力して、パスのズレを確認する
        char currentDir[MAX_PATH];
        GetCurrentDirectoryA( MAX_PATH, currentDir );
        DEBUG_LOG( "Graphics::InitPipeline - カレントディレクトリ: " + std::string( currentDir ) );

        ID3D11Device* pDevice = m_renderer.GetDevice();

        if (FAILED( m_vertexShader.Init( pDevice, L"Source/assets/Hlsl/VertexShader.hlsl" ) ))
        {
            DEBUG_LOG_ERROR( "VertexShader の初期化に失敗しました" );
            return E_FAIL;
        }
        if (FAILED( m_pixelShader.Init( pDevice, L"Source/assets/Hlsl/PixelShader.hlsl" ) ))
        {
            DEBUG_LOG_ERROR( "PixelShader の初期化に失敗しました" );
            return E_FAIL;
        }
        if (FAILED( m_constantBuffer.Init( pDevice ) ))
        {
            DEBUG_LOG_ERROR( "ConstantBuffer の初期化に失敗しました" );
            return E_FAIL;
        }

        return S_OK;
    }

    void Graphics::BeginRender()
    {
        m_renderer.BeginFrame( kClearR, kClearG, kClearB, kClearA );

        // パイプラインへシェーダーをセット
        ID3D11DeviceContext* pContext = m_renderer.GetContext();
        m_vertexShader.Bind( pContext );
        m_pixelShader.Bind( pContext );
    }

    void Graphics::EndRender()
    {
        m_renderer.EndFrame();
    }

    void Graphics::SetCamera( FXMVECTOR eye, FXMVECTOR focus, FXMVECTOR up )
    {
        m_mView = XMMatrixLookAtLH( eye, focus, up );
    }

    void Graphics::SetProjection( float fovY, float aspect, float nearZ, float farZ )
    {
        m_mProj = XMMatrixPerspectiveFovLH( fovY, aspect, nearZ, farZ );
    }

    void Graphics::Draw( Mesh* pMesh, CXMMATRIX world, const XMFLOAT4& color )
    {
        ID3D11DeviceContext* pContext = m_renderer.GetContext();

        // WVP合成してGPUに転送
        SimpleShaderConstantBuffer cb;
        XMMATRIX mWVP = XMMatrixTranspose( world * m_mView * m_mProj );
        XMStoreFloat4x4( &cb.mWVP, mWVP );
        cb.polyColor = color;

        m_constantBuffer.Update( pContext, cb );
        m_constantBuffer.Bind( pContext, 0 );

        pMesh->Draw( pContext );
    }

    ID3D11Device* Graphics::GetDevice() const
    {
        return m_renderer.GetDevice();
    }
}