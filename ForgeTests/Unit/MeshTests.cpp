#include <gtest/gtest.h>
#include "Mesh.h"
#include "Application.h"

using namespace Forge;

class MeshTests : public ::testing::Test
{
protected:
    static std::unique_ptr<Application> app;

    static void SetUpTestSuite()
    {
        app = std::make_unique<Application>(
            GetModuleHandle(nullptr),
            SW_SHOW
        );
    }

    static void TearDownTestSuite()
    {
        app.reset();
    }
};

std::unique_ptr<Application> MeshTests::app = nullptr;

TEST(MeshTests, DefaultTransformIsIdentity)
{
    const Mesh mesh;
    const FTransform t = mesh.GetTransform();
    EXPECT_FLOAT_EQ(t.Position.X, 0);
    EXPECT_FLOAT_EQ(t.Position.Y, 0);
    EXPECT_FLOAT_EQ(t.Position.Z, 0);
    EXPECT_FLOAT_EQ(t.Scale.X, 1);
    EXPECT_FLOAT_EQ(t.Scale.Y, 1);
    EXPECT_FLOAT_EQ(t.Scale.Z, 1);
}

TEST(MeshTests, SetPositionUpdatesTransform)
{
    Mesh mesh;
    mesh.SetPosition(FVector(3, 4, 5));
    const FVector p = mesh.GetPosition();
    EXPECT_FLOAT_EQ(p.X, 3);
    EXPECT_FLOAT_EQ(p.Y, 4);
    EXPECT_FLOAT_EQ(p.Z, 5);
}

TEST(MeshTests, SetScaleUpdatesTransform)
{
    Mesh mesh;
    mesh.SetScale(FVector(2, 3, 4));
    const FVector s = mesh.GetScale();
    EXPECT_FLOAT_EQ(s.X, 2);
    EXPECT_FLOAT_EQ(s.Y, 3);
    EXPECT_FLOAT_EQ(s.Z, 4);
}

TEST(MeshTests, SetRotationUpdatesTransform)
{
    Mesh mesh;
    mesh.SetRotation(FRotator(10, 20, 30));
    const FRotator r = mesh.GetRotation();
    EXPECT_FLOAT_EQ(r.Roll, 10);
    EXPECT_FLOAT_EQ(r.Pitch, 20);
    EXPECT_FLOAT_EQ(r.Yaw, 30);
}

TEST(MeshTests, DefaultMeshHasNoIndexBuffer)
{
    const Mesh mesh;
    EXPECT_FALSE(mesh.HasIndexBuffer());
    EXPECT_TRUE(mesh.GetIndices().empty());
}

TEST(MeshTests, SetMaterialCopiesMaterialProperties)
{
    Mesh mesh;
    Material material(false);
    material.SetDiffuseColor(FColor(0.3f, 0.5f, 0.7f));
    mesh.SetMaterial(material);

    EXPECT_FALSE(mesh.GetMaterial().IsLit());
    EXPECT_NEAR(mesh.GetMaterial().GetDiffuseColor().G, 0.5f, 1e-6f);
}

TEST(MeshTests, SuccessfullyLoadsMeshFromFile)
{
    Mesh mesh("TestAssets/Models/bunny.obj");
    EXPECT_GT(mesh.GetVertices().size(), 0);
    EXPECT_NE(mesh.GetVertexBuffer(), nullptr);
}