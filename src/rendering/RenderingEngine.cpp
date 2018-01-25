#include "rendering/RenderingEngine.h"
#include "core/Entity.h"
#include "rendering/Shader.h"
#include "rendering/Window.h"

namespace ye
{
    RenderingEngine::RenderingEngine(Window const& window) :
        m_swapChain{},
        m_device{},
        m_deviceContext{},
        m_backBuffer{},
        //m_defaultShader{},
        m_window{&window}
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc{};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.Width = window.GetWidth();
        swapChainDesc.BufferDesc.Height = window.GetHeight();
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = window.GetHwnd();
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = true;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        auto hresult = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
                                                     D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device,
                                                     nullptr, &m_deviceContext);
        assert(SUCCEEDED(hresult));

        ID3D11Texture2D* pBackBuffer{};
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

        hresult = m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_backBuffer);
        assert(SUCCEEDED(hresult));

        if (pBackBuffer) pBackBuffer->Release();

        m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, nullptr);

        D3D11_VIEWPORT viewport{};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(window.GetWidth());
        viewport.Height = static_cast<float>(window.GetHeight());

        m_deviceContext->RSSetViewports(1, &viewport);

        //m_defaultShader = new Shader(*this);
    }

    RenderingEngine::~RenderingEngine()
    {
        //if (m_defaultShader) delete m_defaultShader;
        if (m_swapChain)
        {
            m_swapChain->SetFullscreenState(false, nullptr);
            m_swapChain->Release();
        }
        if (m_backBuffer)m_backBuffer->Release();
        if (m_device) m_device->Release();
        if (m_deviceContext) m_deviceContext->Release();
    }

    void RenderingEngine::Render(Entity const& object)
    {
        float color[4] = {0.0f, 0.2f, 0.4f, 1.0f};
        m_deviceContext->ClearRenderTargetView(m_backBuffer, color);
        // TODO
    }

    void RenderingEngine::SwapBuffers()
    {
        m_swapChain->Present(0, 0);
    }
}
