/// Graphicsクラスに頂点情報を出してメッシュを描画するためのクラス

#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace engin
{
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
	};

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		// デバイスを受け取ってバッファを作る
		HRESULT Init(ID3D11Device* pDevice, SimpleVertex* pVertices, int vertexCount);

		// 描画時にセットする
		void Draw(ID3D11DeviceContext* pContext);

	private:
		ID3D11Buffer* m_pVertexBuffer = NULL;
		UINT m_VertexCount = 0;
	};
}