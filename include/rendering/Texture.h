#ifndef YAGE_RENDERING_TEXTURE_H
#define YAGE_RENDERING_TEXTURE_H

#include "core/Config.h"
#include "core/NonCopyable.h"
#include "core/ReferenceCounter.h"
#include "rendering/DirectXCommon.h"

namespace ye
{
    class RenderingEngine;

    class YE_API TextureData : public ReferenceCounter
    {
    public:
        TextureData(RenderingEngine const& renderer, int32_t width, int32_t height, uint8_t** data);
        virtual ~TextureData();

        int32_t GetWidth() const { return m_width; }
        int32_t GetHeight() const { return m_height; }

    private:
        ID3D11Texture2D* m_texture;
        ID3D11ShaderResourceView* m_textureView;
        int32_t m_width;
        int32_t m_height;
    };

    class YE_API Texture : public NonCopyable
    {
    public:
        Texture(RenderingEngine const& renderer, std::string const& fileName);
        Texture(RenderingEngine const& renderer, int32_t width, int32_t height, uint8_t* data);
        Texture(Texture const& other);
        void operator=(Texture texture);
        virtual ~Texture();

        int32_t GetWidth() const { return m_textureData->GetWidth(); }
        int32_t GetHeight() const { return m_textureData->GetHeight(); }

        bool operator==(Texture const& other) const { return m_textureData == other.m_textureData; }
        bool operator!=(Texture const& other) const { return !operator==(other); }

    private:
        static std::map<std::string, TextureData*> s_resourceMap;

        TextureData* m_textureData;
        std::string m_fileName;
    };
}

#endif
