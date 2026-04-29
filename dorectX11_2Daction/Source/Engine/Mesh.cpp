#include "Mesh.h"

namespace engine
{
    HRESULT Mesh::Init( ID3D11Device* pDevice,
        SimpleVertex* pVertices, UINT vertexCount,
        UINT* pIndices, UINT indexCount )
    {
        if (FAILED( CreateVertexBuffer( pDevice, pVertices, vertexCount ) )) return E_FAIL;
        if (FAILED( CreateIndexBuffer( pDevice, pIndices, indexCount ) ))    return E_FAIL;
        return S_OK;
    }

    HRESULT Mesh::CreateVertexBuffer( ID3D11Device* pDevice,
        SimpleVertex* pVertices, UINT vertexCount )
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

    HRESULT Mesh::CreateIndexBuffer( ID3D11Device* pDevice,
        UINT* pIndices, UINT indexCount )
    {
        m_indexCount = indexCount;

        D3D11_BUFFER_DESC bd;
        ZeroMemory( &bd, sizeof( bd ) );
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof( UINT ) * indexCount;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory( &initData, sizeof( initData ) );
        initData.pSysMem = pIndices;

        return pDevice->CreateBuffer( &bd, &initData, m_pIndexBuffer.GetAddressOf() );
    }

    void Mesh::Draw( ID3D11DeviceContext* pContext )
    {
        UINT stride = sizeof( SimpleVertex );
        UINT offset = 0;
        ID3D11Buffer* pVBuf = m_pVertexBuffer.Get();
        pContext->IASetVertexBuffers( 0, 1, &pVBuf, &stride, &offset );
        pContext->IASetIndexBuffer( m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );
        pContext->DrawIndexed( m_indexCount, 0, 0 );
    }
}