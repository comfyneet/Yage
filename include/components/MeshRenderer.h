#ifndef YAGE_COMPONENTS_MESHRENDERER_H
#define YAGE_COMPONENTS_MESHRENDERER_H

#include "core/Config.h"
#include "core/EntityComponent.h"
#include "rendering/Material.h"
#include "rendering/Mesh.h"

namespace ye
{
    class Camera;
    class RenderingEngine;
    class Shader;

    class YE_API MeshRenderer : public EntityComponent
    {
    public:
        MeshRenderer(Mesh const& mesh, Material const& material) :
            m_mesh{mesh},
            m_material{material}
        {
        }

        virtual void Render(Shader const& shader, RenderingEngine const& renderingEngine, Camera const& camera);

    private:
        Mesh m_mesh;
        Material m_material;
    };
}

#endif
