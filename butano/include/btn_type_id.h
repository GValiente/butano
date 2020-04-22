#ifndef BTN_TYPE_ID_H
#define BTN_TYPE_ID_H

#include "btn_common.h"

namespace btn
{

/**
 * @brief Unique type ID without RTTI
 *
 * https://codereview.stackexchange.com/questions/48594/unique-type-id-no-rtti
 * https://github.com/JoaoBaptMG/ReboundTheGame/blob/master/Commons/type_id.hpp
 */
class type_id_t
{

public:
    template<typename Type>
    friend type_id_t type_id();

    type_id_t() = default;

    [[nodiscard]] friend bool operator==(type_id_t a, type_id_t b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] friend bool operator!=(type_id_t a, type_id_t b)
    {
        return a._id != b._id;
    }

protected:
    using id_type = type_id_t();

    id_type* _id = nullptr;

    type_id_t(id_type* id) :
        _id(id)
    {
    }
};

template<typename Type>
[[nodiscard]] type_id_t type_id()
{
    return &type_id<Type>;
}

}

#endif
