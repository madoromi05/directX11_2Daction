#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h> 
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

using namespace DirectX;

namespace engin
{
	// 頂点フォーマットはエンジン側で定義
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
	};

	// 定数バッファ
	struct SimpleShaderConstantBuffer
	{
		XMFLOAT4X4 mWVP;
		XMFLOAT4 vColor;
	};

	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		// 初期化・終了
		HRESULT InitD3D(HWND hWnd, int width, int height);
		HRESULT InitPipeline();
		HRESULT InitModelBuffer(); // 汎用的なモデル（今回は三角形)
		void DestroyD3D();

		// 描画制御
		void BeginRender(); // 描画開始（画面クリア）
		void EndRender();   // 描画終了（Present）

		// カメラ設定
		void SetCamera(FXMVECTOR eye, FXMVECTOR focus, FXMVECTOR up);
		void SetProjection(float fov, float aspect, float nearZ, float farZ);

		// 個別描画（ワールド行列と色を受け取って描画）
		void Draw(CXMMATRIX worldMatrix, const XMFLOAT4& color);

	private:
		ID3D11Device* m_pDevice = NULL;
		ID3D11DeviceContext* m_pDeviceContext = NULL;
		IDXGISwapChain* m_pSwapChain = NULL;
		ID3D11RenderTargetView* m_pRenderTargetView = NULL;
		ID3D11DepthStencilView* m_pDepthStencilView = NULL;
		ID3D11Texture2D* m_pDepthStencil = NULL;

		ID3D11InputLayout* m_pVertexLayout = NULL;
		ID3D11VertexShader* m_pVertexShader = NULL;
		ID3D11PixelShader* m_pPixelShader = NULL;
		ID3D11Buffer* m_pConstantBuffer = NULL;

		// 共有リソース（今回は単純化のため1つのモデルバッファを持つ）
		ID3D11Buffer* m_pVertexBuffer = NULL;

		// 行列保存用
		XMFLOAT4X4 m_mView;
		XMFLOAT4X4 m_mProj;
	};
}