/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITES_AFFINE_MATS_MANAGER_H
#define BN_SPRITES_AFFINE_MATS_MANAGER_H

#include "bn_fixed.h"
#include "bn_intrusive_list.h"

namespace bn
{
    class sprite_shape_size;
    class affine_mat_attributes;

    using sprite_affine_mat_attach_node_type = intrusive_list_node_type;
}

namespace bn::sprite_affine_mats_manager
{
    class commit_data
    {

    public:
        int offset;
        int count;
    };


    void init(void* handles);

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] int create();

    [[nodiscard]] int create(const affine_mat_attributes& attributes);

    [[nodiscard]] int create_optional();

    [[nodiscard]] int create_optional(const affine_mat_attributes& attributes);

    void increase_usages(int id);

    void decrease_usages(int id);

    void attach_sprite(int id, sprite_affine_mat_attach_node_type& attach_node);

    void dettach_sprite(int id, sprite_affine_mat_attach_node_type& attach_node);

    [[nodiscard]] fixed rotation_angle(int id);

    void set_rotation_angle(int id, fixed rotation_angle);

    [[nodiscard]] fixed horizontal_scale(int id);

    void set_horizontal_scale(int id, fixed horizontal_scale);

    [[nodiscard]] fixed vertical_scale(int id);

    void set_vertical_scale(int id, fixed vertical_scale);

    void set_scale(int id, fixed scale);

    void set_scale(int id, fixed horizontal_scale, fixed vertical_scale);

    [[nodiscard]] fixed horizontal_shear(int id);

    void set_horizontal_shear(int id, fixed horizontal_shear);

    [[nodiscard]] fixed vertical_shear(int id);

    void set_vertical_shear(int id, fixed vertical_shear);

    void set_shear(int id, fixed shear);

    void set_shear(int id, fixed horizontal_shear, fixed vertical_shear);

    [[nodiscard]] bool horizontal_flip(int id);

    void set_horizontal_flip(int id, bool horizontal_flip);

    [[nodiscard]] bool vertical_flip(int id);

    void set_vertical_flip(int id, bool vertical_flip);

    [[nodiscard]] const affine_mat_attributes& attributes(int id);

    void set_attributes(int id, const affine_mat_attributes& attributes);

    [[nodiscard]] bool identity(int id);

    [[nodiscard]] bool flipped_identity(int id);

    [[nodiscard]] bool sprite_double_size(int id, const sprite_shape_size& shape_size);

    void reserve_sprite_handles(int sprite_handles_count);

    void reload(int id);

    void update();

    [[nodiscard]] commit_data retrieve_commit_data();
}

#endif
