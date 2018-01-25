#include "rendering/Window.h"
#include "core/String.h"

namespace
{
    std::wstring _windowClassName = L"WindowClass";
    ye::Window* _window{};

    LRESULT CALLBACK _WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
    {
        assert(_window);

        return _window->WndProc(hWnd, message, wParam, lParam);
    }
}

namespace ye
{
    HINSTANCE Window::s_hInstance{};

    Window::Window(const int32_t width, const int32_t height, std::string const& title) :
        m_width{width},
        m_height{height},
        m_title{title},
        m_hWnd{},
        m_input{this},
        m_isCloseRequested{}
    {
        _window = this;

        s_hInstance = GetModuleHandle(nullptr);

        WNDCLASSEX wcex{};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = _WndProc;
        wcex.hInstance = s_hInstance;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.lpszClassName = _windowClassName.c_str();

        RegisterClassEx(&wcex);

        RECT wr = {0, 0, m_width, m_height};
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

        m_hWnd = CreateWindowEx(0, _windowClassName.c_str(), String::Widen(m_title).c_str(), WS_OVERLAPPEDWINDOW, 500,
                                200, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, s_hInstance, nullptr);

        ShowWindow(m_hWnd, SW_SHOW);
    }

    Window::~Window()
    {
        DestroyWindow(m_hWnd);

        UnregisterClass(_windowClassName.c_str(), s_hInstance);
    }

    void Window::Update()
    {
        MSG msg = {};
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // TODO:
        if (msg.message == WM_QUIT /*|| m_input.GetKey(VK_ESCAPE)*/)
        {
            m_isCloseRequested = true;
        }
    }

    LRESULT Window::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
    {
        switch (message)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            m_input.SetKey(wParam, true);
            return 0;

        case WM_KEYUP:
            m_input.SetKey(wParam, false);
            return 0;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
}
