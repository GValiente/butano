/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_win_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_sprite_items_small_variable_font.h"
#include "bn_sprite_palette_items_small_font_red.h"
#include "bn_sprite_palette_items_small_font_orange.h"
#include "bn_sprite_palette_items_small_font_yellow.h"

#include "models/fr_model_3d_items_character_w.h"
#include "models/fr_model_3d_items_character_i.h"
#include "models/fr_model_3d_items_character_n.h"

namespace fr
{

namespace
{
    constexpr int model_anim_frames = 32;
    constexpr int wait_frames_delay = 30;

    constexpr bn::fixed w_model_inc_x = bn::fixed(-20) / model_anim_frames;
    constexpr bn::fixed n_model_inc_x = bn::fixed(20) / model_anim_frames;

    constexpr bn::fixed model_source_y = 220;
    constexpr bn::fixed model_target_y = 160;
    constexpr bn::fixed model_inc_y = (model_target_y - model_source_y) / model_anim_frames;

    constexpr bn::fixed model_source_z = 0;
    constexpr bn::fixed model_target_z = -11;
    constexpr bn::fixed model_inc_z = (model_target_z - model_source_z) / model_anim_frames;

    constexpr bn::fixed model_phi = 16384;
    constexpr bn::fixed model_psi = 65536 - 16384;

    constexpr bn::fixed model_source_theta = 65536 - 32768;
    constexpr bn::fixed model_target_theta = 0;
    constexpr bn::fixed model_inc_theta = (model_target_theta - model_source_theta) / model_anim_frames;

    constexpr bn::color model_colors[] = {
        bn::color(31, 22, 0)
    };

    [[nodiscard]] int _unlock_stages(common_stuff& common_stuff)
    {
        sram_data& storage = common_stuff.storage;
        stages_status& stages = storage.stages();
        bool reverse_race = storage.reverse_race();
        int result = 0;

        switch(storage.difficulty())
        {

        case difficulty_level::EASY:
            if(stages.unlock_stage(difficulty_level::EASY, ! reverse_race))
            {
                ++result;
            }

            if(stages.unlock_stage(difficulty_level::NORMAL, reverse_race))
            {
                ++result;
            }
            break;

        case difficulty_level::NORMAL:
            if(stages.unlock_stage(difficulty_level::NORMAL, ! reverse_race))
            {
                ++result;
            }

            if(stages.unlock_stage(difficulty_level::HARD, reverse_race))
            {
                ++result;
            }
            break;

        case difficulty_level::HARD:
            if(stages.unlock_stage(difficulty_level::HARD, ! reverse_race))
            {
                ++result;
            }
            break;

        default:
            BN_ERROR("Invalid difficulty: ", int(storage.difficulty()));
            break;
        }

        return result;
    }

    [[nodiscard]] int _unlock_models(common_stuff& common_stuff)
    {
        sram_data& storage = common_stuff.storage;
        model_viewer_items_status& models = storage.model_viewer_items();
        bool first = common_stuff.finish_position() == 0;
        int result = 0;

        if(storage.reverse_race())
        {
            switch(storage.difficulty())
            {

            case difficulty_level::EASY:
                if(models.unlock_item(2)) // Pickup
                {
                    ++result;
                }

                if(models.unlock_item(10)) // Fences
                {
                    ++result;
                }

                if(first)
                {
                    if(models.unlock_item(16)) // Checkpoint flag
                    {
                        ++result;
                    }

                    if(models.unlock_item(22)) // Fancy stands
                    {
                        ++result;
                    }
                }
                break;

            case difficulty_level::NORMAL:
                if(models.unlock_item(4)) // Candy van
                {
                    ++result;
                }

                if(models.unlock_item(12)) // Pool
                {
                    ++result;
                }

                if(first)
                {
                    if(models.unlock_item(18)) // Antenna
                    {
                        ++result;
                    }

                    if(models.unlock_item(24)) // Bear
                    {
                        ++result;
                    }

                    if(models.unlock_item(28)) // Explosion
                    {
                        ++result;
                    }
                }
                break;

            case difficulty_level::HARD:
                if(models.unlock_item(6)) // Limo
                {
                    ++result;
                }

                if(models.unlock_item(14)) // Big tree
                {
                    ++result;
                }

                if(first)
                {
                    if(models.unlock_item(8)) // AT van
                    {
                        ++result;
                    }

                    if(models.unlock_item(20)) // Regular stands
                    {
                        ++result;
                    }

                    if(models.unlock_item(26)) // Butano house
                    {
                        ++result;
                    }
                }
                break;

            default:
                BN_ERROR("Invalid difficulty: ", int(storage.difficulty()));
                break;
            }
        }
        else
        {
            switch(storage.difficulty())
            {

            case difficulty_level::EASY:
                if(models.unlock_item(1)) // Microcar
                {
                    ++result;
                }

                if(models.unlock_item(9)) // Small tree
                {
                    ++result;
                }

                if(first)
                {
                    if(models.unlock_item(15)) // Start flag
                    {
                        ++result;
                    }

                    if(models.unlock_item(21)) // Hip house
                    {
                        ++result;
                    }

                    if(models.unlock_item(29)) // Timid statue
                    {
                        ++result;
                    }
                }
                break;

            case difficulty_level::NORMAL:
                if(models.unlock_item(3)) // Rental car
                {
                    ++result;
                }

                if(models.unlock_item(11)) // Arbutus tree
                {
                    ++result;
                }

                if(first)
                {
                    if(models.unlock_item(17)) // Spam post
                    {
                        ++result;
                    }

                    if(models.unlock_item(23)) // Gable house
                    {
                        ++result;
                    }

                    if(models.unlock_item(27)) // Turbo boost!
                    {
                        ++result;
                    }
                }
                break;

            case difficulty_level::HARD:
                if(models.unlock_item(5)) // Taxi
                {
                    ++result;
                }

                if(models.unlock_item(13)) // Fat tree
                {
                    ++result;
                }

                if(first)
                {
                    if(models.unlock_item(7)) // Tuned car
                    {
                        ++result;
                    }

                    if(models.unlock_item(19)) // Camera
                    {
                        ++result;
                    }

                    if(models.unlock_item(25)) // Chinese house
                    {
                        ++result;
                    }
                }
                break;

            default:
                BN_ERROR("Invalid difficulty: ", int(storage.difficulty()));
                break;
            }
        }

        return result;
    }

