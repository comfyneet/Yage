#ifndef YAGE_CORE_REFERENCECOUNTER_H
#define YAGE_CORE_REFERENCECOUNTER_H

#include "core/Config.h"
#include "core/NonCopyable.h"

namespace ye
{
    class YE_API ReferenceCounter : public NonCopyable
    {
    public:
        ReferenceCounter() :
            m_refCount{}
        {
        }

        int32_t GetReferenceCount() const { return m_refCount; }

        void AddReference() { m_refCount++; }

        bool RemoveReference()
        {
            m_refCount--;
            return m_refCount == 0;
        }

    private:
        int32_t m_refCount;
    };
}

#endif
