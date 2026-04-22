#include "Game.h"
#include "Debug.h"
#include "Graphics/FbxLoad/ModelLoader.h"
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

        // 簡単なメッシュのロード
        //engine::SimpleVertex vertices[] =
        //{
        //    { XMFLOAT3( 0.0f,  0.5f, 0.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.5f, 0.0f ) },
        //    { XMFLOAT3( 0.5f, -0.5f, 0.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        //    { XMFLOAT3( -0.5f, -0.5f, 0.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        //};
        //UINT indices[] = {0, 1, 2};

        //if (FAILED( m_pTriangleMesh->Init( m_pGraphics->GetDevice(), vertices, 3, indices, 3 ) ))
        //{
        //    DEBUG_LOG_ERROR( "Mesh の初期化に失敗しました" );
        //    return E_FAIL;
        //}
        //m_pTriangleMesh = std::make_unique<engine::Mesh>();

        // FBXモデルのロード
        engine::ModelLoader loader;
        auto loadedMeshes = loader.Load( m_pGraphics->GetDevice(), "Source/assets/Models/cube.fbx" );
        if (loadedMeshes.empty())
        {
            DEBUG_LOG_ERROR( "モデルのロードに失敗しました" );
            return E_FAIL;
        }
        for (auto& pMesh : loadedMeshes)
        {
            m_pMeshes.push_back( std::move( pMesh ) );
        }
        // GameObjectの初期化
        std::mt19937 rng( std::random_device {}( ) );
        std::uniform_real_distribution<float> distDepth( 10.0f, 42.0f );
        std::uniform_real_distribution<float> distColor( 0.0f, 1.0f );
        std::uniform_real_distribution<float> distRotSpeed( -2.0f, 2.0f );
        std::uniform_real_distribution<float> distOrbitRadius( 1.0f, 8.0f );
        std::uniform_real_distribution<float> distOrbitAngle( 0.0f, DirectX::XM_2PI );
        std::uniform_real_distribution<float> distOrbitSpeed( -1.5f, 1.5f );
        std::uniform_real_distribution<float> distCenterY( -3.0f, 3.0f );

		m_gameObjects.resize( kMaxModel );
        m_motions.resize( kMaxModel );
		for (int i = 0; i < kMaxModel; i++)
		{
            float z = distDepth( rng );
            float r = distColor( rng );
            float g = distColor( rng );
            float b = distColor( rng );

            // 各オブジェクトにランダムな運動パラメータを設定
            m_motions[i].rotSpeed = {distRotSpeed( rng ), distRotSpeed( rng ), distRotSpeed( rng )};
            m_motions[i].orbitRadius = distOrbitRadius( rng );
            m_motions[i].orbitAngle = distOrbitAngle( rng );
            m_motions[i].orbitSpeed = distOrbitSpeed( rng );
            m_motions[i].orbitCenterY = distCenterY( rng );

            // 初期位置を軌道角度から計算
            float x = m_motions[i].orbitRadius * cosf( m_motions[i].orbitAngle );
            float y = m_motions[i].orbitCenterY + m_motions[i].orbitRadius * sinf( m_motions[i].orbitAngle );
            m_gameObjects[i].SetPos( XMFLOAT3( x, y, z ) );
            m_gameObjects[i].SetColor( XMFLOAT4( r, g, b, 1.0f ) );

            for (const auto& pMesh : m_pMeshes)
            {
                m_gameObjects[i].AddMesh( pMesh.get() );
            }
		}

        m_lastTime = timeGetTime();

        DEBUG_LOG( "初期化完了" );
		return S_OK;
	}

    void Game::Update()
    {
        DWORD currentTime = timeGetTime();
        float deltaTime = ( currentTime - m_lastTime ) * 0.001f;
        m_lastTime = currentTime;
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        for (int i = 0; i < kMaxModel; i++)
        {
            auto& obj = m_gameObjects[i];
            auto& motion = m_motions[i];

            // 自転: 各軸を毎フレーム回転
            auto rot = obj.GetRotation();
            rot.x += motion.rotSpeed.x * deltaTime;
            rot.y += motion.rotSpeed.y * deltaTime;
            rot.z += motion.rotSpeed.z * deltaTime;
            obj.SetRotation( rot );

            // 公転: XY平面上の円軌道（Z深度は固定）
            motion.orbitAngle += motion.orbitSpeed * deltaTime;
            float x = motion.orbitRadius * cosf( motion.orbitAngle );
            float y = motion.orbitCenterY + motion.orbitRadius * sinf( motion.orbitAngle );
            auto pos = obj.GetPos();
            obj.SetPos( XMFLOAT3( x, y, pos.z ) );

            obj.Update();
        }
    }
	
	void Game::Render()
	{
		m_pGraphics->BeginRender();

		// カメラ設定（回転などあればここで計算）
		m_pGraphics->SetCamera(
			XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f), // Eye
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),  // Focus
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Up
		);
		// プロジェクション設定
        float aspect = static_cast< float >( m_screenWidth ) / static_cast< float >( m_screenHeight );
        m_pGraphics->SetProjection( DirectX::XM_PI / 4.0f, aspect, 0.1f, 100.0f );

		for (const auto& obj : m_gameObjects)
		{
			XMMATRIX mWorld = obj.GetWorldMatrix();
            for (engine::Mesh* pMesh : obj.GetMeshes())
            {
                m_pGraphics->Draw( pMesh, mWorld, obj.GetColor() );
            }
		}

		m_pGraphics->EndRender();
	}
}