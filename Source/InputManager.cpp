#include "InputManager.h"
#include "Application.h"
#include "framework.h"
#include <algorithm>
#include <strsafe.h>

namespace Forge
{
	InputManager::InputManager()
	{
		pressed_keys = std::unordered_set<Key>();
	}

	InputManager::~InputManager()
	{
	}

	// Provides global access to the application's InputManager instance.
	InputManager* InputManager::Get()
	{
		return Application::Get()->GetInputManager();
	}

	// Processes a key release and notifies all bindings registered for that key.
	void InputManager::ProcessKeyUp(Key key)
	{
		// Ignore the event if the key is already released.
		if (IsKeyUp(key))
			return;

		// Remove the key from the set of currently pressed keys.
		pressed_keys.erase(key);

		wchar_t buffer[256];
		swprintf_s(buffer, _countof(buffer), L"InputManager: %s was released.\n", KeyToString(key));
		OutputDebugStringW(buffer);

		// Check whether any callbacks are bound to this key's release event.
		auto it = _boundKeyEventsReleased.find(key);
		if (it == _boundKeyEventsReleased.end())
			return;

		// Notify all callbacks bound to this key release.
		BroadcastKeyEvent(key, KeyEvent::Up);
	}

	// Processes a key press and notifies all bindings registered for that key.
	void InputManager::ProcessKeyDown(Key key)
	{
		// Ignore the event if the key is already pressed.
		if (IsKeyDown(key))
			return;

		// Add the key to the set of currently pressed keys.
		pressed_keys.insert(key);

		wchar_t buffer[256];
		swprintf_s(buffer, _countof(buffer), L"InputManager: %s was pressed.\n", KeyToString(key));
		OutputDebugStringW(buffer);

		// Check whether any callbacks are bound to this key's press event.
		auto it = _boundKeyEventsPressed.find(key);
		if (it == _boundKeyEventsPressed.end())
			return;

		// Notify all callbacks bound to this key press.
		BroadcastKeyEvent(key, KeyEvent::Down);
	}

	// Updates the state of a key and only processes the event when the
	// requested state differs from the current state.
	void InputManager::SetKeyState(Key key, KeyEvent state)
	{
		if (state == KeyEvent::Down && !IsKeyDown(key))
			ProcessKeyDown(key);
		else if (state == KeyEvent::Up && IsKeyDown(key))
			ProcessKeyUp(key);
	}

	// Processes a mouse button release and notifies all registered bindings.
	void InputManager::ProcessMouseButtonUp(MouseButton button)
	{
		// Ignore the event if the button is already released.
		if (IsMouseButtonUp(button))
			return;

		// Remove the button from the set of currently pressed mouse buttons.
		_pressedMouseButtons.erase(button);

		wchar_t buffer[256];
		swprintf_s(buffer, _countof(buffer), L"InputManager: %s was released.\n", MouseButtonToString(button));
		OutputDebugStringW(buffer);

		// Check whether any callbacks are bound to this button's release event.
		auto it = _boundMouseButtonEventsReleased.find(button);
		if (it == _boundMouseButtonEventsReleased.end())
			return;

		// Notify all callbacks bound to this mouse button release.
		BroadcastMouseButtonEvent(button, MouseEvent::ButtonUp);
	}

	// Processes a mouse button press and notifies all registered bindings.
	void InputManager::ProcessMouseButtonDown(MouseButton button)
	{
		// Ignore the event if the button is already pressed.
		if (IsMouseButtonDown(button))
			return;

		// Add the button to the set of currently pressed mouse buttons.
		_pressedMouseButtons.insert(button);

		wchar_t buffer[256];
		swprintf_s(buffer, _countof(buffer), L"InputManager: %s was pressed.\n", MouseButtonToString(button));
		OutputDebugStringW(buffer);

		// Check whether any callbacks are bound to this button's press event.
		auto it = _boundMouseButtonEventsPressed.find(button);
		if (it == _boundMouseButtonEventsPressed.end())
			return;

		// Notify all callbacks bound to this mouse button press.
		BroadcastMouseButtonEvent(button, MouseEvent::ButtonDown);
	}

