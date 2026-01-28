#include "GameObject.h"

namespace game
{
	GameObject::GameObject()
		: m_Pos(0.0f, 0.0f, 0.0f)
		, m_Color(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}

	void GameObject::Update()
	{
		m_Pos.x += 0.001f;
	}

	void GameObject::SetPos(XMFLOAT3 pos)
	{
		m_Pos = pos;
	}

	void GameObject::SetColor(XMFLOAT4 color)
	{
		m_Color = color;
	}

	XMFLOAT3 GameObject::GetPos() const
	{
		return m_Pos;
	}

	XMFLOAT4 GameObject::GetColor() const
	{
		return m_Color;
	}
}