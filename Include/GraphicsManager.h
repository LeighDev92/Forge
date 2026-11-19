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

		/**
		 * @brief Initializes the DirectX 11 graphics manager.
		 *
		 * Creates the Direct3D device, device context, swap chain, render
		 * targets, shaders, input layouts, rasterizer states, depth-stencil
		 * states, samplers, and constant buffers.
		 *
		 * @param hWnd Handle to the window associated with the graphics device.
		 */
		GraphicsManager(HWND hWnd);

		/**
		 * @brief Copy construction is disabled.
		 */
		GraphicsManager(const GraphicsManager&) = delete;

		/**
		 * @brief Copy assignment is disabled.
		 */
		GraphicsManager& operator=(const GraphicsManager&) = delete;

		/**
		 * @brief Releases graphics resources and associated Direct3D objects.
		 */
		~GraphicsManager();

		/**
		 * @brief Resizes the graphics viewport and associated render targets.
		 *
		 * @param width New viewport width in pixels.
		 * @param height New viewport height in pixels.
		 */
		void Resize(UINT width, UINT height);

		/**
		 * @brief Renders a mesh using the configured graphics pipeline.
		 *
		 * Supports solid rendering, wireframe rendering, or both.
		 *
		 * @param mesh Pointer to the mesh to render.
		 * @param draw_solid Determines whether solid rendering is enabled.
		 * @param draw_wireframe Determines whether wireframe rendering is enabled.
		 */
		void DrawMesh(Mesh* mesh, bool draw_solid = true, bool draw_wireframe = false);

		/**
		 * @brief Sets the active camera used by the graphics manager.
		 *
		 * @param camera Pointer to the camera used for rendering and
		 * world-to-screen coordinate calculations.
		 */
		void SetCamera(Camera* camera);

		/**
		 * @brief Converts a world-space position into normalized clip-space
		 * coordinates.
		 *
		 * Applies the supplied transform and camera matrices to determine
		 * the position's corresponding screen-space location.
		 *
		 * @param world World-space position to convert.
		 * @param transform Transform applied to the world-space position.
		 *
		 * @return Normalized clip-space X and Y coordinates.
		 */
		FVector2D ConvertToClipPosition(FVector world, FTransform transform);

		/**
		 * @brief Determines whether a position on a transformed sphere is
		 * visible to the active camera.
		 *
		 * @param position Position on the sphere's surface.
		 * @param transform Transform applied to the sphere.
		 *
		 * @return True if the position is facing the camera; otherwise,
		 * false.
		 */
		bool IsPositionOnSphereVisibleToCamera(FVector position, FTransform transform) const;

		/**
		 * @brief Presents the rendered frame to the display.
		 *
		 * Swaps the back buffer with the front buffer through the swap chain.
		 */
		void PresentFrame();

		/**
		 * @brief Clears the screen using the specified RGB color.
		 *
		 * @param red Red color component.
		 * @param green Green color component.
		 * @param blue Blue color component.
		 */
		void ClearScreen(float red, float green, float blue) noexcept;

		/**
		 * @brief Clears the active render target view.
		 *
		 * @param red Red color component.
		 * @param green Green color component.
		 * @param blue Blue color component.
		 */
		void ClearRenderTargetView(float red, float green, float blue) noexcept;

		/**
		 * @brief Clears the depth-stencil buffer.
		 *
		 * Resets the depth and stencil information associated with
		 * the active depth-stencil view.
		 */
		void ClearDepthStencilView() noexcept;

		/**
		 * @brief Retrieves the Direct3D 11 device.
		 *
		 * @return Smart pointer to the Direct3D device.
		 */
		const ComPtr<ID3D11Device> GetDevice() const;

		/**
		 * @brief Retrieves the Direct3D 11 device context.
		 *
		 * @return Smart pointer to the Direct3D device context.
		 */
		const ComPtr<ID3D11DeviceContext> GetDeviceContext() const;

		/**
		 * @brief Creates a shader resource view from an image file.
		 *
		 * @param texture Reference to the shader resource view that will
		 * receive the created texture.
		 * @param filename Path to the image file to load.
		 *
		 * @return True if the texture was successfully created;
		 * otherwise, false.
		 */
		bool CreateTextureFromFile(ComPtr<ID3D11ShaderResourceView>& texture, const char* filename);

	private:

		/**
		 * @brief Creates the Direct3D device, device context, and swap chain.
		 *
		 * @param hWnd Handle to the window associated with the graphics device.
		 */
		void CreateDeviceAndSwapChain(HWND hWnd);

		/**
		 * @brief Creates the render target and depth-stencil resources.
		 */
		void CreateRenderTargets();

		/**
		 * @brief Creates and initializes the graphics shaders.
		 */
		void CreateShaders();

		/**
		 * @brief Creates the input layouts used by the graphics pipeline.
		 */
		void CreateInputLayouts();

		/**
		 * @brief Creates the rasterizer states used for solid and wireframe rendering.
		 */
		void CreateRasterizerStates();

		/**
		 * @brief Creates the depth-stencil states used during rendering.
		 */
		void CreateDepthStencilStates();

		/**
		 * @brief Creates the sampler states used for texture sampling.
		 */
		void CreateSamplers();

		/**
		 * @brief Creates the constant buffers used to pass rendering data
		 * to the shaders.
		 */
		void CreateConstantBuffers();


#ifdef _DEBUG
		ComPtr<ID3D11Debug> _debug;
#endif
		ComPtr<ID3D11Device> _device;
		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11DeviceContext> _deviceContext;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;
		ComPtr<ID3D11DepthStencilState> _depthStencilState;
		ComPtr<ID3D11VertexShader> _vertexShader;
		ComPtr<ID3D11PixelShader> _pixelShader;
		ComPtr<ID3D11PixelShader> _wireframePixelShader;
		ComPtr<ID3D11InputLayout> _inputLayout;
		ComPtr<ID3D11RasterizerState> _solidRasterizerState;
		ComPtr<ID3D11RasterizerState> _wireframeRasterizerState;
		ComPtr<ID3D11SamplerState> _textureSamplerState;
		ComPtr<ID3D11Buffer> _modelConstantBuffer;
		ComPtr<ID3D11Buffer> _materialConstantBuffer;
		ComPtr<ID3D11Buffer> _lightingConstantBuffer;

		Camera* _camera;
	};
}