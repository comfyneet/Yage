#ifndef YAGE_CORE_INPUT_H
#define YAGE_CORE_INPUT_H

#include "core/Config.h"
#include "core/Math3D.h"
#include "core/NonCopyable.h"

namespace ye
{
    class Window;

    class YE_API Input : public NonCopyable
    {
    public:
        static int32_t const NUM_KEYS = 256;
        static int32_t const NUM_MOUSEBUTTONS = 8;

        explicit Input(Window* window) :
            m_mouseX{},
            m_mouseY{},
            m_window{window}
        {
        }

        bool GetKey(int32_t keyCode) const { return m_inputs[keyCode]; }
        bool GetKeyDown(int32_t keyCode) const { return m_downKeys[keyCode]; }
        bool GetKeyUp(int32_t keyCode) const { return m_upKeys[keyCode]; }
        bool GetMouse(int32_t keyCode) const { return m_mouseInput[keyCode]; }
        bool GetMouseDown(int32_t keyCode) const { return m_downMouse[keyCode]; }
        bool GetMouseUp(int32_t keyCode) const { return m_upKeys[keyCode]; }

        Vector2f GetMousePosition() const
        {
            return Vector2f(static_cast<float>(m_mouseX), static_cast<float>(m_mouseY));
        }

        void SetCursor(bool visible) const;
        void SetMousePosition(Vector2f const& pos) const;

        void SetKey(const int32_t keyCode, const bool value) { m_inputs[keyCode] = value; }
        void SetKeyDown(const int32_t keyCode, const bool value) { m_downKeys[keyCode] = value; }
        void SetKeyUp(const int32_t keyCode, const bool value) { m_upKeys[keyCode] = value; }
        void SetMouse(const int32_t keyCode, const bool value) { m_mouseInput[keyCode] = value; }
        void SetMouseDown(const int32_t keyCode, const bool value) { m_downMouse[keyCode] = value; }
        void SetMouseUp(const int32_t keyCode, const bool value) { m_upMouse[keyCode] = value; }
        void SetMouseX(const int32_t value) { m_mouseX = value; }
        void SetMouseY(const int32_t value) { m_mouseY = value; }


    private:
        std::array<bool, NUM_KEYS> m_inputs;
        std::array<bool, NUM_KEYS> m_downKeys;
        std::array<bool, NUM_KEYS> m_upKeys;
        std::array<bool, NUM_MOUSEBUTTONS> m_mouseInput;
        std::array<bool, NUM_MOUSEBUTTONS> m_downMouse;
        std::array<bool, NUM_MOUSEBUTTONS> m_upMouse;
        int32_t m_mouseX;
        int32_t m_mouseY;
        Window* m_window;
    };
}

#endif
