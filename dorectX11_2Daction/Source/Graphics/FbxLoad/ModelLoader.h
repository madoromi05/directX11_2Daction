#pragma once
#include "Graphics/Mesh.h"
#include <d3d11.h>
#include <string>
#include <vector>
#include <memory>

namespace engine
{
    class ModelLoader
    {
    public:
        ModelLoader() = default;
        ~ModelLoader() = default;

        // 失敗時は nullptr を返す。
        std::vector<std::unique_ptr<Mesh>> Load( ID3D11Device* pDevice, const std::string& filePath );
    };
}