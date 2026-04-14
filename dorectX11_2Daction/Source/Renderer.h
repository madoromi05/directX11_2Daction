#pragma once
#include <d3d11.h>

// DirectX11の初期化と毎フレームの描画管理を担う。
// アプリケーション起動時にInitを呼び、
// 毎フレームBeginFrame→描画処理→EndFrameの順で使用する。
namespace engine {
	class Renderer {
	public:
		Renderer();
		~Renderer();
		HRESULT Init( HWND hwnd, int width, int height );

		// レンダーターゲットを指定色でクリアする
		void BeginFrame( float r, float g, float b, float a );

		// バックバッファをフロントバッファに反映する。
		void EndFrame();

		ID3D11Device* GetDevice()  const;
		ID3D11DeviceContext* GetContext() const;

	private:
		ID3D11Device* m_pDevice;
		IDXGISwapChain* m_pSwapChain;
		ID3D11DeviceContext* m_pContext;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
	};
}