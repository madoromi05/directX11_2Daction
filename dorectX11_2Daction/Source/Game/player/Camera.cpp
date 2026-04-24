#include "Camera.h"
#include <windows.h>
#include <directxmath.h>

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
