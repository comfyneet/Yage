#include "rendering/Shader.h"
#include "core/String.h"
#include "rendering/DirectXCommon.h"

using namespace DirectX;

namespace
{
    struct _MatrixBufferType
    {
        ye::Matrix4f world;
        ye::Matrix4f view;
        ye::Matrix4f projection;
    };

    struct _LightBufferType
    {
        ye::Vector4f diffuseColor;
        ye::Vector3f lightDirection;
        float padding;
    };

    std::string _LoadShader(std::string const& fileName);
    std::vector<ye::CBuffer> _FindCBuffers(std::string const& shaderText);
    std::string _FindCBufferName(std::string const& structStartToOpeningBrace);
    std::vector<ye::CBufferData> _FindCBufferComponents(std::string const& openingBraceToClosingBrace);
}

namespace ye
{
    std::map<std::string, ShaderData*> Shader::s_resourceMap{};

    ShaderData::ShaderData(RenderingEngine const& renderer, std::string const& vertexFileName,
                           std::string const& pixelFileName) :
        m_vertexShader{},
        m_pixelShader{},
        m_layout{}
    {
        //std::string vertexText = _LoadShader(vertexFileName);

        ID3DBlob* vertexBuffer{};
        ID3DBlob* errorMessage{};
        //HRESULT hResult = D3DCompile(shaderText.c_str(), shaderText.length(), nullptr, nullptr, nullptr, vertexName.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexBuffer, &errorMessage);
        HRESULT hResult = D3DCompileFromFile(String::Widen(vertexFileName).c_str(), nullptr, nullptr, "main", "vs_5_0",
                                             D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexBuffer, &errorMessage);
        assert(SUCCEEDED(hResult));

        ID3DBlob* pixelBuffer{};
        //hResult = D3DCompile(shaderText.c_str(), shaderText.length(), nullptr, nullptr, nullptr, pixelName.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelBuffer, &errorMessage);
        hResult = D3DCompileFromFile(String::Widen(pixelFileName).c_str(), nullptr, nullptr, "main", "ps_5_0",
                                     D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelBuffer, &errorMessage);
        assert(SUCCEEDED(hResult));

        hResult = renderer.GetDevice()->CreateVertexShader(vertexBuffer->GetBufferPointer(),
                                                           vertexBuffer->GetBufferSize(), nullptr, &m_vertexShader);
        assert(SUCCEEDED(hResult));

        hResult = renderer.GetDevice()->CreatePixelShader(pixelBuffer->GetBufferPointer(), pixelBuffer->GetBufferSize(),
                                                          nullptr, &m_pixelShader);
        assert(SUCCEEDED(hResult));

        std::array<D3D11_INPUT_ELEMENT_DESC, 5> layout{};
        layout[0].SemanticName = "POSITION";
        layout[0].SemanticIndex = 0;
        layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout[0].InputSlot = 0;
        layout[0].AlignedByteOffset = 0;
        layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout[0].InstanceDataStepRate = 0;

        layout[1].SemanticName = "TEXCOORD";
        layout[1].SemanticIndex = 0;
        layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
        layout[1].InputSlot = 0;
        layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout[1].InstanceDataStepRate = 0;

        layout[2].SemanticName = "NORMAL";
        layout[2].SemanticIndex = 0;
        layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout[2].InputSlot = 0;
        layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout[2].InstanceDataStepRate = 0;

        layout[3].SemanticName = "TANGENT";
        layout[3].SemanticIndex = 0;
        layout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout[3].InputSlot = 0;
        layout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        layout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout[3].InstanceDataStepRate = 0;

        layout[4].SemanticName = "BINORMAL";
        layout[4].SemanticIndex = 0;
        layout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout[4].InputSlot = 0;
        layout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        layout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout[4].InstanceDataStepRate = 0;

        hResult = renderer.GetDevice()->CreateInputLayout(layout.data(), layout.size(),
                                                          vertexBuffer->GetBufferPointer(),
                                                          vertexBuffer->GetBufferSize(), &m_layout);
        assert(SUCCEEDED(hResult));

        vertexBuffer->Release();
        pixelBuffer->Release();

        D3D11_BUFFER_DESC matrixBufferDesc{};
        matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth = sizeof(_MatrixBufferType);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;

        hResult = renderer.GetDevice()->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
        assert(SUCCEEDED(hResult));

        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        hResult = renderer.GetDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);
        assert(SUCCEEDED(hResult));

        D3D11_BUFFER_DESC lightBufferDesc{};
        lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        lightBufferDesc.ByteWidth = sizeof(_LightBufferType);
        lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        lightBufferDesc.MiscFlags = 0;
        lightBufferDesc.StructureByteStride = 0;

