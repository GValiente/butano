#ifndef BTN_PALETTES_BANK_H
#define BTN_PALETTES_BANK_H

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_color.h"
#include "btn_optional.h"
#include "btn_palette_bpp_mode.h"
#include "../hw/include/btn_hw_palettes.h"

namespace btn
{

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

    [[nodiscard]] int used_count() const;

    [[nodiscard]] int available_count() const
    {
        return hw::palettes::count() - used_count();
    }

    [[nodiscard]] optional<int> find(const span<const color>& colors_ref, palette_bpp_mode bpp_mode);

    [[nodiscard]] optional<int> create(const span<const color>& colors_ref, palette_bpp_mode bpp_mode);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int colors_count(int id) const
    {
        return _palettes[id].slots_count * hw::palettes::colors_per_palette();
    }

    [[nodiscard]] bool eight_bits_per_pixel(int id) const
    {
        return palette_bpp_mode(_palettes[id].bpp_mode) == palette_bpp_mode::BPP_8;
    }

    [[nodiscard]] span<const color> colors_ref(int id) const
    {
        return _palettes[id].colors_span();
    }

    void set_colors_ref(int id, const span<const color>& colors_ref, palette_bpp_mode bpp_mode);

    void reload_colors_ref(int id);

    [[nodiscard]] fixed inverse_intensity(int id) const
    {
        return _palettes[id].inverse_intensity;
    }

    void set_inverse_intensity(int id, fixed intensity);

    [[nodiscard]] fixed grayscale_intensity(int id) const
    {
        return _palettes[id].grayscale_intensity;
    }

    void set_grayscale_intensity(int id, fixed intensity);

    [[nodiscard]] color fade_color(int id) const
    {
        return _palettes[id].fade_color;
    }

    [[nodiscard]] fixed fade_intensity(int id) const
    {
        return _palettes[id].fade_intensity;
    }

    void set_fade(int id, color color, fixed intensity);

    [[nodiscard]] int rotate_count(int id) const
    {
        return _palettes[id].rotate_count;
    }

    void set_rotate_count(int id, int count);

    [[nodiscard]] const optional<color>& transparent_color() const
    {
        return _transparent_color;
    }

    void set_transparent_color(const optional<color>& transparent_color);

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

    [[nodiscard]] fixed inverse_intensity() const
    {
        return _inverse_intensity;
    }

    void set_inverse_intensity(fixed intensity);

    [[nodiscard]] fixed grayscale_intensity() const
    {
        return _grayscale_intensity;
    }

    void set_grayscale_intensity(fixed intensity);

    [[nodiscard]] color fade_color() const
    {
        return _fade_color;
    }

    [[nodiscard]] fixed fade_intensity() const
    {
        return _fade_intensity;
    }

    void set_fade(color color, fixed intensity);

    void update();

    [[nodiscard]] optional<commit_data> retrieve_commit_data();

private:
    class palette
    {

    public:
        const color* colors_ref = nullptr;
        fixed inverse_intensity;
        fixed grayscale_intensity;
        fixed fade_intensity;
        unsigned usages = 0;
        color fade_color;
        int16_t rotate_count = 0;
        uint8_t bpp_mode = 0;
        int8_t slots_count = 0;
        bool update = false;

        span<const color> colors_span() const
        {
            return span<const color>(colors_ref, size_t(hw::palettes::colors_per_palette() * slots_count));
        }

        span<const color> visible_colors_span() const
        {
            return span<const color>(colors_ref + 1, size_t(hw::palettes::colors_per_palette() * slots_count) - 1);
        }
    };

    palette _palettes[hw::palettes::count()] = {};
    color _colors[hw::palettes::colors()] = {};
    optional<int> _first_index_to_commit;
    optional<int> _last_index_to_commit;
    optional<color> _transparent_color;
    fixed _brightness;
    fixed _contrast;
    fixed _intensity;
    fixed _inverse_intensity;
    fixed _grayscale_intensity;
    fixed _fade_intensity;
    int _last_used_4bpp_index = hw::palettes::count() - 1;
    color _fade_color;
    bool _perform_update = false;

    [[nodiscard]] int _bpp8_slots_count() const;

    [[nodiscard]] int _first_4bpp_palette_index() const;
};

}

#endif
