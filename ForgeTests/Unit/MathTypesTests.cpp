#include <gtest/gtest.h>
#include "framework.h"

using namespace Forge;

TEST(FVectorTests, DefaultConstructorInitializesToZero)
{
    const FVector v;
    EXPECT_FLOAT_EQ(v.X, 0.0f);
    EXPECT_FLOAT_EQ(v.Y, 0.0f);
    EXPECT_FLOAT_EQ(v.Z, 0.0f);
}

TEST(FVectorTests, ValueConstructorStoresComponents)
{
    const FVector v(1.0f, -2.0f, 3.5f);
    EXPECT_FLOAT_EQ(v.X, 1.0f);
    EXPECT_FLOAT_EQ(v.Y, -2.0f);
    EXPECT_FLOAT_EQ(v.Z, 3.5f);
}

TEST(FVectorTests, AdditionAndSubtraction)
{
    const FVector a(1, 2, 3), b(4, -2, 1);
    const FVector sum = a + b;
    const FVector difference = a - b;
    EXPECT_FLOAT_EQ(sum.X, 5);
    EXPECT_FLOAT_EQ(sum.Y, 0);
    EXPECT_FLOAT_EQ(sum.Z, 4);
    EXPECT_FLOAT_EQ(difference.X, -3);
    EXPECT_FLOAT_EQ(difference.Y, 4);
    EXPECT_FLOAT_EQ(difference.Z, 2);
}

TEST(FVectorTests, ScalarMultiplicationAndCompoundMultiplication)
{
    const FVector a(1, -2, 3);
    const FVector b = a * 2.0f;
    EXPECT_FLOAT_EQ(b.X, 2);
    EXPECT_FLOAT_EQ(b.Y, -4);
    EXPECT_FLOAT_EQ(b.Z, 6);

    FVector c(1, -2, 3);
    c *= -2.0f;
    EXPECT_FLOAT_EQ(c.X, -2);
    EXPECT_FLOAT_EQ(c.Y, 4);
    EXPECT_FLOAT_EQ(c.Z, -6);
}

TEST(FVectorTests, LengthAndNormalization)
{
    const FVector v(3, 4, 0);
    EXPECT_FLOAT_EQ(v.Length(), 5.0f);
    const FVector n = v.Normalized();
    EXPECT_NEAR(n.X, 0.6f, 1e-6f);
    EXPECT_NEAR(n.Y, 0.8f, 1e-6f);
    EXPECT_NEAR(n.Z, 0.0f, 1e-6f);
    EXPECT_NEAR(n.Length(), 1.0f, 1e-6f);
}

TEST(FVectorTests, NormalizingZeroVectorReturnsZero)
{
    const FVector n = FVector().Normalized();
    EXPECT_FLOAT_EQ(n.X, 0);
    EXPECT_FLOAT_EQ(n.Y, 0);
    EXPECT_FLOAT_EQ(n.Z, 0);
}

TEST(FVectorTests, DotAndCrossProducts)
{
    const FVector x(1, 0, 0), y(0, 1, 0);
    EXPECT_FLOAT_EQ(FVector::Dot(x, y), 0.0f);
    const FVector z = FVector::Cross(x, y);
    EXPECT_FLOAT_EQ(z.X, 0);
    EXPECT_FLOAT_EQ(z.Y, 0);
    EXPECT_FLOAT_EQ(z.Z, 1);
}

TEST(FVector2DTests, ConstructorsAndArithmetic)
{
    const FVector2D zero;
    EXPECT_FLOAT_EQ(zero.X, 0);
    EXPECT_FLOAT_EQ(zero.Y, 0);

    const FVector2D a(2, 3), b(-1, 4);
    const FVector2D sum = a + b;
    const FVector2D diff = a - b;
    const FVector2D scaled = a * 2.f;
    EXPECT_FLOAT_EQ(sum.X, 1);
    EXPECT_FLOAT_EQ(sum.Y, 7);
    EXPECT_FLOAT_EQ(diff.X, 3);
    EXPECT_FLOAT_EQ(diff.Y, -1);
    EXPECT_FLOAT_EQ(scaled.X, 4);
    EXPECT_FLOAT_EQ(scaled.Y, 6);
}

