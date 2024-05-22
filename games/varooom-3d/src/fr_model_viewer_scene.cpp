/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_model_viewer_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_string.h"
#include "bn_blending.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"
#include "fr_model_colors.h"
#include "fr_model_viewer_items.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_big_font.h"
#include "bn_sprite_items_small_variable_font.h"
#include "bn_sprite_palette_items_big_font_yellow.h"
#include "bn_regular_bg_items_title_backdrop_fade.h"
#include "bn_sprite_palette_items_small_font_yellow.h"
#include "bn_regular_bg_items_model_viewer_background.h"
#include "bn_bg_palette_items_title_backdrop_fade_dark.h"

#include "models/fr_model_3d_items_character_interrogation.h"

namespace fr
{

namespace
{
    static_assert(sizeof(model_viewer_items) / sizeof(model_viewer_items[0]) ==
                  model_viewer_items_status::items_count());

    constexpr int max_menu_entries = 10;

    constexpr bn::fixed menu_x = -98;
    constexpr bn::fixed half_menu_width = 60;
    constexpr bn::fixed menu_x_inc = 4;
    constexpr int animation_frames = (half_menu_width / menu_x_inc).right_shift_integer();

    constexpr bn::fixed initial_y = -65;
    constexpr bn::fixed menu_y_inc = 12;
    constexpr bn::fixed initial_menu_y = initial_y + menu_y_inc + 6;

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_in_action()
    {
        bn::bg_palettes::set_fade(bn::colors::black, 1);
        return bn::bg_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action()
    {
        bn::sprite_palettes::set_fade(bn::colors::black, 1);
        return bn::sprite_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_ptr _create_cursor_sprite(common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(menu_x - 10, initial_menu_y + 1, ">", output_sprites);
        text_generator.set_one_sprite_per_character(false);
        return bn::move(output_sprites[0]);
    }
}

model_viewer_scene::model_viewer_scene(common_stuff& common_stuff) :
    _common_stuff(common_stuff),
    _bgs_fade_in_action(_create_bgs_fade_in_action()),
    _sprites_fade_in_action(_create_sprites_fade_in_action()),
    _backdrop_bg(bn::regular_bg_items::title_backdrop_fade.create_bg(0, 0)),
    _menu_bg(bn::regular_bg_items::model_viewer_background.create_bg(
                 (512 - bn::display::width()) / 2, (256 - bn::display::height()) / 2)),
    _cursor_sprite(_create_cursor_sprite(common_stuff))
{
    _backdrop_bg.set_palette(bn::bg_palette_items::title_backdrop_fade_dark);
    _menu_bg.set_priority(2);
    _menu_bg.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);

    bn::sprite_text_generator& big_text_generator = common_stuff.big_text_generator;
    big_text_generator.set_center_alignment();
    big_text_generator.generate(half_menu_width, initial_y, "3D VIEWER", _main_title_sprites);

    model_viewer_items_status& items_status = common_stuff.storage.model_viewer_items();
    bn::string<32> menu_title_text("Unlocked: ");
    menu_title_text.append(bn::to_string<4>(items_status.unlocked_items_count()));
    menu_title_text.append('/');
    menu_title_text.append(bn::to_string<4>(model_viewer_items_status::items_count()));
    big_text_generator.generate(menu_x + 38, initial_y, menu_title_text, _menu_title_sprites);
    big_text_generator.set_left_alignment();

    _load_menu();

    _models.load_colors(default_model_colors);
    _load_model();

    bn::music_items::credits.play(0.5);
}

bn::optional<scene_type> model_viewer_scene::update()
{
    bn::optional<scene_type> result;

    if(_bgs_fade_in_action.done())
    {
        if(_bgs_fade_out_action)
        {
            if(_bgs_fade_out_action->done())
            {
                if(_model)
                {
                    _models.destroy_dynamic_model(*_model);
                    _model = nullptr;
                }

                bn::blending::set_transparency_alpha(1);
                _common_stuff.storage.write();
                bn::music::stop();
                result = scene_type::TITLE;
            }
            else
            {
                _bgs_fade_out_action->update();
                _sprites_fade_out_action->update();
                _music_volume_action->update();
            }
        }
        else
        {
            if(_showing_menu_frames)
            {
                _show_menu();
            }
            else if(_hiding_menu_frames)
            {
                _hide_menu();
            }
            else
            {
                _update_menu();
                _update_model();
            }
        }
    }
    else
    {
        _bgs_fade_in_action.update();
        _sprites_fade_in_action.update();
    }

    if(_model && &_model->item() == &model_3d_items::character_interrogation)
    {
        bn::fixed interrogation_phi = _interrogation_phi + 128;

        if(interrogation_phi > 65536)
        {
            interrogation_phi -= 65536;
        }

        _model->set_phi(interrogation_phi);
        _interrogation_phi = interrogation_phi;
    }

    _models.update(_camera);
    return result;
}

void model_viewer_scene::_load_menu()
{
    for(int item_index = 0; item_index < max_menu_entries; ++item_index)
    {
        _add_new_menu_entry(item_index, item_index, true);
    }
}

void model_viewer_scene::_add_new_menu_entry(int item_index, int menu_entry_index, bool back)
{
    const model_viewer_item& item = model_viewer_items[item_index];
    model_viewer_items_status& items_status = _common_stuff.storage.model_viewer_items();
    bn::sprite_text_generator& text_generator = _common_stuff.big_text_generator;
    bn::string_view text;

    if(items_status.unlocked_item(item_index))
    {
        text = item.name();

        if(! items_status.viewed_item(item_index))
        {
            text_generator.set_palette_item(bn::sprite_palette_items::big_font_yellow);

            if(item_index == _item_index)
            {
                _cursor_sprite.set_palette(bn::sprite_palette_items::small_font_yellow);
            }
        }
    }
    else
    {
        text = bn::string_view("---");
    }

    menu_entry* new_entry;

    if(back)
    {
        _menu_entries.push_back(menu_entry());
        new_entry = &_menu_entries.back();
    }
    else
    {
        _menu_entries.push_front(menu_entry());
        new_entry = &_menu_entries.front();
    }

    bn::fixed menu_y = initial_menu_y + (menu_entry_index * menu_y_inc);
    text_generator.generate(menu_x, menu_y, text, new_entry->text_sprites);
    text_generator.set_palette_item(bn::sprite_items::big_font.palette_item());
}

void model_viewer_scene::_load_model()
{
    model_viewer_items_status& items_status = _common_stuff.storage.model_viewer_items();
    int item_index = _item_index;

    if(_model)
    {
        _models.destroy_dynamic_model(*_model);
    }

    if(items_status.unlocked_item(item_index))
    {
        const model_viewer_item& item = model_viewer_items[item_index];
        const model_3d_item& model_item = item.model_item();
        const bn::span<const vertex_3d>& model_vertices = model_item.vertices();
        bn::fixed bottom_y = model_vertices[0].point().y();
        bn::fixed top_y = bottom_y;

        for(const vertex_3d& model_vertex : model_vertices)
        {
            bn::fixed z = model_vertex.point().y();

            if(z < bottom_y)
            {
                bottom_y = z;
            }
            else if(z > top_y)
            {
                top_y = z;
            }
        }

        bn::fixed model_z = (top_y - bn::abs(bottom_y)) / 2;
        _model = &_models.create_dynamic_model(model_item);
        _model->set_position(point_3d(0, item.y(), model_z));
        _model->set_phi(item.initial_phi());
        _model->set_theta(item.initial_theta());
        _model->set_psi(item.initial_psi());
    }
    else
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_interrogation);
        _model->set_position(point_3d(0, 168, 0));
        _model->set_phi(_interrogation_phi);
        _model->set_theta(0);
        _model->set_psi(49152);
    }
}

