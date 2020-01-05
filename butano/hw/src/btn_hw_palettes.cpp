#include "../include/btn_hw_palettes.h"

#include "tonc.h"
#include "btn_color.h"
#include "btn_memory.h"

namespace btn::hw::palettes
{

namespace
{
    static_assert(sizeof(color) == sizeof(COLOR));
    static_assert(alignof(color) == alignof(COLOR));

    void commit(const color& source_colors_ref, int offset, int count, color& destination_colors_ref)
    {
        memory::copy((&source_colors_ref)[offset], count, (&destination_colors_ref)[offset]);
    }
}

void brightness(int value, int count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    clr_adj_brightness(colors_ptr, colors_ptr, unsigned(count), value);
}

void contrast(int value, int count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    clr_adj_contrast(colors_ptr, colors_ptr, unsigned(count), value);
}

void intensity(int value, int count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    clr_adj_intensity(colors_ptr, colors_ptr, unsigned(count), value);
}

void inverse(int intensity, int count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    COLOR temp_colors[colors()];

    for(int index = 0; index < count; ++index)
    {
        temp_colors[index] = 32767 ^ colors_ptr[index];
    }

    clr_blend_fast(colors_ptr, temp_colors, colors_ptr, unsigned(count), unsigned(intensity));
}

void grayscale(int intensity, int count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    COLOR temp_colors[colors()];
    clr_grayscale(temp_colors, colors_ptr, unsigned(count));
    clr_blend_fast(colors_ptr, temp_colors, colors_ptr, unsigned(count), unsigned(intensity));
}

void fade(color fade_color, int intensity, int count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    clr_fade_fast(colors_ptr, uint16_t(fade_color.value()), colors_ptr, unsigned(count), unsigned(intensity));
}

void rotate(int rotate_count, int colors_count, color& colors_ref)
{
    auto colors_ptr = reinterpret_cast<COLOR*>(&colors_ref);
    clr_rotate(colors_ptr, unsigned(colors_count), rotate_count);
}

void commit_sprites(const color& colors_ref, int offset, int count)
{
    commit(colors_ref, offset, count, *reinterpret_cast<color*>(MEM_PAL_OBJ));
}

void commit_bgs(const color& colors_ref, int offset, int count)
{
    commit(colors_ref, offset, count, *reinterpret_cast<color*>(MEM_PAL_BG));
}

}
