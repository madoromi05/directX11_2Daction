#pragma once
#include "Window.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/GameObject.h"
#include <vector>

#define MAX_MODEL 50

namespace game
{
	class Game : public engin::Window
	{
	public:
		Game();
		~Game();

		// Windowクラスの純粋仮想関数を実装
		void App() override;

		// ゲーム初期化
		HRESULT Init(HINSTANCE hInstance, int width, int height, LPCWSTR name);

	private:
		// エンジン機能（コンポジションで持つ）
		engin::Graphics* m_pGraphics;

		// ゲームデータ
		std::vector<GameObject> m_GameObjects;
	};
}