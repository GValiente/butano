/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TRANSFORMED_MODEL_3D_ITEM_H
#define FR_TRANSFORMED_MODEL_3D_ITEM_H

#include "fr_model_3d.h"
#include "fr_constants_3d.h"

namespace fr
{

template<const model_3d_item& model_3d_item_ref>
class transformed_model_3d_item
{

public:
    constexpr transformed_model_3d_item(bn::fixed x, bn::fixed z, bn::fixed theta) :
        _vertices(_create_array<vertex_3d, vertices_count>(model_3d_item_ref.vertices()[0])),
        _faces(_create_array<face_3d, faces_count>(model_3d_item_ref.faces()[0]))
    {
        const bn::span<const vertex_3d>& input_vertices = model_3d_item_ref.vertices();
        bn::fixed minimum_y = input_vertices[0].point().y();
        int max_cylinder_squared_radius = 0;

        for(int index = 0; index < vertices_count; ++index)
        {
            const point_3d& input_point = input_vertices[index].point();
            int abs_x = bn::abs(input_point.x()).ceil_integer();
            int abs_z = bn::abs(input_point.z()).ceil_integer();
            int cylinder_squared_radius = (abs_x * abs_x) + (abs_z * abs_z);
            max_cylinder_squared_radius = bn::max(max_cylinder_squared_radius, cylinder_squared_radius);
            minimum_y = bn::min(minimum_y, input_point.y());
        }

        model_3d model(model_3d_item_ref);
        model.set_position(point_3d(x, -minimum_y, z));
        model.set_theta(theta);
        model.update();

        int cylinder_radius = bn::sqrt(max_cylinder_squared_radius);
        _vertical_cylinder = model_3d_vertical_cylinder(x, z, cylinder_radius);

        for(int index = 0; index < vertices_count; ++index)
        {
            vertex_3d transformed_vertex(model.transform(input_vertices[index]));
            _vertices[index] = transformed_vertex;
        }

        const bn::span<const face_3d>& input_faces = model_3d_item_ref.faces();

        for(int index = 0; index < faces_count; ++index)
        {
            const face_3d& input_face = input_faces[index];
            vertex_3d rotated_normal(model.rotate(input_face.normal()));

            if(input_face.triangle())
            {
                _faces[index] = face_3d(_vertices, rotated_normal, input_face.first_vertex_index(),
                                        input_face.second_vertex_index(), input_face.third_vertex_index(),
                                        input_face.color_index(), input_face.shading());
            }
            else
            {
                _faces[index] = face_3d(_vertices, rotated_normal, input_face.first_vertex_index(),
                                        input_face.second_vertex_index(), input_face.third_vertex_index(),
                                        input_face.fourth_vertex_index(), input_face.color_index(), input_face.shading());
            }
        }
    }

    [[nodiscard]] constexpr model_3d_item item() const
    {
        return model_3d_item(_vertices, _faces, model_3d_item_ref.collision_face(), &_vertical_cylinder);
    }

private:
    static constexpr int vertices_count = model_3d_item_ref.vertices().size();
    static constexpr int faces_count = model_3d_item_ref.faces().size();

    bn::array<vertex_3d, vertices_count> _vertices;
    bn::array<face_3d, faces_count> _faces;
    model_3d_vertical_cylinder _vertical_cylinder;

    template<typename Type, unsigned Size>
    [[nodiscard]] static constexpr bn::array<Type, Size> _create_array(const Type& value)
    {
        return _create_array_impl(value, bn::make_index_sequence<Size>());
    }

    template<typename Type, unsigned... IndexSequence>
    [[nodiscard]] static constexpr bn::array<Type, sizeof...(IndexSequence)> _create_array_impl(
            Type value, bn::index_sequence<IndexSequence...>)
    {
        return {{ (static_cast<void>(IndexSequence), value)... }};
    }
};

}

#endif
