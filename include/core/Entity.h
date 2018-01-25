#ifndef YAGE_CORE_ENTITY_H
#define YAGE_CORE_ENTITY_H

#include "core/Config.h"
#include "core/Math3D.h"
#include "core/NonCopyable.h"
#include "core/Transform.h"

namespace ye
{
    class Camera;
    class CoreEngine;
    class EntityComponent;
    class RenderingEngine;
    class Shader;

    class YE_API Entity : public NonCopyable
    {
    public:
        explicit Entity(Vector3f const& position = Vector3f()) :
            m_transform(position),
            m_coreEngine{}
        {
        }

        virtual ~Entity() = default;

        Entity* AddChild(Entity* child);
        Entity* AddComponent(EntityComponent* component);

        void RenderAll(Shader const& shader, RenderingEngine const& renderingEngine, Camera const& camera) const;

        Transform* GetTransform() { return &m_transform; }

        // TODO
        void SetEngine(CoreEngine* engine);

    private:
        std::vector<Entity*> m_children;
        std::vector<EntityComponent*> m_components;
        Transform m_transform;
        CoreEngine* m_coreEngine;

        void Render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
    };
}

#endif
