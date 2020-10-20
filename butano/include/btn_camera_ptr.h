/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CAMERA_PTR_H
#define BTN_CAMERA_PTR_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class fixed_point;

class camera_ptr
{

public:
    [[nodiscard]] static camera_ptr create(fixed x, fixed y);

    [[nodiscard]] static camera_ptr create(const fixed_point& position);

    [[nodiscard]] static optional<camera_ptr> create_optional(fixed x, fixed y);

    [[nodiscard]] static optional<camera_ptr> create_optional(const fixed_point& position);

    camera_ptr(const camera_ptr& other);

    camera_ptr& operator=(const camera_ptr& other);

    camera_ptr(camera_ptr&& other) noexcept :
        camera_ptr(other._id)
    {
        other._id = -1;
    }

    camera_ptr& operator=(camera_ptr&& other) noexcept
    {
        btn::swap(_id, other._id);
        return *this;
    }

    ~camera_ptr()
    {
        if(_id >= 0)
        {
            _destroy();
        }
    }

    [[nodiscard]] fixed x() const;

    [[nodiscard]] fixed y() const;

    [[nodiscard]] const fixed_point& position() const;

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);

    [[nodiscard]] int id() const
    {
        return _id;
    }

    void swap(camera_ptr& other)
    {
        btn::swap(_id, other._id);
    }

    friend void swap(camera_ptr& a, camera_ptr& b)
    {
        btn::swap(a._id, b._id);
    }

    [[nodiscard]] friend bool operator==(const camera_ptr& a, const camera_ptr& b) = default;

private:
    int8_t _id;

    explicit camera_ptr(int8_t id) :
        _id(id)
    {
    }

    void _destroy();
};


template<>
struct hash<camera_ptr>
{
    [[nodiscard]] unsigned operator()(const camera_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
