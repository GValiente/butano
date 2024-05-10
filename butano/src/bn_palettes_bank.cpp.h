/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_palettes_bank.h"

#include "bn_math.h"
#include "bn_limits.h"
#include "bn_display.h"
#include "bn_bpp_mode.h"
#include "bn_algorithm.h"
#include "bn_compression_type.h"
#include "../hw/include/bn_hw_decompress.h"

#if BN_CFG_LOG_ENABLED
    #include "bn_log.h"
#endif

namespace bn
{

namespace
{
    void copy_colors(const color* source, int count, color* destination)
    {
        auto int_source = reinterpret_cast<const unsigned*>(source);
        auto int_destination = reinterpret_cast<unsigned*>(destination);
        hw::memory::copy_words(int_source, count / 2, int_destination);
    }
}

uint16_t palettes_bank::colors_hash(const span<const color>& colors)
{
    const color* colors_data = colors.data();
    BN_ASSERT(aligned<4>(colors_data), "Colors are not aligned");

    auto u32_colors = reinterpret_cast<const unsigned*>(colors_data);
    auto result = u32_colors[0] + u32_colors[1] + u32_colors[2];

    // Active palettes hash > 0:
    result |= unsigned(colors.size());

    return uint16_t(result);
}

int palettes_bank::used_colors_count() const
{
    int result = 0;

    for(const palette& pal : _palettes)
    {
        if(pal.usages)
        {
            result += pal.slots_count;
        }
    }

    return result * hw::palettes::colors_per_palette();
}

#if BN_CFG_LOG_ENABLED
    void palettes_bank::log_status() const
    {
        BN_LOG("palettes: ", used_colors_count() / hw::palettes::colors_per_palette());
        BN_LOG('[');

        for(const palette& pal : _palettes)
        {
            if(pal.usages)
            {
                BN_LOG(pal.bpp_8 ? "bpp_8" : "bpp_4",
                       " - slot_index: ", &pal - &_palettes[0],
                       " - slots_count: ", pal.slots_count,
                       " - colors_count: ", pal.slots_count * hw::palettes::colors_per_palette(),
                       " - usages: ", pal.usages);
            }
        }

        BN_LOG(']');
    }
#endif

int palettes_bank::find_bpp_4(const span<const color>& colors, uint16_t hash)
{
    auto bpp_4_indexes_map_it = _bpp_4_indexes_map.find(hash);

    if(bpp_4_indexes_map_it != _bpp_4_indexes_map.end())
    {
        int index = bpp_4_indexes_map_it->second;

        if(_same_colors(colors, index))
        {
            palette& pal = _palettes[index];
            ++pal.usages;
            return index;
        }
    }

    for(int index = hw::palettes::count() - 1, limit = _bpp_8_slots_count(); index >= limit; --index)
    {
        palette& pal = _palettes[index];

        // Active palettes hash > 0:
        if(hash == pal.hash && _same_colors(colors, index))
        {
            ++pal.usages;
            return index;
        }
    }

    return -1;
}

int palettes_bank::find_bpp_8(const span<const color>& colors)
{
    int bpp_8_slots_count = _bpp_8_slots_count();
    int slots_count = colors.size() / hw::palettes::colors_per_palette();

    if(bpp_8_slots_count >= slots_count)
    {
        ++_palettes[0].usages;
        return 0;
    }

    return -1;
}

int palettes_bank::create_bpp_4(const span<const color>& colors, uint16_t hash, bool required)
{
    int colors_count = colors.size();
    int required_slots_count = colors_count / hw::palettes::colors_per_palette();
    int bpp_8_slots_count = _bpp_8_slots_count();
    int free_slots_count = 0;

    for(int index = hw::palettes::count() - 1; index >= bpp_8_slots_count; --index)
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
                pal.usages = 1;
                pal.hash = hash;
                pal.slots_count = int8_t(required_slots_count);
                pal.rotate_range_size = int8_t(colors_count - 1);

                for(int slot = 0; slot < required_slots_count; ++slot)
                {
                    _palettes[index + slot].locked = true;
                }

                _set_colors_bpp_impl(index, colors);
                _bpp_4_indexes_map.insert_or_assign(hash, int16_t(index));
                return index;
            }
        }
    }

    if(required)
    {
        #if BN_CFG_LOG_ENABLED
            log_status();

            BN_ERROR("BPP4 palette create failed. Colors count: ", colors_count,
                     "\n\nThere's no more available colors.",
                     "\nPalettes manager status has been logged.");
        #else
            BN_ERROR("BPP4 palette create failed. Colors count: ", colors_count,
                     "\n\nThere's no more available colors.");
        #endif
    }

    return -1;
}

