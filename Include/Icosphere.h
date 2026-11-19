
#pragma once
#include "Mesh.h"
#include "framework.h"

namespace Forge
{
	/**
	 * @brief Represents a procedurally generated icosphere mesh.
	 *
	 * Extends Mesh to generate a spherical geometry using an
	 * icosahedron as its base geometry. Supports configurable
	 * radius, subdivision levels, inverted geometry, and
	 * spherical UV coordinate generation.
	 */
	class FORGE_API Icosphere : public Mesh
	{
	public:

		/**
		 * @brief Constructs an Icosphere with the specified parameters.
		 *
		 * @param radius Radius of the sphere.
		 * @param subdivisions Number of subdivision levels applied
		 * to the base icosahedron.
		 * @param inverted Determines whether the sphere geometry
		 * should be inverted. Defaults to false.
		 */
		Icosphere(float radius, int subdivisions, bool inverted = false);

		/**
		 * @brief Generates the icosphere geometry.
		 *
		 * Creates the base icosahedron, applies the configured
		 * subdivision levels, and builds the resulting mesh vertices.
		 */
		void Generate();

		/**
		 * @brief Sets the radius of the icosphere.
		 *
		 * @param radius New radius of the sphere.
		 */
		void SetRadius(float radius);

		/**
		 * @brief Sets the subdivision level of the icosphere.
		 *
		 * @param subdivisions New number of subdivision levels.
		 */
		void SetSubdivisions(int subdivisions);

		/**
		 * @brief Retrieves the radius of the icosphere.
		 *
		 * @return float Current sphere radius.
		 */
		float GetRadius() const;

		/**
		 * @brief Retrieves the subdivision level of the icosphere.
		 *
		 * @return Number of subdivision levels.
		 */
		int GetSubdivisions() const;

	protected:
		float _radius;
		int _subdivisions;
		bool _inverted;

		/**
		 * @brief Creates the initial icosahedron geometry.
		 *
		 * Populates the supplied triangle collection with the
		 * faces of the base icosahedron.
		 *
		 * @param triangles Output collection of triangle vertices.
		 */
		void CreateBaseIcosahedron(std::vector<FVector>& triangles);

		/**
		 * @brief Subdivides the supplied triangle geometry.
		 *
		 * Splits existing triangles into smaller triangles using
		 * calculated edge midpoints.
		 *
		 * @param triangles Triangle vertices to subdivide.
		 */
		void Subdivide(std::vector<FVector>& triangles);

		/**
		 * @brief Calculates the midpoint between two vertices.
		 *
		 * @param a First vertex.
		 * @param b Second vertex.
		 *
		 * @return FVector Midpoint between the supplied vertices.
		 */
		FVector GetMidpoint(const FVector& a, const FVector& b) const;

		/**
		 * @brief Adds a triangle to the output vertex collection.
		 *
		 * @param output Collection receiving the triangle vertices.
		 * @param a First triangle vertex.
		 * @param b Second triangle vertex.
		 * @param c Third triangle vertex.
		 */
		void AddTriangle(
			std::vector<FVector>& output,
			const FVector& a,
			const FVector& b,
			const FVector& c
		);

		/**
		 * @brief Builds the final mesh vertices from triangle geometry.
		 *
		 * Converts the generated triangle collection into the
		 * vertex representation used by the Mesh base class.
		 *
		 * @param triangles Collection of generated triangle vertices.
		 */
		void BuildVertices(const std::vector<FVector>& triangles);

		/**
		 * @brief Calculates spherical UV coordinates for a vertex.
		 *
		 * @param vertex Vertex position used to calculate UV coordinates.
		 *
		 * @return FVector2D Calculated UV coordinates.
		 */
		static FVector2D CalculateSphereUV(const FVector& vertex);
	};
}
