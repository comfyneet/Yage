#include "rendering/Camera.h"

namespace ye
{
    Matrix4f Camera::GetViewProjection() const
    {
        return m_projection;
    }
}
