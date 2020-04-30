#ifndef BTN_HBLANK_EFFECTS_MANAGER_H
#define BTN_HBLANK_EFFECTS_MANAGER_H

#include "btn_common.h"

namespace btn
{
    class hblank_effect_handler;
}

namespace btn::hblank_effects_manager
{
    void init();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    void enable();

    void disable();

    [[nodiscard]] int create(const void* values_ptr, int values_count, int target_id,
                             hblank_effect_handler& handler);

    [[nodiscard]] int optional_create(const void* values_ptr, int values_count, int target_id,
                                      hblank_effect_handler& handler);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] const void* values_ref(int id);

    void set_values_ref(int id, const void* values_ptr, int values_count);

    void reload_values_ref(int id);

    [[nodiscard]] bool visible(int id);

    void set_visible(int id, bool visible);

    void update();

    void commit();
}

#endif
