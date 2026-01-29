#pragma once
#include "Window.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/GameObject.h"
#include <vector>

#define MAX_MODEL 50

namespace game
{
	class Game
	{
	public:
		Game();
		~Game();

		// ゲーム初期化
		HRESULT Init(HINSTANCE hInstance, int width, int height, LPCWSTR name);
		// ゲームループ
		void Run();
	private:
		void Update();
		void Render();

		engin::Window* m_pWindow = NULL;
		engin::Graphics* m_pGraphics = NULL;

		// 共有のリソースとしてMeshを持つ
		engin::Mesh* m_pTriangleMesh = NULL;

		std::vector<GameObject> m_GameObjects;
	};
}