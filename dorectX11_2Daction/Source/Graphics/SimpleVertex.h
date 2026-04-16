#pragma once
#include <DirectXMath.h>

namespace engine
{
    struct SimpleVertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texCoord;
    };

}