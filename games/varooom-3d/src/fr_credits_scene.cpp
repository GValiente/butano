/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_credits_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_display.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"
#include "fr_model_colors.h"

#include "bn_music_items.h"
#include "bn_regular_bg_items_logo.h"
#include "bn_regular_bg_items_credits_backdrop.h"
#include "bn_regular_bg_items_credits_player_car.h"

#include "models/fr_model_3d_items_bear.h"
#include "models/fr_model_3d_items_pool.h"
#include "models/fr_model_3d_items_spam.h"
#include "models/fr_model_3d_items_house.h"
#include "models/fr_model_3d_items_turbo.h"
#include "models/fr_model_3d_items_antenna.h"
#include "models/fr_model_3d_items_van_car.h"
#include "models/fr_model_3d_items_big_tree.h"
#include "models/fr_model_3d_items_fat_tree.h"
#include "models/fr_model_3d_items_small_car.h"
#include "models/fr_model_3d_items_a_van_car.h"
#include "models/fr_model_3d_items_common_car.h"
#include "models/fr_model_3d_items_pickup_car.h"
#include "models/fr_model_3d_items_player_car.h"
#include "models/fr_model_3d_items_simple_car.h"
#include "models/fr_model_3d_items_small_tree.h"
#include "models/fr_model_3d_items_start_flag.h"
#include "models/fr_model_3d_items_camera_full.h"
#include "models/fr_model_3d_items_explosion_6.h"
#include "models/fr_model_3d_items_stands_full.h"
#include "models/fr_model_3d_items_station_car.h"
#include "models/fr_model_3d_items_statue_full.h"
#include "models/fr_model_3d_items_arbutus_tree.h"
#include "models/fr_model_3d_items_fence_curve_1.h"
#include "models/fr_model_3d_items_gable_house_full.h"
#include "models/fr_model_3d_items_butano_house_full.h"
#include "models/fr_model_3d_items_fancy_stands_full.h"
#include "models/fr_model_3d_items_chinese_house_full.h"
#include "models/fr_model_3d_items_checkpoint_flag_forwards.h"

namespace fr
{

namespace
{
    constexpr bn::fixed text_y_inc = 0.5;
    constexpr bn::fixed text_y_limit = (bn::display::height() + 32) / 2;

