/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SRAM_DATA_H
#define FR_SRAM_DATA_H

#include "fr_stages_status.h"
#include "fr_brightness_level.h"
#include "fr_model_viewer_items_status.h"

namespace fr
{

class sram_data
{

public:
    void init();

    [[nodiscard]] const stages_status& stages() const
    {
        return _stages;
    }

    [[nodiscard]] stages_status& stages()
    {
        return _stages;
    }

    [[nodiscard]] const model_viewer_items_status& model_viewer_items() const
    {
        return _viewer_items;
    }

    [[nodiscard]] model_viewer_items_status& model_viewer_items()
    {
        return _viewer_items;
    }

    [[nodiscard]] difficulty_level difficulty() const
    {
        return _difficulty_level;
    }

    void set_difficulty(difficulty_level difficulty)
    {
        _difficulty_level = difficulty;
    }

    [[nodiscard]] bool reverse_race() const
    {
        return _reverse_race;
    }

    void set_reverse_race(bool reverse)
    {
        _reverse_race = reverse;
    }

    [[nodiscard]] brightness_level brightness() const
    {
        return _brightness_level;
    }

    void set_brightness(brightness_level brightness);

    [[nodiscard]] bool rumble_allowed() const
    {
        return _rumble_allowed;
    }

    void set_rumble_allowed(bool rumble_allowed)
    {
        _rumble_allowed = rumble_allowed;
    }

    [[nodiscard]] bool brightness_scene_shown() const
    {
        return _brightness_scene_shown;
    }

    void set_brightness_scene_shown(bool shown)
    {
        _brightness_scene_shown = shown;
    }

    [[nodiscard]] bool how_to_play_scene_shown() const
    {
        return _how_to_play_scene_shown;
    }

    void set_how_to_play_scene_shown(bool shown)
    {
        _how_to_play_scene_shown = shown;
    }

    [[nodiscard]] int advice_index() const
    {
        return _advice_index;
    }

    void set_advice_index(int advice_index)
    {
        _advice_index = uint8_t(advice_index);
    }

    void write();

private:
    char _label[8] = {};
    stages_status _stages;
    model_viewer_items_status _viewer_items;
    difficulty_level _difficulty_level = difficulty_level::EASY;
    brightness_level _brightness_level = brightness_level::LOW;
    uint8_t _advice_index = 0;
    bool _reverse_race = false;
    bool _rumble_allowed = true;
    bool _brightness_scene_shown = false;
    bool _how_to_play_scene_shown = false;
};

}

#endif
