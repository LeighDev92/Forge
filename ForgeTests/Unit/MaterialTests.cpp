#include <gtest/gtest.h>
#include "Material.h"

using namespace Forge;

TEST(MaterialTests, DefaultMaterialIsLit)
{
    const Material material;
    EXPECT_TRUE(material.IsLit());
}

TEST(MaterialTests, CanConstructUnlitMaterial)
{
    const Material material(false);
    EXPECT_FALSE(material.IsLit());
}

TEST(MaterialTests, DefaultDiffuseColorIsWhite)
{
    const FColor color = Material().GetDiffuseColor();
    EXPECT_FLOAT_EQ(color.R, 1);
    EXPECT_FLOAT_EQ(color.G, 1);
    EXPECT_FLOAT_EQ(color.B, 1);
    EXPECT_FLOAT_EQ(color.A, 1);
}

TEST(MaterialTests, SetDiffuseColorUpdatesColor)
{
    Material material;
    material.SetDiffuseColor(FColor(0.2f, 0.4f, 0.6f, 0.8f));
    const FColor color = material.GetDiffuseColor();
    EXPECT_FLOAT_EQ(color.R, 0.2f);
    EXPECT_FLOAT_EQ(color.G, 0.4f);
    EXPECT_FLOAT_EQ(color.B, 0.6f);
    EXPECT_FLOAT_EQ(color.A, 0.8f);
}

TEST(MaterialTests, SetDisplacementStrengthUpdatesValue)
{
    Material material;
    material.SetDisplacementStrength(0.35f);
    EXPECT_FLOAT_EQ(material.GetDisplacementStrength(), 0.35f);
}

TEST(MaterialTests, DefaultTexturesAreInvalid)
{
    const Material material;
    EXPECT_FALSE(material.GetMainTexture().IsValidTexture());
    EXPECT_FALSE(material.GetDisplacementMap().IsValidTexture());
}
