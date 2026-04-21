#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace engine
{
	class GameObject
	{
	public:
		GameObject();
        virtual ~GameObject() = default;
        
        void SetPos( DirectX::XMFLOAT3 pos );
        void SetRotation( DirectX::XMFLOAT3 rot );
        void SetScale( DirectX::XMFLOAT3 scale );
        void SetColor( DirectX::XMFLOAT4 color );

        virtual void Update() {}

        DirectX::XMFLOAT3 GetPos() const;
        DirectX::XMFLOAT4 GetColor() const;
        DirectX::XMMATRIX GetWorldMatrix() const;

	private:
        DirectX::XMFLOAT3 m_pos;
        DirectX::XMFLOAT3 m_rot;
        DirectX::XMFLOAT3 m_scale;
        DirectX::XMFLOAT4 m_color;
	};
}