void model_viewer_scene::_show_menu()
{
    bn::fixed inc = menu_x_inc;
    --_showing_menu_frames;

    if(! _showing_menu_frames)
    {
        _menu_keypad.reset();
        _menu_active = true;
    }

    for(bn::sprite_ptr& text_sprite : _main_title_sprites)
    {
        text_sprite.set_x(text_sprite.x() + inc);
    }

    inc *= 2;
    _cursor_sprite.set_x(_cursor_sprite.x() + inc);
    _menu_bg.set_x(_menu_bg.x() + inc);

    for(bn::sprite_ptr& text_sprite : _menu_title_sprites)
    {
        text_sprite.set_x(text_sprite.x() + inc);
    }

    for(menu_entry& entry : _menu_entries)
    {
        for(bn::sprite_ptr& text_sprite : entry.text_sprites)
        {
            text_sprite.set_x(text_sprite.x() + inc);
        }
    }
}

void model_viewer_scene::_hide_menu()
{
    bn::fixed inc = menu_x_inc;
    --_hiding_menu_frames;

    if(! _hiding_menu_frames)
    {
        _menu_active = false;
        _cursor_sprite.set_palette(bn::sprite_items::small_variable_font.palette_item());

        for(bn::sprite_ptr& text_sprite : _menu_entries[_menu_entry_index].text_sprites)
        {
            text_sprite.set_palette(bn::sprite_items::big_font.palette_item());
        }
    }

    for(bn::sprite_ptr& text_sprite : _main_title_sprites)
    {
        text_sprite.set_x(text_sprite.x() - inc);
    }

    inc *= 2;
    _cursor_sprite.set_x(_cursor_sprite.x() - inc);
    _menu_bg.set_x(_menu_bg.x() - inc);

    for(bn::sprite_ptr& text_sprite : _menu_title_sprites)
    {
        text_sprite.set_x(text_sprite.x() - inc);
    }

    for(menu_entry& entry : _menu_entries)
    {
        for(bn::sprite_ptr& text_sprite : entry.text_sprites)
        {
            text_sprite.set_x(text_sprite.x() - inc);
        }
    }
}

