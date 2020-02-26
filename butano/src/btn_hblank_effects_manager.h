#ifndef BTN_HBLANK_EFFECTS_MANAGER_H
#define BTN_HBLANK_EFFECTS_MANAGER_H

#include "btn_span_fwd.h"

namespace btn::hblank_effects_manager
{
    enum class target_type
    {
        REGULAR_BG_HORIZONTAL_POSITION_ABSOLUTE,
        REGULAR_BG_HORIZONTAL_POSITION_RELATIVE
    };

    void init();

    [[nodiscard]] BTN_CODE_CONST int count();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    void enable();

    void disable();

    [[nodiscard]] int create(const span<const int16_t>& values_ref, target_type target, int target_id);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] const span<const int16_t>& values_ref(int id);

    void set_values_ref(int id, const span<const int16_t>& values_ref);

    void reload_values_ref(int id);

    void update();

    void commit();

    void stop();
}

#endif