    void _update_storage(common_stuff& common_stuff)
    {
        sram_data& storage = common_stuff.storage;
        stages_status& stages = storage.stages();
        difficulty_level difficulty = storage.difficulty();
        bool reverse_race = storage.reverse_race();

        int best_position = stages.best_position(difficulty, reverse_race).value_or(bn::numeric_limits<int>::max());
        int best_time = stages.best_time(difficulty, reverse_race).value_or(bn::numeric_limits<int>::max());
        int finish_position = common_stuff.finish_position();
        int finish_time = common_stuff.finish_time();

        if(best_position > finish_position || best_time > finish_time)
        {
            if(best_position < finish_position)
            {
                finish_position = best_position;
            }

            if(best_time < finish_time)
            {
                finish_time = best_time;
            }

            stages.set_best_position_and_time(difficulty, reverse_race, finish_position, finish_time);
        }

        storage.write();
    }
}

win_scene::win_scene(common_stuff& common_stuff) :
    _common_stuff(common_stuff),
    _unlocked_stages(_unlock_stages(common_stuff)),
    _unlocked_models(_unlock_models(common_stuff))
{
    _update_storage(common_stuff);

    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.set_center_alignment();

    bn::fixed text_y = 25;

    if(! _unlocked_stages)
    {
        text_y += 16 / 2;
    }

    if(! _unlocked_models)
    {
        text_y += 22 / 2;
    }

    text_generator.set_palette_item(bn::sprite_palette_items::small_font_red);

    switch(common_stuff.finish_position())
    {

    case 0:
        if(stages_status::dollar_time(common_stuff.storage.difficulty(), common_stuff.storage.reverse_race(),
                                      common_stuff.finish_time()))
        {
            text_generator.generate(0, text_y, "YOU FINISHED 1$ST!", _position_text_sprites);
        }
        else
        {
            text_generator.generate(0, text_y, "YOU FINISHED 1ST!", _position_text_sprites);
        }
        break;

    case 1:
        text_generator.generate(0, text_y, "YOU FINISHED 2ND!", _position_text_sprites);
        break;

    default:
        text_generator.generate(0, text_y, "YOU FINISHED 3RD!", _position_text_sprites);
        break;
    }

    text_y += 16;

    if(_unlocked_stages)
    {
        text_generator.set_palette_item(bn::sprite_palette_items::small_font_orange);

        if(_unlocked_stages == 1)
        {
            text_generator.generate(0, text_y, "YOU UNLOCKED 1 NEW COURSE!", _stages_text_sprites);
        }
        else
        {
            text_generator.generate(0, text_y, "YOU UNLOCKED 2 NEW COURSES!", _stages_text_sprites);
        }

        text_y += 16;
    }

    if(_unlocked_models)
    {
        text_generator.set_palette_item(bn::sprite_palette_items::small_font_yellow);

        switch(_unlocked_models)
        {

        case 1:
            text_generator.generate(0, text_y, "YOU UNLOCKED 1 NEW 3D MODEL!", _models_text_sprites);
            break;

        case 2:
            text_generator.generate(0, text_y, "YOU UNLOCKED 2 NEW 3D MODELS!", _models_text_sprites);
            break;

        case 3:
            text_generator.generate(0, text_y, "YOU UNLOCKED 3 NEW 3D MODELS!", _models_text_sprites);
            break;

        case 4:
            text_generator.generate(0, text_y, "YOU UNLOCKED 4 NEW 3D MODELS!", _models_text_sprites);
            break;

        default:
            text_generator.generate(0, text_y, "YOU UNLOCKED 5 NEW 3D MODELS!", _models_text_sprites);
            break;
        }

        text_y += 11;
        text_generator.generate(0, text_y, "CHECK THEM IN THE 3D VIEWER!", _models_text_sprites);
    }

    text_generator.set_left_alignment();
    text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());

