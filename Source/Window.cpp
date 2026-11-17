#include "Window.h"
#include "Application.h"
#include "InputManager.h"

namespace Forge
{
    Window::Window()
    {
        Register();
        Create();
        Show();
        Update();
    }

    Window::~Window()
    {
    }

    ATOM Window::Register()
    {
        Application* app = Application::Get();
        HINSTANCE hInstance = app->GetHInstance();

        WNDCLASSEXW wcex = { 0 };

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FORGE));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FORGE);
        wcex.lpszClassName = app->GetMainWindowClass();
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    BOOL Window::Create()
    {
        Application* app = Application::Get();

        hWnd = CreateWindowW(app->GetMainWindowClass(), app->GetTitle(), WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, app->GetHInstance(), this);

        if (!hWnd)
        {
            return FALSE;
        }

        return TRUE;
    }

    BOOL Window::Show() const
    {
        return ShowWindow(hWnd, Application::Get()->GetCMDShow());
    }

    BOOL Window::Update() const
    {
        return UpdateWindow(hWnd);
    }

    HWND Window::GetHWND()
    {
        return hWnd;
    }

    LRESULT Window::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_NCCREATE)
        {
            // extract pointer to class object passed into CreateWindw()
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Window* pWnd = static_cast<Window*>(pCreate->lpCreateParams);

            // set WinAPI-managed user data to store pointer to window class
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndProcThunk));
            return pWnd->HandleMessage(hWnd, message, wParam, lParam);
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    LRESULT WINAPI Window::WndProcThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        // retrieve pointer to window class
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        // have the stored window class instance handle the message
        
        if(pWnd)
            return pWnd->HandleMessage(hWnd, message, wParam, lParam);

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    INT_PTR CALLBACK Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
        UNREFERENCED_PARAMETER(lParam);
        switch (message)
        {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        return (INT_PTR)FALSE;
    }
}