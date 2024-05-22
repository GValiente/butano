/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODELS_3D_H
#define FR_MODELS_3D_H

#include "bn_pool.h"
#include "bn_intrusive_list.h"

#include "fr_model_3d.h"
#include "fr_sprite_3d.h"
#include "fr_shape_groups.h"
#include "fr_constants_3d.h"

namespace fr
{

class camera_3d;

class models_3d
{

public:
    void load_colors(const bn::span<const bn::color>& colors)
    {
        _shape_groups.load_colors(colors);
    }

    void clear_colors()
    {
        _shape_groups.load_colors(bn::span<const bn::color>());
    }

    void set_fade(bn::color color, bn::fixed intensity)
    {
        _shape_groups.set_fade(color, intensity);
    }

    void set_static_model_items(const model_3d_item** static_model_items_ptr, int static_models_count);

    [[nodiscard]] model_3d& create_dynamic_model(const model_3d_item& model_item);

    void destroy_dynamic_model(model_3d& model);

    [[nodiscard]] sprite_3d& create_sprite(sprite_3d_item& sprite_item);

    void destroy_sprite(sprite_3d& sprite);

    void update(const camera_3d& camera);

private:
    static constexpr int _max_models = constants_3d::max_static_models + constants_3d::max_dynamic_models;
    static constexpr int _max_vertices = 256;
    static constexpr int _max_faces = 176;

    static_assert(_max_faces <= bn::numeric_limits<uint8_t>::max());

    struct point_2d
    {
        int16_t x;
        int16_t y;
    };

    struct vertex_2d
    {
        int x;
        int y;
        vertex_2d* prev;
        vertex_2d* next;
    };

    struct valid_face_info
    {
        const face_3d* face;
        const point_2d* projected_vertices;
        int projected_z;
    };

    struct visible_face_info
    {
        const valid_face_info* valid_face;
        int top_index;
        int16_t minimum_x;
        int16_t maximum_x;
        int16_t minimum_y;
        int16_t maximum_y;
    };

    const model_3d_item** _static_model_items_ptr = nullptr;
    int _static_models_count = 0;
    int _static_vertices_count = 0;
    int _static_faces_count = 0;

    bn::pool<model_3d, constants_3d::max_dynamic_models> _dynamic_models_pool;
    bn::intrusive_list<model_3d> _dynamic_models_list;
    bn::pool<sprite_3d, constants_3d::max_sprites> _sprites_pool;
    bn::intrusive_list<sprite_3d> _sprites_list;

    visible_face_info _visible_faces_info[_max_faces];
    shape_groups _shape_groups;
    int _vertices_count = 0;
    int _faces_count = 0;

    #if FR_LOG_POLYGONS_PER_SECOND
        int _total_faces_count = 0;
        int _update_calls = 0;
    #endif

    BN_CODE_IWRAM void _process_models(const camera_3d& camera);
};

}

#endif
