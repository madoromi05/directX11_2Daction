#include "GameObject.h"

namespace engine
{
	GameObject::GameObject()
        : m_pos( 0.0f, 0.0f, 0.0f )
        , m_rot( 0.0f, 0.0f, 0.0f )
        , m_scale( 1.0f, 1.0f, 1.0f )
        , m_color( 1.0f, 1.0f, 1.0f, 1.0f )
	{}

    void GameObject::AddMesh( Mesh* pMesh ) { if (pMesh) m_pMeshes.push_back( pMesh ); }
    void GameObject::ClearMeshes() { m_pMeshes.clear(); }

    void GameObject::SetPos( DirectX::XMFLOAT3 pos ) { m_pos = pos; }
    void GameObject::SetRotation( DirectX::XMFLOAT3 rot ) { m_rot = rot; }
    void GameObject::SetScale( DirectX::XMFLOAT3 scale ) { m_scale = scale; }
    void GameObject::SetColor( DirectX::XMFLOAT4 color ) { m_color = color; }

    DirectX::XMFLOAT3 GameObject::GetPos()   const { return m_pos; }
    DirectX::XMFLOAT3 GameObject::GetRotation() const { return m_rot; }
    DirectX::XMFLOAT4 GameObject::GetColor() const { return m_color; }
    const std::vector<Mesh*>& GameObject::GetMeshes() const { return m_pMeshes; }

    DirectX::XMMATRIX GameObject::GetWorldMatrix() const
    {
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling( m_scale.x, m_scale.y, m_scale.z );
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw( m_rot.x, m_rot.y, m_rot.z );
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation( m_pos.x, m_pos.y, m_pos.z );

        return S * R * T;
    }
}