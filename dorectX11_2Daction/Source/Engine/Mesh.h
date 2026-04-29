/// Graphicsクラスに頂点情報を出してメッシュを描画するためのクラス
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "ShaderCode/SimpleVertex.h"

using namespace DirectX;

namespace engine
{
	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		// デバイスを受け取ってバッファを作る
        HRESULT Init( ID3D11Device* pDevice,SimpleVertex* pVertices, UINT vertexCount,UINT* pIndices, UINT indexCount );
		// 描画時にセットする
		void Draw(ID3D11DeviceContext* pContext);

	private:
        HRESULT CreateVertexBuffer( ID3D11Device* pDevice, SimpleVertex* pVertices, UINT vertexCount );
        HRESULT CreateIndexBuffer( ID3D11Device* pDevice, UINT* pIndices, UINT indexCount );
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
        UINT m_vertexCount = 0;
        UINT m_indexCount = 0;
	};
}