/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_credits.h"

#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_music_items.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_mat_attributes.h"
#include "bn_sprite_double_size_mode.h"
#include "bn_sprite_items_stage_1_animals.h"
#include "bn_sprite_items_stage_1_characters_1.h"
#include "bn_sprite_items_stage_1_characters_2.h"
#include "bn_sprite_items_stage_1_monsters_1.h"
#include "bn_sprite_items_stage_1_monsters_2.h"
#include "bn_sprite_items_stage_1_characters_1_team.h"
#include "bn_sprite_items_stage_2_characters_1.h"
#include "bn_sprite_items_stage_2_characters_2.h"
#include "bn_sprite_items_stage_2_monsters_1.h"
#include "bn_sprite_items_stage_2_monsters_2.h"
#include "bn_sprite_items_stage_2_robot.h"
#include "bn_sprite_items_stage_3_blonde.h"
#include "bn_sprite_items_stage_3_characters_1.h"
#include "bn_sprite_items_stage_3_characters_2.h"
#include "bn_sprite_items_stage_3_monsters_1.h"
#include "bn_sprite_items_stage_3_monsters_2.h"
#include "bn_sprite_items_stage_3_dragon_man.h"
#include "bn_sprite_items_stage_4_brain.h"
#include "bn_sprite_items_stage_4_enemies_1.h"
#include "bn_sprite_items_stage_4_enemies_2.h"
#include "bn_sprite_items_stage_4_enemies_3_1.h"
#include "bn_sprite_items_stage_4_enemies_3_2.h"
#include "bn_sprite_items_stage_4_enemies_4.h"
#include "bn_sprite_items_stage_4_meteors_big.h"
#include "bn_sprite_items_stage_4_meteors_small.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr bn::fixed text_y_inc = 0.5;
    constexpr bn::fixed text_y_limit = (bn::display::height() + 16) / 2;
    constexpr int blending_frames = 90;

    struct background_sprite_item
    {
        const bn::sprite_item* sprite_item;
        int graphics_index_0;
        int graphics_index_1;
        int graphics_index_2;
        int graphics_index_3;
    };

    constexpr bn::string_view text_items[] = {
        "BUTANO FIGHTER",
        "",
        "",
        "",
        "A game made with",
        "Butano",
        "github.com/GValiente/butano",
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
        "GRAPHICS",
        "(see credits/graphics.txt for details)",
        "",
        "",
        "",
        "Butano images",
        "By Sun Ladder",
        "commons.wikimedia.org",
        "",
        "",
        "",
        "mountain_clouds.bmp",
        "By Luis Zuno",
        "ansimuz.com",
        "",
        "",
        "",
        "Everything else",
        "By Pixel-boy at Sparklin Labs",
        "twitter.com/2pblog1",
        "",
        "",
        "",
        "MUSIC",
        "(see credits/music.txt for details)",
        "",
        "",
        "",
        "battle_clean.it",
        "By Spring with VinsCool",
        "opengameart.org",
        "",
        "",
        "",
        "cyberrid.mod",
        "By jester/sanity",
        "modarchive.org",
        "",
        "",
        "",
        "minor_boss_r.it",
        "By Spring/Spring Enterprises",
        "opengameart.org",
        "",
        "",
        "",
        "soda7_xcopy_ohc.s3m",
        "By Soda7",
        "modarchive.org",
        "",
        "",
        "",
        "stardstm.mod",
        "By jester/sanity",
        "modarchive.org",
        "",
        "",
        "",
        "galgox_snowheart_melody.xm",
        "By Galgox, Robyn,",
        "MiDoRi, and ViLXDRYAD",
        "modarchive.org",
        "",
        "",
        "",
        "basic_instinct.mod",
        "By jes ^ ter",
        "modarchive.org",
        "",
        "",
        "",
        "fruit.mod",
        "By jester/sanity",
        "modarchive.org",
        "",
        "",
        "",
        "SOUND EFFECTS",
        "By Pixel-boy at Sparklin Labs",
        "twitter.com/2pblog1",
        "(see credits/sound_effects.txt",
        "for details)",
    };

    constexpr background_sprite_item background_sprite_items[] = {
        { &bn::sprite_items::stage_1_animals, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_1_animals, 2, 3, 2, 3 },
        { &bn::sprite_items::stage_1_animals, 4, 5, 4, 5 },
        { &bn::sprite_items::stage_1_animals, 6, 7, 6, 7 },
        { &bn::sprite_items::stage_1_characters_1, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_1_characters_1, 2, 3, 2, 3 },
        { &bn::sprite_items::stage_1_characters_1, 4, 5, 4, 5 },
        { &bn::sprite_items::stage_1_characters_1, 6, 7, 6, 7 },
        { &bn::sprite_items::stage_1_characters_2, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_1_characters_2, 2, 3, 2, 3 },
        { &bn::sprite_items::stage_1_characters_2, 4, 5, 4, 5 },
        { &bn::sprite_items::stage_1_characters_2, 6, 7, 6, 7 },
        { &bn::sprite_items::stage_1_monsters_1, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_1_monsters_1, 2, 3, 2, 3 },
        { &bn::sprite_items::stage_1_monsters_2, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_1_monsters_2, 2, 3, 2, 3 },
        { &bn::sprite_items::stage_1_characters_1_team, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_2_characters_1, 0, 1, 2, 3 },
        { &bn::sprite_items::stage_2_characters_1, 16, 17, 18, 19 },
        { &bn::sprite_items::stage_2_characters_1, 32, 33, 34, 35 },
        { &bn::sprite_items::stage_2_characters_1, 48, 49, 48, 49 },
        { &bn::sprite_items::stage_2_characters_2, 0, 1, 2, 3 },
        { &bn::sprite_items::stage_2_characters_2, 16, 17, 18, 19 },
        { &bn::sprite_items::stage_2_characters_2, 32, 33, 34, 35 },
        { &bn::sprite_items::stage_2_characters_2, 48, 49, 50, 51 },
        { &bn::sprite_items::stage_2_monsters_1, 0, 1, 2, 3 },
        { &bn::sprite_items::stage_2_monsters_1, 16, 17, 18, 19 },
        { &bn::sprite_items::stage_2_monsters_1, 32, 33, 34, 35 },
        { &bn::sprite_items::stage_2_monsters_1, 48, 49, 50, 51 },
        { &bn::sprite_items::stage_2_monsters_2, 0, 1, 2, 3 },
        { &bn::sprite_items::stage_2_monsters_2, 16, 17, 18, 19 },
        { &bn::sprite_items::stage_2_monsters_2, 32, 33, 34, 35 },
        { &bn::sprite_items::stage_2_monsters_2, 48, 49, 50, 51 },
        { &bn::sprite_items::stage_2_robot, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_3_blonde, 0, 1, 0, 2 },
        { &bn::sprite_items::stage_3_characters_1, 0, 1, 0, 2 },
        { &bn::sprite_items::stage_3_characters_1, 6, 7, 6, 8 },
        { &bn::sprite_items::stage_3_characters_1, 12, 13, 12, 14 },
        { &bn::sprite_items::stage_3_characters_2, 0, 1, 0, 2 },
        { &bn::sprite_items::stage_3_characters_2, 6, 7, 6, 8 },
        { &bn::sprite_items::stage_3_monsters_1, 0, 1, 0, 1 },
        { &bn::sprite_items::stage_3_monsters_1, 2, 3, 4, 2 },
        { &bn::sprite_items::stage_3_monsters_1, 5, 6, 5, 7 },
        { &bn::sprite_items::stage_3_monsters_2, 0, 1, 0, 2 },
        { &bn::sprite_items::stage_3_monsters_2, 3, 4, 3, 5 },
        { &bn::sprite_items::stage_3_dragon_man, 0, 1, 0, 2 },
        { &bn::sprite_items::stage_4_brain, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_enemies_1, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_enemies_1, 1, 1, 1, 1 },
        { &bn::sprite_items::stage_4_enemies_1, 2, 2, 2, 2 },
        { &bn::sprite_items::stage_4_enemies_2, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_enemies_2, 1, 1, 1, 1 },
        { &bn::sprite_items::stage_4_enemies_3_1, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_enemies_3_2, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_enemies_4, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_enemies_4, 1, 1, 1, 1 },
        { &bn::sprite_items::stage_4_enemies_4, 2, 2, 2, 2 },
        { &bn::sprite_items::stage_4_meteors_big, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_meteors_big, 1, 1, 1, 1 },
        { &bn::sprite_items::stage_4_meteors_small, 0, 0, 0, 0 },
        { &bn::sprite_items::stage_4_meteors_small, 1, 1, 1, 1 },
        { &bn::sprite_items::stage_4_meteors_small, 2, 2, 2, 2 },
    };

    constexpr bn::array<bn::affine_mat_attributes, bn::display::height()> _hbe_attributes = []{
        bn::array<bn::affine_mat_attributes, bn::display::height()> result;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            bn::affine_mat_attributes& attributes = result[index];
            bn::fixed sin = bn::lut_sin((index * 1024) / bn::display::height());
            attributes.set_rotation_angle((index * 360) / bn::display::height());
            attributes.set_scale(1 + (sin / 2));
        }

        return result;
    }();
}

