#ifndef BTN_THIRD_SPRITE_ATTRIBUTES_H
#define BTN_THIRD_SPRITE_ATTRIBUTES_H

#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

class alignas(alignof(int)) third_sprite_attributes
{

public:
    third_sprite_attributes(sprite_tiles_ptr tiles_ptr, sprite_palette_ptr palette_ptr, int bg_priority);

    [[nodiscard]] const sprite_tiles_ptr& tiles() const
    {
        return _tiles_ptr;
    }

    void set_tiles(const sprite_tiles_ptr& tiles_ptr)
    {
        _tiles_ptr = tiles_ptr;
    }

    void set_tiles(sprite_tiles_ptr&& tiles_ptr)
    {
        _tiles_ptr = move(tiles_ptr);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return _palette_ptr;
    }

    void set_palette(const sprite_palette_ptr& palette_ptr)
    {
        _palette_ptr = palette_ptr;
    }

    void set_palette(sprite_palette_ptr&& palette_ptr)
    {
        _palette_ptr = move(palette_ptr);
    }

    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    void set_bg_priority(int bg_priority);

    [[nodiscard]] friend bool operator==(const third_sprite_attributes& a, const third_sprite_attributes& b)
    {
        return a._tiles_ptr == b._tiles_ptr && a._palette_ptr == b._palette_ptr && a._bg_priority == b._bg_priority;
    }

    [[nodiscard]] friend bool operator!=(const third_sprite_attributes& a, const third_sprite_attributes& b)
    {
        return ! (a == b);
    }

private:
    sprite_tiles_ptr _tiles_ptr;
    sprite_palette_ptr _palette_ptr;
    int8_t _bg_priority;
};

}

#endif

