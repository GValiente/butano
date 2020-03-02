#ifndef BTN_HBLANK_EFFECTS_MANAGER_H
#define BTN_HBLANK_EFFECTS_MANAGER_H

#include "btn_span_fwd.h"
#include "btn_fixed_fwd.h"

namespace btn
{
    class regular_bg_attributes;
}

namespace btn::hblank_effects_manager
{
    enum class target_type
    {
        REGULAR_BG_HORIZONTAL_POSITION,
        REGULAR_BG_VERTICAL_POSITION,
        REGULAR_BG_ATTRIBUTES
    };

    void init();

    [[nodiscard]] BTN_CODE_CONST int count();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    void enable();

    void disable();

    [[nodiscard]] int create(const span<const fixed>& fixed_values_ref, target_type target, int target_id);

    [[nodiscard]] int create(const span<const regular_bg_attributes>& regular_bg_attributes_ref, int target_id);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] span<const fixed> fixed_values_ref(int id);

    void set_values_ref(int id, const void* values_ptr, int values_count);

    void reload_values_ref(int id);

    [[nodiscard]] bool visible(int id);

    void set_visible(int id, bool visible);

    void update();

    void commit();

    void stop();
}

#endif
