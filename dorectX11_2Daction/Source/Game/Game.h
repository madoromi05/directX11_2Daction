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
		HRESULT Init(HWND hWnd, int width, int height);;
		

		void Update();
		void Render();
	private:
		std::vector<GameObject> m_gameObjects;

		engine::Graphics* m_pGraphics = nullptr;
		engine::Mesh* m_pTriangleMesh = nullptr;
		int m_screenWidth = 0;
		int m_screenHeight = 0;
	};
}