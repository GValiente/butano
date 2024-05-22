/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_foreground_3d.h"

#include "bn_point.h"

#include "fr_stage.h"
#include "fr_models_3d.h"
#include "fr_camera_3d.h"
#include "fr_announcer.h"
#include "fr_player_car.h"

namespace fr
{

namespace
{
    [[nodiscard]] int _column(const point_3d& camera_position)
    {
        int column = camera_position.x().right_shift_integer() / model_3d_grid::cell_size;
        BN_ASSERT(column >= 0 && column < model_3d_grid::columns,
                  "Invalid column: ", column, " - ", camera_position.x());

        return column;
    }

    [[nodiscard]] int _row(const point_3d& camera_position)
    {
        int row = camera_position.z().right_shift_integer() / model_3d_grid::cell_size;
        BN_ASSERT(row >= 0 && row < model_3d_grid::rows, "Invalid row: ", row, " - ", camera_position.z());

        return row;
    }

    [[nodiscard]] int _is_left(const bn::point& p0, const bn::point& p1, const bn::point& p2)
    {
        return (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() -  p0.x()) * (p1.y() - p0.y());
    }

    [[nodiscard]] int _winding_number(const bn::point& v0, const bn::point& v1, const bn::point& point)
    {
        if(v0.y() <= point.y())
        {
            if(v1.y() > point.y())
            {
                if(_is_left(v0, v1, point) > 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            if(v1.y() <= point.y())
            {
                if(_is_left(v0, v1, point) < 0)
                {
                    return -1;
                }
            }
        }

        return 0;
    }
}

foreground_3d::foreground_3d(const stage& stage, const camera_3d& camera, models_3d& models)
{
    _reload_static_model_items(stage, camera, models);
}

void foreground_3d::check_collision(const stage& stage, player_car& player_car, announcer& announcer)
{
    if(! player_car.can_crash())
    {
        return;
    }

    const model_3d_item* model_items = stage.model_items().data();
    const model_3d_grid::cell* grid_cells = stage.model_grid().cells();

    for(const point_3d& collision_point : player_car.collision_points())
    {
        bn::point collision_point_2d(collision_point.x().right_shift_integer(),
                                     collision_point.z().right_shift_integer());
        int row = _row(collision_point);
        int column = _column(collision_point);
        const model_3d_grid::cell* grid_cells_row = grid_cells + (row * model_3d_grid::columns);
        const model_3d_grid::cell& grid_cell = grid_cells_row[column];

        for(int index = 0, limit = grid_cell.models_count; index < limit; ++index)
        {
            int model_index = grid_cell.model_indexes[index];
            const model_3d_item& model_item = model_items[model_index];

            if(const face_3d* collision_face = model_item.collision_face())
            {
                const vertex_3d* vertices = model_item.vertices().data();
                const point_3d& v0 = vertices[collision_face->first_vertex_index()].point();
                const point_3d& v1 = vertices[collision_face->second_vertex_index()].point();
                const point_3d& v2 = vertices[collision_face->third_vertex_index()].point();
                bn::point v0_point(v0.x().right_shift_integer(), v0.z().right_shift_integer());
                bn::point v1_point(v1.x().right_shift_integer(), v1.z().right_shift_integer());
                bn::point v2_point(v2.x().right_shift_integer(), v2.z().right_shift_integer());
                int winding_number = _winding_number(v0_point, v1_point, collision_point_2d);
                winding_number += _winding_number(v1_point, v2_point, collision_point_2d);

                if(collision_face->triangle())
                {
                    winding_number += _winding_number(v2_point, v0_point, collision_point_2d);
                }
                else
                {
                    const point_3d& v3 = vertices[collision_face->fourth_vertex_index()].point();
                    bn::point v3_point(v3.x().right_shift_integer(), v3.z().right_shift_integer());
                    winding_number += _winding_number(v2_point, v3_point, collision_point_2d);
                    winding_number += _winding_number(v3_point, v0_point, collision_point_2d);
                }

                if(winding_number)
                {
                    player_car.crash();
                    announcer.play_wall_crash_voice(45);
                    return;
                }
            }
        }
    }
}

void foreground_3d::update(const stage& stage, const camera_3d& camera, models_3d& models)
{
    _reload_static_model_items(stage, camera, models);
}

void foreground_3d::_reload_static_model_items(const stage& stage, const camera_3d& camera, models_3d& models)
{
    BN_ASSERT(camera.position().y() <= 256, "Invalid camera y: ", camera.position().y());

    const point_3d& camera_position = camera.position();
    int row = _row(camera_position);
    int column = _column(camera_position);

    const model_3d_item* model_items = stage.model_items().data();
    const visible_model_3d_grid::cell* visible_cells = stage.visible_model_grid().cells();
    const visible_model_3d_grid::cell& visible_cell = visible_cells[(row * model_3d_grid::rows) + column];
    int static_model_items_count = _reload_static_model_items_impl(camera, model_items, visible_cell);
    models.set_static_model_items(_static_model_items, static_model_items_count);
}

}
