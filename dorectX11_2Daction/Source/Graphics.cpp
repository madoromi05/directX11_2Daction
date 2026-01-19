#include "Graphics.h"

HRESULT Graphics::InitD3D()
{
	HRESULT hr = S_OK;

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	D3D_FEATURE_LEVEL featureLevel;

	// デバイス作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		&featureLevel,
		&m_pDeviceContext
	);

	if (FAILED(hr))
	{
		return hr;
	}

	// レンダーターゲットビューの作成
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr)) return hr;

	// 深度ステンシルテクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	if (FAILED(hr)) return hr;

	// 深度ステンシルビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr)) return hr;

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)WINDOW_WIDTH;
	vp.Height = (FLOAT)WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* pIr = NULL;
	hr = m_pDevice->CreateRasterizerState(&rdc, &pIr);
	if (FAILED(hr)) return hr;

	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}

void Graphics::DestroyD3D()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDevice);
}

HRESULT Graphics::InitPolygon()
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	for (int i = 0; i < MAX_MODEL; i++)
	{
		m_Model[i].vPos = XMFLOAT3(float(rand()) / 1000.0f - 16.0f, float(rand()) / 1000.0f - 16.0f, float(rand()) / 1000.0f + 10.0f);//初期位置はランダム
		m_Model[i].vColor = XMFLOAT4(float(rand()) / 32767.0f, float(rand()) / 32767.0f, float(rand()) / 32767.0f, 1.0f);//色もランダム
	}
	m_iNumModel = MAX_MODEL;

	return S_OK;
}

void Graphics::Render()
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	float angle = (float)timeGetTime() / 1000.0f;
	XMMATRIX mWorld = XMMatrixRotationY(angle);

	// ビュートランスフォーム（視点座標変換）
	XMVECTOR vEyePt = XMVectorSet(0.0f, 1.0f, -2.0f, 0.0f); // カメラ位置
	XMVECTOR vLookatPt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);  // 注視位置
	XMVECTOR vUpVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  // 上方位置
	XMMATRIX mView = XMMatrixLookAtLH(vEyePt, vLookatPt, vUpVec);

	// プロジェクション行列（射影）
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(XM_PI / 4.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//プリミティブをレンダリング

	angle = (float)timeGetTime() / 1000.0f;

	for (int i = 0; i < m_iNumModel; i++)
	{
		// ワールド行列作成（回転 ＋ 位置）
		XMMATRIX mRot = XMMatrixRotationY(angle);
		XMMATRIX mTrans = XMMatrixTranslation(m_Model[i].vPos.x, m_Model[i].vPos.y, m_Model[i].vPos.z);
		XMMATRIX mWorld = mRot * mTrans;

		// コンスタントバッファ更新
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SimpleShaderConstantBuffer cb;

			// 行列合成
			XMMATRIX mWVP = mWorld * mView * mProj;
			mWVP = XMMatrixTranspose(mWVP);
			XMStoreFloat4x4(&cb.mWVP, mWVP);

			// 色設定
			cb.vColor = m_Model[i].vColor;

			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}

		// 描画
		m_pDeviceContext->Draw(3, 0);
	}

	// 4. 最後に1回だけ画面更新
	m_pSwapChain->Present(0, 0);
}

HRESULT Graphics::InitPipeline()
{
	HRESULT hr = S_OK;
	
	// 実行ファイル（exe）のあるパスを取得する
	WCHAR path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);
	PathRemoveFileSpec(path); // ファイル名（exe）を削ってフォルダパスだけにする

	// フォルダパスとファイル名を結合して、絶対パスを作る
	WCHAR vsPath[MAX_PATH] = { 0 };
	wcscpy_s(vsPath, path);
	wcscat_s(vsPath, L"\\VertexShader.cso");

	WCHAR psPath[MAX_PATH] = { 0 };
	wcscpy_s(psPath, path);
	wcscat_s(psPath, L"\\PixelShader.cso");

	// 作ったパスを使って読み込む
	ID3DBlob* pVSBlob = NULL;

	// VertexShader.cso はビルド時に自動生成されます
	hr = D3DReadFileToBlob(vsPath, &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"VertexShader.cso が見つかりません。", L"エラー", MB_OK);
		return hr;
	}

	// バーテックスシェーダーの作成
	hr = m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr)) { pVSBlob->Release(); return hr; }

	// インプットレイアウトの作成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = m_pDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr)) return hr;

	// ピクセルシェーダーも同様に .cso を読み込む
	ID3DBlob* pPSBlob = NULL;
	hr = D3DReadFileToBlob(psPath, &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"PixelShader.cso が見つかりません。", L"エラー", MB_OK);
		return hr;
	}

	// ピクセルシェーダーの作成
	hr = m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;

	// コンスタントバッファの作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SimpleShaderConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}