#pragma once

#include "framework.h"
using Microsoft::WRL::ComPtr;

namespace Forge
{
	class Mesh;
	class Camera;

	class FORGE_API GraphicsManager
	{
	public:
		GraphicsManager(HWND hWnd);
		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;
		~GraphicsManager();
		
		void Resize(UINT width, UINT height);

		void DrawMesh(Mesh* mesh, bool draw_solid=true, bool draw_wireframe=false);

		void SetCamera(Camera* camera);
		FVector2D ConvertToClipPosition(FVector world, FTransform transform);
		bool IsPositionOnSphereVisibleToCamera(FVector position, FTransform transform) const;

		void PresentFrame();
		void ClearRenderTargetView(float red, float green, float blue) noexcept;

		const ComPtr<ID3D11Device> GetDevice() const;
		const ComPtr<ID3D11DeviceContext> GetDeviceContext() const;

		bool CreateTextureFromFile(ComPtr<ID3D11ShaderResourceView>& texture, const char* filename);

	private:

		void CreateDeviceAndSwapChain(HWND hWnd);
		void CreateRenderTargets();
		void CreateShaders();
		void CreateInputLayouts();
		void CreateRasterizerStates();
		void CreateDepthStencilStates();
		void CreateSamplers();
		void CreateConstantBuffers();

#ifdef _DEBUG
		ComPtr<ID3D11Debug> pDebug;
#endif
		ComPtr<ID3D11Device> pDevice;
		ComPtr<IDXGISwapChain> pSwapChain;
		ComPtr<ID3D11DeviceContext> pDeviceContext;
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		ComPtr<ID3D11DepthStencilState> pDepthStencilState;
		ComPtr<ID3D11VertexShader> pVertexShader;
		ComPtr<ID3D11PixelShader> pPixelShader;
		ComPtr<ID3D11PixelShader> pWireframePixelShader;
		ComPtr<ID3D11InputLayout> pInputLayout;
		ComPtr<ID3D11RasterizerState> pSolidRasterizerState;
		ComPtr<ID3D11RasterizerState> pWireframeRasterizerState;
		ComPtr<ID3D11SamplerState> pTextureSamplerState;
		ComPtr<ID3D11Buffer> pModelConstantBuffer;
		ComPtr<ID3D11Buffer> pMaterialConstantBuffer;
		ComPtr<ID3D11Buffer> pLightingConstantBuffer;

		Camera* pCamera;
	};
}