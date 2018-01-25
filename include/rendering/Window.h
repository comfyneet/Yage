#ifndef YAGE_RENDERING_WINDOW_H
#define YAGE_RENDERING_WINDOW_H

#include "core/Config.h"
#include "core/Input.h"
#include "core/NonCopyable.h"
#include "core/Win32Common.h"

namespace ye
{
    class YE_API Window : public NonCopyable
    {
    public:
        Window(int32_t width, int32_t height, std::string const& title);
        virtual ~Window();

        void Update();
        LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        bool IsCloseRequested() const { return m_isCloseRequested; }
        int32_t GetWidth() const { return m_width; }
        int32_t GetHeight() const { return m_height; }
        float GetAspect() const { return static_cast<float>(m_width) / m_height; }
        HWND GetHwnd() const { return m_hWnd; }
        Input const& GetInput() const { return m_input; }

    private:
        static HINSTANCE s_hInstance;
        int32_t m_width;
        int32_t m_height;
        std::string m_title;
        HWND m_hWnd;
        Input m_input;
        bool m_isCloseRequested;
    };
}

#endif
