#ifndef BTN_STRING_H
#define BTN_STRING_H

#include "../3rd_party/etl/include/etl/cstring.h"
#include "btn_functional.h"
#include "btn_string_fwd.h"

namespace btn
{
    template<>
    struct hash<istring>
    {
        [[nodiscard]] size_t operator()(const istring& value) const
        {
            return array_hash(value.data(), value.size());
        }
    };

    template<size_t MaxSize>
    struct hash<string<MaxSize>>
    {
        [[nodiscard]] size_t operator()(const string<MaxSize>& value) const
        {
            return array_hash(value.data(), value.size());
        }
    };

    template<size_t Size>
    inline string<Size - 1> make_string(const char (&text)[Size])
    {
        return etl::make_string(text);
    }
}

#endif
