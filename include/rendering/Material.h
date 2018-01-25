#ifndef YAGE_RENDERING_MATERIAL_H
#define YAGE_RENDERING_MATERIAL_H

#include "core/Config.h"
#include "core/MappedValues.h"
#include "core/NonCopyable.h"
#include "core/ReferenceCounter.h"

namespace ye
{
    class RenderingEngine;
    class Texture;

    class YE_API MaterialData : public ReferenceCounter, public MappedValues
    {
    public:
        explicit MaterialData(RenderingEngine const& renderer) :
            MappedValues{renderer}
        {
        }
    };

    class YE_API Material : public NonCopyable
    {
    public:
        explicit Material(std::string const& materialName);
        Material(RenderingEngine const& renderer, std::string const& materialName, Texture const& diffuse);
        Material(Material const& other);
        virtual ~Material();

        void SetVector3f(std::string const& name, Vector3f const& value) const
        {
            m_materialData->SetVector3f(name, value);
        }

        void SetFloat(std::string const& name, const float value) const { m_materialData->SetFloat(name, value); }
        void SetTexture(std::string const& name, Texture* value) const { m_materialData->SetTexture(name, value); }

        Vector3f const& GetVector3f(std::string const& name) const { return m_materialData->GetVector3f(name); }
        float GetFloat(std::string const& name) const { return m_materialData->GetFloat(name); }
        Texture const& GetTexture(std::string const& name) const { return m_materialData->GetTexture(name); }

    private:
        static std::map<std::string, MaterialData*> s_resourceMap;
        MaterialData* m_materialData;
        std::string m_materialName;
    };
}

#endif
