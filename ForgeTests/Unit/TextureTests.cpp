#include <gtest/gtest.h>
#include "Texture.h"
#include "Application.h"
using namespace Forge;

class TextureTests : public ::testing::Test
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

std::unique_ptr<Application> TextureTests::app = nullptr;

TEST_F(TextureTests, DefaultTextureIsInvalid)
{
    const Texture texture;
    EXPECT_FALSE(texture.IsValidTexture());
}

TEST_F(TextureTests, EmptyFilenameTextureIsInvalid)
{
    const Texture texture("");
    EXPECT_FALSE(texture.IsValidTexture());
}

TEST_F(TextureTests, LoadsValidTexture)
{
    const Texture texture("TestAssets/Textures/test.jpg");
    EXPECT_TRUE(texture.IsValidTexture());
}

TEST_F(TextureTests, FailsToLoadMissingTextureFile)
{
    Texture texture;
    bool loaded = texture.LoadTextureFromFile("TestAssets/Textures/missing.jpg");
    EXPECT_FALSE(loaded);
    EXPECT_FALSE(texture.IsValidTexture());
}
