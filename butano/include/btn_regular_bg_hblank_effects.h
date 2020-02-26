#ifndef BTN_REGULAR_BG_HBLANK_EFFECTS_H
#define BTN_REGULAR_BG_HBLANK_EFFECTS_H

#include "btn_regular_bg_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

enum class regular_bg_hblank_effect_type
{
    HORIZONTAL_POSITION_ABSOLUTE,
    HORIZONTAL_POSITION_RELATIVE,
};

class regular_bg_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static regular_bg_hblank_effect_ptr create(
            regular_bg_ptr bg_ptr, regular_bg_hblank_effect_type type, const span<const int16_t>& values_ref);

    [[nodiscard]] static optional<regular_bg_hblank_effect_ptr> optional_create(
            regular_bg_ptr bg_ptr, regular_bg_hblank_effect_type type, const span<const int16_t>& values_ref);

private:
    regular_bg_ptr _bg_ptr;

    regular_bg_hblank_effect_ptr(int id, regular_bg_ptr&& bg_ptr);
};

}

#endif
