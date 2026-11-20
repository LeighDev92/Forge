#include <gtest/gtest.h>
#include "InputManager.h"
#include "Application.h"

using namespace Forge;

class InputManagerTests : public ::testing::Test
{
protected:
    static std::unique_ptr<Application> app;
    InputManager* input;

    static void SetUpTestSuite()
    {
        app = std::make_unique<Application>(
            GetModuleHandle(nullptr),
            SW_SHOW
        );
    }

    void SetUp() override
    {
        ASSERT_NE(app, nullptr);
        input = app->GetInputManager();

        ASSERT_NE(input, nullptr);
        input->ClearState();
    }
};

std::unique_ptr<Application> InputManagerTests::app = nullptr;

TEST_F(InputManagerTests, KeyDownAndUpUpdateState)
{
    input->ProcessKeyDown(Key::W);
    EXPECT_TRUE(input->IsKeyDown(Key::W));
    EXPECT_FALSE(input->IsKeyUp(Key::W));

    input->ProcessKeyUp(Key::W);
    EXPECT_TRUE(input->IsKeyUp(Key::W));
    EXPECT_FALSE(input->IsKeyDown(Key::W));
}

TEST_F(InputManagerTests, ClearStateReleasesPressedKeys)
{
    input->ProcessKeyDown(Key::A);
    input->ProcessKeyDown(Key::D);
    input->ClearState();
    EXPECT_TRUE(input->IsKeyUp(Key::A));
    EXPECT_TRUE(input->IsKeyUp(Key::D));
}

TEST_F(InputManagerTests, KeyBindingReceivesMatchingPressEvent)
{
    int calls = 0;
    KeyEvent received = KeyEvent::Unknown;
    const BindingID id = input->BindKeyEvent(Key::Space, KeyEvent::Down,
        [&](KeyEvent event) {
            ++calls;
            received = event;
        });

    input->ProcessKeyDown(Key::Space);

    EXPECT_EQ(calls, 1);
    EXPECT_EQ(received, KeyEvent::Down);
    input->UnBindKeyEvent(id);
}

TEST_F(InputManagerTests, UnbindingKeyEventStopsCallback)
{
    int calls = 0;
    const BindingID id = input->BindKeyEvent(Key::W, KeyEvent::Down,
        [&](KeyEvent) {
            ++calls;
        });

    input->UnBindKeyEvent(id);
    input->ProcessKeyDown(Key::W);
    EXPECT_EQ(calls, 0);
}

TEST_F(InputManagerTests, MouseButtonDownAndUpUpdateState)
{
    input->ProcessMouseButtonDown(MouseButton::Left);
    EXPECT_TRUE(input->IsMouseButtonDown(MouseButton::Left));
    EXPECT_FALSE(input->IsMouseButtonUp(MouseButton::Left));

    input->ProcessMouseButtonUp(MouseButton::Left);
    EXPECT_TRUE(input->IsMouseButtonUp(MouseButton::Left));
}

TEST_F(InputManagerTests, MouseMoveUpdatesPosition)
{
    input->ProcessMouseMove(120, 240);
    const MouseState state = input->GetMouseState();
    EXPECT_EQ(state.x, 120);
    EXPECT_EQ(state.y, 240);
}

TEST_F(InputManagerTests, MouseMoveBindingReceivesState)
{
    int calls = 0;
    MouseState received{};
    const BindingID id = input->BindMouseMoveEvent(
        [&](MouseState state) {
            ++calls;
            received = state;
        });

    input->ProcessMouseMove(30, 45);

    EXPECT_EQ(calls, 1);
    EXPECT_EQ(received.x, 30);
    EXPECT_EQ(received.y, 45);
    input->UnBindMouseMoveEvent(id);
}

TEST_F(InputManagerTests, MouseButtonBindingReceivesDownEvent)
{
    int calls = 0;
    MouseEvent received = MouseEvent::Move;
    const BindingID id = input->BindMouseButtonEvent(MouseButton::Right, MouseEvent::ButtonDown,
        [&](MouseEvent event) {
            ++calls;
            received = event;
        });

    input->ProcessMouseButtonDown(MouseButton::Right);

    EXPECT_EQ(calls, 1);
    EXPECT_EQ(received, MouseEvent::ButtonDown);
    input->UnBindMouseButtonEvent(id);
}

TEST_F(InputManagerTests, ScrollBindingReceivesDelta)
{
    int calls = 0;
    int receivedDelta = 0;
    const BindingID id = input->BindMouseScrollEvent(MouseEvent::ScrollVertical,
        [&](int delta) {
            ++calls;
            receivedDelta = delta;
        });

    input->ProcessMouseScroll(MouseEvent::ScrollVertical, 120);

    EXPECT_EQ(calls, 1);
    EXPECT_EQ(receivedDelta, 120);
    input->UnBindMouseScrollEvent(id);
}
