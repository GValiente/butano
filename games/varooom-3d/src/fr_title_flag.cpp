/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_title_flag.h"

#include "fr_models_3d.h"

#include "models/fr_model_3d_items_title_flag.h"

namespace fr
{

namespace
{
    constexpr int wave_angle_inc = 512;
    constexpr int wave_angle_desp = 8192;

    constexpr bn::color model_colors[] = {
        bn::color(14, 16, 19)
    };
}

title_flag::title_flag(models_3d& models) :
    _models(models),
    _vertices(bn::to_array(fr::model_3d_items::title_flag_vertices)),
    _faces(bn::to_array(fr::model_3d_items::title_flag_faces)),
    _model_item(_vertices, _faces)
{
}

title_flag::~title_flag()
{
    if(_model)
    {
        _models.destroy_dynamic_model(*_model);
        _model = nullptr;
    }
}

void title_flag::show()
{
    if(! _model)
    {
        _models.load_colors(model_colors);
        _model = &_models.create_dynamic_model(_model_item);
        _model->set_position(point_3d(44, 112 + 44, 0));
        _model->set_phi(8192 + 4096);
        _model->set_theta(16384);
    }
}

void title_flag::update()
{
    if(_model)
    {
        point_3d position = _model->position();

        if(position.x() > 0)
        {
            position.set_x(position.x() - 1);
            position.set_y(position.y() - 1);
            _model->set_position(position);
        }
        else
        {
            _moving = false;
        }

        int wave_angle = _wave_angle + wave_angle_inc;

        if(wave_angle >= 65536)
        {
            wave_angle -= 65536;
        }

        vertex_3d* vertices = _vertices.data();
        face_3d* faces = _faces.data();
        int vertex_index = 4;
        int face_index = 0;
        _wave_angle = wave_angle;

        for(int column = 0; column < 10; ++column)
        {
            wave_angle += wave_angle_desp;

            if(wave_angle >= 65536)
            {
                wave_angle -= 65536;
            }

            bn::fixed wave_sin = sin(wave_angle);
            bn::fixed vertex_y = wave_sin * 4;

            for(int row = 0; row < 6; ++row)
            {
                vertex_3d& vertex = vertices[vertex_index];
                const point_3d& point = vertex.point();
                vertex = vertex_3d(point.x(), vertex_y, point.z());
                ++vertex_index;
            }

            int face_shading = ((wave_sin + 1) * 4).right_shift_integer();

            if(face_shading > 7)
            {
                face_shading = 7;
            }

            for(int row = 0, row_limit = column % 2 == 0 ? 3 : 2; row < row_limit; ++row)
            {
                face_3d& face = faces[face_index];
                face.set_shading(face_shading);
                ++face_index;
            }
        }
    }
}

}
