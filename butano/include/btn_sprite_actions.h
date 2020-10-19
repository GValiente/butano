#ifndef BTN_SPRITE_ACTIONS_H
#define BTN_SPRITE_ACTIONS_H

#include "btn_array.h"
#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_tiles_item.h"
#include "btn_value_template_actions.h"

namespace btn
{

// visible

class sprite_visible_manager
{

public:
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.visible();
    }

    static void set(bool visible, sprite_ptr& sprite)
    {
        sprite.set_visible(visible);
    }
};


class sprite_visible_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_visible_manager>
{

public:
    sprite_visible_toggle_action(const sprite_ptr& sprite, int duration_frames) :
        bool_toggle_value_template_action(sprite, duration_frames)
    {
    }

    sprite_visible_toggle_action(sprite_ptr&& sprite, int duration_frames) :
        bool_toggle_value_template_action(move(sprite), duration_frames)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// position

class sprite_position_manager
{

public:
    [[nodiscard]] static const fixed_point& get(const sprite_ptr& sprite)
    {
        return sprite.position();
    }

    static void set(const fixed_point& position, sprite_ptr& sprite)
    {
        sprite.set_position(position);
    }
};


class sprite_move_by_action : public by_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    sprite_move_by_action(const sprite_ptr& sprite, fixed delta_x, fixed delta_y) :
        by_value_template_action(sprite, fixed_point(delta_x, delta_y))
    {
    }

    sprite_move_by_action(sprite_ptr&& sprite, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(sprite), fixed_point(delta_x, delta_y))
    {
    }

    sprite_move_by_action(const sprite_ptr& sprite, const fixed_point& delta_position) :
        by_value_template_action(sprite, delta_position)
    {
    }

    sprite_move_by_action(sprite_ptr&& sprite, const fixed_point& delta_position) :
        by_value_template_action(move(sprite), delta_position)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


class sprite_move_to_action : public to_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    sprite_move_to_action(const sprite_ptr& sprite, int duration_frames, fixed final_x, fixed final_y) :
        to_value_template_action(sprite, duration_frames, fixed_point(final_x, final_y))
    {
    }

    sprite_move_to_action(sprite_ptr&& sprite, int duration_frames, fixed final_x, fixed final_y) :
        to_value_template_action(move(sprite), duration_frames, fixed_point(final_x, final_y))
    {
    }

    sprite_move_to_action(const sprite_ptr& sprite, int duration_frames, const fixed_point& final_position) :
        to_value_template_action(sprite, duration_frames, final_position)
    {
    }

    sprite_move_to_action(sprite_ptr&& sprite, int duration_frames, const fixed_point& final_position) :
        to_value_template_action(move(sprite), duration_frames, final_position)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class sprite_move_loop_action : public loop_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    sprite_move_loop_action(const sprite_ptr& sprite, int duration_frames, fixed final_x, fixed final_y) :
        loop_value_template_action(sprite, duration_frames, fixed_point(final_x, final_y))
    {
    }

    sprite_move_loop_action(sprite_ptr&& sprite, int duration_frames, fixed final_x, fixed final_y) :
        loop_value_template_action(move(sprite), duration_frames, fixed_point(final_x, final_y))
    {
    }

    sprite_move_loop_action(const sprite_ptr& sprite, int duration_frames, const fixed_point& final_position) :
        loop_value_template_action(sprite, duration_frames, final_position)
    {
    }

    sprite_move_loop_action(sprite_ptr&& sprite, int duration_frames, const fixed_point& final_position) :
        loop_value_template_action(move(sprite), duration_frames, final_position)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class sprite_move_toggle_action : public toggle_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    sprite_move_toggle_action(const sprite_ptr& sprite, int duration_frames, fixed new_x, fixed new_y) :
        toggle_value_template_action(sprite, duration_frames, fixed_point(new_x, new_y))
    {
    }

    sprite_move_toggle_action(sprite_ptr&& sprite, int duration_frames, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(sprite), duration_frames, fixed_point(new_x, new_y))
    {
    }

    sprite_move_toggle_action(const sprite_ptr& sprite, int duration_frames, const fixed_point& new_position) :
        toggle_value_template_action(sprite, duration_frames, new_position)
    {
    }

