#include "Icosphere.h"

namespace
{
    const double PI = std::acos(-1.0);
}

namespace Forge
{
    Icosphere::Icosphere(float radius, int subdivisions, bool inverted)
    {
        Radius = std::abs(radius);
        Subdivisions = std::min(std::max(0, subdivisions), 8);
        Inverted = inverted;
        Generate();
    }

    void Icosphere::Generate()
    {
        // Clear previously generated geometry
        Vertices.clear();

        // Temporary triangle vertex storage
        std::vector<FVector> triangles;

        // An icosahedron has 20 triangles
        triangles.reserve(60);

        // Generate the initial icosahedron
        CreateBaseIcosahedron(triangles);

        // Subdivide the triangles
        for (int i = 0; i < Subdivisions; i++)
        {
            Subdivide(triangles);
        }

        // Convert the generated geometry into mesh vertices
        BuildVertices(triangles);
    }

    void Icosphere::SetRadius(float radius)
    {
        radius = std::abs(radius);

        if (radius != Radius)
        {
            Radius = radius;
            Generate();
        }
    }

    void Icosphere::SetSubdivisions(int subdivisions)
    {
        Subdivisions = std::min(std::max(0, subdivisions), 8);

        if (subdivisions != Subdivisions)
        {
            Subdivisions = subdivisions;
            Generate();
        }
    }

    float Icosphere::GetRadius() const
    {
        return 0.0f;
    }

    float Icosphere::GetSubdivisions() const
    {
        return 0.0f;
    }

    void Icosphere::CreateBaseIcosahedron(
        std::vector<FVector>& triangles)
    {
        const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

        std::vector<FVector> positions =
        {
            {-1,  t,  0},
            { 1,  t,  0},
            {-1, -t,  0},
            { 1, -t,  0},

            { 0, -1,  t},
            { 0,  1,  t},
            { 0, -1, -t},
            { 0,  1, -t},

            { t,  0, -1},
            { t,  0,  1},
            {-t,  0, -1},
            {-t,  0,  1}
        };

        // Normalize initial positions
        for (auto& position : positions)
        {
            position = position.Normalized();
        }

        // Each group of three indices represents a triangle
        const uint32_t indices[] =
        {
            0,11,5,
            0,5,1,
            0,1,7,
            0,7,10,
            0,10,11,

            1,5,9,
            5,11,4,
            11,10,2,
            10,7,6,
            7,1,8,

            3,9,4,
            3,4,2,
            3,2,6,
            3,6,8,
            3,8,9,

            4,9,5,
            2,4,11,
            6,2,10,
            8,6,7,
            9,8,1
        };

        // Expand the indexed icosahedron into triangle vertices
        for (size_t i = 0; i < std::size(indices); i += 3)
        {
            FVector a = positions[indices[i]];
            FVector b = positions[indices[i + 1]];
            FVector c = positions[indices[i + 2]];

            AddTriangle(triangles, a, b, c);
        }
    }

    FVector Icosphere::GetMidpoint(const FVector& a, const FVector& b) const
    {
        FVector midpoint = (a + b) * 0.5f;
        return midpoint.Normalized();
    }

    void Icosphere::AddTriangle(std::vector<FVector>& output, const FVector& a, const FVector& b, const FVector& c)
    {
        if (Inverted)
        {
            output.push_back(a);
            output.push_back(c);
            output.push_back(b);
        }
        else
        {
            output.push_back(a);
            output.push_back(b);
            output.push_back(c);
        }
    }

    void Icosphere::Subdivide(std::vector<FVector>& triangles)
    {
        std::vector<FVector> new_triangles;

        // Every triangle becomes four triangles
        new_triangles.reserve(triangles.size() * 4);

        for (size_t i = 0; i < triangles.size(); i += 3)
        {
            const FVector& v1 = triangles[i];
            const FVector& v2 = triangles[i + 1];
            const FVector& v3 = triangles[i + 2];

            // Calculate the three edge midpoints
            FVector a = GetMidpoint(v1, v2);
            FVector b = GetMidpoint(v2, v3);
            FVector c = GetMidpoint(v3, v1);

            // Create four new triangles
            AddTriangle(new_triangles, v1, a, c);
            AddTriangle(new_triangles, v2, b, a);
            AddTriangle(new_triangles, v3, c, b);
            AddTriangle(new_triangles, a, b, c);
        }

        triangles = std::move(new_triangles);
    }

    void Icosphere::BuildVertices(
        const std::vector<FVector>& triangles)
    {
        Vertices.reserve(triangles.size());

        for (size_t i = 0; i < triangles.size(); i += 3)
        {
            FVector2D uv0 = CalculateSphereUV(triangles[i]);
            FVector2D uv1 = CalculateSphereUV(triangles[i + 1]);
            FVector2D uv2 = CalculateSphereUV(triangles[i + 2]);

            // Check whether this triangle crosses the U seam
            float minU = std::min({ uv0.X, uv1.X, uv2.X });
            float maxU = std::max({ uv0.X, uv1.X, uv2.X });

            // Correct polar UV coordinates
            if (fabsf(triangles[i].Normalized().Z) > 0.9999f)
                uv0.X = (uv1.X + uv2.X) * 0.5f;

            if (fabsf(triangles[i + 1].Normalized().Z) > 0.9999f)
                uv1.X = (uv0.X + uv2.X) * 0.5f;

            if (fabsf(triangles[i + 2].Normalized().Z) > 0.9999f)
                uv2.X = (uv0.X + uv1.X) * 0.5f;

            if (maxU - minU > 0.5f)
            {
                if (uv0.X < 0.5f) uv0.X += 1.0f;
                if (uv1.X < 0.5f) uv1.X += 1.0f;
                if (uv2.X < 0.5f) uv2.X += 1.0f;
            }

            FVector v0 = triangles[i] * Radius;
            FVector v1 = triangles[i + 1] * Radius;
            FVector v2 = triangles[i + 2] * Radius;

            Vertices.emplace_back(v0, v0.Normalized(), uv0, Color::White);
            Vertices.emplace_back(v1, v1.Normalized(), uv1, Color::White);
            Vertices.emplace_back(v2, v2.Normalized(), uv2, Color::White);
        }

        CreateVertexBuffer();
    }

    FVector2D Icosphere::CalculateSphereUV(const FVector& position)
    {
        FVector normal = position.Normalized();

        float u = 0.5f - atan2(normal.Y, normal.X) / (2.0f * PI);
        float v = 0.5f - asin(normal.Z) / PI;

        return FVector2D(u, v);
    }
}