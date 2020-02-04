#ifndef BF_STATS_H
#define BF_STATS_H

#include "btn_action.h"
#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"

namespace btn
{
    class sprite_text_generator;
}


namespace bf
{

class stats
{

public:
    enum class mode_type
    {
        DISABLED,
        SIMPLE,
        DETAILED
    };

    explicit stats(const btn::sprite_text_generator& text_generator);

    [[nodiscard]] mode_type mode() const
    {
        return _mode;
    }

    void set_mode(mode_type mode);

private:
    class action : public btn::action
    {

    public:
        action(const btn::sprite_text_generator& text_generator, const btn::fixed_point& text_position, bool detailed);

        void update() override;

        [[nodiscard]] bool done() const override
        {
            return false;
        }

    private:
        bool _detailed;
        int16_t _counter = 0;
        const btn::sprite_text_generator& _text_generator;
        btn::vector<btn::sprite_ptr, 2 * 4> _text_sprites;
        btn::fixed_point _text_position;
        btn::fixed _max_cpu_usage;
    };

    const btn::sprite_text_generator& _text_generator;
    btn::optional<action> _action;
    btn::vector<btn::sprite_ptr, 8 * 4> _static_text_sprites;
    mode_type _mode = mode_type::SIMPLE;
};

}

#endif
