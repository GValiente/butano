#ifndef BTN_BGS_MANAGER_H
#define BTN_BGS_MANAGER_H

#include "btn_limits.h"
#include "btn_fixed_fwd.h"

namespace btn
{

class size;
class fixed_point;
class bg_tiles_ptr;
class regular_bg_builder;
class regular_bg_map_ptr;
class regular_bg_attributes;

namespace bgs_manager
{
    [[nodiscard]] BTN_CODE_CONST int max_priority();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] int create(regular_bg_builder&& builder);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] size dimensions(int id);

    [[nodiscard]] const bg_tiles_ptr& tiles(int id);

    void set_tiles(int id, const bg_tiles_ptr& tiles);

    void set_tiles(int id, bg_tiles_ptr&& tiles);

    [[nodiscard]] const regular_bg_map_ptr& map(int id);

    void set_map(int id, const regular_bg_map_ptr& map);

    void set_map(int id, regular_bg_map_ptr&& map);

    [[nodiscard]] const fixed_point& position(int id);

    [[nodiscard]] fixed_point hw_position(int id);

    void set_position(int id, const fixed_point& position);

    [[nodiscard]] int priority(int id);

    void set_priority(int id, int priority);

    [[nodiscard]] bool mosaic_enabled(int id);

    void set_mosaic_enabled(int id, bool mosaic_enabled);

    [[nodiscard]] regular_bg_attributes attributes(int id);

    void set_attributes(int id, const regular_bg_attributes& attributes);

    [[nodiscard]] bool blending_enabled(int id);

    void set_blending_enabled(int id, bool blending_enabled);

    [[nodiscard]] bool visible(int id);

    void set_visible(int id, bool visible);

    [[nodiscard]] bool ignore_camera(int id);

    void set_ignore_camera(int id, bool ignore_camera);

    void update_camera();

    void fill_horizontal_hw_positions(fixed item_position, const fixed& positions_ref, int positions_count,
                                      uint16_t& dest_ref);

    void fill_vertical_hw_positions(fixed item_position, const fixed& positions_ref, int positions_count,
                                    uint16_t& dest_ref);

    void fill_hw_attributes(int id,  const regular_bg_attributes& attributes_ref, int attributes_count,
                            uint16_t& dest_ref);

    void commit();
}

}

#endif
