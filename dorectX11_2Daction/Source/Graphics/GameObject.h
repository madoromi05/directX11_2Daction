#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace game
{
	class GameObject
	{
	public:
		// コンストラクタ
		GameObject();
		virtual ~GameObject() {}

		// 更新処理（移動ロジックはここへ）
		void Update();

		// 初期化用セッター
		void SetPos(XMFLOAT3 pos);
		void SetColor(XMFLOAT4 color);
		void SetRotation(XMFLOAT3 rot);

		// 描画用ゲッター
		XMFLOAT3 GetPos() const;
		XMFLOAT4 GetColor() const;
		XMMATRIX GetWorldMatrix() const;

	private:
		XMFLOAT3 m_Pos;   // 位置
		XMFLOAT3 m_Rot;   // 回転
		XMFLOAT4 m_Color; // 色
	};
}