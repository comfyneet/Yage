#ifndef YAGE_RENDERING_CAMERA_H
#define YAGE_RENDERING_CAMERA_H

#include "core/Config.h"
#include "core/EntityComponent.h"
#include "core/Math3D.h"
#include "core/NonCopyable.h"

namespace ye
{
    class Transform;

    class YE_API Camera : public NonCopyable
    {
    public:
        Camera(Matrix4f const& projection, Transform* transform) :
            m_projection{projection},
            m_transform{transform}
        {
        }

        Transform* GetTransform() { return m_transform; }
        Transform const& GetTransform() const { return *m_transform; }

        Matrix4f GetViewProjection() const;

        void SetProjection(Matrix4f const& projection) { m_projection = projection; }
        void SetTransform(Transform* transform) { m_transform = transform; }

    private:
        Matrix4f m_projection;
        Transform* m_transform;
    };

    class CameraComponent : public EntityComponent
    {
    public:
        CameraComponent(Matrix4f const& projection) :
            m_camera(projection, nullptr)
        {
        }


    private:
        Camera m_camera;
    };
}

#endif