    constexpr bn::string_view text_items[] = {
        "",
        "",
        "",
        "",
        "A game made with",
        "Butano",
        "github.com/GValiente/butano",
        "",
        "",
        "",
        "Music and SFX",
        "Tempest",
        "soundcloud.com/janne-suni",
        "",
        "",
        "",
        "SFX sources and original authors",
        "are listed in the credits folder",
        "",
        "",
        "",
        "Announcer voices",
        "Kaappis",
        "twitter.com/kaappis",
        "",
        "",
        "",
        "3D models",
        "GValiente and CA3",
        "",
        "",
        "",
        "Car 3D models based on",
        "Vehicles Assets PT1",
        "opengameart.org",
        "",
        "",
        "",
        "Driving model based on",
        "car.js",
        "github.com/pakastin/car",
        "",
        "",
        "",
        "Varooom 3D logo",
        "Kaappis",
        "twitter.com/kaappis",
        "",
        "",
        "",
        "GBA Jam 2021 intro",
        "Jeremy Clarke",
        "twitter.com/exelotl",
        "",
        "",
        "",
        "GBA Jam 2021 intro text fonts",
        "Noto Sans and Rajdhani",
        "fonts.google.com",
        "",
        "",
        "",
        "GBA Jam 2021 intro backdrop",
        "Lukasz Lada",
        "unsplash.com",
        "",
        "",
        "",
        "Text fonts and other sprites",
        "Pixel-boy at Sparklin Labs",
        "twitter.com/2pblog1",
        "",
        "",
        "",
        "THIRD PARTY LIBRARIES",
        "",
        "",
        "",
        "GBA hardware access and more",
        "Tonclib",
        "coranac.com/man/tonclib",
        "",
        "Universal GBA Library",
        "github.com/AntonioND/ugba",
        "",
        "agbabi",
        "github.com/felixjones/agbabi",
        "",
        "",
        "",
        "Music and sound",
        "Maxmod",
        "maxmod.devkitpro.org",
        "",
        "",
        "",
        "Fast number to string conversion",
        "posprintf",
        "danposluns.com/gbadev",
        "",
        "",
        "",
        "Fast math routines",
        "gba-modern",
        "github.com/JoaoBaptMG/gba-modern",
        "",
        "Universal GBA Library",
        "github.com/AntonioND/ugba",
        "",
        "",
        "",
        "Fast decompression routines",
        "Cult-of-GBA BIOS",
        "github.com/Cult-of-GBA/BIOS",
        "",
        "",
        "",
        "Pool containers",
        "ETL",
        "www.etlcpp.com",
        "",
        "",
        "",
        "Unique type IDs",
        "CTTI",
        "github.com/Manu343726/ctti",
        "",
        "",
        "",
        "Built with",
        "devkitARM",
        "devkitpro.org",
        "",
        "",
        "",
        "Check the credits folder",
        "for licenses and other details",
        "",
        "",
        "",
        "Thank you for playing!",
    };

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_in_action()
    {
        bn::bg_palettes::set_fade_intensity(1);
        return bn::bg_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action()
    {
        bn::sprite_palettes::set_fade_intensity(1);
        return bn::sprite_palettes_fade_to_action(60, 0);
    }
}

credits_scene::credits_scene(common_stuff& common_stuff) :
    _common_stuff(common_stuff),
    _backdrop_bg(bn::regular_bg_items::credits_backdrop.create_bg(0, 0)),
    _player_car_bg(bn::regular_bg_items::credits_player_car.create_bg(
                       (256 - bn::display::width()) / 2, (256 - bn::display::height()) / 2)),
    _logo_bg(bn::regular_bg_items::logo.create_bg(0, 0)),
    _bgs_fade_in_action(_create_bgs_fade_in_action()),
    _sprites_fade_in_action(_create_sprites_fade_in_action())
{
    _logo_bg.set_priority(2);
    _models.load_colors(default_model_colors);
    bn::music_items::credits.play(0.5);
}

bn::optional<scene_type> credits_scene::update()
{
    bn::optional<scene_type> result;

    if(! _bgs_fade_in_action.done())
    {
        _bgs_fade_in_action.update();
        _sprites_fade_in_action.update();
    }
    else if(_bgs_fade_out_action)
    {
        _bgs_fade_out_action->update();
        _sprites_fade_out_action->update();
        _music_volume_action->update();

        if(_bgs_fade_out_action->done())
        {
            _bgs_fade_out_action.reset();
            _sprites_fade_out_action.reset();
            _music_volume_action.reset();
            bn::music::stop();
            bn::bg_palettes::set_transparent_color(bn::colors::black);
            result = scene_type::TITLE;

            if(_model)
            {
                _models.destroy_dynamic_model(*_model);
                _model = nullptr;
            }
        }
    }
    else if(bn::keypad::start_pressed())
    {
        _finish();
    }

    if(! result)
    {
        _update_models();
        _update_text();

        if(_player_car_counter)
        {
            --_player_car_counter;
        }
        else
        {
            constexpr bn::fixed player_car_bg_y = (256 - bn::display::height()) / 2;
            _player_car_counter = 10;

            if(_player_car_bg.y() == player_car_bg_y)
            {
                _player_car_bg.set_y(player_car_bg_y - 1);
            }
            else
            {
                _player_car_bg.set_y(player_car_bg_y);
            }
        }
    }

    _models.update(_camera);

    return result;
}

void credits_scene::_update_models()
{
    constexpr bn::fixed x_range = 256 + 16;
    constexpr bn::fixed car_scale = 2.5;

    if(! _model)
    {
        bn::fixed phi;
        bn::fixed psi = 32768 + 16384;
        bn::fixed scale = 1;
        unsigned model_index = _model_index;
        _model_index = (model_index + 1) % 29;

        switch(model_index)
        {

        case 0:
            _model = &_models.create_dynamic_model(model_3d_items::bear_full);
            phi = -16384;
            break;

        case 1:
            _model = &_models.create_dynamic_model(model_3d_items::gable_house_full);
            break;

        case 2:
            _model = &_models.create_dynamic_model(model_3d_items::camera_full);
            phi = 32768 - 8192;
            break;

        case 3:
            _model = &_models.create_dynamic_model(model_3d_items::rival_car);
            phi = 16384;
            scale = car_scale;
            break;

        case 4:
            _model = &_models.create_dynamic_model(model_3d_items::checkpoint_flag_forwards_full);
            break;

        case 5:
            _model = &_models.create_dynamic_model(model_3d_items::fancy_stands_full);
            phi = 16384;
            break;

        case 6:
            _model = &_models.create_dynamic_model(model_3d_items::house_full);
            break;

        case 7:
            _model = &_models.create_dynamic_model(model_3d_items::pickup_car);
            phi = 16384;
            scale = car_scale;
            break;

        case 8:
            _model = &_models.create_dynamic_model(model_3d_items::chinese_house_full);
            phi = -16384;
            break;

        case 9:
            _model = &_models.create_dynamic_model(model_3d_items::big_tree_full);
            phi = 16384;
            break;

        case 10:
            _model = &_models.create_dynamic_model(model_3d_items::a_van_car);
            phi = 32768 + 16384;
            scale = car_scale;
            break;

        case 11:
            _model = &_models.create_dynamic_model(model_3d_items::butano_house_full);
            phi = 16384;
            break;

        case 12:
            _model = &_models.create_dynamic_model(model_3d_items::antenna_full);
            break;

        case 13:
            _model = &_models.create_dynamic_model(model_3d_items::van_car);
            phi = 32768 + 16384;
            scale = car_scale;
            break;

        case 14:
            _model = &_models.create_dynamic_model(model_3d_items::start_flag_full);
            break;

        case 15:
            _model = &_models.create_dynamic_model(model_3d_items::station_car);
            phi = 32768 + 16384;
            scale = car_scale;
            break;

        case 16:
            _model = &_models.create_dynamic_model(model_3d_items::spam_full);
            phi = 32768;
            break;

        case 17:
            _model = &_models.create_dynamic_model(model_3d_items::explosion_6);
            phi = 28672;
            psi = 32768 + 4096;
            break;

        case 18:
            _model = &_models.create_dynamic_model(model_3d_items::stands_full);
            phi = 32768;
            break;

        case 19:
            _model = &_models.create_dynamic_model(model_3d_items::fat_tree_full);
            break;

        case 20:
            _model = &_models.create_dynamic_model(model_3d_items::simple_car);
            phi = 16384;
            scale = car_scale;
            break;

        case 21:
            _model = &_models.create_dynamic_model(model_3d_items::statue_full);
            break;

        case 22:
            _model = &_models.create_dynamic_model(model_3d_items::common_car);
            phi = 16384;
            scale = car_scale;
            break;

        case 23:
            _model = &_models.create_dynamic_model(model_3d_items::fence_curve_1_full);
            phi = 16384 - 4096;
            break;

        case 24:
            _model = &_models.create_dynamic_model(model_3d_items::arbutus_tree_full);
            break;

        case 25:
            _model = &_models.create_dynamic_model(model_3d_items::small_car);
            phi = 16384;
            scale = car_scale;
            break;

        case 26:
            _model = &_models.create_dynamic_model(model_3d_items::pool_full);
            phi = 16384;
            break;

        case 27:
            _model = &_models.create_dynamic_model(model_3d_items::turbo);
            phi = 16384;
            break;

        case 28:
            _model = &_models.create_dynamic_model(model_3d_items::small_tree_full);
            break;

        default:
            BN_ERROR("Invalid model index: ", model_index);
            break;
        }

        bn::fixed minimum_vertex_y = bn::fixed::from_data(bn::numeric_limits<int>::max());

        for(const vertex_3d& vertex : _model->item().vertices())
        {
            bn::fixed vertex_y = vertex.point().y();

            if(vertex_y < minimum_vertex_y)
            {
                minimum_vertex_y = vertex_y;
            }
        }

        minimum_vertex_y = minimum_vertex_y.unsafe_multiplication(scale);

        _model->set_position(point_3d(-x_range, -128, minimum_vertex_y + 18));
        _model->set_phi(phi);
        _model->set_theta(0);
        _model->set_psi(psi);
        _model->set_scale(scale);
        return;
    }

    point_3d position = _model->position();

    if(position.x() < x_range)
    {
        position.set_x(position.x() + 3);
        _model->set_position(position);
    }
    else
    {
        _models.destroy_dynamic_model(*_model);
        _model = nullptr;
    }
}

void credits_scene::_update_text()
{
    for(int index = 0, limit = bn::keypad::a_held() ? 8 : 1; index < limit; ++index)
    {
        for(auto it = _text_entries.begin(), end = _text_entries.end(); it != end; )
        {
            text_entry& text = *it;
            bn::fixed text_y = text.y;

            if(text_y <= -text_y_limit)
            {
                if(text.logo)
                {
                    _logo_bg.set_visible(false);
                }

                _text_entries.erase(it);
                end = _text_entries.end();

                if(_text_entries.empty())
                {
                    _finish();
                }
            }
            else
            {
                text_y -= text_y_inc;
                text.y = text_y;
                ++it;

                if(text.logo)
                {
                    _logo_bg.set_y(text_y);
                }
                else
                {
                    for(bn::sprite_ptr& sprite : text.sprites)
                    {
                        sprite.set_y(text_y);
                    }
                }
            }
        }

        if(! _bgs_fade_out_action)
        {
            --_text_counter;

            if(! _text_counter)
            {
                int text_items_count = sizeof(text_items) / sizeof(*text_items);
                _text_counter = (16 / text_y_inc).right_shift_integer();

                if(_text_index < text_items_count)
                {
                    text_entry entry;
                    entry.y = text_y_limit;

                    bool logo = _text_index == 0;
                    entry.logo = logo;

                    if(logo)
                    {
                        _logo_bg.set_y(text_y_limit);
                        _logo_bg.set_visible(true);
                    }
                    else
                    {
                        const bn::string_view& text_item = text_items[_text_index];
                        bn::sprite_text_generator& text_generator = _common_stuff.big_text_generator;
                        text_generator.set_center_alignment();
                        text_generator.generate(0, text_y_limit, text_item, entry.sprites);
                        text_generator.set_left_alignment();
                    }

                    _text_entries.push_back(bn::move(entry));
                    ++_text_index;
                }
            }
        }
    }
}

void credits_scene::_finish()
{
    if(! _bgs_fade_out_action)
    {
        bn::bg_palettes::set_fade_color(bn::colors::black);
        bn::sprite_palettes::set_fade_color(bn::colors::black);
        _bgs_fade_out_action.emplace(60, 1);
        _sprites_fade_out_action.emplace(60, 1);
        _music_volume_action.emplace(60, 0);
    }
}

}
