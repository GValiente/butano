#include "../include/btn_hw_palettes.h"

#include "tonc.h"
#include "btn_math.h"
#include "btn_fixed.h"
#include "btn_color.h"
#include "../include/btn_hw_memory.h"

namespace btn::hw::palettes
{

namespace
{
    static_assert(sizeof(color) == sizeof(COLOR));
    static_assert(alignof(color) == alignof(COLOR));

    void commit(const color* colors_ptr, int offset, int count, COLOR* destination)
    {
        BTN_ASSERT(colors_ptr, "Colors pointer is null");
        BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BTN_ASSERT(count >= 0, "Invalid count: ", count);
        BTN_ASSERT(offset + count <= colors(), "Too much colors: ", offset, " - ", count);
        BTN_ASSERT(destination, "Destination pointer is null");

        hw::memcpy16(destination + offset, colors_ptr + offset, count);
    }
}

void brightness(int value, int count, color* colors_ptr)
{
    BTN_ASSERT(value >= fixed_t<8>(-1).value() && value <= fixed_t<8>(1).value(), "Invalid value: ", value);
    BTN_ASSERT(count >= 0, "Invalid count: ", count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    clr_adj_brightness(output_colors, output_colors, unsigned(count), value);
}

void contrast(int value, int count, color* colors_ptr)
{
    BTN_ASSERT(value >= fixed_t<8>(-1).value() && value <= fixed_t<8>(1).value(), "Invalid value: ", value);
    BTN_ASSERT(count >= 0, "Invalid count: ", count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    clr_adj_contrast(output_colors, output_colors, unsigned(count), value);
}

void intensity(int value, int count, color* colors_ptr)
{
    BTN_ASSERT(value >= fixed_t<8>(-1).value() && value <= fixed_t<8>(1).value(), "Invalid value: ", value);
    BTN_ASSERT(count >= 0, "Invalid count: ", count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    clr_adj_intensity(output_colors, output_colors, unsigned(count), value);
}

void inverse(int intensity, int count, color* colors_ptr)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 32, "Invalid intensity: ", intensity);
    BTN_ASSERT(count >= 0 && count <= colors(), "Invalid count: ", count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    COLOR temp_palette[colors()];

    for(int index = 0; index < count; ++index)
    {
        temp_palette[index] = 32767 ^ output_colors[index];
    }

    clr_blend_fast(output_colors, temp_palette, output_colors, unsigned(count), unsigned(intensity));
}

void grayscale(int intensity, int count, color* colors_ptr)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 32, "Invalid intensity: ", intensity);
    BTN_ASSERT(count >= 0 && count <= colors(), "Invalid count: ", count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    COLOR temp_palette[colors()];
    clr_grayscale(temp_palette, output_colors, unsigned(count));
    clr_blend_fast(output_colors, temp_palette, output_colors, unsigned(count), unsigned(intensity));
}

void fade(color fade_color, int intensity, int count, color* colors_ptr)
{
    BTN_ASSERT(intensity >= 0 && intensity <= 32, "Invalid intensity: ", intensity);
    BTN_ASSERT(count >= 0, "Invalid count: ", count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    clr_fade_fast(output_colors, uint16_t(fade_color.value()), output_colors, unsigned(count), unsigned(intensity));
}

void rotate(int rotate_count, int colors_count, color* colors_ptr)
{
    BTN_ASSERT(abs(rotate_count) < colors_count, "Invalid rotate count: ", rotate_count);
    BTN_ASSERT(colors_count >= 0, "Invalid colors count: ", colors_count);
    BTN_ASSERT(colors_ptr, "Colors pointer is null");

    auto output_colors = reinterpret_cast<COLOR*>(colors_ptr);
    clr_rotate(output_colors, unsigned(colors_count), rotate_count);
}

void commit_sprites(const color* colors_ptr, int offset, int count)
{
    commit(colors_ptr, offset, count, pal_obj_mem);
}

void commit_bgs(const color* colors_ptr, int offset, int count)
{
    commit(colors_ptr, offset, count, pal_bg_mem);
}

}
