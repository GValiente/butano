#ifndef BTN_BGS_MANAGER_H
#define BTN_BGS_MANAGER_H

#include "btn_limits.h"
#include "btn_fixed_fwd.h"
#include "btn_optional_fwd.h"
#include "btn_config_camera.h"

namespace btn
{

class size;
class fixed_point;
class regular_bg_builder;
class regular_bg_map_ptr;
class regular_bg_attributes;
enum class palette_bpp_mode;

namespace bgs_manager
{
    using id_type = void*;

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] id_type create(regular_bg_builder&& builder);

    [[nodiscard]] id_type create_optional(regular_bg_builder&& builder);

    void increase_usages(id_type id);

    void decrease_usages(id_type id);

    [[nodiscard]] optional<int> hw_id(id_type id);

    [[nodiscard]] size dimensions(id_type id);

    [[nodiscard]] const regular_bg_map_ptr& map(id_type id);

    void set_map(id_type id, const regular_bg_map_ptr& map);

    void set_map(id_type id, regular_bg_map_ptr&& map);

    void remove_map(id_type id);

    [[nodiscard]] const fixed_point& position(id_type id);

    [[nodiscard]] fixed_point hw_position(id_type id);

    void set_x(id_type id, fixed x);

    void set_y(id_type id, fixed y);

    void set_position(id_type id, const fixed_point& position);

    [[nodiscard]] int priority(id_type id);

    void set_priority(id_type id, int priority);

    [[nodiscard]] int z_order(id_type id);

    void set_z_order(id_type id, int z_order);

    [[nodiscard]] bool above(id_type id, id_type other_id);

    void swap_order(id_type id, id_type other_id);

    [[nodiscard]] bool mosaic_enabled(id_type id);

    void set_mosaic_enabled(id_type id, bool mosaic_enabled);

    [[nodiscard]] regular_bg_attributes regular_attributes(id_type id);

    void set_regular_attributes(id_type id, const regular_bg_attributes& attributes);

    [[nodiscard]] bool blending_enabled(id_type id);

    void set_blending_enabled(id_type id, bool blending_enabled);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    #if BTN_CFG_CAMERA_ENABLED
        [[nodiscard]] bool ignore_camera(id_type id);

        void set_ignore_camera(id_type id, bool ignore_camera);

        void update_camera();
    #endif

    void update_map_tiles_cbb(int map_id, int tiles_cbb);

    void update_map_palette_bpp_mode(int map_id, palette_bpp_mode new_bpp_mode);

    void reload();

    void fill_hblank_effect_horizontal_positions(fixed base_position, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_vertical_positions(fixed base_position, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_regular_attributes(id_type id, const regular_bg_attributes* attributes_ptr,
                                               uint16_t* dest_ptr);

    void update();

    void commit();
}

}

#endif
