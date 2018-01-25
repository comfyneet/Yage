#ifndef YAGE_CORE_MAPPEDVALUES_H
#define YAGE_CORE_MAPPEDVALUES_H

#include "core/Config.h"
#include "core/Math3D.h"
#include "core/NonCopyable.h"
#include "rendering/Texture.h"

namespace ye
{
    class RenderingEngine;

    class YE_API MappedValues : public NonCopyable
    {
    public:
        explicit MappedValues(RenderingEngine const& renderer) :
            m_defaultTexture{Texture(renderer, "./resources/textures/defaultTexture.png")},
            m_defaultVector3f{Vector3f(0.0f, 0.0f, 0.0f)}
        {
        }

        void SetVector3f(std::string const& name, Vector3f const& value) { m_vector3fMap[name] = value; }
        void SetFloat(std::string const& name, const float value) { m_floatMap[name] = value; }
        void SetTexture(std::string const& name, Texture* value) { m_textureMap[name] = value; }

        Vector3f const& GetVector3f(std::string const& name) const;
        float GetFloat(std::string const& name) const;
        Texture const& GetTexture(std::string const& name) const;

    private:
        std::map<std::string, Vector3f> m_vector3fMap;
        std::map<std::string, float> m_floatMap;
        std::map<std::string, Texture*> m_textureMap;

        Texture m_defaultTexture;
        Vector3f m_defaultVector3f;
    };
}

#endif
