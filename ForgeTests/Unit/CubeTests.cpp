
#include <gtest/gtest.h>

#include "Cube.h"
#include "Application.h"
using namespace Forge;

class CubeTests : public ::testing::Test
{
protected:
    static std::unique_ptr<Application> app;
    std::unique_ptr<Cube> cube;

    static void SetUpTestSuite()
    {
        app = std::make_unique<Application>(
            GetModuleHandle(nullptr),
            SW_SHOW
        );
    }

    void SetUp()
    {
        cube = std::make_unique<Cube>();
    }

    static void TearDownTestSuite()
    {
        app.reset();
    }
};

std::unique_ptr<Application> CubeTests::app = nullptr;

TEST_F(CubeTests, ConstructorCreatesCube)
{
    EXPECT_NE(cube, nullptr);
}

TEST_F(CubeTests, CubeContainsVertices)
{
    EXPECT_FALSE(cube->GetVertices().empty());
}

TEST_F(CubeTests, CubeContainsExpectedVertexCount)
{
    EXPECT_EQ(cube->GetVertices().size(), 36);
}

TEST_F(CubeTests, CubeVerticesHaveValidPositions)
{
    ASSERT_FALSE(cube->GetVertices().empty());

    for (const Vertex& vertex : cube->GetVertices())
    {
        EXPECT_TRUE(std::isfinite(vertex.Position.X));
        EXPECT_TRUE(std::isfinite(vertex.Position.Y));
        EXPECT_TRUE(std::isfinite(vertex.Position.Z));
    }
}

TEST_F(CubeTests, CubeVerticesHaveValidNormals)
{
    ASSERT_FALSE(cube->GetVertices().empty());

    for (const Vertex& vertex : cube->GetVertices())
    {
        EXPECT_TRUE(std::isfinite(vertex.Normal.X));
        EXPECT_TRUE(std::isfinite(vertex.Normal.Y));
        EXPECT_TRUE(std::isfinite(vertex.Normal.Z));
    }
}

TEST_F(CubeTests, CubeCanBeCreatedWithCustomTransform)
{
    FTransform transform;
    transform.Position = FVector(5.0f, 2.0f, 3.0f);
    transform.Scale = FVector(2.0f, 2.0f, 2.0f);

    Cube cube(transform);

    EXPECT_FLOAT_EQ(cube.GetPosition().X, 5.0f);
    EXPECT_FLOAT_EQ(cube.GetPosition().Y, 2.0f);
    EXPECT_FLOAT_EQ(cube.GetPosition().Z, 3.0f);

    EXPECT_FLOAT_EQ(cube.GetScale().X, 2.0f);
    EXPECT_FLOAT_EQ(cube.GetScale().Y, 2.0f);
    EXPECT_FLOAT_EQ(cube.GetScale().Z, 2.0f);
}