	// Updates the state of a mouse button and only processes the event when
	// the requested state differs from the current state.
	void InputManager::SetMouseButtonState(MouseButton button, MouseEvent state)
	{
		if (state == MouseEvent::ButtonDown && !IsMouseButtonDown(button))
			ProcessMouseButtonDown(button);
		else if (state == MouseEvent::ButtonUp && IsMouseButtonUp(button))
			ProcessMouseButtonUp(button);
	}

	// Updates the current mouse position and calculates movement since the
	// previous mouse event.
	void InputManager::ProcessMouseMove(int xPos, int yPos)
	{
		_mouseState.deltaX = xPos - _mouseState.x;
		_mouseState.deltaY = yPos - _mouseState.y;
		_mouseState.x = xPos;
		_mouseState.y = yPos;

		wchar_t buffer[256];
		swprintf_s(buffer, _countof(buffer), L"InputManager: mouse was moved (X: %d, Y: %d, delta X: %d, delta Y: %d).\n", _mouseState.x, _mouseState.y, _mouseState.deltaX, _mouseState.deltaY);
		OutputDebugStringW(buffer);

		// Notify all callbacks that are listening for mouse movement.
		BroadcastMouseMoveEvent(_mouseState);
	}

	// Processes a mouse scroll event and notifies callbacks registered for
	// the corresponding vertical or horizontal scroll direction.
	void InputManager::ProcessMouseScroll(MouseEvent direction, int delta)
	{
		wchar_t buffer[256];
		swprintf_s(buffer, _countof(buffer), L"InputManager: mouse was scrolled %s (delta: %d).\n", direction == MouseEvent::ScrollHorizontal ? L"Horizontal" : L"Vertical", delta);
		OutputDebugStringW(buffer);

		BroadcastMouseScrollEvent(direction, delta);
	}

	// Registers a callback for a specific keyboard key event.
	BindingID InputManager::BindKeyEvent(Key key, KeyEvent event, KeyCallback callback)
	{
		// Only key press and release events can be bound here.
		if (event != KeyEvent::Up && event != KeyEvent::Down)
			return 0;

		// Select the binding map corresponding to the requested key event.
		auto& events = event == KeyEvent::Up ? _boundKeyEventsReleased : _boundKeyEventsPressed;

		// Generate a globally unique ID for this binding.
		BindingID new_id = ++_nextBindingID;

		// Find existing bindings for this key.
		auto it = events.find(key);
		if (it == events.end())
		{
			// No bindings exist for this key, so create a new binding list.
			std::vector<Binding<KeyCallback>> new_bindings;
			new_bindings.emplace_back(new_id, std::move(callback));
			events[key] = std::move(new_bindings);
		}
		else
		{
			// Add the callback to the existing bindings for this key.
			it->second.emplace_back(new_id, std::move(callback));
		}

		return new_id;
	}

