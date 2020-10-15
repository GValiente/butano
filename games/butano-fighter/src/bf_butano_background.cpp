#include "bf_butano_background.h"

#include "btn_color.h"
#include "btn_window.h"
#include "btn_display.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_butano_big_silhouette.h"
#include "btn_regular_bg_items_butano_background.h"

namespace bf
{

namespace
{
    constexpr const int show_hide_frames = 10;

    [[nodiscard]] constexpr btn::array<btn::color, btn::display::height()> _create_palette_hblank_effect_colors()
    {
        btn::array<btn::color, btn::display::height()> result;
        btn::color color(14, 0, 0);
        int redInc = 16;
        int greenInc = 16;
        int blueInc = 8;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            result[index] = color;
            --redInc;
            --greenInc;
            --blueInc;

            if(! redInc)
            {
                color.set_red(color.red() + (index <= 80 ? 1 : -1));
                redInc = 16;
            }

            if(! greenInc)
            {
                color.set_green(btn::max(color.green() + (index <= 80 ? 1 : -1), 0));
                greenInc = 16;
            }

            if(! blueInc)
            {
                color.set_blue(btn::max(color.blue() + (index <= 80 ? 1 : -1), 0));
                blueInc = 16;
            }
        }

        return result;
    }

    constexpr const btn::array<btn::color, btn::display::height()> palette_hblank_effect_colors =
            _create_palette_hblank_effect_colors();
}

void butano_background::show(const btn::fixed_point& silhouette_position, const btn::camera_ptr& camera)
{
    _set_visible();

    btn::regular_bg_ptr bg = _move_action->bg();
    bg.set_priority(0);
    btn::window::sprites().set_show_bg(bg, false);

    btn::sprite_builder builder(btn::sprite_items::butano_big_silhouette, 0);
    builder.set_position(silhouette_position.x(), silhouette_position.y() - 63);
    builder.set_scale(2);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    btn::sprite_ptr silhouette_up_sprite = builder.release_build();
    _silhouette_up_sprite_move_action.emplace(silhouette_up_sprite, show_hide_frames, silhouette_position);
    _silhouette_up_sprite_scale_action.emplace(btn::move(silhouette_up_sprite), show_hide_frames, 0.01);

    builder = btn::sprite_builder(btn::sprite_items::butano_big_silhouette, 1);
    builder.set_position(silhouette_position.x(), silhouette_position.y() + 63);
    builder.set_scale(2);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    btn::sprite_ptr silhouette_down_sprite = builder.release_build();
    _silhouette_down_sprite_move_action.emplace(silhouette_down_sprite, show_hide_frames, silhouette_position);
    _silhouette_down_sprite_scale_action.emplace(btn::move(silhouette_down_sprite), show_hide_frames, 0.01);
}

void butano_background::hide(const btn::fixed_point& silhouette_position, const btn::camera_ptr& camera)
{
    _set_visible();

    btn::regular_bg_ptr bg = _move_action->bg();
    bg.set_priority(0);
    btn::window::sprites().set_show_bg(bg, false);

    btn::sprite_builder builder(btn::sprite_items::butano_big_silhouette, 0);
    builder.set_position(silhouette_position);
    builder.set_scale(0.01);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    btn::sprite_ptr silhouette_up_sprite = builder.release_build();
    _silhouette_up_sprite_move_action.emplace(silhouette_up_sprite, show_hide_frames, silhouette_position.x(),
                                              silhouette_position.y() - 63);
    _silhouette_up_sprite_scale_action.emplace(btn::move(silhouette_up_sprite), show_hide_frames, 2);

    builder = btn::sprite_builder(btn::sprite_items::butano_big_silhouette, 1);
    builder.set_position(silhouette_position);
    builder.set_scale(0.01);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    btn::sprite_ptr silhouette_down_sprite = builder.release_build();
    _silhouette_down_sprite_move_action.emplace(silhouette_down_sprite, show_hide_frames, silhouette_position.x(),
                                                silhouette_position.y() + 63);
    _silhouette_down_sprite_scale_action.emplace(btn::move(silhouette_down_sprite), show_hide_frames, 2);
}

void butano_background::put_under_all()
{
    _set_visible();

    btn::regular_bg_ptr bg = _move_action->bg();
    bg.set_priority(3);
}

void butano_background::update()
{
    if(_move_action)
    {
        _move_action->update();

        if(_silhouette_up_sprite_move_action)
        {
            _silhouette_up_sprite_move_action->update();
            _silhouette_up_sprite_scale_action->update();
            _silhouette_down_sprite_move_action->update();
            _silhouette_down_sprite_scale_action->update();

            if(_silhouette_up_sprite_move_action->done())
            {
                if(_silhouette_up_sprite_move_action->sprite().scale_x() > 1)
                {
                    _move_action.reset();
                    _palette_hblank_effect.reset();
                }

                _silhouette_up_sprite_move_action.reset();
                _silhouette_up_sprite_scale_action.reset();
                _silhouette_down_sprite_move_action.reset();
                _silhouette_down_sprite_scale_action.reset();
            }
        }
    }
}

void butano_background::_set_visible()
{
    if(! _move_action)
    {
        btn::regular_bg_ptr bg = btn::regular_bg_items::butano_background.create_bg(0, 0);
        _palette_hblank_effect = btn::bg_palette_color_hblank_effect_ptr::create(
                    bg.palette(), 1, palette_hblank_effect_colors);
        _move_action.emplace(btn::move(bg), 1, -1);
    }
}

}
