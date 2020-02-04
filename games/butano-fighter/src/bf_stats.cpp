#include "bf_stats.h"

#include "btn_core.h"
#include "btn_display.h"
#include "btn_input_string_stream.h"
#include "btn_sprite_text_generator.h"

namespace bf
{

stats::stats(const btn::sprite_text_generator& text_generator) :
    _text_generator(text_generator)
{
    set_mode(_mode);
}

void stats::set_mode(mode_type mode)
{
    btn::point display_center = btn::display::center();
    int text_height = _text_generator.font().item().shape_size().height() + 4;
    _static_text_sprites.clear();
    _action.reset();

    switch(mode)
    {

    case mode_type::DISABLED:
        break;

    case mode_type::SIMPLE:
        {
            btn::fixed_point cpu_text_position(8 - display_center.x(), text_height - display_center.y());
            _action.emplace(_text_generator, cpu_text_position, false);
            _action->run();
        }
        break;

    case mode_type::DETAILED:
        {
            btn::string<32> text;
            btn::input_string_stream text_stream(text);
            btn::string_view cpu_label = "CPU: ";
            btn::fixed cpu_label_width = _text_generator.width(cpu_label);
            btn::fixed_point cpu_text_position(cpu_label_width + 8 - display_center.x(), text_height - display_center.y());
            text_stream.append(cpu_label);
            _text_generator.generate(cpu_text_position.x() - cpu_label_width, text_height, text, _static_text_sprites);

            text_stream.clear();
            text_stream.append("IWR: ");
            text_stream.append(btn::memory::used_static_iwram());
            text_stream.append("B");
            _text_generator.generate(8, text_height * 2, text, _static_text_sprites);

            text_stream.clear();
            text_stream.append("EWR: ");
            text_stream.append(btn::memory::used_static_ewram());
            text_stream.append("B");
            _text_generator.generate(8, text_height * 3, text, _static_text_sprites);

            _action.emplace(_text_generator, cpu_text_position, true);
            _action->run();
        }
        break;
    }
}

stats::action::action(const btn::sprite_text_generator& text_generator, const btn::fixed_point& text_position,
                      bool detailed) :
    _detailed(detailed),
    _text_generator(text_generator),
    _text_position(text_position)
{
}

void stats::action::update()
{
    _max_cpu_usage = btn::max(_max_cpu_usage, btn::core::cpu_usage());

    if(! _counter)
    {
        btn::fixed max_cpu_pct = _max_cpu_usage * 100;
        btn::string<32> text;
        btn::input_string_stream text_stream(text);

        if(_detailed)
        {
            text_stream.append(max_cpu_pct);
        }
        else
        {
            text_stream.append(max_cpu_pct.integer());
        }

        text_stream.append("%");
        _text_sprites.clear();
        _text_generator.generate(_text_position, text, _text_sprites);

        _max_cpu_usage = 0;
        _counter = 60;
    }

    --_counter;
}

}
