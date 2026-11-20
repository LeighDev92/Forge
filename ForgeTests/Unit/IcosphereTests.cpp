#include <gtest/gtest.h>

#include "Icosphere.h"
#include "Application.h"
using namespace Forge;


class IcosphereTests : public ::testing::Test
{
protected:
    static std::unique_ptr<Application> app;
    std::unique_ptr<Icosphere> ico;

    static void SetUpTestSuite()
    {
        app = std::make_unique<Application>(
            GetModuleHandle(nullptr),
            SW_SHOW
        );
    }

    void SetUp()
    {
        ico = std::make_unique<Icosphere>(1.0f, 1);
    }

    static void TearDownTestSuite()
    {
        app.reset();
    }
};

std::unique_ptr<Application> IcosphereTests::app = nullptr;

TEST_F(IcosphereTests, ConstructorSetsRadius)
{
    EXPECT_FLOAT_EQ(ico->GetRadius(), 1.0f);
}

TEST_F(IcosphereTests, ConstructorSetsSubdivisions)
{
    EXPECT_FLOAT_EQ(ico->GetSubdivisions(), 1.0f);
}

TEST_F(IcosphereTests, SetRadiusUpdatesRadius)
{
    ico->SetRadius(5.0f);

    EXPECT_FLOAT_EQ(ico->GetRadius(), 5.0f);
}

TEST_F(IcosphereTests, SetSubdivisionsUpdatesSubdivisions)
{
    ico->SetSubdivisions(3);

    EXPECT_FLOAT_EQ(ico->GetSubdivisions(), 3.0f);
}

TEST_F(IcosphereTests, GenerateProducesVertices)
{
    ico->Generate();

    EXPECT_FALSE(ico->GetVertices().empty());
}

TEST_F(IcosphereTests, GeneratedVerticesLieOnSphereSurface)
{
    constexpr float Radius = 2.0f;
    constexpr float Tolerance = 0.001f;

    Icosphere sphere(Radius, 2);
    sphere.Generate();

    ASSERT_FALSE(sphere.GetVertices().empty());

    for (const Vertex& vertex : sphere.GetVertices())
    {
        EXPECT_NEAR(vertex.Position.Length(), Radius, Tolerance);
    }
}

TEST_F(IcosphereTests, HigherSubdivisionsGenerateMoreVertices)
{
    Icosphere lowSubdivision(1.0f, 1);
    Icosphere highSubdivision(1.0f, 2);

    lowSubdivision.Generate();
    highSubdivision.Generate();

    EXPECT_GT(
        highSubdivision.GetVertices().size(),
        lowSubdivision.GetVertices().size()
    );
}

TEST_F(IcosphereTests, ChangingRadiusRegeneratesGeometry)
{
    ico->Generate();

    ico->SetRadius(3.0f);

    ASSERT_FALSE(ico->GetVertices().empty());

    for (const Vertex& vertex : ico->GetVertices())
    {
        EXPECT_NEAR(vertex.Position.Length(), 3.0f, 0.001f);
    }
}

TEST_F(IcosphereTests, InvertedSphereCanBeGenerated)
{
    Icosphere invertedSphere(1.0f, 1, true);

    EXPECT_NO_THROW(invertedSphere.Generate());
    EXPECT_FALSE(invertedSphere.GetVertices().empty());
}
