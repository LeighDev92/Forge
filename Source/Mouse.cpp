#pragma once
#include "Mouse.h"

namespace Forge
{
    MouseButton MouseMessageToMouseButton(WPARAM key)
    {
        switch (key)
        {
        case VK_LBUTTON:  return MouseButton::Left;
        case VK_RBUTTON:  return MouseButton::Right;
        case VK_MBUTTON:  return MouseButton::Middle;
        case VK_XBUTTON1: return MouseButton::X1;
        case VK_XBUTTON2: return MouseButton::X2;

        default:          return MouseButton::Unkown;
        }
    }

    const wchar_t* MouseButtonToString(MouseButton button)
    {
        switch (button)
        {
        case MouseButton::Left:   return L"Mouse Left";
        case MouseButton::Right:  return L"Mouse Right";
        case MouseButton::Middle: return L"Mouse Middle";
        case MouseButton::X1:     return L"Mouse X1";
        case MouseButton::X2:     return L"Mouse X2";

        default:                  return L"Unkown";
        }
    }
}