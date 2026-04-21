#include "Game.h"
#include "Debug.h"
#include <random>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace game
{
    Game::Game() {};

    Game::~Game() = default;

	HRESULT Game::Init(HWND hWnd, int width, int height)
	{
		m_screenWidth = width;
		m_screenHeight = height;

        m_pGraphics = std::make_unique<engine::Graphics>();
        if (FAILED( m_pGraphics->InitD3D( hWnd, width, height ) ))
        {
            DEBUG_LOG_ERROR( "D3D の初期化に失敗しました" );
            return E_FAIL;
        }
        if (FAILED( m_pGraphics->InitPipeline() ))
        {
            DEBUG_LOG_ERROR( "パイプラインの初期化に失敗しました" );
            return E_FAIL;
        }

        m_pTriangleMesh = std::make_unique<engine::Mesh>();
        engine::SimpleVertex vertices[] =
        {
            { XMFLOAT3( 0.0f,  0.5f, 0.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.5f, 0.0f ) },
            { XMFLOAT3( 0.5f, -0.5f, 0.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
            { XMFLOAT3( -0.5f, -0.5f, 0.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        };
        UINT indices[] = {0, 1, 2};

        if (FAILED( m_pTriangleMesh->Init( m_pGraphics->GetDevice(), vertices, 3, indices, 3 ) ))
        {
            DEBUG_LOG_ERROR( "Mesh の初期化に失敗しました" );
            return E_FAIL;
        }

        std::mt19937 rng( std::random_device {}( ) );
        std::uniform_real_distribution<float> distPos( -16.0f, 16.0f );
        std::uniform_real_distribution<float> distDepth( 10.0f, 42.0f );
        std::uniform_real_distribution<float> distColor( 0.0f, 1.0f );

		m_gameObjects.resize( kMaxModel );
		for (int i = 0; i < kMaxModel; i++)
		{
            float x = distPos( rng );
            float y = distPos( rng );
            float z = distDepth( rng );
            float r = distColor( rng );
            float g = distColor( rng );
            float b = distColor( rng );
            m_gameObjects[i].SetPos( XMFLOAT3( x, y, z ) );
            m_gameObjects[i].SetColor( XMFLOAT4( r, g, b, 1.0f ) );
		}

        DEBUG_LOG( "初期化完了" );
		return S_OK;
	}

	void Game::Update()
	{
		for (auto& obj : m_gameObjects) {
			obj.Update();
		}
	}
	
	void Game::Render()
	{
		m_pGraphics->BeginRender();

		// カメラ設定（回転などあればここで計算）
		m_pGraphics->SetCamera(
			XMVectorSet(0.0f, 1.0f, -2.0f, 0.0f), // Eye
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),  // Focus
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Up
		);
		// プロジェクション設定
		float aspect = (float)m_screenWidth / (float)m_screenHeight; 
		m_pGraphics->SetProjection(XM_PI / 4.0f, aspect, 0.1f, 100.0f);

		for (const auto& obj : m_gameObjects)
		{
			XMMATRIX mWorld = obj.GetWorldMatrix();
            m_pGraphics->Draw( m_pTriangleMesh.get(), mWorld, obj.GetColor() );
		}

		m_pGraphics->EndRender();
	}
}