#include "core/Entity.h"
#include "core/EntityComponent.h"

namespace ye
{
    Entity* Entity::AddChild(Entity* child)
    {
        m_children.push_back(child);
        child->SetEngine(m_coreEngine);
        // TODO:
        return this;
    }

    Entity* Entity::AddComponent(EntityComponent* component)
    {
        m_components.push_back(component);
        component->SetParent(this);

        return this;
    }

    void Entity::RenderAll(Shader const& shader, RenderingEngine const& renderingEngine, Camera const& camera) const
    {
        Render(shader, renderingEngine, camera);

        for (auto const child : m_children)
        {
            child->RenderAll(shader, renderingEngine, camera);
        }
    }

    void Entity::SetEngine(CoreEngine* engine)
    {
        if (m_coreEngine != engine)
        {
            m_coreEngine = engine;
        }
    }

    void Entity::Render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
    {
        for (auto const component : m_components)
        {
            component->Render(shader, renderingEngine, camera);
        }
    }
}
