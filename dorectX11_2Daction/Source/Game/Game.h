#pragma once
#include "Window.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/GameObject.h"
#include <vector>
#include <memory>

namespace game
{
	class Game
	{
	public:
		Game();
		~Game();

		HRESULT Init(HWND hWnd, int width, int height);;

		void Update();
		void Render();
	private:
        static constexpr int kMaxModel = 50;
		std::vector<engine::GameObject>     m_gameObjects;
        std::unique_ptr<engine::Graphics>   m_pGraphics;
        std::unique_ptr<engine::Mesh>       m_pTriangleMesh;
        std::vector<std::unique_ptr<engine::Mesh>>   m_pMeshes;
		int m_screenWidth = 0;
		int m_screenHeight = 0;
	};
}