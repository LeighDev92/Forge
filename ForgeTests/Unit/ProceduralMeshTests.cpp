#include <gtest/gtest.h>
#include "Cube.h"
#include "Icosphere.h"
#include "SkySphere.h"
#include <cmath>

using namespace Forge;

TEST(CubeTests, GeneratesGeometry)
{
    const Cube cube;
    EXPECT_FALSE(cube.GetVertices().empty());
}

TEST(CubeTests, AppliesInitialTransform)
{
    const FTransform transform(FVector(2, 3, 4), FVector(2, 2, 2), FRotator(0, 0, 0));
    const Cube cube(transform);
    EXPECT_FLOAT_EQ(cube.GetPosition().X, 2);
    EXPECT_FLOAT_EQ(cube.GetPosition().Y, 3);
    EXPECT_FLOAT_EQ(cube.GetPosition().Z, 4);
    EXPECT_FLOAT_EQ(cube.GetScale().X, 2);
}

TEST(IcosphereTests, GeneratesVertices)
{
    const Icosphere sphere(1.0f, 0);
    EXPECT_FALSE(sphere.GetVertices().empty());
}

TEST(IcosphereTests, ReportsConfiguredRadiusAndSubdivision)
{
    const Icosphere sphere(2.5f, 2);
    EXPECT_FLOAT_EQ(sphere.GetRadius(), 2.5f);
    EXPECT_FLOAT_EQ(sphere.GetSubdivisions(), 2.0f);
}

TEST(IcosphereTests, VerticesLieApproximatelyOnRequestedRadius)
{
    const float radius = 2.0f;
    const Icosphere sphere(radius, 1);

    ASSERT_FALSE(sphere.GetVertices().empty());
    for (const Vertex& vertex : sphere.GetVertices())
    {
        EXPECT_NEAR(vertex.Position.Length(), radius, 1e-3f);
    }
}

TEST(IcosphereTests, SubdivisionIncreasesVertexCount)
{
    const Icosphere low(1.0f, 0);
    const Icosphere high(1.0f, 1);
    EXPECT_GT(high.GetVertices().size(), low.GetVertices().size());
}

TEST(IcosphereTests, SetRadiusRegeneratesOrUpdatesGeometry)
{
    Icosphere sphere(1.0f, 1);
    sphere.SetRadius(3.0f);
    EXPECT_FLOAT_EQ(sphere.GetRadius(), 3.0f);
    ASSERT_FALSE(sphere.GetVertices().empty());
    for (const Vertex& vertex : sphere.GetVertices())
        EXPECT_NEAR(vertex.Position.Length(), 3.0f, 1e-3f);
}

TEST(IcosphereTests, SetSubdivisionsUpdatesReportedValue)
{
    Icosphere sphere(1.0f, 0);
    sphere.SetSubdivisions(2);
    EXPECT_FLOAT_EQ(sphere.GetSubdivisions(), 2.0f);
    EXPECT_FALSE(sphere.GetVertices().empty());
}

TEST(SkySphereTests, DefaultConstructionGeneratesGeometry)
{
    const SkySphere sphere;
    EXPECT_FALSE(sphere.GetVertices().empty());
}
