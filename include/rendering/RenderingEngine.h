#ifndef YAGE_RENDERING_RENDERINGENGINE_H
#define YAGE_RENDERING_RENDERINGENGINE_H

#include "core/Config.h"
#include "core/NonCopyable.h"
#include "rendering/DirectXCommon.h"

namespace ye
{
    class Entity;
    class Shader;
    class Window;

    class YE_API RenderingEngine : public NonCopyable
    {
    public:
        explicit RenderingEngine(Window const& window);
        virtual ~RenderingEngine();

        ID3D11Device* GetDevice() const { return m_device; }
        ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }

        void Render(Entity const& object);
        void SwapBuffers();

    private:
        IDXGISwapChain* m_swapChain;
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;
        ID3D11RenderTargetView* m_backBuffer;
        //Shader* m_defaultShader;
        Window const* m_window;
    };
}

#endif
