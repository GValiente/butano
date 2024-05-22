/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_enemies_grid.h"

#include "bf_game_enemy.h"
#include "bf_game_check_hero_bullet_data.h"

#if BF_CFG_ENEMIES_GRID_LOG_ENABLED
    #include "bn_log.h"
    #include "bn_string.h"

    static_assert(BN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace bf::game
{

void enemies_grid::add_enemy(enemy& enemy)
{
    bn::fixed_point enemy_top_left = enemy.top_left();
    int row = _row(enemy_top_left);
    int column = _column(enemy_top_left);
    int enemy_rows = enemy.grid_rows();
    enemy.set_last_grid_row(row);
    enemy.set_last_grid_column(column);

    for(int r = row - 1; r <= row + enemy_rows; ++r)
    {
        _add_enemy_row(r, column, enemy);
    }
}

void enemies_grid::remove_enemy(enemy& enemy)
{
    bn::fixed_point enemy_top_left = enemy.top_left();
    int row = _row(enemy_top_left);
    int column = _column(enemy_top_left);
    int enemy_rows = enemy.grid_rows();

    for(int r = row - 1; r <= row + enemy_rows; ++r)
    {
        _remove_enemy_row(r, column, enemy);
    }
}

bool enemies_grid::update_enemy(enemy& enemy)
{
    bn::fixed_point enemy_top_left = enemy.top_left();
    int old_row = enemy.last_grid_row();
    int old_column = enemy.last_grid_column();
    int new_row = _row(enemy_top_left);
    int new_column = _column(enemy_top_left);
    bool updated = false;

    if(old_row < new_row)
    {
        BN_ASSERT(old_row == new_row - 1, "Enemy is too fast: ", old_row, " - ", new_row);

        enemy.set_last_grid_row(new_row);
        _remove_enemy_row(old_row - 1, old_column, enemy);
        _add_enemy_row(new_row + enemy.grid_rows(), old_column, enemy);
        updated = true;
    }
    else if(old_row > new_row)
    {
        BN_ASSERT(old_row == new_row + 1, "Enemy is too fast: ", old_row, " - ", new_row);

        enemy.set_last_grid_row(new_row);
        _remove_enemy_row(old_row + enemy.grid_rows(), old_column, enemy);
        _add_enemy_row(new_row - 1, old_column, enemy);
        updated = true;
    }

    if(old_column < new_column)
    {
        BN_ASSERT(old_column == new_column - 1, "Enemy is too fast: ", old_column, " - ", new_column);

        enemy.set_last_grid_column(new_column);
        _remove_enemy_column(new_row, old_column - 1, enemy);
        _add_enemy_column(new_row, new_column + enemy.grid_columns(), enemy);
        updated = true;
    }
    else if(old_column > new_column)
    {
        BN_ASSERT(old_column == new_column + 1, "Enemy is too fast: ", old_column, " - ", new_column);

        enemy.set_last_grid_column(new_column);
        _remove_enemy_column(new_row, old_column + enemy.grid_columns(), enemy);
        _add_enemy_column(new_row, new_column - 1, enemy);
        updated = true;
    }

    return updated;
}

bool enemies_grid::check_hero(const bn::fixed_rect& hero_rect) const
{
    bn::fixed_point hero_top_left = hero_rect.top_left();
    int row = _safe_row(hero_top_left);
    int column = _safe_column(hero_top_left);

    for(const enemies_list_node_type& enemies_node : _cells_row(row)[column].enemies())
    {
        if(enemies_node.enemy_ptr->check_hero(hero_rect))
        {
            return true;
        }
    }

    return false;
}

bool enemies_grid::check_hero_bullet(const check_hero_bullet_data& data)
{
    bn::fixed_point bullet_top_left = data.bullet_rect.top_left();
    int row = _safe_row(bullet_top_left);
    int column = _safe_column(bullet_top_left);

    for(const enemies_list_node_type& enemies_node : _cells_row(row)[column].enemies())
    {
        if(enemies_node.enemy_ptr->check_hero_bullet(data))
        {
            return true;
        }
    }

    return false;
}

#if BF_CFG_ENEMIES_GRID_LOG_ENABLED
    void enemies_grid::log() const
    {
        BN_LOG("grid: [");

        for(int r = 0; r < rows; ++r)
        {
            const cell* cells_row = _cells_row(r);
            bn::string<BN_CFG_LOG_MAX_SIZE> string;
            bn::ostringstream stream(string);
            stream.append("\t[");

            for(int c = 0; c < columns; ++c)
            {
                const cell& cell = cells_row[c];
                const enemies_list& enemies = cell.enemies();

                if(enemies.empty())
                {
                    stream.append('_');
                }
                else
                {
                    for(const enemies_list_node_type& enemies_node : enemies)
                    {
                        stream.append(enemies_node.enemy_ptr->tag());
                        stream.append(',');
                    }
                }

                stream.append('\t');
            }

            stream.append(']');
            BN_LOG(stream.string());
        }

        BN_LOG("]");
    }
#endif

int enemies_grid::_column(const bn::fixed_point& top_left)
{
    int column = (top_left.x().right_shift_integer() / constants::enemies_grid_size) + (columns / 2);
    BN_ASSERT(column >= 1 && column < columns - max_cell_increment,
               "Invalid column: ", column, " - ", top_left.x().right_shift_integer());

    return column;
}

int enemies_grid::_row(const bn::fixed_point& top_left)
{
    int row = (top_left.y().right_shift_integer() / constants::enemies_grid_size) + (rows / 2);
    BN_ASSERT(row >= 1 && row < rows - max_cell_increment,
               "Invalid row: ", row, " - ", top_left.y().right_shift_integer());

    return row;
}

int enemies_grid::_safe_column(const bn::fixed_point& top_left)
{
    int column = (top_left.x().right_shift_integer() / constants::enemies_grid_size) + (columns / 2);
    return bn::clamp(column, 0, columns - 1);
}

int enemies_grid::_safe_row(const bn::fixed_point& top_left)
{
    int row = (top_left.y().right_shift_integer() / constants::enemies_grid_size) + (rows / 2);
    return bn::clamp(row, 0, rows - 1);
}

void enemies_grid::_add_enemy_row(int row, int column, enemy& enemy)
{
    cell_type* cells_row = _cells_row(row);
    int enemy_columns = enemy.grid_columns();

    for(int c = column - 1; c <= column + enemy_columns; ++c)
    {
        cells_row[c].add_enemy(enemy, _pool);
    }
}

void enemies_grid::_remove_enemy_row(int row, int column, enemy& enemy)
{
    cell_type* cells_row = _cells_row(row);
    int enemy_columns = enemy.grid_columns();

    for(int c = column - 1; c <= column + enemy_columns; ++c)
    {
        cells_row[c].remove_enemy(enemy, _pool);
    }
}

void enemies_grid::_add_enemy_column(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - 1; r <= row + enemy_rows; ++r)
    {
        _cells_row(r)[column].add_enemy(enemy, _pool);
    }
}

void enemies_grid::_remove_enemy_column(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - 1; r <= row + enemy_rows; ++r)
    {
        _cells_row(r)[column].remove_enemy(enemy, _pool);
    }
}

