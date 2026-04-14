#include "Mesh.h"

namespace engine
{
	Mesh::Mesh() {}

	Mesh::~Mesh()
	{
		if (m_pVertexBuffer) m_pVertexBuffer->Release();
	}

	HRESULT Mesh::Init(ID3D11Device* pDevice, SimpleVertex* pVertices, int vertexCount)
	{
		m_VertexCount = vertexCount;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * vertexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pVertices;

		return pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	}

	void Mesh::Draw(ID3D11DeviceContext* pContext)
	{
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pContext->Draw(m_VertexCount, 0);
	}
}