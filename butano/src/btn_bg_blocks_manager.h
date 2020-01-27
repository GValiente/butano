#ifndef BTN_BG_BLOCKS_MANAGER_H
#define BTN_BG_BLOCKS_MANAGER_H

#include "btn_bg_palette_ptr.h"

namespace btn
{
    class size;
}

namespace btn::bg_blocks_manager
{
    void init();

    [[nodiscard]] optional<int> find(const uint16_t& data_ref, const size& dimensions,
                                     const bg_palette_ptr* palette_ptr);

    [[nodiscard]] optional<int> create(const uint16_t& data_ref, const size& dimensions,
                                       optional<bg_palette_ptr>&& palette_ptr, bool aligned);

    [[nodiscard]] optional<int> allocate(const size& dimensions, optional<bg_palette_ptr>&& palette_ptr, bool aligned);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int hw_id(int id, bool aligned);

    [[nodiscard]] size dimensions(int id);

    [[nodiscard]] const uint16_t* data_ref(int id);

    void set_data_ref(int id, const uint16_t& data_ref, const size& dimensions);

    void reload_data_ref(int id);

    [[nodiscard]] const bg_palette_ptr* palette(int id);

    void set_palette(int id, const optional<bg_palette_ptr>& palette_ptr);

    void set_palette(int id, optional<bg_palette_ptr>&& palette_ptr);

    [[nodiscard]] optional<span<uint16_t>> vram(int id);

    void update();

    void commit();
}

#endif
