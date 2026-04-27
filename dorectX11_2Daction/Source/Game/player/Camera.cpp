#include "Camera.h"
#include <windows.h>
#include <directxmath.h>
#include <algorithm>

namespace engine
{
    Camera::Camera()
        : 
        m_eye(0.0f, 0.0f, -2.0f),
        m_focus(0.0f, 0.0f, 0.0f),
        m_up(0.0f, 1.0f, 0.0f),
        m_fovY(DirectX::XM_PI / 4.0f),
        m_nearZ(0.1f), m_farZ(100.0f)
    {}

    void Camera::Apply(Graphics* pGraphics, float aspect) const
    {
        pGraphics->SetCamera(
            XMLoadFloat3(&m_eye),    // Eye
            XMLoadFloat3(&m_focus),  // Focus
            XMLoadFloat3(&m_up)      // Up
        );
        pGraphics->SetProjection(m_fovY, aspect, m_nearZ, m_farZ);
    }

    void Camera::Move(DirectX::XMFLOAT3 delta)
    {
        m_eye.x += delta.x;
        m_eye.y += delta.y;
        m_eye.z += delta.z;
        m_focus.x += delta.x;
        m_focus.y += delta.y;
        m_focus.z += delta.z;
    }

    void Camera::MoveLocal( DirectX::XMFLOAT3 localDelta )
    {
        // カメラのフォワード・ライト方向を求める（Y成分は除きXZ平面で移動）
        DirectX::XMVECTOR forward = DirectX::XMVector3Normalize(
            DirectX::XMVectorSet(
                m_focus.x - m_eye.x,
                0.0f,
                m_focus.z - m_eye.z,
                0.0f
            )
        );
        DirectX::XMVECTOR right = DirectX::XMVector3Normalize(
            DirectX::XMVector3Cross( DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), forward )
        );

        DirectX::XMVECTOR move =
            DirectX::XMVectorScale( forward, localDelta.z ) +
            DirectX::XMVectorScale( right, localDelta.x );

        DirectX::XMFLOAT3 worldDelta;
        DirectX::XMStoreFloat3( &worldDelta, move );
        worldDelta.y += localDelta.y;

        Move( worldDelta );
    }

    void Camera::UpdateMouseLook( int mouseDeltaX, int mouseDeltaY, float sensitivity )
    {
        m_yaw += mouseDeltaX * sensitivity;
        m_pitch -= mouseDeltaY * sensitivity;  // Y軸は反転（上移動→ピッチ増）
        m_pitch = (std::clamp)( m_pitch, -kMaxPitchRad, kMaxPitchRad );

        UpdateFocusFromAngles();
    }

    void Camera::UpdateFocusFromAngles()
    {
        // オイラー角からフォワードベクトルを算出しfocusを更新する
        // forward.x = cos(pitch) * sin(yaw)
        // forward.y = sin(pitch)
        // forward.z = cos(pitch) * cos(yaw)
        DirectX::XMFLOAT3 forward;
        forward.x = cosf( m_pitch ) * sinf( m_yaw );
        forward.y = sinf( m_pitch );
        forward.z = cosf( m_pitch ) * cosf( m_yaw );

        m_focus.x = m_eye.x + forward.x;
        m_focus.y = m_eye.y + forward.y;
        m_focus.z = m_eye.z + forward.z;
    }
    void Camera::SetPosition(DirectX::XMFLOAT3 eye)
    {
        DirectX::XMFLOAT3 delta = {
            eye.x - m_eye.x,
            eye.y - m_eye.y,
            eye.z - m_eye.z
        };
        Move(delta);
    }
}
