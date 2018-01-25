#include "components/MeshRenderer.h"
#include "rendering/Camera.h"
#include "rendering/RenderingEngine.h"
#include "rendering/Shader.h"

namespace ye
{
    void MeshRenderer::Render(Shader const& shader, RenderingEngine const& renderingEngine, Camera const& camera)
    {
        // TODO

        //shader.Bind(renderingEngine);
        m_mesh.Draw(renderingEngine);
    }
}
