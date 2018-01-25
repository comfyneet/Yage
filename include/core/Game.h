#ifndef YAGE_CORE_GAME_H
#define YAGE_CORE_GAME_H

#include "core/Config.h"
#include "core/Entity.h"
#include "core/NonCopyable.h"

namespace ye
{
    class CoreEngine;
    class Input;
    class RenderingEngine;
    class Window;

    class YE_API Game : public NonCopyable
    {
    public:
        Game() = default;

        virtual ~Game() = default;

        virtual void Init(RenderingEngine const& renderer, Window const& window)
        {
        }

        void ProcessInput(Input const& input, double delta);
        void Update(double delta);
        void Render(RenderingEngine* renderingEngine);

        void SetEngine(CoreEngine* engine) { m_root.SetEngine(engine); }

    protected:
        void AddToScene(Entity* child) { m_root.AddChild(child); }

    private:
        Entity m_root;
    };
}

#endif
