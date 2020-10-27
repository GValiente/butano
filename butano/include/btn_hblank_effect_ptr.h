/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HBLANK_EFFECT_PTR_H
#define BTN_HBLANK_EFFECT_PTR_H

#include "btn_utility.h"
#include "btn_span_fwd.h"
#include "btn_functional.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect.
 *
 * Several hblank_effect_ptr objects may own the same H-Blank effect.
 *
 * The H-Blank effect is released when the last remaining hblank_effect_ptr owning it is destroyed.
 *
 * @ingroup hblank_effect
 */
class hblank_effect_ptr
{

public:
    /**
     * @brief Release the referenced H-Blank effect if no more hblank_effect_ptr objects reference to it.
     */
    ~hblank_effect_ptr()
    {
        if(_id >= 0)
        {
            _destroy();
        }
    }

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _id;
    }

    /**
     * @brief Indicates if this H-Blank effect must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const;

    /**
     * @brief Sets if this H-Blank effect must be committed to the GBA or not.
     */
    void set_visible(bool visible);

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const hblank_effect_ptr& a, const hblank_effect_ptr& b) = default;

protected:
    /// @cond DO_NOT_DOCUMENT

    explicit hblank_effect_ptr(int id) :
        _id(int8_t(id))
    {
    }

    hblank_effect_ptr(const hblank_effect_ptr& other);

    hblank_effect_ptr& operator=(const hblank_effect_ptr& other);

    hblank_effect_ptr(hblank_effect_ptr&& other) noexcept :
        hblank_effect_ptr(other._id)
    {
        other._id = -1;
    }

    hblank_effect_ptr& operator=(hblank_effect_ptr&& other) noexcept
    {
        btn::swap(_id, other._id);
        return *this;
    }

    void swap(hblank_effect_ptr& other)
    {
        btn::swap(_id, other._id);
    }

    friend void swap(hblank_effect_ptr& a, hblank_effect_ptr& b)
    {
        btn::swap(a._id, b._id);
    }

    /// @endcond

private:
    int8_t _id;

    void _destroy();
};


/**
 * @brief Hash support for hblank_effect_ptr.
 *
 * @ingroup hblank_effect
 */
template<>
struct hash<hblank_effect_ptr>
{
    /**
     * @brief Returns the hash of the given hblank_effect_ptr.
     */
    [[nodiscard]] unsigned operator()(const hblank_effect_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
