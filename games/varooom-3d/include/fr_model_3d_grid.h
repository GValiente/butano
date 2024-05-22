/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_3D_GRID_H
#define FR_MODEL_3D_GRID_H

#include "fr_model_3d_item.h"

namespace fr
{

class model_3d_grid
{

public:
    static constexpr int cell_size = 32;
    static constexpr int max_models_per_cell = 3;

    static constexpr int columns = 1024 / cell_size;
    static constexpr int rows = columns;

    class cell
    {

    public:
        uint16_t models_count = 0;
        uint16_t model_indexes[max_models_per_cell] = {};
    };

    constexpr explicit model_3d_grid(const bn::span<const model_3d_item>& model_items)
    {
        for(int model_index = 0, model_limit = model_items.size(); model_index < model_limit; ++model_index)
        {
            const model_3d_item& model_item = model_items[model_index];
            const bn::span<const vertex_3d>& vertices = model_item.vertices();
            int min_x = vertices[0].point().x().right_shift_integer();
            int min_y = vertices[0].point().z().right_shift_integer();
            int max_x = min_x;
            int max_y = min_y;

            for(int vertex_index = 1, vertex_limit = vertices.size(); vertex_index < vertex_limit; ++vertex_index)
            {
                const point_3d& point = vertices[vertex_index].point();
                int x = point.x().right_shift_integer();
                int y = point.z().right_shift_integer();

                if(x < min_x)
                {
                    min_x = x;
                }
                else if(x > max_x)
                {
                    max_x = x;
                }

                if(y < min_y)
                {
                    min_y = y;
                }
                else if(y > max_y)
                {
                    max_y = y;
                }
            }

            int min_cell_x = min_x / cell_size;
            BN_ASSERT(min_cell_x >= 0);

            int min_cell_y = min_y / cell_size;
            BN_ASSERT(min_cell_y >= 0);

            int max_cell_x = max_x / cell_size;
            BN_ASSERT(max_cell_x < columns);

            int max_cell_y = max_y / cell_size;
            BN_ASSERT(max_cell_y < rows);

            for(int cell_y = min_cell_y; cell_y <= max_cell_y; ++cell_y)
            {
                cell* cell_row = _cells + (cell_y * columns);

                for(int cell_x = min_cell_x; cell_x <= max_cell_x; ++cell_x)
                {
                    cell& current_cell = cell_row[cell_x];
                    BN_ASSERT(current_cell.models_count < max_models_per_cell);

                    current_cell.model_indexes[current_cell.models_count] = model_index;
                    ++current_cell.models_count;
                }
            }
        }
    }

    [[nodiscard]] constexpr const cell* cells() const
    {
        return _cells;
    }

private:
    cell _cells[columns * rows];
};

}

#endif
