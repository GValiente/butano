#ifndef BTN_HBLANK_EFFECT_HANDLER_H
#define BTN_HBLANK_EFFECT_HANDLER_H

#include "btn_any_fwd.h"

namespace btn
{

class hblank_effect_handler
{

public:
    virtual ~hblank_effect_handler() = default;

    virtual void setup_target(int target_id, iany& target_last_value) = 0;

    [[nodiscard]] virtual bool target_visible(int target_id) = 0;

    [[nodiscard]] virtual bool target_updated(int target_id, iany& target_last_value) = 0;

    [[nodiscard]] virtual uint16_t* output_register(int target_id, const iany& target_last_value) = 0;

    virtual void write_output_values(int target_id, const iany& target_last_value, const void* input_values_ptr,
                                     uint16_t* output_values_ptr) = 0;

protected:
    hblank_effect_handler() = default;
};

}

#endif
