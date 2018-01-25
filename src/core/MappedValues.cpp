#include "core/MappedValues.h"

namespace ye
{
    Vector3f const& MappedValues::GetVector3f(std::string const& name) const
    {
        std::map<std::string, Vector3f>::const_iterator it = m_vector3fMap.find(name);
        if (it != m_vector3fMap.end())
        {
            return it->second;
        }

        return m_defaultVector3f;
    }

    float MappedValues::GetFloat(std::string const& name) const
    {
        std::map<std::string, float>::const_iterator it = m_floatMap.find(name);
        if (it != m_floatMap.end())
        {
            return it->second;
        }

        return 0;
    }

    Texture const& MappedValues::GetTexture(std::string const& name) const
    {
        std::map<std::string, Texture*>::const_iterator it = m_textureMap.find(name);
        if (it != m_textureMap.end())
        {
            return *it->second;
        }

        return m_defaultTexture;
    }
}
