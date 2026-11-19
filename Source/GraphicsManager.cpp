#include "GraphicsManager.h"
#include "Application.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "Texture.h"
#include <comdef.h>
#include <d3dcompiler.h>
#include "External/stb_image.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX;

#define MAX_POINT_LIGHTS 8
#define MAX_MATERIALS 8

namespace Forge
{
	struct ModelConstantBuffer
	{
		XMMATRIX model;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct FPointLight
	{
		FVector position;
		float padding = 0.f;

		FVector color;
		float intensity;

		float radius;
		float padding2[3] = {};

		FPointLight()
		{
			position = FVector();

			color = { 1.f, 1.f, 1.f };

			intensity = 1.f;
			radius = 100.f;
		}

		FPointLight(FVector position, FVector color, float intensity, float radius)
		{
			this->position = position;
			this->color = color;
			this->intensity = intensity;
			this->radius = radius;
		}
	};

	struct LightingConstantBuffer
	{
		FVector camera_position = FVector();
		float padding = 0.f;

		FPointLight point_lights[MAX_POINT_LIGHTS];

		int num_point_lights = 0.f;
		float padding2[3] = {};

		LightingConstantBuffer()
		{

		}
	};

	struct MaterialConstantBuffer
	{
		FColor diffuseColor = Color::White;
		int hasTexture = false;
		int hasDisplacement = false;
		float displacementStrength = 0.f;
		int isLit = true;

		MaterialConstantBuffer()
		{

		}
	};

	XMMATRIX GetModelMatrix(FTransform transform)
	{
		XMVECTOR translation = XMVectorSet(
			transform.Position.X,
			transform.Position.Z,
			transform.Position.Y, 0.0f);

		XMVECTOR scale = XMVectorSet(
			transform.Scale.X,
			transform.Scale.Z,
			transform.Scale.Y, 0.0f);

		XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(-transform.Rotation.Pitch),
			XMConvertToRadians(-transform.Rotation.Yaw),
			XMConvertToRadians(-transform.Rotation.Roll)
		);

		return XMMatrixAffineTransformation(scale, XMVectorZero(), rotation, translation);
	}

	XMMATRIX GetViewMatrix(FVector cameraPosition, FVector cameraForwardVector, FVector cameraUpVector)
	{
		XMVECTOR eyePosition = XMVectorSet(cameraPosition.X, cameraPosition.Z, cameraPosition.Y, 1.0f);  // Camera position

		FVector position = cameraPosition + cameraForwardVector * 10.f;
		XMVECTOR focusPosition = XMVectorSet(position.X, position.Z, position.Y, 1.0f);  // Look-at point (world center)
		XMVECTOR upDirection = XMVectorSet(cameraUpVector.X, cameraUpVector.Z, cameraUpVector.Y, 0.0f);    // Up axis (+Y)

		return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	}

	XMMATRIX GetProjectionMatrix(float fov, float nearZ, float farZ, bool orthographic)
	{
		RECT rect = Application::Get()->GetMainWindowViewport();
		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		float aspectRatio = static_cast<FLOAT>(width) / static_cast<FLOAT>(height);

		XMMATRIX projectionMatrix;
		if (orthographic)
		{
			projectionMatrix = XMMatrixOrthographicLH(2.0f, 2.0f, nearZ, farZ);
		}
		else
		{
			projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
		}

		return projectionMatrix;
	}

