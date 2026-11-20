#include <gtest/gtest.h>
#include "Camera.h"
using namespace Forge;

TEST(CameraTests, DefaultConstructionStoresDefaultPositionRotationAndFOV)
{
    const Camera camera;
    EXPECT_FLOAT_EQ(camera.GetPosition().X, 0.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().Y, 0.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().Z, 0.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Pitch, 0.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Yaw, 0.f);
    EXPECT_FLOAT_EQ(camera.GetFOV(), 90.f);
}

TEST(CameraTests, ConstructorStoresProvidedValues)
{
    const Camera camera({ 1.f, 2.f, 3.f }, { 4.f, 5.f, 6.f }, 70.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().X, 1.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().Y, 2.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().Z, 3.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Roll, 4.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Pitch, 5.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Yaw, 6.f);
    EXPECT_FLOAT_EQ(camera.GetFOV(), 70.f);
}

TEST(CameraTests, SettersUpdateCameraState)
{
    Camera camera;
    camera.SetPosition(FVector(8.f, 9.f, 10.f));
    camera.SetRotation(FRotator(11.f, 12.f, 13.f));
    camera.SetFOV(60.f);

    EXPECT_FLOAT_EQ(camera.GetPosition().X, 8.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().Y, 9.f);
    EXPECT_FLOAT_EQ(camera.GetPosition().Z, 10.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Roll, 11.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Pitch, 12.f);
    EXPECT_FLOAT_EQ(camera.GetRotation().Yaw, 13.f);
    EXPECT_FLOAT_EQ(camera.GetFOV(), 60.f);
}

TEST(CameraTests, DirectionVectorsAreFinite)
{
    const Camera camera;
    const FVector forward = camera.GetForwardVector();
    const FVector right = camera.GetRightVector();
    const FVector up = camera.GetUpVector();

    EXPECT_TRUE(std::isfinite(forward.X) && std::isfinite(forward.Y) && std::isfinite(forward.Z));
    EXPECT_TRUE(std::isfinite(right.X) && std::isfinite(right.Y) && std::isfinite(right.Z));
    EXPECT_TRUE(std::isfinite(up.X) && std::isfinite(up.Y) && std::isfinite(up.Z));
}

TEST(CameraTests, LookAtDoesNotProduceNonFiniteRotation)
{
    Camera camera(FVector(0.f, -10.f, 0.f));
    camera.LookAt(FVector(0.f, 0.f, 0.f));
    const FRotator r = camera.GetRotation();
    EXPECT_TRUE(std::isfinite(r.Roll));
    EXPECT_TRUE(std::isfinite(r.Pitch));
    EXPECT_TRUE(std::isfinite(r.Yaw));
}

TEST(CameraTests, OrbitAndZoomKeepFiniteOrbitDistance)
{
    Camera camera;
    camera.SetOrbitTarget(true);
    camera.SetOrbitTargetPosition({ 0.f, 0.f, 0.f });
    camera.Orbit(10, -5);
    camera.Zoom(2);
    EXPECT_TRUE(std::isfinite(camera.GetOrbitDistance()));
}