TEST(FVector2DTests, CompoundMultiplication)
{
    FVector2D v(2, -3);
    v *= 0.5f;
    EXPECT_FLOAT_EQ(v.X, 1);
    EXPECT_FLOAT_EQ(v.Y, -1.5f);
}

TEST(FRotatorTests, DefaultAndValueConstructors)
{
    const FRotator zero;
    EXPECT_FLOAT_EQ(zero.Roll, 0);
    EXPECT_FLOAT_EQ(zero.Pitch, 0);
    EXPECT_FLOAT_EQ(zero.Yaw, 0);

    const FRotator r(10, 20, 30);
    EXPECT_FLOAT_EQ(r.Roll, 10);
    EXPECT_FLOAT_EQ(r.Pitch, 20);
    EXPECT_FLOAT_EQ(r.Yaw, 30);
}

TEST(FTransformTests, DefaultConstructorUsesIdentityTransform)
{
    const FTransform t;
    EXPECT_FLOAT_EQ(t.Position.X, 0);
    EXPECT_FLOAT_EQ(t.Position.Y, 0);
    EXPECT_FLOAT_EQ(t.Position.Z, 0);
    EXPECT_FLOAT_EQ(t.Scale.X, 1);
    EXPECT_FLOAT_EQ(t.Scale.Y, 1);
    EXPECT_FLOAT_EQ(t.Scale.Z, 1);
    EXPECT_FLOAT_EQ(t.Rotation.Roll, 0);
    EXPECT_FLOAT_EQ(t.Rotation.Pitch, 0);
    EXPECT_FLOAT_EQ(t.Rotation.Yaw, 0);
}

TEST(FTransformTests, ValueConstructorStoresComponents)
{
    const FTransform t(FVector(1, 2, 3), FVector(2, 3, 4), FRotator(5, 6, 7));
    EXPECT_FLOAT_EQ(t.Position.X, 1);
    EXPECT_FLOAT_EQ(t.Scale.Y, 3);
    EXPECT_FLOAT_EQ(t.Rotation.Yaw, 7);
}

TEST(FColorTests, DefaultConstructorIsOpaqueWhite)
{
    const FColor c;
    EXPECT_FLOAT_EQ(c.R, 1);
    EXPECT_FLOAT_EQ(c.G, 1);
    EXPECT_FLOAT_EQ(c.B, 1);
    EXPECT_FLOAT_EQ(c.A, 1);
}

TEST(FColorTests, ValueConstructorStoresRGBA)
{
    const FColor c(0.1f, 0.2f, 0.3f, 0.4f);
    EXPECT_FLOAT_EQ(c.R, 0.1f);
    EXPECT_FLOAT_EQ(c.G, 0.2f);
    EXPECT_FLOAT_EQ(c.B, 0.3f);
    EXPECT_FLOAT_EQ(c.A, 0.4f);
}

TEST(VertexTests, DefaultConstructorInitializesMembers)
{
    const Vertex v;
    EXPECT_FLOAT_EQ(v.Position.X, 0);
    EXPECT_FLOAT_EQ(v.Normal.Y, 0);
    EXPECT_FLOAT_EQ(v.TexCoords.X, 0);
    EXPECT_FLOAT_EQ(v.Color.A, 1);
}

TEST(ColorConstantsTests, NamedColorsHaveExpectedValues)
{
    EXPECT_FLOAT_EQ(Color::Black.R, 0);
    EXPECT_FLOAT_EQ(Color::Black.G, 0);
    EXPECT_FLOAT_EQ(Color::Black.B, 0);
    EXPECT_FLOAT_EQ(Color::White.R, 1);
    EXPECT_FLOAT_EQ(Color::White.G, 1);
    EXPECT_FLOAT_EQ(Color::White.B, 1);
    EXPECT_FLOAT_EQ(Color::Red.R, 1);
    EXPECT_FLOAT_EQ(Color::Red.G, 0);
    EXPECT_FLOAT_EQ(Color::Red.B, 0);
    EXPECT_FLOAT_EQ(Color::Green.R, 0);
    EXPECT_FLOAT_EQ(Color::Green.G, 1);
    EXPECT_FLOAT_EQ(Color::Green.B, 0);
    EXPECT_FLOAT_EQ(Color::Blue.R, 0);
    EXPECT_FLOAT_EQ(Color::Blue.G, 0);
    EXPECT_FLOAT_EQ(Color::Blue.B, 1);
}
