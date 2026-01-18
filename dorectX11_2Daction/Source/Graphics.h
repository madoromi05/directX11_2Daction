#pragma once
#include <windows.h>
#include "Window.h"
#include <d3d11.h>
#include <DirectXMath.h> 
#include <d3dcompiler.h>
#include <mmsystem.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
};

struct SimpleShaderConstantBuffer
{
	XMFLOAT4X4 mWVP;
	XMFLOAT4 vColor;
};

class Graphics : public Window
{
public:
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitPipeline();
	void Render();
	void DestroyD3D();

	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext = NULL;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D* m_pDepthStencil = NULL;

	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	// モデルごと	
	ID3D11Buffer* m_pVertexBuffer;
};