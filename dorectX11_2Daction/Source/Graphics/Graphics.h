#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "ShaderCode/VertexShader.h"
#include "ShaderCode/PixelShader.h"
#include "ShaderCode/ConstantBuffer.h"
#include "ShaderCode/SimpleShaderConstantBuffer.h"
#include "Graphics/FbxLoad/FbxModel.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace engine
{
	// D3D11描画パイプライン全体のファサード。
	// Renderer・シェーダー・定数バッファを束ね、
	// Game層に描画インターフェースを提供する。
	// InitD3D → InitPipeline の順で初期化し、
	// 毎フレーム BeginRender → Draw → EndRender の順で使用する。
	class Graphics
	{
	public:
		Graphics();
		~Graphics() = default;

		HRESULT InitD3D( HWND hWnd, int width, int height );
		HRESULT InitPipeline();

		void BeginRender();
		void EndRender();

		// View行列を内部に保持する。毎フレームRender開始時に呼ぶ。
		void SetCamera( FXMVECTOR eye, FXMVECTOR focus, FXMVECTOR up );

		// Projection行列を内部に保持する。毎フレームRender開始時に呼ぶ。
		void SetProjection( float fovY, float aspect, float nearZ, float farZ );

		// WVP合成 → 定数バッファ更新 → メッシュ描画を行う。
		void Draw( Mesh* pMesh, CXMMATRIX world, const XMFLOAT4& color );
        void Draw( const engine::FbxModel& model );
		ID3D11Device* GetDevice() const;

	private:
		Renderer                          m_renderer;
		VertexShader                      m_vertexShader;
		PixelShader                       m_pixelShader;
		ConstantBuffer<SimpleShaderConstantBuffer> m_constantBuffer;

		XMMATRIX m_mView;
		XMMATRIX m_mProj;

		static constexpr float kClearR = 0.0f;
		static constexpr float kClearG = 0.0f;
		static constexpr float kClearB = 1.0f;
		static constexpr float kClearA = 1.0f;
	};
}