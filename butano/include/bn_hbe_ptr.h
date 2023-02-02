/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HBE_PTR_H
#define BN_HBE_PTR_H

/**
 * @file
 * bn::hbe_ptr header file.
 *
 * @ingroup hblank_effect
 */

#include "bn_utility.h"
#include "bn_functional.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect.
 *
 * Several hbe_ptr objects may own the same H-Blank effect.
 *
 * The H-Blank effect is released when the last remaining hbe_ptr owning it is destroyed.
 *
 * @ingroup hblank_effect
 */
class hbe_ptr
{

public:
    /**
     * @brief Releases the referenced H-Blank effect if no more hbe_ptr objects reference to it.
     */
    ~hbe_ptr();

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
    [[nodiscard]] friend bool operator==(const hbe_ptr& a, const hbe_ptr& b) = default;

protected:
    /// @cond DO_NOT_DOCUMENT

    explicit hbe_ptr(int id) :
        _id(int8_t(id))
    {
    }

    hbe_ptr(const hbe_ptr& other);

    hbe_ptr& operator=(const hbe_ptr& other);

    hbe_ptr(hbe_ptr&& other) noexcept :
        hbe_ptr(other._id)
    {
        other._id = -1;
    }

    hbe_ptr& operator=(hbe_ptr&& other) noexcept
    {
        bn::swap(_id, other._id);
        return *this;
    }

    void swap(hbe_ptr& other)
    {
        bn::swap(_id, other._id);
    }

    friend void swap(hbe_ptr& a, hbe_ptr& b)
    {
        bn::swap(a._id, b._id);
    }

    /// @endcond

private:
    int8_t _id;
};


/**
 * @brief Hash support for hbe_ptr.
 *
 * @ingroup hblank_effect
 * @ingroup functional
 */
template<>
struct hash<hbe_ptr>
{
    /**
     * @brief Returns the hash of the given hbe_ptr.
     */
    [[nodiscard]] unsigned operator()(const hbe_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
