#ifndef YAGE_RENDERINl_SHADER_H
#define YAGE_RENDERINl_SHADER_H

#include "core/Config.h"
#include "core/Math3D.h"
#include "core/NonCopyable.h"
#include "core/ReferenceCounter.h"
#include "rendering/RenderingEngine.h"

namespace ye
{
    class RenderingEngine;

    class YE_API CBufferData : public NonCopyable
    {
    public:
        CBufferData(std::string const& name, std::string const& type) :
            m_name{name},
            m_type{type}
        {
        }

        std::string const& GetName() const { return m_name; }
        std::string const& GetType() const { return m_type; }

    private:
        std::string m_name;
        std::string m_type;
    };

    class YE_API CBuffer : public NonCopyable
    {
    public:
        CBuffer(std::string const& name, std::vector<CBufferData> const& memberNames) :
            m_name{name},
            m_memberNames{memberNames}
        {
        }

        std::string const& GetName() const { return m_name; }
        std::vector<CBufferData> const& GetMemberNames() const { return m_memberNames; }

    private:
        std::string m_name;
        std::vector<CBufferData> m_memberNames;
    };

    class YE_API StructData : public NonCopyable
    {
    public:
        StructData(std::string const& name, std::string const& semanticName, std::string const& type) :
            m_name{name},
            m_semanticName{semanticName},
            m_type{type}
        {
        }

        std::string const& GetName() const { return m_name; }
        std::string const& GetSemanticName() const { return m_semanticName; }
        std::string const& GetType() const { return m_type; }

    private:
        std::string m_name;
        std::string m_semanticName;
        std::string m_type;
    };

    class YE_API Struct : public NonCopyable
    {
    public:
        Struct(std::string const& name, std::vector<StructData> const& memberNames) :
            m_name{name},
            m_memberNames{memberNames}
        {
        }

        std::string const& GetName() const { return m_name; }
        std::vector<StructData> const& GetMemberNames() const { return m_memberNames; }

    private:
        std::string m_name;
        std::vector<StructData> m_memberNames;
    };

    class YE_API ShaderData : public ReferenceCounter
    {
    public:
        ShaderData(RenderingEngine const& renderer, std::string const& vertexFileName,
                   std::string const& pixelFileName);
        virtual ~ShaderData();

        void Bind(RenderingEngine const& renderer, Matrix4f const& worldMatrix, Matrix4f const& viewMatrix,
                  Matrix4f const& projectionMatrix, ID3D11ShaderResourceView** textureArray,
                  Vector3f const& lightDirection, Vector4f const& diffuseColor) const;

    private:
        ID3D11VertexShader* m_vertexShader;
        ID3D11PixelShader* m_pixelShader;
        ID3D11InputLayout* m_layout;
        ID3D11Buffer* m_matrixBuffer;
        ID3D11SamplerState* m_sampleState;
        ID3D11Buffer* m_lightBuffer;
    };

    class YE_API Shader : public NonCopyable
    {
    public:
        Shader(RenderingEngine const& renderer, std::string const& vertexFileName, std::string const& pixelFileName);
        Shader(Shader const& other);
        virtual ~Shader();

        void Bind(RenderingEngine const& renderer, Matrix4f const& worldMatrix, Matrix4f const& viewMatrix,
                  Matrix4f const& projectionMatrix, ID3D11ShaderResourceView** textureArray,
                  Vector3f const& lightDirection, Vector4f const& diffuseColor) const;

    private:
        static std::map<std::string, ShaderData*> s_resourceMap;

        std::string m_vertexFileName;
        std::string m_pixelFileName;
        ShaderData* m_shaderData;
    };
}

#endif
