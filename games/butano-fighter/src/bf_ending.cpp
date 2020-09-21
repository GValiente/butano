#include "bf_ending.h"

#include "btn_string.h"
#include "btn_keypad.h"
#include "btn_version.h"
#include "btn_music_items.h"
#include "btn_sprite_text_generator.h"
#include "btn_regular_bg_items_butano_huge.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

ending::ending(btn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _butano_background(butano_background),
    _blending_action(60, 0)
{
    btn::string<32> middle_text("BUTANO ENGINE ");
    middle_text.append(btn::to_string<4>(btn::version::major()));
    middle_text.append('.');
    middle_text.append(btn::to_string<4>(btn::version::minor()));
    middle_text.append('.');
    middle_text.append(btn::to_string<4>(btn::version::patch()));

    btn::horizontal_alignment_type old_alignment = text_generator.alignment();
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.generate(0, -20, "Made with", _text_sprites);
    text_generator.generate(0, 0, middle_text, _text_sprites);
    text_generator.generate(0, 20, "https://github.com/GValiente/butano", _text_sprites);
    text_generator.set_alignment(old_alignment);

    for(btn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_blending_enabled(true);
    }

    butano_background.set_hidden();
    btn::music_items::fruit.play(0.4);
}

btn::optional<scene_type> ending::update()
{
    btn::optional<scene_type> result;

    if(_counter)
    {
        if(btn::keypad::a_pressed())
        {
            _counter = 0;
        }
        else
        {
            --_counter;
        }
    }
    else
    {
        if(_blending_action.done())
        {
            _text_sprites.clear();
            btn::blending::set_transparency_alpha(1);
            result = scene_type::TITLE;
        }
        else
        {
            _blending_action.update();
        }
    }

    if(_updated)
    {
        if(! _bg)
        {
            _bg = btn::regular_bg_items::butano_huge.create_bg(0, 0);
            _bg->set_priority(2);
            _bg->set_blending_enabled(true);
            _butano_background.set_visible();
            _butano_background.put_under_all();
        }
    }
    else
    {
        _updated = true;
    }

    return result;
}

}
