#ifndef BTN_SPRITES_AFFINE_MATS_MANAGER_H
#define BTN_SPRITES_AFFINE_MATS_MANAGER_H

#include "btn_fixed_fwd.h"
#include "btn_optional_fwd.h"
#include "btn_intrusive_list.h"

namespace btn
{
    class sprite_affine_mat_attributes;

    using sprite_affine_mat_attach_node_type = intrusive_list_node_type;
}

namespace btn::sprite_affine_mats_manager
{
    class commit_data
    {

    public:
        int offset;
        int count;
    };


    void init(int handles_size, void* handles);

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] int create();

    [[nodiscard]] int create(const sprite_affine_mat_attributes& attributes);

    [[nodiscard]] int create_optional();

    [[nodiscard]] int create_optional(const sprite_affine_mat_attributes& attributes);

    void increase_usages(int id);

    void decrease_usages(int id);

    void attach_sprite(int id, sprite_affine_mat_attach_node_type& attach_node);

    void dettach_sprite(int id, sprite_affine_mat_attach_node_type& attach_node);

    [[nodiscard]] fixed rotation_angle(int id);

    void set_rotation_angle(int id, fixed rotation_angle);

    [[nodiscard]] fixed scale_x(int id);

    void set_scale_x(int id, fixed scale_x);

    [[nodiscard]] fixed scale_y(int id);

    void set_scale_y(int id, fixed scale_y);

    void set_scale(int id, fixed scale);

    void set_scale(int id, fixed scale_x, fixed scale_y);

    [[nodiscard]] bool horizontal_flip(int id);

    void set_horizontal_flip(int id, bool horizontal_flip);

    [[nodiscard]] bool vertical_flip(int id);

    void set_vertical_flip(int id, bool vertical_flip);

    [[nodiscard]] const sprite_affine_mat_attributes& attributes(int id);

    void set_attributes(int id, const sprite_affine_mat_attributes& attributes);

    [[nodiscard]] bool identity(int id);

    [[nodiscard]] bool flipped_identity(int id);

    [[nodiscard]] bool double_size(int id);

    void update();

    [[nodiscard]] optional<commit_data> retrieve_commit_data();
}

#endif
