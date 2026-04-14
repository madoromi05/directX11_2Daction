 #include "Game.h"
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

		m_pGraphics = new engin::Graphics();
		if (FAILED(m_pGraphics->InitD3D(hWnd, width, height))) return E_FAIL;
		if (FAILED(m_pGraphics->InitPipeline())) return E_FAIL;

		m_pTriangleMesh = new engin::Mesh();
		engin::SimpleVertex vertices[] = {
			XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, -0.5f, 0.0f),
			XMFLOAT3(-0.5f, -0.5f, 0.0f),
		};
		// Device取得もゲッター経由で
		if (FAILED(m_pTriangleMesh->Init(m_pGraphics->GetDevice(), vertices, 3))) return E_FAIL;

		// ゲームオブジェクト初期化
		m_gameObjects.resize(MAX_MODEL);
		for (int i = 0; i < MAX_MODEL; i++)
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

			// 初期回転もランダムにするならここで SetRotation
		}

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