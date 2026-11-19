
/**
 * @file InputManager.h
 * @brief Defines the input management system for keyboard and mouse events.
 *
 * Provides input state tracking and event binding functionality. Callbacks
 * can be registered for keyboard events, mouse button events, mouse movement,
 * and mouse scrolling.
 */

#pragma once

#include <unordered_set>
#include <functional>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include "Keys.h"
#include "Mouse.h"

namespace Forge
{
    /**
     * @brief Unique identifier assigned to an input event binding.
     *
     * Used to identify and remove previously registered callbacks.
     */
    using BindingID = uint64_t;

    /**
     * @brief Callback type for keyboard events.
     * @param event The keyboard event being broadcast.
     */
    using KeyCallback = std::function<void(KeyEvent event)>;

    /**
     * @brief Callback type for mouse movement events.
     * @param state The current mouse state, including position and movement delta.
     */
    using MouseMoveCallback = std::function<void(MouseState state)>;

    /**
     * @brief Callback type for mouse button events.
     * @param event The mouse button event being broadcast.
     */
    using MouseButtonCallback = std::function<void(MouseEvent event)>;

    /**
     * @brief Callback type for mouse scroll events.
     * @param delta The scroll delta reported by the input event.
     */
    using MouseScrollCallback = std::function<void(int delta)>;

    /**
     * @brief Stores a callback and its associated binding identifier.
     *
     * @tparam T The callback type stored by the binding.
     */
    template<typename T>
    struct FORGE_API Binding
    {
        /** Unique identifier assigned to this binding. */
        BindingID id;

        /** Callback function associated with this binding. */
        T callback;

        /**
         * @brief Constructs a callback binding.
         *
         * @param _id Unique identifier assigned to the binding.
         * @param _callback Callback function to associate with the binding.
         */
        Binding(BindingID _id, T _callback)
        {
            id = _id;
            callback = std::move(_callback);
        }
    };

    /**
     * @brief Manages keyboard and mouse input state and event callbacks.
     *
     * InputManager tracks pressed keyboard keys and mouse buttons, maintains
     * the current mouse state, and allows callbacks to be registered for
     * specific input events.
     *
     * Registered callbacks can be removed using their associated BindingID.
     */
    class FORGE_API InputManager
    {
    public:

        /**
         * @brief Constructs the input manager.
         */
        InputManager();

        /**
         * @brief Destroys the input manager.
         */
        ~InputManager();

        /**
         * @brief Retrieves the InputManager singleton instance.
         * @return Pointer to the InputManager instance.
         */
        static InputManager* Get();

        /**
         * @brief Processes a keyboard key release.
         * @param The key that was released.
         */
        void ProcessKeyUp(Key key);

        /**
         * @brief Processes a keyboard key press.
         * @param The key that was pressed.
         */
        void ProcessKeyDown(Key key);

        /**
         * @brief Updates the state of a keyboard key.
         * @param key The key whose state is being updated.
         * @param state The new keyboard event state.
         */
        void SetKeyState(Key key, KeyEvent state);

        /**
         * @brief Processes a mouse button release.
         * @param button The mouse button that was released.
         */
        void ProcessMouseButtonUp(MouseButton button);

        /**
         * @brief Processes a mouse button press.
         * @param button The mouse button that was pressed.
         */
        void ProcessMouseButtonDown(MouseButton button);

        /**
         * @brief Updates the state of a mouse button.
         * @param button The mouse button whose state is being updated.
         * @param state The new mouse button event state.
         */
        void SetMouseButtonState(MouseButton button, MouseEvent state);

        /**
         * @brief Processes mouse movement.
         *
         * Updates the mouse position using the supplied coordinates.
         *
         * @param xPos The horizontal mouse position.
         * @param yPos The vertical mouse position.
         */
        void ProcessMouseMove(int xPos, int yPos);

        /**
         * @brief Processes a mouse scroll event.
         * @param direction The scroll direction or event type.
         * @param delta The scroll amount.
         */
        void ProcessMouseScroll(MouseEvent direction, int delta);

        /**
         * @brief Checks whether a keyboard key is released.
         * @param key The key to check.
         * @return True if the key is released; otherwise, false.
         */
        bool IsKeyUp(Key key) const;

        /**
         * @brief Checks whether a keyboard key is pressed.
         * @param key The key to check.
         * @return True if the key is pressed; otherwise, false.
         */
        bool IsKeyDown(Key key) const;

        /**
         * @brief Checks whether a mouse button is released.
         * @param button The mouse button to check.
         * @return True if the button is released; otherwise, false.
         */
        bool IsMouseButtonUp(MouseButton button) const;

