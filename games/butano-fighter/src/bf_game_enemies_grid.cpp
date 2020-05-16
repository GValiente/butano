#include "bf_game_enemies_grid.h"

#include "bf_game_enemy.h"
#include "bf_game_check_hero_bullet_data.h"

#if BF_CFG_ENEMIES_GRID_LOG_ENABLED
    #include "btn_log.h"
    #include "btn_string.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace bf::game
{

enemies_grid::~enemies_grid()
{
    for(cell& cell : _cells)
    {
        cell.clear(_pool);
    }
}

void enemies_grid::add_enemy(enemy& enemy)
{
    btn::fixed_point position = enemy.position();
    int row = _row(position);
    int column = _column(position);
    _add_enemy(row, column, enemy);
}

void enemies_grid::remove_enemy(enemy& enemy)
{
    btn::fixed_point position = enemy.position();
    int row = _row(position);
    int column = _column(position);
    _remove_enemy(row, column, enemy);
}

bool enemies_grid::update_enemy(enemy& enemy)
{
    btn::fixed_point position = enemy.position();
    int old_row = enemy.last_grid_row();
    int old_column = enemy.last_grid_column();
    int new_row = _row(position);
    int new_column = _column(position);
    bool updated = old_row != new_row || old_column != new_column;

    if(updated)
    {
        _remove_enemy(old_row, old_column, enemy);
        _add_enemy(new_row, new_column, enemy);
    }

    return updated;
}

bool enemies_grid::check_hero(const btn::fixed_rect& hero_rect) const
{
    const btn::fixed_point& hero_position = hero_rect.position();
    int row = _safe_row(hero_position);
    int column = _safe_column(hero_position);

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
    const btn::fixed_point& bullet_position = data.bullet_rect.position();
    int row = _safe_row(bullet_position);
    int column = _safe_column(bullet_position);

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
        BTN_LOG("grid: [");

        for(int r = 0; r < rows; ++r)
        {
            const cell* cells_row = _cells_row(r);
            btn::string<BTN_CFG_LOG_MAX_SIZE> string;
            btn::input_string_stream stream(string);
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
            BTN_LOG(stream.string());
        }

        BTN_LOG("]");
    }
#endif

int enemies_grid::_column(const btn::fixed_point& position)
{
    int column = (position.x().integer() / constants::enemies_grid_size) + (columns / 2);
    BTN_ASSERT(column >= cell_increment && column < columns - cell_increment,
               "Invalid column: ", column, " - ", position.x().integer());

    return column;
}

int enemies_grid::_row(const btn::fixed_point& position)
{
    int row = (position.y().integer() / constants::enemies_grid_size) + (rows / 2);
    BTN_ASSERT(row >= cell_increment && row < rows - cell_increment,
               "Invalid row: ", row, " - ", position.y().integer());

    return row;
}

int enemies_grid::_safe_column(const btn::fixed_point& position)
{
    int column = (position.x().integer() / constants::enemies_grid_size) + (columns / 2);
    return btn::clamp(column, 0, columns - 1);
}

int enemies_grid::_safe_row(const btn::fixed_point& position)
{
    int row = (position.y().integer() / constants::enemies_grid_size) + (rows / 2);
    return btn::clamp(row, 0, rows - 1);
}

void enemies_grid::_add_enemy(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();
    enemy.set_last_grid_row(row);
    enemy.set_last_grid_column(column);

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _add_enemy_row(r, column, enemy);
    }
}

void enemies_grid::_remove_enemy(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _remove_enemy_row(r, column, enemy);
    }
}

void enemies_grid::_add_enemy_row(int row, int column, enemy& enemy)
{
    cell* cells_row = _cells_row(row);
    int enemy_columns = enemy.grid_columns();

    for(int c = column - enemy_columns; c <= column + enemy_columns; ++c)
    {
        cells_row[c].add_enemy(enemy, _pool);
    }
}

void enemies_grid::_remove_enemy_row(int row, int column, enemy& enemy)
{
    cell* cells_row = _cells_row(row);
    int enemy_columns = enemy.grid_columns();

    for(int c = column - enemy_columns; c <= column + enemy_columns; ++c)
    {
        cells_row[c].remove_enemy(enemy, _pool);
    }
}

void enemies_grid::_add_enemy_column(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _cells_row(r)[column].add_enemy(enemy, _pool);
    }
}

void enemies_grid::_remove_enemy_column(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _cells_row(r)[column].remove_enemy(enemy, _pool);
    }
}

void enemies_grid::cell::add_enemy(enemy& enemy, enemies_pool& enemies_pool)
{
    for(const enemies_list_node_type& enemies_node : _enemies)
    {
        if(enemies_node.enemy_ptr == &enemy)
        {
            return;
        }
    }

    BTN_ASSERT(! enemies_pool.full(), "No more available enemies in grid");

    enemies_list_node_type& enemies_node = enemies_pool.create(enemy);
    _enemies.push_front(enemies_node);
}

void enemies_grid::cell::remove_enemy(enemy& enemy, enemies_pool& enemies_pool)
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

void enemies_grid::cell::clear(enemies_pool& enemies_pool)
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
