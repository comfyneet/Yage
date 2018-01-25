#include "rendering/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "rendering/RenderingEngine.h"

namespace ye
{
    std::map<std::string, TextureData*> Texture::s_resourceMap{};

    TextureData::TextureData(RenderingEngine const& renderer, const int32_t width, const int32_t height, uint8_t** data) :
        m_texture{},
        m_textureView{},
        m_width{width},
        m_height{height}
    {
        D3D11_TEXTURE2D_DESC textureDesc{};
        textureDesc.Height = height;
        textureDesc.Width = width;
        textureDesc.MipLevels = 0;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        HRESULT hResult = renderer.GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_texture);
        if (FAILED(hResult))
        {
            //if (m_texture) m_texture->Release();
            throw std::exception();
        }

        uint32_t rowPitch = width * 4 * sizeof(uint8_t);
        renderer.GetDeviceContext()->UpdateSubresource(m_texture, 0, nullptr, *data, rowPitch, 0);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = -1;

        hResult = renderer.GetDevice()->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
        if (FAILED(hResult))
        {
            throw std::exception();
        }
        assert(SUCCEEDED(hResult));

        renderer.GetDeviceContext()->GenerateMips(m_textureView);
    }

    TextureData::~TextureData()
    {
        if (m_textureView) m_textureView->Release();
        if (m_texture) m_texture->Release();
    }

    Texture::Texture(RenderingEngine const& renderer, std::string const& fileName) :
        m_textureData{},
        m_fileName{fileName}
    {
        std::map<std::string, TextureData*>::const_iterator it = s_resourceMap.find(fileName);
        if (it != s_resourceMap.end())
        {
            m_textureData = it->second;
            m_textureData->AddReference();
        }
        else
        {
            int32_t x{}, y{}, bytesPerpixel{};
            uint8_t* data = stbi_load(fileName.c_str(), &x, &y, &bytesPerpixel, 4);
            if (!data)
            {
                throw std::exception();
            }

            m_textureData = new TextureData(renderer, x, y, &data);
            stbi_image_free(data);

            s_resourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
        }
    }

    Texture::Texture(RenderingEngine const& renderer, int32_t width, int32_t height, uint8_t* data) :
        m_textureData{}
    {
        m_textureData = new TextureData(renderer, width, height, &data);
    }

    Texture::Texture(Texture const& other) :
        m_textureData(other.m_textureData),
        m_fileName(other.m_fileName)
    {
        m_textureData->AddReference();
    }

    void Texture::operator=(Texture texture)
    {
        char* temp[sizeof(Texture) / sizeof(char)];
        std::memcpy(temp, this, sizeof(Texture));
        std::memcpy(this, &texture, sizeof(Texture));
        std::memcpy(&texture, temp, sizeof(Texture));
    }

    Texture::~Texture()
    {
        if (m_textureData && m_textureData->RemoveReference())
        {
            if (m_fileName.length() > 0)
            {
                s_resourceMap.erase(m_fileName);
            }

            delete m_textureData;
        }
    }
}
