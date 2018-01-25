#include "core/String.h"

namespace ye
{
    std::string String::Narrow(std::wstring const& wstring)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string narrow = converter.to_bytes(wstring);

        return narrow;
    }

    std::wstring String::Widen(std::string const& string)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(string);

        return wide;
    }
}