	// Removes a keyboard binding using its unique BindingID.
	void InputManager::UnBindKeyEvent(BindingID id)
	{
		// Search all key release bindings for the specified ID.
		for (auto it = _boundKeyEventsReleased.begin();
			it != _boundKeyEventsReleased.end();)
		{
			auto& bindings = it->second;

			// Remove the binding with the matching ID.
			bindings.erase(
				std::remove_if(
					bindings.begin(),
					bindings.end(),
					[id](const auto& current)
					{
						return current.id == id;
					}
				),
				bindings.end()
			);

			// Remove the key from the map if it no longer has any bindings.
			if (bindings.empty())
			{
				it = _boundKeyEventsReleased.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Search all key press bindings for the specified ID.
		for (auto it = _boundKeyEventsPressed.begin();
			it != _boundKeyEventsPressed.end();)
		{
			auto& bindings = it->second;

			// Remove the binding with the matching ID.
			bindings.erase(
				std::remove_if(
					bindings.begin(),
					bindings.end(),
					[id](const auto& current)
					{
						return current.id == id;
					}
				),
				bindings.end()
			);

			// Remove the key from the map if it no longer has any bindings.
			if (bindings.empty())
			{
				it = _boundKeyEventsPressed.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Registers a callback for a specific mouse button event.
	BindingID InputManager::BindMouseButtonEvent(MouseButton button, MouseEvent event, MouseButtonCallback callback)
	{
		// Only mouse button press and release events can be bound here.
		if (event != MouseEvent::ButtonUp && event != MouseEvent::ButtonDown)
			return 0;

		// Select the binding map corresponding to the requested button event.
		auto& events = event == MouseEvent::ButtonUp ? _boundMouseButtonEventsReleased : _boundMouseButtonEventsPressed;

		// Generate a globally unique ID for this binding.
		BindingID new_id = ++_nextBindingID;

		// Find existing bindings for this mouse button.
		auto it = events.find(button);
		if (it == events.end())
		{
			// No bindings exist for this button, so create a new binding list.
			std::vector<Binding<MouseButtonCallback>> new_bindings;
			new_bindings.emplace_back(new_id, std::move(callback));
			events[button] = std::move(new_bindings);
		}
		else
		{
			// Add the callback to the existing bindings for this button.
			it->second.emplace_back(new_id, std::move(callback));
		}

		return new_id;
	}

	// Removes a mouse button binding using its unique BindingID.
	void InputManager::UnBindMouseButtonEvent(BindingID id)
	{
		// Search all mouse button press bindings for the specified ID.
		for (auto it = _boundMouseButtonEventsPressed.begin();
			it != _boundMouseButtonEventsPressed.end();)
		{
			auto& bindings = it->second;

			// Remove the binding with the matching ID.
			bindings.erase(
				std::remove_if(
					bindings.begin(),
					bindings.end(),
					[id](const auto& current)
					{
						return current.id == id;
					}
				),
				bindings.end()
			);

			// Remove the button from the map if it no longer has any bindings.
			if (bindings.empty())
			{
				it = _boundMouseButtonEventsPressed.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Search all mouse button release bindings for the specified ID.
		for (auto it = _boundMouseButtonEventsReleased.begin();
			it != _boundMouseButtonEventsReleased.end();)
		{
			auto& bindings = it->second;

			// Remove the binding with the matching ID.
			bindings.erase(
				std::remove_if(
					bindings.begin(),
					bindings.end(),
					[id](const auto& current)
					{
						return current.id == id;
					}
				),
				bindings.end()
			);

			// Remove the button from the map if it no longer has any bindings.
			if (bindings.empty())
			{
				it = _boundMouseButtonEventsReleased.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Registers a callback for mouse movement.
	BindingID InputManager::BindMouseMoveEvent(MouseMoveCallback callback)
	{
		// Generate a globally unique ID for this binding.
		BindingID new_id = ++_nextBindingID;

		_boundMouseMoveEvents.emplace_back(new_id, std::move(callback));

		return new_id;
	}

	// Removes a mouse movement binding using its unique BindingID.
	void InputManager::UnBindMouseMoveEvent(BindingID id)
	{
		_boundMouseMoveEvents.erase(std::remove_if(
			_boundMouseMoveEvents.begin(),
			_boundMouseMoveEvents.end(),
			[id](const auto& current)
			{
				return current.id == id;
			}
		),
			_boundMouseMoveEvents.end()
		);
	}

	// Registers a callback for either vertical or horizontal mouse scrolling.
	BindingID InputManager::BindMouseScrollEvent(MouseEvent scroll_event, MouseScrollCallback callback)
	{
		// Only vertical and horizontal scroll events can be bound here.
		if (scroll_event != MouseEvent::ScrollVertical && scroll_event != MouseEvent::ScrollHorizontal)
			return 0;

		// Select the binding list corresponding to the requested scroll direction.
		auto& event = scroll_event == MouseEvent::ScrollVertical
			? _boundMouseScrollVerticalEvents
			: _boundMouseScrollHorizontalEvents;

		// Generate a globally unique ID for this binding.
		BindingID new_id = ++_nextBindingID;

		event.emplace_back(new_id, std::move(callback));

		return new_id;
	}

	// Removes a mouse scroll binding using its unique BindingID.
	void InputManager::UnBindMouseScrollEvent(BindingID id)
	{
		// Search vertical scroll bindings.
		_boundMouseScrollVerticalEvents.erase(std::remove_if(
			_boundMouseScrollVerticalEvents.begin(),
			_boundMouseScrollVerticalEvents.end(),
			[id](const auto& current)
			{
				return current.id == id;
			}
		),
			_boundMouseScrollVerticalEvents.end()
		);

		// Search horizontal scroll bindings.
		_boundMouseScrollHorizontalEvents.erase(std::remove_if(
			_boundMouseScrollHorizontalEvents.begin(),
			_boundMouseScrollHorizontalEvents.end(),
			[id](const auto& current)
			{
				return current.id == id;
			}
		),
			_boundMouseScrollHorizontalEvents.end()
		);
	}

	// Set each key and mouse button as up
	// Used when the window leaves focus
	void InputManager::ClearState()
	{
		// use copy to avoid deleting while iterating
		auto pressed_keys_copy = pressed_keys;
		for (const auto& key : pressed_keys_copy)
			ProcessKeyUp(key);

		auto pressed_mouse_buttons_copy = _pressedMouseButtons;
		for (const auto& mouse_button : pressed_mouse_buttons_copy)
			ProcessMouseButtonUp(mouse_button);
	}

	// Broadcasts a keyboard event to all callbacks registered for the specified key.
	void InputManager::BroadcastKeyEvent(Key key, KeyEvent event) const
	{
		// Select the binding map corresponding to the event type.
		auto events = event == KeyEvent::Up
			? &_boundKeyEventsReleased
			: &_boundKeyEventsPressed;

		auto it = events->find(key);
		if (it == events->end())
			return;

		// Notify every callback registered for this key event.
		for (const auto& binding : it->second)
		{
			binding.callback(event);
		}
	}

	// Broadcasts a mouse button event to all callbacks registered for the button.
	void InputManager::BroadcastMouseButtonEvent(MouseButton button, MouseEvent event) const
	{
		// Select the binding map corresponding to the event type.
		auto events = event == MouseEvent::ButtonUp
			? &_boundMouseButtonEventsReleased
			: &_boundMouseButtonEventsPressed;

		auto it = events->find(button);
		if (it == events->end())
			return;

		// Notify every callback registered for this mouse button event.
		for (const auto& binding : it->second)
		{
			binding.callback(event);
		}
	}

	// Broadcasts the current mouse state to all mouse movement callbacks.
	void InputManager::BroadcastMouseMoveEvent(MouseState state) const
	{
		for (const auto& binding : _boundMouseMoveEvents)
		{
			binding.callback(state);
		}
	}

	// Broadcasts a scroll event to callbacks registered for the specified direction.
	void InputManager::BroadcastMouseScrollEvent(MouseEvent direction, int delta) const
	{
		if (direction == MouseEvent::ScrollVertical)
		{
			for (const auto& binding : _boundMouseScrollVerticalEvents)
			{
				binding.callback(delta);
			}
		}
		else if (direction == MouseEvent::ScrollHorizontal)
		{
			for (const auto& binding : _boundMouseScrollHorizontalEvents)
			{
				binding.callback(delta);
			}
		}
	}

	// Returns whether the specified key is currently released.
	bool InputManager::IsKeyUp(Key key) const
	{
		return !IsKeyDown(key);
	}

	// Returns whether the specified key is currently pressed.
	bool InputManager::IsKeyDown(Key key) const
	{
		return pressed_keys.count(key);
	}

	// Returns whether the specified mouse button is currently released.
	bool InputManager::IsMouseButtonUp(MouseButton button) const
	{
		return !IsMouseButtonDown(button);
	}

	// Returns whether the specified mouse button is currently pressed.
	bool InputManager::IsMouseButtonDown(MouseButton button) const
	{
		return _pressedMouseButtons.count(button);
	}

	// Returns a copy of the current mouse state.
	MouseState InputManager::GetMouseState() const
	{
		return _mouseState;
	}
}