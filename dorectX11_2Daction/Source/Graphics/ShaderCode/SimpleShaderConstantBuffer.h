#pragma once
#include <DirectXMath.h>

namespace engine
{
    struct SimpleShaderConstantBuffer
    {
        DirectX::XMFLOAT4X4 mWVP;
        DirectX::XMFLOAT4   polyColor;
    };
}