        hResult = renderer.GetDevice()->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);
        assert(SUCCEEDED(hResult));
    }

    ShaderData::~ShaderData()
    {
        if (m_lightBuffer) m_lightBuffer->Release();
        if (m_sampleState) m_sampleState->Release();
        if (m_matrixBuffer) m_matrixBuffer->Release();
        if (m_layout) m_layout->Release();
        if (m_vertexShader) m_vertexShader->Release();
        if (m_pixelShader) m_pixelShader->Release();
    }

    void ShaderData::Bind(RenderingEngine const& renderer, Matrix4f const& worldMatrix, Matrix4f const& viewMatrix,
                          Matrix4f const& projectionMatrix, ID3D11ShaderResourceView** textureArray,
                          Vector3f const& lightDirection, Vector4f const& diffuseColor) const
    {
        // TODO:
        const Matrix4f _worldMatrix(worldMatrix.Transpose());
        const Matrix4f _viewMatrix(viewMatrix.Transpose());
        const Matrix4f _projectionMatrix(projectionMatrix.Transpose());

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        auto hResult = renderer.GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
                                                        &mappedResource);
        assert(SUCCEEDED(hResult));

        _MatrixBufferType* dataPtr = static_cast<_MatrixBufferType*>(mappedResource.pData);
        dataPtr->world = _worldMatrix;
        dataPtr->view = _viewMatrix;
        dataPtr->projection = _projectionMatrix;

        renderer.GetDeviceContext()->Unmap(m_matrixBuffer, 0);

        uint32_t bufferNumber = 0;
        renderer.GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

        renderer.GetDeviceContext()->PSSetShaderResources(0, 2, textureArray);

        hResult = renderer.GetDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        assert(SUCCEEDED(hResult));

        _LightBufferType* dataPtr2 = static_cast<_LightBufferType*>(mappedResource.pData);
        dataPtr2->diffuseColor = diffuseColor;
        dataPtr2->lightDirection = lightDirection;

        renderer.GetDeviceContext()->Unmap(m_lightBuffer, 0);

        bufferNumber = 0;
        renderer.GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

        renderer.GetDeviceContext()->IASetInputLayout(m_layout);

        renderer.GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
        renderer.GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);

        renderer.GetDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);
    }

    Shader::Shader(RenderingEngine const& renderer, std::string const& vertexFileName,
                   std::string const& pixelFileName) :
        m_vertexFileName{vertexFileName},
        m_pixelFileName{pixelFileName},
        m_shaderData{}
    {
        std::map<std::string, ShaderData*>::const_iterator it = s_resourceMap.find(
            m_vertexFileName + "|" + m_pixelFileName);
        if (it != s_resourceMap.end())
        {
            m_shaderData = it->second;
            m_shaderData->AddReference();
        }
        else
        {
            m_shaderData = new ShaderData(renderer, m_vertexFileName, m_pixelFileName);
            s_resourceMap.insert(
                std::pair<std::string, ShaderData*>(m_vertexFileName + "|" + m_pixelFileName, m_shaderData));
        }
    }

    Shader::Shader(Shader const& other) :
        m_vertexFileName{other.m_vertexFileName},
        m_pixelFileName{other.m_pixelFileName},
        m_shaderData{other.m_shaderData}
    {
        m_shaderData->AddReference();
    }

    Shader::~Shader()
    {
        if (m_shaderData && m_shaderData->RemoveReference())
        {
            s_resourceMap.erase(m_vertexFileName + "|" + m_pixelFileName);

            delete m_shaderData;
        }
    }

    void Shader::Bind(RenderingEngine const& renderer, Matrix4f const& worldMatrix, Matrix4f const& viewMatrix,
                      Matrix4f const& projectionMatrix, ID3D11ShaderResourceView** textureArray,
                      Vector3f const& lightDirection, Vector4f const& diffuseColor) const
    {
        m_shaderData->Bind(renderer, worldMatrix, viewMatrix, projectionMatrix, textureArray, lightDirection,
                           diffuseColor);
    }
}

namespace
{
    std::string _LoadShader(std::string const& fileName)
    {
        std::ifstream file{};
        file.open(fileName);

        std::stringstream stream{};
        stream << file.rdbuf();

        return stream.str();
    }

    std::vector<ye::CBuffer> _FindCBuffers(std::string const& shaderText)
    {
        throw std::exception();
    }

    std::string _FindCBufferName(std::string const& structStartToOpeningBrace)
    {
        throw std::exception();
    }

    std::vector<ye::CBufferData> _FindCBufferComponents(std::string const& openingBraceToClosingBrace)
    {
        throw std::exception();
    }
}
