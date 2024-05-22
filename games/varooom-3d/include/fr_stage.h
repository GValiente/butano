/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_STAGE_H
#define FR_STAGE_H

#include "bn_music_item.h"
#include "bn_fixed_point.h"

#include "fr_checkpoint.h"
#include "fr_car_engine.h"
#include "fr_rival_car_info.h"
#include "fr_visible_model_3d_grid.h"

namespace bn
{
    class color;
    class sprite_item;
    class affine_bg_item;
}

namespace fr
{

enum class difficulty_level : uint8_t;

class stage
{

public:
    [[nodiscard]] static const stage& stage_1(difficulty_level difficulty, bool reverse);

    constexpr stage(const bn::span<const model_3d_item>& model_items, const bn::affine_bg_item& ground_bg_item,
                    int slow_ground_tile_index, const bn::affine_bg_item& clouds_bg_item,
                    const bn::fixed_point& clouds_bg_pivot_inc, bn::fixed clouds_bg_transparency_alpha,
                    const bn::sprite_item& player_car_sprite_item, const bn::span<const bn::color>& model_colors,
                    bn::music_item music_item, int countdown_wait_frames, const car_engine& player_car_engine,
                    const bn::span<const checkpoint>& player_checkpoints,
                    const bn::span<const bn::point>& player_checkpoint_vectors, int middle_player_checkpoint_index,
                    const bn::span<const rival_car_info>& rival_car_infos,
                    const bn::span<const checkpoint>& rival_checkpoints, const point_3d& start_position,
                    bn::fixed start_angle, int total_laps, int total_time, int time_increase) :
        _ground_bg_item(ground_bg_item),
        _clouds_bg_item(clouds_bg_item),
        _player_car_sprite_item(player_car_sprite_item),
        _player_car_engine(player_car_engine),
        _clouds_bg_pivot_inc(clouds_bg_pivot_inc),
        _clouds_bg_transparency_alpha(clouds_bg_transparency_alpha),
        _model_items(model_items),
        _model_colors(model_colors),
        _music_item(music_item),
        _player_checkpoints(player_checkpoints),
        _player_checkpoint_vectors(player_checkpoint_vectors),
        _rival_car_infos(rival_car_infos),
        _rival_checkpoints(rival_checkpoints),
        _model_grid(model_items),
        _visible_model_grid(_model_grid.cells()),
        _start_position(start_position),
        _start_angle(start_angle),
        _slow_ground_tile_index(slow_ground_tile_index),
        _countdown_wait_frames(countdown_wait_frames),
        _total_laps(total_laps),
        _total_time(total_time),
        _time_increase(time_increase),
        _middle_player_checkpoint_index(middle_player_checkpoint_index)
    {
        BN_ASSERT(model_items.size() <= constants_3d::max_stage_models);
        BN_ASSERT(player_checkpoints.size() == player_checkpoint_vectors.size());
        BN_ASSERT(middle_player_checkpoint_index >= 0 && middle_player_checkpoint_index <= player_checkpoints.size());
        BN_ASSERT(rival_car_infos.size() <= constants_3d::max_rival_cars);
    }

    [[nodiscard]] constexpr const bn::span<const model_3d_item>& model_items() const
    {
        return _model_items;
    }

    [[nodiscard]] constexpr const model_3d_grid& model_grid() const
    {
        return _model_grid;
    }

    [[nodiscard]] constexpr const visible_model_3d_grid& visible_model_grid() const
    {
        return _visible_model_grid;
    }

    [[nodiscard]] constexpr const bn::affine_bg_item& ground_bg_item() const
    {
        return _ground_bg_item;
    }

    [[nodiscard]] constexpr int slow_ground_tile_index() const
    {
        return _slow_ground_tile_index;
    }

    [[nodiscard]] constexpr const bn::affine_bg_item& clouds_bg_item() const
    {
        return _clouds_bg_item;
    }

    [[nodiscard]] constexpr const bn::fixed_point& clouds_bg_pivot_inc() const
    {
        return _clouds_bg_pivot_inc;
    }

    [[nodiscard]] constexpr bn::fixed clouds_bg_transparency_alpha() const
    {
        return _clouds_bg_transparency_alpha;
    }

    [[nodiscard]] constexpr const bn::sprite_item& player_car_sprite_item() const
    {
        return _player_car_sprite_item;
    }

    [[nodiscard]] constexpr const bn::span<const bn::color>& model_colors() const
    {
        return _model_colors;
    }

    [[nodiscard]] constexpr bn::music_item music_item() const
    {
        return _music_item;
    }

    [[nodiscard]] constexpr int countdown_wait_frames() const
    {
        return _countdown_wait_frames;
    }

    [[nodiscard]] constexpr const car_engine& player_car_engine() const
    {
        return _player_car_engine;
    }

    [[nodiscard]] constexpr const bn::span<const checkpoint>& player_checkpoints() const
    {
        return _player_checkpoints;
    }

    [[nodiscard]] constexpr const bn::span<const bn::point>& player_checkpoint_vectors() const
    {
        return _player_checkpoint_vectors;
    }

    [[nodiscard]] constexpr int middle_player_checkpoint_index() const
    {
        return _middle_player_checkpoint_index;
    }

    [[nodiscard]] constexpr const bn::span<const rival_car_info>& rival_car_infos() const
    {
        return _rival_car_infos;
    }

    [[nodiscard]] constexpr const bn::span<const checkpoint>& rival_checkpoints() const
    {
        return _rival_checkpoints;
    }

    [[nodiscard]] constexpr const point_3d& start_position() const
    {
        return _start_position;
    }

    [[nodiscard]] constexpr bn::fixed start_angle() const
    {
        return _start_angle;
    }

    [[nodiscard]] constexpr int total_laps() const
    {
        return _total_laps;
    }

    [[nodiscard]] constexpr int total_time() const
    {
        return _total_time;
    }

    [[nodiscard]] constexpr int time_increase() const
    {
        return _time_increase;
    }

private:
    const bn::affine_bg_item& _ground_bg_item;
    const bn::affine_bg_item& _clouds_bg_item;
    const bn::sprite_item& _player_car_sprite_item;
    const car_engine& _player_car_engine;
    bn::fixed_point _clouds_bg_pivot_inc;
    bn::fixed _clouds_bg_transparency_alpha;
    bn::span<const model_3d_item> _model_items;
    bn::span<const bn::color> _model_colors;
    bn::music_item _music_item;
    bn::span<const checkpoint> _player_checkpoints;
    bn::span<const bn::point> _player_checkpoint_vectors;
    bn::span<const rival_car_info> _rival_car_infos;
    bn::span<const checkpoint> _rival_checkpoints;
    model_3d_grid _model_grid;
    visible_model_3d_grid _visible_model_grid;
    point_3d _start_position;
    bn::fixed _start_angle;
    int _slow_ground_tile_index;
    int _countdown_wait_frames;
    int _total_laps;
    int _total_time;
    int _time_increase;
    int _middle_player_checkpoint_index;
};

}

#endif
