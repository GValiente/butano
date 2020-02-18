#ifndef BTN_SOUND_ITEM_H
#define BTN_SOUND_ITEM_H

#include "btn_functional.h"

namespace btn
{

class sound_item
{

public:
    constexpr explicit sound_item(int id) :
        _id(id)
    {
    }

    [[nodiscard]] constexpr int id() const
    {
        return _id;
    }

    [[nodiscard]] constexpr friend bool operator==(sound_item a, sound_item b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] constexpr friend bool operator!=(sound_item a, sound_item b)
    {
        return ! (a == b);
    }

private:
    int _id;
};


template<>
struct hash<sound_item>
{
    [[nodiscard]] constexpr unsigned operator()(sound_item value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

