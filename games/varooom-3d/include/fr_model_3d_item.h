/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_3D_ITEM_H
#define FR_MODEL_3D_ITEM_H

#include "bn_math.h"
#include "bn_sprite_tiles_item.h"
#include "bn_sprite_palette_item.h"

#include "fr_point_3d.h"

namespace fr
{

class face_3d
{

public:
    static constexpr int directional_shading = -1;
    static constexpr int max_colors = 10;

    constexpr face_3d(const bn::span<const vertex_3d>& vertices, const vertex_3d& normal, int first_vertex_index,
                      int second_vertex_index, int third_vertex_index, int color_index, int shading) :
        _centroid(_calculate_centroid(vertices, first_vertex_index, second_vertex_index, third_vertex_index)),
        _normal(normal),
        _first_vertex_index(first_vertex_index),
        _second_vertex_index(second_vertex_index),
        _third_vertex_index(third_vertex_index),
        _fourth_vertex_index(first_vertex_index),
        _color_index(color_index),
        _shading(_calculate_shading(shading, normal.point().y())),
        _triangle(true)
    {
        BN_ASSERT(color_index >= 0 && color_index < max_colors, "Invalid color index: ", color_index);
    }

    constexpr face_3d(const bn::span<const vertex_3d>& vertices, const vertex_3d& normal, int first_vertex_index,
                      int second_vertex_index, int third_vertex_index, int fourth_vertex_index, int color_index,
                      int shading) :
        _centroid(_calculate_centroid(vertices, first_vertex_index, second_vertex_index, third_vertex_index,
                                      fourth_vertex_index)),
        _normal(normal),
        _first_vertex_index(first_vertex_index),
        _second_vertex_index(second_vertex_index),
        _third_vertex_index(third_vertex_index),
        _fourth_vertex_index(fourth_vertex_index),
        _color_index(color_index),
        _shading(_calculate_shading(shading, normal.point().y())),
        _triangle(false)
    {
        BN_ASSERT(color_index >= 0 && color_index < max_colors, "Invalid color index: ", color_index);
    }

    [[nodiscard]] constexpr const vertex_3d& centroid() const
    {
        return _centroid;
    }

    [[nodiscard]] constexpr const vertex_3d& normal() const
    {
        return _normal;
    }

    [[nodiscard]] constexpr int first_vertex_index() const
    {
        return _first_vertex_index;
    }

    [[nodiscard]] constexpr int second_vertex_index() const
    {
        return _second_vertex_index;
    }

    [[nodiscard]] constexpr int third_vertex_index() const
    {
        return _third_vertex_index;
    }

    [[nodiscard]] constexpr int fourth_vertex_index() const
    {
        return _fourth_vertex_index;
    }

    [[nodiscard]] constexpr int color_index() const
    {
        return _color_index;
    }

    constexpr void set_color_index(int color_index)
    {
        BN_ASSERT(color_index >= 0 && color_index < max_colors, "Invalid color index: ", color_index);

        _color_index = color_index;
    }

    [[nodiscard]] constexpr int shading() const
    {
        return _shading;
    }

    constexpr void set_shading(int shading)
    {
        _shading = _calculate_shading(shading, _normal.point().y());
    }

    [[nodiscard]] constexpr bool triangle() const
    {
        return _triangle;
    }

private:
    vertex_3d _centroid;
    vertex_3d _normal;
    int16_t _first_vertex_index;
    int16_t _second_vertex_index;
    int16_t _third_vertex_index;
    int16_t _fourth_vertex_index;
    int8_t _color_index;
    int8_t _shading;
    bool _triangle;

