#include <gtest/gtest.h>

#include "Application.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "Mesh.h"
using namespace Forge;

class GraphicsManagerTests : public ::testing::Test
{
protected:
    static std::unique_ptr<Application> App;

    static void SetUpTestSuite()
    {
        App = std::make_unique<Application>(
            GetModuleHandle(nullptr),
            SW_SHOW
        );
    }

    void SetUp() override
    {
        ASSERT_NE(App, nullptr);
        ASSERT_NE(App->GetGraphicsManager(), nullptr);
    }

    static void TearDownTestSuite()
    {
        App.reset();
    }
};

std::unique_ptr<Application> GraphicsManagerTests::App = nullptr;

TEST_F(GraphicsManagerTests, DeviceIsInitialized)
{
    EXPECT_NE(App->GetGraphicsManager()->GetDevice().Get(), nullptr);
}

TEST_F(GraphicsManagerTests, DeviceContextIsInitialized)
{
    EXPECT_NE(App->GetGraphicsManager()->GetDeviceContext().Get(), nullptr);
}

TEST_F(GraphicsManagerTests, SetCameraAcceptsValidCamera)
{
    Camera camera;

    EXPECT_NO_THROW(App->GetGraphicsManager()->SetCamera(&camera));
}

TEST_F(GraphicsManagerTests, SetCameraIgnoresNullCamera)
{
    Camera camera;

    App->GetGraphicsManager()->SetCamera(&camera);
    App->GetGraphicsManager()->SetCamera(nullptr);

    SUCCEED();
}

TEST_F(GraphicsManagerTests, ResizeUpdatesSwapChain)
{
    EXPECT_NO_THROW(App->GetGraphicsManager()->Resize(1024, 768));
}

TEST_F(GraphicsManagerTests, ClearScreenDoesNotThrow)
{
    EXPECT_NO_THROW(App->GetGraphicsManager()->ClearScreen(0.1f, 0.2f, 0.3f));
}

TEST_F(GraphicsManagerTests, ClearRenderTargetDoesNotThrow)
{
    EXPECT_NO_THROW(App->GetGraphicsManager()->ClearRenderTargetView(0.f, 0.f, 0.f));
}

TEST_F(GraphicsManagerTests, ClearDepthStencilDoesNotThrow)
{
    EXPECT_NO_THROW(App->GetGraphicsManager()->ClearDepthStencilView());
}

TEST_F(GraphicsManagerTests, PresentFrameDoesNotThrow)
{
    EXPECT_NO_THROW(App->GetGraphicsManager()->PresentFrame());
}

TEST_F(GraphicsManagerTests, DrawNullMeshDoesNotThrow)
{
    EXPECT_NO_THROW(App->GetGraphicsManager()->DrawMesh(nullptr, true, false));
}

TEST_F(GraphicsManagerTests, MissingTextureReturnsFalse)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

    EXPECT_FALSE(
        App->GetGraphicsManager()->CreateTextureFromFile(
            texture,
            "missing_texture.png"
        )
    );
}