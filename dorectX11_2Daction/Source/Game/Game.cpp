#include "Game.h"
#include "Debug.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace game
{
	Game::Game() {}

	Game::~Game()
	{
		if (m_pTriangleMesh) delete m_pTriangleMesh;
		if (m_pGraphics) delete m_pGraphics;
	}

	HRESULT Game::Init(HWND hWnd, int width, int height)
	{
		m_screenWidth = width;
		m_screenHeight = height;

		m_pGraphics = new engine::Graphics();
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

		m_pTriangleMesh = new engine::Mesh();
		engine::SimpleVertex vertices[] = {
			XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, -0.5f, 0.0f),
			XMFLOAT3(-0.5f, -0.5f, 0.0f),
		};
        if (FAILED( m_pTriangleMesh->Init( m_pGraphics->GetDevice(), vertices, 3 ) ))
        {
            DEBUG_LOG_ERROR( "Mesh の初期化に失敗しました" );
            return E_FAIL;
        }


		m_gameObjects.resize( kMaxModel );
		for (int i = 0; i < kMaxModel; i++)
		{
			// ランダム位置
			float x = float(rand()) / 1000.0f - 16.0f;
			float y = float(rand()) / 1000.0f - 16.0f;
			float z = float(rand()) / 1000.0f + 10.0f;
			m_gameObjects[i].SetPos(XMFLOAT3(x, y, z));

			// ランダム色
			float r = float(rand()) / 32767.0f;
			float g = float(rand()) / 32767.0f;
			float b = float(rand()) / 32767.0f;
			m_gameObjects[i].SetColor(XMFLOAT4(r, g, b, 1.0f));
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

		// 全オブジェクト描画
		float angle = (float)timeGetTime() / 1000.0f;
		XMMATRIX mRot = XMMatrixRotationY(angle);

		for (const auto& obj : m_gameObjects)
		{
			XMMATRIX mWorld = obj.GetWorldMatrix();
			m_pGraphics->Draw(m_pTriangleMesh, mWorld, obj.GetColor());
		}

		// 描画終了
		m_pGraphics->EndRender();
	}
}