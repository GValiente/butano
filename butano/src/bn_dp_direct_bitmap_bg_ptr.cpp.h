/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_dp_direct_bitmap_bg_ptr.h"

#include "bn_color.h"
#include "bn_window.h"
#include "bn_bitmap_bg.h"
#include "bn_bgs_manager.h"
#include "bn_top_left_utils.h"
#include "bn_display_manager.h"
#include "bn_bitmap_bg_attributes.h"
#include "bn_dp_direct_bitmap_bg_builder.h"

namespace bn
{

size dp_direct_bitmap_bg_ptr::dimensions()
{
    return bitmap_bg::dp_direct_size();
}

dp_direct_bitmap_bg_ptr dp_direct_bitmap_bg_ptr::create()
{
    return dp_direct_bitmap_bg_ptr(bgs_manager::create(dp_direct_bitmap_bg_builder()));
}

dp_direct_bitmap_bg_ptr dp_direct_bitmap_bg_ptr::create(const dp_direct_bitmap_bg_builder& builder)
{
    return dp_direct_bitmap_bg_ptr(bgs_manager::create(dp_direct_bitmap_bg_builder(builder)));
}

dp_direct_bitmap_bg_ptr dp_direct_bitmap_bg_ptr::create(dp_direct_bitmap_bg_builder&& builder)
{
    return dp_direct_bitmap_bg_ptr(bgs_manager::create(move(builder)));
}

optional<dp_direct_bitmap_bg_ptr> dp_direct_bitmap_bg_ptr::create_optional()
{
    optional<dp_direct_bitmap_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(dp_direct_bitmap_bg_builder()))
    {
        result = dp_direct_bitmap_bg_ptr(handle);
    }

    return result;
}

optional<dp_direct_bitmap_bg_ptr> dp_direct_bitmap_bg_ptr::create_optional(const dp_direct_bitmap_bg_builder& builder)
{
    optional<dp_direct_bitmap_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(dp_direct_bitmap_bg_builder(builder)))
    {
        result = dp_direct_bitmap_bg_ptr(handle);
    }

    return result;
}

optional<dp_direct_bitmap_bg_ptr> dp_direct_bitmap_bg_ptr::create_optional(dp_direct_bitmap_bg_builder&& builder)
{
    optional<dp_direct_bitmap_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = dp_direct_bitmap_bg_ptr(handle);
    }

    return result;
}

dp_direct_bitmap_bg_ptr::dp_direct_bitmap_bg_ptr(const dp_direct_bitmap_bg_ptr& other) :
    dp_direct_bitmap_bg_ptr(other._handle)
{
    bgs_manager::increase_usages(_handle);
}

dp_direct_bitmap_bg_ptr& dp_direct_bitmap_bg_ptr::operator=(const dp_direct_bitmap_bg_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle)
        {
            bgs_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        bgs_manager::increase_usages(_handle);
    }

    return *this;
}

dp_direct_bitmap_bg_ptr::~dp_direct_bitmap_bg_ptr()
{
    if(_handle)
    {
        bgs_manager::decrease_usages(_handle);
    }
}

span<const color> dp_direct_bitmap_bg_ptr::page() const
{
    auto ptr = reinterpret_cast<color*>(display_manager::bitmap_page());
    return span<const color>(ptr, bitmap_bg::dp_direct_width() * bitmap_bg::dp_direct_height());
}

span<color> dp_direct_bitmap_bg_ptr::page()
{
    auto ptr = reinterpret_cast<color*>(display_manager::bitmap_page());
    return span<color>(ptr, bitmap_bg::dp_direct_width() * bitmap_bg::dp_direct_height());
}

void dp_direct_bitmap_bg_ptr::flip_page_later()
{
    display_manager::flip_bitmap_page_later();
}

void dp_direct_bitmap_bg_ptr::flip_page_now()
{
    display_manager::flip_bitmap_page_now();
}

fixed dp_direct_bitmap_bg_ptr::x() const
{
    return position().x();
}

void dp_direct_bitmap_bg_ptr::set_x(fixed x)
{
    bgs_manager::set_affine_x(_handle, x);
}

fixed dp_direct_bitmap_bg_ptr::y() const
{
    return position().y();
}

void dp_direct_bitmap_bg_ptr::set_y(fixed y)
{
    bgs_manager::set_affine_y(_handle, y);
}

const fixed_point& dp_direct_bitmap_bg_ptr::position() const
{
    return bgs_manager::position(_handle);
}

void dp_direct_bitmap_bg_ptr::set_position(fixed x, fixed y)
{
    bgs_manager::set_affine_position(_handle, fixed_point(x, y));
}

void dp_direct_bitmap_bg_ptr::set_position(const fixed_point& position)
{
    bgs_manager::set_affine_position(_handle, position);
}

