#include "btn_palettes_bank.h"

#include "btn_math.h"
#include "btn_span.h"
#include "btn_limits.h"
#include "btn_memory.h"
#include "btn_algorithm.h"

namespace btn
{

namespace
{
    [[maybe_unused]] bool valid_colors_count(const span<const color>& colors_ref)
    {
        size_t colors_count = colors_ref.size();
        return colors_count >= hw::palettes::colors_per_palette() &&
                colors_count <= hw::palettes::colors() &&
                colors_count % hw::palettes::colors_per_palette() == 0;
    }
}

optional<int> palettes_bank::find(const span<const color>& colors_ref)
{
    BTN_ASSERT(valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());

    palette* palettes_data = _palettes.data();
    size_t colors_count = colors_ref.size();
    int eight_bits_per_pixel_palettes = _eight_bits_per_pixel_palettes;

    if(colors_count == hw::palettes::colors_per_palette())
    {
        int inferior_index = (hw::palettes::count() + eight_bits_per_pixel_palettes) / 2;

        if(BTN_LIKELY(_last_used_4bpp_index))
        {
            inferior_index = *_last_used_4bpp_index;
        }

        int superior_index = inferior_index + 1;
        bool valid_inferior_index = inferior_index >= eight_bits_per_pixel_palettes;
        bool valid_superior_index = superior_index < hw::palettes::count();

        span<const color> visible_colors = colors_ref.subspan(1);

        while(valid_inferior_index || valid_superior_index)
        {
            if(valid_inferior_index)
            {
                palette& pal = palettes_data[inferior_index];

                if(pal.usages)
                {
                    if(visible_colors == _4bpp_color_ptr_span(inferior_index).subspan(1))
                    {
                        ++pal.usages;
                        _last_used_4bpp_index = inferior_index;
                        return inferior_index;
                    }
                }

                --inferior_index;
                valid_inferior_index = inferior_index >= eight_bits_per_pixel_palettes;
            }

            if(valid_superior_index)
            {
                palette& pal = palettes_data[superior_index];

                if(pal.usages)
                {
                    if(visible_colors == _4bpp_color_ptr_span(superior_index).subspan(1))
                    {
                        ++pal.usages;
                        _last_used_4bpp_index = superior_index;
                        return superior_index;
                    }
                }

                ++superior_index;
                valid_superior_index = superior_index < hw::palettes::count();
            }
        }
    }
    else
    {
        if(eight_bits_per_pixel_palettes >= int(colors_count / hw::palettes::colors_per_palette()))
        {
            palette& pal = palettes_data[0];
            ++pal.usages;
            return 0;
        }
    }

    return nullopt;
}

optional<int> palettes_bank::create(const span<const color>& colors_ref)
{
    BTN_ASSERT(valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());

    palette* palettes_data = _palettes.data();
    size_t colors_count = colors_ref.size();
    int eight_bits_per_pixel_palettes = _eight_bits_per_pixel_palettes;

    if(colors_count == hw::palettes::colors_per_palette())
    {
        for(int index = hw::palettes::count() - 1; index >= eight_bits_per_pixel_palettes; --index)
        {
            palette& pal = palettes_data[index];

            if(! pal.usages)
            {
                pal = palette();
                pal.usages = 1;
                set_colors_ref(index, colors_ref);
                return index;
            }
        }
    }
    else
    {
        palette& pal = palettes_data[0];
        auto required_palettes = int(colors_count / hw::palettes::colors_per_palette());

        if(eight_bits_per_pixel_palettes >= required_palettes)
        {
            ++pal.usages;
            return 0;
        }

        if(required_palettes <= _first_4bpp_palette_index())
        {
            if(pal.usages)
            {
                ++pal.usages;
            }
            else
            {
                pal = palette();
                pal.usages = 1;
            }

            _eight_bits_per_pixel_palettes = required_palettes;
            set_colors_ref(0, colors_ref);
            return 0;
        }
    }

    return nullopt;
}

void palettes_bank::increase_usages(int id)
{
    palette& pal = _palettes[size_t(id)];
    ++pal.usages;
}

void palettes_bank::decrease_usages(int id)
{
    palette& pal = _palettes[size_t(id)];
    --pal.usages;

    if(! pal.usages)
    {
        pal = palette();

        if(eight_bits_per_pixel(id))
        {
            _eight_bits_per_pixel_palettes = 0;
        }
    }
}

int palettes_bank::colors_count(int id) const
{
    int result = hw::palettes::colors_per_palette();

    if(eight_bits_per_pixel(id))
    {
        result *= _eight_bits_per_pixel_palettes;
    }

    return result;
}

span<const color> palettes_bank::colors_ref(int id) const
{
    if(! eight_bits_per_pixel(id))
    {
        return _4bpp_color_ptr_span(id);
    }

    return span<const color>(_palettes[0].colors_ref,
            hw::palettes::colors_per_palette() * size_t(_eight_bits_per_pixel_palettes));
}

void palettes_bank::set_colors_ref(int id, const span<const color>& colors_ref)
{
    BTN_ASSERT(valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());
    BTN_ASSERT(int(colors_ref.size()) == colors_count(id), "Colors count mismatch: ",
               colors_ref.size(), " - ", colors_count(id));

    palette& pal = _palettes[size_t(id)];
    pal.colors_ref = colors_ref.data();
    pal.update = true;
    _perform_update = true;

    if(! eight_bits_per_pixel(id))
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::reload_colors_ref(int id)
{
    palette& pal = _palettes[size_t(id)];
    pal.update = true;
    _perform_update = true;

    if(! eight_bits_per_pixel(id))
    {
        _last_used_4bpp_index = id;
    }
}

fixed palettes_bank::inverse_intensity(int id) const
{
    const palette& pal = _palettes[size_t(id)];
    return pal.inverse_intensity;
}

void palettes_bank::set_inverse_intensity(int id, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[size_t(id)];
    pal.inverse_intensity = intensity;
    pal.update = true;
    _perform_update = true;

    if(! eight_bits_per_pixel(id))
    {
        _last_used_4bpp_index = id;
    }
}

fixed palettes_bank::grayscale_intensity(int id) const
{
    const palette& pal = _palettes[size_t(id)];
    return pal.grayscale_intensity;
}

void palettes_bank::set_grayscale_intensity(int id, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[size_t(id)];
    pal.grayscale_intensity = intensity;
    pal.update = true;
    _perform_update = true;

    if(! eight_bits_per_pixel(id))
    {
        _last_used_4bpp_index = id;
    }
}

color palettes_bank::fade_color(int id) const
{
    const palette& pal = _palettes[size_t(id)];
    return pal.fade_color;
}

fixed palettes_bank::fade_intensity(int id) const
{
    const palette& pal = _palettes[size_t(id)];
    return pal.fade_intensity;
}

void palettes_bank::set_fade(int id, color color, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[size_t(id)];
    pal.fade_color = color;
    pal.fade_intensity = intensity;
    pal.update = true;
    _perform_update = true;

    if(! eight_bits_per_pixel(id))
    {
        _last_used_4bpp_index = id;
    }
}

int palettes_bank::rotate_count(int id) const
{
    const palette& pal = _palettes[size_t(id)];
    return pal.rotate_count;
}

void palettes_bank::set_rotate_count(int id, int count)
{
    BTN_ASSERT(abs(count) < colors_count(id), "Invalid count: ", count, " - ", colors_count(id));

    palette& pal = _palettes[size_t(id)];
    pal.rotate_count = count;
    pal.update = true;
    _perform_update = true;

    if(! eight_bits_per_pixel(id))
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::set_transparent_color(const optional<color>& transparent_color)
{
    _transparent_color = transparent_color;
    _perform_update = true;
}

void palettes_bank::set_brightness(fixed brightness)
{
    BTN_ASSERT(brightness >= -1 && brightness <= 1, "Invalid brightness: ", brightness);

    _brightness = brightness;
    _perform_update = true;
}

void palettes_bank::set_contrast(fixed contrast)
{
    BTN_ASSERT(contrast >= -1 && contrast <= 1, "Invalid contrast: ", contrast);

    _contrast = contrast;
    _perform_update = true;
}

void palettes_bank::set_intensity(fixed intensity)
{
    BTN_ASSERT(intensity >= -1 && intensity <= 1, "Invalid intensity: ", intensity);

    _intensity = intensity;
    _perform_update = true;
}

void palettes_bank::set_inverse_intensity(fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    _inverse_intensity = intensity;
    _perform_update = true;
}

void palettes_bank::set_grayscale_intensity(fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    _grayscale_intensity = intensity;
    _perform_update = true;
}

void palettes_bank::set_fade(color color, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    _fade_color = color;
    _fade_intensity = intensity;
    _perform_update = true;
}

void palettes_bank::update()
{
    size_t first_index = numeric_limits<size_t>::max();
    size_t last_index = 0;

    if(_perform_update)
    {
        _perform_update = false;

        color* colors_ptr = _colors.data();
        int brightness = fixed_t<8>(_brightness).value();
        int contrast = fixed_t<8>(_contrast).value();
        int intensity = fixed_t<8>(_intensity).value();
        int inverse_intensity = fixed_t<5>(_inverse_intensity).value();
        int grayscale_intensity = fixed_t<5>(_grayscale_intensity).value();
        int fade_intensity = fixed_t<5>(_fade_intensity).value();
        bool update_all = brightness || contrast || intensity || inverse_intensity || grayscale_intensity ||
                fade_intensity;
        int pal_colors_count = hw::palettes::colors_per_palette();
        int eight_bits_per_pixel_palettes = _eight_bits_per_pixel_palettes;

        if(eight_bits_per_pixel_palettes)
        {
            pal_colors_count *= eight_bits_per_pixel_palettes;
        }

        for(size_t index = 0, limit = _palettes.size(); index < limit; ++index)
        {
            palette& pal = _palettes[index];

            if(pal.update || (update_all && pal.usages))
            {
                pal.update = false;
                first_index = min(first_index, index);
                last_index = max(last_index, index);

                color& pal_colors_ref = colors_ptr[int(index) * pal_colors_count];
                memory::copy(*pal.colors_ref, int(pal_colors_count), pal_colors_ref);

                if(int pal_inverse_intensity = fixed_t<5>(pal.inverse_intensity).value())
                {
                    hw::palettes::inverse(pal_inverse_intensity, pal_colors_count, pal_colors_ref);
                }

                if(int pal_grayscale_intensity = fixed_t<5>(pal.grayscale_intensity).value())
                {
                    hw::palettes::grayscale(pal_grayscale_intensity, pal_colors_count, pal_colors_ref);
                }

                if(int pal_fade_intensity = fixed_t<5>(pal.fade_intensity).value())
                {
                    hw::palettes::fade(pal.fade_color, pal_fade_intensity, pal_colors_count, pal_colors_ref);
                }

                if(pal.rotate_count)
                {
                    hw::palettes::rotate(pal.rotate_count, pal_colors_count, pal_colors_ref);
                }
            }

            pal_colors_count = hw::palettes::colors_per_palette();
        }

        if(_transparent_color)
        {
            colors_ptr[0] = *_transparent_color;
            first_index = 0;
        }

        if(update_all && first_index != numeric_limits<size_t>::max())
        {
            color& all_colors_ref = colors_ptr[int(first_index) * hw::palettes::colors_per_palette()];
            int all_colors_count = hw::palettes::colors_per_palette();

            if(last_index == 0)
            {
                if(eight_bits_per_pixel_palettes)
                {
                    all_colors_count *= eight_bits_per_pixel_palettes;
                }
            }
            else
            {
                all_colors_count *= int(last_index - first_index + 1);
            }

            if(brightness)
            {
                hw::palettes::brightness(brightness, all_colors_count, all_colors_ref);
            }

            if(contrast)
            {
                hw::palettes::contrast(contrast, all_colors_count, all_colors_ref);
            }

            if(intensity)
            {
                hw::palettes::intensity(intensity, all_colors_count, all_colors_ref);
            }

            if(inverse_intensity)
            {
                hw::palettes::inverse(inverse_intensity, all_colors_count, all_colors_ref);
            }

            if(grayscale_intensity)
            {
                hw::palettes::grayscale(grayscale_intensity, all_colors_count, all_colors_ref);
            }

            if(fade_intensity)
            {
                hw::palettes::fade(_fade_color, fade_intensity, all_colors_count, all_colors_ref);
            }
        }
    }

    if(first_index == numeric_limits<size_t>::max())
    {
        _first_index_to_commit.reset();
        _last_index_to_commit.reset();
    }
    else
    {
        _first_index_to_commit = int(first_index);
        _last_index_to_commit = int(last_index);
    }
}

optional<palettes_bank::commit_data> palettes_bank::retrieve_commit_data()
{
    optional<commit_data> result;

    if(_first_index_to_commit)
    {
        int colors_offset = *_first_index_to_commit * hw::palettes::colors_per_palette();
        int colors_count = hw::palettes::colors_per_palette();

        if(*_last_index_to_commit == 0)
        {
            if(_eight_bits_per_pixel_palettes)
            {
                colors_count *= _eight_bits_per_pixel_palettes;
            }
        }
        else
        {
            colors_count *= *_last_index_to_commit - *_first_index_to_commit + 1;
        }

        result = commit_data{ _colors.data(), colors_offset, colors_count };
        _first_index_to_commit.reset();
        _last_index_to_commit.reset();
    }

    return result;
}

span<const color> palettes_bank::_4bpp_color_ptr_span(int index) const
{
    return span<const color>(_palettes[size_t(index)].colors_ref, hw::palettes::colors_per_palette());
}

int palettes_bank::_first_4bpp_palette_index() const
{
    const palette* palettes_data = _palettes.data();

    for(int index = 1; index < hw::palettes::count(); ++index)
    {
        if(palettes_data[index].usages)
        {
            return index;
        }
    }

    return hw::palettes::count();
}

}
