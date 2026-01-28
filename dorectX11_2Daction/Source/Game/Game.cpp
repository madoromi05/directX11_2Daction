#include "Game.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace game
{
	Game::Game() : m_pGraphics(NULL)
	{
	}

	Game::~Game()
	{
		if (m_pGraphics)
		{
			delete m_pGraphics;
			m_pGraphics = NULL;
		}
	}

	HRESULT Game::Init(HINSTANCE hInstance, int width, int height, LPCWSTR name)
	{
		// 1. ウィンドウ作成 (親クラスのメソッド)
		if (FAILED(InitWindow(hInstance, 0, 0, width, height, name)))
		{
			return E_FAIL;
		}

		// 2. Graphicsエンジン作成と初期化
		m_pGraphics = new engin::Graphics();
		if (FAILED(m_pGraphics->InitD3D(m_hWnd, width, height))) return E_FAIL;
		if (FAILED(m_pGraphics->InitPipeline())) return E_FAIL;
		if (FAILED(m_pGraphics->InitModelBuffer())) return E_FAIL;

		// 3. ゲームオブジェクト初期化
		m_GameObjects.resize(MAX_MODEL);
		for (int i = 0; i < MAX_MODEL; i++)
		{
			float x = float(rand()) / 1000.0f - 16.0f;
			float y = float(rand()) / 1000.0f - 16.0f;
			float z = float(rand()) / 1000.0f + 10.0f;
			m_GameObjects[i].SetPos(XMFLOAT3(x, y, z));

			float r = float(rand()) / 32767.0f;
			float g = float(rand()) / 32767.0f;
			float b = float(rand()) / 32767.0f;
			m_GameObjects[i].SetColor(XMFLOAT4(r, g, b, 1.0f));
		}

		return S_OK;
	}

	void Game::App()
	{
		// 1. 更新処理 (Update)
		for (auto& obj : m_GameObjects)
		{
			obj.Update();
		}

		// 2. 描画準備
		m_pGraphics->BeginRender();

		// カメラ設定（回転などあればここで計算）
		m_pGraphics->SetCamera(
			XMVectorSet(0.0f, 1.0f, -2.0f, 0.0f), // Eye
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),  // Focus
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Up
		);
		// プロジェクション設定
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		float width = (float)(rc.right - rc.left);
		float height = (float)(rc.bottom - rc.top);
		m_pGraphics->SetProjection(XM_PI / 4.0f, width / height, 0.1f, 100.0f);

		// 3. 全オブジェクト描画
		float angle = (float)timeGetTime() / 1000.0f;
		XMMATRIX mRot = XMMatrixRotationY(angle);

		for (const auto& obj : m_GameObjects)
		{
			XMFLOAT3 pos = obj.GetPos();
			XMMATRIX mTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);

			// 回転 * 平行移動
			XMMATRIX mWorld = mRot * mTrans;

			// エンジンに描画を依頼
			m_pGraphics->Draw(mWorld, obj.GetColor());
		}

		// 4. 描画終了
		m_pGraphics->EndRender();
	}
}