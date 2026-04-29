#define NOMINMAX
#include "ModelLoader.h"
#include "Engine/Mesh.h"
#include "Engine/ShaderCode/SimpleVertex.h"
#include "Debug.h"
#include <DirectXTex.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

namespace engine
{
    std::vector<std::unique_ptr<Mesh>> ModelLoader::Load( ID3D11Device* pDevice, const std::string& filePath )
    {
        Assimp::Importer importer;

        // aiProcess_Triangulate : ポリゴンを全て三角形に分割する
        const aiScene* pScene = importer.ReadFile( filePath, aiProcess_Triangulate );
        if (pScene == nullptr)
        {
            DEBUG_LOG_ERROR( "モデルのロードに失敗しました: " + filePath
                + " / " + importer.GetErrorString() );
            return {};
        }

        std::vector<std::unique_ptr<Mesh>> meshes;
        meshes.reserve( pScene->mNumMeshes );

        for (UINT meshIdx = 0; meshIdx < pScene->mNumMeshes; ++meshIdx)
        {
            const aiMesh* pAiMesh = pScene->mMeshes[meshIdx];

            // 頂点データを SimpleVertex に変換する
            std::vector<SimpleVertex> vertices( pAiMesh->mNumVertices );
            for (UINT i = 0; i < pAiMesh->mNumVertices; ++i)
            {
                const auto& pos = pAiMesh->mVertices[i];
                vertices[i].position = DirectX::XMFLOAT3( pos.x, pos.y, pos.z );

                if (pAiMesh->HasNormals())
                {
                    const auto& n = pAiMesh->mNormals[i];
                    vertices[i].normal = DirectX::XMFLOAT3( n.x, n.y, n.z );
                }

                // テクスチャ座標はチャンネル0のみ使用する
                if (pAiMesh->HasTextureCoords( 0 ))
                {
                    const auto& uv = pAiMesh->mTextureCoords[0][i];
                    vertices[i].texCoord = DirectX::XMFLOAT2( uv.x, uv.y );
                }
            }

            // インデックスデータを取得する（TriangleList前提）
            std::vector<UINT> indices;
            indices.reserve( pAiMesh->mNumFaces * 3 );
            for (UINT faceIdx = 0; faceIdx < pAiMesh->mNumFaces; ++faceIdx)
            {
                const aiFace& face = pAiMesh->mFaces[faceIdx];
                for (UINT idx = 0; idx < face.mNumIndices; ++idx)
                {
                    indices.push_back( face.mIndices[idx] );
                }
            }

            auto pMesh = std::make_unique<Mesh>();
            if (FAILED( pMesh->Init(
                pDevice,
                vertices.data(),
                static_cast< UINT >( vertices.size() ),
                indices.data(),
                static_cast< UINT >( indices.size() ) ) ))
            {
                DEBUG_LOG_ERROR( "Mesh の初期化に失敗しました (index: "
                    + std::to_string( meshIdx ) + ")" );
                return {};
            }
            meshes.push_back( std::move( pMesh ) );
        }

        return meshes;
    }
}