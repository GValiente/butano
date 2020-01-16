#ifndef BTN_PALETTES_BANK_H
#define BTN_PALETTES_BANK_H

#include "btn_span.h"
#include "btn_array.h"
#include "btn_fixed.h"
#include "btn_color.h"
#include "btn_optional.h"
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

    [[nodiscard]] optional<int> find(const span<const color>& colors_ref);

    [[nodiscard]] int create(const span<const color>& colors_ref);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] span<const color> colors_ref(int id) const;

    void set_colors_ref(int id, const span<const color>& colors_ref);

    void reload_colors_ref(int id);

    [[nodiscard]] fixed inverse_intensity(int id) const;

    void set_inverse_intensity(int id, fixed intensity);

    [[nodiscard]] fixed grayscale_intensity(int id) const;

    void set_grayscale_intensity(int id, fixed intensity);

    [[nodiscard]] color fade_color(int id) const;

    [[nodiscard]] fixed fade_intensity(int id) const;

    void set_fade(int id, color color, fixed intensity);

    [[nodiscard]] int rotate_count(int id) const;

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
        signed rotate_count: 15;
        unsigned update: 1;

        palette()
        {
            rotate_count = 0;
            update = false;
        }
    };

    array<palette, hw::palettes::count()> _palettes = {};
    array<color, hw::palettes::colors()> _colors = {};
    optional<color> _transparent_color;
    fixed _brightness;
    fixed _contrast;
    fixed _intensity;
    fixed _inverse_intensity;
    fixed _grayscale_intensity;
    fixed _fade_intensity;
    color _fade_color;
    optional<int> _last_used_4bpp_index;
    optional<int> _first_index_to_commit;
    optional<int> _last_index_to_commit;
    int _eight_bits_per_pixel_palettes = 0;
    bool _perform_update = false;

    [[nodiscard]] span<const color> _color_ptr_span(int index) const;

    span<const color> _4bpp_color_ptr_span(int index) const;

    [[nodiscard]] int _colors_per_palette(int id) const;

    [[nodiscard]] int _first_4bpp_palette_index() const;
};

}

#endif
