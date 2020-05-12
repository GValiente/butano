#ifndef BF_GAME_GEM_H
#define BF_GAME_GEM_H

#include "btn_span.h"
#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"

namespace btn
{
    class fixed_rect;
}

namespace bf::game
{

class gem
{

public:
    gem(const btn::fixed_point& position, const btn::span<btn::sprite_tiles_ptr>& tiles,
        const btn::sprite_palette_ptr& palette);

    [[nodiscard]] const btn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool intersects_hero(const btn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool done() const;

    [[nodiscard]] int experience(int hero_level) const;

    void update();

private:
    btn::fixed_point _position;
    btn::sprite_ptr _sprite;
    btn::span<btn::sprite_tiles_ptr> _tiles;
    int _counter;
};

}

#endif
