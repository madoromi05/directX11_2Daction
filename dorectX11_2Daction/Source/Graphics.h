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

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
};