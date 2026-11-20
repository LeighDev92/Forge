#include <gtest/gtest.h>
#include "Keys.h"
#include "Mouse.h"

using namespace Forge;

TEST(KeyConversionTests, UnknownVirtualKeyMapsToUnknown)
{
    EXPECT_EQ(VirtualKeyToKey(0), Key::Unknown);
}

TEST(KeyConversionTests, LetterVirtualKeyMapsToLetter)
{
    EXPECT_EQ(VirtualKeyToKey('W'), Key::W);
}

TEST(MouseConversionTests, UnknownMouseMessageMapsToUnknown)
{
    EXPECT_EQ(MouseMessageToMouseButton(0), MouseButton::Unkown);
}