	template <typename T>
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext> pDeviceContext, ComPtr<ID3D11Buffer> pConstantBuffer, T data)
	{
		D3D11_MAPPED_SUBRESOURCE mapped = {};

		HRESULT result = pDeviceContext->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

		if (SUCCEEDED(result))
		{
			memcpy(mapped.pData, &data, sizeof(data));

			pDeviceContext->Unmap(pConstantBuffer.Get(), 0);
		}
	}

	GraphicsManager::GraphicsManager(HWND hWnd)
	{
		CreateDeviceAndSwapChain(hWnd);
		CreateRenderTargets();
		CreateShaders();
		CreateInputLayouts();
		CreateRasterizerStates();
		CreateDepthStencilStates();
		CreateSamplers();
		CreateConstantBuffers();
	}
		
	GraphicsManager::~GraphicsManager()
	{

#ifdef _DEBUG
		if (_debug)
		{
			_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}
#endif

	}

	void GraphicsManager::Resize(UINT width, UINT height)
	{
		if (!_swapChain || !_device || !_deviceContext)
			return;

		// Release anything referencing the old back buffer
		_renderTargetView.Reset();
		_depthStencilView.Reset();

		_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		HRESULT result = _swapChain->ResizeBuffers(0,width,height,DXGI_FORMAT_UNKNOWN,0);

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to resize swap chain buffers");
		}

		ComPtr<ID3D11Texture2D> backBuffer;

		result = _swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to get a handle on the back buffer");
		}

		result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, &_renderTargetView);

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create render target view");
		}

		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;

		result = _device->CreateTexture2D(&depthDesc, nullptr, depthBuffer.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create depth buffer");
		}

		result = _device->CreateDepthStencilView(depthBuffer.Get(), nullptr, _depthStencilView.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create depth stencil view");
		}

		// Rebind the new render target and depth buffer.
		_deviceContext->OMSetRenderTargets(1u, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

		// Update the viewport.
		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		_deviceContext->RSSetViewports(1, &viewport);
	}

	void GraphicsManager::DrawMesh(Mesh* mesh, bool draw_solid, bool draw_wireframe)
	{
		if (!_deviceContext || !_device || !mesh)
		{
			return;
		}

		// Gather Mesh Data
		const std::vector<Vertex>& vertices = mesh->GetVertices();
		const std::vector<unsigned int>& indices = mesh->GetIndices();
		const Material& material = mesh->GetMaterial();
		const Texture& main_texture = material.GetMainTexture();
		const Texture& displacement_map = material.GetDisplacementMap();
		FTransform transform = mesh->GetTransform();

		ComPtr<ID3D11Buffer> pVertexBuffer = mesh->GetVertexBuffer();
		ComPtr<ID3D11Buffer> pIndexBuffer = mesh->GetIndexBuffer();

		const unsigned int num_vertices = vertices.size();
		const unsigned int num_indices = indices.size();

		const FVector cameraPosition = _camera != nullptr ? _camera->GetPosition() : FVector(0.f, -10.f, 0.f);
		const FVector cameraForwardVector = _camera != nullptr ? _camera->GetForwardVector() : FVector(0.f, 1.f, 0.f);
		const FVector cameraUpVector = _camera != nullptr ? _camera->GetUpVector() : FVector(0.f, 0.f, 1.f);

		const FVector cameraPosition_dx11 = { cameraPosition.X, cameraPosition.Z, cameraPosition.Y };

		// Update Constant Buffers
		ModelConstantBuffer model_constant_buffer;
		model_constant_buffer.model = XMMatrixTranspose(GetModelMatrix(mesh->GetTransform()));
		model_constant_buffer.view = XMMatrixTranspose(GetViewMatrix(cameraPosition, cameraForwardVector, cameraUpVector));
		model_constant_buffer.projection = XMMatrixTranspose(GetProjectionMatrix(XM_PIDIV4, 0.1f, 1000.f, false));
		UpdateConstantBuffer<ModelConstantBuffer>(_deviceContext, _modelConstantBuffer, model_constant_buffer);

		LightingConstantBuffer lighting_constant_buffer;
		lighting_constant_buffer.num_point_lights = 1;
		lighting_constant_buffer.point_lights[0].color = {1.f, 1.f, 1.f};
		lighting_constant_buffer.point_lights[0].intensity = 1.0f;
		lighting_constant_buffer.point_lights[0].position = cameraPosition_dx11;
		lighting_constant_buffer.point_lights[0].radius = 1000.f;
		lighting_constant_buffer.camera_position = { 0.f, 0.f, -10.f };
		UpdateConstantBuffer<LightingConstantBuffer>(_deviceContext, _lightingConstantBuffer, lighting_constant_buffer);

		MaterialConstantBuffer material_constant_buffer;
		material_constant_buffer.diffuseColor = material.GetDiffuseColor();
		material_constant_buffer.hasTexture = main_texture.IsValidTexture();
		material_constant_buffer.hasDisplacement = displacement_map.IsValidTexture();
		material_constant_buffer.displacementStrength = material.GetDisplacementStrength();
		material_constant_buffer.isLit = material.IsLit();
		UpdateConstantBuffer<MaterialConstantBuffer>(_deviceContext, _materialConstantBuffer, material_constant_buffer);

		// Bind Data
		const UINT start_slot = 0u;
		const UINT num_buffers = 1u;
		const UINT strides[] = { sizeof(Vertex) };
		const UINT offsets[] = { 0u };
		ID3D11Buffer* vertex_buffers[] = { pVertexBuffer.Get() };
		ID3D11Buffer* constant_buffers[] = { _modelConstantBuffer.Get(), _materialConstantBuffer.Get(), _lightingConstantBuffer.Get() };
		ID3D11RenderTargetView* render_target_views[] = { _renderTargetView.Get() };
		ID3D11ShaderResourceView* shader_resources[] = { main_texture.GetShaderResource().Get(), displacement_map.GetShaderResource().Get() };
		ID3D11SamplerState* sampler_states[] = { _textureSamplerState.Get() };

		_deviceContext->IASetVertexBuffers(0, 1, vertex_buffers, strides, offsets);
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_deviceContext->IASetInputLayout(_inputLayout.Get());

		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0u);
		_deviceContext->VSSetShaderResources(1u, 1u, &shader_resources[1]);
		_deviceContext->VSSetSamplers(0u, 1u, sampler_states);
		_deviceContext->VSSetConstantBuffers(0u, 2u, &constant_buffers[0]);

		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0u);
		_deviceContext->PSSetShaderResources(0u, 1u, shader_resources);
		_deviceContext->PSSetSamplers(0u, 1u, sampler_states);
		_deviceContext->PSSetConstantBuffers(1u, 2u, &constant_buffers[1]);

		_deviceContext->OMSetRenderTargets(1u, render_target_views, _depthStencilView.Get());
		_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 0);

		_deviceContext->RSSetState(_solidRasterizerState.Get());

		// Draw
		if (num_indices && draw_solid)
		{
			_deviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
			_deviceContext->DrawIndexed(num_indices, 0u, 0u);
		}
		else if(draw_solid)
		{
			_deviceContext->Draw(num_vertices, 0u);
		}

		if (draw_wireframe)
		{
			_deviceContext->RSSetState(_wireframeRasterizerState.Get());
			_deviceContext->PSSetShader(_wireframePixelShader.Get(), nullptr, 0);

			if (num_indices)
				_deviceContext->DrawIndexed(num_indices, 0, 0);
			else
				_deviceContext->Draw(num_vertices, 0);

			_deviceContext->RSSetState(_solidRasterizerState.Get());
		}
	}

	void GraphicsManager::SetCamera(Camera* camera)
	{
		if (camera != nullptr)
		{
			_camera = camera;
		}
	}

	FVector2D GraphicsManager::ConvertToClipPosition(FVector local, FTransform transform)
	{
		const FVector cameraPosition = _camera != nullptr ? _camera->GetPosition() : FVector(0.f, -10.f, 0.f);
		const FVector cameraForwardVector = _camera != nullptr ? _camera->GetForwardVector() : FVector(0.f, 1.f, 0.f);
		const FVector cameraUpVector = _camera != nullptr ? _camera->GetUpVector() : FVector(0.f, 0.f, 1.f);

		XMMATRIX model = GetModelMatrix(transform);
		XMMATRIX view = GetViewMatrix(cameraPosition, cameraForwardVector, cameraUpVector);
		XMMATRIX projection = GetProjectionMatrix(XM_PIDIV4, 0.1f, 1000.f, false);
		XMVECTOR world_position = XMVector3TransformCoord(XMVectorSet(local.X, local.Y, local.Z, 1.f), model);
		XMVECTOR clip_position = XMVector4Transform(world_position, XMMatrixMultiply(view, projection));

		float clip_x = XMVectorGetX(clip_position);
		float clip_y = XMVectorGetY(clip_position);
		float clip_w = XMVectorGetW(clip_position);

		if (clip_w <= 0.0f)
			return {};

		float ndcX = clip_x / clip_w;
		float ndcY = clip_y / clip_w;

		D3D11_VIEWPORT viewport;
		UINT num_viewports = 1u;
		_deviceContext->RSGetViewports(
			&num_viewports,
			&viewport
		);

		float screenX =
			viewport.TopLeftX +
			(ndcX + 1.0f) * 0.5f * viewport.Width;

		float screenY =
			viewport.TopLeftY +
			(1.0f - ndcY) * 0.5f * viewport.Height;

		XMVECTOR view_position = XMVector4Transform(
			world_position, view
		);

		return { screenX, screenY };
	}

	bool GraphicsManager::IsPositionOnSphereVisibleToCamera(FVector position, FTransform transform) const
	{
		FVector cameraPosition = _camera->GetPosition();
		FTransform moonTransform = transform;

		XMVECTOR localPosition = XMVectorSet(
			position.X, position.Y, position.Z, 1.0f
		);

		XMVECTOR worldPosition =
			XMVector3TransformCoord(localPosition, GetModelMatrix(transform));

		worldPosition = XMVectorSet(
			XMVectorGetX(worldPosition), 
			XMVectorGetZ(worldPosition), 
			XMVectorGetY(worldPosition), 
			XMVectorGetW(worldPosition));

		// Moon center in renderer coordinates
		XMVECTOR moonCenter = XMVectorSet(
			transform.Position.X,
			transform.Position.Y,
			transform.Position.Z,
			1.0f
		);

		// Camera position
		XMVECTOR camera = XMVectorSet(
			cameraPosition.X,
			cameraPosition.Y,
			cameraPosition.Z,
			1.0f
		);

		// Surface normal
		XMVECTOR normal = XMVector3Normalize(
			XMVectorSubtract(worldPosition, moonCenter)
		);

		// Direction toward camera
		XMVECTOR toCamera = XMVector3Normalize(
			XMVectorSubtract(camera, worldPosition)
		);

		float visibility = XMVectorGetX(
			XMVector3Dot(normal, toCamera)
		);

		return visibility > 0.0f;
	}

	void GraphicsManager::PresentFrame()
	{
		if (_swapChain)
		{
			UINT sync_interval = 1u;					// target 60FPS
			UINT flags = 0u;							
			_swapChain->Present(sync_interval, flags);
		}
	}

	void GraphicsManager::ClearScreen(float red, float green, float blue) noexcept
	{
		ClearRenderTargetView(red, green, blue);
		ClearDepthStencilView();
	}

	void GraphicsManager::ClearRenderTargetView(float red, float green, float blue) noexcept
	{
		if (!_deviceContext || !_renderTargetView)
			return;

		const float color[] = { red, green, blue, 1.0f };
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
	}

	void GraphicsManager::ClearDepthStencilView() noexcept
	{
		if (!_deviceContext || !_depthStencilView)
			return;

		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
	}

	const ComPtr<ID3D11Device> GraphicsManager::GetDevice() const
	{
		return _device;
	}

	const ComPtr<ID3D11DeviceContext> GraphicsManager::GetDeviceContext() const
	{
		return _deviceContext;
	}

	bool GraphicsManager::CreateTextureFromFile(ComPtr<ID3D11ShaderResourceView>& texture, const char* filename)
	{
		int width, height, originalChannels;

		// load image data
		unsigned char* imageData = stbi_load(
			filename,
			&width,
			&height,
			&originalChannels,
			STBI_rgb_alpha
		);

		if (!imageData)
		{
			return false;
		}

		// create the RGBA texture
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA textureData = {};
		textureData.pSysMem = imageData;
		textureData.SysMemPitch = width * 4;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;

		HRESULT result = _device->CreateTexture2D(
			&textureDesc,
			&textureData,
			texture2d.GetAddressOf()
		);

		if (FAILED(result))
		{
			stbi_image_free(imageData);
			return false;
		}

		// Create the shader resource view
		result = _device->CreateShaderResourceView(
			texture2d.Get(),
			nullptr,
			texture.GetAddressOf()
		);

		if (FAILED(result))
		{
			stbi_image_free(imageData);
			return false;
		}

		stbi_image_free(imageData);
		return true;
	}
	
	void GraphicsManager::CreateDeviceAndSwapChain(HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
		swap_chain_desc.BufferDesc.Width = 0;
		swap_chain_desc.BufferDesc.Height = 0;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 0;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.Flags = 0;
		swap_chain_desc.OutputWindow = hWnd;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc.Windowed = TRUE;

		UINT create_device_flags = 0;

#ifdef _DEBUG
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			create_device_flags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swap_chain_desc,
			&_swapChain,
			&_device,
			nullptr,
			&_deviceContext
		);

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create device and swap chain");
		}

