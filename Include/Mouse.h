#pragma once
#include "framework.h"

namespace Forge
{
    /** 
    * @brief Identifies mouse movement, button, and scroll events.
    */
    enum class MouseEvent
    {
        Move,
        ButtonDown,
        ButtonUp,
        ScrollVertical,
        ScrollHorizontal
    };

    /** 
    * @brief Identifies a mouse button.
    */
    enum class MouseButton
    {
        Unkown = 0,

        Left,
        Right,
        Middle,
        X1,
        X2,

        Count
    };

    /**
    * @brief Used to store the current state of the mouse
    */
    struct FORGE_API MouseState
    {
        /**
        * @brief The screen x position.
        */
        int x = 0;

        /**
        * @brief The screen y position.
        */
        int y = 0;

        /**
        * @brief The last change in mouse screen x position.
        */
        int deltaX = 0;

        /**
        * @brief The last change in mouse screen y position.
        */
        int deltaY = 0;
    };

    /**
    * @brief Converts a windows virtual key for mouse buttons to the MouseButton enum value.
    * 
    * @param key The windows virtual key to convert.
    * @return The enum representation of the virtual key.
    */
    FORGE_API MouseButton MouseMessageToMouseButton(WPARAM key);

    /**
     * @brief Converts a mouse button enum value to a wide string.
     * 
     * @param button The mouse button enum value to convert.
     * @return The enum value described as a wide string.
     */
    FORGE_API const wchar_t* MouseButtonToString(MouseButton button);
}
