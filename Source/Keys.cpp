#pragma once
#include "Keys.h"

namespace Forge
{

    Key VirtualKeyToKey(WPARAM vk)
    {
        switch (vk)
        {
        case VK_LBUTTON: return Key::MouseLeft;
        case VK_RBUTTON: return Key::MouseRight;
        case VK_MBUTTON: return Key::MouseMiddle;
        case VK_XBUTTON1: return Key::MouseX1;
        case VK_XBUTTON2: return Key::MouseX2;

        case VK_BACK:    return Key::Backspace;
        case VK_TAB:     return Key::Tab;
        case VK_RETURN:  return Key::Enter;
        case VK_SHIFT:   return Key::Shift;
        case VK_CONTROL: return Key::Control;
        case VK_MENU:    return Key::Alt;
        case VK_PAUSE:   return Key::Pause;
        case VK_CAPITAL: return Key::CapsLock;
        case VK_ESCAPE:  return Key::Escape;
        case VK_SPACE:   return Key::Space;

        case VK_PRIOR: return Key::PageUp;
        case VK_NEXT:  return Key::PageDown;
        case VK_END:   return Key::End;
        case VK_HOME:  return Key::Home;

        case VK_LEFT:  return Key::Left;
        case VK_UP:    return Key::Up;
        case VK_RIGHT: return Key::Right;
        case VK_DOWN:  return Key::Down;

        case VK_INSERT: return Key::Insert;
        case VK_DELETE: return Key::Delete;

            // Numbers
        case '0': return Key::Num0;
        case '1': return Key::Num1;
        case '2': return Key::Num2;
        case '3': return Key::Num3;
        case '4': return Key::Num4;
        case '5': return Key::Num5;
        case '6': return Key::Num6;
        case '7': return Key::Num7;
        case '8': return Key::Num8;
        case '9': return Key::Num9;

            // Letters
        case 'A': return Key::A;
        case 'B': return Key::B;
        case 'C': return Key::C;
        case 'D': return Key::D;
        case 'E': return Key::E;
        case 'F': return Key::F;
        case 'G': return Key::G;
        case 'H': return Key::H;
        case 'I': return Key::I;
        case 'J': return Key::J;
        case 'K': return Key::K;
        case 'L': return Key::L;
        case 'M': return Key::M;
        case 'N': return Key::N;
        case 'O': return Key::O;
        case 'P': return Key::P;
        case 'Q': return Key::Q;
        case 'R': return Key::R;
        case 'S': return Key::S;
        case 'T': return Key::T;
        case 'U': return Key::U;
        case 'V': return Key::V;
        case 'W': return Key::W;
        case 'X': return Key::X;
        case 'Y': return Key::Y;
        case 'Z': return Key::Z;

            // Numpad
        case VK_NUMPAD0: return Key::Numpad0;
        case VK_NUMPAD1: return Key::Numpad1;
        case VK_NUMPAD2: return Key::Numpad2;
        case VK_NUMPAD3: return Key::Numpad3;
        case VK_NUMPAD4: return Key::Numpad4;
        case VK_NUMPAD5: return Key::Numpad5;
        case VK_NUMPAD6: return Key::Numpad6;
        case VK_NUMPAD7: return Key::Numpad7;
        case VK_NUMPAD8: return Key::Numpad8;
        case VK_NUMPAD9: return Key::Numpad9;

        case VK_MULTIPLY: return Key::NumpadMultiply;
        case VK_ADD:      return Key::NumpadAdd;
        case VK_SUBTRACT: return Key::NumpadSubtract;
        case VK_DECIMAL:  return Key::NumpadDecimal;
        case VK_DIVIDE:   return Key::NumpadDivide;

            // Function keys
        case VK_F1:  return Key::F1;
        case VK_F2:  return Key::F2;
        case VK_F3:  return Key::F3;
        case VK_F4:  return Key::F4;
        case VK_F5:  return Key::F5;
        case VK_F6:  return Key::F6;
        case VK_F7:  return Key::F7;
        case VK_F8:  return Key::F8;
        case VK_F9:  return Key::F9;
        case VK_F10: return Key::F10;
        case VK_F11: return Key::F11;
        case VK_F12: return Key::F12;
        case VK_F13: return Key::F13;
        case VK_F14: return Key::F14;
        case VK_F15: return Key::F15;
        case VK_F16: return Key::F16;
        case VK_F17: return Key::F17;
        case VK_F18: return Key::F18;
        case VK_F19: return Key::F19;
        case VK_F20: return Key::F20;
        case VK_F21: return Key::F21;
        case VK_F22: return Key::F22;
        case VK_F23: return Key::F23;
        case VK_F24: return Key::F24;

        case VK_NUMLOCK:   return Key::NumLock;
        case VK_SCROLL:    return Key::ScrollLock;

            // Symbols
        case VK_OEM_1:      return Key::Semicolon;
        case VK_OEM_PLUS:   return Key::Equals;
        case VK_OEM_COMMA:  return Key::Comma;
        case VK_OEM_MINUS:  return Key::Minus;
        case VK_OEM_PERIOD: return Key::Period;
        case VK_OEM_2:      return Key::Slash;
        case VK_OEM_3:      return Key::Grave;
        case VK_OEM_4:      return Key::LeftBracket;
        case VK_OEM_5:      return Key::Backslash;
        case VK_OEM_6:      return Key::RightBracket;
        case VK_OEM_7:      return Key::Apostrophe;

        case VK_LSHIFT:   return Key::LeftShift;
        case VK_RSHIFT:   return Key::RightShift;
        case VK_LCONTROL: return Key::LeftControl;
        case VK_RCONTROL: return Key::RightControl;
        case VK_LMENU:    return Key::LeftAlt;
        case VK_RMENU:    return Key::RightAlt;

        default:
            return Key::Unknown;
        }
    }

