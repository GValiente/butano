/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_MUSIC_ITEM_H
#define BTN_MUSIC_ITEM_H

#include "btn_fixed_fwd.h"
#include "btn_functional.h"

namespace btn
{

/**
 * @brief Contains the required information to play music.
 *
 * The assets conversion tools generate an object of this type in the build folder for each module file
 *
 * Supported module files have *.mod, *.xm, *.s3m or *.it extensions.
 *
 * @ingroup music
 * @ingroup tool
 */
class music_item
{

public:
    /**
     * @brief Constructor.
     * @param id Internal ID.
     */
    constexpr explicit music_item(int id) :
        _id(id)
    {
    }

    /**
     * @brief Returns the internal ID.
     */
    [[nodiscard]] constexpr int id() const
    {
        return _id;
    }

    /**
     * @brief Plays the music specified by this item with default settings.
     *
     * Default settings are volume = 1 and loop enabled.
     */
    void play() const;

    /**
     * @brief Plays the music specified by this item.
     * @param volume Volume level, in the range [0..1].
     */
    void play(fixed volume) const;

    /**
     * @brief Plays the music specified by this item.
     * @param volume Volume level, in the range [0..1].
     * @param loop Indicates if it must be played until it is stopped manually or until end.
     */
    void play(fixed volume, bool loop) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(music_item a, music_item b) = default;

private:
    int _id;
};


/**
 * @brief Hash support for music_item.
 *
 * @ingroup music
 */
template<>
struct hash<music_item>
{
    /**
     * @brief Returns the hash of the given music_item.
     */
    [[nodiscard]] constexpr unsigned operator()(music_item value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

