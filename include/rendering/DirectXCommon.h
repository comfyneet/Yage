#ifndef YAGE_RENDERING_DIRECTXCOMMON_H
#define YAGE_RENDERING_DIRECTXCOMMON_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "core/Config.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace ye
{
    template <typename T>
    class YE_API ComDeleter
    {
        void operator()(T* ptr)
        {
            ptr->Release();
        }
    };

    template <typename T>
    using ComPtr = std::unique_ptr<T, ComDeleter<T>>;
}

#endif
