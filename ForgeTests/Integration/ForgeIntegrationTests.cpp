
#include <gtest/gtest.h>

#include "Application.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Cube.h"
#include "Mesh.h"
using namespace Forge;


class ForgeIntegrationTests : public ::testing::Test
{
protected:
    static std::unique_ptr<Application> app;

    GraphicsManager* graphics = nullptr;
    Camera* camera = nullptr;

    static void SetUpTestSuite()
    {
        ASSERT_NO_THROW(
            app = std::make_unique<Application>(
                GetModuleHandle(nullptr),
                SW_SHOW
            )
        );

        ASSERT_NE(app, nullptr);
    }

    static void TearDownTestSuite()
    {
        app.reset();
    }

    void SetUp() override
    {
        ASSERT_NE(app, nullptr);

        graphics = app->GetGraphicsManager();
        camera = app->GetCamera();

        ASSERT_NE(graphics, nullptr);
        ASSERT_NE(camera, nullptr);
    }
};

std::unique_ptr<Application> ForgeIntegrationTests::app = nullptr;


TEST_F(ForgeIntegrationTests, ApplicationInitializesGraphicsManager)
{
    EXPECT_NE(app->GetGraphicsManager(), nullptr);
}

TEST_F(ForgeIntegrationTests, ApplicationInitializesInputManager)
{
    EXPECT_NE(app->GetInputManager(), nullptr);
}

TEST_F(ForgeIntegrationTests, ApplicationInitializesCamera)
{
    EXPECT_NE(app->GetCamera(), nullptr);
}

TEST_F(ForgeIntegrationTests, ApplicationCreatesValidViewport)
{
    RECT viewport = app->GetMainWindowViewport();

    EXPECT_GT(viewport.right - viewport.left, 0);
    EXPECT_GT(viewport.bottom - viewport.top, 0);
}

TEST_F(ForgeIntegrationTests, ApplicationCanBeginAndEndFrame)
{
    EXPECT_NO_THROW(app->BeginFrame(1.f));
    EXPECT_NO_THROW(app->EndFrame(1.f));
}

TEST_F(ForgeIntegrationTests, Direct3DDeviceIsInitialized)
{
    EXPECT_NE(graphics->GetDevice().Get(), nullptr);
}

TEST_F(ForgeIntegrationTests, Direct3DContextIsInitialized)
{
    EXPECT_NE(graphics->GetDeviceContext().Get(), nullptr);
}

TEST_F(ForgeIntegrationTests, GraphicsManagerCanResize)
{
    EXPECT_NO_THROW(graphics->Resize(1280, 720));
}

TEST_F(ForgeIntegrationTests, GraphicsManagerCanClearScreen)
{
    EXPECT_NO_THROW(graphics->ClearScreen(0.1f, 0.1f, 0.1f));
}

TEST_F(ForgeIntegrationTests, GraphicsManagerCanPresentFrame)
{
    EXPECT_NO_THROW(graphics->PresentFrame());
}

TEST_F(ForgeIntegrationTests, GraphicsManagerCanAssignCamera)
{
    EXPECT_NO_THROW(graphics->SetCamera(camera));
}

TEST_F(ForgeIntegrationTests, GraphicsManagerCanIgnoreNullCamera)
{
    EXPECT_NO_THROW(graphics->SetCamera(nullptr));
}
