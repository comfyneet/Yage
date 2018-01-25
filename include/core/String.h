#ifndef YAGE_CORE_STRING_H
#define YAGE_CORE_STRING_H

#include "core/Config.h"

namespace ye
{
    class YE_API String
    {
    public:
        static std::string Narrow(std::wstring const& wstring);
        static std::wstring Widen(std::string const& string);

        String() = delete;
    };
}

#endif
