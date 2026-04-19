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
        HRESULT Init( ID3D11Device* pDevice, SimpleVertex* pVertices, UINT vertexCount );

		// 描画時にセットする
		void Draw(ID3D11DeviceContext* pContext);

	private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
        UINT m_vertexCount = 0;
	};
}