        /**
         * @brief Checks whether a mouse button is pressed.
         * @param button The mouse button to check.
         * @return True if the button is pressed; otherwise, false.
         */
        bool IsMouseButtonDown(MouseButton button) const;

        /**
         * @brief Retrieves the current mouse state.
         * @return The current mouse position and movement delta.
         */
        MouseState GetMouseState() const;

        /**
         * @brief Registers a callback for a keyboard event.
         *
         * The callback is associated with a specific key and event type.
         *
         * @param key The key to associate with the callback.
         * @param event The keyboard event that triggers the callback.
         * @param callback The callback function to execute.
         * @return A unique identifier for the registered binding.
         */
        BindingID BindKeyEvent(Key key, KeyEvent event, KeyCallback callback);

        /**
         * @brief Removes a previously registered keyboard event callback.
         * @param id The identifier of the binding to remove.
         */
        void UnBindKeyEvent(BindingID id);

        /**
         * @brief Registers a callback for a mouse button event.
         *
         * The callback is associated with a specific mouse button and event type.
         *
         * @param button The mouse button to associate with the callback.
         * @param event The mouse event that triggers the callback.
         * @param callback The callback function to execute.
         * @return A unique identifier for the registered binding.
         */
        BindingID BindMouseButtonEvent(MouseButton button, MouseEvent event, MouseButtonCallback callback);

        /**
         * @brief Removes a previously registered mouse button event callback.
         * @param id The identifier of the binding to remove.
         */
        void UnBindMouseButtonEvent(BindingID id);

        /**
         * @brief Registers a callback for mouse movement events.
         * @param callback The callback function to execute when the mouse moves.
         * @return A unique identifier for the registered binding.
         */
        BindingID BindMouseMoveEvent(MouseMoveCallback callback);

        /**
         * @brief Removes a previously registered mouse movement callback.
         * @param id The identifier of the binding to remove.
         */
        void UnBindMouseMoveEvent(BindingID id);

        /**
         * @brief Registers a callback for mouse scroll events.
         *
         * The callback is associated with a specific scroll direction.
         *
         * @param scroll_event The scroll direction to associate with the callback.
         * @param callback The callback function to execute.
         * @return A unique identifier for the registered binding.
         */
        BindingID BindMouseScrollEvent(MouseEvent scroll_event, MouseScrollCallback callback);

        /**
         * @brief Removes a previously registered mouse scroll callback.
         * @param id The identifier of the binding to remove.
         */
        void UnBindMouseScrollEvent(BindingID id);

        /**
         * @brief Clears the currently tracked input state.
         */
        void ClearState();

    private:

        /**
         * @brief Broadcasts a keyboard event to registered callbacks.
         * @param key The key associated with the event.
         * @param event The keyboard event being broadcast.
         */
        void BroadcastKeyEvent(Key key, KeyEvent event) const;

        /**
         * @brief Broadcasts a mouse button event to registered callbacks.
         * @param button The mouse button associated with the event.
         * @param event The mouse event being broadcast.
         */
        void BroadcastMouseButtonEvent(MouseButton button, MouseEvent event) const;

        /**
         * @brief Broadcasts a mouse movement event to registered callbacks.
         * @param state The mouse state being broadcast.
         */
        void BroadcastMouseMoveEvent(MouseState state) const;

        /**
         * @brief Broadcasts a mouse scroll event to registered callbacks.
         * @param direction The scroll direction associated with the event.
         * @param delta The scroll amount.
         */
        void BroadcastMouseScrollEvent(MouseEvent direction, int delta) const;

        std::unordered_set<Key> pressed_keys;
        std::unordered_map<Key, std::vector<Binding<KeyCallback>>> _boundKeyEventsPressed;
        std::unordered_map<Key, std::vector<Binding<KeyCallback>>> _boundKeyEventsReleased;
        MouseState _mouseState;
        std::unordered_set<MouseButton> _pressedMouseButtons;
        std::unordered_map<MouseButton, std::vector<Binding<MouseButtonCallback>>> _boundMouseButtonEventsPressed;
        std::unordered_map<MouseButton, std::vector<Binding<MouseButtonCallback>>> _boundMouseButtonEventsReleased;
        std::vector<Binding<MouseMoveCallback>> _boundMouseMoveEvents;
        std::vector<Binding<MouseScrollCallback>> _boundMouseScrollHorizontalEvents;
        std::vector<Binding<MouseScrollCallback>> _boundMouseScrollVerticalEvents;
        BindingID _nextBindingID = 0;
    };
}