fixed dp_direct_bitmap_bg_ptr::top_left_x() const
{
    return to_top_left_x(position().x(), dimensions().width());
}

void dp_direct_bitmap_bg_ptr::set_top_left_x(fixed top_left_x)
{
    set_x(from_top_left_x(top_left_x, dimensions().width()));
}

fixed dp_direct_bitmap_bg_ptr::top_left_y() const
{
    return to_top_left_y(position().y(), dimensions().height());
}

void dp_direct_bitmap_bg_ptr::set_top_left_y(fixed top_left_y)
{
    set_y(from_top_left_y(top_left_y, dimensions().height()));
}

fixed_point dp_direct_bitmap_bg_ptr::top_left_position() const
{
    return to_top_left_position(position(), dimensions());
}

void dp_direct_bitmap_bg_ptr::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), dimensions()));
}

void dp_direct_bitmap_bg_ptr::set_top_left_position(const fixed_point& top_left_position)
{
    set_position(from_top_left_position(top_left_position, dimensions()));
}

fixed dp_direct_bitmap_bg_ptr::rotation_angle() const
{
    return bgs_manager::rotation_angle(_handle);
}

void dp_direct_bitmap_bg_ptr::set_rotation_angle(fixed rotation_angle)
{
    bgs_manager::set_rotation_angle(_handle, rotation_angle);
}

void dp_direct_bitmap_bg_ptr::set_rotation_angle_safe(fixed rotation_angle)
{
    bgs_manager::set_rotation_angle(_handle, safe_degrees_angle(rotation_angle));
}

fixed dp_direct_bitmap_bg_ptr::horizontal_scale() const
{
    return bgs_manager::horizontal_scale(_handle);
}

void dp_direct_bitmap_bg_ptr::set_horizontal_scale(fixed horizontal_scale)
{
    bgs_manager::set_horizontal_scale(_handle, horizontal_scale);
}

fixed dp_direct_bitmap_bg_ptr::vertical_scale() const
{
    return bgs_manager::vertical_scale(_handle);
}

void dp_direct_bitmap_bg_ptr::set_vertical_scale(fixed vertical_scale)
{
    bgs_manager::set_vertical_scale(_handle, vertical_scale);
}

void dp_direct_bitmap_bg_ptr::set_scale(fixed scale)
{
    bgs_manager::set_scale(_handle, scale);
}

void dp_direct_bitmap_bg_ptr::set_scale(fixed horizontal_scale, fixed vertical_scale)
{
    bgs_manager::set_scale(_handle, horizontal_scale, vertical_scale);
}

fixed dp_direct_bitmap_bg_ptr::horizontal_shear() const
{
    return bgs_manager::horizontal_shear(_handle);
}

void dp_direct_bitmap_bg_ptr::set_horizontal_shear(fixed horizontal_shear)
{
    bgs_manager::set_horizontal_shear(_handle, horizontal_shear);
}

fixed dp_direct_bitmap_bg_ptr::vertical_shear() const
{
    return bgs_manager::vertical_shear(_handle);
}

void dp_direct_bitmap_bg_ptr::set_vertical_shear(fixed vertical_shear)
{
    bgs_manager::set_vertical_shear(_handle, vertical_shear);
}

void dp_direct_bitmap_bg_ptr::set_shear(fixed shear)
{
    bgs_manager::set_shear(_handle, shear);
}

void dp_direct_bitmap_bg_ptr::set_shear(fixed horizontal_shear, fixed vertical_shear)
{
    bgs_manager::set_shear(_handle, horizontal_shear, vertical_shear);
}

bool dp_direct_bitmap_bg_ptr::horizontal_flip() const
{
    return bgs_manager::horizontal_flip(_handle);
}

void dp_direct_bitmap_bg_ptr::set_horizontal_flip(bool horizontal_flip)
{
    bgs_manager::set_horizontal_flip(_handle, horizontal_flip);
}

bool dp_direct_bitmap_bg_ptr::vertical_flip() const
{
    return bgs_manager::vertical_flip(_handle);
}

void dp_direct_bitmap_bg_ptr::set_vertical_flip(bool vertical_flip)
{
    bgs_manager::set_vertical_flip(_handle, vertical_flip);
}

fixed dp_direct_bitmap_bg_ptr::pivot_x() const
{
    return bgs_manager::pivot_position(_handle).x();
}

void dp_direct_bitmap_bg_ptr::set_pivot_x(fixed pivot_x)
{
    bgs_manager::set_pivot_x(_handle, pivot_x);
}

fixed dp_direct_bitmap_bg_ptr::pivot_y() const
{
    return bgs_manager::pivot_position(_handle).y();
}

void dp_direct_bitmap_bg_ptr::set_pivot_y(fixed pivot_y)
{
    bgs_manager::set_pivot_y(_handle, pivot_y);
}

