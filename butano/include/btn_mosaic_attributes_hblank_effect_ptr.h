#ifndef BTN_MOSAIC_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_MOSAIC_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class mosaic_attributes;

class mosaic_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static mosaic_attributes_hblank_effect_ptr create(
            const span<const mosaic_attributes>& attributes_ref);

    [[nodiscard]] static optional<mosaic_attributes_hblank_effect_ptr> create_optional(
            const span<const mosaic_attributes>& attributes_ref);

    mosaic_attributes_hblank_effect_ptr(const mosaic_attributes_hblank_effect_ptr& other) = default;

    mosaic_attributes_hblank_effect_ptr& operator=(const mosaic_attributes_hblank_effect_ptr& other) = default;

    mosaic_attributes_hblank_effect_ptr(mosaic_attributes_hblank_effect_ptr&& other) = default;

    mosaic_attributes_hblank_effect_ptr& operator=(mosaic_attributes_hblank_effect_ptr&& other) = default;

    [[nodiscard]] span<const mosaic_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const mosaic_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(mosaic_attributes_hblank_effect_ptr& other)
    {
        hblank_effect_ptr::swap(other);
    }

    friend void swap(mosaic_attributes_hblank_effect_ptr& a, mosaic_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    mosaic_attributes_hblank_effect_ptr(int id);
};

}

#endif
