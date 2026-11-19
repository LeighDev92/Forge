
#pragma once
#include "framework.h"
#include "Texture.h"

namespace Forge
{
	/**
	 * @brief Represents the visual properties of a mesh material.
	 *
	 * Stores material color, diffuse texture, displacement map,
	 * displacement strength, and lighting configuration.
	 * Materials define the surface properties used when rendering
	 * a mesh.
	 */
	class FORGE_API Material
	{
	public:

		/**
		 * @brief Constructs a Material with the specified lighting configuration.
		 *
		 * @param is_lit Determines whether lighting calculations
		 * should be applied to the material. Defaults to true.
		 */
		Material(bool is_lit = true);

		/**
		 * @brief Retrieves the material's diffuse color.
		 *
		 * @return Current diffuse color.
		 */
		FColor GetDiffuseColor() const;

		/**
		 * @brief Retrieves the material's primary texture.
		 *
		 * @return Constant reference to the main texture.
		 */
		const Texture& GetMainTexture() const;

		/**
		 * @brief Retrieves the material's displacement map.
		 *
		 * @return Constant reference to the displacement map.
		 */
		const Texture& GetDisplacementMap() const;

		/**
		 * @brief Retrieves the displacement strength.
		 *
		 * @return Current displacement strength.
		 */
		float GetDisplacementStrength() const;

		/**
		 * @brief Determines whether lighting is enabled for the material.
		 *
		 * @return True if lighting is enabled; otherwise, false.
		 */
		bool IsLit() const;

		/**
		 * @brief Sets the material's diffuse color.
		 *
		 * @param color New diffuse color.
		 */
		void SetDiffuseColor(const FColor& color);

		/**
		 * @brief Sets the material's primary texture.
		 *
		 * @param tex Texture to assign as the main texture.
		 */
		void SetMainTexture(const Texture& tex);

		/**
		 * @brief Sets the material's displacement map.
		 *
		 * @param tex Texture to use as the displacement map.
		 */
		void SetDisplacementMap(const Texture& tex);

		/**
		 * @brief Sets the displacement strength.
		 *
		 * Controls the strength of the displacement effect
		 * applied when rendering the material.
		 *
		 * @param strength New displacement strength.
		 */
		void SetDisplacementStrength(float strength);

	private:
		FColor diffuseColor;
		Texture texture;
		Texture displacementMap;
		float displacementStrength;
		bool lit;
	};
}
