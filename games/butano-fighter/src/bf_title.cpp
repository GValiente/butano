#include "bf_title.h"

#include "btn_music.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_blending.h"
#include "btn_music_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_items_butano_big_sprite.h"
#include "btn_sprite_items_butano_big_sprite_alt.h"
#include "bf_scene_type.h"

namespace bf
{

namespace
{
    [[nodiscard]] btn::sprite_ptr _create_butano_up_sprite()
    {
        btn::sprite_builder builder(btn::sprite_items::butano_big_sprite, 0);
        builder.set_y((btn::display::height() / 2) + 32);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    [[nodiscard]] btn::sprite_ptr _create_butano_down_sprite()
    {
        btn::sprite_builder builder(btn::sprite_items::butano_big_sprite, 1);
        builder.set_y((btn::display::height() / 2) + 32 + 64);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    [[nodiscard]] btn::span<const btn::third_sprite_attributes> _create_attributes_span(
            const btn::ivector<btn::third_sprite_attributes>& vector)
    {
        return btn::span<const btn::third_sprite_attributes>(vector.data(), vector.max_size());
    }

    void _build_hblank_effect_attributes(const btn::sprite_ptr& sprite, int graphics_index,
                                         btn::ivector<btn::third_sprite_attributes>& attributes)
    {
        btn::third_sprite_attributes even_sprite_attributes = sprite.third_attributes();
        btn::third_sprite_attributes odd_sprite_attributes = even_sprite_attributes;
        odd_sprite_attributes.set_tiles(btn::sprite_items::butano_big_sprite_alt.tiles_item().create_tiles(graphics_index));

        for(int index = 0, limit = attributes.max_size(); index < limit; index += 2)
        {
            attributes.push_back(even_sprite_attributes);
            attributes.push_back(odd_sprite_attributes);
        }
    }
}

title::title(btn::sprite_text_generator&) :
    _butano_up_sprite(_create_butano_up_sprite()),
    _butano_down_sprite(_create_butano_down_sprite()),
    _butano_up_hblank_effect(btn::third_sprite_attributes_hblank_effect_ptr::create(
                                 _butano_up_sprite, _create_attributes_span(_butano_up_hblank_effect_attributes))),
    _butano_down_hblank_effect(btn::third_sprite_attributes_hblank_effect_ptr::create(
                                 _butano_down_sprite, _create_attributes_span(_butano_down_hblank_effect_attributes)))
{
    _build_hblank_effect_attributes(_butano_up_sprite, 0, _butano_up_hblank_effect_attributes);
    _build_hblank_effect_attributes(_butano_down_sprite, 1, _butano_down_hblank_effect_attributes);
    btn::music::play(btn::music_items::battle_clean);
}

btn::optional<scene_type> title::update()
{
    btn::optional<scene_type> result;

    if(_butano_y_up)
    {
        _butano_y_inc += 0.05;

        if(_butano_y_inc >= 1)
        {
            _butano_y_inc = 1;
            _butano_y_up = false;
        }
    }
    else
    {
        _butano_y_inc -= 0.05;

        if(_butano_y_inc <= -1)
        {
            _butano_y_inc = -1;
            _butano_y_up = true;
        }
    }

    btn::fixed butano_up_sprite_y = _butano_up_sprite.y() + _butano_y_inc;
    _butano_up_sprite.set_y(butano_up_sprite_y);
    _butano_down_sprite.set_y(butano_up_sprite_y + 64);

    if(butano_up_sprite_y <= -26)
    {
        btn::fixed blending_intensity = -butano_up_sprite_y - 26;
        btn::blending::set_intensity_alpha(blending_intensity / 40);
    }
    else
    {
        btn::blending::set_intensity_alpha(0);
    }

    return result;
}

}
