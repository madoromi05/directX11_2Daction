#include "Graphics.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

namespace engin
{
	Graphics::Graphics()
	{
		XMStoreFloat4x4(&m_mView, XMMatrixIdentity());
		XMStoreFloat4x4(&m_mProj, XMMatrixIdentity());
	}

	Graphics::~Graphics()
	{
		DestroyD3D();
	}

	HRESULT Graphics::InitD3D(HWND hWnd, int width, int height)
	{
		HRESULT hr = S_OK;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);
		D3D_FEATURE_LEVEL featureLevel;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext);
		if (FAILED(hr)) return hr;

		ID3D11Texture2D* pBackBuffer = NULL;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr)) return hr;

		hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		SAFE_RELEASE(pBackBuffer);
		if (FAILED(hr)) return hr;

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
		if (FAILED(hr)) return hr;

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr)) return hr;

		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pDeviceContext->RSSetViewports(1, &vp);

		return S_OK;
	}

	HRESULT Graphics::InitPipeline()
	{
		HRESULT hr = S_OK;
		WCHAR path[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, path, MAX_PATH);
		PathRemoveFileSpec(path);

		WCHAR vsPath[MAX_PATH] = { 0 };
		wcscpy_s(vsPath, path);
		wcscat_s(vsPath, L"\\VertexShader.cso");

		WCHAR psPath[MAX_PATH] = { 0 };
		wcscpy_s(psPath, path);
		wcscat_s(psPath, L"\\PixelShader.cso");

		ID3DBlob* pVSBlob = NULL;
		hr = D3DReadFileToBlob(vsPath, &pVSBlob);
		if (FAILED(hr)) return hr;

		hr = m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
		if (FAILED(hr)) { pVSBlob->Release(); return hr; }

		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr)) return hr;

		ID3DBlob* pPSBlob = NULL;
		hr = D3DReadFileToBlob(psPath, &pPSBlob);
		if (FAILED(hr)) return hr;

		hr = m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
		pPSBlob->Release();
		if (FAILED(hr)) return hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SimpleShaderConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
		if (FAILED(hr)) return hr;

		return S_OK;
	}

	HRESULT Graphics::InitModelBuffer()
	{
		SimpleVertex vertices[] = {
			XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, -0.5f, 0.0f),
			XMFLOAT3(-0.5f, -0.5f, 0.0f),
		};
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;

		return m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	}

	void Graphics::DestroyD3D()
	{
		SAFE_RELEASE(m_pVertexLayout);
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);
		SAFE_RELEASE(m_pConstantBuffer);
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDeviceContext);
		SAFE_RELEASE(m_pDepthStencilView);
		SAFE_RELEASE(m_pDepthStencil);
		SAFE_RELEASE(m_pDevice);
	}

	void Graphics::BeginRender()
	{
		float ClearColor[4] = { 0.0f, 0.0f, 0.2f, 1.0f }; // 少し暗い青
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		// パイプライン設定
		m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 頂点バッファ設定（今回は1種類固定）
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}

	void Graphics::EndRender()
	{
		m_pSwapChain->Present(0, 0);
	}

	void Graphics::SetCamera(FXMVECTOR eye, FXMVECTOR focus, FXMVECTOR up)
	{
		XMMATRIX view = XMMatrixLookAtLH(eye, focus, up);
		XMStoreFloat4x4(&m_mView, view);
	}

	void Graphics::SetProjection(float fov, float aspect, float nearZ, float farZ)
	{
		XMMATRIX proj = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
		XMStoreFloat4x4(&m_mProj, proj);
	}

	void Graphics::Draw(CXMMATRIX worldMatrix, const XMFLOAT4& color)
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SimpleShaderConstantBuffer cb;

			// 行列合成 (World * View * Proj)
			XMMATRIX view = XMLoadFloat4x4(&m_mView);
			XMMATRIX proj = XMLoadFloat4x4(&m_mProj);
			XMMATRIX wvp = worldMatrix * view * proj;

			wvp = XMMatrixTranspose(wvp);
			XMStoreFloat4x4(&cb.mWVP, wvp);
			cb.vColor = color;

			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}

		m_pDeviceContext->Draw(3, 0);
	}
}