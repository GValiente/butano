#ifndef BTN_TYPE_NAME_H
#define BTN_TYPE_NAME_H

#include "btn_string_view.h"

namespace _btn
{
    template<typename Type>
    [[nodiscard]] constexpr btn::string_view type_name()
    {
        return btn::string_view(__PRETTY_FUNCTION__);
    }
}

namespace btn
{
    template<typename Type>
    [[nodiscard]] constexpr string_view type_name()
    {
        string_view result = _btn::type_name<typename remove_reference<Type>::type>();
        result = result.substr(58);
        result.remove_suffix(1);
        return result;
    }
}

#endif