    sprite_move_toggle_action(sprite_ptr&& sprite, int duration_frames, const fixed_point& new_position) :
        toggle_value_template_action(move(sprite), duration_frames, new_position)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};


// rotation

class sprite_rotation_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.rotation_angle();
    }

    static void set(fixed rotation_angle, sprite_ptr& sprite)
    {
        sprite.set_rotation_angle(rotation_angle);
    }
};


class sprite_rotate_by_action : public cyclic_by_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    sprite_rotate_by_action(const sprite_ptr& sprite, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(sprite, delta_rotation_angle, 0, 360)
    {
    }

    sprite_rotate_by_action(sprite_ptr&& sprite, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(move(sprite), delta_rotation_angle, 0, 360)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_rotation_angle() const
    {
        return delta_property();
    }
};


class sprite_rotate_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    sprite_rotate_to_action(const sprite_ptr& sprite, int duration_frames, fixed final_rotation_angle) :
        to_value_template_action(sprite, duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    sprite_rotate_to_action(sprite_ptr&& sprite, int duration_frames, fixed final_rotation_angle) :
        to_value_template_action(move(sprite), duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


class sprite_rotate_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    sprite_rotate_loop_action(const sprite_ptr& sprite, int duration_frames, fixed final_rotation_angle) :
        loop_value_template_action(sprite, duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    sprite_rotate_loop_action(sprite_ptr&& sprite, int duration_frames, fixed final_rotation_angle) :
        loop_value_template_action(move(sprite), duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


class sprite_rotate_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    sprite_rotate_toggle_action(const sprite_ptr& sprite, int duration_frames, fixed new_rotation_angle) :
        toggle_value_template_action(sprite, duration_frames, new_rotation_angle)
    {
        BTN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    sprite_rotate_toggle_action(sprite_ptr&& sprite, int duration_frames, fixed new_rotation_angle) :
        toggle_value_template_action(move(sprite), duration_frames, new_rotation_angle)
    {
        BTN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed new_rotation_angle() const
    {
        return new_property();
    }
};


// horizontal_scale

class sprite_horizontal_scale_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_scale();
    }

    static void set(fixed horizontal_scale, sprite_ptr& sprite)
    {
        sprite.set_horizontal_scale(horizontal_scale);
    }
};


class sprite_horizontal_scale_to_action :
        public to_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>
{

public:
    sprite_horizontal_scale_to_action(const sprite_ptr& sprite, int duration_frames, fixed final_horizontal_scale) :
        to_value_template_action(sprite, duration_frames, final_horizontal_scale)
    {
        BTN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    sprite_horizontal_scale_to_action(sprite_ptr&& sprite, int duration_frames, fixed final_horizontal_scale) :
        to_value_template_action(move(sprite), duration_frames, final_horizontal_scale)
    {
        BTN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_horizontal_scale() const
    {
        return final_property();
    }
};


class sprite_horizontal_scale_loop_action :
        public loop_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>
{

public:
    sprite_horizontal_scale_loop_action(const sprite_ptr& sprite, int duration_frames, fixed final_horizontal_scale) :
        loop_value_template_action(sprite, duration_frames, final_horizontal_scale)
    {
        BTN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    sprite_horizontal_scale_loop_action(sprite_ptr&& sprite, int duration_frames, fixed final_horizontal_scale) :
        loop_value_template_action(move(sprite), duration_frames, final_horizontal_scale)
    {
        BTN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_horizontal_scale() const
    {
        return final_property();
    }
};


class sprite_horizontal_scale_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>
{

public:
    sprite_horizontal_scale_toggle_action(const sprite_ptr& sprite, int duration_frames, fixed new_horizontal_scale) :
        toggle_value_template_action(sprite, duration_frames, new_horizontal_scale)
    {
        BTN_ASSERT(new_horizontal_scale > 0, "Invalid new horizontal scale: ", new_horizontal_scale);
    }

    sprite_horizontal_scale_toggle_action(sprite_ptr&& sprite, int duration_frames, fixed new_horizontal_scale) :
        toggle_value_template_action(move(sprite), duration_frames, new_horizontal_scale)
    {
        BTN_ASSERT(new_horizontal_scale > 0, "Invalid new horizontal scale: ", new_horizontal_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed new_horizontal_scale() const
    {
        return new_property();
    }
};


// vertical_scale

class sprite_vertical_scale_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.vertical_scale();
    }

    static void set(fixed vertical_scale, sprite_ptr& sprite)
    {
        sprite.set_vertical_scale(vertical_scale);
    }
};


class sprite_vertical_scale_to_action :
        public to_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>
{

public:
    sprite_vertical_scale_to_action(const sprite_ptr& sprite, int duration_frames, fixed final_vertical_scale) :
        to_value_template_action(sprite, duration_frames, final_vertical_scale)
    {
        BTN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    sprite_vertical_scale_to_action(sprite_ptr&& sprite, int duration_frames, fixed final_vertical_scale) :
        to_value_template_action(move(sprite), duration_frames, final_vertical_scale)
    {
        BTN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_vertical_scale() const
    {
        return final_property();
    }
};


class sprite_vertical_scale_loop_action :
        public loop_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>
{

public:
    sprite_vertical_scale_loop_action(const sprite_ptr& sprite, int duration_frames, fixed final_vertical_scale) :
        loop_value_template_action(sprite, duration_frames, final_vertical_scale)
    {
        BTN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    sprite_vertical_scale_loop_action(sprite_ptr&& sprite, int duration_frames, fixed final_vertical_scale) :
        loop_value_template_action(move(sprite), duration_frames, final_vertical_scale)
    {
        BTN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_vertical_scale() const
    {
        return final_property();
    }
};


class sprite_vertical_scale_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>
{

public:
    sprite_vertical_scale_toggle_action(const sprite_ptr& sprite, int duration_frames, fixed new_vertical_scale) :
        toggle_value_template_action(sprite, duration_frames, new_vertical_scale)
    {
        BTN_ASSERT(new_vertical_scale > 0, "Invalid new vertical scale: ", new_vertical_scale);
    }

    sprite_vertical_scale_toggle_action(sprite_ptr&& sprite, int duration_frames, fixed new_vertical_scale) :
        toggle_value_template_action(move(sprite), duration_frames, new_vertical_scale)
    {
        BTN_ASSERT(new_vertical_scale > 0, "Invalid new vertical scale: ", new_vertical_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed new_vertical_scale() const
    {
        return new_property();
    }
};


// scale

class sprite_scale_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_scale();
    }

    static void set(fixed scale, sprite_ptr& sprite)
    {
        sprite.set_scale(scale, scale);
    }
};


class sprite_scale_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_scale_manager>
{

public:
    sprite_scale_to_action(const sprite_ptr& sprite, int duration_frames, fixed final_scale) :
        to_value_template_action(sprite, duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    sprite_scale_to_action(sprite_ptr&& sprite, int duration_frames, fixed final_scale) :
        to_value_template_action(move(sprite), duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


class sprite_scale_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_scale_manager>
{

public:
    sprite_scale_loop_action(const sprite_ptr& sprite, int duration_frames, fixed final_scale) :
        loop_value_template_action(sprite, duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    sprite_scale_loop_action(sprite_ptr&& sprite, int duration_frames, fixed final_scale) :
        loop_value_template_action(move(sprite), duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


class sprite_scale_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_scale_manager>
{

public:
    sprite_scale_toggle_action(const sprite_ptr& sprite, int duration_frames, fixed new_scale) :
        toggle_value_template_action(sprite, duration_frames, new_scale)
    {
        BTN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    sprite_scale_toggle_action(sprite_ptr&& sprite, int duration_frames, fixed new_scale) :
        toggle_value_template_action(move(sprite), duration_frames, new_scale)
    {
        BTN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    [[nodiscard]] fixed new_scale() const
    {
        return new_property();
    }
};


// horizontal_flip

class sprite_horizontal_flip_manager
{

public:
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_flip();
    }

    static void set(bool horizontal_flip, sprite_ptr& sprite)
    {
        sprite.set_horizontal_flip(horizontal_flip);
    }
};


class sprite_horizontal_flip_toggle_action :
        public bool_toggle_value_template_action<sprite_ptr, sprite_horizontal_flip_manager>
{

public:
    sprite_horizontal_flip_toggle_action(const sprite_ptr& sprite, int duration_frames) :
        bool_toggle_value_template_action(sprite, duration_frames)
    {
    }

    sprite_horizontal_flip_toggle_action(sprite_ptr&& sprite, int duration_frames) :
        bool_toggle_value_template_action(move(sprite), duration_frames)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// vertical_flip

class sprite_vertical_flip_manager
{

public:
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.vertical_flip();
    }

    static void set(bool vertical_flip, sprite_ptr& sprite)
    {
        sprite.set_vertical_flip(vertical_flip);
    }
};


class sprite_vertical_flip_toggle_action :
        public bool_toggle_value_template_action<sprite_ptr, sprite_vertical_flip_manager>
{

public:
    sprite_vertical_flip_toggle_action(const sprite_ptr& sprite, int duration_frames) :
        bool_toggle_value_template_action(sprite, duration_frames)
    {
    }

    sprite_vertical_flip_toggle_action(sprite_ptr&& sprite, int duration_frames) :
        bool_toggle_value_template_action(move(sprite), duration_frames)
    {
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// animation

template<int Size>
class sprite_animate_action
{
    static_assert(Size > 1);

public:
    [[nodiscard]] static sprite_animate_action once(
            const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(sprite, wait_frames, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_animate_action once(
            sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(move(sprite), wait_frames, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_animate_action forever(
            const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(sprite, wait_frames, tiles_item, true, graphics_indexes);
    }

    [[nodiscard]] static sprite_animate_action forever(
            sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(move(sprite), wait_frames, tiles_item, true, graphics_indexes);
    }

    void reset()
    {
        _current_graphics_indexes_index = 0;
        _current_wait_frames = 0;
    }

    void update()
    {
        BTN_ASSERT(! done(), "Action is done");

        if(_current_wait_frames)
        {
            --_current_wait_frames;
        }
        else
        {
            int current_graphics_indexes_index = _current_graphics_indexes_index;
            int current_graphics_index = _graphics_indexes[current_graphics_indexes_index];
            _current_wait_frames = _wait_frames;

            if(current_graphics_indexes_index == 0 ||
                    _graphics_indexes[current_graphics_indexes_index - 1] != current_graphics_index)
            {
                _sprite.set_tiles(_tiles_item, current_graphics_index);
            }

            if(_forever && current_graphics_indexes_index == _graphics_indexes.size() - 1)
            {
                _current_graphics_indexes_index = 0;
            }
            else
            {
                ++_current_graphics_indexes_index;
            }
        }
    }

    [[nodiscard]] bool done() const
    {
        return _current_graphics_indexes_index == _graphics_indexes.size();
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] int wait_frames() const
    {
        return _wait_frames;
    }

    [[nodiscard]] const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    [[nodiscard]] const vector<uint16_t, Size>& graphics_indexes() const
    {
        return _graphics_indexes;
    }

    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    [[nodiscard]] int current_index() const
    {
        return _current_graphics_indexes_index;
    }

private:
    bool _forever = true;
    uint16_t _wait_frames = 0;
    sprite_ptr _sprite;
    sprite_tiles_item _tiles_item;
    vector<uint16_t, Size> _graphics_indexes;
    uint16_t _current_graphics_indexes_index = 0;
    uint16_t _current_wait_frames = 0;

    sprite_animate_action(const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item, bool forever,
                          const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_frames(uint16_t(wait_frames)),
        _sprite(sprite),
        _tiles_item(tiles_item)
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= numeric_limits<decltype(_wait_frames)>::max(), "Too much wait frames: ", wait_frames);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size, "Invalid graphics indexes: ",
                   graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _graphics_indexes.push_back(graphics_index);
        }
    }

    sprite_animate_action(sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item, bool forever,
                          const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_frames(uint16_t(wait_frames)),
        _sprite(move(sprite)),
        _tiles_item(tiles_item)
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= numeric_limits<decltype(_wait_frames)>::max(), "Too much wait frames: ", wait_frames);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size, "Invalid graphics indexes: ",
                   graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _graphics_indexes.push_back(graphics_index);
        }
    }
};

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_once(
        const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::once(
                sprite, wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_once(
        sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::once(
                move(sprite), wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_forever(
        const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::forever(
                sprite, wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_forever(
        sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::forever(
                move(sprite), wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


// cached animation

template<int Size>
class sprite_cached_animate_action
{
    static_assert(Size > 1);

public:
    [[nodiscard]] static sprite_cached_animate_action once(
            const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(sprite, wait_frames, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action once(
            sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(move(sprite), wait_frames, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action once(
            const sprite_ptr& sprite, int wait_frames, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(sprite, wait_frames, false, tiles_list);
    }

    [[nodiscard]] static sprite_cached_animate_action once(
            sprite_ptr&& sprite, int wait_frames, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_frames, false, tiles_list);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(sprite, wait_frames, tiles_item, true, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(move(sprite), wait_frames, tiles_item, true, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            const sprite_ptr& sprite, int wait_frames, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(sprite, wait_frames, true, tiles_list);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_frames, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_frames, true, tiles_list);
    }

    void reset()
    {
        _current_tiles_list_index = 0;
        _current_wait_frames = 0;
    }

    void update()
    {
        BTN_ASSERT(! done(), "Action is done");

        if(_current_wait_frames)
        {
            --_current_wait_frames;
        }
        else
        {
            _current_wait_frames = _wait_frames;
            _sprite.set_tiles(_tiles_list[_current_tiles_list_index]);
            ++_current_tiles_list_index;

            if(_forever && _current_tiles_list_index == _tiles_list.size())
            {
                _current_tiles_list_index = 0;
            }
        }
    }

    [[nodiscard]] bool done() const
    {
        return _current_tiles_list_index == _tiles_list.size();
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] int wait_frames() const
    {
        return _wait_frames;
    }

    [[nodiscard]] const vector<sprite_tiles_ptr, Size>& tiles_list() const
    {
        return _tiles_list;
    }

    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    [[nodiscard]] int current_index() const
    {
        return _current_tiles_list_index;
    }

private:
    bool _forever = true;
    uint16_t _wait_frames = 0;
    sprite_ptr _sprite;
    vector<sprite_tiles_ptr, Size> _tiles_list;
    uint16_t _current_tiles_list_index = 0;
    uint16_t _current_wait_frames = 0;

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_frames(uint16_t(wait_frames)),
        _sprite(sprite)
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= numeric_limits<decltype(_wait_frames)>::max(), "Too much wait frames: ", wait_frames);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size, "Invalid graphics indexes: ",
                   graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _tiles_list.push_back(tiles_item.create_tiles(graphics_index));
        }
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_frames(uint16_t(wait_frames)),
        _sprite(move(sprite))
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= numeric_limits<decltype(_wait_frames)>::max(), "Too much wait frames: ", wait_frames);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size, "Invalid graphics indexes: ",
                   graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _tiles_list.push_back(tiles_item.create_tiles(graphics_index));
        }
    }

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_frames, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _forever(forever),
        _wait_frames(uint16_t(wait_frames)),
        _sprite(sprite)
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= numeric_limits<decltype(_wait_frames)>::max(), "Too much wait frames: ", wait_frames);
        BTN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= Size, "Invalid tiles ptrs: ", tiles_list.size());

        for(sprite_tiles_ptr& tiles : tiles_list)
        {
            _tiles_list.push_back(move(tiles));
        }
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_frames, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _forever(forever),
        _wait_frames(uint16_t(wait_frames)),
        _sprite(move(sprite))
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= numeric_limits<decltype(_wait_frames)>::max(), "Too much wait frames: ", wait_frames);
        BTN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= Size, "Invalid tiles ptrs: ", tiles_list.size());

        for(sprite_tiles_ptr& tiles : tiles_list)
        {
            _tiles_list.push_back(move(tiles));
        }
    }
};

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_once(
        const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::once(
                sprite, wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_once(
        sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::once(
                move(sprite), wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template< typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_forever(
        const sprite_ptr& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::forever(
                sprite, wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template< typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_forever(
        sprite_ptr&& sprite, int wait_frames, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::forever(
                move(sprite), wait_frames, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

}

#endif
