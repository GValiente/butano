/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_models_3d.h"

#include "bn_profiler.h"
#include "../../butano/hw/include/bn_hw_sprites.h"

#include "fr_div_lut.h"
#include "fr_camera_3d.h"
#include "fr_sprite_3d_item.h"

#if FR_DETAILED_PROFILE
    #define FR_PROFILER_START BN_PROFILER_START
    #define FR_PROFILER_STOP BN_PROFILER_STOP
#else
    #define FR_PROFILER_START(id) \
        do \
        { \
        } while(false)

    #define FR_PROFILER_STOP() \
        do \
        { \
        } while(false)
#endif

namespace fr
{

namespace
{
    constexpr int fixed_precision = 18;
    using fixed = bn::fixed_t<fixed_precision>;
}

void models_3d::_process_models(const camera_3d& camera)
{
    constexpr int display_width = bn::display::width();
    constexpr int display_height = bn::display::height();
    constexpr int focal_length_shift = constants_3d::focal_length_shift;
    constexpr int near_plane = 24 * 256 * 16;

    point_2d _projected_vertices[_max_vertices];
    valid_face_info _valid_faces_info[_max_faces];
    int _visible_face_projected_zs[_max_faces];
    uint8_t _visible_face_indexes[_max_faces];

    point_3d camera_position = camera.position();
    bn::fixed camera_phi = camera.phi();
    bn::fixed camera_u_x = camera.u().x();
    bn::fixed camera_u_z = camera.u().z();
    bn::fixed camera_v_x = camera.v().x();
    bn::fixed camera_v_z = camera.v().z();
    int global_vertex_index = 0;
    int valid_faces_count = 0;

    // Project static models:

    FR_PROFILER_START("static_project");

    for(int static_model_index = _static_models_count - 1; static_model_index >= 0; --static_model_index)
    {
        const model_3d_item* model_item = _static_model_items_ptr[static_model_index];
        const vertex_3d* model_vertices = model_item->vertices().data();
        point_2d* projected_vertices = _projected_vertices + global_vertex_index;
        int model_vertices_count = model_item->vertices().size();
        bool valid_model = true;

        for(int index = 0; index < model_vertices_count; ++index)
        {
            const point_3d& model_point = model_vertices[index].point();
            bn::fixed vry = model_point.y() - camera_position.y();
            int vcz = -vry.data();

            if(near_plane <= vcz) [[likely]]
            {
                bn::fixed vrx = (model_point.x() - camera_position.x()) / 16;
                bn::fixed vrz = (model_point.z() - camera_position.z()) / 16;
                int vcx = (vrx.unsafe_multiplication(camera_u_x) + vrz.unsafe_multiplication(camera_u_z)).data();
                int vcy = -(vrx.unsafe_multiplication(camera_v_x) + vrz.unsafe_multiplication(camera_v_z)).data();

                // int scale = (1 << (focal_length_shift + 16 + 4)) / vcz;
                auto scale = int((div_lut_ptr[vcz >> 10] << (focal_length_shift - 8)) >> 6);

                *projected_vertices = {
                    int16_t(((vcx * scale) >> 16) + (display_width / 2)),
                    int16_t(((vcy * scale) >> 16) + (display_height / 2))
                };

                ++projected_vertices;
            }
            else
            {
                valid_model = false;
                break;
            }
        }

        if(valid_model) [[likely]]
        {
            const face_3d* model_faces = model_item->faces().data();
            int model_faces_count = model_item->faces().size();
            projected_vertices = _projected_vertices + global_vertex_index;

            for(int index = model_faces_count - 1; index >= 0; --index)
            {
                const face_3d& face = model_faces[index];
                const point_3d& centroid = face.centroid().point();
                const point_3d& normal = face.normal().point();
                point_3d vr = centroid - camera_position;

                if(vr.safe_dot_product(normal) < 0) [[likely]]
                {
                    int projected_z = -vr.y().data();

                    _valid_faces_info[valid_faces_count] = {
                        &face, projected_vertices, projected_z
                    };

                    ++valid_faces_count;
                }
            }

            global_vertex_index += model_vertices_count;
        }
    }

    FR_PROFILER_STOP();

    // Project dynamic models:

    FR_PROFILER_START("dynamic_project");

    for(model_3d& model : _dynamic_models_list)
    {
        const model_3d_item& model_item = model.item();
        const vertex_3d* model_vertices = model_item.vertices().data();
        point_2d* projected_vertices = _projected_vertices + global_vertex_index;
        int model_vertices_count = model_item.vertices().size();
        bool valid_model = true;
        model.update();

        for(int index = 0; index < model_vertices_count; ++index)
        {
            point_3d model_point = model.transform(model_vertices[index]);
            bn::fixed vry = model_point.y() - camera_position.y();
            int vcz = -vry.data();

            if(near_plane <= vcz) [[likely]]
            {
                bn::fixed vrx = (model_point.x() - camera_position.x()) / 16;
                bn::fixed vrz = (model_point.z() - camera_position.z()) / 16;
                int vcx = (vrx.unsafe_multiplication(camera_u_x) + vrz.unsafe_multiplication(camera_u_z)).data();
                int vcy = -(vrx.unsafe_multiplication(camera_v_x) + vrz.unsafe_multiplication(camera_v_z)).data();

                // int scale = (1 << (focal_length_shift + 16 + 4)) / vcz;
                auto scale = int((div_lut_ptr[vcz >> 10] << (focal_length_shift - 8)) >> 6);

                *projected_vertices = {
                    int16_t(((vcx * scale) >> 16) + (display_width / 2)),
                    int16_t(((vcy * scale) >> 16) + (display_height / 2))
                };

                ++projected_vertices;
            }
            else
            {
                valid_model = false;
                break;
            }
        }

        if(valid_model) [[likely]]
        {
            const face_3d* model_faces = model_item.faces().data();
            int model_faces_count = model_item.faces().size();
            projected_vertices = _projected_vertices + global_vertex_index;

            for(int index = model_faces_count - 1; index >= 0; --index)
            {
                const face_3d& face = model_faces[index];
                point_3d centroid = model.transform(face.centroid());
                point_3d normal = model.rotate(face.normal());
                point_3d vr = centroid - camera_position;

                if(vr.safe_dot_product(normal) < 0) [[likely]]
                {
                    int projected_z = -vr.y().data();

                    _valid_faces_info[valid_faces_count] = {
                        &face, projected_vertices, projected_z
                    };

                    ++valid_faces_count;
                }
            }

            global_vertex_index += model_vertices_count;
        }
    }

    FR_PROFILER_STOP();

    // Cull valid faces:

    visible_face_info* visible_faces = _visible_faces_info;
    int visible_faces_count = 0;

    FR_PROFILER_START("cull_valid_faces");

    for(int face_index = valid_faces_count - 1; face_index >= 0; --face_index)
    {
        const valid_face_info& valid_face = _valid_faces_info[face_index];
        const face_3d* face = valid_face.face;
        const point_2d* projected_vertices = valid_face.projected_vertices;
        const point_2d& pv0 = projected_vertices[face->first_vertex_index()];
        const point_2d& pv1 = projected_vertices[face->second_vertex_index()];
        const point_2d& pv2 = projected_vertices[face->third_vertex_index()];
        const point_2d& pv3 = projected_vertices[face->fourth_vertex_index()];
        int16_t minimum_x = pv0.x;
        int16_t maximum_x = minimum_x;

        auto min_max_x = [&minimum_x, &maximum_x](int16_t value)
        {
            if(value < minimum_x)
            {
                minimum_x = value;
            }
            else if(value > maximum_x)
            {
                maximum_x = value;
            }
        };

        min_max_x(pv1.x);
        min_max_x(pv2.x);
        min_max_x(pv3.x);

        if(minimum_x < display_width && maximum_x >= 0) [[likely]]
        {
            int16_t minimum_y = pv0.y;
            int16_t maximum_y = minimum_y;
            int top_index = 0;

            auto min_max_y = [&minimum_y, &maximum_y, &top_index](int index, int16_t value)
            {
                if(value < minimum_y)
                {
                    top_index = index;
                    minimum_y = value;
                }
                else if(value > maximum_y)
                {
                    maximum_y = value;
                }
            };

            min_max_y(1, pv1.y);
            min_max_y(2, pv2.y);
            min_max_y(3, pv3.y);

            if(minimum_y < display_height && maximum_y >= 0)
            {
                visible_faces[visible_faces_count] = {
                    &valid_face, top_index, minimum_x, maximum_x, minimum_y, maximum_y
                };

                _visible_face_projected_zs[visible_faces_count] = valid_face.projected_z;
                _visible_face_indexes[visible_faces_count] = visible_faces_count;
                ++visible_faces_count;
            }
        }
    }

    FR_PROFILER_STOP();

    // Project and cull sprites:

    FR_PROFILER_START("sprites");

    for(sprite_3d& sprite : _sprites_list)
    {
        const point_3d& sprite_position = sprite.position();
        bn::fixed vry = sprite_position.y() - camera_position.y();
        int vcz = -vry.data();

        if(near_plane <= vcz) [[likely]]
        {
            bn::fixed vrx = (sprite_position.x() - camera_position.x()) / 16;
            bn::fixed vrz = (sprite_position.z() - camera_position.z()) / 16;
            int vcx = (vrx.unsafe_multiplication(camera_u_x) + vrz.unsafe_multiplication(camera_u_z)).data();

            auto sprite_scale = int((div_lut_ptr[vcz >> 10] << (focal_length_shift - 8)) >> 3);
            auto scale = sprite_scale >> 3;
            int sprite_x = ((vcx * scale) >> 16) + (display_width / 2) - 32;

            if(sprite_x < display_width && sprite_x + 64 > 0) [[likely]]
            {
                int vcy = -(vrx.unsafe_multiplication(camera_v_x) + vrz.unsafe_multiplication(camera_v_z)).data();
                int sprite_y = ((vcy * scale) >> 16) + (display_height / 2) - 32;

                if(sprite_y < display_height && sprite_y + 64 > 0) [[likely]]
                {
                    bn::fixed affine_scale = bn::fixed::from_data(sprite_scale).unsafe_multiplication(sprite.scale());

                    if(affine_scale > 0) [[likely]]
                    {
                        int degrees = (camera_phi + sprite.theta()).right_shift_integer() * 360;
                        bn::fixed rotation_angle = bn::fixed::from_data(degrees >> 4);

                        if(rotation_angle >= 360)
                        {
                            rotation_angle -= 360;
                        }

                        sprite_3d_item& sprite_item = sprite.item();
                        bn::sprite_affine_mat_ptr& affine_mat = sprite_item.affine_mat();
                        affine_mat.set_scale(affine_scale);
                        affine_mat.set_rotation_angle(rotation_angle);

                        int attr0 = bn::hw::sprites::first_attributes(
                                    sprite_y, bn::sprite_shape::SQUARE, bn::bpp_mode::BPP_4, 1 << 8,
                                    true, false, false, false);
                        int attr1 = bn::hw::sprites::second_attributes(
                                    sprite_x, bn::sprite_size::HUGE, sprite_item.affine_mat_id());
                        int attr2 = bn::hw::sprites::third_attributes(
                                    sprite_item.tiles_id(), sprite_item.palette_id(), 3);

                        visible_faces[visible_faces_count] = {
                            nullptr, sprite_y, int16_t(attr0), int16_t(attr1), int16_t(attr2), 0
                        };

                        _visible_face_projected_zs[visible_faces_count] = vcz;
                        _visible_face_indexes[visible_faces_count] = visible_faces_count;
                        ++visible_faces_count;
                    }
                }
            }
        }
    }

    FR_PROFILER_STOP();

    if(! visible_faces_count) [[unlikely]]
    {
        return;
    }

    // Sort visible faces:

    FR_PROFILER_START("sort_visible_faces");

    const int* projected_zs = _visible_face_projected_zs;

    bn::sort(_visible_face_indexes, _visible_face_indexes + visible_faces_count, [projected_zs](uint8_t a, uint8_t b)
    {
        return projected_zs[a] > projected_zs[b];
    });

    FR_PROFILER_STOP();

    // Render visible faces:

    FR_PROFILER_START("render_visible_faces");

    _shape_groups.enable_drawing();

    for(int visible_face_index = visible_faces_count - 1; visible_face_index >= 0; --visible_face_index)
    {
        const visible_face_info& visible_face = visible_faces[_visible_face_indexes[visible_face_index]];

        if(const valid_face_info* valid_face = visible_face.valid_face) [[likely]]
        {
            const face_3d* face = valid_face->face;
            int minimum_x = visible_face.minimum_x;
            int maximum_x = visible_face.maximum_x;
            int minimum_y = visible_face.minimum_y;
            int maximum_y = visible_face.maximum_y;

            shape_groups::hline hlines[bn::display::height()];
            bool x_outside = false;

            if(minimum_x < 0)
            {
                minimum_x = 0;
                x_outside = true;
            }

            if(maximum_x > display_width - 1)
            {
                maximum_x = display_width - 1;
                x_outside = true;
            }

            if(minimum_y != maximum_y) [[likely]]
            {
                int y = minimum_y;

                if(minimum_y < 0)
                {
                    minimum_y = 0;
                }

                if(maximum_y > display_height - 1)
                {
                    maximum_y = display_height - 1;
                }

                vertex_2d vertices[4];
                const point_2d* projected_vertices = valid_face->projected_vertices;
                const point_2d& pv0 = projected_vertices[face->first_vertex_index()];
                vertices[0].x = pv0.x;
                vertices[0].y = pv0.y;
                vertices[0].next = &vertices[1];

                const point_2d& pv1 = projected_vertices[face->second_vertex_index()];
                vertices[1].x = pv1.x;
                vertices[1].y = pv1.y;
                vertices[1].prev = &vertices[0];
                vertices[1].next = &vertices[2];

                const point_2d& pv2 = projected_vertices[face->third_vertex_index()];
                vertices[2].x = pv2.x;
                vertices[2].y = pv2.y;
                vertices[2].prev = &vertices[1];

                if(face->triangle())
                {
                    vertices[0].prev = &vertices[2];
                    vertices[2].next = &vertices[0];
                }
                else
                {
                    vertices[0].prev = &vertices[3];
                    vertices[2].next = &vertices[3];

                    const point_2d& pv3 = projected_vertices[face->fourth_vertex_index()];
                    vertices[3].x = pv3.x;
                    vertices[3].y = pv3.y;
                    vertices[3].prev = &vertices[2];
                    vertices[3].next = &vertices[0];
                }

                vertex_2d& top_vertex = vertices[visible_face.top_index];
                vertex_2d* left_top = &top_vertex;
                vertex_2d* right_top = &top_vertex;
                vertex_2d* left_bottom = top_vertex.next;
                vertex_2d* right_bottom = top_vertex.prev;

                while(left_top->y == left_bottom->y) [[unlikely]]
                {
                    left_top = left_bottom;
                    left_bottom = left_bottom->next;
                }

                while(right_top->y == right_bottom->y) [[unlikely]]
                {
                    right_top = right_bottom;
                    right_bottom = right_bottom->prev;
                }

                fixed xl = left_top->x;
                fixed xr = right_top->x;

                fixed left_delta = unsafe_unsigned_lut_division<fixed_precision>(
                            left_bottom->x - left_top->x, left_bottom->y - left_top->y);
                fixed right_delta = unsafe_unsigned_lut_division<fixed_precision>(
                            right_bottom->x - right_top->x, right_bottom->y - right_top->y);

                while(true)
                {
                    int left_bottom_y = left_bottom->y;
                    int right_bottom_y = right_bottom->y;
                    int bottom_y = left_bottom_y;

                    if(right_bottom_y < bottom_y)
                    {
                        bottom_y = right_bottom_y;
                    }

                    if(maximum_y < bottom_y)
                    {
                        bottom_y = maximum_y;
                    }

                    if(y < 0)
                    {
                        int invalid_bottom_y = bottom_y;

                        if(invalid_bottom_y > -1)
                        {
                            invalid_bottom_y = -1;
                        }

                        while(y <= invalid_bottom_y)
                        {
                            xl += left_delta;
                            xr += right_delta;
                            ++y;
                        }
                    }

                    while(y <= bottom_y)
                    {
                        hlines[y] = { xl.right_shift_integer(), xr.right_shift_integer() };
                        xl += left_delta;
                        xr += right_delta;
                        ++y;
                    }

                    if(y <= maximum_y)
                    {
                        if(bottom_y == left_bottom_y)
                        {
                            left_top = left_bottom;
                            left_bottom = left_bottom->next;

                            int delta_y = left_bottom->y - left_top->y;

                            if(delta_y <= 0) [[unlikely]]
                            {
                                left_top = left_bottom;
                                left_bottom = left_bottom->next;
                                delta_y = left_bottom->y - left_top->y;
                            }

                            left_delta = unsafe_unsigned_lut_division<fixed_precision>(
                                        left_bottom->x - left_top->x, delta_y);
                            xl = left_top->x + left_delta;
                        }

                        if(bottom_y == right_bottom_y)
                        {
                            right_top = right_bottom;
                            right_bottom = right_bottom->prev;

                            int delta_y = right_bottom->y - right_top->y;

                            if(delta_y <= 0) [[unlikely]]
                            {
                                right_top = right_bottom;
                                right_bottom = right_bottom->prev;
                                delta_y = right_bottom->y - right_top->y;
                            }

                            right_delta = unsafe_unsigned_lut_division<fixed_precision>(
                                        right_bottom->x - right_top->x, delta_y);
                            xr = right_top->x + right_delta;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                hlines[minimum_y] = { minimum_x, maximum_x };
            }

            int width = maximum_x - minimum_x + 1;
            _shape_groups.add_hlines(unsigned(minimum_y), unsigned(maximum_y), width, x_outside,
                                     face->color_index(), face->shading(), hlines);
        }
        else
        {
            int minimum_y = visible_face.top_index;
            int maximum_y = minimum_y + 64;

            if(minimum_y < 0)
            {
                minimum_y = 0;
            }
            else if(maximum_y > display_height - 1)
            {
                maximum_y = display_height - 1;
            }

            uint16_t attr0 = visible_face.minimum_x;
            uint16_t attr1 = visible_face.maximum_x;
            uint16_t attr2 = visible_face.minimum_y;
            _shape_groups.add_sprite(unsigned(minimum_y), unsigned(maximum_y), attr0, attr1, attr2);
        }
    }

    FR_PROFILER_STOP();
}

}
