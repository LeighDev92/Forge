
#pragma once

#include "framework.h"
#include "Material.h"

namespace Forge
{
	/**
	 * @brief Represents a renderable 3D mesh.
	 *
	 * Stores mesh geometry, transformation data, material properties,
	 * and Direct3D 11 vertex and index buffers.
	 *
	 * Supports loading mesh geometry from OBJ files, modifying
	 * transformations, configuring materials, and providing
	 * geometry and buffer data to the graphics renderer.
	 */
	class FORGE_API Mesh
	{
	public:

		/**
		 * @brief Constructs a Mesh with the specified transform and optional file.
		 *
		 * @param transform Initial transformation applied to the mesh.
		 * Defaults to an identity transform.
		 * @param filename Optional path to an OBJ file used to load mesh geometry.
		 * Defaults to an empty string.
		 */
		Mesh(const FTransform transform = FTransform(), const std::string filename = "");

		/**
		 * @brief Constructs a Mesh from an OBJ file.
		 *
		 * @param filename Path to the OBJ file containing mesh geometry.
		 */
		Mesh(const std::string filename);

		/**
		 * @brief Loads mesh geometry from an OBJ file.
		 *
		 * Populates the mesh's vertex data using the specified file.
		 *
		 * @param filename Path to the OBJ file to load.
		 */
		void LoadFromFile(const std::string filename);

		/**
		 * @brief Retrieves the mesh transformation.
		 *
		 * @return Current mesh transformation.
		 */
		FTransform GetTransform() const;

		/**
		 * @brief Retrieves the mesh position.
		 *
		 * @return Current world position of the mesh.
		 */
		FVector GetPosition() const;

		/**
		 * @brief Retrieves the mesh scale.
		 *
		 * @return Current scale of the mesh.
		 */
		FVector GetScale() const;

		/**
		 * @brief Retrieves the mesh rotation.
		 *
		 * @return Current rotation of the mesh.
		 */
		FRotator GetRotation() const;

		/**
		 * @brief Retrieves the mesh material.
		 *
		 * @return Constant reference to the mesh material.
		 */
		const Material& GetMaterial() const;

		/**
		 * @brief Retrieves the mesh's forward direction vector.
		 *
		 * @return Normalized forward direction derived from the mesh rotation.
		 */
		FVector GetForwardVector() const;

		/**
		 * @brief Retrieves the mesh's right direction vector.
		 *
		 * @return Normalized right direction derived from the mesh rotation.
		 */
		FVector GetRightVector() const;

		/**
		 * @brief Retrieves the mesh's up direction vector.
		 *
		 * @return Normalized up direction derived from
		 * the mesh rotation.
		 */
		FVector GetUpVector() const;

		/**
		 * @brief Retrieves the mesh vertex collection.
		 *
		 * @return Constant reference to the mesh vertex data.
		 */
		const std::vector<Vertex>& GetVertices() const;

		/**
		 * @brief Retrieves the mesh index collection.
		 *
		 * @return const std::vector<unsigned int>& Constant reference to the mesh index data.
		 */
		const std::vector<unsigned int>& GetIndices() const;

		/**
		 * @brief Sets the mesh transformation.
		 *
		 * @param transform New transformation to apply to the mesh.
		 */
		void SetTransform(FTransform transform);

		/**
		 * @brief Sets the mesh position.
		 *
		 * @param position New position of the mesh.
		 */
		void SetPosition(FVector position);

		/**
		 * @brief Sets the mesh scale.
		 *
		 * @param scale New scale of the mesh.
		 */
		void SetScale(FVector scale);

		/**
		 * @brief Sets the mesh rotation.
		 *
		 * @param rotation New rotation of the mesh.
		 */
		void SetRotation(FRotator rotation);

		/**
		 * @brief Assigns a material to the mesh.
		 *
		 * @param mat Material to assign to the mesh.
		 */
		void SetMaterial(const Material& mat);

		/**
		 * @brief Determines whether the mesh contains index data.
		 *
		 * @return bool True if the mesh has an index buffer;
		 * otherwise, false.
		 */
		bool HasIndexBuffer() const;

		/**
		 * @brief Retrieves the Direct3D 11 vertex buffer of the mesh.
		 *
		 * @return Smart pointer to the mesh's vertex buffer.
		 */
		Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();

		/**
		 * @brief Retrieves the Direct3D 11 index buffer of the mesh.
		 *
		 * @return Smart pointer to the mesh's index buffer.
		 */
		Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();

	protected:
		FTransform Transform;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
		Material material;

		/**
		 * @brief Creates the Direct3D 11 vertex buffer.
		 *
		 * Initializes the GPU vertex buffer using the mesh's
		 * current vertex collection.
		 */
		void CreateVertexBuffer();

		/**
		 * @brief Creates the Direct3D 11 index buffer.
		 *
		 * Initializes the GPU index buffer using the mesh's
		 * current index collection.
		 */
		void CreateIndexBuffer();

		/**
		 * @brief Loads vertex data from an OBJ file.
		 *
		 * Parses the specified OBJ file and populates the supplied
		 * vertex collection.
		 *
		 * @param filename Path to the OBJ file to load.
		 * @param outVertices Output collection receiving the
		 * loaded vertex data.
		 *
		 * @return bool True if the OBJ file was successfully loaded;
		 * otherwise, false.
		 */
		static bool loadOBJ(const std::string& filename, std::vector<Vertex>& outVertices);

		/**
		 * @brief Calculates the normal vector of a triangle face.
		 *
		 * @param a First vertex of the triangle.
		 * @param b Second vertex of the triangle.
		 * @param c Third vertex of the triangle.
		 *
		 * @return FVector Calculated face normal vector.
		 */
		static FVector CalculateFaceNormal(const FVector& a, const FVector& b, const FVector& c);
	};
}
