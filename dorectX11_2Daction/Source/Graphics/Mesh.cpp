#include "Mesh.h"

namespace engine
{
    HRESULT Mesh::Init( ID3D11Device* pDevice, SimpleVertex* pVertices, UINT vertexCount )
    {
        m_vertexCount = vertexCount;

        D3D11_BUFFER_DESC bd;
        ZeroMemory( &bd, sizeof( bd ) );
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof( SimpleVertex ) * vertexCount;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory( &initData, sizeof( initData ) );
        initData.pSysMem = pVertices;

        return pDevice->CreateBuffer( &bd, &initData, m_pVertexBuffer.GetAddressOf() );
    }

    void Mesh::Draw( ID3D11DeviceContext* pContext )
    {
        UINT stride = sizeof( SimpleVertex );
        UINT offset = 0;
        ID3D11Buffer* pBuf = m_pVertexBuffer.Get();
        pContext->IASetVertexBuffers( 0, 1, &pBuf, &stride, &offset );
        pContext->Draw( m_vertexCount, 0 );
    }
}