#ifdef _DEBUG
		_device.As(&_debug);
#endif
	}
	
	void GraphicsManager::CreateRenderTargets()
	{
		if (_swapChain && _device)
		{
			ID3D11Resource* pBackBuffer = nullptr;
			HRESULT result = _swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

			if (pBackBuffer)
			{
				result = _device->CreateRenderTargetView(pBackBuffer, nullptr, &_renderTargetView);
				pBackBuffer->Release();
			}
		}
	}
	
	void GraphicsManager::CreateShaders()
	{
		ComPtr<ID3DBlob> pBlob;

		// Load Vertex Shader
		HRESULT result = D3DReadFileToBlob(GetResourcePath("Mesh_VertexShader.cso").wstring().c_str(), &pBlob);
		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] Failed to load Vertex Shader");
		}

		// Create Vertex Shader
		result = _device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &_vertexShader);
		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] Failed to create Vertex Shader");
		}

		// Load Pixel Shader
		result = D3DReadFileToBlob(GetResourcePath("Mesh_PixelShader.cso").wstring().c_str(), &pBlob);
		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawMesh: Failed to load Pixel Shader");
		}

		// Create Pixel Shader
		result = _device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &_pixelShader);
		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawMesh: Failed to create Pixel Shader");
		}

		// Load Wireframe Pixel Shader
		result = D3DReadFileToBlob(GetResourcePath("Mesh_WireframePixelShader.cso").wstring().c_str(), &pBlob);

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawMesh: Failed to load Wireframe Pixel Shader");
		}

		// Create Wireframe Pixel Shader
		result = _device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &_wireframePixelShader);

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawMesh: Failed to create Wireframe Pixel Shader");
		}
	}
	
	void GraphicsManager::CreateInputLayouts()
	{
		ComPtr<ID3DBlob> pBlob;
		HRESULT result = D3DReadFileToBlob(GetResourcePath("Mesh_VertexShader.cso").wstring().c_str(), &pBlob);
		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] Failed to load Vertex Shader");
		}

		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "COLOR", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 32u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};

		UINT num_elements = std::size(input_element_desc);
		
		result = _device->CreateInputLayout(input_element_desc, num_elements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &_inputLayout);
		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawTestMesh: Failed to create Input Layout");
		}
	}
	
	void GraphicsManager::CreateRasterizerStates()
	{
		RECT rect = Application::Get()->GetMainWindowViewport();
		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		D3D11_VIEWPORT viewport;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;

		UINT num_viewports = 1u;
		D3D11_VIEWPORT viewports[] = { viewport };
		_deviceContext->RSSetViewports(num_viewports, viewports);

		D3D11_RASTERIZER_DESC solid_state_desc = {};
		solid_state_desc.CullMode = D3D11_CULL_BACK;
		solid_state_desc.FrontCounterClockwise = FALSE;
		solid_state_desc.DepthClipEnable = TRUE;
		solid_state_desc.FillMode = D3D11_FILL_SOLID;

		HRESULT result = _device->CreateRasterizerState(&solid_state_desc, _solidRasterizerState.GetAddressOf());

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawTestMesh: Failed to create Solid Rasterizer State");
		}

		D3D11_RASTERIZER_DESC wireframe_state_desc = {};
		wireframe_state_desc.FillMode = D3D11_FILL_WIREFRAME;
		wireframe_state_desc.CullMode = D3D11_CULL_BACK;
		wireframe_state_desc.FrontCounterClockwise = FALSE;
		wireframe_state_desc.DepthClipEnable = TRUE;

		result = _device->CreateRasterizerState(&wireframe_state_desc, _wireframeRasterizerState.GetAddressOf());

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawTestMesh: Failed to create Rasterizer State");
		}
	}
	
	void GraphicsManager::CreateDepthStencilStates()
	{
		RECT rect = Application::Get()->GetMainWindowViewport();
		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;

		HRESULT result = _device->CreateTexture2D(&depthDesc, nullptr, pDepthBuffer.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create depth buffer");
		}

		result = _device->CreateDepthStencilView(pDepthBuffer.Get(), nullptr, _depthStencilView.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create depth stencil view");
		}

		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = { };
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

		result = _device->CreateDepthStencilState(&depth_stencil_desc, _depthStencilState.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create depth stencil state");
		}
	}

	void GraphicsManager::CreateSamplers()
	{
		D3D11_SAMPLER_DESC samplerDesc = {};

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = _device->CreateSamplerState(&samplerDesc, _textureSamplerState.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugString(_com_error(result).ErrorMessage());
			throw std::runtime_error("[Graphics Manager] - Failed to create texture sampler state");
		}
	}

	void GraphicsManager::CreateConstantBuffers()
	{
		D3D11_BUFFER_DESC model_buffer_desc = { };
		model_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		model_buffer_desc.ByteWidth = sizeof(ModelConstantBuffer);
		model_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		model_buffer_desc.MiscFlags = 0u;
		model_buffer_desc.StructureByteStride = 0u;
		model_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT result = _device->CreateBuffer(&model_buffer_desc, nullptr, &_modelConstantBuffer);

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] Failed to create Model Constant Buffer");
		}

		D3D11_BUFFER_DESC lighting_buffer_desc = {};

		lighting_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		lighting_buffer_desc.ByteWidth = sizeof(LightingConstantBuffer);
		lighting_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lighting_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result = _device->CreateBuffer(&lighting_buffer_desc, nullptr, &_lightingConstantBuffer);

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] Failed to create Lighting Constant Buffer");
		}

		D3D11_BUFFER_DESC material_buffer_desc = {};

		material_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		material_buffer_desc.ByteWidth = sizeof(MaterialConstantBuffer);
		material_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		material_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result = _device->CreateBuffer(&material_buffer_desc, nullptr, &_materialConstantBuffer);

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] Failed to create Material Constant Buffer");
		}
	}
}