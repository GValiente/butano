#ifndef BTN_SPRITE_AFFINE_MAT_ACTIONS_H
#define BTN_SPRITE_AFFINE_MAT_ACTIONS_H

#include "btn_fixed.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

// rotation

class sprite_affine_mat_rotation_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_affine_mat_ptr& affine_mat)
    {
        return affine_mat.rotation_angle();
    }

    static void set(fixed rotation_angle, sprite_affine_mat_ptr& affine_mat)
    {
        affine_mat.set_rotation_angle(rotation_angle);
    }
};


class sprite_affine_mat_rotate_by_action :
        public cyclic_by_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>
{

public:
    sprite_affine_mat_rotate_by_action(const sprite_affine_mat_ptr& affine_mat, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(affine_mat, delta_rotation_angle, 0, 360)
    {
    }

    sprite_affine_mat_rotate_by_action(sprite_affine_mat_ptr&& affine_mat, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(move(affine_mat), delta_rotation_angle, 0, 360)
    {
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_rotation_angle() const
    {
        return delta_property();
    }
};


class sprite_affine_mat_rotate_to_action :
        public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>
{

public:
    sprite_affine_mat_rotate_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                       fixed final_rotation_angle) :
        to_value_template_action(affine_mat, duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    sprite_affine_mat_rotate_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                       fixed final_rotation_angle) :
        to_value_template_action(move(affine_mat), duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


class sprite_affine_mat_rotate_loop_action :
        public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>
{

public:
    sprite_affine_mat_rotate_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                         fixed final_rotation_angle) :
        loop_value_template_action(affine_mat, duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    sprite_affine_mat_rotate_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                         fixed final_rotation_angle) :
        loop_value_template_action(move(affine_mat), duration_frames, final_rotation_angle)
    {
        BTN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


class sprite_affine_mat_rotate_toggle_action :
        public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>
{

public:
    sprite_affine_mat_rotate_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                           fixed new_rotation_angle) :
        toggle_value_template_action(affine_mat, duration_frames, new_rotation_angle)
    {
        BTN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    sprite_affine_mat_rotate_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                           fixed new_rotation_angle) :
        toggle_value_template_action(move(affine_mat), duration_frames, new_rotation_angle)
    {
        BTN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed new_rotation_angle() const
    {
        return new_property();
    }
};


// scale_x

class sprite_affine_mat_scale_x_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_affine_mat_ptr& affine_mat)
    {
        return affine_mat.scale_x();
    }

    static void set(fixed scale_x, sprite_affine_mat_ptr& affine_mat)
    {
        affine_mat.set_scale_x(scale_x);
    }
};


class sprite_affine_mat_scale_x_to_action :
        public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_x_manager>
{

public:
    sprite_affine_mat_scale_x_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                        fixed final_scale_x) :
        to_value_template_action(affine_mat, duration_frames, final_scale_x)
    {
        BTN_ASSERT(final_scale_x > 0, "Invalid final scale x: ", final_scale_x);
    }

    sprite_affine_mat_scale_x_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames, fixed final_scale_x) :
        to_value_template_action(move(affine_mat), duration_frames, final_scale_x)
    {
        BTN_ASSERT(final_scale_x > 0, "Invalid final scale x: ", final_scale_x);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale_x() const
    {
        return final_property();
    }
};


class sprite_affine_mat_scale_x_loop_action :
        public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_x_manager>
{

public:
    sprite_affine_mat_scale_x_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                          fixed final_scale_x) :
        loop_value_template_action(affine_mat, duration_frames, final_scale_x)
    {
        BTN_ASSERT(final_scale_x > 0, "Invalid final scale x: ", final_scale_x);
    }

    sprite_affine_mat_scale_x_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                          fixed final_scale_x) :
        loop_value_template_action(move(affine_mat), duration_frames, final_scale_x)
    {
        BTN_ASSERT(final_scale_x > 0, "Invalid final scale x: ", final_scale_x);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale_x() const
    {
        return final_property();
    }
};


class sprite_affine_mat_scale_x_toggle_action :
        public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_x_manager>
{

public:
    sprite_affine_mat_scale_x_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                            fixed new_scale_x) :
        toggle_value_template_action(affine_mat, duration_frames, new_scale_x)
    {
        BTN_ASSERT(new_scale_x > 0, "Invalid new scale x: ", new_scale_x);
    }

    sprite_affine_mat_scale_x_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                            fixed new_scale_x) :
        toggle_value_template_action(move(affine_mat), duration_frames, new_scale_x)
    {
        BTN_ASSERT(new_scale_x > 0, "Invalid new scale x: ", new_scale_x);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed new_scale_x() const
    {
        return new_property();
    }
};


// scale_y

class sprite_affine_mat_scale_y_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_affine_mat_ptr& affine_mat)
    {
        return affine_mat.scale_y();
    }

    static void set(fixed scale_y, sprite_affine_mat_ptr& affine_mat)
    {
        affine_mat.set_scale_y(scale_y);
    }
};


