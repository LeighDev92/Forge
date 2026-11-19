
#pragma once
#include "framework.h"

namespace Forge
{
	/**
	 * @brief Manages a texture resource used by the graphics renderer.
	 *
	 * Provides functionality for loading image textures from local
	 * files or remote URLs and accessing the resulting Direct3D 11
	 * shader resource view.
	 */
	class FORGE_API Texture
	{
	public:

		/**
		 * @brief Constructs a Texture instance.
		 *
		 * Optionally loads a texture from the specified image file.
		 *
		 * @param filename Path to the image file to load.
		 * Defaults to an empty string.
		 */
		Texture(std::string filename = "");

		/**
		 * @brief Loads a texture from a local image file.
		 *
		 * @param filename Path to the image file to load.
		 *
		 * @return True if the texture was successfully loaded;
		 * otherwise, false.
		 */
		bool LoadTextureFromFile(std::string filename);

		/**
		 * @brief Loads a texture from a remote URL.
		 *
		 * @param url URL identifying the image resource to load.
		 *
		 * @return True if the texture was successfully loaded;
		 * otherwise, false.
		 */
		bool LoadImageFromURL(std::string url);

		/**
		 * @brief Determines whether the texture has been successfully loaded.
		 *
		 * @return True if the texture is valid; otherwise, false.
		 */
		bool IsValidTexture() const;

		/**
		 * @brief Retrieves the Direct3D 11 shader resource view.
		 *
		 * Provides access to the texture resource for binding
		 * to the graphics pipeline.
		 *
		 * @return Constant reference to the texture's shader resource view.
		 */
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>&
			GetShaderResource() const;

	private:
		bool loaded;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureView;
	};
}