    for(bn::sprite_ptr& text_sprite : _position_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _stages_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _models_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    _models.load_colors(model_colors);

    bn::bg_palettes::set_transparent_color(bn::colors::white);
    bn::bg_palettes::set_fade(bn::colors::white, 0);
    bn::sprite_palettes::set_fade(bn::colors::white, 0);
}

bn::optional<scene_type> win_scene::update()
{
    bn::optional<scene_type> result;

    if(! _position_text_sprites[0].visible())
    {
        _animate();
    }
    else if(! _stages_text_sprites.empty() && ! _stages_text_sprites[0].visible())
    {
        if(_wait_counter)
        {
            --_wait_counter;
        }
        else
        {
            for(bn::sprite_ptr& text_sprite : _stages_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            _announcer.play_unlocked_stages_voice(_unlocked_stages);
            _wait_counter = 150 + wait_frames_delay;
        }
    }
    else if(! _models_text_sprites.empty() && ! _models_text_sprites[0].visible())
    {
        if(_wait_counter)
        {
            --_wait_counter;
        }
        else
        {
            for(bn::sprite_ptr& text_sprite : _models_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            _announcer.play_unlocked_models_voice(_unlocked_models);
            _wait_counter = 300 + wait_frames_delay;
        }
    }
    else if(! _done)
    {
        if(_wait_counter)
        {
            --_wait_counter;
        }
        else
        {
            if(_total_updates >= 1100 - 150 || bn::keypad::a_pressed())
            {
                _bgs_fade_action.emplace(60, 1);
                _sprites_fade_action.emplace(60, 1);

                if(bn::music::playing())
                {
                    _volume_action.emplace(60, 0);
                }

                _done = true;
            }
        }
    }
    else if(_bgs_fade_action)
    {
        _bgs_fade_action->update();
        _sprites_fade_action->update();

        if(_volume_action)
        {
            _volume_action->update();
        }

        if(_bgs_fade_action->done())
        {
            _bgs_fade_action.reset();
            _sprites_fade_action.reset();

            if(_volume_action)
            {
                _volume_action.reset();
                bn::music::stop();
            }

            _models.destroy_dynamic_model(*_w_model);
            _models.destroy_dynamic_model(*_i_model);
            _models.destroy_dynamic_model(*_n_model);

            _wait_counter = 30;
        }
    }
    else if(_wait_counter)
    {
        --_wait_counter;
    }
    else
    {
        bn::bg_palettes::remove_transparent_color();

        if(_common_stuff.storage.difficulty() == difficulty_level::EASY)
        {
            result = scene_type::TITLE;
        }
        else
        {
            result = scene_type::CREDITS;
        }
    }

    _models.update(_camera);
    _announcer.update();
    ++_total_updates;
    return result;
}

void win_scene::_animate()
{
    if(! _w_model)
    {
        _w_model = &_models.create_dynamic_model(model_3d_items::character_w);
        _w_model->set_position(point_3d(0, model_source_y, model_source_z));
        _w_model->set_phi(model_phi);
        _w_model->set_psi(model_psi);
        _w_model->set_theta(model_source_theta);
        return;
    }

    point_3d position = _w_model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + w_model_inc_x);
        position.set_y(position.y() + model_inc_y);
        position.set_z(position.z() + model_inc_z);
        _w_model->set_position(position);
        _w_model->set_theta(_w_model->theta() + model_inc_theta);
        return;
    }

    if(! _i_model)
    {
        _i_model = &_models.create_dynamic_model(model_3d_items::character_i);
        _i_model->set_position(point_3d(0, model_source_y, model_source_z));
        _i_model->set_phi(model_phi);
        _i_model->set_psi(model_psi);
        _i_model->set_theta(model_source_theta);
        _announcer.play_win_voice();
        return;
    }

    position = _i_model->position();

    if(position.y() > model_target_y)
    {
        position.set_y(position.y() + model_inc_y);
        position.set_z(position.z() + model_inc_z);
        _i_model->set_position(position);
        _i_model->set_theta(_i_model->theta() + model_inc_theta);
        return;
    }

    if(! _n_model)
    {
        _n_model = &_models.create_dynamic_model(model_3d_items::character_n);
        _n_model->set_position(point_3d(0, model_source_y, model_source_z));
        _n_model->set_phi(model_phi);
        _n_model->set_psi(model_psi);
        _n_model->set_theta(model_source_theta);
        return;
    }

    position = _n_model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + n_model_inc_x);
        position.set_y(position.y() + model_inc_y);
        position.set_z(position.z() + model_inc_z);
        _n_model->set_position(position);
        _n_model->set_theta(_n_model->theta() + model_inc_theta);
        _wait_counter = 45 + wait_frames_delay;
        return;
    }

    if(_wait_counter)
    {
        --_wait_counter;
        return;
    }

    for(bn::sprite_ptr& text_sprite : _position_text_sprites)
    {
        text_sprite.set_visible(true);
    }

    _announcer.play_finish_position_voice(_common_stuff.finish_position());
    _wait_counter = 100 + wait_frames_delay;
}

}
