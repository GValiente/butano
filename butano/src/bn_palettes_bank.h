/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTES_BANK_H
#define BN_PALETTES_BANK_H

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_color.h"
#include "bn_limits.h"
#include "bn_optional.h"
#include "bn_config_log.h"
#include "bn_unordered_map.h"
#include "bn_identity_hasher.h"
#include "../hw/include/bn_hw_palettes.h"

namespace bn
{

enum class bpp_mode : uint8_t;
enum class compression_type : uint8_t;

class palettes_bank
{

public:
    class commit_data
    {

    public:
        const color* colors_ptr;
        int offset;
        int count;
    };

    [[nodiscard]] static uint16_t colors_hash(const span<const color>& colors);

    [[nodiscard]] int used_colors_count() const;

    [[nodiscard]] int available_colors_count() const
    {
        return hw::palettes::colors() - used_colors_count();
    }

    #if BN_CFG_LOG_ENABLED
        void log_status() const;
    #endif

    [[nodiscard]] int find_bpp_4(const span<const color>& colors, uint16_t hash);

    [[nodiscard]] int find_bpp_8(const span<const color>& colors);

    [[nodiscard]] int create_bpp_4(const span<const color>& colors, uint16_t hash, bool required);

    [[nodiscard]] int create_bpp_8(const span<const color>& colors, compression_type compression, bool required);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int colors_count(int id) const
    {
        return _palettes[id].slots_count * hw::palettes::colors_per_palette();
    }

    [[nodiscard]] bpp_mode bpp(int id) const;

    [[nodiscard]] span<const color> colors(int id) const;

    void set_colors(int id, const span<const color>& colors);

    void set_color(int id, int color_index, color color);

    [[nodiscard]] bool inverted(int id) const
    {
        return _palettes[id].inverted;
    }

    void set_inverted(int id, bool inverted);

    [[nodiscard]] fixed grayscale_intensity(int id) const
    {
        return _palettes[id].grayscale_intensity;
    }

    void set_grayscale_intensity(int id, fixed intensity);

    [[nodiscard]] fixed hue_shift_intensity(int id) const
    {
        return _palettes[id].hue_shift_intensity;
    }

    void set_hue_shift_intensity(int id, fixed intensity);

    [[nodiscard]] color fade_color(int id) const
    {
        return _palettes[id].fade_color;
    }

    [[nodiscard]] fixed fade_intensity(int id) const
    {
        return _palettes[id].fade_intensity;
    }

    void set_fade_color(int id, color color);

    void set_fade_intensity(int id, fixed intensity);

    void set_fade(int id, color color, fixed intensity);

    [[nodiscard]] int rotate_count(int id) const
    {
        return _palettes[id].rotate_count;
    }

    void set_rotate_count(int id, int count);

    [[nodiscard]] int rotate_range_start(int id) const
    {
        return _palettes[id].rotate_range_start;
    }

    [[nodiscard]] int rotate_range_size(int id) const
    {
        return _palettes[id].rotate_range_size;
    }

    void set_rotate_range(int id, int start, int size);

    void reload(int id);

    [[nodiscard]] const optional<color>& transparent_color() const
    {
        return _transparent_color;
    }

    void set_transparent_color(const optional<color>& transparent_color);

    void reload_transparent_color()
    {
        _update = true;
    }

    [[nodiscard]] fixed brightness() const
    {
        return _brightness;
    }

    void set_brightness(fixed brightness);

    [[nodiscard]] fixed contrast() const
    {
        return _contrast;
    }

    void set_contrast(fixed contrast);

    [[nodiscard]] fixed intensity() const
    {
        return _intensity;
    }

    void set_intensity(fixed intensity);

    [[nodiscard]] bool inverted() const
    {
        return _inverted;
    }

    void set_inverted(bool inverted);

    [[nodiscard]] fixed grayscale_intensity() const
    {
        return _grayscale_intensity;
    }

    void set_grayscale_intensity(fixed intensity);

    [[nodiscard]] fixed hue_shift_intensity() const
    {
        return _hue_shift_intensity;
    }

    void set_hue_shift_intensity(fixed intensity);

    [[nodiscard]] color fade_color() const
    {
        return _fade_color;
    }

    [[nodiscard]] fixed fade_intensity() const
    {
        return _fade_intensity;
    }

    void set_fade_color(color color);

    void set_fade_intensity(fixed intensity);

    void set_fade(color color, fixed intensity);

    void update();

    [[nodiscard]] commit_data retrieve_commit_data() const;

    [[nodiscard]] bool global_effects_updated() const
    {
        return _global_effects_updated;
    }

    void reset_commit_data();

    void fill_hblank_effect_colors(int id, const color* source_colors_ptr, uint16_t* dest_ptr) const;

    void fill_hblank_effect_colors(const color* source_colors_ptr, uint16_t* dest_ptr) const;

    void stop()
    {
        _update = false;
    }

private:
    class palette
    {

    public:
        unsigned usages = 0;
        fixed grayscale_intensity;
        fixed hue_shift_intensity;
        fixed fade_intensity;
        color fade_color;
        uint16_t hash = 0;
        int16_t rotate_count = 0;
        int8_t slots_count = 1;
        int8_t rotate_range_start = 1;
        int8_t rotate_range_size = 0;
        bool bpp_8: 1 = false;
        bool inverted: 1 = false;
        bool update: 1 = false;
        bool locked: 1 = false;

        void apply_effects(int dest_colors_count, color* dest_colors_ptr) const;
    };

    palette _palettes[hw::palettes::count()] = {};
    alignas(int) color _initial_colors[hw::palettes::colors()] = {};
    alignas(int) color _final_colors[hw::palettes::colors()] = {};
    optional<color> _transparent_color;
    fixed _brightness;
    fixed _contrast;
    fixed _intensity;
    fixed _grayscale_intensity;
    fixed _hue_shift_intensity;
    fixed _fade_intensity;
    unordered_map<uint16_t, int16_t, hw::palettes::count() * 2, identity_hasher> _bpp_4_indexes_map;
    int _first_index_to_commit = numeric_limits<int>::max();
    int _last_index_to_commit = 0;
    color _fade_color;
    bool _inverted = false;
    bool _update = false;
    bool _update_global_effects = false;
    bool _global_effects_enabled = false;
    bool _global_effects_updated = false;

    [[nodiscard]] bool _same_colors(const span<const color>& colors, int id) const;

    [[nodiscard]] int _bpp_8_slots_count() const;

    [[nodiscard]] int _first_bpp_4_palette_index() const;

    __attribute__((noinline)) void _erase_bpp_4_indexes_map_index(uint16_t hash)
    {
        _bpp_4_indexes_map.erase(hash);
    }

    void _on_global_effect_updated(bool active);

    void _set_colors_bpp_impl(int id, const span<const color>& colors);

    void _update_palette(int id);

    void _apply_global_effects(int dest_colors_count, color* dest_colors_ptr) const;
};

}

#endif
