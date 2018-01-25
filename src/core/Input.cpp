#include "core/Input.h"
#include "rendering/Window.h"

namespace ye
{
    void Input::SetCursor(bool visible) const
    {
        if (visible)
        {
            ShowCursor(true);
        }
        else
        {
            ShowCursor(false);
        }
    }

    void Input::SetMousePosition(Vector2f const& pos) const
    {
        SetCursorPos(pos.GetX(), pos.GetY());
    }
}