class sprite_affine_mat_scale_y_to_action :
        public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_y_manager>
{

public:
    sprite_affine_mat_scale_y_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                        fixed final_scale_y) :
        to_value_template_action(affine_mat, duration_frames, final_scale_y)
    {
        BTN_ASSERT(final_scale_y > 0, "Invalid final scale y: ", final_scale_y);
    }

    sprite_affine_mat_scale_y_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames, fixed final_scale_y) :
        to_value_template_action(move(affine_mat), duration_frames, final_scale_y)
    {
        BTN_ASSERT(final_scale_y > 0, "Invalid final scale y: ", final_scale_y);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale_y() const
    {
        return final_property();
    }
};


class sprite_affine_mat_scale_y_loop_action :
        public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_y_manager>
{

public:
    sprite_affine_mat_scale_y_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                          fixed final_scale_y) :
        loop_value_template_action(affine_mat, duration_frames, final_scale_y)
    {
        BTN_ASSERT(final_scale_y > 0, "Invalid final scale y: ", final_scale_y);
    }

    sprite_affine_mat_scale_y_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                          fixed final_scale_y) :
        loop_value_template_action(move(affine_mat), duration_frames, final_scale_y)
    {
        BTN_ASSERT(final_scale_y > 0, "Invalid final scale y: ", final_scale_y);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale_y() const
    {
        return final_property();
    }
};


class sprite_affine_mat_scale_y_toggle_action :
        public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_y_manager>
{

public:
    sprite_affine_mat_scale_y_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                            fixed new_scale_y) :
        toggle_value_template_action(affine_mat, duration_frames, new_scale_y)
    {
        BTN_ASSERT(new_scale_y > 0, "Invalid new scale y: ", new_scale_y);
    }

    sprite_affine_mat_scale_y_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames,
                                            fixed new_scale_y) :
        toggle_value_template_action(move(affine_mat), duration_frames, new_scale_y)
    {
        BTN_ASSERT(new_scale_y > 0, "Invalid new scale y: ", new_scale_y);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed new_scale_y() const
    {
        return new_property();
    }
};


// scale

class sprite_affine_mat_scale_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_affine_mat_ptr& affine_mat)
    {
        return affine_mat.scale_x();
    }

    static void set(fixed scale, sprite_affine_mat_ptr& affine_mat)
    {
        affine_mat.set_scale(scale, scale);
    }
};


class sprite_affine_mat_scale_to_action :
        public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_manager>
{

public:
    sprite_affine_mat_scale_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                      fixed final_scale) :
        to_value_template_action(affine_mat, duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    sprite_affine_mat_scale_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames, fixed final_scale) :
        to_value_template_action(move(affine_mat), duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


class sprite_affine_mat_scale_loop_action :
        public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_manager>
{

public:
    sprite_affine_mat_scale_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                        fixed final_scale) :
        loop_value_template_action(affine_mat, duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    sprite_affine_mat_scale_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames, fixed final_scale) :
        loop_value_template_action(move(affine_mat), duration_frames, final_scale)
    {
        BTN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


class sprite_affine_mat_scale_toggle_action :
        public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_manager>
{

public:
    sprite_affine_mat_scale_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames,
                                          fixed new_scale) :
        toggle_value_template_action(affine_mat, duration_frames, new_scale)
    {
        BTN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    sprite_affine_mat_scale_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames, fixed new_scale) :
        toggle_value_template_action(move(affine_mat), duration_frames, new_scale)
    {
        BTN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }

    [[nodiscard]] fixed new_scale() const
    {
        return new_property();
    }
};


// horizontal_flip

class sprite_affine_mat_horizontal_flip_manager
{

public:
    [[nodiscard]] static bool get(const sprite_affine_mat_ptr& affine_mat)
    {
        return affine_mat.horizontal_flip();
    }

    static void set(bool horizontal_flip, sprite_affine_mat_ptr& affine_mat)
    {
        affine_mat.set_horizontal_flip(horizontal_flip);
    }
};


class sprite_affine_mat_horizontal_flip_toggle_action :
        public bool_toggle_value_template_action<sprite_affine_mat_ptr, sprite_affine_mat_horizontal_flip_manager>
{

public:
    sprite_affine_mat_horizontal_flip_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames) :
        bool_toggle_value_template_action(affine_mat, duration_frames)
    {
    }

    sprite_affine_mat_horizontal_flip_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames) :
        bool_toggle_value_template_action(move(affine_mat), duration_frames)
    {
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }
};


// vertical_flip

class sprite_affine_mat_vertical_flip_manager
{

public:
    [[nodiscard]] static bool get(const sprite_affine_mat_ptr& affine_mat)
    {
        return affine_mat.vertical_flip();
    }

    static void set(bool vertical_flip, sprite_affine_mat_ptr& affine_mat)
    {
        affine_mat.set_vertical_flip(vertical_flip);
    }
};


class sprite_affine_mat_vertical_flip_toggle_action :
        public bool_toggle_value_template_action<sprite_affine_mat_ptr, sprite_affine_mat_vertical_flip_manager>
{

public:
    sprite_affine_mat_vertical_flip_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_frames) :
        bool_toggle_value_template_action(affine_mat, duration_frames)
    {
    }

    sprite_affine_mat_vertical_flip_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_frames) :
        bool_toggle_value_template_action(move(affine_mat), duration_frames)
    {
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return value();
    }
};

}

#endif
