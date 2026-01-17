#pragma once
#include <windows.h>
#include "Window.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

class Graphics : public Window
{
public:
	HRESULT InitD3D();
	void DestroyD3D();
	void Render();

	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext = NULL;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D* m_pDepthStencil = NULL;
};