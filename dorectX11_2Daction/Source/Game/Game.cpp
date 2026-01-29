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
		if (m_pWindow) delete m_pWindow;
	}

	HRESULT Game::Init(HINSTANCE hInstance, int width, int height, LPCWSTR name)
	{
		// 1. Window作成
		m_pWindow = new engin::Window();
		if (FAILED(m_pWindow->InitWindow(hInstance, 0, 0, width, height, name)))
		{
			return E_FAIL;
		}
		// 2. Graphics作成
		m_pGraphics = new engin::Graphics();
		// Windowハンドルは m_pWindow経由で取得
		if (FAILED(m_pGraphics->InitD3D(m_pWindow->GetHWND(), width, height))) return E_FAIL;
		if (FAILED(m_pGraphics->InitPipeline())) return E_FAIL;

		// 3. Mesh作成
		m_pTriangleMesh = new engin::Mesh();
		engin::SimpleVertex vertices[] = {
			XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, -0.5f, 0.0f),
			XMFLOAT3(-0.5f, -0.5f, 0.0f),
		};
		// Device取得もゲッター経由で
		if (FAILED(m_pTriangleMesh->Init(m_pGraphics->GetDevice(), vertices, 3))) return E_FAIL;

		// 4. ゲームオブジェクト初期化
		m_GameObjects.resize(MAX_MODEL);
		for (int i = 0; i < MAX_MODEL; i++)
		{
			// ランダム位置
			float x = float(rand()) / 1000.0f - 16.0f;
			float y = float(rand()) / 1000.0f - 16.0f;
			float z = float(rand()) / 1000.0f + 10.0f;
			m_GameObjects[i].SetPos(XMFLOAT3(x, y, z));

			// ランダム色
			float r = float(rand()) / 32767.0f;
			float g = float(rand()) / 32767.0f;
			float b = float(rand()) / 32767.0f;
			m_GameObjects[i].SetColor(XMFLOAT4(r, g, b, 1.0f));

			// 初期回転もランダムにするならここで SetRotation
		}

		return S_OK;
	}

	void Game::Run()
	{
		while (true)
		{
			// Windowのメッセージ処理。
			if (!m_pWindow->ProcessMessage()) break;

			Update();
			Render();
		}
	}

	void Game::Update()
	{
		for (auto& obj : m_GameObjects) {
			obj.Update();
		}
	}
	
	void Game::Render()
	{
		// 描画準備
		m_pGraphics->BeginRender();

		// カメラ設定（回転などあればここで計算）
		m_pGraphics->SetCamera(
			XMVectorSet(0.0f, 1.0f, -2.0f, 0.0f), // Eye
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),  // Focus
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Up
		);
		// プロジェクション設定
		RECT rc;
		GetClientRect(m_pWindow->GetHWND(), &rc);
		float width = (float)(rc.right - rc.left);
		float height = (float)(rc.bottom - rc.top);
		m_pGraphics->SetProjection(XM_PI / 4.0f, width / height, 0.1f, 100.0f);

		// 3. 全オブジェクト描画
		float angle = (float)timeGetTime() / 1000.0f;
		XMMATRIX mRot = XMMatrixRotationY(angle);

		for (const auto& obj : m_GameObjects)
		{
			XMMATRIX mWorld = obj.GetWorldMatrix();
			m_pGraphics->Draw(m_pTriangleMesh, mWorld, obj.GetColor());
		}

		// 4. 描画終了
		m_pGraphics->EndRender();
	}
}