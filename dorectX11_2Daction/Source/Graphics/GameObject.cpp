#include "GameObject.h"

namespace game
{
	GameObject::GameObject()
		: m_Pos(0.0f, 0.0f, 0.0f)
		, m_Rot(0.0f, 0.0f, 0.0f) // èâä˙âª
		, m_Color(1.0f, 1.0f, 1.0f, 1.0f)
	{}

	void GameObject::Update()
	{
		m_Rot.y += 0.002f;
	}

	void GameObject::SetPos(XMFLOAT3 pos) { m_Pos = pos; }
	void GameObject::SetColor(XMFLOAT4 color) { m_Color = color; }
	void GameObject::SetRotation(XMFLOAT3 rot) { m_Rot = rot; }

	XMFLOAT3 GameObject::GetPos() const { return m_Pos; }
	XMFLOAT4 GameObject::GetColor() const { return m_Color; }

	XMMATRIX GameObject::GetWorldMatrix() const
	{
		XMMATRIX S = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX R = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
		XMMATRIX T = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);

		// çsóÒÇÃä|ÇØçáÇÌÇπ (Scale * Rotate * Translate)
		return S * R * T;
	}
}