    [[nodiscard]] constexpr static vertex_3d _calculate_centroid(
            const bn::span<const vertex_3d>& vertices, int first_vertex_index, int second_vertex_index,
            int third_vertex_index)
    {
        BN_ASSERT(vertices.size() > 0 && vertices.size() < 32768, "Invalid vertices count: ", vertices.size());
        BN_ASSERT(first_vertex_index >= 0 && first_vertex_index < vertices.size(),
                  "Invalid first vertex index: ", first_vertex_index, " - ", vertices.size());
        BN_ASSERT(second_vertex_index >= 0 && second_vertex_index < vertices.size(),
                  "Invalid second vertex index: ", second_vertex_index, " - ", vertices.size());
        BN_ASSERT(third_vertex_index >= 0 && third_vertex_index < vertices.size(),
                  "Invalid third vertex index: ", third_vertex_index, " - ", vertices.size());

        const point_3d& first_point = vertices.data()[first_vertex_index].point();
        const point_3d& second_point = vertices.data()[second_vertex_index].point();
        const point_3d& third_point = vertices.data()[third_vertex_index].point();
        BN_ASSERT(first_point != second_point, "Vertices are the same");
        BN_ASSERT(first_point != third_point, "Vertices are the same");
        BN_ASSERT(second_point != third_point, "Vertices are the same");

        return vertex_3d((first_point + second_point + third_point) / 3);
    }

    [[nodiscard]] constexpr static vertex_3d _calculate_centroid(
            const bn::span<const vertex_3d>& vertices, int first_vertex_index, int second_vertex_index,
            int third_vertex_index, int fourth_vertex_index)
    {
        BN_ASSERT(vertices.size() > 0 && vertices.size() < 32768, "Invalid vertices count: ", vertices.size());
        BN_ASSERT(first_vertex_index >= 0 && first_vertex_index < vertices.size(),
                  "Invalid first vertex index: ", first_vertex_index, " - ", vertices.size());
        BN_ASSERT(second_vertex_index >= 0 && second_vertex_index < vertices.size(),
                  "Invalid second vertex index: ", second_vertex_index, " - ", vertices.size());
        BN_ASSERT(third_vertex_index >= 0 && third_vertex_index < vertices.size(),
                  "Invalid third vertex index: ", third_vertex_index, " - ", vertices.size());
        BN_ASSERT(fourth_vertex_index >= 0 && fourth_vertex_index < vertices.size(),
                  "Invalid fourth vertex index: ", fourth_vertex_index, " - ", vertices.size());

        const point_3d& first_point = vertices.data()[first_vertex_index].point();
        const point_3d& second_point = vertices.data()[second_vertex_index].point();
        const point_3d& third_point = vertices.data()[third_vertex_index].point();
        const point_3d& fourth_point = vertices.data()[fourth_vertex_index].point();
        BN_ASSERT(first_point != second_point, "Vertices are the same");
        BN_ASSERT(first_point != third_point, "Vertices are the same");
        BN_ASSERT(second_point != third_point, "Vertices are the same");
        BN_ASSERT(fourth_point != first_point, "Vertices are the same");
        BN_ASSERT(fourth_point != second_point, "Vertices are the same");
        BN_ASSERT(fourth_point != third_point, "Vertices are the same");

        return vertex_3d((first_point + second_point + third_point + fourth_point) / 4);
    }

    [[nodiscard]] constexpr static int _calculate_shading(int input_shading, bn::fixed normal_y)
    {
        if(input_shading == directional_shading)
        {
            bn::fixed light_vector_dot_normal = bn::abs(normal_y);
            int result = light_vector_dot_normal.data() >> (12 - 3);
            return bn::min(result, 7);
        }

        BN_ASSERT(input_shading >= 0 && input_shading <= 7, "Invalid shading: ", input_shading);

        return input_shading;
    }
};


class model_3d_vertical_cylinder
{

public:
    constexpr model_3d_vertical_cylinder() = default;

    constexpr model_3d_vertical_cylinder(bn::fixed centroid_x, bn::fixed centroid_z, int integer_radius) :
        _centroid_x(centroid_x),
        _centroid_z(centroid_z),
        _integer_radius(integer_radius)
    {
        BN_ASSERT(integer_radius >= 0, "Invalid integer radius: ", integer_radius);
    }

