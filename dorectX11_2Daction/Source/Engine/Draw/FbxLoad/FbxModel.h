#pragma once
#include "FbxLoad/ModelLoader.h"
#include "Mesh.h"
#include "GameObject.h"
#include <vector>
#include <string>
#include <memory>

namespace engine {

    class FbxModel {
    public:
        bool Load( ID3D11Device* pDevice, const std::string& filePath ) {
            ModelLoader loader;
            m_meshes = loader.Load( pDevice, filePath );
            if (m_meshes.empty()) return false;

            for (auto& pMesh : m_meshes)
                m_gameObject.AddMesh( pMesh.get() );  // 参照を登録

            return true;
        }

        // --- 移動・回転・スケール ---
        void SetPosition( float x, float y, float z ) {
            m_gameObject.SetPos( {x, y, z} );
        }
        void SetPosition( DirectX::XMFLOAT3 pos ) {
            m_gameObject.SetPos( pos );
        }
        void SetRotation( float rx, float ry, float rz ) {
            m_gameObject.SetRotation( {rx, ry, rz} );
        }
        void SetScale( float sx, float sy, float sz ) {
            m_gameObject.SetScale( {sx, sy, sz} );
        }
        void SetScale( float s ) {
            m_gameObject.SetScale( {s, s, s} );
        }

        // --- 差分移動・回転（毎フレームの操作向け）---
        void Move( float dx, float dy, float dz ) {
            auto pos = m_gameObject.GetPos();
            m_gameObject.SetPos( {pos.x + dx, pos.y + dy, pos.z + dz} );
        }
        void Rotate( float drx, float dry, float drz ) {
            auto rot = m_gameObject.GetRotation();
            m_gameObject.SetRotation( {rot.x + drx, rot.y + dry, rot.z + drz} );
        }

        // --- 色 ---
        void SetColor( float r, float g, float b, float a = 1.0f ) {
            m_gameObject.SetColor( {r, g, b, a} );
        }

        // --- ゲッター ---
        const GameObject& GetGameObject() const { return m_gameObject; }

    private:
        std::vector<std::unique_ptr<Mesh>> m_meshes;  // メッシュを所有
        GameObject m_gameObject;                       // トランスフォーム管理
    };

} // namespace engine
