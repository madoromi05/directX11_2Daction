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

		// 描画用ゲッター
		XMFLOAT3 GetPos() const;
		XMFLOAT4 GetColor() const;

	private:
		XMFLOAT3 m_Pos;   // 位置
		XMFLOAT4 m_Color; // 色
	};
}