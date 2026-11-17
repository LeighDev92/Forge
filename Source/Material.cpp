#include "Material.h"
#include "Texture.h"

namespace Forge
{
	Material::Material(bool is_lit)
	{
		displacementStrength = 0.f;
		diffuseColor = Color::White;
		lit = is_lit;
	}

	FColor Material::GetDiffuseColor() const
	{
		return diffuseColor;
	}

	const Texture& Material::GetMainTexture() const
	{
		return texture;
	}
	const Texture& Material::GetDisplacementMap() const
	{
		return displacementMap;
	}

	float Material::GetDisplacementStrength() const
	{
		return displacementStrength;
	}

	bool Material::IsLit() const
	{
		return lit;
	}

	void Material::SetDiffuseColor(const FColor& color)
	{
		diffuseColor = color;
	}

	void Material::SetMainTexture(const Texture& tex)
	{
		texture = tex;
	}

	void Material::SetDisplacementMap(const Texture& tex)
	{
		displacementMap = tex;
	}

	void Material::SetDisplacementStrength(float strength)
	{
		displacementStrength = strength;
	}
}
