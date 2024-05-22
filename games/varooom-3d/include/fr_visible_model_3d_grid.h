/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_VISIBLE_MODEL_3D_GRID_H
#define FR_VISIBLE_MODEL_3D_GRID_H

#include "fr_constants_3d.h"
#include "fr_model_3d_grid.h"

namespace fr
{

class visible_model_3d_grid
{

public:
    class cell
    {

    public:
        uint16_t model_indexes[constants_3d::max_static_models];
        uint16_t models_count = 0;
    };

    constexpr explicit visible_model_3d_grid(const model_3d_grid::cell* grid_cells)
    {
        for(int r = 0; r < model_3d_grid::rows; ++r)
        {
            cell* cells_row = _cells + (r * model_3d_grid::columns);

            for(int c = 0; c < model_3d_grid::columns; ++c)
            {
                cells_row[c] = _create_cell(grid_cells, r, c);
            }
        }
    }

    [[nodiscard]] constexpr const cell* cells() const
    {
        return _cells;
    }

private:
    static constexpr int _view_cells = 160 / model_3d_grid::cell_size;

    cell _cells[model_3d_grid::columns * model_3d_grid::rows];

    [[nodiscard]] static constexpr cell _create_cell(const model_3d_grid::cell* grid_cells, int row, int column)
    {
        uint16_t _model_usages[constants_3d::max_stage_models] = {};
        cell result;

        int ri = bn::max(row - _view_cells, 0);
        int rl = bn::min(row + _view_cells, model_3d_grid::rows - 1);
        int ci = bn::max(column - _view_cells, 0);
        int cl = bn::min(column + _view_cells, model_3d_grid::columns - 1);

        for(int r = ri; r <= rl; ++r)
        {
            const model_3d_grid::cell* grid_cells_row = grid_cells + (r * model_3d_grid::columns);

            for(int c = ci; c <= cl; ++c)
            {
                const model_3d_grid::cell& grid_cell = grid_cells_row[c];

                for(int index = 0, limit = grid_cell.models_count; index < limit; ++index)
                {
                    uint16_t model_index = grid_cell.model_indexes[index];

                    if(uint16_t usages = _model_usages[model_index])
                    {
                        _model_usages[model_index] = usages + 1;
                    }
                    else
                    {
                        BN_ASSERT(result.models_count < constants_3d::max_static_models, "Too many static models");

                        result.model_indexes[result.models_count] = model_index;
                        ++result.models_count;
                        _model_usages[model_index] = 1;
                    }
                }
            }
        }

        return result;
    }
};

}

#endif
