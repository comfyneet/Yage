#ifndef YAGE_CORE_TRANSFORM_H
#define YAGE_CORE_TRANSFORM_H

#include "core/Config.h"
#include "core/Math3D.h"

namespace ye
{
    class YE_API Transform
    {
    public:
        explicit Transform(Vector3f const& position = Vector3f(), const float scale = 1.0f) :
            m_scale{scale}
        {
            m_position = position;
        }

        Vector3f const& GetPosition() const { return m_position; }
        float GetScale() const { return m_scale; }

        void SetPosition(Vector3f const& position) { m_position = position; }
        void SetScale(const float scale) { m_scale = scale; }

    private:
        Vector3f m_position;
        float m_scale;
    };
}

#endif
