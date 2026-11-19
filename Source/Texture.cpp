#include "Texture.h"
#include "Application.h"
#include "GraphicsManager.h"

#include <winhttp.h>
#pragma comment(lib, "Winhttp.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <comdef.h>

namespace Forge
{
	Texture::Texture(std::string filename)
	{
		if (!filename.empty())
		{
			loaded = LoadTextureFromFile(filename);
		}
		else
		{
			loaded = false;
		}
	}

	bool Texture::LoadTextureFromFile(std::string filename)
	{
		loaded = Application::Get()->GetGraphicsManager()->CreateTextureFromFile(_textureView, filename.c_str());
		return loaded;
	}

	bool Texture::LoadImageFromURL(std::string url)
	{
		// Download image bytes
        std::vector<uint8_t> data;

        URL_COMPONENTS components = {};
        components.dwStructSize = sizeof(components);

        components.dwSchemeLength = static_cast<DWORD>(-1);
        components.dwHostNameLength = static_cast<DWORD>(-1);
        components.dwUrlPathLength = static_cast<DWORD>(-1);
        components.dwExtraInfoLength = static_cast<DWORD>(-1);

        std::wstring w_url = ToWideString(url);

        if (!WinHttpCrackUrl(w_url.c_str(), 0, 0, &components))
        {
            return false;
        }

        std::wstring host(
            components.lpszHostName,
            components.dwHostNameLength
        );

        std::wstring path(
            components.lpszUrlPath,
            components.dwUrlPathLength
        );

        if (components.dwExtraInfoLength > 0)
        {
            path.append(
                components.lpszExtraInfo,
                components.dwExtraInfoLength
            );
        }

        INTERNET_PORT port = components.nPort;

        DWORD flags = components.nScheme == INTERNET_SCHEME_HTTPS
            ? WINHTTP_FLAG_SECURE
            : 0;

        HINTERNET session = WinHttpOpen(
            L"Forge",
            WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0
        );

        if (!session)
        {
            return false;
        }


        HINTERNET connection = WinHttpConnect(
            session,
            host.c_str(),
            port,
            0
        );

        if (!connection)
        {
            WinHttpCloseHandle(session);
            return false;
        }

        HINTERNET request = WinHttpOpenRequest(
            connection,
            L"GET",
            path.c_str(),
            nullptr,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            flags
        );

        if (!request)
        {
            WinHttpCloseHandle(connection);
            WinHttpCloseHandle(session);
            return false;
        }

        bool success = false;

        if (WinHttpSendRequest(
            request,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0,
            WINHTTP_NO_REQUEST_DATA,
            0,
            0,
            0))
        {
            if (WinHttpReceiveResponse(request, nullptr))
            {
                DWORD statusCode = 0;
                DWORD statusSize = sizeof(statusCode);

                if (WinHttpQueryHeaders(
                    request,
                    WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                    WINHTTP_HEADER_NAME_BY_INDEX,
                    &statusCode,
                    &statusSize,
                    WINHTTP_NO_HEADER_INDEX))
                {
                    if (statusCode == 200)
                    {
                        DWORD bytesAvailable = 0;

                        while (WinHttpQueryDataAvailable(
                            request,
                            &bytesAvailable) && bytesAvailable > 0)
                        {
                            size_t oldSize = data.size();

                            data.resize(oldSize + bytesAvailable);

                            DWORD bytesRead = 0;

                            if (!WinHttpReadData(
                                request,
                                data.data() + oldSize,
                                bytesAvailable,
                                &bytesRead))
                            {
                                data.clear();
                                break;
                            }

                            data.resize(oldSize + bytesRead);
                        }

                        success = !data.empty();
                    }
                }
            }
        }

        WinHttpCloseHandle(request);
        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        if (!success)
        {
            OutputDebugStringA("Failed to load image from url");
            return false;
        }

		// load pixels from image bytes
		int width = 0;
		int height = 0;
		int channels = 0;

		unsigned char* pixels = stbi_load_from_memory(
			data.data(),
			static_cast<int>(data.size()),
			&width,
			&height,
			&channels,
			STBI_rgb_alpha
		);

		if (!pixels)
		{
			OutputDebugStringA((std::string("Failed to decode image: ") + stbi_failure_reason()).c_str());
			return false;
		}

		// create D3D11 image texture
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = pixels;
		initialData.SysMemPitch = width * 4;
		initialData.SysMemSlicePitch = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		const Microsoft::WRL::ComPtr<ID3D11Device> device = Application::Get()->GetGraphicsManager()->GetDevice();
		HRESULT result = device->CreateTexture2D(&textureDesc, &initialData, texture.GetAddressOf());

		stbi_image_free(pixels);

		if (FAILED(result))
		{
            OutputDebugStringA("Failed to create d3d11 texture");
            OutputDebugString(_com_error(result).ErrorMessage());
			return false;
		}

		// create the shader resource view
		result = device->CreateShaderResourceView(texture.Get(), nullptr, _textureView.GetAddressOf());

		if (FAILED(result))
		{
			OutputDebugStringA("Failed to create d3d11 shader resource view from texture");
            OutputDebugString(_com_error(result).ErrorMessage());
			return false;
		}

		loaded = true;
		return true;
	}

	bool Texture::IsValidTexture() const
	{
		return loaded;
	}

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Texture::GetShaderResource() const
	{
		return _textureView;
	}
}
