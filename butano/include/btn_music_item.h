#ifndef BTN_MUSIC_ITEM_H
#define BTN_MUSIC_ITEM_H

#include "btn_functional.h"

namespace btn
{

class music_item
{

public:
    constexpr explicit music_item(int id) :
        _id(id)
    {
    }

    [[nodiscard]] constexpr int id() const
    {
        return _id;
    }

    [[nodiscard]] constexpr bool operator==(music_item other) const
    {
        return _id == other._id;
    }

    [[nodiscard]] constexpr bool operator!=(music_item other) const
    {
        return ! (*this == other);
    }

private:
    int _id;
};


template<>
struct hash<music_item>
{
    [[nodiscard]] constexpr size_t operator()(music_item value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

