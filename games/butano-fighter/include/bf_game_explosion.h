#ifndef BF_GAME_EXPLOSION_H
#define BF_GAME_EXPLOSION_H

#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace bf::game
{

class explosion
{

public:
    explosion(const btn::sprite_item& sprite_item, const btn::fixed_point& position, int animation_frames,
              int z_order, bool double_size, const btn::camera_ptr& camera);

    [[nodiscard]] bool show_target_sprite() const
    {
        return _animation_index < 3;
    }

    [[nodiscard]] bool done() const
    {
        return _sprites.empty();
    }

    void update();

private:
    const btn::sprite_item& _sprite_item;
    btn::optional<btn::sprite_affine_mat_ptr> _affine_mat;
    btn::vector<btn::sprite_ptr, 4> _sprites;
    int _animation_frames;
    int _animation_index;
    int _frame_counter;
};

}

#endif
