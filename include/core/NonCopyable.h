#ifndef YAGE_CORE_NONCOPYABLE_H
#define YAGE_CORE_NONCOPYABLE_H

#include "core/Config.h"

namespace ye
{
    class YE_API NonCopyable
    {
    public:
        NonCopyable() = default;

        virtual ~NonCopyable() = default;

        NonCopyable(NonCopyable const&) = delete;

        NonCopyable& operator =(NonCopyable const&) = delete;

        NonCopyable(NonCopyable&&) = delete;

        NonCopyable& operator=(NonCopyable&&) = delete;
    };
}

#endif
