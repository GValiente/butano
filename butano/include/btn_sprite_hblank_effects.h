#ifndef BTN_SPRITE_HBLANK_EFFECTS_H
#define BTN_SPRITE_HBLANK_EFFECTS_H

#include "btn_sprite_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class third_sprite_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static third_sprite_attributes_hblank_effect_ptr create(
            sprite_ptr sprite_ptr, const span<const third_sprite_attributes>& attributes_ref);

    [[nodiscard]] static optional<third_sprite_attributes_hblank_effect_ptr> optional_create(
            sprite_ptr sprite_ptr, const span<const third_sprite_attributes>& attributes_ref);

    third_sprite_attributes_hblank_effect_ptr(const third_sprite_attributes_hblank_effect_ptr& other);

    third_sprite_attributes_hblank_effect_ptr& operator=(const third_sprite_attributes_hblank_effect_ptr& other);

    third_sprite_attributes_hblank_effect_ptr(third_sprite_attributes_hblank_effect_ptr&& other);

    third_sprite_attributes_hblank_effect_ptr& operator=(third_sprite_attributes_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite_ptr;
    }

    [[nodiscard]] span<const third_sprite_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const third_sprite_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(third_sprite_attributes_hblank_effect_ptr& other);

    friend void swap(third_sprite_attributes_hblank_effect_ptr& a, third_sprite_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite_ptr;

    third_sprite_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr);
};

}

#endif
