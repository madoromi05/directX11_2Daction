#pragma once
#include "Window.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/GameObject.h"
#include "Game/Player/Camera.h"
#include <vector>
#include <memory>
#ifdef _DEBUG
#include "DebugHUD.h"
#endif

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
        struct ObjectMotion
        {
            DirectX::XMFLOAT3 rotSpeed;   // 自転速度 (rad/sec) 各軸
            float orbitRadius;            // 軌道半径
            float orbitAngle;             // 現在の軌道角度 (rad)
            float orbitSpeed;             // 公転速度 (rad/sec)
            float orbitCenterY;           // 軌道の中心Y座標
        };
        static constexpr int kMaxModel = 50;
		std::vector<engine::GameObject>     m_gameObjects;
        std::vector<ObjectMotion>           m_motions;
        std::unique_ptr<engine::Graphics>   m_pGraphics;
        std::unique_ptr<engine::Mesh>       m_pTriangleMesh;
        std::vector<std::unique_ptr<engine::Mesh>>   m_pMeshes;
        std::vector<engine::FbxModel> m_models;
		int m_screenWidth = 0;
		int m_screenHeight = 0;
        DWORD m_lastTime = 0;
        engine::Camera m_camera;
#ifdef _DEBUG
        game::DebugHUD m_debugHUD;
        float m_deltaTime = 0.0f;
#endif
	};
}