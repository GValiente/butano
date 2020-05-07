#include "btn_palettes_bank.h"

#include "btn_math.h"
#include "btn_span.h"
#include "btn_limits.h"
#include "btn_memory.h"
#include "btn_algorithm.h"
#include "btn_config_palettes.h"

namespace btn
{

namespace
{
    [[maybe_unused]] bool _valid_colors_count(const span<const color>& colors_ref)
    {
        int colors_count = colors_ref.size();
        return colors_count >= hw::palettes::colors_per_palette() &&
                colors_count <= hw::palettes::colors() &&
                colors_count % hw::palettes::colors_per_palette() == 0;
    }
}

unsigned palettes_bank::colors_ref_hash(const span<const color>& colors_ref)
{
    const color* colors_data = colors_ref.data();
    int colors_count = min(colors_ref.size(), BTN_CFG_PALETTES_MAX_HASH_COLORS);
    auto result = unsigned(colors_count);

    for(int index = 1; index < colors_count; ++index)
    {
        result += unsigned(colors_data[index].value());
    }

    return result;
}

int palettes_bank::used_count() const
{
    int result = 0;

    for(const palette& pal : _palettes)
    {
        if(pal.usages)
        {
            result += pal.slots_count;
        }
    }

    return result;
}

int palettes_bank::find_bpp_4(const span<const color>& colors_ref, unsigned hash)
{
    BTN_ASSERT(_valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());

    span<const color> visible_colors = colors_ref.subspan(1);
    int inferior_index = _last_used_4bpp_index;
    int superior_index = inferior_index + 1;
    int bpp8_slots_count = _bpp8_slots_count();
    bool valid_inferior_index = inferior_index >= bpp8_slots_count;
    bool valid_superior_index = superior_index < hw::palettes::count();

    while(valid_inferior_index || valid_superior_index)
    {
        if(valid_inferior_index)
        {
            palette& pal = _palettes[inferior_index];

            if(pal.usages && palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
            {
                if(hash == pal.hash && visible_colors == pal.visible_colors_span())
                {
                    ++pal.usages;
                    _last_used_4bpp_index = inferior_index;
                    return inferior_index;
                }
            }

            --inferior_index;
            valid_inferior_index = inferior_index >= bpp8_slots_count;
        }

        if(valid_superior_index)
        {
            palette& pal = _palettes[superior_index];

            if(pal.usages)
            {
                if(hash == pal.hash && visible_colors == pal.visible_colors_span())
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

    return -1;
}

int palettes_bank::find_bpp_8(const span<const color>& colors_ref)
{
    BTN_ASSERT(_valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());

    int bpp8_slots_count = _bpp8_slots_count();
    int slots_count = colors_ref.size() / hw::palettes::colors_per_palette();

    if(bpp8_slots_count >= slots_count)
    {
        ++_palettes[0].usages;
        return 0;
    }

    return -1;
}

int palettes_bank::create_bpp_4(const span<const color>& colors_ref, unsigned hash)
{
    BTN_ASSERT(_valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());

    int required_slots_count = colors_ref.size() / hw::palettes::colors_per_palette();
    int bpp8_slots_count = _bpp8_slots_count();
    int free_slots_count = 0;

    for(int index = hw::palettes::count() - 1; index >= bpp8_slots_count; --index)
    {
        palette& pal = _palettes[index];

        if(pal.usages || pal.locked)
        {
            free_slots_count = 0;
        }
        else
        {
            ++free_slots_count;

            if(free_slots_count == required_slots_count)
            {
                pal = palette();
                pal.usages = 1;
                pal.bpp_mode = uint8_t(palette_bpp_mode::BPP_4);
                pal.slots_count = int8_t(required_slots_count);

                for(int slot = 0; slot < required_slots_count; ++slot)
                {
                    _palettes[index + slot].locked = true;
                }

                set_colors_ref(index, colors_ref, hash);
                return index;
            }
        }
    }

    return -1;
}

int palettes_bank::create_bpp_8(const span<const color>& colors_ref)
{
    BTN_ASSERT(_valid_colors_count(colors_ref), "Invalid colors count: ", colors_ref.size());

    palette& first_pal = _palettes[0];
    int required_slots_count = colors_ref.size() / hw::palettes::colors_per_palette();
    int bpp8_slots_count = _bpp8_slots_count();

    if(! first_pal.usages || palette_bpp_mode(first_pal.bpp_mode) == palette_bpp_mode::BPP_8)
    {
        if(bpp8_slots_count >= required_slots_count)
        {
            ++first_pal.usages;
            return 0;
        }

        if(required_slots_count <= _first_4bpp_palette_index())
        {
            if(first_pal.usages)
            {
                ++first_pal.usages;
            }
            else
            {
                first_pal = palette();
                first_pal.usages = 1;
                first_pal.bpp_mode = uint8_t(palette_bpp_mode::BPP_8);
            }

            first_pal.slots_count = int8_t(required_slots_count);

            for(int slot = 0; slot < required_slots_count; ++slot)
            {
                _palettes[slot].locked = true;
            }

            set_colors_ref(0, colors_ref, 0);
            return 0;
        }
    }

    return -1;
}

void palettes_bank::increase_usages(int id)
{
    palette& pal = _palettes[id];
    ++pal.usages;
}

void palettes_bank::decrease_usages(int id)
{
    palette& pal = _palettes[id];
    --pal.usages;

    if(! pal.usages)
    {
        for(int slot = 0, slots_count = pal.slots_count; slot < slots_count; ++slot)
        {
            _palettes[id + slot].locked = false;
        }

        pal = palette();
    }
}

void palettes_bank::set_colors_ref(int id, const span<const color>& colors_ref)
{
    palette& pal = _palettes[id];
    unsigned hash;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        hash = colors_ref_hash(colors_ref);
    }
    else
    {
        hash = 0;
    }

    set_colors_ref(id, colors_ref, hash);
}

void palettes_bank::set_colors_ref(int id, const span<const color>& colors_ref, unsigned hash)
{
    BTN_ASSERT(colors_ref.size() == colors_count(id), "Colors count mismatch: ",
               colors_ref.size(), " - ", colors_count(id));

    palette& pal = _palettes[id];
    pal.colors_ref = colors_ref.data();
    pal.hash = hash;
    pal.update = true;
    _update = true;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::reload_colors_ref(int id)
{
    palette& pal = _palettes[id];
    pal.update = true;
    _update = true;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        int colors_count = pal.slots_count * hw::palettes::colors_per_palette();
        pal.hash = colors_ref_hash(span<const color>(pal.colors_ref, colors_count));
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::set_inverted(int id, bool inverted)
{
    palette& pal = _palettes[id];
    pal.inverted = inverted;
    pal.update = true;
    _update = true;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::set_grayscale_intensity(int id, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[id];
    pal.grayscale_intensity = intensity;
    pal.update = true;
    _update = true;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::set_fade(int id, color color, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[id];
    pal.fade_color = color;
    pal.fade_intensity = intensity;
    pal.update = true;
    _update = true;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::set_rotate_count(int id, int count)
{
    BTN_ASSERT(abs(count) < colors_count(id) - 1, "Invalid count: ", count, " - ", colors_count(id));

    palette& pal = _palettes[id];
    pal.rotate_count = int16_t(count);
    pal.update = true;
    _update = true;

    if(palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
    {
        _last_used_4bpp_index = id;
    }
}

void palettes_bank::set_transparent_color(const optional<color>& transparent_color)
{
    _transparent_color = transparent_color;
    _update = true;
    _update_all = true;
}

void palettes_bank::set_brightness(fixed brightness)
{
    BTN_ASSERT(brightness >= -1 && brightness <= 1, "Invalid brightness: ", brightness);

    _brightness = brightness;
    _update = true;
    _update_all = true;
}

void palettes_bank::set_contrast(fixed contrast)
{
    BTN_ASSERT(contrast >= -1 && contrast <= 1, "Invalid contrast: ", contrast);

    _contrast = contrast;
    _update = true;
    _update_all = true;
}

void palettes_bank::set_intensity(fixed intensity)
{
    BTN_ASSERT(intensity >= -1 && intensity <= 1, "Invalid intensity: ", intensity);

    _intensity = intensity;
    _update = true;
    _update_all = true;
}

void palettes_bank::set_inverted(bool inverted)
{
    _inverted = inverted;
    _update = true;
    _update_all = true;
}

void palettes_bank::set_grayscale_intensity(fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    _grayscale_intensity = intensity;
    _update = true;
    _update_all = true;
}

void palettes_bank::set_fade(color color, fixed intensity)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    _fade_color = color;
    _fade_intensity = intensity;
    _update = true;
    _update_all = true;
}

void palettes_bank::update()
{
    int first_index = numeric_limits<int>::max();
    int last_index = 0;

    if(_update)
    {
        bool update_all = _update_all;
        _update = false;
        _update_all = false;

        for(int index = 0, limit = hw::palettes::count(); index < limit; ++index)
        {
            palette& pal = _palettes[index];

            if(pal.update || (update_all && pal.usages))
            {
                int pal_colors_count = pal.slots_count * hw::palettes::colors_per_palette();
                pal.update = false;
                first_index = min(first_index, index);
                last_index = max(last_index, index);

                color* pal_colors_ptr = _colors + (index * hw::palettes::colors_per_palette());
                memory::copy(*pal.colors_ref, pal_colors_count, *pal_colors_ptr);

                if(pal.inverted)
                {
                    hw::palettes::invert(pal_colors_count, pal_colors_ptr);
                }

                if(int pal_grayscale_intensity = fixed_t<5>(pal.grayscale_intensity).value())
                {
                    hw::palettes::grayscale(pal_grayscale_intensity, pal_colors_count, pal_colors_ptr);
                }

                if(int pal_fade_intensity = fixed_t<5>(pal.fade_intensity).value())
                {
                    hw::palettes::fade(pal.fade_color, pal_fade_intensity, pal_colors_count, pal_colors_ptr);
                }

                if(pal.rotate_count)
                {
                    hw::palettes::rotate(pal.rotate_count, pal_colors_count - 1, pal_colors_ptr + 1);
                }
            }
        }

        if(_transparent_color)
        {
            _colors[0] = *_transparent_color;
            first_index = 0;
        }

        if(update_all && first_index != numeric_limits<int>::max())
        {
            color* all_colors_ptr = _colors + (first_index * hw::palettes::colors_per_palette());
            int all_colors_count = (last_index - first_index + max(int(_palettes[last_index].slots_count), 1)) *
                    hw::palettes::colors_per_palette();

            if(int brightness = fixed_t<8>(_brightness).value())
            {
                hw::palettes::brightness(brightness, all_colors_count, all_colors_ptr);
            }

            if(int contrast = fixed_t<8>(_contrast).value())
            {
                hw::palettes::contrast(contrast, all_colors_count, all_colors_ptr);
            }

            if(int intensity = fixed_t<8>(_intensity).value())
            {
                hw::palettes::intensity(intensity, all_colors_count, all_colors_ptr);
            }

            if(_inverted)
            {
                hw::palettes::invert(all_colors_count, all_colors_ptr);
            }

            if(int grayscale_intensity = fixed_t<5>(_grayscale_intensity).value())
            {
                hw::palettes::grayscale(grayscale_intensity, all_colors_count, all_colors_ptr);
            }

            if(int fade_intensity = fixed_t<5>(_fade_intensity).value())
            {
                hw::palettes::fade(_fade_color, fade_intensity, all_colors_count, all_colors_ptr);
            }
        }
    }

    if(first_index == numeric_limits<int>::max())
    {
        _first_index_to_commit.reset();
        _last_index_to_commit.reset();
    }
    else
    {
        _first_index_to_commit = first_index;
        _last_index_to_commit = last_index;
    }
}

optional<palettes_bank::commit_data> palettes_bank::retrieve_commit_data()
{
    optional<commit_data> result;

    if(_first_index_to_commit)
    {
        int first_index = *_first_index_to_commit;
        int last_index = *_last_index_to_commit;
        int colors_offset = first_index * hw::palettes::colors_per_palette();
        int colors_count = (last_index - first_index + max(int(_palettes[last_index].slots_count), 1)) *
                hw::palettes::colors_per_palette();
        result = commit_data{ _colors, colors_offset, colors_count };
        _first_index_to_commit.reset();
        _last_index_to_commit.reset();
    }

    return result;
}

int palettes_bank::_bpp8_slots_count() const
{
    const palette& first_pal = _palettes[0];

    if(first_pal.usages && palette_bpp_mode(first_pal.bpp_mode) == palette_bpp_mode::BPP_8)
    {
        return first_pal.slots_count;
    }

    return 0;
}

int palettes_bank::_first_4bpp_palette_index() const
{
    for(int index = 0; index < hw::palettes::count(); ++index)
    {
        const palette& pal = _palettes[index];

        if(pal.usages && palette_bpp_mode(pal.bpp_mode) == palette_bpp_mode::BPP_4)
        {
            return index;
        }
    }

    return hw::palettes::count();
}

}
