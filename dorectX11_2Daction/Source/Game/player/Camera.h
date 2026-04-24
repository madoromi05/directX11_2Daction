#pragma once
#include "Graphics/Graphics.h"
#include <DirectXMath.h>

namespace engine { class Graphics; }

namespace engine
{
    class Camera
    {
    public:
        Camera();

        // カメラ行列をGraphicsに適用
        void Apply(Graphics* pGraphics, float aspect) const;

        // eye/focusを同時に動かす（視線方向を維持）
        void Move(DirectX::XMFLOAT3 delta);

        // 直接セット
        void SetPosition(DirectX::XMFLOAT3 eye);
        void SetLookAt(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus);

        DirectX::XMFLOAT3 GetPosition() const { return m_eye; }

    private:
        DirectX::XMFLOAT3 m_eye;
        DirectX::XMFLOAT3 m_focus;
        DirectX::XMFLOAT3 m_up;
        float m_fovY;
        float m_nearZ;
        float m_farZ;
    };
}