int palettes_bank::create_bpp_8(const span<const color>& colors, compression_type compression, bool required)
{
    palette& first_pal = _palettes[0];
    int colors_count = colors.size();
    int required_slots_count = colors_count / hw::palettes::colors_per_palette();
    int bpp_8_slots_count = _bpp_8_slots_count();

    if(! first_pal.usages || first_pal.bpp_8)
    {
        if(bpp_8_slots_count >= required_slots_count)
        {
            ++first_pal.usages;
            return 0;
        }

        if(required_slots_count <= _first_bpp_4_palette_index())
        {
            if(first_pal.usages)
            {
                ++first_pal.usages;
            }
            else
            {
                first_pal.usages = 1;
                first_pal.bpp_8 = true;
            }

            first_pal.slots_count = int8_t(required_slots_count);
            first_pal.rotate_range_size = int8_t(colors_count - 1);

            for(int slot = 0; slot < required_slots_count; ++slot)
            {
                _palettes[slot].locked = true;
            }

            alignas(int) color dest_colors_array[hw::palettes::colors()];
            span<const color> dest_colors_span;

            switch(compression)
            {

            case compression_type::NONE:
                BN_ASSERT(aligned<4>(colors.data()), "Colors are not aligned");

                dest_colors_span = colors;
                break;

            case compression_type::LZ77:
                hw::decompress::lz77(colors.data(), dest_colors_array);
                dest_colors_span = span<const color>(dest_colors_array, colors_count);
                break;

            case compression_type::RUN_LENGTH:
                hw::decompress::rl_wram(colors.data(), dest_colors_array);
                dest_colors_span = span<const color>(dest_colors_array, colors_count);
                break;

            case compression_type::HUFFMAN:
                hw::decompress::huff(colors.data(), dest_colors_array);
                dest_colors_span = span<const color>(dest_colors_array, colors_count);
                break;

            default:
                BN_ERROR("Unknown compression type: ", int(compression));
                break;
            }

            _set_colors_bpp_impl(0, dest_colors_span);
            return 0;
        }
    }

    if(required)
    {
        #if BN_CFG_LOG_ENABLED
            log_status();

            BN_ERROR("BPP8 palette create failed. Colors count: ", colors_count,
                     "\n\nThere's no more available colors.",
                     "\nPalettes manager status has been logged.");
        #else
            BN_ERROR("BPP8 palette create failed. Colors count: ", colors_count,
                     "\n\nThere's no more available colors.");
        #endif
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

    if(! pal.usages) [[unlikely]]
    {
        for(int slot = 0, slots_count = pal.slots_count; slot < slots_count; ++slot)
        {
            _palettes[id + slot].locked = false;
        }

        if(! pal.bpp_8)
        {
            _bpp_4_indexes_map.erase(pal.hash);
        }

        pal = palette();
    }
}

bpp_mode palettes_bank::bpp(int id) const
{
    return bpp_mode(_palettes[id].bpp_8);
}

span<const color> palettes_bank::colors(int id) const
{
    int colors_per_palette = hw::palettes::colors_per_palette();
    const color* colors_data = _initial_colors + (id * colors_per_palette);
    int colors_count = colors_per_palette * _palettes[id].slots_count;
    return span<const color>(colors_data, colors_count);
}

void palettes_bank::set_colors(int id, const span<const color>& colors)
{
    BN_BASIC_ASSERT(colors.size() == colors_count(id),
                    "Colors count mismatch: ", colors.size(), " - ", colors_count(id));

    palette& pal = _palettes[id];

    if(pal.bpp_8)
    {
        BN_ASSERT(aligned<4>(colors.data()), "Colors are not aligned");
    }
    else
    {
        uint16_t old_hash = pal.hash;
        uint16_t new_hash = colors_hash(colors);

        if(old_hash != new_hash)
        {
            _bpp_4_indexes_map.erase(old_hash);
            _bpp_4_indexes_map.insert_or_assign(new_hash, int16_t(id));
            pal.hash = new_hash;
        }
    }

    _set_colors_bpp_impl(id, colors);
}

void palettes_bank::set_inverted(int id, bool inverted)
{
    palette& pal = _palettes[id];

    if(pal.inverted != inverted)
    {
        pal.inverted = inverted;
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_grayscale_intensity(int id, fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[id];
    bool update = fixed_t<5>(pal.grayscale_intensity) != fixed_t<5>(intensity);
    pal.grayscale_intensity = intensity;

    if(update)
    {
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_hue_shift_intensity(int id, fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[id];
    bool update = fixed_t<5>(pal.hue_shift_intensity) != fixed_t<5>(intensity);
    pal.hue_shift_intensity = intensity;

    if(update)
    {
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_fade_color(int id, color color)
{
    palette& pal = _palettes[id];
    bool update = pal.fade_color != color && fixed_t<5>(pal.fade_intensity).data();
    pal.fade_color = color;

    if(update)
    {
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_fade_intensity(int id, fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[id];
    bool update = fixed_t<5>(pal.fade_intensity) != fixed_t<5>(intensity);
    pal.fade_intensity = intensity;

    if(update)
    {
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_fade(int id, color color, fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    palette& pal = _palettes[id];
    bool update = pal.fade_color != color || fixed_t<5>(pal.fade_intensity) != fixed_t<5>(intensity);
    pal.fade_color = color;
    pal.fade_intensity = intensity;

    if(update)
    {
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_rotate_count(int id, int count)
{
    palette& pal = _palettes[id];

    if(pal.rotate_count != count)
    {
        BN_ASSERT(abs(count) < pal.rotate_range_size, "Invalid count: ", count, " - ", pal.rotate_range_size);

        pal.rotate_count = int16_t(count);
        pal.update = true;
        _update = true;
    }
}

void palettes_bank::set_rotate_range(int id, int start, int size)
{
    palette& pal = _palettes[id];

    if(pal.rotate_range_start != start || pal.rotate_range_size != size)
    {
        BN_ASSERT(start >= 0 && start <= colors_count(id) - 2, "Invalid start: ", start, " - ", colors_count(id));
        BN_ASSERT(size >= 2 && size <= colors_count(id), "Invalid size: ", size, " - ", colors_count(id));
        BN_ASSERT(start + size <= colors_count(id), "Invalid range: ", start, " - ", size, " - ", colors_count(id));

        int count = pal.rotate_count;
        pal.rotate_range_start = int8_t(start);
        pal.rotate_range_size = int8_t(size);

        if(count)
        {
            BN_ASSERT(abs(count) < size, "Invalid count: ", count, " - ", size);

            pal.update = true;
            _update = true;
        }
    }
}

void palettes_bank::reload(int id)
{
    palette& pal = _palettes[id];
    pal.update = true;
    _update = true;
}

void palettes_bank::set_transparent_color(const optional<color>& transparent_color)
{
    bool update = _transparent_color != transparent_color;
    _transparent_color = transparent_color;
    _update |= update;
}

void palettes_bank::set_brightness(fixed brightness)
{
    BN_ASSERT(brightness >= 0 && brightness <= 1, "Invalid brightness: ", brightness);

    fixed_t<5> output_brightness(brightness);
    bool update = fixed_t<5>(_brightness) != output_brightness;
    _brightness = brightness;

    if(update)
    {
        _on_global_effect_updated(output_brightness.data());
    }
}

void palettes_bank::set_contrast(fixed contrast)
{
    BN_ASSERT(contrast >= 0 && contrast <= 1, "Invalid contrast: ", contrast);

    fixed_t<5> output_contrast(contrast);
    bool update = fixed_t<5>(_contrast) != output_contrast;
    _contrast = contrast;

    if(update)
    {
        _on_global_effect_updated(output_contrast.data());
    }
}

void palettes_bank::set_intensity(fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    fixed_t<5> output_intensity(intensity);
    bool update = fixed_t<5>(_intensity) != output_intensity;
    _intensity = intensity;

    if(update)
    {
        _on_global_effect_updated(output_intensity.data());
    }
}

void palettes_bank::set_inverted(bool inverted)
{
    bool update = _inverted != inverted;
    _inverted = inverted;

    if(update)
    {
        _on_global_effect_updated(inverted);
    }
}

void palettes_bank::set_grayscale_intensity(fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    fixed_t<5> output_intensity(intensity);
    bool update = fixed_t<5>(_grayscale_intensity) != output_intensity;
    _grayscale_intensity = intensity;

    if(update)
    {
        _on_global_effect_updated(output_intensity.data());
    }
}

void palettes_bank::set_hue_shift_intensity(fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    fixed_t<5> output_intensity(intensity);
    bool update = fixed_t<5>(_hue_shift_intensity) != output_intensity;
    _hue_shift_intensity = intensity;

    if(update)
    {
        _on_global_effect_updated(output_intensity.data());
    }
}

void palettes_bank::set_fade_color(color color)
{
    bool update = _fade_color != color && fixed_t<5>(_fade_intensity).data();
    _fade_color = color;

    if(update)
    {
        _update = true;
        _update_global_effects = true;
    }
}

void palettes_bank::set_fade_intensity(fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    fixed_t<5> output_intensity(intensity);
    bool update = fixed_t<5>(_fade_intensity) != output_intensity;
    _fade_intensity = intensity;

    if(update)
    {
        _on_global_effect_updated(output_intensity.data());
    }
}

void palettes_bank::set_fade(color color, fixed intensity)
{
    BN_ASSERT(intensity >= 0 && intensity <= 1, "Invalid intensity: ", intensity);

    fixed_t<5> output_intensity(intensity);
    bool update = _fade_color != color || fixed_t<5>(_fade_intensity) != output_intensity;
    _fade_color = color;
    _fade_intensity = intensity;

    if(update)
    {
        _on_global_effect_updated(output_intensity.data());
    }
}

void palettes_bank::update()
{
    int first_index = numeric_limits<int>::max();
    int last_index = 0;

    if(_update)
    {
        bool update_global_effects = _update_global_effects || _global_effects_enabled;
        _update = false;
        _global_effects_updated = _update_global_effects;
        _update_global_effects = false;

        if(update_global_effects)
        {
            for(int index = 0, limit = hw::palettes::count(); index < limit; )
            {
                const palette& pal = _palettes[index];

                if(pal.usages)
                {
                    _update_palette(index);
                    first_index = min(first_index, index);
                    last_index = index;
                }

                index += pal.slots_count;
            }
        }
        else
        {
            for(int index = 0, limit = hw::palettes::count(); index < limit; )
            {
                const palette& pal = _palettes[index];

                if(pal.update)
                {
                    _update_palette(index);
                    first_index = min(first_index, index);
                    last_index = index;
                }

                index += pal.slots_count;
            }
        }

        if(const color* transparent_color = _transparent_color.get())
        {
            _final_colors[0] = *transparent_color;
            first_index = 0;
        }

        if(_global_effects_enabled && first_index != numeric_limits<int>::max())
        {
            color* all_colors_ptr = _final_colors + (first_index * hw::palettes::colors_per_palette());
            int all_colors_count = (last_index - first_index + _palettes[last_index].slots_count) *
                    hw::palettes::colors_per_palette();
            _apply_global_effects(all_colors_count, all_colors_ptr);
        }
    }

    _first_index_to_commit = first_index;
    _last_index_to_commit = last_index;
}

palettes_bank::commit_data palettes_bank::retrieve_commit_data() const
{
    commit_data result;

    if(_first_index_to_commit != numeric_limits<int>::max())
    {
        int first_index = _first_index_to_commit;
        int last_index = _last_index_to_commit;
        int colors_offset = first_index * hw::palettes::colors_per_palette();
        int colors_count = (last_index - first_index + _palettes[last_index].slots_count) *
                hw::palettes::colors_per_palette();
        result = { _final_colors, colors_offset, colors_count };
    }
    else
    {
        result.colors_ptr = nullptr;
        result.offset = 0;
        result.count = 0;
    }

    return result;
}

void palettes_bank::reset_commit_data()
{
    _first_index_to_commit = numeric_limits<int>::max();
    _last_index_to_commit = 0;
    _global_effects_updated = false;
}

void palettes_bank::fill_hblank_effect_colors(int id, const color* source_colors_ptr, uint16_t* dest_ptr) const
{
    const palette& pal = _palettes[id];
    int dest_colors_count = display::height();
    auto dest_colors_ptr = reinterpret_cast<color*>(dest_ptr);
    copy_colors(source_colors_ptr, dest_colors_count, dest_colors_ptr);
    pal.apply_effects(dest_colors_count, dest_colors_ptr);

    if(_global_effects_enabled)
    {
        _apply_global_effects(dest_colors_count, dest_colors_ptr);
    }
}

void palettes_bank::fill_hblank_effect_colors(const color* source_colors_ptr, uint16_t* dest_ptr) const
{
    int dest_colors_count = display::height();
    auto dest_colors_ptr = reinterpret_cast<color*>(dest_ptr);
    copy_colors(source_colors_ptr, dest_colors_count, dest_colors_ptr);

    if(_global_effects_enabled)
    {
        _apply_global_effects(dest_colors_count, dest_colors_ptr);
    }
}

[[nodiscard]] bool palettes_bank::_same_colors(const span<const color>& colors, int id) const
{
    int colors_per_palette = hw::palettes::colors_per_palette();
    int slots_count = _palettes[id].slots_count;
    int stored_colors_count = colors_per_palette * slots_count;

    if(colors.size() != stored_colors_count)
    {
        return false;
    }

    unsigned four_words_count = unsigned(stored_colors_count) / 8;
    auto u32_colors = reinterpret_cast<const unsigned*>(colors.data());
    auto u32_stored_colors = reinterpret_cast<const unsigned*>(_initial_colors + (id * colors_per_palette));
    return bn_hw_palettes_different_words(four_words_count, u32_colors, u32_stored_colors) == 0;
}

int palettes_bank::_bpp_8_slots_count() const
{
    const palette& first_pal = _palettes[0];

    if(first_pal.usages && first_pal.bpp_8)
    {
        return first_pal.slots_count;
    }

    return 0;
}

int palettes_bank::_first_bpp_4_palette_index() const
{
    for(int index = 0; index < hw::palettes::count(); )
    {
        const palette& pal = _palettes[index];

        if(pal.usages && ! pal.bpp_8)
        {
            return index;
        }

        index += pal.slots_count;
    }

    return hw::palettes::count();
}

void palettes_bank::_on_global_effect_updated(bool active)
{
    _update = true;
    _update_global_effects = true;

    _global_effects_enabled = active || _inverted || fixed_t<5>(_brightness).data() ||
            fixed_t<5>(_contrast).data() || fixed_t<5>(_intensity).data() ||
            fixed_t<5>(_grayscale_intensity).data() || fixed_t<5>(_hue_shift_intensity).data() ||
            fixed_t<5>(_fade_intensity).data();
}

void palettes_bank::_set_colors_bpp_impl(int id, const span<const color>& colors)
{
    palette& pal = _palettes[id];
    copy_colors(colors.data(), colors.size(), _initial_colors + (id * hw::palettes::colors_per_palette()));
    pal.update = true;
    _update = true;
}

void palettes_bank::_update_palette(int id)
{
    palette& pal = _palettes[id];
    const color* initial_pal_colors_ptr = _initial_colors + (id * hw::palettes::colors_per_palette());
    color* final_pal_colors_ptr = _final_colors + (id * hw::palettes::colors_per_palette());
    int pal_colors_count = pal.slots_count * hw::palettes::colors_per_palette();
    copy_colors(initial_pal_colors_ptr, pal_colors_count, final_pal_colors_ptr);
    pal.apply_effects(pal_colors_count, final_pal_colors_ptr);

    if(int rotate_count = pal.rotate_count)
    {
        auto unsigned_final_pal_colors_ptr = reinterpret_cast<const unsigned*>(final_pal_colors_ptr);
        unsigned unsigned_temp_buffer[hw::palettes::colors() / 2];
        hw::memory::copy_words(unsigned_final_pal_colors_ptr, pal_colors_count / 2, unsigned_temp_buffer);

        auto color_temp_buffer_ptr = reinterpret_cast<const color*>(unsigned_temp_buffer);
        int first_index = pal.rotate_range_start;
        hw::palettes::rotate(color_temp_buffer_ptr + first_index, rotate_count, pal.rotate_range_size,
                             final_pal_colors_ptr + first_index);
    }
}

void palettes_bank::_apply_global_effects(int dest_colors_count, color* dest_colors_ptr) const
{
    if(int brightness = fixed_t<5>(_brightness).data())
    {
        hw::palettes::brightness(dest_colors_ptr, brightness, dest_colors_count, dest_colors_ptr);
    }

    if(int contrast = fixed_t<5>(_contrast).data())
    {
        hw::palettes::contrast(dest_colors_ptr, contrast, dest_colors_count, dest_colors_ptr);
    }

    if(int intensity = fixed_t<5>(_intensity).data())
    {
        hw::palettes::intensity(dest_colors_ptr, intensity, dest_colors_count, dest_colors_ptr);
    }

    if(int hue_shift_intensity = fixed_t<5>(_hue_shift_intensity).data())
    {
        hw::palettes::hue_shift(dest_colors_ptr, hue_shift_intensity, dest_colors_count, dest_colors_ptr);
    }

    if(_inverted)
    {
        hw::palettes::aligned_invert(dest_colors_ptr, dest_colors_count, dest_colors_ptr);
    }

    if(int grayscale_intensity = fixed_t<5>(_grayscale_intensity).data())
    {
        hw::palettes::grayscale(dest_colors_ptr, grayscale_intensity, dest_colors_count, dest_colors_ptr);
    }

    if(int fade_intensity = fixed_t<5>(_fade_intensity).data())
    {
        hw::palettes::fade(dest_colors_ptr, _fade_color, fade_intensity, dest_colors_count, dest_colors_ptr);
    }
}

void palettes_bank::palette::apply_effects(int dest_colors_count, color* dest_colors_ptr) const
{
    if(int pal_hue_shift_intensity = fixed_t<5>(hue_shift_intensity).data())
    {
        hw::palettes::hue_shift(dest_colors_ptr, pal_hue_shift_intensity, dest_colors_count, dest_colors_ptr);
    }

    if(inverted)
    {
        hw::palettes::aligned_invert(dest_colors_ptr, dest_colors_count, dest_colors_ptr);
    }

    if(int pal_grayscale_intensity = fixed_t<5>(grayscale_intensity).data())
    {
        hw::palettes::grayscale(dest_colors_ptr, pal_grayscale_intensity, dest_colors_count, dest_colors_ptr);
    }

    if(int pal_fade_intensity = fixed_t<5>(fade_intensity).data())
    {
        hw::palettes::fade(dest_colors_ptr, fade_color, pal_fade_intensity, dest_colors_count, dest_colors_ptr);
    }
}

}
