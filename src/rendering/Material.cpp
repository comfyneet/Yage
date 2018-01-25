#include "rendering/Material.h"

namespace ye
{
    std::map<std::string, MaterialData*> Material::s_resourceMap{};

    Material::Material(std::string const& materialName) :
        m_materialData{},
        m_materialName{materialName}
    {
        if (m_materialName.length() > 0)
        {
            const std::map<std::string, MaterialData*>::const_iterator it = s_resourceMap.find(materialName);
            const auto result = it != s_resourceMap.end();
            assert(result);

            m_materialData = it->second;
            m_materialData->AddReference();
        }
    }

    Material::Material(RenderingEngine const& renderer, std::string const& materialName, Texture const& diffuse) :
        m_materialData{},
        m_materialName{materialName}
    {
        m_materialData = new MaterialData(renderer);
        s_resourceMap[m_materialName] = m_materialData;
    }

    Material::Material(Material const& other) :
        m_materialData(other.m_materialData),
        m_materialName(other.m_materialName)
    {
        m_materialData->AddReference();
    }

    Material::~Material()
    {
        if (m_materialData && m_materialData->RemoveReference())
        {
            if (m_materialName.length() > 0)
            {
                s_resourceMap.erase(m_materialName);
            }

            delete m_materialData;
        }
    }
}
