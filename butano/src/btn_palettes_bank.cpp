#include "btn_palettes_bank.h"

#include "btn_math.h"
#include "btn_span.h"
#include "btn_limits.h"
#include "btn_algorithm.h"

namespace btn
{

namespace
{
    constexpr int colors_per_palette()
    {
        return hw::palettes::colors_per_palette();
    }

    constexpr int intensity_scale()
    {
        return 32;
    }
}

optional<int> palettes_bank::find(const span<const color>& colors)
{
    BTN_ASSERT(colors.size() == colors_per_palette(), "Invalid colors size: ", colors.size());

    int inferior_index = hw::palettes::count() / 2;

    if(BTN_LIKELY(_last_used_index))
    {
        inferior_index = *_last_used_index;
    }

    palette* palettes_data = _palettes.data();
    int superior_index = inferior_index + 1;
    bool valid_inferior_index = inferior_index >= 0;
    bool valid_superior_index = superior_index < hw::palettes::count();

    span<const color> visible_colors = colors.subspan(1);

    while(valid_inferior_index || valid_superior_index)
    {
        if(valid_inferior_index)
        {
            palette& pal = palettes_data[inferior_index];

            if(pal.usages)
            {
                if(visible_colors == _color_ptr_span(inferior_index).subspan(1))
                {
                    ++pal.usages;
                    _last_used_index = inferior_index;
                    return inferior_index;
                }
            }

            --inferior_index;
            valid_inferior_index = inferior_index >= 0;
        }

        if(valid_superior_index)
        {
            palette& pal = palettes_data[superior_index];

            if(pal.usages)
            {
                if(visible_colors == _color_ptr_span(superior_index).subspan(1))
                {
                    ++pal.usages;
                    _last_used_index = superior_index;
                    return superior_index;
                }
            }

            ++superior_index;
            valid_superior_index = superior_index < hw::palettes::count();
        }
    }

    return nullopt;
}

int palettes_bank::create(const span<const color>& colors_ref)
{
    BTN_ASSERT(colors_ref.size() == colors_per_palette(), "Invalid colors count: ", colors_ref.size());

    palette* palettes_data = _palettes.data();

    for(int index = hw::palettes::count() - 1; index >= 0; --index)
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

    BTN_ERROR("All bank palettes are used");
    return 0;
}

void palettes_bank::increase_usages(int id)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    ++pal.usages;
}

void palettes_bank::decrease_usages(int id)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    --pal.usages;

    if(! pal.usages)
    {
        pal = palette();
    }
}

span<const color> palettes_bank::colors_ref(int id) const
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);
    BTN_ASSERT(_palettes[size_t(id)].usages, "Palette is not used: ", id);

    return _color_ptr_span(id);
}

void palettes_bank::set_colors_ref(int id, const span<const color>& colors_ref)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);
    BTN_ASSERT(colors_ref.size() == colors_per_palette(), "Invalid colors count: ", colors_ref.size());

    pal.colors_ref = colors_ref.data();
    pal.update = true;
    _perform_update = true;
    _last_used_index = id;
}

void palettes_bank::reload_colors_ref(int id)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    pal.update = true;
    _perform_update = true;
    _last_used_index = id;
}

fixed palettes_bank::inverse_intensity(int id) const
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    const palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    return pal.inverse_intensity;
}

void palettes_bank::set_inverse_intensity(int id, fixed intensity)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    pal.inverse_intensity = intensity;
    pal.update = true;
    _perform_update = true;
    _last_used_index = id;
}

fixed palettes_bank::grayscale_intensity(int id) const
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    const palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    return pal.grayscale_intensity;
}

void palettes_bank::set_grayscale_intensity(int id, fixed intensity)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    pal.grayscale_intensity = intensity;
    pal.update = true;
    _perform_update = true;
    _last_used_index = id;
}

color palettes_bank::fade_color(int id) const
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    const palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    return pal.fade_color;
}

fixed palettes_bank::fade_intensity(int id) const
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    const palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    return pal.fade_intensity;
}

void palettes_bank::set_fade(int id, color color, fixed intensity)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    pal.fade_color = color;
    pal.fade_intensity = intensity;
    pal.update = true;
    _perform_update = true;
    _last_used_index = id;
}

