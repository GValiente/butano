/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SOUND_ITEM_H
#define BN_SOUND_ITEM_H

/**
 * @file
 * bn::sound_item header file.
 *
 * @ingroup sound
 * @ingroup tool
 */

#include "bn_sound_handle.h"

namespace bn
{

/**
 * @brief Contains the required information to play sound effects.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.wav file.
 *
 * @ingroup sound
 * @ingroup tool
 */
class sound_item
{

public:
    /**
     * @brief Constructor.
     * @param id Internal ID.
     */
    constexpr explicit sound_item(int id) :
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
     * @brief Plays the sound effect specified by this item with default settings.
     *
     * Default settings are volume = 1, speed = 1 and panning = 0.
     * @return Sound effect handle.
     */
    sound_handle play() const;

    /**
     * @brief Plays the sound effect specified by this item.
     * @param volume Volume level, in the range [0..1].
     * @return Sound effect handle.
     */
    sound_handle play(fixed volume) const;

    /**
     * @brief Plays the sound effect specified by this item.
     * @param volume Volume level, in the range [0..1].
     * @param speed Playback rate, in the range [0..64].
     * @param panning Panning level, in the range [-1..1].
     * @return Sound effect handle.
     */
    sound_handle play(fixed volume, fixed speed, fixed panning) const;

    /**
     * @brief Plays the sound effect specified by this item with default settings and the given priority.
     *
     * If there's playing too many sound effects at the same time,
     * sound effects with higher priority are discarded later.
     *
     * Default settings are volume = 1, speed = 1 and panning = 0.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @return Sound effect handle.
     */
    sound_handle play_with_priority(int priority) const;

    /**
     * @brief Plays the sound effect specified by this item with the given priority.
     *
     * If there's playing too many sound effects at the same time,
     * sound effects with higher priority are discarded later.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param volume Volume level, in the range [0..1].
     * @return Sound effect handle.
     */
    sound_handle play_with_priority(int priority, fixed volume) const;

    /**
     * @brief Plays the sound effect specified by this item with the given priority.
     *
     * If there's playing too many sound effects at the same time,
     * sound effects with higher priority are discarded later.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param volume Volume level, in the range [0..1].
     * @param speed Playback rate, in the range [0..64].
     * @param panning Panning level, in the range [-1..1].
     * @return Sound effect handle.
     */
    sound_handle play_with_priority(int priority, fixed volume, fixed speed, fixed panning) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(sound_item a, sound_item b) = default;

private:
    int _id;
};


/**
 * @brief Hash support for sound_item.
 *
 * @ingroup sound
 * @ingroup functional
 */
template<>
struct hash<sound_item>
{
    /**
     * @brief Returns the hash of the given sound_item.
     */
    [[nodiscard]] constexpr unsigned operator()(sound_item value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