    [[nodiscard]] constexpr bn::fixed centroid_x() const
    {
        return _centroid_x;
    }

    [[nodiscard]] constexpr bn::fixed centroid_z() const
    {
        return _centroid_z;
    }

    [[nodiscard]] constexpr int integer_radius() const
    {
        return _integer_radius;
    }

private:
    bn::fixed _centroid_x;
    bn::fixed _centroid_z;
    int _integer_radius = 0;
};


class model_3d_item
{

public:
    constexpr model_3d_item(const bn::span<const vertex_3d>& vertices, const bn::span<const face_3d>& faces) :
        model_3d_item(vertices, faces, nullptr, nullptr)
    {
    }

    constexpr model_3d_item(const bn::span<const vertex_3d>& vertices, const bn::span<const face_3d>& faces,
                            const face_3d* collision_face) :
        model_3d_item(vertices, faces, collision_face, nullptr)
    {
    }

    constexpr model_3d_item(const bn::span<const vertex_3d>& vertices, const bn::span<const face_3d>& faces,
                            const face_3d* collision_face, const model_3d_vertical_cylinder* vertical_cylinder) :
        _vertices(vertices),
        _faces(faces),
        _collision_face(collision_face),
        _vertical_cylinder(vertical_cylinder)
    {
        BN_ASSERT(vertices.size() > 0 && vertices.size() < 32768, "Invalid vertices count: ", vertices.size());
        BN_ASSERT(! faces.empty(), "There's no faces");
    }

    [[nodiscard]] constexpr const bn::span<const vertex_3d>& vertices() const
    {
        return _vertices;
    }

    [[nodiscard]] constexpr const bn::span<const face_3d>& faces() const
    {
        return _faces;
    }

    [[nodiscard]] constexpr const face_3d* collision_face() const
    {
        return _collision_face;
    }

    [[nodiscard]] constexpr const model_3d_vertical_cylinder* vertical_cylinder() const
    {
        return _vertical_cylinder;
    }

private:
    bn::span<const vertex_3d> _vertices;
    bn::span<const face_3d> _faces;
    const face_3d* _collision_face;
    const model_3d_vertical_cylinder* _vertical_cylinder;
};


class face_texture
{

public:
    constexpr face_texture(
            const bn::sprite_palette_item& palette_item, const bn::sprite_tiles_item& small_tiles_item,
            const bn::sprite_tiles_item& normal_tiles_item, const bn::sprite_tiles_item& big_tiles_item,
            const bn::sprite_tiles_item& huge_tiles_item) :
        _palette_item(palette_item),
        _small_tiles_item(small_tiles_item),
        _normal_tiles_item(normal_tiles_item),
        _big_tiles_item(big_tiles_item),
        _huge_tiles_item(huge_tiles_item)
    {
    }

    [[nodiscard]] constexpr const bn::sprite_palette_item& palette_item() const
    {
        return _palette_item;
    }

    [[nodiscard]] constexpr const bn::sprite_tiles_item& small_tiles_item() const
    {
        return _small_tiles_item;
    }

    [[nodiscard]] constexpr const bn::sprite_tiles_item& normal_tiles_item() const
    {
        return _normal_tiles_item;
    }

    [[nodiscard]] constexpr const bn::sprite_tiles_item& big_tiles_item() const
    {
        return _big_tiles_item;
    }

    [[nodiscard]] constexpr const bn::sprite_tiles_item& huge_tiles_item() const
    {
        return _huge_tiles_item;
    }

private:
    const bn::sprite_palette_item& _palette_item;
    const bn::sprite_tiles_item& _small_tiles_item;
    const bn::sprite_tiles_item& _normal_tiles_item;
    const bn::sprite_tiles_item& _big_tiles_item;
    const bn::sprite_tiles_item& _huge_tiles_item;
};

}

#endif
