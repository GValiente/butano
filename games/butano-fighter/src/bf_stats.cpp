/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_stats.h"

#include "bn_core.h"
#include "bn_string.h"
#include "bn_memory.h"
#include "bn_display.h"
#include "bn_sprite_text_generator.h"

namespace bf
{

stats::stats(bn::sprite_text_generator& text_generator) :
    _text_generator(text_generator)
{
    set_mode(_mode);
}

void stats::set_mode(mode_type mode)
{
    int text_x = 8 - (bn::display::width() / 2);
    int text_height = _text_generator.font().item().shape_size().height() + 4;
    _mode = mode;
    _static_text_sprites.clear();
    _text_sprites.clear();
    _max_cpu_usage = 0;
    _counter = 0;

    switch(mode)
    {

    case mode_type::DISABLED:
        break;

    case mode_type::SIMPLE:
        _text_position = bn::fixed_point(text_x, text_height - (bn::display::height() / 2));
        break;

    case mode_type::DETAILED:
        {
            bn::string_view cpu_label = "CPU: ";
            bn::fixed cpu_label_width = _text_generator.width(cpu_label);
            _text_position = bn::fixed_point(text_x + cpu_label_width, text_height - (bn::display::height() / 2));

            bn::sprite_text_generator::alignment_type old_alignment = _text_generator.alignment();
            _text_generator.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);

            int old_bg_priority = _text_generator.bg_priority();
            _text_generator.set_bg_priority(0);

            bn::string<32> text;
            bn::ostringstream text_stream(text);
            text_stream.append(cpu_label);
            _text_generator.generate(text_x, _text_position.y(), text, _static_text_sprites);

            text.clear();
            text_stream.append("IWR: ");
            text_stream.append(bn::memory::used_static_iwram());
            text_stream.append("B");
            _text_generator.generate(text_x, _text_position.y() + text_height, text, _static_text_sprites);

            text.clear();
            text_stream.append("EWR: ");
            text_stream.append(bn::memory::used_static_ewram());
            text_stream.append("B");
            _text_generator.generate(text_x, _text_position.y() + (text_height * 2), text, _static_text_sprites);

            _text_generator.set_alignment(old_alignment);
            _text_generator.set_bg_priority(old_bg_priority);
        }
        break;

    default:
        BN_ERROR("Invalid mode: ", int(mode));
        break;
    }
}

void stats::update()
{
    switch(_mode)
    {

    case mode_type::DISABLED:
        return;

    case mode_type::SIMPLE:
        break;

    case mode_type::DETAILED:
        break;

    default:
        BN_ERROR("Invalid mode: ", int(_mode));
        break;
    }

    _max_cpu_usage = bn::max(_max_cpu_usage, bn::core::last_cpu_usage());

    if(! _counter)
    {
        bn::fixed max_cpu_pct = _max_cpu_usage * 100;
        bn::string<32> text;
        bn::ostringstream text_stream(text);

        switch(_mode)
        {

        case mode_type::DISABLED:
            break;

        case mode_type::SIMPLE:
            text_stream.append(max_cpu_pct.right_shift_integer());
            break;

        case mode_type::DETAILED:
            text_stream.set_precision(4);
            text_stream.append(max_cpu_pct);
            break;

        default:
            BN_ERROR("Invalid mode: ", int(_mode));
            break;
        }

        text_stream.append('%');

        bn::sprite_text_generator::alignment_type old_alignment = _text_generator.alignment();
        int old_bg_priority = _text_generator.bg_priority();
        _text_generator.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
        _text_generator.set_bg_priority(0);
        _text_sprites.clear();
        _text_generator.generate(_text_position, text, _text_sprites);
        _text_generator.set_alignment(old_alignment);
        _text_generator.set_bg_priority(old_bg_priority);

        _max_cpu_usage = 0;
        _counter = 60;
    }

    --_counter;
}

}
