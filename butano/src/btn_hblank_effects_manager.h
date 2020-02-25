#ifndef BTN_HBLANK_EFFECTS_MANAGER_H
#define BTN_HBLANK_EFFECTS_MANAGER_H

#include "btn_span_fwd.h"
#include "btn_optional_fwd.h"

namespace btn::hblank_effects_manager
{

    enum class target_type
    {
        REGULAR_BG_POSITION_X_ABSOLUTE,
        REGULAR_BG_POSITION_X_RELATIVE
    };

    void init();

    [[nodiscard]] BTN_CODE_CONST int count();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    void enable();

    void disable();

    [[nodiscard]] optional<int> create(const span<const int16_t>& values_ref, target_type target, int target_id);

    void increase_usages(int id);

    void decrease_usages(int id);

    void update();

    void commit();

    void stop();
}

#endif
