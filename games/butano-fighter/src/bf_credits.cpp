/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_credits.h"

#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_double_size_mode.h"
#include "btn_sprite_affine_mat_attributes.h"
#include "btn_sprite_items_stage_1_animals.h"
#include "btn_sprite_items_stage_1_characters_1.h"
#include "btn_sprite_items_stage_1_characters_2.h"
#include "btn_sprite_items_stage_1_monsters_1.h"
#include "btn_sprite_items_stage_1_monsters_2.h"
#include "btn_sprite_items_stage_1_characters_1_team.h"
#include "btn_sprite_items_stage_2_characters_1.h"
#include "btn_sprite_items_stage_2_characters_2.h"
#include "btn_sprite_items_stage_2_monsters_1.h"
#include "btn_sprite_items_stage_2_monsters_2.h"
#include "btn_sprite_items_stage_2_robot.h"
#include "btn_sprite_items_stage_3_blonde.h"
#include "btn_sprite_items_stage_3_characters_1.h"
#include "btn_sprite_items_stage_3_characters_2.h"
#include "btn_sprite_items_stage_3_monsters_1.h"
#include "btn_sprite_items_stage_3_monsters_2.h"
#include "btn_sprite_items_stage_3_dragon_man.h"
#include "btn_sprite_items_stage_4_brain.h"
#include "btn_sprite_items_stage_4_enemies_1.h"
#include "btn_sprite_items_stage_4_enemies_2.h"
#include "btn_sprite_items_stage_4_enemies_3_1.h"
#include "btn_sprite_items_stage_4_enemies_3_2.h"
#include "btn_sprite_items_stage_4_enemies_4.h"
#include "btn_sprite_items_stage_4_meteors_big.h"
#include "btn_sprite_items_stage_4_meteors_small.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr const btn::fixed text_y_inc = 0.5;
    constexpr const btn::fixed text_y_limit = (btn::display::height() + 16) / 2;
    constexpr const int blending_frames = 90;

    struct background_sprite_item
    {
        const btn::sprite_item* sprite_item;
        int graphics_index_0;
        int graphics_index_1;
        int graphics_index_2;
        int graphics_index_3;
    };

    constexpr const btn::string_view text_items[] = {
        "BUTANO FIGHTER",
        "",
        "",
        "A game made with",
        "BUTANO",
        "github.com/GValiente/butano",
        "",
        "",
        "GBA hardware access and more",
        "provided by Tonclib",
        "coranac.com/projects/#tonc",
        "",
        "",
        "Music and sound provided by",
        "Maxmod",
        "maxmod.devkitpro.org",
        "",
        "",
        "Fast number to string conversion",
        "provided by posprintf",
        "danposluns.com/gbadev"
        "",
        "",
        "Fast math routines provided by",
        "gba-modern",
        "github.com/JoaoBaptMG/gba-modern",
        "",
        "",
        "Built with",
        "devkitARM",
        "devkitpro.org",
        "",
        "",
        "GRAPHICS",
        "(see credits/graphics.txt for details)",
        "",
        "",
        "Butano images",
        "By Sun Ladder",
        "commons.wikimedia.org",
        "",
        "",
        "mountain_clouds.bmp",
        "By Luis Zuno",
        "ansimuz.com",
        "",
        "",
        "Everything else",
        "By Pixel-boy at Sparklin Labs",
        "twitter.com/2pblog1",
        "",
        "",
        "MUSIC",
        "(see credits/music.txt for details)",
        "",
        "",
        "battle_clean.it",
        "By Spring with VinsCool",
        "opengameart.org",
        "",
        "",
        "cyberrid.mod",
        "By jester/sanity",
        "modarchive.org",
        "",
        "",
        "minor_boss_r.it",
        "By Spring/Spring Enterprises",
        "opengameart.org",
        "",
        "",
        "soda7_xcopy_ohc.s3m",
        "By Soda7",
        "modarchive.org",
        "",
        "",
        "stardstm.mod",
        "By jester/sanity",
        "modarchive.org",
        "",
        "",
        "galgox_snowheart_melody.xm",
        "By Galgox, Robyn,",
        "MiDoRi, and ViLXDRYAD",
        "modarchive.org",
        "",
        "",
        "basic_instinct.mod",
        "By jes ^ ter",
        "modarchive.org",
        "",
        "",
        "fruit.mod",
        "By jester/sanity",
        "modarchive.org",
        "",
        "",
        "SOUND EFFECTS",
        "By Pixel-boy at Sparklin Labs",
        "twitter.com/2pblog1",
        "(see credits/sound_effects.txt",
        "for details)",
    };

    constexpr const background_sprite_item background_sprite_items[] = {
        { &btn::sprite_items::stage_1_animals, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_1_animals, 2, 3, 2, 3 },
        { &btn::sprite_items::stage_1_animals, 4, 5, 4, 5 },
        { &btn::sprite_items::stage_1_animals, 6, 7, 6, 7 },
        { &btn::sprite_items::stage_1_characters_1, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_1_characters_1, 2, 3, 2, 3 },
        { &btn::sprite_items::stage_1_characters_1, 4, 5, 4, 5 },
        { &btn::sprite_items::stage_1_characters_1, 6, 7, 6, 7 },
        { &btn::sprite_items::stage_1_characters_2, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_1_characters_2, 2, 3, 2, 3 },
        { &btn::sprite_items::stage_1_characters_2, 4, 5, 4, 5 },
        { &btn::sprite_items::stage_1_characters_2, 6, 7, 6, 7 },
        { &btn::sprite_items::stage_1_monsters_1, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_1_monsters_1, 2, 3, 2, 3 },
        { &btn::sprite_items::stage_1_monsters_2, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_1_monsters_2, 2, 3, 2, 3 },
        { &btn::sprite_items::stage_1_characters_1_team, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_2_characters_1, 0, 1, 2, 3 },
        { &btn::sprite_items::stage_2_characters_1, 16, 17, 18, 19 },
        { &btn::sprite_items::stage_2_characters_1, 32, 33, 34, 35 },
        { &btn::sprite_items::stage_2_characters_1, 48, 49, 48, 49 },
        { &btn::sprite_items::stage_2_characters_2, 0, 1, 2, 3 },
        { &btn::sprite_items::stage_2_characters_2, 16, 17, 18, 19 },
        { &btn::sprite_items::stage_2_characters_2, 32, 33, 34, 35 },
        { &btn::sprite_items::stage_2_characters_2, 48, 49, 50, 51 },
        { &btn::sprite_items::stage_2_monsters_1, 0, 1, 2, 3 },
        { &btn::sprite_items::stage_2_monsters_1, 16, 17, 18, 19 },
        { &btn::sprite_items::stage_2_monsters_1, 32, 33, 34, 35 },
        { &btn::sprite_items::stage_2_monsters_1, 48, 49, 50, 51 },
        { &btn::sprite_items::stage_2_monsters_2, 0, 1, 2, 3 },
        { &btn::sprite_items::stage_2_monsters_2, 16, 17, 18, 19 },
        { &btn::sprite_items::stage_2_monsters_2, 32, 33, 34, 35 },
        { &btn::sprite_items::stage_2_monsters_2, 48, 49, 50, 51 },
        { &btn::sprite_items::stage_2_robot, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_3_blonde, 0, 1, 0, 2 },
        { &btn::sprite_items::stage_3_characters_1, 0, 1, 0, 2 },
        { &btn::sprite_items::stage_3_characters_1, 6, 7, 6, 8 },
        { &btn::sprite_items::stage_3_characters_1, 12, 13, 12, 14 },
        { &btn::sprite_items::stage_3_characters_2, 0, 1, 0, 2 },
        { &btn::sprite_items::stage_3_characters_2, 6, 7, 6, 8 },
        { &btn::sprite_items::stage_3_monsters_1, 0, 1, 0, 1 },
        { &btn::sprite_items::stage_3_monsters_1, 2, 3, 4, 2 },
        { &btn::sprite_items::stage_3_monsters_1, 5, 6, 5, 7 },
        { &btn::sprite_items::stage_3_monsters_2, 0, 1, 0, 2 },
        { &btn::sprite_items::stage_3_monsters_2, 3, 4, 3, 5 },
        { &btn::sprite_items::stage_3_dragon_man, 0, 1, 0, 2 },
        { &btn::sprite_items::stage_4_brain, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_enemies_1, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_enemies_1, 1, 1, 1, 1 },
        { &btn::sprite_items::stage_4_enemies_1, 2, 2, 2, 2 },
        { &btn::sprite_items::stage_4_enemies_2, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_enemies_2, 1, 1, 1, 1 },
        { &btn::sprite_items::stage_4_enemies_3_1, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_enemies_3_2, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_enemies_4, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_enemies_4, 1, 1, 1, 1 },
        { &btn::sprite_items::stage_4_enemies_4, 2, 2, 2, 2 },
        { &btn::sprite_items::stage_4_meteors_big, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_meteors_big, 1, 1, 1, 1 },
        { &btn::sprite_items::stage_4_meteors_small, 0, 0, 0, 0 },
        { &btn::sprite_items::stage_4_meteors_small, 1, 1, 1, 1 },
        { &btn::sprite_items::stage_4_meteors_small, 2, 2, 2, 2 },
    };

    [[nodiscard]] constexpr btn::array<btn::sprite_affine_mat_attributes, btn::display::height()>
    _create_hblank_effect_attributes()
    {
        btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> result;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            btn::sprite_affine_mat_attributes& attributes = result[index];
            btn::fixed sin = btn::lut_sin((index * 256) / btn::display::height());
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
    btn::blending::set_transparency_alpha(0);
    _blending_action.emplace(blending_frames, 0.5);
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

    --_background_sprite_counter;

    if(! _background_sprite_counter)
    {
        _background_sprite_counter = 60;

        unsigned background_sprite_items_count = sizeof(background_sprite_items) / sizeof(*background_sprite_items);
        unsigned background_sprite_index = _random.get() % background_sprite_items_count;
        const background_sprite_item& background_item = background_sprite_items[background_sprite_index];
        int x = (_random.get() % btn::display::width()) - (btn::display::width() / 2);
        int y_limit = (btn::display::height() + 64) / 2;
        btn::sprite_ptr sprite = background_item.sprite_item->create_sprite(x, y_limit);
        sprite.set_affine_mat(_hblank_effect.affine_mat());
        sprite.set_double_size_mode(btn::sprite_double_size_mode::ENABLED);
        sprite.set_blending_enabled(true);
        x = (_random.get() % btn::display::width()) - (btn::display::width() / 2);
        _background_sprites.push_back({
                btn::sprite_move_to_action(sprite, 300, x, -y_limit),
                btn::create_sprite_animate_action_forever(
                        sprite, 6, background_item.sprite_item->tiles_item(),
                        background_item.graphics_index_0, background_item.graphics_index_1,
                        background_item.graphics_index_2, background_item.graphics_index_3)
        });
    }

    if(_blending_action)
    {
        _blending_action->update();

        if(_blending_action->done())
        {
            _blending_action.reset();
        }
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();

            if(_text_entries.empty())
            {
                _background_sprites.clear();
                btn::blending::set_transparency_alpha(1);
                btn::music::stop();
                result = scene_type::TITLE;
            }
        }
    }

    if(! result)
    {
        for(int index = 0, limit = btn::keypad::a_held() ? 6 : 1; index < limit; ++index)
        {
            for(auto it = _text_entries.begin(), end = _text_entries.end(); it != end; )
            {
                text_entry& text = *it;

                if(text.y <= -text_y_limit)
                {
                    _text_entries.erase(it);
                    end = _text_entries.end();

                    if(_text_entries.empty())
                    {
                        _music_volume_action.emplace(blending_frames, 0);
                        _blending_action.emplace(blending_frames, 0);
                    }
                }
                else
                {
                    text.y -= text_y_inc;
                    ++it;

                    for(btn::sprite_ptr& sprite : text.sprites)
                    {
                        sprite.set_y(text.y);
                    }
                }
            }

            if(! _blending_action)
            {
                --_text_counter;

                if(! _text_counter)
                {
                    int text_items_count = sizeof(text_items) / sizeof(*text_items);
                    _text_counter = (16 / text_y_inc).right_shift_integer();

                    if(_text_index < text_items_count)
                    {
                        const btn::string_view& text_item = text_items[_text_index];
                        ++_text_index;

                        text_entry entry;
                        entry.y = text_y_limit;

                        _text_generator.set_center_alignment();
                        _text_generator.generate(0, text_y_limit, text_item, entry.sprites);

                        _text_entries.push_back(btn::move(entry));
                    }
                }
            }
        }
    }

    return result;
}

}
