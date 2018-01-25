#include "core/CoreEngine.h"
#include "core/Game.h"
#include "core/Timer.h"
#include "rendering/RenderingEngine.h"
#include "rendering/Window.h"

namespace ye
{
    CoreEngine::CoreEngine(const double frameRate, Window* window, RenderingEngine* renderingEngine, Game* game) :
        m_isRunning{},
        m_frameTime{1.0 / frameRate},
        m_window{window},
        m_renderingEngine{renderingEngine},
        m_game{game}
    {
        m_game->SetEngine(this);

        m_game->Init(*renderingEngine, *m_window);
    }

    void CoreEngine::Start()
    {
        if (m_isRunning)
        {
            return;
        }

        m_isRunning = true;

        auto lastTime = Timer::GetTime();
        double frameCounter{};
        double unprocessedTime{};
        int32_t frames{};

        while (m_isRunning)
        {
            bool render{};

            const auto startTime = Timer::GetTime();
            const auto passedTime = startTime - lastTime;
            lastTime = startTime;

            unprocessedTime += passedTime;
            frameCounter += passedTime;

            if (frameCounter >= 1.0)
            {
                const auto totalTime = (1000.0 * frameCounter) / static_cast<double>(frames);

                std::printf("Total Time: %f ms\n", totalTime);

                frames = 0;
                frameCounter = 0.0;
            }

            while (unprocessedTime > m_frameTime)
            {
                m_window->Update();

                if (m_window->IsCloseRequested())
                {
                    Stop();
                }

                m_game->ProcessInput(m_window->GetInput(), m_frameTime);
                m_game->Update(m_frameTime);

                render = true;

                unprocessedTime -= m_frameTime;
            }

            if (render)
            {
                m_game->Render(m_renderingEngine);

                m_renderingEngine->SwapBuffers();

                frames++;
            }
            else
            {
                Timer::Sleep(1);
            }
        }
    }

    void CoreEngine::Stop()
    {
        m_isRunning = false;
    }
}
