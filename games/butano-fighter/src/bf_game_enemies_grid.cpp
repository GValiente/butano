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

void enemies_grid::add_enemy(enemy& enemy)
{
    btn::fixed_point position = enemy.position();
    int row = _row(position);
    int column = _column(position);
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _add_enemy_row(r, column, enemy);
    }
}

void enemies_grid::remove_enemy(enemy& enemy)
{
    btn::fixed_point position = enemy.position();
    int row = _row(position);
    int column = _column(position);
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _remove_enemy_row(r, column, enemy);
    }
}

bool enemies_grid::update_enemy(enemy& enemy)
{
    btn::fixed_point position = enemy.position();
    int old_row = enemy.last_grid_row();
    int old_column = enemy.last_grid_column();
    int new_row = _row(position);
    int new_column = _column(position);
    bool updated = false;
    enemy.set_last_grid_row(new_row);
    enemy.set_last_grid_column(new_column);

    if(old_row < new_row)
    {
        _remove_enemy_row(old_row - 1, old_column, enemy);
        _add_enemy_row(new_row + 1, new_column, enemy);
        updated = true;
    }
    else if(old_row > new_row)
    {
        _remove_enemy_row(old_row + 1, old_column, enemy);
        _add_enemy_row(new_row - 1, new_column, enemy);
        updated = true;
    }

    if(old_column < new_column)
    {
        _remove_enemy_row(old_row, old_column - 1, enemy);
        _add_enemy_column(new_row, new_column + 1, enemy);
        updated = true;
    }
    else if(old_column > new_column)
    {
        _remove_enemy_row(old_row, old_column + 1, enemy);
        _add_enemy_column(new_row, new_column - 1, enemy);
        updated = true;
    }

    return updated;
}

bool enemies_grid::check_hero(const btn::fixed_rect& hero_rect) const
{
    const btn::fixed_point& hero_position = hero_rect.position();
    int row = _row(hero_position);
    int column = _column(hero_position);

    for(enemy* enemy : _cells_row(row)[column].enemies())
    {
        if(enemy->check_hero(hero_rect))
        {
            return true;
        }
    }

    return false;
}

bool enemies_grid::check_hero_bullet(const check_hero_bullet_data& data)
{
    const btn::fixed_point& bullet_position = data.bullet_rect.position();
    int row = _row(bullet_position);
    int column = _column(bullet_position);

    for(enemy* enemy : _cells_row(row)[column].enemies())
    {
        if(enemy->check_hero_bullet(data))
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
            btn::string<(columns * constants::max_enemies_per_grid_cell * 4) + 6> string;
            btn::input_string_stream stream(string);
            stream.append("\t[");

            for(int c = 0; c < columns; ++c)
            {
                const cell& cell = cells_row[c];
                const btn::ivector<enemy*>& enemies = cell.enemies();

                if(enemies.empty())
                {
                    stream.append('_');
                }
                else
                {
                    for(const enemy* enemy : enemies)
                    {
                        stream.append(enemy->tag());
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
    BTN_ASSERT(row >= cell_increment && row < rows - cell_increment, "Invalid row: ", row, " - ", position.x().integer());

    return row;
}

void enemies_grid::_add_enemy_row(int row, int column, enemy& enemy)
{
    cell* cells_row = _cells_row(row);
    int enemy_columns = enemy.grid_columns();

    for(int c = column - enemy_columns; c <= column + enemy_columns; ++c)
    {
        cells_row[c].add_enemy(enemy);
    }
}

void enemies_grid::_remove_enemy_row(int row, int column, enemy& enemy)
{
    cell* cells_row = _cells_row(row);
    int enemy_columns = enemy.grid_columns();

    for(int c = column - enemy_columns; c <= column + enemy_columns; ++c)
    {
        cells_row[c].remove_enemy(enemy);
    }
}

void enemies_grid::_add_enemy_column(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _cells_row(r)[column].add_enemy(enemy);
    }
}

void enemies_grid::_remove_enemy_column(int row, int column, enemy& enemy)
{
    int enemy_rows = enemy.grid_rows();

    for(int r = row - enemy_rows; r <= row + enemy_rows; ++r)
    {
        _cells_row(r)[column].remove_enemy(enemy);
    }
}

void enemies_grid::cell::add_enemy(enemy& enemy)
{
    auto end = _enemies.end();

    if(btn::find(_enemies.begin(), end, &enemy) == end)
    {
        BTN_ASSERT(! _enemies.full(), "No more available enemy indexes");

        _enemies.push_back(&enemy);
    }
}

void enemies_grid::cell::remove_enemy(enemy& enemy)
{
    auto end = _enemies.end();
    auto it = btn::find(_enemies.begin(), end, &enemy);

    if(it != end)
    {
        _enemies.erase(it);
    }
}

}
