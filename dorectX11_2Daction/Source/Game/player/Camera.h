#pragma once
#include "Engine/Graphics.h"
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
        void Move( DirectX::XMFLOAT3 delta );

        // カメラの向きを基準に移動する（FPS移動用）
        void MoveLocal( DirectX::XMFLOAT3 localDelta );

        // マウス入力でヨー・ピッチを更新し、m_focusを再計算する。
        // 毎フレームUpdate内で呼ぶ。
        void UpdateMouseLook( int mouseDeltaX, int mouseDeltaY, float sensitivity );

        void SetPosition( DirectX::XMFLOAT3 eye );

        DirectX::XMFLOAT3 GetPosition() const { return m_eye; }
        float GetYaw()   const { return m_yaw; }
        float GetPitch() const { return m_pitch; }

    private:
        // ヨー・ピッチからフォワードベクトルを計算してm_focusを更新する
        void UpdateFocusFromAngles();

        DirectX::XMFLOAT3 m_eye;
        DirectX::XMFLOAT3 m_focus;
        DirectX::XMFLOAT3 m_up;
        float m_fovY;
        float m_nearZ;
        float m_farZ;

        // FPSカメラのオイラー角（ラジアン）
        // yaw  : Y軸回転（左右）、初期値は+Z方向を向くよう設定
        // pitch: X軸回転（上下）、±kMaxPitchRadでクランプ
        float m_yaw = DirectX::XM_PI;
        float m_pitch = 0.0f;

        static constexpr float kMaxPitchDeg = 100.0f;
        static constexpr float kMaxPitchRad = kMaxPitchDeg * DirectX::XM_PI / 180.0f;
    };
}