void enemies_grid::cell_type::add_enemy(enemy& enemy, enemies_pool& enemies_pool)
{
    for(const enemies_list_node_type& enemies_node : _enemies)
    {
        if(enemies_node.enemy_ptr == &enemy)
        {
            return;
        }
    }

    BN_ASSERT(! enemies_pool.full(), "No more available enemies in grid");

    enemies_list_node_type& enemies_node = enemies_pool.create(enemy);
    _enemies.push_front(enemies_node);
}

void enemies_grid::cell_type::remove_enemy(enemy& enemy, enemies_pool& enemies_pool)
{
    auto before_it = _enemies.before_begin();
    auto it = _enemies.begin();
    auto end = _enemies.end();

    while(it != end)
    {
        enemies_list_node_type& enemies_node = *it;

        if(enemies_node.enemy_ptr == &enemy)
        {
            _enemies.erase_after(before_it);
            enemies_pool.destroy(enemies_node);
            return;
        }

        before_it = it;
        ++it;
    }
}

void enemies_grid::cell_type::clear(enemies_pool& enemies_pool)
{
    auto before_it = _enemies.before_begin();
    auto it = _enemies.begin();
    auto end = _enemies.end();

    while(it != end)
    {
        enemies_list_node_type& enemies_node = *it;
        it = _enemies.erase_after(before_it);
        enemies_pool.destroy(enemies_node);
    }
}

}