int palettes_bank::rotate_count(int id) const
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    const palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);

    return pal.rotate_count;
}

void palettes_bank::set_rotate_count(int id, int count)
{
    BTN_ASSERT(id >= 0 && id < hw::palettes::count(), "Invalid palette id: ", id);

    palette& pal = _palettes[size_t(id)];
    BTN_ASSERT(pal.usages, "Palette is not used: ", id);
    BTN_ASSERT(abs(count) < colors_per_palette(), "Invalid count: ", count);

    pal.rotate_count = count;
    pal.update = true;
    _perform_update = true;
    _last_used_index = id;
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
    size_t first_index = integral_limits<size_t>::max;
    size_t last_index = 0;

    if(_perform_update)
    {
        color* colors_ptr = _colors.data();
        int brightness = fixed_t<8>(_brightness).value();
        int contrast = fixed_t<8>(_contrast).value();
        int intensity = fixed_t<8>(_intensity).value();
        int inverse_intensity = (_inverse_intensity * intensity_scale()).integer();
        int grayscale_intensity = (_grayscale_intensity * intensity_scale()).integer();
        int fade_intensity = (_fade_intensity * intensity_scale()).integer();
        bool update_all = brightness || contrast || intensity || inverse_intensity || grayscale_intensity ||
                fade_intensity;
        int pal_colors_count = colors_per_palette();
        _perform_update = false;

        for(size_t index = 0, limit = _palettes.size(); index < limit; ++index)
        {
            palette& pal = _palettes[index];

            if(pal.update || (update_all && pal.usages))
            {
                pal.update = false;
                first_index = min(first_index, index);
                last_index = max(last_index, index);

                color* pal_colors_ptr = colors_ptr + (int(index) * pal_colors_count);
                memcpy16(pal_colors_ptr, pal.colors_ref, pal_colors_count);

                if(int pal_inverse_intensity = (pal.inverse_intensity * intensity_scale()).integer())
                {
                    hw::palettes::inverse(pal_inverse_intensity, pal_colors_count, pal_colors_ptr);
                }

                if(int pal_grayscale_intensity = (pal.grayscale_intensity * intensity_scale()).integer())
                {
                    hw::palettes::grayscale(pal_grayscale_intensity, pal_colors_count, pal_colors_ptr);
                }

                if(int pal_fade_intensity = (pal.fade_intensity * intensity_scale()).integer())
                {
                    hw::palettes::fade(pal.fade_color, pal_fade_intensity, pal_colors_count, pal_colors_ptr);
                }

                if(pal.rotate_count)
                {
                    hw::palettes::rotate(pal.rotate_count, pal_colors_count, pal_colors_ptr);
                }
            }
        }

        if(_transparent_color)
        {
            colors_ptr[0] = *_transparent_color;
            first_index = 0;
        }

        if(update_all && first_index != integral_limits<size_t>::max)
        {
            color* all_colors_ptr = colors_ptr + (int(first_index) * pal_colors_count);
            int all_colors_count = (int(last_index) - int(first_index) + 1) * pal_colors_count;

            if(brightness)
            {
                hw::palettes::brightness(brightness, all_colors_count, all_colors_ptr);
            }

            if(contrast)
            {
                hw::palettes::contrast(contrast, all_colors_count, all_colors_ptr);
            }

            if(intensity)
            {
                hw::palettes::intensity(intensity, all_colors_count, all_colors_ptr);
            }

            if(inverse_intensity)
            {
                hw::palettes::inverse(inverse_intensity, all_colors_count, all_colors_ptr);
            }

            if(grayscale_intensity)
            {
                hw::palettes::grayscale(grayscale_intensity, all_colors_count, all_colors_ptr);
            }

            if(fade_intensity)
            {
                hw::palettes::fade(_fade_color, fade_intensity, all_colors_count, all_colors_ptr);
            }
        }
    }

    if(first_index == integral_limits<size_t>::max)
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
        int colors_offset = *_first_index_to_commit * colors_per_palette();
        int colors_count = (*_last_index_to_commit - *_first_index_to_commit + 1) * colors_per_palette();
        result = commit_data{ _colors.data(), colors_offset, colors_count };
        _first_index_to_commit.reset();
        _last_index_to_commit.reset();
    }

    return result;
}

}
