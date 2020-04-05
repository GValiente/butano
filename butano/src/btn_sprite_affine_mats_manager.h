#ifndef BTN_SPRITES_AFFINE_MATS_MANAGER_H
#define BTN_SPRITES_AFFINE_MATS_MANAGER_H

#include "btn_fixed_fwd.h"
#include "btn_optional_fwd.h"

namespace btn
{
    class sprite_affine_mat_builder;
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

    [[nodiscard]] int create(const sprite_affine_mat_builder& builder);

    void increase_usages(int id);

    void decrease_usages(int id);

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

    [[nodiscard]] bool is_identity(int id);

    [[nodiscard]] bool double_size(int id);

    [[nodiscard]] bool updated(int id);

    [[nodiscard]] bool updated();

    void update();

    [[nodiscard]] optional<commit_data> retrieve_commit_data();
}

#endif
