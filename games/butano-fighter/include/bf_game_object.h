#ifndef BF_GAME_OBJECT_H
#define BF_GAME_OBJECT_H

#include "btn_sprite_ptr.h"
#include "btn_fixed_size.h"
#include "btn_fixed_point.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{
    class fixed_rect;
}

namespace bf::game
{

class object
{

public:
    [[nodiscard]] static object create_hero_weapon(
            const btn::fixed_point& position, int hero_level, const btn::sprite_palette_ptr& flash_palette,
            const btn::camera_ptr& camera);

    [[nodiscard]] static object create_hero_bomb(
            const btn::fixed_point& position, const btn::sprite_palette_ptr& flash_palette,
            const btn::camera_ptr& camera);

    [[nodiscard]] const btn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool intersects_hero(const btn::fixed_rect& hero_rect) const;

    [[nodiscard]] int experience(int hero_level) const;

    void update();

private:
    btn::fixed_point _position;
    btn::fixed_size _dimensions;
    btn::fixed_point _delta_position;
    btn::sprite_ptr _sprite;
    btn::sprite_palette_ptr _sprite_palette;
    btn::sprite_palette_ptr _flash_palette;
    int _flash_palette_counter;

    object(btn::sprite_ptr&& sprite, const btn::fixed_point& position, const btn::fixed_size& dimensions,
           const btn::fixed_point& delta_position, const btn::sprite_palette_ptr& flash_palette);
};

}

#endif