credits::credits(bn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _text_generator(text_generator),
    _hbe(bn::sprite_affine_mat_attributes_hbe_ptr::create(bn::sprite_affine_mat_ptr::create(), _hbe_attributes))
{
    bn::blending::set_transparency_alpha(0);
    _blending_action.emplace(blending_frames, bn::fixed(0.5));
    butano_background.put_under_all();

    if(! bn::music::playing())
    {
        bn::music_items::fruit.play(0.4);
    }
}

bn::optional<scene_type> credits::update()
{
    bn::optional<scene_type> result;

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
        int x = (_random.get() % bn::display::width()) - (bn::display::width() / 2);
        int y_limit = (bn::display::height() + 64) / 2;
        bn::sprite_ptr sprite = background_item.sprite_item->create_sprite(x, y_limit);
        sprite.set_affine_mat(_hbe.affine_mat());
        sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
        sprite.set_blending_enabled(true);
        x = (_random.get() % bn::display::width()) - (bn::display::width() / 2);
        _background_sprites.push_back({
                bn::sprite_move_to_action(sprite, 300, x, -y_limit),
                bn::create_sprite_animate_action_forever(
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
                bn::blending::set_transparency_alpha(1);
                bn::music::stop();
                result = scene_type::TITLE;
            }
        }
    }

    if(! result)
    {
        for(int index = 0, limit = bn::keypad::a_held() ? 6 : 1; index < limit; ++index)
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

                    for(bn::sprite_ptr& sprite : text.sprites)
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
                        const bn::string_view& text_item = text_items[_text_index];
                        ++_text_index;

                        text_entry entry;
                        entry.y = text_y_limit;

                        _text_generator.set_center_alignment();
                        _text_generator.generate(0, text_y_limit, text_item, entry.sprites);

                        _text_entries.push_back(bn::move(entry));
                    }
                }
            }
        }
    }

    return result;
}

}
