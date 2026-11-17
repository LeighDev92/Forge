#include "MainWindow.h"
#include "Application.h"
#include "InputManager.h"
#include "GraphicsManager.h"

Forge::MainWindow::MainWindow()
{
}

Forge::MainWindow::~MainWindow()
{
}

LRESULT Forge::MainWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(Application::Get()->GetHInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KILLFOCUS:
        InputManager::Get()->ClearState();
        break;
    case WM_KEYDOWN:
    {
        int keyId = LOWORD(wParam);
        InputManager::Get()->ProcessKeyDown(VirtualKeyToKey(keyId));
        break;
    }
    case WM_KEYUP:
    {
        int keyId = LOWORD(wParam);
        InputManager::Get()->ProcessKeyUp(VirtualKeyToKey(keyId));
        break;
    }
    case WM_LBUTTONUP:
    {
        InputManager::Get()->ProcessMouseButtonUp(MouseButton::Left);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        InputManager::Get()->ProcessMouseButtonDown(MouseButton::Left);
        break;
    }
    case WM_RBUTTONUP:
    {
        InputManager::Get()->ProcessMouseButtonUp(MouseButton::Right);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        InputManager::Get()->ProcessMouseButtonDown(MouseButton::Right);
        break;
    }
    case WM_MBUTTONUP:
    {
        InputManager::Get()->ProcessMouseButtonUp(MouseButton::Middle);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        InputManager::Get()->ProcessMouseButtonDown(MouseButton::Middle);
        break;
    }
    case WM_XBUTTONUP:
    {
        WORD id = GET_XBUTTON_WPARAM(wParam);
        MouseButton button = id == XBUTTON1 ? MouseButton::X1 : MouseButton::X2;
        InputManager::Get()->ProcessMouseButtonUp(button);
        break;
    }
    case WM_XBUTTONDOWN:
    {
        WORD id = GET_XBUTTON_WPARAM(wParam);
        MouseButton button = id == XBUTTON1 ? MouseButton::X1 : MouseButton::X2;
        InputManager::Get()->ProcessMouseButtonDown(button);
        break;
    }
    case WM_MOUSEMOVE:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        InputManager::Get()->ProcessMouseMove(xPos, yPos);
        break;
    }
    case WM_MOUSEHWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        InputManager::Get()->ProcessMouseScroll(MouseEvent::ScrollHorizontal, delta);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        InputManager::Get()->ProcessMouseScroll(MouseEvent::ScrollVertical, delta);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);

        if (width != 0 && height != 0)
        {
            Application::Get()->GetGraphicsManager()->Resize(width, height);
        }

        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
