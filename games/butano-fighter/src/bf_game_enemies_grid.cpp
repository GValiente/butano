#include "bf_game_enemies_grid.h"

namespace bf::game
{

void enemies_grid::add_enemy(int enemy_index, const btn::fixed_point& position)
{
    int row = _row(position);
    int column = _column(position);
    _add_enemy_row(enemy_index, row - 1, column);
    _add_enemy_row(enemy_index, row, column);
    _add_enemy_row(enemy_index, row + 1, column);
}

void enemies_grid::remove_enemy(int enemy_index, const btn::fixed_point& position)
{
    int row = _row(position);
    int column = _column(position);
    _remove_enemy_row(enemy_index, row - 1, column);
    _remove_enemy_row(enemy_index, row, column);
    _remove_enemy_row(enemy_index, row + 1, column);
}

void enemies_grid::update_enemy(int enemy_index, const btn::fixed_point& old_position,
                                const btn::fixed_point& new_position)
{
    int old_row = _row(old_position);
    int old_column = _column(old_position);
    int new_row = _row(new_position);
    int new_column = _column(new_position);

    if(old_row < new_row)
    {
        _remove_enemy_row(enemy_index, old_row - 1, old_column);
        _add_enemy_row(enemy_index, new_row + 1, new_column);
    }
    else if(old_row > new_row)
    {
        _remove_enemy_row(enemy_index, old_row + 1, old_column);
        _add_enemy_row(enemy_index, new_row - 1, new_column);
    }

    if(old_column < new_column)
    {
        _remove_enemy_row(enemy_index, old_row, old_column - 1);
        _add_enemy_column(enemy_index, new_row, new_column + 1);
    }
    else if(old_column > new_column)
    {
        _remove_enemy_row(enemy_index, old_row, old_column + 1);
        _add_enemy_column(enemy_index, new_row, new_column - 1);
    }
}

void enemies_grid::_add_enemy_row(int enemy_index, int row, int column)
{
    cell* cells_row = _cells_row(row);
    cells_row[column - 1].add_enemy(enemy_index);
    cells_row[column].add_enemy(enemy_index);
    cells_row[column + 1].add_enemy(enemy_index);
}

void enemies_grid::_remove_enemy_row(int enemy_index, int row, int column)
{
    cell* cells_row = _cells_row(row);
    cells_row[column - 1].remove_enemy(enemy_index);
    cells_row[column].remove_enemy(enemy_index);
    cells_row[column + 1].remove_enemy(enemy_index);
}

void enemies_grid::_add_enemy_column(int enemy_index, int row, int column)
{
    _cells_row(row - 1)[column].add_enemy(enemy_index);
    _cells_row(row)[column].add_enemy(enemy_index);
    _cells_row(row + 1)[column].add_enemy(enemy_index);
}

void enemies_grid::_remove_enemy_column(int enemy_index, int row, int column)
{
    _cells_row(row - 1)[column].remove_enemy(enemy_index);
    _cells_row(row)[column].remove_enemy(enemy_index);
    _cells_row(row + 1)[column].remove_enemy(enemy_index);
}

void enemies_grid::cell::add_enemy(int enemy_index)
{
    auto end = _indexes.end();

    if(btn::find(_indexes.begin(), end, enemy_index) == end)
    {
        BTN_ASSERT(! _indexes.full(), "No more available enemy indexes");

        _indexes.push_back(int8_t(enemy_index));
    }
}

void enemies_grid::cell::remove_enemy(int enemy_index)
{
    auto end = _indexes.end();
    auto it = btn::find(_indexes.begin(), end, enemy_index);

    if(it != end)
    {
        _indexes.erase(it);
    }
}

}
