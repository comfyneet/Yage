#include "core/Game.h"
#include "rendering/RenderingEngine.h"

namespace ye
{
    void Game::ProcessInput(Input const& input, double delta)
    {
    }

    void Game::Update(double delta)
    {
    }

    void Game::Render(RenderingEngine* renderingEngine)
    {
        renderingEngine->Render(m_root);
    }
}
