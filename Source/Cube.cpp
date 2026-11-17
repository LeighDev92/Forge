#include "Cube.h"

namespace Forge
{
	Cube::Cube(FTransform transform)
	{
		Transform = transform;

		std::vector<FVector> positions = {
			// Front Face
			{-0.5f, -0.5f,  0.5f},
			{0.5f, -0.5f,  0.5f},
			{0.5f,  0.5f,  0.5f},
			{0.5f,  0.5f,  0.5f},
			{ -0.5f,  0.5f,  0.5f},
			{-0.5f, -0.5f,  0.5f},

			// Back Face
			{0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},

			// Left Face
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f,  0.5f},
			{-0.5f,  0.5f,  0.5f},
			{-0.5f,  0.5f,  0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},

			// Right Face
			{0.5f, -0.5f,  0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f,  0.5f},
			{0.5f, -0.5f,  0.5f},

			// Top Face
			{-0.5f,  0.5f,  0.5f},
			{0.5f,  0.5f,  0.5f},
			{0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f,  0.5f},

			// Bottom Face
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f,  0.5f},
			{0.5f, -0.5f,  0.5f},
			{-0.5f, -0.5f,  0.5f},
			{-0.5f, -0.5f, -0.5f}
		};

		std::vector<FColor> colors = {
			// Front Face
			Color::Red,

			// Back Face
			Color::Green,

			// Left Face
			Color::Blue,

			// Right Face
			Color::Yellow,

			// Top Face
			Color::Purple,

			// Bottom Face
			Color::Teal,
		};

		std::vector<FVector> faceNormals = {
			// Front Face
			{ 0.f,  0.f,  1.f},

			// Back Face
			{ 0.f,  0.f, -1.f},

			// Left Face
			{-1.f,  0.f,  0.f},

			// Right Face
			{ 1.f,  0.f,  0.f},

			// Top Face
			{ 0.f,  1.f,  0.f},

			// Bottom Face
			{ 0.f, -1.f,  0.f}
		};

		std::vector<FVector2D> faceUVs = {
			// Front Face
			{0.f, 1.f},

			// Back Face
			{1.f, 1.f},

			// Left Face
			{1.f, 0.f},

			// Right Face
			{1.f, 0.f},

			// Top Face
			{0.f, 0.f},

			// Bottom Face
			{0.f, 1.f}
		};

		for (int face = 0; face < 6; ++face)
		{
			for (int vertex = 0; vertex < 6; ++vertex)
			{
				int index = face * 6 + vertex;

				Vertices.emplace_back(
					positions[index],
					faceNormals[face],
					faceUVs[vertex],
					colors[face]
				);
			}
		}

		CreateVertexBuffer();
	}
}