void model_viewer_scene::_update_menu()
{
    if(_menu_active)
    {
        _menu_keypad.update();

        if(bn::keypad::a_pressed())
        {
            model_viewer_items_status& items_status = _common_stuff.storage.model_viewer_items();

            if(items_status.unlocked_item(_item_index))
            {
                items_status.view_item(_item_index);
                _hiding_menu_frames = animation_frames;
                bn::sound_items::blip06.play();
            }
            else
            {
                bn::sound_items::blip05.play();
            }
        }
        else if(bn::keypad::b_pressed() || bn::keypad::start_pressed())
        {
            _exit();
        }
        else if(_menu_keypad.up())
        {
            if(_item_index > 0)
            {
                _decrease_item_index();
            }
        }
        else if(_menu_keypad.down())
        {
            if(_item_index < model_viewer_items_status::items_count() - 1)
            {
                _increase_item_index();
            }
        }
    }
}

void model_viewer_scene::_update_model()
{
    if(! _menu_active)
    {
        if(bn::keypad::b_pressed())
        {
            _showing_menu_frames = animation_frames;
            bn::sound_items::blip06.play();
        }
        else if(bn::keypad::start_pressed())
        {
            _exit();
        }
        else
        {
            if(bn::keypad::left_held())
            {
                _model->set_phi(_model->phi() - 256);
            }
            else if(bn::keypad::right_held())
            {
                _model->set_phi(_model->phi() + 256);
            }

            if(bn::keypad::up_held())
            {
                _model->set_psi(_model->psi() - 256);
            }
            else if(bn::keypad::down_held())
            {
                _model->set_psi(_model->psi() + 256);
            }

            if(bn::keypad::l_held())
            {
                _model->set_theta(_model->theta() - 256);
            }
            else if(bn::keypad::r_held())
            {
                _model->set_theta(_model->theta() + 256);
            }
        }
    }
}

void model_viewer_scene::_increase_item_index()
{
    int item_index = _item_index + 1;
    _item_index = item_index;
    bn::sound_items::blip02.play();

    if(_menu_entry_index < max_menu_entries - 2 || item_index == model_viewer_items_status::items_count() - 1)
    {
        ++_menu_entry_index;
    }
    else
    {
        _menu_entries.pop_front();

        for(menu_entry& entry : _menu_entries)
        {
            for(bn::sprite_ptr& text_sprite : entry.text_sprites)
            {
                text_sprite.set_y(text_sprite.y() - menu_y_inc);
            }
        }

        _add_new_menu_entry(item_index + 1, max_menu_entries - 1, true);
    }

    _load_model();
    _update_menu_cursor();
}

void model_viewer_scene::_decrease_item_index()
{
    int item_index = _item_index - 1;
    _item_index = item_index;
    bn::sound_items::blip02.play();

    if(_menu_entry_index > 1 || item_index == 0)
    {
        --_menu_entry_index;
    }
    else
    {
        _menu_entries.pop_back();

        for(menu_entry& entry : _menu_entries)
        {
            for(bn::sprite_ptr& text_sprite : entry.text_sprites)
            {
                text_sprite.set_y(text_sprite.y() + menu_y_inc);
            }
        }

        _add_new_menu_entry(item_index - 1, 0, false);
    }

    _load_model();
    _update_menu_cursor();
}

void model_viewer_scene::_update_menu_cursor()
{
    model_viewer_items_status& items_status = _common_stuff.storage.model_viewer_items();
    int item_index = _item_index;
    _cursor_sprite.set_y(initial_menu_y + 1 + (_menu_entry_index * 12));

    if(items_status.unlocked_item(item_index) && ! items_status.viewed_item(item_index))
    {
        _cursor_sprite.set_palette(bn::sprite_palette_items::small_font_yellow);
    }
    else
    {
        _cursor_sprite.set_palette(bn::sprite_items::small_variable_font.palette_item());
    }
}

void model_viewer_scene::_exit()
{
    _bgs_fade_out_action.emplace(60, 1);
    _sprites_fade_out_action.emplace(60, 1);
    _music_volume_action.emplace(60, 0);
    bn::sound_items::blip09.play();
}

}