const fixed_point& dp_direct_bitmap_bg_ptr::pivot_position() const
{
    return bgs_manager::pivot_position(_handle);
}

void dp_direct_bitmap_bg_ptr::set_pivot_position(fixed pivot_x, fixed pivot_y)
{
    bgs_manager::set_pivot_position(_handle, fixed_point(pivot_x, pivot_y));
}

void dp_direct_bitmap_bg_ptr::set_pivot_position(const fixed_point& pivot_position)
{
    bgs_manager::set_pivot_position(_handle, pivot_position);
}

const affine_bg_mat_attributes& dp_direct_bitmap_bg_ptr::mat_attributes() const
{
    return bgs_manager::mat_attributes(_handle);
}

void dp_direct_bitmap_bg_ptr::set_mat_attributes(const affine_mat_attributes& mat_attributes)
{
    bgs_manager::set_mat_attributes(_handle, mat_attributes);
}

int dp_direct_bitmap_bg_ptr::priority() const
{
    return bgs_manager::priority(_handle);
}

void dp_direct_bitmap_bg_ptr::set_priority(int priority)
{
    bgs_manager::set_priority(_handle, priority);
}

bool dp_direct_bitmap_bg_ptr::mosaic_enabled() const
{
    return bgs_manager::mosaic_enabled(_handle);
}

void dp_direct_bitmap_bg_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

bool dp_direct_bitmap_bg_ptr::blending_top_enabled() const
{
    return bgs_manager::blending_top_enabled(_handle);
}

void dp_direct_bitmap_bg_ptr::set_blending_top_enabled(bool blending_top_enabled)
{
    bgs_manager::set_blending_top_enabled(_handle, blending_top_enabled);
}

bool dp_direct_bitmap_bg_ptr::blending_bottom_enabled() const
{
    return bgs_manager::blending_bottom_enabled(_handle);
}

void dp_direct_bitmap_bg_ptr::set_blending_bottom_enabled(bool blending_bottom_enabled)
{
    bgs_manager::set_blending_bottom_enabled(_handle, blending_bottom_enabled);
}

bn::green_swap_mode dp_direct_bitmap_bg_ptr::green_swap_mode() const
{
    return bgs_manager::green_swap_mode(_handle);
}

void dp_direct_bitmap_bg_ptr::set_green_swap_mode(bn::green_swap_mode green_swap_mode)
{
    bgs_manager::set_green_swap_mode(_handle, green_swap_mode);
}

bool dp_direct_bitmap_bg_ptr::visible() const
{
    return bgs_manager::visible(_handle);
}

void dp_direct_bitmap_bg_ptr::set_visible(bool visible)
{
    bgs_manager::set_visible(_handle, visible);
}

bool dp_direct_bitmap_bg_ptr::visible_in_window(const window& window) const
{
    return window.show_bg(*this);
}

void dp_direct_bitmap_bg_ptr::set_visible_in_window(bool visible, window& window)
{
    window.set_show_bg(*this, visible);
}

const optional<camera_ptr>& dp_direct_bitmap_bg_ptr::camera() const
{
    return bgs_manager::camera(_handle);
}

void dp_direct_bitmap_bg_ptr::set_camera(const camera_ptr& camera)
{
    bgs_manager::set_camera(_handle, camera_ptr(camera));
}

void dp_direct_bitmap_bg_ptr::set_camera(camera_ptr&& camera)
{
    bgs_manager::set_camera(_handle, move(camera));
}

void dp_direct_bitmap_bg_ptr::set_camera(const optional<camera_ptr>& camera)
{
    if(const camera_ptr* camera_ref = camera.get())
    {
        bgs_manager::set_camera(_handle, camera_ptr(*camera_ref));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void dp_direct_bitmap_bg_ptr::set_camera(optional<camera_ptr>&& camera)
{
    if(camera_ptr* camera_ref = camera.get())
    {
        bgs_manager::set_camera(_handle, move(*camera_ref));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void dp_direct_bitmap_bg_ptr::remove_camera()
{
    bgs_manager::remove_camera(_handle);
}

bitmap_bg_attributes dp_direct_bitmap_bg_ptr::attributes() const
{
    return bgs_manager::bitmap_attributes(_handle);
}

void dp_direct_bitmap_bg_ptr::set_attributes(const bitmap_bg_attributes& attributes)
{
    bgs_manager::set_bitmap_attributes(_handle, attributes);
}

optional<int> dp_direct_bitmap_bg_ptr::hw_id() const
{
    bgs_manager::rebuild_handles();

    optional<int> result;
    int id = bgs_manager::hw_id(_handle);

    if(id >= 0)
    {
        result = id;
    }

    return result;
}

}
