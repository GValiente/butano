#include "bf_credits.h"

#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_double_size_mode.h"
#include "btn_sprite_affine_mat_attributes.h"
#include "btn_sprite_items_stage_3_blonde.h"
#include "btn_sprite_items_stage_1_animals.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr const int blending_frames = 60;

    [[nodiscard]] constexpr btn::array<btn::sprite_affine_mat_attributes, btn::display::height()>
    _create_hblank_effect_attributes()
    {
        btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> result;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            btn::sprite_affine_mat_attributes& attributes = result[index];
            btn::fixed sin = btn::lut_sin((index * 512) / btn::display::height());
            attributes.set_rotation_angle((index * 360) / btn::display::height());
            attributes.set_scale(1 + (sin / 2));
        }

        return result;
    }

    constexpr const btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> _hblank_effect_attributes =
            _create_hblank_effect_attributes();
}

credits::credits(btn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _text_generator(text_generator),
    _hblank_effect(btn::sprite_affine_mat_attributes_hblank_effect_ptr::create(
                       btn::sprite_affine_mat_ptr::create(), _hblank_effect_attributes))
{
    int y_limit = (btn::display::height() + 64) / 2;
    btn::sprite_ptr sprite = btn::sprite_items::stage_3_blonde.create_sprite(0, y_limit);
    sprite.set_affine_mat(_hblank_effect.affine_mat());
    sprite.set_double_size_mode(btn::sprite_double_size_mode::ENABLED);
    sprite.set_blending_enabled(true);
    _background_sprites.push_back({ btn::sprite_move_to_action(sprite, 300, 0, -y_limit),
                                    btn::create_sprite_animate_action_forever(sprite, 6, btn::sprite_items::stage_3_blonde.tiles_item(), 0, 1, 0, 2) });

    btn::blending::set_transparency_alpha(0);
    _blending_action.emplace(blending_frames, 1);
    butano_background.put_under_all();

    if(! btn::music::playing())
    {
        btn::music_items::fruit.play(0.4);
    }
}

btn::optional<scene_type> credits::update()
{
    btn::optional<scene_type> result;

    for(auto it = _background_sprites.begin(), end = _background_sprites.end(); it != end; )
    {
        background_sprite& sprite = *it;

        if(sprite.move_action.done())
        {
            _background_sprites.erase(it);
            end = _background_sprites.end();
        }
        else
        {
            sprite.move_action.update();
            sprite.animate_action.update();
            ++it;
        }
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();
        }
    }

    if(_blending_action)
    {
        _blending_action->update();

        if(_blending_action->done())
        {
            _blending_action.reset();
        }
    }

    return result;
}

}
