#ifndef BF_GAME_ENEMIES_GRID_H
#define BF_GAME_ENEMIES_GRID_H

#include "btn_vector.h"
#include "bf_constants.h"
#include "btn_fixed_point.h"

namespace bf::game
{

class enemies_grid
{

public:
    void add_enemy(int enemy_index, const btn::fixed_point& position);

    void remove_enemy(int enemy_index, const btn::fixed_point& position);

    void update_enemy(int enemy_index, const btn::fixed_point& old_position, const btn::fixed_point& new_position);

private:
    static constexpr int cols = ((constants::view_width * 2) / constants::grid_size) + 3;
    static constexpr int rows = ((constants::view_height * 2) / constants::grid_size) + 3;

    class cell
    {

    public:
        void add_enemy(int enemy_index);

        void remove_enemy(int enemy_index);

    private:
        btn::vector<int8_t, constants::max_enemies_per_grid_cell> _indexes;
    };

    cell _cells[cols * rows];

    [[nodiscard]] static int _column(const btn::fixed_point& position)
    {
        return (position.x().integer() / constants::grid_size) + (cols / 2);
    }

    [[nodiscard]] static int _row(const btn::fixed_point& position)
    {
        return (position.y().integer() / constants::grid_size) + (rows / 2);
    }

    [[nodiscard]] cell* _cells_row(int row)
    {
        return &(_cells[0]) + (cols * row);
    }

    void _add_enemy_row(int enemy_index, int row, int column);

    void _remove_enemy_row(int enemy_index, int row, int column);

    void _add_enemy_column(int enemy_index, int row, int column);

    void _remove_enemy_column(int enemy_index, int row, int column);
};

}

#endif
