#ifndef YAGE_CORE_ENTITYCOMPONENT_H
#define YAGE_CORE_ENTITYCOMPONENT_H

#include "core/Config.h"
#include "core/Entity.h"
#include "core/NonCopyable.h"

namespace ye
{
    class Camera;
    class CoreEngine;
    class Input;
    class RenderingEngine;
    class Shader;
    class Transform;

    class YE_API EntityComponent : public NonCopyable
    {
    public:
        EntityComponent() :
            m_parent{}
        {
        }

        virtual ~EntityComponent() = default;

        virtual void ProcessInput(Input const& input, float delta)
        {
        }

        virtual void Update(float delta)
        {
        }

        virtual void Render(Shader const& shader, RenderingEngine const& renderingEngine, Camera const& camera) const
        {
        }

        virtual void AddToEngine(CoreEngine* engine) const
        {
        }

        Transform* GetTransform() { return m_parent->GetTransform(); }
        Transform const& GetTransform() const { return *m_parent->GetTransform(); }

        virtual void SetParent(Entity* parent) { m_parent = parent; }

    private:
        Entity* m_parent;
    };
}

#endif