    const wchar_t* KeyToString(Key key)
    {
        switch (key)
        {
            // Control
        case Key::Backspace: return L"Backspace";
        case Key::Tab:       return L"Tab";
        case Key::Enter:     return L"Enter";
        case Key::Shift:     return L"Shift";
        case Key::Control:   return L"Control";
        case Key::Alt:       return L"Alt";
        case Key::Pause:     return L"Pause";
        case Key::CapsLock:  return L"Caps Lock";
        case Key::Escape:    return L"Escape";
        case Key::Space:     return L"Space";

        case Key::PageUp:   return L"Page Up";
        case Key::PageDown: return L"Page Down";
        case Key::End:      return L"End";
        case Key::Home:     return L"Home";

        case Key::Left:  return L"Left";
        case Key::Up:    return L"Up";
        case Key::Right: return L"Right";
        case Key::Down:  return L"Down";

        case Key::Insert: return L"Insert";
        case Key::Delete: return L"Delete";

            // Numbers
        case Key::Num0: return L"0";
        case Key::Num1: return L"1";
        case Key::Num2: return L"2";
        case Key::Num3: return L"3";
        case Key::Num4: return L"4";
        case Key::Num5: return L"5";
        case Key::Num6: return L"6";
        case Key::Num7: return L"7";
        case Key::Num8: return L"8";
        case Key::Num9: return L"9";

            // Letters
        case Key::A: return L"A";
        case Key::B: return L"B";
        case Key::C: return L"C";
        case Key::D: return L"D";
        case Key::E: return L"E";
        case Key::F: return L"F";
        case Key::G: return L"G";
        case Key::H: return L"H";
        case Key::I: return L"I";
        case Key::J: return L"J";
        case Key::K: return L"K";
        case Key::L: return L"L";
        case Key::M: return L"M";
        case Key::N: return L"N";
        case Key::O: return L"O";
        case Key::P: return L"P";
        case Key::Q: return L"Q";
        case Key::R: return L"R";
        case Key::S: return L"S";
        case Key::T: return L"T";
        case Key::U: return L"U";
        case Key::V: return L"V";
        case Key::W: return L"W";
        case Key::X: return L"X";
        case Key::Y: return L"Y";
        case Key::Z: return L"Z";

            // Numpad
        case Key::Numpad0:        return L"Numpad 0";
        case Key::Numpad1:        return L"Numpad 1";
        case Key::Numpad2:        return L"Numpad 2";
        case Key::Numpad3:        return L"Numpad 3";
        case Key::Numpad4:        return L"Numpad 4";
        case Key::Numpad5:        return L"Numpad 5";
        case Key::Numpad6:        return L"Numpad 6";
        case Key::Numpad7:        return L"Numpad 7";
        case Key::Numpad8:        return L"Numpad 8";
        case Key::Numpad9:        return L"Numpad 9";

        case Key::NumpadMultiply: return L"Numpad *";
        case Key::NumpadAdd:      return L"Numpad +";
        case Key::NumpadSubtract: return L"Numpad -";
        case Key::NumpadDecimal:  return L"Numpad .";
        case Key::NumpadDivide:   return L"Numpad /";

            // Function keys
        case Key::F1:  return L"F1";
        case Key::F2:  return L"F2";
        case Key::F3:  return L"F3";
        case Key::F4:  return L"F4";
        case Key::F5:  return L"F5";
        case Key::F6:  return L"F6";
        case Key::F7:  return L"F7";
        case Key::F8:  return L"F8";
        case Key::F9:  return L"F9";
        case Key::F10: return L"F10";
        case Key::F11: return L"F11";
        case Key::F12: return L"F12";
        case Key::F13: return L"F13";
        case Key::F14: return L"F14";
        case Key::F15: return L"F15";
        case Key::F16: return L"F16";
        case Key::F17: return L"F17";
        case Key::F18: return L"F18";
        case Key::F19: return L"F19";
        case Key::F20: return L"F20";
        case Key::F21: return L"F21";
        case Key::F22: return L"F22";
        case Key::F23: return L"F23";
        case Key::F24: return L"F24";

            // Lock keys
        case Key::NumLock:    return L"Num Lock";
        case Key::ScrollLock: return L"Scroll Lock";

            // Symbols
        case Key::Semicolon:    return L";";
        case Key::Equals:       return L"=";
        case Key::Comma:        return L",";
        case Key::Minus:        return L"-";
        case Key::Period:       return L".";
        case Key::Slash:        return L"/";
        case Key::Grave:        return L"`";
        case Key::LeftBracket:  return L"[";
        case Key::Backslash:    return L"\\";
        case Key::RightBracket: return L"]";
        case Key::Apostrophe:   return L"'";

            // Modifier keys
        case Key::LeftShift:    return L"Left Shift";
        case Key::RightShift:   return L"Right Shift";
        case Key::LeftControl:  return L"Left Control";
        case Key::RightControl: return L"Right Control";
        case Key::LeftAlt:      return L"Left Alt";
        case Key::RightAlt:     return L"Right Alt";

        default:                return L"Unknown";
        }
    }
}