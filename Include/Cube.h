#pragma once
#include "Mesh.h"

namespace Forge
{
	/** 
	* @brief Mesh implementation representing a cube.
	*/
	class FORGE_API Cube : public Mesh
	{
	public:
		Cube(FTransform transform = FTransform());
	};
}
