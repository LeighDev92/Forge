#include "Mesh.h"
#include "Application.h"
#include "GraphicsManager.h"
#include "Material.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Camera.h"

namespace
{
	const double PI = std::acos(-1.0);
}

namespace Forge
{
	Mesh::Mesh(const FTransform transform, const std::string filename)
	{
		Transform = transform;

		if (!filename.empty())
		{
			LoadFromFile(filename);
			CreateVertexBuffer();
			CreateIndexBuffer();
		}
	}

	Mesh::Mesh(const std::string filename)
	{
		Transform = FTransform();

		if (!filename.empty())
		{
			LoadFromFile(filename);
			CreateVertexBuffer();
			CreateIndexBuffer();
		}
	}

	void Mesh::LoadFromFile(const std::string filename)
	{
		loadOBJ(filename, Vertices);
	}

	FTransform Mesh::GetTransform() const
	{
		return Transform;
	}

	FVector Mesh::GetPosition() const
	{
		return Transform.Position;
	}

	FVector Mesh::GetScale() const
	{
		return Transform.Scale;
	}

	FRotator Mesh::GetRotation() const
	{
		return Transform.Rotation;
	}

	const Material& Mesh::GetMaterial() const
	{
		return material;
	}

	FVector Mesh::GetForwardVector() const
	{
		float pitch = Transform.Rotation.Pitch * PI / 180.f;
		float yaw = Transform.Rotation.Yaw * PI / 180.f;

		return FVector(
			-sinf(yaw) * cosf(pitch),
			cosf(yaw) * cosf(pitch),
			sinf(pitch)
		).Normalized();
	}

	FVector Mesh::GetRightVector() const
	{
		FVector worldUp(0.f, 0.f, 1.f);
		return FVector::Cross(GetForwardVector(), worldUp).Normalized();
	}

	FVector Mesh::GetUpVector() const
	{
		return FVector::Cross(GetRightVector(), GetForwardVector()).Normalized();
	}

	const std::vector<Vertex>& Mesh::GetVertices() const
	{
		return Vertices;
	}

	const std::vector<unsigned int>& Mesh::GetIndices() const
	{
		return Indices;
	}

	void Mesh::SetTransform(FTransform transform)
	{
		Transform = transform;
	}

	void Mesh::SetPosition(FVector position)
	{
		Transform.Position = position;
	}

	void Mesh::SetScale(FVector scale)
	{
		Transform.Scale = scale;
	}

	void Mesh::SetRotation(FRotator rotation)
	{
		Transform.Rotation = rotation;
	}
	void Mesh::SetMaterial(const Material& mat)
	{
		material = mat;
	}
	bool Mesh::HasIndexBuffer() const
	{
		return Indices.size();
	}
	Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer()
	{
		return pVertexBuffer;
	}
	Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer()
	{
		return pIndexBuffer;
	}
	void Mesh::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC vertex_buffer_desc = { };
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.ByteWidth = Vertices.size() * sizeof(Vertex);
		vertex_buffer_desc.CPUAccessFlags = 0u;
		vertex_buffer_desc.MiscFlags = 0u;
		vertex_buffer_desc.StructureByteStride = sizeof(Vertex);
		vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vertices_sub_resource_data = { };
		vertices_sub_resource_data.pSysMem = Vertices.data();

		HRESULT result = Application::Get()->GetGraphicsManager()->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertices_sub_resource_data, &pVertexBuffer);

		if (FAILED(result))
		{
			throw std::runtime_error("[GraphicsManager] DrawTestMesh: Failed to create Vertex Buffer");
		}
	}
	void Mesh::CreateIndexBuffer()
	{
		if (Indices.size())
		{
			D3D11_BUFFER_DESC index_buffer_desc = {};
			index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_buffer_desc.ByteWidth = Indices.size() * sizeof(unsigned int);
			index_buffer_desc.CPUAccessFlags = 0u;
			index_buffer_desc.MiscFlags = 0u;
			index_buffer_desc.StructureByteStride = sizeof(unsigned int);
			index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA indices_sub_resource_data = {};
			indices_sub_resource_data.pSysMem = Indices.data();

			HRESULT result = Application::Get()->GetGraphicsManager()->GetDevice()->CreateBuffer(&index_buffer_desc, &indices_sub_resource_data, &pIndexBuffer);

			if (FAILED(result))
			{
				throw std::runtime_error("[GraphicsManager] DrawTestMesh: Failed to create Index Buffer");
			}
		}
	}
	bool Mesh::loadOBJ(const std::string& filename, std::vector<Vertex>& outVertices)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::index_t> indices;
		std::vector<tinyobj::material_t> materials;
		std::string warning, error;

		// Load the model
		bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filename.c_str(), nullptr, true);

		if (!warning.empty())
		{
			std::stringstream ss("TINYOBJLOADER:: Warning: ");
			ss >> warning;
			OutputDebugStringA(ss.str().c_str());
		}

		if (!error.empty())
		{ 
			std::stringstream ss("TINYOBJLOADER:: Error: ");
			ss << error;
			OutputDebugStringA(ss.str().c_str());
		}

		if (!success)
		{
			return false;
		}

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

				//size_t num_face_vertices = size_t(shapes[s].mesh.num_face_vertices[f]);

				int material_id = shapes[s].mesh.material_ids[f];
				float alpha = 1.0f; // Default to fully opaque

				if (material_id >= 0 && material_id < materials.size())
				{
					alpha = materials[material_id].dissolve;
				}

				// Loop over vertices in the face.

				Vertex v0, v1, v2;
				for (size_t v = 0; v < 3; v++) {
					// Access the indices for position, normal, and texture coordinate
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					FVector vertex;
					FColor color;
					FVector2D tex_coords;

					// Position (XYZ)
					vertex.X = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
					vertex.Y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
					vertex.Z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

					// tinyobjloader populates attrib.colors parallel to attrib.vertices
					if (!attrib.colors.empty()) {
						color.R = attrib.colors[3 * size_t(idx.vertex_index) + 0];
						color.G = attrib.colors[3 * size_t(idx.vertex_index) + 1];
						color.B = attrib.colors[3 * size_t(idx.vertex_index) + 2];
						color.A = alpha;
					}
					else {
						color.R = color.G = color.B = 1.0f;
					}

					// Texture Coordinates (U, V)
					if (idx.texcoord_index >= 0)
					{
						tex_coords.X = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						tex_coords.Y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
					}

					v0 = v1;
					v1 = v2;
					v2 = {vertex, FVector(), tex_coords, color};
				}
				index_offset += 3;

				v0.Normal = v1.Normal = v2.Normal = CalculateFaceNormal(v0.Position, v1.Position, v2.Position);

				outVertices.push_back(v0);
				outVertices.push_back(v1);
				outVertices.push_back(v2);
			}
		}

		return true;
	}
	FVector Mesh::CalculateFaceNormal(const FVector& a, const FVector& b, const FVector& c)
	{
		FVector edge1 = b - a;
		FVector edge2 = c - a;

		return FVector::Cross(edge1, edge2).Normalized();
	}
}