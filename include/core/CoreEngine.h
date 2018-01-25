#ifndef YAGE_COREENGINE_H
#define YAGE_COREENGINE_H

#include "core/Config.h"
#include "core/NonCopyable.h"

namespace ye
{
    class Game;
    class RenderingEngine;
    class Window;

    class YE_API CoreEngine : public NonCopyable
    {
    public:
        CoreEngine(double frameRate, Window* window, RenderingEngine* renderingEngine, Game* game);

        void Start();
        void Stop();

    private:
        bool m_isRunning;
        double m_frameTime;
        Window* m_window;
        RenderingEngine* m_renderingEngine;
        Game* m_game;
    };
}

#endif
